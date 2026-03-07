#include <iostream>
#include <windows.h>
#include <wchar.h>
#include <winuser.h>
#include <windef.h>

#define IsPressed 0x8000

using namespace std;

int main() {

    char Word[21];
    int ind = 0;

    unsigned char PressedKeys;

    while (ind < 21) {
        //"An idiot admires complexity, a genius admires simplicity"
        //-Terry Davis
        for (int keyCode = 0; keyCode < 256; ++keyCode) {
            if (GetAsyncKeyState(keyCode) & IsPressed) {


            }
        }

    }

    return 0;
}