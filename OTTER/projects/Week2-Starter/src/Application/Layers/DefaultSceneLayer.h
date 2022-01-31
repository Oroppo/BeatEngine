#pragma once
#include "Application/ApplicationLayer.h"
#include "json.hpp"
#include "../../Gameplay/Scene.h"
#include "../../Gameplay/MeshResource.h"
#include "../../Gameplay/Material.h"
#include "../../Gameplay/GameObject.h"

/**
 * This example layer handles creating a default test scene, which we will use 
 * as an entry point for creating a sample scene
 */
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

	void SpawnStartPlat(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material,
		std::string ObjName, glm::vec3 pos,glm::vec3 rot, glm::vec3 scale);

	void SpawnObj(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName, glm::vec3 pos,
		glm::vec3 rot, glm::vec3 scale, Gameplay::GameObject::Sptr parent = nullptr);

	// Inherited from ApplicationLayer

	virtual void OnAppLoad(const nlohmann::json& config) override;

protected:
	void _CreateScene();
};