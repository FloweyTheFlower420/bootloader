#ifndef __STAGE2_DRIVER_DISK_DISK_H__
#define __STAGE2_DRIVER_DISK_DISK_H__
#include <cstdint>
#include <cstddef>

namespace fs
{
    class disk_driver
    {
        bool valid;

        uint8_t id;
        uint32_t cache_sector_size;
        uint64_t cache_disk_sector_count;
        void* io_buffer;
    public: 
        disk_driver(uint8_t id);
        inline disk_driver() : valid(false) {}
        void read(size_t bytes, size_t offset, void* buffer) const;
        inline uint32_t sector_size() const { return cache_sector_size; }
        inline uint64_t disk_sector_count() const { return cache_disk_sector_count; }
        inline bool is_valid() const { return valid; }
        ~disk_driver();
    };
}

#endif
