#ifndef __STAGE2_REALMODE_E820_H__
#define __STAGE2_REALMODE_E820_H__
#include <cstdint>

struct memory_map
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t extend_bitfield;
};

memory_map* read_memory_map();

#endif