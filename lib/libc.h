#ifndef __LIB_LIBC_H__
#define __LIB_LIBC_H__
#include <cstddef>

void* memcpy(void* dest, const void* src, size_t n);
int memcmp(const void* a, const void* b, size_t n);
void* memset(void* str, int c, size_t n);
size_t strlen(const char* str);
int strcasecmp(const char* s1, const char* s2);
void panic(const char* str);

template <typename T>
T min(T a, T b)
{
    return a < b ? a : b;
}

#endif
