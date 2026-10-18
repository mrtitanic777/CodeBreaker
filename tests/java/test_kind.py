# -*- coding: utf-8 -*-
"""The java kind on CodeBreaker's path (verify/java.py through verify/reconstruct.py, the store, and
source/java_writer.py): a folder of Java source is a module, its classes are its functions, each is
compiled on its own and filed NOT JUDGED (compiles, not run) or DOES NOT COMPILE."""

import os
import shutil

import pytest

from codebreaker.verify import java, reconstruct

HERE = os.path.dirname(os.path.abspath(__file__))
FIXTURES = os.path.join(HERE, 'fixtures', 'java')
CLASSES = sorted(os.path.splitext(f)[0] for f in os.listdir(FIXTURES) if f.endswith('.java'))


def test_a_folder_of_java_is_a_java_module(tmp_path):
    assert reconstruct.kind_name(FIXTURES) == 'java'
    assert reconstruct.kind_name(os.path.join(FIXTURES, 'Block.java')) == 'java'
    assert reconstruct.kind_name(str(tmp_path)) is None          # a folder with no .java in it
    jar = tmp_path / 'game.jar'
    jar.write_bytes(b'PK')
    assert reconstruct.kind_name(str(jar)) == 'java'


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


@pytest.mark.skipif(not java.compiler(), reason='needs a host C++ compiler (g++)')
def test_classes_are_judged_filed_and_written(tmp_path, monkeypatch):
    from codebreaker.store import functions as F
    from codebreaker.source import module_writer
    monkeypatch.setenv('CB_FUNCTIONS_DB', str(tmp_path / 'functions.sqlite'))
    fs = F.open()
    sha, digest = reconstruct.run(FIXTURES, fs=fs, log=lambda *_: None)
    best = fs.best(sha, digest)
    assert sorted(best) == CLASSES
    for cls, row in best.items():
        assert row['class'] in ('not-judged', 'verified'), (cls, row.get('reason'), row.get('detail'))
    if java.jdk()[0]:                       # MathHelper's static float methods run beside the JVM, and agree
        assert best['MathHelper']['class'] == 'verified', best['MathHelper'].get('detail')
        assert 'every result alike' in best['MathHelper']['detail']
    # nothing changed: nothing is judged again
    seen = []
    reconstruct.run(FIXTURES, fs=fs, log=seen.append)
    assert ' 0 judged now' in seen[0]
    # the C++ folder, each class labelled, and STATUS.md
    out = tmp_path / 'cpp'
    module_writer.write_module(FIXTURES, verify=False, out_dir=str(out), log=lambda *_: None)
    head = (out / 'Block.cpp').read_text(encoding='utf-8').splitlines()[:2]
    assert 'by CodeBreaker' in head[0] and head[1].startswith('// [NOT JUDGED]')
    assert '| Block | NOT JUDGED |' in (out / 'STATUS.md').read_text(encoding='utf-8')


def test_a_class_that_does_not_compile_says_why(tmp_path):
    if not java.compiler():
        pytest.skip('needs a host C++ compiler (g++)')
    bad = tmp_path / 'Bad.cpp'
    bad.write_text('int f() { return undeclared_name; }\n', encoding='utf-8')
    verdict, reason, limits, detail = java.judge((java.compiler(), str(tmp_path), None, '', '', '', str(tmp_path)), ('Bad', str(bad), ''))
    assert verdict == 'no-compile' and 'undeclared_name' in reason and limits == []
