#!/usr/bin/env python3
"""Tests for real-mode programs and boot media: the reader (formats/dos.py), unpacking, function
discovery, interrupt names, the description for the decompiler, and disk / CD images.
Run:  python tests/test_dos.py     (exit code 0 = all passed)

Checks that need the retail corpus, the built decompiler or an image file are SKIPPED when
those are not present."""
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.analyze import dos_functions      # noqa: E402
from codebreaker.base import corpus                # noqa: E402
from codebreaker.formats import dos, media         # noqa: E402
from codebreaker.symbols import dos_interrupts     # noqa: E402

RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _retail(name):
    path = corpus.retail(name) if os.path.isdir(corpus.root()) else None
    if not path:
        raise Skip("%s is not in the corpus" % name)
    return path


# --- the reader ------------------------------------------------------------------------------
def test_kinds():
    assert dos.kind(_retail("find.exe")) == "mz"
    assert dos.kind(_retail("more.com")) == "com"
    assert dos.kind(_retail("himem.sys")) == "driver"
    assert dos.kind(_retail("ne2000.dos")) == "driver"          # a driver linked as an .EXE
    assert dos.kind(_retail("winboot.sys")) == "kernel"
    assert dos.kind(_retail("country.sys")) is None             # data with a program's extension
    assert dos.kind(_retail("kernel32.dll")) is None            # a Windows module is not a DOS program


def test_com_and_driver_layout():
    p = dos.read(_retail("more.com"))
    assert p.base == dos.LOAD * 16 and p.entries == [(dos.LOAD * 16 + 0x100, "entry", "near")]
    assert p.image[:2] == b"\xCD\x20" and p.image[0x100:] == open(p.path, "rb").read()
    d = dos.read(_retail("himem.sys"))
    assert [x.name for x in d.devices] == ["XMSXXXX0"] and d.devices[0].character
    names = [n for _va, n, _dist in d.entries]
    assert names == ["XMSXXXX0_strategy", "XMSXXXX0_interrupt"], names


def test_relocations_are_applied():
    path = _retail("snapshot.exe")
    data = open(path, "rb").read()
    p = dos.read(path)
    nreloc, header = struct.unpack_from("<H", data, 6)[0], struct.unpack_from("<H", data, 8)[0] * 16
    table = struct.unpack_from("<H", data, 0x18)[0]
    assert len(p.relocations) == nreloc == 7
    for i in range(nreloc):
        off, seg = struct.unpack_from("<HH", data, table + 4 * i)
        stored = struct.unpack_from("<H", data, header + seg * 16 + off)[0]
        assert p.word(dos.LOAD * 16 + seg * 16 + off) == (stored + dos.LOAD) & 0xFFFF


def test_exepack_static_equals_emulated():
    path = _retail("find.exe")
    assert dos.packer(path) == "EXEPACK"
    data = open(path, "rb").read()
    h = dos._mz(data)
    module = bytes(data[h["header"]:h["size"]])
    static = dos.unpack_exepack(module, h["cs"], h["ip"])
    assert len(static[0]) == 12016 and static[2:4] == (0, 0x18DA), (len(static[0]), static[2:])
    ran = dos.unpack_emulated(module, [], h["cs"], h["ip"], h["ss"], h["sp"])
    assert ran[0] == static[0] and ran[1] == sorted(static[1]) and ran[2:] == static[2:]
    p = dos.read(path)
    assert p.unpacked == "static" and p.image == static[0] and p.entries[0][0] == dos.LOAD * 16 + 0x18DA


def test_unpacked_file():
    """A packed program written back unpacked reads as the same program, no longer packed."""
    p = dos.read(_retail("find.exe"))
    out = os.path.join(tempfile.mkdtemp(prefix="cb_unpack_"), "find.exe")
    with open(out, "wb") as f:
        f.write(dos.unpacked_file(p))
    q = dos.read(out)
    assert dos.packer(out) is None and q.unpacked is None
    assert q.image == p.image and q.relocations == p.relocations
    assert (q.cs, q.ip, q.ss, q.sp) == (p.cs, p.ip, p.ss, p.sp)


