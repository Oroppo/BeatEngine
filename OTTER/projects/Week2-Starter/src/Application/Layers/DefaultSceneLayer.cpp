#include "DefaultSceneLayer.h"

// GLM math library
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/common.hpp> // for fmod (floating modulus)

#include <filesystem>

// Graphics
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Texture2D.h"
#include "Graphics/TextureCube.h"
#include "Graphics/VertexTypes.h"
#include "Graphics/Font.h"
#include "Graphics/GuiBatcher.h"
#include "Graphics/Framebuffer.h"

// Utilities
#include "Utils/MeshBuilder.h"
#include "Utils/MeshFactory.h"
#include "Utils/ObjLoader.h"
#include "Utils/ImGuiHelper.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/FileHelpers.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/StringUtils.h"
#include "Utils/GlmDefines.h"

// Gameplay
#include "Gameplay/Material.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"

// Components
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/Components/Camera.h"
#include "Gameplay/Components/RotatingBehaviour.h"
#include "Gameplay/Components/CharacterController.h"
#include "Gameplay/Components/JumpBehaviour.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/MaterialSwapBehaviour.h"
#include "Gameplay/Components/MoveThings.h"
#include "Gameplay/Components/ScoreComponent.h"
#include "Gameplay/Components/LevelMover.h"
#include "Gameplay/Components/BackgroundMover.h"
#include "Gameplay/Components/VinylAnim.h"
#include "Gameplay/Components/ForeGroundMover.h"
#include "Gameplay/Components/SeekBehaviour.h"
#include "Gameplay/Components/BeatTimer.h"
#include "Gameplay/Components/BuildingAnim.h"
#include "Gameplay/Components/MaterialSwap.h"
#include "Gameplay/Components/SimpleCameraControl.h"
#include "Gameplay/Components/TriggerVolumeEnterBehaviour.h"


// Physics
#include "Gameplay/Physics/RigidBody.h"
#include "Gameplay/Physics/Colliders/BoxCollider.h"
#include "Gameplay/Physics/Colliders/PlaneCollider.h"
#include "Gameplay/Physics/Colliders/SphereCollider.h"
#include "Gameplay/Physics/Colliders/ConvexMeshCollider.h"
#include "Gameplay/Physics/Colliders/CylinderCollider.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Graphics/DebugDraw.h"

// GUI
#include "Gameplay/Components/GUI/RectTransform.h"
#include "Gameplay/Components/GUI/GuiPanel.h"
#include "Gameplay/Components/GUI/GuiText.h"
#include "Gameplay/InputEngine.h"

//Sound
//#include "Sound/AudioEngine.h"
#include "Fmod.h"
//#include "FMOD/ToneFire.h"

#include "Application/Application.h"
#include "Gameplay/Components/ParticleSystem.h"



DefaultSceneLayer::DefaultSceneLayer() :
	ApplicationLayer()
{
	Name = "Default Scene";
	Overrides = AppLayerFunctions::OnAppLoad;
}

DefaultSceneLayer::~DefaultSceneLayer() = default;

// Object Spawning Functions lines 99-623 CTRL + M, CTRL + O to collapse functions

