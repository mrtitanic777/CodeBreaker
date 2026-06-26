# -*- coding: utf-8 -*-
"""Module 5 tests: end-to-end pipeline (discover -> parse -> emit -> verify -> report)."""

import os
import shutil
import tempfile
import unittest

from j2x import ir, pipeline


class TestInheritanceResolution(unittest.TestCase):
    def test_inherited_fields_populated(self):
        a = ir.JClass('A')
        a.fields.append(ir.Field('x', ir.JType('int'), access=ir.PUBLIC))
        a.fields.append(ir.Field('secret', ir.JType('int'), access=ir.PRIVATE))   # not inherited
        b = ir.JClass('B', parent='A')
        c = ir.JClass('C', parent='B')
        pipeline.resolve_inheritance([a, b, c])
        self.assertEqual([f.name for f in b.inherited_fields], ['x'])     # private skipped
        self.assertEqual([f.name for f in c.inherited_fields], ['x'])     # transitive through B
        self.assertEqual(a.inherited_fields, [])

FOO = """package a;
public class Foo {
    public int x;
    public Foo(int var1) { this.x = var1; }
    public int getX() { return this.x; }
}
"""
IBAR = """package a;
public interface IBar {
    void doIt();
}
"""


class TestPipeline(unittest.TestCase):
    def setUp(self):
        self.d = tempfile.mkdtemp()
        self.src = os.path.join(self.d, 'src')
        self.out = os.path.join(self.d, 'out')
        os.makedirs(self.src)
        with open(os.path.join(self.src, 'Foo.java'), 'w', encoding='utf-8') as f:
            f.write(FOO)
        with open(os.path.join(self.src, 'IBar.java'), 'w', encoding='utf-8') as f:
            f.write(IBAR)
        self.summary = pipeline.convert(self.src, self.out)

    def tearDown(self):
        shutil.rmtree(self.d, ignore_errors=True)

    def test_summary(self):
        self.assertEqual(self.summary['parsed'], 2)
        self.assertEqual(self.summary['headers'], 2)
        self.assertEqual(self.summary['sources'], 1)        # interface is header-only

    def test_files_written(self):
        self.assertTrue(os.path.isfile(os.path.join(self.out, 'Foo.h')))
        self.assertTrue(os.path.isfile(os.path.join(self.out, 'Foo.cpp')))
        self.assertTrue(os.path.isfile(os.path.join(self.out, 'IBar.h')))
        self.assertFalse(os.path.isfile(os.path.join(self.out, 'IBar.cpp')))

    def test_report_written(self):
        self.assertTrue(os.path.isfile(os.path.join(self.out, 'report', 'index.md')))

    def test_translation_applied(self):
        with open(os.path.join(self.out, 'Foo.cpp'), encoding='utf-8') as f:
            cpp = f.read()
        self.assertIn('this->x = var1;', cpp)
        self.assertIn('int Foo::getX()', cpp)


if __name__ == '__main__':
    unittest.main()
