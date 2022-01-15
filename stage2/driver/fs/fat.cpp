#include "fat.h"
#include <cstdint>

struct fat_bpb
{
    uint8_t bootsector_jmp_boot[3];
    char bootsector_oem_name[8];
    uint16_t bytes_per_sec;
    uint8_t sector_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t fat_count;
    uint16_t root_entry_count;
    uint16_t unused0;
    uint8_t media;
    uint16_t unused1;
    uint16_t sec_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors;
    uint32_t fat_size;
    uint16_t extended_flags;
    uint16_t fs_version;
    uint32_t root_cluster;
    uint16_t fs_info;
    uint16_t bk_boot_sector;
    uint8_t reserved0[12];
    uint8_t bootsector_drive_nuber;
    uint8_t bootsector_reserved1;
    uint8_t bootsector_boot_sig;
    uint32_t bootsector_volume_id;
    char bootsector_volume_label[11];
    char filesystem_type[8];
} __attribute__((packed));

void read_fat()
{
    fat_bpb* bpb = nullptr;
    auto root_dir_sectors = ((bpb->root_entry_count * 32) + (bpb->bytes_per_sec - 1)) / bpb->bytes_per_sec;
    auto first_data_sector = bpb->reserved_sector_count + (bpb->fat_count * bpb->fat_size) + root_dir_sectors;
    auto cluster_count = first_data_sector / bpb->sector_per_cluster;
}
