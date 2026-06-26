# -*- coding: utf-8 -*-
"""Module 4b tests: .cpp emission + crash-class safety by construction."""

import os
import unittest

from j2x import ir, types, parse
from j2x.emit import source, header
from j2x.ledger import Ledger, Category

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
JAVA = os.path.join(ROOT, 'original_java')


def _block():
    c = ir.JClass('Block')
    c.fields.append(ir.Field('tickOnLoad', ir.JType('boolean', 1), is_static=True,
                             initializer='new boolean[256]', line=5))
    c.fields.append(ir.Field('blocksList', ir.JType('Block', 1), is_static=True,
                             initializer='new Block[256]', line=6))
    c.fields.append(ir.Field('stone', ir.JType('Block'), is_static=True, initializer=None, line=7))
    c.fields.append(ir.Field('blockID', ir.JType('int'), access=ir.PUBLIC))
    c.fields.append(ir.Field('stepSound', ir.JType('StepSound')))
    c.fields.append(ir.Field('slipperiness', ir.JType('float')))

    ctor = ir.Method('Block', is_ctor=True,
                     params=[ir.Param('var1', ir.JType('int')), ir.Param('var2', ir.JType('Material'))], line=20)
    ctor.body = [(20, 'Block(int var1, Material var2) {'),
                 (21, '\tthis.stepSound = new StepSound();'),
                 (22, '\tthis.blockID = var1;'), (23, '}')]
    c.methods.append(ctor)

    deleg = ir.Method('Block', is_ctor=True,
                      params=[ir.Param('var1', ir.JType('int')), ir.Param('var2', ir.JType('int')),
                              ir.Param('var3', ir.JType('Material'))], line=26)
    deleg.body = [(26, 'Block(int var1, int var2, Material var3) {'),
                  (27, '\tthis(var1, var3);'), (28, '\tthis.blockID = var2;'), (29, '}')]
    c.methods.append(deleg)

    gm = ir.Method('getBlockID', ir.JType('int'), line=32)
    gm.body = [(32, 'int getBlockID() {'), (33, '\treturn this.blockID;'), (34, '}')]
    c.methods.append(gm)
    return c


class TestSourceSafety(unittest.TestCase):
    def setUp(self):
        self.c = _block()
        self.r = types.TypeResolver(known_classes={'Block', 'StepSound', 'Material'})
        self.led = Ledger()
        self.cpp = source.render_source(self.c, self.r, ledger=self.led)

    def test_header(self):
        self.assertTrue(self.cpp.startswith('#include "stdafx.h"'))
        self.assertIn('#include "Block.h"', self.cpp)

    def test_uninit_primitive_array_value_initialized(self):
        # boolean[] -> JArray<bool>, zero-filled + length-tracked like Java
        self.assertIn('JArray<bool > Block::s_tickOnLoad = JArray<bool >(256);', self.cpp)

    def test_uninit_pointer_array_value_initialized(self):
        # Block[] -> JArray<Block*> (length-tracked array of pointers), value-init to NULLs
        self.assertIn('JArray<Block* > Block::s_blocksList = JArray<Block* >(256);', self.cpp)

    def test_forward_declared_static_is_nulled(self):
        self.assertIn('Block* Block::s_stone = NULL;', self.cpp)

    def test_ctor_zero_inits_members(self):
        self.assertIn('blockID(0)', self.cpp)
        self.assertIn('stepSound(NULL)', self.cpp)
        self.assertIn('slipperiness(0)', self.cpp)

    def test_rule_of_three_destructor_frees_heap(self):
        self.assertIn('Block::~Block()', self.cpp)
        self.assertIn('delete stepSound;', self.cpp)
        self.assertIn('stepSound = NULL;', self.cpp)
        self.assertTrue(any(e.rule == 'rule-of-three-dtor' for e in self.led.events))

    def test_delegation_inlined(self):
        # the raw this(...) call is gone; the target ctor's registration is inlined
        self.assertNotIn('this(var1, var3);', self.cpp)
        self.assertIn('// inlined this(var1, var3)', self.cpp)
        # the 2-arg ctor's body (stepSound alloc + blockID set) now runs in the 3-arg ctor too
        self.assertEqual(self.cpp.count('this->stepSound = new StepSound();'), 2)
        evs = [e for e in self.led.events if e.rule == 'ctor-delegation-inlined']
        self.assertTrue(evs and evs[0].category == Category.RULE)

    def test_method_signature(self):
        self.assertIn('int Block::getBlockID()', self.cpp)

    def test_uninit_array_events_logged(self):
        self.assertEqual(sum(1 for e in self.led.events if e.rule == 'uninit-array-fixed'), 2)


class TestSuperCtor(unittest.TestCase):
    def test_super_call_moves_to_init_list(self):
        c = ir.JClass('Sub', parent='Base')
        c.fields.append(ir.Field('x', ir.JType('int')))
        ctor = ir.Method('Sub', is_ctor=True, params=[ir.Param('var1', ir.JType('int'))], line=1)
        ctor.body = [(1, 'Sub(int var1) {'), (2, '\tsuper(var1, 5);'), (3, '\tthis.x = var1;'), (4, '}')]
        c.methods.append(ctor)
        cpp = source.render_source(c, types.TypeResolver(known_classes={'Base'}))
        self.assertIn(': Base(var1, 5)', cpp)
        self.assertNotIn('super(var1, 5);', cpp)
        self.assertIn('this->x = var1;', cpp)


