#include "Level1Scene.h"

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
#include "Graphics/Textures/Texture2D.h"
#include "Graphics/Textures/TextureCube.h"
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
#include "Gameplay/Components/JumpBehaviour.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/MaterialSwapBehaviour.h"
#include "Gameplay/Components/TriggerVolumeEnterBehaviour.h"
#include "Gameplay/Components/SimpleCameraControl.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Gameplay/Components/BeatTimer.h"
#include "Gameplay/Components/MoveThings.h"
#include "Gameplay/Components/SeekBehaviour.h"
#include "Gameplay/Components/RotatingBehaviour.h"
#include "Gameplay/Components/CharacterController.h"
#include "Gameplay/Components/LevelMover.h"
#include "Gameplay/Components/BackgroundMover.h"
#include "Gameplay/Components/EnvironmentMover.h"
#include "Gameplay/Components/ScoreComponent.h"
#include "Gameplay/Components/VinylAnim.h"
#include "Gameplay/Components/ForegroundMover.h"
#include "Gameplay/Components/BuildingAnim.h"
#include "Gameplay/Components/MaterialSwap.h"
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

#include "Application/Application.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Graphics/Textures/Texture3D.h"
#include "Graphics/Textures/Texture1D.h"

//Testing...
#include "SpawnFunctions.h"
#include "Resource Definitions.h"


Level1Scene::Level1Scene() :
	ApplicationLayer()
{
	Name = "Level1";
	Overrides = AppLayerFunctions::OnAppLoad;
}

Level1Scene::~Level1Scene() = default;

void Level1Scene::OnAppLoad(const nlohmann::json & config) {
	_CreateScene();
}



