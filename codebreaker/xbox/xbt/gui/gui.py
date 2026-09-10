#!/usr/bin/env python3
"""
XboxTranspiler GUI — a full graphical front-end over the whole toolset.

A themed tkinter workbench that drives every XboxTranspiler command: pick an
input, fill a form, hit Run, watch output stream live. No command is hidden —
the command list is pulled live from the CLI (BUILTINS + plugins), so any tool
added to the CLI shows up here automatically. Flagship commands get hand-written
forms in REGISTRY; everything else gets a generic input + raw-arguments form.

Design remodeled from LCE Studio (same ttk 'clam' base, neutral-gray palette
with a green accent, light/dark themes, merged tab-header chrome, and the
worker-thread -> queue -> main-thread-poller async pattern that keeps the UI
responsive while a subprocess runs).

Launch:  XboxTranspiler gui        (or: python tools/gui.py)
"""
DESCRIPTION = "graphical front-end over every XboxTranspiler command"

import os
import queue
import subprocess
import sys
import threading

import tkinter as tk
from tkinter import ttk, filedialog, messagebox

APP_DIR = os.path.dirname(os.path.abspath(__file__))
# entry point lives in the sibling cli/ subpackage (xbt/cli/xboxtranspiler.py);
# fall back to same-dir for the legacy flat layout.
_PKG = os.path.dirname(APP_DIR)
ENTRY = os.path.join(_PKG, "cli", "xboxtranspiler.py")
if not os.path.exists(ENTRY):
    ENTRY = os.path.join(APP_DIR, "xboxtranspiler.py")

# --------------------------------------------------------------------------- #
# Command registry.  Each entry: (cmd, category, blurb, [arg-specs]).
# arg-spec kinds:
#   infile  — input file picker (positional unless 'flag' given)
#   outfile — save-as picker (needs 'flag' or written positionally)
#   flag    — checkbox; emits 'flag' when on
#   opt     — labelled text/int entry; emits [flag, value] when non-empty
#   choice  — combobox; emits [flag, value]
# Any command not listed still appears with a generic {infile + raw args} form.
# --------------------------------------------------------------------------- #
CATEGORIES = ["Xbox 360", "Decompile", "Assets & Diff", "Windows", "Crypto", "Tools"]

