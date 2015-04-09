/**
 * SE3376.502 Program 5
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <limits.h>
#include <sys/inotify.h>
#include <cstring>
#include "program5.h"

static int watch_fd = -1;
static int watch_wd = -1;

static bool watching = false;

void startWatching(std::string dir) {
  fprintf(log_fd, "Watching directory: %s\n", dir.c_str());

  size_t bufsiz = sizeof(struct inotify_event) + PATH_MAX + 1;
  struct inotify_event *event = (struct inotify_event *) malloc(bufsiz);

  // Init inotify
  //
  if (watch_fd <= 0) {
    watch_fd = inotify_init();
  }

  // Check if inotify init'd successfully
  //
  if (watch_fd == -1) {
    if (configVerbose) fprintf(log_fd, "Failed to init inotify");
    exit(1);
  }

  // Add directory to inotify watch
  //
  watch_wd = inotify_add_watch(watch_fd, dir.c_str(), IN_MODIFY);


  // Wait for modify events
  //
  watching = true;
  while (watching) {
    read(watch_fd, event, bufsiz);
    if (!watching) break;

    char* filename = (*event).name;
    if ((*event).mask != IN_MODIFY) continue;

    std::string timestamp = getTimestamp();
    std::string outputPath = vPath + "/" + std::string(filename) + "." + timestamp;
    std::string inputPath = configWatchDir + "/" + std::string(filename);

    if (configVerbose) fprintf(log_fd, "File %s modified at %s. Saving... \n", filename, timestamp.c_str());

    bool saveResult = copyFile(inputPath.c_str(), outputPath.c_str());
    if (configVerbose && saveResult) fprintf(log_fd, "File saved! %s\n", outputPath.c_str());
  }

  watching = false;
  free(event);

  fprintf(log_fd, "Stopped watching directory\n");
}

void stopWatching() {
  watching = false;

  if (watch_fd > 0) {
    inotify_rm_watch(watch_fd, watch_wd);
  }

  close(watch_fd);
}
