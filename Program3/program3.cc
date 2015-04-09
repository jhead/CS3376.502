/*
 * Program 3
 * CS3376.502
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>

std::string executeCommand(const char* command) {
    FILE* file = popen(command, "r");
    std::string data;

    char buffer[128];
    char* temp;
    int total = 0;
    while ((temp = fgets(buffer, 128, file))) {
        data += buffer;
        total += strlen(temp);
    }

    // Strip trailing newline character
    if (total > 0 && data.at(total - 1) == '\n') {
        data = data.substr(0, total - 1);
    }

    return data;
}

std::string executeCommand(const std::string command) {
    return executeCommand(command.c_str());
}

std::string getWorkingDirectory() {
    return executeCommand("pwd");
}

int main() {
    std::string BASE_PATH = getWorkingDirectory();

    std::string GAWK_PATH = BASE_PATH + "/gawk/bin/gawk";
    std::string cmd1 = GAWK_PATH + " --version";
    std::string cmd2 = GAWK_PATH + " -f awk.code numbers.txt";

    // Display gawk location and commands
    std::cout << "gawk at: " << GAWK_PATH << "\n";
    std::cout << "Shellcmd1: " << cmd1 << "\n";
    std::cout << "Shellcmd2: " << cmd2 << "\n\n";

    // Call gawk --version
    std::string cmd1_output = executeCommand(cmd1);
    std::cout << "The first call to gawk returned:\n\n" << cmd1_output << "\n\n";

    // Call gawk code
    std::string cmd2_output = executeCommand(cmd2);
    std::cout << "The second call to gawk returned: " << cmd2_output << "\n\n";

    // Parse output from gawk, display column values, and sum columns
    std::string buffer;
    std::stringstream ss(cmd2_output);
    int val, sum = 0, col = 1;

    while (ss >> buffer) {
        std::istringstream(buffer) >> val;
        sum += val;

        printf("The sum of column %d is: %d\n", col, val);
        col++;
    }

    printf("The sum of the two numbers is: %d\n", sum);

    return 0;
}
