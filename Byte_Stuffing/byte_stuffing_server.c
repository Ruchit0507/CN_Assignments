#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
    int server, client;
    struct sockaddr_in address;
    char frame[200];
    int i;
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
    i = 1;
    while(frame[i] != 'F')
    {
        if(frame[i] == 'E')
        {
            i++;
        }
        printf("%c", frame[i]);
        i++;
    }
    printf("\n");
    close(client);
    close(server);
    return 0;
}