#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_uv;

out vec3 vcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void
main()
{
  vcolor = vec3(0.3, 0.5, 0.5);
  gl_Position = projection * view * model * vec4(pos, 1.0);
}
