#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;   // 光源位置
uniform vec3 viewPos;    // 相机/观察者位置
uniform vec3 lightColor; // 光源颜色

void main()
{    
    // 1. 环境光 (Ambient)
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // 2. 漫反射 (Diffuse)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // 3. 镜面反射 (Specular)
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 是反光度(Shininess)
    vec3 specular = specularStrength * spec * lightColor;  
        
    // 组合结果
    vec3 objectColor = texture(texture_diffuse1, TexCoords).rgb;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}