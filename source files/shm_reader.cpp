#include "shm_reader.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define SHM_NAME "/sequence"
#define MAX_BUSES 10

std::vector<int> readSequenceFromSHM() {
    std::vector<int> result;
    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        return result;
    }
    int* data = static_cast<int*>(mmap(nullptr, MAX_BUSES * sizeof(int), PROT_READ, MAP_SHARED, fd, 0));
    if (data == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return result;
    }
    for (int i = 0; i < MAX_BUSES; ++i) {
        if (data[i] != 0) result.push_back(data[i]);
    }
    munmap(data, MAX_BUSES * sizeof(int));
    close(fd);
    return result;
}