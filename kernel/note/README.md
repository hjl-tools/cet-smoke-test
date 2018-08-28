# Tests for kernel CET note parser.

All tests have CET violations.

Indirect branch without ENDBR:

1. ibt1: It has 2 NT_GNU_PROPERTY_TYPE_0 notes in the PT_NOTE segment.
CET should be disabled by kernel.
2. ibt2: It has a 16K property before GNU_PROPERTY_X86_FEATURE_1_AND
property in the PT_NOTE segment.  CET should be enabled by kernel.
3. ibt3: It has a 16K note before GNU_PROPERTY_X86_FEATURE_1_AND note
in the PT_NOTE segment.  CET should be enabled by kernel.
4. ibt4: It has a regular GNU_PROPERTY_X86_FEATURE_1_AND note in the
PT_NOTE segment.  CET should be enabled by kernel.
5. ibt5: It has 2 NT_GNU_PROPERTY_TYPE_0 notes in the PT_NOTE segment.
CET should be disabled by kernel.

Shadow stack mismatch:

1. shstk1: It has 2 NT_GNU_PROPERTY_TYPE_0 notes in the PT_NOTE segment.
CET should be disabled by kernel.
2. shstk2: It has a 16K property before GNU_PROPERTY_X86_FEATURE_1_AND
property in the PT_NOTE segment.  CET should be enabled by kernel.
3. shstk3: It has a 16K note before GNU_PROPERTY_X86_FEATURE_1_AND note
in the PT_NOTE segment.  CET should be enabled by kernel.
4. shstk4: It has a regular GNU_PROPERTY_X86_FEATURE_1_AND note in the
PT_NOTE segment.  CET should be enabled by kernel.
5. shstk5: It has 2 NT_GNU_PROPERTY_TYPE_0 notes in the PT_NOTE segment.
CET should be disabled by kernel.
