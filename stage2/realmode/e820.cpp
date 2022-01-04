#include "e820.h"
#include "realmode.h"
constexpr auto MAX_E820_SIZE = (0x4000) / sizeof(memory_map);

memory_map* read_memory_map()
{
    static uint8_t buf[24] = {0};
    // buffer location
    memory_map* mmap = (memory_map*)(0x10000);
    
    uint16_t index = 0;
    uint32_t next_call = 0;
    cpustate ret;

    do 
    {
        ret = bios_interrupt_pmode(0x15, {.a = 0x0000e820, .b = next_call, .c = 24, .d = 0x534D4150, .D = (uint32_t)buf});
        
        mmap[index] = *((memory_map*)buf);
        index++;
        if(ret.cl() == 20)
            mmap->extend_bitfield = 0;
    }
    while(ret.a == 0x534D4150 && ret.b != 0 && !ret.carry() && index < MAX_E820_SIZE);

    // "null" entry to determine end of data
    mmap[index].base = 0;
    mmap[index].length = 0;
    mmap[index].type = 0;
    mmap[index].extend_bitfield = 0;

    return (memory_map*)(0x500);
}