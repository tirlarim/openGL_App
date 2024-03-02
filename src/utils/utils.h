#pragma once

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#include "../Shader.hpp"

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

#pragma once
