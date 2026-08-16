# -*- coding: utf-8 -*-
"""Module 4b tests: .cpp emission + crash-class safety by construction."""

import os
import unittest

from j2ctool import ir, types, parse
from j2ctool.emit import source, header
from j2ctool.ledger import Ledger, Category

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
JAVA = os.path.join(ROOT, 'tests', 'fixtures', 'java')


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
        # Vec3D is a value type, but getIntermediateWithXValue() returns `createVector(...) : null`
        # -- a Vec3D-or-null result -> promoted to Vec3D*. (Alpha's subtract() returns a value.)
        c = parse.parse_file(os.path.join(JAVA, 'Vec3D.java'))
        r = types.TypeResolver(value_types={'Vec3D'})
        cpp = source.render_source(c, r)
        self.assertIn('Vec3D* Vec3D::getIntermediateWithXValue(Vec3D var1, double var2)', cpp)
        self.assertIn('return NULL;', cpp)          # Java null -> NULL
        self.assertNotIn('return null;', cpp)
        h = header.render_header(c, r)
        self.assertIn('virtual Vec3D* getIntermediateWithXValue(Vec3D var1, double var2);', h)


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


class TestNestedAggregateArrayStatic(unittest.TestCase):
    """`static int[][] x = new int[][]{{...},{...}}` -> nested JArrayInit (C++03 has no braced
    new[][]). The 2-D `new T[][]{...}` field must NOT survive as a C bracket-init (the C2087
    'missing subscript' crash class); it builds JArray<JArray<int> > via the comma-operator builder."""
    def setUp(self):
        c = ir.JClass('ModelBed')
        c.fields.append(ir.Field('bedDirection', ir.JType('int', array_dims=2),
                                 is_static=True, is_final=True,
                                 initializer='new int[][]{{1, 0}, {3, 2}}', line=6))
        self.r = types.TypeResolver(known_classes={'ModelBed'})
        self.h = header.render_header(c, self.r)
        self.cpp = source.render_source(c, self.r)

    def test_header_declares_nested_jarray_static(self):
        self.assertIn('static JArray<JArray<int > > s_bedDirection;', self.h)

    def test_no_braced_new_array(self):
        self.assertNotIn('new int[]', self.cpp)          # no invalid braced new[][] (C2087)

    def test_source_builds_nested_jarrayinit(self):
        # zero-init def now, real nested value assigned in initStatics()
        self.assertIn('JArray<JArray<int > > ModelBed::s_bedDirection;', self.cpp)
        self.assertIn('ModelBed::s_bedDirection = (JArrayInit<JArray<int > >(), '
                      '(JArrayInit<int >(), 1, 0), (JArrayInit<int >(), 3, 2));', self.cpp)


class TestValueTypeDefaultCtor(unittest.TestCase):
    """A value-type class with only parameterized ctors gets a synthesized zero-init default
    ctor, so it can be held by value as a field (AxisAlignedBB boundingBox;)."""
    def setUp(self):
        c = ir.JClass('AxisAlignedBB')
        c.fields.append(ir.Field('minX', ir.JType('double')))
        c.fields.append(ir.Field('hit', ir.JType('Vec3D')))
        ctor = ir.Method('AxisAlignedBB', is_ctor=True,
                         params=[ir.Param('var1', ir.JType('double'))], line=1)
        ctor.body = [(1, 'AxisAlignedBB(double var1) {'), (2, '\tthis.minX = var1;'), (3, '}')]
        c.methods.append(ctor)
        self.r = types.TypeResolver(value_types={'AxisAlignedBB', 'Vec3D'})
        self.h = header.render_header(c, self.r)
        self.cpp = source.render_source(c, self.r)

    def test_header_declares_default_ctor(self):
        self.assertIn('AxisAlignedBB();', self.h)

    def test_source_defines_zero_init_default_ctor(self):
        self.assertIn('AxisAlignedBB::AxisAlignedBB()', self.cpp)
        self.assertIn('minX(0)', self.cpp)            # zero-inits primitive members

    def test_no_synth_when_default_ctor_exists(self):
        c = ir.JClass('Vec3D')
        ctor = ir.Method('Vec3D', is_ctor=True, line=1)   # already a 0-arg ctor
        ctor.body = [(1, 'Vec3D() {'), (2, '}')]
        c.methods.append(ctor)
        h = header.render_header(c, types.TypeResolver(value_types={'Vec3D'}))
        self.assertEqual(h.count('\tVec3D();'), 1)    # the parsed ctor only; not duplicated


