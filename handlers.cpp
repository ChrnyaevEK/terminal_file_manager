#include "handlers.h"
#include <iostream>
#include <string>
#include <cstdio>
#include "gui.h"
#include "core.h"
#include <direct.h>

using namespace std;

void changeWorkingDirectory() {  // Move to directory
    string path;  // Hold new path
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);  // Move cursor
    stdMsgOut("Enter path: ");
    cin >> path;  // Get path
    errno_t err = _chdir(path.c_str());
    refresh();
    if (err) {  // Change dir.
        stdMsgOut("Error - path not found!");  // Error
    } else {
        stdMsgOut("OK");  // Success
    }
}

void createNewFile() {  // Create new file
    string name;  // Hold name
    FILE *infile;  // File pointer to close file
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    stdMsgOut("Enter file name: ");
    cin >> name;  // Get name
    errno_t err = fopen_s(&infile, name.c_str(), "w+");
    refresh();
    if (err) {  // Error
        stdMsgOut("Error! Could not create file...");
    } else {  // Ok
        fclose(infile);  // Close file
        stdMsgOut("OK");
    }
}

void removeFile() {
    string name;  // Hold name
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    stdMsgOut("Enter file name: ");
    cin >> name;  // Get name
    errno_t err = remove(name.c_str());  // Remove file
    refresh();
    if (err) {  // Error
        stdMsgOut("Error! Could not remove file...");
    } else {  // Success
        stdMsgOut("OK");
    }
}


void renameFile() {  // Renames file
    string name;  // Holds new name
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    stdMsgOut("Enter new file name: ");
    cin >> name;  // Get new name
    errno_t err = rename(STATE::files[STATE::files.size() - STATE::fileIndex - 1].c_str(), name.c_str());
    refresh();
    if (err) {
        stdMsgOut("Error! Could not rename file...");
    } else {
        stdMsgOut("OK");
    }
}
