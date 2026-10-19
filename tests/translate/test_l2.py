# -*- coding: utf-8 -*-
"""L2 as JSON (docs/IR_UNIFICATION.md): the schema and ir.py describe the same tree, lowered Java is valid
L2 and round-trips, and the binary-facing types agreed with the decompiler (L2_BINARY_TYPES.md) are
expressible."""

import json

import pytest

from codebreaker.translate import ir, l2
from codebreaker.translate.frontends import java as fj
from codebreaker.translate.frontends.java import lower as L
from codebreaker.verify import java

jsonschema = pytest.importorskip('jsonschema')

SOURCE = '''import java.util.*;

public class Mix {
	static int count;
	long[] cells = new long[3];

	int step(int a, char c) {
		int b = a >>> 2 ^ c;
		for (long x : this.cells) b += (int)x;
		Runnable r = () -> count++;
		return b > 0 ? b % 7 : -b + "!".length();
	}
}
'''


def test_the_schema_is_valid_and_names_every_node():
    s = l2.schema()
    jsonschema.Draft202012Validator.check_schema(s)
    in_schema = set(s['$defs']['Node']['properties']['node']['enum'])
    assert in_schema == set(l2._NODES)                           # ir.py and the schema in step


@pytest.mark.skipif(not java.jdk()[0], reason='needs a JDK')
def test_lowered_java_is_valid_l2_and_round_trips(tmp_path):
    (tmp_path / 'src').mkdir()
    (tmp_path / 'src' / 'Mix.java').write_text(SOURCE, encoding='utf-8')
    unit = L.lower(fj.dump(str(tmp_path / 'src'), str(tmp_path / 'out'))['Mix'], 'Mix')
    doc = l2.to_json(unit)
    l2.validate(json.loads(json.dumps(doc)))                     # through text, as a file would be
    again = l2.to_json(l2.from_json(doc))
    assert again == doc


def test_binary_facing_types_are_expressible():
    far_str = ir.pointer_to(ir.Type('char', signed=True), space='far')
    near_ds = ir.pointer_to(ir.Type('u16'), space='near', seg='ds')
    fn = ir.Type('fn', conv='pascal', distance='far', pops=6, loadds=True,
                 params=[{'name': 'hwnd', 'type': ir.Type('u16'), 'at': {'stack': 4}}],
                 result={'type': ir.Type('i32'), 'at': {'regs': ['DX', 'AX']}}, preserves=['SI', 'DI', 'DS'])
    rect = ir.Type('record', 'RECT', size=8, role='struct', packed=True,
                   fields=[{'name': n, 'offset': 2 * i, 'type': ir.Type('i16')} for i, n in enumerate('ltrb')])
    frag = ir.MachineFragment(length=12, reason='register-arguments', why='AX:DX in', asm='mov ax, dx',
                              origin={'module': 'USER.EXE', 'segment': 3, 'offset': 0x1a2})
    sel = ir.Local(sym=ir.Sym('local', 's'), type=ir.Type('selector', of={'module_segment': 44}), init=None, mods=[])
    far = ir.ExprStmt(expr=ir.FarAddress(selector=ir.Name(sym=ir.Sym('local', 's'), type=ir.Type('selector')),
                                         offset=ir.Lit(value=16, type=ir.Type('u16')), type=far_str))
    cmp_ = ir.ExprStmt(expr=ir.Binary(op='<', left=ir.Lit(value=1, type=ir.Type('i16')),
                                      right=ir.Lit(value=2, type=ir.Type('i16')), signed=False, type=ir.BOOL))
    block = ir.Block(stmts=[frag, sel, far, cmp_])
    doc = l2.to_json(block)
    l2.validate(doc)
    assert l2.to_json(l2.from_json(doc)) == doc
    for t in (far_str, near_ds, fn, rect, ir.Type('fnptr', space='far', fn=fn)):
        d = l2.type_json(t)
        jsonschema.Draft202012Validator({'$ref': '#/$defs/Type', '$defs': l2.schema()['$defs']}).validate(d)
        assert l2.type_from(d) == t
