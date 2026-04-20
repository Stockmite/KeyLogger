#include <winsock2.h>
#include <windows.h>
#include <ws2def.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>

#define Buffer_lenght 256
#define queue_len 1

int main() {

    WSADATA wsadata;
    int StartupResults = WSAStartup(MAKEWORD(2,2), &wsadata);
    if (StartupResults) {
        perror("WSAStartup failed: ");
        return 1;
    }

    SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(8080);

    bind(ServerSocket, (SOCKADDR*)&addr, sizeof(addr));

    listen(ServerSocket, queue_len);

    SOCKET ClientSocket = accept(ServerSocket, 0, 0);

    char databuf[Buffer_lenght] = {'\0'};
    int result = recv(ClientSocket, databuf, Buffer_lenght, 0);

    if (result == SOCKET_ERROR) {
        printf("Error occured: %d\n", WSAGetLastError());
        return 1;
    }

    FILE* password_storage = fopen("passwords.txt", "a");
    fputs(databuf, password_storage);
    fputc(' ', password_storage);

}