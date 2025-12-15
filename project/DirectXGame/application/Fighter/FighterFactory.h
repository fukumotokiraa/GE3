#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include "BaseFighter.h"

enum class FighterType { Knight, Mage /* ... */ };

class FighterFactory {
public:
	using Creator = std::function<std::unique_ptr<BaseFighter>()>;

	static void Register(FighterType type, Creator creator);
	static std::unique_ptr<BaseFighter> Create(FighterType type);

private:
	static std::unordered_map<FighterType, Creator> & Registry();
};

