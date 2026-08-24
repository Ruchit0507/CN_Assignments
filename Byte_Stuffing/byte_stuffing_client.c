#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
    int client;
    struct sockaddr_in address;
    char data[100];
    char frame[200];
    int i, j = 0;
    client = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(client, (struct sockaddr*)&address, sizeof(address));
    printf("Enter data: ");
    scanf("%s", data);
    frame[j++] = 'F';
    for(i = 0; data[i] != '\0'; i++)
    {
        if(data[i] == 'F' || data[i] == 'E')
        {
            frame[j++] = 'E';
        }

        frame[j++] = data[i];
    }
    frame[j++] = 'F';
    frame[j] = '\0';
    printf("Transmitted Frame: %s\n", frame);
    send(client, frame, sizeof(frame), 0);
    close(client);
    return 0;
}