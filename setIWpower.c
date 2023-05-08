#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>

// procedure to get ssid of IW card - pass socket, interface name, ssid, and wireless request
void getSSID(int socketDescriptor, char* interfaceName, char* ssid, struct iwreq wireless_req){

    strncpy(wireless_req.ifr_name, interfaceName, IFNAMSIZ);

    if(ioctl(socketDescriptor, SIOCGIWESSID, &wireless_req) == -1){
        perror("ioctl");
        close(socketDescriptor);
    }

    strncpy(ssid, wireless_req.u.essid.pointer, sizeof(ssid));
}


int main(int argc, char** argv){

    struct iwreq wireless_req;
    int descriptor;
    char ssid[IW_ESSID_MAX_SIZE + 1];


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

    // run function to get ssid of IW card through ioctl
    getSSID(descriptor, argv[1], ssid ,wireless_req);

    if(ssid != "\0")
        printf("SSID: %s\n", ssid);
    else
        printf("SSID: Not found!\n");

    close(descriptor);
    return EXIT_SUCCESS;
}

