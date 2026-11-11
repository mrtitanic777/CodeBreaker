// Achievements.h -- CodeBreaker-emitted achievement ids + unlock seam. See ACHIEVEMENTS.md.
#ifndef J2X_ACHIEVEMENTS_H
#define J2X_ACHIEVEMENTS_H

// Platform achievement ids (match the title config / XLAST project).
#define ACH_OPEN_INVENTORY           1
#define ACH_MINE_WOOD                2
#define ACH_BUILD_WORK_BENCH         3
#define ACH_DIAMONDS                 4
#define ACH_ON_ARAIL                 5
#define ACH_COOK_FISH                6
#define ACH_FLY_PIG                  7

// Unlock an achievement for a signed-in user. On hardware this writes to the profile over
// the online service (queues offline); off-hardware it is a no-op returning true.
bool Achievement_Unlock(int userIndex, int achievementId);

#endif // J2X_ACHIEVEMENTS_H
