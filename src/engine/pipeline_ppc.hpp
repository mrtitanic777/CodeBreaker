// engine/pipeline_ppc.hpp -- `match` and `repair` for an Xbox 360 image, and `repair --truth` (internal to source/pipeline_*.cpp).
//
// The Xbox 360 compiler (settings: powerpc_tools) makes an object whose relocation records say which instruction words hold what the linker fills in --
// the 24-bit distance of a `bl`, the 14 bits of a conditional branch, a 16-bit half of an address -- so a function's compiled code is compared with the
// original's word by word with only those bits left out, its opcode and registers still compared (compare/mask: distance_words). The image has no such
// records: the object's mask is laid over the original's function at the same places, and the original's own calls out of the function are masked as well.
#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/cli.hpp"
#include "formats/powerpc_image.hpp"
#include "engine/pipeline_world.hpp"
#include "verify/authored/repair.hpp"

namespace cb::engine::pipeline {

// Whether a module is an Xbox 360 image: an .xex, or a PE file whose machine is PowerPC.
bool is_powerpc_module(World& world, const std::filesystem::path& path, const std::string& given);

// match <image> [--flags SWITCHES] [--try] [--json FILE]; an .xex is unpacked first (xex::basefile).
int match_powerpc_command(World& world, const std::filesystem::path& path, const std::string& given, const cli::Args& args);

// repair <image> [--flags SWITCHES] [--first N]
int repair_powerpc_command(World& world, const std::filesystem::path& path, const verify::authored::repair::Arguments& arguments);

// repair --truth <compiler> [target]: the search tried on a period compiler's own units.
int repair_truth_command(World& world, const std::vector<std::string>& words);

// ---- the parts the tests reach ----

// The bits a linker fills in, as the masks of a function's words say them.
inline constexpr std::uint32_t kPpcMaskLi = 0x03FFFFFC;     // the 24-bit distance of b / bl
inline constexpr std::uint32_t kPpcMaskHalf = 0x0000FFFF;   // a 16-bit half of an address

// One function compiled by the Xbox 360 compiler `cl` (cl.exe) under `flags`, read from its object; nullopt when it does not compile.
std::optional<verify::authored::repair::Code> ppc_compile_function(const std::filesystem::path& cl, std::string_view header, std::string_view source,
                                                                   std::string_view name, std::string_view flags, const std::filesystem::path& work);

// The original's code of each function address, with the bits that depend on where things are (the words of one function end before its padding).
std::map<std::uint64_t, verify::authored::repair::Code> ppc_original_code(const formats::powerpc_image::Image& image, const std::vector<std::uint64_t>& addresses);

// How far a compiled function is from the original's, in words, outside the masked bits.
verify::authored::search16::ByteVerdict ppc_compare(const verify::authored::repair::Code& built, const verify::authored::repair::Code& original);

}  // namespace cb::engine::pipeline
