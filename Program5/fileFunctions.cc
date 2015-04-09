/**
 * SE3376.502 Program 5
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <fstream>
#include "program5.h"

using namespace std;

bool copyFile(const char* source, const char* dest) {
  ifstream in;
  ofstream out;

  in.open(source, ios::in | ios::binary);

  if (!in.is_open()) {
    fprintf(log_fd, "Failed to open input file: %s %s\n", source, strerror(errno));
    return false;
  }

  out.open(dest, ios::out | ios::binary | ios::trunc);

  if (!out.is_open()) {
    fprintf(log_fd, "Failed to open output file: %s %s\n", dest, strerror(errno));
    return false;
  }

  int bufsize = 1024;
  char* memblock = new char[bufsize];

  while (!in.eof() && !in.fail()) {
    in.read(memblock, bufsize);
    out.write(memblock, in.gcount());
  }

  in.close();
  out.close();

  return true;
}

struct stat info;

bool createDirectory(std::string name) {

  int result = stat(name.c_str(), &info);

  if (result == -1 && errno != ENOENT) {
    printf("Failed to create directory %s: %s\n", name.c_str(), strerror(EBADF));
    return false;
  }

  result = mkdir(name.c_str(), 0700);

  if (result != 0) {
    if (errno == EEXIST) {
      return true; // Already exists
    }

    return false; // Unable to create dir
  }

  return true; // Created
}

void writeProcessId(int pid) {
  char buf [64];
  int len = snprintf(buf, sizeof(buf), "%d", pid);

  int fd = open("dirmond.pid", O_WRONLY | O_CREAT, 0600);

  write(fd, buf, len);

  close(fd);
}

void deleteProcessId() {
  unlink("dirmond.pid");
}
