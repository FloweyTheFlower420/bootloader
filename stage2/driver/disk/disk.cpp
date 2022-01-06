#include "disk.h"
#include <realmode/realmode.h>
#include <new>

struct disk_info_packet
{
    uint16_t buf_size;
    uint16_t flags;
    uint32_t p_cylinders;
    uint32_t p_heads;
    uint32_t p_sectors;
    uint64_t total_sectors;
    uint16_t bytes_per_sector;
} __attribute__((packed));

disk_driver::disk_driver(uint8_t id)  : id(id) 
{
    // use bios to get disk dataCF clear if successful
    auto out = bios_interrupt_pmode(0x13, {.a = 0x48, .d = id, .S = 0x7000});
    if(!out.carry() || out.ah() != 0)
    {
        valid = false;
        return;
    }

    disk_info_packet_v1* info = (disk_info_packet_v1*)(0x7000);
    
    cache_sector_size = info->bytes_per_sector;
    cache_disk_sector_count = info->total_sectors;
}

/*
00h    BYTE    size of packet (10h or 18h)
01h    BYTE    reserved (0)
02h    WORD    number of blocks to transfer (max 007Fh for Phoenix EDD)
04h    DWORD   -> transfer buffer
08h    QWORD   starting absolute block number
*/

struct disk_address_packet
{
    uint8_t size;
    uint8_t reserved;
    uint16_t blocks;
    uint32_t load_at;
    uint64_t start_block;
} __attribute__((packed));

constexpr auto PATIENCE = 1; // how much times should loading be tried in case of fail

void* disk_driver::read_bytes(uint32_t bytes, uint32_t hint, uint64_t offset = 0) const
{
    auto patience = PATIENCE;
    // obtain the # of sectors to read
    uint32_t blocks = (bytes + cache_sector_size - 1) / cache_sector_size;
    // obtain the sector offset from offset
    uint64_t sector_start = offset / cache_sector_size;

    while(patience--)
    {
        *((disk_address_packet*)0x7000) = { 0x10, 0, blocks, hint, sector_size };
        auto out = bios_interrupt_pmode(0x13, {.a = 0x42, .d = id, .S = 0x7000});
        if(out.ah() == 0 && !out.carry())
            break; // loaded bytes.
    }

    return hint + offset % cache_sector_size;
}
