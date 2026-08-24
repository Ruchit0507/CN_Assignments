#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main()
{
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in server;
    int serverLen = sizeof(server);
    int n;
    int frame;
    int ack;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("=================================\n");
    printf("       STOP-AND-WAIT CLIENT\n");
    printf("=================================\n");

    printf("Enter number of frames: ");
    scanf("%d", &n);

    printf("\nStarting transmission...\n\n");

    for (frame = 0; frame < n; frame++)
    {
        printf("Sending Frame %d...\n", frame);

        Sleep(1000);

        sendto(
            clientSocket,
            (char *)&frame,
            sizeof(frame),
            0,
            (struct sockaddr *)&server,
            sizeof(server)
        );

        recvfrom(
            clientSocket,
            (char *)&ack,
            sizeof(ack),
            0,
            (struct sockaddr *)&server,
            &serverLen
        );

        printf("ACK %d received\n\n", ack);
    }

    frame = -1;

    sendto(
        clientSocket,
        (char *)&frame,
        sizeof(frame),
        0,
        (struct sockaddr *)&server,
        sizeof(server)
    );

    printf("Transmission completed.\n");

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}