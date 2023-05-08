#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>

char getSSID(int socketDescriptor, char* interfaceName){

    struct iwreq wireless_req;
    char ssid[IW_ESSID_MAX_SIZE + 1];

    memset(&wireless_req, 0, sizeof(struct iwreq));
    memset(ssid, 0, sizeof(ssid));
    wireless_req.u.essid.pointer = ssid;
    wireless_req.u.essid.length = IW_ESSID_MAX_SIZE;
    iwreq.u.essid.flags = 0;

    strncpy(wireless_req.ifr_name, interfaceName, IFNAMSIZ);

    if(ioctl(socketDescriptor, SIOCGIWESSID, &wireless_req) == -1){
        perror("ioctl");
        close(socketDescriptor)
        return "\0";
    }

    strncpy(ssid, wireless_req.u.essid.pointer, sizeof(ssid));
    return ssid;
}


int main(int argc, char** argv){
    int descriptor;
    char ssid[IW_ESSID_MAX_SIZE + 1];
    
    memset(ssid, 0, sizeof(ssid));

    descriptor = socket(PF_INET, SOCK_DGRAM, 0);
    if(descriptor == -1){
        perror("socket");
        return -1;
    }
    ssid = getSSID(descriptor, argv[1]);
    if(ssid != "\0")
        printf("SSID: %s\n", ssid);
    else
        printf("SSID: Not found!\n");

    close(descriptor);
    return EXIT_SUCCESS;
}

