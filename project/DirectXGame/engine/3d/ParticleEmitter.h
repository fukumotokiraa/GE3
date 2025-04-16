#pragma once
#include "ParticleManager.h"

class ParticleEmitter
{
public:
	ParticleEmitter(std::unordered_map<std::string, ParticleGroup>& particleGroups, float emissionInterval);

	void Update();

	void Emit(const std::string& name, const Vector3& translate, uint32_t count);

private:
	std::mt19937 randomEngine_;

	std::unordered_map<std::string, ParticleGroup>& particleGroups_;

	float emissionInterval_;

	float currentTime_;

};

