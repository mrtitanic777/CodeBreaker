"""CodeBreaker -- reconstruct readable, compilable source from binaries, and prove it behaves
like the original. The program is C++ (native/cb: `codebreaker`, `codebreaker.exe`); the one thing in
this package is the terminal interface, a thin client of the program's service:

    ui/          the terminal interface (Textual): client.py talks to `codebreaker serve`, terminal.py shows it
    __main__     `python -m codebreaker [ui [module]]` starts it; every other command is the native program's

The rest of this folder is data the native program reads (tables, runtimes, toolchain profiles), not code.
"""
