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

namespace
{
    kernel::DynamicExportRegistrar g_1413("xam.xex", 1413, __imp__XamGetOnlineCountryString);
    kernel::DynamicExportRegistrar g_1501("xam.xex", 1501, __imp__XamAvatarShutdown);
    kernel::DynamicExportRegistrar g_1521("xam.xex", 1521, __imp__XamAvatarManifestGetBodyType);
}

GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1492, xam_Ordinal_1492)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1502, XamAvatarGetManifestLocalUser)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1503, XamAvatarGetManifestsByXuid)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1504, XamAvatarGetAssetsResultSize)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1505, XamAvatarGetAssets)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1506, XamAvatarGenerateMipMaps)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1508, XamAvatarSetManifest) // 7258 binds it
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1509, xam_Ordinal_1509)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1510, xam_Ordinal_1510)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1511, XamAvatarLoadAnimation)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1512, XamAvatarBeginEnumAssets)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1513, XamAvatarEndEnumAssets)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1514, XamAvatarEnumAssets)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1515, XamAvatarGetMetadataRandom)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1516, xam_Ordinal_1516)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1517, xam_Ordinal_1517)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1518, XamAvatarGetMetadataSignedOutProfileCount)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1519, XamAvatarGetMetadataSignedOutProfile)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1536, xam_Ordinal_1536)
GUEST_EXPORT_UNIMPLEMENTED("xam.xex", 1566, XamUserLockLogonPreviewMode)
