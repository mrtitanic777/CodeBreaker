#include "stdafx.h"
#include "EntityList.h"
#include "EntityPig.h"

// ==== CodeBreaker-generated reflection factory (replaces newInstance/getConstructor) ====
// Built from the class registration table; returns a real `new` instead of the NULL stub.
Entity* EntityList_createByReflection(Class* c, World* a0)
{
	if (!c) return NULL;
	if (*c == Class(typeid(EntityPig))) return new EntityPig(a0);
	if (*c == Class(typeid(EntityCow))) return new EntityCow(a0);
	if (*c == Class(typeid(EntitySheep))) return new EntitySheep(a0);
	return NULL;
}


void EntityList::registerAll() {
	addMapping(j2xClassOf(typeid(EntityPig)), "Pig", 90);
	addMapping(j2xClassOf(typeid(EntityCow)), "Cow", 92);
	addIdClassMapping(Class(typeid(EntitySheep)), 91);
	addMapping(j2xClassOf(typeid(EntityPig)), "PigAgain", 93);
	addMapping(j2xClassOf(typeid(World)), "NotAnEntity", 1);
	registerHelper(j2xClassOf(typeid(Ignored)));
}

Entity* EntityList::createEntityByName(JavaString var0, World* var1) {
	Entity* var2 = NULL;
	var2 = EntityList_createByReflection(this->stringToClassMapping[var0], (World*)var1);
	return var2;
}
