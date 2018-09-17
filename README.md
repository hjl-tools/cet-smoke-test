# Intel Control-flow Enforcement Technology (CET) Smoke Test

1. kernel: Tests for CET kernel.
2. note: Tests for glibc CET note parser.
3. ucontext: Tests for ucontext functions.
4. quick: Tests to verify that the whole system has CET enabled.

To clean up all tests:

$ make clean

To run all tests:

$ make

1. To run CET smoke test for x86-64, create an empty cet.config file.
2. To run CET smoke test for x32, create cet.config file with

TARGET=x32
