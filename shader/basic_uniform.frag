#version 460

//current position from last stage
in vec3 crntPosFrag;
//current normal from last stage
in vec3 crntNormFrag;

uniform struct LightInfo{
    vec4 Position;
    vec3 Ld;
    vec3 La;
    vec3 Ls;
} Lights[3];

uniform struct MaterialInfo{
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;
} Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform vec3 camPos;

//get position and normal to camera space
void getCamSpaceValues(out vec3 normal, out vec4 position){
    normal = normalize(NormalMatrix * crntNormFrag);

    position = ModelViewMatrix * vec4(crntPosFrag, 1.0);
}

layout (location = 0) out vec4 FragColor;

vec3 blingPhongModel(int light, vec3 position, vec3 normal){
    //Ambient
    vec3 ambient = Lights[light].La*Material.Ka;

    vec3 lightDir = normalize(vec3(Lights[light].Position.xyz - position));
    vec3 viewDir = normalize(camPos - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float sDotn = max(dot(halfwayDir, normal), 0.0);
    vec3 diffuse = Lights[light].Ld*Material.Kd*sDotn;

    vec3 specular = vec3(0.0);
    if (sDotn>0.0){
        vec3 reflectDir = reflect(-lightDir,normal);
        float specAmount = pow(max(dot(reflectDir, normal), 0.0), Material.Shininess);
        specular = specAmount * Material.Ks * Lights[light].Ls;
    }

    return ambient + diffuse + specular;
}

void main() {

    //Diffuse
    vec3 norm;
    vec4 position;
    getCamSpaceValues(norm, position);

    vec3 Colour = vec3(0.0);
    for (int i=0; i<3; i++){
        Colour += blingPhongModel(i, position.xyz, norm);
    }

    FragColor = vec4(Colour, 1.0);
}
