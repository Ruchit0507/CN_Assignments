#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
    int client;
    struct sockaddr_in address;
    char data[200];
    char frame[300];
    int i = 0, j, k = 0, count;
    client = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(client, (struct sockaddr*)&address, sizeof(address));
    printf("Enter data: ");
    scanf(" %[^\n]", data);
    while(data[i] != '\0')
    {
        if(data[i] == ' ')
        {
            frame[k++] = ' ';
            i++;
            continue;
        }
        count = 0;
        j = i;
        while(data[j] != ' ' && data[j] != '\0')
        {
            count++;
            j++;
        }
        frame[k++] = count + '0';
        j = i;
        while(data[j] != ' ' && data[j] != '\0')
        {
            frame[k++] = data[j];
            j++;
        }
        i = j;
    }
    frame[k] = '\0';
    printf("Transmitted Frame: %s\n", frame);
    send(client, frame, k + 1, 0);
    close(client);
    return 0;
}