REGISTRY = [
    # ---- Xbox 360 pipeline ------------------------------------------------
    ("xex", "Xbox 360",
     "XEX read + full write surface: compress(LZX)/decompress/encrypt/machine/limits/…", [
        ("choice", "Operation", None,
         ["info", "compress", "decompress", "flat", "encrypt", "decrypt",
          "machine", "remove-limits", "bounding", "resources", "xml", "idc", "patch"]),
        ("infile", "XEX file (or base .xex for patch)", None, True),
        ("opt", "Arg2 (machine d|r · remove-limits opts amrz · bounding path · patch file)",
         None, "text"),
        ("outfile", "Output (file / dir for resources / idc)", None, None),
        ("flag", "Encrypt output (-e, for compress)", "-e", None),
        ("flag", "Section map (info)", "--sections", None),
        ("flag", "Imports (info)", "--imports", None),
        ("outfile", "Dump basefile to (info)", "--dump", None),
    ]),
    ("xeniapatch", "Xbox 360",
     "Apply Xenia .patch.toml game patches to a XEX (static) or live console (JRPC2)", [
        ("choice", "Operation", None, ["info", "verify", "apply", "live"]),
        ("infile", "Patch .toml file", None, True),
        ("infile", "Target XEX (verify/apply) or console IP (live)", None, False),
        ("outfile", "Output XEX (apply)", None, None),
        ("opt", "Single patch name (--name)", "--name", "text"),
        ("flag", "Apply all patches incl. disabled (--all)", "--all", None),
    ]),
    ("flashplan", "Xbox 360",
     "RGH/JTAG flash planner: pick xeBuild image type + secure-boot block (advisor)", [
        ("choice", "Operation", None, ["plan", "image-type", "secure-boot", "cpld", "hacks", "motherboards"]),
        ("opt", "Console model (e.g. 'Corona 4G') / class / IDCODE", None, "text"),
        ("opt", "Hack (e.g. 'RGH 3')", None, "text"),
        ("flag", "Corona nandmu (--nandmu)", "--nandmu", None),
    ]),
    ("xbdm", "Xbox 360",
     "Dev-console debug monitor: info/modules/drives + data breakpoints (what accesses X)", [
        ("opt", "Console IP", None, "text"),
        ("choice", "Operation", None, ["info", "modules", "drives", "break", "watch"]),
        ("opt", "Address (break/watch, hex)", None, "text"),
        ("opt", "Size (watch)", None, "int"),
        ("choice", "Access (watch)", None, ["write", "read", "readwrite", "execute"]),
    ]),
    ("memscan", "Xbox 360",
     "Memory scanner + pointer-chain / code-cave finder (trainers, over XBDM)", [
        ("choice", "Operation", None, ["scan", "caves", "pointer"]),
        ("opt", "Console IP", None, "text"),
        ("opt", "Lo addr (hex)", None, "text"),
        ("opt", "Hi addr / value / target (hex)", None, "text"),
        ("opt", "Value type (--type)", "--type", "text"),
    ]),
    ("xinput", "Xbox 360",
     "Controller automation over XBDM: press buttons, turbo/rapid-fire, run macros", [
        ("opt", "Console IP", None, "text"),
        ("choice", "Operation", None, ["press", "turbo", "macro", "connect", "disconnect", "profile"]),
        ("opt", "Argument (buttons e.g. 'A B' / macro-file / xuid)", None, "text"),
        ("opt", "Pad user (--user)", "--user", "int"),
        ("opt", "Hold ms (--hold, press)", "--hold", "int"),
        ("opt", "Rate Hz (--hz, turbo)", "--hz", "int"),
    ]),
    ("fatx", "Xbox 360",
     "FATX filesystem: list/extract, recover deleted files, carve by signature", [
        ("choice", "Operation", None, ["list", "info", "extract", "recover", "carve", "add", "mkdir", "rm"]),
        ("infile", "FATX image / partition", None, True),
        ("dir", "Output folder (extract / recover / carve)", None, None),
        ("flag", "Decrypt encrypted XEX when carving (--decrypt)", "--decrypt", None),
    ]),
    ("sysfiles", "Xbox 360",
     "Dashboard/NAND system files: triage/blob/fonts/xref/genuine", [
        ("choice", "Operation", None, ["triage", "blob", "fonts", "xref", "genuine"]),
        ("infile", "File or folder", None, True),
        ("dir", "Output folder (fonts)", None, None),
    ]),
    ("ppctrace", "Decompile",
     "Single-step PowerPC interpreter/tracer: run a function, watch registers", [
        ("infile", "XEX / raw basefile", None, True),
        ("opt", "Entry VA (--vaddr, hex)", "--vaddr", "text"),
        ("opt", "Max steps (--steps)", "--steps", "int"),
        ("flag", "Per-step register trace (--trace)", "--trace", None),
    ]),
    ("ppcdecomp", "Decompile",
     "Decompile a PowerPC function to readable pseudo-C (Ghidra-independent)", [
        ("infile", "XEX / EXE", None, True),
        ("opt", "Function address (--addr, hex)", "--addr", "text"),
        ("flag", "Whole program (--all)", "--all", None),
        ("opt", "Limit functions (--limit)", "--limit", "int"),
        ("outfile", "Write .c to (for --all)", None, None),
        ("flag", "Self-test", "--selftest", None),
    ]),
    ("ppcobj", "Decompile",
     "Read/write PowerPC-BE COFF objects: dump a .obj, or emit one from assembly", [
        ("choice", "Operation", None, ["dump", "emit"]),
        ("infile", "Object (.obj) or assembly (.s)", None, True),
        ("outfile", "Output .obj (for emit)", None, None),
        ("opt", "Function name (--name, for emit)", "--name", "text"),
        ("flag", "Self-test", "--selftest", None),
    ]),
    ("xexdiff", "Assets & Diff",
     "Diff two XEX builds: functions (address-invariant), strings, or imports", [
        ("infile", "Old XEX", None, True),
        ("infile", "New XEX", None, True),
        ("flag", "Diff strings (--strings)", "--strings", None),
        ("flag", "Diff imports (--imports)", "--imports", None),
        ("opt", "Decompile top N changed (--show)", "--show", "int"),
        ("flag", "Self-test", "--selftest", None),
    ]),
    ("xexrun", "Decompile",
     "Run a XEX from its entry on the PPC interpreter; HLE-stub + log kernel/XAM imports", [
        ("infile", "XEX file", None, True),
        ("opt", "Start VA (--start, hex)", "--start", "text"),
        ("opt", "Max steps (--steps)", "--steps", "int"),
        ("opt", "Import stub return (--stub-ret)", "--stub-ret", "text"),
        ("flag", "Self-test", "--selftest", None),
    ]),
    ("xma", "Xbox 360",
     "XMA/XMA2 audio -> playable WAV (wrap RIFF-XMA2 natively, decode via ffmpeg)", [
        ("choice", "Operation", None, ["bank", "decode", "wrap"]),
        ("infile", "Wave bank (.xwb) or raw .xma", None, True),
        ("outfile", "Output (.wav) — for 'bank', a folder", None, None),
        ("opt", "Channels (--channels)", "--channels", "int"),
        ("opt", "Sample rate (--rate)", "--rate", "int"),
    ]),
    ("bink", "Xbox 360",
     "Bink video (.bik): native header info + ffmpeg decode/extract", [
        ("choice", "Operation", None, ["info", "decode"]),
        ("infile", "Bink file (.bik)", None, True),
        ("outfile", "Output (.mp4 / .wav / ...) [decode]", None, None),
        ("flag", "Audio only (--audio)", "--audio", None),
        ("flag", "Video only (--video)", "--video", None),
    ]),
    ("shader", "Xbox 360",
     "Disassemble Xbox 360 Xenos GPU shader microcode (control flow + ALU/fetch)", [
        ("infile", "Shader microcode (.bin)", None, True),
        ("flag", "Little-endian", "--le", None),
    ]),
    ("xbe", "Xbox 360",
     "Read an Original Xbox XBE executable: header/certificate/sections", [
        ("choice", "Operation", None, ["info", "sections"]),
        ("infile", "XBE file (default.xbe)", None, True),
    ]),
    ("eeprom", "Xbox 360",
     "Parse an Original Xbox EEPROM: serial/MAC/video + decrypt HDD key & region", [
        ("infile", "EEPROM dump (256 bytes)", None, True),
    ]),
    ("tu", "Xbox 360",
     "Title-Update / DLC: inspect a TU, apply it to a base title, or list DLC", [
        ("choice", "Operation", None, ["info", "apply", "dlc"]),
        ("infile", "Update (.xexp / TU pkg) or base .xex (apply)", None, True),
        ("infile", "Update (.xexp / TU pkg) [apply]", None, False),
        ("outfile", "Output .xex (apply)", None, None),
    ]),
    ("bindiff", "Assets & Diff",
     "Binary diff/patch between two builds (XEX images or any file): make/apply", [
        ("choice", "Operation", None, ["make", "apply", "info"]),
        ("infile", "Old / source file", None, True),
        ("infile", "New file (make) / patch (apply)", None, False),
        ("outfile", "Output (patch for make, file for apply)", None, None),
    ]),
    ("smc", "Crypto",
     "Xbox 360 SMC: decrypt/encrypt/info/extract/config (fans, temps, power)", [
        ("choice", "Operation", None, ["info", "decrypt", "encrypt", "extract", "config"]),
        ("infile", "SMC image or NAND dump", None, True),
        ("outfile", "Output (decrypt/encrypt/extract)", None, None),
    ]),
    ("savegame", "Xbox 360",
     "Edit Xbox 360 STFS game saves: info/extract/patch/repack + resign", [
        ("choice", "Operation", None, ["info", "extract", "patch", "repack"]),
        ("infile", "Save package (CON)", None, True),
        ("opt", "inner file / folder (patch/repack/extract)", None, "text"),
        ("opt", "offset (patch)", None, "text"),
        ("opt", "hex value (patch)", None, "text"),
        ("outfile", "Output (patch/repack)", None, None),
        ("infile", "Keyvault to resign with (KV_dec.bin)", None, False),
    ]),
    ("ppcasm", "Xbox 360",
     "Assemble PowerPC (mnemonic -> big-endian bytes) for patching/injection", [
        ("opt", "Instruction (e.g. 'b 0x82001000')", None, "text"),
        ("opt", "Virtual address", None, "text"),
        ("infile", "…or assemble a file (--file)", "--file", False),
    ]),
    ("xexinject", "Xbox 360",
     "Patch or hook XEX code at a virtual address, then re-emit a resigned XEX", [
        ("choice", "Operation", None, ["nop", "bytes", "asm", "hook"]),
        ("infile", "XEX file", None, True),
        ("opt", "Virtual address", None, "text"),
        ("opt", "count / hex / \"asm\" / cave VA", None, "text"),
        ("opt", "hook asm (hook only)", None, "text"),
        ("outfile", "Output XEX", None, None),
    ]),
    ("nandtool", "Xbox 360",
     "NAND image build: verify / strip (unecc) / add (ecc) / fix the 26-bit page ECC", [
        ("choice", "Operation", None, ["info", "verify", "unecc", "ecc", "fix"]),
        ("infile", "NAND image (528-page) or raw (512-page)", None, True),
        ("outfile", "Output image (unecc/ecc/fix)", None, None),
    ]),
    ("texture", "Xbox 360",
     "Untile an Xbox 360 texture and export a standard DDS (DXT1/3/5)", [
        ("choice", "Operation", None, ["untile", "dds"]),
        ("infile", "Tiled texture data", None, True),
        ("opt", "Width", None, "int"),
        ("opt", "Height", None, "int"),
        ("choice", "Format", None, ["dxt1", "dxt5", "dxt3"]),
        ("outfile", "Output .dds", None, None),
        ("flag", "No endian swap", "--no-swap", None),
    ]),
    ("xpr", "Xbox 360",
     "Extract textures from Xbox .xpr resource bundles (XPR0 original / XPR2 360) -> DDS", [
        ("choice", "Operation", None, ["info", "dds"]),
        ("infile", ".xpr bundle", None, True),
        ("outfile", "Output .dds (for 'dds')", None, None),
        ("opt", "Width (XPR2 only)", None, "int"),
        ("opt", "Height (XPR2 only)", None, "int"),
    ]),
    ("xwb", "Xbox 360",
     "Extract an XACT wave bank (.xwb): PCM -> wav, XMA/ADPCM raw + format table", [
        ("choice", "Operation", None, ["info", "extract"]),
        ("infile", "Wave bank (.xwb)", None, True),
        ("dir", "Output folder (extract)", None, None),
    ]),
    ("xur", "Xbox 360",
     "Read/edit a compiled XUI scene (.xur): header/strings + set (repack)", [
        ("choice", "Operation", None, ["info", "strings", "set"]),
        ("infile", "Compiled UI scene (.xur)", None, True),
        ("opt", "String index (set)", None, "text"),
        ("opt", "New text (set)", None, "text"),
        ("outfile", "Output .xur (set)", None, None),
    ]),
    ("stealth", "Xbox 360",
     "Disc stealth verify (abgx360-class): SS/DMI/PFI media-ID match + XGD checks", [
        ("infile", "SS.bin (or a folder with ss/dmi/pfi)", None, True),
        ("infile", "DMI.bin", None, False),
        ("infile", "PFI.bin (optional)", None, False),
        ("infile", "default.xex (optional media-ID cross-check)", "--xex", False),
    ]),
    ("jrpc2", "Xbox 360",
     "JRPC2 dev console: cpukey/temps + real-time editing (peek/poke/freeze/call)", [
        ("opt", "Console IP", None, "text"),
        ("choice", "Operation", None, ["cpukey", "temps", "peek", "poke", "freeze", "call"]),
        ("opt", "Address / module", None, "text"),
        ("opt", "Value / ordinal", None, "text"),
        ("opt", "Type (u8/u16/u32/u64/float/bytes)", None, "text"),
    ]),
    ("ppcdiscover", "Xbox 360", "Discover PowerPC function boundaries (.pdata + bl-descent)", [
        ("infile", "XEX / EXE", None, True),
        ("flag", "List functions", "--list", None),
    ]),
    ("ppcdisasm", "Xbox 360", "Disassemble the PowerPC .text (big-endian Xenon)", [
        ("infile", "XEX / EXE", None, True),
        ("opt", "Limit (functions)", "--limit", "int"),
        ("opt", "Virtual address", "--vaddr", "text"),
    ]),
    ("ppcrecon", "Xbox 360", "Byte-exact PowerPC reconstruction (.long-backed)", [
        ("infile", "XEX / EXE", None, True),
        ("outfile", "Write .S to", "-o", None),
        ("opt", "Virtual address", "--vaddr", "text"),
    ]),
    ("ppccc", "Xbox 360", "Compile C with the XDK PowerPC cl.exe (matching-decomp)", [
        ("infile", "C source", None, True),
        ("opt", "Compiler flags", "--flags", "text"),
    ]),
    ("ppcrecomp", "Xbox 360", "Static-recompile PowerPC -> runnable C++ (RexGlue-style)", [
        ("infile", "XEX / EXE", None, True),
        ("opt", "Virtual address", "--vaddr", "text"),
        ("flag", "Emit C++", "--emit", None),
        ("flag", "Check only", "--check", None),
    ]),
    ("ppcboot", "Xbox 360", "Runtime scaffold: guest memory + image load + host-shim", [
        ("infile", "XEX / EXE", None, True),
    ]),
    ("ppcrun", "Xbox 360", "End-to-end: recompile -> C++ -> run, verify vs native", [
        ("infile", "XEX / EXE", None, False),
        ("flag", "Self-test", "--selftest", None),
        ("opt", "Expression", "--expr", "text"),
    ]),
    ("project", "Xbox 360", "Scaffold a complete buildable PC project from a .xex", [
        ("infile", "XEX file", None, True),
    ]),

    # ---- Xbox 360 content: packages, discs, profiles, GoD ----------------
    ("stfs", "Xbox 360",
     "STFS CON/LIVE/PIRS packages. resign: <pkg> <KV> [out] (converts LIVE/PIRS->CON) "
     "· pack: <folder> <out> <template> · extract: <pkg> [outdir]", [
        ("choice", "Operation", None, ["info", "list", "extract", "resign", "pack"]),
        ("infile", "Package / folder", None, False),
        ("infile", "KV (resign) / out (pack)", None, False),
        ("infile", "out (resign) / template (pack)", None, False),
        ("flag", "Bind console id (resign)", "--bind", None),
    ]),
    ("gdf", "Xbox 360", "Read an Xbox 360 disc image / SVOD (XDVDFS): info/list/extract", [
        ("choice", "Operation", None, ["info", "list", "extract"]),
        ("infile", "Disc image (.iso) / SVOD dir", None, True),
        ("opt", "Path on disc (extract)", None, "text"),
        ("outfile", "Output file (extract)", None, None),
    ]),
    ("datcheck", "Xbox 360",
     "Verify a dump against a Redump/No-Intro DAT (is this a clean, canonical dump?)", [
        ("choice", "Operation", None, ["verify", "name", "info"]),
        ("infile", "File to check (verify/name) or DAT (info)", None, True),
        ("infile", "DAT .xml (verify/name)", None, False),
    ]),
    ("chd", "Xbox 360",
     "CHD (MAME) disc images: native header info, and chd<->iso via chdman", [
        ("choice", "Operation", None, ["info", "toiso", "tochd", "verify"]),
        ("infile", "CHD file (or source .iso for tochd)", None, True),
        ("outfile", "Output (.iso / .chd)", None, None),
    ]),
    ("zar", "Xbox 360",
     "ZArchive (.zar): list/extract/hash/integrity-verify, or pack a directory", [
        ("choice", "Operation", None, ["list", "extract", "hash", "verify", "pack"]),
        ("infile", "Archive (.zar) — for pack, the OUTPUT path", None, True),
        ("dir", "Directory (extract out / pack source)", None, None),
    ]),
    ("ciso", "Xbox 360",
     "CSO/CCI compressed disc images: info/list/extract, and convert to/from ISO", [
        ("choice", "Operation", None, ["info", "list", "extract", "toiso", "tocso", "tocci"]),
        ("infile", "Image (.cso/.cci) or source .iso", None, True),
        ("outfile", "Output (.iso / .cso / .cci)", None, None),
        ("dir", "Output folder (extract)", None, None),
    ]),
    ("god", "Xbox 360", "GoD/SVOD: read a package, or convert an NXE-installed game to GoD", [
        ("choice", "Operation", None, ["info", "list", "install2god"]),
        ("dir", "GoD .data directory (info/list)", None, None),
        ("infile", "Installed STFS header (install2god)", None, False),
        ("outfile", "Output GoD header (install2god)", None, None),
    ]),
    ("iso2god", "Xbox 360", "Convert an Xbox 360 ISO to Games-on-Demand (GoD/SVOD)", [
        ("infile", "Source ISO", None, True),
        ("dir", "Output directory", None, None),
        ("opt", "Title name", "--title", "text"),
    ]),
    ("god2iso", "Xbox 360", "Convert a GoD/SVOD package back to a raw ISO", [
        ("dir", "GoD .data directory", None, None),
        ("outfile", "Output ISO", None, None),
    ]),
    ("xex2god", "Xbox 360", "Wrap a standalone default.xex as Games-on-Demand (GoD)", [
        ("infile", "default.xex", None, True),
        ("dir", "Output directory", None, None),
        ("opt", "Title name", "--title", "text"),
    ]),
    ("xdbf", "Xbox 360",
     "XDBF/GPD profiles & achievements: read (info/list/ach) + edit (unlock/unlock-all)", [
        ("choice", "Operation", None,
         ["info", "list", "ach", "extract", "unlock", "unlock-all"]),
        ("infile", "GPD / profile file", None, True),
        ("opt", "Achievement id (unlock) / output dir (extract)", None, "text"),
        ("outfile", "Output GPD (unlock / unlock-all)", None, None),
    ]),
    ("account", "Xbox 360", "Decrypt a profile Account file (gamertag, XUID)", [
        ("choice", "Operation", None, ["info"]),
        ("infile", "Account file", None, True),
    ]),

    # ---- Decompile / matching-decompilation ------------------------------
    ("permute", "Decompile", "Matching-decompilation search: idioms -> byte-exact", [
        ("choice", "Arch", "--arch", ["ppc", "x86"]),
        ("infile", "XEX file", "--xex", False),
        ("opt", "Virtual address", "--vaddr", "text"),
        ("infile", "Recon C", None, False),
        ("infile", "Choices JSON", "--choices", False),
        ("opt", "Function", "--func", "text"),
        ("flag", "Self-test", "--selftest", None),
    ]),
    ("oracle", "Decompile", "Codegen oracle / flag-pinner (byte-exact stage F)", [
        ("infile", "Input", None, False),
    ]),
    ("segdiff", "Decompile", "Whole-segment byte-match harness (mask-aware verdict)", [
        ("infile", "Input", None, False),
    ]),
    ("conduct", "Decompile", "Drive a whole module to byte-exact, unattended", [
        ("infile", "XEX / EXE", None, False),
    ]),
    ("verify", "Decompile", "GHOST-check reconstructions against the binary", [
        ("infile", "Input", None, False),
    ]),
    ("ledger", "Decompile", "Coverage ledger (cited bytes, per tier)", [
        ("infile", "XEX / EXE", None, False),
        ("flag", "Carve", "--carve", None),
        ("flag", "Gaps", "--gaps", None),
    ]),
    ("ghidra", "Decompile", "Run Ghidra's decompiler on a module (hypothesis)", [
        ("infile", "XEX / EXE", None, True),
    ]),

    # ---- Assets & build-diffing (folded-in RETool) -----------------------
    ("retool", "Assets & Diff", "Diff class/scene/import inventories & timelines; byte-patches", [
        ("choice", "Subcommand", None,
         ["info", "classes", "scenes", "imports", "strings", "funcs",
          "timeline", "patchdiff", "patchapply"]),
        ("infile", "Image A (.exe)", None, False),
        ("infile", "Image B (.exe)", None, False),
        ("opt", "Extra images / args", None, "text"),
    ]),
    ("pck", "Assets & Diff", "Unpack Minecraft LCE .pck containers", [
        ("infile", ".pck file", None, True),
        ("flag", "Extract (-x)", "-x", None),
    ]),
    ("xzp", "Assets & Diff", "Unpack XUIZ .xzp containers (UI + texture packs)", [
        ("infile", ".xzp file", None, True),
        ("flag", "Extract (-x)", "-x", None),
    ]),

    # ---- Windows superset (NE / PE / LE / OMF) ---------------------------
    ("target", "Windows", "Module inventory + tri-format tier map", [
        ("infile", "Binary", None, False),
    ]),
    ("petrace", "Windows", "PE (32-bit) function carve + call resolution", [
        ("infile", "PE / EXE", None, True),
    ]),
    ("lift", "Windows", "Deterministic NE -> C transliteration", [
        ("infile", "NE module", None, True),
    ]),
    ("cc16", "Windows", "Compile 16-bit C with MSVC 1.52c (byte-exact path)", [
        ("infile", "C source", None, True),
    ]),
    ("asmrecon", "Windows", "disasm -> MASM byte-exact reconstruction", [
        ("infile", "Module", None, True),
    ]),
    ("objextract", "Windows", "Pull code bytes + reloc mask from an OMF .OBJ", [
        ("infile", ".OBJ file", None, True),
    ]),
    ("iso", "Windows", "Read / extract an ISO9660 / Joliet image", [
        ("infile", ".iso file", None, True),
    ]),
    ("extract", "Windows", "Extract the Win95 spanning Quantum cabinet set", [
        ("infile", "Cabinet", None, False),
    ]),

    # ---- Tools / analysis / system ---------------------------------------
    ("identify", "Tools",
     "Auto-detect an Xbox/Xbox 360 file's format (magic + structure) and name its tool", [
        ("infile", "File to identify", None, True),
        ("flag", "Self-test", "--selftest", None),
    ]),
    ("report", "Tools",
     "One-page markdown analysis report (format, build, sections, imports, strings)", [
        ("infile", "File to analyze", None, True),
        ("outfile", "Write .md to (optional)", None, None),
        ("flag", "Self-test", "--selftest", None),
    ]),
    ("view", "Tools",
     "In-app viewers: hex dump, byte diff, and PowerPC disassembly of any file/region", [
        ("choice", "Operation", None, ["hex", "diff", "disasm", "decomp"]),
        ("infile", "File (A for diff)", None, True),
        ("infile", "File B (diff only)", None, False),
        ("opt", "Offset (--offset, hex)", "--offset", "text"),
        ("opt", "Length (--len)", "--len", "text"),
        ("opt", "Virtual address (--vaddr, hex)", "--vaddr", "text"),
    ]),
    ("server", "Tools",
     "Headless REST server: drive every command over HTTP (automation / web console)", [
        ("opt", "Host (--host)", "--host", "text"),
        ("opt", "Port (--port)", "--port", "int"),
        ("opt", "Bearer token (--token, required for non-loopback)", "--token", "text"),
        ("flag", "Self-test", "--selftest", None),
    ]),
    ("datacode", "Tools", "Classify data vs code in a segment (strings/tables)", [
        ("infile", "Module", None, True),
    ]),
    ("reachability", "Tools", "Control-flow reachability + safe data regions", [
        ("infile", "Module", None, True),
    ]),
    ("disasm16", "Tools", "Capstone 16-bit recursive-descent code/data (diagnostic)", [
        ("infile", "Module", None, True),
    ]),
    ("annotate", "Tools", "Label data regions in reconstructed .asm (comment-only)", [
        ("infile", ".asm file", None, True),
    ]),
    ("names", "Tools", "Per-module naming oracle (exports + Midage)", [
        ("infile", "Module", None, False),
    ]),
    ("ordinals", "Tools", "Generate ORDINALS.md from the export tables", []),
    ("doctor", "Tools", "Check every dependency is wired up", []),
    ("config", "Tools", "Show / set external tool paths", []),
    # ---- Xbox 360 crypto (Xbox-360-Crypto, GoobyCorp, BSD-3) --------------
    ("crypto-kv", "Crypto", "keyvault tool (reads CPU/DVD keys)", [
        ("opt", "Extra args", None, "text"),
    ]),
    ("crypto-cpukey", "Crypto", "CPU key utilities (ECC/UID)", [
        ("opt", "Extra args", None, "text"),
    ]),
    ("crypto-keystore", "Crypto", "keyvault / key store inspection", [
        ("infile", "Input", None, False),
    ]),
    ("crypto-xdk", "Crypto", "extract XDK content (input output)", [
        ("infile", "Input", None, True),
        ("outfile", "Output", None, None),
        ("opt", "Prefix", "-p", "text"),
    ]),
    # ---- auto-added so every CLI command is reachable ---------------------
    ("masmlift", "Windows", "Assemble hand-asm with period MASM (byte-exact path)", [
        ("infile", "ASM source", None, True),
    ]),
    ("midage", "Windows", "Read the Midageproject .SYM symbol data", [
        ("infile", ".SYM file", None, False),
    ]),
    ("vmmsvc", "Windows", "Extract VxD/VMM service tables from the DDK", [
        ("infile", "DDK path", None, False),
    ]),
    ("refscan", "Tools", "Score an external source tree's Win95 relevance", [
        ("infile", "Source tree", None, False),
    ]),
    ("refpull", "Tools", "Pull the relevant slice of a source tree into reference/", [
        ("infile", "Source tree", None, False),
    ]),
    ("progressui", "Tools", "Native comctl32 progress meter (demo: --selftest)", [
        ("flag", "Self-test", "--selftest", None),
    ]),
    ("roadmap", "Tools", "Show the project roadmap", []),
    ("reconcount", "Tools", "Count reconstructed functions/files (project tally)", []),
]


