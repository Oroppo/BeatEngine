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

// Animations
#include "Gameplay/Animation/MorphAnimator.h"
#include "Gameplay/Animation/MorphMeshRenderer.h"

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
#include "Gameplay/Components/SpawnLoop.h"


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
#include "Application/InitResources.h"



DefaultSceneLayer::DefaultSceneLayer() :
	ApplicationLayer()
{
	Name = "Default Scene";
	Overrides = AppLayerFunctions::OnAppLoad;
}

DefaultSceneLayer::~DefaultSceneLayer() = default;

// Object Spawning Functions lines 99-623 CTRL + M, CTRL + O to collapse functions





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
		FontVCR->Bake();
		leafTex->SetMinFilter(MinFilter::Nearest);
		leafTex->SetMagFilter(MagFilter::Nearest);

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
		{
			StartPlatformMaterial->Name = "StartPlatform";
			StartPlatformMaterial->Set("u_Material.Diffuse", StartTex);
			StartPlatformMaterial->Set("u_Material.Shininess", 0.1f);
		}


		{
			UIMat->Name = "UIButton";
			UIMat->Set("u_Material.Diffuse", StartTex);
			UIMat->Set("u_Material.Shininess", 0.1f);
		}

		{
			SmallPlatformMaterial->Name = "SmallPlatform";
			SmallPlatformMaterial->Set("u_Material.Diffuse", SmallTex);
			SmallPlatformMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			WallJumpMaterial->Name = "WallJump";
			WallJumpMaterial->Set("u_Material.Diffuse", WallJumpTex);
			WallJumpMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			BeatGemMaterial->Name = "BeatGem";
			BeatGemMaterial->Set("u_Material.Diffuse", GemOff);
			BeatGemMaterial->Set("u_Material.Shininess", 0.2f);
		}

		{
			BeatGemMaterial->Name = "BeatGem";
			BeatGemMaterial->Set("u_Material.Diffuse", GemTex);
			BeatGemMaterial->Set("u_Material.Shininess", 0.2f);
		}

		{
			VinylMaterial->Name = "Vinyl";
			VinylMaterial->Set("u_Material.Diffuse", VinylTex);
			VinylMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			CDMaterial->Name = "CD";
			CDMaterial->Set("u_Material.Diffuse", CDTex);
			CDMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			CharacterMaterial->Name = "Character";
			CharacterMaterial->Set("u_Material.Diffuse", CharacterTex);
			CharacterMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			DiscoBallMaterial->Name = "DiscoBall";
			DiscoBallMaterial->Set("u_Material.Diffuse", DiscoBallTex);
			DiscoBallMaterial->Set("u_Material.Shininess", -0.4f);
		}

		{
			LoseScreenMaterial->Name = "Lose Screen";
			LoseScreenMaterial->Set("u_Material.Diffuse", LoseScreenTex);
			LoseScreenMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			Car1Material->Name = "Car1";
			Car1Material->Set("u_Material.Diffuse", Car1Tex);
			Car1Material->Set("u_Material.Shininess", 0.1f);
		}

		{
			SemiTruckMaterial->Name = "Semi1";
			SemiTruckMaterial->Set("u_Material.Diffuse", SemiTruckTex);
			SemiTruckMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			PickupTruckMaterial->Name = "Pickup1";
			PickupTruckMaterial->Set("u_Material.Diffuse", PickupTruckTex);
			PickupTruckMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			BuildingMaterial->Name = "Building";
			BuildingMaterial->Set("u_Material.Diffuse", BuildingTex);
			BuildingMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			KBuildingMaterial->Name = "KBuilding";
			KBuildingMaterial->Set("u_Material.Diffuse", KBuilding1Tex);
			KBuildingMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			KBuilding2Material->Name = "KBuilding2";
			KBuilding2Material->Set("u_Material.Diffuse", KBuilding2Tex);
			KBuilding2Material->Set("u_Material.Shininess", 0.1f);
		}

		{
			KBuilding3Material->Name = "KBuilding3";
			KBuilding3Material->Set("u_Material.Diffuse", KBuilding3Tex);
			KBuilding3Material->Set("u_Material.Shininess", 0.1f);
		}

		{
			SmallWallJumpMaterial->Name = "Small Wall Jump";
			SmallWallJumpMaterial->Set("u_Material.Diffuse", SmallWallJumpTex);
			SmallWallJumpMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			SuperSmallWallJumpMaterial->Name = "Super Small Wall Jump";
			SuperSmallWallJumpMaterial->Set("u_Material.Diffuse", SuperSmallWallJumpTex);
			SuperSmallWallJumpMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			PianoMaterial->Name = "Piano";
			PianoMaterial->Set("u_Material.Diffuse", FallingPlatTex);
			PianoMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			HalfCirclePlatMaterial->Name = "Half Circle Plat";
			HalfCirclePlatMaterial->Set("u_Material.Diffuse", HalfCirclePlatTex);
			HalfCirclePlatMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			StairsRightMaterial->Name = "Stairs Right";
			StairsRightMaterial->Set("u_Material.Diffuse", StairsRightTex);
			StairsRightMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			StairsLeftMaterial->Name = "Stairs Left";
			StairsLeftMaterial->Set("u_Material.Diffuse", StairsLeftTex);
			StairsLeftMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			SpeakerMaterial->Name = "Speaker Material";
			SpeakerMaterial->Set("u_Material.Diffuse", SpeakerTex);
			SpeakerMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			SquarePlatMaterial->Name = "Square Platform";
			SquarePlatMaterial->Set("u_Material.Diffuse", SquarePlatTex);
			SquarePlatMaterial->Set("u_Material.Shininess", 0.1f);
		}

		{
			FloatingLightMaterial->Name = "FLoating Light";
			FloatingLightMaterial->Set("u_Material.Diffuse", FloatingLightTex);
			FloatingLightMaterial->Set("u_Material.Shininess", 0.1f);
		}

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
			camera->SetPostion({ -1.410, -3.500, 2.450 });
			camera->LookAt(glm::vec3(0.0f));
			camera->SetRotation(glm::vec3(-103, 180, -180));

			//camera->Add<SimpleCameraControl>();

			//Camera::Sptr cam = camera->Add<Camera>();
			// Make sure that the camera is set as the scene's main camera!
			//scene->MainCamera = cam;
		}

		SpawnFunctions Spawner;

		// Background and forground vehicles\\
		Give these Parents for Foreground/Background Blocks if we have enough objects to do that with!
		Spawner.SpawnBackGroundCar(scene, Car1Mesh, Car1Material, "Car1", glm::vec3(14.870f, 7.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		Spawner.SpawnBackGroundCar(scene, SemiTruckMesh, SemiTruckMaterial, "Semi1", glm::vec3(28.870f, 7.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		Spawner.SpawnForeGroundCar(scene, Car1Mesh, Car1Material, "Car2", glm::vec3(-9.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		Spawner.SpawnForeGroundCar(scene, PickupTruckMesh, PickupTruckMaterial, "Pickup1", glm::vec3(-18.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		Spawner.SpawnBackGroundBuilding(scene, KBuilding1Mesh, KBuildingMaterial, "KBuilding1", glm::vec3(-1.0f, 21.880f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.780f, 1.470f, 1.0f));
		Spawner.SpawnBackGroundBuilding(scene, KBuilding2Mesh, KBuilding2Material, "KBuilding2", glm::vec3(25.670, 21.880f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.780f, 1.470f, 1.0f));
		Spawner.SpawnBackGroundBuilding(scene, KBuilding3Mesh, KBuilding3Material, "KBuilding3", glm::vec3(-30.530f, 21.880f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.780f, 1.470f, 1.0f));
		Spawner.SpawnBackGroundBuilding(scene, OvalBuilding, OvalBuildingMaterial, "OvalBuilding", glm::vec3(13.730f, 27.510f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.720f, 3.500f, 1.0f));

		// Tutorial

		Spawner.SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-9.820f, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-6.070f, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-3.320f, 5.610f, -2.200f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-0.400f, 5.610f, -4.040f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-2.110f, 5.610f, 5.440f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(-0.350f, 5.610f, 3.070f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		Spawner.SpawnWallJump(scene, WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(2.430f, 5.610f, 3.930f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		Spawner.SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", glm::vec3(2.020f, 5.610f, -1.910f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		Spawner.SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(-2.110f, 5.610f, 6.010f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		Spawner.SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(6.360f, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnBackGroundCar(scene, Car1Mesh, Car1Material, "Car1", glm::vec3(14.870f, 9.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		Spawner.SpawnBackGroundCar(scene, SemiTruckMesh, SemiTruckMaterial, "Semi1", glm::vec3(28.870f, 9.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		Spawner.SpawnForeGroundCar(scene, Car1Mesh, Car1Material, "Car2", glm::vec3(-9.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		Spawner.SpawnForeGroundCar(scene, PickupTruckMesh, PickupTruckMaterial, "Pickup1", glm::vec3(-18.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));


		// 1st Block		
		Spawner.SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-9.820f + 22, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-6.070f + 22, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-2.840f + 22, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnObj(scene, SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(2.760f + 22, 5.610f, -1.770f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		Spawner.SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem", glm::vec3(0.120f + 22, 5.610f, -3.160f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		Spawner.SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(5.640f + 22, 5.610f, 0.080f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		Spawner.SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(6.360f + 22, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 1
		Spawner.SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-6.030f + 22, 5.610f, -3.220f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		Spawner.SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(-2.710f + 22, 5.610f, -3.190f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		Spawner.SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(0.170f + 22, 5.610f, -2.380f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		Spawner.SpawnCD(scene, CD, CDMaterial, "CD", glm::vec3(2.640f + 22, 5.610f, -0.770f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));

		//trust me if I could find a better way i'd have done it but rn we can't pass materials to 
		//the Material Swap Component so we instead add Dummy Children Objects to hold the Materials instead.
		{
			GameObject::Sptr MaterialDummyOn = scene->CreateGameObject("Material Dummy On");
			RenderComponent::Sptr renderer = MaterialDummyOn->Add<RenderComponent>();
			renderer->SetMesh(BeatGem);
			renderer->SetMaterial(BeatGemMaterial);
			renderer->IsEnabled = false;
		}
		{
			GameObject::Sptr MaterialDummyOff = scene->CreateGameObject("Material Dummy Off");
			RenderComponent::Sptr renderer = MaterialDummyOff->Add<RenderComponent>();
			renderer->SetMesh(BeatGem);
			renderer->SetMaterial(BeatGemOffMaterial);
			renderer->IsEnabled = false;
		}

		GameObject::Sptr GameManager = scene->CreateGameObject("GameManager");
		{
			GameManager->Add <SpawnLoop>();
			GameManager->Add <BeatTimer>();
		}

		// Player:
		GameObject::Sptr character = scene->CreateGameObject("Character/Player");
		{
			// Set position in the scene
			character->SetPostion(glm::vec3(-10.270f, 5.710f, -3.800f));
			character->SetRotation(glm::vec3(90.0f, 0.0f, 90.0f));
			character->SetScale(glm::vec3(0.7f, 0.7f, 0.7f));

			// Add some behaviour that relies on the physics body
			//character->Add<JumpBehaviour>();
			character->Add<CharacterController>();

			// Create and attach a renderer for the paddle
			RenderComponent::Sptr renderer = character->Add<RenderComponent>();
			renderer->SetMesh(CharacterMesh);
			renderer->SetMaterial(CharacterMaterial);

			// Add a kinematic rigid body to the paddle
			RigidBody::Sptr physics = character->Add<RigidBody>(RigidBodyType::Dynamic);
			auto rb = physics->AddCollider(BoxCollider::Create(glm::vec3(0.2, 0.85, 0.15)));
			rb->SetPosition(glm::vec3(0.0, 0.75, 0.0));

			//TriggerVolume::Sptr volume = character->Add<TriggerVolume>();
			//volume->SetFlags(TriggerTypeFlags::Statics | TriggerTypeFlags::Kinematics);

			//BoxCollider::Sptr collider = BoxCollider::Create(glm::vec3(0.3f, 0.3f, 0.3f));
			//collider->SetPosition(glm::vec3(0.f, 0.25f, 0.f));
			//volume->AddCollider(collider);


			
			MorphMeshRenderer::Sptr morph1 = character->Add<MorphMeshRenderer>();
			morph1->SetMorphMeshRenderer(DiscoBotMesh1, CharacterMaterial);
			Morphanimator::Sptr RunAnim = character->Add<Morphanimator>();

			std::vector<MeshResource::Sptr> KeyFrames;
			std::vector<MeshResource::Sptr> KeyFrames2;
			KeyFrames.push_back(DiscoBotMesh2);
			KeyFrames.push_back(DiscoBotMesh3);
			KeyFrames.push_back(DiscoBotMesh4);
			KeyFrames.push_back(DiscoBotMesh5);
			KeyFrames.push_back(DiscoBotMesh6);
			KeyFrames.push_back(DiscoBotMesh7);
			KeyFrames.push_back(DiscoBotMesh8);
			KeyFrames.push_back(DiscoBotMesh9);
			
			KeyFrames2.push_back(DiscoBotMesh2);
			KeyFrames2.push_back(DiscoBotMesh3);
			KeyFrames2.push_back(DiscoBotMesh4);
			KeyFrames2.push_back(DiscoBotMesh5);
			KeyFrames2.push_back(DiscoBotMesh6);
			KeyFrames2.push_back(DiscoBotMesh7);


			RunAnim->SetInitial();
			RunAnim->SetFrameTime(0.1f);
			RunAnim->SetFrames(KeyFrames);
		}

		GameObject::Sptr DiscoBall = scene->CreateGameObject("DiscoBall");
		{
			DiscoBall->SetPostion(glm::vec3(-10.270f, 5.710f, -1.0f));
			DiscoBall->SetRotation(glm::vec3(90.0f, 0.0f, 90.0f));
			DiscoBall->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

			RenderComponent::Sptr renderer = DiscoBall->Add<RenderComponent>();
			renderer->SetMesh(DiscoBallMesh);
			renderer->SetMaterial(DiscoBallMaterial);

			SeekBehaviour::Sptr seeking = DiscoBall->Add<SeekBehaviour>();
			seeking->seekTo(character);

			RigidBody::Sptr ballphysics = DiscoBall->Add<RigidBody>(RigidBodyType::Dynamic);
		}

		/////////////////////////// UI //////////////////////////////
		/*
		{//Main Menu Block

			{//Logo
				GameObject::Sptr logo = scene->CreateGameObject("MainMenu Logo");

				RectTransform::Sptr transform = logo->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 750, 750 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 750, 750 });

				GuiPanel::Sptr logoPanel = logo->Add<GuiPanel>();
				logoPanel->SetTexture(TexBeatLogo);
				logoPanel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				logoPanel->SetBorderRadius(0);
				logoPanel->IsEnabled = false;



				transform->SetPosition({app.GetWindowSize().x * 0.5, 300 });

			}

			{//Play Button
				GameObject::Sptr button = scene->CreateGameObject("MainMenu Play Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 200, 100 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 200, 100 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexPlayButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.20, app.GetWindowSize().y * 0.8 });

			}

			{//Options Button
				GameObject::Sptr button = scene->CreateGameObject("MainMenu Options Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 200, 100 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 200, 100 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexOptionsButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.35, app.GetWindowSize().y * 0.8 });

			}

			{//Music Button
				GameObject::Sptr button = scene->CreateGameObject("MainMenu Music Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 200, 100 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 200, 100 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexMusicButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.8 });

			}

			{//Credits Button
				GameObject::Sptr button = scene->CreateGameObject("MainMenu Credits Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 200, 100 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 200, 100 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexCreditsButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;



				transform->SetPosition({ app.GetWindowSize().x * 0.65, app.GetWindowSize().y * 0.8 });

			}

			{//Quit Button
				GameObject::Sptr button = scene->CreateGameObject("MainMenu Quit Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 200, 100 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 200, 100 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexQuitButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;



				transform->SetPosition({ app.GetWindowSize().x * 0.8, app.GetWindowSize().y * 0.8 });

			}

		}


		{//Pause Menu Block

			{//Dim BG
				GameObject::Sptr background = scene->CreateGameObject("PauseMenu Dimmed Background");

				RectTransform::Sptr transform = background->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 1920, 1080 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 1920, 1080 });

				GuiPanel::Sptr panel = background->Add<GuiPanel>();
				panel->SetTexture(TexDimmedBG);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.5 });

			}

			{//Background
				GameObject::Sptr background = scene->CreateGameObject("PauseMenu Background");

				RectTransform::Sptr transform = background->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 400, 750 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 400, 750 });

				GuiPanel::Sptr panel = background->Add<GuiPanel>();
				panel->SetTexture(TexPauseMenu);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.5 });

			}

			{//Resume Button
				GameObject::Sptr button = scene->CreateGameObject("PauseMenu Resume Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 300, 150 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 300, 150 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexResumeButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.28 });

			}

			{//Options Button
				GameObject::Sptr button = scene->CreateGameObject("PauseMenu Options Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 300, 150 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 300, 150 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexOptionsButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;

				transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.43 });

			}

			{//Resync Button
				GameObject::Sptr button = scene->CreateGameObject("PauseMenu Resync Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 300, 150 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 300, 150 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexResyncButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;

				transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.58 });

			}

			{//Quit Button
				GameObject::Sptr button = scene->CreateGameObject("PauseMenu Quit Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 300, 150 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 300, 150 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexQuitButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.73 });

			}
		}

		{//Game Over Block

			{//Dim BG
				GameObject::Sptr background = scene->CreateGameObject("GameOver Dimmed Background");

				RectTransform::Sptr transform = background->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 1920, 1080 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 1920, 1080 });

				GuiPanel::Sptr panel = background->Add<GuiPanel>();
				panel->SetTexture(TexDimmedBG);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.5 });

			}

			{//Game Over Text
				GameObject::Sptr button = scene->CreateGameObject("GameOver Text");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 809, 249 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 809, 249 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexGameOverText);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.2 });

			}

			{//Score breakdown
				GameObject::Sptr button = scene->CreateGameObject("GameOver Score Breakdown");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 504 * 0.75, 475 * 0.75 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 504 * 0.75, 475 * 0.75 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexScoreBreakdown);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.4, app.GetWindowSize().y * 0.5 });

			}


			{//Quit
				GameObject::Sptr button = scene->CreateGameObject("GameOver Quit Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 300, 150 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 300, 150 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexQuitButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;

				transform->SetPosition({ app.GetWindowSize().x * 0.35, app.GetWindowSize().y * 0.8 });

			}

			{//Continue Button
				GameObject::Sptr button = scene->CreateGameObject("GameOver Continue Button");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 300, 150 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 300, 150 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexContinueButton);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;


				transform->SetPosition({ app.GetWindowSize().x * 0.65, app.GetWindowSize().y * 0.8 });

			}
		}

		{//Tutorial Blocks

			{//Movement
				GameObject::Sptr button = scene->CreateGameObject("Movement Tutorial");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 700 * 0.75, 500 * 0.75 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 700 * 0.75, 500 * 0.75 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexMovementTutorial);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);


				transform->SetPosition({ app.GetWindowSize().x * 0.2, app.GetWindowSize().y * 0.2 });

			}

			{//Wall Jump
				GameObject::Sptr button = scene->CreateGameObject("Wall Jump Tutorial");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 700 * 0.75, 500 * 0.75 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 700 * 0.75, 500 * 0.75 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexWallJumpTutorial);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);


				transform->SetPosition({ app.GetWindowSize().x * 0.2, app.GetWindowSize().y * 0.6 });

			}

			{//Beat Gem
				GameObject::Sptr button = scene->CreateGameObject("Beat Gem Tutorial");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 700 * 0.75, 500 * 0.75 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 700 * 0.75, 500 * 0.75 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexBeatGemTutorial);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);


				transform->SetPosition({ app.GetWindowSize().x * 0.6, app.GetWindowSize().y * 0.2 });

			}

			{//Vinyls
				GameObject::Sptr button = scene->CreateGameObject("Vinyl Tutorial");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 700 * 0.75, 500 * 0.75 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 700 * 0.75, 500 * 0.75 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexVinylsTutorial);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);


				transform->SetPosition({ app.GetWindowSize().x * 0.6, app.GetWindowSize().y * 0.6 });

			}
		}


		{//HUD
			{//Beat Bar
				GameObject::Sptr button = scene->CreateGameObject("HUD Score Display");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0, 0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 500, 100 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 500, 100 });

				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexScoreDisplay);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;



				transform->SetPosition({ 250, 50 });

			}

			{//Score Text
				GameObject::Sptr button = scene->CreateGameObject("GameOver Score Text");

				RectTransform::Sptr transform = button->Add<RectTransform>();
				transform->SetPosition({ 0,0 });
				transform->SetRotationDeg(0);
				transform->SetSize({ 504, 475 });
				transform->SetMin({ 0, 0 });
				transform->SetMax({ 504, 475 });

				transform->SetPosition({ 450 , 80 });


				GuiPanel::Sptr panel = button->Add<GuiPanel>();
				panel->SetTexture(TexScoreBreakdown);
				panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
				panel->SetBorderRadius(0);
				panel->IsEnabled = false;

				GuiText::Sptr text = button->Add<GuiText>();
				text->SetColor(glm::vec4(0.f));
				text->SetFont(FontVCR);
				text->SetText("0");
				text->IsEnabled = false;

				text->SetTextScale(4.0f);

			}
		*/
			/*
				{//Beat Bar
					GameObject::Sptr button = scene->CreateGameObject("HUD Beat Bar");

					RectTransform::Sptr transform = button->Add<RectTransform>();
					transform->SetPosition({ 0, 0 });
					transform->SetRotationDeg(0);
					transform->SetSize({ 800 * 0.75, 300 * 0.75 });
					transform->SetMin({ 0, 0 });
					transform->SetMax({ 800 * 0.75, 300 * 0.75 });

					GuiPanel::Sptr panel = button->Add<GuiPanel>();
					panel->SetTexture(TexBeatBar);
					panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					panel->SetBorderRadius(0);


					transform->SetPosition({ app.GetWindowSize().x * 0.5, app.GetWindowSize().y * 0.9 });

				}

				{//Beat Tick
					GameObject::Sptr button = scene->CreateGameObject("HUD Beat Tick");

					RectTransform::Sptr transform = button->Add<RectTransform>();
					transform->SetPosition({ 0, 0 });
					transform->SetRotationDeg(0);
					transform->SetSize({ 50 * 0.75, 170 * 0.75 });
					transform->SetMin({ 0, 0 });
					transform->SetMax({ 50 * 0.75, 170 * 0.75 });

					GuiPanel::Sptr panel = button->Add<GuiPanel>();
					panel->SetTexture(TexBeatBarTick);
					panel->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					panel->SetBorderRadius(0);


					transform->SetPosition({ app.GetWindowSize().x * 0.4, app.GetWindowSize().y * 0.9 });

				}
		}
			*/


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


