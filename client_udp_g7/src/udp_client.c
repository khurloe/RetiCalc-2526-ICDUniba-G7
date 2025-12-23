#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define BUFFER_SIZE 1024

// ===========================================
// FUNZIONE PER DISEGNARE LA LOCANDINA ASCII
// ===========================================
void printBanner() {
    printf("\n");
    printf("        CCCCCCCCCCCCClllllll   iiii                                                 tttt\n");
    printf("     CCC::::::::::::Cl:::::l  i::::i                                             ttt:::t\n");
    printf("   CC:::::::::::::::Cl:::::l   iiii                                              t:::::t\n");
    printf("  C:::::CCCCCCCC::::Cl:::::l                                                     t:::::t\n");
    printf(" C:::::C       CCCCCC l::::l iiiiiii     eeeeeeeeeeee    nnnn  nnnnnnnn    ttttttt:::::ttttttt\n");
    printf("C:::::C               l::::l i:::::i   ee::::::::::::ee  n:::nn::::::::nn  t:::::::::::::::::t\n");
    printf("C:::::C               l::::l  i::::i  e::::::eeeee:::::een::::::::::::::nn t:::::::::::::::::t\n");
    printf("C:::::C               l::::l  i::::i e::::::e     e:::::enn:::::::::::::::ntttttt:::::::tttttt\n");
    printf("C:::::C               l::::l  i::::i e:::::::eeeee::::::e  n:::::nnnn:::::n      t:::::t\n");
    printf("C:::::C               l::::l  i::::i e:::::::::::::::::e   n::::n    n::::n      t:::::t\n");
    printf("C:::::C               l::::l  i::::i e::::::eeeeeeeeeee    n::::n    n::::n      t:::::t\n");
    printf(" C:::::C       CCCCCC l::::l  i::::i e:::::::e             n::::n    n::::n      t:::::t    tttttt\n");
    printf("  C:::::CCCCCCCC::::Cl::::::li::::::ie::::::::e            n::::n    n::::n      t::::::tttt:::::t\n");
    printf("   CC:::::::::::::::Cl::::::li::::::i e::::::::eeeeeeee    n::::n    n::::n      tt::::::::::::::t\n");
    printf("     CCC::::::::::::Cl::::::li::::::i  ee:::::::::::::e    n::::n    n::::n        tt:::::::::::tt\n");
    printf("        CCCCCCCCCCCCClllllllliiiiiiii    eeeeeeeeeeeeee    nnnnnn    nnnnnn          ttttttttttt\n");
    printf("\n");
}

// ===========================================
// FUNZIONE PRINCIPALE
// ===========================================
int main() {
    WSADATA wsa;
    char server_hostname[100];
    char server_port_str[10];
    SOCKET sockfd;
    struct sockaddr_in servaddr;
    int len = sizeof(servaddr);
    char buffer[BUFFER_SIZE];
    struct hostent *server_host;

    // Pulisci lo schermo e stampa il banner
    system("cls");
    printBanner();
    system("pause");

    system("cls");

    // Stampa la stringa ricevuta con un box ASCII
    printf("\n");
    printf("+--------------------------------------------------+\n");
    printf("|                      CLIENT                      |\n");
    printf("+--------------------------------------------------+\n");

    // Inizializzazione Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    // Lettura nome host e porta del server
    printf("Inserisci il nome host del server: ");
    fgets(server_hostname, sizeof(server_hostname), stdin);
    server_hostname[strcspn(server_hostname, "\n")] = 0;

    printf("Inserisci la porta del server: ");
    fgets(server_port_str, sizeof(server_port_str), stdin);
    server_port_str[strcspn(server_port_str, "\n")] = 0;
    int server_port = atoi(server_port_str);

    // Risoluzione DNS
    server_host = gethostbyname(server_hostname);
    if (server_host == NULL) {
        printf("Errore nella risoluzione dell'hostname\n");
        WSACleanup();
        return 1;
    }

    // Creazione socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port);
    memcpy(&servaddr.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

    // Invia messaggio "Hello"
    sendto(sockfd, "Hello", 5, 0,
        (struct sockaddr*)&servaddr, sizeof(servaddr));

    // Legge stringa da input
    printf("Inserisci una stringa: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    // Invia stringa al server
    sendto(sockfd, buffer, strlen(buffer), 0,
        (struct sockaddr*)&servaddr, sizeof(servaddr));

    // Riceve risposta dal server
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
        (struct sockaddr*)&servaddr, &len);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Stringa '%s' ricevuta dal server indirizzo: %s\n",
            buffer, inet_ntoa(servaddr.sin_addr));
    }

    closesocket(sockfd);
    WSACleanup();
    system("pause");
    return 0;
}