class TestChainedCtorDelegation(unittest.TestCase):
    """Chained this() delegation (A -> B -> C) must recursively inline,
    not silently drop the inner delegation (the ItemStack itemID=0 bug)."""
    def setUp(self):
        c = ir.JClass('ItemStack')
        c.fields.append(ir.Field('itemID', ir.JType('int')))
        c.fields.append(ir.Field('stackSize', ir.JType('int')))
        c.fields.append(ir.Field('itemDamage', ir.JType('int')))
        # 3-arg: the leaf — actually assigns fields
        ctor3 = ir.Method('ItemStack', is_ctor=True,
                          params=[ir.Param('var1', ir.JType('int')),
                                  ir.Param('var2', ir.JType('int')),
                                  ir.Param('var3', ir.JType('int'))], line=10)
        ctor3.body = [(10, 'ItemStack(int var1, int var2, int var3) {'),
                      (11, '\tthis.itemID = var1;'),
                      (12, '\tthis.stackSize = var2;'),
                      (13, '\tthis.itemDamage = var3;'), (14, '}')]
        c.methods.append(ctor3)
        # 2-arg: delegates to 3-arg
        ctor2 = ir.Method('ItemStack', is_ctor=True,
                          params=[ir.Param('var1', ir.JType('int')),
                                  ir.Param('var2', ir.JType('int'))], line=16)
        ctor2.body = [(16, 'ItemStack(int var1, int var2) {'),
                      (17, '\tthis(var1, var2, 0);'), (18, '}')]
        c.methods.append(ctor2)
        # 1-arg: delegates to 2-arg (chained)
        ctor1 = ir.Method('ItemStack', is_ctor=True,
                          params=[ir.Param('var1', ir.JType('int'))], line=20)
        ctor1.body = [(20, 'ItemStack(int var1) {'),
                      (21, '\tthis(var1, 1);'), (22, '}')]
        c.methods.append(ctor1)
        self.r = types.TypeResolver(known_classes={'ItemStack'})
        self.led = Ledger()
        self.cpp = source.render_source(c, self.r, ledger=self.led)

    def test_1arg_ctor_inlines_field_assignments(self):
        # The 1-arg ctor must end up with the 3-arg ctor's field assignments
        # (via 1-arg -> 2-arg -> 3-arg), not an empty body
        lines = self.cpp.splitlines()
        in_1arg = False
        body_lines = []
        for line in lines:
            if 'ItemStack::ItemStack(int var1)' in line:
                in_1arg = True
            elif in_1arg and line.startswith('}'):
                break
            elif in_1arg:
                body_lines.append(line)
        body = '\n'.join(body_lines)
        self.assertIn('this->itemID = var1', body)
        self.assertIn('this->stackSize = 1', body)
        self.assertIn('this->itemDamage = 0', body)

    def test_2arg_ctor_inlines_field_assignments(self):
        lines = self.cpp.splitlines()
        in_2arg = False
        body_lines = []
        for line in lines:
            if 'ItemStack::ItemStack(int var1, int var2)' in line:
                in_2arg = True
            elif in_2arg and line.startswith('}'):
                break
            elif in_2arg:
                body_lines.append(line)
        body = '\n'.join(body_lines)
        self.assertIn('this->itemID = var1', body)
        self.assertIn('this->stackSize = var2', body)
        self.assertIn('this->itemDamage = 0', body)


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


