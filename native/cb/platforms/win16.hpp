// cb/platforms/win16.hpp -- 16-bit Windows: NE modules (Windows 3.x and the 16-bit half of Windows 95). Their functions
// come back as C or, where they were written in assembly, as assembly.
//
// What the descriptor says: the x86 in 16-bit protected mode; the Pascal and C conventions of 16-bit Windows (the
// decompiler writes __stdcall16far ...); the 16-bit emulator is the judge's runner; OpenWatcom's wcc compiles the C,
// and the module is linked by wlink with what is kept as assembly assembled by wasm. Its steps on the engine are the
// win16 ones. (The NE reader, the emulator, the build and the lane that reconstructs it are the formats, emulate,
// build and verify layers'; they key their own tables on `Id::Win16`. The toolchain's paths are the build layer's:
// `build::Watcom` finds wcc, wlink, wasm and the medium-model library clibm.lib.)
//
// What this module does itself is what the stages ask of the NE reader through the platform: the segment table, the
// entry table, and calling one export with zero arguments.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "cb/formats/ne.hpp"
#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform win16();

// The segment table of the NE module at `path`. Throws IoError / FormatError.
std::vector<formats::ne::Segment> ne_segments(const std::filesystem::path& path);
// The entry table (every ordinal, in order).
std::vector<formats::ne::Entry> ne_entries(const std::filesystem::path& path);

// How many argument words an export is called with when nothing says: the platform's judge convention.
inline constexpr unsigned kWin16ArgumentWords = 8;

// What calling an export reads of the emulator's record: how the run ended, and the registers it left.
struct Win16Run {
    std::string ended;
    std::map<std::string, std::int64_t> results;   // by register name ("AX", "DX" ...)
};
// The platform's runner: `path`'s export `ordinal`, called with these argument words.
using Win16Runner =
    std::function<Win16Run(const std::filesystem::path& path, std::uint32_t ordinal, std::span<const std::int64_t> args)>;

struct Win16Call {
    std::string ended;                 // how the run ended; "error: <kind>" when the emulator gave up
    std::optional<std::int64_t> ax;    // the result register, when the run left one
};

// One export of a module called with zero arguments (kWin16ArgumentWords of them). The emulator giving up (any
// cb::Error but a UsageError or InternalError, which are a broken setup or a bug) is a result too.
Win16Call call_win16_export(const std::filesystem::path& path, std::uint32_t ordinal, const Win16Runner& run);

}  // namespace cb::platforms
