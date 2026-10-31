"""cpp17.py -- the translator's C++17 back end, for its Python callers: a thin shim over the native one.

The back end is C++ (native/l2/src/cpp17_*.cc, in cbl2.dll; docs/NATIVE.md step 5, docs/TRANSLATOR_DESIGN.md):
ONE C++17 emitter, with the source language's rules chosen by the unit's language (Unit.lang) --
'python' on native/ported/include/py.hpp, 'java' (or none) on codebreaker/translate/runtime/cpp17/jrt.hpp;
'csharp', 'c' and 'cpp' have a place and decline until their rules are written. This module only hands
the L2 over (l2.to_json) and the text back, until its callers move to C++ themselves:

    write_unit(unit)       a Python module (the Python front end, docs/PYTHON_FRONTEND.md) -> (header,
                           source, {function: (first, last) source line})
    program(units, main)   a whole Java program from L2 -> its C++ text (the modern forms C++17 has no word
                           for are lowered first: translate/desugar.py)
    Bodies(ctx, units)     inside the Java transpiler (`convert(..., cpp_std='C++17')`): every body written
                           by the C++03 back end's writer with the C++17 forms (cpp03.Bodies17) -- the
                           transpiler's declarations are C++03's, so its bodies move to C++ with cpp03

Both raise Decline for what the back end cannot write. Without cbl2.dll there is no C++17 back end
(native_l2.why_not() says why; build: native/l2/CMakeLists.txt).
"""
import os

from codebreaker.translate import l2, native_l2

RUNTIME = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'runtime', 'cpp17')
FLAGS = ('-std=c++17', '-fwrapv', '-w', '-O1')
API_MAPS = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), 'java', 'api_maps')


class Decline(Exception):
    """The C++17 back end does not write this unit (the reason says why)."""


def header_path(module):
    return module.replace('.', '/') + '.hpp'


def source_path(module):
    return module.replace('.', '/') + '.cpp'


def _native():
    if native_l2._load() is None:
        raise RuntimeError('the C++17 back end is native: %s' % native_l2.why_not())


def write_unit(unit, banner=None):
    """An ir.Unit of the Python front end -> (header text, source text, {function: (first, last) source
    line}) -- the lines say where each function's body is in the source, so a compiler's error can be
    pinned on it."""
    _native()
    doc = l2.to_json(unit)
    if doc.get('lang') is None:
        doc['lang'] = 'python'                            # (a unit the Python front end made before it said so)
    h, s, spans = native_l2.call('cpp17_unit', doc, API_MAPS, plain=0)
    return h, s, dict((k, tuple(v)) for k, v in spans.items())


def program(units, main=None):
    """The C++17 text of a whole Java program (its L2 units), or raise Decline."""
    from codebreaker.translate import desugar
    _native()
    desugar.run(units, keep=('foreach', 'switchexpr'))
    docs = [l2.to_json(u) for u in units]
    return native_l2.call('cpp17_program', docs, main, plain=0)


def Bodies(ctx, units):
    """The transpiler's bodies in C++17 (cpp03's writer with the C++17 forms)."""
    from codebreaker.translate.backends import cpp03
    return cpp03.Bodies17(ctx, units)
