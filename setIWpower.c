#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>
#include <stdbool.h>

#define IW_MAX_POWER_POLAND_DBM 20

// procedure to get ssid of IW card - interface name, ssid, and wireless request
void getSSID(int socketDescriptor, char* ssid, struct iwreq wireless_req);

// procedure to get power of IW card - power, and wireless request
void getIWPower(char* powerDbm, struct iwreq wireless_req);

// procedure to set power of IW card - power in dBM, and wireless request
void setIWPower(int power, struct iwreq wireless_req);

// procedure to check power if user can change
bool checkAllowedPower(int power);

// procedure to adaptive mode of IW card controlling
void adaptiveMode(char* power, struct iwreq wireless_req);

// utils function to create new descriptor
int createDescriptor();

// get frequency of IW card
void getIWFrequency(struct iwreq wireless_req);


int main(int argc, char** argv){

    // declaration of structure to operate on Wireless interface and local variables
    struct iwreq wireless_req;
    int descriptor;
    char ssid[IW_ESSID_MAX_SIZE + 1];
    char powerDbm[100];

    // reset variables
    memset(&wireless_req, 0, sizeof(struct iwreq));
    memset(ssid, 0, sizeof(ssid));
    memset(powerDbm, 0, sizeof(powerDbm));

    // fill structure
    wireless_req.u.essid.pointer = ssid;
    wireless_req.u.essid.length = IW_ESSID_MAX_SIZE;
    wireless_req.u.essid.flags = 0;

    descriptor = createDescriptor();

    if(descriptor == -1){
        perror("socket");
        return -1;
    }

    //pass interface name to structure
    strncpy(wireless_req.ifr_name, argv[1], IFNAMSIZ);

    // run function to get ssid of IW card through ioctl
    getSSID(descriptor, ssid ,wireless_req);

    if(strlen(ssid) != 0){
        printf("SSID: %s\n", ssid);

        // run function to get power and frequency of IW card through ioctl
        getIWPower(powerDbm,wireless_req);
        getIWFrequency(wireless_req);
    }else{
        printf("SSID: Not found!\n");
        close(descriptor);
        descriptor = -1;
        return -1;
    }
    
    printf("What do you want to do? \n");
    printf("1. Set power card manually -> type 's'\n");
    printf("2. Adaptive card monitor mode -> type 'a'\n");
    printf("3. Exit -> type 'e'\n");
    printf("========================================\n");

    char userChoice;
    printf("Enter your choice: ");
    scanf("%c", &userChoice);

    switch (userChoice){
        case 's':{
            printf("Enter power: ");
            int power;
            scanf("%d", &power);
            if(checkAllowedPower(power)){
                setIWPower(power, wireless_req);
                getIWPower(powerDbm, wireless_req);
            }else{
                printf("Power is too high!\n");
            }
            break;
        }

        case 'a':{
            adaptiveMode(powerDbm ,wireless_req);
            break;
        }

        case 'e':
        exit(1);
        break;
    }
    printf("========================================\n");
    
    close(descriptor);
    return EXIT_SUCCESS;
}

void getSSID(int socketDescriptor, char* ssid, struct iwreq wireless_req){

    if(ioctl(socketDescriptor, SIOCGIWESSID, &wireless_req) == -1){
        perror("ioctl");
        close(socketDescriptor);
    }

    strncpy(ssid, wireless_req.u.essid.pointer, sizeof(ssid));
}

void getIWPower(char* powerDbm, struct iwreq wireless_req){

    int socketDescriptor = createDescriptor();
    if(socketDescriptor == -1){
        perror("socket");
        return;
    }
    
    wireless_req.u.txpower.flags = IW_TXPOW_DBM;
    wireless_req.u.txpower.fixed = 0;

    char powerInStr[100];
    

    if(ioctl(socketDescriptor, SIOCGIWTXPOW, &wireless_req) == -1){
        perror("ioctl");
        close(socketDescriptor);
    }
    snprintf(powerInStr, sizeof(powerInStr), "%d", wireless_req.u.txpower.value);

    strncpy(powerDbm, powerInStr, sizeof(powerDbm));
    printf("Power IW card: %s[dBm]\n", powerDbm);
    return;
}

void setIWPower(int power, struct iwreq wireless_req){

    int socketDescriptor = createDescriptor();

    if(socketDescriptor == -1){
        perror("socket");
        return;
    }

    // TODO: to investigate this flags

    wireless_req.u.txpower.value = power;
    wireless_req.u.txpower.fixed = 1;
    wireless_req.u.txpower.disabled = 0;
    wireless_req.u.txpower.flags = IW_TXPOW_DBM;

    if(ioctl(socketDescriptor, SIOCSIWTXPOW, &wireless_req) == -1){
        perror("ioctl");
        close(socketDescriptor);
    }
}


void adaptiveMode(char* power, struct iwreq wireless_req){

    int socketDescriptor = createDescriptor();

    if(socketDescriptor == -1){
        perror("socket");
        return;
    }

    int userChoice = 0;
    printf("Enter min value: ");
    scanf("%d", &userChoice);

    printf("Adaptive mode started.\n");
    printf("========================================\n");
    
    while(1){
        
        sleep(5);
        getIWPower(power, wireless_req);

        if(atoi(power) < userChoice || atoi(power) == IW_MAX_POWER_POLAND_DBM -1 ){

            printf("I have to power up.\n");
            setIWPower(atoi(power) + 1, wireless_req);
        }
    }
}

bool checkAllowedPower(int power){
    if(power < 0 || power > IW_MAX_POWER_POLAND_DBM){
        return false;
    }
    return true;
}


int createDescriptor(){
    int descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    return descriptor;
}

void getIWFrequency(struct iwreq wireless_req){

    int socketDescriptor = createDescriptor();
    if(socketDescriptor == -1){
        perror("socket");
        return;
    }

    char powerInStr[100];
    
    if(ioctl(socketDescriptor, SIOCGIWFREQ, &wireless_req) == -1){
        perror("ioctl");
        close(socketDescriptor);
    }
    snprintf(powerInStr, sizeof(powerInStr), "%d", wireless_req.u.freq.m);

    // strncpy(powerDbm, powerInStr, sizeof(powerDbm));
    printf("Frequency IW card: %s[GHz]\n", powerInStr);
    return;
}