# -*- coding: utf-8 -*-
"""Module 4 tests: header emission."""

import os
import unittest

from j2x import ir, types, parse
from j2x.emit import header
from j2x.ledger import Ledger

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
JAVA = os.path.join(ROOT, 'tests', 'fixtures', 'java')


class TestHeaderHandBuilt(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('Foo', parent='Bar')
        c.fields.append(ir.Field('field', ir.JType('Block'), access=ir.PUBLIC))
        c.fields.append(ir.Field('count', ir.JType('int'), access=ir.PUBLIC))
        c.fields.append(ir.Field('list', ir.JType('Block', array_dims=1), is_static=True))
        c.fields.append(ir.Field('name', ir.JType('String'), is_static=True))
        c.methods.append(ir.Method('Foo', is_ctor=True, params=[ir.Param('id', ir.JType('int'))]))
        c.methods.append(ir.Method('doIt', ir.JType('void'),
                                   params=[ir.Param('b', ir.JType('Block'))]))
        self.c = c
        self.r = types.TypeResolver(known_classes={'Block', 'Bar'})
        self.h = header.render_header(c, self.r)

    def test_include_guard(self):
        self.assertTrue(self.h.startswith('#ifndef FOO_H\n#define FOO_H'))
        self.assertTrue(self.h.rstrip().endswith('#endif // FOO_H'))

    def test_parent_included_not_forward_declared(self):
        self.assertIn('#include "Bar.h"', self.h)
        self.assertNotIn('class Bar;', self.h)

    def test_reference_member_forward_declared(self):
        self.assertIn('class Block;', self.h)

    def test_class_declaration_with_base(self):
        self.assertIn('class Foo : public Bar {', self.h)

    def test_constructor_and_destructor(self):
        self.assertIn('Foo(int id);', self.h)
        self.assertIn('virtual ~Foo();', self.h)

    def test_fields(self):
        self.assertIn('\tBlock* field;', self.h)            # reference instance field -> pointer
        self.assertIn('\tint count;', self.h)
        self.assertIn('\tstatic JArray<Block* > s_list;', self.h)   # ref array -> JArray<Block*>, s_ prefix
        self.assertIn('\tstatic JavaString s_name;', self.h)   # String -> JavaString

    def test_string_pulls_its_header(self):
        self.assertIn('#include "JavaString.h"', self.h)     # String -> JavaString (subclass of std::string)

    def test_method_is_virtual(self):
        self.assertIn('virtual void doIt(Block* b);', self.h)


class TestHeaderRealFile(unittest.TestCase):
    def test_vec3d_header_structure(self):
        c = parse.parse_file(os.path.join(JAVA, 'Vec3D.java'))
        r = types.TypeResolver(value_types={'Vec3D'})
        led = Ledger()
        h = header.render_header(c, r, ledger=led)
        self.assertIn('#ifndef VEC3D_H', h)
        self.assertIn('class Vec3D', h)
        self.assertIn('double xCoord;', h)                  # value-type field stays bare (Alpha: double coords)
        self.assertIn('virtual ~Vec3D();', h)
        self.assertTrue(any(line.strip().startswith('Vec3D(') for line in h.split('\n')))
        self.assertIn('#endif // VEC3D_H', h)
        # the emitter reported a PORTED event for the header
        self.assertTrue(any(e.rule == 'header' for e in led.events))


class TestSignedByteFields(unittest.TestCase):
    """Java `byte` is SIGNED, so EVERY scalar byte field emits `signed char` by default (the flipped
    policy: -1 must not read back as 255). Only `byte[]` buffers stay unsigned char (read via & 0xFF).
    A specific scalar can be forced unsigned via typemap.UNSIGNED_BYTE_SCALAR_FIELDS (empty by default)."""

    def test_scalar_byte_emits_signed_char_and_buffer_stays_unsigned(self):
        c = ir.JClass('Packet1Login')
        c.fields.append(ir.Field('dimension', ir.JType('byte'), access=ir.PUBLIC))
        c.fields.append(ir.Field('payload', ir.JType('byte', array_dims=1), access=ir.PUBLIC))
        h = header.render_header(c, types.TypeResolver())
        self.assertIn('signed char dimension;', h)          # nether = -1
        self.assertNotIn('unsigned char dimension;', h)
        self.assertIn('unsigned char', h)                   # byte[] payload buffer stays unsigned
        self.assertNotIn('signed char payload', h)

    def test_any_scalar_byte_is_signed_by_default(self):
        c = ir.JClass('SomeClass')
        c.fields.append(ir.Field('flags', ir.JType('byte'), access=ir.PUBLIC))
        h = header.render_header(c, types.TypeResolver())
        self.assertIn('signed char flags;', h)              # flipped default (was unsigned)

    def test_unsigned_escape_hatch(self):
        from j2x import typemap
        typemap.UNSIGNED_BYTE_SCALAR_FIELDS.add(('SomeClass', 'raw'))
        try:
            c = ir.JClass('SomeClass')
            c.fields.append(ir.Field('raw', ir.JType('byte'), access=ir.PUBLIC))
            h = header.render_header(c, types.TypeResolver())
            self.assertIn('unsigned char raw;', h)
        finally:
            typemap.UNSIGNED_BYTE_SCALAR_FIELDS.discard(('SomeClass', 'raw'))


if __name__ == '__main__':
    unittest.main()
