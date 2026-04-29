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
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

uniform bool flag;

uniform float Time; // animation time

//wave parameters
uniform float Amp = 0.075;

void main()
{
    vec4 pos = vec4(VertexPosition, 1.0);
    if(flag){      
        // translate vertexes on y coordinates
        float wave = (dot(pos.xy, vec2(0.5)) + Time*2.0) ;
        pos.y = Amp*sin(wave) * (pos.x+8) * 0.5 + 0.5;    

        // compute normal vector
        vec3 n = vec3(0.0);
        n.xy = normalize(vec2(cos(wave),1.0));

        // pass values to the fragment shader
        crntPosFrag = (ModelViewMatrix * pos).xyz;
        crntNormFrag = NormalMatrix * n;       
    }
    else{
        crntPosFrag = vec3(Model * vec4(VertexPosition, 1.0));
    
        crntNormFrag = VertexNormal;

        vertTangent = VertexTangent;
    }

    TexCoord = VertexTexCoord;
    gl_Position = MVP * pos;
}
