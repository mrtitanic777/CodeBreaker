#ifndef RENDERSORTER_H
#define RENDERSORTER_H

// Auto-generated from RenderSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

class EntityPlayer;
class WorldRenderer;

class RenderSorter {
public:
	RenderSorter();
	RenderSorter(EntityPlayer* var1);
	virtual ~RenderSorter();

	virtual int a(WorldRenderer* var1, WorldRenderer* var2);
	virtual int compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2);
	bool operator()(void* a, void* b) { return compare(a, b) < 0; }

public: // was private
	EntityPlayer* m_field_4274_a;

};

#endif // RENDERSORTER_H