#ifndef __DISKAPI_H__
#define __DISKAPI_H__
#include <cstddef>
#include <cstdint>
#include <config.h>
#include DISK_IMPL_PATH

namespace fs
{
    class filesystem
    {
    public:
        virtual size_t read(const char* path, size_t n, void* buffer) = 0;
    };

    class partition
    {
    public:
        template<typename T>
        T* makefs(uint32_t index)
        {
            return new T(0);
        }
    };

    class disk_helper
    {
        uint64_t off;
        disk_driver& driver;
    public:
        inline disk_helper(uint64_t off, disk_driver& driver) : off(off), driver(driver) {}
        inline void read(size_t count, size_t offset, void* buffer)
        {
            driver.read(count, off + offset, buffer);
        }
    };
}

#endif
