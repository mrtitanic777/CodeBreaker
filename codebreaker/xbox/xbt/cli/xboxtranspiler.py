#!/usr/bin/env python3
"""
XboxTranspiler — a unified reverse-engineering workbench for Xbox 360 (and PC).

Forked from WinTranspiler (the Windows 1.0/3.1/95 teardown toolchain) and
extended to the Xbox 360: it reads XEX executables, decodes the embedded PE,
and disassembles / byte-exact-reconstructs the big-endian **PowerPC** (Xenon)
code the same way the PC path handles x86. The original NE/PE/LE (Windows)
loaders are retained — this is a superset, dispatched per file by format.

One entry point over every tool in this project. Runs as `python
xboxtranspiler.py <command> [args]` today and packages to a standalone
`XboxTranspiler.exe` (see build_exe.py). New tools drop into `plugins/` and
register automatically; a rebuild bakes them into the EXE — so the toolset
grows without ever touching this file.

What is IN the unit vs DRIVEN by it (honest, on purpose):
  * IN      — every tool we wrote (the XEX loader, the PowerPC disassembler and
              byte-exact reconstructor, plus the inherited PC parsers/carve/
              ledger/verify). Bundled into the EXE. capstone (PPC) rides along.
  * DRIVEN  — big external binaries found by path (see `config`): Ghidra (Java,
              ideally with the Xbox-360 loader extensions), and an optional
              PowerPC assembler (llvm-mc / GNU as) for mnemonic verification.

    XboxTranspiler                 list every command
    XboxTranspiler xex default.xex XEX header + embedded-PE summary
    XboxTranspiler ppcdisasm default.xex     disassemble the PowerPC .text
    XboxTranspiler ppcrecon default.xex -o x.S   byte-exact PPC reconstruction
    XboxTranspiler <cmd> [args]    run a tool (args pass straight through)
    XboxTranspiler doctor          check every dependency is wired up
    XboxTranspiler config          show / set external paths

Heavy passes fan out across CPU cores: pass `-j N` (or set $WIN95RE_JOBS) to
any tool that supports it. See parallel.py for why the workload is
CPU/subprocess-parallel, not GPU.
"""
import json
import os
import runpy
import sys

APP_DIR = os.path.dirname(os.path.abspath(__file__))
FROZEN = getattr(sys, "frozen", False)
# Three distinct roots, kept straight on purpose:
#   BUNDLE   - where bundled tools/data live (temp unpack dir when frozen).
#   EXE_DIR  - where the .exe itself sits; config persists here so it survives
#              the onefile unpack and travels with the binary.
#   PROJECT  - the user's working dir; the tools resolve files/, reference/,
#              reconstructed/ relative to cwd, so doctor must check the same.
BUNDLE = getattr(sys, "_MEIPASS", APP_DIR)

# --- package layout: xbt/<domain>/*.py --------------------------------------
# APP_DIR is xbt/cli/; the package root xbt/ is its parent. Inject every domain
# subpackage dir onto sys.path so the tools' flat sibling imports keep resolving
# unchanged, and index every module by basename for command dispatch.
PKG_ROOT = getattr(sys, "_MEIPASS", os.path.dirname(APP_DIR))
_SUBDIRS = ("core", "xbox", "ppc", "win", "match", "assets", "crypto", "cli", "gui")
for _s in _SUBDIRS:
    _d = os.path.join(PKG_ROOT, _s)
    if os.path.isdir(_d) and _d not in sys.path:
        sys.path.insert(0, _d)
if PKG_ROOT not in sys.path:
    sys.path.insert(0, PKG_ROOT)
MODULE_INDEX = {}
for _base in (PKG_ROOT,) + tuple(os.path.join(PKG_ROOT, _s) for _s in _SUBDIRS):
    if os.path.isdir(_base):
        for _f in os.listdir(_base):
            if _f.endswith(".py"):
                MODULE_INDEX.setdefault(_f, os.path.join(_base, _f))
EXE_DIR = os.path.dirname(sys.executable) if FROZEN else APP_DIR
CONFIG_PATH = os.path.join(EXE_DIR, "XboxTranspiler.config.json")


