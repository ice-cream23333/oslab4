#include "string.h"

void *memset(void *dst, int c, uint64 n) {
    char *cdst = (char *)dst;
    for (uint64 i = 0; i < n; ++i)
        cdst[i] = c;

    return dst;
}

void *memcpy(void *dst, void *src, uint64 n) 
{
    char *dst_reg = (char *)dst;
    char *src_reg = (char *)src;
    for (uint64 i = 0; i < n; ++i) 
        dst_reg[i] = src_reg[i];

    return dst;
}