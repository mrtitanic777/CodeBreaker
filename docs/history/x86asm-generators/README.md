# x86asm's one-time generators

These scripts made x86asm's generated data (native/cb/build/x86asm) once, from NASM 3.02 while CodeBreaker still had its
NASM fork (`native/nasm`, deleted when x86asm replaced it). They read NASM's own generated tables and ran `nasm.exe`,
neither of which exists any more, so they cannot be run again: the files they wrote are now the source, edited by hand.
They are kept as the record of how each file was made.

| Script | Made |
|---|---|
| gen_insns_table.py | insns_table.cpp: the instruction templates and their byte-code, from NASM's insnsa.c / insnsb.c / iflag.c / insnsi.h |
| gen_encode_tests.py | encode_cases.inc, encode_masm_cases.inc: instruction lines with the bytes nasm.exe gave (and the lines it refused) |
| gen_preproc_tests.py | the expected output in preproc_test.cpp: `nasm.exe -E` and `--masm -E` of each case |
| genmac.py | macros_std.cpp: NASM's standard macro sets as data |
| genpptok.py | pptok.hpp / pptok.cpp: the preprocessor's directive tokens |
