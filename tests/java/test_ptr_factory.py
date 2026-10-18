# -*- coding: utf-8 -*-
"""Regression: a stdlib static factory whose owning class is a runtime STUB (not in the parsed
corpus) still gets `->` on the result. Canonical: `Runtime.getRuntime().maxMemory()` -- Runtime.h
declares `static Runtime* getRuntime()`, so the memory getters must chain with `->`, not `.`
(C2228 'left of .maxMemory must have class/struct/union, type is Runtime *')."""

import os
import tempfile

from codebreaker.java import transpile as pipeline, profiles


def _convert_snippet(body):
    d = tempfile.mkdtemp()
    out = tempfile.mkdtemp()
    with open(os.path.join(d, 'Gui.java'), 'w', encoding='utf-8') as f:
        f.write('package net.minecraft.client.gui;\npublic class Gui {\n public void draw() {\n%s\n }\n}\n' % body)
    try:
        pipeline.convert(d, out, platform=None, edition='indev')
        return open(os.path.join(out, 'Gui.cpp'), encoding='utf-8').read()
    finally:
        profiles.use('minecraft_xbox360')


def test_runtime_getruntime_chains_with_arrow():
    cpp = _convert_snippet('  long a = Runtime.getRuntime().maxMemory();\n'
                           '  long b = Runtime.getRuntime().totalMemory();\n'
                           '  long c = Runtime.getRuntime().freeMemory();')
    assert 'Runtime::getRuntime()->maxMemory()' in cpp
    assert 'Runtime::getRuntime()->totalMemory()' in cpp
    assert 'Runtime::getRuntime()->freeMemory()' in cpp
    assert 'getRuntime().maxMemory' not in cpp        # the buggy `.` form is gone
