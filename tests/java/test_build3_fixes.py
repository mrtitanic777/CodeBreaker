# -*- coding: utf-8 -*-
"""Regressions for the third Indev build pass:
 * a STATIC method shadowing an inherited INSTANCE method (BlockFlowing.flow vs BlockFluid.flow) is
   renamed so it doesn't trip C4526 (static "cannot override" the base virtual).
 * `arrayField = null` -> `arrayField = JArray<T>()` (a JArray value can't hold NULL)."""

import os
import tempfile

from codebreaker.java import parse, transpile as pipeline, profiles
from codebreaker.java.transpile import build_method_renames


def test_static_shadowing_inherited_instance_is_renamed():
    base = parse.parse_source('public class Base { boolean flow(int x) { return false; } }')
    sub = parse.parse_source('public class Sub extends Base { static boolean flow(int x) { return true; } }')
    renames = build_method_renames([base, sub])
    assert 'flow' in renames['Sub']            # the static one is renamed (flow -> flow_)
    assert 'flow' not in renames.get('Base', set())   # the base's virtual is untouched


def test_non_shadowing_static_not_renamed():
    # a static method with no inherited instance method of that name must NOT be renamed
    c = parse.parse_source('public class C { static int helper(int x) { return x; } }')
    assert 'helper' not in build_method_renames([c]).get('C', set())


def test_static_overload_alongside_instance_override_not_renamed():
    # NoiseGeneratorPerlin: a static gen(double) HELPER coexists with the instance gen(double,double)
    # OVERRIDE of an abstract base. Different signatures -> legal C++ overload (no C4526); renaming by
    # name would kill the override and leave the class abstract (C2259). Neither must be renamed.
    base = parse.parse_source('public abstract class Base { public abstract double gen(double a, double b); }')
    sub = parse.parse_source('public final class Sub extends Base {\n'
                             ' private static double gen(double a) { return a; }\n'
                             ' public double gen(double a, double b) { return a + b; } }')
    renames = build_method_renames([base, sub])
    assert 'gen' not in renames['Sub']


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


def test_array_field_null_init_dropped():
    # `String[] levels = null` field initializer -> dropped (JArray default-constructs to empty); no `= NULL`
    cpp = _convert({'G.java': 'public class G { public String[] levels = null; }'})['G']
    assert 'levels = NULL' not in cpp


def test_array_field_null_reassign_becomes_empty():
    cpp = _convert({'G.java': (
        'public class G {\n public String[] levels;\n'
        ' public void clear() {\n  this.levels = null;\n } }')})['G']
    assert 'this->levels = JArray<JavaString >();' in cpp   # reassign -> empty JArray, not NULL
    assert 'this->levels = NULL' not in cpp
