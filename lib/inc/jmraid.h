#ifndef _JMRAID_H_
#define _JMRAID_H_

#include "disk.h"

struct jmraid
{
	struct disk disk;
	uint64_t unused_sector;
	uint32_t vendor_id;
	uint32_t seq_id;
	uint8_t unused_sector_data[SECTOR_SIZE];
	bool is_unused_sector_data_valid;
	bool is_disk_open;
};

struct jmraid_chip_info
{
	char product_name[0x20 + 1];
	char manufacturer[0x20 + 1];
	uint32_t serial_number;
	uint8_t firmware_version[4];
};

struct jmraid_sata_info_item
{
	char model_name[0x28 + 1];
	char serial_number[0x14 + 1];
	uint64_t capacity;
	uint8_t port_type;
	uint8_t port_speed;
	uint8_t page_0_state;
	uint8_t page_0_raid_index;
	uint8_t page_0_raid_member_index;
	uint8_t port;
};

struct jmraid_sata_info
{
	struct jmraid_sata_info_item item[5];
};

struct jmraid_sata_port_info
{
	char model_name[0x28 + 1];
	char serial_number[0x14 + 1];
	char firmware_version[0x08 + 1];
	uint64_t capacity;
	uint64_t capacity_used;
	uint8_t port_type;
	uint8_t port;
	uint8_t page_0_state;
	uint8_t page_0_raid_index;
	uint8_t page_0_raid_member_index;
};

struct jmraid_raid_port_info_member
{
	uint8_t ready;
	uint8_t lba48_support;
	uint8_t sata_page;
	uint8_t sata_port;
	uint32_t sata_base;
	uint64_t sata_size;
};

struct jmraid_raid_port_info
{
	char model_name[0x28 + 1];
	char serial_number[0x14 + 1];
	uint8_t port_state;
	uint8_t level;
	uint64_t capacity;
	uint8_t state;
	uint8_t member_count;
	uint16_t rebuild_priority;
	uint16_t standby_timer;
	char password[0x08 + 1];
	uint64_t rebuild_progress;
	struct jmraid_raid_port_info_member member[5];
};

struct jmraid_disk_smart_info_attribute
{
	uint8_t id;
	uint16_t flags;
	uint8_t current_value;
	uint8_t worst_value;
	uint64_t raw_value;
	uint8_t threshold;
};

struct jmraid_disk_smart_info
{
	struct jmraid_disk_smart_info_attribute attribute[30];
};

void jmraid_init(struct jmraid *jmraid);

bool jmraid_open(struct jmraid *jmraid, const char *disk_name, uint32_t vendor_id);
bool jmraid_close(struct jmraid *jmraid);

bool jmraid_detect_vendor_id(struct jmraid *jmraid, uint32_t *vendor_id);

bool jmraid_get_chip_info(struct jmraid *jmraid, struct jmraid_chip_info *info);
bool jmraid_get_sata_info(struct jmraid *jmraid, struct jmraid_sata_info *info);
bool jmraid_get_sata_port_info(struct jmraid *jmraid, uint8_t index, struct jmraid_sata_port_info *info);
bool jmraid_get_raid_port_info(struct jmraid *jmraid, uint8_t index, struct jmraid_raid_port_info *info);
bool jmraid_get_disk_smart_info(struct jmraid *jmraid, uint8_t index, struct jmraid_disk_smart_info *info);

bool jmraid_ata_identify_device(struct jmraid *jmraid, uint8_t sata_port, uint8_t *data_out);
bool jmraid_ata_smart_read_data(struct jmraid *jmraid, uint8_t sata_port, uint8_t *data_out);

// low level functions

bool jmraid_disk_open(struct jmraid *jmraid, const char *disk_name);
bool jmraid_disk_close(struct jmraid *jmraid);
bool jmraid_disk_read_sector(struct jmraid *jmraid, uint64_t sector, uint8_t *data);
bool jmraid_disk_write_sector(struct jmraid *jmraid, uint64_t sector, const uint8_t *data);

void jmraid_set_unused_sector(struct jmraid *jmraid, uint64_t unused_sector);
void jmraid_set_vendor_id(struct jmraid *jmraid, uint32_t vendor_id);

bool jmraid_find_unused_sector(struct jmraid *jmraid, uint32_t num, uint64_t *sector);
bool jmraid_backup_unused_sector_data(struct jmraid *jmraid);
bool jmraid_restore_unused_sector_data(struct jmraid *jmraid);
bool jmraid_prepare_unused_sector(struct jmraid *jmraid);

bool jmraid_send_handshake(struct jmraid *jmraid, uint32_t magic);

bool jmraid_invoke_command(struct jmraid *jmraid, const uint8_t *data_in, uint32_t size_in, uint8_t *data_out, uint32_t size_out);
bool jmraid_invoke_command_get_chip_info(struct jmraid *jmraid, uint8_t *data_out, uint32_t size_out);
bool jmraid_invoke_command_get_sata_info(struct jmraid *jmraid, uint8_t *data_out, uint32_t size_out);
bool jmraid_invoke_command_get_sata_port_info(struct jmraid *jmraid, uint8_t sata_port, uint8_t *data_out, uint32_t size_out);
bool jmraid_invoke_command_get_raid_port_info(struct jmraid *jmraid, uint8_t raid_port, uint8_t *data_out, uint32_t size_out);
bool jmraid_invoke_command_ata_passthrough(struct jmraid *jmraid, uint8_t sata_port, uint8_t ata_read_addr, uint8_t ata_read_size, const uint8_t *ata_data, uint8_t *data_out, uint32_t size_out);

void parse_jmraid_chip_info(const uint8_t *src, struct jmraid_chip_info *dst);
void parse_jmraid_sata_info(const uint8_t *src, struct jmraid_sata_info *dst);
void parse_jmraid_sata_port_info(const uint8_t *src, struct jmraid_sata_port_info *dst);
void parse_jmraid_raid_port_info(const uint8_t *src, struct jmraid_raid_port_info *dst);
void parse_jmraid_disk_smart_info(const uint8_t *src1, const uint8_t *src2, struct jmraid_disk_smart_info *dst);

#endif
