#shader vertex
#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 1) in vec3 vertexNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 v_TexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    FragPos = vec3(u_model * vec4(vertexPosition, 1.0));
    Normal = mat3(transpose(inverse(u_model))) * vertexNormal;  
    v_TexCoord = texCoord;
    
    gl_Position = u_projection * u_view * vec4(FragPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;


struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 lightPosition;
    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 v_TexCoord;


uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    FragColor = vec4(result, 1.0);
}