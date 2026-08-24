#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define TIMEOUT 2000

int main()
{
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in server;
    int serverLen = sizeof(server);

    int totalFrames;
    int frame;
    int ack;

    srand((unsigned int)time(NULL));

    WSAStartup(MAKEWORD(2, 2), &wsa);

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    {
        DWORD timeout = TIMEOUT;

        setsockopt(
            clientSocket,
            SOL_SOCKET,
            SO_RCVTIMEO,
            (char *)&timeout,
            sizeof(timeout)
        );
    }

    printf("Stop and Wait ARQ\n\n");

    printf("Enter number of frames: ");
    scanf("%d", &totalFrames);

    printf("\n");

    for (frame = 0; frame < totalFrames; frame++)
    {
        int acknowledged = 0;
        int firstTry = 1;

        while (!acknowledged)
        {
            DWORD start;
            DWORD end;
            DWORD timeTaken;

            printf("Sending %d\n", frame);

            start = GetTickCount();

            if (firstTry && (rand() % 100) < 30)
            {
                printf("Frame %d lost\n", frame);

                firstTry = 0;

                Sleep(TIMEOUT);

                end = GetTickCount();
                timeTaken = end - start;

                printf("Timeout after %lu ms\n",
                       (unsigned long)timeTaken);

                printf("Sending %d again\n\n", frame);

                continue;
            }

            firstTry = 0;

            sendto(
                clientSocket,
                (char *)&frame,
                sizeof(frame),
                0,
                (struct sockaddr *)&server,
                sizeof(server)
            );

            {
                int bytes = recvfrom(
                    clientSocket,
                    (char *)&ack,
                    sizeof(ack),
                    0,
                    (struct sockaddr *)&server,
                    &serverLen
                );

                end = GetTickCount();
                timeTaken = end - start;

                if (bytes != SOCKET_ERROR && ack == frame)
                {
                    printf("ACK %d received\n", ack);
                    printf("Time: %lu ms\n\n",
                           (unsigned long)timeTaken);

                    acknowledged = 1;
                }
                else
                {
                    printf("Timeout\n");
                    printf("Sending %d again\n\n", frame);
                }
            }
        }
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

    printf("Transmission complete.\n");

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}