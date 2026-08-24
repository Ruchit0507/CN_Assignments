#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
    int server, client;
    struct sockaddr_in address;
    char frame[300];
    int i;
    int count = 0;
    server = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;
    bind(server, (struct sockaddr*)&address, sizeof(address));
    listen(server, 5);
    printf("Waiting for client...\n");
    client = accept(server, NULL, NULL);
    recv(client, frame, sizeof(frame), 0);
    printf("Received Frame: %s\n", frame);
    printf("Received Data: ");
    i = 8;
    while(frame[i + 8] != '\0')
    {
        if(frame[i] == '1')
        {
            printf("1");
            count++;
            if(count == 5)
            {
                i++;
                count = 0;
            }
        }
        else
        {
            printf("0");
            count = 0;
        }
        i++;
    }
    printf("\n");
    close(client);
    close(server);
    return 0;
}