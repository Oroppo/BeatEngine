#pragma once
#include "Gameplay/GameObject.h"
#include "Gameplay/MeshResource.h"
#include "Graphics/Textures/Texture1D.h"
#include "Graphics/Textures/Texture2D.h"
#include "Graphics/Textures/Texture3D.h"
#include "Graphics/Textures/TextureCube.h"
#include "Graphics/Font.h"
#include "Gameplay/Material.h"


//Bunch of external Headers for Global Namespace Variables, DO NOT redefine these during the programs runtime.
using namespace Gameplay;

//Shaders
extern ShaderProgram::Sptr reflectiveShader;
//reflectiveShader->SetDebugName("Reflective");

// This shader handles our basic materials without reflections (cause they expensive)
 extern ShaderProgram::Sptr basicShader;
//basicShader->SetDebugName("Blinn-phong");

// This shader handles our basic materials without reflections (cause they expensive)
 extern  ShaderProgram::Sptr specShader;
//specShader->SetDebugName("Textured-Specular");

// This shader handles our foliage vertex shader example
 extern ShaderProgram::Sptr foliageShader;
//foliageShader->SetDebugName("Foliage");

// This shader handles our cel shading example
 extern  ShaderProgram::Sptr toonShader;
//toonShader->SetDebugName("Toon Shader");

// This shader handles our displacement mapping example
 extern  ShaderProgram::Sptr displacementShader;
//displacementShader->SetDebugName("Displacement Mapping");

// This shader handles our tangent space normal mapping
 extern ShaderProgram::Sptr tangentSpaceMapping;
 extern ShaderProgram::Sptr multiTextureShader;


 extern Texture2D::Sptr				 boxTexture;
 extern Texture2D::Sptr				    boxSpec;
 extern Texture2D::Sptr				  monkeyTex;
 extern Texture2D::Sptr				    leafTex;
 extern Texture1D::Sptr					toonLut;
 extern TextureCube::Sptr			testCubemap;
 extern ShaderProgram::Sptr        skyboxShader;

 extern MeshResource::Sptr monkeyMesh;
 extern Texture3D::Sptr lut;
 extern MeshResource::Sptr SmallPlatform;
 extern MeshResource::Sptr WallJump;
 extern MeshResource::Sptr BeatGem;
 extern MeshResource::Sptr Vinyl;
 extern MeshResource::Sptr CD;
 extern MeshResource::Sptr Building;
 extern MeshResource::Sptr KBuilding1Mesh;
 extern MeshResource::Sptr KBuilding2Mesh;
 extern MeshResource::Sptr KBuilding3Mesh;
 extern MeshResource::Sptr OvalBuilding;
 extern MeshResource::Sptr CharacterMesh;
 extern MeshResource::Sptr DiscoBallMesh;
 extern MeshResource::Sptr StartPlatform;
 extern MeshResource::Sptr Car1Mesh;
 extern MeshResource::Sptr SemiTruckMesh;
 extern MeshResource::Sptr PickupTruckMesh;
 extern MeshResource::Sptr SmallWallJump;
 extern MeshResource::Sptr SuperSmallWallJump;
 extern MeshResource::Sptr FallingPlat;
 extern MeshResource::Sptr HalfCirclePlat;
 extern MeshResource::Sptr StairsRight;
 extern MeshResource::Sptr StairsLeft;
 extern MeshResource::Sptr Speaker;
 extern MeshResource::Sptr SquarePlat;
 extern MeshResource::Sptr FloatingLight;
 extern MeshResource::Sptr DiscoBotMesh1;
 extern MeshResource::Sptr DiscoBotMesh2;
 extern MeshResource::Sptr DiscoBotMesh3;
 extern MeshResource::Sptr DiscoBotMesh4;
 extern MeshResource::Sptr DiscoBotMesh5;
 extern MeshResource::Sptr DiscoBotMesh6;
 extern MeshResource::Sptr DiscoBotMesh7;
 extern MeshResource::Sptr DiscoBotMesh8;
 extern MeshResource::Sptr DiscoBotMesh9;
 extern MeshResource::Sptr BotJump1;
 extern MeshResource::Sptr BotJump2;
 extern MeshResource::Sptr BotJump3;
 extern MeshResource::Sptr BotJump4;
 extern MeshResource::Sptr BotJump5;
 extern MeshResource::Sptr BotJump6;
 extern MeshResource::Sptr BotJump7;

 extern Texture2D::Sptr StartTex;
 extern Texture2D::Sptr SmallTex;
 extern Texture2D::Sptr VinylTex;
 extern Texture2D::Sptr CDTex;
 extern Texture2D::Sptr GemTex;
 extern Texture2D::Sptr GemOff;
 extern Texture2D::Sptr CharacterTex;
 extern Texture2D::Sptr LoseScreenTex;
 extern Texture2D::Sptr SmallWallJumpTex;
 extern Texture2D::Sptr SuperSmallWallJumpTex;
 extern Texture2D::Sptr WallJumpTex;
 extern Texture2D::Sptr Car1Tex;
 extern Texture2D::Sptr SemiTruckTex;
 extern Texture2D::Sptr PickupTruckTex;
 extern Texture2D::Sptr KBuilding1Tex;
 extern Texture2D::Sptr KBuilding2Tex;
 extern Texture2D::Sptr KBuilding3Tex;
 extern Texture2D::Sptr BuildingTex;
 extern Texture2D::Sptr OvalBuildingTex;
 extern Texture2D::Sptr DiscoBallTex;
 extern Texture2D::Sptr FallingPlatTex;
 extern Texture2D::Sptr HalfCirclePlatTex;
 extern Texture2D::Sptr TexBeatLogo;
 extern Texture2D::Sptr TexPlayButton;
 extern Texture2D::Sptr TexOptionsButton;
 extern Texture2D::Sptr TexMusicButton;
 extern Texture2D::Sptr TexCreditsButton;
 extern Texture2D::Sptr TexQuitButton;
 extern Texture2D::Sptr TexResumeButton;
 extern Texture2D::Sptr TexResyncButton;
 extern Texture2D::Sptr TexContinueButton;
 extern Texture2D::Sptr TexPauseMenu;
 extern Texture2D::Sptr TexDimmedBG;
 extern Texture2D::Sptr TexScoreBreakdown;
 extern Texture2D::Sptr TexGameOverText;
 extern Texture2D::Sptr TexMovementTutorial;
 extern Texture2D::Sptr TexWallJumpTutorial;
 extern Texture2D::Sptr TexBeatGemTutorial;
 extern Texture2D::Sptr TexVinylsTutorial;
 extern Texture2D::Sptr StairsRightTex;
 extern Texture2D::Sptr StairsLeftTex;
 extern Texture2D::Sptr SpeakerTex;
 extern Texture2D::Sptr SquarePlatTex;
 extern Texture2D::Sptr FloatingLightTex;
 extern Texture2D::Sptr TexBeatBar;
 extern Texture2D::Sptr TexBeatBarTick;
 extern Texture2D::Sptr TexScoreDisplay;

 extern Font::Sptr FontVCR;
