#pragma once
#include "IComponent.h"

class SpawnLoop : public Gameplay::IComponent
{
public:
	typedef std::shared_ptr<SpawnLoop> Sptr;

	enum Difficulty {
		Easy,
		Medium,
		Hard
	};

	Difficulty GetDifficulty() {
		return _Difficulty;
	}
	SpawnLoop();
	virtual ~SpawnLoop() = default;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	int ToSpawn();
	float GetSpawnTimer();
	void SpawnBlock();

	virtual void RenderImGui() override;
	MAKE_TYPENAME(SpawnLoop);

	virtual nlohmann::json ToJson() const override;
	static SpawnLoop::Sptr FromJson(const nlohmann::json& blob);
private:
	Difficulty _Difficulty;
	int _BlockToSpawn;
	float _SpawnTimer = 20;
	bool _isDirty=false;
};

