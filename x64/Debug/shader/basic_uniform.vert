#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

//position to send
out vec3 crntPosFrag;
//normal to send
out vec3 crntNormFrag;
//text coord to send
out vec2 TexCoord;
// tangent coords to send
out vec4 vertTangent;

uniform mat4 MVP;
uniform mat4 Model;

void main()
{
    crntPosFrag = vec3(Model * vec4(VertexPosition, 1.0));
    
    crntNormFrag = VertexNormal;

    TexCoord = VertexTexCoord;

    vertTangent = VertexTangent;

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
