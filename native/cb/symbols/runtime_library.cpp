#include "cb/symbols/runtime_library.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>

#include "cb/compare/mask.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/symbols/libraries/package.hpp"
#include "cb/symbols/x86.hpp"

namespace cb::symbols::runtime_library {

namespace {

namespace rl_text = cb::text;

std::string sha1_prefix(ByteView bytes) { return crypto::to_hex(crypto::sha1(bytes)).substr(0, 16); }

std::string lead_key(ByteView bytes) { return std::string(as_text(bytes)); }

}  // namespace

// ---- signatures ---------------------------------------------------------------------------------------------------

std::optional<Signature> signature(ByteView code, ByteView mask) {
    const std::size_t size = std::min(code.size(), kLongest);
    std::vector<std::uint32_t> masked;
    for (std::size_t o = 0; o < std::min(size, mask.size()); ++o) {
        if (mask[o] != 0) masked.push_back(static_cast<std::uint32_t>(o));
    }
    if (size - masked.size() < kFewest) return std::nullopt;
    const Bytes body = compare::mask::blank(code.first(size), masked);
    std::array<bool, 256> seen{};
    std::size_t distinct = 0;
    for (const std::uint8_t b : body) {
        if (!seen[b]) {
            seen[b] = true;
            ++distinct;
        }
    }
    if (distinct < 4) return std::nullopt;   // padding, a table of zeros
    Signature sig;
    sig.size = size;
    std::size_t i = 0;
    while (i < masked.size()) {
        std::size_t j = i;
        while (j + 1 < masked.size() && masked[j + 1] == masked[j] + 1) ++j;
        if (!sig.mask_text.empty()) sig.mask_text += ',';
        sig.mask_text += std::format("{:x}.{:x}", masked[i], j - i + 1);
        i = j + 1;
    }
    sig.digest = sha1_prefix(body);
    return sig;
}

std::vector<std::uint32_t> mask_offsets(std::string_view mask_text) {
    std::vector<std::uint32_t> out;
    if (mask_text.empty()) return out;
    for (const std::string_view run : rl_text::split(mask_text, ',')) {
        const std::size_t dot = run.find('.');
        const auto start = dot == std::string_view::npos ? std::nullopt : rl_text::parse_int<std::uint32_t>(run.substr(0, dot), 16);
        const auto length = dot == std::string_view::npos ? std::nullopt : rl_text::parse_int<std::uint32_t>(run.substr(dot + 1), 16);
        if (!start || !length || *length > kLongest || *start > kLongest) {
            throw ParseError("signature mask", 0, 0, std::format("\"{}\" is not a run of offset.length", run));
        }
        for (std::uint32_t k = 0; k < *length; ++k) out.push_back(*start + k);
    }
    return out;
}

SignatureTable SignatureTable::from_rows(std::span<const SignatureRow> rows) {
    SignatureTable table;
    for (const SignatureRow& row : rows) {
        Entry e;
        e.size = row.size;
        e.masked = mask_offsets(row.mask_text);
        e.digest = row.digest;
        e.what = {row.name, row.library};
        Bytes lead;
        try {
            lead = crypto::from_hex(row.lead_hex);
        } catch (const FormatError& err) {
            throw FormatError("runtime signatures", 0, std::format("lead \"{}\": {}", row.lead_hex, err.what()));
        }
        table.by_lead_[lead_key(lead)].push_back(std::move(e));
        ++table.rows_;
    }
    return table;
}

std::optional<Identified> SignatureTable::match(ByteView code) const {
    for (std::size_t n = std::min(kLead, code.size()); n > 0; --n) {
        const auto hit = by_lead_.find(lead_key(code.first(n)));
        if (hit == by_lead_.end()) continue;
        for (const Entry& e : hit->second) {
            if (e.size > code.size()) continue;
            if (sha1_prefix(compare::mask::blank(code.first(e.size), e.masked)) == e.digest) return e.what;
        }
    }
    return std::nullopt;
}

// ---- identifying ----------------------------------------------------------------------------------------------------

std::string c_name(std::string_view symbol) {
    if (symbol.starts_with('?')) return std::string(symbol);
    std::string_view name = symbol.starts_with('_') ? symbol.substr(1) : symbol;
    const std::size_t last = name.rfind('@');
    if (last != std::string_view::npos && last + 1 < name.size() &&
        std::all_of(name.begin() + static_cast<std::ptrdiff_t>(last) + 1, name.end(), [](char c) { return c >= '0' && c <= '9'; })) {
        // `_f@8` is stdcall, `@f@8` fastcall: the name is what is before the count (after a fastcall name's leading @)
        const std::size_t from = name.starts_with('@') ? 1 : 0;
        return std::string(name.substr(from, name.find('@', from) - from));
    }
    return std::string(name);
}

const std::map<std::string, std::string, std::less<>>& runtime_libraries() {
    static const std::map<std::string, std::string, std::less<>> table = [] {
        std::map<std::string, std::string, std::less<>> t;
        for (const char* k : {"CRTDLL", "MSVCRT", "MSVCRT10", "MSVCRT20", "MSVCRT40"}) {
            t[k] = "C runtime (this module is the runtime library)";
        }
        t["MSVCIRT"] = "C++ stream runtime (this module is the runtime library)";
        const std::string_view classes = " (this module is the class library)";
        t["MFC30"] = std::string("MFC 3.0") + std::string(classes);
        t["MFCO30"] = std::string("MFC 3.0 OLE") + std::string(classes);
        t["MFCD30"] = std::string("MFC 3.0 database") + std::string(classes);
        t["MFCN30"] = std::string("MFC 3.0 sockets") + std::string(classes);
        t["MFC40"] = std::string("MFC 4.0") + std::string(classes);
        t["MFCO40"] = std::string("MFC 4.0 OLE") + std::string(classes);
        t["MFC250"] = std::string("MFC 2.5") + std::string(classes);
        return t;
    }();
    return table;
}

const std::string* runtime_library_of(std::string_view stem) {
    const auto& table = runtime_libraries();
    const auto it = table.find(stem);
    return it == table.end() ? nullptr : &it->second;
}

namespace {

// How far a stub's code is followed to see whether it calls through a thunk pointer.
constexpr std::size_t kStubWindow = 512;

const CodeRegion* region_of(std::span<const CodeRegion> code, std::uint64_t address) {
    for (const CodeRegion& r : code) {
        if (r.contains(address)) return &r;
    }
    return nullptr;
}

// Whether the code at `va`, followed through `hops` more jumps, calls through a pointer kept in the code itself.
bool reaches_thunk_pointer(std::span<const CodeRegion> code, const platforms::Decoder& cs, std::uint64_t va, int hops) {
    const CodeRegion* r = region_of(code, va);
    if (r == nullptr) return false;
    const std::size_t from = static_cast<std::size_t>(va - r->va);
    const ByteView window = r->bytes.subspan(from, std::min(kStubWindow, r->bytes.size() - from));
    constexpr std::string_view kPointer = "dword ptr [0x";
    for (const x86::Insn& ins : x86::decode(cs, window, va)) {
        if (ins.mnemonic == "call" && ins.op_str.starts_with(kPointer) && ins.op_str.ends_with(']')) {
            const auto slot = rl_text::parse_int<std::uint64_t>(
                std::string_view(ins.op_str).substr(kPointer.size() - 2, ins.op_str.size() - kPointer.size() + 1), 16);
            if (slot && region_of(code, *slot) != nullptr) return true;
        }
        if (ins.mnemonic == "jmp" && ins.op_str.starts_with("0x")) {
            // (a short stub sets up and jumps to the common thunk code: `mov cl, 3 / jmp`)
            const auto target = rl_text::parse_int<std::uint64_t>(ins.op_str, 16);
            return hops > 0 && target && reaches_thunk_pointer(code, cs, *target, hops - 1);
        }
        if (ins.mnemonic == "ret" || ins.mnemonic == "retn" || ins.mnemonic == "int3") return false;
    }
    return false;
}

}  // namespace

std::vector<std::uint64_t> flat_thunk_stubs(std::span<const CodeRegion> code, std::span<const std::uint64_t> starts,
                                            platforms::DecoderSet& decoders) {
    const platforms::Decoder& cs = decoders.get(platforms::Arch::X86, 32, false);
    std::vector<std::uint64_t> out;
    for (const std::uint64_t va : starts) {
        if (reaches_thunk_pointer(code, cs, va, 1)) out.push_back(va);
    }
    return out;
}

std::map<std::uint64_t, Identified> identify_pe(const SignatureTable& table, const Pe32Module& module,
                                                platforms::DecoderSet& decoders) {
    std::map<std::uint64_t, Identified> out;
    for (const std::uint64_t va : module.function_starts) {
        if (const CodeRegion* r = region_of(module.code, va)) {
            const std::size_t from = static_cast<std::size_t>(va - r->va);
            const ByteView code = r->bytes.subspan(from, std::min(kLongest, r->bytes.size() - from));
            if (auto hit = table.match(code)) out[va] = std::move(*hit);
        }
    }
    if (module.flat_thunk) {
        for (const std::uint64_t va : flat_thunk_stubs(module.code, module.function_starts, decoders)) {
            out.try_emplace(va, Identified{"thunk stub", std::string(kThunkCompiler)});
        }
    }
    if (const std::string* library = runtime_library_of(module.stem)) {
        for (const PeExport& e : module.exports) {
            std::string name = e.name.value_or("");
            if (name.empty()) {
                const auto named = module.ordinal_names.find(e.ordinal);
                if (named != module.ordinal_names.end()) name = named->second;
            }
            const std::uint64_t at = module.image_base + e.rva;
            if (!name.empty() && region_of(module.code, at) != nullptr) out.try_emplace(at, Identified{std::move(name), *library});
        }
    }
    return out;
}

std::map<SegOff, Identified> identify_ne(const SignatureTable& table, const Ne16Module& module) {
    std::map<SegOff, Identified> out;
    std::map<std::uint16_t, const Ne16Segment*> segments;
    for (const Ne16Segment& s : module.segments) segments[s.index] = &s;
    for (const SegOff& where : module.function_starts) {
        const auto s = segments.find(where.segment);
        if (s == segments.end()) throw FormatError("ne function", where.segment, "a function starts in a segment the module does not have");
        const ByteView bytes = s->second->bytes;
        const std::size_t from = std::min<std::size_t>(where.offset, bytes.size());
        const std::size_t to = std::min<std::size_t>(bytes.size(), static_cast<std::size_t>(where.offset) + kLongest);
        if (auto hit = table.match(bytes.subspan(from, to - from))) out[where] = std::move(*hit);
    }
    if (const std::string* library = runtime_library_of(rl_text::upper(module.module_name))) {
        for (const Ne16Entry& e : module.entries) {
            const auto s = segments.find(e.segment);
            if (s == segments.end() || !s->second->code) continue;
            const auto named = module.ordinal_names.find(e.ordinal);
            if (named == module.ordinal_names.end() || named->second.empty()) continue;
            out.try_emplace(SegOff{e.segment, e.offset}, Identified{named->second, *library});
        }
    }
    return out;
}

// ---- which compiler ---------------------------------------------------------------------------------------------------

namespace {

struct Version {
    std::uint32_t major, minor;
    friend auto operator<=>(const Version&, const Version&) = default;
};

// What the PE linker's version says.
std::string_view linker32(Version v) {
    static const std::map<Version, std::string_view> t = {
        {{2, 50}, "Microsoft Visual C++ 2.x"},      {{2, 55}, "Microsoft Visual C++ 2.x"},
        {{2, 60}, "Microsoft Visual C++ 2.x"},      {{3, 0}, "Microsoft Visual C++ 4.0"},
        {{3, 10}, "Microsoft Visual C++ 4.0/4.1"},  {{4, 20}, "Microsoft Visual C++ 4.2"},
        {{5, 0}, "Microsoft Visual C++ 5.0"},       {{5, 10}, "Microsoft Visual C++ 5.0"},
        {{5, 12}, "Microsoft Visual C++ 5.0"},      {{6, 0}, "Microsoft Visual C++ 6.0"},
        {{2, 25}, "Borland C++ (TLINK32)"}};
    const auto it = t.find(v);
    return it == t.end() ? std::string_view() : it->second;
}

// 16-bit: the segmented linker's version says which generation of Microsoft's tools linked the module (the compiler or
// assembler itself is not recorded in the file).
std::string_view linker16(Version v) {
    static const std::map<Version, std::string_view> t = {
        {{5, 10}, "Microsoft tools of the C 6.0 generation"},
        {{5, 13}, "Microsoft tools of the C 6.0 generation"},
        {{5, 15}, "Microsoft tools of the C 6.0 generation"},
        {{5, 30}, "Microsoft tools of the C/C++ 7.0 generation"},
        {{5, 50}, "Microsoft tools of the Visual C++ 1.0 generation (16-bit)"},
        {{5, 60}, "Microsoft tools of the Visual C++ 1.5 generation (16-bit)"}};
    const auto it = t.find(v);
    return it == t.end() ? std::string_view() : it->second;
}

struct Mark {
    std::string_view text;
    std::string_view tool;
};
constexpr std::array<Mark, 7> kMarks = {{{"Borland C++ - Copyright", "Borland C++"},
                                         {"Borland Intl", "Borland C++"},
                                         {"WATCOM C/C++", "Watcom C/C++"},
                                         {"Microsoft Visual C++ Runtime Library", "Microsoft Visual C++"},
                                         {"MS Run-Time Library", "Microsoft C"},
                                         {"Microsoft (R) C ", "Microsoft C"},
                                         {"__CxxFrameHandler", "Microsoft Visual C++ (C++ exception handling)"}}};

std::string_view runtime_dll(std::string_view stem) {
    static const std::map<std::string, std::string_view, std::less<>> t = {
        {"CRTDLL", "the system C runtime (CRTDLL)"},
        {"MSVCRT20", "Visual C++ 2.x's runtime DLL"},
        {"MSVCRT40", "Visual C++ 4.x's runtime DLL"},
        {"MSVCRT", "Visual C++ 4.2 or later's runtime DLL"},
        {"MSVCRT10", "Visual C++ 1.0 (32-bit)'s runtime DLL"},
        {"MFC30", "MFC 3.0 (Visual C++ 2.x)"},
        {"MFC40", "MFC 4.0 (Visual C++ 4.x)"},
        {"MFC250", "MFC 2.5 (Visual C++ 1.5)"},
        {"VBRUN300", "Visual Basic 3.0"},
        {"MSVBVM50", "Visual Basic 5.0"}};
    const auto it = t.find(stem);
    return it == t.end() ? std::string_view() : it->second;
}

// The module's name without its last extension, in capitals ("KERNEL32.dll" -> "KERNEL32").
std::string module_stem(std::string_view file_name) {
    const std::size_t dot = file_name.rfind('.');
    return rl_text::upper(dot == std::string_view::npos || dot == 0 ? file_name : file_name.substr(0, dot));
}

std::size_t find_last(ByteView haystack, std::string_view needle, std::size_t from, std::size_t end) {
    const ByteView n = as_bytes(needle);
    if (end > haystack.size()) end = haystack.size();
    if (end < from + n.size()) return std::string_view::npos;
    for (std::size_t at = end - n.size() + 1; at-- > from;) {
        if (std::equal(n.begin(), n.end(), haystack.begin() + static_cast<std::ptrdiff_t>(at))) return at;
    }
    return std::string_view::npos;
}

std::uint32_t u32_at(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | (static_cast<std::uint32_t>(b[at + 1]) << 8) |
           (static_cast<std::uint32_t>(b[at + 2]) << 16) | (static_cast<std::uint32_t>(b[at + 3]) << 24);
}

}  // namespace

std::vector<ToolRecord> tool_block(ByteView file, std::size_t header_offset) {
    constexpr std::size_t kStubEnd = 0x40;            // the DOS header; the block is after it
    constexpr std::uint32_t kStart = 0x536E6144;      // "DanS": where the block starts
    const std::size_t at = find_last(file, "Rich", kStubEnd, header_offset);
    if (at == std::string_view::npos || at + 8 > file.size()) return {};
    const std::uint32_t key = u32_at(file, at + 4);
    constexpr std::size_t kPadding = 16;   // the start mark and three zero words, then the records
    std::size_t start = at;
    while (start >= kStubEnd + 4) {
        start -= 4;
        if ((u32_at(file, start) ^ key) != kStart) continue;
        std::vector<ToolRecord> out;
        for (std::size_t p = start + kPadding; p + 8 <= at; p += 8) {
            const std::uint32_t id = u32_at(file, p) ^ key;
            out.push_back({id >> 16, id & 0xFFFF, u32_at(file, p + 4) ^ key});
        }
        return out;
    }
    return {};   // no start mark: not a tool block
}

CompilerInfo describe_compiler(const CompilerFacts& facts, std::span<const std::string> libraries) {
    CompilerInfo info;
    std::string guess;
    std::vector<std::string> uses;
    if (facts.kind == CompilerFacts::Kind::Pe || facts.kind == CompilerFacts::Kind::Ne) {
        const bool pe = facts.kind == CompilerFacts::Kind::Pe;
        info.linker = std::format("{}.{:02}", facts.linker_major, facts.linker_minor);
        const Version v{facts.linker_major, facts.linker_minor};
        guess = std::string(pe ? linker32(v) : linker16(v));
        info.evidence.push_back(std::format("linker version {} in the header", info.linker));
        if (pe) {
            const std::vector<ToolRecord> rich = tool_block(facts.file, facts.pe_header_offset);
            if (!rich.empty()) {
                std::string shown;
                for (std::size_t k = 0; k < std::min<std::size_t>(6, rich.size()); ++k) {
                    if (k != 0) shown += ", ";
                    shown += std::format("{}.{} x{}", rich[k].product, rich[k].build, rich[k].count);
                }
                info.evidence.push_back("tool block before the header: " + shown);
            }
            std::set<std::string> stems;
            for (const std::string& dll : facts.imported_modules) stems.insert(module_stem(dll));
            uses.assign(stems.begin(), stems.end());
        } else {
            for (const std::string& m : facts.module_references) uses.push_back(rl_text::upper(m));
        }
    }
    const std::string_view file_text = as_text(facts.file);
    for (const Mark& mark : kMarks) {
        if (file_text.find(mark.text) == std::string_view::npos) continue;
        info.evidence.push_back(std::format("the text '{}'", mark.text));
        if (guess.empty() || mark.tool.starts_with("Borland") || mark.tool.starts_with("Watcom")) guess = std::string(mark.tool);
    }
    for (const std::string& stem : uses) {
        const std::string_view what = runtime_dll(stem);
        if (!what.empty()) info.evidence.push_back(std::format("imports {}: {}", stem, what));
    }
    // The libraries the identified functions came from, most functions first (equal counts keep first-met order).
    std::vector<std::pair<std::string, std::size_t>> counts;
    for (const std::string& library : libraries) {
        const auto it = std::find_if(counts.begin(), counts.end(), [&](const auto& c) { return c.first == library; });
        if (it == counts.end()) {
            counts.emplace_back(library, 1);
        } else {
            ++it->second;
        }
    }
    std::vector<std::pair<std::string, std::size_t>> ranked = counts;
    std::stable_sort(ranked.begin(), ranked.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    for (std::size_t k = 0; k < std::min<std::size_t>(3, ranked.size()); ++k) {
        info.evidence.push_back(std::format("{} functions from {}", ranked[k].second, ranked[k].first));
    }
    if (guess.empty() && !ranked.empty()) {
        // a library is described as "<compiler>: <library>"
        guess = ranked.front().first.substr(0, ranked.front().first.find(':'));
    }
    info.compiler = guess.empty() ? "not established" : guess;
    return info;
}

// ---- making the signatures ----------------------------------------------------------------------------------------

namespace {

bool is_any_of(std::string_view s, std::initializer_list<std::string_view> options) {
    return std::find(options.begin(), options.end(), s) != options.end();
}

// `reg` as the 16-bit register name or its 32-bit one ("bp" or "ebp").
bool is_reg(std::string_view s, std::string_view reg) {
    return s == reg || (s.size() == reg.size() + 1 && s[0] == 'e' && s.substr(1) == reg);
}

// "a, b" with each side the named register, in either width.
bool is_reg_pair(std::string_view s, std::string_view a, std::string_view b) {
    const std::size_t comma = s.find(", ");
    return comma != std::string_view::npos && is_reg(s.substr(0, comma), a) && is_reg(s.substr(comma + 2), b);
}

bool is_hex_literal(std::string_view s) {
    if (s.starts_with("0x")) s.remove_prefix(2);
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); });
}

}  // namespace

