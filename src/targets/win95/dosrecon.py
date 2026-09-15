#!/usr/bin/env python3
r"""dosrecon.py -- reconstruction organizer for the Win95 real-mode base (IO.SYS + COMMAND.COM).

The real Win95 IO.SYS is WINBOOT.SYS: the MS-DOS 7.0 real-mode kernel (MSLOAD + MSBIO + MSDOS)
with a Win95-specific real-mode layer bolted on -- the registry preloader, the INT 2Fh AX=1613h
"WinBootDir" provider, IFSHLP/DoubleSpace auto-mount, the LOGO, and the MSDOS.SYS [Paths]/[Options]
handling. The DOS 6.0-derived `msdos/` tree has the shared kernel but NOT that Win95 layer -- which
is exactly the registry gate the VMM32 boot hits.

win95mzdec produces a raw whole-file decode (`--tree`), but a raw dump mixes code with data (the
device-driver header table, the BPB, message/string tables) and gives every routine a `sub_<off>`
label. This pass turns that census into an organized, navigable reconstruction:

  * CODE vs DATA classification per discovered function (data = the device table, BPB, string/message
    regions, or runs the 16-bit decoder can't validate);
  * per-function characterization: the INT services it issues and the strings it references;
  * NAMING of the Win95 real-mode layer from those signals (registry_load references SYSTEM.DAT, the
    2Fh dispatcher that answers AX=1613h, the CONFIG.SYS parser, the logo, ...);
  * an organized source tree + a triage REPORT.md with real counts and the Win95-layer work-list.

    wt --target win95 dosrecon [--iosys|--command|--both] [--out DIR]

Read-only against the corpus; writes only under design/recon/.
"""
import os
import re
import sys
import csv
import subprocess

from src.core import paths

try:
    import capstone
except ImportError:
    capstone = None

# --- corpus binaries (the REAL Win95 real-mode files) ---------------------------------------
# Core MS-DOS 7.10 = IO.SYS + COMMAND.COM. The rest are the bundled real-mode drivers/utilities
# shipped with 7.10 (HIMEM/EMM386/DRVSPACE/IFSHLP/SETVER/RAMDRIVE) -- reconstructed for a complete
# port. The .SYS device drivers have no MZ header (loaded flat by CONFIG.SYS).
_BINS = {
    "iosys":    ("winboot.sys", "IO.SYS (WINBOOT.SYS) -- MS-DOS 7.0 real-mode kernel"),
    "command":  ("command.com", "COMMAND.COM -- the Win95 command interpreter"),
    "himem":    ("himem.sys",   "HIMEM.SYS -- XMS / HMA extended-memory manager"),
    "emm386":   ("emm386.exe",  "EMM386.EXE -- expanded-memory / UMB provider"),
    "drvspace": ("drvspace.sys", "DRVSPACE.SYS -- DriveSpace compressed-volume real-mode driver"),
    "ifshlp":   ("ifshlp.sys",  "IFSHLP.SYS -- the IFS (installable file system) real-mode helper"),
    "setver":   ("setver.exe",  "SETVER.EXE -- the DOS version-table TSR"),
    "ramdrive": ("ramdrive.sys", "RAMDRIVE.SYS -- the RAM-disk device driver"),
}
_CORE = ("iosys", "command")
_DRIVERS = ("himem", "emm386", "drvspace", "ifshlp", "setver", "ramdrive")


def _design_root():
    """The Win95 design arena that actually holds the corpus. Prefer the resolver, but fall back to
    the sibling Windows95 repo (<...>/GitHub/Windows95/design) when the resolver can't find it."""
    dr = paths.design_root()
    if os.path.isdir(os.path.join(dr, "corpus")):
        return dr
    sib = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design")
    if os.path.isdir(os.path.join(sib, "corpus")):
        return sib
    return dr


def _corpus(name):
    root = _design_root()
    for cand in (
        os.path.join(root, "corpus", "files", "setup", "inf", name),
        os.path.join(root, "corpus", "files", "WINDOWS", "COMMAND", name),
    ):
        if os.path.isfile(cand):
            return cand
    # last resort: search
    for dp, _dn, fn in os.walk(os.path.join(root, "corpus")):
        for f in fn:
            if f.lower() == name.lower():
                return os.path.join(dp, f)
    return None


def _mzdec():
    b = os.path.join(paths.repo_root(), "native", "decompiler", "build", "bin", "win95mzdec.exe")
    return b if os.path.isfile(b) else None


# --- IO.SYS layout (mirror of the native loader) --------------------------------------------
def _layout(data):
    """Return (is_iosys, body_end, load_base, image) matching win95mzdec's loader."""
    import struct
    if data[:2] != b"MZ":
        return False, 0, 0, data          # raw .SYS device driver: whole file flat, entry 0
    last_page, pages, _rel, hdr_par = struct.unpack("<HHHH", data[2:10])
    hdr = hdr_par * 16
    total = pages * 512
    if last_page:
        total = (pages - 1) * 512 + last_page
    total = min(total, len(data))
    is_iosys = hdr > total // 2
    if is_iosys:
        return True, hdr, 0x0070, data[:total]
    # normal MZ: image is past the header
    return False, 0, 0, data[hdr:total]


# --- string table ---------------------------------------------------------------------------
def _string_regions(image):
    """Map linear offset -> string for printable runs >= 4, used for code->string xref."""
    strings = {}
    for m in re.finditer(rb"[\x20-\x7e]{4,}", image):
        strings[m.start()] = m.group().decode("latin1")
    return strings