# every command in the CLI, so unlisted ones still get a generic form
ALL_COMMANDS = [
    "xex", "ppcdisasm", "ppcrecon", "ppccc", "ppcrecomp", "ppcdiscover", "ppcrun",
    "project", "ppcboot", "iso", "extract", "target", "ordinals", "names", "midage",
    "vmmsvc", "refscan", "refpull", "ledger", "verify", "lift", "petrace", "ghidra",
    "cc16", "objextract", "oracle", "segdiff", "permute", "masmlift", "asmrecon",
    "conduct", "datacode", "reachability", "disasm16", "annotate", "progressui",
    "retool", "pck", "xzp", "doctor", "config", "gui",
    "stfs", "gdf", "god", "iso2god", "god2iso", "xex2god", "xdbf", "account",
    "crypto-kv", "crypto-cpukey", "crypto-keystore", "crypto-xdk",
]


# Category for auto-discovered commands that have no explicit REGISTRY form.
AUTO_CATEGORY = "More"


def _cli_commands():
    """(name, description) for every command the unified CLI exposes -- the
    BUILTINS table plus any auto-registered plugins. Imported live from the CLI
    module so the GUI can never fall behind it. Returns [] if the import fails
    (frozen/exotic layouts), in which case the GUI uses REGISTRY + ALL_COMMANDS."""
    try:
        cli_dir = os.path.join(_PKG, "cli")
        if os.path.isdir(cli_dir) and cli_dir not in sys.path:
            sys.path.insert(0, cli_dir)
        import xboxtranspiler as xt
        cmds = [(n, d) for (n, _s, d) in getattr(xt, "BUILTINS", [])]
        try:
            cmds += [(n, d) for (n, _p, d) in xt.load_plugins()]
        except Exception:
            pass
        return cmds
    except Exception:
        return []


