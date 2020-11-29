#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include "core.h"
#include "gui.h"

using namespace std;
enum navigationMap {
    FIRST, SECOND
};

HANDLE hStdOut;
SMALL_RECT consoleRect; // Corner coordinates
CONSOLE_SCREEN_BUFFER_INFO csbInfo; // Information about console window
COORD cursorPosition = {0, 1}; // Cursor coordinate
WORD wokWindowAttributes = (HACKER_MODE) ? 0b00001010 : 0b00001111;
WORD inactiveItemAttributes = 31;
WORD activeItemAttributes = 160;

COORD inputLine;

const int navigationItemsAmount = 2;  // Navigation items count
const int maxNavItemsInRow = 3;

const char applicationTitle[30] = "[TFM] Terminal File Manager";
const char widePadding[5] = "    ";  // Space between items

int mainWindowHeight;
int mainWindowWidth;

int inputLineRow;  // Input line row
int sourceAreaStart;  // Source area is a on the top
int sourceAreaEnd;
int targetAreaStart;  // Target area is at the bottom
int targetAreaEnd;

// TODO - remove ---------
char baseSourcePath[100] = "/test/subtest/subsubtest";
char baseTargetPath[100] = "/test/subtest/subsubtest";
// TODO - remove ---------


int workingAreaHeight;  // They are the same height

NAVIGATION_ITEM navigation[navigationItemsAmount] = {
        {1,  0, "Ctrl + N", "Create new file"},
        {31, 0, "Ctrl + D", "Delete file"},
};

void configureConsole() {
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, wokWindowAttributes);
    GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
    consoleRect = csbInfo.srWindow;

    // TODO - read width / height of console
    //    int columns = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
    //    int rows = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;

    mainWindowHeight = 40;
    mainWindowWidth = 100;
    workingAreaHeight = 10;

    inputLineRow = 1 + workingAreaHeight + 1 + workingAreaHeight + 1;  // Input line row
    sourceAreaStart = 2;  // Source area is a on the top
    sourceAreaEnd = sourceAreaStart + workingAreaHeight;
    targetAreaStart = sourceAreaEnd + 1;  // Target area is at the bottom
    targetAreaEnd = targetAreaStart + workingAreaHeight;
}

void buildGUI() {

    int cursorCol = 0; // Cursor column
    int cursorRow = 0; // Cursor row
    // position (col, row), title, description, handler
    if (mainWindowHeight < 10) {
        cout << "Your window is too small...\n";
        system("pause");
        exit(-1);
    }

    // ------------------------------------------------------------------------------------------------ Divider
    cout << applicationTitle;
    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int i = 0; i != mainWindowWidth; i++) {
        cout << ((baseSourcePath[i] == NULL) ? '-' : baseSourcePath[i]);
    }
    cursorRow += workingAreaHeight;
    // ------------------------------------------------------------------------------------------------ Divider
    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int i = 0; i != mainWindowWidth; i++) {
        cout << ((baseTargetPath[i] == NULL) ? '-' : baseTargetPath[i]);
    }

    cursorRow += workingAreaHeight;
    // ------------------------------------------------------------------------------------------------ Divider
    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int i = 0; i != mainWindowWidth; i++) {
        cout << '-';
    }
    // ------------------------------------------------------------------------------------------------------ Input line
    cursorRow++;
    setCursorPosition(0, cursorRow);
    cout << "cmd:";

    // ------------------------------------------------------------------------------------------------ Navigation items
    for (int itemIndex = 0; itemIndex != navigationItemsAmount; itemIndex++) {  // Draw navigation
        if (!(itemIndex % maxNavItemsInRow)) cursorRow++;  // Max items in row - go to next row
        NAVIGATION_ITEM item = navigation[itemIndex];
        setCursorPosition(0, cursorRow++);
        cout << item.title << widePadding << item.description;
    }
}

void fillWorkingArea(TFM_FILE *files) {
    for (int i = 0; i != workingAreaHeight; i++) {
        setCursorPosition(0, sourceAreaStart + i);  // Set cursor at source area start
        if (NULL == &files[i]) break;
        TFM_FILE f = files[i];
        printf("%-50s%s", f.title, f.lastChange);
    }
}

// Sets cursor position
void setCursorPosition(short col, short row) {
    SetConsoleCursorPosition(hStdOut, COORD{col, row});
}

void clear(bool navigationOnly) {
    //int i;
    //string s(80, ' ');
    //SetConsoleTextAttribute(hStdOut, woкkWindowAttributes);
    //if (it == 0) gotoxy(0, consolRect.Top + 1);
    //else gotoxy(0, consolRect.Top);
    //for (i = consolRect.Top; i < curspos.Y + 1; i++)
    //	cout << s.c_str();
    //gotoxy(0, 0);
}

void listenKeyboard() {

}

HANDLE hStdin;
DWORD fdwSaveOldMode;

VOID ErrorExit(LPCSTR);

VOID KeyEventProc(KEY_EVENT_RECORD);

VOID MouseEventProc(MOUSE_EVENT_RECORD);

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

int main(VOID) {
    DWORD cNumRead, fdwMode, i;
    INPUT_RECORD irInBuf[128];
    int counter = 0;

    // Get the standard input handle.

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");

    // Save the current input mode, to be restored on exit.

    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        ErrorExit("GetConsoleMode");

    // Enable the window and mouse input events.

    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;
    if (!SetConsoleMode(hStdin, fdwMode))
        ErrorExit("SetConsoleMode");

    // Loop to read and handle the next 500 input events.

    while (counter++ <= 500) {
        // Wait for the events.

        if (!ReadConsoleInput(
                hStdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead)) // number of records read
            ErrorExit("ReadConsoleInput");

        // Dispatch the events to the appropriate handler.

        for (i = 0; i < cNumRead; i++) {
            switch (irInBuf[i].EventType) {
                case KEY_EVENT: // keyboard input
                    KeyEventProc(irInBuf[i].Event.KeyEvent);
                    break;

                case MOUSE_EVENT: // mouse input
                    MouseEventProc(irInBuf[i].Event.MouseEvent);
                    break;

                case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                    ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
                    break;

                case FOCUS_EVENT:  // disregard focus events

                case MENU_EVENT:   // disregard menu events
                    break;

                default:
                    ErrorExit("Unknown event type");
                    break;
            }
        }
    }

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    return 0;
}

VOID ErrorExit(LPCSTR lpszMessage) {
    fprintf(stderr, "%s\n", lpszMessage);

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    ExitProcess(0);
}

VOID KeyEventProc(KEY_EVENT_RECORD ker) {
    printf("Key event: ");

    if (ker.bKeyDown)
        printf("key pressed\n");
    else printf("key released\n");
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer) {
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
    printf("Mouse event: ");

    switch (mer.dwEventFlags) {
        case 0:

            if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                printf("left button press \n");
            } else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
                printf("right button press \n");
            } else {
                printf("button press\n");
            }
            break;
        case DOUBLE_CLICK:
            printf("double click\n");
            break;
        case MOUSE_HWHEELED:
            printf("horizontal mouse wheel\n");
            break;
        case MOUSE_MOVED:
            printf("mouse moved\n");
            break;
        case MOUSE_WHEELED:
            printf("vertical mouse wheel\n");
            break;
        default:
            printf("unknown\n");
            break;
    }
}

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr) {
    printf("Resize event\n");
    printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}