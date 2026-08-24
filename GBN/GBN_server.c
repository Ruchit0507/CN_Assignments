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

    int expected = 0;
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

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("=================================\n");
    printf("        GO-BACK-N SERVER\n");
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

        printf("Received Frame %d\n", frame);

        if (frame == expected)
        {
            printf("Frame %d accepted\n", frame);

            sendto(
                sock,
                (char *)&frame,
                sizeof(frame),
                0,
                (struct sockaddr *)&client,
                clientLen
            );

            printf("ACK %d sent\n\n", frame);

            expected++;
        }
        else
        {
            printf("Frame %d discarded\n", frame);

            if (expected > 0)
            {
                int ack = expected - 1;

                sendto(
                    sock,
                    (char *)&ack,
                    sizeof(ack),
                    0,
                    (struct sockaddr *)&client,
                    clientLen
                );

                printf("Duplicate ACK %d sent\n\n", ack);
            }
        }
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}