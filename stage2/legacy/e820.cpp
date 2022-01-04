asm (".code16gcc");
asm (".code16");

#include <cstdint>
#include "realmode.h"

static memory_map buf;

constexpr auto MAX_E820_SIZE = (0x500 - 0x4000) / sizeof(memory_map);

memory_map* read_memory_map()
{
    // buffer location
    memory_map* mmap = (memory_map*)(0x500);
    
    uint16_t index = 0;
    uint32_t next_call = 0;
    uint32_t magic = 0;
    bool should_exit = false;
    do 
    {
        uint32_t real_size;
        __asm__ __volatile__(
            "int $0x15"
            : "=a"(magic),
              "=b"(next_call),
              "=c"(real_size)
            : "D"(&buf),
              "a"(0x0000e820),
              "b"(next_call),
              "c"(24),
              "d"(0x534D4150)
        );

        uint32_t tmp;
        __asm__ __volatile__(
            "pushf\n"
            "pop %%esi"
            : "=S"(tmp)
            :
        );

        should_exit = tmp & 1;
        uint8_t to_copy = real_size & 0x000000FF;
        mmap[index] = buf;
        index++;
        if(to_copy == 20)
            mmap->extend_bitfield = 0;
    }
    while(magic == 0x534D4150 && next_call != 0 && !should_exit && index < MAX_E820_SIZE);

    // "null" entry to determine end of data
    mmap[index].base = 0;
    mmap[index].length = 0;
    mmap[index].type = 0;
    mmap[index].extend_bitfield = 0;

    return (memory_map*)(0x500);
}