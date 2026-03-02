#include "Mage.h"
#include "ModelManager.h"
#include <cmath>

void Mage::Initialize()
{
	// 陣営に応じて使用モデルを切り替える
	const Status* st = GetStatus();
	const bool isEnemy = (st && st->faction == Faction::Enemy);

	const std::string modelFile = isEnemy ? "mageEnemy.gltf" : "mage.gltf";

	// モデル読み込みと Object3d をセットアップ
	ModelManager::GetInstance()->LoadModel(modelFile);
	mageModel = new Model();
	mageObject = new Object3d();
	mageObject->Initialize(object3dCommon_, mageModel);
	mageObject->SetModel(modelFile);
	mageObject->GetTransform().translate = { 3.0f,1.0f,-4.0f };

	// 注意: SpawnFighter で既に faction をセットしている想定なので、
	//       ここでは faction を上書きしないように他フィールドのみ代入する
	mageStatus.hp = 80;
	mageStatus.maxHp = 80;
	mageStatus.move = 1; // moveRange
	mageStatus.range = 4; // range
	mageStatus.power = 10; // attack
	mageStatus.attackSpeed = 0.6f; // attackSpeed

	// 弾関連初期化
	bulletModel = nullptr;
	bulletObject = nullptr;
	bulletActive = false;
	bulletTarget = nullptr;
	bulletElapsed = 0.0f;
	bulletDuration = 0.25f;
}

void Mage::Finalize()
{
	if (bulletObject) {
		delete bulletObject;
		bulletObject = nullptr;
	}
	if (bulletModel) {
		delete bulletModel;
		bulletModel = nullptr;
	}

	delete mageObject;
	delete mageModel;
}

void Mage::Update()
{
	// Mage 本体更新
	if (mageObject) mageObject->Update();

	// 弾の更新
	if (bulletActive && bulletObject) {
		// フレーム固定（既存コード基準で 1/60）
		const float dt = 1.0f / 60.0f;
		bulletElapsed += dt;
		float t = (bulletDuration > 0.0f) ? (bulletElapsed / bulletDuration) : 1.0f;
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;

		// 線形補間で移動
		Vector3 pos = {
			bulletStart.x + (bulletEnd.x - bulletStart.x) * t,
			bulletStart.y + (bulletEnd.y - bulletStart.y) * t,
			bulletStart.z + (bulletEnd.z - bulletStart.z) * t
		};
		bulletObject->GetTransform().translate = pos;
		bulletObject->Update();

		// 着弾処理
		if (t >= 1.0f) {
			// 対象がまだ存在し、生存していればダメージ適用
			if (bulletTarget) {
				Status* ts = bulletTarget->GetStatus();
				Status* my = GetStatus();
				if (ts && my && ts->IsAlive()) {
					ts->hp = max(0, ts->hp - my->power);
					bulletTarget->OnHit();
				}
			}

			// 弾オブジェクト破棄
			delete bulletObject;
			bulletObject = nullptr;
			delete bulletModel;
			bulletModel = nullptr;
			bulletActive = false;
			bulletTarget = nullptr;
			bulletElapsed = 0.0f;
		}
	}
}

void Mage::Draw()
{
	// Mage 本体描画
	if (mageObject) mageObject->Draw();

	// 弾描画
	if (bulletActive && bulletObject) bulletObject->Draw();
}

void Mage::OnHit()
{
	// 点滅の長さと間隔はここで調整
	if (mageObject) {
		mageObject->StartBlink(0.6f, 0.08f); // 0.6秒、0.08秒間隔で点滅
	}
}

void Mage::Attack(BaseFighter* target)
{
	// 既に弾が飛んでいる場合は二重発射を防ぐ（要件に合わせて調整可）
	if (bulletActive) {
		// クールはリセットしない（既に発射済み）場合は何もしない
		return;
	}

	if (!target) return;

	// 弾モデル読み込みと生成
	const std::string bulletFile = "mageBullet.gltf";
	ModelManager::GetInstance()->LoadModel(bulletFile);
	bulletModel = new Model();
	bulletObject = new Object3d();
	bulletObject->Initialize(object3dCommon_, bulletModel);
	bulletObject->SetModel(bulletFile);

	// 開始位置は自分のオブジェクト位置、終端は対象のオブジェクト位置（存在しない場合はその場で消える）
	Vector3 start = {0.0f,0.0f,0.0f};
	Vector3 end = {0.0f,0.0f,0.0f};
	if (mageObject) start = mageObject->GetTransform().translate;
	if (target && target->GetObject3d()) end = target->GetObject3d()->GetTransform().translate;

	bulletStart = start;
	bulletEnd = end;
	bulletElapsed = 0.0f;

	// 距離に応じて移動時間を決定（速度：6.0f を仮定）
	float dx = end.x - start.x;
	float dy = end.y - start.y;
	float dz = end.z - start.z;
	float dist = std::sqrt(dx*dx + dy*dy + dz*dz);
	const float speed = 6.0f;
	bulletDuration = max(0.05f, dist / speed);

	bulletTarget = target;
	bulletActive = true;

	// クールダウンは発射時にリセット（実際のダメージは着弾時）
	ResetAttackTimer();
}
