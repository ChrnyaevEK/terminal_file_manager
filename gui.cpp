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

const char smallPadding[2] = " ";  // Space between title and description
const char widePadding[3] = "  ";  // Space between items

int mainWindowHeight;
int mainWindowWidth;
int navigationItemWidth;

COORD sourceArea_start;  // Source area is a left one
COORD targetArea_start;  // Target area is on the right
int workingArea_height;  // They are the same height

NAVIGATION_ITEM navigation[navigationItemsAmount] = {
        { 1, 0, "Ctrl + N" ,"Create new file" },
        { 31, 0, "Ctrl + D", "Delete file" },
};

void configureConsole(){
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, wokWindowAttributes);
    GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
    consoleRect = csbInfo.srWindow;

    // TODO - read width / height of console
    //    int columns = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
    //    int rows = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;

    mainWindowWidth = 100;
    mainWindowHeight = 40;

    // Count item width
    NAVIGATION_ITEM item = navigation[0];
    navigationItemWidth = sizeof(item.title) + sizeof(item.description) + sizeof(smallPadding) + sizeof(widePadding);

    // Count available working area width (mind dividers!)
    workingArea_height = int(maxNavItemsInRow / navigationItemsAmount) + 1 + 3;  // +1 - navigation width, +3 - dividers
    workingArea_height -= 1; // Another divider between two areas
    workingArea_height = int(workingArea_height);
}

void buildGUI() {
    // position (col, row), title, description, handler
    int currentRow = 0;
    if (mainWindowHeight < 10){
        cout << "Your window is too small...\n";
        system("pause");
        exit(-1);
    }
    // ------------------------------------------------------------------------------------------------ Navigation items
    for (int itemIndex = 0; itemIndex != navigationItemsAmount; itemIndex++) {  // Draw navigation
        if (!(itemIndex % maxNavItemsInRow))  currentRow ++;  // Max items in row - go to next row
        NAVIGATION_ITEM item = navigation[itemIndex];
        setCursorPosition(itemIndex * navigationItemWidth, currentRow);
        cout << item.title << smallPadding << item.description << widePadding;
    }


    // ------------------------------------------------------------------------------------------------ Divider
    currentRow++;
    setCursorPosition(0, currentRow);
    for (int i = 0; i != mainWindowWidth; i++){
        cout << '-';
    }


    currentRow += workingArea_height;


    // ------------------------------------------------------------------------------------------------ Divider
    currentRow++;
    setCursorPosition(0, currentRow);
    for (int i = 0; i != mainWindowWidth; i++){
        cout << '-';
    }



    currentRow += workingArea_height;


    // ------------------------------------------------------------------------------------------------ Divider
    currentRow++;
    setCursorPosition(0, currentRow);
    for (int i = 0; i != mainWindowWidth; i++){
        cout << '-';
    }




    // ------------------------------------------------------------------------------------------------------ Input line
    currentRow++;
    setCursorPosition(0, currentRow);
    cout << ':';




    const short inputLineRow = currentRow;  // Save input line coordinates
    inputLine = {inputLineRow, 1};

    cout << '\n';
    system("pause");
    exit(0);
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