def _meaningful(s):
    """Filter coincidental immediate->offset matches: keep strings that read like real text/paths,
    not short punctuation runs from misdecoded operand bytes."""
    if not s or len(s) < 5:
        return False
    letters = sum(c.isalpha() for c in s)
    if letters < max(3, len(s) // 2):
        return False
    return True


# --- per-function analysis ------------------------------------------------------------------
def _analyze_fn(image, fstart, fend, strings, md):
    """Disassemble one function; return (kind, ninsn, nbad, ints:set, strs:list)."""
    ints, strs = set(), []
    ninsn = nbad = 0
    a = fstart
    seglen = min(fend, len(image))
    while a < seglen:
        chunk = bytes(image[a:min(a + 15, seglen)])
        got = list(md.disasm(chunk, a))
        if not got:
            nbad += 1
            a += 1
            continue
        ins = got[0]
        ninsn += 1
        mn = ins.mnemonic
        if mn == "int":
            op = ins.op_str.strip()
            ints.add(op)
        # string xref: try first-segment (disp==offset) and the code's own 64KB block
        for tok in re.findall(r"0x[0-9a-fA-F]+", ins.op_str):
            v = int(tok, 16)
            for off in (v, (a & ~0xFFFF) + v):
                s = strings.get(off)
                if s and _meaningful(s) and len(s) <= 64:
                    strs.append(s); break
        a += ins.size
    # classify code vs data: data regions decode as mostly-bad or trivial
    total = ninsn + nbad
    if total == 0:
        kind = "empty"
    elif nbad > ninsn:
        kind = "data"
    elif ninsn <= 2 and not ints and not strs:
        kind = "stub"
    else:
        kind = "code"
    return kind, ninsn, nbad, ints, strs


# --- naming heuristics for the Win95 real-mode layer ----------------------------------------
_NAME_RULES = [
    (re.compile(r"SYSTEM\.DA", re.I),      "registry"),
    (re.compile(r"\.DAT\b", re.I),          "registry"),
    (re.compile(r"CONFIG\.SYS", re.I),      "config_parse"),
    (re.compile(r"AUTOEXEC", re.I),         "autoexec"),
    (re.compile(r"COMMAND\.COM", re.I),     "load_command"),
    (re.compile(r"WINBOOT|\\WINDOWS", re.I),"winbootdir"),
    (re.compile(r"IFSHLP", re.I),           "ifshlp"),
    (re.compile(r"HIMEM|XMS", re.I),        "xms_himem"),
    (re.compile(r"DBLSPACE|DRVSPACE", re.I),"cvf_mount"),
    (re.compile(r"Windows 95|MS-DOS", re.I),"banner"),
    (re.compile(r"LOGO", re.I),             "logo"),
    (re.compile(r"MSDOS\.SYS", re.I),       "msdossys_cfg"),
]


def _name_for(fstart, kind, ints, strs, region):
    if kind != "code":
        return None
    tags = []
    for s in strs:
        for rx, tag in _NAME_RULES:
            if rx.search(s) and tag not in tags:
                tags.append(tag)
    if tags:
        return "%s_%06X" % (tags[0], fstart)
    # int-service based
    if "0x2f" in ints:
        return "int2f_%06X" % fstart
    if "0x21" in ints:
        return "dos21_%06X" % fstart
    if "0x13" in ints:
        return "disk13_%06X" % fstart
    if "0x10" in ints:
        return "video10_%06X" % fstart
    return None


# INT 21h function names by AH -- the AUTHENTIC Microsoft names, extracted from the MIT-licensed
# MS-DOS source (DOS/DISPATCH.ASM function table). Annotated at each call site as evidence.
_DOS21 = {
    0x00: "Abort", 0x01: "Std_Con_Input", 0x02: "Std_Con_Output", 0x03: "Std_Aux_Input",
    0x04: "Std_Aux_Output", 0x05: "Std_Printer_Output", 0x06: "Raw_Con_IO",
    0x07: "Raw_Con_Input", 0x08: "Std_Con_Input_No_Echo", 0x09: "Std_Con_String_Output",
    0x0A: "Std_Con_String_Input", 0x0B: "Std_Con_Input_Status", 0x0C: "Std_Con_Input_Flush",
    0x0D: "Disk_Reset", 0x0E: "Set_Default_Drive", 0x0F: "FCB_Open", 0x10: "FCB_Close",
    0x11: "Dir_Search_First", 0x12: "Dir_Search_Next", 0x13: "FCB_Delete",
    0x14: "FCB_Seq_Read", 0x15: "FCB_Seq_Write", 0x16: "FCB_Create", 0x17: "FCB_Rename",
    0x19: "Get_Default_Drive", 0x1A: "Set_DMA", 0x1F: "Get_Default_DPB",
    0x21: "FCB_Random_Read", 0x22: "FCB_Random_Write", 0x23: "Get_FCB_File_Length",
    0x24: "Get_FCB_Position", 0x25: "Set_Interrupt_Vector", 0x26: "Create_Process_Data_Block",
    0x27: "FCB_Random_Read_Block", 0x28: "FCB_Random_Write_Block",
    0x29: "Parse_File_Descriptor", 0x2A: "Get_Date", 0x2B: "Set_Date", 0x2C: "Get_Time",
    0x2D: "Set_Time", 0x2E: "Set_Verify_On_Write", 0x2F: "Get_DMA", 0x30: "Get_Version",
    0x31: "Keep_Process", 0x32: "Get_DPB", 0x33: "Set_CTRL_C_Trapping", 0x34: "Get_InDOS_Flag",
    0x35: "Get_Interrupt_Vector", 0x36: "Get_Drive_Freespace", 0x37: "Char_Oper",
    0x38: "International", 0x39: "MKDir", 0x3A: "RMDir", 0x3B: "CHDir", 0x3C: "Creat",
    0x3D: "Open", 0x3E: "Close", 0x3F: "Read", 0x40: "Write", 0x41: "Unlink", 0x42: "LSeek",
    0x43: "CHMod", 0x44: "IOCtl", 0x45: "XDup", 0x46: "XDup2", 0x47: "Current_Dir",
    0x48: "Alloc", 0x49: "Dealloc", 0x4A: "Setblock", 0x4B: "Exec", 0x4C: "Exit", 0x4D: "Wait",
    0x4E: "Find_First", 0x4F: "Find_Next", 0x50: "Set_Current_PDB", 0x51: "Get_Current_PDB",
    0x52: "Get_In_Vars", 0x53: "SetDPB", 0x54: "Get_Verify_On_Write", 0x55: "Dup_PDB",
    0x56: "Rename", 0x57: "File_Times", 0x58: "AllocOper", 0x59: "GetExtendedError",
    0x5A: "CreateTempFile", 0x5B: "CreateNewFile", 0x5C: "LockOper", 0x5D: "ServerCall",
    0x5E: "UserOper", 0x5F: "AssignOper", 0x60: "TrueName", 0x62: "Get_PSP",
    0x63: "Get_DBCS_Vector", 0x65: "Get_Extended_Country", 0x66: "Get_Set_Codepage",
    0x68: "Commit_File", 0x6C: "Extended_Open", 0x71: "LFN", 0x73: "FAT32_Ext",
}
_INT_NAME = {"0x21": "DOS", "0x2f": "multiplex", "0x13": "BIOS disk", "0x10": "BIOS video",
             "0x16": "BIOS kbd", "0x15": "BIOS misc", "0x25": "set int vec",
             "0x26": "abs disk write", "0x19": "reboot", "0x11": "equip", "0x12": "memsize",
             "0x2a": "network", "0x14": "serial", "0x17": "printer", "0x1a": "RTC", "0x67": "EMS"}

# INT 2Fh multiplex by full AX (the documented public interface: Windows, IFS/network redirector,
# XMS driver control, MSCDEX). Names HIMEM/IFSHLP/the Win95 real-mode layer's 2Fh traffic.
_INT2F = {
    0x1100: "IFS_installcheck", 0x1101: "IFS_removedir", 0x1103: "IFS_mkdir", 0x1105: "IFS_rmdir",
    0x1106: "IFS_chdir", 0x1108: "IFS_read", 0x1109: "IFS_write", 0x110B: "IFS_lock",
    0x110C: "IFS_getspace", 0x110E: "IFS_setattr", 0x110F: "IFS_getattr", 0x1116: "IFS_open",
    0x1117: "IFS_create", 0x111B: "IFS_findfirst", 0x111C: "IFS_findnext", 0x111D: "IFS_closeall",
    0x1122: "IFS_procterm", 0x1123: "IFS_qualify", 0x1125: "IFS_pathcmp", 0x112E: "IFS_specialredir",
    0x1500: "MSCDEX_installcheck", 0x1501: "MSCDEX_driverlist", 0x150B: "MSCDEX_drivecheck",
    0x1600: "WIN_enh_installcheck", 0x1605: "WIN_startup_bcast", 0x1606: "WIN_exit_bcast",
    0x1607: "WIN_vxd_api", 0x1608: "WIN_init_complete", 0x1609: "WIN_begin_exit",
    0x160A: "WIN_get_version", 0x1610: "WIN_release_timeslice", 0x1611: "WIN_get_deviceapi",
    0x1613: "WIN_get_winbootdir", 0x1685: "WIN_switch_vm", 0x168A: "WIN_get_vxd_entry",
    0x4300: "XMS_installcheck", 0x4310: "XMS_get_entry",
    0x4A01: "WIN_get_hma_free", 0x4A02: "WIN_alloc_hma",
}
# XMS driver functions by AH (HIMEM.SYS, reached via the entry from INT 2Fh 4310h).
_XMS = {
    0x00: "XMS_version", 0x01: "XMS_request_hma", 0x02: "XMS_release_hma", 0x03: "XMS_global_enable_a20",
    0x04: "XMS_global_disable_a20", 0x05: "XMS_local_enable_a20", 0x06: "XMS_local_disable_a20",
    0x07: "XMS_query_a20", 0x08: "XMS_query_free", 0x09: "XMS_alloc", 0x0A: "XMS_free",
    0x0B: "XMS_move", 0x0C: "XMS_lock", 0x0D: "XMS_unlock", 0x0E: "XMS_get_handle_info",
    0x0F: "XMS_realloc", 0x10: "XMS_alloc_umb", 0x11: "XMS_free_umb", 0x12: "XMS_realloc_umb",
}
# EMS (INT 67h) by AH (EMM386.EXE / EMM).
_EMS = {
    0x40: "EMS_status", 0x41: "EMS_get_frame", 0x42: "EMS_get_pages", 0x43: "EMS_alloc",
    0x44: "EMS_map", 0x45: "EMS_free", 0x46: "EMS_version", 0x47: "EMS_save_map",
    0x48: "EMS_restore_map", 0x4B: "EMS_get_handle_count", 0x4C: "EMS_get_handle_pages",
    0x4E: "EMS_get_set_map", 0x4F: "EMS_get_set_partial", 0x50: "EMS_map_multi",
    0x51: "EMS_realloc", 0x58: "EMS_get_mappable", 0x59: "EMS_get_hw_config", 0x5A: "EMS_alloc_std",
}
# BIOS INT 13h (disk) / INT 10h (video) by AH -- the common ones in the boot/device paths.
_INT13 = {0x00: "disk_reset", 0x01: "disk_status", 0x02: "disk_read", 0x03: "disk_write",
          0x04: "disk_verify", 0x08: "disk_getparams", 0x15: "disk_gettype", 0x41: "disk_ext_check",
          0x42: "disk_ext_read", 0x43: "disk_ext_write", 0x48: "disk_ext_getparams"}
_INT10 = {0x00: "set_video_mode", 0x01: "set_cursor_shape", 0x02: "set_cursor_pos",
          0x03: "get_cursor", 0x06: "scroll_up", 0x07: "scroll_down", 0x08: "read_char_attr",
          0x09: "write_char_attr", 0x0E: "teletype_out", 0x0F: "get_video_mode", 0x13: "write_string"}


def _beautify_name(fstart, kind, ints, strs, region):
    """Always return a name: evidence-based where possible (Win95 layer strings, INT service),
    else the sub_<addr> reconstruction label. Address suffix keeps every name unique."""
    if kind == "data":
        # a run that is mostly one printable string -> str_, else data_
        for s in strs:
            if len(s) >= 6:
                return "data_%06X" % fstart
        return "data_%06X" % fstart
    nm = _name_for(fstart, kind, ints, strs, region)
    return nm or ("sub_%06X" % fstart)


def _emit_asm(which, outroot, image, is_iosys, body_end, starts, ends, strings, md,
              namemap=None, fkind=None, desc=""):
    r"""Emit annotated MASM-style reconstruction per region, with DS-tracked string xref
    resolution and INT-service comments. This is the faithful authored form for 16-bit real-mode
    code (MS-DOS/COMMAND.COM were written in assembly). The body loads at real-mode segment 0x0070,
    so a runtime DS value D addresses file offset (D*16 + disp) - 0x0700."""
    LOAD_LIN = 0x0070 * 16 if is_iosys else 0

    def region_of(a):
        if not is_iosys:
            return which
        if a < 0x0800:
            return "msload"
        if a < body_end:
            return "msbio"
        return "sysinit"

    def resolve_str(ds_base, disp, faddr=0):
        """Resolve a displacement to a string. Try (a) a known DS base, (b) the first body
        segment (disp == file offset), (c) the code's own 64KB block (position-independent
        CS-relative data lives in the same segment as the code)."""
        cands = []
        if ds_base is not None:
            cands.append(ds_base * 16 + disp - LOAD_LIN)
        cands.append(disp)                                  # seg 0x70 / first block
        cands.append((faddr & ~0xFFFF) + disp)              # code's 64KB block
        for off in cands:
            s = strings.get(off)
            if s and _meaningful(s) and len(s) <= 96:
                return s
        return None

    outdir = os.path.join(outroot, which)
    os.makedirs(outdir, exist_ok=True)
    start_set = set(starts)
    namemap = namemap or {}
    fkind = fkind or {}

    def nm(a):
        return namemap.get(a, "sub_%06X" % a)
    _JCC = {"jmp", "je", "jne", "jz", "jnz", "jb", "jnb", "jbe", "ja", "jae", "jl", "jle",
            "jg", "jge", "js", "jns", "jc", "jnc", "jo", "jno", "jp", "jnp", "jcxz", "jecxz",
            "jpe", "jpo", "jna", "jnae", "jnb", "jnbe", "jng", "jnge", "jnl", "jnle",
            "loop", "loope", "loopne", "loopz", "loopnz", "call"}

    def fn_insns(s, e):
        out = []
        a = s
        seglen = min(e, len(image))
        n = 0
        while a < seglen and n < 6000:
            got = list(md.disasm(bytes(image[a:min(a + 15, seglen)]), a))
            if not got:
                out.append((a, None)); a += 1; n += 1; continue
            out.append((a, got[0])); a += got[0].size; n += 1
        return out

    # group functions by region, in address order
    regions = {}
    for s in starts:
        regions.setdefault(region_of(s), []).append(s)

    def masm_hex(ops):
        """Rewrite capstone syntax to MASM: 0xNN -> 0NNh radix form, and add an explicit ds:
        override to bare direct-numeric memory operands ([043Fh] -> ds:[043Fh]) which MASM
        requires (capstone omits the default segment)."""
        ops = re.sub(r"0x[0-9a-fA-F]+", lambda m: "0" + m.group(0)[2:].upper() + "h", ops)
        # bare [<number>] direct memory (hex ...h or decimal), no register / no existing seg
        # override -> add ds: (MASM omits the default segment that capstone drops)
        ops = re.sub(r"(?<![a-zA-Z0-9:\]])\[([0-9A-Fa-f]+h|\d+)\]", r"ds:[\1]", ops)
        return ops

    written = []
    for r, rstarts in regions.items():
        # pass 1 over the whole region: decode every function, collect all branch/call targets
        fdecode = {s: fn_insns(s, ends[s]) for s in rstarts}
        rlo, rhi = min(rstarts), max(ends[s] for s in rstarts)
        boundaries = {a for s in rstarts for a, ins in fdecode[s]}   # real instruction starts
        labels = {}          # intra-region branch targets -> global label
        externs = set()      # sub_ referenced but outside this region
        for s in rstarts:
            for a, ins in fdecode[s]:
                if ins and ins.mnemonic in _JCC and ins.op_str.startswith("0x"):
                    try: t = int(ins.op_str, 16)
                    except ValueError: continue
                    if ins.mnemonic == "call":
                        if t in start_set and not (rlo <= t < rhi):
                            externs.add(t)                       # call to a function in another region
                    else:
                        if t in start_set and not (rlo <= t < rhi):
                            externs.add(t)                       # branch to a function in another region
                        elif rlo <= t < rhi and t not in start_set and t in boundaries:
                            # only label a target that is a real instruction boundary; misaligned
                            # targets (jumps into data-as-code) are emitted as db at their site
                            labels.setdefault(t, "L%06X" % t)

        f = open(os.path.join(outdir, r + ".asm"), "w")
        f.write(
            ";==============================================================================\n"
            "; %s.asm -- %s\n"
            ";   region: %s   (reverse-engineered from the REAL Win95 binary, not the 6.0 tree)\n"
            ";==============================================================================\n"
            "; Reconstructed by CodeBreaker dosrecon. 16-bit real-mode; assembles with the period\n"
            "; Microsoft MASM 6.13 (ML /c). Beautified: evidence-based names (Win95-layer strings,\n"
            "; INT services), label-resolved control flow, DS-tracked string xrefs, service comments,\n"
            "; code/data separation. Names lacking evidence keep the sub_<fileoffset> reconstruction\n"
            "; label -- a stripped binary holds no name for a private routine.\n"
            ";\n"
            "; Naming references (public / open-source, for the API surface only -- NOT leaked source):\n"
            ";   * the documented interrupt catalog (DOS/BIOS/XMS/EMS/IFS/Windows INT 2Fh);\n"
            ";   * authentic Microsoft DOS function names from the MIT-licensed MS-DOS source\n"
            ";     (github.com/microsoft/MS-DOS, DISPATCH.ASM function table).\n\n"
            "        .386p\n" % (r, desc or which.upper(), r))
        if externs:
            f.write("\n; ---- imported from sibling regions of this module ----\n")
            for t in sorted(externs):
                f.write("        EXTRN %s:NEAR\n" % nm(t))
        f.write("\n_TEXT   SEGMENT byte public 'CODE'\n        ASSUME cs:_TEXT\n")

        # a 16-bit 1995 kernel has no SSE/MMX/32-bit-only forms; their presence means the run is
        # DATA misdecoded as code -> emit the whole function as db bytes instead of instructions.
        _SUSPECT = ("xmm", "ymm", "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7")
        _SUSPECT_MN = {"shufps", "aad", "aam", "salc", "icebp", "fcmovb", "vmcall"}

        def is_data_fn(dec):
            n = len(dec) or 1
            bad = 0
            for _a, ins in dec:
                if ins is None:
                    bad += 1; continue
                if ins.mnemonic in _SUSPECT_MN or any(t in ins.op_str for t in _SUSPECT):
                    bad += 1
            return bad * 100 // n >= 12       # >=12% implausible -> data

        for s in rstarts:
            dec = fdecode[s]
            if is_data_fn(dec):
                f.write("\n;----- %s : data block (0x%06X, %d bytes) ----------------------\n"
                        % (nm(s), s, ends[s] - s))
                f.write("%s label near\n" % nm(s))
                end = min(ends[s], len(image))
                a = s
                while a < end:
                    if a in labels:
                        f.write("%s:\n" % labels[a])
                    # emit up to 16 bytes, but never past the next label (so labels stay aligned)
                    run = min(16, end - a)
                    for j in range(1, run):
                        if (a + j) in labels:
                            run = j; break
                    chunk = bytes(image[a:a + run])
                    asc = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
                    f.write("        db " + ",".join("0%02Xh" % b for b in chunk)
                            + (" " * max(1, 52 - 5 * run)) + "; " + asc + "\n")
                    a += run
                continue
            rc = fkind.get(s, "code")
            f.write("\n;----- %s (0x%06X, %s) %s\n"
                    % (nm(s), s, rc, "-" * max(0, 40 - len(nm(s)))))
            f.write("%s label near\n" % nm(s))
            ds_base = None
            pend_ax = None
            pend_ah = None
            for a, ins in fdecode[s]:
                if a in labels:
                    f.write("%s:\n" % labels[a])
                if ins is None:
                    f.write("        db 0%02Xh\n" % image[a]); continue
                mn, ops = ins.mnemonic, ins.op_str
                cmt = ""
                # instructions MASM 16-bit won't accept in these forms (mostly bytes from a data
                # region misdecoded as code) -> emit their raw bytes as db, keeping the disasm as a
                # comment. Faithful and always assembles.
                hard = (mn in ("aad", "aam", "salc", "shufps", "icebp", "invd", "wbinvd",
                               "xadd", "lar", "lsl", "ud2", "rsm", "sysenter", "sysexit",
                               "cmpxchg", "cmpxchg8b", "rdtsc", "rdmsr", "wrmsr", "syscall", "sysret")
                        or mn.startswith("cmov") or mn.startswith("fcmov") or mn.startswith("set")
                        or any(t in ops for t in ("xmm", "ymm", "mm0", "mm1", "mm2", "mm3",
                                                  "mm4", "mm5", "mm6", "mm7", "st(", "cr0", "cr2",
                                                  "cr3", "cr4", "dr0", "dr7", "tr6", "tr7"))
                        or (mn in ("call", "jmp", "lcall", "ljmp") and ("," in ops or ":" in ops))
                        or (mn in ("movzx", "movsx")           # 16<-16 movzx/movsx is invalid (misdecode)
                            and re.search(r",\s*(ax|bx|cx|dx|si|di|bp|sp)\b", ops)
                            and not re.search(r"^(e[a-z]{2})", ops))
                        or (ins.bytes and (ins.bytes[0] in (0x9A, 0xEA)
                                           or (len(ins.bytes) > 1 and ins.bytes[0] == 0x66
                                               and ins.bytes[1] in (0x9A, 0xEA)))))  # far direct
                # short-only jumps (jcxz/loop*) to a far target are misdecoded data -> db
                if mn in ("jcxz", "jecxz", "loop", "loope", "loopne") and ops.startswith("0x"):
                    try:
                        if abs(int(ops, 16) - (a + ins.size)) > 120:
                            hard = True
                    except ValueError:
                        pass
                if hard:
                    bys = ",".join("0%02Xh" % b for b in ins.bytes)
                    f.write("        db %-28s ; %s %s\n" % (bys, mn, ops))
                    continue
                # capstone->MASM mnemonic fixups
                if mn == "int3":
                    mn, ops = "int", "3"
                elif mn == "int1":
                    mn, ops = "int", "1"
                elif mn == "pushal":
                    mn, ops = "pushad", ""
                elif mn == "popal":
                    mn, ops = "popad", ""
                elif mn == "nop" and ops:            # multi-byte nop -> MASM nop takes no operand
                    ops = ""
                # string ops carry implicit operands in MASM; capstone spells them out (and may
                # merge a rep/repe/repne prefix into the mnemonic) -> keep mnemonic, drop operands
                elif mn.split()[-1] in ("lodsb", "lodsw", "lodsd", "stosb", "stosw", "stosd",
                                        "scasb", "scasw", "scasd", "movsb", "movsw", "movsd",
                                        "cmpsb", "cmpsw", "cmpsd", "insb", "insw", "insd",
                                        "outsb", "outsw", "outsd"):
                    ops = ""
                # lds/les/l?s load a far pointer; capstone emits a stray "ptr" -> size it
                elif mn in ("lds", "les", "lfs", "lgs", "lss") and ", ptr " in ops:
                    ops = ops.replace(", ptr ", ", dword ptr ")
                # normalize far call/jmp
                far = mn in ("lcall", "ljmp")
                if mn == "lcall": mn = "call"
                elif mn == "ljmp": mn = "jmp"
                # branch/call operand -> label / sub_ name; unresolvable numeric target -> db
                if mn in _JCC and ops.startswith("0x"):
                    try:
                        t = int(ops, 16)
                    except ValueError:
                        t = None
                    if t is not None:
                        if t in start_set:
                            if not (rlo <= t < rhi):
                                externs.add(t)
                            ops = nm(t)
                        elif t in labels:
                            ops = labels[t]
                        else:
                            bys = ",".join("0%02Xh" % b for b in ins.bytes)
                            f.write("        db %-28s ; %s 0%06Xh (misaligned/data target)\n"
                                    % (bys, mn, t)); continue
                elif far and "[" in ops and "ptr" not in ops:
                    ops = "fword ptr " + ops           # indirect far through memory
                # near indirect jmp/call through memory needs an explicit size (MASM)
                if mn in ("jmp", "call") and "[" in ops and "ptr" not in ops and not far:
                    ops = "word ptr " + ops
                # lgdt/lidt/sgdt/sidt take a 6-byte (fword) descriptor; capstone omits the size
                if mn in ("lgdt", "lidt", "sgdt", "sidt") and "[" in ops and "ptr" not in ops:
                    ops = "fword ptr " + ops
                # DS + AH tracking (for string xrefs and the DOS service annotation)
                if mn == "mov" and ops.startswith(("ax,", "bx,")) and "0x" in ops:
                    try: pend_ax = int(ops.split(",")[1], 16)
                    except ValueError: pend_ax = None
                elif mn == "mov" and ops.startswith("ds,"):
                    ds_base = pend_ax
                if mn == "mov" and ops.startswith("ah,") and "0x" in ops:
                    try: pend_ah = int(ops.split(",")[1], 16)
                    except ValueError: pend_ah = None
                elif mn == "mov" and ops.startswith("ax,") and "0x" in ops:
                    try: pend_ah = (int(ops.split(",")[1], 16) >> 8) & 0xFF
                    except ValueError: pend_ah = None
                if ins.mnemonic == "int":
                    cmt = _int_comment(ins.op_str)
                    iv = ins.op_str
                    if iv == "0x21" and pend_ah in _DOS21:
                        cmt = "INT 21h AH=%02Xh %s" % (pend_ah, _DOS21[pend_ah])
                    elif iv == "0x2f" and pend_ax in _INT2F:
                        cmt = "INT 2Fh AX=%04Xh %s" % (pend_ax, _INT2F[pend_ax])
                    elif iv == "0x67" and pend_ah in _EMS:
                        cmt = "INT 67h AH=%02Xh %s" % (pend_ah, _EMS[pend_ah])
                    elif iv == "0x13" and pend_ah in _INT13:
                        cmt = "INT 13h AH=%02Xh %s" % (pend_ah, _INT13[pend_ah])
                    elif iv == "0x10" and pend_ah in _INT10:
                        cmt = "INT 10h AH=%02Xh %s" % (pend_ah, _INT10[pend_ah])
                for tok in re.findall(r"0x[0-9a-fA-F]+", ins.op_str):
                    v = int(tok, 16)
                    st = resolve_str(ds_base, v, a)
                    if st:
                        cmt = '"%s"' % st.replace('"', "'"); break
                ops = masm_hex(ops)
                # jcxz/loop* are short-only (+-127); MASM re-encodes sizes differently from the
                # original build, so a genuine in-range jump can drift out of reach. Emit a
                # trampoline (short branch to a near jmp) so range is guaranteed regardless.
                if mn in ("jcxz", "jecxz", "loop", "loope", "loopne") and ops and ops[0].isalpha():
                    tl = "Tj%06X" % a
                    f.write("        %s %s\n        jmp short %s_s\n%s_h: jmp %s\n%s_s:\n"
                            % (mn, tl + "_h", tl, tl, ops, tl))
                    continue
                txt = ("%s %s" % (mn, ops)).rstrip()
                if cmt:
                    f.write("        %-34s ; %s\n" % (txt, cmt))
                else:
                    f.write("        %s\n" % txt)
        f.write("\n_TEXT   ENDS\n        END\n")
        f.close()
        written.append(r)
    return written


def _int_comment(op):
    m = {"0x21": "DOS", "0x2f": "multiplex", "0x13": "BIOS disk", "0x10": "BIOS video",
         "0x16": "BIOS kbd", "0x15": "BIOS misc", "0x25": "set int vec", "0x26": "abs disk write",
         "0x19": "reboot", "0x11": "equip", "0x12": "memsize", "0x2a": "network"}
    return "INT %s (%s)" % (op, m.get(op, "?"))


def _struct_hash(image, s, e, md):
    """Structural hash of a function: opcode mnemonics + operand shapes, with immediates/
    displacements masked so link-time addresses don't matter. Identifies the same routine
    statically linked into multiple components."""
    import hashlib
    seq = []
    a = s
    end = min(e, len(image))
    n = 0
    while a < end and n < 400:
        g = list(md.disasm(bytes(image[a:min(a + 15, end)]), a))
        if not g:
            seq.append("db"); a += 1; n += 1; continue
        i = g[0]
        shape = re.sub(r"0x[0-9a-f]+", "#", i.op_str)      # mask constants
        seq.append(i.mnemonic + "|" + shape)
        a += i.size; n += 1
    if len(seq) < 6:
        return None
    return hashlib.md5("\n".join(seq).encode()).hexdigest()[:10]


def _global_dedup(targets):
    """Across all components, find functions with identical structure in >=2 components -> shared
    runtime/library code. Returns {(which, addr): 'shared_<h>'} for those functions."""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    mz = _mzdec()
    by_hash = {}                       # hash -> set(which); and per-(which,addr)
    per = {}                           # (which, addr) -> hash
    for which in targets:
        fname, _ = _BINS[which]
        path = _corpus(fname)
        if not path or not mz:
            continue
        data = open(path, "rb").read()
        _is, _be, _lb, image = _layout(data)
        if _is:
            image = data
        out = subprocess.run([mz, path, "--names"], capture_output=True, text=True).stdout
        starts = sorted({int(l.split("\t")[0], 16) for l in out.splitlines() if l.split("\t")[0]})
        ends = {s: (starts[i + 1] if i + 1 < len(starts) else len(image)) for i, s in enumerate(starts)}
        for s in starts:
            h = _struct_hash(image, s, ends[s], md)
            if h:
                per[(which, s)] = h
                by_hash.setdefault(h, set()).add(which)
    shared = {}
    for (which, s), h in per.items():
        if len(by_hash.get(h, ())) >= 2:
            shared[(which, s)] = "shared_%s" % h
    return shared


def _run_one(which, outroot, shared=None):
    fname, desc = _BINS[which]
    path = _corpus(fname)
    if not path:
        print("  MISSING corpus file: %s" % fname)
        return None
    data = open(path, "rb").read()
    is_iosys, body_end, load_base, image = _layout(data)
    # for iosys the image is the whole file; for MZ it's the load module
    strings = _string_regions(image if not is_iosys else data)
    if is_iosys:
        image = data  # whole-file flat, matches loader linear==fileoffset
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)

    # function starts from win95mzdec --names
    mz = _mzdec()
    starts = []
    if mz:
        out = subprocess.run([mz, path, "--names"], capture_output=True, text=True).stdout
        for line in out.splitlines():
            p = line.split("\t")
            if p and p[0]:
                try:
                    starts.append(int(p[0], 16))
                except ValueError:
                    pass
    # synthetic starts at region heads so EVERY byte is covered: file start (MZ header + the
    # MSLOAD/PSP entry before the first discovered function) and, for IO.SYS, the SYSINIT boundary.
    starts.append(0)
    if is_iosys and body_end:
        starts.append(body_end)
    starts = sorted(set(s for s in starts if s < len(image)))
    ends = {s: (starts[i + 1] if i + 1 < len(starts) else len(image)) for i, s in enumerate(starts)}

    def region_of(a):
        if not is_iosys:
            return which
        if a < 0x0800:
            return "msload"
        if a < body_end:
            return "msbio"
        return "sysinit"

    rows = []
    counts = {"code": 0, "data": 0, "stub": 0, "empty": 0}
    win95_layer = []
    namemap = {}                       # addr -> beautified name (for labels + call refs)
    fkind = {}                         # addr -> kind (for role comments)
    for s in starts:
        e = ends[s]
        if e - s > 0x4000:      # cap runaway (data gaps); analyze the head
            e = s + 0x4000
        kind, ni, nb, ints, strs = _analyze_fn(image, s, e, strings, md)
        counts[kind] = counts.get(kind, 0) + 1
        nm = _name_for(s, kind, ints, strs, region_of(s))
        base = _beautify_name(s, kind, ints, strs, region_of(s))
        # shared runtime (identical structure in >=2 components) overrides a bare sub_ label
        if shared and (which, s) in shared and base.startswith("sub_"):
            base = "%s_%06X" % (shared[(which, s)], s)
        namemap[s] = base
        fkind[s] = kind
        rows.append({
            "addr": "0x%06X" % s, "end": "0x%06X" % ends[s], "size": ends[s] - s,
            "region": region_of(s), "kind": kind, "ninsn": ni,
            "ints": " ".join(sorted(ints)), "name": nm or "",
            "strings": " | ".join(dict.fromkeys(strs))[:200],
        })
        if nm and any(t in nm for t in ("registry", "config", "winbootdir", "ifshlp",
                                        "load_command", "cvf_mount", "msdossys", "logo",
                                        "banner", "autoexec", "int2f")):
            win95_layer.append((s, nm, sorted(ints), list(dict.fromkeys(strs))[:6]))

    outdir = os.path.join(outroot, which)
    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, "triage.csv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["addr", "end", "size", "region", "kind",
                                          "ninsn", "ints", "name", "strings"])
        w.writeheader()
        w.writerows(rows)
    asm_regions = _emit_asm(which, outroot, image, is_iosys, body_end, starts, ends, strings, md,
                            namemap, fkind, desc)
    print("  emitted annotated asm: %s" % ", ".join("%s.asm" % r for r in asm_regions))
    return {
        "which": which, "path": path, "desc": desc, "is_iosys": is_iosys,
        "body_end": body_end, "nfuncs": len(starts), "counts": counts,
        "win95_layer": win95_layer, "outdir": outdir,
    }


