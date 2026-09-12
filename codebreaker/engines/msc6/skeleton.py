#!/usr/bin/env python3
r"""skeleton.py -- msc6 STEP-2 skeleton via CodeBreaker's ir_engine lift.

Replaces the external-Ghidra step-2 decompile with CodeBreaker's OWN 16-bit-native,
symbol-resolved lift (`engine/ir_engine/il.py`). Given a byte-exact reconstructed .asm
function, it lifts to structured C -- imports named, control flow recovered, and NONE of
Ghidra's `unaff_SS`/`CONCAT` segment garbage -- as the starting point the recipe-driven
search refines toward byte-exact.

    wt msc6 skeleton <asm_file> [--faithful]

`--faithful` keeps every value-producing instruction (the lossless material the byte-exact
search needs); the default readable path runs dead-store elimination for a cleaner draft.
"""
import os, sys
from codebreaker.core import cores
cores.set_target(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                              "targets", "win95"))
from codebreaker.ir import il


def skeleton(asm_path, faithful=False):
    """Lift one reconstructed .asm function to a C step-2 skeleton (string)."""
    name = os.path.splitext(os.path.basename(asm_path))[0]
    return il.lift_asm_function(asm_path, name, faithful=faithful)


if __name__ == "__main__":
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if not args:
        print(__doc__); sys.exit(2)
    print(skeleton(args[0], "--faithful" in sys.argv))
