# -*- coding: utf-8 -*-
"""
test_beta_fixes.py -- regression tests locking in the Beta 1.6.6 hardening pass:
typed enums, StringBuilder, signed-byte flip, collection-copy, leak auto-transform (+ thread
safety), synthetic-marker drop, and the new hang/leak detector rules. Stdlib unittest only.
"""

import unittest

from codebreaker.java import parse, transpile as pipeline, rules
from codebreaker.java.emit import header
from codebreaker.java import ir
from codebreaker.java.ledger import Ledger


def _resolve(*srcs):
    classes = [parse.parse_source(s) for s in srcs]
    classes, _markers = pipeline.drop_synthetic_artifacts(classes)
    pipeline.resolve_inheritance(classes)
    pipeline.compute_mi_forwards(classes)
    return classes, pipeline.build_resolver(classes)


class TestTypedEnum(unittest.TestCase):
    SRC = ('package sample;\n'
           'public enum EnumToolMaterial {\n'
           '  WOOD(0, 59, 2.0F, 0), IRON(2, 250, 6.0F, 2);\n'
           '  private final int harvestLevel;\n'
           '  private final int maxUses;\n'
           '  private final float eff;\n'
           '  private final int dmg;\n'
           '  private EnumToolMaterial(int var3, int var4, float var5, int var6) {\n'
           '    this.harvestLevel = var3; this.maxUses = var4; this.eff = var5; this.dmg = var6;\n'
           '  }\n'
           '  public int getMaxUses() { return this.maxUses; }\n'
           '}\n')

    def setUp(self):
        self.classes, self.r = _resolve(self.SRC)
        self.cls = self.classes[0]

    def test_detected_as_typed(self):
        self.assertIn('EnumToolMaterial', self.r.typed_enums)
        self.assertTrue(self.r.is_pointer(ir.JType('EnumToolMaterial')))   # object, not int

    def test_header_has_singletons_and_fields(self):
        h = header.render_header(self.cls, self.r)
        self.assertIn('class EnumToolMaterial', h)
        self.assertIn('static EnumToolMaterial* WOOD;', h)
        self.assertIn('static EnumToolMaterial* IRON;', h)
        self.assertIn('int maxUses;', h)
        self.assertIn('getMaxUses', h)

    def test_constant_only_enum_stays_int(self):
        _cls, r = _resolve('package sample;\npublic enum E { a, b, c; }\n')
        self.assertNotIn('E', r.typed_enums)
        self.assertEqual(r.field_type(ir.JType('E')), 'int')


class TestSignedByte(unittest.TestCase):
    def test_scalar_signed_buffer_unsigned(self):
        _c, r = _resolve('package sample;\npublic class P {}\n')
        self.assertEqual(r.field_type(ir.JType('byte')), 'signed char')
        self.assertEqual(r.field_type(ir.JType('byte', array_dims=1)), 'JArray<unsigned char >')


class TestSyntheticMarkerDrop(unittest.TestCase):
    def test_marker_and_bridge_ctor_dropped_but_base_kept(self):
        marker = 'package sample;\nclass Empty2 {}\n'
        user = ('package sample;\nclass Foo {\n'
                '  private Foo() {}\n  Foo(Empty2 var1) { this(); }\n}\n')
        base = 'package sample;\nclass NoiseGenerator {}\n'
        child = 'package sample;\nclass NoiseGeneratorOctaves extends NoiseGenerator {}\n'
        kept, markers = pipeline.drop_synthetic_artifacts(
            [parse.parse_source(marker), parse.parse_source(user),
             parse.parse_source(base), parse.parse_source(child)])
        names = set(c.name for c in kept)
        self.assertIn('Empty2', markers)              # returned marker set
        self.assertNotIn('Empty2', names)             # marker dropped
        self.assertIn('NoiseGenerator', names)        # empty-but-a-base -> kept
        foo = next(c for c in kept if c.name == 'Foo')
        self.assertEqual(len(foo.constructors()), 1)  # bridge ctor dropped, private ctor kept