class TestRuleOfThreeNoCopy(unittest.TestCase):
    """Heap-corruption defense: owning classes forbid copying (no double-free)."""
    def test_owning_class_disables_copy(self):
        c = _block()                                   # ctor does this.stepSound = new StepSound()
        r = types.TypeResolver(known_classes={'Block', 'StepSound', 'Material'})
        led = Ledger()
        h = header.render_header(c, r, ledger=led)
        self.assertIn('private:', h)
        self.assertIn('Block(const Block&);', h)
        self.assertIn('Block& operator=(const Block&);', h)
        self.assertTrue(any(e.rule == 'rule-of-three-nocopy' for e in led.events))

    def test_value_type_keeps_copy(self):
        c = parse.parse_file(os.path.join(JAVA, 'Vec3D.java'))   # owns no heap
        r = types.TypeResolver(value_types={'Vec3D'})
        h = header.render_header(c, r)
        self.assertNotIn('Vec3D(const Vec3D&);', h)
        self.assertNotIn('private:', h)

    def test_array_field_does_not_disable_copy(self):
        # arrays are non-owning JArray<T> values now, so an array-element assignment does NOT
        # force copy-disable (a shallow copy shares the buffer; no double-free)
        c = ir.JClass('FX')
        c.fields.append(ir.Field('fxLayers', ir.JType('List', array_dims=1)))
        ctor = ir.Method('FX', is_ctor=True, line=1)
        ctor.body = [(1, 'FX() {'), (2, '\tthis.fxLayers[0] = new ArrayList();'), (3, '}')]
        c.methods.append(ctor)
        h = header.render_header(c, types.TypeResolver())
        self.assertNotIn('FX(const FX&);', h)

    def test_pointer_field_owner_disables_copy(self):
        # a real pointer field new'd in the ctor still owns heap -> copy disabled (Rule of Three)
        c = ir.JClass('W')
        c.fields.append(ir.Field('prov', ir.JType('WorldProvider')))
        ctor = ir.Method('W', is_ctor=True, line=1)
        ctor.body = [(1, 'W() {'), (2, '\tthis.prov = new WorldProvider();'), (3, '}')]
        c.methods.append(ctor)
        h = header.render_header(c, types.TypeResolver(known_classes={'WorldProvider'}))
        self.assertIn('W(const W&);', h)


class TestSourceRealFiles(unittest.TestCase):
    def test_vec3d_source_renders(self):
        c = parse.parse_file(os.path.join(JAVA, 'Vec3D.java'))
        r = types.TypeResolver(value_types={'Vec3D'})
        cpp = source.render_source(c, r)
        self.assertIn('#include "stdafx.h"', cpp)
        self.assertIn('Vec3D::Vec3D(', cpp)
        self.assertIn('Vec3D::~Vec3D()', cpp)

    def test_interface_has_no_source(self):
        c = parse.parse_file(os.path.join(JAVA, 'ICamera.java'))
        r = types.TypeResolver()
        self.assertIsNone(source.render_source(c, r))

    def test_dual_use_return_promoted_to_pointer(self):
        # Vec3D is a value type, but subtract() does `return new Vec3D(...)` -> Vec3D*
        c = parse.parse_file(os.path.join(JAVA, 'Vec3D.java'))
        r = types.TypeResolver(value_types={'Vec3D'})
        cpp = source.render_source(c, r)
        self.assertIn('Vec3D* Vec3D::subtract(Vec3D var1)', cpp)
        self.assertIn('return NULL;', cpp)          # Java null -> NULL
        self.assertNotIn('return null;', cpp)
        h = header.render_header(c, r)
        self.assertIn('virtual Vec3D* subtract(Vec3D var1);', h)


class TestAggregateArrayStatic(unittest.TestCase):
    """`static T[] x = new T[]{a, b}` -> a JArray over a file-static data array (C++03 has no
    braced new[]), with element refs translated (Block.planks -> Block::s_planks)."""
    def setUp(self):
        c = ir.JClass('ItemAxe')
        c.fields.append(ir.Field('blocksEffectiveAgainst', ir.JType('Block', array_dims=1),
                                 is_static=True, is_final=True,
                                 initializer='new Block[]{Block.planks, Block.wood}', line=5))
        self.r = types.TypeResolver(known_classes={'Block', 'ItemAxe'})
        self.h = header.render_header(c, self.r)
        self.cpp = source.render_source(c, self.r)

    def test_header_declares_jarray_static(self):
        self.assertIn('static JArray<Block* > s_blocksEffectiveAgainst;', self.h)

    def test_source_defines_translated_array(self):
        self.assertIn('static Block* _ItemAxe_s_blocksEffectiveAgainst_data[] = '
                      '{ Block::s_planks, Block::s_wood };', self.cpp)
        self.assertIn('JArray<Block* > ItemAxe::s_blocksEffectiveAgainst('
                      '_ItemAxe_s_blocksEffectiveAgainst_data, 2);', self.cpp)
        self.assertNotIn('new Block[]', self.cpp)        # no invalid braced new[]


