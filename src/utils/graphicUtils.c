#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void printGraphicsCardInfo() {
  const GLubyte* vendor = glGetString(GL_VENDOR);
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  printf("Graphics Card Vendor: %s\n", vendor);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL Version: %s\n", version);
  // Check for errors
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) printf("OpenGL error: %d\n", err);
}

void printOpenGLLimits() {
  GLint value;
  GLfloat floatValue;
  GLfloat floatValues[4];
  // Integer-based limits
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
  printf("Max Texture Size: %d\n", value);
  glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &value);
  printf("Max Viewport Dimensions: %d\n", value);
  glGetIntegerv(GL_MAX_CLIP_DISTANCES, &value);
  printf("Max Clip Distances: %d\n", value);
  glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &value);
  printf("Max Renderbuffer Size: %d\n", value);
  glGetIntegerv(GL_MAX_SAMPLES, &value);
  printf("Max Samples: %d\n", value);
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
  printf("Max Vertex Attributes: %d\n", value);
  glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &value);
  printf("Max Vertex Uniform Components: %d\n", value);
  glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &value);
  printf("Max Fragment Uniform Components: %d\n", value);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &value);
  printf("Max Combined Texture Image Units: %d\n", value);
  // Float-based limits
  glGetFloatv(GL_DEPTH_CLEAR_VALUE, &floatValue);
  printf("Depth Clear Value: %f\n", floatValue);
  glGetFloatv(GL_LINE_WIDTH_RANGE, floatValues);
  printf("Line Width Range: %f to %f\n", floatValues[0], floatValues[1]);
  glGetFloatv(GL_POINT_SIZE_RANGE, floatValues);
  printf("Point Size Range: %f to %f\n", floatValues[0], floatValues[1]);
  glGetFloatv(GL_BLEND_COLOR, floatValues);
  printf("Blend Color: %f, %f, %f, %f\n", floatValues[0], floatValues[1], floatValues[2], floatValues[3]);
  // Check for errors
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) printf("OpenGL error: %d\n", err);
}

void checkHardware() {
  printGraphicsCardInfo();
  printOpenGLLimits();
}
