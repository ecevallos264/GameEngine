#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
//layout (location = 1) in vec2 aTextureCoordinates;

//out vec2 textureCoordinates;
out vec3 Color;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
//        gl_Position =  vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Color = aColor;
}