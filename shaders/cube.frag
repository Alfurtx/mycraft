#version 330 core

out vec4 frag_color;

in vec3 vcolor;
in vec2 vtexuv;

uniform sampler2D chunk_texture;

void
main()
{
  // frag_color = vec4(0.7f, 0.5f, 0.2f, 1.0f);
  // frag_color = vec4(vcolor.xyz, 1.0f);
  vec4 aux = texture(chunk_texture, vtexuv);
  if(aux.a < 0.1) discard;
  frag_color = aux;
}
