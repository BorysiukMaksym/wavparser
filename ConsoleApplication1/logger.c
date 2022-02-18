#include "logger.h"

void print_buf_hex(uint8_t *buf, uint32_t datalen)
{
    uint32_t cntr = datalen;

    while(cntr) {
        printf("%02x  ", *(buf + datalen - cntr));
        cntr--;
    }
    printf("\r\n");
}
