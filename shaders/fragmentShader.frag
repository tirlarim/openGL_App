#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float transparency;

void main() {
  float scale = 1.0f;
  FragColor = vec4(lightColor * objectColor * transparency, 1.0f);
}
