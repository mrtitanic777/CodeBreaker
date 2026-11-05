# -*- coding: utf-8 -*-
"""No function lost while the decompiler writes L2 (docs/IR_UNIFICATION.md): a small 16-bit function
for each kind of function the native program died on, or wrote an L2 tree for that the C back end
could not write, run through the native program with CB_L2=1.

  - a switch on a pointer: PrintC casts each case label, `case (int2 *)0x1:`, and the cast PrintL2
    met while it spelled the label waited for operands and took the statements after the switch
    (USER sub_3_160A, sub_4_3FBE: declined; here, `return *param_1 + 1` lost its value)
  - an array element's constant index past 9, which PrintC writes as hexadecimal text
    (`aiStack_40[0xb]`): Unsupported (COMCTL32 sub_02C195, SHELL32 sub_0252EC: declined)
  - a supplied jump table (a `J` line) whose jump the function's flow never reaches: the core's
    JumpTable::switchOver read the op it had none of, and the process died (USER's
    MSGTHKCONNECTIONDATALS, sub_28_444E)
"""
import os
import re
import subprocess

import pytest

from codebreaker.decompile import decompiler
from codebreaker.translate import l2
from codebreaker.translate.backends import c

# f(int2 *p): push bp / mov bp, sp / mov bx, [bp+4] / mov ax, [bx] / cmp bx, 4 / ja 30 / shl bx, 1 /
# jmp cs:[bx+18h]; its table at 18h (22h 26h 22h 26h 22h); 22h: inc ax / pop bp / ret;
# 26h: dec ax / pop bp / ret; 30h: xor ax, ax / pop bp / ret
CODE = bytes.fromhex(
    "55" "8bec" "8b5e04" "8b07" "83fb04" "7723" "d1e3" "2effa71800" "90909090"
    "2200" "2600" "2200" "2600" "2200"
    "405dc390" "485dc390" "909090909090"
    "31c05dc3")


def _run(tmp_path, extra=(), lines=None, code=CODE):
    if not decompiler.available():
        pytest.skip("the native decompiler is not built")
    path = tmp_path / "code.bin"
    path.write_bytes(code)
    lines = [ln.replace("{code}", str(path)) for ln in lines] if lines else [
        "arch\tx86:LE:16:Protected Mode:default",
        "bytes\t10000000\t%s\t0\t%x" % (path, len(code)),
        "F\t10000000\tf\t0\t__cdecl16near",
        "J\t1000000f\t10000022,10000026,10000022,10000026,10000022"] + list(extra)
    desc = tmp_path / "description.txt"
    desc.write_text("\n".join(lines) + "\n")
    env = dict(os.environ, CB_LANGUAGES=decompiler._languages(), CB_L2="1", CB_THREADS="1", CB_STRUCTS="1")
    r = subprocess.run([decompiler._exe(), str(desc)], capture_output=True, text=True, errors="replace", env=env,
                       timeout=300)
    return r


def _unsupported(node):
    if isinstance(node, dict):
        return (node.get("node") == "Unsupported") + sum(_unsupported(v) for v in node.values())
    if isinstance(node, list):
        return sum(_unsupported(v) for v in node)
    return 0


def _function(out):
    body = re.search(r"(?ms)^/\*@@ f 10000000 \*/\n(.*)", out).group(1)
    printed, facts = decompiler.split_notes(body)
    return printed, facts["l2"]


def test_switch_on_a_pointer_keeps_its_case_labels_cast(tmp_path):
    r = _run(tmp_path)
    assert r.returncode == 0, r.stderr[-500:]
    printed, doc = _function(r.stdout)
    assert "case (int2 *)0x1:" in printed
    l2.validate(doc)
    assert _unsupported(doc) == 0
    switch = next(s for s in doc["body"] if s.get("node") == "Switch")
    labels = [lab for case in switch["cases"] for lab in case["labels"]]
    assert labels and all(lab["node"] == "Cast" and lab["x"]["node"] == "Lit" for lab in labels)
    # (C switches on integers only: the pointer as the unsigned integer of its width, each label its number)
    mine = c.function(doc)
    assert "switch((uint2)param_1) {" in mine and "case 0x1:" in mine and "(int2 *)0x1:" not in mine
    assert "return *param_1 + 1;" in mine and "return *param_1 + -1;" in mine


