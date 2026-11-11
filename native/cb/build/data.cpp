#include "cb/build/data.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <unordered_map>
#include <utility>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

namespace cb::build::data {

namespace {

namespace ne = formats::ne;
namespace pe = formats::pe;

// what a section may hold that the linker or the resource compiler makes anew: the data directories of exports,
// imports, resources, relocations and debug information
constexpr std::size_t kRemadeDirectories[] = {0, 1, 2, 5, 6};
constexpr std::uint32_t kCodeCharacteristics = 0x20000020; // code, or executable

// The part of a section a function's code is not counted over: a function runs to the next one, the last 1 MiB on.
constexpr std::uint64_t kLastFunctionSpan = 0x100000;

// A run of zeros this long is written as a ZeroRun.
constexpr std::size_t kMinZeroRun = 64;

// An NE data segment is at most this many bytes.
constexpr std::uint32_t kSegmentLimit = 0x10000;

// a code segment's selector, as each compiler can say it in a constant
constexpr std::string_view kSelectorOf = "#ifdef __WATCOMC__\n"
                                         "#define SELECTOR_OF(symbol, name) ((__segment)&symbol)\n"
                                         "#else\n"
                                         "#define SELECTOR_OF(symbol, name) __segname(name)\n"
                                         "#endif";

// An answer that is not a name: nullopt and "" are the same.
std::optional<std::string> named(std::optional<std::string> answer) {
    if (answer && answer->empty())
        return std::nullopt;
    return answer;
}

std::optional<std::string> name_at(const FunctionAt32& function_at, std::uint64_t va) {
    return function_at ? named(function_at(va)) : std::nullopt;
}
std::optional<std::string> name_at(const Names16& names, Address16 where) {
    return names.function_at ? named(names.function_at(where)) : std::nullopt;
}
std::optional<std::string> symbol_of(const Names16& names, std::uint16_t segment) {
    return names.segment_symbol ? named(names.segment_symbol(segment)) : std::nullopt;
}

std::string join_lines(const std::vector<std::string>& lines) {
    return text::join(lines, "\n");
}

// ---- writing an object ------------------------------------------------------------------------------------------

// The 16 bytes of a row as `0x..,` items and as a comment of the printable ones (not the three characters that would
// end or escape the comment).
std::vector<std::string> rows(ByteView blob) {
    std::vector<std::string> out;
    for (std::size_t i = 0; i < blob.size(); i += 16) {
        const ByteView row = blob.subspan(i, std::min<std::size_t>(16, blob.size() - i));
        std::string items, shown;
        for (const std::uint8_t b : row) {
            items += std::format("0x{:02x},", b);
            shown += (b >= 0x20 && b < 0x7F && b != 0x2A && b != 0x2F && b != 0x5C) ? static_cast<char>(b) : '.';
        }
        out.push_back(std::format("        {:<96}/* {} */", items, shown));
    }
    return out;
}

// `a` and `b` as a name: letters, digits and `_` stay, anything else is `_`.
std::string identifier(std::string_view s) {
    return text::sanitize_identifier(s);
}

// ---- 32-bit -----------------------------------------------------------------------------------------------------

struct Located {
    std::string_view prefix; // `#define NAME (*(TYPE *)`
    std::string_view digits; // the hex digits of the address
};

// A header line `#define NAME (*(TYPE *)0xADDRESS)`: lower-case hex, nothing after the closing parenthesis.
std::optional<Located> global_definition(std::string_view line) {
    constexpr std::string_view kDefine = "#define ";
    if (!line.starts_with(kDefine))
        return std::nullopt;
    std::size_t i = kDefine.size();
    const std::size_t name_start = i;
    while (i < line.size() && (std::isalnum(static_cast<unsigned char>(line[i])) || line[i] == '_'))
        ++i;
    if (i == name_start || line.substr(i, 4) != " (*(")
        return std::nullopt;
    i += 4;
    // the type: anything but parentheses, ending in `*`, then the `)` that closes the cast
    const std::size_t paren = line.find_first_of("()", i);
    if (paren == std::string_view::npos || line[paren] != ')' || paren == i || line[paren - 1] != '*')
        return std::nullopt;
    const std::size_t prefix_end = paren + 1;
    if (line.substr(prefix_end, 2) != "0x")
        return std::nullopt;
    const std::size_t digits_start = prefix_end + 2;
    std::size_t j = digits_start;
    while (j < line.size() && ((line[j] >= '0' && line[j] <= '9') || (line[j] >= 'a' && line[j] <= 'f')))
        ++j;
    if (j == digits_start || j + 1 != line.size() || line[j] != ')')
        return std::nullopt;
    return Located{line.substr(0, prefix_end), line.substr(digits_start, j - digits_start)};
}

// The value of hex digits; nullopt when it does not fit 64 bits.
std::optional<std::uint64_t> hex_value(std::string_view digits) {
    while (digits.size() > 1 && digits.front() == '0')
        digits.remove_prefix(1);
    return text::parse_int<std::uint64_t>(digits, 16);
}

bool is_word_char(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}
bool is_hex_digit(char c) {
    return std::isxdigit(static_cast<unsigned char>(c)) != 0;
}

// The low 32 bits of hex digits of any length.
std::uint32_t low32(std::string_view digits) {
    if (digits.size() > 8)
        digits.remove_prefix(digits.size() - 8);
    return text::parse_int<std::uint32_t>(digits, 16).value_or(0);
}

} // namespace

std::optional<std::string> Data::place(std::uint64_t va) const {
    for (const Part& p : parts) {
        if (p.va <= va && va < p.va + p.size)
            return std::format("((char *)&{} + 0x{:x})", p.symbol, va - p.va);
    }
    return std::nullopt;
}

std::string object(std::string_view struct_name, std::string_view name, std::span<const Piece> pieces, bool far,
                   std::optional<std::string_view> based) {
    // zeros at the very end need no initialiser (when they are not all there is)
    const bool tail = pieces.size() > 1 && std::holds_alternative<ZeroRun>(pieces.back());
    const std::size_t written = tail ? pieces.size() - 1 : pieces.size();
    std::vector<std::string> members, values;
    for (std::size_t i = 0; i < written; ++i) {
        std::visit(
            [&](const auto& p) {
                using T = std::decay_t<decltype(p)>;
                if constexpr (std::is_same_v<T, ByteRun>) {
                    members.push_back(std::format("    uint1 b_{:05x}[{}];", p.offset, p.bytes.size()));
                    values.push_back("    {\n" + join_lines(rows(p.bytes)) + "\n    },");
                } else if constexpr (std::is_same_v<T, ZeroRun>) {
                    members.push_back(std::format("    uint1 z_{:05x}[{}];", p.offset, p.count));
                    values.push_back("    { 0 },");
                } else {
                    members.push_back(std::format("    {} p_{:05x};", p.ctype, p.offset));
                    values.push_back("    " + p.expression + "," + (p.note.empty() ? "" : "    /* " + p.note + " */"));
                }
            },
            pieces[i]);
    }
    if (tail) {
        const ZeroRun& z = std::get<ZeroRun>(pieces.back());
        members.push_back(std::format("    uint1 z_{:05x}[{}];", z.offset, z.count));
    }
    const std::string where = far ? "__far " : (based ? std::format("__based(__segname(\"{}\")) ", *based) : "");
    return std::format(
        "#pragma pack(push, 1)\nstruct {0} {{\n{1}\n}};\n#pragma pack(pop)\nstruct {0} {2}{3} = {{\n{4}\n}};\n",
        struct_name, join_lines(members), where, name, join_lines(values));
}

std::vector<Piece> cut_pieces(ByteView blob, std::uint32_t size, const Pointers& pointers) {
    std::vector<Piece> out;
    std::uint32_t at = 0;

    // bytes lo..hi of the blob (zero-padded past its end): runs of zeros become ZeroRuns
    auto plain = [&](std::uint32_t lo, std::uint32_t hi) {
        if (hi <= lo)
            return;
        Bytes buf(hi - lo, 0);
        if (lo < blob.size()) {
            const std::size_t n = std::min<std::size_t>(hi - lo, blob.size() - lo);
            std::copy_n(blob.begin() + lo, n, buf.begin());
        }
        std::size_t done = 0;
        std::size_t i = 0;
        while (i < buf.size()) {
            if (buf[i] != 0) {
                ++i;
                continue;
            }
            std::size_t end = i;
            while (end < buf.size() && buf[end] == 0)
                ++end;
            if (end - i >= kMinZeroRun) {
                if (i > done)
                    out.push_back(
                        ByteRun{static_cast<std::uint32_t>(lo + done), Bytes(buf.begin() + done, buf.begin() + i)});
                out.push_back(ZeroRun{static_cast<std::uint32_t>(lo + i), static_cast<std::uint32_t>(end - i)});
                done = end;
            }
            i = end;
        }
        if (done < buf.size())
            out.push_back(ByteRun{static_cast<std::uint32_t>(lo + done), Bytes(buf.begin() + done, buf.end())});
    };

    for (const auto& [offset, slot] : pointers) {
        if (offset < at)
            continue; // overlaps the one before: left out
        plain(at, offset);
        out.push_back(PointerCell{offset, slot.ctype, slot.expression, slot.note});
        at = offset + slot.width;
    }
    plain(at, size);
    return out;
}

// ---- 32-bit -----------------------------------------------------------------------------------------------------

RelocatedWords relocated_words(const pe::Image& pe) {
    RelocatedWords out;
    const ByteView data = pe.data();
    for (const std::uint64_t rva : pe.base_relocs()) {
        const auto off = pe.rva_to_offset(rva);
        if (off && *off + 4 <= data.size()) {
            io::ByteReader reader(data, "pe");
            reader.seek(*off);
            out[pe.header().image_base + rva] = reader.u32();
        }
    }
    return out;
}

std::map<std::uint64_t, std::set<std::uint64_t>> address_constants(const pe::Image& pe,
                                                                   std::span<const std::uint64_t> starts_in) {
    const RelocatedWords words = relocated_words(pe);
    std::vector<std::uint64_t> starts(starts_in.begin(), starts_in.end());
    std::sort(starts.begin(), starts.end());
    std::map<std::uint64_t, std::set<std::uint64_t>> out;
    for (const std::uint64_t s : starts)
        out[s];
    for (std::size_t n = 0; n < starts.size(); ++n) {
        const std::uint64_t s = starts[n];
        const std::uint64_t end = n + 1 < starts.size() ? starts[n + 1] : s + kLastFunctionSpan;
        for (auto it = words.lower_bound(s); it != words.end() && it->first < end; ++it)
            out[s].insert(it->second);
    }
    return out;
}

Data data32(const pe::Image& image, std::string_view file_name, const FunctionAt32& function_at,
            const std::map<std::uint64_t, std::string>& named_places) {
    Data d;
    const std::uint64_t base = image.header().image_base;
    const auto sections = image.sections();
    const auto directories = image.data_directories();
    std::set<std::uint32_t> remade;
    for (const std::size_t i : kRemadeDirectories) {
        if (i < directories.size() && directories[i].rva != 0)
            remade.insert(directories[i].rva);
    }
    d.movable = !image.base_relocs().empty();

    std::uint32_t number = 0;
    for (const pe::Section& s : sections) {
        ++number;
        const std::uint32_t size = s.virtual_size != 0 ? std::max(s.virtual_size, s.raw_size) : s.raw_size;
        if ((s.characteristics & kCodeCharacteristics) != 0 || remade.contains(s.virtual_address) || size == 0 ||
            s.name == ".rsrc" || s.name == ".reloc" || s.name == ".idata" || s.name == ".edata") {
            continue;
        }
        const std::uint64_t start = base + s.virtual_address;
        d.sections.push_back({number, s.name, start, size});
        std::vector<std::uint64_t> cuts{start};
        for (const auto& [va, unused] : named_places) {
            if (start < va && va < start + size)
                cuts.push_back(va);
        }
        cuts.push_back(start + size);
        for (std::size_t i = 0; i + 1 < cuts.size(); ++i) {
            const std::uint64_t lo = cuts[i], hi = cuts[i + 1];
            const auto exported = named_places.find(lo);
            std::string symbol =
                exported != named_places.end() && !exported->second.empty()
                    ? exported->second
                    : std::format("codebreaker_data_{}{}", number, lo > start ? std::format("_{:x}", lo - start) : "");
            d.parts.push_back({std::move(symbol), lo, static_cast<std::uint32_t>(hi - lo), number});
        }
    }

    const RelocatedWords words = relocated_words(image);
    std::vector<std::string> out{
        std::format("/* The initialised data of {}: each data section laid out as in the original. A member",
                    text::lower(file_name)),
        " * p_NNNNN is a pointer the original's relocation table marks. */", ""};
    std::vector<std::string> declarations;
    const ByteView file = image.data();
    for (const Part& part : d.parts) {
        const pe::Section& s = sections[part.section - 1];
        const std::uint64_t skip = part.va - (base + s.virtual_address);
        const std::uint64_t first = std::min<std::uint64_t>(s.raw_ptr + skip, file.size());
        const std::uint64_t last = std::min<std::uint64_t>(
            s.raw_ptr + std::max<std::uint64_t>(skip, std::min<std::uint64_t>(s.raw_size, skip + part.size)),
            file.size());
        const ByteView blob = file.subspan(first, last > first ? last - first : 0);

        Pointers pointers;
        const std::uint64_t limit = part.va + blob.size(); // a word must lie wholly in the blob
        for (auto it = words.lower_bound(part.va); it != words.end() && it->first + 4 <= limit; ++it) {
            const std::uint64_t target = it->second;
            const std::uint32_t at = static_cast<std::uint32_t>(it->first - part.va);
            if (const auto fn = name_at(function_at, target)) {
                pointers[at] = {4, "void *", "(void *)" + *fn, ""};
            } else if (const auto place = d.place(target)) {
                pointers[at] = {4, "void *", *place, ""};
            } else {
                d.unresolved.push_back(
                    {std::format("{}+0x{:x}", s.name, it->first - part.va + skip),
                     std::format("points at 0x{:x}, which is neither a reconstructed function nor carried data",
                                 target)});
            }
        }
        d.pointers += pointers.size();
        d.size += part.size;
        declarations.push_back(std::format("struct layout_of_{0}; extern struct layout_of_{0} {0};", part.symbol));
        out.push_back(std::format("/* section {} + 0x{:x}: 0x{:x} bytes at 0x{:x} in the original */", s.name, skip,
                                  part.size, part.va));
        const std::vector<Piece> pieces = cut_pieces(blob, part.size, pointers);
        out.push_back(object("layout_of_" + part.symbol, part.symbol, pieces));
    }
    d.declarations = join_lines(declarations) + "\n";
    d.source = join_lines(out);
    return d;
}

bool layout_holds(ByteView omf_object, const Data& d) {
    formats::omf::Object obj;
    try {
        obj = formats::omf::read(omf_object);
    } catch (const FormatError&) { // the compiler's object cannot be read: the layout is not shown to hold
        return false;
    }
    // name -> (segment, offset): the last definition of a name wins
    std::unordered_map<std::string, const formats::omf::Public*> where;
    for (const formats::omf::Public& p : obj.publics)
        where[p.name] = &p;

    struct First {
        std::optional<std::string> segment;
        std::int64_t offset = 0;
        std::uint64_t start = 0;
    };
    std::map<std::uint32_t, First> first; // section -> its first object
    for (const Part& part : d.parts) {
        const auto it = where.find("_" + part.symbol);
        if (it == where.end())
            return false;
        const formats::omf::Public& at = *it->second;
        const auto [found, inserted] = first.try_emplace(part.section, First{at.segment, at.offset, part.va});
        const First& f = found->second;
        (void)inserted;
        if (at.segment != f.segment || at.offset - f.offset != static_cast<std::int64_t>(part.va - f.start))
            return false;
    }
    return true;
}

Relocated relocate_header(std::string_view header, const Data& d) {
    Relocated out;
    std::string text;
    bool first_line = true;
    for (const std::string_view line : text::split(header, '\n')) {
        if (!first_line)
            text += '\n';
        first_line = false;
        const auto def = global_definition(line);
        const auto value = def ? hex_value(def->digits) : std::nullopt;
        if (!value) {
            text += line;
            continue;
        }
        if (const auto place = d.place(*value)) {
            text += def->prefix;
            text += *place;
            text += ')';
        } else {
            out.lost.push_back(*value);
            text += line;
        }
    }
    out.text = d.declarations + text;
    return out;
}

Relocated relocate_text(std::string_view text, const std::set<std::uint64_t>& addresses, const Data& d,
                        const FunctionAt32& function_at) {
    Relocated out;
    const std::size_t brace = text.find('{');
    if (brace == std::string_view::npos) {
        out.text = std::string(text);
        return out;
    }
    out.text = std::string(text.substr(0, brace + 1));
    const std::string_view body = text.substr(brace + 1);

    // (?<![\w.])(-?)0x([0-9a-fA-F]+)\b, scanned left to right
    std::size_t i = 0;
    while (i < body.size()) {
        const bool boundary_ok = i == 0 || !(is_word_char(body[i - 1]) || body[i - 1] == '.');
        std::size_t p = i;
        const bool negative = boundary_ok && body[p] == '-';
        if (negative)
            ++p;
        std::size_t digits = 0;
        bool matched = false;
        if (boundary_ok && body.substr(p, 2) == "0x") {
            std::size_t q = p + 2;
            while (q < body.size() && is_hex_digit(body[q]))
                ++q;
            digits = q - (p + 2);
            matched = digits > 0 && (q == body.size() || !is_word_char(body[q]));
            if (matched) {
                const std::string_view hex = body.substr(p + 2, digits);
                std::optional<std::uint64_t> value; // nullopt: not a 32-bit address
                if (negative) {
                    value = static_cast<std::uint32_t>(0u - low32(hex));
                } else if (const auto v = hex_value(hex); v && *v <= 0xFFFFFFFFu) {
                    value = *v;
                }
                if (value && addresses.contains(*value)) {
                    if (const auto fn = name_at(function_at, *value)) {
                        out.text += "((int4)" + *fn + ")";
                        i = q;
                        continue;
                    }
                    if (const auto place = d.place(*value)) {
                        out.text += "((int4)" + *place + ")";
                        i = q;
                        continue;
                    }
                    out.lost.push_back(*value);
                }
                out.text.append(body.substr(i, q - i));
                i = q;
                continue;
            }
        }
        out.text += body[i];
        ++i;
    }
    return out;
}

// ---- 16-bit -----------------------------------------------------------------------------------------------------

std::vector<RelocationPlace> places(std::span<const ne::Relocation> relocations, ByteView blob) {
    std::vector<RelocationPlace> out;
    for (std::size_t n = 0; n < relocations.size(); ++n) {
        const ne::Relocation& r = relocations[n];
        std::uint32_t off = r.offset;
        std::uint32_t hops = 0;
        while (off != 0xFFFF && off + 1 < blob.size() && hops < 0x10000) {
            const std::uint16_t word = static_cast<std::uint16_t>(blob[off] | (blob[off + 1] << 8));
            out.push_back(
                {static_cast<std::uint16_t>(off), r.source, static_cast<std::uint16_t>(r.additive() ? word : 0), n});
            if (r.additive())
                break;
            off = word;
            ++hops;
        }
    }
    return out;
}

namespace {

std::uint16_t word_at(ByteView b, std::size_t at) {
    return static_cast<std::uint16_t>(b[at] | (b[at + 1] << 8));
}

// The declaration, in data.c, of another data segment the build carries: an object of its own layout (far_segments),
// named as the code knows it.
std::string segment_declaration(std::uint16_t n) {
    return std::format("struct codebreaker_segment_{0}_t;\nextern struct codebreaker_segment_{0}_t "
                       "__based(__segname(\"CODEBREAKER_SEGMENT_{0}\")) codebreaker_segment_{0};",
                       n);
}

// Pieces cut so that each offset starts one.
std::vector<Piece> cut_at(const std::vector<Piece>& pieces, const std::vector<std::uint32_t>& offsets) {
    std::vector<Piece> out;
    for (const Piece& piece : pieces) {
        if (const auto* run = std::get_if<ByteRun>(&piece)) {
            std::vector<std::uint32_t> bounds{run->offset};
            for (const std::uint32_t c : offsets) {
                if (run->offset < c && c < run->offset + run->bytes.size())
                    bounds.push_back(c);
            }
            bounds.push_back(static_cast<std::uint32_t>(run->offset + run->bytes.size()));
            for (std::size_t i = 0; i + 1 < bounds.size(); ++i) {
                out.push_back(ByteRun{bounds[i], Bytes(run->bytes.begin() + (bounds[i] - run->offset),
                                                       run->bytes.begin() + (bounds[i + 1] - run->offset))});
            }
        } else if (const auto* zeros = std::get_if<ZeroRun>(&piece)) {
            std::vector<std::uint32_t> bounds{zeros->offset};
            for (const std::uint32_t c : offsets) {
                if (zeros->offset < c && c < zeros->offset + zeros->count)
                    bounds.push_back(c);
            }
            bounds.push_back(zeros->offset + zeros->count);
            for (std::size_t i = 0; i + 1 < bounds.size(); ++i)
                out.push_back(ZeroRun{bounds[i], bounds[i + 1] - bounds[i]});
        } else {
            out.push_back(piece);
        }
    }
    return out;
}

// The offset of a piece.
std::uint32_t offset_of(const Piece& piece) {
    return std::visit([](const auto& p) { return p.offset; }, piece);
}

struct SegmentFixups {
    Pointers pointers;
    std::vector<std::string> externs;
};

// The fix-ups of segment `s` (its file bytes in `blob`, cleared where the loader writes): the places that are pointers,
// and the imports named. The same for the automatic data segment and for each other data segment the build carries.
// `own` is the number of the segment itself when it is not the automatic one.
SegmentFixups relocated(const ne::Module& module, const ne::Segment& s, Bytes& blob,
                        std::optional<std::uint16_t> auto_segment, const std::set<std::uint16_t>& far_segments,
                        const Names16& names, Data& d, std::string_view label, std::optional<std::uint16_t> own) {
    std::map<std::uint32_t, ne::Entry> entries; // by ordinal: the last of a repeated ordinal wins
    for (const ne::Entry& e : module.entries()) {
        if (e.kind != ne::Entry::Kind::Unused)
            entries[e.ordinal] = e;
    }
    const std::vector<std::string> refs = module.module_refs();
    SegmentFixups result;
    Pointers& pointers = result.pointers;

    const std::vector<ne::Relocation> relocations = module.relocations(s);
    const std::vector<RelocationPlace> all = places(relocations, blob);
    std::set<std::uint16_t> relocated_offsets;
    for (const RelocationPlace& p : all)
        relocated_offsets.insert(p.offset);
    const Bytes original = blob;

    auto is_auto = [&](std::uint16_t seg) { return auto_segment && seg == *auto_segment; };
    auto is_far = [&](std::uint16_t seg) { return far_segments.contains(seg); };

    for (const RelocationPlace& place : all) {
        const ne::Relocation& r = relocations[place.record];
        const std::uint16_t off = place.offset;
        const std::uint8_t src = place.source;
        const std::uint16_t add = place.addend;
        const std::size_t width = src == 3 ? 4 : src == 0 ? 1 : 2;
        // the file keeps a chain link there, not a value (clamped to the bytes there are)
        for (std::size_t i = off; i < off + width && i < blob.size(); ++i)
            blob[i] = 0;
        const std::string where = std::format("{}+0x{:04x}", label, off);

        if (const auto* ord = std::get_if<ne::ImportOrdinalRef>(&r.target)) {
            const std::string mod = 0 < ord->module && ord->module <= refs.size() ? refs[ord->module - 1] : "?";
            const std::string symbol = std::format("{}_{}", identifier(mod), ord->ordinal);
            d.imports[symbol] = {mod, ord->ordinal};
            if (src == 3 && add == 0) {
                pointers[off] = {4, "void __far *", "(void __far *)" + symbol, ""};
                result.externs.push_back(symbol);
            } else if (src == 5 && add == 0) { // its offset: KERNEL's __WINFLAGS, a value
                pointers[off] = {2, "uint2", "(uint2)" + symbol, ""};
                result.externs.push_back(symbol);
            } else if (src == 2) { // its selector (KERNEL's __0040H)
                pointers[off] = {2, "__segment", "(__segment)" + symbol, ""};
                result.externs.push_back(symbol);
            } else {
                d.unresolved.push_back({where, "part of the address of " + symbol});
            }
            continue;
        }
        if (const auto* by_name = std::get_if<ne::ImportNameRef>(&r.target)) {
            const std::string mod =
                0 < by_name->module && by_name->module <= refs.size() ? refs[by_name->module - 1] : "?";
            const std::string imported = module.imported_name(by_name->name_offset);
            const std::string symbol = identifier(imported);
            d.imports[symbol] = {mod, imported};
            if (src == 3 && add == 0) {
                pointers[off] = {4, "void __far *", "(void __far *)" + symbol, ""};
                result.externs.push_back(symbol);
            } else if (src == 5 && add == 0) {
                pointers[off] = {2, "uint2", "(uint2)" + symbol, ""};
                result.externs.push_back(symbol);
            } else if (src == 2) {
                pointers[off] = {2, "__segment", "(__segment)" + symbol, ""};
                result.externs.push_back(symbol);
            } else {
                d.unresolved.push_back({where, "part of the address of " + symbol});
            }
            continue;
        }
        const auto* internal = std::get_if<ne::InternalRef>(&r.target);
        if (internal == nullptr) {
            d.unresolved.push_back({where, "a floating-point fixup"});
            continue;
        }
        std::uint16_t target_segment = 0, target_offset = 0;
        if (internal->segment == 0xFF) {
            const auto e = entries.find(internal->entry);
            if (e == entries.end()) {
                d.unresolved.push_back(
                    {where, std::format("names entry {}, which the module does not have", internal->entry)});
                continue;
            }
            target_segment = e->second.segment;
            target_offset = e->second.offset;
        } else {
            target_segment = internal->segment;
            target_offset = internal->entry;
        }
        target_offset = static_cast<std::uint16_t>(target_offset + add);
        const std::optional<std::string> fn = name_at(names, {target_segment, target_offset});

        // A selector of a code segment right after a word that is a function's offset there: a far pointer to that
        // function whose offset the linker wrote and whose selector the loader does (SETUPX's table of section
        // handlers, DS:0F66: 4:52D2, 4:55D2, 4:56F8). Written as the pointer to the function by name, the linker writes
        // its offset where the function is now -- as a number it stayed the original's, and when the function moved,
        // GenInstallEx called whatever was put in its place (Setup: SU99414 at "Preparing to copy files").
        const bool word_before = off >= 2 && !relocated_offsets.contains(static_cast<std::uint16_t>(off - 2)) &&
                                 !pointers.contains(static_cast<std::uint32_t>(off - 2));
        const std::uint32_t before = off >= 2 ? off - 2u : 0;
        if (src == 2 && word_before && !is_auto(target_segment) && !(own && target_segment == *own)) {
            const std::uint16_t offset_there = word_at(original, before);
            if (const auto function = name_at(names, {target_segment, offset_there})) {
                pointers.erase(off);
                pointers[before] = {4, "void __far *", "(void __far *)" + *function,
                                    std::format("{}:{:04X}", target_segment, offset_there)};
                continue;
            }
        }
        // The same for a selector of a data segment the build carries: with the word before it, a far pointer into that
        // segment -- the same bytes either way (the segment's object starts at its offset 0), and a constant Microsoft
        // C takes too, where a selector alone is not
        if (src == 2 && word_before && (is_auto(target_segment) || is_far(target_segment))) {
            const std::uint16_t word = word_at(original, before);
            const std::string base = is_auto(target_segment)
                                         ? "(char __far *)&codebreaker_data"
                                         : std::format("(uint1 __far *)&codebreaker_segment_{}", target_segment);
            pointers.erase(off);
            pointers[before] = {4, "void __far *", std::format("(void __far *)({} + 0x{:x})", base, word), ""};
            continue;
        }
        if (src == 5 && (is_auto(target_segment) ||
                         (own && target_segment == *own))) { // an offset into this very segment: a number
            blob[off] = static_cast<std::uint8_t>(target_offset & 0xFF);
            blob[off + 1] = static_cast<std::uint8_t>(target_offset >> 8);
        } else if (src == 3 && fn) {
            // (as void __far *: the same four bytes, and a constant Microsoft C takes for a __pascal function as well,
            // where it refused a cast to a pointer to a cdecl function)
            pointers[off] = {4, "void __far *", "(void __far *)" + *fn, ""};
        } else if (src == 3 && is_auto(target_segment)) {
            pointers[off] = {4, "void __far *",
                             std::format("(void __far *)((char *)&codebreaker_data + 0x{:x})", target_offset), ""};
        } else if (src == 3 && is_far(target_segment)) {
            pointers[off] = {4, "void __far *",
                             std::format("(void __far *)((uint1 __far *)&codebreaker_segment_{} + 0x{:x})",
                                         target_segment, target_offset),
                             ""};
        } else if (src == 2 && is_auto(target_segment)) { // a segment's selector, which the loader writes: its own,
            pointers[off] = {2, "__segment", "(__segment)&codebreaker_data", ""};
        } else if (src == 2 && is_far(target_segment)) { // a segment carried,
            pointers[off] = {2, "__segment", std::format("(__segment)&codebreaker_segment_{}", target_segment), ""};
        } else if (const auto symbol = src == 2 ? symbol_of(names, target_segment) : std::nullopt) { // a code segment's
            // (OpenWatcom: the selector of a symbol in that segment; Microsoft C has no such constant, but names the
            // segment -- CODE<n>, as /NT CODE<n> builds it)
            pointers[off] = {2, "__segment", std::format("SELECTOR_OF({}, \"CODE{}\")", *symbol, target_segment),
                             std::format("the selector of segment {}", target_segment)};
        } else {
            const std::string what = src == 5 ? std::format("an offset in segment {}", target_segment)
                                     : src == 3
                                         ? std::format("a far pointer to {}:{:04x}", target_segment, target_offset)
                                         : std::format("a fixup of kind {}", src);
            d.unresolved.push_back({where, what + ", which has no C form here"});
        }
    }
    return result;
}

// `extern uint4 __far __stdcall NAME();` for each import symbol a pointer names, once, in name order.
std::vector<std::string> extern_declarations(const std::vector<std::string>& externs) {
    const std::set<std::string> unique(externs.begin(), externs.end());
    std::vector<std::string> out;
    for (const std::string& x : unique)
        out.push_back("extern uint4 __far __stdcall " + x + "();");
    return out;
}

// The numbers of the segments the C text names as `codebreaker_segment_N`.
std::set<std::uint16_t> named_segments(const std::string& text) {
    constexpr std::string_view kStem = "codebreaker_segment_";
    std::set<std::uint16_t> out;
    for (std::size_t at = text.find(kStem); at != std::string::npos; at = text.find(kStem, at + 1)) {
        if (at > 0 && is_word_char(text[at - 1]))
            continue;
        std::size_t end = at + kStem.size();
        while (end < text.size() && text[end] >= '0' && text[end] <= '9')
            ++end;
        if (end == at + kStem.size() || (end < text.size() && is_word_char(text[end])))
            continue;
        if (const auto n = text::parse_int<std::uint16_t>(
                std::string_view(text).substr(at + kStem.size(), end - at - kStem.size())))
            out.insert(*n);
    }
    return out;
}

} // namespace

Data data16(const ne::Module& module, const Names16& names, const std::set<std::uint16_t>& far_segments) {
    Data d;
    const std::uint16_t auto_segment = module.header().autodata_segment;
    const std::vector<ne::Segment> segments = module.segments();
    if (auto_segment == 0 || auto_segment > segments.size()) {
        d.source =
            std::format("/* {} has no automatic data segment. */\nchar codebreaker_data = 1;\n", module.module_name());
        return d;
    }
    const ne::Segment& s = segments[auto_segment - 1];
    const ByteView file = module.data();
    Bytes blob;
    if (s.file_offset != 0 && s.file_offset < file.size()) {
        const std::uint64_t end = std::min<std::uint64_t>(s.file_offset + s.length, file.size());
        blob.assign(file.begin() + s.file_offset, file.begin() + end);
    }
    const std::uint32_t size = s.file_offset != 0 ? std::max<std::uint32_t>(static_cast<std::uint32_t>(blob.size()),
                                                                            std::min(s.min_alloc, kSegmentLimit))
                                                  : std::min(s.min_alloc, kSegmentLimit);

    SegmentFixups fixups = relocated(module, s, blob, auto_segment, far_segments, names, d, "data", std::nullopt);
    d.pointers = fixups.pointers.size();
    d.size = size;

    std::vector<std::string> head{
        std::format("/* The automatic data segment of {}: 0x{:x} bytes (0x{:x} of them in the file).",
                    module.module_name(), size, blob.size()),
        " * It must be the first thing linked into the data segment: the code addresses it by offset. */"};
    for (std::string& x : extern_declarations(fixups.externs))
        head.push_back(std::move(x));
    head.emplace_back(kSelectorOf);

    // data the module exports (KERNEL's THHOOK): the segment is cut there, and from there on it is a variable of the
    // export's own name -- the parts follow one another in the segment
    std::map<std::uint32_t, std::string> export_names; // ordinal -> name; the non-resident names win
    for (const std::vector<ne::Name>& table : {module.resident_names(), module.nonresident_names()}) {
        for (const ne::Name& n : table) {
            if (n.ordinal != 0)
                export_names[n.ordinal] = n.name;
        }
    }
    std::map<std::uint32_t, std::string> cuts; // offset -> name; of two exports at one offset the higher ordinal wins
    std::map<std::uint32_t, ne::Entry> entries;
    for (const ne::Entry& e : module.entries()) {
        if (e.kind != ne::Entry::Kind::Unused)
            entries[e.ordinal] = e;
    }
    for (const auto& [ordinal, e] : entries) {
        if (e.segment == auto_segment && 0 < e.offset && e.offset < size) {
            const auto name = export_names.find(ordinal);
            cuts[e.offset] = name != export_names.end() ? name->second : std::format("export_{}", ordinal);
        }
    }
    std::vector<std::uint32_t> cut_offsets;
    for (const auto& [offset, name] : cuts)
        cut_offsets.push_back(offset);
    d.pieces = cut_at(cut_pieces(blob, size, fixups.pointers), cut_offsets);
    for (const auto& [offset, name] : cuts)
        d.cuts[offset] = identifier(name);

    std::vector<std::string> parts;
    std::uint32_t start = 0;
    cut_offsets.push_back(size);
    for (const std::uint32_t at : cut_offsets) {
        std::vector<Piece> part;
        for (const Piece& piece : d.pieces) {
            if (start <= offset_of(piece) && offset_of(piece) < at)
                part.push_back(piece);
        }
        const std::string name = start == 0 ? "codebreaker_data" : identifier(cuts.at(start));
        if (!part.empty()) {
            parts.push_back(object(std::format("codebreaker_data_{}", start != 0 ? name : "segment"), name, part));
            if (start != 0)
                d.exported[{auto_segment, static_cast<std::uint16_t>(start)}] = name;
        }
        start = at;
    }
    // (a far pointer into one of the module's other data segments names its bytes: declared here, not left to the C's
    // header -- with no C function using segment 41, SETUPX's data did not compile, and the build fell back to a data
    // segment of one byte)
    for (const std::uint16_t n : named_segments(join_lines(parts)))
        head.push_back(segment_declaration(n));
    d.source = join_lines(head) + "\n" + join_lines(parts);
    return d;
}

std::string far_segments(const std::map<std::uint16_t, Bytes>& segments, const ne::Module* module,
                         const Names16& names) {
    std::map<std::uint16_t, ne::Segment> of_module;
    std::optional<std::uint16_t> auto_segment;
    if (module != nullptr) {
        for (const ne::Segment& s : module->segments())
            of_module[s.index] = s;
        auto_segment = module->header().autodata_segment;
    }
    std::set<std::uint16_t> carried;
    for (const auto& [n, image] : segments)
        carried.insert(n);

    std::vector<std::string> out;
    std::vector<std::string> externs;
    Data scratch;
    for (const auto& [n, image] : segments)
        out.push_back(segment_declaration(n));
    for (const auto& [n, image] : segments) {
        const auto s = of_module.find(n);
        const auto size = static_cast<std::uint32_t>(image.size());
        Pointers pointers;
        Bytes blob;
        if (module != nullptr && s != of_module.end() && s->second.file_offset != 0) {
            const ByteView file = module->data();
            if (s->second.file_offset < file.size()) {
                const std::uint64_t end =
                    std::min<std::uint64_t>(s->second.file_offset + s->second.length, file.size());
                blob.assign(file.begin() + s->second.file_offset, file.begin() + end);
            }
            if (blob.size() < size)
                blob.resize(size, 0);
            SegmentFixups fixups =
                relocated(*module, s->second, blob, auto_segment, carried, names, scratch, std::format("seg{}", n), n);
            pointers = std::move(fixups.pointers);
            externs.insert(externs.end(), fixups.externs.begin(), fixups.externs.end());
        } else {
            blob = image;
        }
        blob.resize(std::min<std::size_t>(blob.size(), size));
        out.push_back(object(std::format("codebreaker_segment_{}_t", n), std::format("codebreaker_segment_{}", n),
                             cut_pieces(blob, size, pointers), false, std::format("CODEBREAKER_SEGMENT_{}", n)));
    }
    std::vector<std::string> all = extern_declarations(externs);
    all.insert(all.end(), out.begin(), out.end());
    return join_lines(all) + (out.empty() ? "" : "\n");
}

} // namespace cb::build::data
