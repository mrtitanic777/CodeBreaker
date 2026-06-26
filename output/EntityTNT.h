#ifndef ENTITYTNT_H
#define ENTITYTNT_H

// Auto-generated from EntityTNT.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include <string>

class NBTTagCompound;
class World;

class EntityTNT : public Entity {
public:
	EntityTNT();
	EntityTNT(World* var1, float var2, float var3, float var4);
	virtual ~EntityTNT();

	int m_fuse;

	virtual bool canBeCollidedWith();
	virtual void onEntityUpdate();
	virtual float getShadowSize();

public: // was protected
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();

};

#endif // ENTITYTNT_H