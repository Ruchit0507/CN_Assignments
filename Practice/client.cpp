#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
using namespace std;
int mzin(){
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);

    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    string message;
    cout << "Enter Message: ";
    getline(cin, message);
    
    send(clientSocket, message.c_str(), message.length(), 0);

    char buffer[1024];

    int byteReceived = recv(clientSocket, buffer, sizeof(buffer)-1, 0);
    if(byteReceived > 0){
        buffer[byteReceived] = '\0';
        cout << "Server" << buffer << endl;
    }
     close(clientSocket);

     return 0;
    
}