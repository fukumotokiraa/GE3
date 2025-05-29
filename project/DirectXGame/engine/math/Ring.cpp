#include "Ring.h"
#include <cmath>

void Ring::CreateRing()
{
	for (uint32_t index = 0; index < kRingDivide; index++) {
		float sin = std::sin(index * radianPerDivide);
		float cos = std::cos(index * radianPerDivide);
		float sinNext = std::sin((index + 1) * radianPerDivide);
		float cosNext = std::cos((index + 1) * radianPerDivide);
		float u = float(index) / float(kRingDivide);
		float uNext = float(index + 1) / float(kRingDivide);
		float angle = index * radianPerDivide;
		float x = std::sin(angle) * ((kOuterRadius + kInnerRadius) * 0.5f);
		float y = std::cos(angle) * ((kOuterRadius + kInnerRadius) * 0.5f);
		Position = { x, y };
		Rotation = angle;
		Scale = { (kOuterRadius - kInnerRadius) * 0.5f, (kOuterRadius - kInnerRadius) * 0.5f };
	}
}
