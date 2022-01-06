#include <cstdint>
#include <realmode/realmode.h>
#include <realmode/e820.h>
#include <realmode/video.h>
#include <utils.h>

BEGIN_16;
__asm__ ("jmpl $0x0000, $main");

// 0x3000 - real mode stack pointer
// 0x7000 - protected mode stack pointer
// 0x7000 - real-mode buffer

// 0x9000 - Disk IO Buffer
// 0x10000 - start of data
// 0x10000-0x14000 - e820 block
// 0x14000 - vbe controller info
// 0x14200 - vbe mode info

struct 
{
    uint32_t loaded_address; // DONE
    uint32_t loaded_size;

    uint8_t boot_device;

    memory_map* mmap_ptr;

    bool has_memory_mode;
    vbe_controller_info* controller;
    vbe_mode_info* mode;
} bootloader_packet;



[[noreturn]] void main()
{
    protected_mode();
    a20();
    BEGIN_32;
    check_for_long_mode();

    // perform e820
    memory_map* mmap = read_memory_map();
    bootloader_packet.mmap_ptr = mmap;
    if((bootloader_packet.has_memory_mode = set_video_mode()))
    {
        bootloader_packet.controller = (vbe_controller_info*) 0x14000;
        bootloader_packet.mode = (vbe_mode_info*) 0x14200;
    }

    __builtin_unreachable();
}

// when entering kernel
// 32 bit protected mode, no paging
// long mode must be allowed
// eax = location of memory map
// ebx = len of memory map
