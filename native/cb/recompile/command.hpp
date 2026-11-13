// cb/recompile/command.hpp -- the Xbox 360 recompiler's three commands, complete.
//
//     recompile <image or .xex> [--all] [--at ADDR --size N] [-o FILE] [--limit N] [-j N] [--check] [--runtime HEADER]
//     recompile-project <title> [-o <folder>] [--name N] [--title-files <dir>] [--build] [--check [seconds]] [--runtime x360|rexglue] [-j N]
//     xenon-recompile <config.toml>
//
//     table.set("recompile", command::recompile_runner({.config = config}));
//     table.set("recompile-project", command::recompile_project_runner({.config = config}));
//     table.set("xenon-recompile", command::xenon_recompile_runner({.config = config}));
//
// Each is the module's command (powerpc::main, project::run_command, xenon::xenon_recompile_command) with everything it was given as a hook
// made from the configuration: images read through cb::formats (an .xex through its basefile, the xex program of setting xex_bin), the Xenon
// vector decoder, the Converted folder of the project (setting converted), the title finder over the Xbox containers, XenonRecomp's tool chain
// (settings visual_studio, ppc_binutils; built on demand), the rexglue kernel core under native/rexglue, the console's ordinal names
// (codebreaker/symbols/console_ordinals.json), and g++ (setting gpp, else PATH) for `--check`.
#pragma once

#include <filesystem>
#include <functional>
#include <span>
#include <string>

#include "cb/base/config.hpp"
#include "cb/formats/powerpc_image.hpp"
#include "cb/recompile/config.hpp"
#include "cb/recompile/powerpc.hpp"
#include "cb/recompile/project.hpp"
#include "cb/recompile/switches.hpp"

namespace cb::recompile::command {

struct Environment {
    cb::config::Config config;
};

// ---- an image, as each module reads it ----------------------------------------------------------------------------------

// A PowerPC image or an .xex (through its basefile, as setting xex_bin and the state folder give it).
formats::powerpc_image::Image read_image(const std::filesystem::path& file, const Environment& environment);

// What discovery, the disassembler and the recompiler read of an image.
discover::Image discovery_image(const formats::powerpc_image::Image& image);
powerpc::LoadedImage loaded_image(const formats::powerpc_image::Image& image);
// What the switch-table finder reads, and what the configuration reads.
switches::CodeImage code_image(const formats::powerpc_image::Image& image);
config::Title config_title(const formats::powerpc_image::Image& image);

// The Xenon's vector set, ahead of Capstone's reading.
disassemble::VectorDecoder vector_decoder();

// ---- the commands ---------------------------------------------------------------------------------------------------------------

int recompile(std::span<const std::string> args, const Environment& environment);
int recompile_project(std::span<const std::string> args, const Environment& environment);
int xenon_recompile(std::span<const std::string> args, const Environment& environment);

using Runner = std::function<int(std::span<const std::string>)>;
Runner recompile_runner(const Environment& environment);
Runner recompile_project_runner(const Environment& environment);
Runner xenon_recompile_runner(const Environment& environment);

}  // namespace cb::recompile::command
