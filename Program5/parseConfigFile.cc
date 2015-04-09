/**
 * SE3376.502 Program 5
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <rude/config.h>
#include "program5.h"

using namespace std;

bool configured;
bool configVerbose;
int configNumVersions;
string configLogFile;
string configPassword;
string configWatchDir;

const string options[] = { "Verbose", "NumVersions", "LogFile", "Password", "WatchDir" };

static void missing(string name) {
  printf("Missing config option: %s\nPlease add option to configuration file and try again.\n", name.c_str());
  exit(1);
}

bool parseConfigFile(string configFilename) {
  if (log_fd) {
    fprintf(log_fd, "Loading config file: %s...\n", configFilename.c_str());
  } else {
    printf("Loading config file: %s...\n", configFilename.c_str());
  }

  rude::Config config;

  config.load(configFilename.c_str());
  config.setSection("Parameters");

  bool missingAny = false;
  for (int i = 0; i < array_size(options); i++) {
    if (!config.exists(options[i].c_str())) {
      missing(options[i]);
      missingAny = true;
    }
  }

  if (missingAny) {
    return false;
  }

  configVerbose = config.getBoolValue("Verbose");
  configNumVersions = config.getIntValue("NumVersions");
  configLogFile = config.getStringValue("LogFile");

  if (!configured) {
    configPassword = config.getStringValue("Password");
    configWatchDir = config.getStringValue("WatchDir");
  } else if (configVerbose) {
    fprintf(log_fd, "Ignoring Password and WatchDir config changes during reload\n");
  }

  return (configured = true);
}
