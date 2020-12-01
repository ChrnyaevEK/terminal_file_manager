#include <iostream>
#include <windows.h>
#include <conio.h>
#include <clocale>
#include "gui.h"
#include "core.h"

using namespace std;


int main() {
    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];

    configureConsole(); // Configure console attributes and count dimensions
    configureSystem();
    doDimensions();
    buildGUI();

    // Loop to read and handle the next 500 input events.
    while (true) {
        // Wait for the events.
        if (!ReadConsoleInput(
                hStdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead))  // number of records read
            errorExit("ReadConsoleInput");

        // Dispatch the events to the appropriate handler.
        for (i = 0; i < cNumRead; i++) {
            switch (irInBuf[i].EventType) {
                case KEY_EVENT: // keyboard input
                    keyEventProc(irInBuf[i].Event.KeyEvent);
                    doDimensions();
                    buildGUI();
                    break;


                case WINDOW_BUFFER_SIZE_EVENT: // disregard scrn buf. resizing
                    doDimensions();
                    buildGUI();
                    break;
                case MOUSE_EVENT: // disregard mouse input
                case FOCUS_EVENT:  // disregard focus events
                case MENU_EVENT:   // disregard menu events
                    break;

                default:
                    errorExit("Unknown event type");
                    break;
            }
        }
        if (STATE::exit) break;
    }
    return 0;
}
