#ifndef ENTITYLIST_H
#define ENTITYLIST_H

// Auto-generated from EntityList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>
#include <string>
#include <typeinfo>

class Class;
class Entity;
class NBTTagCompound;
class World;

class EntityList {
public:
	virtual ~EntityList();

	// Key types restored to match the Java generics the transpiler flattened to
	// <String,void*>: Map<String,Class>, Map<Class,String>, Map<Integer,Class>,
	// Map<Class,Integer>. Class keys use the Class* pointer identity.
	static std::map<std::string, void*> s_stringToClassMapping;
	static std::map<void*, void*> s_classToStringMapping;
	static std::map<int, void*> s_IDtoClassMapping;
	static std::map<void*, void*> s_classToIDMapping;

	static void initStatics();   // run the entity registrations in order (called by j2x_initAll)
	static void addMapping(Class* var0, const JavaString& var1, int var2);
	static Entity* createEntityInWorld(const JavaString& var0, World* var1);
	static Entity* createEntityFromNBT(NBTTagCompound* var0, World* var1);
	static Entity* createEntity(int var0, World* var1);
	static int getEntityID(Entity* var0);
	static JavaString getEntityString(Entity* var0);

	// ---- Functional C++ RTTI registry (replaces the dead Class*-keyed maps) ----
	// Factory for a (World*) ctor; returns the constructed object as void*.
	typedef void* (*EntityFactory)(void* world);
	// name  -> factory   (was: stringToClassMapping)
	static std::map<std::string, EntityFactory> s_nameToFactory;
	// id    -> factory   (was: IDtoClassMapping)
	static std::map<int, EntityFactory> s_idToFactory;
	// typeid(*entity).name() -> registered name (was: classToStringMapping)
	static std::map<std::string, std::string> s_typeToName;
	// typeid(*entity).name() -> registered id   (was: classToIDMapping)
	static std::map<std::string, int> s_typeToID;

	// Templated registration: keys class identity by typeid(T).name() and stores a
	// (World*) factory for T. Replaces the reflection-based addMapping for real types.
	template<class T>
	static void registerEntity(const std::string& name, int id);
};

#endif // ENTITYLIST_H