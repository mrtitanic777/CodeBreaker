#!/usr/bin/env python3
r"""engine.backbone -- the modular toolchain API backbone (Phase 2).

Import this package and you get BACKBONE, a registry with every available adapter
already registered. Swap a tool by name; auto-select the best one by role or hint:

    from src.toolchain.backbone import BACKBONE as bb
    bb.compiler().compile("int far pascal WEP(){return 1;}", flags="/AM /G2 /Gsw /Os")
    bb.disassembler("capstone-x86-16").disasm(code_bytes)
    bb.detect("compiler", bits=16, compiler="msc-6.0a")          # pick by hint
    bb.catalog()                                                  # what's registered/available

Adding a compiler/linker/assembler/disassembler is a new file under adapters/ with a
`register(backbone)` function, listed in _ADAPTERS below -- no call-site changes.
"""
from src.toolchain.backbone.base import BACKBONE, Backbone, ToolResult, Insn  # noqa: F401
from src.toolchain.backbone.base import Compiler, Assembler, Linker, Disassembler  # noqa: F401

_ADAPTERS = (
    "engine.backbone.adapters.msc6_compiler",
    "engine.backbone.adapters.msvc_compiler",
    "engine.backbone.adapters.capstone_disasm",
    "engine.backbone.adapters.masm_assembler",
    "engine.backbone.adapters.nasm_assembler",
    "engine.backbone.adapters.link_linker",
)


def _load(backbone=BACKBONE):
    import importlib
    for mod in _ADAPTERS:
        try:
            m = importlib.import_module(mod)
            m.register(backbone)
        except Exception as e:                      # a broken adapter never breaks the rest
            backbone.__dict__.setdefault("_errors", []).append((mod, str(e)))
    return backbone


_load()
