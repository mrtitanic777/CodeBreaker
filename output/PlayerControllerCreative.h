#ifndef PLAYERCONTROLLERCREATIVE_H
#define PLAYERCONTROLLERCREATIVE_H

// Auto-generated from PlayerControllerCreative.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerController.h"

class EntityPlayer;
class Minecraft;
class MobSpawner;
class World;

class PlayerControllerCreative : public PlayerController {
public:
	PlayerControllerCreative();
	PlayerControllerCreative(Minecraft* var1);
	virtual ~PlayerControllerCreative();

	virtual void onRespawn(EntityPlayer* var1);
	virtual bool shouldDrawHUD();
	virtual void onWorldChange(World* var1);
	virtual void onUpdate();

public: // was private
	MobSpawner* m_mobSpawner;


};

#endif // PLAYERCONTROLLERCREATIVE_H