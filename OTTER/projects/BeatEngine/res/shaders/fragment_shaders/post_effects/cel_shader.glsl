#version 430

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

uniform layout(binding = 0) sampler2D s_Image;
uniform layout(binding = 1) sampler1D s_ToonTerm;

uniform float u_Discard;

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting

void main() {	
	// Get albedo from the material
	vec4 albedoColor = texture(s_Image, inUV);

    // Using a LUT to allow artists to tweak toon shading settings
    albedoColor.r = texture(s_ToonTerm, albedoColor.r).r;
    albedoColor.g = texture(s_ToonTerm, albedoColor.g).g;
    albedoColor.b = texture(s_ToonTerm, albedoColor.b).b;
	albedoColor.a = 1.0;

	//if (albedoColor.a < u_Discard) {
	//	discard;
	//}

	outColor = albedoColor;

	//// We can use another texture to store things like our lighting settings
	//vec4 lightingParams = texture(s_Image, inUV);
	//
	//// Discarding fragments who's alpha is below the material's threshold
	//if (albedoColor.a < u_Discard) {
	//	discard;
	//}
	//
	//// Extract albedo from material, and store shininess
	//albedo_specPower = vec4(albedoColor.rgb, lightingParams.x);
	//
	//// Normalize our input normal
    //// Read our tangent from the map, and convert from the [0,1] range to [-1,1] range
    ////vec3 normal = texture(u_Material.NormalMap, inUV).rgb;
    ////normal = normal * 2.0 - 1.0;
	////
    ////// Here we apply the TBN matrix to transform the normal from tangent space to view space
    ////normal = normalize(inTBN * normal);
	////
	////// Map [-1, 1] to [0, 1]
	////normal = clamp((normal + 1) / 2.0, 0, 1);
	////normal_metallic = vec4(normal, lightingParams.y);
	//
	//// Extract emissive from the material
	//emissive = texture(u_Material.EmissiveMap, inUV);
	//
	//view_pos = inViewPos;
}