# -*- coding: utf-8 -*-
"""
test_beta_fixes.py -- regression tests locking in the Beta 1.6.6 hardening pass:
typed enums, StringBuilder, signed-byte flip, collection-copy, leak auto-transform (+ thread
safety), synthetic-marker drop, and the new hang/leak detector rules. Stdlib unittest only.
"""

import unittest

from j2ctool import parse, pipeline, rules
from j2ctool.emit import header, source, statements
from j2ctool import ir
from j2ctool.ledger import Ledger


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

    def test_source_builds_table_and_preserves_method_body(self):
        cpp = source.render_source(self.cls, self.r)
        self.assertIn('WOOD = new EnumToolMaterial(0, 59, 2.0F, 0);', cpp)
        self.assertIn('WOOD->__ordinal = 0;', cpp)
        self.assertIn('return this->maxUses;', cpp)                 # getter body preserved (was dropped)

    def test_constant_only_enum_stays_int(self):
        _cls, r = _resolve('package sample;\npublic enum E { a, b, c; }\n')
        self.assertNotIn('E', r.typed_enums)
        self.assertEqual(r.field_type(ir.JType('E')), 'int')


class TestSignedByte(unittest.TestCase):
    def test_scalar_signed_buffer_unsigned(self):
        _c, r = _resolve('package sample;\npublic class P {}\n')
        self.assertEqual(r.field_type(ir.JType('byte')), 'signed char')
        self.assertEqual(r.field_type(ir.JType('byte', array_dims=1)), 'JArray<unsigned char >')


class TestLeakTransform(unittest.TestCase):
    def _translate(self, body_line, extra_src=()):
        srcs = ['package sample;\npublic class Host {\n public void go() {\n  %s\n }\n}\n' % body_line]
        srcs += list(extra_src)
        classes, r = _resolve(*srcs)
        host = next(c for c in classes if c.name == 'Host')
        m = host.method('go')
        return '\n'.join(statements.translate_body(m, host, r))

    def test_throwaway_new_is_stackified(self):
        out = self._translate('(new WorldGenLakes(1)).generate(this);',
                              ('package sample;\npublic class WorldGenLakes '
                               '{ public WorldGenLakes(int a){} public void generate(Host h){} }\n',))
        self.assertIn('WorldGenLakes _j2xleak0(1);', out)
        self.assertIn('_j2xleak0.generate(', out)
        self.assertNotIn('new WorldGenLakes', out)

    def test_thread_start_is_NOT_stackified(self):
        out = self._translate('(new Worker(this)).start();',
                              ('package sample;\npublic class Worker extends Thread '
                               '{ public Worker(Host h){} }\n',))
        self.assertIn('new Worker', out)              # must stay heap: async object outlives the call
        self.assertNotIn('_j2xleak', out)


class TestCollectionCopy(unittest.TestCase):
    def test_arraylist_copy_preserves_source(self):
        src = ('package sample;\npublic class C { public void m() {'
               ' java.util.List x = new java.util.ArrayList(this.items); } }\n')
        classes, r = _resolve(src)
        c = classes[0]
        out = '\n'.join(statements.translate_body(c.method('m'), c, r))
        self.assertIn('(this->items)', out)           # source collection preserved (not dropped -> empty)


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
        from j2ctool.emit import ownership
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
        from j2ctool.emit import source
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
        from j2ctool.emit import source
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

    def test_guard_emitted_for_transient_use(self):
        out, _ = self._entity('    ArrayList<AABB> aABBs = this.level.getCubes(this.bb);\n'
                              '    for (int i = 0; i < aABBs.size(); i++) { aABBs.get(i).clipYCollide(this.bb, 0.0F); }')
        self.assertIn('J2X_ScopedFreeElems<JList<AABB*> > _j2xfree_aABBs(aABBs);', out)

    def test_guard_skipped_when_element_retained(self):
        # element stored into a field -> it outlives the guard; freeing would dangle, so DO NOT guard.
        out, _ = self._entity('    ArrayList<AABB> aABBs = this.level.getCubes(this.bb);\n'
                              '    this.sel = aABBs.get(0);')
        self.assertNotIn('J2X_ScopedFreeElems', out)

    def test_borrowed_return_not_owned(self):
        from j2ctool.emit import ownership
        # a getter that returns a FIELD (a borrowed view) must never be treated as owned-return.
        src = ('package s;\nimport java.util.ArrayList;\npublic class Pool {\n  ArrayList<AABB> all;\n'
               '  public ArrayList<AABB> getAll() { return this.all; }\n}\n')
        classes, r = _resolve(src)
        self.assertNotIn('getAll', ownership.owned_return_container_methods(classes, r))


