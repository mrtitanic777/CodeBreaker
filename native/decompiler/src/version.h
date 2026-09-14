// XEXMagic - build identity.
//
// The analysis cache stores this alongside the results. A newer build can
// decompile differently, so a cache written by a different build is stale by
// definition and is re-analysed rather than trusted.

#pragma once

#include <cstdint>
#include <string>

namespace xexmagic {

// Bump when the release changes. Shown in the UI and stored in the cache.
constexpr const char* kVersion = "0.1.0";

// Layout of the analysis cache. Bump when the stored fields change; an older
// file is discarded rather than misread.
constexpr uint32_t kSchemaVersion = 1;

// Identity of the running executable: version plus the build stamp of the
// binary itself, so any rebuild - including one that only touched the
// decompiler - invalidates caches written by the previous build.
std::string BuildId();

}  // namespace xexmagic
