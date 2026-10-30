# -*- coding: utf-8 -*-
"""One module model (l2.module) and the back ends that write it: the assembly writers' trees are the
files they write, the readable pass and the 16-bit port read a module from its tree, and the module's
source file is written from its tree and the store's facts (docs/ARCHITECTURE.md, "Built: one Module
model, the writers as back ends")."""

import pytest

from codebreaker.translate import l2
from codebreaker.translate.backends import asm, module_file


def _valid(doc):
    jsonschema = pytest.importorskip('jsonschema')
    jsonschema.Draft202012Validator(l2.schema()).validate(doc)


def test_an_assembly_is_its_lines_as_fragments():
    L = l2.Assembly(["; head", "bits 16"], syntax="nasm", module="x.dll")
    L.part("seg1", "segment", {"module": "x.dll", "segment": 1, "offset": 0}, 4)
    L.append("")
    L.extend(["section seg1", "    db 1,2,3,4"])
    L.part("empty")                                       # (a part with no lines is left out)
    L.part("tail", "end")
    L += ["", "; end"]
    doc = L.document()
    _valid(doc)
    assert [u["name"] for u in doc["units"]] == ["head", "seg1", "tail"]
    assert doc["units"][1]["origin"] == {"module": "x.dll", "segment": 1, "offset": 0}
    assert all(u["syntax"] == "nasm" for u in doc["units"])
    assert asm.module(doc) == "\n".join(L)
    text, first = asm.lines(doc)
    assert first == {0: 1, 1: 3, 2: 6}
    lines = text.split("\n")
    assert lines[first[1] - 1] == "" and lines[first[1]] == "section seg1" and lines[first[2]] == "; end"


def test_the_assembly_back_end_writes_comments_and_declines_c():
    doc = l2.module([l2.comment("status"), l2.comment(""), l2.fragment("mov ax, 1", name="f", syntax="masm"),
                     l2.fragment(name="g", within="f")])
    assert asm.module(doc) == "; status\n;\nmov ax, 1"
    with pytest.raises(asm.Decline):
        asm.module(l2.module([l2.function("f", "int f(void) { return 1; }")]))


def test_emit_makes_each_function_a_fragment():
    from codebreaker.source import image_writer as w

    class Insn(object):
        size = 1

        def __init__(self, at):
            self.at = at
    image = bytes([0xC9, 0xAA, 0xC9])
    code = {0: Insn(0), 2: Insn(2)}
    functions = w.functions([0, 2], {0: "start"}, "sub_%04X")
    out = w.lines(["; test"], "sjasmplus", "t.bin")
    w.emit(out, 0, 3, image.__getitem__, lambda a: a, lambda a, addr: code.get(a), {0: "start", 2: "sub_0002"},
           functions, lambda i: "RET", lambda data: w.data_lines(data, "DEFB", "DEFS %d,$%02X"))
    doc = out.document()
    _valid(doc)
    assert [(u["name"], u["reason"]) for u in doc["units"]] == [("head", "head"), ("start", "function"),
                                                                 ("sub_0002", "function")]
    assert w.text(out) == "\n".join(out) == "; test\n\nstart:\n        RET\n        DEFB $AA\n\nsub_0002:\n        RET"


def test_the_readable_pass_reads_the_module_tree():
    from codebreaker.source import readable
    doc = l2.module([l2.function("1000", "int4 sub_1000(void)\n{\n  return 1;\n}\n"),
                     l2.function("2000", "int4 sub_2000(void)\n{\n  sub_1000();\n  return 0;\n}\n")],
                    header="typedef struct RECT RECT;\n")
    _valid(doc)
    r = readable.Readable(doc)
    assert sorted(r.texts) == ["1000", "2000"] and r._header_text.startswith("typedef")
    assert "sub_1000" in r.results_unused()


