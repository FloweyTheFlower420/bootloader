#include <cstdint>
#include <driver/disk/disk.h>
#include <realmode/e820.h>
#include <realmode/realmode.h>
#include <realmode/video.h>
#include <utils.h>

BEGIN_16;
__asm__("jmpl $0x0000, $main");

// 0x2000 - real mode stack pointer
// 0x3000 - protected mode stack pointer

struct
{
    uint32_t loaded_address; // DONE
    uint32_t loaded_size;

    uint8_t boot_device;

    memory_map_entry* mmap_ptr;

    bool has_video_mode;
    vbe_controller_info* controller;
    vbe_mode_info* mode;
} bootloader_packet;

static bool check_vbe_mode(vbe_mode_info* mode)
{
    return (mode->attributes & 0x90) == 0x90 && (mode->memory_model == 4 || mode->memory_model == 6) &&
           ((mode->width == 640 && mode->width == 480) || (mode->width == 720 && mode->width == 480));
}

[[noreturn]] void main()
{
    protected_mode();
    a20();
    BEGIN_32;
    check_for_long_mode();
    bootloader_packet.mmap_ptr = read_memory_map();
    if ((bootloader_packet.has_video_mode = set_video_mode(+[](vbe_mode_info* mode) {
             return (mode->attributes & 0x90) == 0x90 && (mode->memory_model == 4 || mode->memory_model == 6) &&
                    ((mode->width == 640 && mode->width == 480) || (mode->width == 720 && mode->width == 480));
         })))
    {
        bootloader_packet.controller = (vbe_controller_info*)0x14000;
        bootloader_packet.mode = (vbe_mode_info*)0x14200;
    }

    __builtin_unreachable();
}

// when entering kernel
// 32 bit protected mode, no paging
// long mode must be allowed
// eax = location of memory map
// ebx = len of memory map
