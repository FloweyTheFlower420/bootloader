#ifndef __STAGE2_DRIVER_DISK_DISK_H__
#define __STAGE2_DRIVER_DISK_DISK_H__

class disk_driver
{
public:
    template <typename T>
    T* read(uint8_t disk_id, uint64_t offset) { return (T*)read_bytes(disk_id, offset); }

    virtual void* read_bytes(uint8_t disk_id, uint64_t offset);
    virtual uint32_t sector_size(uint8_t disk_id); // the day we have > ~4million byte sectors is the day i stop doing osdev
    virtual uint64_t disk_sector_count(uint8_t disk_id);
};

void* read_bytes(uint8_t drive_id, )

#endif