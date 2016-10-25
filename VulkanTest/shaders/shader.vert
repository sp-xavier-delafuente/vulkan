#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	vec4 lightPos;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 outLightVec;
layout(location = 3) out vec3 outViewVec;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	fragTexCoord = inTexCoord;
	vec4 pos = ubo.model * vec4(inPosition, 1.0);
	vec3 lPos = mat3(ubo.model) * ubo.lightPos.xyz;
	outLightVec = lPos - pos.xyz;
	outNormal = mat3(ubo.model) * inNormal;
	outViewVec = -pos.xyz;
}
