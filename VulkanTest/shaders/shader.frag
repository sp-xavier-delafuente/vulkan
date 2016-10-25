#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 inLightVec;
layout(location = 3) in vec3 inViewVec;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 N = normalize(inNormal);
	vec3 L = normalize(inLightVec);
	vec3 V = normalize(inViewVec);
	vec3 R = reflect(-L, N);
    vec4 color = texture(texSampler, fragTexCoord);
	vec3 diffuse = max(dot(N, L), 0.0) * color.xyz;
	vec3 ambient = vec3(0.1, 0.1, 0.1) * color.rgb;
	outColor = vec4(diffuse * color.rgb + ambient, 1.0);		
}
