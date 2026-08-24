#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define TIMEOUT 2000

int main()
{
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    int serverLen = sizeof(server);

    int totalFrames;
    int windowSize;
    int lostFrame;

    int base = 0;
    int nextFrame = 0;
    int lossDone = 0;

    printf("=================================\n");
    printf("        GO-BACK-N CLIENT\n");
    printf("=================================\n");

    printf("Enter number of frames: ");
    scanf("%d", &totalFrames);

    printf("Enter window size: ");
    scanf("%d", &windowSize);

    printf("Enter frame to lose once (-1 for no loss): ");
    scanf("%d", &lostFrame);

    if (lostFrame < 0 || lostFrame >= totalFrames)
        lostFrame = -1;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == INVALID_SOCKET)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    {
        DWORD timeout = TIMEOUT;

        setsockopt(
            sock,
            SOL_SOCKET,
            SO_RCVTIMEO,
            (char *)&timeout,
            sizeof(timeout)
        );
    }

    printf("\nClient started.\n\n");

    while (base < totalFrames)
    {
        while (
            nextFrame < base + windowSize &&
            nextFrame < totalFrames
        )
        {
            if (
                nextFrame == lostFrame &&
                lossDone == 0
            )
            {
                printf("Frame %d LOST\n", nextFrame);
                lossDone = 1;
            }
            else
            {
                printf("Sending Frame %d\n", nextFrame);

                sendto(
                    sock,
                    (char *)&nextFrame,
                    sizeof(nextFrame),
                    0,
                    (struct sockaddr *)&server,
                    sizeof(server)
                );
            }

            nextFrame++;
        }

        {
            int ack;

            int bytes = recvfrom(
                sock,
                (char *)&ack,
                sizeof(ack),
                0,
                (struct sockaddr *)&server,
                &serverLen
            );

            if (bytes != SOCKET_ERROR)
            {
                printf("ACK %d received\n", ack);

                if (ack >= base)
                    base = ack + 1;
            }
            else
            {
                printf("\nTIMEOUT for Frame %d\n", base);
                printf(
                    "Go-Back-N retransmitting from Frame %d\n\n",
                    base
                );

                nextFrame = base;
            }
        }
    }

    {
        int endSignal = -1;

        sendto(
            sock,
            (char *)&endSignal,
            sizeof(endSignal),
            0,
            (struct sockaddr *)&server,
            sizeof(server)
        );
    }

    printf("\nAll frames transmitted successfully.\n");

    closesocket(sock);
    WSACleanup();

    return 0;
}