class TestHangRules(unittest.TestCase):
    def test_hotpath_alloc_flags_new_in_loop(self):
        cpp = ('void World::generate() {\n'
               '  for (int i = 0; i < n; ++i) {\n'
               '    EntityItem* e = new EntityItem(1);\n'
               '  }\n}\n')
        found = list(rules.rule_hotpath_alloc('World.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertTrue(any(f[3] == 'hotpath-alloc' for f in found))

    def test_hotpath_alloc_skips_constructor_loop(self):
        # a `new` in a loop inside a CONSTRUCTOR builds a fixed structure ONCE per object -- not a
        # per-frame allocation storm, so it must not be flagged.
        cpp = ('World::World() {\n  for (int i = 0; i < n; ++i) {\n'
               '    this->chunks[i] = new Chunk(i);\n  }\n}\n')
        found = list(rules.rule_hotpath_alloc('World.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertFalse(any(f[3] == 'hotpath-alloc' for f in found))


class TestOwnershipBroadening(unittest.TestCase):
    """No-GC ownership: the destructor must free EVERY heap member -- factory- and init()-allocated,
    scalar and container -- but NEVER a borrowed (passed-in) pointer, which would double-free."""

    def _cls(self, src, name):
        from codebreaker.java.emit import ownership
        classes, r = _resolve(src)
        c = [c for c in classes if c.name == name][0]
        return c, r, ownership

    def test_factory_and_init_allocations_owned(self):
        # allocations in a NON-constructor (init) and via a create* factory are owned, not just ctor `new`.
        src = ('package s;\npublic class Game {\n'
               '  Timer timer; Level level; FloatBuffer buf;\n'
               '  public Game() { this.timer = new Timer(); this.buf = BufferUtils.createFloatBuffer(4); }\n'
               '  public void init() { this.level = new Level(); }\n'
               '}\n')
        c, r, own = self._cls(src, 'Game')
        owned = own.owned_pointers(c, r)
        self.assertEqual({'timer', 'level', 'buf'}, set(owned))

    def test_borrowed_param_pointer_not_owned(self):
        # `this.level = level;` (from a ctor param) is a BORROW -- deleting it double-frees the real owner.
        src = ('package s;\npublic class Renderer {\n'
               '  Level level; Tess t;\n'
               '  public Renderer(Level level) { this.t = new Tess(); this.level = level; }\n'
               '}\n')
        c, r, own = self._cls(src, 'Renderer')
        owned = own.owned_pointers(c, r)
        self.assertIn('t', owned)
        self.assertNotIn('level', owned)
        self.assertTrue(own.owns_heap(c, r))          # still owns t -> non-copyable

    def test_ambiguous_field_excluded(self):
        # a field allocated in one path and aliased to a param in another is ambiguous -> not deleted.
        src = ('package s;\npublic class C {\n  Node n;\n'
               '  public C() { this.n = new Node(); }\n'
               '  public void set(Node x) { this.n = x; }\n}\n')
        c, r, own = self._cls(src, 'C')
        self.assertNotIn('n', own.owned_pointers(c, r))

    def test_container_of_new_owned_listener_list_not(self):
        # a list every element of which is `new` is owned (loop-delete); a list of passed-in observers isn't.
        src = ('package s;\nimport java.util.List;\npublic class W {\n'
               '  List<Zombie> zombies; List<Listener> listeners;\n'
               '  public void spawn() { this.zombies.add(new Zombie()); }\n'
               '  public void addListener(Listener l) { this.listeners.add(l); }\n'
               '}\n')
        c, r, own = self._cls(src, 'W')
        cont = own.owned_containers(c, r)
        self.assertIn('zombies', cont)
        self.assertNotIn('listeners', cont)

    def test_dtor_frees_scalars_and_container_not_borrowed(self):
        src = ('package s;\nimport java.util.List;\npublic class Game {\n'
               '  Level level; Timer timer; List<Zombie> zombies;\n'
               '  public Game() { this.timer = new Timer(); }\n'
               '  public void init() { this.level = new Level(); this.zombies.add(new Zombie()); }\n'
               '}\n')
        c, r, own = self._cls(src, 'Game')
        from codebreaker.java.emit import source
        dtor = '\n'.join(source._render_dtor(c, r, Ledger()))
        self.assertIn('delete timer;', dtor)
        self.assertIn('delete level;', dtor)
        self.assertIn('delete zombies[_i];', dtor)


class TestOwnedReturnCleanup(unittest.TestCase):
    """A builder method returns a container of heap objects the CALLER must free (Level.getCubes ->
    new AABBs, iterated then discarded in Entity.move). The emitter drops an RAII element-deleter after
    the call -- but only when it's a fresh owned container AND the elements are used transiently."""

    LEVEL = ('package s;\nimport java.util.ArrayList;\npublic class Level {\n'
             '  public ArrayList<AABB> getCubes(AABB box) {\n'
             '    ArrayList<AABB> cubes = new ArrayList<AABB>();\n'
             '    cubes.add(new AABB(0,0,0,1,1,1));\n    return cubes;\n  }\n}\n')

    def _entity(self, body):
        from codebreaker.java.emit import source
        ent = ('import java.util.ArrayList;\npackage s;\npublic class Entity {\n  Level level; AABB bb; AABB sel;\n'
               '  public void move() {\n%s\n  }\n}\n' % body)
        classes, _ = pipeline.drop_synthetic_artifacts([parse.parse_source(self.LEVEL), parse.parse_source(ent)])
        pipeline.resolve_inheritance(classes)
        pipeline.compute_mi_forwards(classes)
        r = pipeline.build_resolver(classes)
        e = [c for c in classes if c.name == 'Entity'][0]
        return source.render_source(e, r, Ledger()), r

    def test_getcubes_recognized_as_owned_return(self):
        _out, r = self._entity('    ArrayList<AABB> a = this.level.getCubes(this.bb);')
        self.assertIn('getCubes', r.own_catalog.return_methods)

    def test_borrowed_return_not_owned(self):
        from codebreaker.java.emit import ownership
        # a getter that returns a FIELD (a borrowed view) must never be treated as owned-return.
        src = ('package s;\nimport java.util.ArrayList;\npublic class Pool {\n  ArrayList<AABB> all;\n'
               '  public ArrayList<AABB> getAll() { return this.all; }\n}\n')
        classes, r = _resolve(src)
        self.assertNotIn('getAll', ownership.owned_return_container_methods(classes, r))


class TestFieldInitOwnership(unittest.TestCase):
    """RetroMCP-style decompiles field-INITIALIZE members (`Timer timer = new Timer();`). The emitter
    moves these into the ctor, but they live on Field.initializer, not a method body -- so the ownership
    analysis (and thus the destructor) must scan initializers or it silently leaks them."""

    def test_field_initialized_new_is_owned(self):
        from codebreaker.java.emit import ownership
        src = ('package s;\npublic class G {\n'
               '  Timer timer = new Timer();\n'          # field-init new
               '  FloatBuffer buf = BufferUtils.createFloatBuffer(4);\n'   # field-init factory
               '  Level level;\n'                        # borrowed (set from a param below)
               '  public G(Level l) { this.level = l; }\n}\n')
        classes, r = _resolve(src)
        owned = ownership.owned_pointers([c for c in classes if c.name == 'G'][0], r)
        self.assertIn('timer', owned)
        self.assertIn('buf', owned)
        self.assertNotIn('level', owned)                 # borrowed -> not owned


class TestSilentDispatchCommentChain(unittest.TestCase):
    def test_interior_comment_does_not_truncate_chain(self):
        # a comment between two else-ifs must NOT hide the terminal J2X_UNHANDLED (the glEnableClientState FP).
        cpp = ('void f(int c) {\n'
               '  if (c == GL_A) x=1; else if (c == GL_B) x=2;\n'
               '  // an explanatory comment mid-chain\n'
               '  else if (c == GL_C) x=3;\n'
               '  else J2X_UNHANDLED("f", c);\n}\n')
        found = list(rules.rule_silent_dispatch('f.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertFalse([x for x in found if x[3] == 'silent-dispatch'])

    def test_still_flags_a_genuinely_silent_chain(self):
        cpp = ('void f(int c) {\n  if (c == GL_A) x=1; else if (c == GL_B) x=2;\n'
               '  else if (c == GL_C) x=3;\n}\n')
        found = list(rules.rule_silent_dispatch('f.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertTrue([x for x in found if x[3] == 'silent-dispatch'])


class TestVirtualCallInCtorRule(unittest.TestCase):
    HDR = 'class Foo {\npublic:\n\tFoo();\n\tvirtual ~Foo();\n\tvirtual void init();\n};\n'
    CPP = 'Foo::Foo() {\n\tthis->init();\n}\n'

    def test_flagged_when_hierarchy_unknown(self):
        # per-file / unit-test call: override_pairs is None -> stay conservative and flag.
        found = list(rules.rule_virtual_call_in_ctor('Foo.cpp', self.CPP, self.HDR, rules._PlatCtx('xbox360')))
        self.assertTrue(any(f[3] == 'virtual-call-in-ctor' for f in found))

    def test_not_flagged_when_no_subclass_overrides(self):
        plat = rules._PlatCtx('xbox360')
        plat.override_pairs = frozenset()          # whole tree known; nobody overrides Foo::init
        found = list(rules.rule_virtual_call_in_ctor('Foo.cpp', self.CPP, self.HDR, plat))
        self.assertFalse(found)

    def test_flagged_when_a_subclass_overrides(self):
        plat = rules._PlatCtx('xbox360')
        plat.override_pairs = frozenset({('Foo', 'init')})
        found = list(rules.rule_virtual_call_in_ctor('Foo.cpp', self.CPP, self.HDR, plat))
        self.assertTrue(any(f[3] == 'virtual-call-in-ctor' for f in found))

    def test_infinite_loop_flagged_without_break(self):
        cpp = 'void A::b() {\n  while (true) {\n    doStuff();\n  }\n}\n'
        found = list(rules.rule_infinite_loop('A.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertTrue(any(f[3] == 'infinite-loop' for f in found))

    def test_infinite_loop_not_flagged_with_break(self):
        cpp = 'void A::b() {\n  while (true) {\n    if (x) break;\n  }\n}\n'
        found = list(rules.rule_infinite_loop('A.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertFalse(found)


class TestDerivedNullability(unittest.TestCase):
    def test_return_null_getter_is_derived(self):
        src = ('package sample;\npublic class Inv {\n'
               '  public ItemStack getStackInSlot(int var1) {\n'
               '    if(var1 < 0) { return null; }\n    return this.contents;\n  }\n'
               '  public int size() { return 0; }\n}\n')
        classes, r = _resolve(src)
        getters, gtypes = pipeline.derive_nullability(classes, r)
        self.assertIn('getStackInSlot', getters)          # reference return + `return null` -> nullable
        self.assertEqual(gtypes['getStackInSlot'], 'ItemStack')
        self.assertNotIn('size', getters)                 # int return can't form getter()->deref

    def test_value_return_null_not_derived(self):
        src = ('package sample;\npublic class S {\n'
               '  public String name() { return null; }\n}\n')     # String is a value type here
        classes, r = _resolve(src)
        getters, _ = pipeline.derive_nullability(classes, r)
        self.assertNotIn('name', getters)


class TestStaticInitOrder(unittest.TestCase):
    def test_dependency_ordered_before_dependent(self):
        # B's static init reads A's statics -> A must come first, even though B<A alphabetically.
        a = 'package sample;\npublic class Zebra { public static int x = 1; }\n'
        b = ('package sample;\npublic class Apple {\n'
             '  public static int y = Zebra.x + 1;\n}\n')
        classes = [parse.parse_source(a), parse.parse_source(b)]
        pipeline.resolve_inheritance(classes)
        order = pipeline.compute_static_init_order(classes, {'Zebra', 'Apple'})
        self.assertLess(order.index('Zebra'), order.index('Apple'))   # dep before dependent

    def test_seed_priority_preserved(self):
        # Material before Block is a seeded (ctor-mediated) order even with no textual dep.
        m = 'package sample;\npublic class Material { public static int x = 1; }\n'
        b = 'package sample;\npublic class Block { public static int y = 2; }\n'
        classes = [parse.parse_source(b), parse.parse_source(m)]
        pipeline.resolve_inheritance(classes)
        from codebreaker.java import profiles
        prev = profiles.active().name
        profiles.use('minecraft_xbox360')                  # the seed is Minecraft's (its project's profile)
        try:
            order = pipeline.compute_static_init_order(classes, {'Material', 'Block'})
        finally:
            profiles.use(prev)
        self.assertLess(order.index('Material'), order.index('Block'))


class TestSideEffectInBranchRule(unittest.TestCase):
    def test_flags_impure_return_branch(self):
        cpp = ('bool R::a() {\n  if (cond) {\n    this->loadTexture("/x.png");\n'
               '    return true;\n  }\n  return false;\n}\n')
        found = list(rules.rule_side_effect_in_branch('R.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertTrue(any('side-effect-in-branch' == f[3] for f in found))

    def test_pure_branch_not_flagged(self):
        cpp = 'bool R::a() {\n  if (cond) {\n    return true;\n  }\n  return false;\n}\n'
        found = list(rules.rule_side_effect_in_branch('R.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertFalse(found)

    def test_return_value_call_not_flagged(self):
        # `return f(x);` is not a side-effecting STATEMENT -- the call is the returned value. (Textures
        # cache-hit `return map.get(k);`, Input `return std::string(s);` were false positives.)
        cpp = ('std::string R::a() {\n  if (cache.containsKey(k)) { return cache.get(k); }\n'
               '  return std::string();\n}\n')
        found = list(rules.rule_side_effect_in_branch('R.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertFalse(found)

    def test_nested_early_return_guard_not_flagged(self):
        # `if (flg & 4) { if (p > n) return -1; p += 2; }` -- reducing `return -1;` must not leave
        # `if (...) ;` that reads as an impure call (the Inflate gzip-header false positive).
        cpp = 'int R::a() {\n  if (flg & 4) { if (p > n) return -1; p += 2; }\n  return p;\n}\n'
        found = list(rules.rule_side_effect_in_branch('R.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertFalse(found)

    def test_real_side_effect_assignment_flagged(self):
        # a genuine guarded side effect (`this->x = new ...; return;`) still flags.
        cpp = ('void R::a() {\n  if (isHit) {\n    this->hit = new HitResult(1);\n    return;\n  }\n}\n')
        found = list(rules.rule_side_effect_in_branch('R.cpp', cpp, None, rules._PlatCtx('xbox360')))
        self.assertTrue(any(f[3] == 'side-effect-in-branch' for f in found))


class TestGL11Stub(unittest.TestCase):
    """The emitted GL11 (and NV) stubs must carry every GL symbol the render corpus references,
    so RenderEngine/Tessellator/EntityRenderer/etc. compile against them (gl11-stub error class)."""

    def setUp(self):
        from codebreaker.java import runtime
        self.gl11 = runtime.FILES['GL11.h']
        self.nvfog = runtime.FILES['NVFogDistance.h']

    def test_missing_gl11_constants_now_present(self):
        for name in ('GL_BYTE', 'GL_TRIANGLES', 'GL_NORMAL_ARRAY', 'GL_NEAREST_MIPMAP_LINEAR',
                     'GL_TEXTURE', 'GL_ZERO', 'GL_GEQUAL', 'GL_UNPACK_ALIGNMENT'):
            self.assertIn('const int %s' % name, self.gl11, '%s missing from GL11 stub' % name)

    def test_missing_gl11_functions_now_present(self):
        for fn in ('glNormalPointer', 'glScaled'):
            self.assertIn('void %s(' % fn, self.gl11, '%s missing from GL11 stub' % fn)

    def test_nvfogdistance_holder_emitted(self):
        # EntityRenderer references NVFogDistance.GL_FOG_DISTANCE_MODE_NV / GL_EYE_RADIAL_NV
        self.assertIn('namespace NVFogDistance', self.nvfog)
        self.assertIn('GL_FOG_DISTANCE_MODE_NV', self.nvfog)
        self.assertIn('GL_EYE_RADIAL_NV', self.nvfog)

    def test_nvfogdistance_is_static_holder(self):
        # so Name.MEMBER resolves to Name::MEMBER (enum-like) and gets its own #include
        from codebreaker.java import typemap
        self.assertIn('NVFogDistance', typemap.RUNTIME_STATIC_HOLDERS)
        self.assertIn('NVFogDistance', typemap.RUNTIME_HEADERS)
        self.assertIn('NVFogDistance', typemap.RUNTIME_ENUM_LIKE)


if __name__ == '__main__':
    unittest.main()
