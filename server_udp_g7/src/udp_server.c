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
    printf("   SSSSSSSSSSSSSSS                                                                                                          \n");
    printf(" SS:::::::::::::::S                                                                                                         \n");
    printf("S:::::SSSSSS::::::S                                                                                                         \n");
    printf("S:::::S     SSSSSSS                                                                                                         \n");
    printf("S:::::S                eeeeeeeeeeee    rrrrr   rrrrrrrrr   vvvvvvv           vvvvvvv    eeeeeeeeeeee    rrrrr   rrrrrrrrr   \n");
    printf("S:::::S              ee::::::::::::ee  r::::rrr:::::::::r   v:::::v         v:::::v   ee::::::::::::ee  r::::rrr:::::::::r  \n");
    printf(" S::::SSSS          e::::::eeeee:::::eer:::::::::::::::::r   v:::::v       v:::::v   e::::::eeeee:::::eer:::::::::::::::::r \n");
    printf("  SS::::::SSSSS    e::::::e     e:::::err::::::rrrrr::::::r   v:::::v     v:::::v   e::::::e     e:::::err::::::rrrrr::::::r\n");
    printf("    SSS::::::::SS  e:::::::eeeee::::::e r:::::r     r:::::r    v:::::v   v:::::v    e:::::::eeeee::::::e r:::::r     r:::::r\n");
    printf("       SSSSSS::::S e:::::::::::::::::e  r:::::r     rrrrrrr     v:::::v v:::::v     e:::::::::::::::::e  r:::::r     rrrrrrr\n");
    printf("            S:::::Se::::::eeeeeeeeeee   r:::::r                  v:::::v:::::v      e::::::eeeeeeeeeee   r:::::r            \n");
    printf("            S:::::Se:::::::e            r:::::r                   v:::::::::v       e:::::::e            r:::::r            \n");
    printf("SSSSSSS     S:::::Se::::::::e           r:::::r                    v:::::::v        e::::::::e           r:::::r            \n");
    printf("S::::::SSSSSS:::::S e::::::::eeeeeeee   r:::::r                     v:::::v          e::::::::eeeeeeee   r:::::r            \n");
    printf("S:::::::::::::::SS   ee:::::::::::::e   r:::::r                      v:::v            ee:::::::::::::e   r:::::r            \n");
    printf(" SSSSSSSSSSSSSSS       eeeeeeeeeeeeee   rrrrrrr                       vvv               eeeeeeeeeeeeee   rrrrrrr            \n");
    printf("\n");
}

void removeVowels(char *str) {
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (!(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')) {
            str[j++] = c;
        }
    }
    str[j] = '\0';
}

// ===========================================
// FUNZIONE PRINCIPALE
// ===========================================
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Utilizzo: %s <porta>\n", argv[0]);
        return 1;
    }

    WSADATA wsa;
    int port = atoi(argv[1]);
    SOCKET sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int len = sizeof(cliaddr);
    char buffer[BUFFER_SIZE];

    system("cls");
    printBanner();
    system("pause");

    system("cls");

    // Stampa la stringa ricevuta con un box ASCII
    printf("\n");
    printf("+--------------------------------------------------+\n");
    printf("|                      SERVER                      |\n");
    printf("+--------------------------------------------------+\n");


    // Inizializzazione Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    // Creazione socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // Binding
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("Server UDP in ascolto sulla porta %d...\n", port);

    while (1) {
        // Riceve messaggio "Hello"
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
            (struct sockaddr*)&cliaddr, &len);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Ricevuti dati dal client indirizzo: %s\n",
                inet_ntoa(cliaddr.sin_addr));
        }

        // Riceve stringa dal client
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
            (struct sockaddr*)&cliaddr, &len);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Stringa ricevuta dal client: %s\n", buffer);

            // Rimuove vocali
            removeVowels(buffer);

            // Invia stringa modificata al client
            sendto(sockfd, buffer, strlen(buffer), 0,
                (struct sockaddr*)&cliaddr, len);
        }
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
