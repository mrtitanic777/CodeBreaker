# -*- coding: utf-8 -*-
"""The native L2 core and C back end (native/l2, CB_NATIVE_L2) against the Python, on small L2 documents:
every call the C back end's own tests make (tests/translate/test_c_backend.py) run through both writers
-- the text, the exception, and the facts the call wrote, whole -- and those tests passing with the
native writer on; (The L2 core's round trip and schema: test_native_l2_core.py.)
The module gate (every function of the gate modules, both paths) is docs/NATIVE.md's, step 5."""
import copy
import json
import os

import pytest

from codebreaker.translate import l2, native_l2
from codebreaker.translate.backends import c

pytestmark = pytest.mark.skipif(native_l2._load() is None, reason="native/l2 not built: %s" % native_l2.why_not())

_ENTRIES = ("function", "flags", "signature", "type_text", "expr", "condition", "module", "fragment", "seg_layout",
            "arguments_at", "parameters", "words", "argument_words", "prototypes", "takes32", "module_names32",
            "type_names", "parameter_names", "struct_accesses", "caller_frame_plan", "segment_plan")


def _backend_tests():
    from tests.translate import test_c_backend as t
    return t, [getattr(t, n) for n in sorted(dir(t)) if n.startswith("test_") and callable(getattr(t, n))]


def _run(tests, tmp_path):
    import inspect
    for f in tests:
        if "tmp_path" in inspect.signature(f).parameters:
            f(tmp_path)
        else:
            f()


@pytest.fixture
def native(monkeypatch):
    monkeypatch.setenv("CB_NATIVE_L2", "1")
    assert native_l2.enabled()


def test_the_backend_tests_pass_with_the_native_writer(native, tmp_path):
    _t, tests = _backend_tests()
    _run(tests, tmp_path)


def _tree(x):
    """An L2 document as L2 is: a tree. (The tests build some with one node object at several places;
    JSON -- and so the native writer -- has each place its own node, as the decompiler's documents do.)"""
    return json.loads(json.dumps(x)) if isinstance(x, dict) and "node" in x else x


def _outcome(fn, args, kw):
    """(the value or the exception's name, the facts as the call left them)"""
    args, kw = tuple(_tree(a) for a in copy.deepcopy(args)), copy.deepcopy(kw)
    facts = kw.get("facts", args[3] if fn is c._py_function and len(args) > 3 else None)
    try:
        got = fn(*args, **kw)
    except Exception as e:                               # (Decline, KeyError, ValueError ...)
        got = ("raises", type(e).__name__)
    if isinstance(got, set):
        got = sorted(got, key=repr)
    out = {k: facts[k] for k in ("novalue", "registers", "declared", "selector_decls", "selector_shared", "interrupts",
                                 "frame", "selector_imports", "code_reads", "names", "heading", "structs_used")
           if facts and k in facts} if isinstance(facts, dict) else None
    return got, out


def test_every_backend_test_call_the_same_both_ways(monkeypatch, tmp_path):
    """Each call into backends/c.py that its tests make, recorded with the Python writer, made again through
    the native one: the same text or value, the same exception, the same facts written."""
    monkeypatch.setenv("CB_NATIVE_L2", "0")
    calls = []
    for name in _ENTRIES:
        disp = getattr(c, name)

        def rec(*a, _name=name, _disp=disp, **kw):
            calls.append((_name, copy.deepcopy(a), copy.deepcopy(kw)))
            return _disp(*a, **kw)
        monkeypatch.setattr(c, name, rec)
    _t, tests = _backend_tests()
    _run(tests, tmp_path)
    assert len(calls) > 40
    monkeypatch.undo()
    differ = []
    for name, a, kw in calls:
        want = _outcome(getattr(c, "_py_" + name), a, kw)
        got = _outcome(native_l2._ENTRIES[name], a, kw)
        if want != got:
            differ.append((name, want, got))
    assert not differ, differ[:3]
