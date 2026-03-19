#include <iostream>
#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <winuser.h>
#include <windef.h>
#include <map>

#define shiftcode1 0x10
#define shiftcode2 0xA0
#define IsPressed 0x8000
#define entercode 0x0D
#define backspace 0x08

#define cur_thread 0

using namespace std;

bool DoesValGiveChar(int KeyCode) {
    return 16 <= KeyCode && KeyCode <= 255;
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

        bool MakeUppercase = GetAsyncKeyState(shiftcode1) & GetAsyncKeyState(shiftcode2) & IsPressed;

        //"An idiot admires complexity, a genius admires simplicity"
        //-Terry Davis

        BYTE KBstate[256]; GetKeyboardState(KBstate);

        for (int KeyCode = 0; KeyCode < 256; KeyCode++) {

            if (KeyCode==shiftcode1 || KeyCode==shiftcode2) {continue;}
            if ((GetAsyncKeyState(KeyCode) & IsPressed)) {
                switch (KeyCode) {
                    case entercode:
                        if (ind > 0) {
                            EnterPressed = true;
                            KeyCode = 255;
                            break;
                        }
                        break;
                    case backspace:
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

                                wchar_t PressedKey = (MakeUppercase) 
                                ? ValToCharMap[(SHORT)KeyCode | 0x100] : ValToCharMap[(SHORT)KeyCode];


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