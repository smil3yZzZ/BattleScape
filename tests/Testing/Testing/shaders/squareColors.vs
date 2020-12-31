#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 viewportScreen;
uniform vec2 worldScreenOrigin;

out vec3 quadColor;
out vec4 pPos;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   pPos = projection * view * model * vec4(aPos, 1.0);
   quadColor = aColor;
}
