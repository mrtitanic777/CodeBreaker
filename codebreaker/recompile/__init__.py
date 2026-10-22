"""recompile -- a whole program, ahead of time, as C++ that builds and runs on a PC.

The other deliverable beside decompiling (docs/RECOMPILE.md): not source to read, but the
program itself carried to another machine. Today: Xbox 360 titles.

    disassemble.py    PowerPC (and VMX128) words as text, the code/data split
    discover.py       every function: .pdata, the entry, calls, pointers in the data, seeds
    builders/         each instruction as C++ statements (derived from the rexglue SDK's code
                      generator; BSD-3, see NOTICE)
    jumptable.py      the switch tables behind a bctr, so a switch stays a switch
    powerpc.py        a function, or the whole image, as C++; the runtime header
    project.py        the buildable project: sources, imports, kernel core, GPU subsystem, assets
    gpu.py            the native D3D12 GPU subsystem the project gets

    python -m codebreaker recompile <image or .xex> --all -o game.cpp
    python -m codebreaker recompile-project <image or .xex> -o <folder>
"""
