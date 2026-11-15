// cb/formats/chf.hpp -- Fairchild Channel F cartridge images (`.chf`, or a `.bin` that starts $55 $2B): the bytes that sit in the F8's memory from $0800.
//
// The console's CPU is a Fairchild F8 (3850). Its two 1 KB BIOS ROMs are at $0000-$07FF (not in a cartridge file); the cartridge's ROM is at $0800 and up (a "Videocart" is 1-3 KB,
// some have 2 KB of RAM at $2800) and the BIOS starts it by running the code at $0800: a cartridge's first instructions are `LR 5,A` and `NOP` ($55 $2B), which also mark it
// as a cartridge to the BIOS. Everything else the machine does goes through the ports: the video RAM, the controllers and the sound are written and read with IN / OUT / INS / OUTS.
//
// A cartridge file has no header: it is taken to be one by its extension (`.chf`) or by its first two bytes, and a size that is a whole number of 256-byte pages up to 62 KB.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>

#include "cb/core/bytes.hpp"

namespace cb::formats::chf {

inline constexpr std::uint32_t kBase = 0x0800;
inline constexpr std::size_t kMaxSize = 0x10000 - kBase;

struct Rom {
    Bytes data;                 // the file: the memory from $0800
    std::uint32_t address(std::size_t offset) const { return kBase + static_cast<std::uint32_t>(offset); }
};

// The BIOS routines that Videocarts call, by address (from the Channel F programming documentation: the clear-screen, delay, push-K, pop-K and draw-character routines).
const std::map<std::uint32_t, std::string>& bios();

// FormatError when the file is not one (empty, over 62 KB, not whole pages).
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// A `.chf`, or a `.bin` starting $55 $2B, that parses. Reads the first bytes and the size only.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::chf
