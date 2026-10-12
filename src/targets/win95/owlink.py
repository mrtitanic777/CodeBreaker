#!/usr/bin/env python3
r"""owlink.py -- link an authored module into a real binary with OpenWatcom, resolving REAL imports.

The compile lane proves our authored C is a legal translation unit; this lane proves it LINKS into a
working module -- the "runs from our source" step. The three undefined-symbol classes a module leaves
after compile are handled honestly:

  1. CRT stubs (__STK, ...)      -> compile with -s (no stack probe); self-contained.
  2. Win16 API imports           -> real wlink `import` directives from the binary's own import table
                                    (win95dec --imports). NOT return-0 stubs -- the linked module calls
                                    the actual KERNEL/USER/GDI/... export.
  3. internal sub_XXXX the        -> stubbed (flagged): functions the decompiler has not recovered yet.
     decompiler didn't emit          This is the genuine completeness gap, reported per module.

    wt --target win95 owlink <module> [--diff]

Reports: compiled OK, #real-imports resolved, #internal stubs, linked output + size, and (with --diff)
a byte comparison against the retail binary. Byte-identity is NOT expected (different toolchain); the
point is a functional module built from our source that calls the real APIs.
"""
import os
import re
import sys
import glob
import subprocess

from src.core import paths, toolchain
from src.targets.win95 import authsrc, owcheck


def _ow():
    return toolchain.openwatcom_root()


def _dec(fmt):
    return authsrc._native(fmt)


def _find_binary(module):
    dr = authsrc._dr()
    cands = glob.glob(os.path.join(dr, "corpus", "files", "**", module + ".*"), recursive=True)
    # The decoder resolves imported ordinals -> API names by loading the sibling KERNEL/USER/GDI
    # modules from the TARGET binary's own directory. Several corpus locations hold the same file
    # (WINDOWS/SYSTEM, setup/inf, ...) but only the real system directory carries those siblings, so
    # a copy from setup/inf resolves every import to a bare MOD_ordinal that never matches the call
    # names in the authored C (which WAS decompiled from the WINDOWS/SYSTEM copy). Prefer the copy
    # whose directory actually contains kernel/gdi/user so imports resolve to real names and link.
    def _has_siblings(p):
        d = os.path.dirname(p)
        return any(os.path.isfile(os.path.join(d, n)) for n in
                   ("KERNEL.EXE", "kernel.exe", "GDI.EXE", "gdi.exe", "USER.EXE", "user.exe"))
    cands.sort(key=lambda p: (0 if _has_siblings(p) else 1,
                              0 if os.path.sep + "SYSTEM" + os.path.sep in p.upper() else 1))
    if cands:
        return cands[0]
    for rel in (("corpus/files/setup/inf/%s.exe" % module), ("corpus/files/setup/inf/%s.dll" % module)):
        p = os.path.join(dr, rel)
        if os.path.isfile(p):
            return p
    return None


def _imports(binpath, fmt):
    """[(module, apiname)] from the binary's own import table (NE + PE)."""
    if fmt not in ("NE", "PE"):
        return []
    dec = _dec(fmt)
    out = subprocess.run([dec, binpath, "--imports"], capture_output=True, text=True).stdout
    imps = []
    for line in out.splitlines():
        p = line.split("\t")
        if len(p) == 2 and p[1].strip():
            imps.append((p[0].strip(), p[1].strip()))
    return imps


def _exports(binpath, fmt):
    """[(ordinal_or_None, name, addr_or_None, is_data)] from the module's own export/entry table. NE
    has ordinals + a linear address + a code/data tag per export (addr lets the build lane resolve an
    export by ADDRESS when our recovered function name differs from the retail export name); PE lists
    names only."""
    if fmt not in ("NE", "PE"):
        return []
    dec = _dec(fmt)
    out = subprocess.run([dec, binpath, "--exports"], capture_output=True, text=True).stdout
    exps = []
    for line in out.splitlines():
        p = line.split("\t")
        if fmt == "NE" and len(p) >= 2 and p[1].strip():
            addr = int(p[2], 16) if len(p) >= 3 and p[2].strip() else None
            is_data = len(p) >= 4 and p[3].strip() == "DATA"
            exps.append((p[0].strip(), p[1].strip(), addr, is_data))
        elif fmt == "PE" and p and p[0].strip():
            rva = int(p[1], 16) if len(p) >= 2 and p[1].strip() else None
            exps.append((None, p[0].strip(), rva, False))
    return exps


