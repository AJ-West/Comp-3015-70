#version 460

in float Transp;
in vec2 TexCoord;
uniform sampler2D ParticleTex;

layout (location = 1) out vec4 HDRColor;

void main() {    
    HDRColor = texture(ParticleTex, TexCoord);

    //Mix with black as it gets older to simulate smoke
    //HDRColor = vec4(mix(vec3(0,0,0), HDRColor.xyz, Transp), HDRColor.a);
    //HDRColor.a *= Transp;
}