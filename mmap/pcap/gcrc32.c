#include <stdio.h>

unsigned int reverse(unsigned int num) {

    unsigned int result = 0;
    unsigned int n;
    unsigned int bit;

    for (n = 0; n < 32; n++) {
        bit = ((num >>n)  & 0x00000001);
        result = result ^ (bit << n);
    }

    return result;
}

unsigned int crc32(unsigned char *message) {
    int i, j;
    unsigned int byte, crc;
    i = 0;
    crc = 0xFFFFFFFF;
    while (message[i] != 0) {
        byte = message[i];
// Get next byte.
        byte = reverse(byte);
// 32-bit reversal.
        for (j = 0; j <= 7; j++) {
// Do eight times.
            if ((int)(crc ^ byte) < 0)
                crc = (crc << 1) ^ 0x04C11DB7;
            else crc = crc << 1;
            byte = byte << 1;
// Ready next msg bit.
        }
        i = i + 1;
    }
    return reverse(~crc);
}


int main(void) {
    char str[] = "Hello, World!";
    unsigned int t = ~crc32(str);
    printf("0x%08x\n", t);

    return 0;
}
