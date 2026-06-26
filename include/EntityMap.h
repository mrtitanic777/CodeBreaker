#ifndef ENTITYMAP_H
#define ENTITYMAP_H

// Auto-generated from EntityMap.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <vector>

struct AxisAlignedBB;
class Entity;
class EntityMapSlot;

class EntityMap {
public:
	EntityMap();
	EntityMap(int var1, int var2, int var3);
	virtual ~EntityMap();

	int width;
	int m_depth;
	int height;
	std::vector<void*>* /* PORT: determine element type */ m_entityGrid;
	std::vector<void*> /* PORT: determine element type */ m_entities;

	virtual void insert(Entity* var1);
	virtual void remove(Entity* var1);
	virtual std::vector<void*> /* PORT: determine element type */ a(Entity* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual std::vector<void*> /* PORT: determine element type */ a(Entity* var1, const AxisAlignedBB& var2);
	virtual void updateEntities();

private:
	EntityMapSlot* m_slot;
	EntityMapSlot* m_slot2;
	std::vector<void*> /* PORT: determine element type */ m_entitiesExcludingEntity;

	std::vector<void*> /* PORT: determine element type */ a(Entity* var1, float var2, float var3, float var4, float var5, float var6, float var7, std::vector<void*> /* PORT: determine element type */ var8);

};

#endif // ENTITYMAP_H