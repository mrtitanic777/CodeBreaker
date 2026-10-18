# -*- coding: utf-8 -*-
"""Regressions for two bugs that together broke World.checkIfAABBIsClear1
(`entityMap.a((Entity)null, var1).size()`):

1. derive_nullability keyed nullable getters by NAME globally, so an obfuscated `a()` returning a
   nullable pointer in one class (Pathfinder.a -> PathPoint) mistyped `entityMap.a()` (returns List).
   Fixed with an ambiguity guard (a name with inconsistent return types is not a global getter).
2. _deref_cast_results treated a CALL-ARG cast (`a((Entity*)NULL, ...)`) as a grouping cast and derefed
   the CALL RESULT's `.size()` -> `->size()` on a List value. Fixed by skipping call/index arg parens."""

import os
import tempfile

from j2ctool import pipeline, profiles
from j2ctool.emit.statements import _deref_cast_results


class _Res:  # minimal resolver: every cast type is a pointer
    def is_pointer(self, jt):
        return True


def test_deref_skips_call_arg_cast():
    s = 'x->a((Entity*)NULL, var1).size()'
    assert _deref_cast_results(s, _Res()) == s            # .size() (on the result) stays '.'


def test_deref_still_handles_grouping_cast():
    out = _deref_cast_results('y = ((Block*)obj).getName();', _Res())
    assert ')->getName()' in out                          # ((Block*)obj).m -> ->m (genuine cast grouping)


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


def test_end_to_end_list_result_size_stays_dot():
    cpp = _convert({
        'Entity.java': 'public class Entity { public int id; }',
        'AABB.java': 'public class AABB { public double x; }',
        'EntityMap.java': ('import java.util.List;\nimport java.util.ArrayList;\n'
                           'public class EntityMap { public List es = new ArrayList();\n'
                           ' public final List a(Entity e, AABB bb) { return this.es; } }'),
        'World.java': ('public class World { public EntityMap entityMap;\n'
                       ' public boolean f(AABB var1) { return this.entityMap.a((Entity)null, var1).size() == 0; } }'),
        # obfuscated collision: Pathfinder.a returns a nullable pointer -> must NOT type EntityMap.a
        'Pathfinder.java': 'public class Pathfinder { public PathPoint a(int x){ if(x<0) return null; return new PathPoint(); } }',
        'PathPoint.java': 'public class PathPoint { public int idx; }',
    })['World']
    assert 'a((Entity*)NULL, var1).size()' in cpp          # '.' not '->', and no PathPoint* hoist
    assert '->size()' not in cpp
    assert 'PathPoint' not in cpp                           # the mis-typed hoist is gone
