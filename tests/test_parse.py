# -*- coding: utf-8 -*-
"""Module 2 tests: Java -> IR parser. Uses real decompiled fixtures in original_java/."""

import glob
import os
import unittest

from j2x import parse, ir

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
JAVA = os.path.join(ROOT, 'original_java')


def _has(name):
    return os.path.isfile(os.path.join(JAVA, name + '.java'))


class TestValueType(unittest.TestCase):
    def setUp(self):
        self.c = parse.parse_file(os.path.join(JAVA, 'Vec3D.java'))

    def test_header(self):
        self.assertEqual(self.c.name, 'Vec3D')
        self.assertEqual(self.c.package, 'net.minecraft.game.physics')
        self.assertFalse(self.c.is_interface)
        self.assertIsNone(self.c.parent)

    def test_fields(self):
        self.assertEqual([f.name for f in self.c.fields], ['xCoord', 'yCoord', 'zCoord'])
        self.assertTrue(all(repr(f.jtype) == 'float' for f in self.c.fields))
        self.assertTrue(all(not f.is_static for f in self.c.fields))

    def test_constructor(self):
        ctors = self.c.constructors()
        self.assertEqual(len(ctors), 1)
        self.assertEqual(ctors[0].arity, 3)
        self.assertTrue(ctors[0].is_ctor)

    def test_method_signature_and_body(self):
        m = self.c.method('subtract')
        self.assertEqual(repr(m.return_type), 'Vec3D')
        self.assertEqual([(p.name, repr(p.jtype)) for p in m.params], [('var1', 'Vec3D')])
        self.assertEqual(m.access, ir.PUBLIC)
        self.assertTrue(len(m.body) >= 1)            # body lines captured with line numbers
        self.assertTrue(all(isinstance(ln, int) for ln, _ in m.body))


class TestInterface(unittest.TestCase):
    def test_interface_method_is_abstract(self):
        c = parse.parse_file(os.path.join(JAVA, 'ICamera.java'))
        self.assertTrue(c.is_interface)
        self.assertEqual(len(c.methods), 1)
        m = c.methods[0]
        self.assertEqual(m.name, 'isBoundingBoxInFrustrum')
        self.assertTrue(m.is_abstract)
        self.assertEqual(m.body, [])
        self.assertEqual(repr(m.params[0].jtype), 'AxisAlignedBB')


class TestEnum(unittest.TestCase):
    def test_enum_constants(self):
        c = parse.parse_file(os.path.join(JAVA, 'EnumOS.java'))
        self.assertTrue(c.is_enum)
        self.assertEqual([e.name for e in c.enum_consts],
                         ['linux', 'solaris', 'windows', 'macos', 'unknown'])


class TestBlock(unittest.TestCase):
    def setUp(self):
        self.c = parse.parse_file(os.path.join(JAVA, 'Block.java'))

    def test_two_constructors(self):
        arities = sorted(m.arity for m in self.c.constructors())
        self.assertEqual(arities, [2, 3])            # the blockID=0 / container-flag ctors

    def test_static_array_field_with_initializer(self):
        f = self.c.field('blocksList')
        self.assertIsNotNone(f)
        self.assertTrue(f.is_static)
        self.assertTrue(f.is_final)
        self.assertEqual(f.jtype, ir.JType('Block', array_dims=1))
        self.assertEqual(f.initializer, 'new Block[256]')

    def test_forward_declared_static_has_no_initializer(self):
        f = self.c.field('stone')                    # `public static final Block stone;`
        self.assertIsNotNone(f)
        self.assertTrue(f.is_static)
        self.assertIsNone(f.initializer)


class TestCorpusSmoke(unittest.TestCase):
    """The parser must survive every real file without throwing and produce a named class."""
    def test_parse_all_original_java(self):
        files = glob.glob(os.path.join(JAVA, '*.java'))
        self.assertGreater(len(files), 100)
        failures = []
        for path in files:
            try:
                c = parse.parse_file(path)
                if not c.name:
                    failures.append((os.path.basename(path), 'no class name'))
            except Exception as e:                    # noqa: BLE001
                failures.append((os.path.basename(path), str(e)))
        self.assertEqual(failures, [], 'parser failed on %d files: %s'
                         % (len(failures), failures[:10]))


if __name__ == '__main__':
    unittest.main()
