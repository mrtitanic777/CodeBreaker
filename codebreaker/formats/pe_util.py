#!/usr/bin/env python3
r"""pe32util.py -- shared 32-bit PE helpers for the Layer-2 reconstruction
pipeline (classify32 / emit_c / ml32 / l2pipe).

Thin conveniences over the engine PE loader + capstone: carve a module, slice a
function's bytes, enumerate its named (exported) functions, disassemble. Resolves
the module through the active win95 target; `ensure_target()` makes the tools
runnable standalone as well as under `wt --target win95`.
"""
import os
import re
import sys

import capstone

from codebreaker.base import paths

_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
_CARVE = {}
_IMPORTS = {}

# Bootstrap: put the target dir on sys.path so bare sibling imports (cc32,
# segdiff, target, classify32, ...) resolve, and activate the win95 target so
# cc32's cores-based `target` works -- for standalone `python -m` use. Idempotent
# and harmless when `wt` already did this. Importing pe32util FIRST in each
# pipeline module is enough to arm the whole set.


def ensure_target():
    """Point cores at win95 + cd into a data root, idempotently (mirrors
    msvc2cov._setup so a tool works with no `wt` wrapper)."""
    from codebreaker.base import registry as cores
    roots = ([os.path.join(os.environ["WT_DATA_ROOT"], "win95")]
             if os.environ.get("WT_DATA_ROOT") else [])
    roots += [paths.data_root(), os.getcwd()]
    for r in roots:
        if os.path.isdir(os.path.join(r, "files")) or os.path.isdir(os.path.join(r, "src")):
            os.chdir(r)
            return


def carve(module):
    ensure_target()
    if module not in _CARVE:
        from codebreaker.base import registry as _registry
        target = _registry.get("target")
        from codebreaker.formats import pe_trace
        _CARVE[module] = pe_trace.PeCarve(target.resolve(module))
    return _CARVE[module]


def image_base(module):
    return carve(module).pe.image_base


def imports(module):
    if module not in _IMPORTS:                            # memoized: re-parsed-per-function was O(n^2)
        _IMPORTS[module] = dict(carve(module).pe.imports())
    return _IMPORTS[module]


def disasm(b, va=0):
    return list(_MD.disasm(b, va))


