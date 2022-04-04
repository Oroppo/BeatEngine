#include "ColorCorrectionEffect.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

ColorCorrectionEffect::ColorCorrectionEffect() :
	ColorCorrectionEffect(true) { }

ColorCorrectionEffect::ColorCorrectionEffect(bool defaultLut) :
	PostProcessingLayer::Effect(),
	_shader(nullptr),
	_strength(1.0f),
	Lut(nullptr),
	checked(false),
	Lut2(nullptr)
{
	Name = "Color Correction";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/color_correction.glsl" }
	});

	if (defaultLut) {
		Lut = ResourceManager::CreateAsset<Texture3D>("luts/cool.cube");
		Lut2 = ResourceManager::CreateAsset<Texture3D>("luts/LessSaturationGreenRed.cube");
	}
}

ColorCorrectionEffect::~ColorCorrectionEffect() = default;

void ColorCorrectionEffect::Apply(const Framebuffer::Sptr& gBuffer)
{
	_shader->Bind();
	if (checked == false)
	{
		Lut->Bind(1);
	}
	else
	{
		Lut2->Bind(1);
	}
	_shader->SetUniform("u_Strength", _strength);
}



void ColorCorrectionEffect::RenderImGui()
{
	ImGui::Checkbox("Swap LUT", &checked);
	LABEL_LEFT(ImGui::LabelText, "LUT", Lut ? Lut->GetDebugName().c_str() : "none");
	LABEL_LEFT(ImGui::LabelText, "LUT2", Lut2 ? Lut2->GetDebugName().c_str() : "none");
	LABEL_LEFT(ImGui::SliderFloat, "Strength", &_strength, 0, 1);
}

ColorCorrectionEffect::Sptr ColorCorrectionEffect::FromJson(const nlohmann::json& data)
{
	ColorCorrectionEffect::Sptr result = std::make_shared<ColorCorrectionEffect>(false);
	result->Enabled = JsonGet(data, "enabled", true);
	result->_strength = JsonGet(data, "strength", result->_strength);
	result->Lut = ResourceManager::Get<Texture3D>(Guid(data["lut"].get<std::string>()));
	return result;
}

nlohmann::json ColorCorrectionEffect::ToJson() const
{
	return {
		{ "enabled", Enabled },
		{ "lut", Lut != nullptr ? Lut->GetGUID().str() : "null" }, 
		{ "strength", _strength }
	};
}