def find_project_root():
    """Locate the project root (the dir holding the `.xboxtranspiler-root` sentinel) by
    walking up from the EXE/script, then cwd. Lets the tools resolve files/,
    reference/, reports/, ORDINALS.md correctly no matter where the EXE is
    double-clicked from (it lives under tools/dist/)."""
    for base in (EXE_DIR, APP_DIR, os.getcwd()):
        d = base
        for _ in range(6):
            if os.path.exists(os.path.join(d, ".xboxtranspiler-root")):
                return d
            up = os.path.dirname(d)
            if up == d:
                break
            d = up
    return os.getcwd()


PROJECT = find_project_root()

# command -> (script basename, one-line help). Each script is runnable as
# __main__ with its own argv (that IS the unification — no tool was forked).
BUILTINS = [
    # --- Xbox 360 (PowerPC / XEX) path ---
    ("gui",       "gui.py",       "full graphical workbench over every command"),
    ("xex",       "xex.py",       "read an XEX: header, decode, embedded-PE sections"),
    ("stfs",      "stfs.py",      "Xbox 360 STFS packages (CON/LIVE/PIRS): info/list/extract/pack/resign"),
    ("xdbf",      "xdbf.py",      "Xbox 360 XDBF/GPD profiles & achievements: read + edit (unlock)"),
    ("account",   "account.py",   "decrypt an Xbox 360 profile Account file (gamertag, XUID)"),
    ("god",       "god.py",       "Xbox 360 GoD/SVOD: read a package + convert NXE-installed game to GoD"),
    ("iso2god",   "god.py",       "convert an Xbox 360 ISO to Games-on-Demand (GoD/SVOD)"),
    ("god2iso",   "god.py",       "convert a GoD/SVOD package back to a raw ISO"),
    ("xex2god",   "god.py",       "convert a standalone XEX to Games-on-Demand (GoD)"),
    ("gdf",       "gdf.py",       "read Xbox 360 disc images / SVOD (XDVDFS): info/list/extract"),
    ("stealth",   "stealth.py",   "verify Xbox 360 disc stealth (SS/DMI/PFI): media-ID match, XGD checks"),
    ("selftest",  "selftest.py",  "run built-in regression checks (resign crypto, round-trips)"),
    ("fuzz",      "fuzz.py",      "fuzz the XEX/STFS/FATX/CISO parsers with mutated inputs (hardening)"),
    ("pipeline",  "pipeline.py",  "run a pipeline script: chain XboxTranspiler commands with variables"),
    ("server",    "server.py",    "headless REST server: drive every command over HTTP (automation / web console)"),
    ("sysfiles",  "sysfiles.py",  "Xbox 360 system files: triage/blob/fonts/xref/genuine"),
    ("texture",   "texture.py",   "Xbox 360 texture untile -> DDS (DXT1/3/5)"),
    ("xpr",       "xpr.py",       "extract textures from Xbox .xpr bundles (XPR0 original / XPR2 360) -> DDS"),
    ("xwb",       "xwb.py",       "extract Xbox 360 XACT wave banks (.xwb): PCM->wav, XMA/ADPCM raw"),
    ("xma",       "xma.py",       "Xbox 360 XMA/XMA2 audio -> playable WAV (RIFF-XMA2 wrap + ffmpeg decode)"),
    ("bink",      "bink.py",      "Xbox 360 Bink video (.bik): native header info + ffmpeg decode/extract"),
    ("xur",       "xur.py",       "read Xbox 360 XUR (compiled XUI) scenes: header/sections + strings"),
    ("shader",    "shader.py",    "disassemble Xbox 360 Xenos GPU shader microcode (control flow + ALU/fetch)"),
    ("pm4",       "pm4.py",       "decode Xbox 360 Xenos GPU PM4 command buffers (register writes/draws/loads)"),
    # --- Xbox 360 hardware / filesystem / console tools ---
    ("fatx",      "fatx.py",      "Xbox 360 / Xbox FATX filesystem: info/list/extract, recover deleted, carve"),
    ("xbe",       "xbe.py",       "read an Original Xbox XBE executable: header/certificate/sections"),
    ("eeprom",    "eeprom.py",    "parse an Original Xbox EEPROM: serial/MAC/video + HDD key & region"),
    ("savegame",  "savegame.py",  "edit Xbox 360 STFS game saves: info/extract/patch/repack (+resign)"),
    ("xgd",       "xgd_iso.py",   "read an Xbox 360 disc image (XGD ISO): list/extract"),
    ("ciso",      "ciso.py",      "Xbox CSO/CCI compressed disc images: info/list/extract + to/from ISO"),
    ("datcheck",  "datcheck.py",  "verify a dump against a Redump/No-Intro DAT (CRC32/MD5/SHA1 match)"),
    ("zar",       "zar.py",       "ZArchive (.zar) container: list/extract/hash/verify + pack a directory"),
    ("chd",       "chd.py",       "CHD (MAME) Xbox disc images: header info + chdman-driven chd<->iso/verify"),
    ("bindiff",   "bindiff.py",   "binary diff/patch between two builds (XEX images or any file): make/apply"),
    ("view",      "view.py",      "in-app viewers: hex dump, byte diff, and PowerPC disassembly of any file/region"),
    ("identify",  "identify.py",  "auto-detect an Xbox/Xbox 360 file's format (magic + structure) and name its tool"),
    ("report",    "report.py",    "one-page markdown analysis report (format, build, sections, imports, strings)"),
    ("tu",        "tu.py",        "Xbox 360 Title-Update / DLC manager: inspect, apply a TU to a base title, read DLC"),
    ("xexpatch",  "xex_patch.py", "patch a XEX2: clear the PAL-50 region flag (info/dry-run)"),
    ("nand",      "nand.py",      "inspect an Xbox 360 NAND dump (type/header/HV extract)"),
    ("nandtool",  "nand_tool.py", "Xbox 360 NAND image build: ecc/unecc/verify/fix (spare + 26-bit ECC)"),
    ("flashplan", "flashplan.py", "RGH/JTAG flash planner: image-type + secure-boot-block advisor"),
    ("bootchain", "bootchain.py", "parse the Xbox 360 boot chain (CB/CD/CE/CF/CG) from NAND"),
    ("fuses",     "fuses.py",     "parse an eFuse dump (fuselines, CPU key, retail/devkit)"),
    ("hypervisor","hypervisor.py","analyze a hypervisor image (info; clean patches vs base)"),
    ("smc",       "smc.py",       "Xbox 360 SMC: decrypt/encrypt/info/extract/config (fans/temps/power)"),
    ("cpukey",    "cpukey_ecd.py","validate or generate an Xbox 360 CPU key (ECD + Hamming)"),
    ("xbdm",      "xbdm.py",      "talk to a dev console over XBDM (info/modules/drives/raw/break/watch)"),
    ("jrpc2",     "jrpc2.py",     "JRPC2 dev console: cpukey/temps + RTE peek/poke/freeze/call"),
    # --- Xbox 360 offline analysis (NexGen) ---
    ("keyvault",  "keyvault.py",  "parse a keyvault (KV_dec.bin): console id, cert, fields"),
    ("cryptotest","crypto_oracle.py","run XeCrypt self-tests against known vectors"),
    ("xstrings",  "strings.py",   "scan a binary for strings (categorized, ASCII+UTF-16)"),
    ("heap",      "heap.py",      "analyze a XAM heap dump (header, pools, fragmentation)"),
    ("pagetable", "pagetable.py", "parse a page-table dump (PTEs, protected regions)"),
    ("etx",       "etx.py",       "decode an ETX trace ring buffer (header/events/strings)"),
    ("analyze360","analysis.py",  "scan PowerPC: function prologues + crypto constants"),
    ("exports360","exports.py",   "parse a PE export directory (.edata) -> ordinal map"),
    ("brutalbranch","brutal_branch.py","find encrypted-memory branch-patch candidates in a kernel"),
    ("ppcdisasm", "disasm_ppc.py","disassemble Xbox 360 PowerPC (.text), big-endian"),
    ("vmx128",    "vmx128.py",    "decode Xbox 360 VMX128 vector instructions (128-register AltiVec)"),
    ("c2model",   "c2model.py",   "predict c2 register allocation + store order (matching decompilation)"),
    ("demangle",  "msvc_demangle.py","demangle MSVC C++ decorated names (?foo@bar@@... -> readable signature)"),
    ("ppctrace",  "ppctrace.py",  "single-step PowerPC interpreter/tracer (integer core, dynamic analysis)"),
    ("xexrun",    "xexrun.py",    "run a XEX from its entry on the PPC interpreter; HLE-stub + log kernel/XAM imports"),
    ("xexdiff",   "xexdiff.py",   "diff two XEX builds at the function level, address-invariant (what logic changed)"),
    ("ppcasm",    "ppcasm.py",    "assemble Xbox 360 PowerPC (mnemonic -> bytes) for patching"),
    ("xexinject", "xexinject.py", "patch/hook XEX code at a virtual address, re-emit resigned"),
    ("xeniapatch","xeniapatch.py","apply Xenia .patch.toml game patches to a XEX (static) or live console"),
    ("xinput",    "xinput.py",    "Xbox 360 controller automation over XBDM: press/turbo/macro via a virtual pad"),
    ("memscan",   "memscan.py",   "Xbox 360 memory scanner + pointer-chain/code-cave finder (trainers over XBDM)"),
    ("ppcrecon",  "asmrecon_ppc.py","byte-exact PowerPC reconstruction (.long-backed)"),
    ("ppccc",     "ppc_cc.py",    "compile C with the XDK PowerPC compiler (matching-decomp)"),
    ("ppcrecomp", "ppcrecomp.py", "static-recompile PowerPC -> runnable C++ (RexGlue-style)"),
    ("ppcdiscover","ppcdiscover.py","discover function boundaries (.pdata + bl-descent)"),
    ("callgraph",  "callgraph.py", "build a PowerPC call graph / xref index (callers, callees, DOT)"),
    ("pdata",     "pdata.py",     "parse the Xbox 360 .pdata exception-unwind table (extents + prolog/EH flags)"),
    ("ppcdecomp", "ppcdecomp.py", "decompile PowerPC functions to readable pseudo-C (Ghidra-independent)"),
    ("ppcobj",    "ppc_obj.py",   "read/write PowerPC-BE COFF objects (XDK format): dump or emit a linkable .obj"),
    ("ppcsym",    "ppcsym.py",    "evidence-graded symbol map (imports+entry+RTTI+pdata) -> json/map/Ghidra script"),
    ("ppcrun",     "ppcrun.py",    "end-to-end: recompile PowerPC -> C++ -> run, verify vs native"),
    ("project",    "ppcproject.py","scaffold a complete buildable PC project from a .xex"),
    ("ppcboot",    "ppcboot.py",   "runtime scaffold: guest memory + image load + host-shim/setjmp"),
    # --- inherited PC path (Windows NE/PE/LE) ---
    ("iso",       "iso9660.py",   "read/extract an ISO9660/Joliet image"),
    ("extract",   "cab.py",       "extract the Win95 spanning Quantum cabinet set"),
    ("target",    "target.py",    "module inventory + tri-format tier map"),
    ("ordinals",  "genordinals.py", "generate ORDINALS.md from the export tables"),
    ("names",     "names.py",     "per-module naming oracle (exports + Midage)"),
    ("midage",    "midage.py",    "read the Midageproject .SYM symbol data"),
    ("vmmsvc",    "vmmsvc.py",    "extract VxD/VMM service tables from the DDK"),
    ("refscan",   "refscan.py",   "score an external source tree's Win95 relevance"),
    ("refpull",   "refpull.py",   "pull the relevant slice of a source tree into reference/"),
    ("ledger",    "ledger.py",    "coverage ledger (cited bytes, per tier)"),
    ("verify",    "verify.py",    "GHOST-check reconstructions against the binary"),
    ("lift",      "lift.py",      "deterministic NE->C transliteration"),
    ("petrace",   "pe_trace.py",  "PE (32-bit) function carve + call resolution"),
    ("ghidra",    "ghidra.py",    "run Ghidra's decompiler on a module (hypothesis)"),
    ("ghidraxex", "ghidraxex.py", "headless XEX -> Ghidra (XEXLoaderWV): functions/imports dumps"),
    ("cc16",      "cc16.py",      "compile 16-bit C with MSVC 1.52c (byte-exact path)"),
    # note: the ledger/coverage dashboard now lives in ../ledger/ (run its .bat)
    ("objextract","objextract.py","pull code bytes + reloc mask from an OMF .OBJ"),
    ("oracle",    "oracle.py",    "codegen oracle / flag-pinner (byte-exact stage F)"),
    ("segdiff",   "segdiff.py",   "whole-segment byte-match harness (mask-aware verdict)"),
    ("permute",   "permute.py",   "matching-decompilation search: idioms -> byte-exact"),
    ("masmlift",  "masmlift.py",  "assemble hand-asm with period MASM (byte-exact path)"),
    ("asmrecon",  "asmrecon.py",  "disasm->MASM byte-exact reconstruction (db-fallback)"),
    ("conduct",   "conduct.py",   "drive a whole module to byte-exact, unattended (the conductor)"),
    ("datacode",  "datacode.py",  "classify data vs code in a segment (strings/tables)"),
    ("reachability","reachability.py","control-flow reachability + safe data regions"),
    ("disasm16",  "disasm16.py",  "capstone 16-bit recursive-descent code/data (diagnostic)"),
    ("annotate",  "annotate.py",  "label data regions in reconstructed .asm (comment-only)"),
    ("progressui","progressui.py","native comctl32 progress meter (demo: --selftest)"),
    # --- Xbox 360 crypto / keyvault / NAND (from Xbox-360-Crypto, GoobyCorp, BSD-3) ---
    ("crypto-keystore", "keystore.py",     "keyvault / key store inspection"),
    ("crypto-kv",       "kv_tool.py",      "keyvault tool"),
    ("crypto-xdk",      "xdk_extract.py",  "extract XDK content"),
    ("crypto-cpukey",   "cpu_key.py",      "CPU key utilities"),
]

