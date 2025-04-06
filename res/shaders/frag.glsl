#version 450

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(binding = 1) uniform sampler2D texSampler;

layout(push_constant) uniform PushConstantData {
    vec3 viewPos;
    mat4 modelMatrix;
    Light light;
};

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in float fragShininess;

layout(location = 0) out vec4 outColor;

vec3 texColor = texture(texSampler, fragTexCoord).rgb;

void main() {
    // ambient
    vec3 ambient = light.ambient * texColor;
  	
    // diffuse 
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fragShininess);
    vec3 specular = light.specular * spec * texColor;
        
    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, 1.0);
    // outColor = vec4(norm * 0.5 + 0.5, 1.0);
}