def _valid_obj(op):
    """A real OMF object exists and is non-empty (starts with THEADR/LHEADR 0x80/0x82). A 0-byte or
    truncated file (wcc killed mid-write) is NOT valid and must not be linked (E2146)."""
    try:
        with open(op, "rb") as f:
            b = f.read(1)
        return len(b) == 1 and b[0] in (0x80, 0x82)
    except OSError:
        return False


def _compile_piece(cc, flags, cfile, bld, env):
    """Compile one .c piece, returning its .obj name or None. If wcc HANGS (the optimizer runs away on
    a pathological function -- what left kernel32's heap.obj truncated/empty -> E2146) the run is killed
    and retried with -od (optimizations disabled), which sidesteps the hang; the -od pass gets a longer
    budget since it is the reliable path. The obj is validated (non-empty OMF) before being accepted."""
    o = (cfile[:-2] if cfile.endswith(".c") else cfile) + ".obj"
    op = os.path.join(bld, o)

    def _try(extra, timeout):
        try:
            if os.path.isfile(op):
                os.remove(op)
        except OSError:
            pass
        try:
            subprocess.run([cc] + flags + extra + [cfile], cwd=bld, capture_output=True,
                           text=True, env=env, timeout=timeout)
        except subprocess.TimeoutExpired:
            pass
        return _valid_obj(op)

    if _try([], 150):
        return o
    if _try(["-od"], 400):                                  # optimizer hang -> unoptimized, more time
        return o
    return None


def _clib16_lib():
    """Path to the REAL 16-bit large-model Windows C runtime library. This from-source Watcom's
    rel/lib286/clibl.lib is an empty 1.5KB stub -- the complete library (5500+ symbols: __U4M/__U4D
    integer mul-div, __HugeValue, __8087/__FDU87 fp, and the full CRT) is the actual per-target build
    under bld/clib/library/windows.086/ml. Reference it directly. Returns its path, or "" if absent."""
    lib = os.path.join(toolchain.openwatcom_src(), "bld", "clib", "library", "windows.086", "ml",
                       "clibl.lib")
    return lib if os.path.isfile(lib) else ""


def _ne_is_library(binpath):
    """True if an NE module has the LIBRARY flag (0x8000) set -- i.e. it is a DLL, not an application.
    The Win16 core (KRNL386/GDI/USER) ships as .EXE yet sets this; real apps (dialer, ...) do not, even
    though they also have entry-table entries. Determines whether to build a DLL or an application."""
    try:
        import struct
        d = open(binpath, "rb").read(0x40)
        ne = struct.unpack("<I", d[0x3c:0x40])[0]
        with open(binpath, "rb") as f:
            f.seek(ne)
            h = f.read(0x10)
        if h[:2] != b"NE":
            return False
        flags = struct.unpack("<H", h[0x0c:0x0e])[0]
        return bool(flags & 0x8000)
    except (OSError, struct.error, IndexError):
        return False


def _entry(binpath, fmt):
    """The entry-point address (PE RVA / NE linear from CS:IP) or '' if unavailable."""
    if fmt not in ("NE", "PE"):
        return ""
    dec = _dec(fmt)
    out = subprocess.run([dec, binpath, "--entry"], capture_output=True, text=True).stdout.strip()
    return out.splitlines()[0].strip() if out else ""


def _name_at(binpath, fmt, addr):
    """The name the emitted C uses for the function at `addr`. --names lists every discovered start as
    `addr<TAB>name`; an unnamed start (e.g. the header entry point, which no call site references) prints
    an empty name, but --module still emits it with the `sub_%06X` fallback. Mirror that fallback so a
    discovered-but-unnamed entry resolves to the symbol the C actually defines (else `option start` is
    never set and CS:IP links as 0:0)."""
    dec = _dec(fmt)
    out = subprocess.run([dec, binpath, "--names"], capture_output=True, text=True).stdout
    for line in out.splitlines():
        p = line.split("\t")
        if not p or not p[0].strip():
            continue
        try:
            a = int(p[0], 16)
        except ValueError:
            continue
        if a == addr:
            nm = p[1].strip() if len(p) >= 2 else ""
            return nm if nm else ("sub_%06X" % addr)
    return ""


