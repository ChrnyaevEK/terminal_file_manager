#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "core.h"

int STATE::fileIndex = 0;
char STATE::path[200];
bool STATE::exit = false;
vector<string> STATE::files;

#include <direct.h>

using namespace std;

void configureSystem() {
     _getcwd(STATE::path,200);
}

void fillFiles() {
    // https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
    STATE::files.clear();
    string search_path = "./*.*";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                STATE::files.emplace_back(fd.cFileName);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
}