//FontVCR->Bake();

//Create our materials
extern Material::Sptr StartPlatformMaterial;
extern Material::Sptr UIMat;
extern Material::Sptr SmallPlatformMaterial;
extern Material::Sptr WallJumpMaterial;
extern Material::Sptr BeatGemMaterial;
extern Material::Sptr BeatGemOffMaterial;
extern Material::Sptr VinylMaterial;
extern Material::Sptr CDMaterial ;
extern Material::Sptr CharacterMaterial ;
extern Material::Sptr DiscoBallMaterial ;
extern Material::Sptr LoseScreenMaterial;
extern Material::Sptr Car1Material;
extern Material::Sptr SemiTruckMaterial;
extern Material::Sptr PickupTruckMaterial ;
extern Material::Sptr BuildingMaterial ;
extern Material::Sptr KBuildingMaterial ;
extern Material::Sptr KBuilding2Material;
extern Material::Sptr KBuilding3Material;
extern Material::Sptr SmallWallJumpMaterial;
extern Material::Sptr SuperSmallWallJumpMaterial;
extern Material::Sptr PianoMaterial;
extern Material::Sptr HalfCirclePlatMaterial;
extern Material::Sptr StairsRightMaterial;
extern Material::Sptr StairsLeftMaterial;
extern Material::Sptr SpeakerMaterial;
extern Material::Sptr SquarePlatMaterial;
extern Material::Sptr FloatingLightMaterial;
extern Material::Sptr OvalBuildingMaterial;