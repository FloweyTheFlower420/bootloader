#include "libc.h"

void* memcpy(void* dest, const void* src, size_t n)
{
    while(n--)
        *dest++ = *src++;
}

int memcmp(const void* a, const char* b, size_t n)
{
    unsigned char u1, u2;

    for ( ; n-- ; s1++, s2++) 
    {
        u1 = * (unsigned char *) s1;
        u2 = * (unsigned char *) s2;
        if ( u1 != u2) 
            return (u1-u2);
    }
    
    return 0;
}

size_t strlen(const char* str)
{
    size_t s = 0;
    while(*str++)
        s++;
    return s;
}
