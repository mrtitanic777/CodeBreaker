// codebreaker/formats/xbox/crypto/bootchain.hpp -- typed C++ from codebreaker.formats.xbox.crypto.bootchain.
//
// Xbox 360 bootchain stage parser.
//
// Ported from xenon-bltool.
// Parses and identifies bootloader stages from NAND dumps:
//   CB (2BL), CB_B, CD (4BL), CE (5BL), CF (6BL), CG (7BL)
//
// Each stage has a common 16-byte header:
//   uint16 magic, uint16 version, uint16 pairing, uint16 flags,
//   uint32 entrypoint, uint32 size
#pragma once

#include <optional>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::bootchain {

// -- Bootloader types --

constexpr int64_t BL_1BL = 0;
constexpr int64_t BL_CB = 1;
constexpr int64_t BL_SC = 2;
constexpr int64_t BL_CD = 3;
constexpr int64_t BL_CE = 4;
constexpr int64_t BL_CF = 5;
constexpr int64_t BL_CG = 6;
constexpr int64_t BL_HV = 0x10;

extern const py::dict<int64_t, py::str> BL_NAMES;

// Magic values for bootloader identification
constexpr int64_t MAGIC_CB = 0x4342;   // "CB"
constexpr int64_t MAGIC_CD = 0x4344;   // "CD"
constexpr int64_t MAGIC_CE = 0x4345;   // "CE"
constexpr int64_t MAGIC_CF = 0x4346;   // "CF"
constexpr int64_t MAGIC_CG = 0x4347;   // "CG"
constexpr int64_t MAGIC_HV = 0x4856;   // "HV"

extern const py::dict<int64_t, int64_t> MAGIC_MAP;

// (a @dataclass in Python)
struct BootloaderHeader {
    int64_t magic = 0;
    int64_t version = 0;
    int64_t pairing = 0;
    int64_t flags = 0;
    int64_t entrypoint = 0;
    int64_t size = 0;
    int64_t bl_type = -1;
    py::str bl_name = py::str("Unknown");
    bool is_devkit = false;
    bool is_manufacturing = false;
    bool is_cba = false;
    py::bytes key = py::bytes();
    int64_t offset_in_nand = 0;
};

// Full bootchain analysis result. (a @dataclass in Python; stages starts as an empty list)
struct BootchainInfo {
    py::list<BootloaderHeader> stages;
    int64_t nand_size = 0;
    int64_t header_magic = 0;
    int64_t cb_version = 0;
    int64_t cd_version = 0;
    int64_t ce_version = 0;
};

// -- Parsing --

// Parse a 16-byte bootloader header.
std::optional<BootloaderHeader> parse_header(const py::bytes& data, int64_t offset = 0);

// Sanity check a parsed header.
bool _is_valid_header(const BootloaderHeader& h);

// -- NAND scanning --

// Standard NAND offsets for bootloader stages
extern const py::dict<py::str, int64_t> NAND_OFFSETS;

// Scan a NAND dump for all bootloader stages.
//
// Looks for bootloader headers at known offsets and by
// scanning for magic values.
BootchainInfo scan_nand(const py::bytes& data);

// Extract a raw bootloader stage from a NAND dump.
py::bytes extract_stage(const py::bytes& data, const BootloaderHeader& stage);

// Format bootchain info as a readable report.
py::str format_bootchain(const BootchainInfo& info);

extern const py::str DESCRIPTION;

// main(): the command line without the program name (sys.argv[1:]); prints to stdout.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::bootchain
