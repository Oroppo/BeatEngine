#pragma once
#include "Application/ApplicationLayer.h"
#include "json.hpp"
#include "../../Gameplay/Scene.h"
#include "../../Gameplay/MeshResource.h"
#include "../../Gameplay/Material.h"
#include "../../Gameplay/GameObject.h"
#include "SpawnFunctions.h"

/**
 * This example layer handles creating a default test scene, which we will use 
 * as an entry point for creating a sample scene
 */
//using namespace Gameplay;
//using namespace Gameplay::Physics;
class DefaultSceneLayer final : public ApplicationLayer {
public:
	MAKE_PTRS(DefaultSceneLayer)

	DefaultSceneLayer();
	virtual ~DefaultSceneLayer();

	//enum states {
	//	PlayState = 0,
	//	PauseState = 1,
	//	LoseState = 2,
	//};


	// Inherited from ApplicationLayer

	virtual void OnAppLoad(const nlohmann::json& config) override;


protected:
	void _CreateScene();
};