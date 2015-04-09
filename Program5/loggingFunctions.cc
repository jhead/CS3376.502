/**
 * SE3376.502 Program 5
 * Author: Justin Head (jxh122430@utdallas.edu)
 */


#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "program5.h"

FILE* log_fd;

std::string getTimestamp() {
  const int bufsiz = 256;

  const time_t timer = time(NULL);
  tm* ts = localtime(&timer);

  int h = (*ts).tm_hour;
  int m = (*ts).tm_min;
  int s = (*ts).tm_sec;
  int d = (*ts).tm_mday;
  int mo = (*ts).tm_mon + 1;
  int y = (*ts).tm_year + 1900;

  char timestamp [bufsiz];
  snprintf(timestamp, bufsiz, "%d.%01d.%01d-%01d.%01d.%01d", y, mo, d, h, m, s);

  return std::string(timestamp);
}

FILE* openLog() {
  log_fd = fopen(configLogFile.c_str(), "wb+");

  if (log_fd == NULL) {
    printf("Failed to open log file %s: %s\n", configLogFile.c_str(), strerror(errno));
    exit(errno);
  }

  setbuf(log_fd, NULL);
  return log_fd;
}

void closeLog() {
  fclose(log_fd);
}
