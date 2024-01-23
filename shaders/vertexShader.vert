#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

void main() {
  gl_Position = transform * vec4(aPos, 0.85f);
  ourColor = aColor; // set the output variable to a dark-red color
  TexCoord = aTextureCoord;
}
