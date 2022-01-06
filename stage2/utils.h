#ifndef __STAGE2_UTILS_H__
#define __STAGE2_UTILS_H__

void check_for_long_mode();
void panic(const char* str);
void* memcpy(void* dest, void* src, size_t count);

#endif