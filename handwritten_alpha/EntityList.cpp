#include "stdafx.h"
// Hand-ported from EntityList.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// EntityList is the entity-name <-> entity-class <-> numeric-id registry. The
// Java original drives this entirely off reflection: a HashMap of
// String/Integer keys to java.lang.Class tokens, with new entities built via
//   clazz.getConstructor(World.class).newInstance(world)
// and reverse lookups done with entity.getClass().
//
// C++03 has no reflection, so the "Class token" is redesigned away (user
// approved port-or-design):
//   * Forward direction (name/id -> new Entity)  : a static factory table that
//     maps each registered name to a small creator function taking (World*).
//   * Reverse direction (live Entity -> name/id) : a most-derived-first
//     dynamic_cast ladder (RTTI is enabled in the project; this matches the
//     established idiom in RenderManager.cpp / TileEntity).
//
// The header's four std::map<std::string, void*>* members are kept EXACTLY as
// declared and are still populated for faithfulness/inspection: the canonical
// "class" key is the entity NAME string, and the numeric maps stringify the int
// id. The hot paths use the factory table / dynamic_cast ladder rather than the
// void* maps so no unsafe (int)void* round-trips are needed.

#include "EntityList.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "World.h"

// Concrete entity classes the registry can instantiate.
#include "Entity.h"
#include "EntityArrow.h"
#include "EntitySnowball.h"
#include "EntityItem.h"
#include "EntityPainting.h"
#include "EntityLiving.h"
#include "EntityMobs.h"
#include "EntityCreeper.h"
#include "EntitySkeleton.h"
#include "EntitySpider.h"
#include "EntityZombieSimple.h"
#include "EntityZombie.h"
#include "EntitySlime.h"
#include "EntityGhast.h"
#include "EntityPigZombie.h"
#include "EntityPig.h"
#include "EntitySheep.h"
#include "EntityCow.h"
#include "EntityChicken.h"
#include "EntityTNTPrimed.h"
#include "EntityFallingSand.h"
#include "EntityMinecart.h"
#include "EntityBoat.h"

#include <string>
#include <map>
#include <vector>
#include <sstream>

// Header-declared storage (kept verbatim; populated by initStatics()).
std::map<std::string, void*>* EntityList::s_stringToClassMapping;
std::map<std::string, void*>* EntityList::s_classToStringMapping;
std::map<std::string, void*>* EntityList::s_IDtoClassMapping;
std::map<std::string, void*>* EntityList::s_classToIDMapping;

// ----------------------------------------------------------------------------
// Factory table: replaces the Java reflective "Class token + newInstance".
// ----------------------------------------------------------------------------
namespace {
	typedef Entity* (*EntityCreatorFn)(World*);

	struct EntityReg {
		std::string     name;     // Java mapping name ("Arrow", "Zombie", ...)
		int             id;       // numeric id
		EntityCreatorFn create;   // builds `new EntitySub(world)`
	};

	// name -> registry entry, and id -> registry entry.
	std::map<std::string, EntityReg> g_byName;
	std::map<int, EntityReg>         g_byId;

	// Template creator: `return new T(world);` for any T(World*).
	template <class T>
	Entity* createT(World* world) {
		return new T(world);
	}

	std::string intToStr(int v) {
		std::ostringstream ss;
		ss << v;
		return ss.str();
	}

