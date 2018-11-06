#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern int in_dso_1 (void);

static int
check (void)
{
  if (in_dso_1 () != 0x1234678)
    {
      puts ("in_dso_1 () != 0x1234678");
      exit (1);
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  if (argc > 1)
    return check ();

  execl (argv[0], argv[0], "--restart",  NULL);

  abort ();

  return -1;
}
