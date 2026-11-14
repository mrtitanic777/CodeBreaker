# -*- coding: utf-8 -*-
"""The java kind's lane (verify/lanes/java.py): a folder of Java source is a module, its classes are its functions, each is
compiled on its own and filed NOT JUDGED (compiles, not run) or DOES NOT COMPILE. (Native: verify.lanes.java.*.)"""

import os
import shutil

import pytest

from codebreaker.verify.lanes import java

HERE = os.path.dirname(os.path.abspath(__file__))
FIXTURES = os.path.join(HERE, 'fixtures', 'java')


def test_a_jar_asks_to_be_decompiled_first(tmp_path):
    jar = tmp_path / 'game.jar'
    jar.write_bytes(b'PK')
    with pytest.raises(SystemExit, match='decompile'):
        java.prepare(str(jar), str(tmp_path / 'work'))


def test_results_are_filed_under_the_source(tmp_path):
    src = tmp_path / 'src'
    shutil.copytree(FIXTURES, str(src))
    first = java.sha(str(src))
    assert first == java.sha(str(src))                           # stable
    with open(str(src / 'Block.java'), 'a', encoding='utf-8') as f:
        f.write('\n// edited\n')
    assert java.sha(str(src)) != first                           # a changed class is a changed module


def test_a_class_that_does_not_compile_says_why(tmp_path):
    if not java.compiler():
        pytest.skip('needs a host C++ compiler (g++)')
    bad = tmp_path / 'Bad.cpp'
    bad.write_text('int f() { return undeclared_name; }\n', encoding='utf-8')
    verdict, reason, limits, detail = java.judge((java.compiler(), str(tmp_path), None, '', '', '', str(tmp_path), {}), ('Bad', str(bad), ''))
    assert verdict == 'no-compile' and 'undeclared_name' in reason and limits == []
