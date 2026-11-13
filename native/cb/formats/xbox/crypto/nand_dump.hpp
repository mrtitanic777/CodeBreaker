// cb/formats/xbox/crypto/nand_dump.hpp -- the `crypto nand` tool: what a NAND dump is (by its size and its header) and the hypervisor's
// first 256 KB cut out of it.
//
//     nand_dump::Summary s = nand_dump::summarise(dump);     // type, header fields
//     Bytes hv = nand_dump::extract_hypervisor(dump);         // the first 0x40000 bytes
//
// The type is told by the size alone: 16 MB is the small-block flash, 64 MB the big-block (Jasper), 256 MB a Corona MMC and 512 MB a
// Corona 4 GB MMC. The header is the first bytes of the flash image: a two-byte magic (FF 4F on a real flash), build, QFE, flags,
// entry point and size, all big-endian.
//
// (The page-level tools of the NAND images, ECC and spares, are nand_tool.hpp's.)
//
// Commands of `crypto nand`: `info <dump>` and `extract-hv <dump> <out.bin>` (also `hv`).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"

namespace cb::formats::xbox::crypto::nand_dump {

inline constexpr std::size_t kHypervisorSize = 0x40000;
inline constexpr std::size_t kHeaderSize = 0x80;

struct Header {
    std::array<std::uint8_t, 2> magic{};
    std::uint16_t build = 0;
    std::uint16_t qfe = 0;
    std::uint16_t flags = 0;
    std::uint32_t entry_point = 0;
    std::uint32_t size = 0;
};

struct Summary {
    std::uint64_t size = 0;
    std::string type;                    // "16MB (small block)" ... or "Unknown (<size> bytes)"
    std::optional<Header> header;        // none for a dump shorter than 0x80 bytes
};

std::string type_of(std::uint64_t size);
Summary summarise(ByteView dump);

// The first 0x40000 bytes. FormatError for a dump shorter than that.
Bytes extract_hypervisor(ByteView dump);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::nand_dump
