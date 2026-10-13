#!/usr/bin/env python3
"""Tests for names: decorated C++ names, imports by number, Windows prototypes, runtime-library
functions, and names that follow a body from module to module.
Run:  python tests/test_symbols.py     (exit code 0 = all passed)

Checks that need the retail corpus or the OpenWatcom headers are SKIPPED without them."""
import os
import struct
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.base import corpus                                   # noqa: E402
from codebreaker.build import watcom                                  # noqa: E402
from codebreaker.store import functions as F                          # noqa: E402
from codebreaker.symbols import demangle, names, ordinals, prototypes, runtime_library   # noqa: E402

RESULTS = []


class Skip(Exception):
    pass


def _corpus(*modules):
    if not os.path.isdir(corpus.root()) or not all(corpus.retail(m) for m in modules):
        raise Skip("retail corpus not present")
    return [corpus.retail(m) for m in modules]


def test_demangle():
    d = demangle.declaration
    assert d("?AddRef@CFoo@@UAGKXZ") == "public: virtual unsigned long __stdcall CFoo::AddRef(void)"
    assert d("??0CString@@QAE@PBD@Z") == "public: __thiscall CString::CString(char const *)"
    assert d("??1CString@@QAE@XZ") == "public: __thiscall CString::~CString(void)"
    assert d("??BCString@@QBEPBDXZ") == "public: __thiscall CString::operator char const *(void)const"
    assert d("?classCObject@CObject@@2UCRuntimeClass@@A") == "public: static struct CRuntimeClass CObject::classCObject"
    assert d("??_7CObject@@6B@") == "const CObject::`vftable'"
    assert d("?x@@YAXP6AHH@ZPAY03H@Z") == "void __cdecl x(int (__cdecl*)(int),int (*)[4])"
    assert d("?Create@CButton@@QAEHPBDKABUtagRECT@@PAVCWnd@@I@Z") == \
        "public: int __thiscall CButton::Create(char const *,unsigned long,struct tagRECT const &,class CWnd *,unsigned int)"
    # 16-bit: near and far
    assert d("??0CString@@REC@XZ") == "public: __far __pascal CString::CString(void)"
    assert d("??H@ZC?EVCString@@AFV0@PFD@Z") == \
        "class CString __far __pascal operator+(class CString const __far &,char const __far *)"
    assert demangle.short("?AddRef@CFoo@@UAGKXZ") == "CFoo::AddRef"
    assert d("?not a name") is None and d("plain") is None and demangle.short("") is None
    assert demangle.plain("_LZRead@12") == ("LZRead", "__stdcall", 12)
    assert demangle.plain("@Fast@8") == ("Fast", "__fastcall", 8) and demangle.plain("_cdecl_name")[0] == "cdecl_name"
    assert demangle.readable("_LZRead@12") == "LZRead" and demangle.readable("?bad") == "?bad"
    # what the name says about the stack: `this` and a class returned by value are hidden arguments
    assert demangle.stack_bytes("?AddRef@CFoo@@UAGKXZ") == 4
    assert demangle.stack_bytes("?GetAt@CStringArray@@QBE?AVCString@@H@Z") == 8
    assert demangle.stack_bytes("?x@@YAXH@Z") is None            # the caller removes them
    p = demangle.parse("?Create@CButton@@QAEHPBDKABUtagRECT@@PAVCWnd@@I@Z")
    assert p["class"] == "CButton" and p["returns"] == "int" and len(p["parameters"]) == 5 and p["access"] == "public"


def test_definition_file_and_library():
    d = tempfile.mkdtemp()
    p = os.path.join(d, "thing.def")
    with open(p, "w") as f:
        f.write("LIBRARY THING\nEXPORTS\n    ??0CFoo@@QAE@XZ @ 256 NONAME ; a comment\n    Second=Internal @ 257\n")
    assert ordinals.read_definition_file(p) == {"THING": {256: "??0CFoo@@QAE@XZ", 257: "Second"}}
    # a 32-bit import library: one member that imports THING.7 by number as _Seven@4
    member = struct.pack("<HHHHIIHH", 0, 0xFFFF, 0, 0x14C, 0, 20, 7, 0) + b"_Seven@4\0THING.dll\0"
    head = b"imp/".ljust(16) + b"0".ljust(12) + b"0".ljust(6) + b"0".ljust(6) + b"0".ljust(8) + \
        str(len(member)).encode().ljust(10) + b"`\n"
    lib = os.path.join(d, "thing.lib")
    with open(lib, "wb") as f:
        f.write(b"!<arch>\n" + head + member)
    assert ordinals.read_import_library(lib) == {"THING": {7: "_Seven@4"}}


