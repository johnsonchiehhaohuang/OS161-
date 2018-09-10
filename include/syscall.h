#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys_write(int fd, const void *buf, size_t nbytes, int* retval);
int sys_read(int fd, char* buf, size_t nbytes, int* retval); 
int sys_getpid(void);
int sys_fork(struct trapframe *tf, pid_t* retval);
int sys_waitpid(pid_t pid, int *returncode, int flags, int* retval);
void sys_exit(int code);
int sys_execv(char* program, char** uargs);
int sys___time(time_t *seconds, unsigned long *nanoseconds, int* retval);
void *sys_sbrk(int amount, int* retval);

#endif /* _SYSCALL_H_ */