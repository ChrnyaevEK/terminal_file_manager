#pragma once
#define PATH_MAX 200
#define FILES_MAX 1000

typedef struct {  // File representation
    char title[51];
    char lastChange[11];
} TFM_FILE;

namespace STATE {
    extern int sourceFileIndex;
    extern char sourcePath[PATH_MAX];
    extern TFM_FILE sourceFiles[FILES_MAX];
    extern int targetFileIndex;
    extern char targetPath[PATH_MAX];
    extern TFM_FILE targetFiles[FILES_MAX];
    extern bool exit;
};

bool configureSystem();
