#include <iostream>
#include <cstring>
#include <cmath>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int s, ns;
    char ip[100];
    char out[1000];

    s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server, client;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (sockaddr*)&server, sizeof(server));
    listen(s, 5);

    cout << "Server waiting...\n";

    socklen_t len = sizeof(client);
    ns = accept(s, (sockaddr*)&client, &len);

    recv(ns, ip, sizeof(ip), 0);
    ip[99] = '\0';

    int a, b, c, d;

    if (sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d) != 4 ||
        a < 0 || a > 255 ||
        b < 0 || b > 255 ||
        c < 0 || c > 255 ||
        d < 0 || d > 255) {

        strcpy(out, "Invalid IP address\n");
        send(ns, out, strlen(out), 0);

        close(ns);
        close(s);
        return 0;
    }

    char cls;
    int nid, hid;
    long long networks, hosts;
    int na, nb, nc, nd;
    int ba, bb, bc, bd;

    if (a >= 1 && a <= 126) {
        cls = 'A';
        nid = 8;
        hid = 24;
        networks = 126;
        hosts = pow(2, 24) - 2;

        na = a;
        nb = 0;
        nc = 0;
        nd = 0;

        ba = a;
        bb = 255;
        bc = 255;
        bd = 255;
    }
    else if (a >= 128 && a <= 191) {
        cls = 'B';
        nid = 16;
        hid = 16;
        networks = pow(2, 14);
        hosts = pow(2, 16) - 2;

        na = a;
        nb = b;
        nc = 0;
        nd = 0;

        ba = a;
        bb = b;
        bc = 255;
        bd = 255;
    }
    else if (a >= 192 && a <= 223) {
        cls = 'C';
        nid = 24;
        hid = 8;
        networks = pow(2, 21);
        hosts = pow(2, 8) - 2;

        na = a;
        nb = b;
        nc = c;
        nd = 0;

        ba = a;
        bb = b;
        bc = c;
        bd = 255;
    }
    else if (a >= 224 && a <= 239) {
        strcpy(out, "Class D\nNID and HID not applicable\n");
        send(ns, out, strlen(out), 0);

        close(ns);
        close(s);
        return 0;
    }
    else {
        strcpy(out, "Class E\nNID and HID not applicable\n");
        send(ns, out, strlen(out), 0);

        close(ns);
        close(s);
        return 0;
    }

    int la = na;
    int lb = nb;
    int lc = nc;
    int ld = nd + 1;

    sprintf(out,
        "Class: %c\n"
        "NID bits: %d\n"
        "HID bits: %d\n"
        "No. of networks: %lld\n"
        "No. of hosts: %lld\n"
        "IP Range: %d.%d.%d.%d - %d.%d.%d.%d\n"
        "L.B.A: %d.%d.%d.%d\n"
        "D.B.A: %d.%d.%d.%d\n",
        cls,
        nid,
        hid,
        networks,
        hosts,
        na, nb, nc, nd,
        ba, bb, bc, bd,
        la, lb, lc, ld,
        ba, bb, bc, bd
    );

    send(ns, out, strlen(out), 0);

    close(ns);
    close(s);

    return 0;
}