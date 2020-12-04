#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "core.h"
#include "gui.h"
#include <direct.h>

using namespace std;
HANDLE hStdOut;
HANDLE hStdin;
CONSOLE_SCREEN_BUFFER_INFO csbInfo; // Information about console window
COORD cursorPosition = {0, 1}; // Cursor coordinate
WORD wokWindowAttributes = (HACKER_MODE) ? 0b00001010 : 0b00001111;
WORD activeItemAttributes = 160;

const char DIM::applicationTitle[30] = "[TFM] Terminal File Manager";
int DIM::mainWindowHeight;
int DIM::workingAreaHeight;  // They are the same height
int DIM::mainWindowWidth;
DIM::COORD DIM::inputLine;  // Input line row
DIM::AREA DIM::workingArea;  // Source area is a on the top


NAVIGATION_ITEM navigation[] = {
        {"SHIFT + S", "Change source path"},
        {"SHIFT + T", "Change target path"},
        {"SHIFT + N", "Create file in selected area"},
        {"SHIFT + A", "Switch working area"},
};

void doDimensions() {
    GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
    DIM::mainWindowWidth = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
    DIM::mainWindowHeight = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;
    DIM::workingAreaHeight = DIM::mainWindowHeight - 4 - size(navigation);
    DIM::inputLine = {0, short(3 + DIM::workingAreaHeight + size(navigation))};  // Input line
    DIM::workingArea = {size(navigation) + 3,
                        short(size(navigation) + 3 + DIM::workingAreaHeight)};  // Source area is a on the top
}

void configureConsole() {
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
    if (hStdin == INVALID_HANDLE_VALUE)
        errorExit("GetStdHandle");

    // Enable the window and mouse input events.
    if (!SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS))
        errorExit("Could not do SetConsoleMode");
}

void buildGUI() {
    clear();
    SetConsoleTextAttribute(hStdOut, wokWindowAttributes);
    int cursorRow = 0;

    if (DIM::mainWindowHeight < 10) errorExit("Your window is too small...\n");

    cout << DIM::applicationTitle;  // Title
    // ------------------------------------------------------------------------------------------------ Navigation items
    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int i = 0; i != DIM::mainWindowWidth; i++) { cout << '-'; }
    // ------------------------------------------------------------------------------------------------ Divider
    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int itemIndex = 0; itemIndex != size(navigation); itemIndex++) {  // Draw navigation
        NAVIGATION_ITEM item = navigation[itemIndex];
        setCursorPosition(0, cursorRow++);
        cout << item.title << "     " << item.description;
    }

    setCursorPosition(0, cursorRow);
    for (int i = 0; i != DIM::mainWindowWidth; i++) { cout << '-'; }
    cursorRow += DIM::workingAreaHeight;
    setCursorPosition(0, cursorRow);

    // ------------------------------------------------------------------------------------------------ Divider
    for (int i = 0; i != DIM::mainWindowWidth; i++) { cout << '-'; }
    // ------------------------------------------------------------------------------------------------------ Input line
    cursorRow++;
    setCursorPosition(0, cursorRow);  // Input
}

void cleanWorkingArea() {
    SetConsoleTextAttribute(hStdOut, wokWindowAttributes);
    for (int pos = DIM::workingArea.START; pos != DIM::workingArea.END - 1; pos++) {
        setCursorPosition(0, pos);  // Set cursor at source area start
        printf("%c[2K", 27);
    }
};


void fillWorkingArea() {
    int pos = DIM::workingArea.START;
    int wait = STATE::fileIndex;
    setCursorPosition(0, pos++);
    cout << "Path   ";
    for (int i = 0; i != DIM::mainWindowWidth; i++) { cout << STATE::path[i]; }
    for (auto ri = STATE::files.rbegin(); ri != STATE::files.rend(); ri++) {
        if (wait) {
            wait--;
            continue;  // Move selected area
        }
        if (pos == DIM::workingArea.START + 1) {
            SetConsoleTextAttribute(hStdOut, activeItemAttributes);
        } else {
            SetConsoleTextAttribute(hStdOut, wokWindowAttributes);
        }
        if (pos >= DIM::workingArea.END - 1) break;
        setCursorPosition(0, pos);  // Set cursor at source area start
        printf(" - %s", (*ri).c_str());
        pos++;
    }
}

// Sets cursor position
void setCursorPosition(short col, short row) {
    SetConsoleCursorPosition(hStdOut, COORD{col, row});
}

void clear() {
    system("cls"); // clear console
}

void stdMsgOut(LPCSTR Message) {
    clear();
    configureSystem();
    configureConsole();
    fillFiles();
    buildGUI();
    fillWorkingArea();
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    printf("%s", Message);
}


void errorExit(LPCSTR Message) {
    printf("%s\n", Message);
    system("pause");
    exit(0);
}


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

void keyEventProc(KEY_EVENT_RECORD ker) {
    // https://docs.microsoft.com/ru-ru/windows/win32/inputdev/virtual-key-codes?redirectedfrom=MSDN
    // https://docs.microsoft.com/en-us/windows/console/key-event-record-str
    if (!ker.bKeyDown) {
        if (ker.dwControlKeyState & LEFT_CTRL_PRESSED) {
            switch (ker.wVirtualKeyCode) {
                case 0x50:  // CTRL + P (source)
                    changeWorkingDirectory();
                    break;
                case 0x4E:  // CTRL + N (new)
                    // Create file in selected area
                    createNewFile();
                    break;
                case 0x44:  // CTRL + D (delete)
                    // Delete file
                    removeFile();
                    break;
                case 0x52:  // CTRL + R (rename)
                    // Rename file
                    renameFile();
                    break;
            }
        } else {
            switch (ker.wVirtualKeyCode) {
                case VK_UP:  // CTRL + Arrow up
                    // Move file selection up
                    if (STATE::fileIndex > 0) {
                        cleanWorkingArea();
                        STATE::fileIndex--;
                        fillWorkingArea();
                    }
                    break;
                case VK_DOWN:  // CTRL + Arrow down
                    // Move file selection down
                    if (STATE::fileIndex >= 0 && STATE::fileIndex < STATE::files.size() - 1) {
                        cleanWorkingArea();
                        STATE::fileIndex++;
                        fillWorkingArea();
                    }
                    break;
            }
        }
    }
}