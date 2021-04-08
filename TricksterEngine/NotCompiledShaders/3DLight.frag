#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 lightColor = vec3(1.0,1.0,1.0);
    vec3 lightPos = vec3(0.0, 3.0, 0.0);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
	vec3 norm = normalize(fragNormal);
    
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(fragNormal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;




    vec4 result = vec4(ambient + diffuse, 1.0) * texture(texSampler, fragTexCoord);
    outColor = result;
}