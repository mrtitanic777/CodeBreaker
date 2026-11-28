// source/console_risc.hpp -- inside the console writer: what the vectored shape's RISC processors (console_mips.cpp, console_arm.cpp) write
// the same way: data rows in GNU as's (strings, fills, words that may be names, bytes) and a header's fields as the description lists them.
#pragma once

#include <algorithm>
#include <cstdint>
#include <format>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "core/text.hpp"
#include "source/console_shapes.hpp"

namespace cb::source::console_writer::risc {

inline constexpr unsigned kStringMin = 4;     // a run of printable bytes this long is a string
inline constexpr unsigned kStringMax = 96;
inline constexpr std::size_t kFillMin = 16;   // a run of one value this long is `.space` / `.fill`

inline std::uint32_t word_at(ByteView b, std::size_t off, bool big) {
    if (big) {
        return static_cast<std::uint32_t>(b[off]) << 24 | static_cast<std::uint32_t>(b[off + 1]) << 16 | static_cast<std::uint32_t>(b[off + 2]) << 8 |
               static_cast<std::uint32_t>(b[off + 3]);
    }
    return static_cast<std::uint32_t>(b[off]) | static_cast<std::uint32_t>(b[off + 1]) << 8 | static_cast<std::uint32_t>(b[off + 2]) << 16 |
           static_cast<std::uint32_t>(b[off + 3]) << 24;
}

inline bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }

inline std::string hex8(std::uint32_t v) { return std::format("0x{:08X}", v); }

inline std::string byte_row(ByteView bytes) {
    std::vector<std::string> items;
    for (const std::uint8_t b : bytes) items.push_back(std::format("0x{:02X}", b));
    return "        .byte " + text::join(items, ", ");
}

inline std::size_t printable_run(ByteView buf, std::size_t off, std::size_t limit) {
    std::size_t run = 0;
    while (off + run < limit && run < kStringMax && printable(buf[off + run])) ++run;
    return run;
}

inline std::size_t same_run(ByteView buf, std::size_t off, std::size_t limit) {
    std::size_t run = 1;
    while (off + run < limit && buf[off + run] == buf[off]) ++run;
    return run;
}

// Where a string starts inside the word at buf[at] (1 to 3 bytes in), or 0.
inline std::size_t string_inside_word(ByteView buf, std::size_t at, std::size_t limit) {
    for (std::size_t k = 1; k < 4 && at + k < limit; ++k)
        if (printable_run(buf, at + k, limit) >= kStringMin) return k;
    return 0;
}

// One line of data from buf[off], which is at `addr`, to before `limit`: a string, a run of one value, up to four words (a word that `named`
// names is written as the name), or a few bytes. How many bytes it took, and the line.
struct DataItem {
    std::string line;
    std::size_t took = 0;
};
inline DataItem data_item(ByteView buf, std::size_t off, std::size_t limit, std::uint32_t addr, bool big,
                          const std::function<std::optional<std::string>(std::uint32_t)>& named) {
    if (const std::size_t run = printable_run(buf, off, limit); run >= kStringMin) {
        const bool terminated = off + run < limit && buf[off + run] == 0;
        return {std::format("        {} \"{}\"", terminated ? ".asciz" : ".ascii", as_text(buf.subspan(off, run))), run + (terminated ? 1u : 0u)};
    }
    if (const std::size_t same = same_run(buf, off, limit); same >= kFillMin)
        return {buf[off] == 0 ? std::format("        .space {}", same) : std::format("        .fill {}, 1, 0x{:02X}", same, buf[off]), same};
    if (addr % 4 == 0 && off + 4 <= limit) {
        if (const std::size_t lead = string_inside_word(buf, off, limit); lead != 0) return {byte_row(buf.subspan(off, lead)), lead};   // (the bytes before a string)
        std::vector<std::string> items;
        std::size_t took = 0;
        while (items.size() < 4 && off + took + 4 <= limit) {
            const std::size_t at = off + took;
            if (took != 0 && (printable_run(buf, at, limit) >= kStringMin || same_run(buf, at, limit) >= kFillMin || string_inside_word(buf, at, limit) != 0)) break;
            const std::optional<std::string> name = named(addr + static_cast<std::uint32_t>(took));
            items.push_back(name ? *name : hex8(word_at(buf, at, big)));
            took += 4;
        }
        return {"        .word " + text::join(items, ", "), took};
    }
    const std::size_t room = addr % 4 != 0 ? 4 - addr % 4 : limit - off;
    const std::size_t count = std::min<std::size_t>({room, limit - off, 16});
    return {byte_row(buf.subspan(off, count)), count};
}

// Data rows from `from` to `to` of `buf`, each at its own address (`at` + offset), naming no word.
inline void items(std::vector<std::string>& out, ByteView buf, std::size_t from, std::size_t to, std::uint32_t at, bool big) {
    for (std::size_t k = from; k < to;) {
        const DataItem item = data_item(buf, k, to, at + static_cast<std::uint32_t>(k - from), big, [](std::uint32_t) { return std::nullopt; });
        out.push_back(item.line);
        k += item.took;
    }
}

// A header's fields as the description lists them (VectorField: ascii, zeros, word, address, bytes, byte, items; comment, byte_rows, padded).
// `word_name` writes a word that is an address.
inline void header_fields(std::vector<std::string>& out, const Vectored& shape, ByteView file, const Values& values, const std::function<std::string(std::uint32_t)>& word_name) {
    const bool big = shape.big_endian;
    const auto zero = [](std::uint8_t b) { return b == 0; };
    for (const VectorField& f : shape.fields) {
        const ByteView part = file.subspan(f.at, f.size);
        const std::string what = fill(f.what, values);
        if (f.as == "ascii") {
            out.push_back("        .ascii \"" + std::string(as_text(part)) + "\"");
        } else if (f.as == "comment") {   // a comment line of its own
            out.push_back("        ; " + what);
        } else if (f.as == "byte_rows") {   // rows of `row` bytes
            for (std::size_t at = 0; at < part.size(); at += f.row) out.push_back(byte_row(part.subspan(at, std::min<std::size_t>(f.row, part.size() - at))));
        } else if (f.as == "padded") {    // a text and the zeros after it, or bytes
            std::size_t length = 0;
            while (length < part.size() && printable(part[length])) ++length;
            if (!std::all_of(part.begin() + static_cast<std::ptrdiff_t>(length), part.end(), zero)) {
                out.push_back(byte_row(part));
                continue;
            }
            if (length != 0) out.push_back(std::format("        .ascii \"{}\"", as_text(part.first(length))));
            if (length < part.size()) out.push_back(std::format("        .space {}", part.size() - length));
        } else if (f.as == "zeros") {
            out.push_back(std::ranges::all_of(part, zero) ? std::format("        .space {}", f.size) : byte_row(part));
        } else if (f.as == "word" || f.as == "address") {
            const std::uint32_t v = word_at(file, f.at, big);
            out.push_back(std::format("        .word {}    ; {}", f.as == "address" ? word_name(v) : hex8(v), what));
        } else if (f.as == "bytes") {
            if (std::ranges::all_of(part, zero)) out.push_back(std::format("        .space {}    ; {}", f.size, what));
            else if (std::ranges::all_of(part, printable)) out.push_back(std::format("        .ascii \"{}\"    ; {}", as_text(part), what));
            else out.push_back(byte_row(part) + "    ; " + what);
        } else if (f.as == "byte") {
            out.push_back(byte_row(part) + "    ; " + what);
        } else {   // "items": data rows
            items(out, file, f.at, f.at + f.size, f.at, big);
        }
    }
}

}  // namespace cb::source::console_writer::risc