def test_kernel_image_parts():
    p = dos.read(_retail("winboot.sys"))
    assert [label for _lo, _n, label in p.spans] == ["bios and dos", "sysinit", "loader"]
    assert p.unpacked == "static"                               # SYSINIT is stored with EXEPACK
    names = [n for _va, n, _dist in p.entries]
    assert names[:3] == ["bios_entry", "loader_entry", "sysinit_entry"] and "CON_interrupt" in names
    assert [d.name for d in p.devices][:4] == ["CON", "AUX", "PRN", "CLOCK$"]
    assert p.image[dos.LOADER_SEGMENT * 16 - p.base:][:2] == b"MZ"


# --- functions and interrupts --------------------------------------------------------------------
def test_function_discovery():
    a = dos_functions.discover(dos.read(_retail("find.exe")))
    sure = [f for f in a.functions.values() if f.sure]
    assert len(sure) == 60 and {f.evidence for f in sure} == {"entry", "called"}, len(sure)
    assert all(f.evidence in ("pointer", "prologue", "follows") or not f.sure or f.evidence in ("entry", "called")
               for f in a.functions.values())
    assert len(a.functions) - len(sure) <= 3                    # guesses are few here, and marked
    entry = a.functions[dos.LOAD * 16 + 0x18DA]
    assert entry.name == "entry" and entry.evidence == "entry"
    c = a.census()
    assert c["total"] == 12016 and c["code"] > 4000 and c["guessed code"] < 100
    assert c["code"] + c["guessed code"] + c["not classified"] + c["jump tables"] == c["total"]
    named = [dos_interrupts.name(i.number, i.ah, i.al) for i in a.interrupts]
    assert len(named) >= 30 and sum(1 for n in named if n.exact) >= 25
    assert "DOS_Exit" in {n.name for n in named}


def test_installed_handler_is_found():
    a = dos_functions.discover(dos.read(_retail("dlchlp.exe")))
    handlers = [f for f in a.functions.values() if f.evidence == "vector"]
    assert len(handlers) == 1 and handlers[0].name.startswith("int_5C_handler") and handlers[0].distance == "far"


def test_interrupt_names():
    assert dos_interrupts.name(0x21, 0x4C).name == "DOS_Exit"
    assert dos_interrupts.name(0x21, 0x44, 0x00).name == "DOS_IoctlGetDeviceInformation"
    assert dos_interrupts.name(0x21, 0x44).name == "DOS_Ioctl"
    assert dos_interrupts.name(0x2F, 0x16, 0x00).name == "Multiplex_WindowsEnhancedModeCheck"
    assert dos_interrupts.name(0x33, 0x00, 0x03).name == "Mouse_GetPositionAndButtons"
    assert dos_interrupts.name(0x10, 0x0E).arguments.startswith("AL = character")
    assert not dos_interrupts.name(0x21).exact and not dos_interrupts.name(0x99).exact
    i = dos_functions.Interrupt(0x1234, 0x21, 0x09, None, None)
    text = dos_interrupts.annotate("x = swi(0x21);", [i])
    assert "DOS_WriteString" in text and "swi(0x21) /* DOS_WriteString */" in text


def test_description_for_the_decompiler():
    from codebreaker.decompile import decompiler, describe
    d = describe.describe_dos(_retail("find.exe"), tempfile.mkdtemp())
    lines = open(d.path).read().splitlines()
    assert lines[0] == "arch\tx86:LE:16:Real Mode:default" and lines[2] == "reg\tDS\t%x" % d.program.ds
    functions = [x.split("\t") for x in lines if x.startswith("F\t")]
    assert len(functions) == len(d.names) >= 50
    assert all(f[4] in ("__cdecl16far", "__cdecl16near", "__stdcall16far", "__stdcall16near") for f in functions)
    assert ["%x" % (dos.LOAD * 16 + 0x18DA), "entry"] in [f[1:3] for f in functions]
    if not decompiler.available():
        raise Skip("description written; native/decompiler is not built")
    three = sorted(d.names)[:3]
    out, order, _by_va, _err = decompiler.run(d.path, only=three)
    assert len(order) >= 2, order


