#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
    int client;
    struct sockaddr_in address;
    char data[100];
    char frame[300];
    int i, j = 0;
    int count = 0;
    client = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(client, (struct sockaddr*)&address, sizeof(address));
    printf("Enter binary data: ");
    scanf("%s", data);
    frame[j++] = '0';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '0';
    for(i = 0; data[i] != '\0'; i++)
    {
        frame[j++] = data[i];
        if(data[i] == '1')
        {
            count++;
            if(count == 5)
            {
                frame[j++] = '0';
                count = 0;
            }
        }
        else
        {
            count = 0;
        }
    }
    frame[j++] = '0';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '1';
    frame[j++] = '0';
    frame[j] = '\0';
    printf("Transmitted Frame: %s\n", frame);
    send(client, frame, sizeof(frame), 0);
    close(client);
    return 0;
}