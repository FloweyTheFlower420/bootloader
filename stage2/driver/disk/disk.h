#ifndef __STAGE2_DRIVER_DISK_DISK_H__
#define __STAGE2_DRIVER_DISK_DISK_H__
#include <cstdint>

class disk_driver
{
    bool valid;
    uint8_t id;
    uint32_t cache_sector_size;
    uint64_t cache_disk_sector_count;

    static disk_driver cache[256];
    static uint8_t present[256 / 8];
public:

    static void initalize();
    static disk_driver& get_driver(uint8_t id);

    disk_driver(uint8_t id);
    inline disk_driver() : valid(false) {}
    // loads <bytes> bytes of disk (minimum) from offset <offset> bytes into the disk.
    // returns a pointer to loaded address
    // the data will be loaded at hint if offset is aligned with sector_size()
    // the data will be loaded at hint + offset % sector_size() otherwise
    void* read_bytes(uint32_t bytes, uint32_t hint, uint64_t offset = 0) const;
    inline uint32_t sector_size() const { return cache_sector_size; }
    inline uint64_t disk_sector_count() const { return cache_disk_sector_count; }
    inline bool is_valid() const { return valid; }
};

#endif
