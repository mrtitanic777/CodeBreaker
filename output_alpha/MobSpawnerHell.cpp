#include "stdafx.h"
// Auto-generated from MobSpawnerHell.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: inherited biome-spawn lists are m_biomeMonsters/m_biomeCreatures
// (void*). The Java arrays held entity .class literals ({EntityGhast.class, EntityPigZombie.class}
// / empty), which have no C++03 equivalent (no reflection-based spawning on 360) -> left NULL.

#include "MobSpawnerHell.h"
#include "Common.h"

MobSpawnerHell::MobSpawnerHell()
{
		// PORT-BLOCKED: m_biomeMonsters was {EntityGhast.class, EntityPigZombie.class};
		//              m_biomeCreatures was new Class[0]. No .class literals in C++03.
		m_biomeMonsters = NULL;
		m_biomeMonsters_count = 0;
		m_biomeCreatures = NULL;
		m_biomeCreatures_count = 0;
}

MobSpawnerHell::~MobSpawnerHell() {
	// No dynamic resources
}
