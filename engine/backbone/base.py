#!/usr/bin/env python3
r"""base.py -- the modular toolchain API backbone (Phase 2).

One uniform interface per tool ROLE (compiler / assembler / linker / disassembler)
so the reconstruction pipeline swaps a tool by NAME instead of rewriting call sites.
Every concrete tool is an Adapter registered in the Backbone; callers ask the
backbone for a role (optionally a specific name/version) and get back a live
adapter with a single documented method. Adding MSVC 2.0 beside MSC 6.0a, or
Ghidra beside capstone, is then a new adapter file -- never an edit to the driver.

Design contract (mirrors the no-fabrication doctrine of the inverse engine):
  * An adapter that cannot run on this machine reports available()==False and is
    skipped by auto-selection -- it never silently substitutes a different tool.
  * Every call returns a ToolResult; ok==False carries the reason, never a guess.
"""
from __future__ import annotations
import abc


class ToolResult:
    """Uniform result across every backbone call."""
    __slots__ = ("ok", "output", "text", "stdout", "stderr", "meta")

    def __init__(self, ok, output=b"", text=b"", stdout="", stderr="", meta=None):
        self.ok = ok            # bool: did the tool succeed
        self.output = output    # bytes: primary artifact (obj / exe / ne image)
        self.text = text        # bytes: extracted code (_TEXT) when meaningful
        self.stdout = stdout    # str
        self.stderr = stderr    # str: the failure reason when ok is False
        self.meta = meta or {}  # dict: adapter-specific extras

    def __repr__(self):
        return "ToolResult(ok=%s, output=%dB, text=%dB%s)" % (
            self.ok, len(self.output or b""), len(self.text or b""),
            "" if self.ok else ", err=%r" % self.stderr[:80])


class Insn:
    """One decoded instruction -- the disassembler-role result element."""
    __slots__ = ("addr", "size", "bytes", "mnemonic", "op_str")

    def __init__(self, addr, size, raw, mnemonic, op_str):
        self.addr = addr; self.size = size; self.bytes = raw
        self.mnemonic = mnemonic; self.op_str = op_str

    def __repr__(self):
        return "%06x: %-8s %s" % (self.addr, self.mnemonic, self.op_str)


class Adapter(abc.ABC):
    """Base of every tool adapter. `kind` is the role; `name` is the swap key."""
    kind = None          # 'compiler' | 'assembler' | 'linker' | 'disassembler'
    name = None          # unique within its kind, e.g. 'msc-6.0a', 'capstone-x86-16'
    version = None       # human version string (optional)

    def available(self) -> bool:
        """True only if this tool can actually run here (paths resolve, deps present)."""
        return True

    def describe(self) -> dict:
        return {"kind": self.kind, "name": self.name, "version": self.version,
                "available": self.available()}


# ---- role interfaces: each declares the single method callers use --------------

class Compiler(Adapter):
    kind = "compiler"

    @abc.abstractmethod
    def compile(self, source: str, *, flags=None, **kw) -> ToolResult:
        """Compile C source; ToolResult.text carries the extracted _TEXT bytes."""


class Assembler(Adapter):
    kind = "assembler"

    @abc.abstractmethod
    def assemble(self, source: str, *, flags=None, **kw) -> ToolResult:
        """Assemble ASM source to an object; ToolResult.output carries the OBJ."""


class Linker(Adapter):
    kind = "linker"

    @abc.abstractmethod
    def link(self, objs, *, defs=None, libs=None, opts=None, **kw) -> ToolResult:
        """Link objects into an image; ToolResult.output carries the NE/exe bytes."""


class Disassembler(Adapter):
    kind = "disassembler"

    @abc.abstractmethod
    def disasm(self, code: bytes, *, base=0, **kw):
        """Decode bytes -> list[Insn]. Returns [] on nothing decodable."""


ROLES = {c.kind: c for c in (Compiler, Assembler, Linker, Disassembler)}


class Backbone:
    """Registry + selector. Swap tools by name; auto-select the first available."""

    def __init__(self):
        self._reg = {}          # (kind, name) -> adapter instance
        self._default = {}      # kind -> name (preferred when none requested)

    # -- registration -----------------------------------------------------------
    def register(self, adapter: Adapter, default=False):
        if adapter.kind not in ROLES:
            raise ValueError("unknown role %r" % adapter.kind)
        self._reg[(adapter.kind, adapter.name)] = adapter
        if default or adapter.kind not in self._default:
            self._default[adapter.kind] = adapter.name
        return adapter

    # -- lookup -----------------------------------------------------------------
    def get(self, kind, name=None):
        """Return the adapter for `kind`. name=None -> the default if available,
        else the first available one. Raises LookupError if none can run."""
        if kind not in ROLES:
            raise ValueError("unknown role %r" % kind)
        if name is not None:
            a = self._reg.get((kind, name))
            if a is None:
                raise LookupError("no %s named %r (have: %s)" % (
                    kind, name, ", ".join(self.names(kind)) or "none"))
            return a
        pref = self._default.get(kind)
        if pref and self._reg[(kind, pref)].available():
            return self._reg[(kind, pref)]
        for (k, nm), a in self._reg.items():
            if k == kind and a.available():
                return a
        raise LookupError("no available %s adapter (registered: %s)" % (
            kind, ", ".join(self.names(kind)) or "none"))

    # role-named conveniences: backbone.compiler('msc-6.0a') etc.
    def compiler(self, name=None): return self.get("compiler", name)
    def assembler(self, name=None): return self.get("assembler", name)
    def linker(self, name=None): return self.get("linker", name)
    def disassembler(self, name=None): return self.get("disassembler", name)

    # -- introspection ----------------------------------------------------------
    def names(self, kind):
        return [nm for (k, nm) in self._reg if k == kind]

    def catalog(self):
        return [a.describe() for a in self._reg.values()]

    def detect(self, kind, **hints):
        """Pick the best available adapter for `kind` given hints (e.g. arch, bits,
        version). Returns an adapter or None. Adapters opt in via a `score(**hints)`
        method (higher = better); default score is 0 for any available adapter."""
        best, best_s = None, -1
        for (k, nm), a in self._reg.items():
            if k != kind or not a.available():
                continue
            s = a.score(**hints) if hasattr(a, "score") else 0
            if s > best_s:
                best, best_s = a, s
        return best


# process-wide singleton (adapters self-register into this on import)
BACKBONE = Backbone()
