#ifndef ENTITYMAPSLOT_H
#define ENTITYMAPSLOT_H

// Auto-generated from EntityMapSlot.java
// Target: Xbox 360 / C++03 / DirectX 9

class Entity;
class EntityMap;

class EntityMapSlot {
public:
	EntityMapSlot();
	EntityMapSlot(EntityMap* var1, unsigned char var2);
	EntityMapSlot(EntityMap* var1);
	virtual ~EntityMapSlot();

	virtual EntityMapSlot* init(float var1, float var2, float var3);
	virtual void a(Entity* var1);
	virtual void b(Entity* var1);

private:
	int m_xSlot;
	int m_ySlot;
	int m_zSlot;
	EntityMap* m_entityMap;

	int a(EntityMapSlot* var0);
	int b(EntityMapSlot* var0);
	int c(EntityMapSlot* var0);

};

#endif // ENTITYMAPSLOT_H