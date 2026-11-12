// XAM exports the early NXE dashboards (7258 ... 7371) look up at run time (XexGetProcedureAddress) instead of
// importing: their avatar API, bound by ordinal one after another at start-up (7357: li r4,1500 ... 1521, 1536,
// 1566 through its GetProcAddress wrapper, 0x92132420; 7258 also 1508). The later NXE dashboards import the same
// functions. With XamAvatarInitialize answering that the avatar system is not available (xam_system.cpp), the dash's
// avatar wrappers stop before calling the others, so a lookup must succeed and a call reports itself (the name is
// Xenia's table's; ordinals it does not name keep their number).

#include "kernel/kernel.h"

// Implemented as imports: the lookup gives the same host function.
PPC_FUNC(__imp__XamGetOnlineCountryString);
PPC_FUNC(__imp__XamAvatarShutdown);
PPC_FUNC(__imp__XamAvatarManifestGetBodyType);

// Ordinals 1492 and 1536 sit among the avatar ordinals but are not exports of XAM 17559: its export table leaves both
// empty (the owner's decompilation of xam.xex, xam/reference/_exports.txt, gives them the image base) and none of the
// dashboards checked (7258, 7357, 8495, 9199) imports or binds them: 7357's only "li r4,1492" and "li r4,1536" are
// allocation sizes, its lookups are 1500-1521 and 1566. Xenia's tables do not name them either. A lookup is answered "not found", as for any empty slot (kernel::IsAbsentXamExport).
bool kernel::IsAbsentXamExport(uint16_t ordinal)
{
    return ordinal == 1492 || ordinal == 1536;
}

namespace
{
    kernel::DynamicExportRegistrar g_1413("xam.xex", 1413, __imp__XamGetOnlineCountryString);
    kernel::DynamicExportRegistrar g_1501("xam.xex", 1501, __imp__XamAvatarShutdown);
    kernel::DynamicExportRegistrar g_1521("xam.xex", 1521, __imp__XamAvatarManifestGetBodyType);
}