// For spawning general objects
void DefaultSceneLayer::SpawnObj(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f), 
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {

	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Startplatform = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Startplatform->SetPostion(pos);
		Startplatform->SetRotation(rot);
		Startplatform->SetScale(scale);

		Startplatform->Add<LevelMover>();

		// Create and attach a renderer for the monkey
		RenderComponent::Sptr renderer = Startplatform->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this monkey
		RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		//physics->AddCollider(BoxCollider::Create(glm::vec3(1.0f, 1.0f, 1.0f)));


		// FIX THIS //
		ICollider::Sptr Box1 = physics->AddCollider(BoxCollider::Create(glm::vec3(0.87f, 0.5f, 0.4f)));
		Box1->SetPosition(glm::vec3(0.f, 0.f, 0.f));
		Box1->SetScale(glm::vec3(1, 1, 1));

		//if (parent != nullptr) {
		//	parent->AddChild(Startplatform);
		//}
	}
}
// For spawning start/end platforms
void DefaultSceneLayer::SpawnStartPlat(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Startplatform = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Startplatform->SetPostion(pos);
		Startplatform->SetRotation(rot);
		Startplatform->SetScale(scale);

		Startplatform->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Startplatform->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		physics->AddCollider(BoxCollider::Create(glm::vec3(1.8f, 5.2f, 1.0f)));
	}
}
// For spawning beat gems
void DefaultSceneLayer::SpawnGem(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr MaterialOn, Gameplay::Material::Sptr MaterialOff, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Gem = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Gem->SetPostion(pos);
		Gem->SetRotation(rot);
		Gem->SetScale(scale);

		//Add Components
		Gem->Add<LevelMover>();
		Gem->Add<RotatingBehaviour>();
		Gem->Add<MaterialSwap>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Gem->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(MaterialOff);

		TriggerVolume::Sptr volume = Gem->Add<TriggerVolume>();
		volume->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));
		//volume->SetFlags(TriggerTypeFlags::Statics | TriggerTypeFlags::Kinematics);

		// Add a dynamic rigid body to this object
		//RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		//physics->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));
		// For Gem Colliders X = left/right Y = Up/Down Z = Towards/Away
	}
}
// For spawning Vinyls
void DefaultSceneLayer::SpawnCollectable(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Collectable = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Collectable->SetPostion(pos);
		Collectable->SetRotation(rot);
		Collectable->SetScale(scale);

		//Add Components
		Collectable->Add<LevelMover>();
		Collectable->Add<VinylAnim>();
		Collectable->Add<RotatingBehaviour>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Collectable->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		//RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Colliders X is towards Cam, Y is up/down , Z is Left and Right
		//ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));

		TriggerVolume::Sptr volume = Collectable->Add<TriggerVolume>();
		volume->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));
		//volume->SetPostion(glm::vec3(0.0f, 0.5f, 0.0f));
	}
}
// For spawning CD Collectables
void DefaultSceneLayer::SpawnCD(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr CD = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		CD->SetPostion(pos);
		CD->SetRotation(rot);
		CD->SetScale(scale);

		//Add Components
		CD->Add<LevelMover>();
		CD->Add<RotatingBehaviourCD>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = CD->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		//RigidBody::Sptr physics = Startplatform->Add<RigidBody>(RigidBodyType::Kinematic);
		//physics->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));

		TriggerVolume::Sptr volume = CD->Add<TriggerVolume>();
		volume->AddCollider(BoxCollider::Create(glm::vec3(0.5f, 0.5f, 0.5f)));
	}
}
// For spawning Wall Jump Platforms
void DefaultSceneLayer::SpawnWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr WallJump = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		WallJump->SetPostion(pos);
		WallJump->SetRotation(rot);
		WallJump->SetScale(scale);

		WallJump->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = WallJump->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = WallJump->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.3f, 0.5f, 3.2f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning building1 object
void DefaultSceneLayer::SpawnBuilding(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Building = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Building->SetPostion(pos);
		Building->SetRotation(rot);
		Building->SetScale(scale);

		Building->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Building->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Building->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(1.590f, 5.000f, 2.000f)));
		CollectCollider->SetPosition(glm::vec3(-0.190f, 1.280f, -0.080f));
	}
}
// For spawning building2 object
void DefaultSceneLayer::SpawnBuilding2(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Building2 = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Building2->SetPostion(pos);
		Building2->SetRotation(rot);
		Building2->SetScale(scale);

		Building2->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Building2->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Building2->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(1.700f, 3.500f, 2.000f)));
		CollectCollider->SetPosition(glm::vec3(0.570f, -3.230f, 1.150f));
	}
}
// For spawning building3 object
void DefaultSceneLayer::SpawnBuilding3(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Building3 = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Building3->SetPostion(pos);
		Building3->SetRotation(rot);
		Building3->SetScale(scale);

		Building3->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Building3->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Building3->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(1.500f, 4.800f, 1.700f)));
		CollectCollider->SetPosition(glm::vec3(-0.040f, -0.990f, 0.000f));
	}
}
// For spawning Small Wall Jump Platforms
void DefaultSceneLayer::SpawnSmallWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr SmallWallJump = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		SmallWallJump->SetPostion(pos);
		SmallWallJump->SetRotation(rot);
		SmallWallJump->SetScale(scale);

		SmallWallJump->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = SmallWallJump->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = SmallWallJump->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.3f, 0.5f, 2.5f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning very small wall jumps
