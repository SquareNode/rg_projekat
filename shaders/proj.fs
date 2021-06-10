#version 330 core

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
}; 

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 nor;
in vec3 fragPos;

uniform vec3 viewPos;
uniform Material mat;
uniform Light light;
uniform SpotLight spotLight;
uniform DirLight dirL;

uniform bool lampa;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{	
	vec3 ambient = light.ambient * mat.ambient;
	
	vec3 n = normalize(nor);
	vec3 light_dir = normalize(light.position - fragPos);
	
	float diff = max(dot(n, light_dir), 0.0f);
	vec3 diffuse = light.diffuse * (diff * mat.diffuse);
	
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-light_dir, n);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	vec3 specular = light.specular * (spec * mat.specular);

	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
	light.quadratic * (distance * distance));    
	
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;   
	
	vec3 res = ambient + diffuse + specular;
	res += CalcDirLight(dirL, n, viewDir);
	if (lampa) {
		res += CalcSpotLight(spotLight, n, fragPos, viewDir);
	}
    FragColor = vec4(res, 1.0);
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * mat.diffuse;
    vec3 diffuse = light.diffuse * diff * mat.diffuse;
    vec3 specular = light.specular * spec * mat.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    // combine results
    vec3 ambient = light.ambient * mat.diffuse;
    vec3 diffuse = light.diffuse * (diff * mat.diffuse);
    vec3 specular = light.specular * (spec * mat.specular);
    return (ambient + diffuse + specular);
}
