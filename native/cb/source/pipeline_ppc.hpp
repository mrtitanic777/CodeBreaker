// cb/source/pipeline_ppc.hpp -- `match` and `repair` for an Xbox 360 image, and `repair --truth` (internal to source/pipeline_*.cpp).
//
// The Xbox 360 compiler (settings: powerpc_tools) makes an object whose relocation records say which instruction words hold what the linker fills in --
// the 24-bit distance of a `bl`, the 14 bits of a conditional branch, a 16-bit half of an address -- so a function's compiled code is compared with the
// original's word by word with only those bits left out, its opcode and registers still compared (compare/mask: distance_words). The image has no such
// records: the object's mask is laid over the original's function at the same places, and the original's own calls out of the function are masked as well.
#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/source/pipeline_world.hpp"
#include "cb/verify/authored/repair.hpp"

namespace cb::source::pipeline {

// Whether a module is an Xbox 360 image: an .xex, or a PE file whose machine is PowerPC.
bool is_powerpc_module(World& world, const std::filesystem::path& path, const std::string& given);

// match <image> [--flags SWITCHES] [--try] [--json FILE]; an .xex is unpacked first (xex::basefile).
int match_powerpc_command(World& world, const std::filesystem::path& path, const std::string& given, const cli::Args& args);

// repair <image> [--flags SWITCHES] [--first N]
int repair_powerpc_command(World& world, const std::filesystem::path& path, const verify::authored::repair::Arguments& arguments);

// repair --truth <compiler> [target]: the search tried on a period compiler's own units.
int repair_truth_command(World& world, const std::vector<std::string>& words);

}  // namespace cb::source::pipeline
