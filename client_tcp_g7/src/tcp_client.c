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
    else if (strcmp(type, "SERVER") == 0) {
        printf("[SERVER] ");
    }
    else if (strcmp(type, "INPUT") == 0) {
        printf("[INPUT] ");
    }
    else if (strcmp(type, "ERROR") == 0) {
        printf("[ERROR] ");
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
// FUNZIONE PRINCIPALE
// ===========================================
int main() {
    WSADATA wsa;
    SOCKET sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = { 0 };

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

    printMessage("INFO", "Inizializzazione client TCP...");

    // Inizializzazione Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printMessage("ERROR", "WSAStartup fallito!");
        return 1;
    }

    printMessage("SUCCESS", "Winsock inizializzato correttamente");

    // Creazione socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printMessage("ERROR", "Creazione socket fallita!");
        WSACleanup();
        return -1;
    }

    printMessage("SUCCESS", "Socket creato correttamente");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converti indirizzo IP
    if (inet_addr("127.0.0.1") == INADDR_NONE) {
        printMessage("ERROR", "Indirizzo IP non valido");
        closesocket(sock);
        WSACleanup();
        return -1;
    }
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connessione al server
    printMessage("INFO", "Tentativo di connessione al server 127.0.0.1:8080");

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printMessage("ERROR", "Connessione al server fallita!");
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    printMessage("SUCCESS", "Connesso al server!");
    printSeparator();

    system("cls");
    // Stampa la stringa ricevuta con un box ASCII
    printf("\n");
    printf("+--------------------------------------------------+\n");
    printf("|                      CLIENT                      |\n");
    printf("+--------------------------------------------------+\n");

    // Invia messaggio "Hello"
    printMessage("SERVER", "Invio messaggio di saluto...");
    send(sock, "Hello", 5, 0);

    // Legge stringa da input
    printMessage("INPUT", "Digita una stringa da inviare al server:");
    printf("\n");
    printf("+--------------------------------------------------+\n");
    printf("|                 INSERISCI TESTO                  |\n");
    printf("+--------------------------------------------------+\n");
    printf("   ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Rimuove newline
    printf("+--------------------------------------------------+\n");

    // Invia stringa al server
    printMessage("SERVER", "Invio stringa al server...");
    send(sock, buffer, strlen(buffer), 0);

    // Riceve risposta dal server
    printMessage("SERVER", "In attesa di risposta dal server...");
    int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';

        printf("\n");
        printf("+--------------------------------------------------+\n");
        printf("|           RISPOSTA RICEVUTA DAL SERVER           |\n");
        printf("+--------------------------------------------------+\n");
        printf("                                                    \n");
        printf("        Stringa modificata:                         \n");
        printf("        %s", buffer);
        for(int i = strlen(buffer); i < 30; i++) printf(" ");
        printf("         \n");
        printf("                                                    \n");
        printf("+--------------------------------------------------+\n\n");

        printMessage("SUCCESS", "Dati ricevuti correttamente dal server");
    }

    printSeparator();
    printMessage("INFO", "Chiusura connessione...");

    closesocket(sock);
    WSACleanup();

    printf("\n");
    printMessage("SUCCESS", "Client terminato correttamente!");
    printf("\nPremi un tasto per uscire...");
    getchar();

    return 0;
}
