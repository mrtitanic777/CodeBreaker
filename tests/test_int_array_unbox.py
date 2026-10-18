# -*- coding: utf-8 -*-
"""Regression: a raw List/Map holds a primitive array (int[]); reading + casting it
`(int[])list.remove(i)` must unbox the JObject element via JObject::operator JArray<T>() ->
`(JArray<int >)(...)`. `(int[])` is not valid C++ and _CAST doesn't match a `[]` target, so this
relies on _expr_object_kind recognizing the raw-list element as a JObject. Covers the simple `this->`
receiver AND a `var->field.remove(...)` field chain (Light's lighting-update queue, LevelGenerator)."""

import os
import tempfile

from j2ctool import pipeline, profiles


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


def test_int_array_cast_of_raw_list_remove():
    cpp = _convert({'Holder.java': (
        'import java.util.List;\nimport java.util.ArrayList;\n'
        'public class Holder { public List q = new ArrayList();\n'
        ' public int[] pop() { return (int[])this.q.remove(this.q.size() - 1); } }')})['Holder']
    assert '(JArray<int >)(this->q.remove(' in cpp
    assert '(int[])' not in cpp


def test_int_array_cast_field_chain():
    cpp = _convert({
        'Holder.java': ('import java.util.List;\nimport java.util.ArrayList;\n'
                        'public class Holder { public List q = new ArrayList(); }'),
        'User.java': ('public class User { public int[] f(Holder h) {\n'
                      '  return (int[])h.q.remove(h.q.size() - 1); } }'),
    })['User']
    assert '(JArray<int >)(h->q.remove(' in cpp
    assert '(int[])' not in cpp