def test_ordinals():
    _corpus("user.exe", "krnl386.exe", "wsock32.dll")
    assert ordinals.linker_name("USER", 91) == "GETDLGITEM" and ordinals.source("USER", 91) == "user.exe"
    assert ordinals.name("KERNEL", 5).upper() == "LOCALALLOC"       # KERNEL is krnl386.exe
    assert ordinals.name("WSOCK32", 3) == "closesocket" and ordinals.identifier("WSOCK32", 3) == "closesocket"
    assert ordinals.name("USER", 60000) is None and ordinals.name("NO_SUCH_MODULE", 1) is None
    assert len(ordinals.table("USER")) > 400
    if corpus.retail("mfc30.dll"):                                  # unnamed in the DLL: from its definition file
        raw = ordinals.name("MFC30", 800)
        assert raw.startswith("?Create@CButton@@") and ordinals.identifier("MFC30", 800) is None
        assert demangle.short(raw) == "CButton::Create" and ordinals.source("MFC30", 800) == "MFC30.DEF"
    if corpus.retail("commdlg.dll"):
        imp = ordinals.imports16(corpus.retail("commdlg.dll"))
        assert len(imp) > 100 and all(imp.values()), [k for k, v in imp.items() if not v]


def test_prototypes():
    d = tempfile.mkdtemp()
    p = os.path.join(d, "x.h")
    with open(p, "w") as f:
        f.write("/* c */\n#define X 1\nWINUSERAPI HWND WINAPI GetThing( HWND hWnd, int nIndex );\n"
                "int WINAPIV Printf( LPSTR, LPCSTR, ... );\nSTDAPI_(ULONG) Count( void );\n"
                "BOOL CALLBACK Each( WNDENUMPROC lpfn, void (*pfn)(int, int) );\n")
    got = prototypes.read_header(p)
    assert got["GetThing"]["returns"] == "HWND" and got["GetThing"]["convention"] == "stdcall"
    assert got["GetThing"]["parameters"] == [["HWND", "hWnd"], ["int", "nIndex"]]
    assert got["Printf"]["convention"] == "cdecl" and got["Printf"]["parameters"][-1] == ["...", ""]
    assert got["Count"]["returns"] == "ULONG" and got["Count"]["parameters"] == []
    assert len(got["Each"]["parameters"]) == 2
    if not os.path.isdir(os.path.join(watcom.openwatcom_root() or "", "h", "nt")):
        raise Skip("OpenWatcom headers not present")
    assert prototypes.of("GetDlgItem")["returns"] == "HWND" and len(prototypes.of("GetDlgItem")["parameters"]) == 2
    assert prototypes.of("GETDLGITEM", 16)["header"] == "win16.h"
    assert prototypes.spelling("MESSAGEBOX") == "MessageBox" and prototypes.spelling("NO_SUCH_API") == "NO_SUCH_API"
    assert prototypes.declaration("LZRead", 16).startswith("int ") and prototypes.of("NoSuchFunction") is None


def _object(code, reloc_at):
    """A 32-bit object file with one function `_probe` whose bytes at reloc_at..+4 the linker fills in."""
    section = b".text\0\0\0" + struct.pack("<IIIIIIHHI", 0, 0, len(code), 20 + 40, 20 + 40 + len(code), 0, 1, 0, 0x60000020)
    reloc = struct.pack("<IIH", reloc_at, 0, 0x14)
    symptr = 20 + 40 + len(code) + 10
    symbol = b"_probe\0\0" + struct.pack("<IhHBB", 0, 1, 0x20, 2, 0)
    return struct.pack("<HHIIIHH", 0x14C, 1, 0, symptr, 1, 0, 0) + section + code + reloc + symbol + struct.pack("<I", 4)


def test_signatures():
    code = bytes.fromhex("558bec8b450803450c8b4d1033c8e811223344d1e083c0075dc20c00")
    obj = os.path.join(tempfile.mkdtemp(), "probe.obj")
    with open(obj, "wb") as f:
        f.write(_object(code, 17))
    bits, found = runtime_library.library_functions(obj)
    assert bits == 32 and [(n, c, sorted(m)) for n, c, m in found] == [("_probe", code, [17, 18, 19, 20])], found
    sig = runtime_library.signature(code, found[0][2])
    linked = code[:17] + b"\xAA\xBB\xCC\xDD" + code[21:]                 # the same function where a linker put it
    assert runtime_library.signature(linked, {17, 18, 19, 20}) == sig
    assert runtime_library.signature(code[:6] + b"\x09" + code[7:], {17, 18, 19, 20}) != sig
    assert runtime_library.signature(b"\x55\x8b\xec\x5d\xc3", set()) is None    # nothing to tell it by
    assert runtime_library.substance(32, code, len(code)) >= 4
    assert runtime_library.substance(16, bytes.fromhex("8cd89045558bec1e8ed833c08d66fe1f5d4dcb90"), 20) == 0
    assert runtime_library._c_name("__allmul") == "_allmul" and runtime_library._c_name("_f@8") == "f"
    # the shipped signatures: the 32-bit and 16-bit runtime both there, and they find themselves
    table = runtime_library._signatures()
    assert len(table["32"]) > 500 and len(table["16"]) > 500
    lead, entries = next(iter(table["32"].items()))
    assert all(len(e) == 5 for e in entries)


