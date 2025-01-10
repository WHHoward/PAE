#include "common.h"
#include "syscall.h"
#include "proc.h"
#include "fs.h"

extern void naive_uload(PCB *pcb, const char *filename);

static int programbrk = 0;

int sys_open(const char *pathname, int flags, int mode) {
  return fs_open(pathname, flags, mode);
}

//close
int sys_close(int fd) {
  return fs_close(fd);
}

//read
int sys_read(int fd, void *buf, size_t count) {
  return fs_read(fd, buf, count);
}

//write
int sys_write(int fd, const void *buf, size_t count) {
  return fs_write(fd, buf, count);
}

//lseek
int sys_lseek(int fd, size_t offset, int whence) {
  return fs_lseek(fd, offset, whence);
}

//brk
int sys_brk(void *addr) {
  programbrk = (uintptr_t)addr;
  return 0;
}

//yield
int sys_yield() {
  _yield();
  return 0;
}

//execve
int sys_execve(const char *pathname, char *const argv[], char *const envp[]) {
  printf("execve: %s\n", pathname);
  naive_uload(NULL, pathname);
  return 0;
}

//exit
void sys_exit(int status) {
//  _halt(status);
  sys_execve("/bin/init", NULL, NULL);
}
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_exit:
      sys_exit(a[1]);
      break;
    case SYS_yield:
      c->GPRx = sys_yield();
      break;
    case SYS_open:
      c->GPRx = sys_open((const char *)a[1], a[2], a[3]);
      break;
    case SYS_read:
      c->GPRx = sys_read(a[1], (void *)a[2], a[3]);
      break;
    case SYS_write:
      c->GPRx = sys_write(a[1], (const void *)a[2], a[3]);
      break;
    case SYS_close:
      c->GPRx = sys_close(a[1]);
      break;
    case SYS_lseek:
      c->GPRx = sys_lseek(a[1], a[2], a[3]);
      break;
    case SYS_brk:
      c->GPRx = sys_brk((void *)a[1]);
      break;
    case SYS_execve:
      c->GPRx = sys_execve((const char *)a[1], (char *const *)a[2], (char *const *)a[3]);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
