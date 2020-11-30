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

namespace STATE {
    extern int sourceFileIndex;
    extern char *sourcePath;
    extern int targetFileIndex;
    extern char *targetPath;
    extern bool exit;
};

namespace DIM {
    extern int mainWindowHeight;
    extern int mainWindowWidth;
    extern const char applicationTitle[30];  // Space between items
    typedef struct {
        int ROW;
        int COL;
    } COORD;
    typedef struct {
        int START;
        int END;
    } AREA;
    extern COORD inputLine;  // Input line
    extern AREA sourceArea;  // Source area is a on the top
    extern AREA targetArea;  // Target area is at the bottom
    extern int workingAreaHeight;
}



extern HANDLE hStdOut;
extern HANDLE hStdin;

extern CONSOLE_SCREEN_BUFFER_INFO csbInfo; // Information about console window
extern WORD wokWindowAttributes;
extern WORD inactiveItemAttributes;  // Inactive navigation item
extern WORD activeItemAttributes;  // Active navigation item


// TODO remove --------------------------------
extern char baseSourcePath[100];  // current source dir path TODO - unlimited
extern char baseTargetPath[100];  // current target dir path TODO - unlimited
typedef struct {  // File representation
    char title[51];
    char lastChange[11];
} TFM_FILE;
// TODO remove --------------------------------



void buildGUI();
void fillWorkingArea(TFM_FILE *files, DIM::AREA area);
void configureConsole();

void setCursorPosition(short col, short row); // Move caret to col row
void handleUserInput();  // Should return user input
void clear(bool navigationOnly);  // Clear console, if navigationOnly - do not clear the user info, only navigation area
void userInput();
void errorExit(LPCSTR);
void keyEventProc(KEY_EVENT_RECORD);
void mouseEventProc(MOUSE_EVENT_RECORD);