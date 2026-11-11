#pragma once

#include <cstdint>

// The avatar data XAM answers without an avatar system (kernel/xam_avatar.cpp).
namespace kernel::avatar
{
    // XamAvatarGetManifestsByXuid after its privilege check: for each of `count` (1 to 16) XUIDs at guest address
    // `xuids`, the profile's 1000-byte manifest at `metadata` + i * 1000 and the result 0 at `results` + i * 4, or the
    // bytes zeroed and 0x80990001 (a profile without an avatar, or a player this console has no profile of). The call's
    // own HRESULT: S_OK, E_INVALIDARG for a bad count or a null pointer.
    uint32_t ManifestsByXuid(uint32_t count, uint32_t xuids, uint32_t results, uint32_t metadata);
}
