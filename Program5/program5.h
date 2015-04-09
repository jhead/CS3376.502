/**
 * SE3376.502 Program 5
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

// Args
extern bool daemonMode;
extern std::string configFilename;

// Config options
extern bool configured;
extern bool configVerbose;
extern int configNumVersions;
extern std::string configLogFile;
extern std::string configPassword;
extern std::string configWatchDir;
extern std::string vPath;
extern FILE* log_fd;

// Function declarations
void parseCommandLine(int, char**);

bool parseConfigFile(std::string);

void startWatching(std::string);
void stopWatching();

void writeProcessId(int);
void deleteProcessId();
void handleSignal(int);

std::string getTimestamp();
FILE* openLog();
void closeLog();

bool copyFile(const char*, const char*);
bool fileExists(std::string);
bool createDirectory(std::string);

// A hacky way to get the size of an arbitrary array. This is probably bad.
template<size_t SIZE, class T> inline int array_size(T (&arr)[SIZE]) {
    return static_cast<int>(SIZE);
}