def test_the_port_reads_its_module_tree():
    from codebreaker.port import asm16
    text = "_TEXT SEGMENT\n_f PROC NEAR\n\tmov ax, 1\n\tret\n_f ENDP\n_TEXT ENDS\nEND\n"
    doc = asm16.module("f.asm", text)
    _valid(doc)
    assert asm.module(doc) == text
    p = asm16.Port(doc, asm16.CTypes([], []))
    assert p.doc is doc and len(p.lines) == len(asm16.parse(text))


FACTS = dict(name="x.dll", stem="x", date="2026-10-30", digest="abc", order=["verified", "no-source"],
             titles={"verified": "VERIFIED", "no-source": "NO SOURCE"}, counts={"verified": 1, "no-source": 1},
             total=2, kind="pe", whole=None, real=0, header=True)


def test_the_module_file_is_written_from_its_tree_and_the_facts():
    doc = l2.module([l2.function("f", "int f(void)\n{\n  return 1;\n}", comments=["[VERIFIED] f  @ 1000"]),
                     l2.comment("[NO SOURCE] @ 2000 -- not measured on the current verifier")])
    _valid(doc)
    [(name, text)] = module_file.c(doc, FACTS)
    assert name == "x.c"
    assert text.startswith("/* x.dll -- reconstructed source.\n *\n * STATUS, generated 2026-10-30 by CodeBreaker")
    assert " *   VERIFIED                         1   50.0%\n" in text
    assert text.endswith('#include "x.h"\n\n/* [VERIFIED] f  @ 1000 */\nint f(void)\n{\n  return 1;\n}\n\n'
                         '/* [NO SOURCE] @ 2000 -- not measured on the current verifier */\n')
    parts = module_file.c(doc, FACTS, split=1)
    assert [p[0] for p in parts] == ["x_1.c", "x_2.c"]
    assert parts[1][1].startswith("/* x.dll -- reconstructed source, part 2 of 2 (the status is in x_1.c). */\n\n"
                                  '#include "x.h"\n\n/* [NO SOURCE]')


def test_a_drivers_file_is_its_status_then_its_source():
    facts = dict(FACTS, how="judged as one module", rows=[("0001", "Init", "NO SOURCE", "")])
    text = module_file.asm(l2.module([l2.fragment("S1 segment\nS1 ends\n", syntax="masm")]), facts)
    assert text.startswith("; x.dll -- reconstructed source.\n;\n; STATUS, generated 2026-10-30 by CodeBreaker")
    assert ";   VERIFIED                         1   50.0%\n" in text
    assert text.endswith(";\n; judged as one module\n;   0001       Init                         NO SOURCE \n;\nS1 segment\nS1 ends\n")
    assert module_file.asm(l2.module(), facts).endswith(";\n; (no source could be written)\n")


def test_a_made_module_is_its_tree():
    """c_text's Module, Module16 and ModulePowerPC are one l2.Made: the module made compilable as
    an L2 Module (each function a MethodDecl with its C and its place, the header the Module's),
    texts / order / by_va / functions read from it, a change written into a document of its own
    (a copy keeps the one it had), the lane's tables beside it."""
    import copy
    import pickle
    m = l2.Made("x.dll", "H", {"a": "A", "b": "B"}, ["a", "b"], {0x10: "a", 0x20: "b"}, near={"a"})
    l2.validate(m.doc)
    assert (m.texts, m.order, m.by_va, m.header, m.near) == ({"a": "A", "b": "B"}, ["a", "b"], {0x10: "a", 0x20: "b"}, "H", {"a"})
    assert m.functions == {0x10: ("a", "A"), 0x20: ("b", "B")}
    assert l2.units(m.doc)[0]["origin"] == {"module": "x.dll", "va": 0x10}
    other = copy.copy(m)
    other.texts = dict(other.texts, a="Z")
    other.texts["b"] = "Y"
    assert m.texts == {"a": "A", "b": "B"} and other.texts == {"a": "Z", "b": "Y"}
    m.add("c", "C", 0x30)
    assert m.order == ["a", "b", "c"] and pickle.loads(pickle.dumps(m)).texts["c"] == "C"