def _build_effective_registry():
    """REGISTRY (hand-written rich forms) + an auto-generated generic entry for
    every CLI command not already covered -- so a newly added command appears in
    the GUI automatically, with a basic {input + raw args} form, without anyone
    editing this file. This is the guarantee that nothing is ever CLI-only."""
    reg = list(REGISTRY)
    have = {e[0] for e in REGISTRY}
    for name, desc in _cli_commands():
        if name in have or name == "gui":              # skip covered + self
            continue
        have.add(name)
        reg.append((name, AUTO_CATEGORY, desc or "(run with arguments)",
                    [("infile", "Input", None, False)]))
    return reg


EFFECTIVE = _build_effective_registry()
# surface the "More" tab only if auto-discovery actually turned something up
if any(e[1] == AUTO_CATEGORY for e in EFFECTIVE) and AUTO_CATEGORY not in CATEGORIES:
    CATEGORIES.append(AUTO_CATEGORY)


def _spec_for(cmd):
    for entry in EFFECTIVE:
        if entry[0] == cmd:
            return entry
    return (cmd, AUTO_CATEGORY, "(run with raw arguments)",
            [("infile", "Input", None, False)])


class App(tk.Tk):
    THEMES = {
        "light": dict(BG="#f5f6f8", INK="#20262e", MUTED="#6b7785", LINE="#e2e5ea",
                      FIELD="#ffffff", TAB="#e6e9ee", BTN="#e7e9ee", BTN_HI="#dbdee4",
                      CONSOLE="#1b1f26", CONSOLE_FG="#d7dde6", SEL="#d7e3f2",
                      ACCENT="#2e7d32", ACCENT_HI="#388e3c", DISABLED="#9aa79b",
                      MENU_BG="#ffffff", MENU_FG="#20262e"),
        "dark":  dict(BG="#20242b", INK="#e7ebf1", MUTED="#8d99a8", LINE="#333b46",
                      FIELD="#2b313b", TAB="#2b313b", BTN="#2f3742", BTN_HI="#3a434f",
                      CONSOLE="#12151a", CONSOLE_FG="#c7d0db", SEL="#33465e",
                      ACCENT="#38a349", ACCENT_HI="#43bd56", DISABLED="#5b6470",
                      MENU_BG="#2b313b", MENU_FG="#e7ebf1"),
    }

    def __init__(self):
        super().__init__()
        self.title("XboxTranspiler")
        self.geometry("1360x900")
        self.minsize(1080, 680)
        self.theme = "dark"
        self.path = tk.StringVar(value="")           # current input file
        self.status = tk.StringVar(value="Ready")
        self.raw_extra = tk.StringVar(value="")
        self._busy = False
        self._proc = None
        self._q = queue.Queue()
        self._hairlines = []
        self._menus = []
        self._tabbtns = []
        self._cur_cmd = None
        self._form_vars = []                          # [(spec, tkvar)]
        self._style = ttk.Style(self)
        try:
            self._style.theme_use("clam")
        except tk.TclError:
            pass
        self._apply_theme(self.theme)
        self._build_header()
        self._build_body()
        self._build_statusbar()
        self._apply_theme(self.theme)
        self._select_cat(CATEGORIES[0])
        self._poll()

    # ---------------------------------------------------------- theming
    def _apply_theme(self, name):
        self.theme = name
        p = self.THEMES[name]
        self.P = p
        st = self._style
        base = ("Segoe UI", 10)
        self.configure(background=p["BG"])
        for w in ("TFrame", "TLabelframe", "TPanedwindow"):
            st.configure(w, background=p["BG"], borderwidth=0)
        st.configure("Bar.TFrame", background=p["BG"])
        st.configure("Card.TLabelframe", background=p["BG"], padding=12)
        st.configure("Card.TLabelframe.Label", background=p["BG"], foreground=p["MUTED"],
                     font=("Segoe UI Semibold", 9))
        st.configure("TLabel", background=p["BG"], foreground=p["INK"], font=base)
        st.configure("Muted.TLabel", background=p["BG"], foreground=p["MUTED"], font=base)
        st.configure("H1.TLabel", background=p["BG"], foreground=p["INK"],
                     font=("Segoe UI Light", 20))
        st.configure("Blurb.TLabel", background=p["BG"], foreground=p["MUTED"],
                     font=("Segoe UI", 10))
        st.configure("TCheckbutton", background=p["BG"], foreground=p["INK"], font=base)
        st.map("TCheckbutton", background=[("active", p["BG"])], foreground=[("active", p["INK"])])
        st.configure("TButton", padding=(12, 6), font=base, background=p["BTN"],
                     foreground=p["INK"], borderwidth=0)
        st.map("TButton", background=[("active", p["BTN_HI"]), ("disabled", p["BTN"])],
               foreground=[("disabled", p["MUTED"])])
        st.configure("Accent.TButton", foreground="#ffffff", background=p["ACCENT"],
                     font=("Segoe UI Semibold", 10), padding=(18, 7), borderwidth=0)
        st.map("Accent.TButton", background=[("active", p["ACCENT_HI"]), ("disabled", p["DISABLED"])])
        st.configure("Ghost.TButton", padding=(14, 7))
        st.configure("TEntry", fieldbackground=p["FIELD"], foreground=p["INK"],
                     insertcolor=p["INK"], padding=5, borderwidth=1)
        st.configure("TCombobox", fieldbackground=p["FIELD"], foreground=p["INK"],
                     background=p["BTN"], arrowcolor=p["INK"], padding=4)
        st.map("TCombobox", fieldbackground=[("readonly", p["FIELD"])])
        st.configure("Menu.TMenubutton", background=p["BG"], foreground=p["INK"],
                     padding=(10, 6), borderwidth=0, arrowcolor=p["BG"], font=base)
        st.map("Menu.TMenubutton", background=[("active", p["BTN_HI"])])
        st.configure("Tab.TButton", background=p["BG"], foreground=p["MUTED"],
                     padding=(15, 8), borderwidth=0, font=("Segoe UI", 10))
        st.map("Tab.TButton", background=[("active", p["BTN_HI"])], foreground=[("active", p["INK"])])
        st.configure("TabOn.TButton", background=p["BG"], foreground=p["ACCENT"],
                     padding=(15, 8), borderwidth=0, font=("Segoe UI Semibold", 10))
        st.map("TabOn.TButton", background=[("active", p["BG"])], foreground=[("active", p["ACCENT"])])
        st.configure("Cmd.TButton", background=p["BG"], foreground=p["INK"],
                     padding=(10, 7), borderwidth=0, anchor="w", font=base)
        st.map("Cmd.TButton", background=[("active", p["BTN_HI"])])
        st.configure("CmdOn.TButton", background=p["SEL"], foreground=p["INK"],
                     padding=(10, 7), borderwidth=0, anchor="w", font=("Segoe UI Semibold", 10))
        st.map("CmdOn.TButton", background=[("active", p["SEL"])])
        st.configure("TProgressbar", background=p["ACCENT"], troughcolor=p["TAB"], borderwidth=0)
        st.configure("TSeparator", background=p["LINE"])
        for hr in self._hairlines:
            try: hr.configure(background=p["LINE"])
            except tk.TclError: pass
        for mn in self._menus:
            try: mn.configure(background=p["MENU_BG"], foreground=p["MENU_FG"],
                              activebackground=p["SEL"], activeforeground=p["INK"])
            except tk.TclError: pass
        con = getattr(self, "console", None)
        if con is not None:
            con.configure(background=p["CONSOLE"], foreground=p["CONSOLE_FG"],
                          insertbackground=p["CONSOLE_FG"], highlightthickness=0)

    def _toggle_theme(self):
        self._apply_theme("light" if self.theme == "dark" else "dark")

    def _hr(self, parent):
        f = tk.Frame(parent, height=1, background=self.P["LINE"])
        self._hairlines.append(f)
        return f

    # ---------------------------------------------------------- header
    def _build_header(self):
        bar = ttk.Frame(self, style="Bar.TFrame", padding=(8, 5))
        bar.pack(side="top", fill="x")
        fm = tk.Menu(self, tearoff=0)
        fm.add_command(label="Open input…", command=self.on_open, accelerator="Ctrl+O")
        fm.add_command(label="Clear input", command=lambda: self.path.set(""))
        fm.add_separator()
        fm.add_command(label="Quit", command=self.destroy)
        ttk.Menubutton(bar, text="File", menu=fm, style="Menu.TMenubutton").pack(side="left")
        vm = tk.Menu(self, tearoff=0)
        self._dark_var = tk.BooleanVar(value=(self.theme == "dark"))
        vm.add_checkbutton(label="Dark mode", variable=self._dark_var,
                           command=self._toggle_theme, accelerator="Ctrl+D")
        vm.add_command(label="Clear console", command=lambda: self._console_clear())
        ttk.Menubutton(bar, text="View", menu=vm, style="Menu.TMenubutton").pack(side="left", padx=(2, 8))
        self._menus = [fm, vm]
        ttk.Separator(bar, orient="vertical").pack(side="left", fill="y", padx=(2, 10), pady=5)
        # category tab buttons
        for cat in CATEGORIES:
            b = ttk.Button(bar, text=cat, style="Tab.TButton",
                           command=lambda c=cat: self._select_cat(c))
            b.pack(side="left")
            self._tabbtns.append((cat, b))
        # current-input chip on the right
        ttk.Label(bar, textvariable=self.path, style="Muted.TLabel").pack(side="right", padx=(0, 8))
        ttk.Label(bar, text="input:", style="Muted.TLabel").pack(side="right")
        self.bind("<Control-o>", lambda e: self.on_open())
        self.bind("<Control-d>", lambda e: (self._dark_var.set(not self._dark_var.get()),
                                            self._toggle_theme()))
        self._hr(self).pack(side="top", fill="x")

    def _select_cat(self, cat):
        self._cur_cat = cat
        for c, b in self._tabbtns:
            b.configure(style="TabOn.TButton" if c == cat else "Tab.TButton")
        # repopulate command rail
        for w in self.rail.winfo_children():
            w.destroy()
        cmds = [e[0] for e in EFFECTIVE if e[1] == cat]
        # EFFECTIVE = REGISTRY + auto-discovered CLI commands, so nothing is hidden
        for name in cmds:
            b = ttk.Button(self.rail, text=name, style="Cmd.TButton",
                           command=lambda n=name: self._select_cmd(n))
            b.pack(side="top", fill="x")
        if cmds:
            self._select_cmd(cmds[0])

    # ---------------------------------------------------------- body
    def _build_body(self):
        body = ttk.Frame(self, style="Bar.TFrame")
        body.pack(side="top", fill="both", expand=True)
        # left rail — command list
        railwrap = ttk.Frame(body, style="Bar.TFrame", padding=(6, 8))
        railwrap.pack(side="left", fill="y")
        ttk.Label(railwrap, text="COMMANDS", style="Muted.TLabel",
                  font=("Segoe UI Semibold", 9)).pack(side="top", anchor="w", padx=6, pady=(0, 4))
        self.rail = ttk.Frame(railwrap, style="Bar.TFrame")
        self.rail.pack(side="top", fill="y")
        self._hr(body).pack(side="left", fill="y")
        # right — form (top) + console (bottom)
        right = ttk.Frame(body, style="Bar.TFrame")
        right.pack(side="left", fill="both", expand=True)
        self.formwrap = ttk.Frame(right, style="Bar.TFrame", padding=(16, 12))
        self.formwrap.pack(side="top", fill="x")
        self._hr(right).pack(side="top", fill="x")
        conbar = ttk.Frame(right, style="Bar.TFrame", padding=(10, 4))
        conbar.pack(side="top", fill="x")
        ttk.Label(conbar, text="OUTPUT", style="Muted.TLabel",
                  font=("Segoe UI Semibold", 9)).pack(side="left")
        ttk.Button(conbar, text="Clear", style="Ghost.TButton",
                   command=self._console_clear).pack(side="right")
        cframe = ttk.Frame(right, style="Bar.TFrame")
        cframe.pack(side="top", fill="both", expand=True)
        self.console = tk.Text(cframe, wrap="none", font=("Consolas", 10), borderwidth=0,
                               background="#12151a", foreground="#c7d0db", padx=10, pady=8)
        ys = ttk.Scrollbar(cframe, orient="vertical", command=self.console.yview)
        xs = ttk.Scrollbar(cframe, orient="horizontal", command=self.console.xview)
        self.console.configure(yscrollcommand=ys.set, xscrollcommand=xs.set, state="disabled")
        ys.pack(side="right", fill="y"); xs.pack(side="bottom", fill="x")
        self.console.pack(side="left", fill="both", expand=True)

    def _select_cmd(self, cmd):
        self._cur_cmd = cmd
        for w in self.formwrap.winfo_children():
            w.destroy()
        self._form_vars = []
        name, cat, blurb, args = _spec_for(cmd)
        ttk.Label(self.formwrap, text=cmd, style="H1.TLabel").pack(side="top", anchor="w")
        ttk.Label(self.formwrap, text=blurb, style="Blurb.TLabel").pack(side="top", anchor="w", pady=(0, 10))
        grid = ttk.Frame(self.formwrap, style="Bar.TFrame")
        grid.pack(side="top", fill="x")
        row = 0
        for spec in args:
            kind, label = spec[0], spec[1]
            var = tk.StringVar()
            if kind in ("infile", "outfile", "dir"):
                # default an input picker to the current global input
                if kind == "infile" and not self._form_vars:
                    var.set(self.path.get())
                ttk.Label(grid, text=label, style="Muted.TLabel").grid(row=row, column=0, sticky="w", pady=4, padx=(0, 10))
                e = ttk.Entry(grid, textvariable=var, width=54)
                e.grid(row=row, column=1, sticky="we", pady=4)
                cmd_pick = {"outfile": self._pick_save, "dir": self._pick_dir}.get(kind, self._pick_open)
                ttk.Button(grid, text="…", width=3, style="Ghost.TButton",
                           command=lambda v=var, f=cmd_pick: f(v)).grid(row=row, column=2, padx=(6, 0))
            elif kind == "flag":
                bv = tk.BooleanVar(value=False)
                ttk.Checkbutton(grid, text=label, variable=bv).grid(row=row, column=1, sticky="w", pady=4)
                var = bv
            elif kind == "choice":
                choices = spec[3]
                var.set(choices[0])
                ttk.Label(grid, text=label, style="Muted.TLabel").grid(row=row, column=0, sticky="w", pady=4, padx=(0, 10))
                ttk.Combobox(grid, textvariable=var, values=choices, state="readonly",
                             width=24).grid(row=row, column=1, sticky="w", pady=4)
            else:  # opt
                ttk.Label(grid, text=label, style="Muted.TLabel").grid(row=row, column=0, sticky="w", pady=4, padx=(0, 10))
                ttk.Entry(grid, textvariable=var, width=32).grid(row=row, column=1, sticky="w", pady=4)
            self._form_vars.append((spec, var))
            row += 1
        grid.columnconfigure(1, weight=1)
        # raw extra args + run/stop
        rawrow = ttk.Frame(self.formwrap, style="Bar.TFrame")
        rawrow.pack(side="top", fill="x", pady=(10, 0))
        ttk.Label(rawrow, text="Extra args", style="Muted.TLabel").pack(side="left", padx=(0, 10))
        ttk.Entry(rawrow, textvariable=self.raw_extra).pack(side="left", fill="x", expand=True)
        btns = ttk.Frame(self.formwrap, style="Bar.TFrame")
        btns.pack(side="top", fill="x", pady=(12, 0))
        self.btn_run = ttk.Button(btns, text="Run", style="Accent.TButton", command=self.on_run)
        self.btn_run.pack(side="left")
        self.btn_stop = ttk.Button(btns, text="Stop", style="Ghost.TButton", command=self.on_stop, state="disabled")
        self.btn_stop.pack(side="left", padx=(8, 0))
        # show the command line preview
        self.cmdline = tk.StringVar(value="")
        ttk.Label(self.formwrap, textvariable=self.cmdline, style="Muted.TLabel",
                  font=("Consolas", 9)).pack(side="top", anchor="w", pady=(10, 0))

    # ---------------------------------------------------------- pickers
    def _pick_open(self, var):
        p = filedialog.askopenfilename(title="Select input")
        if p:
            var.set(p)
            if not self.path.get():
                self.path.set(p)

    def _pick_save(self, var):
        p = filedialog.asksaveasfilename(title="Save output as")
        if p:
            var.set(p)

    def _pick_dir(self, var):
        p = filedialog.askdirectory(title="Select folder")
        if p:
            var.set(p)

    def on_open(self):
        p = filedialog.askopenfilename(
            title="Open input (.xex / .exe / …)",
            filetypes=[("Xbox / PE images", "*.xex *.exe *.xexp"), ("All files", "*.*")])
        if p:
            self.path.set(p)
            if self._cur_cmd:
                self._select_cmd(self._cur_cmd)     # refresh so infile pre-fills

    # ---------------------------------------------------------- run
    def _build_argv(self):
        name, cat, blurb, args = _spec_for(self._cur_cmd)
        argv = [self._cur_cmd]
        for spec, var in self._form_vars:
            kind = spec[0]
            flag = spec[2] if len(spec) > 2 else None
            if kind == "flag":
                if var.get():
                    argv.append(flag)
            elif kind == "choice":
                val = var.get().strip()
                if val:
                    if flag:
                        argv += [flag, val]
                    else:
                        argv.append(val)            # positional subcommand (retool)
            else:  # infile / outfile / opt
                val = var.get().strip()
                if val:
                    if flag:
                        argv += [flag, val]
                    else:
                        argv.append(val)
        extra = self.raw_extra.get().strip()
        if extra:
            argv += extra.split()
        return argv

    def on_run(self):
        if self._busy:
            return
        argv = self._build_argv()
        self.cmdline.set("XboxTranspiler " + " ".join(argv))
        self._console_clear()
        self._console_write("$ XboxTranspiler " + " ".join(argv) + "\n\n")
        self._set_busy(True, "Running %s…" % self._cur_cmd)

        def worker():
            try:
                proc = subprocess.Popen(
                    [sys.executable, ENTRY] + argv,
                    stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                    text=True, bufsize=1, cwd=os.path.dirname(_PKG),
                    env=dict(os.environ, PYTHONIOENCODING="utf-8"))
                self._proc = proc
                for line in iter(proc.stdout.readline, ""):
                    self._q.put(("line", line))
                proc.stdout.close()
                rc = proc.wait()
                self._q.put(("done", rc))
            except Exception as e:
                self._q.put(("line", "\n[gui] failed to launch: %s\n" % e))
                self._q.put(("done", -1))

        threading.Thread(target=worker, daemon=True).start()

    def on_stop(self):
        p = self._proc
        if p and p.poll() is None:
            try:
                p.terminate()
                self._console_write("\n[stopped]\n")
            except Exception:
                pass

    # ---------------------------------------------------------- console + async
    def _console_clear(self):
        self.console.configure(state="normal")
        self.console.delete("1.0", "end")
        self.console.configure(state="disabled")

    def _console_write(self, text):
        self.console.configure(state="normal")
        self.console.insert("end", text)
        self.console.see("end")
        self.console.configure(state="disabled")

    def _poll(self):
        try:
            while True:
                kind, payload = self._q.get_nowait()
                if kind == "line":
                    self._console_write(payload)
                elif kind == "done":
                    self._proc = None
                    self._set_busy(False, "Exit %s" % payload)
                    self._console_write("\n[exit %s]\n" % payload)
        except queue.Empty:
            pass
        self.after(50, self._poll)

    def _set_busy(self, busy, msg=""):
        self._busy = busy
        if msg:
            self.status.set(msg)
        try:
            self.btn_run.configure(state="disabled" if busy else "normal")
            self.btn_stop.configure(state="normal" if busy else "disabled")
        except (AttributeError, tk.TclError):
            pass
        if busy:
            self.progress.start(12)
        else:
            self.progress.stop()

    # ---------------------------------------------------------- statusbar
    def _build_statusbar(self):
        sb = ttk.Frame(self, style="Bar.TFrame")
        sb.pack(side="bottom", fill="x")
        self._hr(sb).pack(side="top", fill="x")
        inner = ttk.Frame(sb, style="Bar.TFrame", padding=(12, 3))
        inner.pack(side="top", fill="x")
        ttk.Label(inner, textvariable=self.status, style="Muted.TLabel").pack(side="left")
        self.progress = ttk.Progressbar(inner, mode="indeterminate", length=160)
        self.progress.pack(side="right")


def main():
    App().mainloop()


if __name__ == "__main__":
    main()
