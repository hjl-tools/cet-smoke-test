static void
foo (void)
{
}

static void
__attribute__ ((noinline, noclone))
xxx (void (*f) (void))
{
  f ();
}

void
bar (void)
{
  xxx (foo);
}
