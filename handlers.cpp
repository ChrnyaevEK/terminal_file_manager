#include "handlers.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "gui.h"
#include <direct.h>

using namespace std;

void changeWorkingDirectory() {
    string path;
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    stdMsgOut("Enter path: ");
    cin >> path;
    if (_chdir(path.c_str())) {
        stdMsgOut("Error - path not found!");
    } else {
        stdMsgOut("OK");
    }
}

void createNewFile() {
    string name;
    FILE *infile;
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    stdMsgOut("Enter file name: ");
    cin >> name;
    errno_t err = fopen_s(&infile, name.c_str(), "w+");
    if (err) {
        stdMsgOut("Error! Could not create file...");
    } else {
        fclose(infile);
        stdMsgOut("OK");
    }
}

void removeFile() {
    string name;
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    stdMsgOut("Enter file name: ");
    cin >> name;
    errno_t err = remove(name.c_str());
    if (err) {
        stdMsgOut("Error! Could not remove file...");
    } else {
        stdMsgOut("OK");
    }
}


void renameFile() {
    string nameOld;
    string nameNew;
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    stdMsgOut("Enter old file name: ");
    cin >> nameOld;
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    stdMsgOut("Enter new file name: ");
    cin >> nameNew;
    errno_t err = rename(nameOld.c_str(), nameNew.c_str());
    if (err) {
        stdMsgOut("Error! Could not rename file...");
    } else {
        stdMsgOut("OK");
    }
}