# --- media ---------------------------------------------------------------------------------------
def test_boot_floppy():
    path = os.path.join(corpus.root(), "setup", "media", "boot.img")
    if not os.path.isfile(path):
        raise Skip("no boot floppy image in the corpus")
    v, = media.volumes(path)
    assert v.kind == "fat12" and v.label == "BOOT95B" and v.boot["system"] == "MSWIN4.1"
    files = {e.path: e for e in v.files()}
    assert len(files) == 31 and files["IO.SYS"].size == 214836
    assert v.read(files["IO.SYS"]) == open(_retail("winboot.sys"), "rb").read()
    out = tempfile.mkdtemp()
    assert media.extract(path, out, ["*.sys"]) == 8 and os.path.getsize(os.path.join(out, "HIMEM.SYS")) == 33191


def test_cd_image():
    path = os.path.join(os.path.dirname(corpus.design_dir()), "..", "win1.0", "Win1.0.iso")
    if not os.path.isfile(path):
        raise Skip("no small CD image on this machine")
    v = media.volumes(path)[0]
    files = [e for e in v.files() if not e.is_dir]
    assert v.kind == "iso9660" and len(files) == 35
    assert all(len(v.read(e)) == e.size for e in files[:5])


def test_not_an_image():
    p = os.path.join(tempfile.mkdtemp(), "x.img")
    with open(p, "wb") as f:
        f.write(b"this is not a disk" * 4000)
    try:
        media.volumes(p)
    except ValueError:
        return
    raise AssertionError("garbage was read as an image")


# --- the native DOS runner (build/native_dos.py) vs DOSBox -----------------------------------
# A tiny 16-bit source, assembled with the real MASM.EXE on CodeBreaker's own Unicorn executive
# (no DOSBox) and -- when DOSBox is also set up -- proven byte-identical to the same tool there.
_TINY_ASM = ("_TEXT\tSEGMENT BYTE PUBLIC 'CODE'\n\tASSUME CS:_TEXT\n\tPUBLIC foo\n"
             "foo\tPROC NEAR\n\tmov\tax, 1\n\tadd\tax, bx\n\tret\nfoo\tENDP\n"
             "_TEXT\tENDS\n\tEND\n")


def _masm_set_up():
    from codebreaker.base import settings
    root = settings.get("masm")
    return bool(root and os.path.isfile(os.path.join(root, "BIN", "MASM.EXE")))


def test_native_dos_switch():
    """CB_NATIVE_DOS and CB_NO_DOSBOX decide the runner; the routine boundary is enforceable."""
    from codebreaker.build import native_dos
    from codebreaker.base import settings
    saved = {k: os.environ.get(k) for k in ("CB_NATIVE_DOS", "CB_NO_DOSBOX")}
    try:
        os.environ.pop("CB_NO_DOSBOX", None)
        os.environ["CB_NATIVE_DOS"] = "0"
        assert native_dos.enabled("masm") is False
        os.environ["CB_NATIVE_DOS"] = "1"
        assert native_dos.enabled("masm") is native_dos.available()
        os.environ.pop("CB_NATIVE_DOS", None)                  # "auto": the proven tools (MASM, TNT ML)
        assert native_dos.enabled("masm") is native_dos.available()
        assert native_dos.enabled("ml") is native_dos.available()
        assert native_dos.enabled("cl") is False                # (not proven: DOSBox)
        # CB_NO_DOSBOX makes a byte-match spot-check refuse rather than silently spawn the emulator
        os.environ["CB_NO_DOSBOX"] = "1"
        assert settings.dosbox_x() == ""
        try:
            settings.dosbox_x(require=True)
            raise AssertionError("dosbox_x(require=True) did not refuse under CB_NO_DOSBOX")
        except RuntimeError:
            pass
    finally:
        for k, v in saved.items():
            os.environ.pop(k, None)
            if v is not None:
                os.environ[k] = v


def test_native_masm_assembles():
    """The real MASM.EXE on our own emulator turns a tiny proc into a valid OMF object."""
    if not _masm_set_up():
        raise Skip("MASM is not set up (settings: masm)")
    try:
        import unicorn  # noqa: F401
    except Exception:
        raise Skip("unicorn is not installed")
    from codebreaker.build import native_dos, msc6
    out = native_dos.assemble(_TINY_ASM, stem="T")
    assert out.get("obj") and out["obj"][0] == 0x80, "no OMF object was produced"
    code, _mask = msc6.object_code(out["obj"])
    assert code == bytes.fromhex("b8010003c3c3"), code.hex()   # mov ax,1 / add ax,bx / ret


