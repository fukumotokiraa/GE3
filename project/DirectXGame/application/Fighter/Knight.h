#pragma once
#include "BaseFighter.h"

class Knight : public BaseFighter
{
public:
	~Knight() override = default;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

private:


};

