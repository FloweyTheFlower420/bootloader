#include "fat.h"
#include "libc.h"
namespace fsimpl
{
    bool check_fat_from_buffer(void* buffer)
    {
        fat1n_ebpb* f1n = (fat1n_ebpb*)((uint8_t*)buffer + sizeof(fat_bpb))
        fat32_ebpb* f32 = (fat32_ebpb*)(f1n);

        return memcmp(f1n->system_id, fat32_bpb::FAT32_SYSTEM_ID, 8) == 0 ||
               memcmp(f1n->system_id, fat32_bpb::FAT16_SYSTEM_ID, 8) == 0 ||
               memcmp(buffer, fat32_bpb::FAT12_SYSTEM_ID, 8) == 0;
    }

    
    
}