def test_switch_on_a_far_pointer_is_written_on_its_four_bytes():
    ptr = {"kind": "ptr", "space": "far", "to": {"kind": "i16"}}
    sym = {"kind": "local", "name": "p"}
    case = lambda v, body: {"node": "Case", "labels": [{"node": "Cast", "type": ptr, "x": {
        "node": "Lit", "type": ptr, "value": v, "text": "0x%x" % v}}], "default": False, "body": body}
    doc = {"node": "MethodDecl", "name": "g", "kind": "function", "ret": {"kind": "i16"},
           "sym": {"kind": "function", "name": "g", "type": {"kind": "fn", "conv": "cdecl", "distance": "far",
                                                             "params": [], "result": {"at": "none"}}},
           "params": [], "body": [
               {"node": "Local", "sym": sym, "type": ptr},
               {"node": "Switch", "expr": {"node": "Name", "type": ptr, "sym": sym}, "cases": [
                   case(8, [{"node": "Return", "expr": {"node": "Lit", "type": {"kind": "i16"}, "value": 1}}]),
                   case(0x10002, [{"node": "Break"}])]},
               {"node": "Return", "expr": {"node": "Lit", "type": {"kind": "i16"}, "value": 0}}]}
    l2.validate(doc)
    mine = c.function(doc)
    assert "switch((uint4)p) {" in mine and "case 0x8:" in mine and "case 0x10002:" in mine


def test_an_array_index_past_9_is_a_literal(tmp_path):
    # g(int4 i), 32-bit: sub esp, 40h / xor ecx, ecx / L: mov dword [esp+ecx*4], 0 / inc ecx /
    # cmp ecx, 10h / jl L / mov dword [esp+2Ch], 0FFh / mov ecx, [esp+44h] / mov eax, [esp+ecx*4] /
    # add esp, 40h / ret 4 -- PrintC writes the constant index of an element of a variable it already
    # has as text, in hexadecimal past 9 (`aiStack_40[0xb]`); PrintL2 made it Unsupported (COMCTL32's
    # sub_02C195, SHELL32's sub_0252EC: declined)
    code = bytes.fromhex("83ec40" "31c9" "c7048c00000000" "41" "83f910" "7cf3" "c744242cff000000" "8b4c2444"
                         "8b048c" "83c440" "c20400")
    r = _run(tmp_path, lines=["arch\tx86:LE:32:default:windows", "bytes\t10001000\t{code}\t0\t%x" % len(code),
                              "F\t10001000\tg\t4\t__stdcall"], code=code)
    assert r.returncode == 0, r.stderr[-500:]
    body = re.search(r"(?ms)^/\*@@ g 10001000 \*/\n(.*)", r.stdout).group(1)
    printed, facts = decompiler.split_notes(body)
    doc = facts["l2"]
    assert "aiStack_40[0xb] = 0xff;" in printed
    l2.validate(doc)
    assert _unsupported(doc) == 0
    assert "aiStack_40[0xb] = 0xff;" in c.function(doc)


def test_a_supplied_jump_table_flow_never_reaches_is_dropped(tmp_path):
    # (a second table, at 2Ah: padding no flow goes through)
    r = _run(tmp_path, ["J\t1000002a\t10000022,10000026"])
    assert r.returncode == 0, "the native program died: %r" % r.stderr[-500:]
    printed, doc = _function(r.stdout)
    assert "Supplied jumptable not reached by flow" in printed
    l2.validate(doc)
    assert _unsupported(doc) == 0
    assert "switch" in c.function(doc)
