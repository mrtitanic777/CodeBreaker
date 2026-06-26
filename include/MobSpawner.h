#ifndef MOBSPAWNER_H
#define MOBSPAWNER_H

// Auto-generated from MobSpawner.java
// Target: Xbox 360 / C++03 / DirectX 9

class World;

class MobSpawner {
public:
	MobSpawner();
	MobSpawner(World* var1);
	virtual ~MobSpawner();

	virtual void performSpawning();

private:
	World* worldObj;

};

#endif // MOBSPAWNER_H