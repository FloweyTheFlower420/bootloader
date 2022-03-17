#include "libc.h"

void* memcpy(void* _dest, const void* _src, size_t n)
{
    const char* src = (const char*) _src;
    char* dest = (char*) _dest;
    while(n--)
        *dest++ = *src++;
    return _dest;
}

int memcmp(const void* a, const void* b, size_t n)
{
    const char* s1 = (const char*) a;
    const char* s2 = (const char*) b;
    for (;n--; s1++, s2++) 
    {
        unsigned char u1, u2;
        u1 = *s1;
        u2 = *s2;        
        if ( u1 != u2) 
            return u1 - u2;
    }
    
    return 0;
}

void *memset(void* str, int c, size_t n)
{
    char* buf = (char*) str;
    while(n--)
        *buf++ = c;
    return str;
}

inline static char tolower(char ch)
{
    if(ch >= 65 && ch <= 90)
        return ch + 32;
    return ch;
}


int strcasecmp(const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    int result;
    if (p1 == p2)
        return 0;
    while ((result = tolower (*p1) - tolower (*p2++)) == 0)
    if (*p1++ == '\0')
        break;
    return result;
}

size_t strlen(const char* str)
{
    size_t s = 0;
    while(*str++)
        s++;
    return s;
}
