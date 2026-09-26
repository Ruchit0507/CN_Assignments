#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int main(){
int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

sockaddr_in serverAddress;

serverAddress.sin_family = AF_INET;
serverAddress.sin_port = htons(5000);
serverAddress.sin_addr.s_addr = INADDR_ANY;

bind(serverSocket, (struct sockadder*)&serverAddress, sizeof(serverAddress));

listen(serverSocket, 5);

int clientSocket = accept(serverSocket, NULL, NULL);

char buffer[1024];
int byteRecerived = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

if(byteRecerived > 0){
    buffer[byteRecerived] = '\0';
    cout << "Client" << buffer << endl;
}
string reply = "Hello from Server";

send(clientSocket, reply.c_str(), reply.length(), 0);

close(clientSocket);
close(serverSocket);

return 0;
}