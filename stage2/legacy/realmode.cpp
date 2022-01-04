__asm__ __volatile__(".code16gcc");
__asm__ __volatile__(".code16");
__asm__ __volatile__("jmpl $0x0000, $main");

#include "realmode.h"

// realmode memory area
// "memory map"
// 0x500 - 0x4000 | e820 memory map data
// 0x4000 - VBE info block
// 0x4200 - Video mode block

void lgdt()
{
    static uint8_t gdt[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Null entry
        0xff, 0xff, 0x00, 0x00, 0x00, 0x9a, 0xCF, 0x00, // code
        0xff, 0xff, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00, // data
    };

    struct {
        uint16_t limit = sizeof(gdt);
        uint32_t base = (uint32_t)gdt;
    } __attribute__((packed))
}

bootloader_packet packet;

[[noreturn]] int main()
{
    memory_map* mmap = read_memory_map();
    /*vbe_controller_info* info = read_controller_info();
    if(info != nullptr)
    {
        uint32_t ptr = info->video_mode_ptr.to_ptr();
        uint16_t mode_id = fetch_far(ptr);
        vbe_mode_info_structure* mode = nullptr;

        while(mode_id != 0xFFFF)
        {
            mode = read_mode_info(mode_id);
            if(mode == nullptr)
                continue;
            if(
                (mode->attributes & 0x90) == 0x90 &&
                (mode->memory_model == 4 || mode->memory_model == 6) &&
                ((mode->width == 640 && mode->width == 480) || (mode->width == 720 && mode->width == 480))
            ) {
                break;
            }
            ptr += 2;
            mode_id = fetch_far(ptr);
        }

        if(mode == nullptr || !set_mode(mode, mode_id))
           packet.found_valid_vbe = false;
    }
    else
        packet.found_valid_vbe = false;
    

     
    // run away to protected mode where things are a bit more sane


    __asm__ __volatile__("ljmp $0x0,$0x8000");*/
    panic("foo");
    __builtin_unreachable();
}

void panic(const char* buf)
{
    while(1) {}
}