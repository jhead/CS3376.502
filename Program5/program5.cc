/**
 * SE3376.502 Program 5
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <signal.h>
#include "program5.h"

std::string vPath;

int main(int argc, char** argv) {

  // Setup command line functionality
  //
  parseCommandLine(argc, argv);

  // Load config file; exit if config load fails
  //
  bool configResult = parseConfigFile(configFilename);
  if (!configResult) {
    exit(1);
  }

  // Create .versions dir if necessary
  //
  vPath = configWatchDir + std::string("/.versions");
  if (!createDirectory(vPath)) {
    printf("Failed to create .versions directory in %s. Does it exist?\n", configWatchDir.c_str());
    exit(1);
  }

  // Register signal handlers
  //
  signal(SIGINT, handleSignal);
  signal(SIGTERM, handleSignal);
  signal(SIGHUP, handleSignal);

  // Fork if daemon mode enabled
  //
  pid_t pid = 0;
  if (daemonMode) {
    pid = fork();
  }

  if (daemonMode && pid > 0) {
    printf("Daemon forked. New PID: %d\n", pid);
    writeProcessId(pid);
    return 0;
  }

  if (!daemonMode) printf("Running. See log for details: %s\n", configLogFile.c_str());

  openLog();
  startWatching(configWatchDir);
}

void handleSignal(int signum) {
  if (signum == SIGTERM || signum == SIGINT) {

    fprintf(log_fd, "Shutting down...\n");
    if (!daemonMode) {
      printf("Shutting down...\n");
    }

    stopWatching();
    closeLog();
    deleteProcessId();

    exit(signum);
  } else if (signum == SIGHUP) {
    fprintf(log_fd, "Received SIGHUP\n");

    if (parseConfigFile(configFilename)) {
      fprintf(log_fd, "Reloaded config file\n");
    } else {
      fprintf(log_fd, "Config reload failed\n");
    }
  }
}
