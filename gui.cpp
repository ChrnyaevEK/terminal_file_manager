#include <windows.h>
#include <conio.h>
#include <iostream>
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
int globalCursorCol; // Cursor column
int globalCursorRow; // Cursor row

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
    targetAreaStart = sourceAreaStart + 1;  // Target area is at the bottom
    targetAreaEnd = targetAreaStart + workingAreaHeight;

    globalCursorCol = 0; // Cursor column
    globalCursorRow = 0; // Cursor row
}

void buildGUI() {
    // position (col, row), title, description, handler
    if (mainWindowHeight < 10) {
        cout << "Your window is too small...\n";
        system("pause");
        exit(-1);
    }

    // ------------------------------------------------------------------------------------------------ Divider
    cout << applicationTitle;
    globalCursorRow++;
    setCursorPosition(0, globalCursorRow);
    for (int i = 0; i != mainWindowWidth; i++) {
        cout << ((baseSourcePath[i] == NULL) ? '-' : baseSourcePath[i]);
    }
    globalCursorRow += workingAreaHeight;
    // ------------------------------------------------------------------------------------------------ Divider
    globalCursorRow++;
    setCursorPosition(0, globalCursorRow);
    for (int i = 0; i != mainWindowWidth; i++) {
        cout << ((baseTargetPath[i] == NULL) ? '-' : baseTargetPath[i]);
    }

    globalCursorRow += workingAreaHeight;
    // ------------------------------------------------------------------------------------------------ Divider
    globalCursorRow++;
    setCursorPosition(0, globalCursorRow);
    for (int i = 0; i != mainWindowWidth; i++) {
        cout << '-';
    }
    // ------------------------------------------------------------------------------------------------------ Input line
    globalCursorRow++;
    setCursorPosition(0, globalCursorRow);
    cout << "cmd:";

    // ------------------------------------------------------------------------------------------------ Navigation items
    for (int itemIndex = 0; itemIndex != navigationItemsAmount; itemIndex++) {  // Draw navigation
        if (!(itemIndex % maxNavItemsInRow)) globalCursorRow++;  // Max items in row - go to next row
        NAVIGATION_ITEM item = navigation[itemIndex];
        setCursorPosition(0, globalCursorRow++);
        cout << item.title << widePadding << item.description;
    }

    cout << '\n';
    system("pause");
    exit(0);
}

void fillWorkingArea(TFM_FILE *files) {
    globalCursorRow = sourceAreaStart;
    setCursorPosition(0, globalCursorRow);  // Set cursor at source area start
    for (int i = 0; i != workingAreaHeight; i++) {
        TFM_FILE f = files[i];
        cout << f.title << widePadding << f.lastChange;
        globalCursorRow++;
    }
}

// Sets cursor position
void setCursorPosition(short col, short row) {
    SetConsoleCursorPosition(hStdOut, COORD{col, row});
}

void getCursorPosition() {
    //GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
    //curspos = csbInfo.dwCursorPosition;
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

// Change cursor visibility
void showCursor(bool visible) {
//    CONSOLE_CURSOR_INFO ccInfo;
//    ccInfo.bVisible = visible;
//    ccInfo.dwSize = 20;
//    SetConsoleCursorInfo(hStdOut, &ccInfo);
}
