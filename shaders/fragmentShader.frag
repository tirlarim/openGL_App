#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float op;

void main() {
  float scale = 1.0f;
  FragColor = mix(texture(texture1, vec2(TexCoord.x*scale, TexCoord.y*scale)),
                  texture(texture2, vec2(TexCoord.x*scale, TexCoord.y*scale)), op);
}
