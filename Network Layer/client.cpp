#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
int main() {
    int s;
    char ip[100];
    char out[1000];

    s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(s, (sockaddr*)&server, sizeof(server));

    cout << "Enter IP address: ";
    cin >> ip;

    send(s, ip, strlen(ip) + 1, 0);

    int n = recv(s, out, sizeof(out) - 1, 0);
    out[n] = '\0';

    cout << "\n" << out;

    close(s);

    return 0;
}