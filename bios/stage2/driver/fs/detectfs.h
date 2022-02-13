#ifndef __STAGE2_DRIVER_FS_DETECTFS_H__
#define __STAGE2_DRIVER_FS_DETECTFS_H__
#include <cstdint>
#include "fat.h"

// filesystem config

void* read(uint8_t disk, uint8_t partition, const char* path)
{
    if(check_fat(disk, partition))
        return read_fat(disk, partition, path);
    
}

#endif
