#include <stdio.h>
#include <time.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__

#include <sys/sysctl.h>
#include <unistd.h>

#else
#include <unistd.h>
#endif


int milliSleep(long milliseconds) {
  struct timespec ts;
  int res;
  if (milliseconds < 0) {
    errno = EINVAL;
    return -1;
  }
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000;
  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);
  return res;
}

long getFileSize(FILE* fp) {
  long size;
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return size;
}

int max(int a, int b) {
  return a > b ? a : b;
}

unsigned short normalizeIndex(int i, unsigned short limit) {
  return i < 0 ? limit - 1 : i >= limit ? 0 : i;
}

#ifdef _WIN32
unsigned char getNumberOfCores() {
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  return sysinfo.dwNumberOfProcessors;
}
#endif

#ifdef __APPLE__

unsigned char getNumberOfCores() {
  int nm[2];
  size_t len = 4;
  uint32_t count;
  nm[0] = CTL_HW;
  nm[1] = HW_AVAILCPU;
  sysctl(nm, 2, &count, &len, NULL, 0);
  if (count < 1) {
    nm[1] = HW_NCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);
    if (count < 1) { count = 1; }
  }
  return count;
}

#endif

#ifdef __linux__
unsigned char getNumberOfCores() {
  return sysconf(_SC_NPROCESSORS_ONLN);
}
#endif
