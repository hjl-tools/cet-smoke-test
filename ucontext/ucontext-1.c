#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

static ucontext_t ctx[5];
static volatile int done;

static void
__attribute__((noinline, noclone))
f2 (void)
{
  done++;
  puts ("swap contexts in f2");
  if (swapcontext (&ctx[4], &ctx[2]) != 0)
    {
      printf ("%s: setcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  puts ("end f2");
  exit (done == 2 ? EXIT_SUCCESS : EXIT_FAILURE);
}

static void
__attribute__((noinline, noclone))
f1 (void)
{
  puts ("start f1");
  if (getcontext (&ctx[2]) != 0)
    {
      printf ("%s: getcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  if (done)
    {
      puts ("set context in f1");
      if (setcontext (&ctx[3]) != 0)
	{
	  printf ("%s: setcontext: %m\n", __FUNCTION__);
	  exit (EXIT_FAILURE);
	}
    }
  f2 ();
}

static void
f3 (void)
{
  f1 ();
}

int
main (void)
{
  char st1[32768];
  puts ("making contexts");
  if (getcontext (&ctx[0]) != 0)
    {
      printf ("%s: getcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  if (getcontext (&ctx[1]) != 0)
    {
      printf ("%s: getcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  ctx[1].uc_stack.ss_sp = st1;
  ctx[1].uc_stack.ss_size = sizeof st1;
  ctx[1].uc_link = &ctx[0];
  makecontext (&ctx[1], (void (*) (void)) f3, 0);
  puts ("swap contexts");
  if (swapcontext (&ctx[3], &ctx[1]) != 0)
    {
      printf ("%s: setcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  if (done != 1)
    exit (EXIT_FAILURE);
  done++;
  puts ("set context");
  if (setcontext (&ctx[4]) != 0)
    {
      printf ("%s: setcontext: %m\n", __FUNCTION__);
      exit (EXIT_FAILURE);
    }
  exit (EXIT_FAILURE);
}
