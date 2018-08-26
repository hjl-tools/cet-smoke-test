# Tests for glibc CET note parser.

To test newly built glibc, create config.make:

1. Define GLIBC-SOURCE-DIR to the glibc source directory.
2. Define GLIBC-BUILD-DIR to the glibc build directory.

All tests have CET violations.

Indirect branch without ENDBR:

1. ibt1: It has 2 NT_GNU_PROPERTY_TYPE_0 notes in the PT_NOTE segment.
CET should be disabled by glibc.
2. ibt2: It has a 16K property before GNU_PROPERTY_X86_FEATURE_1_AND
property in the PT_NOTE segment.  CET should be enabled by glibc.
3. ibt3: It has a 16K note before GNU_PROPERTY_X86_FEATURE_1_AND note
in the PT_NOTE segment.  CET should be enabled by glibc.
4. ibt4: It has a regular GNU_PROPERTY_X86_FEATURE_1_AND note in the
PT_NOTE segment.  CET should be enabled by glibc.

Shadow stack mismatch:

1. shstk1: It has 2 NT_GNU_PROPERTY_TYPE_0 notes in the PT_NOTE segment.
CET should be disabled by glibc.
2. shstk2: It has a 16K property before GNU_PROPERTY_X86_FEATURE_1_AND
property in the PT_NOTE segment.  CET should be enabled by glibc.
3. shstk3: It has a 16K note before GNU_PROPERTY_X86_FEATURE_1_AND note
in the PT_NOTE segment.  CET should be enabled by glibc.
4. shstk4: It has a regular GNU_PROPERTY_X86_FEATURE_1_AND note in the
PT_NOTE segment.  CET should be enabled by glibc.
