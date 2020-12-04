#include <windows.h>
#include "gui.h"
#include "core.h"

using namespace std;


int main() {
    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];
    setlocale(LC_ALL,"");

    refresh();  // Show folder
    while (true) {  // MAIN EVENT LOOP
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
                case KEY_EVENT: // Keyboard input
                    keyEventProc(irInBuf[i].Event.KeyEvent);
                    break;
                case WINDOW_BUFFER_SIZE_EVENT:  // Window resize
                    doDimensions();  // Count sizes
                    buildGUI();  // Static GUI
                    fillWorkingArea();  // Files
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