def _assemble(outroot, results):
    """Assemble the emitted region .asm files with the period MASM 6.13 (ML /c). Proves the
    reconstruction is compilable (the 'C' in Logan's premise) with a from-that-era toolchain."""
    ml = os.path.join(paths.repo_root(), "toolchains", "period", "assemblers", "masm-6.13", "ML.EXE")
    if not os.path.isfile(ml):
        print("  (ML.EXE not found; skipping assemble)"); return
    print("\n== assemble (MASM 6.13 ML /c) ==")
    total_err = total_obj = 0
    for r in results:
        d = os.path.join(outroot, r["which"])
        for asm in sorted(f for f in os.listdir(d) if f.endswith(".asm")):
            base = asm[:-4]
            obj = os.path.join(d, base + ".obj")
            log = os.path.join(d, base + ".asmlog")
            p = subprocess.run([ml, "/c", "/Fo", obj, os.path.join(d, asm)],
                               capture_output=True, text=True, cwd=d)
            out = (p.stdout or "") + (p.stderr or "")
            open(log, "w").write(out)
            errs = out.count("error A")
            sz = os.path.getsize(obj) if os.path.isfile(obj) else 0
            total_err += errs; total_obj += sz
            print("  %-8s/%-12s errors=%-3d obj=%d bytes" % (r["which"], asm, errs, sz))
    print("  TOTAL: %d errors, %d bytes of object code" % (total_err, total_obj))
    if total_err == 0:
        print("  *** the reconstruction assembles clean with the period MASM ***")


