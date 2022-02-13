#ifndef __STAGE2_DRIVER_FS_FAT_H__
#define __STAGE2_DRIVER_FS_FAT_H__
#include <utils.h>
#include "detectfs.h"

namespace fsimpl
{
    void check_fat(uint8_t disk, uint8_t partition);
    void* read_fat(uint8_t disk, uint8_t partition, const char* path);
}

#endif
