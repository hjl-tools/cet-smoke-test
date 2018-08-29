#define _GNU_SOURCE
#include <unistd.h>
#include <syscall.h>
#include <asm/prctl.h>

#ifndef ARCH_CET_STATUS
# define ARCH_CET_STATUS	0x3001
#endif

#define STRING_COMMA_LEN(STR) (STR), (sizeof (STR) - 1)

static int
length (const char *s)
{
  int l;

  for (l = 0; *s++ != 0; l++);
  return l;
}

void
error (const char *str)
{
  int fd = STDOUT_FILENO;
  syscall (SYS_write, fd, str, length (str));
  syscall (SYS_write, fd, STRING_COMMA_LEN ("\n"));
  syscall (SYS_exit, 1);
}

int
main ()
{
  unsigned long long cet_status[3];
  int ret;

  ret = syscall (SYS_arch_prctl, ARCH_CET_STATUS, cet_status);
  if (ret != 0)
    error ("arch_prctl (ARCH_CET_STATUS)");
  if ((cet_status[0] & 0x3) != 0x3)
    error ("ARCH_CET_STATUS");
  syscall (SYS_exit, 0);
  return 0;
}