# Commands whose NAME is itself the target module's first-arg subcommand. god.py
# bundles several converters and dispatches on argv[1]; these aliases must have
# their own name injected as that subcommand when run through the unified CLI.
OP_ALIAS_COMMANDS = {"iso2god", "god2iso", "xex2god"}


def load_plugins():
    """[(name, path, desc)] for every plugins/*.py exposing DESCRIPTION.

    A plugin is any file in plugins/ that (a) is runnable as __main__ and (b)
    defines a module-level string DESCRIPTION. Drop one in; it appears here and
    in the EXE on next build. Nothing else to wire.
    """
    out = []
    pdir = os.path.join(PKG_ROOT, "cli", "plugins")
    if not os.path.isdir(pdir):
        pdir = os.path.join(BUNDLE, "plugins")
    if not os.path.isdir(pdir):
        return out
    for fn in sorted(os.listdir(pdir)):
        if not fn.endswith(".py") or fn.startswith("_"):
            continue
        path = os.path.join(pdir, fn)
        desc = ""
        try:
            for line in open(path, encoding="utf-8"):
                if line.strip().startswith("DESCRIPTION"):
                    desc = line.split("=", 1)[1].strip().strip("'\"")
                    break
        except OSError:
            pass
        out.append((fn[:-3], path, desc or "(plugin)"))
    return out


