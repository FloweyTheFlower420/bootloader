#ifndef __LIB_FAT_H__
#define __LIB_FAT_H__
#include <diskapi.h>
#include <memory.h>

namespace fsimpl
{
    // utils
    template<typename T, typename U>
    constexpr T div_roundup(T t, U u)
    {
        return (t + u - 1) / u;
    }

    template<typename T>
    constexpr T max(T t1, T t2)
    {
        return t1 > t2 ? t1 : t2;
    }

    class fat_filesystem : fs::filesystem
    {
        enum class fat_type
        {
            NOT_FAT = -1,
            EXFAT = 0,
            FAT12 = 12,
            FAT16 = 16,
            FAT32 = 32
        };

        struct dirent_info
        {
            size_t cluster;
            bool is_directory;
            size_t file_size;
        };
        
        fs::disk_helper disk;
        // Bytes per FAT sector
        uint16_t bps;
        // Sectors per FAT cluster
        uint8_t spc;
        uint16_t reserved_sectors;
        uint8_t fat_count;
        uint32_t hidden_sectors;
        uint32_t sectors_per_fat;
        uint32_t fat_start_lba;
        uint32_t data_start_lba;
        uint32_t root_cluster;
        uint16_t root_entries;
        uint32_t root_start;
        uint32_t root_size;
        fat_type type;

        inline size_t cluster_to_sector(size_t cluster) { return cluster * spc; }
        inline uint64_t sector_to_byte(size_t sector) { return (uint64_t) sector * bps; }
        inline uint64_t cluster_to_byte(size_t cluster) { return (uint64_t) cluster * spc * bps; }

        size_t next_cluster(size_t cluster);
        size_t cluster_to_real(size_t cluster);
        dirent_info first_cluster_of(const char* segment, size_t current_dir_cluster);
        dirent_info first_cluster_of_root_legacy(const char* segment);

        inline dirent_info first_cluster_of_smart(const char* segment, size_t cluster, bool is_root)
        {
            if(is_root && type != fat_type::FAT32)
                return first_cluster_of_root_legacy(segment);
            return first_cluster_of(segment, cluster);
        }

    public:
        static constexpr const char* FAT32_SYSTEM_ID = "FAT32   ";
        static constexpr const char* FAT16_SYSTEM_ID = "FAT16   ";
        static constexpr const char* FAT12_SYSTEM_ID = "FAT12   ";
        static constexpr size_t READ_FAILED = (size_t) -1;

        fat_filesystem(uint64_t offset, fs::disk_driver& driver);

        size_t read(const char* filename, size_t n, void* buffer);
    };
}

#endif