def link_module(module, do_diff=False):
    binpath = _find_binary(module)
    if not binpath:
        return {"ok": False, "error": "no source binary for %s" % module}
    fmt = authsrc.classify(binpath)[0]
    bits = 32 if fmt == "PE" else 16
    cpath = os.path.join(authsrc._dr(), "recon", "authored", "c", module + ".c")
    if not os.path.isfile(cpath):
        return {"ok": False, "error": "no authored C for %s (run authsrc first)" % module}

    ow = _ow()
    bld = os.path.join(authsrc._dr(), "recon", "authored", "_link", module)
    os.makedirs(bld, exist_ok=True)
    import shutil
    # Clear stale build inputs first. This dir persists across runs, and an earlier split (or a
    # single-file authoring) leaves pieces behind -- e.g. a 100k-line `setupx.c` from when the module
    # was one file, which now fails E1118 (segment too large) and drops every function defined in it,
    # or duplicate pieces from a prior split shape. Start from a clean tree so only the CURRENT pieces
    # are compiled and linked.
    for f in os.listdir(bld):
        if f.endswith((".c", ".h", ".obj", ".o", ".asm")):
            try:
                os.remove(os.path.join(bld, f))
            except OSError:
                pass
    env = owcheck._env()
    cc = os.path.join(ow, "binnt", "wcc.exe" if bits == 16 else "wcc386.exe")
    tgt = "windows" if bits == 16 else "nt"
    # -s: no stack-overflow probe -> no __STK CRT dependency (self-contained scaffold)
    cflags = ["-bt=" + tgt, "-q", "-s"]
    if bits == 16:
        # -fpc: floating point via library CALLS, not inline 8087. Inline 8087 (-fpi, the default) emits
        # FI*RQQ/FJ*RQQ fp-fixup symbols resolved by a linker fp-fixup mechanism that our custom NE
        # template doesn't wire up (left FIFRQQ/FJFRQQ undefined in any float-using module); -fpc calls
        # the ordinary math-library helpers instead, which link normally.
        cflags += ["-mc", "-3", "-fpi87"]
    # Prefer the multi-file split tree: compile each piece to its own obj (separate segments), which is
    # how the module was really built AND the only way a big 16-bit module fits (one giant .c overruns
    # the 64K code segment). Fall back to the single authored .c for small/un-split modules.
    treedir = os.path.join(authsrc._dr(), "recon", "authored", "tree", module)
    # DATA-segment lane (16-bit NE): reconstruct the module's DGROUP as authored assembly so the code's
    # DGROUP-relative accesses resolve and exported data (THHOOK, __GP, ...) is present. Writes
    # tree/<module>/<module>_data.asm; the tree loop below assembles it. PE data is handled by wcc386.
    data_exports = []
    if fmt == "NE":
        try:
            from src.targets.win95 import dataseg
            dr = dataseg.write_module(module)
            if dr.get("ok"):
                data_exports = dr["data_exports"]
        except Exception:                                          # noqa: BLE001
            pass
    objs = []
    if os.path.isdir(treedir) and any(f.endswith(".c") for f in os.listdir(treedir)):
        for f in sorted(os.listdir(treedir)):
            if f.endswith((".c", ".h")):
                shutil.copy2(os.path.join(treedir, f), os.path.join(bld, f))
        for f in sorted(os.listdir(treedir)):
            if f.endswith(".c"):
                # each piece into its OWN code segment (-nt): 16-bit NE caps a segment at 64K, and
                # wlink merges same-named segments, so without this every piece's code piles into one
                # _TEXT and overruns. Large model (-ml, far code) so a call from one piece's segment to
                # a function in another links as a FAR call -- a near call cannot cross segments (E2052).
                pcf = (["-bt=" + tgt, "-q", "-s", "-ml", "-3", "-fpi87", "-nt=%s_TEXT" % f[:-2]]
                       if bits == 16 else cflags)
                o = _compile_piece(cc, pcf, f, bld, env)
                if o:
                    objs.append(o)
        # authored assembly pieces (the reconstructed DGROUP data module): assemble with wasm. Its
        # obj must link FIRST so its _DATA sits at DGROUP offset 0 -- the code accesses globals by
        # DGROUP-relative offset, so the authored data image has to start at 0 even if wcc also emits
        # incidental _DATA (string literals) that then follows harmlessly above the real data.
        data_objs = []
        for f in sorted(os.listdir(treedir)):
            if f.endswith(".asm"):
                shutil.copy2(os.path.join(treedir, f), os.path.join(bld, f))
                subprocess.run([os.path.join(ow, "binnt", "wasm.exe"), "-q", "-zq", f],
                               cwd=bld, capture_output=True, text=True, env=env, timeout=180)
                o = f[:-4] + ".obj"
                if os.path.isfile(os.path.join(bld, o)):
                    data_objs.append(o)
        objs = data_objs + objs
    else:
        shutil.copy2(cpath, os.path.join(bld, module + ".c"))
        o = _compile_piece(cc, cflags, module + ".c", bld, env)
        if o:
            objs.append(o)
    src = cpath                                            # for the defined-exports scan
    if not objs:
        return {"ok": False, "error": "compile produced no obj"}

    # real imports -> wlink `import` directives (name-consistent with the C's symbols), so the module
    # calls the actual API instead of a return-0 stub. NE: by ordinal or name from KERNEL/USER/GDI/...
    # PE: from the IAT (MODULE.func); the decompiler names the call MODULE_func.
    imps = _imports(binpath, fmt)

    def _q(sym):
        # Quote a wlink symbol ONLY when it contains a character wlink parses specially (a C++ mangled
        # name: '@' is its response-file operator, '?'/'$' are directive punctuation). A plain
        # identifier must stay UNQUOTED -- wlink matches a quoted 'NAME_' as a distinct literal that does
        # NOT resolve the obj's reference to NAME_, so quoting every symbol breaks normal imports.
        return ("'%s'" % sym) if re.search(r"[@?$]", sym) else sym

    imp_lines = []
    for mod, api in imps:
        if fmt == "PE":
            # Ordinal-only import: the decoder names it `@N`. Emitting `'MOD'.'@N'` makes wlink read the
            # leading `@` as its response-file operator and try to open a file "N" (E2093: cannot open N,
            # a hard parse failure that killed the whole link -- most PE modules import something by
            # ordinal). Emit the numeric ordinal form `import MOD_N 'MOD'.N` instead; if the recovered C
            # never references it, the directive is simply unused.
            # The C calls a PE import as MOD_api / MOD_ordinal; wcc386's default (register) convention
            # decorates the reference with a TRAILING '_' (obj symbol `MOD_api_`). The import
            # directive's INTERNAL name must carry that '_' or the reference is undefined and the import
            # gets stubbed instead of linked -- this was stubbing most PE imports corpus-wide.
            mo = re.match(r"^@(\d+)$", api)
            if mo:
                imp_lines.append("import %s '%s'.%s" % (_q("%s_%s_" % (mod, mo.group(1))), mod, mo.group(1)))
            else:
                # A C++ mangled import (??2@YAPAXI@Z ...) carries '@'/'?', which wlink otherwise reads as
                # its response-file operator (E2093) -- _q() quotes only those so the link doesn't die,
                # while plain imports stay unquoted so they still resolve the obj's reference.
                imp_lines.append("import %s '%s'.'%s'" % (_q("%s_%s_" % (mod, api)), mod, api))
            continue
        # NE 16-bit: the authored C declares each imported API as a plain `extern int API()`, which
        # wcc (compact-model __watcall) references with a TRAILING underscore -- the obj carries the
        # symbol `API_`, not `API`. The wlink `import` directive's internal name must therefore be the
        # decorated `API_` (mapped onto the real export `API`); an undecorated internal name never
        # matched the reference, so every Win16 API call fell through to a return-0 stub instead of
        # calling the real KERNEL/USER/GDI export.
        mo = re.match(r"^%s_(\d+)$" % re.escape(mod), api)
        if mo:
            imp_lines.append("import %s '%s'.%s" % (_q(api + "_"), mod, mo.group(1)))
        else:
            imp_lines.append("import %s '%s'.'%s'" % (_q(api + "_"), mod, api))

    # C-DERIVED IMPORT SAFETY NET (PE): a few modules' IAT parse under-reads (jetadmin.cpl: --imports
    # sees 14 but the code calls 166 APIs), so those calls would stub. The authored C already spells
    # every import as MODULE_ApiName -- derive a directive for any MODULE_ApiName the C references that
    # we didn't already import. `MODULE` must be a known Win32 DLL so an internal name never masquerades
    # as an import; the func is everything after the first '_' (ImageList_Create keeps its own '_').
    if fmt == "PE":
        _PE_IMPORT_MODS = ("KERNEL32", "USER32", "GDI32", "ADVAPI32", "COMCTL32", "COMDLG32", "SHELL32",
                           "OLE32", "OLEAUT32", "WINMM", "VERSION", "WINSPOOL", "RPCRT4", "MPR",
                           "NETAPI32", "LZ32", "SHLWAPI", "WSOCK32", "CRTDLL", "DDRAW", "DPLAY")
        already = set(re.findall(r"import '?(\S+?)_'? '", "\n".join(imp_lines)))
        ctext = open(src, encoding="utf-8", errors="replace").read()
        for mod, fn in set(re.findall(r"\b(%s)_([A-Za-z][A-Za-z0-9_]*)\b" % "|".join(_PE_IMPORT_MODS),
                                      ctext)):
            internal = "%s_%s" % (mod, fn)
            if internal not in already:
                imp_lines.append("import %s '%s'.'%s'" % (_q(internal + "_"), mod, fn))
                already.add(internal)

    # real exports -> wlink `export` directives, so the module exports its functions AND wlink keeps
    # the exported code + everything it calls (otherwise it dead-code-eliminates to the entry stub).
    # Only export names actually DEFINED here (an undefined export aborts the link).
    exports_list = _exports(binpath, fmt)
    # DLL vs application is the NE LIBRARY flag (0x8000), NOT the presence of exports. The Win16 core
    # (KRNL386/GDI/USER) ships as .EXE yet sets the library flag -> a DLL. A real application (dialer,
    # winfile, ...) also has entry-table entries but is NOT a library: building it as `format windows
    # dll` pulls the app startup into a DLL build and collides (undefined WinMain-side symbols, stub-vs-
    # import W1171). Classify by the flag; emit export directives whenever there IS an export surface.
    is_dll = binpath.lower().endswith((".dll", ".cpl", ".drv", ".ocx")) \
        or (fmt == "NE" and _ne_is_library(binpath))
    # emit export directives whenever there IS an export surface: every NE module (apps export via the
    # entry table too), and PE DLLs. A PE .exe app is not exported.
    emit_exports = len(exports_list) > 0 and (fmt == "NE" or is_dll)
    # A function DEFINITION's opening line: `<type...> NAME(<args>)` with no trailing `;`. The arg span
    # must be `[^;\n]*` NOT `[^;]*` -- a negated class matches newlines too, so `[^;]*` lets one match
    # swallow following lines up to the next `;`, and findall (non-overlapping) then never emits the
    # definitions inside that swallowed span. That silently dropped real exports corpus-wide (krnl386
    # FREEMODULE/LOADRESOURCE/ADDATOM/OPENPATHNAME/ALLOCCSTODSALIAS among them).
    defined = set(re.findall(r"(?m)^[A-Za-z].*?\b([A-Za-z_]\w*)\s*\([^;\n]*\)\s*$",
                             open(src, encoding="utf-8", errors="replace").read()))
    # Address -> OUR recovered function name, from the authored C's `// <name>  @ 0x<addr>` annotations.
    # Lets us resolve an export whose retail NAME we didn't propagate onto the recovered function (the
    # function IS recovered, just named sub_XXXXX): we then export it under the real retail name at the
    # real ordinal, aliased to our symbol -- instead of dropping the export.
    addr2name = {}
    for m in re.finditer(r"//\s*([A-Za-z_]\w*)\s+@ 0x([0-9A-Fa-f]+)",
                         open(src, encoding="utf-8", errors="replace").read()):
        addr2name[int(m.group(2), 16)] = m.group(1)
    data_export_names = {nm for _o, nm, _off in data_exports}   # data exports the DGROUP lane rebuilt
    exp_lines = []
    missing_exports = []                                   # (ordinal, name, reason) -- honest residue
    if emit_exports:
        for ordn, name, addr, is_data in exports_list:
            # Symbol decoration must match how the decompiler DECLARED the function. The recovered
            # bodies are `int __fastcall NAME(...)`, and Watcom's 16-bit __fastcall decoration is a
            # LEADING '@' (symbol `@NAME`); wcc386's register calling convention is a TRAILING '_'.
            # (Imported Win16 APIs are the other case -- declared plain `extern`, decorated `NAME_` --
            # handled in the import block above.) Quote the NE symbol: an unquoted leading '@' is
            # wlink's read-directives-from-file syntax, not a symbol reference.
            # The exported NAME (LHS) is quoted too: a C++ module exports MANGLED names (??0Foo@@...),
            # whose '@' wlink would otherwise read as its response-file operator (E2093).
            if name in defined:                            # retail name matches a recovered function
                if fmt == "PE":
                    exp_lines.append("export %s='%s_'" % (_q(name), name))
                else:
                    exp_lines.append("export %s.%s='@%s'" % (_q(name), ordn, name))
                continue
            # Name didn't match; resolve by ADDRESS. Our recovered function at the export's address may
            # carry a different symbol (sub_XXXXX) -- export the real retail name/ordinal aliased to it.
            our = addr2name.get(addr) if addr is not None else None
            if fmt == "NE" and our and not is_data and our in defined:
                exp_lines.append("export %s.%s='@%s'" % (_q(name), ordn, our))
            elif fmt == "PE" and our and our in defined:
                # PE export whose retail name we didn't propagate onto the recovered function: export
                # the real name aliased to our recovered symbol at that RVA (trailing '_' decoration).
                exp_lines.append("export %s='%s_'" % (_q(name), our))
            elif is_data and name in data_export_names:
                # exported data reconstructed by the DGROUP data lane (dataseg): export the data symbol
                # (an assembly PUBLIC label, undecorated) at its real ordinal.
                exp_lines.append("export %s.%s=%s" % (_q(name), ordn, name))
            elif is_data:
                missing_exports.append((ordn, name, "data-export (not reconstructed)"))
            elif addr is not None and our is None:
                missing_exports.append((ordn, name, "no function recovered @ 0x%X" % addr))
            else:
                missing_exports.append((ordn, name, "unresolved"))
    # Entry point (CS:IP in the NE header / AddressOfEntryPoint in PE). For an application it pulls the
    # call tree; for a LIBRARY it is the DLL init (LibMain) the loader calls at load time. A Win16 core
    # library (KRNL386/GDI/USER) with CS:IP=0:0 is UNLOADABLE -- WIN.COM can't call its LibMain, so
    # loading it as the enhanced kernel fails and Windows falls back to standard mode. Map retail's
    # entry addr to our recovered function and set it for BOTH applications and libraries.
    entry_sym = ""
    if fmt in ("NE", "PE"):
        ev = _entry(binpath, fmt)
        if ev and ev != "0":
            eaddr = int(ev, 16)
            # The authored C annotates every function `// <name>  @ 0x<8-hex-addr>`; authsrc's naming
            # engine RENAMES the raw sub_XXXXX (e.g. the entry becomes helper_01B453), so resolve the
            # entry symbol from the C's own annotation at the entry addr -- that is exactly the symbol
            # the C defines. Fall back to the decoder's raw name only if the annotation isn't present.
            ctext = open(src, encoding="utf-8", errors="replace").read()
            m = re.search(r"//\s*([A-Za-z_]\w*)\s+@ 0x0*%X\b" % eaddr, ctext)
            nm = m.group(1) if m else _name_at(binpath, fmt, eaddr)
            if nm and nm in defined:
                entry_sym = ("'@%s'" % nm) if fmt == "NE" else (nm + "_")   # NE __fastcall @ / PE _

    # iterative link: whatever is still undefined (internal sub_ the decompiler didn't emit, CRT
    # helpers) gets a return-0 stub -- flagged as the completeness gap, NOT the real imports.
    # A 16-bit NE library MUST carry the NE library flag (0x8000) or the loader treats it as an
    # application and won't load it as a DLL/kernel -- our krnl386 was 0x0302 (application, MULTIPLEDATA)
    # vs retail 0x8301 (library|SINGLEDATA), which made it unloadable as the enhanced kernel. The
    # library bit comes from wlink's `option dll` (added below for NE libs); NOT from `system
    # windows_dll`, whose DLL template pulls in libentry.obj -> an undefined LIBMAIN our recovered code
    # can't satisfy in Watcom's decoration. Keep the plain `system windows` startup and set the bit
    # directly.
    system = "windows" if bits == 16 else "nt"
    dll_opt = (bits == 16 and is_dll)
    out = module + (".dll" if binpath.lower().endswith(".dll") else ".exe")
    stubs = set()
    for _it in range(20):
        with open(os.path.join(bld, "stubs.c"), "w", newline="\n") as f:
            f.write("/* internal targets the decompiler has not recovered -- flagged stubs */\n")
            for s in sorted(stubs):
                f.write("int %s(){return 0;}\n" % s)
        stubflags = (["-bt=" + tgt, "-q", "-s", "-ml", "-3", "-fpi87"] if bits == 16 else cflags)
        subprocess.run([cc] + stubflags + ["stubs.c"], cwd=bld, capture_output=True, text=True, env=env)
        with open(os.path.join(bld, "link.lnk"), "w", newline="\n") as f:
            if dll_opt:
                # NE library: `format windows dll` sets the 0x8000 library bit without the stock
                # `windows_dll` system's `libfile libentry.obj` (-> undefined LIBMAIN). `library windows`
                # gives the CRT startup its INITTASK/WAITEVENT/INITAPP. Keep the bare format (not a custom
                # `system`, which would override wlink's default runtime libraries and lose __U4M &c.).
                lp = os.path.join(ow, "lib286").replace("\\", "/")
                f.write("format windows dll\nname %s\noption quiet\noption map=%s.map\n" % (out, module))
                f.write("libpath '%s'\nlibpath '%s/win'\nlibrary windows\noption nocaseexact\n"
                        "option stack=8k, heapsize=1k\n" % (lp, lp))
            else:
                f.write("system %s\nname %s\noption quiet\noption map=%s.map\n" % (system, out, module))
            if entry_sym:
                f.write("option start=%s\n" % entry_sym)
            if bits == 16:
                # The compiler emits runtime-helper calls -- __U4M/__U4D/__I4M/__I4D (32-bit integer
                # mul/div), __HugeValue, __8087/__FDU87 (fp), and the rest of the CRT. This from-source
                # Watcom's rel/lib286/clibl.lib is an empty STUB; the COMPLETE 16-bit large-model Windows
                # runtime is the actual build under bld/clib/library/windows.086/ml. Reference it by FULL
                # PATH so it wins over the empty rel stub (which would win libpath order otherwise).
                cglib = (_clib16_lib() or os.path.join(ow, "lib286", "clibl.lib")).replace("\\", "/")
                f.write("libpath '%s'\nlibrary '%s'\n"
                        % (os.path.join(ow, "lib286").replace("\\", "/"), cglib))
            else:
                # 32-bit: the wcc386 runtime (clib3r) + FP pack (math3r) supply the compiler helpers our
                # PE code references -- __8087 and the FP/util routines float ops pull in. Real toolchain
                # code, linked not stubbed.
                f.write("libpath '%s'\nlibpath '%s'\nlibrary clib3r,math3r\n" %
                        (os.path.join(ow, "lib386", "nt").replace("\\", "/"),
                         os.path.join(ow, "lib386").replace("\\", "/")))
            for o in objs:
                f.write("file %s\n" % o)
            f.write("file stubs.obj\n")
            for l in imp_lines:
                f.write(l + "\n")
            for l in exp_lines:
                f.write(l + "\n")
        lr = subprocess.run([os.path.join(ow, "binnt", "wlink.exe"), "@link.lnk"], cwd=bld,
                            capture_output=True, text=True, env=env)
        undef = set(re.findall(r"E2028: (\w+) is an undefined reference", lr.stdout + lr.stderr))
        undef = {u[:-1] if u.endswith("_") else u for u in undef}     # drop wcc's calling-conv '_'
        new = undef - stubs
        if not new:
            break
        stubs |= new
    outpath = os.path.join(bld, out)
    res = {"ok": os.path.isfile(outpath), "output": outpath if os.path.isfile(outpath) else None,
           "fmt": fmt, "imports": len(imp_lines), "exports": len(exp_lines), "stubs": len(stubs),
           "size": os.path.getsize(outpath) if os.path.isfile(outpath) else 0,
           "retail_size": os.path.getsize(binpath),
           "missing_exports": missing_exports if emit_exports else []}
    if not res["ok"]:
        res["log"] = (lr.stdout + lr.stderr)[-800:]
    return res