class TestOwnedScalarArgCleanup(unittest.TestCase):
    """Pattern B: an owned-scalar builder (`AABB.expand -> new AABB`) used inline as an argument leaks.
    Hoist + RAII-delete it, but ONLY when the receiving method provably just reads that argument."""

    AABB = ('package s;\npublic class AABB {\n  float x0;\n'
            '  public AABB expand(float a) { return new AABB(); }\n}\n')

    def _render(self, sink_cls, move_body):
        from j2ctool.emit import source
        ent = ('package s;\npublic class Entity {\n  Sink sink; AABB bb;\n'
               '  public void move() {\n%s\n  }\n}\n' % move_body)
        classes, _ = pipeline.drop_synthetic_artifacts(
            [parse.parse_source(self.AABB), parse.parse_source(sink_cls), parse.parse_source(ent)])
        pipeline.resolve_inheritance(classes)
        pipeline.compute_mi_forwards(classes)
        r = pipeline.build_resolver(classes)
        e = [c for c in classes if c.name == 'Entity'][0]
        return source.render_source(e, r, Ledger())

    READER = ('package s;\npublic class Sink {\n'
              '  public int take(AABB a) { return (int)a.x0; }\n}\n')          # only READS the arg
    RETAINER = ('package s;\npublic class Sink {\n  AABB kept;\n'
                '  public int take(AABB a) { this.kept = a; return 0; }\n}\n')  # STORES the arg

    def test_hoist_when_receiver_only_reads(self):
        out = self._render(self.READER, '    this.sink.take(this.bb.expand(1.0F));')
        self.assertIn('J2X_ScopedDelete<AABB >', out)
        self.assertIn('= this->bb->expand(1.0F);', out)

    def test_no_hoist_when_receiver_retains_arg(self):
        out = self._render(self.RETAINER, '    this.sink.take(this.bb.expand(1.0F));')
        self.assertNotIn('J2X_ScopedDelete', out)          # store -> would dangle -> leave it

    def test_no_hoist_when_result_captured(self):
        # `AABB a = bb.expand(...)` captures the result in a local; it's not an uncaptured temp arg.
        out = self._render(self.READER, '    AABB a = this.bb.expand(1.0F);\n    this.sink.take(a);')
        self.assertNotIn('J2X_ScopedDelete', out)


class TestOwnedReassignCleanup(unittest.TestCase):
    """Reassigning an OWNED scalar pointer member leaks the previous object on the 360 (the Entity.bb /
    setPos leak: Zombie.tick -> resetPos -> setPos allocates a new AABB every reset). The emitter must
    delete-before-reassign -- but only for owned members, and idempotently."""

    def _render(self, body):
        from j2ctool.emit import source
        src = ('package s;\npublic class Entity {\n  Level level; AABB bb;\n'
               '  public Entity(Level level) { this.level = level; }\n%s\n}\n' % body)
        classes, _ = pipeline.drop_synthetic_artifacts([parse.parse_source(src)])
        pipeline.resolve_inheritance(classes)
        pipeline.compute_mi_forwards(classes)
        r = pipeline.build_resolver(classes)
        return source.render_source(classes[0], r, Ledger())

    def test_owned_reassign_gets_delete(self):
        out = self._render('  public void setPos(float x) { this.bb = new AABB(x); }')
        self.assertIn('delete this->bb;', out)
        self.assertIn('this->bb = new AABB(x);', out)

    def test_borrowed_member_not_deleted(self):
        # `level` is assigned from a ctor param (borrowed) -> never delete-before-reassign it.
        out = self._render('  public void rebind(Level l) { this.level = l; }')
        self.assertNotIn('delete this->level;', out)

    def test_idempotent_when_delete_already_present(self):
        # a hand-written / template delete-before-reassign must NOT get a second delete.
        out = self._render('  public void setPos(float x) { this.bb = new AABB(x); }')
        self.assertEqual(out.count('delete this->bb;'), 1)   # setPos only (dtor uses `delete bb;`)


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
        order = pipeline.compute_static_init_order(classes, {'Material', 'Block'})
        self.assertLess(order.index('Material'), order.index('Block'))


