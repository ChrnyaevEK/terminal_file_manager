#pragma once
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

namespace STATE {
    extern int fileIndex;
    extern char path[200];
    extern vector<string> files;
    extern bool exit;
};

void configureSystem();
void fillFiles();
