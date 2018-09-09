#ifndef _DISK_H_
#define _DISK_H_

#include <types.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define SECTOR_SIZE 512

struct disk
{
#ifdef _WIN32
	HANDLE handle;
#endif
};

void disk_init(struct disk *disk);

bool disk_open(struct disk *disk, const char *name, const char *access);
bool disk_close(struct disk *disk);

bool disk_read_sector(struct disk *disk, uint64_t sector, uint8_t *data);
bool disk_write_sector(struct disk *disk, uint64_t sector, const uint8_t *data);

#endif
