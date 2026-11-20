#include "stdafx.h"
#include "World.h"
#include "ReflectionFactory.h"

void World::spawn(World* var1) {
	EntityLiving* var4 = (EntityLiving*)EntityList_createByReflection(var3, (World*)var1);
	TileEntity* var5 = (TileEntity*)TileEntity_createByReflection(this->tiles[var6]);
	Packet* var7 = (Packet*)TileEntity_createByReflection(var8);
	Other* var9 = (Other*)var3->getConstructor((JArrayInit<Class* >(), j2xClassOf(typeid(Chunk)))).newInstance((JArrayInit<JObject >(), var1));
}