def test_native_masm_matches_dosbox():
    """One sample assembled both ways is byte-identical (the native runner's acceptance gate)."""
    if not _masm_set_up():
        raise Skip("MASM is not set up (settings: masm)")
    from codebreaker.base import settings
    if not (settings.get("dosbox_x") and os.path.isfile(settings.get("dosbox_x"))):
        raise Skip("DOSBox-X is not set up (settings: dosbox_x)")
    import importlib
    from codebreaker.build import masm as masm_mod
    saved = os.environ.get("CB_NATIVE_DOS")
    os.environ.pop("CB_NO_DOSBOX", None)
    try:
        os.environ["CB_NATIVE_DOS"] = "1"
        importlib.reload(masm_mod)
        native = masm_mod.assemble(_TINY_ASM, stem="T").get("obj")
        os.environ["CB_NATIVE_DOS"] = "0"
        importlib.reload(masm_mod)
        dosbox = masm_mod.assemble(_TINY_ASM, stem="T").get("obj")
        assert native and dosbox and native == dosbox, (
            "native and DOSBox .OBJ differ (%s vs %s bytes)" % (len(native or b""), len(dosbox or b"")))
    finally:
        os.environ.pop("CB_NATIVE_DOS", None)
        if saved is not None:
            os.environ["CB_NATIVE_DOS"] = saved
        importlib.reload(masm_mod)


