"""The C and C++ front end: clang's typed AST, lowered into the IR (docs/TRANSLATOR_DESIGN.md, phase 5).

clang parses and attributes the source (`-Xclang -ast-dump=json -fsyntax-only`): every expression's
type, every implicit conversion, the declaration each name refers to, the overload and template
instance chosen. lower.py turns the declarations of the program's own files into one ir.Unit.

    from codebreaker.translate.frontends import clang
    unit = clang.lower_file('prog.c')                       # C (the language from the extension)
    unit = clang.lower_file('prog.cpp', std='c++17')        # C++
    doc = clang.l2_file('prog.c')                           # the same as an L2 document (cached)

The target is fixed (`TRIPLE`, LLP64 Windows x64): it is the data model the judge's own clang build of
the program runs with, so sizeof, struct layouts and `long` agree between the original and the
translation. clang is found from settings.json's "clang", else Visual Studio's bundled LLVM, else PATH.
"""
import hashlib
import json
import os
import shutil
import subprocess

from codebreaker.translate import l2
from codebreaker.translate.frontends.clang import lower as _lower

TRIPLE = 'x86_64-pc-windows-msvc'
REV = 1                     # bump when the lowering changes what it makes of the same source
_VS_LLVM = (r'C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang.exe',
            r'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang.exe')
TIMEOUT = 300


def clang():
    """The clang executable, or None."""
    try:
        from codebreaker.base import settings
        got = settings.get('clang')
        if got and os.path.isfile(got):
            return got
    except Exception:
        pass
    for p in _VS_LLVM:
        if os.path.isfile(p):
            return p
    return shutil.which('clang')


def language_of(path):
    return 'c' if path.lower().endswith('.c') else 'c++'


def flags(language, std=None, includes=(), quote_includes=(), defines=()):
    """clang's command-line options for parsing (and the judge's compiling) a source file."""
    out = ['--target=' + TRIPLE]
    if language == 'c':
        out += ['-x', 'c', '-std=' + (std or 'c11')]
    else:
        out += ['-x', 'c++', '-std=' + (std or 'c++17')]
    out += ['-Wno-everything', '-D_CRT_SECURE_NO_WARNINGS', '-D_CRT_NONSTDC_NO_WARNINGS']
    for d in includes:
        out += ['-I', d]
    for d in quote_includes:
        out += ['-iquote', d]
    for d in defines:
        out += ['-D' + d]
    return out


def dump(path, language=None, std=None, includes=(), quote_includes=(), defines=()):
    """clang's JSON AST of one source file (a dict). Raises RuntimeError with clang's errors."""
    exe = clang()
    if not exe:
        raise RuntimeError('the C/C++ front end needs clang (settings.json: clang)')
    language = language or language_of(path)
    cmd = [exe] + flags(language, std, includes, quote_includes, defines) + \
        ['-Xclang', '-ast-dump=json', '-fsyntax-only', path]
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=TIMEOUT)
    if r.returncode != 0:
        raise RuntimeError('clang: ' + r.stderr.decode('utf-8', 'replace')[-1500:])
    return json.loads(r.stdout.decode('utf-8', 'surrogateescape'))


def lower_file(path, language=None, std=None, includes=(), quote_includes=(), defines=(), own=None):
    """One source file -> ir.Unit. `own`: the files whose declarations are the program's (default: the
    file itself and the headers beside it)."""
    language = language or language_of(path)
    tree = dump(path, language, std, includes, quote_includes, defines)
    lw = _lower.Lowering(tree, path, own=own, language=language, std=std, triple=TRIPLE)
    return lw.unit()


def _key(path, language, std, includes, quote_includes, defines, own):
    h = hashlib.sha1()
    h.update(repr((REV, language, std, list(includes), list(quote_includes), list(defines), TRIPLE)).encode())
    here = os.path.dirname(os.path.abspath(path))
    files = [path] + list(own or [])
    files += [os.path.join(here, f) for f in sorted(os.listdir(here)) if f.lower().endswith(('.h', '.hpp', '.hh'))]
    for f in files:
        try:
            with open(f, 'rb') as fh:
                h.update(fh.read())
        except OSError:
            pass
    for src in (_lower.__file__, os.path.join(os.path.dirname(_lower.__file__), 'ctypes.py')):
        with open(src, 'rb') as fh:
            h.update(fh.read())
    h.update(repr(clang()).encode())
    return h.hexdigest()[:16]


def l2_file(path, language=None, std=None, includes=(), quote_includes=(), defines=(), own=None, cache=True):
    """lower_file as an L2 document (a dict), cached under the state folder by the source and the lowering."""
    language = language or language_of(path)
    key = _key(path, language, std, includes, quote_includes, defines, own)
    where = None
    if cache:
        try:
            from codebreaker.base import settings
            where = settings.state('translate', 'clang', '%s.%s.json' % (os.path.basename(path), key))
        except Exception:
            where = None
        if where and os.path.isfile(where):
            with open(where, encoding='utf-8') as f:
                return json.load(f)
    doc = l2.to_json(lower_file(path, language, std, includes, quote_includes, defines, own))
    if where:
        os.makedirs(os.path.dirname(where), exist_ok=True)
        with open(where, 'w', encoding='utf-8') as f:
            json.dump(doc, f)
    return doc