def main(argv):
    if capstone is None:
        print("dosrecon: needs capstone (pip install capstone)")
        return 2
    which = "both"
    outroot = os.path.join(_design_root(), "recon")
    do_build = False
    i = 0
    while i < len(argv):
        a = argv[i]
        if a in ("--iosys", "--command", "--himem", "--emm386", "--drvspace",
                 "--ifshlp", "--setver", "--ramdrive"):
            which = a[2:]
        elif a == "--both":
            which = "both"
        elif a == "--drivers":
            which = "drivers"
        elif a == "--all":
            which = "all"
        elif a == "--build":
            do_build = True
        elif a == "--out" and i + 1 < len(argv):
            outroot = argv[i + 1]; i += 1
        i += 1

    if which == "both":
        targets = list(_CORE)
    elif which == "drivers":
        targets = list(_DRIVERS)
    elif which == "all":
        targets = list(_CORE) + list(_DRIVERS)
    else:
        targets = [which]
    # cross-component structural dedup names shared runtime/library code (identify-once)
    shared = {}
    if len(targets) > 1:
        print("== cross-component dedup ==")
        shared = _global_dedup(targets)
        ncomp = len(set(w for (w, _a) in shared))
        print("  %d shared-runtime function instances across %d components" % (len(shared), ncomp))
    results = []
    for t in targets:
        print("== dosrecon %s ==" % t)
        r = _run_one(t, outroot, shared)
        if r:
            results.append(r)
            c = r["counts"]
            print("  %d functions: code=%d data=%d stub=%d empty=%d"
                  % (r["nfuncs"], c.get("code", 0), c.get("data", 0),
                     c.get("stub", 0), c.get("empty", 0)))
            print("  Win95 real-mode layer candidates: %d" % len(r["win95_layer"]))

    # combined REPORT.md
    rep = os.path.join(outroot, "REPORT.md")
    with open(rep, "w") as f:
        f.write("# Win95 real-mode base reconstruction -- triage report\n\n")
        f.write("Reverse-engineered from the REAL corpus binaries (not the 6.0-derived tree).\n\n")
        for r in results:
            c = r["counts"]
            f.write("## %s\n\n%s\n\n" % (r["which"].upper(), r["desc"]))
            f.write("- source: `%s`\n" % os.path.basename(r["path"]))
            f.write("- layout: %s\n" % ("IO.SYS (body+SYSINIT, body_end=0x%06X)" % r["body_end"]
                                        if r["is_iosys"] else "normal MZ"))
            f.write("- functions: **%d** (code=%d, data=%d, stub=%d, empty=%d)\n"
                    % (r["nfuncs"], c.get("code", 0), c.get("data", 0),
                       c.get("stub", 0), c.get("empty", 0)))
            f.write("- **Win95 real-mode layer candidates: %d**\n\n" % len(r["win95_layer"]))
            if r["win95_layer"]:
                f.write("| addr | name | INT | strings |\n|---|---|---|---|\n")
                for s, nm, ints, strs in r["win95_layer"]:
                    f.write("| 0x%06X | %s | %s | %s |\n"
                            % (s, nm, " ".join(ints), "; ".join(strs).replace("|", "\\|")[:80]))
                f.write("\n")
    print("\nwrote %s" % rep)
    if do_build:
        _assemble(outroot, results)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
