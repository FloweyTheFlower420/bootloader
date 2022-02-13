#include "fat.h"
#include <cstdint>
#include <config.h>

#if !define(FS_FAT)
namespace fsimpl
{
    bool check_fat(uint8_t disk, uint8_t partition)
    {
        return false;
    }

    void* read_fat(uint8_t disk, uint8_t partition, const char* path)
    {
        return nullptr;
    }
}

#else

namespace fsimpl
{
    struct fat32_bpb 
    {
        uint8_t jump[3];
        char oem[8];
        uint16_t bytes_per_sector;
        uint8_t sectors_per_cluster;
        uint16_t reserved_sectors;
        uint8_t fats_count;
        uint16_t directory_entries_count;
        uint16_t sector_totals;
        uint8_t media_descriptor_type;
        uint16_t sectors_per_fat_16;
        uint16_t sectors_per_track;
        uint16_t heads_count;
        uint32_t hidden_sectors_count;
        uint32_t large_sectors_count;
        uint32_t sectors_per_fat_32;
        uint16_t flags;
        uint16_t fat_version_number;
        uint32_t root_directory_cluster;
        uint16_t fs_info_sector;
        uint16_t backup_boot_sector;
        uint8_t reserved[12];
        uint8_t drive_number;
        uint8_t nt_flags;
        uint8_t signature;
        uint32_t volume_serial_number;
        char label[11];
        char system_identifier[8];
    } __attribute__((packed));

    struct fat32_directory_entry 
    {
        char file_name_and_ext[8 + 3];
        uint8_t attribute;
        uint8_t file_data_1[8];
        uint16_t cluster_num_high;
        uint8_t file_data_2[4];
        uint16_t cluster_num_low;
        uint32_t file_size_bytes;
    } __attribute__((packed));

    struct fat32_lfn_entry 
    {
        uint8_t sequence_number;
        char name1[10];
        uint8_t attribute;
        uint8_t type;
        uint8_t dos_checksum;
        char name2[12];
        uint16_t first_cluster;
        char name3[4];
    } __attribute__((packed));


    bool check_fat(uint8_t disk, uint8_t partition)
    {
         
        
    }

    void* read_fat(uint8_t disk, uint8_t partition, const char* path)
    {
        return nullptr;
    }
}

#endif
