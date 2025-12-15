#include "FighterFactory.h"
#include "Knight.h"
#include "Mage.h"
#include <stdexcept>

std::unordered_map<FighterType, FighterFactory::Creator>& FighterFactory::Registry() {
	static std::unordered_map<FighterType, Creator> registry;
	return registry;
}

void FighterFactory::Register(FighterType type, Creator creator) {
	Registry()[type] = std::move(creator);
}

// 初期登録（どこかの初期化コードで実行）
static bool RegisterBuiltins() {
	FighterFactory::Register(FighterType::Knight, [](){ return std::make_unique<Knight>(); });
	FighterFactory::Register(FighterType::Mage,   [](){ return std::make_unique<Mage>(); });
	return true;
}
static bool registered = RegisterBuiltins();

std::unique_ptr<BaseFighter> FighterFactory::Create(FighterType type) {
	auto it = Registry().find(type);
	if (it == Registry().end()) {
		throw std::runtime_error("Unknown FighterType");
	}
	return it->second();
}
