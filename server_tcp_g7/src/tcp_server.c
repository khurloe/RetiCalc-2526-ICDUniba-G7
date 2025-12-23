#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#define PORT 8080
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

// ===========================================
// FUNZIONE PER STAMPARE UN MESSAGGIO CON STILE
// ===========================================
void printMessage(const char* type, const char* message) {
    time_t now;
    time(&now);
    struct tm* local = localtime(&now);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", local);

    printf("[%s] ", timestamp);

    if (strcmp(type, "INFO") == 0) {
        printf("[INFO] ");
    }
    else if (strcmp(type, "SUCCESS") == 0) {
        printf("[SUCCESS] ");
    }
    else if (strcmp(type, "CLIENT") == 0) {
        printf("[CLIENT] ");
    }
    else if (strcmp(type, "ERROR") == 0) {
        printf("[ERROR] ");
    }
    else if (strcmp(type, "SERVER") == 0) {
        printf("[SERVER] ");
    }

    printf("%s\n", message);
}

// ===========================================
// FUNZIONE PER STAMPARE UN SEPARATORE
// ===========================================
void printSeparator() {
    printf("\n");
    for(int i = 0; i < 60; i++) printf("-");
    printf("\n\n");
}

// ===========================================
// FUNZIONE PER RIMUOVERE VOCALI DA UNA STRINGA
// ===========================================
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
int main() {
    WSADATA wsa;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = { 0 };

    // Pulisci lo schermo e stampa il banner
    system("cls");
    printBanner();
    system("pause");

    system("cls");

    // Stampa la stringa ricevuta con un box ASCII
    printf("\n");
    printf("+--------------------------------------------------+\n");
    printf("|                      SERVER                      |\n");
    printf("+--------------------------------------------------+\n");

    printMessage("INFO", "Inizializzazione server TCP...");

    // Inizializzazione Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printMessage("ERROR", "WSAStartup fallito!");
        return 1;
    }

    printMessage("SUCCESS", "Winsock inizializzato correttamente");

    // Creazione socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printMessage("ERROR", "Creazione socket fallita!");
        WSACleanup();
        return 1;
    }

    printMessage("SUCCESS", "Socket creato correttamente");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        printMessage("ERROR", "Bind fallito!");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printMessage("SUCCESS", "Bind effettuato sulla porta 8080");

    // Listening
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printMessage("ERROR", "Listen fallito!");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printMessage("INFO", "Server in ascolto sulla porta 8080");
    printSeparator();

    printMessage("SERVER", "Pronto per accettare connessioni client...");
    printSeparator();

    while (1) {
        // Accettazione connessione
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) == INVALID_SOCKET) {
            printMessage("ERROR", "Accettazione connessione fallita!");
            continue;
        }

        system("cls");
        // Stampa la stringa ricevuta con un box ASCII
        printf("\n");
        printf("+--------------------------------------------------+\n");
        printf("|                      SERVER                      |\n");
        printf("+--------------------------------------------------+\n");

        printMessage("CLIENT", "==============================================");
        printMessage("CLIENT", "Nuovo client connesso!");

        // Riceve messaggio "Hello"
        int bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            char client_info[100];
            sprintf(client_info, "Indirizzo IP: %s", inet_ntoa(address.sin_addr));
            printMessage("CLIENT", client_info);
        }

        // Riceve stringa dal client
        bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';

            // Stampa la stringa ricevuta con un box ASCII
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("|            STRINGA RICEVUTA DAL CLIENT           |\n");
            printf("+--------------------------------------------------+\n");
            printf("   %-46s                                           \n", buffer);
            printf("+--------------------------------------------------+\n");

            // Rimuove vocali
            char original[BUFFER_SIZE];
            strcpy(original, buffer);
            removeVowels(buffer);

            // Mostra il risultato della trasformazione
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("|            TRASFORMAZIONE VOCALI                 |\n");
            printf("+--------------------------------------------------+\n");
            printf("   Originale:  %-34s                               \n", original);
            printf("   Senza vocali: %-33s                             \n", buffer);
            printf("+--------------------------------------------------+\n\n");

            // Invia stringa modificata al client
            send(new_socket, buffer, strlen(buffer), 0);
            printMessage("SERVER", "Stringa modificata inviata al client");
        }

        printMessage("CLIENT", "Connessione chiusa");
        printMessage("CLIENT", "==============================================");
        printSeparator();
        printMessage("SERVER", "In attesa di nuovi client...");
        printSeparator();

        closesocket(new_socket);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
