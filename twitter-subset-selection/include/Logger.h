#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Logger {
public:
    Logger();
    Logger(const string& outputFileName);
    ~Logger();
    void log(const string& logMessage);
    void outputToFile();

protected:
    string outputDirectory;
    string outputFileName;
    enum {
        CONSOLE,
        FILE
    } outputLocation = CONSOLE;
    vector<string> logs;
};

class ContinuousLogger : private Logger {

};

#endif