	// Most-derived-first resolution of a live entity to its registered name.
	// Mirrors Java's entity.getClass() reverse lookup. Order matters: subclasses
	// (PigZombie before Zombie, the concrete mobs before the EntityMobs/EntityLiving
	// fallbacks) must be tested first.
	const char* resolveEntityName(Entity* e) {
		if (e == NULL) {
			return "";
		}
		if (dynamic_cast<EntityArrow*>(e))        return "Arrow";
		if (dynamic_cast<EntitySnowball*>(e))     return "Snowball";
		if (dynamic_cast<EntityItem*>(e))         return "Item";
		if (dynamic_cast<EntityPainting*>(e))     return "Painting";
		if (dynamic_cast<EntityCreeper*>(e))      return "Creeper";
		if (dynamic_cast<EntitySkeleton*>(e))     return "Skeleton";
		if (dynamic_cast<EntitySpider*>(e))       return "Spider";
		if (dynamic_cast<EntityZombieSimple*>(e)) return "Giant";
		if (dynamic_cast<EntityPigZombie*>(e))    return "PigZombie"; // before Zombie
		if (dynamic_cast<EntityZombie*>(e))       return "Zombie";
		if (dynamic_cast<EntitySlime*>(e))        return "Slime";
		if (dynamic_cast<EntityGhast*>(e))        return "Ghast";
		if (dynamic_cast<EntityPig*>(e))          return "Pig";
		if (dynamic_cast<EntitySheep*>(e))        return "Sheep";
		if (dynamic_cast<EntityCow*>(e))          return "Cow";
		if (dynamic_cast<EntityChicken*>(e))      return "Chicken";
		if (dynamic_cast<EntityTNTPrimed*>(e))    return "PrimedTnt";
		if (dynamic_cast<EntityFallingSand*>(e))  return "FallingSand";
		if (dynamic_cast<EntityMinecart*>(e))     return "Minecart";
		if (dynamic_cast<EntityBoat*>(e))         return "Boat";
		if (dynamic_cast<EntityMobs*>(e))         return "Monster"; // EntityMobs base
		if (dynamic_cast<EntityLiving*>(e))       return "Mob";     // EntityLiving base
		return ""; // unregistered (e.g. players, FX) -> Java returned null
	}

	// Empty string kept alive for the const-ref return of getEntityString().
	std::string g_emptyName;
}

// ----------------------------------------------------------------------------
// Construction / destruction
// ----------------------------------------------------------------------------
EntityList::EntityList() {
}

EntityList::~EntityList() {
	// No dynamic resources owned per-instance.
}

namespace {
	// Shared registration used by both the member addMapping() and initStatics().
	// `var0` carries the EntityReg* describing the class/creator.
	void registerMapping(void* var0, const std::string& var1, int var2) {
		EntityReg* reg = (EntityReg*)var0;

		g_byName[var1] = *reg;
		g_byId[var2]   = *reg;

		// Populate the header maps for inspection/faithfulness. The canonical
		// class key is the entity name string; the id map stringifies the id.
		if (EntityList::s_stringToClassMapping != NULL) {
			(*EntityList::s_stringToClassMapping)[var1]       = var0;
			(*EntityList::s_classToStringMapping)[var1]       = (void*)(&g_byName[var1].name);
			(*EntityList::s_IDtoClassMapping)[intToStr(var2)] = var0;
			(*EntityList::s_classToIDMapping)[var1]           = var0;
		}
	}
}

// Java: addMapping(Class, String, int). Here the "class" is encoded as the name
// string (the factory table holds the real constructor). Non-static to match the
// header; delegates to the shared file-local registrar.
void EntityList::addMapping(void* var0, const std::string& var1, int var2) {
	registerMapping(var0, var1, var2);
}

// ----------------------------------------------------------------------------
// Forward construction (name/id -> new Entity)
// ----------------------------------------------------------------------------
Entity* EntityList::createEntityInWorld(const std::string& var0, World* var1) {
	Entity* var2 = NULL;

	std::map<std::string, EntityReg>::iterator it = g_byName.find(var0);
	if (it != g_byName.end() && it->second.create != NULL) {
		var2 = it->second.create(var1);
	}

	return var2;
}

Entity* EntityList::createEntityFromNBT(NBTTagCompound* var0, World* var1) {
	Entity* var2 = NULL;

	const std::string& id = var0->getString("id");
	std::map<std::string, EntityReg>::iterator it = g_byName.find(id);
	if (it != g_byName.end() && it->second.create != NULL) {
		var2 = it->second.create(var1);
	}

	if (var2 != NULL) {
		var2->readFromNBT(var0);
	} else {
		DbgPrintF("Skipping Entity with id %s\n", id.c_str());
	}

	return var2;
}

