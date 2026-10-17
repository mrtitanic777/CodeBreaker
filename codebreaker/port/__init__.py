"""port -- recovered source made source a compiler of today builds.

What CodeBreaker recovers is the program as it was written: C of before the standard, for a 16-bit machine,
on the C library and assembler routines it was linked with. `port` takes such a tree (read only; it is never
changed) and writes, into a folder of its own, the same program for a modern compiler and C library:

    python -m codebreaker.port SOURCE_DIR OUT_DIR [--compiler gcc|clang|msvc] [--runtime PROFILE]

- early_c.py    the C itself: prototypes, explicit types, exact widths where the bytes of a structure matter,
                the declarations of the C library its own; then the compiler's own complaints mended in a loop
- runtimes/     what each old runtime was (data): which of the program's modules were its C library, and which
                of the calls it makes the platform layer supplies
- platform/     the platform layers: the old system's calls (DOS's handles, its keyboard line, the memory
                above 64K) over the C library of today
- compilers.json how each compiler is run and its complaints read
- verify.py     the original (under the emulator) and the port run on the same inputs, their outputs compared

Every change is listed in the report (PORT.md) with the reason; what could not be made portable is said so.
"""
