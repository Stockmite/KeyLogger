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
    return 16 <= KeyCode && KeyCode <= 127;
}

int main() {

    const int max_len = 21;
    char Word[max_len];
    int ind = 0;

    bool PressedKeys[128] = {false};

    while (ind < 20) {
        bool MakeUppercase = GetAsyncKeyState(shiftcode1) & GetAsyncKeyState(shiftcode2) & IsPressed;

        //"An idiot admires complexity, a genius admires simplicity"
        //-Terry Davis
        for (int KeyCode = 0; KeyCode < 128; KeyCode++) {

            if (KeyCode==shiftcode1 || KeyCode==shiftcode2) {continue;}
            if ((GetAsyncKeyState(KeyCode) & IsPressed)) {
                switch (KeyCode) {
                    case shiftcode1:
                        if (ind > 0) {
                            Word[ind] = '\0';
                            ind = max_len;
                            KeyCode = 128;
                            break;
                        }

                    default:
                        if (DoesValGiveChar(KeyCode)) {
                            int charIndex = KeyCode - 48;

                            if (!PressedKeys[charIndex]) {
                                char SelectedChar = MapVirtualKey(KeyCode, MAPVK_VK_TO_CHAR);
                                char PressedKey = (MakeUppercase) ? SelectedChar : tolower(SelectedChar);

                                PressedKeys[charIndex] = true;
                                if ((bool)SelectedChar) {
                                    Word[ind] = PressedKey;
                                }
                                ind++;
                            }
                        }
                        break;
                }

                
            } else {
                if (DoesValGiveChar(KeyCode)) {
                    int charIndex = KeyCode - 48;

                    PressedKeys[charIndex] = false;
                }
            }
        }

    }

    Sleep(100);

    Word[max_len - 1] = '\0';

    printf("%s\n", Word);

    return 0;
}