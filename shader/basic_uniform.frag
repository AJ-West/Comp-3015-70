#version 460

//current position from last stage
in vec3 crntPosFrag;
//current normal from last stage
in vec3 crntNormFrag;
// tex coord from last stage
in vec2 TexCoord;
// tangents from last stage
in vec4 vertTangent;

layout(binding=0) uniform sampler2D HDRTex;
layout(binding=1) uniform sampler2D metalTex;
layout(binding=2) uniform sampler2D metalNormal;
layout(binding=3) uniform sampler2D rustTex;
layout(binding=4) uniform sampler2D rustNormal;
layout(binding=5) uniform sampler2D woodTex;
layout(binding=6) uniform sampler2D woodNormal;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 HDRColor;

//for HDR
uniform int Pass; // Pass number
uniform float AveLum;

//XYZ/RGB conversion matrices from http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
uniform mat3 rgb2xyz = mat3(
0.4124564, 0.2126729, 0.0193339,
0.3575761, 0.7151522, 0.1191920,
0.1804375, 0.0721750, 0.9503041
);

uniform mat3 xyz2rgb = mat3(
3.2404542, -0.9692660, 0.0556434,
-1.5371385, 1.8760108, -0.2040259 ,
-0.4985314, 0.0415560, 1.0572252
);

uniform float Exposure = 0.1;
uniform float White = 0.928;
uniform bool DoToneMap = true;

//HDR end

uniform struct FogInfo{
    float MaxDist;
    float MinDist;
    vec3 Colour;
}Fog;
float fogFactor = 0.0f;

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

// for toon shading
const int levels = 10;
const float scaleFactor = 1.0/levels;

uniform bool arrow = false;

//get position and normal to camera space
void getCamSpaceValues(out vec3 normal, out vec4 position){
    normal = normalize(NormalMatrix * crntNormFrag);

    position = ModelViewMatrix * vec4(crntPosFrag, 1.0);
}

vec3 blingPhongModelNormal(int light, vec3 position, vec3 normal, mat3 objectLocal){
    //Ambient
    vec3 ambient = Lights[light].La;
    
    vec3 lightDir = normalize(objectLocal* vec3(Lights[light].Position.xyz - position));
    vec3 viewDir = normalize(camPos - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float sDotn = max(dot(halfwayDir, normal), 0.0);
    vec3 diffuse = Lights[light].Ld*floor(sDotn*levels)*scaleFactor;
    //vec3 diffuse = Lights[light].Ld*sDotn;

    vec3 specular = vec3(0.0);
    if (sDotn>0.0){
        vec3 reflectDir = reflect(-lightDir,normal);
        float specAmount = pow(max(dot(reflectDir, normal), 0.0), Material.Shininess);
        specular = specAmount * Lights[light].Ls;
    }

    return ambient + diffuse + specular;
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

void calcFog(in vec4 position){
    float dist = abs(position.z);

    fogFactor = (Fog.MaxDist - dist)/(Fog.MaxDist - Fog.MinDist);

    fogFactor = clamp(fogFactor,0.0,1.0); 
}

void pass1Bow(){
    //Diffuse
    vec4 position;
    mat3 objectLocal;
    calcNormalMapValues(position, objectLocal);

    vec3 metalNorm = texture(metalNormal, TexCoord).xyz;
    vec3 rustNorm = texture(rustNormal, TexCoord).xyz;
    vec3 norm = vec3(metalNorm.xy + rustNorm.xy, metalNorm.z * rustNorm.z);
    //vec3 norm = texture(metalNormal, TexCoord).xyz;
    norm.xy = 2.0*norm.xy - 1.0;

    calcFog(position);

    for (int i=0; i<1; i++){
        HDRColor += blingPhongModelNormal(i, position.xyz, norm, objectLocal);
    }

    vec4 metalTextColour = texture(metalTex, TexCoord);
    vec4 rustTextColour = texture(rustTex, TexCoord);

    vec3 textColour = mix(metalTextColour.rgb, rustTextColour.rgb, rustTextColour.a);
    //vec3 textColour = texture(metalTex, TexCoord).rgb;

    HDRColor *= textColour;
    if(abs(position.z) > Fog.MinDist)
    HDRColor = mix(Fog.Colour, HDRColor, fogFactor);
}

void pass1Arrow(){
    //Diffuse
    vec4 position;
    mat3 objectLocal;
    calcNormalMapValues(position, objectLocal);

    vec3 norm = texture(woodNormal, TexCoord).xyz;
    norm.xy = 2.0*norm.xy - 1.0;

    calcFog(position);

    for (int i=0; i<1; i++){
        HDRColor += blingPhongModelNormal(i, position.xyz, norm, objectLocal);
    }

    HDRColor *= texture(woodTex, TexCoord).rgb;
    if(abs(position.z) > Fog.MinDist)
    HDRColor = mix(Fog.Colour, HDRColor, fogFactor);
}

void pass2(){
    vec4 HDRTexColour = texture(HDRTex, TexCoord);

    vec3 Colour = HDRTexColour.rgb;

    //Convert to XYZ
    vec3 xyzCol = rgb2xyz*Colour;

    //Convert to xyY
    float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;

    vec3 xyYCol = vec3(xyzCol.x/xyzSum, xyzCol.y/xyzSum, xyzCol.z);

    //Apply tone mapping operation to luminance
    float L = (Exposure * xyYCol.z)/AveLum;
    L = (L*(1+L/(White*White)))/(1+L);

    //Using new luminance convert back to XYZ
    xyzCol.x = (L*xyYCol.x)/xyYCol.y;
    xyzCol.y = L;
    xyzCol.z = (L*(1-xyYCol.x-xyYCol.y))/xyYCol.y;

    //Convert back to RGB and send outpput to buffer
    if(DoToneMap){
        Colour = xyz2rgb * xyzCol;
        //Colour.x = floor(Colour.x * levels)*scaleFactor;
        //Colour.y = floor(Colour.y * levels)*scaleFactor;
        //Colour.z = floor(Colour.z * levels)*scaleFactor;
    }
    FragColor = vec4(Colour, 1.0);    
}

void main() {
    if(Pass == 1 && !arrow)
    pass1Bow();
    if(Pass == 1 && arrow)
    pass1Arrow();
    else if(Pass == 2)
    pass2();    
}