def test_identify():
    crt, lz, hlp = _corpus("crtdll.dll", "lzexpand.dll", "winhlp32.exe")
    found = runtime_library.identify(crt)
    by_signature = [n for n, lib in found.values() if "this module" not in lib]
    assert len(by_signature) > 150, len(by_signature)                   # 300 today
    assert sum(1 for _n, lib in found.values() if "this module" in lib) > 200
    linked_in = runtime_library.identify(hlp)                           # a program with the C runtime linked in
    assert {"_cinit", "_amsg_exit", "_except_handler3"} <= {n for n, _lib in linked_in.values()}, sorted(linked_in.values())[:20]
    assert all("C runtime" in lib for _n, lib in linked_in.values()), set(lib for _n, lib in linked_in.values())
    info = runtime_library.compiler(crt, found)
    assert info["linker"] == "2.50" and "Visual C++ 2" in info["compiler"], info
    assert runtime_library.compiler(lz)["linker"].startswith("5."), runtime_library.compiler(lz)
    assert all(isinstance(a, int) for a in runtime_library.identify(lz))


def test_store_identified_and_bodies():
    fs = F.open(os.path.join(tempfile.mkdtemp(), "functions.sqlite"))
    fs.add_module("m1", "A.DLL", "x", "pe")
    fs.add_module("m2", "B.DLL", "y", "pe")
    fs.add_functions("m1", ["10", "20", "30", "40"])
    fs.record("m1", "10", "decompiler", "1", "s", "f10", "int f10(){}", "p", "equivalent", limits="coverage=90")
    fs.record("m1", "20", "decompiler", "1", "s", "f20", "int f20(){}", "p", "divergent", reason="ret-differs")
    fs.record("m1", "30", "decompiler", "1", "s", "f30", "int f30(){}", "p", "divergent", reason="ret-differs")
    plain = fs.tally("p", "m1")
    fs.set_identified("m1", {"10": ("memcpy", "C runtime"), "20": ("strlen", "C runtime"), "40": ("_chkstk", "C runtime")})
    assert fs.identified("m1")["20"] == ("strlen", "C runtime")
    assert fs.tally("p", "m1") == plain                                 # the verdicts themselves are untouched
    s = fs.scores("p", "m1")
    assert s == {"verified": 1, "weak": 0, "identified": 2, "unverified": 1, "not-judged": 0, "no-compile": 0,
                 "no-source": 0}, s                                     # verified stays verified
    assert sum(s.values()) == sum(plain.values())
    fs.set_identified("m1", {})
    assert fs.scores("p", "m1")["identified"] == 0 and fs.scores("p", "m1")["unverified"] == 2
    # bodies: a name follows the body
    fs.set_bodies("m1", {"10": ("aaaa", "StrCopy", "export"), "20": ("bbbb", None, None), "30": ("cccc", None, None)})
    fs.set_bodies("m2", {"70": ("aaaa", None, None), "80": ("bbbb", None, None), "90": ("dddd", None, None)})
    assert fs.names_from_bodies("m2") == {"70": ("StrCopy", "the same body in a.dll")}
    fs.name_body("bbbb", "ParseLine")
    assert fs.names_from_bodies("m1")["20"] == ("ParseLine", "named by hand")
    assert fs.names_from_bodies("m2")["80"] == ("ParseLine", "named by hand")
    assert fs.shared_bodies() == (6, 4, 2, 4)


def test_names_of_a_module():
    (lz,) = _corpus("lzexpand.dll")
    fs = F.open(os.path.join(tempfile.mkdtemp(), "functions.sqlite"))
    got = names.of_module(lz, fs)
    sources = {s.split(" (")[0] for _a, _k, _n, s in got["functions"]}
    assert "export" in sources and len(got["functions"]) >= 10
    assert any(n.upper() == "LZOPENFILE" for _a, _k, n, _s in got["functions"])
    assert got["imports"] and all(name and "KERNEL." not in name for m, _o, name, _s in got["imports"] if m == "KERNEL")
    bodies = names.bodies(lz)
    assert bodies and fs.bodies(corpus.sha(lz))                         # recorded on first use
    address = sorted(bodies)[0]
    digest = names.set_name(lz, address, "FirstBody", fs)
    assert digest == bodies[address]
    again = {a: (n, s) for a, _k, n, s in names.of_module(lz, fs)["functions"]}
    assert again[address][0] in ("FirstBody",) or again[address][1] == "export", again[address]
    assert names.key(lz, address).count(":") == 1


def main():
    for fn in (test_demangle, test_definition_file_and_library, test_ordinals, test_prototypes, test_signatures,
               test_identify, test_store_identified_and_bodies, test_names_of_a_module):
        try:
            fn()
            RESULTS.append((fn.__name__, "PASS", ""))
        except Skip as s:
            RESULTS.append((fn.__name__, "SKIP", str(s)))
        except Exception as e:
            RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, str(e)[:300])))
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
