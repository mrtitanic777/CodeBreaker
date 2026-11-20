#include "stdafx.h"
#include "World.h"

void World::spawn(World* var1) {
	EntityLiving* var4 = (EntityLiving*)var3->getConstructor((JArrayInit<Class* >(), j2xClassOf(typeid(World)))).newInstance((JArrayInit<JObject >(), var1));
	TileEntity* var5 = (TileEntity*)this->tiles[var6]->newInstance();
	Packet* var7 = (Packet*)var8->newInstance();
	Other* var9 = (Other*)var3->getConstructor((JArrayInit<Class* >(), j2xClassOf(typeid(Chunk)))).newInstance((JArrayInit<JObject >(), var1));
}
