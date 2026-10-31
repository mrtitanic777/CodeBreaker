# -*- coding: utf-8 -*-
"""The native L2 core (native/l2: the node model of ir.py, l2.to_json / from_json, l2.validate) against the
Python: L2 documents of each kind -- a decompiled function with its layouts and conventions, a C unit with a
record layout and type parameters, a Java unit with generics, lambdas and a C# property, the module model,
a MachineFragment, a non-ASCII literal -- through both round trips (json.dumps of the result, byte for byte)
and both schema checks. The gate over every recorded document is docs/NATIVE.md's (step 5)."""
import json

import pytest

from codebreaker.translate import l2, native_l2

pytestmark = pytest.mark.skipif(native_l2._load() is None, reason="native/l2 not built: %s" % native_l2.why_not())


def _docs():
    """L2 documents of each kind: a decompiled function (16-bit, with layouts and conventions), the module
    model's, a Java front end's shapes (classes, generics, lambdas), C#'s and clang's optional fields."""
    fn16 = {"node": "MethodDecl", "name": "sub_1_0000", "kind": "function",
            "origin": {"module": "m.dll", "segment": 1, "offset": 0},
            "sym": {"kind": "function", "name": "sub_1_0000", "static": True,
                    "type": {"kind": "fn", "conv": "stdcall", "distance": "far", "pops": 4,
                             "params": [{"name": "a1", "type": {"kind": "ptr", "space": "far", "to": {"kind": "u8"}},
                                         "at": {"stack": 6}}],
                             "result": {"type": {"kind": "i16"}, "at": {"reg": "AX"}}, "variadic": False}},
            "ret": {"kind": "i16"}, "mods": ["__stdcall16far"],
            "params": [{"node": "Param", "sym": {"kind": "param", "name": "a1"},
                        "type": {"kind": "ptr", "space": "far", "to": {"kind": "u8"}}}],
            "body": [{"node": "Local", "sym": {"kind": "local", "name": "axStack_c"},
                      "type": {"kind": "array", "count": 8, "elem": {"kind": "class", "name": "xunknown1"}}},
                     {"node": "If", "cond": {"node": "Binary", "op": "==", "signed": True,
                                             "left": {"node": "Name", "sym": {"kind": "param", "name": "a1"}},
                                             "right": {"node": "Lit", "value": 0, "text": "0x0"}},
                      "then": {"node": "Goto", "label": "LAB_1"}},
                     {"node": "Labeled", "label": "LAB_1", "body": []},
                     {"node": "Return", "expr": {"node": "Lit", "value": 1.5}}],
            "comments": ["WARNING: a note"]}
    record = {"node": "Unit", "path": "a.c", "lang": "c", "target": {"pointer": 4, "long": 4, "char": "signed"},
              "types": [{"node": "ClassDecl", "name": "S", "kind": "struct",
                         "layout": {"kind": "record", "size": 8, "name": "S", "packed": False,
                                    "fields": [{"name": "a", "offset": 0, "type": {"kind": "i32"}},
                                               {"name": "f", "offset": 4,
                                                "type": {"kind": "fnptr", "space": "flat",
                                                         "fn": {"kind": "fn", "conv": "cdecl", "params": [],
                                                                "result": {"at": "none"}}}}]},
                         "members": [], "tparams": [{"node": "TypeParam", "name": "T", "bounds": [{"kind": "class", "name": "x.Y"}],
                                                     "flags": ["class"]}]}]}
    java = {"node": "Unit", "package": "p", "imports": ["java.util.List"], "lang": "java",
            "types": [{"node": "ClassDecl", "name": "A", "qname": "p.A", "kind": "class", "mods": ["public"],
                       "extends": {"kind": "class", "name": "java.lang.Object"},
                       "implements": [{"kind": "class", "name": "java.util.List", "args": [{"kind": "wildcard", "bound": "extends",
                                                                                            "type": {"kind": "typevar", "name": "T"}}]}],
                       "members": [{"node": "MethodDecl", "name": "m", "kind": "method", "line": 3, "col": 5,
                                    "sym": {"kind": "method", "name": "m", "owner": "p.A", "params": [{"kind": "i32"}],
                                            "ret": {"kind": "void"}, "refs": ["", "ref"], "flags": ["virtual"]},
                                    "params": [{"node": "Param", "sym": {"kind": "param", "name": "x"}, "type": {"kind": "i32"}}],
                                    "body": {"node": "Block", "stmts": [
                                        {"node": "ExprStmt", "expr": {"node": "Lambda", "params": [], "body": {
                                            "node": "Lit", "value": float("inf"), "type": {"kind": "f64"}}}},
                                        {"node": "Local", "sym": {"kind": "local", "name": "y"},
                                         "type": {"kind": "array", "elem": {"kind": "i32"}, "rank": 2},
                                         "init": {"node": "NewArray", "elem": {"kind": "i32"}, "dims": [
                                             {"node": "Lit", "value": 3}], "init": None}},
                                        {"node": "Throw", "expr": {"node": "New", "sym": {"kind": "ctor", "name": "E"},
                                                                   "args": []}}]}},
                                   {"node": "Property", "sym": {"kind": "property", "name": "P"}, "type": {"kind": "i32"},
                                    "accessors": [{"node": "MethodDecl", "kind": "get", "name": "get_P", "body": None}]}]}]}
    module = l2.Made("m.dll", "/* header */", {"f": "int f(void) { return 0; }"}, ["f"], {0x1000: "f"}).doc
    return [fn16, record, java, module, {"node": "Module", "units": [l2.fragment("ret", name="g", syntax="wasm")]},
            {"node": "Lit", "value": "it's é \U0001F600", "junk": True}]


def test_l2_round_trip_and_schema_the_same_both_ways():
    for d in _docs():
        d = json.loads(json.dumps(d, allow_nan=True))
        want = json.dumps(l2.to_json(l2.from_json(d)))
        assert native_l2.roundtrip_text(d) == want, d.get("node")
        try:
            l2.validate(d)
            valid = True
        except Exception:
            valid = False
        assert native_l2.validate(d)[0] == valid, d.get("node")
