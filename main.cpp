#include <iostream>
#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <winuser.h>
#include <windef.h>
#include <map>

#define IsPressed 0x8000

#define cur_thread 0

using namespace std;

bool DoesValGiveChar(int KeyCode) {
    return 16 <= KeyCode && KeyCode <= 255;
}

bool IsKeyPressed(int KeyCode) {

    return (bool)(GetAsyncKeyState(KeyCode) & IsPressed);

}

int main() {

    //This seems inefficient. But if it works, it works
    HKL layout = GetKeyboardLayout(cur_thread);

    char Characters[90] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz1234567890!@#$%¨&*(){}[]~;:/?|'-_=+";
    map<SHORT, char> ValToCharMap;

    for (int ind = 0; ind < 90; ind++) {
        SHORT input_val = VkKeyScanExA(Characters[ind], layout);
        ValToCharMap.insert({input_val, Characters[ind]});
    }

    string Word = "";
    int ind = 0;

    bool PressedKeys[256] = {false};
    bool EnterPressed = false;

    while (!EnterPressed) {

        bool IsShiftPressed = IsKeyPressed(VK_SHIFT);
        bool IsAltPressed = IsKeyPressed(VK_MENU);

        //"An idiot admires complexity, a genius admires simplicity"
        //-Terry Davis

        BYTE KBstate[256]; GetKeyboardState(KBstate);

        for (int KeyCode = 0; KeyCode < 256; KeyCode++) {

            if (IsKeyPressed(KeyCode)) {
                switch (KeyCode) {
                    case VK_RETURN:
                        if (ind > 0) {
                            EnterPressed = true;
                            KeyCode = 255;
                            break;
                        }
                        break;
                    case VK_BACK:
                        if (ind > 0 & !PressedKeys[KeyCode]) {
                            PressedKeys[KeyCode] = true;
                            ind--;
                            Word.pop_back();
                            break;
                        }
                        break;

                    default:
                        if (DoesValGiveChar(KeyCode)) {

                            if (!PressedKeys[KeyCode]) {

                                SHORT KeyVal = KeyCode;
                                KeyVal = (IsShiftPressed) ? KeyVal | 0x100 : KeyVal;
                                KeyVal = (IsAltPressed) ? KeyVal | 0x200 : KeyVal;
                                wchar_t PressedKey = ValToCharMap[(SHORT)KeyVal];

                                PressedKeys[KeyCode] = true;
                                Word.push_back(PressedKey);
                                ind++;
                            }
                        }
                        break;
                }

                
            } else {
                PressedKeys[KeyCode] = false;
            }
        }

    }

    Sleep(100);

    cout << Word << endl;

    return 0;
}