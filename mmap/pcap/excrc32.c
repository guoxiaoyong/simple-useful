
#if 0

unsigned long crc_32_tab[256]= {0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,0x0edb8832,…, 0x5a05df1b, 0x2d02ef8d};
//事先计算出的参数表，共有256项，未全部列出。


unsigned long GenerateCRC32(char xdata * DataBuf,unsigned long len)
{
    unsigned long oldcrc32;
    unsigned long crc32;
    unsigned long oldcrc;
    unsigned int charcnt;
    char c,t;
    oldcrc32 = 0x00000000; //初值为0
    charcnt=0;
    while (len--) {
        t= (oldcrc32 >> 24) & 0xFF; //要移出的字节的值
        oldcrc=crc_32_tab[t]; //根据移出的字节的值查表
        c=DataBuf[charcnt]; //新移进来的字节值
        oldcrc32= (oldcrc32 << 8) | c; //将新移进来的字节值添在寄存器末字节中
        oldcrc32=oldcrc32^oldcrc; //将寄存器与查出的值进行xor运算
        charcnt++;
    }
    crc32=oldcrc32;
    return crc32;
}
#endif


#if 1
//参数表可以先在PC机上算出来，也可在程序初始化时完成。下面是用于计算参数表的c语言子程序，在Visual C++ 6.0下编译通过。
#include <stdio.h>
unsigned long int crc32_table[256];
unsigned long int ulPolynomial = 0x04c11db7;


unsigned long int Reflect(unsigned long int ref, char ch) {
    unsigned long int value(0);
// 交换bit0和bit7，bit1和bit6，类推
    for(int i = 1; i < (ch + 1); i++)
    {   if(ref & 1)
            value |= 1 << (ch - i);
        ref >>= 1;
    }
    return value;
}


void init_crc32_table()
{
    unsigned long int crc,temp;
// 256个值
    for(int i = 0; i <= 0xFF; i++) {   
            temp=Reflect(i, 8);
        crc32_table[i]= temp<< 24;
        for (int j = 0; j < 8; j++) {
            unsigned long int t1,t2;
            unsigned long int flag=crc32_table[i]&0x80000000;
            t1=(crc32_table[i] << 1);
            if(flag==0)
                t2=0;
            else
                t2=ulPolynomial;
            crc32_table[i] =t1^t2 ;
        }
        crc=crc32_table[i];
        crc32_table[i] = Reflect(crc32_table[i], 32);
    }
}

#endif


