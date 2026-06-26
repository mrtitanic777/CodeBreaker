#ifndef ENTITYSORTER_H
#define ENTITYSORTER_H

// Auto-generated from EntitySorter.java
// Target: Xbox 360 / C++03 / DirectX 9

class Entity;
class WorldRenderer;

class EntitySorter {
public:
	EntitySorter();
	EntitySorter(Entity* var1);
	virtual ~EntitySorter();

	virtual int a(WorldRenderer* var1, WorldRenderer* var2);
	virtual int compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2);
	bool operator()(void* a, void* b) { return compare(a, b) < 0; }

public: // was private
	Entity* m_field_1594_a;

};

#endif // ENTITYSORTER_H