void DefaultSceneLayer::SpawnSuperSmallWallJump(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr SuperSmallWallJump = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		SuperSmallWallJump->SetPostion(pos);
		SuperSmallWallJump->SetRotation(rot);
		SuperSmallWallJump->SetScale(scale);

		SuperSmallWallJump->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = SuperSmallWallJump->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = SuperSmallWallJump->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning right facing stairs
void DefaultSceneLayer::SpawnStairsRight(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr StairsRight = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		StairsRight->SetPostion(pos);
		StairsRight->SetRotation(rot);
		StairsRight->SetScale(scale);

		StairsRight->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = StairsRight->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = StairsRight->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(-0.120f, 0.460f, 1.030f));
		CollectCollider->SetScale(glm::vec3(13.138, 7.218, 0.5));
		CollectCollider->SetRotation(glm::vec3(68.0f, 0.000f, 0.00f));
	}
}
// For spawning Left facing stairs
void DefaultSceneLayer::SpawnStairsLeft(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr StairsLeft = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		StairsLeft->SetPostion(pos);
		StairsLeft->SetRotation(rot);
		StairsLeft->SetScale(scale);

		StairsLeft->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = StairsLeft->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = StairsLeft->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(-0.120f, 0.460f, 1.030f));
		CollectCollider->SetScale(glm::vec3(13.138, 7.218, 0.5));
		CollectCollider->SetRotation(glm::vec3(68.0f, 0.000f, 0.00f));
	}
}
// For spawning small speakers
void DefaultSceneLayer::SpawnSpeaker(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Speaker = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Speaker->SetPostion(pos);
		Speaker->SetRotation(rot);
		Speaker->SetScale(scale);

		Speaker->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Speaker->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);
	}
}
// For spawning Square platforms
void DefaultSceneLayer::SpawnSquarePlat(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr SquarePlat = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		SquarePlat->SetPostion(pos);
		SquarePlat->SetRotation(rot);
		SquarePlat->SetScale(scale);

		SquarePlat->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = SquarePlat->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = SquarePlat->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning Floating Street Lights
void DefaultSceneLayer::SpawnFloatingLights(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr FloatingLights = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		FloatingLights->SetPostion(pos);
		FloatingLights->SetRotation(rot);
		FloatingLights->SetScale(scale);

		FloatingLights->Add<LevelMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = FloatingLights->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = FloatingLights->Add<RigidBody>(RigidBodyType::Kinematic);
		// For Wall Jump Colliders, X = Left/Right Y = towards/away, z = Up/Down
		ICollider::Sptr CollectCollider = physics->AddCollider(BoxCollider::Create(glm::vec3(0.100f, 0.200f, 0.800f)));
		CollectCollider->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
// For spawning background cars
void DefaultSceneLayer::SpawnBackGroundCar(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Car1 = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Car1->SetPostion(pos);
		Car1->SetRotation(rot);
		Car1->SetScale(scale);

		//Add Components
		Car1->Add<BackgroundMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Car1->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Is background Object and therefore has no colliders
		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Car1->Add<RigidBody>(RigidBodyType::Kinematic);
	}
}
// For spawning Foreground cars
void DefaultSceneLayer::SpawnForeGroundCar(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr Car1 = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		Car1->SetPostion(pos);
		Car1->SetRotation(rot);
		Car1->SetScale(scale);

		//Add Components
		Car1->Add<ForeGroundMover>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = Car1->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Is background Object and therefore has no colliders
		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = Car1->Add<RigidBody>(RigidBodyType::Kinematic);
	}
}
// For spawning Background Buildings
void DefaultSceneLayer::SpawnBackGroundBuilding(Gameplay::Scene::Sptr scene, Gameplay::MeshResource::Sptr Mesh, Gameplay::Material::Sptr Material, std::string ObjName = "DeezNuts",
	glm::vec3 pos = glm::vec3(-10.900f, 5.610f, -4.920f), glm::vec3 rot = glm::vec3(180.0f, 0.0f, 180.0f),
	glm::vec3 scale = glm::vec3(0.350f, 0.350f, 0.350f)) {
	// Tutorial Stuff
	using namespace Gameplay;
	using namespace Gameplay::Physics;
	GameObject::Sptr KBuilding = scene->CreateGameObject(ObjName);
	{
		// Set position in the scene
		KBuilding->SetPostion(pos);
		KBuilding->SetRotation(rot);
		KBuilding->SetScale(scale);

		//Add Components
		// Background mover ment for cars
		KBuilding->Add<BackgroundBuildingMover>();
		KBuilding->Add<BuildingAnim>();

		// Create and attach a renderer for the Object
		RenderComponent::Sptr renderer = KBuilding->Add<RenderComponent>();
		renderer->SetMesh(Mesh);
		renderer->SetMaterial(Material);

		// Is background Object and therefore has no colliders
		// Add a dynamic rigid body to this object
		RigidBody::Sptr physics = KBuilding->Add<RigidBody>(RigidBodyType::Kinematic);
	}
}




void DefaultSceneLayer::OnAppLoad(const nlohmann::json& config) {
	_CreateScene();
}

