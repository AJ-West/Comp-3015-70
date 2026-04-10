#version 460

in float Transp;
in vec2 TexCoord;
uniform sampler2D ParticleTex;

layout(binding=0) uniform sampler2D HDRTex;

//Render pass
uniform int Pass;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 HDRColor;

//for HDR
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

void pass2(){
    vec4 HDRTexColour = texture(ParticleTex, TexCoord);

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
    FragColor.a = HDRTexColour.a;
}

void main() {
    if(Pass == 1)
        HDRColor = texture(ParticleTex, TexCoord).rgb;
    else
        pass2();
    
    //Mix with black as it gets older to simulate smoke
    //FragColor = vec4(mix(vec3(0,0,0), FragColor.xyz, Transp), FragColor.a);
    //FragColor.a *= Transp;
}
