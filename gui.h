#pragma once
#define HACKER_MODE true
#define DEBUG

#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_LEFT 75
#define KEY_ENTER 13

typedef void(*FUN)(); // Navigation function pointer
typedef struct {  // Navigation item
    int col, row;
    const char title[10];  //  Keyboard shortcut to use
    const char description[30]; // Navigation button action description
    //    FUN handler;  // Handler  TODO - bind handler
} NAVIGATION_ITEM;
extern int navigationItemWidth;  // Width of navigation item

extern HANDLE hStdOut;
extern SMALL_RECT consoleRect; // Corner coordinates
extern COORD cursorPosition; // Cursor coordinate
extern CONSOLE_SCREEN_BUFFER_INFO csbInfo; // Information about console window
extern WORD wokWindowAttributes;
extern WORD inactiveItemAttributes;  // Inactive navigation item
extern WORD activeItemAttributes;  // Active navigation item

extern int mainWindowHeight;
extern int mainWindowWidth;
extern const int navigationItemsAmount;  // Navigation items count
extern const int maxNavItemsInRow;

extern const char smallPadding[2];  // Space between title and description
extern const char widePadding[3];  // Space between items

extern COORD inputLine;  // Input line index

extern COORD sourceArea_start;  // Source area is a left one
extern COORD targetArea_start;  // Target area is on the right
extern int workingArea_height;  // They are the same height

void buildGUI();
void configureConsole();

void setCursorPosition(short col, short row); // Move caret to col row
void handleUserInput();  // Should return user input
void clear(bool navigationOnly);  // Clear console, if navigationOnly - do not clear the user info, only navigation area
void getCursorPosition();  // Set cursorPosition global variable
void showCursor(bool visible);  // Show / hide cursor

