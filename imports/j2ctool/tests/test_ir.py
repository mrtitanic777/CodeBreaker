# -*- coding: utf-8 -*-
"""Module 1 tests: the IR data shapes (the parse<->emit contract)."""

import unittest

from j2ctool import ir


class TestJType(unittest.TestCase):
    def test_scalar(self):
        t = ir.JType('int')
        self.assertFalse(t.is_array)
        self.assertFalse(t.is_generic)
        self.assertEqual(repr(t), 'int')

    def test_array_dims_and_element(self):
        t = ir.JType('int', array_dims=2)
        self.assertTrue(t.is_array)
        self.assertEqual(repr(t), 'int[][]')
        self.assertEqual(t.element(), ir.JType('int', array_dims=1))
        self.assertEqual(t.element().element(), ir.JType('int'))

    def test_element_of_scalar_is_self(self):
        t = ir.JType('Block')
        self.assertEqual(t.element(), t)

    def test_generic(self):
        t = ir.JType('List', args=[ir.JType('ItemStack')])
        self.assertTrue(t.is_generic)
        self.assertEqual(repr(t), 'List<ItemStack>')

    def test_equality_and_hash(self):
        self.assertEqual(ir.JType('int', 1), ir.JType('int', 1))
        self.assertNotEqual(ir.JType('int', 1), ir.JType('int', 2))
        self.assertNotEqual(ir.JType('int'), ir.JType('long'))
        # usable as dict/set keys
        s = {ir.JType('int'), ir.JType('int'), ir.JType('long')}
        self.assertEqual(len(s), 2)


class TestMethod(unittest.TestCase):
    def test_arity_and_signature(self):
        m = ir.Method('setBlock', ir.JType('void'),
                      params=[ir.Param('x', ir.JType('int')),
                              ir.Param('b', ir.JType('Block'))])
        self.assertEqual(m.arity, 2)
        self.assertEqual(m.param_types, [ir.JType('int'), ir.JType('Block')])
        self.assertEqual(m.signature(), ('setBlock', ('int', 'Block')))

    def test_ctor_flag(self):
        m = ir.Method('Block', is_ctor=True)
        self.assertTrue(m.is_ctor)
        self.assertEqual(m.arity, 0)


class TestJClass(unittest.TestCase):
    def _block(self):
        c = ir.JClass('Block', parent='Object')
        c.fields.append(ir.Field('blockID', ir.JType('int'), access=ir.PUBLIC))
        c.fields.append(ir.Field('blocksList', ir.JType('Block', array_dims=1),
                                 is_static=True, initializer='new Block[256]'))
        c.methods.append(ir.Method('Block', is_ctor=True,
                                   params=[ir.Param('id', ir.JType('int'))]))
        c.methods.append(ir.Method('getBlockID', ir.JType('int')))
        return c

    def test_field_lookup(self):
        c = self._block()
        self.assertIsNotNone(c.field('blockID'))
        self.assertIsNone(c.field('nope'))

    def test_static_vs_instance(self):
        c = self._block()
        self.assertEqual([f.name for f in c.instance_fields()], ['blockID'])
        self.assertEqual([f.name for f in c.static_fields()], ['blocksList'])

    def test_constructors(self):
        c = self._block()
        ctors = c.constructors()
        self.assertEqual(len(ctors), 1)
        self.assertTrue(ctors[0].is_ctor)

    def test_owns_heap_field_signal(self):
        c = self._block()
        self.assertTrue(c.owns_heap_field())   # blocksList = new Block[256]
        plain = ir.JClass('Plain')
        plain.fields.append(ir.Field('x', ir.JType('int')))
        self.assertFalse(plain.owns_heap_field())


if __name__ == '__main__':
    unittest.main()
