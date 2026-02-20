#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

//for showing faces
//flat out vec3 LightIntensity;

//for smooth shading
out vec3 LightIntensity;

//position to send
out vec3 crntPosFrag;
//normal to send
out vec3 crntNormFrag;

uniform mat4 MVP;
uniform mat4 Model;

void main()
{
    //crntPosFrag = vec3(Model * vec4(VertexPosition, 1.0));
    crntPosFrag = VertexPosition;
    crntNormFrag = VertexNormal;
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
