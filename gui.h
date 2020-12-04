#pragma once
#define HACKER_MODE true
#define DEBUG

typedef void(*FUN)(); // Navigation function pointer
typedef struct {  // Navigation item
    const char title[10];  //  Keyboard shortcut to use
    const char description[30]; // Navigation button action description
    //    FUN handler;  // Handler  TODO - bind handler
} NAVIGATION_ITEM;


namespace DIM {
    extern int mainWindowHeight;
    extern int mainWindowWidth;
    extern const char applicationTitle[30];  // Space between items
    typedef struct {
        int COL;
        int ROW;
    } COORD;
    typedef struct {
        int START;
        int END;
    } AREA;
    extern COORD inputLine;  // Input line
    extern AREA workingArea;  // Source area is a on the top
    extern int workingAreaHeight;
}


extern HANDLE hStdOut;
extern HANDLE hStdin;

extern CONSOLE_SCREEN_BUFFER_INFO csbInfo; // Information about console window
extern WORD wokWindowAttributes;
extern WORD activeItemAttributes;  // Active navigation item


void buildGUI();

void fillWorkingArea();
void cleanWorkingArea();
void changeWorkingDirectory();
void createNewFile();
void removeFile();
void renameFile();

void configureConsole();

void doDimensions();

void clear();
void cleanLine();

void setCursorPosition(short col, short row); // Move caret to col row

void errorMsg(LPCSTR);
void errorExit(LPCSTR);

void keyEventProc(KEY_EVENT_RECORD);