// cb/symbols/libraries/coff.cpp -- COFF objects and archives (see coff.hpp).
#include "cb/symbols/libraries/coff.hpp"

#include <algorithm>
#include <map>
#include <tuple>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::symbols::libraries::coff {
namespace {

using std::string;
using std::string_view;

constexpr string_view kArchiveMagic = "!<arch>\n";
constexpr std::size_t kMemberHeader = 60;
constexpr std::size_t kSymbolRecord = 18;
constexpr std::size_t kSectionHeader = 40;
constexpr std::uint32_t kPowerPcMinFunction = 8;   // bytes: a symbol that starts less than two instructions is a label

// A name the data holds up to a NUL (or to the end of the data when it has none), read as Latin-1.
string cstring_at(ByteView data, std::uint64_t at) {
    if (at >= data.size()) return {};
    const string_view rest = as_text(data.subspan(static_cast<std::size_t>(at)));
    return text::from_latin1(as_bytes(rest.substr(0, rest.find('\0'))));
}

// The raw data of a section, bounds-checked.
ByteView section_bytes(ByteView data, const Section& s) { return io::checked_range(data, s.ptr, s.size); }

// ---- x86 -----------------------------------------------------------------------------------------------------------

// The external functions of a 32-bit x86 object: name, bytes, byte mask, offset in the section.
std::vector<Function> x86_functions(const Object& obj) {
    struct Code {
        ByteView bytes;
        Bytes masked;   // 1 where a relocation fills the byte
        std::vector<std::tuple<std::uint32_t, string, std::uint8_t>> symbols;   // (offset, name, storage class)
    };
    std::map<int, Code> sections;   // by section number, iterated in order
    int number = 0;
    for (const Section& sec : obj.sections()) {
        ++number;
        if ((sec.chars & kSectionCode) == 0 || sec.size == 0 || sec.ptr == 0) continue;   // holds code
        Code code{section_bytes(obj.data(), sec), {}, {}};
        code.masked.assign(code.bytes.size(), 0);
        for (const Relocation& r : obj.relocations(sec)) {
            const std::uint32_t width = (r.type == 1 || r.type == 2) ? 2 : 4;   // DIR16 / REL16, else a 32-bit field
            for (std::uint32_t k = 0; k < width; ++k) {
                if (r.offset + std::uint64_t{k} < code.masked.size()) code.masked[r.offset + k] = 1;
            }
        }
        sections.emplace(number, std::move(code));
    }
    for (const auto& s : obj.symbols()) {
        // a function: external, or static and typed as a function (jump-table labels are neither)
        if (!s || !sections.contains(s->sec) || !(s->cls == 2 || (s->cls == 3 && s->type == 0x20))) continue;
        sections.at(s->sec).symbols.emplace_back(s->value, s->name, s->cls);
    }
    std::vector<Function> out;
    for (auto& [index, code] : sections) {
        auto& symbols = code.symbols;
        std::sort(symbols.begin(), symbols.end());
        for (std::size_t k = 0; k < symbols.size(); ++k) {
            const auto& [start, name, storage] = symbols[k];
            std::uint64_t end = code.bytes.size();
            for (std::size_t j = k + 1; j < symbols.size(); ++j) {
                if (std::get<0>(symbols[j]) > start) {
                    end = std::get<0>(symbols[j]);
                    break;
                }
            }
            if (storage != 2) continue;
            // (a symbol past the end of its section names no bytes)
            const std::size_t from = std::min<std::uint64_t>(start, code.bytes.size());
            const std::size_t to = std::min<std::uint64_t>(end, code.bytes.size());
            Function f;
            f.name = name;
            f.code.assign(code.bytes.begin() + static_cast<std::ptrdiff_t>(from), code.bytes.begin() + static_cast<std::ptrdiff_t>(to));
            f.mask.assign(code.masked.begin() + static_cast<std::ptrdiff_t>(from), code.masked.begin() + static_cast<std::ptrdiff_t>(to));
            f.at = start;
            out.push_back(std::move(f));
        }
    }
    return out;
}

// ---- PowerPC ---------------------------------------------------------------------------------------------------------

// Every function in a PowerPC object's code sections: name, bytes, the bits left out of each word, offset in the section.
std::vector<Function> powerpc_functions(const Object& obj) {
    std::vector<Function> out;
    int number = 0;
    for (const Section& sec : obj.sections()) {
        ++number;
        if ((sec.chars & kSectionCode) == 0 || sec.size < 8) continue;
        const ByteView code = section_bytes(obj.data(), sec);
        const std::size_t words = code.size() / 4;
        std::vector<std::uint32_t> ignore(words, 0);
        bool skip = false;
        for (const Relocation& r : obj.relocations(sec)) {
            if (skip) {   // a PAIR carries data, not a place
                skip = false;
                continue;
            }
            const std::uint16_t type = r.type & 0xFF;
            if (type == kRelPair) continue;
            if (type == kRelRefHi) skip = true;
            const std::optional<std::uint32_t> bits = field_bits(r.type);
            const std::size_t w = (r.offset & ~std::uint32_t{3}) / 4;
            if (w >= words) continue;
            if (!bits && (r.offset & 3)) {   // an unaligned 32-bit field: two words
                ignore[w] = 0xFFFFFFFFu;
                if (w + 1 < words) ignore[w + 1] = 0xFFFFFFFFu;
            } else {
                ignore[w] |= bits.value_or(0xFFFFFFFFu);
            }
        }
        std::vector<const Symbol*> syms;
        for (const auto& s : obj.symbols()) {
            if (!s || s->sec != number || !(s->cls == 2 || s->cls == 3)) continue;
            if (s->name.starts_with('.') || s->name.starts_with('$')) continue;
            if ((s->type & 0x20) || s->cls == 2) syms.push_back(&*s);
        }
        std::stable_sort(syms.begin(), syms.end(), [](const Symbol* a, const Symbol* b) { return a->value < b->value; });
        for (std::size_t k = 0; k < syms.size(); ++k) {
            const std::uint64_t start = syms[k]->value;
            if (start > code.size()) continue;   // (a symbol past the end of its section names no bytes)
            const std::uint64_t end = k + 1 < syms.size() ? std::min<std::uint64_t>(syms[k + 1]->value, code.size()) : code.size();
            if (end < start + kPowerPcMinFunction || (start & 3)) continue;
            Function f;
            f.name = syms[k]->name;
            f.code.assign(code.begin() + static_cast<std::ptrdiff_t>(start), code.begin() + static_cast<std::ptrdiff_t>(end));
            f.ignore.assign(ignore.begin() + static_cast<std::ptrdiff_t>(start / 4), ignore.begin() + static_cast<std::ptrdiff_t>(end / 4));
            f.at = static_cast<std::uint32_t>(start);
            out.push_back(std::move(f));
        }
    }
    return out;
}

}  // namespace

