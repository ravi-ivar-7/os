#include <stdio.h>
#include <string.h>

// Simulated FAT32 Boot Sector / BPB fields
typedef struct {
    char oem_name[8];
    unsigned short bytes_per_sector;
    unsigned char sectors_per_cluster;
    unsigned short reserved_sectors;
    unsigned char fat_count;
    unsigned int total_sectors;
    unsigned int sectors_per_fat;
    unsigned int root_cluster;
    char volume_label[11];
} FAT32_BPB;

// Simulated ext4 Superblock fields
typedef struct {
    unsigned int inode_count;
    unsigned int block_count;
    unsigned int free_blocks;
    unsigned int free_inodes;
    unsigned int first_data_block;
    unsigned int log_block_size; // actual size = 1024 << log_block_size
    unsigned int inodes_per_group;
    unsigned int blocks_per_group;
    unsigned short magic;        // 0xEF53
    unsigned short state;        // 1 = clean, 2 = has errors
    char volume_name[16];
} EXT4_Superblock;

void print_fat32_info(FAT32_BPB *bpb) {
    printf("=== FAT32 Volume Info ===\n");
    printf("  OEM Name:             %.8s\n", bpb->oem_name);
    printf("  Bytes per Sector:     %u\n", bpb->bytes_per_sector);
    printf("  Sectors per Cluster:  %u\n", bpb->sectors_per_cluster);
    printf("  Cluster Size:         %u bytes\n", bpb->bytes_per_sector * bpb->sectors_per_cluster);
    printf("  Reserved Sectors:     %u\n", bpb->reserved_sectors);
    printf("  Number of FATs:       %u\n", bpb->fat_count);
    printf("  Total Sectors:        %u\n", bpb->total_sectors);
    printf("  Volume Label:         %.11s\n", bpb->volume_label);
    printf("  Volume Size:          %.2f MB\n",
           (float)(bpb->total_sectors * bpb->bytes_per_sector) / (1024 * 1024));
}

void print_ext4_info(EXT4_Superblock *sb) {
    int block_size = 1024 << sb->log_block_size;
    printf("\n=== ext4 Superblock Info ===\n");
    printf("  Volume Name:          %.16s\n", sb->volume_name);
    printf("  Magic Number:         0x%04X (%s)\n", sb->magic, sb->magic == 0xEF53 ? "valid ext4" : "invalid");
    printf("  State:                %s\n", sb->state == 1 ? "Clean" : "Errors detected");
    printf("  Block Size:           %d bytes\n", block_size);
    printf("  Total Blocks:         %u\n", sb->block_count);
    printf("  Free Blocks:          %u\n", sb->free_blocks);
    printf("  Total Inodes:         %u\n", sb->inode_count);
    printf("  Free Inodes:          %u\n", sb->free_inodes);
    printf("  Inodes per Group:     %u\n", sb->inodes_per_group);
    printf("  Blocks per Group:     %u\n", sb->blocks_per_group);
    printf("  Filesystem Usage:     %.1f%%\n",
           100.0f * (sb->block_count - sb->free_blocks) / sb->block_count);
}

void explain_block_groups(EXT4_Superblock *sb) {
    int block_size = 1024 << sb->log_block_size;
    int num_groups = (sb->block_count + sb->blocks_per_group - 1) / sb->blocks_per_group;
    printf("\n=== Block Group Layout ===\n");
    printf("  Total block groups: %d\n", num_groups);
    printf("  Each group contains (%u blocks x %d bytes = %.1f MB):\n",
           sb->blocks_per_group, block_size,
           (float)(sb->blocks_per_group * block_size) / (1024*1024));
    printf("  +--------------+-----------------+----------+-------------+-----------+------------+\n");
    printf("  | Superblock*  | Group Descrptrs | Blk Bmap | Inode Bmap  | Inode Tbl | Data Blks  |\n");
    printf("  +--------------+-----------------+----------+-------------+-----------+------------+\n");
    printf("  (* superblock is backed up in all groups in ext4)\n");
}

int main() {
    // Simulated FAT32 BPB
    FAT32_BPB bpb = {
        .oem_name = "MSDOS5.0",
        .bytes_per_sector = 512,
        .sectors_per_cluster = 8,
        .reserved_sectors = 32,
        .fat_count = 2,
        .total_sectors = 131072, // 64MB volume
        .sectors_per_fat = 512,
        .root_cluster = 2,
        .volume_label = "MY USB DRV "
    };
    print_fat32_info(&bpb);

    // Simulated ext4 Superblock
    EXT4_Superblock sb = {
        .inode_count = 655360,
        .block_count = 2621440,
        .free_blocks = 1500000,
        .free_inodes = 600000,
        .first_data_block = 0,
        .log_block_size = 2,    // 4096 byte blocks
        .inodes_per_group = 8192,
        .blocks_per_group = 32768,
        .magic = 0xEF53,
        .state = 1,
        .volume_name = "linux-root"
    };
    print_ext4_info(&sb);
    explain_block_groups(&sb);

    return 0;
}
