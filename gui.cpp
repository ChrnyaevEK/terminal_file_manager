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

const int navigationItemsAmount = 2;  // Navigation items count
const int maxNavItemsInRow = 3;

const char smallPadding[2] = " ";  // Space between title and description
const char widePadding[3] = "  ";  // Space between items

void buildGUI() {
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, wokWindowAttributes);
    GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
    consoleRect = csbInfo.srWindow;

    int columns = 100;
    int rows = 40;
    int navHeight = 2;  // Menu higth (rows)

    // position (col, row), title, description, handler
    NAVIGATION_ITEM navigation[navigationItemsAmount] = {
            { 1, 0, "Ctrl + N" ,"Create new file" },
            { 31, 0, "Ctrl + D", "Delete file" },
    };
    navigationMap selectedItem = FIRST;  // Selected element

    //    int columns = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;
    //    int rows = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;

    int currentRow = 0;
    for (; currentRow != rows - navHeight - 2; currentRow++) {  // 2 rows for dividers
        setCursorPosition(columns / 2, currentRow);
        cout << '|';
    }
    setCursorPosition(0, currentRow);
    for (int i=0; i != columns; i++){
        cout << '-';
    }
    setCursorPosition(0, ++currentRow);
    cout << "Input:";
    setCursorPosition(0, ++currentRow);
    for (int i=0; i != columns; i++){
        cout << '-';
    }

    for (int itemIndex = 0; itemIndex != navigationItemsAmount; itemIndex++) {  // Draw navigation
        if (!(itemIndex % maxNavItemsInRow))  currentRow ++;  // Max items in row - go to next row
        NAVIGATION_ITEM item = navigation[itemIndex];
        // Count item width
        int itemWidth = sizeof(item.title) + sizeof(item.description) + sizeof(smallPadding) + sizeof(widePadding);
        setCursorPosition(itemIndex * itemWidth, currentRow);
        cout << item.title << smallPadding << item.description << widePadding;
    }


    cout << '\n';
    system("pause");
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
