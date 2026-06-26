#include "stdafx.h"
// Auto-generated from PlayerLoader.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerLoader.h"
#include "Common.h"
#include "Entity.h"
#include "IProgressUpdate.h"
#include "Minecraft.h"
#include "World.h"
#include <string>
#include "EntityPlayerSP.h"
#include "LevelLoader.h"

PlayerLoader::PlayerLoader()
	: mc(NULL)
{
}

PlayerLoader::PlayerLoader(Minecraft* var1, IProgressUpdate* var2)
	: LevelLoader(var2)
	, mc(NULL)
{
		mc = var1;
}

PlayerLoader::~PlayerLoader() {
	mc = NULL;
}

Entity* PlayerLoader::loadEntity(World* var1, const std::string& var2) {
		return (Entity*)(var2 == "LocalPlayer" ? new EntityPlayerSP(mc, var1, mc->m_session) : LevelLoader::loadEntity(var1, var2));
}
