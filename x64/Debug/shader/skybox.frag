#version 460

in vec3 Vec;

layout(binding=0) uniform samplerCube SkyBoxTex;

layout (location = 1) out vec3 HDRColor; // need to set output to HDRColor else wont be displayed in the quad

void main() {
    vec3 texColor = texture(SkyBoxTex, normalize(Vec)).rgb;

    HDRColor = texColor;
}