std::vector<Member> members(ByteView data) {
    std::vector<Member> out;
    if (data.size() < kArchiveMagic.size() || as_text(data.subspan(0, kArchiveMagic.size())) != kArchiveMagic) return out;
    std::size_t at = kArchiveMagic.size();
    ByteView long_names;
    while (at + kMemberHeader <= data.size()) {
        const ByteView head = data.subspan(at, kMemberHeader);
        // the size is the last number of the header (the first 32-bit tools misplaced it)
        string fields(as_text(head.subspan(16, 42)));
        std::replace(fields.begin(), fields.end(), '\0', ' ');
        const auto tokens = text::split_ws(fields);
        const auto size = tokens.empty() ? std::nullopt : text::parse_int<std::uint64_t>(tokens.back());
        if (!size) break;
        const std::size_t body_at = at + kMemberHeader;
        const std::size_t available = data.size() - body_at;
        const ByteView body = data.subspan(body_at, static_cast<std::size_t>(std::min<std::uint64_t>(*size, available)));
        at = *size > available ? data.size() : body_at + static_cast<std::size_t>(*size) + (*size & 1);
        string label(text::trim(as_text(head.subspan(0, 16))));
        if (label == "//") {
            long_names = body;
        } else if (label != "/" && !label.empty()) {
            if (label.size() > 1 && label.starts_with('/') &&
                std::all_of(label.begin() + 1, label.end(), [](char c) { return c >= '0' && c <= '9'; })) {
                const auto index = text::parse_int<std::uint64_t>(string_view(label).substr(1));
                string_view names = index && *index < long_names.size() ? as_text(long_names.subspan(static_cast<std::size_t>(*index))) : string_view();
                names = names.substr(0, names.find('\0'));
                names = names.substr(0, names.find('\n'));
                label = string(names);   // (still the archive's bytes: decoded once, below)
            }
            while (label.ends_with('/')) label.pop_back();
            out.push_back({text::from_latin1(as_bytes(label)), body});
        }
    }
    return out;
}

