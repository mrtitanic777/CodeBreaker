#ifndef PLAYERLOADER_H
#define PLAYERLOADER_H

// Auto-generated from PlayerLoader.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "LevelLoader.h"
#include <string>

class Entity;
class IProgressUpdate;
class Minecraft;
class World;

class PlayerLoader : public LevelLoader {
public:
	PlayerLoader();
	PlayerLoader(Minecraft* var1, IProgressUpdate* var2);
	virtual ~PlayerLoader();


protected:
	virtual Entity* loadEntity(World* var1, const std::string& var2);

private:
	Minecraft* mc;

};

#endif // PLAYERLOADER_H