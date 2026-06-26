#ifndef DEBUG_H
#define DEBUG_H

// Debug helpers (handwritten_alpha override). Fitted to Alpha's World API.

class World;

namespace Debug {
	// Places two debug chests BESIDE the spawn point (m_spawnX +-1), pre-filled with
	// (nearly) one of every block + a few starter items, for testing. The indev spawn
	// house is gone in this Alpha build, so they anchor to spawn coords directly. Not
	// auto-called -- wire into the post-generate path if a starter chest is wanted.
	void spawnDebugChests(World* level);

	// DEBUG: force-spawn one of each hostile (night) mob -- skeleton/creeper/spider/zombie --
	// right next to the player, bypassing the normal light/day-night spawn gate. Respects
	// difficulty: does nothing on Peaceful (m_difficultySetting==0).
	void forceSpawnNightMobs(World* level);
}

#endif // DEBUG_H
