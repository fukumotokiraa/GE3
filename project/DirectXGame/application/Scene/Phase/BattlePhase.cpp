#include "PhaseCommon.h"
#include "BattlePhase.h"
#include "Fighter/Knight.h"
#include "Fighter/Mage.h"

#include <queue>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>

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
// 経路は start から goal への順で StagePos を格納。到達済みならサイズ 1（start のみ）。
std::vector<StagePos> FindPathToRange(StagePos start, StagePos posTarget, int targetRange)
{
	std::array<std::array<bool, kStageWidth>, kStageHeight> visited{};
	std::array<std::array<StagePos, kStageWidth>, kStageHeight> parent{};
	std::queue<StagePos> q;

	auto inBounds = [](int x, int y) {
		return x >= 0 && x < kStageWidth && y >= 0 && y < kStageHeight;
	};

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

	// --- 攻撃処理: 各プレイヤーが最寄りの敵を攻撃 ---
	for (auto p : players) {
		if (!p) continue;
		if (enemies.empty()) break;
		// 最寄りの敵を選択
		StagePos pPos = p->GetGridPos();
		int bestDist = INT_MAX;
		BaseFighter* target = nullptr;
		for (auto e : enemies) {
			if (!e) continue;
			StagePos ePos = e->GetGridPos();
			int d = Manhattan(pPos.x, pPos.y, ePos.x, ePos.y);
			if (d < bestDist) { bestDist = d; target = e; }
		}
		if (target) {
			if (bestDist <= p->GetStatus()->range && p->CanAttack()) {
				p->Attack(target);
			}
		}
	}

	// --- 敵側も同様に攻撃（各敵が最寄りのプレイヤーを攻撃） ---
	for (auto e : enemies) {
		if (!e) continue;
		if (players.empty()) break;
		StagePos ePos = e->GetGridPos();
		int bestDist = INT_MAX;
		BaseFighter* target = nullptr;
		for (auto p : players) {
			if (!p) continue;
			StagePos pPos = p->GetGridPos();
			int d = Manhattan(ePos.x, ePos.y, pPos.x, pPos.y);
			if (d < bestDist) { bestDist = d; target = p; }
		}
		if (target) {
			if (bestDist <= e->GetStatus()->range && e->CanAttack()) {
				e->Attack(target);
			}
		}
	}

	// 移動処理は moveInterval ごと
	if (movementTimer_ >= moveInterval_) {
		// プレイヤー側を先に移動（複数ユニットにも対応）
		if (!players.empty() && !enemies.empty()) {
			// プレイヤー各ユニット
			for (auto p : players) {
				if (!p) continue;
				// 最寄りの敵を選択
				StagePos pPos = p->GetGridPos();
				int bestDist = INT_MAX;
				BaseFighter* target = nullptr;
				for (auto e : enemies) {
					if (!e) continue;
					StagePos ePos = e->GetGridPos();
					int d = Manhattan(pPos.x, pPos.y, ePos.x, ePos.y);
					if (d < bestDist) { bestDist = d; target = e; }
				}
				if (!target) continue;
				StagePos tPos = target->GetGridPos();
				int range = p->GetStatus()->range;
				if (Manhattan(pPos.x, pPos.y, tPos.x, tPos.y) > range) {
					auto path = FindPathToRange(pPos, tPos, range);
					if (path.size() > 1) {
						StagePos next = path[1];
						p->SetGridPos(next.x, next.y);
						if (p->GetObject3d()) {
							auto w = GridToWorld(next.x, next.y);
							p->GetObject3d()->GetTransform().translate = w;
						}
					}
				}
			}

			// 敵各ユニット（プレイヤー移動後の位置を考慮）
			// 更新されたプレイヤー位置を反映するため、players を再取得しても良いが簡易的に現状の players を使う
			for (auto e : enemies) {
				if (!e) continue;
				StagePos ePos = e->GetGridPos();
				int bestDist = INT_MAX;
				BaseFighter* target = nullptr;
				for (auto p : players) {
					if (!p) continue;
					StagePos pPos = p->GetGridPos();
					int d = Manhattan(ePos.x, ePos.y, pPos.x, pPos.y);
					if (d < bestDist) { bestDist = d; target = p; }
				}
				if (!target) continue;
				StagePos tPos = target->GetGridPos();
				int range = e->GetStatus()->range;
				if (Manhattan(ePos.x, ePos.y, tPos.x, tPos.y) > range) {
					auto path = FindPathToRange(ePos, tPos, range);
					if (path.size() > 1) {
						StagePos next = path[1];
						e->SetGridPos(next.x, next.y);
						if (e->GetObject3d()) {
							auto w = GridToWorld(next.x, next.y);
							e->GetObject3d()->GetTransform().translate = w;
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