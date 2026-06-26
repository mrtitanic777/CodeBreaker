# -*- coding: utf-8 -*-
"""Module 4c pass 1 tests: per-method symbol table + value/pointer access."""

import unittest

from j2x import ir, types
from j2x.emit import statements


def _world():
    c = ir.JClass('World')
    c.fields.append(ir.Field('blockID', ir.JType('int')))
    c.fields.append(ir.Field('provider', ir.JType('WorldProvider')))   # reference field -> pointer
    m = ir.Method('tick', ir.JType('void'),
                  params=[ir.Param('entity', ir.JType('Entity')), ir.Param('x', ir.JType('int'))], line=1)
    m.body = [
        (1, 'void tick(Entity entity, int x) {'),
        (2, '\tVec3D var1 = entity.getPos();'),
        (3, '\tvar1.xCoord = 5.0F;'),
        (4, '\tthis.blockID = x;'),
        (5, '\tBlock var2 = this.getBlock();'),
        (6, '\tvar2.onUpdate();'),
        (7, '\tthis.provider.doStuff();'),
        (8, '}'),
    ]
    c.methods.append(m)
    return c, m


class TestScope(unittest.TestCase):
    def test_build_scope_collects_params_fields_locals(self):
        c, m = _world()
        scope = statements.build_scope(c, m)
        self.assertEqual(repr(scope['entity']), 'Entity')      # param
        self.assertEqual(repr(scope['x']), 'int')              # param
        self.assertEqual(repr(scope['blockID']), 'int')        # field
        self.assertEqual(repr(scope['provider']), 'WorldProvider')
        self.assertEqual(repr(scope['var1']), 'Vec3D')         # local
        self.assertEqual(repr(scope['var2']), 'Block')         # local


class TestAccessTranslation(unittest.TestCase):
    def setUp(self):
        self.c, self.m = _world()
        self.r = types.TypeResolver(known_classes={'Entity', 'Block', 'World', 'WorldProvider'},
                                    value_types={'Vec3D'})
        self.out = statements.translate_body(self.m, self.c, self.r)

    def test_pointer_param_access(self):
        self.assertIn('Vec3D var1 = entity->getPos();', self.out)

    def test_value_local_keeps_dot(self):
        self.assertIn('var1.xCoord = 5.0F;', self.out)          # Vec3D is a value type

    def test_this_becomes_arrow(self):
        self.assertIn('this->blockID = x;', self.out)

    def test_reference_local_decl_gets_pointer(self):
        self.assertIn('Block* var2 = this->getBlock();', self.out)

    def test_pointer_local_access(self):
        self.assertIn('var2->onUpdate();', self.out)

    def test_chained_pointer_field(self):
        self.assertIn('this->provider->doStuff();', self.out)


if __name__ == '__main__':
    unittest.main()
