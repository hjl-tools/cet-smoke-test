#include <stdio.h>
#include <stdlib.h>

extern int in_dso_1 (void);

int
main (void)
{
  if (in_dso_1 () != 0x1234678)
    {
      puts ("in_dso_1 () != 0x1234678");
      exit (1);
    }

  return 0;
}
