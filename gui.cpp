#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include "core.h"
#include "gui.h"

using namespace std;

HANDLE hStdOut;
HANDLE hStdin;
CONSOLE_SCREEN_BUFFER_INFO csbInfo; // Information about console window
COORD cursorPosition = {0, 1}; // Cursor coordinate
WORD wokWindowAttributes = (HACKER_MODE) ? 0b00001010 : 0b00001111;
WORD inactiveItemAttributes = 31;
WORD activeItemAttributes = 160;

const char DIM::applicationTitle[30] = "[TFM] Terminal File Manager";
int DIM::mainWindowHeight;
int DIM::workingAreaHeight;  // They are the same height
int DIM::mainWindowWidth;
DIM::COORD DIM::inputLine;  // Input line row
DIM::AREA DIM::sourceArea;  // Source area is a on the top
DIM::AREA DIM::targetArea;  // Target area is at the bottom

int STATE::sourceFileIndex;
char *STATE::sourcePath;
int STATE::targetFileIndex;
char *STATE::targetPath;
bool STATE::exit = false;

NAVIGATION_ITEM navigation[] = {
        {1,  0, "Ctrl + N", "Create new file"},
        {31, 0, "Ctrl + D", "Delete file"},
};

void configureConsole() {
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, wokWindowAttributes);
    GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
    if (hStdin == INVALID_HANDLE_VALUE)
        errorExit("GetStdHandle");

    // Enable the window and mouse input events.
    if (!SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS))
        errorExit("SetConsoleMode");

    DIM::mainWindowHeight = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
    DIM::mainWindowWidth = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;
    DIM::workingAreaHeight = 10;

    DIM::inputLine = {1 + DIM::workingAreaHeight + 1 + DIM::workingAreaHeight + 1, 0};  // Input line
    DIM::sourceArea = {2, short (2 + DIM::sourceArea.START)};  // Source area is a on the top
    DIM::targetArea = {DIM::sourceArea.END + 1, DIM::sourceArea.END + 1 + DIM::workingAreaHeight };  // Target area is at the bottom
}

void buildGUI() {
    int cursorRow = 0;

    if (DIM::mainWindowHeight < 10) errorExit("Your window is too small...\n");

    // ------------------------------------------------------------------------------------------------ Divider
    cout << DIM::applicationTitle;

    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int i = 0; i != DIM::mainWindowWidth; i++) {
        cout << ((STATE::sourcePath[i] == NULL) ? '-' : STATE::sourcePath[i]);
    }
    cursorRow += DIM::workingAreaHeight;

    // ------------------------------------------------------------------------------------------------ Divider
    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int i = 0; i != DIM::mainWindowWidth; i++) {
        cout << ((STATE::targetPath[i] == NULL) ? '-' : STATE::targetPath[i]);
    }

    cursorRow += DIM::workingAreaHeight;
    // ------------------------------------------------------------------------------------------------ Divider
    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int i = 0; i != DIM::mainWindowWidth; i++) { cout << '-'; }
    // ------------------------------------------------------------------------------------------------------ Input line
    cursorRow++;
    setCursorPosition(0, cursorRow);
    cout << "cmd:";

    // ------------------------------------------------------------------------------------------------ Navigation items
    for (int itemIndex = 0; itemIndex != size(navigation); itemIndex++) {  // Draw navigation
        NAVIGATION_ITEM item = navigation[itemIndex];
        setCursorPosition(0, cursorRow++);
        cout << item.title << "     " << item.description;
    }
}

void fillWorkingArea(TFM_FILE *files, DIM::AREA area) {
    for (int i = area.START; i != area.END; i++) {
        setCursorPosition(0, area.START + i);  // Set cursor at source area start
        if (NULL != &files[i]) {
            TFM_FILE f = files[i];
            printf("%-50s%s", f.title, f.lastChange);
        }
    }
}

// Sets cursor position
void setCursorPosition(short col, short row) {
    SetConsoleCursorPosition(hStdOut, COORD{col, row});
}

void errorExit(LPCSTR Message) {
    printf("%s\n", Message);
    system("pause");
    exit(0);
}

void keyEventProc(KEY_EVENT_RECORD ker) {
    // https://docs.microsoft.com/ru-ru/windows/win32/inputdev/virtual-key-codes?redirectedfrom=MSDN
    // https://docs.microsoft.com/en-us/windows/console/key-event-record-str
    if (!ker.bKeyDown) {
        if (ker.dwControlKeyState == SHIFT_PRESSED) {
            switch (ker.wVirtualKeyCode) {
                case 0x53:  // SHIFT + S (source)
                    // Process source area path
                    break;
                case 0x54:  // SHIFT + T (target)
                    // Process target area path
                    break;
                case 0x4E:  // SHIFT + N (new)
                    // Create file in selected are
                    break;
                case 0x41:  // SHIFT + A (area)
                    // switch working area ()
                    break;
            }
        } else if (ker.dwControlKeyState == LEFT_CTRL_PRESSED) {
            switch (ker.wVirtualKeyCode) {
                case VK_UP:  // CTRL + Arrow up
                    // Move file selection up
                    break;
                case VK_DOWN:  // CTRL + Arrow down
                    // Move file selection down
                    break;
            }
        }
    }
}