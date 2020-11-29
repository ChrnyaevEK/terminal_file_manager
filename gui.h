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

extern HANDLE hStdOut;
extern SMALL_RECT consoleRect; // Corner coordinates

extern CONSOLE_SCREEN_BUFFER_INFO csbInfo; // Information about console window
extern WORD wokWindowAttributes;
extern WORD inactiveItemAttributes;  // Inactive navigation item
extern WORD activeItemAttributes;  // Active navigation item

extern int mainWindowHeight;
extern int mainWindowWidth;

extern const int navigationItemsAmount;  // Navigation items count
extern const int maxNavItemsInRow;

extern const char widePadding[5];  // Space between items
extern const char applicationTitle[30];  // Space between items

extern int inputLineRow;  // Input line row
extern int sourceAreaStart;  // Source area is a on the top
extern int sourceAreaEnd;
extern int targetAreaStart;  // Target area is at the bottom
extern int targetAreaEnd;
extern int workingAreaHeight;



// TODO remove --------------------------------
extern char baseSourcePath[100];  // current source dir path TODO - unlimited
extern char baseTargetPath[100];  // current target dir path TODO - unlimited
typedef struct {  // File representation
    char title[51];
    char lastChange[11];
} TFM_FILE;
// TODO remove --------------------------------



void buildGUI();
void fillWorkingArea(TFM_FILE *files);
void configureConsole();

void setCursorPosition(short col, short row); // Move caret to col row
void handleUserInput();  // Should return user input
void clear(bool navigationOnly);  // Clear console, if navigationOnly - do not clear the user info, only navigation area
void listenKeyboard();