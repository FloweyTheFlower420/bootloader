#include "disk.h"
#include <config.h>
#include <diskapi.h>
#include <libc.h>
#include <new>
#include <realmode/realmode.h>
#include <utils.h>
#include MALLOC_IMPL_PATH

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

struct disk_address_packet
{
    uint8_t size;
    uint8_t reserved;
    uint16_t blocks;
    uint32_t load_at;
    uint64_t start_block;
} __attribute__((packed));

using namespace fs;

disk_driver::disk_driver(uint8_t id) : id(id)
{
    disk_info_packet info;
    // use bios to get disk data
    auto out = bios_interrupt_pmode(0x13, {.a = 0x48, .d = id, .S = (uint32_t)&info});
    if (!out.carry() || out.ah() != 0)
    {
        valid = false;
        return;
    }

    cache_sector_size = info.bytes_per_sector;
    cache_disk_sector_count = info.total_sectors;
    valid = true;
    io_buffer = alloc::malloc(cache_sector_size);
}

void disk_driver::read(size_t bytes, size_t offset, void* buf) const
{
    auto patience = BIOS_DISK_PATIENCE;

    char* dest = (char*)buf;
    char* src = (char*)io_buffer;
    uint64_t sector = offset / cache_sector_size;
    uint32_t start_block_offset = offset % cache_sector_size;
    src += start_block_offset;
    size_t n = min(cache_sector_size - start_block_offset, bytes);

    while (bytes > 0)
    {
        disk_address_packet dap{0x10, 0, 1, (uint32_t)src, sector};
        while (patience--)
        {
            auto out = bios_interrupt_pmode(0x13, {.a = 0x42, .d = id, .S = (uint32_t)&dap});
            if (out.ah() == 0 && !out.carry())
            {
                memcpy(dest, src, n);
                dest += n;
                bytes -= n;
                n = min(cache_sector_size, bytes);
                src = (char*)io_buffer;

                break;
            }
        }

        if (!patience)
            panic("Disk IO error");
    }
}

disk_driver::~disk_driver()
{
    alloc::free(io_buffer);
}