bool is_frame_instruction(std::string_view mnemonic, std::string_view operands) {
    const std::string full = std::string(rl_text::trim(std::format("{} {}", mnemonic, operands)));
    const std::string_view t = full;
    // The instruction's text is matched from its start; only some of the forms must also end there.
    if (t.starts_with("nop") || t.starts_with("leave") || t.starts_with("ret") || t.starts_with("int3") || t.starts_with("lea sp, ")) {
        return true;
    }
    if (t.starts_with("sub ") || t.starts_with("add ")) {
        const std::string_view rest = t.substr(4);
        return rest.starts_with("sp, ") || rest.starts_with("esp, ") || rest.starts_with("bp, ");
    }
    if (t.starts_with("push ") || t.starts_with("pop ")) {
        const std::string_view reg = t.substr(t.find(' ') + 1);
        return is_reg(reg, "bp") || is_reg(reg, "si") || is_reg(reg, "di") || is_reg(reg, "bx") || reg == "ds" || reg == "es";
    }
    if (t.starts_with("mov ")) {
        const std::string_view rest = t.substr(4);
        if (is_reg_pair(rest, "bp", "sp") || is_reg_pair(rest, "sp", "bp") || is_any_of(rest, {"ds, ax", "ax, ds", "es, ax"})) return true;
        // `return 0`, or any constant
        const std::size_t comma = rest.find(", ");
        return comma != std::string_view::npos && (is_reg(rest.substr(0, comma), "ax") || is_reg(rest.substr(0, comma), "dx")) &&
               is_hex_literal(rest.substr(comma + 2));
    }
    if (t.starts_with("xor ")) return is_reg_pair(t.substr(4), "ax", "ax") || is_reg_pair(t.substr(4), "dx", "dx");
    if (t.starts_with("inc ") || t.starts_with("dec ")) return t.substr(4) == "bp";
    return false;
}