def cmd_map():
    m = {name: (MODULE_INDEX.get(script, os.path.join(BUNDLE, script)), desc) for name, script, desc in BUILTINS}
    for name, path, desc in load_plugins():
        m.setdefault(name, (path, desc))       # builtins win on name clash
    return m


def load_config():
    # Tool paths live in the shared xbt/core/config.py (single source of truth,
    # also read by the toolchain modules). `reference` is PROJECT-relative, so it
    # is resolved here where PROJECT is known.
    import config as _cfg
    c = _cfg.load()
    c.setdefault("reference", os.path.join(PROJECT, "reference"))
    return c


def cmd_list():
    print("XboxTranspiler - Xbox 360 (PowerPC/XEX) + PC reverse-engineering workbench\n")
    print("BUILTIN COMMANDS:")
    for name, _script, desc in BUILTINS:
        print("  %-11s %s" % (name, desc))
    plugs = load_plugins()
    if plugs:
        print("\nPLUGINS (plugins/*.py):")
        for name, _path, desc in plugs:
            print("  %-11s %s" % (name, desc))
    print("\n  doctor       check that every dependency is wired up")
    print("  config       show / set external tool paths")
    print("  roadmap      print the project roadmap (ROADMAP.md)")
    print("\nrun:  WinTranspiler <command> [args...]")


