// formats/xbox/crypto/bootchain.hpp -- find the bootloader stages (CB, CD, CE, CF, CG) of an Xbox 360 NAND dump and read their headers.
//
//     bootchain::Bootchain chain = bootchain::scan(nand);
//     for (const bootchain::StageHeader& s : chain.stages) ...      // sorted by place in the dump
//     std::string text = bootchain::report(chain);
//
// From xenon-bltool. Every stage starts with the same 16-byte header (big-endian): magic, build, pairing, flags, entry point, size. CB, CD,
// CE, CF and CG have a 16-byte salt/nonce after it. The stages are looked for at the places a retail dump has them (CB at 0x8000, CB_B after it,
// CD after that, CE at 0x70000, 0x73000 or 0x95000), then anywhere on a 16-byte step in the first 2 MB; a header with a build of 0 or a size of
// 0 or over 16 MB is not one.
//
// `crypto bootchain <dump> [--extract N --output FILE]` prints the report, and writes stage N (the report's order from 0) to a file.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"

namespace cb::formats::xbox::crypto::bootchain {

enum class StageKind { Cb, Sc, Cd, Ce, Cf, Cg, Hv, Unknown };

struct StageHeader {
    std::uint16_t magic = 0;
    std::uint16_t build = 0;
    std::uint16_t pairing = 0;
    std::uint16_t flags = 0;
    std::uint32_t entry_point = 0;
    std::uint32_t size = 0;
    StageKind kind = StageKind::Unknown;
    std::string name;                                 // "CB/2BL", "CD/4BL", "CB_B/2BL_B" ...
    bool devkit = false;                              // pairing bit 0x8000
    bool manufacturing = false;                       // flags bit 0x0001
    bool cba = false;                                 // flags bit 0x0800
    std::optional<std::array<std::uint8_t, 16>> key;  // the 16 bytes after the header, of the stages that have them
    std::uint64_t offset = 0;                         // where the stage is in the dump
};

struct Bootchain {
    std::uint64_t nand_size = 0;
    std::vector<StageHeader> stages;
    std::uint16_t cb_build = 0;
    std::uint16_t cd_build = 0;
    std::uint16_t ce_build = 0;
};

// The header at `offset`; nullopt when fewer than 16 bytes are left there.
std::optional<StageHeader> parse_header(ByteView data, std::uint64_t offset = 0);

// Whether a header looks like a stage: a known kind, a size of 1..16 MB and a non-zero build.
bool plausible(const StageHeader& header);

Bootchain scan(ByteView nand);

// The stage's bytes (to the end of the dump if the stage runs past it).
Bytes extract(ByteView nand, const StageHeader& stage);

// "NAND Bootchain Analysis (<size> bytes)" and a row per stage with its build, place, size, entry point and flags, a line after it for the key.
std::string report(const Bootchain& chain);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::bootchain
