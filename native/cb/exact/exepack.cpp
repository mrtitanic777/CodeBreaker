#include "cb/exact/exepack.hpp"

#include <algorithm>
#include <array>
#include <string_view>

#include "cb/core/io.hpp"

namespace cb::exact::exepack {
namespace {

constexpr std::string_view kSignature = "RB";
constexpr std::string_view kMessage = "Packed file is corrupt";
constexpr std::size_t kMzHeaderBytes = 0x20;       // the smallest file that can hold an MZ header and be packed
constexpr std::size_t kPackedHeaderBytes = 18;     // EXEPACK's header: seven words, then (for the old form) padding
constexpr std::size_t kGroups = 16;                // one group of relocations for each 64K of the image

bool starts_mz(ByteView b) {
    if (b.size() < 2) return false;
    return (b[0] == 'M' && b[1] == 'Z') || (b[0] == 'Z' && b[1] == 'M');
}

bool has_signature(ByteView b, std::size_t at) { return as_text(b.subspan(at, 2)) == kSignature; }

}  // namespace

std::optional<Packed> read(ByteView file) {
    if (!starts_mz(file) || file.size() < kMzHeaderBytes) return std::nullopt;
    io::ByteReader mz(file, "exepack");
    mz.seek(8);
    const std::uint32_t header_paragraphs = mz.u16();
    mz.seek(0x16);
    const std::uint32_t cs = mz.u16();
    const std::uint64_t at = static_cast<std::uint64_t>(header_paragraphs) * 16 + static_cast<std::uint64_t>(cs) * 16;
    // (the packed file's CS:0 - EXEPACK's header; the signature sits at word 7 or, in the long form, word 8)
    if (at + kPackedHeaderBytes > file.size() || (!has_signature(file, at + 14) && !has_signature(file, at + 16)))
        return std::nullopt;

    io::ByteReader r(file, "exepack");
    r.seek(at);
    Packed p;
    p.ip = r.u16();
    p.cs = r.u16();
    p.mem = r.u16();
    p.size = r.u16();
    p.sp = r.u16();
    p.ss = r.u16();
    p.dest = r.u16();

    const std::string_view text = as_text(file);
    const std::size_t found = text.find(kMessage, at);
    if (found == std::string_view::npos) return p;
    io::ByteReader table(file, "exepack relocations");
    table.seek(found + kMessage.size());
    for (std::uint32_t group = 0; group < kGroups; ++group) {
        if (table.remaining() < 2) break;
        const std::uint32_t count = table.u16();
        for (std::uint32_t k = 0; k < count; ++k) p.relocs.push_back(group * 0x10000u + table.u16());
    }
    return p;
}

}  // namespace cb::exact::exepack
