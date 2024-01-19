#ifndef OPENGL_APP_UTILS_H
#define OPENGL_APP_UTILS_H

#include <stdio.h>

unsigned char getNumberOfCores();
int milliSleep(long milliseconds);
long getFileSize(FILE* fp);
int max(int a, int b);
int min(int a, int b);
unsigned short normalizeIndex(int i, unsigned short limit);

#endif //OPENGL_APP_UTILS_H
