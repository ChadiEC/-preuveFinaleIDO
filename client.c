#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pigpio.h>

// port utilisé
#define PORT 8888
// ip du conteur
#define DEST_IP "10.10.0.174"
// message si le bouton est appuyé
#define MESSAGE_ON "1\n"
//message si le bouton est a son état normal(pas appuyé)
#define MESSAGE_OFF "0\n"
// size du buffer
#define BUFFER_SIZE 1024
//addresse i2c
#define ADR_I2C 0x70
//addresse de la matrice
#define ADR_SYS_MATRICE 0x20
// l'addresse d'affichage de la matrice
#define ADR_AFFICHAGE_MATRICE 0x80
// Pin gpio du bouton(signal)
#define BTN_GPIO 17 

int main() {
    // handle de la matrice
    int handle;

    // Initialisation de pigpio
    if (gpioInitialise() < 0) {
        printf("Erreur d'initialisation pigpio\n");
        return 1;
    }

    // Configuration du GPIO pour le bouton
    gpioSetMode(BTN_GPIO, PI_INPUT);
    gpioSetPullUpDown(BTN_GPIO, PI_PUD_UP); // Utiliser la résistance de pull-up interne

    // Récupération du handle de la matrice
    handle = i2cOpen(1, ADR_I2C, 0);
    if (handle < 0) {
        printf("Erreur d'accès à la matrice LED\n");
        gpioTerminate();
        return 1;
    }

    // Initialiser la matrice
    i2cWriteByteData(handle, ADR_SYS_MATRICE | 1, 0x00);
    i2cWriteByteData(handle, ADR_AFFICHAGE_MATRICE | 1, 0x00);

    int sock = 0, value;
    struct sockaddr_in dest_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Création du socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Erreur de création du socket\n");
        gpioTerminate();
        return 1;
    }

    // Initialisation la structure de l'adresse IP
    memset(&dest_addr, '0', sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
    dest_addr.sin_port = htons(PORT);

    

    // Création de la connexion
    if (connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        printf("Erreur de connexion\n");
        close(sock);
        gpioTerminate();
        return 1;
    }

    while (1) {
        
        // Lire l'état du bouton
        int btn_state = gpioRead(BTN_GPIO);

        // Envoyer l'état de l'équipe au conteneur

        //Bouton appuyé
        if (btn_state == 0) { 
            send(sock, MESSAGE_ON, strlen(MESSAGE_ON), 0);
        } else if (btn_state == 1) { 
            // Bouton off
            send(sock, MESSAGE_OFF, strlen(MESSAGE_OFF), 0);
        }

        // Attendre la réponse du conteneur
        value = read(sock, buffer, BUFFER_SIZE);
        if (value > 0) {
            printf("Message reçu :%s\n", buffer);
            time_sleep(0.5);
            
            // Stockage de l'équipe et de l'état dans des variables
            char team = buffer[0];
            char state = buffer[2];

            // Structuration de la logique pour allumer les LED de la matrice selon les informations reçues
            if (team == '1') {
                if(state == '1'){
                i2cWriteByteData(handle, 0x00, 0x80);
                }else if(state == '0'){
                i2cWriteByteData(handle, 0x00, 0x00);
                }

            } else if (team == '2') {
                if(state == '1'){
                i2cWriteByteData(handle, 0x00, 0x40);
                }else if (state == '0'){
                i2cWriteByteData(handle, 0x00, 0x00);
                }

            } else if (team == '3') {
                if(state == '1'){
                i2cWriteByteData(handle, 0x00, 0x20);
                }if (state == '0'){
                i2cWriteByteData(handle, 0x00, 0x00);
                }

            } else if (team == '4') {
                if(state == '1') {
                i2cWriteByteData(handle, 0x00, 0x16);
                }else if(state == '0'){
                i2cWriteByteData(handle, 0x00, 0x00);
                }

            } else if (team == '5') {
                if (state == '1') {
                i2cWriteByteData(handle, 0x00, 0x08);
                }else if(state == '0'){
                i2cWriteByteData(handle, 0x00, 0x00);
                }
            } else if (team == '6') {
                if (state == '1'){
                i2cWriteByteData(handle, 0x00, 0x04);
                }else if (state == '0'){
                i2cWriteByteData(handle, 0x00, 0x00);
                }
            } else if (team == '7') {
                if(state == '1'){
                i2cWriteByteData(handle, 0x00, 0x02);
                }else if (state == '0'){
                i2cWriteByteData(handle, 0x00, 0x00);
                }

            } else if (team == '8') {
                if (state == '1'){
                i2cWriteByteData(handle, 0x00, 0x01);
                }else if (state == '0'){
                i2cWriteByteData(handle, 0x00, 0x00);
                }

            } else if (team == '9') {
                if (state == '1') {
                i2cWriteByteData(handle, 0x02, 0x80);
                }else if (state == '0'){
                i2cWriteByteData(handle, 0x02, 0x00);
                }
            }
        }

    
        time_sleep(0.5);
        
    }

    // Nettoyage
    i2cClose(handle);
    gpioTerminate();
    close(sock);
    return 0;
}