# --- the DPMI host (exact/emu/dpmi.py) -------------------------------------------------------
# A tiny 16-bit protected-mode DPMI client, built by hand as a raw MZ and run through
# dos.run(dpmi=True): it detects the host (INT 2Fh/1687h), switches to protected mode, allocates and
# bases an LDT descriptor (INT 31h 0000/0007/0008) and writes through it, reflects an INT 21h file
# create+write to real mode (INT 31h 0300), writes the console with a direct protected-mode INT 21h,
# and exits. No DOSBox and no external tool -- this is the host's own end-to-end proof.
def _dpmi_client_exe():
    code = bytearray()

    def emit(b):
        code.extend(b)
    emit(b"\x0E\x1F")                            # push cs ; pop ds  (our data lives in the code segment)
    emit(b"\xB8\x87\x16\xCD\x2F")                # mov ax,1687h ; int 2Fh  -> ES:DI = switch entry
    emit(b"\x89\x3E\x00\x00"); dioff = len(code) - 2         # mov [DI_VAR],di
    emit(b"\x8C\x06\x00\x00"); esoff = len(code) - 2         # mov [ES_VAR],es
    emit(b"\x31\xC0")                            # xor ax,ax  (AX bit0=0 -> 16-bit client)
    emit(b"\xFF\x1E\x00\x00"); calloff = len(code) - 2       # call far [DI_VAR]  -> protected mode
    emit(b"\x31\xC0\xB9\x01\x00\xCD\x31")        # xor ax,ax ; mov cx,1 ; int 31h  (0000: alloc descriptor)
    emit(b"\xA3\x00\x00"); seloff = len(code) - 2            # mov [SEL_VAR],ax
    emit(b"\xB8\x06\x00\x8C\xDB\xCD\x31")        # mov ax,6 ; mov bx,ds ; int 31h  (0006: DS base -> CX:DX)
    emit(b"\x83\xC2\x40")                        # add dx,40h  (point the new selector 0x40 bytes in)
    emit(b"\x8B\x1E\x00\x00"); seloff2 = len(code) - 2       # mov bx,[SEL_VAR]
    emit(b"\xB8\x07\x00\xCD\x31")                # mov ax,7 ; int 31h  (0007: set base = CX:DX)
    emit(b"\xB8\x08\x00")                        # mov ax,8
    emit(b"\x8B\x1E\x00\x00"); seloff3 = len(code) - 2       # mov bx,[SEL_VAR]
    emit(b"\x31\xC9\xBA\xFF\xFF\xCD\x31")        # xor cx,cx ; mov dx,0FFFFh ; int 31h  (0008: set limit)
    emit(b"\x8E\x06\x00\x00"); seloff4 = len(code) - 2       # mov es,[SEL_VAR]
    emit(b"\x26\xC7\x06\x00\x00\x4B\x4A")        # mov word es:[0],'JK'  (write through the new selector)
    emit(b"\x8C\xD8\x8E\xC0")                    # mov ax,ds ; mov es,ax  (ES=DS for the call structs)
    emit(b"\xB8\x06\x00\x8C\xDB\xCD\x31")        # mov ax,6 ; mov bx,ds ; int 31h  (DS base -> CX:DX)
    emit(b"\x89\xD0\xC1\xE8\x04\xC1\xE1\x0C\x09\xC8")        # ax = (dx>>4)|(cx<<12)  (real-mode seg of DS)
    emit(b"\xA3\x00\x00"); realsegoff = len(code) - 2        # mov [REALSEG],ax
    # --- reflect INT 21h AH=3Ch (create OUT.TXT) via INT 31h 0300 ---
    emit(b"\xBF\x00\x00"); rmcs_di = len(code) - 2           # mov di,RMCS
    emit(b"\xB9\x19\x00\x31\xC0\xF3\xAB")        # mov cx,25 ; xor ax,ax ; rep stosw  (zero the structure)
    emit(b"\xC7\x06\x00\x00\x00\x3C"); rmcs_eax = len(code) - 4      # mov word [RMCS+1Ch],3C00h (EAX)
    emit(b"\xC7\x06\x00\x00\x00\x00"); rmcs_dx = len(code) - 4; dxf = len(code) - 2   # [RMCS+14h]=FNAME (DX)
    emit(b"\xA1\x00\x00"); rs2 = len(code) - 2               # mov ax,[REALSEG]
    emit(b"\xA3\x00\x00"); rmcs_ds = len(code) - 2           # mov [RMCS+24h],ax (DS)
    emit(b"\xBF\x00\x00"); rmcs_di2 = len(code) - 2          # mov di,RMCS
    emit(b"\xB8\x00\x03\xBB\x21\x00\x31\xC9\xCD\x31")        # mov ax,0300 ; mov bx,0021 ; xor cx,cx ; int 31h
    emit(b"\xA1\x00\x00"); rmcs_eax_rd = len(code) - 2       # mov ax,[RMCS+1Ch]  (file handle)
    emit(b"\xA3\x00\x00"); handleoff = len(code) - 2         # mov [HANDLE],ax
    # --- reflect INT 21h AH=40h (write "hi\n") via INT 31h 0300 ---
    emit(b"\xBF\x00\x00"); rmcs_di3 = len(code) - 2          # mov di,RMCS
    emit(b"\xB9\x19\x00\x31\xC0\xF3\xAB")        # zero the structure
    emit(b"\xC7\x06\x00\x00\x00\x40"); rmcs_eax2 = len(code) - 4     # [RMCS+1Ch]=4000h (EAX)
    emit(b"\xA1\x00\x00"); handleoff2 = len(code) - 2        # mov ax,[HANDLE]
    emit(b"\xA3\x00\x00"); rmcs_ebx = len(code) - 2          # mov [RMCS+10h],ax (EBX = handle)
    emit(b"\xC7\x06\x00\x00\x03\x00"); rmcs_ecx = len(code) - 4      # [RMCS+18h]=3 (ECX = count)
    emit(b"\xC7\x06\x00\x00\x00\x00"); rmcs_dx2 = len(code) - 4; dxm = len(code) - 2   # [RMCS+14h]=MSG (DX)
    emit(b"\xA1\x00\x00"); rs3 = len(code) - 2               # mov ax,[REALSEG]
    emit(b"\xA3\x00\x00"); rmcs_ds2 = len(code) - 2          # mov [RMCS+24h],ax (DS)
    emit(b"\xBF\x00\x00"); rmcs_di4 = len(code) - 2          # mov di,RMCS
    emit(b"\xB8\x00\x03\xBB\x21\x00\x31\xC9\xCD\x31")        # int 31h 0300 (write)
    # --- direct protected-mode INT 21h AH=09h (print "OK$"), then exit AX=4C00 ---
    emit(b"\xBA\x00\x00"); msg2dx = len(code) - 2            # mov dx,MSG2
    emit(b"\xB4\x09\xCD\x21")                    # mov ah,9 ; int 21h
    emit(b"\xB8\x00\x4C\xCD\x21")                # mov ax,4C00h ; int 21h

    data = (len(code) + 1) & ~1
    while len(code) < data:
        code.append(0)
    DI_VAR, ES_VAR, SEL_VAR, REALSEG, HANDLE = data, data + 2, data + 4, data + 6, data + 8
    RMCS = data + 10
    FNAME, MSG, MSG2 = RMCS + 0x34, RMCS + 0x34 + 8, RMCS + 0x34 + 12
    end = MSG2 + 4
    while len(code) < end:
        code.append(0)
    code[FNAME:FNAME + 8] = b"OUT.TXT\x00"
    code[MSG:MSG + 3] = b"hi\n"
    code[MSG2:MSG2 + 3] = b"OK$"
    for idx, val in ((dioff, DI_VAR), (esoff, ES_VAR), (calloff, DI_VAR), (seloff, SEL_VAR),
                     (seloff2, SEL_VAR), (seloff3, SEL_VAR), (seloff4, SEL_VAR), (realsegoff, REALSEG),
                     (rmcs_di, RMCS), (rmcs_eax, RMCS + 0x1C), (rmcs_dx, RMCS + 0x14), (dxf, FNAME),
                     (rs2, REALSEG), (rmcs_ds, RMCS + 0x24), (rmcs_di2, RMCS), (rmcs_eax_rd, RMCS + 0x1C),
                     (handleoff, HANDLE), (rmcs_di3, RMCS), (rmcs_eax2, RMCS + 0x1C), (handleoff2, HANDLE),
                     (rmcs_ebx, RMCS + 0x10), (rmcs_ecx, RMCS + 0x18), (rmcs_dx2, RMCS + 0x14), (dxm, MSG),
                     (rs3, REALSEG), (rmcs_ds2, RMCS + 0x24), (rmcs_di4, RMCS), (msg2dx, MSG2)):
        struct.pack_into("<H", code, idx, val & 0xFFFF)
    image = bytes(code)
    sp = ((len(image) + 0x200) & ~1) + 0x100
    pages = (32 + len(image) + sp + 0x100 + 511) // 512
    mz = bytearray(32)
    struct.pack_into("<2sHHHHHHHHHHH", mz, 0, b"MZ", (32 + len(image)) % 512, pages, 0, 2, 0x10, 0xFFFF,
                     0, sp, 0, 0, 0)
    struct.pack_into("<H", mz, 0x18, 0x40)       # relocation-table offset (no relocations)
    return bytes(mz) + image