Entity* EntityList::createEntity(int var0, World* var1) {
	Entity* var2 = NULL;

	std::map<int, EntityReg>::iterator it = g_byId.find(var0);
	if (it != g_byId.end() && it->second.create != NULL) {
		var2 = it->second.create(var1);
	}

	if (var2 == NULL) {
		DbgPrintF("Skipping Entity with id %d\n", var0);
	}

	return var2;
}

// ----------------------------------------------------------------------------
// Reverse lookup (live Entity -> id/name) via the dynamic_cast ladder.
// ----------------------------------------------------------------------------
int EntityList::getEntityID(Entity* var0) {
	const char* name = resolveEntityName(var0);
	std::map<std::string, EntityReg>::iterator it = g_byName.find(name);
	if (it != g_byName.end()) {
		return it->second.id;
	}
	return 0; // unregistered (Java would have NPE'd; 0 is the safe sentinel)
}

const std::string& EntityList::getEntityString(Entity* var0) {
	const char* name = resolveEntityName(var0);
	std::map<std::string, EntityReg>::iterator it = g_byName.find(name);
	if (it != g_byName.end()) {
		return it->second.name;
	}
	g_emptyName.clear();
	return g_emptyName; // Java returned null -> empty string (callers test .empty())
}

// ----------------------------------------------------------------------------
// Registry build (was Java `static {}`).
// ----------------------------------------------------------------------------
void EntityList::initStatics() {
	if (s_stringToClassMapping == NULL) {
		s_stringToClassMapping = new std::map<std::string, void*>();
		s_classToStringMapping = new std::map<std::string, void*>();
		s_IDtoClassMapping     = new std::map<std::string, void*>();
		s_classToIDMapping     = new std::map<std::string, void*>();
	}

	g_byName.clear();
	g_byId.clear();

	// Helper to register one mapping (name, id, creator). EntityReg is stack-built
	// then handed to registerMapping(), which copies it into the registry maps.
	struct Local {
		static void reg(const std::string& name, int id, EntityCreatorFn fn) {
			EntityReg e;
			e.name   = name;
			e.id     = id;
			e.create = fn;
			registerMapping((void*)&e, name, id);
		}
	};

	Local::reg("Arrow",       10, &createT<EntityArrow>);
	Local::reg("Snowball",    11, &createT<EntitySnowball>);
	Local::reg("Item",         1, &createT<EntityItem>);
	Local::reg("Painting",     9, &createT<EntityPainting>);
	Local::reg("Mob",         48, &createT<EntityLiving>);
	Local::reg("Monster",     49, &createT<EntityMobs>);
	Local::reg("Creeper",     50, &createT<EntityCreeper>);
	Local::reg("Skeleton",    51, &createT<EntitySkeleton>);
	Local::reg("Spider",      52, &createT<EntitySpider>);
	Local::reg("Giant",       53, &createT<EntityZombieSimple>);
	Local::reg("Zombie",      54, &createT<EntityZombie>);
	Local::reg("Slime",       55, &createT<EntitySlime>);
	Local::reg("Ghast",       56, &createT<EntityGhast>);
	Local::reg("PigZombie",   57, &createT<EntityPigZombie>);
	Local::reg("Pig",         90, &createT<EntityPig>);
	Local::reg("Sheep",       91, &createT<EntitySheep>);
	Local::reg("Cow",         92, &createT<EntityCow>);
	Local::reg("Chicken",     93, &createT<EntityChicken>);
	Local::reg("PrimedTnt",   20, &createT<EntityTNTPrimed>);
	Local::reg("FallingSand", 21, &createT<EntityFallingSand>);
	Local::reg("Minecart",    40, &createT<EntityMinecart>);
	Local::reg("Boat",        41, &createT<EntityBoat>);
}
