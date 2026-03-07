#include <iostream>
#include <windows.h>
#include <wchar.h>
#include <winuser.h>
#include <windef.h>

#define IsPressed 0x8000

using namespace std;

bool DoesKeyCodeGiveChar(int KeyCode) {
    return false;
}

int main() {

    char Word[21];
    int ind = 0;

    unsigned char PressedKeys = 0x0;

    while (ind < 21) {
        //"An idiot admires complexity, a genius admires simplicity"
        //-Terry Davis
        for (int KeyCode = 0; KeyCode < 255; KeyCode++) {
            if (GetAsyncKeyState(KeyCode) & IsPressed) {

            }
        }

    }

    return 0;
}