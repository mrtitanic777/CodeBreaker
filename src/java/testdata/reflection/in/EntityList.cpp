#include "stdafx.h"
#include "EntityList.h"
#include "EntityPig.h"

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
	// J2X-REVIEW[reflection-unsupported]
	var2 = (Entity*)this->stringToClassMapping[var0]->getConstructor((JArrayInit<Class* >(), j2xClassOf(typeid(World)))).newInstance((JArrayInit<JObject >(), var1));
	return var2;
}
