# -*- coding: utf-8 -*-
"""Regression: `.equals()` on a corpus reference type that declares NO equals() lowers to reference
identity `(a == b)` (Java Object.equals), instead of calling a non-existent member (EntityMapSlot ->
C2039 'equals is not a member'). A class that DOES declare/inherit equals keeps its call."""

import os
import tempfile

from codebreaker.java import transpile as pipeline, profiles


def _convert(files):
    d = tempfile.mkdtemp(); out = tempfile.mkdtemp()
    for name, src in files.items():
        with open(os.path.join(d, name), 'w', encoding='utf-8') as f:
            f.write(src)
    try:
        pipeline.convert(d, out, platform=None)
        return {n[:-5]: open(os.path.join(out, n[:-5] + '.cpp'), encoding='utf-8').read()
                for n in files if n.endswith('.java')}
    finally:
        profiles.use('minecraft_xbox360')


def test_noequals_class_uses_identity():
    cpp = _convert({
        'Slot.java': 'public class Slot { public int x; }',
        'M.java': 'public class M { public boolean f(Slot a, Slot b) { return a.equals(b); } }',
    })['M']
    assert '(a == b)' in cpp
    assert '->equals(' not in cpp and '.equals(' not in cpp


def test_class_with_equals_keeps_call():
    cpp = _convert({
        'Slot2.java': 'public class Slot2 { public boolean equals(Slot2 o) { return true; } }',
        'N.java': 'public class N { public boolean f(Slot2 a, Slot2 b) { return a.equals(b); } }',
    })['N']
    assert 'a->equals(b)' in cpp        # a real equals() is preserved, not turned into identity
    assert '(a == b)' not in cpp
