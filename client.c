#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pigpio.h>


#define PORT 8888
#define DEST_IP "10.10.0.174"
#define MESSAGE "hello!\n"
#define BUFFER_SIZE 1024
#define ADR_I2C 0x70
#define ADR_SYS_MATRICE 0x20
#define ADR_AFFICHAGE_MATRICE 0x80

int main() {

    int handle;

    //Initialisation de pigpio
     if (gpioInitialise() < 0) {
        printf("Erreur d'initialisation pigpio\n");
        return 1;
    }

    //Récupération du handle de la matrice
    handle = i2cOpen(1, ADR_I2C, 0);
    
    if (handle < 0) {
        printf("Erreur d'accès à la matrice LED\n");
        gpioTerminate();
        return 1;
    }
    //Initialiser la matrice
    i2cWriteByteData(handle, ADR_SYS_MATRICE | 1, 0x00);
    i2cWriteByteData(handle, ADR_AFFICHAGE_MATRICE | 1, 0x00);

    //Création des variables à utiliser
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

    
    //Stockage de l'équipe et de l'état dans des variable
    char team = buffer[0];
    char state = buffer[2];

    //Structuration de la logique pour allumer les led de matrice selon les informations reçues
    if(team == '1' && state == '1'){
        i2cWriteByteData(handle, 0x00, 0x80);
        time_sleep(1);
        i2cWriteByteData(handle, 0x00, 0x00);
    }else if(team == '2' && state == '1'){
        i2cWriteByteData(handle, 0x00, 0x40);
        time_sleep(1);
        i2cWriteByteData(handle, 0x00, 0x00);
    }else if(team == '3' && state == '1'){
        i2cWriteByteData(handle, 0x00, 0x20);
        time_sleep(1);
        i2cWriteByteData(handle, 0x00, 0x00);
    }else if(team == '4' && state == '1'){
        i2cWriteByteData(handle, 0x00, 0x16);
        time_sleep(1);
        i2cWriteByteData(handle, 0x00, 0x00);   
    }else if(team == '5' && state == '1'){
        i2cWriteByteData(handle, 0x00, 0x08);
        time_sleep(1);
        i2cWriteByteData(handle, 0x00, 0x00);        
    }else if(team == '6' && state == '1'){
        i2cWriteByteData(handle, 0x00, 0x04);
        time_sleep(1);
        i2cWriteByteData(handle, 0x00, 0x00);        
    }else if(team == '7' && state == '1'){
        i2cWriteByteData(handle, 0x00, 0x02);
        time_sleep(1);
        i2cWriteByteData(handle, 0x00, 0x00);        
    }else if(team == '8' && state == '1'){
        i2cWriteByteData(handle, 0x00, 0x01);
        time_sleep(1);
        i2cWriteByteData(handle, 0x00, 0x00);       
    }else if(team == '9' && state == '1'){
        i2cWriteByteData(handle, 0x02, 0x80);
        time_sleep(1);
        i2cWriteByteData(handle, 0x02, 0x00);       
    }else{
        i2cClose(handle);
        gpioTerminate();
    }
    close(sock);
    return 0;

    
}
