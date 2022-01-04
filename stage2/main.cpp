#include <cstdint>
#include "realmode/realmode.h"
#include "realmode/e820.h"
__asm__ (".code16gcc");
__asm__ (".code16");
__asm__ ("jmpl $0x0000, $main");

//bootloader_packet packet;

/*
void check_for_long_mode()
{
    __asm__ __volatile__("pusha");
    uint32_t eax;
    __asm__ __volatile__("cpuid": "=a"(eax) : "a"(0x80000000));
    if(eax < 0x80000001)
        panic("I need 64-bit cpu!");
    __asm__ __volatile__("cpuid": "=d"(eax) : "a"(0x80000001));
    if(!(eax & (1 << 29)))
        panic("I need 64-bit cpu!");
    __asm__ __volatile__("popa");
}*/

// 0x3000 - real mode stack pointer
// 0x7000 - protected mode stack pointer
// 0x10000 - start of data
// 0x10000-0x14000 - e820 block

struct {
    uint32_t loaded_address; // DONE
    uint32_t loaded_size;
    uint32_t mmap_ptr;

    uint8_t boot_device;

    uint16_t vbe_version;
    uint16_t total_memory_used;
    uint16_t width;
    uint16_t height;
    uint8_t bpp;

    uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;

    uint32_t framebuffer;
} bootloader_packet;

void main()
{
    protected_mode();
    a20();

    // perform e820
    memory_map* mmap = read_memory_map();
    mmap_ptr = (uint32_t)mmap;

}

// when entering kernel
// 32 bit protected mode, no paging
// long mode must be allowed
// eax = location of memory map
// ebx = len of memory map
