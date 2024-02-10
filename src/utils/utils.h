#ifndef OPENGL_APP_UTILS_H
#define OPENGL_APP_UTILS_H

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#include "../shader.hpp"

extern "C" {
#include <stdio.h>

unsigned char getNumberOfCores();
int milliSleep(long milliseconds);
long getFileSize(FILE* fp);
unsigned short normalizeIndex(int i, unsigned short limit);
void printOpenGLLimits();
void printGraphicsCardInfo();
void checkHardware();
}

#endif //OPENGL_APP_UTILS_H
