#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define IP_Target "192.168.56.1"
#define Port_Target 9000

// KLIENT

int main() {
    int status;

    SOCKET gniazdo1;
    struct sockaddr_in ser;
    char buf[1024];

    WSADATA wsaData; // przed inicjalizacją gniazd
    if (WSAStartup( MAKEWORD( 2, 0 ), &wsaData )){printf("blad WSDATA\n"); return 0;}

    gniazdo1 = socket(AF_INET, SOCK_STREAM, 0);
    if(gniazdo1==-1) {printf("blad socket\n");}

    ser.sin_family = AF_INET;
    ser.sin_port = htons(Port_Target);
    ser.sin_addr.s_addr = inet_addr(IP_Target);

    status = connect(gniazdo1, (struct sockaddr *) &ser, sizeof(ser)); // łączenie gniazdo1
    if (status < 0) { printf("blad connect\n");}

    while(1) {
        status = recv(gniazdo1, buf, (sizeof buf) - 1,0); // odbieram 200 znaków, status "zapisuje od 0 do 198" do "buf"
        if (status <= 0) { printf("blad recv\n"); break;}
        buf[status] = '\0';
        if (buf[0]=='Q') {
            char *wiadomosc = buf+2;
            printf("Wynk gry: %s\n", wiadomosc);
            break;
        }

        printf("Plansza:#%s#\n", buf);
        printf("Poddaj sie: Q\npodaj ruch: i/j\n");
        scanf("%s", buf);

        status = send(gniazdo1, buf, strlen(buf),0); // wysyła z gniazda1 zawartość buffa o długości bufa na podany port adresu IP
        if (strcmp(buf, "Q") == 0) {printf("Zakonczyles polaczenie\n"); break;}
    }
    closesocket(gniazdo1);
    WSACleanup();
    printf("program sie skonczyl\n");
    int x;
    scanf("%d", &x);
    return 0;

}