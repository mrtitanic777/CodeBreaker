# -*- coding: utf-8 -*-
"""Module 4c pass 1 tests: per-method symbol table + value/pointer access."""

import unittest

from j2ctool import ir, types
from j2ctool.emit import statements
from j2ctool.ledger import Ledger


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


class TestStaticAndApi(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('Block')
        m = ir.Method('test', ir.JType('void'), line=1)
        m.body = [
            (1, 'void test() {'),
            (2, '\tBlock var1 = Block.stone;'),
            (3, '\tint i = Block.getRenderType(var1);'),
            (4, '\tDirection var2 = Direction.NORTH;'),
            (5, '\tfloat f = Math.sqrt(2.0F);'),
            (6, '\tfloat g = Math.max(f, 1.0F);'),
            (7, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Block', 'Direction'}, enum_types={'Direction'})
        self.out = statements.translate_body(m, c, r)

    def test_static_field_gets_scope_and_prefix(self):
        self.assertIn('Block* var1 = Block::s_stone;', self.out)

    def test_static_method_call(self):
        self.assertIn('int i = Block::getRenderType(var1);', self.out)

    def test_enum_constant_no_prefix(self):
        self.assertIn('int var2 = Direction::NORTH;', self.out)   # enum type -> int, constant scoped

    def test_math_mapping(self):
        self.assertIn('float f = sqrtf(2.0F);', self.out)
        self.assertIn('float g = max(f, 1.0F);', self.out)


class TestCollectionsAndString(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('Inv')
        c.fields.append(ir.Field('items', ir.JType('List')))
        c.fields.append(ir.Field('name', ir.JType('String')))
        m = ir.Method('run', ir.JType('void'), params=[ir.Param('var1', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void run(int var1) {'),
            (2, '\tthis.items.add(var1);'),
            (3, '\tObject var2 = this.items.get(0);'),
            (4, '\tboolean b = this.items.isEmpty();'),
            (5, '\tchar c = this.name.charAt(2);'),
            (6, '\tint n = this.items.size();'),
            (7, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_add_to_push_back(self):
        self.assertIn('this->items.push_back(var1);', self.out)

    def test_get_to_index(self):
        # `Object var2 = items.get(0)` -> JObject local absorbing the void* element
        self.assertIn('JObject var2 = this->items[0];', self.out)

    def test_isempty_to_empty(self):
        self.assertIn('bool b = this->items.empty();', self.out)

    def test_charat_to_index(self):
        self.assertIn('char c = this->name[2];', self.out)

    def test_size_unchanged(self):
        self.assertIn('int n = this->items.size();', self.out)


class TestValueContainers(unittest.TestCase):
    """Java collections are by-value std::vector/map/set; `new ArrayList()` is value
    construction, not a heap pointer, so method calls use '.' not '->'."""
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('tagList', ir.JType('List')))
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void f(int var1) {'),
            (2, '\tList var2 = new ArrayList();'),       # value local, not a pointer
            (3, '\tvar2.add(var1);'),                    # value receiver -> push_back
            (4, '\tthis.tagList = new ArrayList();'),     # field assign -> value temporary
            (5, '\tArrayList var3 = new ArrayList();'),
            (6, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_local_container_decl_default_constructs(self):
        self.assertIn('JList<JObject> var2;', self.out)              # initializer dropped
        self.assertFalse(any('var2 =' in l and 'new' in l for l in self.out), self.out)

    def test_value_container_method_uses_dot(self):
        self.assertIn('var2.push_back(var1);', self.out)           # '.' not '->'

    def test_field_assign_is_value_temporary(self):
        self.assertIn('this->tagList = JList<JObject>();', self.out)


class TestMapSetString(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('map', ir.JType('HashMap')))
        c.fields.append(ir.Field('set', ir.JType('HashSet')))
        m = ir.Method('f', ir.JType('void'),
                      params=[ir.Param('var1', ir.JType('String')), ir.Param('var2', ir.JType('Object'))], line=1)
        m.body = [
            (1, 'void f(String var1, Object var2) {'),
            (2, '\tthis.map.put(var1, var2);'),
            (3, '\tObject var3 = this.map.get(var1);'),
            (4, '\tboolean b = this.map.containsKey(var1);'),
            (5, '\tthis.map.remove(var1);'),
            (6, '\tthis.set.add(var1);'),
            (7, '\tboolean c = this.set.contains(var1);'),
            (8, '\tboolean d = var1.equals("foo");'),
            (9, '\tString var4 = var1.substring(2, 5);'),
            (10, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_map_put(self):
        self.assertIn('this->map[var1] = var2;', self.out)

    def test_split_top_commas_handles_arrow(self):
        # '->' must not mis-balance the comma splitter (regression: map.put(a->b, c))
        self.assertEqual(statements._split_top_commas('var1->entityID, var9'),
                         ['var1->entityID', 'var9'])

    def test_map_get(self):
        self.assertIn('JObject var3 = this->map.get(var1);', self.out)

    def test_map_containskey_and_remove(self):
        self.assertIn('bool b = this->map.containsKey(var1);', self.out)
        self.assertIn('this->map.erase(var1);', self.out)

    def test_set_add_and_contains(self):
        # sets map to JSet<T> (a std::set subclass with the native Java API), so add/contains are
        # NOT remapped to insert/count -- they bind to JSet's own methods (correct Java return types).
        self.assertIn('this->set.add(var1);', self.out)
        self.assertIn('bool c = this->set.contains(var1);', self.out)

    def test_string_equals(self):
        self.assertIn('bool d = (var1 == "foo");', self.out)

    def test_string_substring(self):
        self.assertIn('JavaString var4 = var1.substr(2, (5) - (2));', self.out)


class TestStringConcat(unittest.TestCase):
    def test_concat_return_to_jcat(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('x', ir.JType('float')))
        c.fields.append(ir.Field('y', ir.JType('float')))
        m = ir.Method('toString', ir.JType('String'), line=1)
        m.body = [(1, 'String toString() {'),
                  (2, '\treturn "(" + this.x + ", " + this.y + ")";'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return (JCat() + "(" + this->x + ", " + this->y + ")");', out)

    def test_concat_in_call_arg(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tSystem.out.println("n=" + var1 + "!");'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertTrue(any('(JCat() + "n=" + var1 + "!")' in l for l in out), out)

    def test_concat_in_assignment(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tString s = "a" + var1 + "b";'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertTrue(any('= (JCat() + "a" + var1 + "b");' in l for l in out), out)

    def test_float_mod_to_fmodf(self):
        self.assertEqual(statements._rewrite_float_mod('var1 % 4.0F'), 'fmodf(var1, 4.0F)')
        self.assertEqual(statements._rewrite_float_mod('this->c % 4.0F == 0.0F'),
                         'fmodf(this->c, 4.0F) == 0.0F')

    def test_int_mod_unchanged(self):
        self.assertEqual(statements._rewrite_float_mod('a[i % 4] = b'), 'a[i % 4] = b')
        self.assertEqual(statements._rewrite_float_mod('x = y % z'), 'x = y % z')

    def test_no_concat_leaves_plain_return(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('int'), line=1)
        m.body = [(1, 'int f() {'), (2, '\treturn a + b;'), (3, '}')]   # numeric add, no string
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return a + b;', out)

    def test_literal_method_not_wrapped(self):
        # "abc".length() + 5 is numeric, not concat -- must NOT be wrapped
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('int'), line=1)
        m.body = [(1, 'int f() {'), (2, '\treturn "abc".length() + 5;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertFalse(any('JCat()' in l for l in out), out)


class TestCasts(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'),
                      params=[ir.Param('var1', ir.JType('Object')), ir.Param('f', ir.JType('float'))], line=1)
        m.body = [
            (1, 'void f(Object var1, float f) {'),
            (2, '\tBlock var2 = (Block)var1;'),
            (3, '\tint var3 = (int)f;'),
            (4, '\tVec3D var4 = (Vec3D)var1;'),
            (5, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Block'}, value_types={'Vec3D'})
        self.out = statements.translate_body(m, c, r)

    def test_reference_cast_gets_pointer(self):
        self.assertIn('Block* var2 = (Block*)var1;', self.out)

    def test_primitive_cast_unchanged(self):
        self.assertIn('int var3 = (int)f;', self.out)

    def test_value_cast_unchanged(self):
        self.assertIn('Vec3D var4 = (Vec3D)var1;', self.out)


class TestExpressionDeref(unittest.TestCase):
    """Index-result and cast-result expressions yield pointers -> '.' becomes '->'."""
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('entities', ir.JType('Entity', array_dims=1)))   # ref array
        c.fields.append(ir.Field('points', ir.JType('Vec3D', array_dims=1)))      # value array
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('Object'))], line=1)
        m.body = [
            (1, 'void f(Object var1) {'),
            (2, '\tBlock var2 = this.entities[0].getBlock();'),     # ref-array index -> ->
            (3, '\tString s = Block.blocksList[var1].getName();'),  # registry index -> ->
            (4, '\tfloat z = this.points[0].xCoord;'),              # value-array index -> stays .
            (5, '\tint i = ((Block)var1).getRenderType();'),        # cast-result -> ->
            (6, '\tfloat w = ((Vec3D)var1).xCoord;'),               # value cast-result -> stays .
            (7, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Block', 'Entity', 'R'}, value_types={'Vec3D'})
        self.out = statements.translate_body(m, c, r)

    def test_ref_array_index_deref(self):
        self.assertIn('Block* var2 = this->entities[0]->getBlock();', self.out)

    def test_registry_index_deref(self):
        self.assertIn('JavaString s = Block::s_blocksList[var1]->getName();', self.out)

    def test_value_array_index_keeps_dot(self):
        self.assertIn('float z = this->points[0].xCoord;', self.out)

    def test_cast_result_deref(self):
        self.assertIn('int i = ((Block*)var1)->getRenderType();', self.out)

    def test_value_cast_result_keeps_dot(self):
        self.assertIn('float w = ((Vec3D)var1).xCoord;', self.out)


class TestPrimitiveCastAndMathConst(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void f(int var1) {'),
            (2, '\tbyte var2 = (byte)var1;'),          # primitive cast remapped
            (3, '\tboolean var3 = (boolean)var1;'),
            (4, '\tfloat var4 = (float)var1;'),         # self-mapping -> unchanged
            (5, '\tfloat p = (float)Math.PI;'),         # Math.PI constant -> literal
            (6, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_byte_cast_remapped(self):
        self.assertIn('signed char var2 = (signed char)var1;', self.out)   # scalar byte is signed (Java-faithful)

    def test_boolean_cast_remapped(self):
        self.assertIn('bool var3 = (bool)var1;', self.out)

    def test_self_mapping_cast_unchanged(self):
        self.assertIn('float var4 = (float)var1;', self.out)

    def test_math_pi_constant(self):
        self.assertIn('float p = (float)3.1415927F;', self.out)


class TestNullableValueLocals(unittest.TestCase):
    """A value-type local used as a nullable reference (assigned/compared null, or new'd)
    must be emitted as a pointer -- a C++ value type can't hold null."""
    def setUp(self):
        c = ir.JClass('AxisAlignedBB')
        m = ir.Method('trace', ir.JType('void'), params=[ir.Param('var1', ir.JType('Vec3D'))], line=1)
        m.body = [
            (1, 'void trace(Vec3D var1) {'),
            (2, '\tVec3D var3 = var1.getIntermediate();'),   # dual-use return
            (3, '\tif(!this.isVecIn(var3)) {'),
            (4, '\t\tvar3 = null;'),                         # null -> must be pointer
            (5, '\t}'),
            (6, '\tVec3D var8 = null;'),                     # declared null
            (7, '\tif(var3 != null) {'),
            (8, '\t\tvar8 = var3;'),
            (9, '\t}'),
            (10, '\tVec3D var9 = var1.normalize();'),         # value local, no null use
            (11, '\tfloat f = var9.xCoord;'),
            (12, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'AxisAlignedBB'}, value_types={'Vec3D'})
        self.out = statements.translate_body(m, c, r)

    def test_nullable_local_becomes_pointer(self):
        self.assertIn('Vec3D* var3 = var1.getIntermediate();', self.out)
        self.assertIn('Vec3D* var8 = NULL;', self.out)

    def test_nullable_local_assignments_and_compares(self):
        self.assertIn('var3 = NULL;', self.out)
        self.assertIn('var8 = var3;', self.out)
        self.assertTrue(any('var3 != NULL' in l for l in self.out), self.out)

    def test_plain_value_local_stays_value(self):
        self.assertIn('Vec3D var9 = var1.normalize();', self.out)   # no null use -> stays value
        self.assertIn('float f = var9.xCoord;', self.out)           # value access keeps '.'


class TestBareStaticPrefix(unittest.TestCase):
    def test_bare_static_field_gets_prefix(self):
        c = ir.JClass('MathHelper')
        c.fields.append(ir.Field('SIN_TABLE', ir.JType('float', array_dims=1), is_static=True))
        m = ir.Method('sin', ir.JType('float'), params=[ir.Param('var0', ir.JType('float'))], line=1)
        m.body = [(1, 'float sin(float var0) {'),
                  (2, '\treturn SIN_TABLE[(int)var0 & 255];'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return s_SIN_TABLE[(int)var0 & 255];', out)

    def test_bare_static_prefix_skips_string_literals(self):
        # a static field named 'stone' must NOT corrupt the string literal "stone"
        c = ir.JClass('Block')
        c.fields.append(ir.Field('stone', ir.JType('Block'), is_static=True))
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tthis.register(stone, "stone");'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Block'}))
        self.assertTrue(any('s_stone, "stone"' in l for l in out), out)   # ident renamed, literal intact

    def test_qualified_static_not_double_prefixed(self):
        c = ir.JClass('Block')
        c.fields.append(ir.Field('blocksList', ir.JType('Block', array_dims=1), is_static=True))
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tBlock var1 = Block.blocksList[0];'),   # qualified -> Block::s_blocksList
                  (3, '\tBlock var2 = blocksList[1];'),          # bare -> s_blocksList
                  (4, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Block'}))
        self.assertIn('Block* var1 = Block::s_blocksList[0];', out)
        self.assertIn('Block* var2 = s_blocksList[1];', out)
        self.assertFalse(any('s_s_' in l for l in out), out)    # never double-prefixed


class TestChainInference(unittest.TestCase):
    """Member environment drives chained '.' vs '->' (static reference field; pointer
    field of a typed receiver)."""
    def _resolver(self):
        # Block has a static pointer field 'tilledField' and an int field 'blockID';
        # World has a pointer field 'random' (Random). Flattened member env, as the
        # pipeline would build it.
        members = {
            'Block': {'fields': {'tilledField': ir.JType('Block'), 'blockID': ir.JType('int')},
                      'methods': {}},
            'World': {'fields': {'random': ir.JType('Random')}, 'methods': {}},
        }
        return types.TypeResolver(known_classes={'Block', 'World', 'Random', 'C'},
                                  value_types={'Vec3D'}, members=members)

    def test_static_reference_field_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tint i = Block.tilledField.blockID;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('int i = Block::s_tilledField->blockID;', out)

    def test_pointer_field_of_param_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('World'))], line=1)
        m.body = [(1, 'void f(World var1) {'),
                  (2, '\tfloat g = var1.random.nextFloat();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('float g = var1->random->nextFloat();', out)

    def test_pointer_field_chain_with_string_return(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('World'))], line=1)
        m.body = [(1, 'void f(World var1) {'),
                  (2, '\tString s = var1.random.toString();'),   # random is ptr -> ->
                  (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('JavaString s = var1->random->toString();', out)

    def test_deep_pointer_chain(self):
        # this->mc->options->keyBindInventory.keyCode  -- every link a pointer field, to depth
        members = {
            'C': {'fields': {'mc': ir.JType('Minecraft')}, 'methods': {}},
            'Minecraft': {'fields': {'options': ir.JType('GameSettings')}, 'methods': {}},
            'GameSettings': {'fields': {'keyBindInventory': ir.JType('KeyBinding')}, 'methods': {}},
            'KeyBinding': {'fields': {'keyCode': ir.JType('int')}, 'methods': {}},
        }
        r = types.TypeResolver(known_classes={'C', 'Minecraft', 'GameSettings', 'KeyBinding'},
                               members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'), (2, '\tint k = this.mc.options.keyBindInventory.keyCode;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('int k = this->mc->options->keyBindInventory->keyCode;', out)

    def test_bare_inherited_static_chain(self):
        # s_rand.nextFloat() -- bare emitted static (Item.rand inherited), should deref
        members = {'ItemBow': {'fields': {'rand': ir.JType('Random')}, 'methods': {}}}
        r = types.TypeResolver(known_classes={'ItemBow', 'Random'}, members=members)
        c = ir.JClass('ItemBow')
        m = ir.Method('f', ir.JType('float'), line=1)
        m.body = [(1, 'float f() {'), (2, '\treturn s_rand.nextFloat();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertTrue(any('s_rand->nextFloat()' in l for l in out), out)

    def test_container_method_on_foreign_receiver(self):
        # obj->listField.get(i) / .add(x) -- receiver not in scope; mapped via the member env
        members = {'World': {'fields': {'players': ir.JType('List')}, 'methods': {}}}
        r = types.TypeResolver(known_classes={'World', 'C'}, members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('World'))], line=1)
        m.body = [(1, 'void f(World var1) {'),
                  (2, '\tObject var2 = var1.players.get(0);'),
                  (3, '\tvar1.players.add(var2);'), (4, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('JObject var2 = var1->players[0];', out)
        self.assertIn('var1->players.push_back(var2);', out)

    def test_array_index_field_chain(self):
        # var5->mainInventory[i].writeToNBT() -- array field of a foreign object, index -> element
        members = {'InventoryPlayer': {'fields': {'mainInventory': ir.JType('ItemStack', array_dims=1)},
                                       'methods': {}}}
        r = types.TypeResolver(known_classes={'InventoryPlayer', 'ItemStack', 'C'}, members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'),
                      params=[ir.Param('var5', ir.JType('InventoryPlayer'))], line=1)
        m.body = [(1, 'void f(InventoryPlayer var5) {'),
                  (2, '\tvar5.mainInventory[var3].writeToNBT(var4);'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('var5->mainInventory[var3]->writeToNBT(var4);', out)

    def test_unknown_receiver_left_alone(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tint i = Mystery.foo.bar;'), (3, '}')]   # Mystery unknown -> untouched
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertTrue(any('Mystery' in l and '.bar' in l for l in out), out)


class TestBoxedUnbox(unittest.TestCase):
    """JObject boxing/unboxing round-trip: a boxed-wrapper cast on a JObject-valued operand must
    NOT lower to a primitive spelling (which would put .intValue()/.byteValue() on a raw int) --
    the cast is dropped and the unbox binds on the JObject. Nullable boxed LOCALS become JObject
    (a value that holds null AND unboxes), never a pointer-to-primitive."""

    def _class(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('map', ir.JType('HashMap')))
        return c

    def _emit(self, body_lines, params=None):
        c = self._class()
        m = ir.Method('f', ir.JType('int'), params=params or [ir.Param('k', ir.JType('int'))], line=1)
        m.body = [(1, 'int f(int k) {')] + list(enumerate(body_lines, start=2)) + [(99, '}')]
        c.methods.append(m)
        return statements.translate_body(m, c, types.TypeResolver())

    def test_cast_unbox_on_map_value_drops_wrapper_cast(self):
        # ((Integer)map.get(k)).intValue() -- operand is a raw-map value (JObject); the (Integer)
        # cast must be DROPPED, leaving .intValue() on the JObject (NOT `((int)...).intValue()`).
        out = self._emit(['\treturn ((Integer)this.map.get(k)).intValue();'])
        joined = '\n'.join(out)
        self.assertIn('(this->map.get(k)).intValue()', joined)
        self.assertNotIn('(int)', joined)          # wrapper cast dropped, not lowered to (int)

    def test_byte_unbox_on_map_value(self):
        # ((Byte)map.get(k)).byteValue() -- (Byte) dropped, byteValue() binds on the JObject.
        out = self._emit(['\treturn ((Byte)this.map.get(k)).byteValue();'])
        joined = '\n'.join(out)
        self.assertIn('(this->map.get(k)).byteValue()', joined)
        self.assertNotIn('(signed char)', joined)

    def test_nullable_boxed_local_becomes_jobject(self):
        # A boxed-wrapper local compared to null is NULLABLE. It must become a JObject VALUE
        # (holds null + unboxes), never `int* var1` (which can't hold the boxed value nor unbox).
        out = self._emit([
            '\tInteger var1 = (Integer)this.map.get(k);',
            '\tif(var1 == null) { return 0; }',
            '\treturn var1.intValue();',
        ])
        joined = '\n'.join(out)
        self.assertIn('JObject var1 = ', joined)   # not `int* var1`
        self.assertNotIn('int* var1', joined)
        self.assertNotIn('var1->intValue()', joined)   # value, not pointer -> `.`, not `->`
        self.assertIn('var1.intValue()', joined)
        self.assertIn('var1 == NULL', joined)


class TestCallResultChains(unittest.TestCase):
    """A call/ctor result that is a pointer, then .member -> -> (method-result inference)."""
    def _resolver(self):
        members = {
            'ItemStack': {'fields': {}, 'methods': {'getItem': ir.JType('Item')}},
            'CraftingManager': {'fields': {},
                                'methods': {'getInstance': ir.JType('CraftingManager'),
                                            'findRecipe': ir.JType('ItemStack')}},
        }
        return types.TypeResolver(
            known_classes={'ItemStack', 'Item', 'CraftingManager', 'ThreadDownloadImage', 'C'},
            members=members)

    def test_instance_method_result_chain(self):
        # getItem() is a known-nullable getter, so with its return type known the chain is NULL-guarded
        # (the crash-class defense added in a1db028) and every hop on the ItemStack* receiver uses '->'
        # -- never a '.' on a pointer. Local resolver so getItem()'s return type is resolvable.
        r = types.TypeResolver(known_classes={'C', 'ItemStack', 'Item'},
                               members={'ItemStack': {'fields': {},
                                                      'methods': {'getItem': ir.JType('Item')}}})
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var6', ir.JType('ItemStack'))], line=1)
        m.body = [(1, 'void f(ItemStack var6) {'),
                  (2, '\tvar6.getItem().onPlaced();'), (3, '}')]
        c.methods.append(m)
        out = '\n'.join(statements.translate_body(m, c, r))
        self.assertIn('var6->getItem()', out)          # receiver hop uses '->'
        self.assertIn('->onPlaced()', out)             # result hop uses '->', not '.'
        self.assertNotIn('getItem().onPlaced', out)    # never a '.' deref on the pointer
        self.assertIn('NULL', out)                     # nullable getter is guarded
        self.assertIn('Item* ', out)                   # typed temp (C++03), not `auto`
        self.assertNotIn('auto', out)                  # no C++11 in C++03 output

    def test_static_method_result_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tItemStack var1 = CraftingManager.getInstance().findRecipe();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('CraftingManager::getInstance()->findRecipe();', out[0])

    def test_fluent_new_builder_chain(self):
        # new BlockX(...).setHardness(a).setResistance(b) -- builder returns Block*, deref to depth
        members = {'BlockX': {'fields': {}, 'methods': {'setHardness': ir.JType('Block')}},
                   'Block': {'fields': {}, 'methods': {'setHardness': ir.JType('Block'),
                                                       'setResistance': ir.JType('Block')}}}
        r = types.TypeResolver(known_classes={'BlockX', 'Block', 'C'}, members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tBlock var1 = new BlockX(1, 2).setHardness(1.5F).setResistance(10.0F);'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('Block* var1 = (new BlockX(1, 2))->setHardness(1.5F)->setResistance(10.0F);', out)
        # already-parenthesized form (as the decompiler emits it) chains too
        m2 = ir.Method('g', ir.JType('void'), line=1)
        m2.body = [(1, 'void g() {'),
                   (2, '\tBlock var2 = (new BlockX(1, 2)).setHardness(1.5F).setResistance(10.0F);'), (3, '}')]
        c2 = ir.JClass('C'); c2.methods.append(m2)
        out2 = statements.translate_body(m2, c2, r)
        self.assertIn('Block* var2 = (new BlockX(1, 2))->setHardness(1.5F)->setResistance(10.0F);', out2)

    def test_new_result_chain_wrapped(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tnew ThreadDownloadImage(this, var1).start();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('(new ThreadDownloadImage(this, var1))->start();', out)

    def test_already_parenthesized_new_result(self):
        # source already wrote (new X(...)).start() -> just fix the '.' to '->'
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\t(new ThreadDownloadImage(this, var1, var2)).start();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('(new ThreadDownloadImage(this, var1, var2))->start();', out)


class TestSuperAndInstanceof(unittest.TestCase):
    def test_instanceof_to_dynamic_cast(self):
        c = ir.JClass('R', parent='Base')
        m = ir.Method('f', ir.JType('boolean'), params=[ir.Param('var1', ir.JType('Entity'))], line=1)
        m.body = [(1, 'boolean f(Entity var1) {'),
                  (2, '\treturn var1 instanceof EntityPlayer;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Entity', 'EntityPlayer'}))
        self.assertIn('return (dynamic_cast<EntityPlayer*>(var1) != NULL);', out)

    def test_super_method_to_qualified(self):
        c = ir.JClass('R', parent='EntityLiving')
        m = ir.Method('onUpdate', ir.JType('void'), line=1)
        m.body = [(1, 'void onUpdate() {'), (2, '\tsuper.onUpdate();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('EntityLiving::onUpdate();', out)


class TestInheritedFields(unittest.TestCase):
    def test_inherited_pointer_field_access(self):
        c = ir.JClass('ModelSheep', parent='ModelBase')
        c.inherited_fields = [ir.Field('head', ir.JType('ModelRenderer'))]   # inherited, pointer
        m = ir.Method('render', ir.JType('void'), line=1)
        m.body = [(1, 'void render() {'), (2, '\tthis.head.addBox(1, 2, 3);'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'ModelRenderer'}))
        self.assertIn('this->head->addBox(1, 2, 3);', out)   # inherited field resolved as pointer


class TestInBodyNewArray(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [
            (1, 'void f() {'),
            (2, '\tint[] var1 = new int[256];'),
            (3, '\tboolean[] var2 = new boolean[10];'),
            (4, '\tBlock[] var3 = new Block[256];'),
            (5, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Block'}))

    def test_primitive_array_to_jarray(self):
        self.assertIn('JArray<int > var1 = JArray<int >(256);', self.out)
        self.assertIn('JArray<bool > var2 = JArray<bool >(10);', self.out)   # boolean->bool too

    def test_reference_array_to_jarray_of_pointers(self):
        self.assertIn('JArray<Block* > var3 = JArray<Block* >(256);', self.out)


class TestLabeledGoto(unittest.TestCase):
    def test_labeled_break_and_continue(self):
        lines = [
            'label0:',
            'for(int var1 = 0; var1 < 4; ++var1) {',
            'for(int var2 = 0; var2 < 4; ++var2) {',
            'if(a) {',
            'continue label0;',
            '}',
            'if(b) {',
            'break label0;',
            '}',
            '}',
            '}',
        ]
        out = statements._translate_labels(lines)
        self.assertNotIn('label0:', out)                  # original label removed
        self.assertIn('goto label0_cont;', out)
        self.assertIn('goto label0_brk;', out)
        ci, bi = out.index('label0_cont:;'), out.index('label0_brk:;')
        self.assertLess(ci, bi)
        self.assertEqual(out[ci + 1], '}')                # cont sits just before the outer loop's close
        self.assertEqual(out[ci + 2], 'label0_brk:;')     # brk just after it

    def test_unlabeled_break_continue_untouched(self):
        lines = ['while(x) {', 'break;', 'continue;', '}']
        self.assertEqual(statements._translate_labels(lines), lines)


class TestNullDerefGuard(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('World')
        c.fields.append(ir.Field('chunks', ir.JType('Chunk', 1)))    # Chunk[] -> pointer slots
        m = ir.Method('tick', ir.JType('void'),
                      params=[ir.Param('entity', ir.JType('Entity')),
                              ir.Param('var1', ir.JType('int')), ir.Param('k', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void tick(Entity entity, int var1, int k) {'),
            (2, '\tBlock.blocksList[var1].onUpdate(this, var1, k, var1);'),
            (3, '\tthis.chunks[k].markDirty();'),
            (4, '\tentity.move();'),
            (5, '}'),
        ]
        c.methods.append(m)
        self.led = Ledger()
        r = types.TypeResolver(known_classes={'Block', 'Chunk', 'World', 'Entity'})
        self.out = statements.translate_body(m, c, r, ledger=self.led)

    def test_registry_deref_guarded(self):
        self.assertIn('if (Block::s_blocksList[var1] != NULL) { '
                      'Block::s_blocksList[var1]->onUpdate(this, var1, k, var1); }', self.out)

    def test_scope_array_deref_guarded(self):
        self.assertIn('if (this->chunks[k] != NULL) { this->chunks[k]->markDirty(); }', self.out)

    def test_non_index_call_not_guarded(self):
        self.assertIn('entity->move();', self.out)
        self.assertNotIn('if (entity', '\n'.join(self.out))

    def test_guard_events_logged(self):
        self.assertEqual(sum(1 for e in self.led.events if e.rule == 'null-deref-guard'), 2)


class TestSwitchCaseWrap(unittest.TestCase):
    def test_case_bodies_get_block_scope(self):
        lines = [
            '\tswitch(var3) {',
            '\tcase 0:',
            '\tint a = 1;',
            '\tbreak;',
            '\tcase 1:',
            '\tItemStack* b = make();',
            '\tuse(b);',
            '\tbreak;',
            '\tdefault:',
            '\tfallback();',
            '\t}',
        ]
        out = statements._wrap_switch_cases(lines)
        text = '\n'.join(out)
        # every label opens a scope
        self.assertIn('case 0: {', text)
        self.assertIn('case 1: {', text)
        self.assertIn('default: {', text)
        # the declaration in case 1 no longer crosses the default label
        self.assertLess(out.index('\tItemStack* b = make();'),
                        out.index('\tdefault: {'))
        # braces balance
        self.assertEqual(text.count('{'), text.count('}'))

    def test_fallthrough_empty_label_preserved(self):
        lines = ['\tswitch(x) {', '\tcase 0:', '\tcase 1:', '\ty = 1;', '\tbreak;', '\t}']
        out = statements._wrap_switch_cases(lines)
        text = '\n'.join(out)
        self.assertIn('case 0: {', text)
        self.assertIn('case 1: {', text)
        self.assertEqual(text.count('{'), text.count('}'))

    def test_nested_block_in_case_not_mistaken(self):
        # a guard block inside a case body must not close the switch case early
        lines = [
            '\tswitch(x) {',
            '\tcase 0:',
            '\t{ Item* it = get(); if (it != NULL) { use(it); } }',
            '\tint z = 2;',
            '\tbreak;',
            '\t}',
        ]
        out = statements._wrap_switch_cases(lines)
        text = '\n'.join(out)
        self.assertIn('case 0: {', text)
        self.assertEqual(text.count('{'), text.count('}'))
        # the trailing int z is still inside the case block (before the closing braces)
        self.assertLess(out.index('\tint z = 2;'), len(out) - 1)

    def test_translate_body_wraps_switch(self):
        c = ir.JClass('Foo')
        m = ir.Method('pick', ir.JType('void'),
                      params=[ir.Param('k', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void pick(int k) {'),
            (2, '\tswitch(k) {'),
            (3, '\tcase 0:'),
            (4, '\t\tint a = 1;'),
            (5, '\t\tbreak;'),
            (6, '\tcase 1:'),
            (7, '\t\tint b = 2;'),
            (8, '\t\tbreak;'),
            (9, '\t}'),
            (10, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Foo'})
        out = statements.translate_body(m, c, r)
        text = '\n'.join(out)
        self.assertIn('case 0: {', text)
        self.assertIn('case 1: {', text)
        self.assertEqual(text.count('{'), text.count('}'))
class TestStubMemberEnv(unittest.TestCase):
    """Runtime-stub method return types are adopted into the member environment so a chain THROUGH a
    stub result resolves '.' vs '->'. Pointer returns become the pointee JType; std::string -> String;
    void*/JArray/primitives are omitted (chain resolver stays conservative)."""
    def setUp(self):
        from j2ctool import pipeline
        self.env = pipeline.build_member_env([])          # no corpus -- only the stub classes

    def test_pointer_return_registered(self):
        self.assertEqual(self.env['File']['methods']['getParentFile'].name, 'File')
        self.assertEqual(self.env['Applet']['methods']['getDocumentBase'].name, 'URL')
        self.assertEqual(self.env['Node']['methods']['getChildNodes'].name, 'NodeList')

    def test_static_factory_from_extra_line(self):
        # `static Toolkit* getDefaultToolkit()` lives in `extra`, not ret_methods. (Runtime is no longer a
        # member-env stub -- it has a real baked implementation now, so it isn't in the stub env.)
        self.assertEqual(self.env['Toolkit']['methods']['getDefaultToolkit'].name, 'Toolkit')
        self.assertNotIn('Runtime', self.env)

    def test_string_return_mapped_to_String(self):
        self.assertEqual(self.env['File']['methods']['getName'].name, 'String')

    def test_ambiguous_returns_omitted(self):
        # void* (Toolkit.getSystemClipboard) and JArray<> (File.listFiles) are not usable pointees
        self.assertNotIn('getSystemClipboard', self.env['Toolkit']['methods'])
        self.assertNotIn('listFiles', self.env['File']['methods'])

    def test_corpus_class_inherits_stub_methods(self):
        # a parsed class extending a stub base picks up the stub's methods via the flatten
        sub = ir.JClass('MyApplet', parent='Applet')
        env = __import__('j2ctool.pipeline', fromlist=['build_member_env']).build_member_env([sub])
        self.assertEqual(env['MyApplet']['methods']['getDocumentBase'].name, 'URL')


class TestStubResultChains(unittest.TestCase):
    """End-to-end: a method whose RESULT is a stub pointer gets '->' on the following access."""
    def _members(self):
        from j2ctool import pipeline
        return pipeline.build_member_env([])

    def test_stub_result_member_becomes_arrow(self):
        # getAbsoluteFile() returns File* and is NOT a nullable getter, so it isolates the chain deref
        # (getParentFile would additionally trigger the null-guard hoist -- see the corpus output).
        c = ir.JClass('Foo')
        m = ir.Method('bar', ir.JType('void'), params=[ir.Param('f', ir.JType('File'))], line=1)
        m.body = [(1, 'void bar(File f) {'), (2, '\tf.getAbsoluteFile().mkdirs();'), (3, '}')]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Foo'}, members=self._members())
        out = statements.translate_body(m, c, r)
        self.assertIn('f->getAbsoluteFile()->mkdirs();', out)

    def test_cast_root_chain_all_arrows(self):
        # ((Element*)x)->getChildNodes().item(0).getNodeValue() -- every hop is a pointer result
        r = types.TypeResolver(known_classes={'Element', 'Node', 'NodeList'}, members=self._members())
        out = statements._resolve_chains(
            '((Element*)x)->getChildNodes().item(0).getNodeValue()', {}, r, 'Foo')
        self.assertEqual(out, '((Element*)x)->getChildNodes()->item(0)->getNodeValue()')

    def test_value_cast_root_keeps_dot(self):
        r = types.TypeResolver(value_types={'Vec3D'},
                               members={'Vec3D': {'fields': {'xCoord': ir.JType('double')},
                                                  'statics': set(), 'methods': {}}})
        out = statements._resolve_chains('((Vec3D)x).xCoord', {}, r, 'Foo')
        self.assertEqual(out, '((Vec3D)x).xCoord')


class TestIterHelperDeref(unittest.TestCase):
    """`coll.iterator()` was lowered to j2xIter(coll) (Iterator*); the following `.next()` must be '->'."""
    def test_iter_helper_result_arrow(self):
        self.assertEqual(statements._deref_iter_helpers('j2xIter(this->coll).next()'),
                         'j2xIter(this->coll)->next()')

    def test_values_and_keys_helpers(self):
        self.assertEqual(statements._deref_iter_helpers('j2xValues(m).next()'), 'j2xValues(m)->next()')
        self.assertEqual(statements._deref_iter_helpers('j2xKeys(m).hasNext()'), 'j2xKeys(m)->hasNext()')

    def test_nested_arg_balanced(self):
        self.assertEqual(statements._deref_iter_helpers('(int)j2xIter(a.b(c)).next()'),
                         '(int)j2xIter(a.b(c))->next()')

    def test_non_helper_untouched(self):
        self.assertEqual(statements._deref_iter_helpers('x.foo()'), 'x.foo()')
        self.assertEqual(statements._deref_iter_helpers('notj2xIter(a).b()'), 'notj2xIter(a).b()')


if __name__ == '__main__':
    unittest.main()
# -*- coding: utf-8 -*-
"""Module 4c pass 1 tests: per-method symbol table + value/pointer access."""

import unittest

from j2ctool import ir, types
from j2ctool.emit import statements
from j2ctool.ledger import Ledger


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


class TestStaticAndApi(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('Block')
        m = ir.Method('test', ir.JType('void'), line=1)
        m.body = [
            (1, 'void test() {'),
            (2, '\tBlock var1 = Block.stone;'),
            (3, '\tint i = Block.getRenderType(var1);'),
            (4, '\tDirection var2 = Direction.NORTH;'),
            (5, '\tfloat f = Math.sqrt(2.0F);'),
            (6, '\tfloat g = Math.max(f, 1.0F);'),
            (7, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Block', 'Direction'}, enum_types={'Direction'})
        self.out = statements.translate_body(m, c, r)

    def test_static_field_gets_scope_and_prefix(self):
        self.assertIn('Block* var1 = Block::s_stone;', self.out)

    def test_static_method_call(self):
        self.assertIn('int i = Block::getRenderType(var1);', self.out)

    def test_enum_constant_no_prefix(self):
        self.assertIn('int var2 = Direction::NORTH;', self.out)   # enum type -> int, constant scoped

    def test_math_mapping(self):
        self.assertIn('float f = sqrtf(2.0F);', self.out)
        self.assertIn('float g = max(f, 1.0F);', self.out)


class TestCollectionsAndString(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('Inv')
        c.fields.append(ir.Field('items', ir.JType('List')))
        c.fields.append(ir.Field('name', ir.JType('String')))
        m = ir.Method('run', ir.JType('void'), params=[ir.Param('var1', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void run(int var1) {'),
            (2, '\tthis.items.add(var1);'),
            (3, '\tObject var2 = this.items.get(0);'),
            (4, '\tboolean b = this.items.isEmpty();'),
            (5, '\tchar c = this.name.charAt(2);'),
            (6, '\tint n = this.items.size();'),
            (7, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_add_to_push_back(self):
        self.assertIn('this->items.push_back(var1);', self.out)

    def test_get_to_index(self):
        # `Object var2 = items.get(0)` -> JObject local absorbing the void* element
        self.assertIn('JObject var2 = this->items[0];', self.out)

    def test_isempty_to_empty(self):
        self.assertIn('bool b = this->items.empty();', self.out)

    def test_charat_to_index(self):
        self.assertIn('char c = this->name[2];', self.out)

    def test_size_unchanged(self):
        self.assertIn('int n = this->items.size();', self.out)


class TestValueContainers(unittest.TestCase):
    """Java collections are by-value std::vector/map/set; `new ArrayList()` is value
    construction, not a heap pointer, so method calls use '.' not '->'."""
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('tagList', ir.JType('List')))
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void f(int var1) {'),
            (2, '\tList var2 = new ArrayList();'),       # value local, not a pointer
            (3, '\tvar2.add(var1);'),                    # value receiver -> push_back
            (4, '\tthis.tagList = new ArrayList();'),     # field assign -> value temporary
            (5, '\tArrayList var3 = new ArrayList();'),
            (6, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_local_container_decl_default_constructs(self):
        self.assertIn('JList<JObject> var2;', self.out)              # initializer dropped
        self.assertFalse(any('var2 =' in l and 'new' in l for l in self.out), self.out)

    def test_value_container_method_uses_dot(self):
        self.assertIn('var2.push_back(var1);', self.out)           # '.' not '->'

    def test_field_assign_is_value_temporary(self):
        self.assertIn('this->tagList = JList<JObject>();', self.out)


class TestMapSetString(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('map', ir.JType('HashMap')))
        c.fields.append(ir.Field('set', ir.JType('HashSet')))
        m = ir.Method('f', ir.JType('void'),
                      params=[ir.Param('var1', ir.JType('String')), ir.Param('var2', ir.JType('Object'))], line=1)
        m.body = [
            (1, 'void f(String var1, Object var2) {'),
            (2, '\tthis.map.put(var1, var2);'),
            (3, '\tObject var3 = this.map.get(var1);'),
            (4, '\tboolean b = this.map.containsKey(var1);'),
            (5, '\tthis.map.remove(var1);'),
            (6, '\tthis.set.add(var1);'),
            (7, '\tboolean c = this.set.contains(var1);'),
            (8, '\tboolean d = var1.equals("foo");'),
            (9, '\tString var4 = var1.substring(2, 5);'),
            (10, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_map_put(self):
        self.assertIn('this->map[var1] = var2;', self.out)

    def test_split_top_commas_handles_arrow(self):
        # '->' must not mis-balance the comma splitter (regression: map.put(a->b, c))
        self.assertEqual(statements._split_top_commas('var1->entityID, var9'),
                         ['var1->entityID', 'var9'])

    def test_map_get(self):
        self.assertIn('JObject var3 = this->map.get(var1);', self.out)

    def test_map_containskey_and_remove(self):
        self.assertIn('bool b = this->map.containsKey(var1);', self.out)
        self.assertIn('this->map.erase(var1);', self.out)

    def test_set_add_and_contains(self):
        # sets map to JSet<T> (a std::set subclass with the native Java API), so add/contains are
        # NOT remapped to insert/count -- they bind to JSet's own methods (correct Java return types).
        self.assertIn('this->set.add(var1);', self.out)
        self.assertIn('bool c = this->set.contains(var1);', self.out)

    def test_string_equals(self):
        self.assertIn('bool d = (var1 == "foo");', self.out)

    def test_string_substring(self):
        self.assertIn('JavaString var4 = var1.substr(2, (5) - (2));', self.out)


class TestJMapViews(unittest.TestCase):
    """Java Map is JMap<K,V> (a std::map subclass with the Java API): the view methods
    values()/keySet()/entrySet() bind to JMap members, so they survive translation (not rewritten to
    std::map, which lacks them). Fields declared HashMap/Map/TreeMap emit JMap<JObject, JObject>."""
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('map', ir.JType('HashMap')))
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [
            (1, 'void f() {'),
            (2, '\tthis.list.addAll(this.map.values());'),          # values() as a collection arg
            (3, '\tIterator var1 = this.map.entrySet().iterator();'),  # entrySet().iterator()
            (4, '\tint n = this.map.size();'),
            (5, '\tthis.map.clear();'),
            (6, '\tboolean e = this.map != null;'),                 # null-compare -> != 0
            (7, '}'),
        ]
        c.methods.append(m)
        self.out = '\n'.join(statements.translate_body(m, c, types.TypeResolver()))

    def test_field_is_jmap(self):
        self.assertEqual(types.TypeResolver().field_type(ir.JType('HashMap')), 'JMap<JObject, JObject>')

    def test_values_preserved_as_member(self):
        self.assertIn('this->map.values()', self.out)              # NOT rewritten away; binds to JMap::values

    def test_entryset_iterator_preserved(self):
        self.assertIn('this->map.entrySet().iterator()', self.out)

    def test_size_and_clear_inherited(self):
        self.assertIn('this->map.size()', self.out)
        self.assertIn('this->map.clear()', self.out)

    def test_null_compare(self):
        # `map != null` -> `map != NULL` (0); binds to JMap::operator!=(long long) (never-null by value).
        self.assertIn('this->map != NULL', self.out)


class TestJMapRuntimeHeader(unittest.TestCase):
    """The emitted JMap.h exposes the full Java Map surface as real members so calls bind on any
    receiver (chained/foreign), mirroring the JList/JSet pattern."""
    def setUp(self):
        from j2ctool import runtime
        self.files = runtime.generate_files()

    def test_jmap_header_emitted(self):
        self.assertIn('JMap.h', self.files)
        self.assertIn('JIterator.h', self.files)                  # Iterator extracted so JMap can reuse it

    def test_jmap_has_java_map_surface(self):
        h = self.files['JMap.h']
        self.assertIn('class JMap : public std::map<K, V>', h)
        for member in ('put', 'get', 'containsKey', 'remove', 'isEmpty',
                       'values', 'keySet', 'entrySet', 'operator==', 'operator!='):
            self.assertIn(member, h, member)

    def test_list_and_set_expose_iterator_member(self):
        # chained/temporary receivers (map.values().iterator(), (JSet<..>(..)).iterator()) can't be
        # reached by the text rewrite, so JList/JSet carry a real iterator() member.
        self.assertIn('Iterator* iterator()', self.files['JList.h'])
        self.assertIn('Iterator* iterator()', self.files['JSet.h'])

    def test_entry_carries_key_value(self):
        e = self.files['Entry.h']
        self.assertIn('Entry(void* k, void* v)', e)
        self.assertIn('getKey', e)
        self.assertIn('getValue', e)


class TestStringConcat(unittest.TestCase):
    def test_concat_return_to_jcat(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('x', ir.JType('float')))
        c.fields.append(ir.Field('y', ir.JType('float')))
        m = ir.Method('toString', ir.JType('String'), line=1)
        m.body = [(1, 'String toString() {'),
                  (2, '\treturn "(" + this.x + ", " + this.y + ")";'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return (JCat() + "(" + this->x + ", " + this->y + ")");', out)

    def test_concat_in_call_arg(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tSystem.out.println("n=" + var1 + "!");'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertTrue(any('(JCat() + "n=" + var1 + "!")' in l for l in out), out)

    def test_concat_in_assignment(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tString s = "a" + var1 + "b";'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertTrue(any('= (JCat() + "a" + var1 + "b");' in l for l in out), out)

    def test_float_mod_to_fmodf(self):
        self.assertEqual(statements._rewrite_float_mod('var1 % 4.0F'), 'fmodf(var1, 4.0F)')
        self.assertEqual(statements._rewrite_float_mod('this->c % 4.0F == 0.0F'),
                         'fmodf(this->c, 4.0F) == 0.0F')

    def test_int_mod_unchanged(self):
        self.assertEqual(statements._rewrite_float_mod('a[i % 4] = b'), 'a[i % 4] = b')
        self.assertEqual(statements._rewrite_float_mod('x = y % z'), 'x = y % z')

    def test_no_concat_leaves_plain_return(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('int'), line=1)
        m.body = [(1, 'int f() {'), (2, '\treturn a + b;'), (3, '}')]   # numeric add, no string
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return a + b;', out)

    def test_literal_method_not_wrapped(self):
        # "abc".length() + 5 is numeric, not concat -- must NOT be wrapped
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('int'), line=1)
        m.body = [(1, 'int f() {'), (2, '\treturn "abc".length() + 5;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertFalse(any('JCat()' in l for l in out), out)


class TestCasts(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'),
                      params=[ir.Param('var1', ir.JType('Object')), ir.Param('f', ir.JType('float'))], line=1)
        m.body = [
            (1, 'void f(Object var1, float f) {'),
            (2, '\tBlock var2 = (Block)var1;'),
            (3, '\tint var3 = (int)f;'),
            (4, '\tVec3D var4 = (Vec3D)var1;'),
            (5, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Block'}, value_types={'Vec3D'})
        self.out = statements.translate_body(m, c, r)

    def test_reference_cast_gets_pointer(self):
        self.assertIn('Block* var2 = (Block*)var1;', self.out)

    def test_primitive_cast_unchanged(self):
        self.assertIn('int var3 = (int)f;', self.out)

    def test_value_cast_unchanged(self):
        self.assertIn('Vec3D var4 = (Vec3D)var1;', self.out)


class TestExpressionDeref(unittest.TestCase):
    """Index-result and cast-result expressions yield pointers -> '.' becomes '->'."""
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('entities', ir.JType('Entity', array_dims=1)))   # ref array
        c.fields.append(ir.Field('points', ir.JType('Vec3D', array_dims=1)))      # value array
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('Object'))], line=1)
        m.body = [
            (1, 'void f(Object var1) {'),
            (2, '\tBlock var2 = this.entities[0].getBlock();'),     # ref-array index -> ->
            (3, '\tString s = Block.blocksList[var1].getName();'),  # registry index -> ->
            (4, '\tfloat z = this.points[0].xCoord;'),              # value-array index -> stays .
            (5, '\tint i = ((Block)var1).getRenderType();'),        # cast-result -> ->
            (6, '\tfloat w = ((Vec3D)var1).xCoord;'),               # value cast-result -> stays .
            (7, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Block', 'Entity', 'R'}, value_types={'Vec3D'})
        self.out = statements.translate_body(m, c, r)

    def test_ref_array_index_deref(self):
        self.assertIn('Block* var2 = this->entities[0]->getBlock();', self.out)

    def test_registry_index_deref(self):
        self.assertIn('JavaString s = Block::s_blocksList[var1]->getName();', self.out)

    def test_value_array_index_keeps_dot(self):
        self.assertIn('float z = this->points[0].xCoord;', self.out)

    def test_cast_result_deref(self):
        self.assertIn('int i = ((Block*)var1)->getRenderType();', self.out)

    def test_value_cast_result_keeps_dot(self):
        self.assertIn('float w = ((Vec3D)var1).xCoord;', self.out)


class TestPrimitiveCastAndMathConst(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void f(int var1) {'),
            (2, '\tbyte var2 = (byte)var1;'),          # primitive cast remapped
            (3, '\tboolean var3 = (boolean)var1;'),
            (4, '\tfloat var4 = (float)var1;'),         # self-mapping -> unchanged
            (5, '\tfloat p = (float)Math.PI;'),         # Math.PI constant -> literal
            (6, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_byte_cast_remapped(self):
        self.assertIn('signed char var2 = (signed char)var1;', self.out)   # scalar byte is signed (Java-faithful)

    def test_boolean_cast_remapped(self):
        self.assertIn('bool var3 = (bool)var1;', self.out)

    def test_self_mapping_cast_unchanged(self):
        self.assertIn('float var4 = (float)var1;', self.out)

    def test_math_pi_constant(self):
        self.assertIn('float p = (float)3.1415927F;', self.out)


class TestNullableValueLocals(unittest.TestCase):
    """A value-type local used as a nullable reference (assigned/compared null, or new'd)
    must be emitted as a pointer -- a C++ value type can't hold null."""
    def setUp(self):
        c = ir.JClass('AxisAlignedBB')
        m = ir.Method('trace', ir.JType('void'), params=[ir.Param('var1', ir.JType('Vec3D'))], line=1)
        m.body = [
            (1, 'void trace(Vec3D var1) {'),
            (2, '\tVec3D var3 = var1.getIntermediate();'),   # dual-use return
            (3, '\tif(!this.isVecIn(var3)) {'),
            (4, '\t\tvar3 = null;'),                         # null -> must be pointer
            (5, '\t}'),
            (6, '\tVec3D var8 = null;'),                     # declared null
            (7, '\tif(var3 != null) {'),
            (8, '\t\tvar8 = var3;'),
            (9, '\t}'),
            (10, '\tVec3D var9 = var1.normalize();'),         # value local, no null use
            (11, '\tfloat f = var9.xCoord;'),
            (12, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'AxisAlignedBB'}, value_types={'Vec3D'})
        self.out = statements.translate_body(m, c, r)

    def test_nullable_local_becomes_pointer(self):
        self.assertIn('Vec3D* var3 = var1.getIntermediate();', self.out)
        self.assertIn('Vec3D* var8 = NULL;', self.out)

    def test_nullable_local_assignments_and_compares(self):
        self.assertIn('var3 = NULL;', self.out)
        self.assertIn('var8 = var3;', self.out)
        self.assertTrue(any('var3 != NULL' in l for l in self.out), self.out)

    def test_plain_value_local_stays_value(self):
        self.assertIn('Vec3D var9 = var1.normalize();', self.out)   # no null use -> stays value
        self.assertIn('float f = var9.xCoord;', self.out)           # value access keeps '.'


class TestBareStaticPrefix(unittest.TestCase):
    def test_bare_static_field_gets_prefix(self):
        c = ir.JClass('MathHelper')
        c.fields.append(ir.Field('SIN_TABLE', ir.JType('float', array_dims=1), is_static=True))
        m = ir.Method('sin', ir.JType('float'), params=[ir.Param('var0', ir.JType('float'))], line=1)
        m.body = [(1, 'float sin(float var0) {'),
                  (2, '\treturn SIN_TABLE[(int)var0 & 255];'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return s_SIN_TABLE[(int)var0 & 255];', out)

    def test_bare_static_prefix_skips_string_literals(self):
        # a static field named 'stone' must NOT corrupt the string literal "stone"
        c = ir.JClass('Block')
        c.fields.append(ir.Field('stone', ir.JType('Block'), is_static=True))
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tthis.register(stone, "stone");'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Block'}))
        self.assertTrue(any('s_stone, "stone"' in l for l in out), out)   # ident renamed, literal intact

    def test_qualified_static_not_double_prefixed(self):
        c = ir.JClass('Block')
        c.fields.append(ir.Field('blocksList', ir.JType('Block', array_dims=1), is_static=True))
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tBlock var1 = Block.blocksList[0];'),   # qualified -> Block::s_blocksList
                  (3, '\tBlock var2 = blocksList[1];'),          # bare -> s_blocksList
                  (4, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Block'}))
        self.assertIn('Block* var1 = Block::s_blocksList[0];', out)
        self.assertIn('Block* var2 = s_blocksList[1];', out)
        self.assertFalse(any('s_s_' in l for l in out), out)    # never double-prefixed


class TestChainInference(unittest.TestCase):
    """Member environment drives chained '.' vs '->' (static reference field; pointer
    field of a typed receiver)."""
    def _resolver(self):
        # Block has a static pointer field 'tilledField' and an int field 'blockID';
        # World has a pointer field 'random' (Random). Flattened member env, as the
        # pipeline would build it.
        members = {
            'Block': {'fields': {'tilledField': ir.JType('Block'), 'blockID': ir.JType('int')},
                      'methods': {}},
            'World': {'fields': {'random': ir.JType('Random')}, 'methods': {}},
        }
        return types.TypeResolver(known_classes={'Block', 'World', 'Random', 'C'},
                                  value_types={'Vec3D'}, members=members)

    def test_static_reference_field_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tint i = Block.tilledField.blockID;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('int i = Block::s_tilledField->blockID;', out)

    def test_pointer_field_of_param_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('World'))], line=1)
        m.body = [(1, 'void f(World var1) {'),
                  (2, '\tfloat g = var1.random.nextFloat();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('float g = var1->random->nextFloat();', out)

    def test_pointer_field_chain_with_string_return(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('World'))], line=1)
        m.body = [(1, 'void f(World var1) {'),
                  (2, '\tString s = var1.random.toString();'),   # random is ptr -> ->
                  (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('JavaString s = var1->random->toString();', out)

    def test_deep_pointer_chain(self):
        # this->mc->options->keyBindInventory.keyCode  -- every link a pointer field, to depth
        members = {
            'C': {'fields': {'mc': ir.JType('Minecraft')}, 'methods': {}},
            'Minecraft': {'fields': {'options': ir.JType('GameSettings')}, 'methods': {}},
            'GameSettings': {'fields': {'keyBindInventory': ir.JType('KeyBinding')}, 'methods': {}},
            'KeyBinding': {'fields': {'keyCode': ir.JType('int')}, 'methods': {}},
        }
        r = types.TypeResolver(known_classes={'C', 'Minecraft', 'GameSettings', 'KeyBinding'},
                               members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'), (2, '\tint k = this.mc.options.keyBindInventory.keyCode;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('int k = this->mc->options->keyBindInventory->keyCode;', out)

    def test_bare_inherited_static_chain(self):
        # s_rand.nextFloat() -- bare emitted static (Item.rand inherited), should deref
        members = {'ItemBow': {'fields': {'rand': ir.JType('Random')}, 'methods': {}}}
        r = types.TypeResolver(known_classes={'ItemBow', 'Random'}, members=members)
        c = ir.JClass('ItemBow')
        m = ir.Method('f', ir.JType('float'), line=1)
        m.body = [(1, 'float f() {'), (2, '\treturn s_rand.nextFloat();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertTrue(any('s_rand->nextFloat()' in l for l in out), out)

    def test_container_method_on_foreign_receiver(self):
        # obj->listField.get(i) / .add(x) -- receiver not in scope; mapped via the member env
        members = {'World': {'fields': {'players': ir.JType('List')}, 'methods': {}}}
        r = types.TypeResolver(known_classes={'World', 'C'}, members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('World'))], line=1)
        m.body = [(1, 'void f(World var1) {'),
                  (2, '\tObject var2 = var1.players.get(0);'),
                  (3, '\tvar1.players.add(var2);'), (4, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('JObject var2 = var1->players[0];', out)
        self.assertIn('var1->players.push_back(var2);', out)

    def test_array_index_field_chain(self):
        # var5->mainInventory[i].writeToNBT() -- array field of a foreign object, index -> element
        members = {'InventoryPlayer': {'fields': {'mainInventory': ir.JType('ItemStack', array_dims=1)},
                                       'methods': {}}}
        r = types.TypeResolver(known_classes={'InventoryPlayer', 'ItemStack', 'C'}, members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'),
                      params=[ir.Param('var5', ir.JType('InventoryPlayer'))], line=1)
        m.body = [(1, 'void f(InventoryPlayer var5) {'),
                  (2, '\tvar5.mainInventory[var3].writeToNBT(var4);'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('var5->mainInventory[var3]->writeToNBT(var4);', out)

    def test_unknown_receiver_left_alone(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tint i = Mystery.foo.bar;'), (3, '}')]   # Mystery unknown -> untouched
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertTrue(any('Mystery' in l and '.bar' in l for l in out), out)


class TestCallResultChains(unittest.TestCase):
    """A call/ctor result that is a pointer, then .member -> -> (method-result inference)."""
    def _resolver(self):
        members = {
            'ItemStack': {'fields': {}, 'methods': {'getItem': ir.JType('Item')}},
            'CraftingManager': {'fields': {},
                                'methods': {'getInstance': ir.JType('CraftingManager'),
                                            'findRecipe': ir.JType('ItemStack')}},
        }
        return types.TypeResolver(
            known_classes={'ItemStack', 'Item', 'CraftingManager', 'ThreadDownloadImage', 'C'},
            members=members)

    def test_instance_method_result_chain(self):
        # getItem() is a known-nullable getter, so with its return type known the chain is NULL-guarded
        # (the crash-class defense added in a1db028) and every hop on the ItemStack* receiver uses '->'
        # -- never a '.' on a pointer. Local resolver so getItem()'s return type is resolvable.
        r = types.TypeResolver(known_classes={'C', 'ItemStack', 'Item'},
                               members={'ItemStack': {'fields': {},
                                                      'methods': {'getItem': ir.JType('Item')}}})
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var6', ir.JType('ItemStack'))], line=1)
        m.body = [(1, 'void f(ItemStack var6) {'),
                  (2, '\tvar6.getItem().onPlaced();'), (3, '}')]
        c.methods.append(m)
        out = '\n'.join(statements.translate_body(m, c, r))
        self.assertIn('var6->getItem()', out)          # receiver hop uses '->'
        self.assertIn('->onPlaced()', out)             # result hop uses '->', not '.'
        self.assertNotIn('getItem().onPlaced', out)    # never a '.' deref on the pointer
        self.assertIn('NULL', out)                     # nullable getter is guarded
        self.assertIn('Item* ', out)                   # typed temp (C++03), not `auto`
        self.assertNotIn('auto', out)                  # no C++11 in C++03 output

    def test_static_method_result_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tItemStack var1 = CraftingManager.getInstance().findRecipe();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('CraftingManager::getInstance()->findRecipe();', out[0])

    def test_fluent_new_builder_chain(self):
        # new BlockX(...).setHardness(a).setResistance(b) -- builder returns Block*, deref to depth
        members = {'BlockX': {'fields': {}, 'methods': {'setHardness': ir.JType('Block')}},
                   'Block': {'fields': {}, 'methods': {'setHardness': ir.JType('Block'),
                                                       'setResistance': ir.JType('Block')}}}
        r = types.TypeResolver(known_classes={'BlockX', 'Block', 'C'}, members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tBlock var1 = new BlockX(1, 2).setHardness(1.5F).setResistance(10.0F);'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('Block* var1 = (new BlockX(1, 2))->setHardness(1.5F)->setResistance(10.0F);', out)
        # already-parenthesized form (as the decompiler emits it) chains too
        m2 = ir.Method('g', ir.JType('void'), line=1)
        m2.body = [(1, 'void g() {'),
                   (2, '\tBlock var2 = (new BlockX(1, 2)).setHardness(1.5F).setResistance(10.0F);'), (3, '}')]
        c2 = ir.JClass('C'); c2.methods.append(m2)
        out2 = statements.translate_body(m2, c2, r)
        self.assertIn('Block* var2 = (new BlockX(1, 2))->setHardness(1.5F)->setResistance(10.0F);', out2)

    def test_new_result_chain_wrapped(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tnew ThreadDownloadImage(this, var1).start();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('(new ThreadDownloadImage(this, var1))->start();', out)

    def test_already_parenthesized_new_result(self):
        # source already wrote (new X(...)).start() -> just fix the '.' to '->'
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\t(new ThreadDownloadImage(this, var1, var2)).start();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('(new ThreadDownloadImage(this, var1, var2))->start();', out)


class TestSuperAndInstanceof(unittest.TestCase):
    def test_instanceof_to_dynamic_cast(self):
        c = ir.JClass('R', parent='Base')
        m = ir.Method('f', ir.JType('boolean'), params=[ir.Param('var1', ir.JType('Entity'))], line=1)
        m.body = [(1, 'boolean f(Entity var1) {'),
                  (2, '\treturn var1 instanceof EntityPlayer;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Entity', 'EntityPlayer'}))
        self.assertIn('return (dynamic_cast<EntityPlayer*>(var1) != NULL);', out)

    def test_super_method_to_qualified(self):
        c = ir.JClass('R', parent='EntityLiving')
        m = ir.Method('onUpdate', ir.JType('void'), line=1)
        m.body = [(1, 'void onUpdate() {'), (2, '\tsuper.onUpdate();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('EntityLiving::onUpdate();', out)


class TestInheritedFields(unittest.TestCase):
    def test_inherited_pointer_field_access(self):
        c = ir.JClass('ModelSheep', parent='ModelBase')
        c.inherited_fields = [ir.Field('head', ir.JType('ModelRenderer'))]   # inherited, pointer
        m = ir.Method('render', ir.JType('void'), line=1)
        m.body = [(1, 'void render() {'), (2, '\tthis.head.addBox(1, 2, 3);'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'ModelRenderer'}))
        self.assertIn('this->head->addBox(1, 2, 3);', out)   # inherited field resolved as pointer


class TestInBodyNewArray(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [
            (1, 'void f() {'),
            (2, '\tint[] var1 = new int[256];'),
            (3, '\tboolean[] var2 = new boolean[10];'),
            (4, '\tBlock[] var3 = new Block[256];'),
            (5, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Block'}))

    def test_primitive_array_to_jarray(self):
        self.assertIn('JArray<int > var1 = JArray<int >(256);', self.out)
        self.assertIn('JArray<bool > var2 = JArray<bool >(10);', self.out)   # boolean->bool too

    def test_reference_array_to_jarray_of_pointers(self):
        self.assertIn('JArray<Block* > var3 = JArray<Block* >(256);', self.out)


class TestLabeledGoto(unittest.TestCase):
    def test_labeled_break_and_continue(self):
        lines = [
            'label0:',
            'for(int var1 = 0; var1 < 4; ++var1) {',
            'for(int var2 = 0; var2 < 4; ++var2) {',
            'if(a) {',
            'continue label0;',
            '}',
            'if(b) {',
            'break label0;',
            '}',
            '}',
            '}',
        ]
        out = statements._translate_labels(lines)
        self.assertNotIn('label0:', out)                  # original label removed
        self.assertIn('goto label0_cont;', out)
        self.assertIn('goto label0_brk;', out)
        ci, bi = out.index('label0_cont:;'), out.index('label0_brk:;')
        self.assertLess(ci, bi)
        self.assertEqual(out[ci + 1], '}')                # cont sits just before the outer loop's close
        self.assertEqual(out[ci + 2], 'label0_brk:;')     # brk just after it

    def test_unlabeled_break_continue_untouched(self):
        lines = ['while(x) {', 'break;', 'continue;', '}']
        self.assertEqual(statements._translate_labels(lines), lines)


class TestNullDerefGuard(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('World')
        c.fields.append(ir.Field('chunks', ir.JType('Chunk', 1)))    # Chunk[] -> pointer slots
        m = ir.Method('tick', ir.JType('void'),
                      params=[ir.Param('entity', ir.JType('Entity')),
                              ir.Param('var1', ir.JType('int')), ir.Param('k', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void tick(Entity entity, int var1, int k) {'),
            (2, '\tBlock.blocksList[var1].onUpdate(this, var1, k, var1);'),
            (3, '\tthis.chunks[k].markDirty();'),
            (4, '\tentity.move();'),
            (5, '}'),
        ]
        c.methods.append(m)
        self.led = Ledger()
        r = types.TypeResolver(known_classes={'Block', 'Chunk', 'World', 'Entity'})
        self.out = statements.translate_body(m, c, r, ledger=self.led)

    def test_registry_deref_guarded(self):
        self.assertIn('if (Block::s_blocksList[var1] != NULL) { '
                      'Block::s_blocksList[var1]->onUpdate(this, var1, k, var1); }', self.out)

    def test_scope_array_deref_guarded(self):
        self.assertIn('if (this->chunks[k] != NULL) { this->chunks[k]->markDirty(); }', self.out)

    def test_non_index_call_not_guarded(self):
        self.assertIn('entity->move();', self.out)
        self.assertNotIn('if (entity', '\n'.join(self.out))

    def test_guard_events_logged(self):
        self.assertEqual(sum(1 for e in self.led.events if e.rule == 'null-deref-guard'), 2)


if __name__ == '__main__':
    unittest.main()
# -*- coding: utf-8 -*-
"""Module 4c pass 1 tests: per-method symbol table + value/pointer access."""

import unittest

from j2ctool import ir, types
from j2ctool.emit import statements
from j2ctool.ledger import Ledger


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


class TestStaticAndApi(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('Block')
        m = ir.Method('test', ir.JType('void'), line=1)
        m.body = [
            (1, 'void test() {'),
            (2, '\tBlock var1 = Block.stone;'),
            (3, '\tint i = Block.getRenderType(var1);'),
            (4, '\tDirection var2 = Direction.NORTH;'),
            (5, '\tfloat f = Math.sqrt(2.0F);'),
            (6, '\tfloat g = Math.max(f, 1.0F);'),
            (7, '\tint r = Math.round(f);'),
            (8, '\tvoid* cs = Charset.forName("UTF-8");'),
            (9, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Block', 'Direction'}, enum_types={'Direction'})
        self.out = statements.translate_body(m, c, r)

    def test_static_field_gets_scope_and_prefix(self):
        self.assertIn('Block* var1 = Block::s_stone;', self.out)

    def test_static_method_call(self):
        self.assertIn('int i = Block::getRenderType(var1);', self.out)

    def test_enum_constant_no_prefix(self):
        self.assertIn('int var2 = Direction::NORTH;', self.out)   # enum type -> int, constant scoped

    def test_math_mapping(self):
        self.assertIn('float f = sqrtf(2.0F);', self.out)
        self.assertIn('float g = max(f, 1.0F);', self.out)

    def test_math_round_uses_shim(self):
        # Math.round has no clean C stdlib equivalent -> the overloaded javaMathRound prelude shim.
        self.assertIn('int r = javaMathRound(f);', self.out)
        self.assertNotIn('Math.round', self.out)

    def test_charset_static_holder(self):
        # java.nio.charset Charset.forName(...) is a static factory -> `::`, not a dangling `.`.
        blob = '\n'.join(self.out)
        self.assertIn('Charset::forName("UTF-8")', blob)
        self.assertNotIn('Charset.forName', blob)


class TestCollectionsAndString(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('Inv')
        c.fields.append(ir.Field('items', ir.JType('List')))
        c.fields.append(ir.Field('name', ir.JType('String')))
        m = ir.Method('run', ir.JType('void'), params=[ir.Param('var1', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void run(int var1) {'),
            (2, '\tthis.items.add(var1);'),
            (3, '\tObject var2 = this.items.get(0);'),
            (4, '\tboolean b = this.items.isEmpty();'),
            (5, '\tchar c = this.name.charAt(2);'),
            (6, '\tint n = this.items.size();'),
            (7, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_add_to_push_back(self):
        self.assertIn('this->items.push_back(var1);', self.out)

    def test_get_to_index(self):
        # `Object var2 = items.get(0)` -> JObject local absorbing the void* element
        self.assertIn('JObject var2 = this->items[0];', self.out)

    def test_isempty_to_empty(self):
        self.assertIn('bool b = this->items.empty();', self.out)

    def test_charat_to_index(self):
        self.assertIn('char c = this->name[2];', self.out)

    def test_size_unchanged(self):
        self.assertIn('int n = this->items.size();', self.out)


class TestValueContainers(unittest.TestCase):
    """Java collections are by-value std::vector/map/set; `new ArrayList()` is value
    construction, not a heap pointer, so method calls use '.' not '->'."""
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('tagList', ir.JType('List')))
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void f(int var1) {'),
            (2, '\tList var2 = new ArrayList();'),       # value local, not a pointer
            (3, '\tvar2.add(var1);'),                    # value receiver -> push_back
            (4, '\tthis.tagList = new ArrayList();'),     # field assign -> value temporary
            (5, '\tArrayList var3 = new ArrayList();'),
            (6, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_local_container_decl_default_constructs(self):
        self.assertIn('JList<JObject> var2;', self.out)              # initializer dropped
        self.assertFalse(any('var2 =' in l and 'new' in l for l in self.out), self.out)

    def test_value_container_method_uses_dot(self):
        self.assertIn('var2.push_back(var1);', self.out)           # '.' not '->'

    def test_field_assign_is_value_temporary(self):
        self.assertIn('this->tagList = JList<JObject>();', self.out)


class TestMapSetString(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('map', ir.JType('HashMap')))
        c.fields.append(ir.Field('set', ir.JType('HashSet')))
        m = ir.Method('f', ir.JType('void'),
                      params=[ir.Param('var1', ir.JType('String')), ir.Param('var2', ir.JType('Object'))], line=1)
        m.body = [
            (1, 'void f(String var1, Object var2) {'),
            (2, '\tthis.map.put(var1, var2);'),
            (3, '\tObject var3 = this.map.get(var1);'),
            (4, '\tboolean b = this.map.containsKey(var1);'),
            (5, '\tthis.map.remove(var1);'),
            (6, '\tthis.set.add(var1);'),
            (7, '\tboolean c = this.set.contains(var1);'),
            (8, '\tboolean d = var1.equals("foo");'),
            (9, '\tString var4 = var1.substring(2, 5);'),
            (10, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_map_put(self):
        self.assertIn('this->map[var1] = var2;', self.out)

    def test_split_top_commas_handles_arrow(self):
        # '->' must not mis-balance the comma splitter (regression: map.put(a->b, c))
        self.assertEqual(statements._split_top_commas('var1->entityID, var9'),
                         ['var1->entityID', 'var9'])

    def test_map_get(self):
        self.assertIn('JObject var3 = this->map.get(var1);', self.out)

    def test_map_containskey_and_remove(self):
        self.assertIn('bool b = this->map.containsKey(var1);', self.out)
        self.assertIn('this->map.erase(var1);', self.out)

    def test_set_add_and_contains(self):
        # sets map to JSet<T> (a std::set subclass with the native Java API), so add/contains are
        # NOT remapped to insert/count -- they bind to JSet's own methods (correct Java return types).
        self.assertIn('this->set.add(var1);', self.out)
        self.assertIn('bool c = this->set.contains(var1);', self.out)

    def test_string_equals(self):
        self.assertIn('bool d = (var1 == "foo");', self.out)

    def test_string_substring(self):
        self.assertIn('JavaString var4 = var1.substr(2, (5) - (2));', self.out)


class TestStringConcat(unittest.TestCase):
    def test_concat_return_to_jcat(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('x', ir.JType('float')))
        c.fields.append(ir.Field('y', ir.JType('float')))
        m = ir.Method('toString', ir.JType('String'), line=1)
        m.body = [(1, 'String toString() {'),
                  (2, '\treturn "(" + this.x + ", " + this.y + ")";'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return (JCat() + "(" + this->x + ", " + this->y + ")");', out)

    def test_concat_in_call_arg(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tSystem.out.println("n=" + var1 + "!");'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertTrue(any('(JCat() + "n=" + var1 + "!")' in l for l in out), out)

    def test_concat_in_assignment(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tString s = "a" + var1 + "b";'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertTrue(any('= (JCat() + "a" + var1 + "b");' in l for l in out), out)

    def test_float_mod_to_fmodf(self):
        self.assertEqual(statements._rewrite_float_mod('var1 % 4.0F'), 'fmodf(var1, 4.0F)')
        self.assertEqual(statements._rewrite_float_mod('this->c % 4.0F == 0.0F'),
                         'fmodf(this->c, 4.0F) == 0.0F')

    def test_int_mod_unchanged(self):
        self.assertEqual(statements._rewrite_float_mod('a[i % 4] = b'), 'a[i % 4] = b')
        self.assertEqual(statements._rewrite_float_mod('x = y % z'), 'x = y % z')

    def test_no_concat_leaves_plain_return(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('int'), line=1)
        m.body = [(1, 'int f() {'), (2, '\treturn a + b;'), (3, '}')]   # numeric add, no string
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return a + b;', out)

    def test_literal_method_not_wrapped(self):
        # "abc".length() + 5 is numeric, not concat -- must NOT be wrapped
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('int'), line=1)
        m.body = [(1, 'int f() {'), (2, '\treturn "abc".length() + 5;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertFalse(any('JCat()' in l for l in out), out)


class TestCasts(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'),
                      params=[ir.Param('var1', ir.JType('Object')), ir.Param('f', ir.JType('float'))], line=1)
        m.body = [
            (1, 'void f(Object var1, float f) {'),
            (2, '\tBlock var2 = (Block)var1;'),
            (3, '\tint var3 = (int)f;'),
            (4, '\tVec3D var4 = (Vec3D)var1;'),
            (5, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Block'}, value_types={'Vec3D'})
        self.out = statements.translate_body(m, c, r)

    def test_reference_cast_gets_pointer(self):
        self.assertIn('Block* var2 = (Block*)var1;', self.out)

    def test_primitive_cast_unchanged(self):
        self.assertIn('int var3 = (int)f;', self.out)

    def test_value_cast_unchanged(self):
        self.assertIn('Vec3D var4 = (Vec3D)var1;', self.out)


class TestExpressionDeref(unittest.TestCase):
    """Index-result and cast-result expressions yield pointers -> '.' becomes '->'."""
    def setUp(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('entities', ir.JType('Entity', array_dims=1)))   # ref array
        c.fields.append(ir.Field('points', ir.JType('Vec3D', array_dims=1)))      # value array
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('Object'))], line=1)
        m.body = [
            (1, 'void f(Object var1) {'),
            (2, '\tBlock var2 = this.entities[0].getBlock();'),     # ref-array index -> ->
            (3, '\tString s = Block.blocksList[var1].getName();'),  # registry index -> ->
            (4, '\tfloat z = this.points[0].xCoord;'),              # value-array index -> stays .
            (5, '\tint i = ((Block)var1).getRenderType();'),        # cast-result -> ->
            (6, '\tfloat w = ((Vec3D)var1).xCoord;'),               # value cast-result -> stays .
            (7, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'Block', 'Entity', 'R'}, value_types={'Vec3D'})
        self.out = statements.translate_body(m, c, r)

    def test_ref_array_index_deref(self):
        self.assertIn('Block* var2 = this->entities[0]->getBlock();', self.out)

    def test_registry_index_deref(self):
        self.assertIn('JavaString s = Block::s_blocksList[var1]->getName();', self.out)

    def test_value_array_index_keeps_dot(self):
        self.assertIn('float z = this->points[0].xCoord;', self.out)

    def test_cast_result_deref(self):
        self.assertIn('int i = ((Block*)var1)->getRenderType();', self.out)

    def test_value_cast_result_keeps_dot(self):
        self.assertIn('float w = ((Vec3D)var1).xCoord;', self.out)


class TestPrimitiveCastAndMathConst(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void f(int var1) {'),
            (2, '\tbyte var2 = (byte)var1;'),          # primitive cast remapped
            (3, '\tboolean var3 = (boolean)var1;'),
            (4, '\tfloat var4 = (float)var1;'),         # self-mapping -> unchanged
            (5, '\tfloat p = (float)Math.PI;'),         # Math.PI constant -> literal
            (6, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver())

    def test_byte_cast_remapped(self):
        self.assertIn('signed char var2 = (signed char)var1;', self.out)   # scalar byte is signed (Java-faithful)

    def test_boolean_cast_remapped(self):
        self.assertIn('bool var3 = (bool)var1;', self.out)

    def test_self_mapping_cast_unchanged(self):
        self.assertIn('float var4 = (float)var1;', self.out)

    def test_math_pi_constant(self):
        self.assertIn('float p = (float)3.1415927F;', self.out)


class TestNullableValueLocals(unittest.TestCase):
    """A value-type local used as a nullable reference (assigned/compared null, or new'd)
    must be emitted as a pointer -- a C++ value type can't hold null."""
    def setUp(self):
        c = ir.JClass('AxisAlignedBB')
        m = ir.Method('trace', ir.JType('void'), params=[ir.Param('var1', ir.JType('Vec3D'))], line=1)
        m.body = [
            (1, 'void trace(Vec3D var1) {'),
            (2, '\tVec3D var3 = var1.getIntermediate();'),   # dual-use return
            (3, '\tif(!this.isVecIn(var3)) {'),
            (4, '\t\tvar3 = null;'),                         # null -> must be pointer
            (5, '\t}'),
            (6, '\tVec3D var8 = null;'),                     # declared null
            (7, '\tif(var3 != null) {'),
            (8, '\t\tvar8 = var3;'),
            (9, '\t}'),
            (10, '\tVec3D var9 = var1.normalize();'),         # value local, no null use
            (11, '\tfloat f = var9.xCoord;'),
            (12, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'AxisAlignedBB'}, value_types={'Vec3D'})
        self.out = statements.translate_body(m, c, r)

    def test_nullable_local_becomes_pointer(self):
        self.assertIn('Vec3D* var3 = var1.getIntermediate();', self.out)
        self.assertIn('Vec3D* var8 = NULL;', self.out)

    def test_nullable_local_assignments_and_compares(self):
        self.assertIn('var3 = NULL;', self.out)
        self.assertIn('var8 = var3;', self.out)
        self.assertTrue(any('var3 != NULL' in l for l in self.out), self.out)

    def test_plain_value_local_stays_value(self):
        self.assertIn('Vec3D var9 = var1.normalize();', self.out)   # no null use -> stays value
        self.assertIn('float f = var9.xCoord;', self.out)           # value access keeps '.'


class TestBareStaticPrefix(unittest.TestCase):
    def test_bare_static_field_gets_prefix(self):
        c = ir.JClass('MathHelper')
        c.fields.append(ir.Field('SIN_TABLE', ir.JType('float', array_dims=1), is_static=True))
        m = ir.Method('sin', ir.JType('float'), params=[ir.Param('var0', ir.JType('float'))], line=1)
        m.body = [(1, 'float sin(float var0) {'),
                  (2, '\treturn SIN_TABLE[(int)var0 & 255];'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return s_SIN_TABLE[(int)var0 & 255];', out)

    def test_bare_static_prefix_skips_string_literals(self):
        # a static field named 'stone' must NOT corrupt the string literal "stone"
        c = ir.JClass('Block')
        c.fields.append(ir.Field('stone', ir.JType('Block'), is_static=True))
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tthis.register(stone, "stone");'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Block'}))
        self.assertTrue(any('s_stone, "stone"' in l for l in out), out)   # ident renamed, literal intact

    def test_qualified_static_not_double_prefixed(self):
        c = ir.JClass('Block')
        c.fields.append(ir.Field('blocksList', ir.JType('Block', array_dims=1), is_static=True))
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tBlock var1 = Block.blocksList[0];'),   # qualified -> Block::s_blocksList
                  (3, '\tBlock var2 = blocksList[1];'),          # bare -> s_blocksList
                  (4, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Block'}))
        self.assertIn('Block* var1 = Block::s_blocksList[0];', out)
        self.assertIn('Block* var2 = s_blocksList[1];', out)
        self.assertFalse(any('s_s_' in l for l in out), out)    # never double-prefixed


class TestChainInference(unittest.TestCase):
    """Member environment drives chained '.' vs '->' (static reference field; pointer
    field of a typed receiver)."""
    def _resolver(self):
        # Block has a static pointer field 'tilledField' and an int field 'blockID';
        # World has a pointer field 'random' (Random). Flattened member env, as the
        # pipeline would build it.
        members = {
            'Block': {'fields': {'tilledField': ir.JType('Block'), 'blockID': ir.JType('int')},
                      'methods': {}},
            'World': {'fields': {'random': ir.JType('Random')}, 'methods': {}},
        }
        return types.TypeResolver(known_classes={'Block', 'World', 'Random', 'C'},
                                  value_types={'Vec3D'}, members=members)

    def test_static_reference_field_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tint i = Block.tilledField.blockID;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('int i = Block::s_tilledField->blockID;', out)

    def test_pointer_field_of_param_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('World'))], line=1)
        m.body = [(1, 'void f(World var1) {'),
                  (2, '\tfloat g = var1.random.nextFloat();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('float g = var1->random->nextFloat();', out)

    def test_pointer_field_chain_with_string_return(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('World'))], line=1)
        m.body = [(1, 'void f(World var1) {'),
                  (2, '\tString s = var1.random.toString();'),   # random is ptr -> ->
                  (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('JavaString s = var1->random->toString();', out)

    def test_deep_pointer_chain(self):
        # this->mc->options->keyBindInventory.keyCode  -- every link a pointer field, to depth
        members = {
            'C': {'fields': {'mc': ir.JType('Minecraft')}, 'methods': {}},
            'Minecraft': {'fields': {'options': ir.JType('GameSettings')}, 'methods': {}},
            'GameSettings': {'fields': {'keyBindInventory': ir.JType('KeyBinding')}, 'methods': {}},
            'KeyBinding': {'fields': {'keyCode': ir.JType('int')}, 'methods': {}},
        }
        r = types.TypeResolver(known_classes={'C', 'Minecraft', 'GameSettings', 'KeyBinding'},
                               members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'), (2, '\tint k = this.mc.options.keyBindInventory.keyCode;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('int k = this->mc->options->keyBindInventory->keyCode;', out)

    def test_bare_inherited_static_chain(self):
        # s_rand.nextFloat() -- bare emitted static (Item.rand inherited), should deref
        members = {'ItemBow': {'fields': {'rand': ir.JType('Random')}, 'methods': {}}}
        r = types.TypeResolver(known_classes={'ItemBow', 'Random'}, members=members)
        c = ir.JClass('ItemBow')
        m = ir.Method('f', ir.JType('float'), line=1)
        m.body = [(1, 'float f() {'), (2, '\treturn s_rand.nextFloat();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertTrue(any('s_rand->nextFloat()' in l for l in out), out)

    def test_container_method_on_foreign_receiver(self):
        # obj->listField.get(i) / .add(x) -- receiver not in scope; mapped via the member env
        members = {'World': {'fields': {'players': ir.JType('List')}, 'methods': {}}}
        r = types.TypeResolver(known_classes={'World', 'C'}, members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var1', ir.JType('World'))], line=1)
        m.body = [(1, 'void f(World var1) {'),
                  (2, '\tObject var2 = var1.players.get(0);'),
                  (3, '\tvar1.players.add(var2);'), (4, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('JObject var2 = var1->players[0];', out)
        self.assertIn('var1->players.push_back(var2);', out)

    def test_array_index_field_chain(self):
        # var5->mainInventory[i].writeToNBT() -- array field of a foreign object, index -> element
        members = {'InventoryPlayer': {'fields': {'mainInventory': ir.JType('ItemStack', array_dims=1)},
                                       'methods': {}}}
        r = types.TypeResolver(known_classes={'InventoryPlayer', 'ItemStack', 'C'}, members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'),
                      params=[ir.Param('var5', ir.JType('InventoryPlayer'))], line=1)
        m.body = [(1, 'void f(InventoryPlayer var5) {'),
                  (2, '\tvar5.mainInventory[var3].writeToNBT(var4);'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('var5->mainInventory[var3]->writeToNBT(var4);', out)

    def test_unknown_receiver_left_alone(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tint i = Mystery.foo.bar;'), (3, '}')]   # Mystery unknown -> untouched
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertTrue(any('Mystery' in l and '.bar' in l for l in out), out)


class TestCallResultChains(unittest.TestCase):
    """A call/ctor result that is a pointer, then .member -> -> (method-result inference)."""
    def _resolver(self):
        members = {
            'ItemStack': {'fields': {}, 'methods': {'getItem': ir.JType('Item')}},
            'CraftingManager': {'fields': {},
                                'methods': {'getInstance': ir.JType('CraftingManager'),
                                            'findRecipe': ir.JType('ItemStack')}},
        }
        return types.TypeResolver(
            known_classes={'ItemStack', 'Item', 'CraftingManager', 'ThreadDownloadImage', 'C'},
            members=members)

    def test_instance_method_result_chain(self):
        # getItem() is a known-nullable getter, so with its return type known the chain is NULL-guarded
        # (the crash-class defense added in a1db028) and every hop on the ItemStack* receiver uses '->'
        # -- never a '.' on a pointer. Local resolver so getItem()'s return type is resolvable.
        r = types.TypeResolver(known_classes={'C', 'ItemStack', 'Item'},
                               members={'ItemStack': {'fields': {},
                                                      'methods': {'getItem': ir.JType('Item')}}})
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var6', ir.JType('ItemStack'))], line=1)
        m.body = [(1, 'void f(ItemStack var6) {'),
                  (2, '\tvar6.getItem().onPlaced();'), (3, '}')]
        c.methods.append(m)
        out = '\n'.join(statements.translate_body(m, c, r))
        self.assertIn('var6->getItem()', out)          # receiver hop uses '->'
        self.assertIn('->onPlaced()', out)             # result hop uses '->', not '.'
        self.assertNotIn('getItem().onPlaced', out)    # never a '.' deref on the pointer
        self.assertIn('NULL', out)                     # nullable getter is guarded
        self.assertIn('Item* ', out)                   # typed temp (C++03), not `auto`
        self.assertNotIn('auto', out)                  # no C++11 in C++03 output

    def test_static_method_result_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tItemStack var1 = CraftingManager.getInstance().findRecipe();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('CraftingManager::getInstance()->findRecipe();', out[0])

    def test_fluent_new_builder_chain(self):
        # new BlockX(...).setHardness(a).setResistance(b) -- builder returns Block*, deref to depth
        members = {'BlockX': {'fields': {}, 'methods': {'setHardness': ir.JType('Block')}},
                   'Block': {'fields': {}, 'methods': {'setHardness': ir.JType('Block'),
                                                       'setResistance': ir.JType('Block')}}}
        r = types.TypeResolver(known_classes={'BlockX', 'Block', 'C'}, members=members)
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tBlock var1 = new BlockX(1, 2).setHardness(1.5F).setResistance(10.0F);'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, r)
        self.assertIn('Block* var1 = (new BlockX(1, 2))->setHardness(1.5F)->setResistance(10.0F);', out)
        # already-parenthesized form (as the decompiler emits it) chains too
        m2 = ir.Method('g', ir.JType('void'), line=1)
        m2.body = [(1, 'void g() {'),
                   (2, '\tBlock var2 = (new BlockX(1, 2)).setHardness(1.5F).setResistance(10.0F);'), (3, '}')]
        c2 = ir.JClass('C'); c2.methods.append(m2)
        out2 = statements.translate_body(m2, c2, r)
        self.assertIn('Block* var2 = (new BlockX(1, 2))->setHardness(1.5F)->setResistance(10.0F);', out2)

    def test_new_result_chain_wrapped(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tnew ThreadDownloadImage(this, var1).start();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('(new ThreadDownloadImage(this, var1))->start();', out)

    def test_already_parenthesized_new_result(self):
        # source already wrote (new X(...)).start() -> just fix the '.' to '->'
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\t(new ThreadDownloadImage(this, var1, var2)).start();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('(new ThreadDownloadImage(this, var1, var2))->start();', out)


class TestSuperAndInstanceof(unittest.TestCase):
    def test_instanceof_to_dynamic_cast(self):
        c = ir.JClass('R', parent='Base')
        m = ir.Method('f', ir.JType('boolean'), params=[ir.Param('var1', ir.JType('Entity'))], line=1)
        m.body = [(1, 'boolean f(Entity var1) {'),
                  (2, '\treturn var1 instanceof EntityPlayer;'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Entity', 'EntityPlayer'}))
        self.assertIn('return (dynamic_cast<EntityPlayer*>(var1) != NULL);', out)

    def test_super_method_to_qualified(self):
        c = ir.JClass('R', parent='EntityLiving')
        m = ir.Method('onUpdate', ir.JType('void'), line=1)
        m.body = [(1, 'void onUpdate() {'), (2, '\tsuper.onUpdate();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('EntityLiving::onUpdate();', out)


class TestInheritedFields(unittest.TestCase):
    def test_inherited_pointer_field_access(self):
        c = ir.JClass('ModelSheep', parent='ModelBase')
        c.inherited_fields = [ir.Field('head', ir.JType('ModelRenderer'))]   # inherited, pointer
        m = ir.Method('render', ir.JType('void'), line=1)
        m.body = [(1, 'void render() {'), (2, '\tthis.head.addBox(1, 2, 3);'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver(known_classes={'ModelRenderer'}))
        self.assertIn('this->head->addBox(1, 2, 3);', out)   # inherited field resolved as pointer


class TestInBodyNewArray(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [
            (1, 'void f() {'),
            (2, '\tint[] var1 = new int[256];'),
            (3, '\tboolean[] var2 = new boolean[10];'),
            (4, '\tBlock[] var3 = new Block[256];'),
            (5, '}'),
        ]
        c.methods.append(m)
        self.out = statements.translate_body(m, c, types.TypeResolver(known_classes={'Block'}))

    def test_primitive_array_to_jarray(self):
        self.assertIn('JArray<int > var1 = JArray<int >(256);', self.out)
        self.assertIn('JArray<bool > var2 = JArray<bool >(10);', self.out)   # boolean->bool too

    def test_reference_array_to_jarray_of_pointers(self):
        self.assertIn('JArray<Block* > var3 = JArray<Block* >(256);', self.out)


class TestLabeledGoto(unittest.TestCase):
    def test_labeled_break_and_continue(self):
        lines = [
            'label0:',
            'for(int var1 = 0; var1 < 4; ++var1) {',
            'for(int var2 = 0; var2 < 4; ++var2) {',
            'if(a) {',
            'continue label0;',
            '}',
            'if(b) {',
            'break label0;',
            '}',
            '}',
            '}',
        ]
        out = statements._translate_labels(lines)
        self.assertNotIn('label0:', out)                  # original label removed
        self.assertIn('goto label0_cont;', out)
        self.assertIn('goto label0_brk;', out)
        ci, bi = out.index('label0_cont:;'), out.index('label0_brk:;')
        self.assertLess(ci, bi)
        self.assertEqual(out[ci + 1], '}')                # cont sits just before the outer loop's close
        self.assertEqual(out[ci + 2], 'label0_brk:;')     # brk just after it

    def test_unlabeled_break_continue_untouched(self):
        lines = ['while(x) {', 'break;', 'continue;', '}']
        self.assertEqual(statements._translate_labels(lines), lines)


class TestNullDerefGuard(unittest.TestCase):
    def setUp(self):
        c = ir.JClass('World')
        c.fields.append(ir.Field('chunks', ir.JType('Chunk', 1)))    # Chunk[] -> pointer slots
        m = ir.Method('tick', ir.JType('void'),
                      params=[ir.Param('entity', ir.JType('Entity')),
                              ir.Param('var1', ir.JType('int')), ir.Param('k', ir.JType('int'))], line=1)
        m.body = [
            (1, 'void tick(Entity entity, int var1, int k) {'),
            (2, '\tBlock.blocksList[var1].onUpdate(this, var1, k, var1);'),
            (3, '\tthis.chunks[k].markDirty();'),
            (4, '\tentity.move();'),
            (5, '}'),
        ]
        c.methods.append(m)
        self.led = Ledger()
        r = types.TypeResolver(known_classes={'Block', 'Chunk', 'World', 'Entity'})
        self.out = statements.translate_body(m, c, r, ledger=self.led)

    def test_registry_deref_guarded(self):
        self.assertIn('if (Block::s_blocksList[var1] != NULL) { '
                      'Block::s_blocksList[var1]->onUpdate(this, var1, k, var1); }', self.out)

    def test_scope_array_deref_guarded(self):
        self.assertIn('if (this->chunks[k] != NULL) { this->chunks[k]->markDirty(); }', self.out)

    def test_non_index_call_not_guarded(self):
        self.assertIn('entity->move();', self.out)
        self.assertNotIn('if (entity', '\n'.join(self.out))

    def test_guard_events_logged(self):
        self.assertEqual(sum(1 for e in self.led.events if e.rule == 'null-deref-guard'), 2)


if __name__ == '__main__':
    unittest.main()


class JCatTernaryTests(unittest.TestCase):
    def test_jcat_not_wrap_for_header(self):
        # a for-header (top-level ';') must never be JCat-wrapped -- caused `for((JCat() + var = ...`
        self.assertIsNone(statements._to_jcat('var9 = var7[i++] + " "; i < n; var9 = var9 + " "'))

    def test_jcat_not_wrap_embedded_assignment(self):
        self.assertIsNone(statements._to_jcat('var9 = var7[i++] + " "'))

    def test_jcat_recurses_ternary_branches(self):
        # number-first concat inside a ternary branch must be lowered (double + " s" won't compile raw)
        out = statements._to_jcat('a > 0.5 ? f(a) + " y" : var2 + " s"')
        self.assertEqual(out, 'a > 0.5 ? (JCat() + f(a) + " y") : (JCat() + var2 + " s")')

    def test_jcat_ternary_skips_scope_colon(self):
        # the `::` in Base::foo() must not be mistaken for the ternary ':'
        out = statements._to_jcat('a > 0.5 ? Base::foo()->g(a) + " y" : var2 + " s"')
        self.assertIn('(JCat() + Base::foo()->g(a) + " y")', out)
        self.assertIn('(JCat() + var2 + " s")', out)

    def test_jcat_nested_parenthesized_ternary(self):
        out = statements._to_jcat('a ? x + " p" : (b ? y + " q" : z + " r")')
        self.assertIn('(JCat() + y + " q")', out)
        self.assertIn('(JCat() + z + " r")', out)


class NullableGetterHoistTests(unittest.TestCase):
    def test_hoist_bails_on_if_condition(self):
        # hoisting inside an if-condition strands the trailing `{` and mangles the qualifier
        s = 'if(NetworkManager::getReadThread(this->x)->isAlive()) {'
        self.assertIsNone(statements._hoist_nullable_getter(s, None, types.TypeResolver()))

    def test_hoist_bails_on_while_condition(self):
        s = 'while(NetworkManager::getReadThread(this->x)->isAlive()) {'
        self.assertIsNone(statements._hoist_nullable_getter(s, None, types.TypeResolver()))

    def test_recv_qual_start_absorbs_scope(self):
        s = 'a = NetworkManager::getReadThread(x)->f();'
        i = s.index('getReadThread')
        self.assertEqual(s[statements._recv_qual_start(s, i):i], 'NetworkManager::')

    def test_recv_qual_start_absorbs_arrow_and_dot(self):
        s = 'this->obj.getReadThread(x)->f();'
        i = s.index('getReadThread')
        self.assertEqual(s[statements._recv_qual_start(s, i):i], 'this->obj.')

    def test_recv_qual_start_no_qualifier(self):
        s = 'getReadThread(x)->f();'
        i = s.index('getReadThread')
        self.assertEqual(statements._recv_qual_start(s, i), i)


class ForInitObjectDeclTests(unittest.TestCase):
    def test_forinit_pointer_class_gets_star_and_arrow(self):
        # a for-init loop var of a pointer class must be `T*` with `->` access and a null compare
        c = ir.JClass('MCHash')
        c.fields.append(ir.Field('slots', ir.JType('MCHashEntry', array_dims=1)))
        m = ir.Method('lookup', ir.JType('Object'), line=1)
        m.body = [(1, 'Object lookup(int var1) {'),
                  (2, '\tfor(MCHashEntry var3 = this.slots[0]; var3 != null; var3 = var3.nextEntry) {'),
                  (3, '\t\tif(var3.hashEntry == var1) { return var3.valueEntry; }'),
                  (4, '\t}'), (5, '\treturn null;'), (6, '}')]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'MCHash', 'MCHashEntry'})
        out = '\n'.join(statements.translate_body(m, c, r))
        self.assertIn('MCHashEntry* var3 =', out)
        self.assertIn('var3 != NULL', out)
        self.assertIn('var3->nextEntry', out)
        self.assertIn('var3->hashEntry', out)


class ArrayCastUnboxTests(unittest.TestCase):
    def _emit(self, lines):
        c = ir.JClass('R')
        c.fields.append(ir.Field('m', ir.JType('HashMap')))
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {')] + [(i + 2, l) for i, l in enumerate(lines)] + [(99, '}')]
        c.methods.append(m)
        return '\n'.join(statements.translate_body(m, c, types.TypeResolver()))

    def test_int_array_cast_of_map_value_unboxes_to_jarray(self):
        # (int[])map.get(k) -> (JArray<int >)(map value) -- (int[]) is not valid C++
        out = self._emit(['\tint[] var1 = (int[])this.m.get(k);'])
        self.assertIn('(JArray<int >)(this->m.get(k))', out)
        self.assertNotIn('(int[])', out)


class ThisIdentEqTests(unittest.TestCase):
    def _scope(self):
        return {'var1': ir.JType('Object'), 'other': ir.JType('Entity')}

    def test_this_eq_object_becomes_identeq(self):
        self.assertEqual(statements._rewrite_this_ident_eq('if(this == var1) {', self._scope()),
                         'if(j2xIdentEq(this, var1)) {')

    def test_object_eq_this_becomes_identeq(self):
        self.assertEqual(statements._rewrite_this_ident_eq('return var1 == this;', self._scope()),
                         'return j2xIdentEq(this, var1);')

    def test_this_neq_object(self):
        self.assertEqual(statements._rewrite_this_ident_eq('if(this != var1) {', self._scope()),
                         'if(!j2xIdentEq(this, var1)) {')

    def test_this_eq_typed_pointer_unchanged(self):
        # comparing this to a typed pointer is a normal pointer compare -- must NOT be rewritten
        self.assertEqual(statements._rewrite_this_ident_eq('if(this == other) {', self._scope()),
                         'if(this == other) {')


class WarningFixTests(unittest.TestCase):
    def test_shift_paren_arith_operand(self):
        self.assertEqual(statements._paren_shift_operands('a = x - 3 >> 4;'), 'a = (x - 3) >> 4;')
        self.assertEqual(statements._paren_shift_operands('a = b * 121 >> 15 & 15;'),
                         'a = (b * 121) >> 15 & 15;')

    def test_shift_no_arith_unchanged(self):
        self.assertEqual(statements._paren_shift_operands('a = x >> b;'), 'a = x >> b;')
        self.assertEqual(statements._paren_shift_operands('a = h >> 16 ^ h;'), 'a = h >> 16 ^ h;')

    def test_shift_paren_idempotent(self):
        self.assertEqual(statements._paren_shift_operands('a = (x - 3) >> 4;'), 'a = (x - 3) >> 4;')

    def test_shift_not_assign_or_ushr(self):
        self.assertEqual(statements._paren_shift_operands('a >>= 2;'), 'a >>= 2;')


class ContainerElementArrowTests(unittest.TestCase):
    """FIXED BUG list-elem-dot-arrow: indexing a JList<T*> yields a pointer element, so the
    following call must be `->`, not `.` (this->zombies[i]->tick())."""

    def test_field_container_element_uses_arrow(self):
        c = ir.JClass('World')
        c.fields.append(ir.Field('zombies', ir.JType('List', args=[ir.JType('Zombie')])))
        m = ir.Method('tick', ir.JType('void'), params=[], line=1)
        m.body = [
            (1, 'void tick() {'),
            (2, '\tfor (int i = 0; i < this.zombies.size(); i++) {'),
            (3, '\t\tthis.zombies.get(i).tick();'),
            (4, '\t}'),
            (5, '}'),
        ]
        c.methods.append(m)
        members = {'World': {'fields': {'zombies': ir.JType('List', args=[ir.JType('Zombie')])},
                             'methods': {}, 'parent': None},
                   'Zombie': {'fields': {}, 'methods': {'tick': ir.JType('void')}, 'parent': None}}
        r = types.TypeResolver(known_classes={'World', 'Zombie'}, members=members)
        out = statements.translate_body(m, c, r)
        self.assertIn('this->zombies[i]->tick();', out)
        self.assertNotIn('this->zombies[i].tick();', out)

    def test_scope_var_container_element_uses_arrow(self):
        c = ir.JClass('World')
        m = ir.Method('move', ir.JType('void'),
                      params=[ir.Param('aABBs', ir.JType('List', args=[ir.JType('AABB')]))], line=1)
        m.body = [
            (1, 'void move(List aABBs) {'),
            (2, '\taABBs.get(0).clipYCollide();'),
            (3, '}'),
        ]
        c.methods.append(m)
        r = types.TypeResolver(known_classes={'World', 'AABB'})
        out = statements.translate_body(m, c, r)
        self.assertIn('aABBs[0]->clipYCollide();', out)
