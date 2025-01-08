#pragma once
#include <string>
#include <vector>

class CliArguments {
public:
    static void showHelp();
    static std::string parseGameName(int argc, char* argv[]);
private:
    static const char* USAGE;
};
