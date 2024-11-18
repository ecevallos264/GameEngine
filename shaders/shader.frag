#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 textureCoordinates;

uniform sampler2D ourTexture;
void main()
{
    FragColor = vec4(vertexColor, 1.0);
//    FragColor = texture(ourTexture, textureCoordinates);
}