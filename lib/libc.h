#ifndef __LIB_LIBC_H__
#define __LIB_LIBC_H__
#include <cstddef>

void* memcpy(void*, const void*, size_t);
int memcmp(const void*, const void*, size_t);
size_t strlen(const char*);

#endif