std::size_t substance(platforms::DecoderSet& decoders, Bits bits, ByteView code, std::size_t size) {
    const platforms::Decoder& cs = decoders.get(platforms::Arch::X86, bits == Bits::X32 ? 32 : 16, false);
    std::size_t n = 0;
    for (const platforms::Insn& i : cs.disasm(code.first(std::min(size, code.size())), 0)) {
        if (!is_frame_instruction(i.mnemonic, i.op_str)) ++n;
    }
    return n;
}

std::size_t Learned::count(Bits bits) const {
    return static_cast<std::size_t>(std::count_if(rows.begin(), rows.end(), [&](const LearnedRow& r) { return r.bits == bits; }));
}

namespace {

struct LearnKey {
    Bits bits;
    std::size_t size;
    std::string mask;
    std::string digest;
    friend auto operator<=>(const LearnKey&, const LearnKey&) = default;
};

std::string file_label_name(const std::filesystem::path& file) { return rl_text::upper(file.filename().string()); }

}  // namespace

Learned learn(std::span<const LearnSource> sources, platforms::DecoderSet& decoders,
              const std::function<void(const std::string&)>& log) {
    Learned learned;
    std::map<LearnKey, std::size_t> found;                       // key -> index in `all` (lookup only)
    std::map<LearnKey, std::set<std::string>> names;             // the C names that share a signature
    std::vector<std::pair<LearnKey, LearnedRow>> all;            // in the order learned
    for (const LearnSource& source : sources) {
        libraries::Library library;
        try {
            library = libraries::read_file(source.file);
        } catch (const Error& e) {   // one library of the batch: the rest are still read
            log(std::format("{}: {}", source.file.generic_string(), e.what()));
            continue;
        }
        const std::string label = std::format("{} ({})", source.what, file_label_name(source.file));
        const std::size_t need = source.what.find("MFC") != std::string::npos ? 8 : 4;
        if (library.machine == libraries::Machine::PowerPc) {
            log(std::format("{}: not x86 code", source.file.generic_string()));
            continue;
        }
        const Bits bits = library.machine == libraries::Machine::X86_16 ? Bits::X16 : Bits::X32;
        std::size_t added = 0;
        for (const libraries::Function& f : library.functions) {
            const std::optional<Signature> sig = signature(f.code, f.mask);
            std::size_t lead = 0;
            while (lead < kLead && lead < f.code.size() && !(lead < f.mask.size() && f.mask[lead] != 0)) ++lead;
            if (!sig || lead == 0) continue;
            const LearnKey key{bits, sig->size, sig->mask_text, sig->digest};
            const std::string name = c_name(f.name);
            names[key].insert(name);
            if (found.contains(key) || substance(decoders, bits, f.code, sig->size) < need) continue;
            auto lib_at = std::find(learned.libraries.begin(), learned.libraries.end(), label);
            if (lib_at == learned.libraries.end()) lib_at = learned.libraries.insert(learned.libraries.end(), label);
            LearnedRow row;
            row.bits = bits;
            row.library = static_cast<std::size_t>(lib_at - learned.libraries.begin());
            row.row = {crypto::to_hex(ByteView(f.code).first(lead)), sig->size, sig->mask_text, sig->digest, name, {}};
            found[key] = all.size();
            all.emplace_back(key, std::move(row));
            ++added;
        }
        log(std::format("{:<60} {:>5} functions, {:>5} new signatures", label, library.functions.size(), added));
    }
    std::size_t unsure = 0;
    for (auto& [key, row] : all) {
        // Two differently named functions sharing a signature that has linker bytes in it: which one a match is, is not known.
        if (!key.mask.empty() && names[key].size() > 1) {
            ++unsure;
            continue;
        }
        learned.rows.push_back(std::move(row));
    }
    log(std::format("{} signatures left out: shared by differently named functions", unsure));
    return learned;
}

std::string format_signature_file(const Learned& learned) {
    auto quoted = [](std::string_view s) { return Json(std::string(s)).dump(-1, ' ', true); };
    std::string out = "{\"libraries\": [";
    for (std::size_t i = 0; i < learned.libraries.size(); ++i) {
        if (i != 0) out += ", ";
        out += quoted(learned.libraries[i]);
    }
    out += "],\n";
    bool first_block = true;
    for (const Bits bits : {Bits::X32, Bits::X16}) {
        if (!first_block) out += ",\n";
        first_block = false;
        out += std::format("\"{}\": [\n", static_cast<int>(bits));
        bool first_row = true;
        for (const LearnedRow& r : learned.rows) {
            if (r.bits != bits) continue;
            if (!first_row) out += ",\n";
            first_row = false;
            out += std::format("[{},{},{},{},{},{}]", quoted(r.row.lead_hex), r.row.size, quoted(r.row.mask_text),
                               quoted(r.row.digest), quoted(r.row.name), r.library);
        }
        out += "\n]";
    }
    out += "\n}\n";
    return out;
}

}  // namespace cb::symbols::runtime_library
