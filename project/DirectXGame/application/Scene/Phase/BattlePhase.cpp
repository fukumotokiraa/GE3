#include "PhaseCommon.h"
#include "BattlePhase.h"
#include "Fighter/Knight.h"
#include "Fighter/Mage.h"

#include <queue>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <climits>

namespace {

// ステージ固定パラメータ（Stage と合わせる）
const int kStageWidth = 8;
const int kStageHeight = 8;
const float kBlockSize = 1.0f;
const float kWorldHeight = 1.0f;
const float kOriginX = -4.0f;
const float kOriginY = -4.0f;

// マンハッタン距離
inline int Manhattan(int x1, int y1, int x2, int y2) {
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// グリッド -> ワールド座標
inline Vector3 GridToWorld(int gx, int gy) {
	return { kOriginX + gx * kBlockSize, kWorldHeight, kOriginY + gy * kBlockSize };
}

// BFS で「start から探索し、相手 posTarget とのマンハッタン距離が targetRange に等しい最短セル」への経路を返す。
// occupied が true のセルは通行不可（ただし start は許可）。見つからなければ start のみを返す。
std::vector<StagePos> FindPathToRange(
	StagePos start,
	StagePos posTarget,
	int targetRange,
	const std::array<std::array<bool, kStageWidth>, kStageHeight>& occupied)
{
	std::array<std::array<bool, kStageWidth>, kStageHeight> visited{};
	std::array<std::array<StagePos, kStageWidth>, kStageHeight> parent{};
	std::queue<StagePos> q;

	auto inBounds = [](int x, int y) {
		return x >= 0 && x < kStageWidth && y >= 0 && y < kStageHeight;
	};

	// 開始セルは常に訪問済みとしてキューに入れる（自分は占有セルにいる想定）
	q.push(start);
	visited[start.y][start.x] = true;
	parent[start.y][start.x] = start;

	// 4方向
	const int dx[4] = { 1, -1, 0, 0 };
	const int dy[4] = { 0, 0, 1, -1 };

	while (!q.empty()) {
		auto cur = q.front(); q.pop();

		// 目標条件：cur のマンハッタン距離が targetRange ならこれが目的セル
		if (Manhattan(cur.x, cur.y, posTarget.x, posTarget.y) == targetRange) {
			// パス復元
			std::vector<StagePos> path;
			StagePos p = cur;
			while (!(p.x == parent[p.y][p.x].x && p.y == parent[p.y][p.x].y)) {
				path.push_back(p);
				p = parent[p.y][p.x];
			}
			// 最後に start を追加
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return path;
		}

		for (int i = 0; i < 4; ++i) {
			int nx = cur.x + dx[i];
			int ny = cur.y + dy[i];
			if (!inBounds(nx, ny)) continue;
			if (visited[ny][nx]) continue;
			// 占有セルは通行不可。ただし開始セルは既に処理済みなので別扱い不要。
			if (occupied[ny][nx]) continue;

			visited[ny][nx] = true;
			parent[ny][nx] = cur;
			q.push({ nx, ny });
		}
	}

	// 見つからなければ start のみを返す（移動なし）
	return std::vector<StagePos>{ start };
}

} // namespace

void BattlePhase::Initialize(PhaseCommon* phaseCommon)
{
	phaseCommon_ = phaseCommon;
	movementTimer_ = 0.0f;
}

void BattlePhase::Finalize()
{
	// PhaseCommon 側でメモリ管理しているため特になし
}

void BattlePhase::Update()
{
	// 固定フレーム想定（既存コードに合わせて 1/60 秒刻み）
	const float dt = 1.0f / 60.0f;
	movementTimer_ += dt;

	if (!phaseCommon_) return;

	// プレイヤー側・敵側の全ユニットを取得
	auto players = phaseCommon_->GetAllOfType<BaseFighter>(Team::Player);
	auto enemies = phaseCommon_->GetAllOfType<BaseFighter>(Team::Enemy);

	// --- 攻撃タイマー更新（全ユニット） ---
	for (auto p : players) if (p) p->UpdateAttackTimer(dt);
	for (auto e : enemies) if (e) e->UpdateAttackTimer(dt);

	// ヘルパ: ターゲットが有効か検証（生存かつ射程内）。無効ならクリアして false を返す。
	auto validateTarget = [&](BaseFighter* self, BaseFighter*& target) -> bool {
		if (!target) return false;
		Status* ts = target->GetStatus();
		if (!ts || !ts->IsAlive()) {
			self->ClearCurrentTarget();
			target = nullptr;
			return false;
		}
		Status* my = self->GetStatus();
		if (!my) { self->ClearCurrentTarget(); target = nullptr; return false; }
		StagePos sPos = self->GetGridPos();
		StagePos tPos = target->GetGridPos();
		if (Manhattan(sPos.x, sPos.y, tPos.x, tPos.y) > my->range) {
			// 射程外になったらロック解除
			self->ClearCurrentTarget();
			target = nullptr;
			return false;
		}
		return true;
	};

	// --- 攻撃処理: 各プレイヤーが現在のロックターゲット（無ければ最寄り）を攻撃 ---
	for (auto p : players) {
		if (!p) continue;
		BaseFighter* target = p->GetCurrentTarget();
		// 無効ならクリア済みになる
		if (!validateTarget(p, target)) {
			// 最寄りの敵を新たにロック（距離は問わない。攻撃は射程チェックで行う）
			int bestDist = INT_MAX;
			BaseFighter* best = nullptr;
			StagePos pPos = p->GetGridPos();
			for (auto e : enemies) {
				if (!e) continue;
				StagePos ePos = e->GetGridPos();
				int d = Manhattan(pPos.x, pPos.y, ePos.x, ePos.y);
				if (d < bestDist) { bestDist = d; best = e; }
			}
			if (best) { p->SetCurrentTarget(best); target = best; }
		}
		// 攻撃判定（ターゲットが射程内かつクールが空いていれば攻撃）
		if (target && p->GetStatus()) {
			StagePos pPos = p->GetGridPos();
			StagePos tPos = target->GetGridPos();
			int dist = Manhattan(pPos.x, pPos.y, tPos.x, tPos.y);
			if (dist <= p->GetStatus()->range && p->CanAttack()) {
				p->Attack(target);
			}
		}
	}

	// --- 敵側も同様にターゲットロックで攻撃 ---
	for (auto e : enemies) {
		if (!e) continue;
		BaseFighter* target = e->GetCurrentTarget();
		if (!validateTarget(e, target)) {
			int bestDist = INT_MAX;
			BaseFighter* best = nullptr;
			StagePos ePos = e->GetGridPos();
			for (auto p : players) {
				if (!p) continue;
				StagePos pPos = p->GetGridPos();
				int d = Manhattan(ePos.x, ePos.y, pPos.x, pPos.y);
				if (d < bestDist) { bestDist = d; best = p; }
			}
			if (best) { e->SetCurrentTarget(best); target = best; }
		}
		if (target && e->GetStatus()) {
			StagePos ePos = e->GetGridPos();
			StagePos tPos = target->GetGridPos();
			int dist = Manhattan(ePos.x, ePos.y, tPos.x, tPos.y);
			if (dist <= e->GetStatus()->range && e->CanAttack()) {
				e->Attack(target);
			}
		}
	}

	// 移動処理は moveInterval ごと
	if (movementTimer_ >= moveInterval_) {
		// 占有マップを作成（true = そのセルは既に誰かがいる／予約済み）
		std::array<std::array<bool, kStageWidth>, kStageHeight> occupied{};
		for (auto &row : occupied) row.fill(false);

		// 現在位置で埋める（プレイヤー＋敵）
		for (auto p : players) {
			if (!p) continue;
			StagePos pos = p->GetGridPos();
			if (pos.x >= 0 && pos.x < kStageWidth && pos.y >= 0 && pos.y < kStageHeight)
				occupied[pos.y][pos.x] = true;
		}
		for (auto e : enemies) {
			if (!e) continue;
			StagePos pos = e->GetGridPos();
			if (pos.x >= 0 && pos.x < kStageWidth && pos.y >= 0 && pos.y < kStageHeight)
				occupied[pos.y][pos.x] = true;
		}

		auto findAlternative = [&](const StagePos &cur, const StagePos &target)->StagePos {
			// 近傍の未占有セルをターゲットへの距離で選択
			const int dx[4] = { 1, -1, 0, 0 };
			const int dy[4] = { 0, 0, 1, -1 };
			int bestDist = INT_MAX;
			StagePos best = cur;
			for (int i = 0; i < 4; ++i) {
				int nx = cur.x + dx[i];
				int ny = cur.y + dy[i];
				if (nx < 0 || nx >= kStageWidth || ny < 0 || ny >= kStageHeight) continue;
				if (occupied[ny][nx]) continue;
				int d = Manhattan(nx, ny, target.x, target.y);
				if (d < bestDist) {
					bestDist = d;
					best = { nx, ny };
				}
			}
			return best;
		};

		// プレイヤー各ユニットを順に移動（移動可能なら起点を空け、移動先を予約）
		if (!players.empty() && !enemies.empty()) {
			for (auto p : players) {
				if (!p) continue;
				StagePos pPos = p->GetGridPos();

				// 現在のロックターゲットを確認／無ければ最寄りに設定
				BaseFighter* target = p->GetCurrentTarget();
				if (!validateTarget(p, target)) {
					int bestDist = INT_MAX;
					BaseFighter* best = nullptr;
					for (auto e : enemies) {
						if (!e) continue;
						StagePos ePos = e->GetGridPos();
						int d = Manhattan(pPos.x, pPos.y, ePos.x, ePos.y);
						if (d < bestDist) { bestDist = d; best = e; }
					}
					if (best) { p->SetCurrentTarget(best); target = best; }
				}
				if (!target) continue;

				StagePos tPos = target->GetGridPos();
				int range = p->GetStatus() ? p->GetStatus()->range : 0;
				if (Manhattan(pPos.x, pPos.y, tPos.x, tPos.y) > range) {
					// 占有マップを考慮した BFS で経路探索（長距離迂回対応）
					auto path = FindPathToRange(pPos, tPos, range, occupied);
					if (path.size() > 1) {
						StagePos next = path[1];
						// 次が占有されている場合は代替近傍セルを探す（保険）
						StagePos dest = next;
						if (!(next.x >= 0 && next.x < kStageWidth && next.y >= 0 && next.y < kStageHeight) ||
							occupied[next.y][next.x]) {
							dest = findAlternative(pPos, tPos);
						}
						// dest が現位置でなければ移動（未占有が保証されている）
						if (!(dest.x == pPos.x && dest.y == pPos.y)) {
							// 起点を空ける
							if (pPos.x >= 0 && pPos.x < kStageWidth && pPos.y >= 0 && pPos.y < kStageHeight)
								occupied[pPos.y][pPos.x] = false;
							// 目的地を予約
							if (dest.x >= 0 && dest.x < kStageWidth && dest.y >= 0 && dest.y < kStageHeight)
								occupied[dest.y][dest.x] = true;

							// 実際の移動
							p->SetGridPos(dest.x, dest.y);
							if (p->GetObject3d()) {
								auto w = GridToWorld(dest.x, dest.y);
								p->GetObject3d()->GetTransform().translate = w;
							}
						}
					}
				}
			}

			// 敵各ユニット（プレイヤー移動後の占有状態を反映）
			for (auto e : enemies) {
				if (!e) continue;
				StagePos ePos = e->GetGridPos();

				BaseFighter* target = e->GetCurrentTarget();
				if (!validateTarget(e, target)) {
					int bestDist = INT_MAX;
					BaseFighter* best = nullptr;
					for (auto p : players) {
						if (!p) continue;
						StagePos pPos = p->GetGridPos();
						int d = Manhattan(ePos.x, ePos.y, pPos.x, pPos.y);
						if (d < bestDist) { bestDist = d; best = p; }
					}
					if (best) { e->SetCurrentTarget(best); target = best; }
				}
				if (!target) continue;

				StagePos tPos = target->GetGridPos();
				int range = e->GetStatus() ? e->GetStatus()->range : 0;
				if (Manhattan(ePos.x, ePos.y, tPos.x, tPos.y) > range) {
					auto path = FindPathToRange(ePos, tPos, range, occupied);
					if (path.size() > 1) {
						StagePos next = path[1];
						StagePos dest = next;
						if (!(next.x >= 0 && next.x < kStageWidth && next.y >= 0 && next.y < kStageHeight) ||
							occupied[next.y][next.x]) {
							dest = findAlternative(ePos, tPos);
						}
						if (!(dest.x == ePos.x && dest.y == ePos.y)) {
							// 起点を空ける
							if (ePos.x >= 0 && ePos.x < kStageWidth && ePos.y >= 0 && ePos.y < kStageHeight)
								occupied[ePos.y][ePos.x] = false;
							// 目的地を予約
							if (dest.x >= 0 && dest.x < kStageWidth && dest.y >= 0 && dest.y < kStageHeight)
								occupied[dest.y][dest.x] = true;

							// 実際の移動
							e->SetGridPos(dest.x, dest.y);
							if (e->GetObject3d()) {
								auto w = GridToWorld(dest.x, dest.y);
								e->GetObject3d()->GetTransform().translate = w;
							}
						}
					}
				}
			}
		}
		movementTimer_ -= moveInterval_;
	}
}

void BattlePhase::Draw()
{
	// 描画は PhaseCommon 側の Draw が呼ばれるため特に無し
}