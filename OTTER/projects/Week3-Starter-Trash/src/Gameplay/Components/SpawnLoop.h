#pragma once
#include "IComponent.h"
#include "Gameplay/Material.h"
#include "Gameplay/Scene.h"
#include "Gameplay/MeshResource.h"
#include "Application/Layers/SpawnFunctions.h"
#include "Resource Definitions.h"
#include "Utils/FileHelpers.h"

//Meant to be a singleton class please ONLY add this to the GameManager GameObject
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
	void CreateList(const nlohmann::json&);

	virtual void RenderImGui() override;
	MAKE_TYPENAME(SpawnLoop);

	virtual nlohmann::json ToJson() const override;
	static SpawnLoop::Sptr FromJson(const nlohmann::json& blob);
private:
	Difficulty _Difficulty;
	
	std::map<std::string, Guid> MaterialList;
	std::map<std::string, Guid> MeshList;
	int _BlockToSpawn;
	float _SpawnTimer = 20;
	bool _isDirty=false;

	//This is Disgusting.
	MeshResource::Sptr monkeyMesh;
	MeshResource::Sptr SmallPlatform;
	MeshResource::Sptr WallJump;
	MeshResource::Sptr BeatGem;
	MeshResource::Sptr Vinyl;
	MeshResource::Sptr CD;
	MeshResource::Sptr Building;
	MeshResource::Sptr KBuilding1Mesh;
	MeshResource::Sptr KBuilding2Mesh;
	MeshResource::Sptr KBuilding3Mesh;
	MeshResource::Sptr OvalBuilding;
	MeshResource::Sptr CharacterMesh;
	MeshResource::Sptr DiscoBallMesh;
	MeshResource::Sptr StartPlatform;
	MeshResource::Sptr Car1Mesh;
	MeshResource::Sptr SemiTruckMesh;
	MeshResource::Sptr PickupTruckMesh;
	MeshResource::Sptr SmallWallJump;
	MeshResource::Sptr SuperSmallWallJump;
	MeshResource::Sptr FallingPlat;
	MeshResource::Sptr HalfCirclePlat;
	MeshResource::Sptr StairsRight;
	MeshResource::Sptr StairsLeft;
	MeshResource::Sptr Speaker;
	MeshResource::Sptr SquarePlat;
	MeshResource::Sptr FloatingLight;
	MeshResource::Sptr DiscoBotMesh1;
	MeshResource::Sptr DiscoBotMesh2;
	MeshResource::Sptr DiscoBotMesh3;
	MeshResource::Sptr DiscoBotMesh4;
	MeshResource::Sptr DiscoBotMesh5;
	MeshResource::Sptr DiscoBotMesh6;
	MeshResource::Sptr DiscoBotMesh7;
	MeshResource::Sptr DiscoBotMesh8;
	MeshResource::Sptr DiscoBotMesh9;
	MeshResource::Sptr BotJump1;
	MeshResource::Sptr BotJump2;
	MeshResource::Sptr BotJump3;
	MeshResource::Sptr BotJump4;
	MeshResource::Sptr BotJump5;
	MeshResource::Sptr BotJump6;
	MeshResource::Sptr BotJump7;

	Material::Sptr StartPlatformMaterial;
	Material::Sptr UIMat;
	Material::Sptr SmallPlatformMaterial;
	Material::Sptr WallJumpMaterial;
	Material::Sptr BeatGemMaterial;
	Material::Sptr BeatGemOffMaterial;
	Material::Sptr VinylMaterial;
	Material::Sptr CDMaterial;
	Material::Sptr CharacterMaterial;
	Material::Sptr DiscoBallMaterial;
	Material::Sptr LoseScreenMaterial;
	Material::Sptr Car1Material;
	Material::Sptr SemiTruckMaterial;
	Material::Sptr PickupTruckMaterial;
	Material::Sptr BuildingMaterial;
	Material::Sptr KBuildingMaterial;
	Material::Sptr KBuilding2Material;
	Material::Sptr KBuilding3Material;
	Material::Sptr SmallWallJumpMaterial;
	Material::Sptr SuperSmallWallJumpMaterial;
	Material::Sptr PianoMaterial;
	Material::Sptr HalfCirclePlatMaterial;
	Material::Sptr StairsRightMaterial;
	Material::Sptr StairsLeftMaterial;
	Material::Sptr SpeakerMaterial;
	Material::Sptr SquarePlatMaterial;
	Material::Sptr FloatingLightMaterial;
	Material::Sptr OvalBuildingMaterial;
};

