#pragma once

#include <cstdint>
#include <optional>

// XMP, XAM's system music player (app 0xFA), as XAM 17559 keeps it (xam_xmp.cpp).
namespace kernel::xmp
{
    // Handles an XMP app message; nullopt if the message is not implemented (the caller stops on it).
    // Results are HRESULTs.
    std::optional<uint32_t> Dispatch(uint32_t message, uint32_t buffer, uint32_t length);
}