def test_dpmi_host_protected_mode():
    try:
        import unicorn  # noqa: F401
    except Exception:
        raise Skip("unicorn is not installed")
    from codebreaker.exact.emu import dos
    work = tempfile.mkdtemp(prefix="cb_dpmi_")
    with open(os.path.join(work, "T.EXE"), "wb") as f:
        f.write(_dpmi_client_exe())
    saved = os.environ.get("DOSRUN_FAST_LIMIT")
    os.environ["DOSRUN_FAST_LIMIT"] = "10"
    try:
        r = dos.run(os.path.join(work, "T.EXE"), work, "", fast=False, dpmi=True)
    finally:
        os.environ.pop("DOSRUN_FAST_LIMIT", None)
        if saved is not None:
            os.environ["DOSRUN_FAST_LIMIT"] = saved
    assert r.exit == 0, "client exited %r" % (r.exit,)
    assert bytes(r.console) == b"OK", bytes(r.console)                 # direct protected-mode INT 21h/09h
    out = os.path.join(work, "OUT.TXT")
    assert os.path.isfile(out) and open(out, "rb").read() == b"hi\n"   # reflected INT 21h create+write


# --- the DOS memory arena (exact/emu/dos.py keeps real MCBs in guest RAM) -------------------------
# A hand-built program that does what a DOS extender's loader does: shrink its block (4Ah), allocate
# (48h), ask for too much (error 8, BX = the largest), find the first MCB through the list of lists
# (52h, ES:[BX-2]), walk the chain printing each header's signature, check the last block ends at the
# PSP's memory top, free (49h), free a non-block (error 9), and see the freed block coalesce.
def _mcb_walker_exe():
    code, labels, fix = bytearray(), {}, []

    def e(b):
        code.extend(b)

    def jcc(op, label):                          # a short jump to a label (resolved below)
        code.extend(bytes([op, 0]))
        fix.append((len(code) - 1, label))

    def label(name):
        labels[name] = len(code)
    e(b"\xBB\x00\x01\xB4\x4A\xCD\x21"); jcc(0x72, "fail")          # mov bx,100h ; mov ah,4Ah ; int 21h ; jc
    e(b"\xBB\x10\x00\xB4\x48\xCD\x21"); jcc(0x72, "fail")          # mov bx,10h ; mov ah,48h ; int 21h ; jc
    e(b"\x8B\xE8")                                                  # mov bp,ax   (block A)
    e(b"\xBB\xFF\xFF\xB4\x48\xCD\x21"); jcc(0x73, "fail")          # mov bx,0FFFFh ; 48h ; jnc fail
    e(b"\x3D\x08\x00"); jcc(0x75, "fail")                           # cmp ax,8 ; jne fail
    e(b"\x8B\xF3")                                                  # mov si,bx   (the largest)
    e(b"\xB4\x52\xCD\x21\x26\x8B\x47\xFE")                          # mov ah,52h ; int 21h ; mov ax,es:[bx-2]
    label("walk")
    e(b"\x8E\xC0\x26\x8A\x16\x00\x00\xB4\x02\xCD\x21")              # mov es,ax ; mov dl,es:[0] ; ah=2 ; int 21h
    e(b"\x80\xFA\x5A"); jcc(0x74, "done")                           # cmp dl,'Z' ; je done
    e(b"\x80\xFA\x4D"); jcc(0x75, "fail")                           # cmp dl,'M' ; jne fail
    e(b"\x8C\xC0\x26\x03\x06\x03\x00\x40"); jcc(0xEB, "walk")       # ax = es + es:[3] + 1 ; jmp walk
    label("done")
    e(b"\x8C\xC0\x26\x03\x06\x03\x00\x40\x8B\xF8")                  # di = end of the last block
    e(b"\xB4\x62\xCD\x21\x8E\xC3\x26\x3B\x3E\x02\x00"); jcc(0x75, "fail")   # == PSP:[2] (memory top)
    e(b"\x8E\xC5\xB4\x49\xCD\x21"); jcc(0x72, "fail")              # mov es,bp ; 49h ; jc fail
    e(b"\xB8\x00\x50\x8E\xC0\xB4\x49\xCD\x21"); jcc(0x73, "fail")  # es=5000h (not a block) ; 49h ; jnc fail
    e(b"\x3D\x09\x00"); jcc(0x75, "fail")                           # cmp ax,9 ; jne fail
    e(b"\xBB\xFF\xFF\xB4\x48\xCD\x21\x2B\xDE\x83\xFB\x11"); jcc(0x75, "fail")   # largest now = old + 10h + 1
    e(b"\xB8\x00\x4C\xCD\x21")                                      # exit 0
    label("fail")
    e(b"\xB8\x01\x4C\xCD\x21")                                      # exit 1
    for at, name in fix:
        code[at] = (labels[name] - (at + 1)) & 0xFF
    sp = (len(code) + 0x100) & ~1
    mz = bytearray(32)
    struct.pack_into("<2sHHHHHHHHHHH", mz, 0, b"MZ", (32 + len(code)) % 512, (32 + len(code) + 511) // 512, 0, 2,
                     0x10, 0xFFFF, 0, sp, 0, 0, 0)
    struct.pack_into("<H", mz, 0x18, 0x1C)
    return bytes(mz) + bytes(code)


