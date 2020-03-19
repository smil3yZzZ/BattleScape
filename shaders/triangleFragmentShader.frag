#version 330 core

in vec3 quadColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(quadColor/255.0, 1.0);
}
