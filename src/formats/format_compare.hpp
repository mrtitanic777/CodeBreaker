// formats/format_compare.hpp -- the proof of the format engine: its tree of a file, field by field against the
// hand-written reader of the same format (mz: dos.cpp, pe: pe.cpp, ne: ne.cpp, ines: nes.cpp).
#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/format_engine.hpp"

namespace cb::formats::engine {

struct Mismatch {
    std::string field, engine, reader;
};

struct Comparison {
    std::string format;
    bool reader_accepts = false;   // the hand-written reader reads the file as this format
    bool engine_accepts = false;   // the description reads it
    std::string engine_error;      // why not, when the reader accepted
    std::size_t compared = 0;      // values compared
    std::size_t mismatch_count = 0;
    std::vector<Mismatch> mismatches;   // the first few
    bool round_trip = false;            // write(read(file)) is the file
    bool covered_round_trip = false;    // the bytes the tree names, written onto zeros, are the file's bytes there
    std::uint64_t covered = 0, file_size = 0;
    std::string note;                   // what was not compared (a packed program's relocations ...)

    // The two readers agree: both accept and no value differs, or neither accepts.
    bool agree() const { return reader_accepts == engine_accepts && mismatch_count == 0; }
};

// A digest of everything the readers of pe, ne, le, dos (MZ), nes, xbe, xdbf, pck, cab, the 32-bit ELF readers (mips_elf, ppc_elf) and the
// console programs' (ps3, vita, orbis, nro) answer about the file (format_check --readers). `file` (when given) is where the data came from: a
// cabinet's folders are read from it. With `text`, the digest's own lines (format_check --readers-text: what differs).
std::string reader_digest(ByteView data, const std::filesystem::path& file = {}, bool text = false);

// Every format compare() knows, in the order format_check goes through them.
const std::vector<std::string>& compared_formats();

// format: one of compared_formats() (the description must be in the registry).
Comparison compare(const Registry& registry, std::string_view format, ByteView data);

}  // namespace cb::formats::engine
