#pragma once

enum class Faction {
	Player,
	Enemy
};

struct Status {
	Faction faction;//陣営
	int hp;//現在体力
	int maxHp;//最大体力
	int move; // 移動距離
	int range;     // 攻撃射程
	int power;    // 攻撃力
	float attackSpeed = 1.0f;//攻撃速度

	bool IsAlive() const { return hp > 0; }
};

struct StagePos {
	int x;
	int y;
};
