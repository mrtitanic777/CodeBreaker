// cb-unity: alone (file-local helpers named for this module)
// Debug symbols: CodeView records appended to an executable, and Microsoft's .SYM files.
#include <algorithm>

#include "core/error.hpp"
#include "core/io.hpp"
#include "core/text.hpp"
#include "symbols/name_data.hpp"

namespace cb::symbols::names {

namespace {

// Where `needle` first occurs in `data` at or after `from`.
std::optional<std::size_t> find_marker(ByteView data, std::string_view needle, std::size_t from) {
    if (from >= data.size()) return std::nullopt;
    const ByteView marker = as_bytes(needle);
    const auto at = std::search(data.begin() + static_cast<std::ptrdiff_t>(from), data.end(), marker.begin(), marker.end());
    return at == data.end() ? std::nullopt : std::optional<std::size_t>(static_cast<std::size_t>(at - data.begin()));
}

// A length-prefixed name at `at`, as text; what is past the end of the data is cut off (a name that runs past the end is read as far as it goes).
std::string pascal_name(ByteView data, std::size_t at) {
    if (at >= data.size()) throw TruncatedError("symbols", at, 1, 0);
    const std::size_t length = data[at];
    const std::size_t from = at + 1;
    const std::size_t to = std::min(data.size(), from + length);
    return text::from_latin1(data.subspan(from, to - from));
}

}  // namespace

std::map<core16::SegOff, std::string> codeview(ByteView data) {
    std::map<core16::SegOff, std::string> out;
    std::optional<std::size_t> found = find_marker(data, "NB09", 0);
    while (found && out.empty()) {
        const std::size_t base = *found;
        try {
            io::ByteReader r(data, "codeview");
            r.seek(base + 4);
            const std::uint64_t directory = base + r.u32();
            r.seek(directory);
            const std::uint64_t head = r.u16(), entry = r.u16(), count = r.u32();
            // the directory's entries: a kind of subsection, the module it is of, where it is (from the marker) and how long it is
            for (std::uint64_t i = 0; i < (12 <= entry && entry <= 32 ? count : 0); ++i) {
                r.seek(directory + head + i * entry);
                const std::uint32_t kind = r.u16();
                r.u16();   // the module
                const std::uint64_t offset = r.u32();
                const std::int64_t size = r.i32();
                if (kind != 0x129 && kind != 0x12A && kind != 0x134) continue;   // public symbols, procedure symbols, global symbols
                std::uint64_t s = base + offset + (kind == 0x134 ? 0 : 16);
                const std::uint64_t end = std::min<std::uint64_t>(base + offset + static_cast<std::uint64_t>(std::max<std::int64_t>(size, 0)), data.size());
                while (s + 4 <= end) {
                    r.seek(s);
                    const std::uint32_t length = r.u16(), record = r.u16();
                    if (length < 2) break;
                    std::optional<std::uint64_t> at;
                    if (record == 0x0203) at = s + 4;   // a public symbol
                    else if (record == 0x0204 || record == 0x0205) at = s + 28;   // a procedure
                    if (at) {
                        r.seek(*at);
                        const std::uint32_t symbol_offset = r.u32(), segment = r.u16();
                        const std::string name = pascal_name(data, static_cast<std::size_t>(*at + (record == 0x0203 ? 8 : 9)));
                        if (!name.empty() && segment) out.try_emplace(core16::SegOff{segment, symbol_offset}, name);   // (the first at a place stays)
                    }
                    s += 2 + length;
                }
            }
        } catch (const FormatError&) {
            out.clear();   // damaged: nothing of this one, and the next marker, if any, is tried
        }
        found = find_marker(data, "NB09", base + 4);
    }
    return out;
}

std::optional<std::vector<SymSegment>> read_sym(ByteView data) {
    try {
        io::ByteReader r(data, "sym");
        r.seek(10);
        const std::uint32_t count = r.u16();
        std::uint64_t seg = std::uint64_t{r.u16()} * 16;
        std::vector<SymSegment> out;
        for (std::uint32_t k = 0; k < count; ++k) {
            if (seg == 0 || seg + 21 > data.size()) break;
            r.seek(seg);
            const std::uint32_t next = r.u16(), symbol_count = r.u16(), table = r.u16(), number = r.u16();
            r.seek(seg + 14);
            const bool wide = (r.u8() & 1) != 0;   // a 32-bit symbol file: its values are four bytes
            SymSegment segment;
            segment.number = number;
            for (std::uint32_t i = 0; i < symbol_count; ++i) {
                r.seek(seg + table + 2 * std::uint64_t{i});
                const std::uint64_t at = seg + r.u16();
                if (at + 5 > data.size()) break;
                r.seek(at);
                const std::uint32_t value = wide ? r.u32() : r.u16();
                const std::size_t length_at = static_cast<std::size_t>(at + (wide ? 4 : 2));
                segment.symbols[value] = pascal_name(data, length_at);
            }
            out.push_back(std::move(segment));
            if (std::uint64_t{next} * 16 <= seg) break;
            seg = std::uint64_t{next} * 16;
        }
        return out;
    } catch (const FormatError&) {
        return std::nullopt;
    }
}

std::map<std::uint32_t, std::string> sym_objects(ByteView data) {
    std::map<std::uint32_t, std::string> out;
    try {
        io::ByteReader r(data, "sym");
        r.seek(10);
        const std::uint32_t count = r.u16();
        std::uint64_t seg = std::uint64_t{r.u16()} * 16;
        for (std::uint32_t k = 0; k < count; ++k) {
            if (seg == 0 || seg + 21 > data.size()) break;
            r.seek(seg);
            const std::uint32_t next = r.u16();
            r.skip(4);   // the symbol count and the table's place
            const std::uint32_t number = r.u16();
            const std::string name = pascal_name(data, static_cast<std::size_t>(seg + 20));
            if (!name.empty()) out[number] = name;
            if (std::uint64_t{next} * 16 <= seg) break;
            seg = std::uint64_t{next} * 16;
        }
        return out;
    } catch (const FormatError&) {
        return {};
    }
}

}  // namespace cb::symbols::names
