#version 330 core

in vec4 color;
uniform vec4 barva;
out vec4 FragColor;


void main()
{
    FragColor = barva;
}