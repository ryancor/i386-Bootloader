#include "../../grub_framework/headers/s_stdint.h"
#include "../../grub_framework/headers/s_string.h"

#pragma once

#define PATH_SEPARATOR '/'
#define PATH_SEPARATOR_STRING "/"
#define PATH_UP  ".."
#define PATH_DOT "."

#define O_RDONLY     0x0000
#define O_WRONLY     0x0001
#define O_RDWR       0x0002
#define O_APPEND     0x0008
#define O_CREAT      0x0200
#define O_TRUNC      0x0400
#define O_EXCL       0x0800
#define O_NOFOLLOW   0x1000
#define O_PATH       0x2000
#define O_NONBLOCK   0x4000
#define O_DIRECTORY  0x8000

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x04
#define FS_BLOCKDEVICE 0x08
#define FS_PIPE        0x10
#define FS_SYMLINK     0x20
#define FS_MOUNTPOINT  0x40

#define _IFMT       0170000 /* type of file */
#define     _IFDIR  0040000 /* directory */
#define     _IFCHR  0020000 /* character special */
#define     _IFBLK  0060000 /* block special */
#define     _IFREG  0100000 /* regular */
#define     _IFLNK  0120000 /* symbolic link */
#define     _IFSOCK 0140000 /* socket */
#define     _IFIFO  0010000 /* fifo */

struct fs_node;

typedef uint32_t (*read_type_t) (struct fs_node *, uint64_t, uint32_t, uint8_t *);
typedef uint32_t (*write_type_t) (struct fs_node *, uint64_t, uint32_t, uint8_t *);
typedef void (*open_type_t) (struct fs_node *, unsigned int flags);
typedef void (*close_type_t) (struct fs_node *);
typedef struct dirent *(*readdir_type_t) (struct fs_node *, uint32_t);
typedef struct fs_node *(*finddir_type_t) (struct fs_node *, char *name);
typedef int (*create_type_t) (struct fs_node *, char *name, uint16_t permission);
typedef int (*unlink_type_t) (struct fs_node *, char *name);
typedef int (*mkdir_type_t) (struct fs_node *, char *name, uint16_t permission);
typedef int (*ioctl_type_t) (struct fs_node *, int request, void * argp);
typedef int (*get_size_type_t) (struct fs_node *);
typedef int (*chmod_type_t) (struct fs_node *, int mode);
typedef int (*symlink_type_t) (struct fs_node *, char * name, char * value);
typedef int (*readlink_type_t) (struct fs_node *, char * buf, size_t size);
typedef int (*selectcheck_type_t) (struct fs_node *);
typedef int (*selectwait_type_t) (struct fs_node *, void * process);
typedef int (*chown_type_t) (struct fs_node *, int, int);
typedef void (*truncate_type_t) (struct fs_node *);

typedef struct fs_node
{
	char name[256];			// the filename
	void *device;			// device object
	uint32_t mask;			// the permissions mask
	uint32_t uid;			// the owning user
	uint32_t gid;			// the owning group
	uint32_t flags;			// Flags
	uint32_t inode;			// Inode number
	uint32_t length;		// size of the file
	uint32_t impl;			// Used to keep track which fs it belongs to
	uint32_t open_flags;		// Flags padded to open
	
	// times
	uint32_t atime;			// Accessed
	uint32_t mtime;			// Modified
	uint32_t ctime;			// Created

	// File operations
	struct fs_node *ptr;
	int32_t refcount;
	uint32_t nlink;

	read_type_t read;
	write_type_t write;
	open_type_t open;
	close_type_t close;
	readdir_type_t readdir;
	finddir_type_t finddir;
	create_type_t create;
	mkdir_type_t mkdir;
	ioctl_type_t ioctl;
	get_size_type_t get_size;
	chmod_type_t chmod;
	unlink_type_t unlink;
	symlink_type_t symlink;
	readlink_type_t readlink;
	truncate_type_t truncate;

	selectcheck_type_t selectcheck;
	selectwait_type_t selectwait;
} fs_node_t;

struct dirent
{
	uint32_t ino;
	char name[256];
};

extern fs_node_t *fs_root;

uint32_t read_fs(fs_node_t *node, uint64_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_fs(fs_node_t *node, uint64_t offset, uint32_t size, uint8_t *buffer);