class DelegationTargetSelectionTests(unittest.TestCase):
    """Same-arity sibling ctors must resolve by param TYPE, not by arg count alone."""
    def _vtx_class(self):
        c = ir.JClass('PTV')
        c.fields.append(ir.Field('vector3D', ir.JType('Vec3D')))
        # 0: the delegating float x5 ctor -> this(Vec3D::createVectorHelper(...), var4, var5)
        d = ir.Method('PTV', is_ctor=True,
                      params=[ir.Param('var%d' % i, ir.JType('float')) for i in range(1, 6)], line=1)
        # 1: (PTV, float, float) -- WRONG target (self-type)
        c1 = ir.Method('PTV', is_ctor=True,
                       params=[ir.Param('var1', ir.JType('PTV')), ir.Param('var2', ir.JType('float')),
                               ir.Param('var3', ir.JType('float'))], line=5)
        # 2: (Vec3D, float, float) -- the RIGHT target
        c2 = ir.Method('PTV', is_ctor=True,
                       params=[ir.Param('var1', ir.JType('Vec3D')), ir.Param('var2', ir.JType('float')),
                               ir.Param('var3', ir.JType('float'))], line=8)
        c.methods += [d, c1, c2]
        return c, d, c1, c2

    def test_factory_arg_selects_matching_ctor(self):
        c, d, c1, c2 = self._vtx_class()
        args = ['Vec3D::createVectorHelper((double)var1, (double)var2, (double)var3)', 'var4', 'var5']
        self.assertIs(source._select_deleg_target(args, d, c), c2)

    def test_new_arg_selects_matching_ctor(self):
        c, d, c1, c2 = self._vtx_class()
        args = ['new Vec3D(1.0, 2.0, 3.0)', 'var4', 'var5']
        self.assertIs(source._select_deleg_target(args, d, c), c2)

    def test_infer_arg_type_forms(self):
        self.assertEqual(source._infer_arg_type('new ItemStack(var5)'), 'ItemStack')
        self.assertEqual(source._infer_arg_type('(Vec3D)obj'), 'Vec3D')
        self.assertEqual(source._infer_arg_type('Vec3D::createVectorHelper(a, b)'), 'Vec3D')
        self.assertIsNone(source._infer_arg_type('var5 + 1'))


class DelegationArgParenTests(unittest.TestCase):
    def test_paren_arg_wraps_cast(self):
        # a compound arg must be parenthesized so `(Block*)var1` substituted into `var1.blockID`
        # yields `((Block*)var1)->blockID`, not a cast of the whole field access
        self.assertEqual(source._paren_arg('(Block*)var1'), '((Block*)var1)')

    def test_paren_arg_leaves_simple(self):
        self.assertEqual(source._paren_arg('var1'), 'var1')
        self.assertEqual(source._paren_arg('var1.blockID'), 'var1.blockID')
        self.assertEqual(source._paren_arg('1'), '1')
        self.assertEqual(source._paren_arg('(already)'), '(already)')


class DelegationFieldParamCollisionTests(unittest.TestCase):
    """FIXED BUG delegation-field-name: a delegating ctor whose TARGET assigns a field that shares a
    param's name (this.pos = pos) must not substitute the field on the LHS -- the botched Vertex
    ctor emitted `this->(new Vec3(..)) = (new Vec3(..));`."""

    def _vertex(self):
        c = ir.JClass('Vertex')
        c.fields.append(ir.Field('pos', ir.JType('Vec3')))
        c.fields.append(ir.Field('u', ir.JType('float')))
        c.fields.append(ir.Field('v', ir.JType('float')))
        d = ir.Method('Vertex', is_ctor=True,
                      params=[ir.Param('x', ir.JType('float')), ir.Param('y', ir.JType('float')),
                              ir.Param('z', ir.JType('float')), ir.Param('u', ir.JType('float')),
                              ir.Param('v', ir.JType('float'))], line=10)
        d.body = [(10, 'Vertex(float x, float y, float z, float u, float v) {'),
                  (11, '\tthis(new Vec3(x, y, z), u, v);'), (12, '}')]
        t = ir.Method('Vertex', is_ctor=True,
                      params=[ir.Param('pos', ir.JType('Vec3')), ir.Param('u', ir.JType('float')),
                              ir.Param('v', ir.JType('float'))], line=13)
        t.body = [(13, 'Vertex(Vec3 pos, float u, float v) {'),
                  (14, '\tthis.pos = pos;'), (15, '\tthis.u = u;'), (16, '\tthis.v = v;'), (17, '}')]
        c.methods.extend([d, t])
        return c

    def test_field_lhs_not_substituted(self):
        c = self._vertex()
        r = types.TypeResolver(known_classes={'Vertex', 'Vec3'})
        cpp = source.render_source(c, r, ledger=Ledger())
        self.assertNotIn('this->(new Vec3', cpp)              # the botched form is gone
        self.assertIn('this->pos = (new Vec3(x, y, z));', cpp)
