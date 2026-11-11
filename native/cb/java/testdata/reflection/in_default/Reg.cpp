#include "a.h"
void f() { registerEntity(j2xClassOf(typeid(Zombie)), 1); registerEntity(j2xClassOf(typeid(Skeleton)), 2); addMapping(j2xClassOf(typeid(Hidden)), 3); }
Packet* g(int id) { return (Packet*)classes[id]->newInstance(); }
