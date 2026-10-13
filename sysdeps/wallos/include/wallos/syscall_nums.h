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
