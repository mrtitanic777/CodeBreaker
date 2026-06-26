#ifndef ENTITYLIST_H
#define ENTITYLIST_H

// Auto-generated from EntityList.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class Class;
class Entity;
class Map;
class NBTTagCompound;
class World;

class EntityList {
public:
	EntityList();
	virtual ~EntityList();
	static void initStatics();  // run once at startup (was Java static {})

	static Entity* createEntityInWorld(const std::string& var0, World* var1);
	static Entity* createEntityFromNBT(NBTTagCompound* var0, World* var1);
	static Entity* createEntity(int var0, World* var1);
	static int getEntityID(Entity* var0);
	static const std::string& getEntityString(Entity* var0);

public: // was private
	static std::map<std::string, void*>* s_stringToClassMapping;
	static std::map<std::string, void*>* s_classToStringMapping;
	static std::map<std::string, void*>* s_IDtoClassMapping;
	static std::map<std::string, void*>* s_classToIDMapping;

	void addMapping(void* var0, const std::string& var1, int var2);

};

#endif // ENTITYLIST_H