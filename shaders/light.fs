#version 330 core

uniform vec3 col;
out vec4 FragColor;

in vec2 Tex;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, Tex);
}