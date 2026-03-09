#include <iostream>
#include <windows.h>
#include <wchar.h>
#include <winuser.h>
#include <windef.h>

#define IsPressed 0x8000
#define shiftcode1 16
#define shiftcode2 160

using namespace std;

bool DoesValGiveChar(int KeyCode) {
    return 48 <= KeyCode && KeyCode <= 90;
}

int main() {

    char Word[21];
    int ind = 0;

    bool PressedKeys[52] = {false};

    while (ind < 100) {
        bool MakeUppercase = GetAsyncKeyState(shiftcode1) & GetAsyncKeyState(shiftcode2) & IsPressed;

        //"An idiot admires complexity, a genius admires simplicity"
        //-Terry Davis
        for (int KeyCode = 0; KeyCode < 255; KeyCode++) {
            if ((GetAsyncKeyState(KeyCode) & IsPressed)) {
                if (DoesValGiveChar(KeyCode)) {
                    int charIndex = KeyCode - 48;

                    if (!PressedKeys[charIndex]) {
                        char SelectedChar = static_cast<char>(KeyCode);
                        char PressedKey = (MakeUppercase) ? SelectedChar : tolower(SelectedChar);

                        PressedKeys[charIndex] = true;
                        printf("%c", PressedKey);
                        ind++;
                    }
                }

                
            } else {
                if (DoesValGiveChar(KeyCode)) {
                    int charIndex = KeyCode - 48;

                    if (PressedKeys[charIndex]) {

                        PressedKeys[charIndex] = false;
                    }
                }
            }
        }

    }

    return 0;
}