#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

//position to send
out vec3 crntPosFrag;
//normal to send
out vec3 crntNormFrag;
//text coord to send
out vec2 TexCoord;

uniform mat4 MVP;
uniform mat4 Model;

void main()
{
    crntPosFrag = vec3(Model * vec4(VertexPosition, 1.0));
    //crntPosFrag = VertexPosition;
    
    crntNormFrag = VertexNormal;

    TexCoord = VertexTexCoord;

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
