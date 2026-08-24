#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main()
{
    WSADATA wsa;
    SOCKET serverSocket;
    struct sockaddr_in server, client;
    int clientLen = sizeof(client);
    int frame;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(serverSocket, (struct sockaddr *)&server, sizeof(server));

    printf("Server started...\n");
    printf("Waiting for frames...\n\n");

    while (1)
    {
        recvfrom(
            serverSocket,
            (char *)&frame,
            sizeof(frame),
            0,
            (struct sockaddr *)&client,
            &clientLen
        );

        if (frame == -1)
            break;

        printf("Received: %d\n", frame);

        Sleep(500);

        sendto(
            serverSocket,
            (char *)&frame,
            sizeof(frame),
            0,
            (struct sockaddr *)&client,
            clientLen
        );

        printf("ACK sent: %d\n\n", frame);
    }

    printf("Done.\n");

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}