void Level1Scene::_CreateScene()
{
	using namespace Gameplay;
	using namespace Gameplay::Physics;

	Application& app = Application::Get();

	bool loadScene = false;
	// For now we can use a toggle to generate our scene vs load from file
	if (loadScene && std::filesystem::exists("scene.json")) {
		app.LoadScene("scene.json");
	}
	else {
		 Scene::Sptr scene = std::make_shared<Scene>();
		
		 ShaderProgram::Sptr reflectiveShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_environment_reflective.glsl" }
		});
		reflectiveShader->SetDebugName("Reflective");
		
		
		 ShaderProgram::Sptr basicShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_blinn_phong_textured.glsl" }
		});
		basicShader->SetDebugName("Blinn-phong");
		
		
		 ShaderProgram::Sptr specShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/textured_specular.glsl" }
		});
		specShader->SetDebugName("Textured-Specular");
		
		 ShaderProgram::Sptr foliageShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/foliage.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/screendoor_transparency.glsl" }
		});
		foliageShader->SetDebugName("Foliage");
		
		
		 ShaderProgram::Sptr toonShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/toon_shading.glsl" }
		});
		toonShader->SetDebugName("Toon Shader");
		
		
		 ShaderProgram::Sptr displacementShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/displacement_mapping.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_tangentspace_normal_maps.glsl" }
		});
		displacementShader->SetDebugName("Displacement Mapping");
		
		 ShaderProgram::Sptr tangentSpaceMapping = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_tangentspace_normal_maps.glsl" }
		});
		tangentSpaceMapping->SetDebugName("Tangent Space Mapping");
		
		 ShaderProgram::Sptr multiTextureShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/vert_multitextured.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_multitextured.glsl" }
		});
		multiTextureShader->SetDebugName("Multitexturing");
		
		
		 MeshResource::Sptr monkeyMesh = ResourceManager::CreateAsset<MeshResource>("Monkey.obj");
		
		
		 Texture2D::Sptr    boxTexture = ResourceManager::CreateAsset<Texture2D>("textures/box-diffuse.png");
		 Texture2D::Sptr    boxSpec = ResourceManager::CreateAsset<Texture2D>("textures/box-specular.png");
		 Texture2D::Sptr    monkeyTex = ResourceManager::CreateAsset<Texture2D>("textures/monkey-uvMap.png");
		 Texture2D::Sptr    leafTex = ResourceManager::CreateAsset<Texture2D>("textures/leaves.png");
		leafTex->SetMinFilter(MinFilter::Nearest);
		leafTex->SetMagFilter(MagFilter::Nearest);
		
		
		
		 Texture1D::Sptr toonLut = ResourceManager::CreateAsset<Texture1D>("luts/toon-1D.png");
		toonLut->SetWrap(WrapMode::ClampToEdge);
		
		
		 TextureCube::Sptr testCubemap = ResourceManager::CreateAsset<TextureCube>("cubemaps/ocean/ocean.jpg");
		
		 ShaderProgram::Sptr      skyboxShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/skybox_vert.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/skybox_frag.glsl" }
		});
		
		
		 Texture3D::Sptr lut = ResourceManager::CreateAsset<Texture3D>("luts/cool.CUBE");
		 MeshResource::Sptr SmallPlatform = ResourceManager::CreateAsset<MeshResource>("SmallSpeakerPlatformV5.obj");
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
		
		 Texture2D::Sptr StartTex = ResourceManager::CreateAsset<Texture2D>("textures/LStartPlatformTex.png");
		 Texture2D::Sptr SmallTex = ResourceManager::CreateAsset<Texture2D>("textures/DanceFloorTex2.png");
		 Texture2D::Sptr VinylTex = ResourceManager::CreateAsset<Texture2D>("textures/VinylTex.png");
		 Texture2D::Sptr CDTex = ResourceManager::CreateAsset<Texture2D>("textures/CDTex.png");
		 Texture2D::Sptr GemTex = ResourceManager::CreateAsset<Texture2D>("textures/Gem.png");
		 Texture2D::Sptr GemOff = ResourceManager::CreateAsset<Texture2D>("textures/GemOff.png");
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
		 Texture2D::Sptr TexScoreDisplay = ResourceManager::CreateAsset<Texture2D>("textures/GUI/ScoreDisplay.png");
		
		 Font::Sptr FontVCR = ResourceManager::CreateAsset<Font>("fonts/VCR.ttf", 16.f);
		 FontVCR->Bake();
		
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
		
		 Material::Sptr BeatGemMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			BeatGemMaterial->Name = "BeatGem";
			BeatGemMaterial->Set("u_Material.Diffuse", GemTex);
			BeatGemMaterial->Set("u_Material.Shininess", 0.1f);
		}
		
		 Material::Sptr BeatGemOffMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			BeatGemOffMaterial->Name = "BeatGemOff";
			BeatGemOffMaterial->Set("u_Material.Diffuse", GemOff);
			BeatGemOffMaterial->Set("u_Material.Shininess", 0.1f);
		}
		
		
		 Material::Sptr VinylMaterial = ResourceManager::CreateAsset<Material>(toonShader);
		{
			VinylMaterial->Name = "Vinyl";
			VinylMaterial->Set("u_Material.Diffuse", VinylTex);
			VinylMaterial->Set("u_Material.Shininess", 0.1f);
			VinylMaterial->Set("u_Material.Steps", 4);
		}
		
		 Material::Sptr CDMaterial = ResourceManager::CreateAsset<Material>(toonShader);
		{
			CDMaterial->Name = "CD";
			CDMaterial->Set("u_Material.Diffuse", CDTex);
			CDMaterial->Set("u_Material.Shininess", 0.1f);
			CDMaterial->Set("u_Material.Steps", 4);
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

	
		//// Create some lights for our scene
		//scene->Lights.resize(2);
		//scene->Lights[0].Position = glm::vec3(0.0f, 1.0f, 3.0f);
		//scene->Lights[0].Color = glm::vec3(1.0f, 1.0f, 1.0f);
		//scene->Lights[0].Range = 50.0f;
		
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
		scene->Lights[1].Position = glm::vec3(6.840f, 5.610f, 3.0f);
		scene->Lights[1].Color = glm::vec3((1.0f, 0.99f, 0.99f));
		scene->Lights[1].Range = 50.0f;

		// Set up the scene's camera
		GameObject::Sptr camera = scene->CreateGameObject("Main Camera");
		{
			camera->SetPostion(glm::vec3(-1.410, -3.500, 2.450));
			camera->LookAt(glm::vec3(0.0f));
			camera->SetRotation(glm::vec3(-103, 180, -180));

			Camera::Sptr cam = camera->Add<Camera>();
			//cam->SetOrthoEnabled(true);
			//cam->SetOrthoVerticalScale(19.0f);
			//cam->SetFovRadians(105.f);
			//cam->SetNearPlane(0.3);

			// Make sure that the camera is set as the scene's main camera!
			scene->MainCamera = cam;
		}

		//This is a game object built purely to manage game systems i.e. Scene Swaps
		//Level Spawning, Score Counting, and a few miscellaneous systems
		GameObject::Sptr GameManager = scene->CreateGameObject("GameManager");
		{
			//Pos-Rot-Scale Doesn't matter
			RigidBody::Sptr physics = GameManager->Add<RigidBody>(RigidBodyType::Kinematic);
			GameManager->Add<BeatTimer>();
			GameManager->Add<SpawnLoop>();
			//ScoreComponent
			//LevelSpawningComponent
			//Scene Swapper

		}


		//float const distanceFromBlock = 20.f;
		SpawnFunctions spawner;
		// Background and forground vehicles\\
		Give these Parents for Foreground/Background Blocks if we have enough objects to do that with!
		spawner.SpawnBackGroundCar(scene, Car1Mesh, Car1Material, "Car1", glm::vec3(14.870f, 7.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		spawner.SpawnBackGroundCar(scene,SemiTruckMesh, SemiTruckMaterial, "Semi1", glm::vec3(28.870f, 7.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		spawner.SpawnForeGroundCar(scene,Car1Mesh, Car1Material, "Car2", glm::vec3(-9.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		spawner.SpawnForeGroundCar(scene,PickupTruckMesh, PickupTruckMaterial, "Pickup1", glm::vec3(-18.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		spawner.SpawnBackGroundBuilding(scene,KBuilding1Mesh, KBuildingMaterial, "KBuilding1", glm::vec3(-1.0f, 21.880f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.780f, 1.470f, 1.0f));
		spawner.SpawnBackGroundBuilding(scene,KBuilding2Mesh, KBuilding2Material, "KBuilding2", glm::vec3(25.670, 21.880f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.780f, 1.470f, 1.0f));
		spawner.SpawnBackGroundBuilding(scene,KBuilding3Mesh, KBuilding3Material, "KBuilding3", glm::vec3(-30.530f, 21.880f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.780f, 1.470f, 1.0f));
		spawner.SpawnBackGroundBuilding(scene,OvalBuilding, OvalBuildingMaterial, "OvalBuilding", glm::vec3(13.730f, 27.510f, -46.040f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.720f, 3.500f, 1.0f));

		// Tutorial

		spawner.SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-9.820f, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnObj(scene,SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-6.070f, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnObj(scene,SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-3.320f, 5.610f, -2.200f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnObj(scene,SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-0.400f, 5.610f, -4.040f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnObj(scene,SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-2.110f, 5.610f, 5.440f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnWallJump(scene,WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(-0.350f, 5.610f, 3.070f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		spawner.SpawnWallJump(scene,WallJump, WallJumpMaterial, "Wall Jump", glm::vec3(2.430f, 5.610f, 3.930f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.210f, 1.500f));
		spawner.SpawnGem(scene, BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem 4", glm::vec3(2.020f, 5.610f, -1.910f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		spawner.SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(-2.110f, 5.610f, 6.010f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		spawner.SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(6.360f, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnBackGroundCar(scene,Car1Mesh, Car1Material, "Car1", glm::vec3(14.870f, 9.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		spawner.SpawnBackGroundCar(scene,SemiTruckMesh, SemiTruckMaterial, "Semi1", glm::vec3(28.870f, 9.80f, 2.7f), glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		spawner.SpawnForeGroundCar(scene,Car1Mesh, Car1Material, "Car2", glm::vec3(-9.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));
		spawner.SpawnForeGroundCar(scene,PickupTruckMesh, PickupTruckMaterial, "Pickup1", glm::vec3(-18.970f, 0.470f, -1.90f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.250f, 0.250f, 0.250f));


		// 1st Block		
		spawner.SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "StartPlatform", glm::vec3(-9.820f + 22, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnObj(scene,SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-6.070f + 22, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnObj(scene,SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(-2.840f + 22, 5.610f, -4.150f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnObj(scene,SmallPlatform, SmallPlatformMaterial, "Small Platform", glm::vec3(2.760f + 22, 5.610f, -1.770f), glm::vec3(180.0f, 0.0f, 180.0f), glm::vec3(0.350f, 0.350f, 0.350f));
		spawner.SpawnGem(scene,BeatGem, BeatGemMaterial, BeatGemOffMaterial, "BeatGem 4", glm::vec3(0.120f + 22, 5.610f, -3.160f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.500f, 0.500f, 0.500f));
		spawner.SpawnCollectable(scene, Vinyl, VinylMaterial, "Vinyl", glm::vec3(5.640f + 22, 5.610f, 0.080f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		spawner.SpawnStartPlat(scene, StartPlatform, StartPlatformMaterial, "EndPlatform", glm::vec3(6.360f + 22, 5.610f, -9.10f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.350f, 0.350f, 0.350f));

		// CDs for Block 1
		spawner.SpawnCD(scene,CD, CDMaterial, "CD", glm::vec3(-6.030f + 22, 5.610f, -3.220f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		spawner.SpawnCD(scene,CD, CDMaterial, "CD", glm::vec3(-2.710f + 22, 5.610f, -3.190f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		spawner.SpawnCD(scene,CD, CDMaterial, "CD", glm::vec3(0.170f + 22, 5.610f, -2.380f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));
		spawner.SpawnCD(scene,CD, CDMaterial, "CD", glm::vec3(2.640f + 22, 5.610f, -0.770f), glm::vec3(90.000f, 0.0f, 90.000f), glm::vec3(1.000f, 1.000f, 1.000f));



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

			TriggerVolume::Sptr volume = character->Add<TriggerVolume>();
			volume->SetFlags(TriggerTypeFlags::Statics | TriggerTypeFlags::Kinematics);

			BoxCollider::Sptr collider = BoxCollider::Create(glm::vec3(0.3f, 0.3f, 0.3f));
			collider->SetPosition(glm::vec3(0.f, 0.25f, 0.f));
			volume->AddCollider(collider);

			//MorphMeshRenderer::Sptr morph1 = character->Add<MorphMeshRenderer>();
			//morph1->SetMorphMeshRenderer(DiscoBotMesh1, CharacterMaterial);
			//Morphanimator::Sptr RunAnim = character->Add<Morphanimator>();

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


			//RunAnim->SetInitial();
			//RunAnim->SetFrameTime(0.1f);
			//RunAnim->SetFrames(KeyFrames);
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