def cmd_doctor():
    cfg = load_config()
    print("XboxTranspiler doctor - dependency check\n")
    def chk(label, ok, detail=""):
        print("  [%s] %-14s %s" % ("OK" if ok else "--", label, detail))
    # our own bundled bits
    chk("tools", all((s in MODULE_INDEX) or os.path.exists(os.path.join(BUNDLE, s)) for _n, s, _d in BUILTINS),
        "%d builtin commands" % len(BUILTINS))
    chk("files/", os.path.isdir(os.path.join(PROJECT, "files")),
        "drop .xex titles here (or pass a full path)")
    # capstone + its PowerPC backend (the Xbox 360 disassembler)
    try:
        import capstone
        has_ppc = hasattr(capstone, "CS_ARCH_PPC")
        chk("capstone", True, capstone.__version__ +
            (" (x86 + PowerPC)" if has_ppc else " (NO PowerPC backend!)"))
        chk("capstone PPC", has_ppc, "big-endian Xenon disassembly")
    except Exception:
        chk("capstone", False, "pip install capstone (needed for ppcdisasm)")
    # AES for the XEX decrypt front-end (retail images)
    aeslib = ""
    for m in ("Crypto", "cryptography"):
        try:
            __import__(m); aeslib = m; break
        except Exception:
            pass
    chk("AES (XEX decrypt)", True,
        (aeslib + " lib") if aeslib else "pure-Python fallback (slow but works)")
    # XDK PowerPC compiler — the byte-exact matching-decomp backend (permute --arch ppc)
    xdk = cfg.get("xdk") or ""
    cl = os.path.join(xdk, "bin", "win32", "cl.exe")
    chk("XDK PPC cl.exe", os.path.exists(cl),
        (xdk + "  (matching decompilation)") if os.path.exists(cl)
        else "set: XboxTranspiler config xdk \"<Xbox 360 SDK>\"")
    # optional PowerPC assembler for ppcrecon mnemonic verification
    import shutil
    ppc_as = cfg.get("ppc_as") or ""
    as_ok = bool(ppc_as and (shutil.which(ppc_as) or os.path.exists(ppc_as))) \
        or shutil.which("llvm-mc") is not None
    chk("PowerPC as", as_ok,
        (ppc_as or "llvm-mc") if as_ok else "optional: llvm-mc / powerpc GNU as "
        "(ppcrecon is byte-exact without it, just unverified mnemonics)")
    # external heavyweights (driven, not embedded)
    chk("Ghidra", os.path.exists(os.path.join(cfg["ghidra"], "support", "analyzeHeadless.bat")),
        cfg["ghidra"] + "  (add the Xbox-360 loader extension for best decomp)")
    chk("DOSBox-X", os.path.exists(cfg["dosbox_x"]), cfg["dosbox_x"])
    chk("MSVC 1.52c", os.path.exists(os.path.join(cfg["msvc152c"], "BIN", "CL.EXE")),
        cfg["msvc152c"])
    chk("MASM", os.path.exists(os.path.join(cfg["masm"], "MASM.EXE")), cfg["masm"])
    chk("7-Zip", os.path.exists(cfg["sevenzip"]), cfg["sevenzip"])
    import shutil as _sh
    _chdman = (cfg.get("chdman") and (os.path.exists(cfg["chdman"]) or _sh.which(cfg["chdman"]))) or _sh.which("chdman")
    chk("chdman (CHD)", bool(_chdman), (_chdman or "optional: MAME chdman for chd<->iso (config chdman \"<path>\")"))
    _ffm = (cfg.get("ffmpeg") and (os.path.exists(cfg["ffmpeg"]) or _sh.which(cfg["ffmpeg"]))) or _sh.which("ffmpeg")
    chk("ffmpeg (media)", bool(_ffm), (_ffm or "optional: XMA/Bink decode (config ffmpeg \"<path>\")"))
    # Free60 Xenon toolchain (crypto build_lib / shadowboot .bin assembly)
    xb = cfg.get("xenon_bin", "bin")
    chk("Xenon as", os.path.exists(os.path.join(xb, "xenon-as.exe")),
        (xb + "  (xenon-as/objcopy)") if os.path.exists(os.path.join(xb, "xenon-as.exe"))
        else "optional: set XboxTranspiler config xenon_bin \"<free60 bin dir>\"")
    chk("reference/", os.path.isdir(cfg["reference"]),
        "wow16 / DDK / SDK / Midage (hypothesis corpus)")
    import shutil
    chk("java (JDK)", shutil.which("java") is not None, "for Ghidra headless")
    chk("CPU", True, "%d cores (fan-out default; override per tool with -j N)"
        % (os.cpu_count() or 1))


