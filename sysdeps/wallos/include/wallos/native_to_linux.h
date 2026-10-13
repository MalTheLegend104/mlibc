#include "syscall_returns.h"

#define LINUX_EINVAL 22

static const int native_to_linux[] = {
    [E_NOERROR] = 0,
    [E_INVALID] = 22,     // EINVAL
    [E_EXIST] = 17,       // EEXIST
    [E_NOSYSCALL] = 38,   // ENOSYS
    [E_NOSPACE] = 28,     // ENOSPC
    [E_NOENTRY] = 2,      // ENOENT
    [E_BUSY] = 16,        // EBUSY
    [E_BADF] = 9,         // EBADF
    [E_IO] = 5,           // EIO
    [E_FAULT] = 14,       // EFAULT
    [E_NOMEM] = 12,       // ENOMEM
    [E_ACCESS] = 13,      // EACCES
    [E_NOTDIR] = 20,      // ENOTDIR
    [E_ISDIR] = 21,       // EISDIR
    [E_SPIPE] = 29,       // ESPIPE
    [E_NOTTY] = 25,       // ENOTTY
    [E_INTR] = 4,         // EINTR
    [E_AGAIN] = 11,       // EAGAIN
    [E_TIMEDOUT] = 110,   // ETIMEDOUT
    [E_MFILE] = 24,       // EMFILE
    [E_NOTSUP] = 95,      // EOPNOTSUPP
    [E_NAMETOOLONG] = 36, // ENAMETOOLONG
    [E_NOTEMPTY] = 39,    // ENOTEMPTY
    [E_OVERFLOW] = 75,    // EOVERFLOW
    [E_NFILE] = 23,       // ENFILE
};

int e_to_linux(int e) {
	if (e < 0 || (size_t)e >= sizeof(native_to_linux) / sizeof(native_to_linux[0])) {
		return LINUX_EINVAL;
	}
	return native_to_linux[e];
}
