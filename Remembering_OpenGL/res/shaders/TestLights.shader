#shader vertex
#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 vertexNormal;

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

//struct Light {
//    vec3 position;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};


// like the sun
struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


// like a lightbulb
struct PointLight
{
    vec3 position;

    float constant;
    float linTerm;
    float quadTerm;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff; // phi value
    float outerCutOff; // gamma value

    float constant;
    float linTerm;
    float quadTerm;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 Normal;
in vec3 FragPos;
in vec2 v_TexCoord;

#define N_POINT_LIGHTS 4


uniform vec3 u_viewPos;
uniform DirectionalLight dLight;
uniform PointLight pLight[N_POINT_LIGHTS];
uniform SpotLight sLight;

////uniform vec3 u_lightPos;
//uniform vec3 u_viewPos;
//uniform DirectionalLight dLight;
//uniform PointLight pLight[4];
//uniform SpotLight sLight;


uniform Material material;


vec3 CalculateDirectional(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 CalculateSpot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_viewPos - FragPos);

    vec3 result = CalculateDirectional(dLight, norm, viewDir);

    for (int i = 0; i < N_POINT_LIGHTS; i++)
        result += CalculatePoint(pLight[i], norm, FragPos, viewDir);

    result += CalculateSpot(sLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0f);
}



vec3 CalculateDirectional(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-light.direction);

    //Calculate diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Get specular
    vec3 reflectionDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectionDir), 0.0), material.shininess);

    //create all light components
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

    return (ambient + diffuse + specular);
}


vec3 CalculatePoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.position - fragPos);

    //Calculate diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Get specular
    vec3 reflectionDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectionDir), 0.0), material.shininess);

    //Compute attenuation
    float d = length(light.position - fragPos);
    float atten = 1.0f / (light.constant + (light.linTerm * d) + (light.quadTerm * d * d));

    //create all light components
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

    return ((ambient + diffuse + specular) * atten);
}


vec3 CalculateSpot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    // direction of the light ray. We have light.direction for where spot is aiming
    vec3 lightDir = normalize(light.position - fragPos);

    //Calculate diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //Get specular
    vec3 reflectionDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectionDir), 0.0), material.shininess);

    //Compute attenuation
    float d = length(light.position - fragPos);
    float atten = 1.0f / (light.constant + (light.linTerm * d) + (light.quadTerm * d * d));

    //Spotlight intensity
    float costheta = dot(lightDir, normalize(-light.direction));
    float cosepsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((costheta - light.outerCutOff) / cosepsilon, 0.0, 1.0);

    //create all light components
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

    return ((ambient + diffuse + specular) * atten * intensity);
}