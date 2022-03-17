#include "utils.h"
#include <cstddef>
#include <cstdint>
#include <libc.h>

void check_for_long_mode()
{
    uint32_t eax;
    __asm__ __volatile__("cpuid": "=a"(eax) : "a"(0x80000000) : "ebx", "ecx");
    if(eax < 0x80000001)
        panic("I need 64-bit cpu!");
    __asm__ __volatile__("cpuid": "=d"(eax) : "a"(0x80000001) : "ebx", "ecx");
    if(!(eax & (1 << 29)))
        panic("I need 64-bit cpu!");
}

void panic(const char* str)
{
    uint8_t* video = (uint8_t*)0xb8000;
    while(*str)
    {
        *video++ = *str++;
        *video++ = 7;
    }

    while(1) { __asm__ __volatile__("hlt"); }
}
