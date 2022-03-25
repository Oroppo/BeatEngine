#version 430

#include "../fragments/fs_common_inputs.glsl"

// We output a single color to the color buffer
layout(location = 0) out vec4 frag_color;

////////////////////////////////////////////////////////////////
/////////////// Instance Level Uniforms ////////////////////////
////////////////////////////////////////////////////////////////

// Represents a collection of attributes that would define a material
// For instance, you can think of this like material settings in 
// Unity
struct Material {
	sampler2D	Diffuse;
	float		Shininess;
	vec3		ColorOffset;
	float		Focus;
};
// Create a uniform for the material
uniform Material u_Material;

////////////////////////////////////////////////////////////////
///////////// Application Level Uniforms ///////////////////////
////////////////////////////////////////////////////////////////

#include "../fragments/multiple_point_lights.glsl"

////////////////////////////////////////////////////////////////
/////////////// Frame Level Uniforms ///////////////////////////
////////////////////////////////////////////////////////////////
#include "../fragments/color_correction.glsl"

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	// Normalize our input normal
	vec3 normal = normalize(inNormal);

	// Use the lighting calculation that we included from our partial file
	vec3 lightAccumulation = CalcAllLightContribution(inWorldPos, normal, u_CamPos.xyz, u_Material.Shininess, u_toggleKeys);

	// Get the albedo from the diffuse / albedo map
	vec4 textureColor = texture(u_Material.Diffuse, inUV);
	// combine for the final result
	vec3 result = lightAccumulation * inColor * textureColor.rgb;

	//float ChromaticAberration = u_Material.Focus / 10.0 + 8.0;
	float ChromaticAberration = 800 / 10.0 + 8.0;

	vec2 coords = (inUV - 0.5) * 2.0;
    float coordDot = dot (coords, coords);
	vec2 texel = 1.0 / u_ScreenSize;
    
    vec2 precompute = ChromaticAberration * coordDot * coords;
    vec2 uvR = inUV - texel.xy * precompute;
    vec2 uvB = inUV + texel.xy * precompute;

	vec3 chromaticColor;
	chromaticColor.r = texture(u_Material.Diffuse, uvR).r;
    chromaticColor.g = texture(u_Material.Diffuse, inUV).g;
    chromaticColor.b = texture(u_Material.Diffuse, uvB).b;


	frag_color = vec4((ColorCorrect(result) * chromaticColor) + chromaticColor, textureColor.a);
}