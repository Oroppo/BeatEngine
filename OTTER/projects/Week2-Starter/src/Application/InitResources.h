#pragma once
#include "Gameplay/MeshResource.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Gameplay/Material.h"
#include "Graphics/Font.h"
#include "Graphics/Textures/Texture2D.h"
#include "Graphics/Textures/TextureCube.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Light.h"
#include "Gameplay/Animation/MorphMeshRenderer.h"
#include "Gameplay/Animation/MorphAnimator.h"
#include "Utils/MeshBuilder.h"

//Meshes
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


//Fonts
Font::Sptr FontVCR = ResourceManager::CreateAsset<Font>("fonts/VCR.ttf", 16.f);

//Shaders
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
Texture2D::Sptr    boxTexture = ResourceManager::CreateAsset<Texture2D>("textures/box-diffuse.png");
Texture2D::Sptr    boxSpec = ResourceManager::CreateAsset<Texture2D>("textures/box-specular.png");
Texture2D::Sptr    monkeyTex = ResourceManager::CreateAsset<Texture2D>("textures/monkey-uvMap.png");
Texture2D::Sptr    leafTex = ResourceManager::CreateAsset<Texture2D>("textures/leaves.png");

// Create our materials
Material::Sptr StartPlatformMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr UIMat = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr SmallPlatformMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr WallJumpMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr BeatGemMaterial = ResourceManager::CreateAsset<Material>(reflectiveShader);
Material::Sptr BeatGemOffMaterial = ResourceManager::CreateAsset<Material>(reflectiveShader);
Material::Sptr VinylMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr CDMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr CharacterMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr DiscoBallMaterial = ResourceManager::CreateAsset<Material>(reflectiveShader);
Material::Sptr LoseScreenMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr Car1Material = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr SemiTruckMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr PickupTruckMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr BuildingMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr KBuildingMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr KBuilding2Material = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr KBuilding3Material = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr SmallWallJumpMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr SuperSmallWallJumpMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr PianoMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr HalfCirclePlatMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr StairsRightMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr StairsLeftMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr SpeakerMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr SquarePlatMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr FloatingLightMaterial = ResourceManager::CreateAsset<Material>(basicShader);
Material::Sptr OvalBuildingMaterial = ResourceManager::CreateAsset<Material>(basicShader);
