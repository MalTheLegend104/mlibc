#include "mlibc/tcb.hpp"
#include <abi-bits/errno.h>
#include <bits/ensure.h>
#include <mlibc/all-sysdeps.hpp>
#include <string.h>
#include <wallos/syscall.h>

// ANCHOR: stub
#define STUB(msg)                                                                                  \
	({                                                                                             \
		__ensure(!"STUB function was called: " msg);                                               \
		__builtin_unreachable();                                                                   \
	})
// ANCHOR_END: stub

namespace mlibc {

// The kernel returns an E_* status in rax (0 = success). Every result comes back through a user
// out-pointer that the kernel only writes on success. abi-bits/errno.h must use the kernel's E_*
// numbers, so the status is handed to mlibc unchanged.
static inline int status(long r) { return static_cast<int>(r); }

void Sysdeps<LibcPanic>::operator()() {
	// sysdep<LibcLog>("!!! mlibc panic !!!\r\n");
	sysdep<Exit>(-1);
	__builtin_trap();
}

void Sysdeps<LibcLog>::operator()(const char *msg) { syscall(SYS_DEBUG_LOG, msg, strlen(msg)); }

void Sysdeps<Exit>::operator()(int code) {
	sysdep<LibcLog>("!!! mlibc exit !!!\r\n");
	syscall(SYS_EXIT, code);
	__builtin_unreachable();
}

int Sysdeps<Isatty>::operator()(int fd) {
	// Returns 0 for a tty, ENOTTY otherwise. There is no ioctl syscall yet, so treat the
	// standard descriptors (all backed by the console) as ttys and everything else as not.
	// Replace with ioctl(fd, TCGETS) once sys_ioctl exists.
	return (fd >= 0 && fd <= 2) ? 0 : ENOTTY;
}

int Sysdeps<Write>::operator()(int fd, void const *buf, size_t size, ssize_t *ret) {
	STUB("Write");
	size_t written = 0;
	int err = status(syscall(SYS_WRITE, fd, buf, size, &written));
	if (err == 0) {
		*ret = static_cast<ssize_t>(written);
	}
	return err;
}

int Sysdeps<Read>::operator()(int fd, void *buf, unsigned long size, long *ret) {
	STUB("Read");
	size_t got = 0;
	int err = status(syscall(SYS_READ, fd, buf, size, &got));
	if (err == 0) {
		*ret = static_cast<long>(got);
	}
	return err;
}

int Sysdeps<Open>::operator()(const char *path, int flags, unsigned int mode, int *fd) {
	STUB("Open");
	// This contains all the O_* mappings
	// We're going to follow the linux ABI for this just because I don't want to deal with mapping
	// them abis\linux\fcntl.h
	int out = -1;
	int err = status(syscall(SYS_OPEN, path, flags, mode, &out));
	if (err == 0) {
		*fd = out;
	}
	return err;
}

int Sysdeps<Close>::operator()(int fd) {
	STUB("Close");
	return status(syscall(SYS_CLOSE, fd));
}

int Sysdeps<Seek>::operator()(int fd, off_t offset, int whence, off_t *new_offset) {
	if (fd == 0) {
		// stdin is fd 0
		// this should be changed when I actually have the syscall
		// things can technically close the stdin fd and open a new one
		return ESPIPE;
	}

	off_t out = 0;
	int err = status(syscall(SYS_SEEK, fd, offset, whence, &out));
	if (err == 0) {
		*new_offset = out;
	}
	return err;
}

int Sysdeps<TcbSet>::operator()(void *pointer) {
	// STUB("TcbSet");
	return status(syscall(SYS_SET_FS_BASE, pointer));
}

int Sysdeps<VmMap>::operator()(
    void *hint, size_t size, int prot, int flags, int fd, off_t offset, void **window
) {
	mmap_args_t args = {};
	args.hint = reinterpret_cast<uint64_t>(hint);
	args.len = size;
	args.prot = prot;
	args.flags = flags;
	args.fd = fd;
	args.off = offset;
	int err = status(syscall(SYS_MMAP, &args));
	if (err == 0) {
		*window = reinterpret_cast<void *>(args.result);
	}
	return err;
}

int Sysdeps<VmUnmap>::operator()(void *pointer, size_t size) {
	return status(syscall(SYS_MUNMAP, pointer, size));
}

// This is part of an option that I don't want to explicitly support yet (posix?)
// The kernel supports it, I'll deal with it later
// int Sysdeps<VmProtect>::operator()(void *pointer, size_t size, int prot) {
// 	return status(syscall(SYS_MPROTECT, pointer, size, prot));
// }

int Sysdeps<AnonAllocate>::operator()(size_t size, void **pointer) {
	return sysdep<VmMap>(
	    nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0, pointer
	);
}

int Sysdeps<AnonFree>::operator()(void *pointer, unsigned long size) {
	return sysdep<VmUnmap>(pointer, size);
}

// Kernel contract: truncate `expected` to int, return E_AGAIN if *pointer != expected,
// E_TIMEDOUT on expiry, E_INTR if interrupted. Check whether this mlibc version passes a
// relative or an absolute timeout.
int Sysdeps<FutexWait>::operator()(int *pointer, int expected, timespec const *time) {
	STUB("FutexWait");
	return status(syscall(SYS_FUTEX_WAIT, pointer, expected, time));
}

int Sysdeps<FutexWake>::operator()(int *pointer, bool all) {
	STUB("FutexWake");
	return status(syscall(SYS_FUTEX_WAKE, pointer, all));
}

int Sysdeps<ClockGet>::operator()(int clock, time_t *secs, long *nanos) {
	STUB("ClockGet");
	timespec ts = {};
	int err = status(syscall(SYS_CLOCK_GET, clock, &ts));
	if (err == 0) {
		*secs = ts.tv_sec;
		*nanos = ts.tv_nsec;
	}
	return err;
}

} // namespace mlibc
