#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <asm/types.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include "netdevice.h"
//#include <linux/netdevice.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>
#include <netinet/ether.h>


struct {
    struct nlmsghdr nh;
    struct ifinfomsg ifi;
} struReq;


int main()
{
    int  nSocket, nLen, nAttrLen;
    char szBuffer[4096];


    struct sockaddr_nl struAddr;
    struct nlmsghdr *pstruNL;
    struct ifinfomsg *pstruIF;
    struct rtattr *pstruAttr;
    struct net_device_stats *pstruInfo;
    struct ether_addr *pstruEther;

    /** 创建一个PF_NETLINK的SOCKET,使用NETLINK_ROUTE协议 */
    nSocket = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (nSocket < 0)
    {
        fprintf(stderr, "error when creating socket %s\n", strerror(errno));
        return(2);
    }

    /** 绑定地址 */
    memset(&struAddr, 0 , sizeof(struAddr));
    struAddr.nl_family = AF_NETLINK;

    struAddr.nl_pid = getpid();
    struAddr.nl_groups = 0 ;

    if (bind(nSocket, (struct sockaddr *)&struAddr, sizeof(struAddr)) < 0 )
    {
        fprintf(stderr, "bind socket error %s \n", strerror(errno));
        return -1;
    }

    /* *   发送一个请求 */

    memset(&struReq, 0 , sizeof(struReq));
    struReq.nh.nlmsg_len = NLMSG_LENGTH(sizeof(struReq));
    struReq.nh.nlmsg_type = RTM_GETLINK;
    struReq.nh.nlmsg_flags = NLM_F_REQUEST|NLM_F_DUMP;
    struReq.ifi.ifi_family = AF_UNSPEC;
    memset(&struAddr, 0 , sizeof(struAddr));
    struAddr.nl_family = AF_NETLINK;
    struAddr.nl_pid = 0;
    struAddr.nl_groups = 0;
    if (sendto(nSocket, &struReq, struReq.nh.nlmsg_len, 0 , (struct sockaddr *)&struAddr, sizeof(struAddr)) < 0 ) {
        fprintf(stderr , "error sending packet: %s \n" , strerror(errno));
        return(-1);
    }

    /** 循环接收数据,直到超时 */
    alarm(30);
    memset(szBuffer, 0, sizeof(szBuffer));

    while ((nLen = recv(nSocket, szBuffer, sizeof(szBuffer), 0))) {

        alarm(0);
        pstruNL = (struct  nlmsghdr *)szBuffer;
        /* *   判断是否继续有数据 */
        while (NLMSG_OK(pstruNL, nLen)) {
            /* *   数据已经获取完成 */
            if (pstruNL -> nlmsg_type == NLMSG_DONE)
                break ;

            if (pstruNL -> nlmsg_type == NLMSG_ERROR) {
                /* *   发生一个错误 */
                struct nlmsgerr *pstruError;
                pstruError = (struct nlmsgerr *)NLMSG_DATA(pstruNL);
                fprintf(stderr, "error [%s]\n", strerror(-pstruError -> error));
                break;
            }

            /*   下面通过宏获取数据 */
            pstruIF = NLMSG_DATA(pstruNL);
            fprintf(stderr, "device captured [%d] message\n ", pstruIF->ifi_index);
            fprintf(stderr, "\t device type:");

            switch (pstruIF -> ifi_type) {
            case  ARPHRD_ETHER:
                fprintf(stderr , "ethernet \n" );
                break;
            case  ARPHRD_PPP:
                fprintf(stderr , "PPP\n" );
                break;
            case  ARPHRD_LOOPBACK:
                fprintf(stderr , "loop device\n");
                break;
            default :
                fprintf(stderr , "unknown\n");
                break;
            }


            fprintf(stderr , "\t device status:" );
            if ((pstruIF -> ifi_flags & IFF_UP )== IFF_UP)
                fprintf(stderr , "up" );
            if ((pstruIF -> ifi_flags & IFF_BROADCAST) == IFF_BROADCAST)
                fprintf(stderr , "broadcast" );
            if ((pstruIF -> ifi_flags & IFF_DEBUG) == IFF_DEBUG)
                fprintf(stderr , "debug" );
            if ((pstruIF -> ifi_flags & IFF_LOOPBACK) == IFF_LOOPBACK)
                fprintf(stderr , "loopback" );
            if ((pstruIF -> ifi_flags & IFF_POINTOPOINT) == IFF_POINTOPOINT)
                fprintf(stderr , "point-to-point" );
            if ((pstruIF -> ifi_flags & IFF_RUNNING) == IFF_RUNNING)
                fprintf(stderr , "running" );
            if ((pstruIF -> ifi_flags & IFF_NOARP) == IFF_NOARP)
                fprintf(stderr , "noarp" );
            if ((pstruIF -> ifi_flags & IFF_PROMISC) == IFF_PROMISC)
                fprintf(stderr , "promisc" );
            if ((pstruIF -> ifi_flags & IFF_NOTRAILERS) == IFF_NOTRAILERS)
                fprintf(stderr , "notrailers" );
            if ((pstruIF -> ifi_flags & IFF_ALLMULTI) == IFF_ALLMULTI)
                fprintf(stderr , "allmulti" );
            if ((pstruIF -> ifi_flags & IFF_MASTER) == IFF_MASTER)
                fprintf(stderr , "master" );
            if ((pstruIF -> ifi_flags & IFF_SLAVE) == IFF_SLAVE)
                fprintf(stderr , "slave" );
            if ((pstruIF -> ifi_flags & IFF_MULTICAST) == IFF_MULTICAST)
                fprintf(stderr , "multicast" );
            if ((pstruIF -> ifi_flags & IFF_PORTSEL) == IFF_PORTSEL)
                fprintf(stderr , "slave" );
            if ((pstruIF -> ifi_flags & IFF_AUTOMEDIA) == IFF_AUTOMEDIA)
                fprintf(stderr , "automedia" );
            if ((pstruIF -> ifi_flags & IFF_DYNAMIC) == IFF_DYNAMIC)
                fprintf(stderr , "dynamic" );

            fprintf(stderr , "\n" );
            /**   下面通过宏获取属性 */
            pstruAttr = IFLA_RTA(pstruIF);
            nAttrLen = NLMSG_PAYLOAD(pstruNL, sizeof(struct ifinfomsg));
            while (RTA_OK(pstruAttr, nAttrLen))
            {
                switch (pstruAttr->rta_type)
                {
                case  IFLA_IFNAME:
                    fprintf(stderr , "\t device name: %s \n", (char *)RTA_DATA(pstruAttr));
                    break ;
                case  IFLA_MTU:
                    fprintf(stderr , "\t device mtu: %d \n",  *(unsigned  int*)RTA_DATA(pstruAttr));
                    break;

                case IFLA_QDISC:
                    fprintf(stderr , "\t device line: %s \n " ,
                            (char *)RTA_DATA(pstruAttr));
                    break;
                case IFLA_ADDRESS:
                    if (pstruIF -> ifi_type == ARPHRD_ETHER)
                    {
                        pstruEther = (struct ether_addr *) RTA_DATA(pstruAttr);
                        fprintf(stderr , "\t MAC: %s \n" , ether_ntoa(pstruEther));
                    }
                    break;
                case IFLA_BROADCAST:
                    if (pstruIF -> ifi_type == ARPHRD_ETHER)
                    {
                        pstruEther = (struct ether_addr *) RTA_DATA(pstruAttr);
                        fprintf(stderr , "\t broadcast MAC : %s \n" , ether_ntoa(pstruEther));
                    }
                    break;
                case IFLA_STATS:
                    pstruInfo = (struct net_device_stats*) RTA_DATA(pstruAttr);
                    fprintf(stderr, "\t recv msg: \n");

                    fprintf(stderr, "\t\t recv packet: %lu byte: %lu \n", pstruInfo->rx_packets, pstruInfo->rx_bytes);
                    fprintf(stderr, "\t\t errors: %lu  dropped: %lu" "multicast:%lu collisions: %lu\n",
                            pstruInfo->rx_errors, pstruInfo->rx_dropped, pstruInfo->multicast, pstruInfo->collisions);

                    fprintf(stderr , "\t\t length : %lu over : %lu   crc: %lu "
                            "frame : %lu   fifo : %lu  missed : %lu\n " ,
                            pstruInfo -> rx_length_errors,
                            pstruInfo -> rx_over_errors,
                            pstruInfo -> rx_crc_errors,
                            pstruInfo -> rx_frame_errors,
                            pstruInfo -> rx_fifo_errors,
                            pstruInfo -> rx_missed_errors);
                    fprintf(stderr , " \t send message: \n" );
                    fprintf(stderr , " \t \t send packets: %lu  byte: %lu \n" , pstruInfo -> tx_packets, pstruInfo -> tx_bytes);
                    fprintf(stderr , " \t \t errors : %lu   dropped: %lu \n", pstruInfo -> tx_errors, pstruInfo -> tx_dropped);
                    fprintf(stderr , " \t \t aborted : %lu  carrier : %lu   fifo: %lu "
                            " heartbeat: %lu   window :%lu \n " ,
                            pstruInfo -> tx_aborted_errors,
                            pstruInfo -> tx_carrier_errors,
                            pstruInfo -> tx_fifo_errors,
                            pstruInfo -> tx_heartbeat_errors,
                            pstruInfo -> tx_window_errors);
                    break;
                default:
                    break;
                }

                /**   继续下一个属性 */
                pstruAttr = RTA_NEXT(pstruAttr, nAttrLen);
            }

            /**   继续下一个数据 */
            pstruNL = NLMSG_NEXT(pstruNL, nLen);
        }


        memset(szBuffer, 0 , sizeof(szBuffer));
        alarm(30);
    }

    return(0) ;
}





