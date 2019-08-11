#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

/////////////////////HW1
struct syscall_log_info {
int syscall_num;
int syscall_res;
int time;
};
///////////////////HW1

int enable_syscalls_logging (pid_t pid, int size) {
	unsigned int res;
	__asm__(
			"int $0x80;"
			: "=a" (res)
			: "0" (243) ,"b" (pid) ,"c" (size)
			: "memory"
	);

	
	if (res >= (unsigned long)(-125) ){
		errno = -res;
		res = -1;
	}
	return (int) res;
}


int disable_syscalls_logging (pid_t pid) {
	unsigned int res;

	__asm__(
			"int $0x80;"
			: "=a" (res)
			: "0" (244) ,"b" (pid)
			: "memory"
	);
	if (res >= (unsigned long)(-125) ){
		errno = -res;
		res = -1;
	}
	return (int) res;
}

int get_syscalls_log(pid_t pid, int size, struct syscall_log_info* user_mem) {
	unsigned int res;
	
	__asm__(
			"int $0x80;"
			: "=a" (res)
			: "0" (245) ,"b" (pid) ,"c" (size), "d" (user_mem)
			: "memory"
	);
	
	if (res >= (unsigned long)(-125) ){
		errno = -res;
		res = -1;
	}
	return (int) res;
}
