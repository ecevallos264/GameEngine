#version 330 core
in vec4 Color;
out vec4 FragColor;

//in vec2 textureCoordinates;

//uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform vec3 shapeColor;

void main()
{
    FragColor = Color;
//    FragColor = mix(texture(texture1, textureCoordinates), texture(texture2, textureCoordinates), 0.2);
}