void DefaultSceneLayer::_CreateScene()
{
	using namespace Gameplay;
	using namespace Gameplay::Physics;

	Application& app = Application::Get();

	bool loadScene = false;
	// For now we can use a toggle to generate our scene vs load from file
	if (loadScene && std::filesystem::exists("scene.json")) {
		app.LoadScene("scene.json");
	} else {
		// This time we'll have 2 different shaders, and share data between both of them using the UBO
		// This shader will handle reflective materials 

		//Meshes
		//MeshResource::Sptr SmallPlatform = ResourceManager::CreateAsset<MeshResource>("SmallSpeakerPlatformV5.obj");
		MeshResource::Sptr WallJump = ResourceManager::CreateAsset<MeshResource>("WallJumpV6.obj");
		MeshResource::Sptr BeatGem = ResourceManager::CreateAsset<MeshResource>("Gem.obj");
		MeshResource::Sptr Vinyl = ResourceManager::CreateAsset<MeshResource>("VinylV2.obj");
		MeshResource::Sptr CD = ResourceManager::CreateAsset<MeshResource>("CDwithUnwrap.obj");
		MeshResource::Sptr Building = ResourceManager::CreateAsset<MeshResource>("RBuilding.obj");
		MeshResource::Sptr KBuilding1Mesh = ResourceManager::CreateAsset<MeshResource>("KBuilding.obj");
		MeshResource::Sptr KBuilding2Mesh = ResourceManager::CreateAsset<MeshResource>("KBuilding2.obj");
		MeshResource::Sptr KBuilding3Mesh = ResourceManager::CreateAsset<MeshResource>("KBuilding3.obj");
		MeshResource::Sptr OvalBuilding = ResourceManager::CreateAsset<MeshResource>("OvalBuilding.obj");
		MeshResource::Sptr CharacterMesh = ResourceManager::CreateAsset<MeshResource>("discobot.obj");
		MeshResource::Sptr DiscoBallMesh = ResourceManager::CreateAsset<MeshResource>("DiscoBall2.obj");
		MeshResource::Sptr StartPlatform = ResourceManager::CreateAsset<MeshResource>("LStartPlatform.obj");
		MeshResource::Sptr Car1Mesh = ResourceManager::CreateAsset<MeshResource>("FutureCar1.obj");
		MeshResource::Sptr SemiTruckMesh = ResourceManager::CreateAsset<MeshResource>("Semitruck.obj");
		MeshResource::Sptr PickupTruckMesh = ResourceManager::CreateAsset<MeshResource>("FuturePickup.obj");
		MeshResource::Sptr SmallWallJump = ResourceManager::CreateAsset<MeshResource>("SmallWallJump.obj");
		MeshResource::Sptr SuperSmallWallJump = ResourceManager::CreateAsset<MeshResource>("SuperSmallWallJump.obj");
		MeshResource::Sptr FallingPlat = ResourceManager::CreateAsset<MeshResource>("pianoplatform.obj");
		MeshResource::Sptr HalfCirclePlat = ResourceManager::CreateAsset<MeshResource>("HalfCriclePlat.obj");
		MeshResource::Sptr StairsRight = ResourceManager::CreateAsset<MeshResource>("StairCaseR.obj");
		MeshResource::Sptr StairsLeft = ResourceManager::CreateAsset<MeshResource>("StairCaseL.obj");
		MeshResource::Sptr Speaker = ResourceManager::CreateAsset<MeshResource>("speaker.obj");
		MeshResource::Sptr SquarePlat = ResourceManager::CreateAsset<MeshResource>("SquarePlatform.obj");
		MeshResource::Sptr FloatingLight = ResourceManager::CreateAsset<MeshResource>("FloatingStreetLight.obj");
		MeshResource::Sptr DiscoBotMesh1 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run1.obj");
		MeshResource::Sptr DiscoBotMesh2 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run2.obj");
		MeshResource::Sptr DiscoBotMesh3 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run3.obj");
		MeshResource::Sptr DiscoBotMesh4 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run4.obj");
		MeshResource::Sptr DiscoBotMesh5 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run5.obj");
		MeshResource::Sptr DiscoBotMesh6 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run6.obj");
		MeshResource::Sptr DiscoBotMesh7 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run7.obj");
		MeshResource::Sptr DiscoBotMesh8 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run8.obj");
		MeshResource::Sptr DiscoBotMesh9 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/run9.obj");
		MeshResource::Sptr BotJump1 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump1.obj");
		MeshResource::Sptr BotJump2 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump2.obj");
		MeshResource::Sptr BotJump3 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump3.obj");
		MeshResource::Sptr BotJump4 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump4.obj");
		MeshResource::Sptr BotJump5 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump5.obj");
		MeshResource::Sptr BotJump6 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump6.obj");
		MeshResource::Sptr BotJump7 = ResourceManager::CreateAsset<MeshResource>("CharacterAnims/jump7.obj");
		//Textures
		Texture2D::Sptr StartTex = ResourceManager::CreateAsset<Texture2D>("textures/LStartPlatformTex.png");
		Texture2D::Sptr SmallTex = ResourceManager::CreateAsset<Texture2D>("textures/DanceFloorTex2.png");
		Texture2D::Sptr VinylTex = ResourceManager::CreateAsset<Texture2D>("textures/VinylTex.png");
		Texture2D::Sptr CDTex = ResourceManager::CreateAsset<Texture2D>("textures/CDTex.png");
		Texture2D::Sptr GemTex = ResourceManager::CreateAsset<Texture2D>("textures/Gem.png");
		Texture2D::Sptr CharacterTex = ResourceManager::CreateAsset<Texture2D>("textures/shirt.png");
		Texture2D::Sptr LoseScreenTex = ResourceManager::CreateAsset<Texture2D>("textures/Game_Over_Screen.png");
		Texture2D::Sptr SmallWallJumpTex = ResourceManager::CreateAsset<Texture2D>("textures/SmallWallJumpTexBlue.png");
		Texture2D::Sptr SuperSmallWallJumpTex = ResourceManager::CreateAsset<Texture2D>("textures/SmallWallJumpTexRed.png");
		Texture2D::Sptr WallJumpTex = ResourceManager::CreateAsset<Texture2D>("textures/WallJumpTex.png");
		Texture2D::Sptr Car1Tex = ResourceManager::CreateAsset<Texture2D>("textures/FutureCarTex.png");
		Texture2D::Sptr SemiTruckTex = ResourceManager::CreateAsset<Texture2D>("textures/SemiTruckTexV2.png");
		Texture2D::Sptr PickupTruckTex = ResourceManager::CreateAsset<Texture2D>("textures/PickupTruckTex.png");
		Texture2D::Sptr KBuilding1Tex = ResourceManager::CreateAsset<Texture2D>("textures/KBuildingTex.png");
		Texture2D::Sptr KBuilding2Tex = ResourceManager::CreateAsset<Texture2D>("textures/KBuilding2Tex.png");
		Texture2D::Sptr KBuilding3Tex = ResourceManager::CreateAsset<Texture2D>("textures/KBuilding3Tex.png");
		Texture2D::Sptr BuildingTex = ResourceManager::CreateAsset<Texture2D>("textures/RBuildingTex.png");
		Texture2D::Sptr OvalBuildingTex = ResourceManager::CreateAsset<Texture2D>("textures/OvalBuildingTex.png");
		Texture2D::Sptr DiscoBallTex = ResourceManager::CreateAsset<Texture2D>("textures/DiscoBallTexV2.png");
		Texture2D::Sptr FallingPlatTex = ResourceManager::CreateAsset<Texture2D>("textures/pianotex.png");
		Texture2D::Sptr HalfCirclePlatTex = ResourceManager::CreateAsset<Texture2D>("textures/halfCircleTex.png");
		Texture2D::Sptr TexBeatLogo = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BeatLogo.png");
		Texture2D::Sptr TexPlayButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BPlay.png");
		Texture2D::Sptr TexOptionsButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BOptions.png");
		Texture2D::Sptr TexMusicButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BMusic.png");
		Texture2D::Sptr TexCreditsButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BCredits.png");
		Texture2D::Sptr TexQuitButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BQuit.png");
		Texture2D::Sptr TexResumeButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BResume.png");
		Texture2D::Sptr TexResyncButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BResync.png");
		Texture2D::Sptr TexContinueButton = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BContinue.png");
		Texture2D::Sptr TexPauseMenu = ResourceManager::CreateAsset<Texture2D>("textures/GUI/PauseMenuBG.png");
		Texture2D::Sptr TexDimmedBG = ResourceManager::CreateAsset<Texture2D>("textures/GUI/DimBG.png");
		Texture2D::Sptr TexScoreBreakdown = ResourceManager::CreateAsset<Texture2D>("textures/GUI/ScoreBreakdown.png");
		Texture2D::Sptr TexGameOverText = ResourceManager::CreateAsset<Texture2D>("textures/GUI/GameOverText.png");
		Texture2D::Sptr TexMovementTutorial = ResourceManager::CreateAsset<Texture2D>("textures/GUI/Movement.png");
		Texture2D::Sptr TexWallJumpTutorial = ResourceManager::CreateAsset<Texture2D>("textures/GUI/WallJump.png");
		Texture2D::Sptr TexBeatGemTutorial = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BeatGems.png");
		Texture2D::Sptr TexVinylsTutorial = ResourceManager::CreateAsset<Texture2D>("textures/GUI/Vinyls.png");
		Texture2D::Sptr StairsRightTex = ResourceManager::CreateAsset<Texture2D>("textures/StairCaseRTex.png");
		Texture2D::Sptr StairsLeftTex = ResourceManager::CreateAsset<Texture2D>("textures/StarTexL.png");
		Texture2D::Sptr SpeakerTex = ResourceManager::CreateAsset<Texture2D>("textures/speakertex.png");
		Texture2D::Sptr SquarePlatTex = ResourceManager::CreateAsset<Texture2D>("textures/SquarePlatformTex.png");
		Texture2D::Sptr FloatingLightTex = ResourceManager::CreateAsset<Texture2D>("textures/StreetLightTex.png");
		Texture2D::Sptr TexBeatBar = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BeatBar.png");
		Texture2D::Sptr TexBeatBarTick = ResourceManager::CreateAsset<Texture2D>("textures/GUI/BeatBarTick.png");
		Texture2D::Sptr GemOff = ResourceManager::CreateAsset<Texture2D>("textures/GemOff.png");
		Texture2D::Sptr TexScoreDisplay = ResourceManager::CreateAsset<Texture2D>("textures/GUI/ScoreDisplay.png");

		Font::Sptr FontVCR = ResourceManager::CreateAsset<Font>("fonts/VCR.ttf", 16.f);
		FontVCR->Bake();
		ShaderProgram::Sptr reflectiveShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_environment_reflective.glsl" }
		});

		// This shader handles our basic materials without reflections (cause they expensive)
		ShaderProgram::Sptr basicShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_blinn_phong_textured.glsl" }
		});

		// This shader handles our basic materials without reflections (cause they expensive)
		ShaderProgram::Sptr specShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/textured_specular.glsl" }
		});

		// This shader handles our foliage vertex shader example
		ShaderProgram::Sptr foliageShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/foliage.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/screendoor_transparency.glsl" }
		});

		// This shader handles our cel shading example
		ShaderProgram::Sptr toonShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/toon_shading.glsl" }
		});

		// This shader handles our displacement mapping example
		ShaderProgram::Sptr displacementShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/displacement_mapping.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_tangentspace_normal_maps.glsl" }
		});

		// This shader handles our tangent space normal mapping
		ShaderProgram::Sptr tangentSpaceMapping = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_tangentspace_normal_maps.glsl" }
		});

		// This shader handles our multitexturing example
		ShaderProgram::Sptr multiTextureShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/vert_multitextured.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_multitextured.glsl" }
		});

		// Load in the meshes
		MeshResource::Sptr monkeyMesh = ResourceManager::CreateAsset<MeshResource>("Monkey.obj");

		// Load in some textures
		Texture2D::Sptr    boxTexture   = ResourceManager::CreateAsset<Texture2D>("textures/box-diffuse.png");
		Texture2D::Sptr    boxSpec      = ResourceManager::CreateAsset<Texture2D>("textures/box-specular.png");
		Texture2D::Sptr    monkeyTex    = ResourceManager::CreateAsset<Texture2D>("textures/monkey-uvMap.png");
		Texture2D::Sptr    leafTex      = ResourceManager::CreateAsset<Texture2D>("textures/leaves.png");
		leafTex->SetMinFilter(MinFilter::Nearest);
		leafTex->SetMagFilter(MagFilter::Nearest);


		// Here we'll load in the cubemap, as well as a special shader to handle drawing the skybox
		TextureCube::Sptr testCubemap = ResourceManager::CreateAsset<TextureCube>("cubemaps/ocean/ocean.jpg");
		ShaderProgram::Sptr      skyboxShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/skybox_vert.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/skybox_frag.glsl" }
		});

		// Create an empty scene
		Scene::Sptr scene = std::make_shared<Scene>();

		// Setting up our enviroment map
		scene->SetSkyboxTexture(testCubemap);
		scene->SetSkyboxShader(skyboxShader);
		// Since the skybox I used was for Y-up, we need to rotate it 90 deg around the X-axis to convert it to z-up
		scene->SetSkyboxRotation(glm::rotate(MAT4_IDENTITY, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));

		// Create our materials
		// This will be our box material, with no environment reflections

	// Create our materials
		Material::Sptr StartPlatformMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			StartPlatformMaterial->Name = "StartPlatform";
			StartPlatformMaterial->Set("u_Material.Diffuse", StartTex);
			StartPlatformMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr UIMat = ResourceManager::CreateAsset<Material>(basicShader);
		{
			UIMat->Name = "UIButton";
			UIMat->Set("u_Material.Diffuse", StartTex);
			UIMat->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SmallPlatformMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			SmallPlatformMaterial->Name = "SmallPlatform";
			SmallPlatformMaterial->Set("u_Material.Diffuse", SmallTex);
			SmallPlatformMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr WallJumpMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			WallJumpMaterial->Name = "WallJump";
			WallJumpMaterial->Set("u_Material.Diffuse", WallJumpTex);
			WallJumpMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr BeatGemMaterial = ResourceManager::CreateAsset<Material>(reflectiveShader);
		{
			BeatGemMaterial->Name = "BeatGem";
			BeatGemMaterial->Set("u_Material.Diffuse", GemOff);
			BeatGemMaterial->Set("u_Material.Shininess", 0.2f);
		}

		Material::Sptr BeatGemOffMaterial = ResourceManager::CreateAsset<Material>(reflectiveShader);
		{
			BeatGemMaterial->Name = "BeatGem";
			BeatGemMaterial->Set("u_Material.Diffuse", GemTex);
			BeatGemMaterial->Set("u_Material.Shininess", 0.2f);
		}


		Material::Sptr VinylMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			VinylMaterial->Name = "Vinyl";
			VinylMaterial->Set("u_Material.Diffuse", VinylTex);
			VinylMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr CDMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			CDMaterial->Name = "CD";
			CDMaterial->Set("u_Material.Diffuse", CDTex);
			CDMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr CharacterMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			CharacterMaterial->Name = "Character";
			CharacterMaterial->Set("u_Material.Diffuse", CharacterTex);
			CharacterMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr DiscoBallMaterial = ResourceManager::CreateAsset<Material>(reflectiveShader);
		{
			DiscoBallMaterial->Name = "DiscoBall";
			DiscoBallMaterial->Set("u_Material.Diffuse", DiscoBallTex);
			DiscoBallMaterial->Set("u_Material.Shininess", -0.4f);
		}

		Material::Sptr LoseScreenMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			LoseScreenMaterial->Name = "Lose Screen";
			LoseScreenMaterial->Set("u_Material.Diffuse", LoseScreenTex);
			LoseScreenMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr Car1Material = ResourceManager::CreateAsset<Material>(basicShader);
		{
			Car1Material->Name = "Car1";
			Car1Material->Set("u_Material.Diffuse", Car1Tex);
			Car1Material->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SemiTruckMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			SemiTruckMaterial->Name = "Semi1";
			SemiTruckMaterial->Set("u_Material.Diffuse", SemiTruckTex);
			SemiTruckMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr PickupTruckMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			PickupTruckMaterial->Name = "Pickup1";
			PickupTruckMaterial->Set("u_Material.Diffuse", PickupTruckTex);
			PickupTruckMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr BuildingMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			BuildingMaterial->Name = "Building";
			BuildingMaterial->Set("u_Material.Diffuse", BuildingTex);
			BuildingMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr KBuildingMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			KBuildingMaterial->Name = "KBuilding";
			KBuildingMaterial->Set("u_Material.Diffuse", KBuilding1Tex);
			KBuildingMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr KBuilding2Material = ResourceManager::CreateAsset<Material>(basicShader);
		{
			KBuilding2Material->Name = "KBuilding2";
			KBuilding2Material->Set("u_Material.Diffuse", KBuilding2Tex);
			KBuilding2Material->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr KBuilding3Material = ResourceManager::CreateAsset<Material>(basicShader);
		{
			KBuilding3Material->Name = "KBuilding3";
			KBuilding3Material->Set("u_Material.Diffuse", KBuilding3Tex);
			KBuilding3Material->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SmallWallJumpMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			SmallWallJumpMaterial->Name = "Small Wall Jump";
			SmallWallJumpMaterial->Set("u_Material.Diffuse", SmallWallJumpTex);
			SmallWallJumpMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SuperSmallWallJumpMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			SuperSmallWallJumpMaterial->Name = "Super Small Wall Jump";
			SuperSmallWallJumpMaterial->Set("u_Material.Diffuse", SuperSmallWallJumpTex);
			SuperSmallWallJumpMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr PianoMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			PianoMaterial->Name = "Piano";
			PianoMaterial->Set("u_Material.Diffuse", FallingPlatTex);
			PianoMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr HalfCirclePlatMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			HalfCirclePlatMaterial->Name = "Half Circle Plat";
			HalfCirclePlatMaterial->Set("u_Material.Diffuse", HalfCirclePlatTex);
			HalfCirclePlatMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr StairsRightMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			StairsRightMaterial->Name = "Stairs Right";
			StairsRightMaterial->Set("u_Material.Diffuse", StairsRightTex);
			StairsRightMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr StairsLeftMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			StairsLeftMaterial->Name = "Stairs Left";
			StairsLeftMaterial->Set("u_Material.Diffuse", StairsLeftTex);
			StairsLeftMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SpeakerMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			SpeakerMaterial->Name = "Speaker Material";
			SpeakerMaterial->Set("u_Material.Diffuse", SpeakerTex);
			SpeakerMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr SquarePlatMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			SquarePlatMaterial->Name = "Square Platform";
			SquarePlatMaterial->Set("u_Material.Diffuse", SquarePlatTex);
			SquarePlatMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr FloatingLightMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			FloatingLightMaterial->Name = "FLoating Light";
			FloatingLightMaterial->Set("u_Material.Diffuse", FloatingLightTex);
			FloatingLightMaterial->Set("u_Material.Shininess", 0.1f);
		}

		Material::Sptr OvalBuildingMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			OvalBuildingMaterial->Name = "Oval Building";
			OvalBuildingMaterial->Set("u_Material.Diffuse", OvalBuildingTex);
			OvalBuildingMaterial->Set("u_Material.Shininess", 0.1f);
		}

		// Create some lights for our scene
		// Create some lights for our scene
		scene->Lights.resize(6);
		//scene->Lights[0].Position = glm::vec3(0.0f, 1.0f, 3.0f);
		scene->Lights[0].Color = glm::vec3(1.0f, 1.0f, 1.0f);
		scene->Lights[0].Range = 100.0f;

		scene->Lights[2].Position = glm::vec3(-1.380f, 17.460f, -5.710f);
		scene->Lights[2].Color = glm::vec3(1.0f, 1.0f, 1.0f);
		scene->Lights[2].Range = 50;

		scene->Lights[3].Position = glm::vec3(-25.380f, 14.060f, -14.020f);
		scene->Lights[3].Color = glm::vec3(1.0f, 1.0f, 1.0f);
		scene->Lights[3].Range = 187;

		scene->Lights[4].Position = glm::vec3(25.380f, 14.060f, -14.020f);
		scene->Lights[4].Color = glm::vec3(1.0f, 1.0f, 1.0f);
		scene->Lights[4].Range = 187;

		scene->Lights[5].Color = glm::vec3(1.0f, 1.0f, 1.0f);


		// Red/Green light
		scene->Lights[1].Color = glm::vec3((1.0f, 0.99f, 0.99f));
		scene->Lights[1].Range = 100.0f;


		// We'll create a mesh that is a simple plane that we can resize later
		MeshResource::Sptr planeMesh = ResourceManager::CreateAsset<MeshResource>();
		planeMesh->AddParam(MeshBuilderParam::CreatePlane(ZERO, UNIT_Z, UNIT_X, glm::vec2(1.0f)));
		planeMesh->GenerateMesh();

		MeshResource::Sptr sphere = ResourceManager::CreateAsset<MeshResource>();
		sphere->AddParam(MeshBuilderParam::CreateIcoSphere(ZERO, ONE, 5));
		sphere->GenerateMesh();

		// Set up the scene's camera
		GameObject::Sptr camera = scene->MainCamera->GetGameObject()->SelfRef();
		{
			camera->SetPostion({ -9, -6, 15 });
			camera->LookAt(glm::vec3(0.0f));

			camera->Add<SimpleCameraControl>();

			//Camera::Sptr cam = camera->Add<Camera>();
			// Make sure that the camera is set as the scene's main camera!
			//scene->MainCamera = cam;
		}


		/////////////////////////// UI //////////////////////////////
		/*
		GameObject::Sptr canvas = scene->CreateGameObject("UI Canvas");
		{
			RectTransform::Sptr transform = canvas->Add<RectTransform>();
			transform->SetMin({ 16, 16 });
			transform->SetMax({ 256, 256 });

			GuiPanel::Sptr canPanel = canvas->Add<GuiPanel>();


			GameObject::Sptr subPanel = scene->CreateGameObject("Sub Item");
			{
				RectTransform::Sptr transform = subPanel->Add<RectTransform>();
				transform->SetMin({ 10, 10 });
				transform->SetMax({ 128, 128 });

				GuiPanel::Sptr panel = subPanel->Add<GuiPanel>();
				panel->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

				panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 16.0f);
				font->Bake();

				GuiText::Sptr text = subPanel->Add<GuiText>();
				text->SetText("Hello world!");
				text->SetFont(font);

				monkey1->Get<JumpBehaviour>()->Panel = text;
			}

			canvas->AddChild(subPanel);
		}
		*/

		GameObject::Sptr particles = scene->CreateGameObject("Particles");
		{
			ParticleSystem::Sptr particleManager = particles->Add<ParticleSystem>();  
			particleManager->AddEmitter(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 10.0f), 10.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); 
		}

		GuiBatcher::SetDefaultTexture(ResourceManager::CreateAsset<Texture2D>("textures/ui-sprite.png"));
		GuiBatcher::SetDefaultBorderRadius(8);

		// Save the asset manifest for all the resources we just loaded
		ResourceManager::SaveManifest("scene-manifest.json");
		// Save the scene to a JSON file
		scene->Save("scene.json");

		// Send the scene to the application
		app.LoadScene(scene);
	}

}


