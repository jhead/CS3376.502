/**
 * SE3376.502 Program 5
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

#include <tclap/CmdLine.h>
#include "program5.h"

using namespace std;

bool daemonMode;
string configFilename;

void parseCommandLine(int argc, char** argv) {
  TCLAP::CmdLine* cmd = new TCLAP::CmdLine("cs3376dirmond Directory Monitor Daemon", ' ', "0.1");

  try {

    TCLAP::SwitchArg daemonSwitch("d","daemon","Run in daemon mode (forks to run as a daemon).", *cmd);

    TCLAP::UnlabeledValueArg<string> configSwitch("configfile", "The name of the configuration file. Defaults to cs3376dirmond.conf", false, "cs3376dirmond.conf", "config filename");
    cmd->add(configSwitch);

    cmd->parse(argc, argv);

    daemonMode = daemonSwitch.getValue();
    configFilename = configSwitch.getValue();

  } catch (TCLAP::ArgException &e) {
    cerr << "Error: " << e.error() << e.argId() << endl;
    exit(1);
  }
}
