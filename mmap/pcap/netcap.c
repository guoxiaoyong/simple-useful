/*###############################################################################
 *
 *
使用netfilter的抓包程序，并且保存到文件中。这是根据网上的代码改编的。

其中关于文件操作的解释：
  另外源文件里面要#include   <linux/unistd.h>
  如果报错，很可能是因为使用的缓冲区超过了用户空间的地址范围。一般系统调用会要求你使用的缓冲区不能在内核区。这个可以用set_fs()、 get_fs()来解决。在读写文件前先得到当前fs：
  mm_segment_t   old_fs=get_fs();
  并设置当前fs为内核fs：set_fs(KERNEL_DS);
  在读写文件后再恢复原先fs:   set_fs(old_fs);
  set_fs()、get_fs()等相关宏在文件include/asm/uaccess.h中定义。
  个人感觉这个办法比较简单。

  另外就是用flip_open函数打开文件，得到struct file *的指针fp。使用指针fp进行相应操作，如读文件可以用fp->f_ops->read。最后用filp_close()函数关闭文件。 filp_open()、filp_close()函数在fs/open.c定义，在include/linux/fs.h中声明。

解释一点:
    系统调用本来是提供给用户空间的程序访问的，所以，对传递给它的参数（比如上面的buf），它默认会认为来自用户空间，在 ->write()函数中，为了保护内核空间，一般会用get_fs()得到的值来和USER_DS进行比较，从而防止用户空间程序“蓄意”破坏内核空间；

   而现在要在内核空间使用系统调用，此时传递给->write（）的参数地址就是内核空间的地址了，在USER_DS之上(USER_DS ~ KERNEL_DS)，如果不做任何其它处理，在write()函数中，会认为该地址超过了USER_DS范围，所以会认为是用户空间的“蓄意破坏”，从而不允许进一步的执行； 为了解决这个问题； set_fs(KERNEL_DS);将其能访问的空间限制扩大到KERNEL_DS,这样就可以在内核顺利使用系统调用了！

*/


#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/tcp.h>
#include <linux/netfilter_ipv4.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>


#define  MY_FILE "/root/test/LogFile"
struct file *file = NULL;
char buf[512];

static int __init init(void) {

    mm_segment_t old_fs;
    printk("Hello, I'm the module that intends to write messages to file.\n");

    if(file == NULL) {
        file = filp_open(MY_FILE, O_RDWR|O_APPEND|O_CREAT, 0644);
    }

    if (IS_ERR(file)) {
        printk("error occured while opening file %s, exiting...\n", MY_FILE);
        return 0;
    }

    sprintf(buf, "%s", "The Messages.");

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    file->f_op->write(file, (char *)buf, strlen(buf), &file->f_pos);
    set_fs(old_fs);

    return 0;
}



static void __exit fini(void) {

    if(file != NULL) filp_close(file, NULL);

}


static struct nf_hook_ops nfho;


unsigned int hook_func(unsigned int hooknum, struct sk_buff **skb, const struct net_device *in,
                       const struct net_device *out, int (*okfn)(struct sk_buff *))
{
    struct sk_buff *sb = *skb;
    unsigned char src_ip[4];
    *(unsigned int *)src_ip = sb->nh.iph->saddr;
    sprintf(buf, "A packet from:%d.%d.%d.%d Detected!",
            src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
//printk("A packet from:%d.%d.%d.%d Detected!",
//  src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
    mm_segment_t old_fs;
    bool have_data = false;
    switch(sb->nh.iph->protocol) {

      case IPPROTO_TCP: {   
        //printk("It's a TCP PACKET\n");
        //sprintf(buf, "%s It's a TCP PACKET\n", buf);
        //struct tcphdr *tcph = (void *)sb->nh.iph + (sb->nh.iph->ihl << 4);
        struct tcphdr *tcph = (struct tcphdr*)(sb->data+(sb->nh.iph->ihl*4));

        sprintf(buf, "%s It's a TCP PACKET, port is %d\n", buf, ntohs(tcph->dest));
//            sprintf(buf, "%s It's a TCP PACKET, port is %d\n", buf, ntohs(sb->h.th->dest));

        have_data = true;
    }
    break;
//         case IPPROTO_ICMP:
//             //printk("It's a ICMP PACKET\n");
//             sprintf(buf, "%s It's a ICMP PACKET\n", buf);
//             break;
//         case IPPROTO_UDP:
//             //printk("It's a UDP PACKET\n");
//             sprintf(buf, "%s It's a UDP PACKET\n", buf);
//             break;
    //default:
    //have_data = false;
    }

    if ( have_data )
    {
        old_fs = get_fs();
        set_fs(KERNEL_DS);
        file->f_op->write(file, (char *)buf, strlen(buf), &file->f_pos);
        set_fs(old_fs);
    }

    return NF_ACCEPT;
}



int init_module() {

    init();

    nfho.hook = hook_func;
    nfho.hooknum  = NF_IP_PRE_ROUTING;
    nfho.pf       = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;

    nf_register_hook(&nfho);

    return 0;
}



void cleanup_module() {

   nf_unregister_hook(&nfho);
   fini();

}



/*
Makefile

obj-m := pcap.o
KERNELBUILD := /lib/modules/`uname -r`/build
default:
 make -C $(KERNELBUILD) M=$(shell pwd) modules
clean:
 rm -rf *.o .*.cmd *.ko *.mod.c .tmp_versions

 */





