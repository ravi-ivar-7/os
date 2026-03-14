#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FILE_PATH "test.txt"

int main() {
    int fd;
    struct stat file_stat;
    char *map;

    // Open a file for reading and writing
    fd = open(FILE_PATH, O_RDWR);
    if (fd == -1) {
        perror("Error opening file. Please ensure test.txt exists.");
        exit(EXIT_FAILURE);
    }

    // Get file size
    if (fstat(fd, &file_stat) == -1) {
        perror("Error getting file size.");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Map the file into memory
    map = mmap(NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("Error mmapping the file.");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    // We can close the file descriptor after mmap, the mapping remains.
    close(fd);

    printf("Original File Content:\n%s\n", map);
    
    printf("Modifying mapped memory...\n");
    // Change some characters directly in memory
    for (int i = 0; i < file_stat.st_size && i < 12; i++) {
        if (map[i] >= 'a' && map[i] <= 'z') map[i] -= 32; // Uppercase
        if (map[i] >= 'A' && map[i] <= 'Z') map[i] += 32; // Lowercase
    }
    
    // Write a new string directly over the memory view
    const char *new_text = "\nMODIFIED BY MMAP!";
    if ((size_t)file_stat.st_size > strlen(new_text)) {
        // Find end to append
        int i = 0;
        while(map[i] != '\0' && i < file_stat.st_size) i++;
        if (i > 0) i--; // overwrite newline
        
        // Ensure we don't write past file size
        size_t remaining = (size_t)(file_stat.st_size - i);
        size_t to_copy = remaining < strlen(new_text) ? remaining : strlen(new_text);
        strncpy(&map[i], new_text, to_copy);
    }

    printf("Memory Modified. The OS will automatically sync changes to the file.\n\n");

    // Sync memory changes to disk
    if (msync(map, file_stat.st_size, MS_SYNC) == -1) {
        perror("Could not sync the file to disk.");
    }

    // Unmap the completely modified file
    if (munmap(map, file_stat.st_size) == -1) {
        perror("Error unmapping the file.");
    }

    printf("Operation Complete. Check the contents of test.txt.\n");
    return 0;
}