def test_mcb_arena_walk():
    try:
        import unicorn  # noqa: F401
    except Exception:
        raise Skip("unicorn is not installed")
    from codebreaker.exact.emu import dos as emu
    import re
    work = tempfile.mkdtemp(prefix="cb_mcb_")
    try:
        with open(os.path.join(work, "W.EXE"), "wb") as f:
            f.write(_mcb_walker_exe())
        for psp in (None, 0x1000):                # the default load and a high one (DOS's own block over the gap)
            r = emu.run(os.path.join(work, "W.EXE"), work, "", psp=psp, fast=True)
            assert r.exit == 0, "the walker failed a check (psp %r, console %r)" % (psp, r.console)
            assert re.fullmatch(rb"M{3,}Z", bytes(r.console)), bytes(r.console)
    finally:
        import shutil
        shutil.rmtree(work, ignore_errors=True)


# --- TNT ML (the Win95 DDK's Phar Lap ML.EXE) on the executive + DPMI host --------------------------
_TINY32 = ("\t.386p\n_TEXT\tSEGMENT DWORD PUBLIC USE32 'CODE'\n\tPUBLIC foo\nfoo\tPROC NEAR\n"
           "\tmov\teax, 1\n\tadd\teax, ebx\n\tret\nfoo\tENDP\n_TEXT\tENDS\n\tEND\n")


