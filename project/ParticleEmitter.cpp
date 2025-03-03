#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(std::mt19937 randomEngine, std::unordered_map<std::string, ParticleGroup>& particleGroups, float emissionInterval)
	: randomEngine_(randomEngine),
	particleGroups_(particleGroups),
	emissionInterval_(emissionInterval),
	currentTime_(0.0f) {
	
}

void ParticleEmitter::Update()
{
	currentTime_ += 1.0f / 60.0f;
	if (currentTime_ >= emissionInterval_) {
		for (auto& group : particleGroups_) {
			ParticleGroup& particleGroup = group.second;
			Emit(group.first, particleGroup.particles.front().transform.translate, 10);
		}
		currentTime_ = 0.0f;
	}
}

void ParticleEmitter::Emit(const std::string& name, const Vector3& translate, uint32_t count)
{
	ParticleManager::GetInstance()->Emit(name, translate, count);
}
