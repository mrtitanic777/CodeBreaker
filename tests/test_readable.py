#!/usr/bin/env python3
"""Tests for the names written into a function's source after judging (source/readable.py).
Run:  python tests/test_readable.py     (exit code 0 = all passed)

The SDK constants come from the SDK headers the prototypes are read from; checks that need them
are SKIPPED where they are not on this machine."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.source import readable                     # noqa: E402
from codebreaker.translate import l2                         # noqa: E402

RESULTS = []
SAMPLE = '''int4 sub_1(int4 a1)
{
  int4 iVar1;
  int4 iVar2;
  int4 uVar3;
  iVar1 = LocalAlloc(0x40,100);
  ShowWindow(a1,5);
  iVar2 = GetDC(a1);
  for (uVar3 = 0; uVar3 < 10; uVar3 = uVar3 + 1) {
    MessageBoxA(a1,"keep 0x40 in a string",0,0x24);
  }
  ReleaseDC(a1,iVar2);
  pcRam10094 = GetProcAddress(iVar1,0x1000);
  return iVar1;
}'''


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


def _need_sdk():
    if readable.constant("ShowWindow", "nCmdShow", 5) is None:
        raise Skip("no SDK headers here")


def test_constants():
    _need_sdk()
    c = readable.constant
    assert c("MessageBoxA", "uType", 0x24) == "MB_ICONQUESTION | MB_YESNO"
    assert c("LocalAlloc", "uFlags", 0x40) == "LMEM_ZEROINIT"
    assert c("ShowWindow", "nCmdShow", 0) == "SW_HIDE"
    assert c("RegOpenKeyExA", "hKey", 0x80000002) == "HKEY_LOCAL_MACHINE"
    assert c("CreateWindowExA", "dwStyle", 0x50010000) == "WS_CHILD | WS_VISIBLE | WS_TABSTOP"
    assert c("CreateWindowExA", "dwStyle", 0x10CF0000) == "WS_VISIBLE | WS_OVERLAPPEDWINDOW"
    assert c("GetWindowLongA", "nIndex", -16) == "GWL_STYLE"
    assert c("ShowWindow", "nCmdShow", 99) is None                  # no such value: the number stays
    assert c("LocalAlloc", "uBytes", 0x40) is None                 # a size is not a flag


def test_one_function():
    _need_sdk()
    m = readable.Readable(l2.module([l2.function("sub_1", SAMPLE)]), lambda address: "DllGetVersion" if address == 0x1000 else None)
    text = m.function("sub_1")
    for wanted in ("int4 sub_1(int4 hWnd)", "hMem = LocalAlloc(LMEM_ZEROINIT,100);", "ShowWindow(hWnd,SW_SHOW);",
                   "hdc = GetDC(hWnd);", "for (i = 0; i < 10; i = i + 1)", "MB_ICONQUESTION | MB_YESNO",
                   '"keep 0x40 in a string"', "g_pfnDllGetVersion = GetProcAddress(hMem,0x1000 /* \"DllGetVersion\" */);"):
        assert wanted in text, (wanted, text)
    assert m.constants_used == {"LMEM_ZEROINIT": 0x40, "SW_SHOW": 5, "MB_ICONQUESTION": 0x20, "MB_YESNO": 4}
    assert "#define MB_YESNO 0x4" in m.definitions()
    assert "g_pfnDllGetVersion" in m.header("extern int4 pcRam10094;")
    assert readable.calls_hint(SAMPLE).startswith("memory and windows: LocalAlloc, ShowWindow")


WNDPROC = """int4 wndproc(int4 a1, uint4 a2, int4 a3, int4 a4)
{
  if (a2 == 0x10) {
    return 0;
  }
  switch (a2) {
  case 2:
    PostQuitMessage(0);
    break;
  case 0xf:
    GetClientRect(a1,iVar1);
    *(int4 *)(iVar1 + 8) = *(int4 *)(iVar1 + 8) - 4;
    wsprintfA(a4,0x1000,0x2000,*(int4 *)iVar1);
    break;
  }
  return DefWindowProcA(a1,a2,a3,a4);
}"""


def test_comparisons_fields_and_strings():
    _need_sdk()
    strings = {0x1000: "%s = %ld", 0x2000: "count"}
    m = readable.Readable(l2.module([l2.function("w", WNDPROC)]), strings.get)
    text = m.function("w")
    for wanted in ("if (Msg == WM_CLOSE)", "case WM_DESTROY:", "case WM_PAINT:",
                   "((RECT *)lpRect)->right = ((RECT *)lpRect)->right - 4;",
                   '0x1000 /* "%s = %ld" */', '0x2000 /* %s: "count" */', "((RECT *)lpRect)->left /* %ld */"):
        assert wanted in text, (wanted, text)
    assert "typedef struct RECT" in m.struct_definitions() and "int4 right;" in m.struct_definitions()


def test_same_object_code():
    """The renamed text compiles to the same object code as the text it came from."""
    from codebreaker.build import functions, watcom
    ow = watcom._ow()
    if not os.path.exists(ow["wcc"]):
        raise Skip("OpenWatcom is not set up")
    _need_sdk()
    header = ("typedef int int4;\nint4 __stdcall LocalAlloc(int4,int4);\nint4 __stdcall ShowWindow(int4,int4);\n"
              "int4 __stdcall GetDC(int4);\nint4 __stdcall ReleaseDC(int4,int4);\n"
              "int4 __stdcall MessageBoxA(int4,char *,int4,int4);\nint4 __stdcall GetProcAddress(int4,int4);\n"
              "#define pcRam10094 (*(int4 *)0x10094)\n")         # globals are bound to their addresses
    m = readable.Readable(l2.module([l2.function("sub_1", SAMPLE)]), lambda address: "DllGetVersion" if address == 0x1000 else None)
    texts = {"before": SAMPLE, "after": m.function("sub_1")}
    env = watcom._ow_env(ow)
    out = {}
    for which, hdr in (("before", header), ("after", m.header(header))):
        folder = tempfile.mkdtemp(prefix="readable_")
        stems, dropped = functions.compile_all(ow["wcc"], ["-zq", "-bt=nt", "-3s", "-ox"], env, hdr,
                                               {which: texts[which]}, [which], folder, jobs=1)
        assert not dropped, dropped
        out[which] = functions.object_digest(os.path.join(folder, stems[which] + ".obj"))
    assert out["before"] == out["after"]


def main():
    for fn in (test_constants, test_one_function, test_comparisons_fields_and_strings, test_same_object_code):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
