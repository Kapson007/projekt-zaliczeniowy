#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/wireless.h>
#include <sys/ioctl.h>



int main(int argc, char** argv){
    int descriptor;
    struct iwreq wireless_req;
    char ssid[IW_ESSID_MAX_SIZE + 1];
    memset(ssid, 0, sizeof(ssid));
    memset(&wireless_req, 0, sizeof(struct iwreq));
    wireless_req.u.essid.pointer = ssid;
    wireless_req.u.essid.length = IW_ESSID_MAX_SIZE;
    iwreq.u.essid.flags = 0;

    descriptor = socket(PF_INET, SOCK_DGRAM, 0);
    if(descriptor == -1){
        perror("socket");
        return -1;
    }

    strncpy(wireless_req.ifr_name, argv[1], IFNAMSIZ);
    

    if(ioctl(descriptor, SIOCGIWESSID, &wireless_req) == -1){
        perror("ioctl");
        close(descriptor)
        return -1;
    }
    strncpy(ssid, wireless_req.u.essid.pointer, sizeof(ssid));
    printf("SSID: %s\n", ssid);

    close(descriptor);
    return EXIT_SUCCESS


}

