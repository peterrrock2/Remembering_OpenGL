#shader vertex
#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertexNormal;

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
    sampler2D specular;
    float shininess;
};

//struct Material {
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//    float shininess;
//};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 v_TexCoord;


//uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform Light light;
uniform Material material;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, v_TexCoord).rgb;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, v_TexCoord).rgb;

    // specular
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, v_TexCoord).rgb;
    //vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}