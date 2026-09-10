# Ghidra headless: parse period C headers into a .gdt archive.
# args: <out.gdt> <include_dir> <header_full_path> [more headers/include dirs...]
# Any arg after the first header that is a directory is added as another -I path.
import os
from ghidra.app.util.cparser.C import CParserUtils
from ghidra.program.model.data import FileDataTypeManager
from ghidra.util.task import ConsoleTaskMonitor
from java.io import File

import glob as _glob
a = list(getScriptArgs())
gdt_path = a[0]
inc = a[1]
rest = a[2:]
headers = []
extra_incs = []
for x in rest:
    if os.path.isdir(x):                       # a dir -> include path AND parse every header in it
        extra_incs.append(x)
        headers += sorted(_glob.glob(os.path.join(x, "*.[Hh]")))
    else:
        headers.append(x)
monitor = ConsoleTaskMonitor()

# --- comprehensive define kit to get old Windows headers through Ghidra's C parser ---
BLANK = [
    # calling conventions / segment keywords
    "_far", "_near", "far", "near", "FAR", "NEAR", "PASCAL", "pascal", "CDECL",
    "_cdecl", "__cdecl", "_stdcall", "__stdcall", "__fastcall", "_fastcall",
    "WINAPI", "WINAPIV", "CALLBACK", "APIENTRY", "APIPRIVATE", "PASCAL",
    "_export", "__export", "huge", "_huge", "_loadds", "cdecl",
    # SAL / annotations
    "IN", "OUT", "OPTIONAL", "UNALIGNED", "__unaligned", "volatile",
    # declspec family
    "__declspec", "DECLSPEC_IMPORT", "DECLSPEC_EXPORT", "DECLSPEC_NORETURN",
    "DECLSPEC_NOTHROW", "DECLSPEC_SELECTANY", "DECLSPEC_NOVTABLE",
    "DECLSPEC_DEPRECATED", "DECLSPEC_UUID",
    # api import/export decorations
    "NTSYSAPI", "NTAPI", "WINBASEAPI", "WINUSERAPI", "WINGDIAPI", "WINADVAPI",
    "WINOLEAPI", "WINCOMMCTRLAPI", "WINCOMMDLGAPI", "SHSTDAPI", "SHDOCAPI",
    "_CRTIMP", "__RPC_FAR", "RPC_ENTRY", "__RPC_API", "__RPC_USER",
    "STDMETHODCALLTYPE", "STDAPICALLTYPE", "__inline", "__forceinline",
]
VAL = {
    "VOID": "void", "CONST": "const", "WIN32": "1", "_WIN32": "1",
    "_X86_": "1", "_M_IX86": "500", "i386": "1", "_INTEGRAL_MAX_BITS": "64",
    "__int64": "long long", "__int32": "int", "__int16": "short", "__int8": "char",
    "inline": " ", "MIDL_PASS": "1",
}
args = ["-I" + inc] + ["-I" + d for d in extra_incs]
for d in BLANK:
    args.append("-D%s=" % d)
for k, v in VAL.items():
    args.append("-D%s=%s" % (k, v))

dtm = FileDataTypeManager.createFileArchive(File(gdt_path))
import sys
import re
import tempfile
# Parse header-by-header into the accumulating archive so one header with a fatal
# preprocessor #error is skipped, not fatal to the rest. Headers that partially
# parse still contribute their types before their error. CURATION: a header that
# demands a parent ("X.H to be included first") is retried through a wrapper that
# includes the parent first -- reclaiming that header's own types.
_tmp = tempfile.mkdtemp()
_ORDER = re.compile(r"([A-Za-z0-9_]+\.[Hh])\s+to be included first")
ok_h = 0
recovered_h = 0
fail_h = 0
failed = []


def _parse(files):
    CParserUtils.parseHeaderFiles(None, files, args, dtm, monitor)


for h in headers:
    try:
        _parse([h])
        ok_h += 1
        continue
    except:
        emsg = str(sys.exc_info()[1])
    done = False
    m = _ORDER.search(emsg)                       # include-order guard -> retry with parent
    if m:
        parent = os.path.join(inc, m.group(1))
        if os.path.exists(parent):
            wrap = os.path.join(_tmp, "_wrap.h")
            try:
                fh = open(wrap, "w")
                fh.write('#include "%s"\n#include "%s"\n'
                         % (parent.replace("\\", "/"), h.replace("\\", "/")))
                fh.close()
                _parse([wrap])
                recovered_h += 1
                done = True
            except:
                pass
    if not done:
        fail_h += 1
        failed.append(os.path.basename(h))
print("HEADERS clean=%d recovered=%d had_errors=%d" % (ok_h, recovered_h, fail_h))
if failed:
    print("HDRS_WITH_ERRORS= %s" % ", ".join(failed[:30]))

try:
    n = dtm.getDataTypeCount(True)
except Exception:
    n = -1
try:
    dtm.save()
except Exception as e:
    print("SAVE_ERR= %s" % e)
dtm.close()
print("GDT_BUILT path=%s types=%d" % (gdt_path, n))
