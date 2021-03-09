#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColorPrimary;
//layout (location = 2) in vec3 aColorSecondary;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 quadColorPrimary;
//out vec3 quadColorSecondary;
out vec4 pPos;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   pPos = vec4(aPos, 1.0);
   quadColorPrimary = aColorPrimary;
   //quadColorSecondary = aColorSecondary;
}
