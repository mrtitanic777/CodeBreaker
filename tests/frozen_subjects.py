"""What is frozen, beside the passes of the C text: each format reader and each kind of
description, run on one small retail binary, reduced to plain data. The same functions make the
frozen copy (tools/freeze_tests.py) and the one a test compares with it (tests/test_frozen.py).

A subject is (group, name, corpus file, function of the file's path -> plain data). The corpus
is not in the repository: a test whose file is missing, or not the same file (its SHA-256 is
kept with the frozen copy), is skipped."""
import hashlib
import os
import re
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

LIST_CAP = 300            # items of a list kept whole; past that, its length and a digest


def plain(v, depth=0):
    """Any reader's result as JSON-able data: objects by their public attributes, large bytes
    and long lists by their size and SHA-256 (a change still shows; the fixture stays small)."""
    if depth > 8:
        return "<deep>"
    if v is None or isinstance(v, (bool, int, float, str)):
        return v
    if isinstance(v, (bytes, bytearray)):
        v = bytes(v)
        return v.hex() if len(v) <= 32 else {"len": len(v), "sha256": hashlib.sha256(v).hexdigest()}
    if isinstance(v, dict):
        return {str(k): plain(x, depth + 1) for k, x in sorted(v.items(), key=lambda kv: repr(kv[0]))}
    if isinstance(v, (list, tuple, set, frozenset)):
        items = sorted(v, key=repr) if isinstance(v, (set, frozenset)) else list(v)
        if len(items) > LIST_CAP:
            digest = hashlib.sha256(repr([plain(x, depth + 1) for x in items]).encode()).hexdigest()
            return {"count": len(items), "head": [plain(x, depth + 1) for x in items[:20]], "sha256": digest}
        return [plain(x, depth + 1) for x in items]
    if hasattr(v, "__dict__"):
        return {k: plain(x, depth + 1) for k, x in sorted(vars(v).items())
                if not k.startswith("_") and not callable(x)}
    return repr(v)


def sha256(path):
    with open(path, "rb") as f:
        return hashlib.sha256(f.read()).hexdigest()


def _local(text, *paths):
    """A text with this machine's paths taken out (a description names its module and folder)."""
    for p, word in paths:
        for spelling in {p, p.replace("\\", "/"), p.replace("/", "\\"), p.lower()}:
            text = text.replace(spelling, word)
    return text


# --- format readers ------------------------------------------------------------------------------

def ne(path):
    from codebreaker.formats.ne import NEFile
    f = NEFile(path)
    return {"module": f.module_name(), "segments": plain(f.segments()), "entries": plain(f.entries()),
            "exports": plain(f.export_ordinals()), "resident": plain(f.resident_names()),
            "nonresident": plain(f.nonresident_names()), "refs": plain(f.module_refs()),
            "relocations": {str(s["idx"]): plain(f.relocations(s)) for s in f.segments() if s["file_off"]},
            "resources": plain(f.resources())}


def pe(path):
    from codebreaker.formats.pe import PEFile
    f = PEFile(path)
    return {"machine": f.machine_name(), "dll": f.is_dll(), "module": f.module_name(),
            "exports": plain(f.exports()), "imports": plain(f.imports()),
            "code_sections": plain(f.code_sections()), "relocs": plain(f.base_relocs()),
            "optional": plain(f.optional_header()), "build": plain(f.build_meta()),
            "codeview": plain(f.debug_codeview())}


def le(path):
    from codebreaker.formats.le import LEFile
    f = LEFile.from_file(path)
    return {"header": plain(f.header()), "description": f.description(), "ddb": plain(f.ddb()),
            "name": plain(f.resident_name()), "imports": plain(f.import_modules()),
            "fixups": plain(f.fixups())}


def dos(path):
    from codebreaker.formats import dos as D
    return {"kind": D.kind(path), "packer": D.packer(path),
            "program": {k: v for k, v in plain(D.read(path)).items() if k != "path"}}


def cab(path):
    from codebreaker.formats import cab as C
    c = C.read(path)
    return {"info": plain(c.info), "files": plain(c.files)}


def fonts(path):
    from codebreaker.formats import fonts as F
    return [{"describe": f.describe(), "glyph_A": plain(f.glyph("A")), "font": plain(f)} for f in F.fonts_in(path)]


def resources(path):
    from codebreaker.formats import resources as R
    return plain(R.read(path))


def detect(path):
    from codebreaker.formats import detect as D
    return {"signature": plain(D.signature(path)), "format": plain(D.format_of(path)), "lane": plain(D.lane(path))}


def inspect(path):
    from codebreaker.formats import inspect as I
    d = plain(I.describe(path))
    d.pop("path", None)
    return d


def pe_functions(path):
    from codebreaker.formats import pe_functions as P
    return plain(P.function_spans(path))


# --- descriptions (decompile/describe.py) --------------------------------------------------------

