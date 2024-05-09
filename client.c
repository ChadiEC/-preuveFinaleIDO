#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define DEST_IP "10.10.0.174"
#define MESSAGE "hello!\n"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, value;
    struct sockaddr_in dest_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    // Création du socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // Initialisation la structure de l'adresse IP 
    memset(&dest_addr, '0', sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
    dest_addr.sin_port = htons(PORT);

    // Création de la connexion
    connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    
    // Envoi du message
    send(sock, MESSAGE, strlen(MESSAGE), 0);

    // Attente de la réponse
    value = read(sock, buffer, BUFFER_SIZE);
    printf("Message reçu : %s\n", buffer);

    close(sock);
    return 0;
}