class TestNullableValueParams(unittest.TestCase):
    """A value-struct param used as a nullable reference (compared/assigned null) becomes a
    pointer in the signature -- callers in the raytrace dual-use chain pass Vec3D* already."""
    def setUp(self):
        c = ir.JClass('AxisAlignedBB')
        m = ir.Method('isVecInYZ', ir.JType('boolean'),
                      params=[ir.Param('var1', ir.JType('Vec3D'))], line=1)
        m.body = [(1, 'boolean isVecInYZ(Vec3D var1) {'),
                  (2, '\treturn var1 == null ? false : var1.yCoord > this.minY;'), (3, '}')]
        c.methods.append(m)
        self.r = types.TypeResolver(value_types={'Vec3D'})
        self.h = header.render_header(c, self.r)
        self.cpp = source.render_source(c, self.r)

    def test_header_param_is_pointer(self):
        self.assertIn('virtual bool isVecInYZ(Vec3D* var1);', self.h)

    def test_source_param_is_pointer_and_body_derefs(self):
        self.assertIn('bool AxisAlignedBB::isVecInYZ(Vec3D* var1)', self.cpp)
        self.assertTrue(any('var1->yCoord' in l for l in self.cpp.splitlines()), self.cpp)

    def test_non_nullable_value_param_stays_value(self):
        c = ir.JClass('Vec3D')
        m = ir.Method('distance', ir.JType('float'),
                      params=[ir.Param('var1', ir.JType('Vec3D'))], line=1)
        m.body = [(1, 'float distance(Vec3D var1) {'),
                  (2, '\treturn var1.xCoord - this.xCoord;'), (3, '}')]   # no null use
        c.methods.append(m)
        h = header.render_header(c, types.TypeResolver(value_types={'Vec3D'}))
        self.assertIn('virtual float distance(Vec3D var1);', h)   # stays value


class TestJavaStringMapping(unittest.TestCase):
    """String maps to JavaString (a std::string subclass) and every header includes it."""
    def test_header_includes_javastring_and_uses_it(self):
        c = ir.JClass('C')
        c.fields.append(ir.Field('name', ir.JType('String')))
        m = ir.Method('greet', ir.JType('String'),
                      params=[ir.Param('who', ir.JType('String'))], line=1)
        c.methods.append(m)
        h = header.render_header(c, types.TypeResolver())
        self.assertIn('#include "JavaString.h"', h)
        self.assertIn('JavaString name;', h)                       # field by value
        self.assertIn('virtual JavaString greet(const JavaString& who);', h)  # return value, param const&


class TestTransitiveIncludes(unittest.TestCase):
    """A .cpp that holds a T and reaches T's field/method types through chains must
    #include their full definitions (one level), driven by the member environment."""
    def test_field_type_of_referenced_class_is_included(self):
        item = ir.JClass('ItemHoe')
        m = ir.Method('use', ir.JType('void'), params=[ir.Param('var2', ir.JType('World'))], line=1)
        m.body = [(1, 'void use(World var2) {'),
                  (2, '\tint n = var2.random.nextInt(8);'), (3, '}')]   # var2.random -> Random*
        item.methods.append(m)
        # member env: World has a Random* field 'random' (as the pipeline would build it)
        members = {'World': {'fields': {'random': ir.JType('Random')}, 'methods': {}}}
        r = types.TypeResolver(known_classes={'World', 'ItemHoe', 'Random'}, members=members)
        cpp = source.render_source(item, r)
        self.assertIn('#include "World.h"', cpp)
        self.assertIn('#include "Random.h"', cpp)    # transitively, via World's field type


class TestStringReturnUnification(unittest.TestCase):
    """String is always returned by value (std::string), never promoted to a pointer --
    even when the body `return null` -- so virtual overrides keep a consistent return type."""
    def _string_returner(self, body_lines):
        c = ir.JClass('Mob', parent='Entity')
        m = ir.Method('getTexture', ir.JType('String'), line=1)
        m.body = [(1, 'String getTexture() {')] + list(enumerate(body_lines, 2)) + [(99, '}')]
        c.methods.append(m)
        return c

    def test_null_string_return_stays_value(self):
        c = self._string_returner(['\treturn null;'])
        r = types.TypeResolver(known_classes={'Entity'})
        h = header.render_header(c, r)
        cpp = source.render_source(c, r)
        self.assertIn('virtual JavaString getTexture();', h)   # String -> JavaString, NOT a pointer
        self.assertNotIn('JavaString* getTexture', h)
        self.assertIn('return std::string();', cpp)             # null -> empty, never NULL char*
        self.assertNotIn('return NULL;', cpp)

    def test_literal_string_return_value(self):
        c = self._string_returner(['\treturn "/mob/char.png";'])
        r = types.TypeResolver(known_classes={'Entity'})
        self.assertIn('virtual JavaString getTexture();', header.render_header(c, r))


if __name__ == '__main__':
    unittest.main()