def _link_job(args):
    """Picklable worker: link one module, never raise (a crash in one must not kill the sweep)."""
    module, do_diff = args
    try:
        return module, link_module(module, do_diff)
    except Exception as e:                                          # noqa: BLE001
        return module, {"ok": False, "error": "exception: %s" % e}


def _all_modules():
    """Every module we have authored C for (the compile+link candidates)."""
    cdir = os.path.join(authsrc._dr(), "recon", "authored", "c")
    return [f[:-2] for f in sorted(os.listdir(cdir)) if f.endswith(".c")]


def main(argv):
    do_diff = "--diff" in argv
    if "--all" in argv:
        import csv, time
        from concurrent.futures import ProcessPoolExecutor, as_completed
        jobs_n = int(argv[argv.index("-j") + 1]) if "-j" in argv else min((os.cpu_count() or 4), 12)
        mods = _all_modules()
        t0 = time.time()
        rows, okn, iden, total_stubs = [], 0, 0, 0
        outcsv = os.path.join(authsrc._dr(), "recon", "authored", "BUILDLINK.csv")
        print("== owlink --all: compiling + linking %d modules with OpenWatcom (%d workers) ==" % (len(mods), jobs_n))
        with ProcessPoolExecutor(max_workers=jobs_n) as ex:
            futs = [ex.submit(_link_job, (m, False)) for m in mods]
            for k, fut in enumerate(as_completed(futs)):
                m, r = fut.result()
                if r.get("ok"):
                    okn += 1; total_stubs += r.get("stubs", 0)
                    pct = 100.0 * r["size"] / max(r.get("retail_size", 1), 1)
                    rows.append((m, r["fmt"], "OK", r["imports"], r.get("exports", 0), r["stubs"],
                                 r["size"], r.get("retail_size", 0), "%.0f" % pct))
                else:
                    rows.append((m, r.get("fmt", "?"), "FAIL", 0, 0, 0, 0, 0,
                                 (r.get("error") or r.get("log", ""))[:60]))
                if (k + 1) % 50 == 0:
                    print("  ... %d/%d linked (%d ok)" % (k + 1, len(mods), okn))
        with open(outcsv, "w", newline="") as f:
            w = csv.writer(f)
            w.writerow(["module", "fmt", "status", "imports", "exports", "stubs", "size", "retail_size", "pct"])
            for r in sorted(rows):
                w.writerow(r)
        print("\n== corpus link ==")
        print("  linked OK: %d / %d modules  (%.0f%%)" % (okn, len(mods), 100.0 * okn / max(len(mods), 1)))
        print("  residual internal stubs across all: %d (avg %.1f/module)" % (total_stubs, total_stubs / max(okn, 1)))
        print("  %.0fs -> %s" % (time.time() - t0, outcsv))
        return 0
    mods = [a for a in argv if not a.startswith("-")]
    if not mods:
        print(__doc__); return 0
    for m in mods:
        r = link_module(m, do_diff)
        if r.get("ok"):
            print("  LINK-OK %-12s [%s]  %d imports, %d exports, %d stubs -> %s (%d bytes; retail %d, %.0f%%)"
                  % (m, r["fmt"], r["imports"], r.get("exports", 0), r["stubs"], os.path.basename(r["output"]),
                     r["size"], r["retail_size"], 100.0 * r["size"] / max(r["retail_size"], 1)))
        else:
            print("  LINK-NO %-12s  %s" % (m, r.get("error") or r.get("log", "")[-200:]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
