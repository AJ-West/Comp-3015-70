#version 460

// for show faces
//flat in vec3 LightIntensity;

//for smooth shading
vec3 LightIntensity;

//current position from last stage
in vec3 crntPosFrag;
//current normal from last stage
in vec3 crntNormFrag;

/*uniform struct LightInfo{
    vec4 Position;
    vec3 Ld;
    vec3 La;
    vec3 Ls;
}Light;

uniform struct MaterialInfo{
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    vec3 Shininess;
}Material;*/

uniform vec4 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform vec3 camPos;

//get position and normal to camera space
void getCamSpaceValues(out vec3 normal, out vec4 position){
    normal = normalize(NormalMatrix * crntNormFrag);

    position = ModelViewMatrix * vec4(crntPosFrag, 1.0);
}

layout (location = 0) out vec4 FragColor;

void main() {    
    //PHONG
    //Ambient
    float ambient = 1.0f;

    //Diffuse
    vec3 norm;
    vec4 position;
    getCamSpaceValues(norm, position);

    vec3 lightDir = normalize(vec3(LightPosition - position));
    vec3 viewDir = normalize(camPos - crntPosFrag);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 diffuse = Ld*Kd* max(dot(halfwayDir, norm), 0.0);
    

    //Specular
    float specularLight = 1.5f;
    vec3 reflectDir = reflect(-lightDir,norm);
    float specAmount = pow(max(dot(reflectDir, norm), 0.0f), 2);
    float specular = specAmount * specularLight;

    LightIntensity = ambient * diffuse * specular;

    FragColor = vec4(LightIntensity, 1.0);
}
