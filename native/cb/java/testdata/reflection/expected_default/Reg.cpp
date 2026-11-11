#include "a.h"

// ==== CodeBreaker-generated reflection factory (replaces newInstance/getConstructor) ====
// Built from the class registration table; returns a real `new` instead of the NULL stub.
Packet* Reg_createByReflection(Class* c)
{
	if (!c) return NULL;
	if (*c == Class(typeid(Zombie))) return new Zombie();
	if (*c == Class(typeid(Skeleton))) return new Skeleton();
	if (*c == Class(typeid(Hidden))) return new Hidden();
	return NULL;
}

void f() { registerEntity(j2xClassOf(typeid(Zombie)), 1); registerEntity(j2xClassOf(typeid(Skeleton)), 2); addMapping(j2xClassOf(typeid(Hidden)), 3); }
Packet* g(int id) { return Reg_createByReflection(classes[id]); }
