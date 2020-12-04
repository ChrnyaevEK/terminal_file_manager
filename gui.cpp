#include <windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "core.h"
#include "gui.h"
#include "handlers.h"

using namespace std;
HANDLE hStdOut;  // Console out
HANDLE hStdin;  // Console in
CONSOLE_SCREEN_BUFFER_INFO csbInfo; // Information about console window
WORD wokWindowAttributes = (HACKER_MODE) ? 0b00001010 : 0b00001111;  // Hacker mode ...
WORD activeItemAttributes = 160;  // Selected file mode

const char DIM::applicationTitle[30] = "[TFM] Terminal File Manager";  // App title
int DIM::mainWindowHeight;  // Console height
int DIM::mainWindowWidth;  // Console width
int DIM::workingAreaHeight;  // Working area height
DIM::COORD DIM::inputLine;  // Input line
DIM::AREA DIM::workingArea;  // Working area

NAVIGATION_ITEM navigation[] = {
        {"CTRL + P", "Change path"},
        {"CTRL + N", "Create new file"},
        {"CTRL + D", "Delete file"},
        {"CTRL + R", "Rename file"},
};

void doDimensions() {  // Function count sizes for current run
    GetConsoleScreenBufferInfo(hStdOut, &csbInfo);  // Get info
    DIM::mainWindowWidth = csbInfo.srWindow.Right - csbInfo.srWindow.Left + 1;  // Count console width
    DIM::mainWindowHeight = csbInfo.srWindow.Bottom - csbInfo.srWindow.Top + 1;  // Count console height
    DIM::workingAreaHeight = DIM::mainWindowHeight - 4 - size(navigation);  // Count working area height
    DIM::inputLine = {0, short(3 + DIM::workingAreaHeight + size(navigation))};  // Input row
    DIM::workingArea = {size(navigation) + 3,
                        short(size(navigation) + 3 + DIM::workingAreaHeight)};  // Count working area start end
}

void configureConsole() {  // Set up console parameters
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
    if (hStdin == INVALID_HANDLE_VALUE)
        errorExit("GetStdHandle");

    // Enable the window and mouse input events.
    if (!SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS))
        errorExit("Could not do SetConsoleMode");
}

void buildGUI() {  // Build static part of GUI
    clear();  // Clean console
    SetConsoleTextAttribute(hStdOut, wokWindowAttributes);  // Set test attrib.
    int cursorRow = 0;

    if (DIM::mainWindowHeight < 10) errorExit("Your window is too small...\n");  // Too small window?

    cout << DIM::applicationTitle;  // Title

    // ------------------------------------------------------------------------------------------------ Divider
    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int i = 0; i != DIM::mainWindowWidth; i++) { cout << '-'; }

    // ------------------------------------------------------------------------------------------------ Navigation items
    cursorRow++;
    setCursorPosition(0, cursorRow);
    for (int itemIndex = 0; itemIndex != size(navigation); itemIndex++) {  // Draw navigation
        NAVIGATION_ITEM item = navigation[itemIndex];
        setCursorPosition(0, cursorRow++);
        cout << item.title << "     " << item.description;
    }

    // ------------------------------------------------------------------------------------------------ Divider
    setCursorPosition(0, cursorRow);
    for (int i = 0; i != DIM::mainWindowWidth; i++) { cout << '-'; }
    cursorRow += DIM::workingAreaHeight;
    setCursorPosition(0, cursorRow);

    // ------------------------------------------------------------------------------------------------ Divider
    for (int i = 0; i != DIM::mainWindowWidth; i++) { cout << '-'; }
    // ------------------------------------------------------------------------------------------------------ Input line
    cursorRow++;
    setCursorPosition(0, cursorRow);  // Input line
}

void cleanWorkingArea() {  // Clean only working area, not static GUI
    SetConsoleTextAttribute(hStdOut, wokWindowAttributes);
    for (int pos = DIM::workingArea.START; pos != DIM::workingArea.END - 1; pos++) {
        setCursorPosition(0, pos);
        printf("%c[2K", 27);
    }
}


void fillWorkingArea() {  // Fill working area with files
    int pos = DIM::workingArea.START;
    int wait = STATE::fileIndex;  // Show selected file as first
    setCursorPosition(0, pos++);
    cout << "Path   ";
    for (int i = 0; i != DIM::mainWindowWidth; i++) { cout << STATE::path[i]; }
    for (auto ri = STATE::files.rbegin(); ri != STATE::files.rend(); ri++) {
        if (wait) {  // Wait for selected file
            wait--;
            continue;
        }
        if (pos == DIM::workingArea.START + 1) {  // Highlight first file
            SetConsoleTextAttribute(hStdOut, activeItemAttributes);  // Highlight
        } else {
            SetConsoleTextAttribute(hStdOut, wokWindowAttributes);
        }
        if (pos >= DIM::workingArea.END - 1) break;  // Do not write out ouf working are
        setCursorPosition(0, pos);  // Set cursor at source area start
        printf(" - %s", (*ri).c_str());  // Print file name
        pos++;
    }
}

// Sets cursor position
void setCursorPosition(short col, short row) {
    SetConsoleCursorPosition(hStdOut, COORD{col, row});
}

void clear() {
    system("cls"); // Clear console with system command
}

void refresh(){  // Build whole GUI again
    clear();
    configureSystem();  // Read data
    configureConsole();  // Setup console
    doDimensions();  // Count sizes
    buildGUI();  // Static GUI
    fillWorkingArea();  // Files
}

void stdMsgOut(LPCSTR Message) {  // Standard msg output
    setCursorPosition(DIM::inputLine.COL, DIM::inputLine.ROW);
    SetConsoleTextAttribute(hStdOut, wokWindowAttributes);  // Set test attrib.
    printf("%s", Message);
}


void errorExit(LPCSTR Message) {  // Print msg and exit the program
    printf("%s\n", Message);
    system("pause");
    exit(0);
}


void keyEventProc(KEY_EVENT_RECORD ker) {  // HANDLES KEYBOARD EVENTS
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