# -*- coding: utf-8 -*-
"""Tests for reflection_factory: Java class-registry reflection -> generated C++ factory."""
import os
import tempfile
import shutil
import unittest

from j2x import reflection_factory as rf


WORLD_ARG_CPP = '''\
#include "EntityList.h"
#include "Entity.h"

Entity* EntityList::createEntityInWorld(World* var1, int var5)
{
\tEntity* var2 = NULL;
\tClass* var3 = (Class*)s_idToClassMapping[var5];
\t// J2X-REVIEW[reflection-unsupported] getConstructor/newInstance stubbed to NULL
\tvar2 = (Entity*)var3->getConstructor((JArrayInit<Class* >(), Class(typeid(World)))).newInstance((JArrayInit<JObject >(), var1));
\treturn var2;
}

void EntityList::initStatics()
{
\taddMapping(Class(typeid(EntityPig)), "Pig", 90);
\taddMapping(Class(typeid(EntityCow)), "Cow", 92);
\taddMapping(Class(typeid(EntitySheep)), "Sheep", 91);
}
'''

NOARG_CPP = '''\
#include "TileEntity.h"

TileEntity* TileEntity::createAndLoadEntity(NBTTagCompound* var0)
{
\tTileEntity* var1 = NULL;
\tClass* var2 = (Class*)s_nameToClassMap[var0->getString("id")];
\tvar1 = (TileEntity*)var2->newInstance();
\treturn var1;
}

void TileEntity::initStatics()
{
\taddMapping(Class(typeid(TileEntityFurnace)), "Furnace");
\taddMapping(Class(typeid(TileEntityChest)), "Chest");
}
'''

REUSE_CPP = '''\
#include "SpawnerAnimals.h"
#include "EntityLiving.h"

void SpawnerAnimals::spawn(World* var0, Class** var33, int var8)
{
\tEntityLiving* var34 = (EntityLiving*)var33[var8]->getConstructor((JArrayInit<Class* >(), Class(typeid(World)))).newInstance((JArrayInit<JObject >(), var0));
}
'''


class TestProcess(unittest.TestCase):
    def test_world_arg_factory(self):
        new, n, fac = rf.process(WORLD_ARG_CPP, 'EntityList')
        self.assertEqual(n, 1)
        self.assertEqual(fac['ret'], 'Entity')
        self.assertEqual(fac['args'], ['World'])
        self.assertEqual(set(fac['classes']), {'EntityPig', 'EntityCow', 'EntitySheep'})
        # World (the ctor arg) must NOT be a concrete class in the switch
        self.assertNotIn('World', fac['classes'])
        self.assertIn('Entity* EntityList_createByReflection(Class* c, World* a0)', new)
        self.assertIn('if (!c) return NULL;', new)
        self.assertIn('if (*c == Class(typeid(EntityPig))) return new EntityPig(a0);', new)
        # call site rewritten with the World cast
        self.assertIn('EntityList_createByReflection(var3, (World*)var1)', new)
        self.assertNotIn('->getConstructor', new)             # no live reflection call left
        # the resolved review comment is stripped
        self.assertNotIn('J2X-REVIEW[reflection-unsupported]', new)

    def test_noarg_factory(self):
        new, n, fac = rf.process(NOARG_CPP, 'TileEntity')
        self.assertEqual(n, 1)
        self.assertEqual(fac['args'], [])
        self.assertEqual(set(fac['classes']), {'TileEntityFurnace', 'TileEntityChest'})
        self.assertIn('TileEntity* TileEntity_createByReflection(Class* c)', new)
        self.assertIn('return new TileEntityFurnace();', new)
        self.assertIn('TileEntity_createByReflection(var2)', new)
        self.assertNotIn('->newInstance()', new)

    def test_dynamic_getclass_not_registered(self):
        # Class(typeid(*(this))) is a getClass() key lookup, not a registration -> excluded
        txt = NOARG_CPP + '\nJavaString n = s_map[Class(typeid(*(this)))];\n'
        _, _, fac = rf.process(txt, 'TileEntity')
        self.assertNotIn('this', fac['classes'])

    def test_j2xclassof_registration_format(self):
        # the live emitter spells X.class as j2xClassOf(typeid(X)) (not Class(typeid(X))) -- the
        # factory pass must match BOTH so registries are recognized (regression: it once matched only
        # Class(...), silently leaving every reflection site as an uncompilable void*->newInstance()).
        txt = WORLD_ARG_CPP.replace('Class(typeid(', 'j2xClassOf(typeid(')
        new, n, fac = rf.process(txt, 'EntityList')
        self.assertEqual(n, 1)
        self.assertEqual(set(fac['classes']), {'EntityPig', 'EntityCow', 'EntitySheep'})
        self.assertIn('EntityList_createByReflection(var3, (World*)var1)', new)
        self.assertIn('if (*c == Class(typeid(EntityPig))) return new EntityPig(a0);', new)  # factory cmp stays Class()

    def test_no_reflection_is_noop(self):
        txt = '#include "Foo.h"\nint Foo::bar() { return 1; }\n'
        new, n, fac = rf.process(txt, 'Foo')
        self.assertEqual(n, 0)
        self.assertIsNone(fac)


class TestRun(unittest.TestCase):
    def setUp(self):
        self.d = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.d, ignore_errors=True)

    def _w(self, name, txt):
        with open(os.path.join(self.d, name), 'w', encoding='utf-8') as f:
            f.write(txt)

    def test_run_generates_and_reuses(self):
        self._w('EntityList.cpp', WORLD_ARG_CPP)
        self._w('TileEntity.cpp', NOARG_CPP)
        self._w('SpawnerAnimals.cpp', REUSE_CPP)
        r = rf.run(self.d)
        self.assertEqual(len(r['registries']), 2)
        self.assertEqual(len(r['reused']), 1)
        # shared header exists and declares both factories
        hdr = open(os.path.join(self.d, 'ReflectionFactory.h'), encoding='utf-8').read()
        self.assertIn('Entity* EntityList_createByReflection(Class* c, World* a0);', hdr)
        self.assertIn('TileEntity* TileEntity_createByReflection(Class* c);', hdr)
        # reuse site rewritten + include injected
        sa = open(os.path.join(self.d, 'SpawnerAnimals.cpp'), encoding='utf-8').read()
        self.assertIn('#include "ReflectionFactory.h"', sa)
        self.assertIn('(EntityLiving*)EntityList_createByReflection(var33[var8], (World*)var0)', sa)

    def test_run_is_idempotent(self):
        self._w('EntityList.cpp', WORLD_ARG_CPP)
        self._w('SpawnerAnimals.cpp', REUSE_CPP)
        rf.run(self.d)
        r2 = rf.run(self.d)
        self.assertEqual(r2['total_sites'], 0)
        # factory defined exactly once
        el = open(os.path.join(self.d, 'EntityList.cpp'), encoding='utf-8').read()
        self.assertEqual(el.count('Entity* EntityList_createByReflection(Class* c, World* a0)'), 1)
        sa = open(os.path.join(self.d, 'SpawnerAnimals.cpp'), encoding='utf-8').read()
        self.assertEqual(sa.count('#include "ReflectionFactory.h"'), 1)


if __name__ == '__main__':
    unittest.main()
