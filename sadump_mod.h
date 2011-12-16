/*
 * sadump_mod.h
 *
 * Created by: HATAYAMA, Daisuke <d.hatayama@jp.fujitsu.com>
 *
 * Copyright (C) 2011  FUJITSU LIMITED
 * Copyright (C) 2011  NEC Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _SADUMP_MOD_H
#define _SADUMP_MOD_H

#if defined(__x86__) || defined(__x86_64__)

#include <stdint.h>

typedef struct efi_time {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t pad1;
	uint32_t nanosecond;
#define EFI_UNSPECIFIED_TIMEZONE 2047
	int16_t timezone;
	uint8_t daylight;
	uint8_t pad2;
} efi_time_t;

typedef struct {
	uint32_t data1;
	uint16_t data2;
	uint16_t data3;
	uint8_t data4[8];
} efi_guid_t;

struct sadump_part_header {
#define SADUMP_SIGNATURE1	0x75646173
#define SADUMP_SIGNATURE2	0x0000706d
	uint32_t signature1;	/* sadu */
	uint32_t signature2;	/* mp\0\0 */
	uint32_t enable;	/* set sadump service */
	uint32_t reboot;	/* number of seconds until reboot. 1-3600 */
	uint32_t compress;	/* memory image format. */
	uint32_t recycle;	/* dump device recycle */
	uint32_t label[16];	/* reserve */
	efi_guid_t sadump_id;	/* system UUID */
	efi_guid_t disk_set_id;	/* disk set UUID */
	efi_guid_t vol_id;	/* device UUID */
	efi_time_t time_stamp;	/* time stamp */
	uint32_t set_disk_set;	/* device type */
#define SADUMP_MAX_DISK_SET_NUM 16
	uint32_t reserve;	/* Padding for Alignment */
	uint64_t used_device;	/* used device */
#define DUMP_PART_HEADER_MAGICNUM_SIZE 982
	uint32_t magicnum[DUMP_PART_HEADER_MAGICNUM_SIZE]; /* magic number */
};

struct sadump_volume_info {
	efi_guid_t id;		/* volume id */
	uint64_t vol_size;	/* device size */
	uint32_t status;	/* device status */
	uint32_t cache_size;	/* cache size */
};

struct sadump_disk_set_header {
	uint32_t disk_set_header_size;	/* disk set header size */
	uint32_t disk_num;	/* disk number */
	uint64_t disk_set_size;	/* disk set size */
#define DUMP_DEVICE_MAX 16
	struct sadump_volume_info vol_info[DUMP_DEVICE_MAX - 1];
	/* struct VOL_INFO array */
};

struct sadump_header {
#define SADUMP_SIGNATURE "sadump\0\0"
	char signature[8];	/* = "sadump\0\0" */
	uint32_t header_version;	/* Dump header version */
	uint32_t reserve;	/* Padding for Alignment */
	efi_time_t timestamp;	/* Time stamp */
	uint32_t status;	/* Above flags */
	uint32_t compress;	/* Above flags */
	uint32_t block_size;	/* Size of a block in byte */
#define SADUMP_DEFAULT_BLOCK_SIZE 4096
	uint32_t extra_hdr_size;	/* Size of host dependent
					 * header in blocks (reserve)
					 */
	uint32_t sub_hdr_size;	/* Size of arch dependent header in blocks */
	uint32_t bitmap_blocks;	/* Size of Memory bitmap in block */
	uint32_t dumpable_bitmap_blocks;	/* Size of Memory bitmap in block */
	uint32_t max_mapnr;	/* = max_mapnr */
	uint32_t total_ram_blocks;	/* Size of Memory in block */
	uint32_t device_blocks;	/* Number of total blocks in the dump device */
	uint32_t written_blocks;	/* Number of written blocks */
	uint32_t current_cpu;	/* CPU# which handles dump */
	uint32_t nr_cpus;	/* Number of CPUs */
};

struct sadump_apic_state {
	uint64_t ApicId;	/* Local Apic ID register */
	uint64_t Ldr;		/* Logical Destination Register */
};

struct sadump_smram_cpu_state {
	uint64_t Reserved1[58];
	uint32_t GdtUpper, LdtUpper, IdtUpper;
	uint32_t Reserved2[3];
	uint64_t IoEip;
	uint64_t Reserved3[10];
	uint32_t Cr4;
	uint32_t Reserved4[18];
	uint32_t GdtLower;
	uint32_t GdtLimit;
	uint32_t IdtLower;
	uint32_t IdtLimit;
	uint32_t LdtLower;
	uint32_t LdtLimit;
	uint32_t LdtInfo;
	uint64_t Reserved5[6];
	uint64_t Eptp;
	uint32_t EptpSetting;
	uint32_t Reserved6[5];
	uint32_t Smbase;
	uint32_t SmmRevisionId;
	uint16_t IoInstructionRestart;
	uint16_t AutoHaltRestart;
	uint32_t Reserved7[6];
	uint32_t R15Lower, R15Upper, R14Lower, R14Upper;
	uint32_t R13Lower, R13Upper, R12Lower, R12Upper;
	uint32_t R11Lower, R11Upper, R10Lower, R10Upper;
	uint32_t R9Lower, R9Upper, R8Lower, R8Upper;
	uint32_t RaxLower, RaxUpper, RcxLower, RcxUpper;
	uint32_t RdxLower, RdxUpper, RbxLower, RbxUpper;
	uint32_t RspLower, RspUpper, RbpLower, RbpUpper;
	uint32_t RsiLower, RsiUpper, RdiLower, RdiUpper;
	uint32_t IoMemAddrLower, IoMemAddrUpper;
	uint32_t IoMisc, Es, Cs, Ss, Ds, Fs, Gs;
	uint32_t Ldtr, Tr;
	uint64_t Dr7, Dr6, Rip, Ia32Efer, Rflags;
	uint64_t Cr3, Cr0;
};

struct sadump_page_header {
	uint64_t page_flags;
	uint32_t size;
	uint32_t flags;
};

struct sadump_media_header {
	efi_guid_t sadump_id;	// system UUID
	efi_guid_t disk_set_id;	// disk set UUID
	efi_time_t time_stamp;	/* time stamp */
	char sequential_num;	// Medium sequential number
	char term_cord;		// Termination cord
	char disk_set_header_size;	// Size of original disk set header
	char disks_in_use;	// Number of used disks of original dump device
	char reserve[4044];	// reserve feild
};

#endif /* defined(__x86__) && defined(__x86_64__) */

/*
 * Type of sadump related formats
 */
enum sadump_format_type {
	SADUMP_UNKNOWN = 0,
	SADUMP_SINGLE_PARTITION,
	SADUMP_DISKSET,
	SADUMP_MEDIA_BACKUP
};

#endif /* _SADUMP_MOD_H */