def cmd_config(args):
    cfg = load_config()
    if not args:
        print("# %s" % CONFIG_PATH)
        for k, v in cfg.items():
            print("  %-12s %s" % (k, v))
        print("\nset with:  WinTranspiler config <key> <path>")
        return
    if len(args) >= 2:
        import config as _cfg
        cfg[args[0]] = args[1]
        _cfg.save(cfg)
        print("set %s = %s" % (args[0], args[1]))


def cmd_gui():
    """The double-click / `gui` experience: open the full graphical workbench
    (gui.py) — a themed front-end over every command. Falls back to the passive
    live-progress monitor only if tkinter / the GUI module is unavailable."""
    try:
        if BUNDLE not in sys.path:
            sys.path.insert(0, BUNDLE)
        import gui
        gui.main()
        return
    except Exception as e:
        print("full GUI unavailable (%s); falling back to progress monitor" % e)
    try:
        import progress
        progress.monitor()
    except Exception as e:
        print("progress UI unavailable: %s" % e)


def main():
    import multiprocessing
    multiprocessing.freeze_support()   # so process pools work in the frozen EXE
    # tools emit UTF-8 (box drawing, gamertags, hex tables); the Windows console
    # is cp1252 by default, so force UTF-8 with replacement to avoid crashes.
    for _s in (sys.stdout, sys.stderr):
        try:
            _s.reconfigure(encoding="utf-8", errors="replace")
        except Exception:
            pass
    os.chdir(PROJECT)                  # so files/, reference/, reports/ resolve
    # expose the configured PowerPC assembler to ppcrecon (asmrecon_ppc reads it)
    try:
        _as = load_config().get("ppc_as")
        if _as:
            os.environ["XBOXTRANSPILER_PPC_AS"] = _as
    except Exception:
        pass
    argv = sys.argv[1:]
    # Double-clicking the EXE (frozen, no args) opens the Ready monitor window.
    if FROZEN and not argv:
        cmd_gui(); return
    if argv and argv[0] == "gui":
        cmd_gui(); return
    # global --ui: light up the native progress meter for whatever tool runs.
    ui = "--ui" in argv
    if ui:
        argv = [a for a in argv if a != "--ui"]
        sys.argv = [sys.argv[0]] + argv
        try:
            import progress
            progress.enable()
            progress.begin("XboxTranspiler")   # window appears NOW, before any phase
        except Exception:
            ui = False
    if not argv or argv[0] in ("list", "-h", "--help", "help"):
        cmd_list(); return
    cmd, rest = argv[0], argv[1:]
    if cmd == "doctor":
        cmd_doctor(); return
    if cmd == "config":
        cmd_config(rest); return
    if cmd == "roadmap":
        rp = os.path.join(PROJECT, "ROADMAP.md")
        print(open(rp, encoding="utf-8").read() if os.path.exists(rp)
              else "no ROADMAP.md at project root")
        return
    tools = cmd_map()
    if cmd not in tools:
        print("unknown command: %s\n" % cmd); cmd_list(); sys.exit(2)
    script, _desc = tools[cmd]
    # run the tool exactly as if invoked directly: set its argv and exec its
    # __main__. This is the unification — every tool runs unchanged, in-process.
    # A few commands' NAME is itself the module's first-arg subcommand (god.py
    # dispatches iso2god/god2iso/xex2god on argv[1]); inject it so the alias works
    # through the unified CLI, not just when god.py is run directly.
    inject = [cmd] if cmd in OP_ALIAS_COMMANDS else []
    sys.argv = [os.path.basename(script)] + inject + rest
    sys.path.insert(0, os.path.dirname(script))
    try:
        runpy.run_path(script, run_name="__main__")
    finally:
        # close the meter cleanly for auto-mode tools (managed tools like the
        # conductor already call finish themselves; finish is idempotent).
        if ui:
            try:
                import progress
                # play the Xbox/Win completion sound and show the tool's stashed
                # status report (falls back to "Done" if a tool didn't set one).
                progress.finish(beep=True, linger=0.8)
            except Exception:
                pass


if __name__ == "__main__":
    main()