def _ml_set_up():
    from codebreaker.build import native_dos
    try:
        native_dos.ml_files()
    except native_dos.NativeDosError as e:
        raise Skip(str(e))
    try:
        import unicorn  # noqa: F401
    except Exception:
        raise Skip("unicorn is not installed")


def test_native_tnt_ml_assembles():
    """The 32-bit DOS-extended ML.EXE runs to completion on our own CPU and makes an object."""
    _ml_set_up()
    from codebreaker.build import native_dos
    made, console = native_dos.ml("/c /Fom.obj m.asm", {"m.asm": _TINY32}, ["m.obj"])
    obj = made.get("m.obj")
    assert obj and obj[0] == 0x80, "no OMF object (console %r)" % console[-300:]
    assert bytes.fromhex("b801000000" "03c3" "c3") in obj, obj.hex()   # mov eax,1 / add eax,ebx / ret
    made, console = native_dos.ml("-coff -c -Fom.obj m.asm", {"m.asm": _TINY32}, ["m.obj"])
    assert made.get("m.obj") and made["m.obj"][:2] == b"\x4c\x01", "no COFF object (console %r)" % console[-300:]


def test_native_tnt_ml_matches_dosbox():
    """The same source through TNT ML under DOSBox-X: byte-identical objects (OMF and COFF)."""
    _ml_set_up()
    from codebreaker.base import settings
    if not (settings.get("dosbox_x") and os.path.isfile(settings.get("dosbox_x"))):
        raise Skip("DOSBox-X is not set up (settings: dosbox_x)")
    os.environ.pop("CB_NO_DOSBOX", None)
    import shutil
    from codebreaker.build import native_dos, dos_tools
    tc = tempfile.mkdtemp(prefix="cb_tc_")
    try:
        for n, p in native_dos.ml_files().items():
            shutil.copy(p, os.path.join(tc, n))
        for cmd in ("/c /Fom.obj m.asm", "-coff -c -Fom.obj m.asm"):
            made, _con = native_dos.ml(cmd, {"m.asm": _TINY32}, ["m.obj"])
            ok, out, work = dos_tools.run({"c": tc}, ["c:\\ml.exe %s > asm.txt" % cmd], ["m.obj", "asm.txt"],
                                          files={"m.asm": _TINY32}, timeout=180)
            if work:
                shutil.rmtree(work, ignore_errors=True)
            a, b = made.get("m.obj") or b"", out.get("m.obj") or b""
            if cmd.startswith("-coff"):                       # (the COFF header's TimeDateStamp is the clock)
                a, b = a[:4] + a[8:], b[:4] + b[8:]
            assert b and a == b, (
                "%s: native and DOSBox objects differ (%d vs %d bytes)" % (cmd, len(made.get("m.obj") or b""),
                                                                           len(out.get("m.obj") or b"")))
    finally:
        shutil.rmtree(tc, ignore_errors=True)


def main():
    for fn in (test_kinds, test_com_and_driver_layout, test_relocations_are_applied,
               test_exepack_static_equals_emulated, test_unpacked_file, test_kernel_image_parts, test_function_discovery,
               test_installed_handler_is_found, test_interrupt_names, test_description_for_the_decompiler,
               test_boot_floppy, test_cd_image, test_not_an_image,
               test_native_dos_switch, test_native_masm_assembles, test_native_masm_matches_dosbox,
               test_dpmi_host_protected_mode, test_mcb_arena_walk, test_native_tnt_ml_assembles,
               test_native_tnt_ml_matches_dosbox):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
