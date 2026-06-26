#ifndef ENTITYSORTER_H
#define ENTITYSORTER_H

// Auto-generated from EntitySorter.java
// Target: Xbox 360 / C++03 / DirectX 9

class EntityPlayer;

class EntitySorter {
public:
	EntitySorter();
	EntitySorter(EntityPlayer* var1);
	virtual ~EntitySorter();

	virtual int compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2);
	bool operator()(void* a, void* b) { return compare(a, b) < 0; }

public: // was private
	EntityPlayer* m_player;

};

#endif // ENTITYSORTER_H