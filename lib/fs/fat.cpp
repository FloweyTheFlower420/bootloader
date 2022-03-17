#include "fat.h"
#include <libc.h>
namespace fsimpl
{
    struct fat_bpb
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
    } __attribute__((packed)); 

    struct fat1n_ebpb
    {
        uint8_t drive_number;
        uint8_t flags;
        uint8_t signature;
        uint32_t serial_no;
        char label[11];
        char system_id[8];
    } __attribute__((packed));

    struct fat32_ebpb
    {
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
        char system_id[8];
    } __attribute__((packed));

    struct fat_directory_entry
    {
        char file_name_and_ext[8 + 3];
        uint8_t attribute;
        uint8_t file_data_1[8];
        uint16_t cluster_num_high;
        uint8_t file_data_2[4];
        uint16_t cluster_num_low;
        uint32_t file_size_bytes;
    } __attribute__((packed));

    struct fat_lfn_entry
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


    size_t fat_filesystem::next_cluster(size_t cluster)
    {
        uint16_t buf16 = 0;
        uint32_t buf32 = 0;
        switch (type)
        {
            case fat_type::FAT12:
                while(true)
                {
                    disk.read(sizeof(buf16), fat_start_lba * bps + (cluster + cluster / 2), &buf16);
                    if (cluster % 2 == 0)
                        buf16 &= 0xfff;
                    else
                        buf16 >>= 4;

                    if(buf16 > 0xff8)
                        return 0xffffffff;
                    else if(buf16 != 0xff7)
                        return buf16;
                }

            case fat_type::FAT16:
                while(true)
                {
                    disk.read(sizeof(buf16), fat_start_lba * bps + cluster * sizeof(uint16_t), &buf16);
                    
                    if(buf16 > 0xfff8)
                        return 0xffffffff;
                    else if(buf16 != 0xfff7)
                        return buf16;
                }
            case fat_type::FAT32:
                while(true)
                {
                    disk.read(sizeof(buf32), fat_start_lba * bps + cluster * sizeof(uint32_t), &buf32);
                    buf32 &= 0x0fffffff;

                    if(buf32 > 0x0ffffff8)
                        return 0xffffffff;
                    else if(buf32 != 0x0ffffff7)
                        return buf32;
                }
            default:
                __builtin_unreachable();
        }
    }


    static inline void fat32_lfncpy(char* destination, const void* source, unsigned int size) 
    {
        for (unsigned int i = 0; i < size; i++)
            *(((uint8_t*) destination) + i) = *(((uint8_t*) source) + (i * 2));
    }

    static inline bool is_same_dirent(const char* lfn, const char* normal, const char* target)
    {
        if(strcasecmp(lfn, target) == 0)
            return true;
        
        char buf[12] {0};
        int i = 0;
        bool has_ext = false;
        while(*target && i < 12)
        {
            if(*target == '.')
            {
                if(has_ext)
                    return false;
                else
                    has_ext = true;
            }
            else
            {
                buf[i] = *target;
                i++;
            }

            target++;
        }

        return strcasecmp(buf, normal) == 0;
    }

    size_t fat_filesystem::cluster_to_real(size_t cluster)
    {
        return root_start - 2 + cluster + (type == fat_type::FAT32 ? 0 : root_size / spc);
    }

    fat_filesystem::dirent_info fat_filesystem::first_cluster_of_root_legacy(const char* name)
    {
        constexpr auto LFN_SIZE = 13 * 20 + 1;
        const size_t root_size_bytes = sector_to_byte(root_size);
        auto sector_region = mbuffer(root_size_bytes);
        auto lfn_region = mbuffer(LFN_SIZE);
        auto* dirents = sector_region.data<fat_directory_entry>();
        auto* lfn_buffer = lfn_region.data<char>();

        disk.read(root_size_bytes, cluster_to_byte(root_cluster), dirents);
        for(size_t i = 0; i <  root_size_bytes / sizeof(fat_directory_entry); i++)
        {
            uint8_t first_byte = *((uint8_t*) &dirents[i]);
            if(first_byte == 0)
                return { READ_FAILED, 0, 0 };
            else if(first_byte == 0xe5)
                continue;

            if(dirents[i].attribute == 0xf)
            {
                fat_lfn_entry* lfn_entry = (fat_lfn_entry*) &(dirents[i]);
                size_t lfn_index = ((lfn_entry->sequence_number & ~(0x40)) - 1);

                if(lfn_index >= 20)
                    panic("FAT: lfn_index >= 20"); 
                char* target = lfn_buffer + 13 * lfn_index;
                fat32_lfncpy(target + 00, lfn_entry->name1, 5);
                fat32_lfncpy(target + 05, lfn_entry->name2, 6);
                fat32_lfncpy(target + 11, lfn_entry->name3, 2);
            }
            else
            {
                if(is_same_dirent(lfn_buffer, dirents[i].file_name_and_ext, name))
                    return { 
                        ((size_t)dirents[i].cluster_num_high >> 16) + dirents[i].cluster_num_low, 
                        (bool)(dirents[i].attribute & 0x10),
                        dirents[i].file_size_bytes
                    };

                lfn_region.set(0);
            }
        }

        return { READ_FAILED, 0, 0 }; 

    }

    fat_filesystem::dirent_info fat_filesystem::first_cluster_of(const char* name, size_t current_dir_cluster)
    {
        constexpr auto LFN_SIZE = 13 * 20 + 1;
        auto sector_region = mbuffer(bps);
        auto lfn_region = mbuffer(LFN_SIZE);
        auto* dirents = sector_region.data<fat_directory_entry>();
        auto* lfn_buffer = lfn_region.data<char>();

        while(current_dir_cluster)
        {
            for(size_t sector = 0; sector < spc; sector++)
            {
                disk.read(bps, cluster_to_byte(cluster_to_real(current_dir_cluster) + sector), dirents);
                for(size_t i = 0; i < bps / sizeof(fat_directory_entry); i++)
                {
                    uint8_t first_byte = *((uint8_t*) &dirents[i]);
                    if(first_byte == 0)
                        return { READ_FAILED, 0, 0 };
                    else if(first_byte == 0xe5)
                        continue;

                    if(dirents[i].attribute == 0xf)
                    {
                        fat_lfn_entry* lfn_entry = (fat_lfn_entry*) &(dirents[i]);
                        size_t lfn_index = ((lfn_entry->sequence_number & ~(0x40)) - 1);

                        if(lfn_index >= 20)
                            panic("FAT: lfn_index >= 20");

                        char* target = lfn_buffer + 13 * lfn_index;
                        fat32_lfncpy(target + 00, lfn_entry->name1, 5);
                        fat32_lfncpy(target + 05, lfn_entry->name2, 6);
                        fat32_lfncpy(target + 11, lfn_entry->name3, 2);
                    }
                    else
                    {
                        if(is_same_dirent(lfn_buffer, dirents[i].file_name_and_ext, name))
                            return { 
                                ((size_t)dirents[i].cluster_num_high >> 16) + dirents[i].cluster_num_low, 
                                (bool)(dirents[i].attribute & 0x10),
                                dirents[i].file_size_bytes
                            };

                        lfn_region.set(0);
                    }
                }
            }

            current_dir_cluster = next_cluster(current_dir_cluster);
        }

        return { READ_FAILED, 0, 0 }; 
    }

    fat_filesystem::fat_filesystem(uint64_t offset, fs::disk_driver& driver) : disk(offset, driver)
    {
        auto region = mbuffer(512);
        void* buf = region.data<void>();
        disk.read(512, 0, buf);
        
        fat_bpb* bpb = (fat_bpb*)buf;
        fat1n_ebpb* f1n = (fat1n_ebpb*)((uint8_t*)buf + sizeof(fat_bpb));
        fat32_ebpb* f32 = (fat32_ebpb*)(f1n);
        
        if(memcmp(f32->system_id, FAT32_SYSTEM_ID, 8) == 0)
            type = fat_type::FAT32;
        else if(memcmp(f1n->system_id, FAT16_SYSTEM_ID, 8) == 0)
            type = fat_type::FAT16;
        else if(memcmp(f1n->system_id, FAT12_SYSTEM_ID, 8) == 0)
            type = fat_type::FAT12;
        else
        {
            type = fat_type::NOT_FAT;
            return;
        }

        bps = bpb->bytes_per_sector;
        spc = bpb->sectors_per_cluster;
        reserved_sectors = bpb->reserved_sectors;
        fat_count = bpb->fats_count;
        hidden_sectors = bpb->hidden_sectors_count;
        sectors_per_fat = type == fat_type::FAT32 ? f32->sectors_per_fat_32 : bpb->sectors_per_fat_16;
        root_cluster = type == fat_type::FAT32 ? f32->root_directory_cluster 
                                               : (bpb->reserved_sectors + (fat_count * sectors_per_fat)) / spc;
        fat_start_lba = bpb->reserved_sectors;
        root_entries = bpb->directory_entries_count;
        root_start = (reserved_sectors + fat_count * sectors_per_fat) / spc;
        root_size = div_roundup(root_entries * sizeof(fat_directory_entry), bps);

        switch (type) 
        {
            case fat_type::FAT12:
            case fat_type::FAT16:
                data_start_lba = root_start + root_size;
                break;
            case fat_type::FAT32:
                data_start_lba = root_start;
                break;
            default:
                __builtin_unreachable();
        }
    }

    size_t fat_filesystem::read(const char* filename, size_t n, void* buffer)
    {
        mbuffer fname_buffer(256);
        fname_buffer.set(0);
        char* fname = fname_buffer.data<char>();

        if(*filename != '/')
            return READ_FAILED;
        filename++;

        bool is_root = true;
        dirent_info info { root_cluster, true, 0 };
        while(*filename)
        {
            if(*filename == '/')
            {
                fname = fname_buffer.data<char>();
                if(strlen(fname) != 0)
                {
                    if(!info.is_directory)
                        return READ_FAILED;

                    info = first_cluster_of_smart(fname, info.cluster, is_root);
                    is_root = false;

                    if(info.cluster == READ_FAILED)
                        return READ_FAILED;
                }

                fname_buffer.set(0);
                filename++;
            }
            else
            {
                *fname++ = *filename++;
            }
        }

        fname = fname_buffer.data<char>();
        if(!info.is_directory)
            return READ_FAILED;
        if(strlen(fname) == 0)
            return READ_FAILED;

        info = first_cluster_of_smart(fname, info.cluster, is_root);
        if(info.cluster == READ_FAILED)
            return READ_FAILED;

        size_t cluster = info.cluster;
        size_t bytes_to_read = min(n, info.file_size);
        
        while(cluster != 0xffffffff)
        {
            disk.read(min(cluster_to_byte(1), (uint64_t)n), cluster_to_byte(cluster_to_real(cluster)), buffer);
            if(n <= cluster_to_byte(1)) 
                break;
            n -= cluster_to_byte(1);
            buffer = (void*)((char*) buffer + cluster_to_byte(n));
            cluster = next_cluster(cluster);
        }

        return bytes_to_read;
    }

}
