/* 
 * SE3376.502: Program 2
 * Author: Justin Head (jxh122430@utdallas.edu)
 */

#include <iostream>
#include <fstream>
#include <map>
#include <tclap/CmdLine.h>
#include <algorithm>

using namespace std;

enum { LOWER, UPPER, INFILE, OUTFILE };
static map<int, bool> switchMap;
static map<int, string> optionMap;

static string loadInput(string filename) {
    ifstream inputStream;
    inputStream.open(filename.c_str(), ios::in);

    string data = "";
    char line[256];
    while (inputStream.is_open() && !inputStream.eof()) {
        inputStream.getline(line, 256);
        data.append(line);
    }

    if (inputStream.is_open()) {
        inputStream.close();
    }

    return data;
}

static string processInput(string input) {
    string output = input;

    if (switchMap[LOWER]) {
        transform(output.begin(), output.end(), output.begin(), ::tolower);
    } else if (switchMap[UPPER]) {
        transform(output.begin(), output.end(), output.begin(), ::toupper);
    }

    return output;
}

void setupCommandLine(TCLAP::CmdLineInterface* cmd, int argc, char** argv) {
    try {
        TCLAP::SwitchArg lowerSwitch("l","lower","Convert all text to lowercase.", *cmd);
        TCLAP::SwitchArg upperSwitch("u","upper","Convert all text to uppercase.", *cmd);

        TCLAP::UnlabeledValueArg<string> inputArg("infile", "The input file name.", true, "infile.txt", "input filename", false);
        TCLAP::ValueArg<string> outputArg("o","outfile", "The name of the output file", false, "output.txt", "output filename");
        
        cmd->add(outputArg);
        cmd->add(inputArg);

        cmd->parse(argc, argv);

        switchMap[LOWER] = lowerSwitch.getValue();
        switchMap[UPPER] = upperSwitch.getValue();
        optionMap[INFILE] = inputArg.getValue();
        optionMap[OUTFILE] = outputArg.getValue();

        if (switchMap[LOWER] && switchMap[UPPER]) {
            throw TCLAP::CmdLineParseException("Options -l/--lower and -u/--upper cannot be used together.");
        }
    } catch (TCLAP::ArgException &e) {
        cerr << "Error: " << e.error() << e.argId() << endl;
        cmd->getOutput()->usage(*cmd); // Broken, why?

        exit(1);
    }
}

bool writeOutput(string data, string filename) {
    ofstream outputStream;
    outputStream.open(filename.c_str(), ios::out);

    if (outputStream.is_open()) {
        outputStream << data;

        outputStream.close();
        return true;
    }

    return false;
}

int main(int argc, char** argv) {
    TCLAP::CmdLine* cmd = new TCLAP::CmdLine("CS3376.502 Program 2 (Justin Head)", ' ', "1.0");
    setupCommandLine(cmd, argc, argv);
    
    try {
        cout << "Input File: " << optionMap[INFILE] << endl;
        cout << "Output File: " << optionMap[OUTFILE] << "\n\n";

        string inputData = loadInput(optionMap[INFILE]);
        cout << "Input Data: \"" << inputData << "\"\n";

        string outputData = processInput(inputData);
        cout << "Output Data: \"" << outputData << "\"\n";

        writeOutput(outputData, optionMap[OUTFILE]);
    } catch(exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}