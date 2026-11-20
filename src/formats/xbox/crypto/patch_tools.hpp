// formats/xbox/crypto/patch_tools.hpp -- the hypervisor-patch pipeline: assemble the patch sources, check the patch tables for places that
// overlap, sign the expansions, and read a patch table back as assembly.
//
//     patch_tools::compile_patches(tools, base);                    // `crypto patch_compile`
//     auto conflicts = patch_tools::find_conflicts(table);          // `crypto patch_checker`
//     patch_tools::build_expansions(tools, store, base, random);    // `crypto exp_build_task`
//     std::string source = patch_tools::disassemble(table);         // `crypto patch_dumper`
//
// A patch table (what xeBuild and the bootloader patcher read) is a list of records, each an address, a count of words and the words, all
// big-endian, ended by the address 0xFFFFFFFF. The sources are PowerPC assembly for the Free60 xenon-as (`tools.bin_dir`), built with the defines
// of their target (build_lib.hpp's PatchTarget). Paths are under a base folder (the current one for the tools): sources in Patches/, builds into
// Output/.
//
//     patch_compile    RGLoader (Patches/RGL/17559-dev/RGLoader-dev.S): Output/NonZero/RGL_flash.bin and RGL_hdd.bin, Output/Zero/VRGL_flash.bin and
//                      VRGL_hdd.bin; Patches/SC0.S for devkits: Output/Compiled/Patches/SC0_dev.bin and SpoofySC0_dev.bin (the SPOOFY define)
//     patch_checker    the four RGLoader tables above, or the tables named
//     patch_build_task compile, then check
//     exp_build_task   assemble HVPP and Spoofy (devkit and retail), sign each as an expansion (generic and xbOnline ids; devkit HXPR encrypted,
//                      test kit SIGM, retail HXPR unencrypted) and write each as a C++ array (ExpansionData) beside it
//     patch_dumper     a patch table as assembly: `MAKEPATCH address` ... `9:` blocks of instructions (`.long` for a word Capstone cannot decode)
#pragma once

#include <cstdint>
#include <filesystem>
#include <span>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "formats/xbox/crypto/build_lib.hpp"
#include "formats/xbox/crypto/keystore.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::patch_tools {

// Two patches whose places touch or overlap (a patch is the address to the address plus its bytes; neighbours with no gap count).
struct Conflict {
    std::uint32_t a_start = 0, a_end = 0;
    std::uint32_t b_start = 0, b_end = 0;
};

// Every ordered pair of conflicting patches of the table (A, B and B, A both). ParseError/TruncatedError for a table that is cut short.
std::vector<Conflict> find_conflicts(ByteView table);
// "Patch Conflict!" blocks for them, as the tool prints; empty when there are none.
std::string conflict_report(std::span<const Conflict> conflicts);

// The six assemblies of patch_compile; the files written, in order.
std::vector<std::filesystem::path> compile_patches(const build_lib::PatchTools& tools, const std::filesystem::path& base);

// The expansions of exp_build_task; the files written, in order. The key store gives the HVX key (checked) and 1BL key.
std::vector<std::filesystem::path> build_expansions(const build_lib::PatchTools& tools, const keystore::KeyStore& store, const std::filesystem::path& base,
                                                    xecrypt::RandomSource& random = xecrypt::os_random());

// The assembly of a patch table, the instructions as Capstone's PowerPC decoder writes them.
std::string disassemble(ByteView table);

cli::Command checker_command();
cli::Command compile_command(const build_lib::PatchTools& tools);
cli::Command build_task_command(const build_lib::PatchTools& tools);
cli::Command exp_build_command(const keystore::KeyStore& store, const build_lib::PatchTools& tools);
cli::Command dumper_command();

}  // namespace cb::formats::xbox::crypto::patch_tools