Object Object::parse(ByteView data) {
    io::ByteReader r(data, "coff object");
    Object obj;
    obj.data_ = data;
    obj.machine_ = r.u16();
    const std::uint32_t section_count = r.u16();
    r.skip(4);   // the time stamp
    obj.symptr_ = r.u32();
    const std::uint32_t symbol_count = r.u32();
    const std::uint32_t optional_size = r.u16();
    r.skip(2);   // the characteristics
    obj.sections_.reserve(section_count);
    for (std::uint32_t i = 0; i < section_count; ++i) {
        r.seek(20ull + optional_size + std::uint64_t{i} * kSectionHeader);
        Section s;
        s.name = string(text::trim(as_text(r.bytes(8)), string_view("\0", 1)));
        r.skip(8);   // the virtual size and address
        s.size = r.u32();
        s.ptr = r.u32();
        s.relptr = r.u32();
        r.skip(4);   // the line numbers' offset
        s.nrel = r.u16();
        r.skip(2);   // their count
        s.chars = r.u32();
        obj.sections_.push_back(std::move(s));
    }
    const std::uint64_t strings = std::uint64_t{obj.symptr_} + std::uint64_t{symbol_count} * kSymbolRecord;
    for (std::uint32_t i = 0; i < symbol_count && obj.symptr_ != 0;) {
        const std::uint64_t at = std::uint64_t{obj.symptr_} + std::uint64_t{i} * kSymbolRecord;
        if (at + kSymbolRecord > data.size()) break;   // (a record cut short: the table ends)
        io::ByteReader rec(data.subspan(static_cast<std::size_t>(at), kSymbolRecord), "coff symbol");
        const ByteView raw_name = rec.bytes(8);
        Symbol s;
        if (raw_name[0] == 0 && raw_name[1] == 0 && raw_name[2] == 0 && raw_name[3] == 0) {   // a long name, in the string table
            io::ByteReader off(raw_name.subspan(4), "coff symbol");
            s.name = cstring_at(data, strings + off.u32());
        } else {
            s.name = text::from_latin1(as_bytes(text::trim(as_text(raw_name), string_view("\0", 1))));
        }
        s.value = rec.u32();
        s.sec = rec.i16();
        s.type = rec.u16();
        s.cls = rec.u8();
        const std::uint8_t aux = rec.u8();
        obj.symbols_.emplace_back(std::move(s));
        obj.symbols_.insert(obj.symbols_.end(), aux, std::nullopt);
        i += 1u + aux;
    }
    return obj;
}

std::vector<Relocation> Object::relocations(const Section& section) const {
    const ByteView table = io::checked_range(data_, section.relptr, std::uint64_t{section.nrel} * 10);
    io::ByteReader r(table, "coff relocations");
    std::vector<Relocation> out;
    out.reserve(section.nrel);
    for (std::uint32_t k = 0; k < section.nrel; ++k) {
        Relocation rel;
        rel.offset = r.u32();
        rel.symbol = r.u32();
        rel.type = r.u16();
        out.push_back(rel);
    }
    return out;
}

ByteView Object::raw_data(const Section& section) const { return section_bytes(data_, section); }

std::optional<std::uint32_t> field_bits(std::uint16_t relocation_type) {
    switch (relocation_type & 0xFF) {
        case kRelRel24:
        case kRelAddr24: return 0x03FFFFFCu;
        case kRelRel14:
        case kRelAddr14: return 0x0000FFFCu;
        case kRelRefHi:
        case kRelRefLo:
        case kRelAddr16:
        case kRelSecRelLo:
        case kRelSecRelHi:
        case kRelGpRel: return 0x0000FFFFu;
        default: return std::nullopt;
    }
}

std::vector<Function> functions(ByteView data) {
    if (data.size() < 20) return {};
    const Object obj = Object::parse(data);
    if (obj.machine() == kI386 && obj.symbol_table_offset() != 0) return x86_functions(obj);
    if (obj.machine() == kPowerPc && !obj.symbols().empty()) return powerpc_functions(obj);
    return {};
}

std::optional<ImportByOrdinal> import_by_ordinal(ByteView member) {
    if (member.size() < 20) return std::nullopt;
    try {
        if (member[0] == 0 && member[1] == 0 && member[2] == 0xFF && member[3] == 0xFF) {   // the short form
            io::ByteReader r(member, "import library member");
            r.seek(16);
            const std::uint32_t hint = r.u16();
            const std::uint32_t kind = r.u16();
            if (((kind >> 2) & 7) != 0) return std::nullopt;   // by name, not by number
            const string_view rest = as_text(member.subspan(20));
            const std::size_t z = rest.find('\0');
            string_view dll;
            if (z != string_view::npos) {
                const string_view after = rest.substr(z + 1);
                dll = after.substr(0, after.find('\0'));
            }
            return ImportByOrdinal{text::from_latin1(as_bytes(dll)), text::from_latin1(as_bytes(rest.substr(0, z))), hint};
        }
        // the long form: a little object file whose import address table slot (.idata$5, or $4) holds the ordinal
        const Object obj = Object::parse(member);
        std::optional<std::uint32_t> ordinal;
        for (const Section& s : obj.sections()) {
            if (s.name != ".idata$5" && s.name != ".idata$4") continue;
            if (s.size < 4) continue;
            io::ByteReader slot(io::checked_range(member, s.ptr, 4), "import library member");
            const std::uint32_t value = slot.u32();
            if (value & 0x80000000u) ordinal = value & 0xFFFF;
        }
        if (!ordinal) return std::nullopt;
        string symbol;
        string dll;
        for (const auto& s : obj.symbols()) {
            if (!s) continue;
            if (s->name.starts_with("__imp_")) symbol = s->name.substr(6);
            else if (s->name.starts_with("__IMPORT_DESCRIPTOR_")) dll = s->name.substr(20);
        }
        if (symbol.empty() || dll.empty()) return std::nullopt;
        return ImportByOrdinal{dll, symbol, *ordinal};
    } catch (const TruncatedError&) {   // a damaged member: nothing is read from it
        return std::nullopt;
    } catch (const FormatError&) {
        return std::nullopt;
    }
}

}  // namespace cb::symbols::libraries::coff
