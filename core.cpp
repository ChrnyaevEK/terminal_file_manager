#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "core.h"

int STATE::sourceFileIndex = 0;
char STATE::sourcePath[PATH_MAX];
int STATE::targetFileIndex = 0;
char STATE::targetPath[PATH_MAX];
bool STATE::exit = false;

#include <direct.h>

using namespace std;

bool getCurrentDirectory(char *path) {
    return _getcwd(path, PATH_MAX);
}

bool configureSystem() {
    return getCurrentDirectory(STATE::sourcePath);
}

bool fillSourceFiles(char *path, TFM_FILE *files) {

}
