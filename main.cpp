#include <iostream>
#include <windows.h>
#include <wchar.h>
#include <winuser.h>
#include <windef.h>

#define shiftcode1 0x10
#define shiftcode2 0xA0
#define IsPressed 0x8000

using namespace std;

bool DoesValGiveChar(int KeyCode) {
    return 0 <= KeyCode && KeyCode <= 255;
}

int main() {

    char Word[21];
    int ind = 0;

    bool PressedKeys[255] = {false};

    printf("%d\n",ind);

    while (ind < 20) {
        bool MakeUppercase = GetAsyncKeyState(shiftcode1) & GetAsyncKeyState(shiftcode2) & IsPressed;

        //"An idiot admires complexity, a genius admires simplicity"
        //-Terry Davis
        for (int KeyCode = 0; KeyCode < 255; KeyCode++) {
            if (KeyCode==shiftcode1 || KeyCode==shiftcode2) {continue;}
            if ((GetAsyncKeyState(KeyCode) & IsPressed)) {
                if (DoesValGiveChar(KeyCode)) {
                    int charIndex = KeyCode - 48;

                    if (!PressedKeys[charIndex]) {
                        char SelectedChar = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
                        char PressedKey = (MakeUppercase) ? SelectedChar : tolower(SelectedChar);

                        PressedKeys[charIndex] = true;
                        if ((bool)SelectedChar) {
                            printf("%c", PressedKey);
                        }
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

    fflush(stdin);

    Word[20] = '\0';

    printf("%s\n", Word);

    return 0;
}