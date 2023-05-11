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

// procedure to get ssid of IW card - pass socket, interface name, ssid, and wireless request
void getSSID(int socketDescriptor, char* ssid, struct iwreq wireless_req);

// procedure to get power of IW card - pass socket, power, and wireless request
void getIWPower(int socketDescriptor ,char* powerDbm, struct iwreq wireless_req);

// procedure to set power of IW card - pass socket, power in dBM, and wireless request
void setIWPower(int socketDescriptor, int power, struct iwreq wireless_req);

// procedure to check power if user can change
bool checkAllowedPower(int power);

void adaptiveMode(int descriptor,char* power ,struct iwreq wireless_req);



int main(int argc, char** argv){

    struct iwreq wireless_req;
    int descriptor;
    char ssid[IW_ESSID_MAX_SIZE + 1];
    char powerDbm[100];


    memset(&wireless_req, 0, sizeof(struct iwreq));
    memset(ssid, 0, sizeof(ssid));
    wireless_req.u.essid.pointer = ssid;
    wireless_req.u.essid.length = IW_ESSID_MAX_SIZE;
    wireless_req.u.essid.flags = 0;

    
    memset(ssid, 0, sizeof(ssid));

    descriptor = socket(PF_INET, SOCK_DGRAM, 0);
    if(descriptor == -1){
        perror("socket");
        return -1;
    }

    strncpy(wireless_req.ifr_name, argv[1], IFNAMSIZ);

    // run function to get ssid of IW card through ioctl
    getSSID(descriptor, ssid ,wireless_req);

   

    if(ssid != "\0" && powerDbm !="\0"){
        printf("SSID: %s\n", ssid);

        // run function to get power of IW card through ioctl
        getIWPower(descriptor, powerDbm,wireless_req);
    }else{
        printf("SSID: Not found!\n");
        close(descriptor);
        return -1;
    }
    // setIWPower(descriptor, 18, wireless_req);
    // getIWPower(descriptor, powerDbm, wireless_req);
    // printf("Actual power of IW card: %s[dBm]\n", powerDbm);
    
   while (1)
    {     
        printf("What do you want to do? \n");
        printf("1. Set power card manually -> type 's'\n");
        printf("2. Adaptive card monitor mode -> type 'a'\n");
        printf("3. Exit -> type 'e'\n");

        char userChoice;
        printf("Enter your choice: ");
        scanf("%c", &userChoice);

        switch (userChoice)
        {
            case 's':{
                int power;
                printf("Enter power in dBm: ");
                scanf("%d", &power);
                if(checkAllowedPower(power)){
                    setIWPower(descriptor, power, wireless_req);
                    getIWPower(descriptor, powerDbm, wireless_req);
                }else{
                    printf("Power is too high!\n");
                }
                break;
            }

            case 'a':{
                adaptiveMode(descriptor,powerDbm ,wireless_req);
                break;
            }
    
            case 'e':
            exit(1);
            break;
        }
    }
    

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

void getIWPower(int socketDescriptor, char* powerDbm, struct iwreq wireless_req){
    
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
}

void setIWPower(int socketDescriptor, int power, struct iwreq wireless_req){
    wireless_req.u.txpower.value = power;
    wireless_req.u.txpower.fixed = 1;
    wireless_req.u.txpower.disabled = 0;
    wireless_req.u.txpower.flags = IW_TXPOW_DBM;

    if(ioctl(socketDescriptor, SIOCSIWTXPOW, &wireless_req) == -1){
        perror("ioctl");
        close(socketDescriptor);
    }
    printf("Power set to %d dBm\n", power);
}

bool checkAllowedPower(int power){
    if(power > IW_MAX_POWER_POLAND_DBM){
        return false;
    }
    return true;
}

void adaptiveMode(int descriptor,char* power, struct iwreq wireless_req){
    int userChoice = 0;
    printf("Enter min value: ");
    scanf("%d", &userChoice);
    
    while(1){
        
        sleep(5);
        getIWPower(descriptor, power, wireless_req);

        if(atoi(power) < userChoice && atoi(power) == IW_MAX_POWER_POLAND_DBM -1 ){

            printf("I have to power up.\n");
            setIWPower(descriptor, atoi(power) + 1, wireless_req);

        }
    }
    return;
}