#version 460

//current position from last stage
in vec3 crntPosFrag;
//current normal from last stage
in vec3 crntNormFrag;
// tex coord from last stage
in vec2 TexCoord;
// tangents from last stage
in vec4 vertTangent;

layout(binding=2) uniform sampler2D textureTex;
layout(binding=3) uniform sampler2D normalTex;

layout (location = 0) out vec4 FragColor;

uniform struct FogInfo{
    float MaxDist;
    float MinDist;
    vec3 Colour;
}Fog;

uniform struct LightInfo{
    vec4 Position;
    vec3 Ld;
    vec3 La;
    vec3 Ls;
} Lights[3];

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform vec3 camPos;

// for toon shading
const int levels = 4;
const float scaleFactor = 1.0/levels;

vec3 blingPhongModel(int light, vec3 position, vec3 normal, mat3 objectLocal){
    //Ambient
    vec3 ambient = Lights[light].La;
    
    vec3 lightDir = normalize(objectLocal* vec3(Lights[light].Position.xyz - position));
    vec3 viewDir = normalize(camPos - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float sDotn = max(dot(halfwayDir, normal), 0.0);
    vec3 diffuse = Lights[light].Ld*floor(sDotn*levels)*scaleFactor;

    /*vec3 specular = vec3(0.0);
    if (sDotn>0.0){
        vec3 reflectDir = reflect(-lightDir,normal);
        float specAmount = pow(max(dot(reflectDir, normal), 0.0), Material.Shininess);
        specular = specAmount * Lights[light].Ls;
    }*/

    return ambient + diffuse;// + specular;
}

void calcNormalMapValues(out vec4 position, out mat3 toObjectLocal){
    //Transform normal and tangent to eye space
    vec3 norm = normalize(NormalMatrix * crntNormFrag);
    vec3 tang = normalize(NormalMatrix * vec3(vertTangent));

    // Compute binormal
    vec3 binormal = normalize(cross(norm,tang))*vertTangent.w;

    //Transformation matrix
    toObjectLocal = mat3(
        tang.x, binormal.x, norm.x,
        tang.y, binormal.y, norm.y,
        tang.z, binormal.z, norm.z
    );

    position = ModelViewMatrix * vec4(crntPosFrag, 1.0);
}

void main() {

    //Diffuse
    vec4 position;
    mat3 objectLocal;
    calcNormalMapValues(position, objectLocal);

    vec3 norm = texture(normalTex, TexCoord).xyz;
    norm.xy = 2.0*norm.xy - 1.0;

    float dist = abs(position.z);

    float fogFactor = (Fog.MaxDist - dist)/(Fog.MaxDist - Fog.MinDist);

    fogFactor = clamp(fogFactor,0.0,1.0); 

    vec3 phongColour = vec3(0.0);
    for (int i=0; i<3; i++){
        phongColour += blingPhongModel(i, position.xyz, norm, objectLocal);
    }

    vec3 Colour = mix(Fog.Colour, phongColour, fogFactor);
        
    Colour *= texture(textureTex, TexCoord).xyz;

    FragColor = vec4(Colour, 1.0);
}
