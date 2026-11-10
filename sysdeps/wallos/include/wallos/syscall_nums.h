#ifndef WALLOS_SYSCALL_NUMS_H
#define WALLOS_SYSCALL_NUMS_H

#include <stdint.h>

// This header will be exported as a user mode header, so syscall numbers can be seen in user mode.
// Always explicitly set the syscall number.

// These values are part of the ABI.
// Never renumber, only add.
// If one needs to be removed, leave that number empty
enum {
	SYS_EXIT = 0,
	SYS_YIELD = 1,
	SYS_DEBUG_LOG = 2,
	SYS_WRITE = 3,
	SYS_READ = 4,
	SYS_OPEN = 5,
	SYS_CLOSE = 6,
	SYS_SEEK = 7,
	SYS_MMAP = 8,
	SYS_MUNMAP = 9,
	SYS_MPROTECT = 10,
	SYS_SET_FS_BASE = 11,
	SYS_CLOCK_GET = 12,
	SYS_FUTEX_WAIT = 13,
	SYS_FUTEX_WAKE = 14,
};

#define WALL_PROT_NONE 0x0
#define WALL_PROT_READ 0x1
#define WALL_PROT_WRITE 0x2
#define WALL_PROT_EXEC 0x4

#define WALL_MAP_SHARED 0x000001 // Not supported yet, see sys_mmap.c
#define WALL_MAP_PRIVATE 0x000002
#define WALL_MAP_FIXED 0x000010 // Replaces whatever is at the address
#define WALL_MAP_ANONYMOUS 0x000020
#define WALL_MAP_GROWSDOWN 0x000100
#define WALL_MAP_NORESERVE 0x004000 // Accepted and ignored
#define WALL_MAP_POPULATE 0x008000
#define WALL_MAP_STACK 0x020000   // Accepted and ignored
#define WALL_MAP_HUGETLB 0x040000 // 2MB pages, anonymous only
#define WALL_MAP_FIXED_NOREPLACE 0x100000

// Argument block for SYS_MMAP (passed by pointer in arg0: six inputs plus a result don't fit in
// registers). Fixed-width types so the layout is identical in kernel and libc.
typedef struct {
	uint64_t hint;
	uint64_t len;
	int32_t prot;
	int32_t flags;
	int32_t fd;
	int32_t pad;
	int64_t off;
	uint64_t result; // out: mapped address, written only on success
} mmap_args_t;

#endif // WALLOS_SYSCALL_NUMS_H
