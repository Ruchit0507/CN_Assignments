#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAX_FRAMES 100

int main()
{
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server, client;
    int clientLen = sizeof(client);

    int received[MAX_FRAMES] = {0};
    int frame;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == INVALID_SOCKET)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sock,
             (struct sockaddr *)&server,
             sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("=================================\n");
    printf("     SELECTIVE REPEAT SERVER\n");
    printf("=================================\n");
    printf("Waiting for frames...\n\n");

    while (1)
    {
        int bytes = recvfrom(
            sock,
            (char *)&frame,
            sizeof(frame),
            0,
            (struct sockaddr *)&client,
            &clientLen
        );

        if (bytes == SOCKET_ERROR)
            continue;

        if (frame == -1)
        {
            printf("\nTransmission completed.\n");
            break;
        }

        if (frame < 0 || frame >= MAX_FRAMES)
            continue;

        printf("Received Frame %d\n", frame);

        if (received[frame] == 0)
        {
            received[frame] = 1;

            printf(
                "Frame %d accepted and buffered\n",
                frame
            );
        }
        else
        {
            printf(
                "Duplicate Frame %d\n",
                frame
            );
        }

        sendto(
            sock,
            (char *)&frame,
            sizeof(frame),
            0,
            (struct sockaddr *)&client,
            clientLen
        );

        printf("ACK %d sent\n\n", frame);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}