// ReflectionFactory.h -- CodeBreaker-generated. Declares the reflection factories so any file can
// construct a registered class by its Class* (replacing Java newInstance()).
#ifndef J2X_REFLECTION_FACTORY_H
#define J2X_REFLECTION_FACTORY_H

class Class; class Entity; class TileEntity; class World;
Entity* EntityList_createByReflection(Class* c, World* a0);
TileEntity* TileEntity_createByReflection(Class* c);

#endif // J2X_REFLECTION_FACTORY_H
