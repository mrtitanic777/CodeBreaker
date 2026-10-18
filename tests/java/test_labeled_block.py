# -*- coding: utf-8 -*-
"""Regression: a Java LABELED BLOCK (`label: { ... break label; }`, not a labeled loop) must lower to
`goto label_brk;` + a `label_brk:;` target after the block. Previously _translate_labels only handled
labeled LOOPS, so a labeled block left an invalid C++ `break label;` (Indev's Frustrum / the decompiler's
`Label_NNNN` blocks -> C2146 'missing ; before label' + C2043 'illegal break')."""

from codebreaker.java.emit.statements.controlflow import _translate_labels


def test_labeled_block_break_becomes_goto():
    lines = [
        'label63: {',
        'if (a > b) {',
        'if (a <= c) { r = a; break label63; }',
        '} else if (b > c) { r = b; break label63; }',
        'r = c;',
        '}',
    ]
    out = _translate_labels(lines)
    joined = '\n'.join(out)
    assert 'break label63' not in joined                 # no invalid labeled break survives
    assert joined.count('goto label63_brk;') == 2        # both breaks became gotos
    assert 'label63_brk:;' in joined                     # the target label is emitted...
    assert out.index('label63_brk:;') > out.index('r = c;')   # ...AFTER the block body
    assert not any(ln.strip().startswith('label63:') for ln in out)  # the `label63:` prefix is stripped


def test_labeled_loop_still_uses_cont_and_brk():
    lines = [
        'outer: for (int i = 0; i < n; ++i) {',
        'if (x) continue outer;',
        'if (y) break outer;',
        '}',
    ]
    out = _translate_labels(lines)
    joined = '\n'.join(out)
    assert 'goto outer_cont;' in joined and 'outer_cont:;' in joined
    assert 'goto outer_brk;' in joined and 'outer_brk:;' in joined


def test_unlabeled_break_untouched():
    lines = ['for (;;) {', 'break;', '}']
    assert _translate_labels(lines) == lines
