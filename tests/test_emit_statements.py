# -*- coding: utf-8 -*-
"""Module 4c pass 1 tests: per-method symbol table + value/pointer access."""

import unittest

from j2x import ir, types
from j2x.emit import statements
from j2x.ledger import Ledger


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
        self.assertIn('void* var2 = this->items[0];', self.out)

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
        self.assertIn('std::vector<void*> var2;', self.out)        # initializer dropped
        self.assertFalse(any('var2 =' in l and 'new' in l for l in self.out), self.out)

    def test_value_container_method_uses_dot(self):
        self.assertIn('var2.push_back(var1);', self.out)           # '.' not '->'

    def test_field_assign_is_value_temporary(self):
        self.assertIn('this->tagList = std::vector<void*>();', self.out)


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

    def test_map_get(self):
        self.assertIn('void* var3 = this->map[var1];', self.out)

    def test_map_containskey_and_remove(self):
        self.assertIn('bool b = this->map.count(var1);', self.out)
        self.assertIn('this->map.erase(var1);', self.out)

    def test_set_add_and_contains(self):
        self.assertIn('this->set.insert(var1);', self.out)
        self.assertIn('bool c = this->set.count(var1);', self.out)

    def test_string_equals(self):
        self.assertIn('bool d = (var1 == "foo");', self.out)

    def test_string_substring(self):
        self.assertIn('JavaString var4 = var1.substr(2, (5) - (2));', self.out)


class TestStringConcat(unittest.TestCase):
    def test_concat_return_to_ostringstream(self):
        c = ir.JClass('R')
        c.fields.append(ir.Field('x', ir.JType('float')))
        c.fields.append(ir.Field('y', ir.JType('float')))
        m = ir.Method('toString', ir.JType('String'), line=1)
        m.body = [(1, 'String toString() {'),
                  (2, '\treturn "(" + this.x + ", " + this.y + ")";'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertTrue(any('std::ostringstream _oss' in l and '_oss << "(" << this->x'
                            in l and 'return _oss.str();' in l for l in out), out)

    def test_no_concat_leaves_plain_return(self):
        c = ir.JClass('R')
        m = ir.Method('f', ir.JType('int'), line=1)
        m.body = [(1, 'int f() {'), (2, '\treturn a + b;'), (3, '}')]   # numeric add, no string
        c.methods.append(m)
        out = statements.translate_body(m, c, types.TypeResolver())
        self.assertIn('return a + b;', out)


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
        self.assertIn('unsigned char var2 = (unsigned char)var1;', self.out)

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
        self.assertIn('void* var2 = var1->players[0];', out)
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
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), params=[ir.Param('var6', ir.JType('ItemStack'))], line=1)
        m.body = [(1, 'void f(ItemStack var6) {'),
                  (2, '\tvar6.getItem().onPlaced();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('var6->getItem()->onPlaced();', out)

    def test_static_method_result_chain(self):
        c = ir.JClass('C')
        m = ir.Method('f', ir.JType('void'), line=1)
        m.body = [(1, 'void f() {'),
                  (2, '\tItemStack var1 = CraftingManager.getInstance().findRecipe();'), (3, '}')]
        c.methods.append(m)
        out = statements.translate_body(m, c, self._resolver())
        self.assertIn('CraftingManager::getInstance()->findRecipe();', out[0])

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
