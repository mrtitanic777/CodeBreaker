// Achievements.cpp -- CodeBreaker-emitted. Routes in-code unlocks to the platform write.
#include "stdafx.h"
#include "Achievements.h"

#if defined(_XBOX) || defined(_XENON)
  #include <xtl.h>
#endif
#include <string.h>

bool Achievement_Unlock(int userIndex, int achievementId)
{
#if defined(_XBOX) || defined(_XENON)
    XUSER_ACHIEVEMENT ach;
    ach.dwUserIndex = (DWORD)userIndex;
    ach.dwAchievementId = (DWORD)achievementId;
    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));
    // async LIVE write; queues + syncs later if offline.
    DWORD r = XUserWriteAchievements(1, &ach, &ov);
    if (r != ERROR_IO_PENDING && r != ERROR_SUCCESS) return false;
    while (!XHasOverlappedIoCompleted(&ov)) { }
    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;
#else
    (void)userIndex; (void)achievementId; return true;
#endif
}