def _described(path, make):
    with tempfile.TemporaryDirectory(prefix="cbfrozen_") as folder:
        desc = make(path, folder)
        with open(desc, encoding="utf-8") as f:
            text = f.read()
        # the bytes it hands the decompiler (describe.bin ...), by digest
        files = {}
        for dirpath, _dirs, names in os.walk(folder):
            for n in names:
                p = os.path.join(dirpath, n)
                if os.path.normcase(p) != os.path.normcase(desc):
                    files[os.path.relpath(p, folder).replace("\\", "/")] = sha256(p)
    return {"description": _local(text, (folder, "<folder>"), (path, "<module>"), (ROOT, "<root>")),
            "files": files}


def describe16(path):
    from codebreaker.decompile import describe as D
    return _described(path, lambda p, f: D.describe16(p, f).path)


def describe32(path):
    from codebreaker.decompile import describe as D
    return _described(path, lambda p, f: D.describe32(p, D.function_starts(p), f)[0])


def describe_dos(path):
    from codebreaker.decompile import describe as D
    return _described(path, lambda p, f: D.describe_dos(p, f).path)


def describe_le(path):
    from codebreaker.decompile import describe as D
    from codebreaker.formats import le as L
    out = {}
    for n, lf in L.open_vxds(path):
        out[n] = _described(path, lambda p, f: D.describe_le(lf, n, os.path.join(f, re.sub(r"\W", "_", n))).path)
    return out


# --- a whole module's compilable C (decompile/c_text.py; needs the native decompiler) ----------

def c_text16(path):
    from codebreaker.decompile import c_text
    with tempfile.TemporaryDirectory(prefix="cbfrozen_") as work:
        m = c_text._compilable16(path, work)
    return {"header": m.header, "texts": m.texts, "order": m.order,
            "imports": {k: list(v) for k, v in m.imports.items()},
            "near": sorted(m.near), "wide": sorted(m.wide), "novalue": sorted(m.novalue),
            "failed": m.failed, "interrupts": plain(m.interrupts),
            "code_data": {str(k): v for k, v in sorted(m.code_data.items())}}


def c_text32(path):
    from codebreaker.decompile import c_text
    from codebreaker.verify.lanes import win32
    with tempfile.TemporaryDirectory(prefix="cbfrozen_") as work:
        header, texts, order, _by_va, used, in_ecx, in_edx = c_text.compilable32(
            path, win32.LANES["pe"].functions(path), work)[:7]
    return {"header": header, "texts": texts, "order": order, "imports": used,
            "in_ecx": sorted(in_ecx), "in_edx": sorted(in_edx)}


def _corpus(name):
    from codebreaker.base import corpus
    return corpus.retail(name)


def _setup(name):
    from codebreaker.base import corpus
    p = os.path.join(corpus.root(), "setup", "media", name)
    return p if os.path.isfile(p) else None


# (group, name, file, function)
SUBJECTS = [
    ("formats", "ne_lzexpand", lambda: _corpus("lzexpand.dll"), ne),
    ("formats", "ne_keyboard", lambda: _corpus("keyboard.drv"), ne),
    ("formats", "pe_lz32", lambda: _corpus("lz32.dll"), pe),
    ("formats", "pe_notepad", lambda: _corpus("notepad.exe"), pe),
    ("formats", "le_vmouse", lambda: _corpus("vmouse.vxd"), le),
    ("formats", "dos_mode", lambda: _corpus("mode.com"), dos),
    ("formats", "cab_mini", lambda: _setup("MINI.CAB"), cab),
    ("formats", "fonts_vgafix", lambda: _corpus("vgafix.fon"), fonts),
    ("formats", "resources_notepad", lambda: _corpus("notepad.exe"), resources),
    ("formats", "resources_lzexpand", lambda: _corpus("lzexpand.dll"), resources),
    ("formats", "detect_vmouse", lambda: _corpus("vmouse.vxd"), detect),
    ("formats", "detect_mode", lambda: _corpus("mode.com"), detect),
    ("formats", "inspect_lzexpand", lambda: _corpus("lzexpand.dll"), inspect),
    ("formats", "pe_functions_lz32", lambda: _corpus("lz32.dll"), pe_functions),
    ("describe", "ne_lzexpand", lambda: _corpus("lzexpand.dll"), describe16),
    ("describe", "ne_keyboard", lambda: _corpus("keyboard.drv"), describe16),
    ("describe", "pe_lz32", lambda: _corpus("lz32.dll"), describe32),
    ("describe", "le_vmouse", lambda: _corpus("vmouse.vxd"), describe_le),
    ("describe", "dos_mode", lambda: _corpus("mode.com"), describe_dos),
]

# recorded with the passes (tools/freeze_tests.py c_text); replayed only where the decompiler is built
MODULE_SUBJECTS = [
    ("c_text_module", "lzexpand_dll", lambda: _corpus("lzexpand.dll"), c_text16),
    ("c_text_module", "lz32_dll", lambda: _corpus("lz32.dll"), c_text32),
]
