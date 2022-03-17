#include "e820.h"
#include "realmode.h"
#include <config.h>
#include MALLOC_IMPL_PATH

memory_map_entry* read_memory_map()
{
    static memory_map buffer;

    // buffer location
    memory_map_entry* root, *curr;
    root = curr = (memory_map_entry*)alloc::malloc(sizeof(memory_map_entry));
    root->next = nullptr;

    uint32_t next_call = 0;
    cpustate ret;

    do 
    {
        ret = bios_interrupt_pmode(0x15, {.a = 0x0000e820, .b = next_call, .c = 24, .d = 0x534D4150, .D = (uint32_t)&buffer});
        curr->data = buffer;
        if(ret.cl() == 20)
            curr->data.extend_bitfield = 0;
        curr = (curr->next = (memory_map_entry*)alloc::malloc(sizeof(memory_map_entry)));
        if(curr == nullptr)
            break;


    }
    while(ret.a == 0x534D4150 && ret.b != 0 && !ret.carry());

    return root;
}
