#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/wireless.h>


//struct to hold collected information
typedef struct signalInfo {
    char mac[18];
    char ssid[33];
    int bitrate;
    int level;
} signalInfo;

typedef struct iw_statistics iw_statistics;
typedef struct ifreq ifreq;



int getSignalInfo(signalInfo *sigInfo, char *iwname) {

    struct iwreq req;
    int s;
    strcpy(req.ifr_name, iwname);

    struct iw_statistics *stats;

    //have to use a socket for ioctl
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //make room for the iw_statistics object
    req.u.data.pointer 
            = (struct iw_statistics *)malloc(sizeof(struct iw_statistics));
    req.u.data.length = sizeof(struct iw_statistics);

    //this will gather the signal strength
    if(ioctl(sockfd, SIOCGIWSTATS, &req) == -1) {
        //die with error, invalid interface
        fprintf(stderr, "Invalid interface.\n");
        return(-1);
    }
    else if(((iw_statistics *)req.u.data.pointer)->qual.updated & IW_QUAL_DBM) {
        //signal is measured in dBm and is valid for us to use
        sigInfo->level=((iw_statistics *)req.u.data.pointer)->qual.level - 256;
    }

    //SIOCGIWESSID for ssid
    char buffer[32];
    memset(buffer, 0, 32);
    req.u.essid.pointer = buffer;
    req.u.essid.length = 32;
    //this will gather the SSID of the connected network
    if(ioctl(sockfd, SIOCGIWESSID, &req) == -1) {
        //die with error, invalid interface
        return(-1);
    }
    else {
        memcpy(&sigInfo->ssid, req.u.essid.pointer, req.u.essid.length);
        memset(&sigInfo->ssid[req.u.essid.length],0,1);
    }

    //SIOCGIWRATE for bits/sec (convert to mbit)
    int bitrate=-1;
    //this will get the bitrate of the link
    if(ioctl(sockfd, SIOCGIWRATE, &req) == -1) {
        fprintf(stderr, "bitratefail");
        return(-1);
    } else {
        memcpy(&bitrate, &req.u.bitrate, sizeof(int));
        sigInfo->bitrate=bitrate/1000000;
    }


    //SIOCGIFHWADDR for mac addr
    ifreq req2;
    strcpy(req2.ifr_name, iwname);
    //this will get the mac address of the interface
    if(ioctl(sockfd, SIOCGIFHWADDR, &req2) == -1) {
        fprintf(stderr, "mac error");
        return(-1);
    }
    else {
        sprintf(sigInfo->mac, "%.2X", (unsigned char)req2.ifr_hwaddr.sa_data[0]);
        for(s = 1; s < 6; s++) {
            sprintf(sigInfo->mac+strlen(sigInfo->mac), ":%.2X", (unsigned char)req2.ifr_hwaddr.sa_data[s]);
        }
    }
    close(sockfd);

}


int main(void) {

   signalInfo si;
   char iwname[] = "wlan0";

   getSignalInfo(&si, iwname);

   printf("MAC: %s\n", si.mac);
   printf("SSID: %s\n", si.ssid);
   printf("bitrate = %d\n", si.bitrate);
   printf("level = %d\n", si.level);


   return 0;
}


