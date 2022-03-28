#version 420

#include "../fragments/vs_common.glsl"
layout(location = 6) in vec3 inPos2;
layout(location = 7) in vec3 inNorm2;
layout(location = 8) in vec3 inColor2;

//a uniform so that we can perform LERP
uniform float u_t;

void main() {

	// lerp logic

	//for smooth interpolation

	//float alpha = smoothstep(0.0, 1.0, u_t);
	//vec3 newPos = (inPosition*alpha) + (inPos2* 1-alpha);

	//regular interpolation
	vec3 newPos = inPosition + (inPos2 - inPosition) * u_t;

	//NewPos cast as a vec4 for mat4 multiplication
	gl_Position = u_ModelViewProjection * vec4(newPos,1);

	outWorldPos = gl_Position.xyz;

	vec3 newNormal = mix(inNormal, inNorm2, u_t);

	// Normals
	outNormal = mat3(u_NormalMatrix) * newNormal;

	// Pass our UV coords to the fragment shader
	outUV = inUV;

	///////////
	outColor = inColor;

	
}