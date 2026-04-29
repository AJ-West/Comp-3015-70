#version 460

in float Transp;
in vec2 TexCoord;
uniform sampler2D ParticleTex;

// must ensure is consistent with other shaders
layout (location = 1) out vec4 HDRColor;

void main() {    
    HDRColor = texture(ParticleTex, TexCoord);
}