class TestClassLiteralAndReflection(unittest.TestCase):
    def _go(self, body, extra=()):
        src = ['package sample;\npublic class R {\n%s\n}\n' % body]
        src += list(extra)
        classes, r = _resolve(*src)
        c = next(x for x in classes if x.name == 'R')
        out = {}
        for m in c.methods:
            if not m.is_ctor:
                out[m.name] = '\n'.join(statements.translate_body(m, c, r))
        return out, classes, r

    def test_class_literal_is_stable_not_dangling(self):
        out, _c, _r = self._go('public Class m() { return World.class; }',
                              ('package sample;\npublic class World {}\n',))
        self.assertIn('j2xClassOf(typeid(World))', out['m'])   # stable Class*, not Class() temp / s_class

    def test_class_literal_in_static_init_not_undefined_static(self):
        # `.class` in a static-initializer expr must NOT become the undefined `World::s_class`
        from j2ctool.emit import source
        _cls, r = _resolve('package sample;\npublic class World {}\n')
        expr = source._static_ref('World.class', r, (), None)
        self.assertIn('j2xClassOf(typeid(World))', expr)
        self.assertNotIn('s_class', expr)

    def test_static_init_reference_cast_gets_pointer(self):
        # a reference-type cast in a static initializer must become a POINTER cast (the statement
        # translator does this in method bodies; static inits bypass it and need it applied here)
        from j2ctool.emit import source
        _cls, r = _resolve(
            'package sample;\npublic class BlockGrass extends Block {}\n',
            'package sample;\npublic class Block {}\n')
        expr = source._static_ref('(BlockGrass)(new BlockGrass(2))', r, (), None)
        self.assertIn('(BlockGrass*)', expr)
        self.assertNotIn('(BlockGrass)(', expr)

    def test_static_init_value_cast_unchanged(self):
        # a value-type cast (String -> "") must NOT gain a star
        from j2ctool.emit import source
        _cls, r = _resolve('package sample;\npublic class Block {}\n')
        expr = source._static_ref('(String)null', r, (), None)
        self.assertNotIn('*', expr)

    def test_static_init_ternary_cast_distributed(self):
        from j2ctool.emit import source
        _cls, r = _resolve(
            'package sample;\npublic class A extends Base {}\n',
            'package sample;\npublic class B extends Base {}\n',
            'package sample;\npublic class Base {}\n')
        expr = source._static_ref('(Base)(c ? new A() : new B())', r, (), None)
        self.assertIn('(Base*)(new A())', expr)
        self.assertIn('(Base*)(new B())', expr)

    def test_reflection_flagged(self):
        out, _c, _r = self._go('public Object m() { return this.factory.newInstance(); }')
        self.assertIn('J2X-REVIEW[reflection-unsupported]', out['m'])
        self.assertIn('newInstance', out['m'])                 # call kept (flagged, not deleted)


class TestStaticHolderInitializers(unittest.TestCase):
    """Static-initializer expressions skip the statement translator, so `_static_ref` must lower
    every static-holder access to `::` itself: runtime-support holders (Logger/NumberFormat/Locale/
    Charset), java.lang.Math functions, and enum `values()`. Regression for the 'X.member ->
    undeclared identifier / illegal use of type/namespace' crash class in StatBase/SaveHandler/
    NoiseGenerator2/EnumOptionsMappingHelper."""

    def _r(self):
        _cls, r = _resolve('package sample;\npublic class Dummy {}\n')
        return r

    def test_runtime_holder_static_method_and_const(self):
        expr = source._static_ref('NumberFormat.getIntegerInstance(Locale.US)', self._r(), (), None)
        self.assertEqual(expr, 'NumberFormat::getIntegerInstance(Locale::US)')

    def test_runtime_holder_getlogger(self):
        expr = source._static_ref('Logger.getLogger("Minecraft")', self._r(), (), None)
        self.assertEqual(expr, 'Logger::getLogger("Minecraft")')

    def test_math_in_static_init(self):
        expr = source._static_ref('0.5 * (Math.sqrt(3.0) - 1.0)', self._r(), (), None)
        self.assertIn('sqrtf(3.0)', expr)
        self.assertNotIn('Math.', expr)

    def test_enum_values_in_array_size(self):
        # `new int[EnumOptions.values().length]` static field: a CONST-ONLY enum has no values()
        # method (it's a struct of `static const int`), so its `.values().length` lowers to the emitted
        # `EnumOptions::_J2X_COUNT` constant -- never a `.values()` (which would not compile).
        srcs = ('package sample;\npublic enum EnumOptions { A, B; }\n',
                'package sample;\npublic class Helper {\n'
                '  private static final int[] map = new int[EnumOptions.values().length];\n'
                '}\n')
        classes, r = _resolve(*srcs)
        helper = [c for c in classes if c.name == 'Helper'][0]
        defs, inits = source._render_statics(helper, r, Ledger())
        blob = '\n'.join(defs + inits)
        self.assertIn('EnumOptions::_J2X_COUNT', blob)
        self.assertNotIn('.values()', blob)


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


