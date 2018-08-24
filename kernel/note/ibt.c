#define _GNU_SOURCE
#include <unistd.h>
#include <syscall.h>

static void
__attribute__ ((noinline, noclone))
foo (void (*f) (void))
{
  f ();
}

static void
bar (void)
{
}

int
main ()
{
  foo (bar);
  syscall (SYS_exit, 0);
  return 0;
}
