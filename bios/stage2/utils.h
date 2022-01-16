#ifndef __STAGE2_UTILS_H__
#define __STAGE2_UTILS_H__
#include <cstddef>

void check_for_long_mode();
void panic(const char* str);
void* memcpy(void*, void*, size_t);
#endif