class TestSideEffectOrdering(unittest.TestCase):
    """GUARANTEE against the RenderPig bug class: the statement translator MUST preserve statement
    order and control-flow structure -- it must NEVER collapse an if/else into a predicate, hoist a
    side-effecting call out of a guard, or reorder short-circuit operands. Each test pins one shape
    that the old monolithic converter got wrong; they fail loudly if any such 'simplification' returns."""

    def _go(self, body):
        src = 'package sample;\npublic class R {\n%s\n}\n' % body
        classes, r = _resolve(src)
        c = classes[0]
        out = {}
        for m in c.methods:
            if not m.is_ctor:
                out[m.name] = '\n'.join(statements.translate_body(m, c, r))
        return out

    def test_side_effect_before_shortcircuit_return_preserved(self):
        # the exact RenderPig shape: loadTexture() must run, IN ORDER, before the return; the
        # return must stay a short-circuit && (NOT collapsed, NOT reordered, NOT dropped).
        out = self._go('protected boolean a(EntityPig var1, int var2) {\n'
                       '  this.loadTexture("/mob/saddle.png");\n'
                       '  return var2 == 0 && var1.rideable;\n}')['a']
        lines = [l.strip() for l in out.splitlines() if l.strip()]
        self.assertEqual(lines[0], 'this->loadTexture("/mob/saddle.png");')   # unconditional, first
        self.assertTrue(lines[1].startswith('return '))
        self.assertIn('&&', lines[1])                                          # short-circuit intact
        self.assertEqual(out.count('loadTexture'), 1)                          # not duplicated/hoisted

    def test_if_else_not_collapsed_side_effect_stays_guarded(self):
        out = self._go('protected boolean b(int var1) {\n'
                       '  if(var1 == 0) { this.loadTexture("/x.png"); return true; }\n'
                       '  else { return false; }\n}')['b']
        self.assertIn('if(', out.replace(' ', ''))          # the if survives (not collapsed to `return var1==0`)
        self.assertIn('loadTexture', out)                   # side effect not dropped
        # loadTexture must be INSIDE the guard: an `if` must appear before it in the text
        self.assertLess(out.index('if'), out.index('loadTexture'))

    def test_side_effecting_shortcircuit_rhs_not_reordered(self):
        out = self._go('protected boolean d(int var1) {\n'
                       '  return var1 == 0 && this->loadAndCheck();\n}')['d']
        self.assertIn('var1 == 0 && ', out)                 # RHS call stays AFTER the guard (short-circuit)

    def test_getter_hoist_not_moved_across_preceding_call(self):
        # a nullable getter deref preceded by another call must NOT be hoisted above that call
        # (would reorder two evaluations). It stays in place rather than being lifted to the head.
        out = self._go('protected void e(int var1) {\n'
                       '  this.render();\n'
                       '  int var2 = this.getItem(var1).itemID;\n}')['e']
        lines = [l.strip() for l in out.splitlines() if l.strip()]
        self.assertEqual(lines[0], 'this->render();')       # the preceding call stays first, untouched


class TestGL11Stub(unittest.TestCase):
    """The emitted GL11 (and NV) stubs must carry every GL symbol the render corpus references,
    so RenderEngine/Tessellator/EntityRenderer/etc. compile against them (gl11-stub error class)."""

    def setUp(self):
        from j2ctool import runtime
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
        from j2ctool import typemap
        self.assertIn('NVFogDistance', typemap.RUNTIME_STATIC_HOLDERS)
        self.assertIn('NVFogDistance', typemap.RUNTIME_HEADERS)
        self.assertIn('NVFogDistance', typemap.RUNTIME_ENUM_LIKE)


if __name__ == '__main__':
    unittest.main()
