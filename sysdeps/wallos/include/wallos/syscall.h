#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

#include "syscall_nums.h"
#include "syscall_returns.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Register convention:
 *   RAX (entry)  syscall_num
 *   RDI          arg0
 *   RSI          arg1
 *   RDX          arg2
 *   R10          arg3
 *   R8           arg4
 *   R9           arg5
 *   RAX (return) ret
 *
 * The `syscall` instruction itself clobbers RCX (saved RIP) and R11 (saved RFLAGS).
 */

__attribute__((__always_inline__)) static inline long syscalln0(uint64_t call) {
	long ret;
	asm volatile("syscall" : "=a"(ret) : "a"(call) : "rcx", "r11", "memory");
	return ret;
}

__attribute__((__always_inline__)) static inline long syscalln1(uint64_t call, uint64_t arg0) {
	long ret;
	asm volatile("syscall" : "=a"(ret) : "a"(call), "D"(arg0) : "rcx", "r11", "memory");
	return ret;
}

__attribute__((__always_inline__)) static inline long
syscalln2(uint64_t call, uint64_t arg0, uint64_t arg1) {
	long ret;
	asm volatile("syscall" : "=a"(ret) : "a"(call), "D"(arg0), "S"(arg1) : "rcx", "r11", "memory");
	return ret;
}

__attribute__((__always_inline__)) static inline long
syscalln3(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
	long ret;
	asm volatile("syscall"
	             : "=a"(ret)
	             : "a"(call), "D"(arg0), "S"(arg1), "d"(arg2)
	             : "rcx", "r11", "memory");
	return ret;
}

__attribute__((__always_inline__)) static inline long
syscalln4(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	long ret;
	register uint64_t arg3r asm("r10") = arg3; // put arg3 in r10
	asm volatile("syscall"
	             : "=a"(ret)
	             : "a"(call), "D"(arg0), "S"(arg1), "d"(arg2), "r"(arg3r)
	             : "rcx", "r11", "memory");
	return ret;
}

__attribute__((__always_inline__)) static inline long syscalln5(
    uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4
) {
	long ret;
	register uint64_t arg3r asm("r10") = arg3; // put arg3 in r10
	register uint64_t arg4r asm("r8") = arg4;  // put arg4 in r8
	asm volatile("syscall"
	             : "=a"(ret)
	             : "a"(call), "D"(arg0), "S"(arg1), "d"(arg2), "r"(arg3r), "r"(arg4r)
	             : "rcx", "r11", "memory");
	return ret;
}

__attribute__((__always_inline__)) static inline long syscalln6(
    uint64_t call,
    uint64_t arg0,
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5
) {
	long ret;
	register uint64_t arg3r asm("r10") = arg3; // put arg3 in r10
	register uint64_t arg4r asm("r8") = arg4;  // put arg4 in r8
	register uint64_t arg5r asm("r9") = arg5;  // put arg5 in r9
	asm volatile("syscall"
	             : "=a"(ret)
	             : "a"(call), "D"(arg0), "S"(arg1), "d"(arg2), "r"(arg3r), "r"(arg4r), "r"(arg5r)
	             : "rcx", "r11", "memory");
	return ret;
}

#ifdef __cplusplus
}
__attribute__((__always_inline__)) static inline long _syscall(uint64_t call) {
	return syscalln0(call);
}
__attribute__((__always_inline__)) static inline long _syscall(uint64_t call, uint64_t arg0) {
	return syscalln1(call, arg0);
}
__attribute__((__always_inline__)) static inline long
_syscall(uint64_t call, uint64_t arg0, uint64_t arg1) {
	return syscalln2(call, arg0, arg1);
}
__attribute__((__always_inline__)) static inline long
_syscall(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
	return syscalln3(call, arg0, arg1, arg2);
}
__attribute__((__always_inline__)) static inline long
_syscall(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	return syscalln4(call, arg0, arg1, arg2, arg3);
}
__attribute__((__always_inline__)) static inline long
_syscall(uint64_t call, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
	return syscalln5(call, arg0, arg1, arg2, arg3, arg4);
}
__attribute__((__always_inline__)) static inline long _syscall(
    uint64_t call,
    uint64_t arg0,
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5
) {
	return syscalln6(call, arg0, arg1, arg2, arg3, arg4, arg5);
}

template <typename... T>
__attribute__((__always_inline__)) static inline long syscall(uint64_t call, T... args) {
	return _syscall(call, (uint64_t)(args)...);
}
#else
#define GET_SYSCALL(a0, a1, a2, a3, a4, a5, a6, name, ...) name
#define syscall(...)                                                                                       \
	GET_SYSCALL(__VA_ARGS__, syscalln6, syscalln5, syscalln4, syscalln3, syscalln2, syscalln1, syscalln0)( \
	    __VA_ARGS__                                                                                        \
	)
#endif

#endif // SYSCALL_H
