#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec3 vcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void
main()
{
  vcolor = color;
  gl_Position = projection * view * model * vec4(pos, 1.0);
}
