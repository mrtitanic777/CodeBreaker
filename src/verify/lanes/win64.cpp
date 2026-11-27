// verify/lanes/win64.cpp -- see win64.hpp.
#include "verify/lanes/win64.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <format>
#include <functional>
#include <set>
#include <unordered_map>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "core16_types.hpp"
#include "emulate/unicorn_uc.hpp"
#include "formats/pe.hpp"
#include "platforms/decoder.hpp"

namespace stdfs = std::filesystem;

namespace cb::verify::lanes {

namespace {

constexpr std::uint64_t kPage = 0x1000;
std::uint64_t page_down(std::uint64_t a) { return a & ~(kPage - 1); }
std::uint64_t page_up(std::uint64_t a) { return (a + kPage - 1) & ~(kPage - 1); }

// RBX, RBP, RSI, RDI, R12-R15 at entry: a function that hands one back changed is caught though nothing here keeps a value there.
struct Saved {
    const char* name;
    std::uint64_t sentinel;
};
constexpr Saved kSaved[] = {{"RBX", 0xB0B0B0B0B0B0B0B0}, {"RBP", 0xBDBDBDBDBDBDBDBD}, {"RSI", 0x5151515151515151}, {"RDI", 0xD1D1D1D1D1D1D1D1},
                            {"R12", 0x1212121212121212}, {"R13", 0x1313131313131313}, {"R14", 0x1414141414141414}, {"R15", 0x1515151515151515}};
constexpr const char* kArgRegisters[] = {"RCX", "RDX", "R8", "R9"};

int reg(std::string_view name) {
    const std::optional<int> id = emulate::unicorn_uc::x86_register(name);
    if (!id) throw InternalError(std::format("win64: Unicorn has no register {}", name));
    return *id;
}

std::string vector_text(const Win64Lane::Vector& v) {
    std::string out = "args";
    for (const std::int64_t a : v.args) out += std::format(" {:x}", static_cast<std::uint64_t>(a));
    if (v.stub_ret) out += std::format(" ret {:x}", static_cast<std::uint64_t>(*v.stub_ret));
    if (v.fill == Win64Lane::Fill::Pointers) out += " pointers";
    if (v.fill == Win64Lane::Fill::Zeros) out += " zeros";
    for (const guided::MemInit& m : v.mem) out += std::format(" mem {:x}:{}", m.address, crypto::to_hex(m.data));
    return out;
}

// The bytes the scratch memory starts with: counting up, so a pointer argument reads something other than zeros; or each 8-byte
// word the address of the word 0x48 bytes on (round the scratch memory), so a structure's pointer leads somewhere readable.
const Bytes& scratch_bytes(Win64Lane::Fill fill) {
    static const Bytes counting = [] {
        Bytes b(Win64Lane::kScratchSize);
        for (std::size_t i = 0; i < b.size(); ++i) b[i] = static_cast<std::uint8_t>(i * 7 + 1);
        return b;
    }();
    static const Bytes linked = [] {
        Bytes b(Win64Lane::kScratchSize);
        for (std::size_t o = 0; o < b.size(); o += 8) {
            const std::uint64_t to = Win64Lane::kScratchBase + (o + 0x48) % Win64Lane::kScratchSize;
            for (int k = 0; k < 8; ++k) b[o + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(to >> (8 * k));
        }
        return b;
    }();
    static const Bytes zeros(Win64Lane::kScratchSize, 0);
    switch (fill) {
    case Win64Lane::Fill::Pointers: return linked;
    case Win64Lane::Fill::Zeros: return zeros;
    case Win64Lane::Fill::Counting: break;
    }
    return counting;
}

Bytes le64(std::uint64_t value) {
    Bytes out(8);
    for (int k = 0; k < 8; ++k) out[static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(value >> (8 * k));
    return out;
}

constexpr std::uint64_t mask_of(unsigned size) { return size >= 8 ? ~std::uint64_t{0} : (std::uint64_t{1} << (8 * size)) - 1; }

void put_le64(Bytes& b, std::size_t at, std::uint64_t value) {
    for (int k = 0; k < 8; ++k) b[at + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(value >> (8 * k));
}

// The thread's and the process's environment blocks as a process's first thread has them: the block's own address, the stack's
// extent, the process's and thread's ids; the process's image.
Bytes teb_bytes() {
    Bytes b(Win64Lane::kTebSize, 0);
    put_le64(b, 0x08, Win64Lane::kStackBase + Win64Lane::kStackSize);   // StackBase
    put_le64(b, 0x10, Win64Lane::kStackBase);                           // StackLimit
    put_le64(b, 0x30, Win64Lane::kTebBase);                             // Self
    put_le64(b, 0x40, Win64Lane::kProcessId);                           // ClientId
    put_le64(b, 0x48, Win64Lane::kThreadId);
    put_le64(b, 0x60, Win64Lane::kPebBase);                             // ProcessEnvironmentBlock
    return b;
}
Bytes peb_bytes(std::uint64_t image_base) {
    Bytes b(Win64Lane::kPebSize, 0);
    put_le64(b, 0x10, image_base);   // ImageBaseAddress
    return b;
}

void map_image(core16::Uc& uc, const Win64Lane::Image& image) {
    uc.mem_map(image.base, page_up(image.end) - image.base);
    for (const Win64Lane::Image::Region& r : image.regions) uc.mem_write(r.va, r.bytes);
}

// "const char *", " LPVOID ": the type without `const`, `CONST` and the spaces round it.
std::string trimmed_type(std::string_view type) {
    std::string out;
    for (std::string_view word : text::split_ws(type))
        if (word != "const" && word != "CONST" && word != "__restrict" && word != "WINAPI") out += (out.empty() || word == "*" ? "" : " ") + std::string(word);
    while (out.find(" *") != std::string::npos) out.replace(out.find(" *"), 2, "*");
    return out;
}

// An 8-byte word of the stack (nullopt where nothing is mapped).
std::optional<std::uint64_t> read_stack(core16::Uc& uc, std::uint64_t at) {
    try {
        const Bytes b = uc.mem_read(at, 8);
        std::uint64_t v = 0;
        for (int k = 0; k < 8; ++k) v |= static_cast<std::uint64_t>(b[static_cast<std::size_t>(k)]) << (8 * k);
        return v;
    } catch (const emulate::unicorn_uc::UcFault&) {
        return std::nullopt;
    }
}

}  // namespace

// ---- what imports give back --------------------------------------------------------------------------------------------------

bool Win64Lane::is_pointer_type(std::string_view type) {
    std::string t(trimmed_type(type));
    if (t.empty()) return false;
    if (t.back() == '*') return true;
    static const std::set<std::string, std::less<>> kPointers = {
        "HANDLE", "HMODULE", "HINSTANCE", "HLOCAL", "HGLOBAL", "HWND", "HMENU", "HICON", "HCURSOR", "HBRUSH", "HDC", "HFONT", "HBITMAP",
        "HPEN", "HRGN", "HPALETTE", "HKEY", "HRSRC", "HDESK", "HWINSTA", "HMONITOR", "HACCEL", "HHOOK", "HKL", "HDWP", "FARPROC", "PVOID",
        "LPVOID", "LPCVOID", "PCVOID", "LPSTR", "LPWSTR", "LPCSTR", "LPCWSTR", "PSTR", "PWSTR", "PCSTR", "PCWSTR", "LPTSTR", "LPCTSTR",
        "PTSTR", "PCTSTR", "PBYTE", "LPBYTE", "PSLIST_ENTRY", "PSINGLE_LIST_ENTRY", "PRUNTIME_FUNCTION", "PTP_POOL", "PTP_WORK",
        "PTP_TIMER", "PTP_WAIT", "PTP_IO", "PTP_CLEANUP_GROUP", "LPTOP_LEVEL_EXCEPTION_FILTER", "PVECTORED_EXCEPTION_HANDLER", "HDEVNOTIFY",
        "HPOWERNOTIFY", "HMETAFILE", "HENHMETAFILE", "HGDIOBJ", "HCOLORSPACE", "HGLRC", "LPWCH", "LPCH", "PWCHAR", "PCHAR", "PUCHAR",
        "PULONG", "PDWORD", "LPDWORD", "PIMAGE_NT_HEADERS", "PIMAGE_NT_HEADERS64", "PPEB", "PTEB", "PLIST_ENTRY"};
    return kPointers.contains(t);
}

std::uint64_t Win64Lane::answer(const Context& context, const std::string& import, const Vector& v) {
    const std::uint64_t given = static_cast<std::uint64_t>(v.stub_ret.value_or(0));
    if (given == 0 || !context.pointer_results.contains(import)) return given;
    // (a pointer the input gives -- into the scratch memory, a repair's -- stays: it is one already)
    if (given >= kScratchBase && given < kScratchBase + kScratchSize) return given;
    return kAnswerBase;
}

// ---- the inputs ----------------------------------------------------------------------------------------------------------------

std::vector<Win64Lane::Vector> Win64Lane::vectors() {
    const auto s = [](std::uint64_t k) { return static_cast<std::int64_t>(kScratchBase + 0x1000 * k); };
    std::vector<Vector> out;
    const auto add = [&](std::vector<std::int64_t> args, std::optional<std::int64_t> ret = std::nullopt, Fill fill = Fill::Counting) {
        Vector v;
        v.args = std::move(args);
        v.args.resize(kArguments, 0);
        v.stub_ret = ret;
        v.fill = fill;
        out.push_back(std::move(v));
    };
    add({0, 0, 0, 0, 0, 0});
    add({1, 2, 3, 4, 5, 6}, 1);
    add({s(0), s(1), s(2), s(3), s(4), s(5)});
    add({s(0), 1, s(2), 0, 0, 0}, 1);
    add({-1, 0x7FFFFFFF, 0x80, 0x10, -2, 3});
    add({s(0), 0, 0, 0, 0, 0}, 0x123);
    add({0x100, s(1), 0xFFFF, s(3), 0, 1});
    add({s(0), s(1), s(2), s(3), s(4), s(5)}, 1, Fill::Pointers);
    add({s(0), 0, s(2), 2, 0, 0}, 0, Fill::Pointers);
    // structures of nulls and zero counts; imports that hand back a pointer to memory of pointers
    add({s(0), s(1), s(2), s(3), s(4), s(5)}, std::nullopt, Fill::Zeros);
    add({s(0), 1, s(2), 0, 0, 0}, 1, Fill::Zeros);
    add({s(0), s(1), s(2), s(3), 0, 0}, s(6), Fill::Pointers);
    return out;
}

std::vector<Win64Lane::Vector> Win64Lane::directed(ByteView code, std::uint64_t va, std::size_t most) {
    thread_local platforms::DecoderSet decoders;
    std::vector<std::uint64_t> seen;   // the constants, each once, in the order the code first has them
    for (const platforms::Insn& insn : decoders.get(platforms::Arch::X86, 64).disasm(code, va)) {
        if (insn.mnemonic != "cmp" && insn.mnemonic != "sub" && insn.mnemonic != "test") continue;
        const std::string_view ops = insn.op_str;
        const std::size_t comma = ops.rfind(", ");
        if (comma == std::string_view::npos) continue;
        if (ops.starts_with("rsp") || ops.starts_with("rbp")) continue;   // (the frame, not a value)
        std::string_view literal = ops.substr(comma + 2);
        const bool negative = literal.starts_with("-");
        if (negative) literal.remove_prefix(1);
        std::optional<std::uint64_t> c = literal.starts_with("0x") ? text::parse_int<std::uint64_t>(literal.substr(2), 16) : text::parse_int<std::uint64_t>(literal);
        if (!c) continue;
        const std::uint64_t value = negative ? ~*c + 1 : *c;
        if (value != 0 && value != 1 && std::ranges::find(seen, value) == seen.end()) seen.push_back(value);
    }
    std::vector<Vector> out;
    for (std::size_t k = 0; k < seen.size() && k < most; ++k) {
        const auto c = static_cast<std::int64_t>(seen[k]);
        Vector all;
        all.args.assign(kArguments, c);
        all.stub_ret = c;
        out.push_back(all);
        Vector beside;   // (pointers into the scratch memory, the constant between them)
        for (std::size_t a = 0; a < kArguments; ++a) beside.args.push_back(a % 2 == 0 ? static_cast<std::int64_t>(kScratchBase + 0x1000 * a) : c);
        beside.stub_ret = c;
        out.push_back(beside);
    }
    return out;
}

guided::Input Win64Lane::to_guided(const Vector& v) {
    guided::Input g;
    for (const std::int64_t a : v.args) g.args.push_back(static_cast<std::uint64_t>(a));
    g.stub_ret = static_cast<std::uint64_t>(v.stub_ret.value_or(0));   // (zero is a value imports return too: a test of it can be turned)
    g.mem_init = v.mem;
    g.extra["fill"] = static_cast<std::int64_t>(v.fill);
    return g;
}

Win64Lane::Vector Win64Lane::from_guided(const guided::Input& g) {
    Vector v;
    for (std::size_t a = 0; a < g.args.size() && a < kMostArguments; ++a) v.args.push_back(static_cast<std::int64_t>(g.args[a]));
    v.args.resize(std::max(v.args.size(), kArguments), 0);
    if (g.stub_ret && *g.stub_ret != 0) v.stub_ret = static_cast<std::int64_t>(*g.stub_ret);
    if (g.mem_init) v.mem = *g.mem_init;
    if (const auto f = g.extra.find("fill"); f != g.extra.end())
        if (const std::int64_t* n = std::get_if<std::int64_t>(&f->second); n && *n >= 0 && *n <= static_cast<std::int64_t>(Fill::Zeros)) v.fill = static_cast<Fill>(*n);
    return v;
}

std::optional<bool> Win64Lane::condition(std::string_view cc, std::uint64_t rflags, std::uint64_t rcx) {
    const bool cf = (rflags & 0x1) != 0, pf = (rflags & 0x4) != 0, zf = (rflags & 0x40) != 0, sf = (rflags & 0x80) != 0, of = (rflags & 0x800) != 0;
    if (cc == "o") return of;
    if (cc == "no") return !of;
    if (cc == "b" || cc == "c" || cc == "nae") return cf;
    if (cc == "ae" || cc == "nb" || cc == "nc") return !cf;
    if (cc == "e" || cc == "z") return zf;
    if (cc == "ne" || cc == "nz") return !zf;
    if (cc == "be" || cc == "na") return cf || zf;
    if (cc == "a" || cc == "nbe") return !cf && !zf;
    if (cc == "s") return sf;
    if (cc == "ns") return !sf;
    if (cc == "p" || cc == "pe") return pf;
    if (cc == "np" || cc == "po") return !pf;
    if (cc == "l" || cc == "nge") return sf != of;
    if (cc == "ge" || cc == "nl") return sf == of;
    if (cc == "le" || cc == "ng") return zf || sf != of;
    if (cc == "g" || cc == "nle") return !zf && sf == of;
    if (cc == "rcxz") return rcx == 0;
    if (cc == "ecxz") return (rcx & 0xFFFFFFFF) == 0;
    if (cc == "cxz") return (rcx & 0xFFFF) == 0;
    return std::nullopt;
}

namespace {

// A general register: its size in bytes and the 64-bit register it is part of (`high`: AH, BH, CH, DH -- bits 8 to 15).
struct Gpr {
    std::string_view full;
    unsigned size = 0;
    bool high = false;
};
std::optional<Gpr> gpr(std::string_view name) {
    static const std::unordered_map<std::string_view, Gpr> table = [] {
        std::unordered_map<std::string_view, Gpr> t;
        struct Legacy {
            std::string_view q, d, w, b, h;
        };
        static constexpr Legacy kLegacy[] = {{"RAX", "EAX", "AX", "AL", "AH"}, {"RBX", "EBX", "BX", "BL", "BH"}, {"RCX", "ECX", "CX", "CL", "CH"},
                                             {"RDX", "EDX", "DX", "DL", "DH"}, {"RSI", "ESI", "SI", "SIL", ""},  {"RDI", "EDI", "DI", "DIL", ""},
                                             {"RBP", "EBP", "BP", "BPL", ""},  {"RSP", "ESP", "SP", "SPL", ""}};
        for (const Legacy& l : kLegacy) {
            t[l.q] = {l.q, 8};
            t[l.d] = {l.q, 4};
            t[l.w] = {l.q, 2};
            t[l.b] = {l.q, 1};
            if (!l.h.empty()) t[l.h] = {l.q, 1, true};
        }
        static constexpr std::string_view kNumbered[][4] = {{"R8", "R8D", "R8W", "R8B"},     {"R9", "R9D", "R9W", "R9B"},     {"R10", "R10D", "R10W", "R10B"},
                                                            {"R11", "R11D", "R11W", "R11B"}, {"R12", "R12D", "R12W", "R12B"}, {"R13", "R13D", "R13W", "R13B"},
                                                            {"R14", "R14D", "R14W", "R14B"}, {"R15", "R15D", "R15W", "R15B"}};
        for (const auto& n : kNumbered) {
            t[n[0]] = {n[0], 8};
            t[n[1]] = {n[0], 4};
            t[n[2]] = {n[0], 2};
            t[n[3]] = {n[0], 1};
        }
        return t;
    }();
    const auto it = table.find(name);
    if (it == table.end()) return std::nullopt;
    return it->second;
}

std::string upper(std::string_view s) {
    std::string out(s);
    for (char& c : out) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return out;
}

std::string_view trimmed(std::string_view s) {
    while (!s.empty() && s.front() == ' ') s.remove_prefix(1);
    while (!s.empty() && s.back() == ' ') s.remove_suffix(1);
    return s;
}

// "0x2a", "42", "-0x10": the number, wrapped to 64 bits.
std::optional<std::uint64_t> number(std::string_view s) {
    const bool negative = s.starts_with("-");
    if (negative) s.remove_prefix(1);
    if (s.empty() || !std::isdigit(static_cast<unsigned char>(s.front()))) return std::nullopt;
    const std::optional<std::uint64_t> n = s.starts_with("0x") ? text::parse_int<std::uint64_t>(s.substr(2), 16) : text::parse_int<std::uint64_t>(s);
    if (!n) return std::nullopt;
    return negative ? ~*n + 1 : *n;
}

}  // namespace

std::optional<Win64Lane::Operand> Win64Lane::operand(std::string_view text) {
    text = trimmed(text);
    if (text.empty()) return std::nullopt;
    Operand o;
    static constexpr std::pair<std::string_view, unsigned> kSizes[] = {{"byte ptr ", 1}, {"word ptr ", 2}, {"dword ptr ", 4}, {"qword ptr ", 8}};
    for (const auto& [prefix, size] : kSizes)
        if (text.starts_with(prefix)) {
            o.size = size;
            text.remove_prefix(prefix.size());
            break;
        }
    if (const std::size_t open = text.find('['); open != std::string_view::npos) {
        if (open != 0 || !text.ends_with("]")) return std::nullopt;   // (a segment's address, or a size this does not read)
        o.kind = Operand::Kind::Mem;
        std::string_view inner = text.substr(1, text.size() - 2);
        bool minus = false;
        while (!inner.empty()) {
            const std::size_t plus = inner.find(" + "), less = inner.find(" - ");
            const std::size_t cut = std::min(plus, less);
            const std::string_view term = trimmed(inner.substr(0, cut));
            if (const std::size_t star = term.find('*'); star != std::string_view::npos) {
                const std::string r = upper(term.substr(0, star));
                const std::optional<std::uint64_t> scale = number(term.substr(star + 1));
                if (!gpr(r) || !scale || minus) return std::nullopt;
                o.index = r;
                o.scale = static_cast<unsigned>(*scale);
            } else if (const std::optional<std::uint64_t> n = number(term)) {
                o.imm += minus ? ~*n + 1 : *n;
            } else {
                const std::string r = upper(term);
                if ((r != "RIP" && !gpr(r)) || minus) return std::nullopt;
                (o.base.empty() ? o.base : o.index) = r;
            }
            if (cut == std::string_view::npos) break;
            minus = cut == less;
            inner.remove_prefix(cut + 3);
        }
        return o;
    }
    if (const std::optional<std::uint64_t> n = number(text)) {
        o.kind = Operand::Kind::Imm;
        o.imm = *n;
        return o;
    }
    const std::string r = upper(text);
    const std::optional<Gpr> g = gpr(r);
    if (!g) return std::nullopt;
    o.kind = Operand::Kind::Reg;
    o.reg = r;
    o.size = g->size;
    return o;
}

std::vector<std::optional<Win64Lane::Operand>> Win64Lane::operands(std::string_view op_str) {
    std::vector<std::optional<Operand>> out;
    if (trimmed(op_str).empty()) return out;
    for (std::string_view part : text::split(op_str, ',')) out.push_back(operand(part));
    return out;
}

// ---- an image ------------------------------------------------------------------------------------------------------------------

Win64Lane::Image Win64Lane::read_image(const stdfs::path& path) {
    const formats::pe::Image pe = formats::pe::Image::open(path);
    if (pe.header().machine != formats::pe::kMachineX64) throw FormatError(fs::to_text(path), 0, "not an x64 module");
    Image out;
    out.base = pe.header().image_base;
    out.end = out.base + pe.optional_header().image_size;
    const ByteView file = pe.data();
    out.regions.push_back({out.base, Bytes(file.begin(), file.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(file.size(), pe.optional_header().headers_size))), false});
    for (const formats::pe::Section& s : pe.sections()) {
        const ByteView data = pe.section_data(s);
        out.regions.push_back({out.base + s.virtual_address, Bytes(data.begin(), data.end()), (s.characteristics & 0x80000000u) != 0});
        if ((s.characteristics & 0x80000000u) != 0 && s.virtual_size > data.size())   // (the zeros past the file's bytes are written too)
            out.regions.back().bytes.resize(s.virtual_size, 0);
    }
    for (const formats::pe::Import& i : pe.imports()) out.slots.emplace(i.slot_va, i.name);
    return out;
}

// ---- one run -------------------------------------------------------------------------------------------------------------------

namespace {

// A machine made ready to run `entry` on input `v`: the images, the import landings, the scratch memory as the input fills it, the
// stack as a call leaves it and the argument registers.
struct Ready {
    std::unique_ptr<core16::Uc> uc;
    std::vector<std::string> landed;    // the import of each landing, in order
    std::uint64_t landing_size = 0;
    std::uint64_t rsp = 0;
};

Ready ready(const Win64Lane::Context& context, std::uint64_t entry, const Win64Lane::Vector& v, bool rebuilt) {
    Ready r;
    r.uc = emulate::unicorn_uc::open_x86(64);
    core16::Uc& uc = *r.uc;
    map_image(uc, context.original);
    if (rebuilt && context.rebuilt) {
        map_image(uc, *context.rebuilt);
        // jmp qword ptr [rip+0] ; <the original's address>   -- 14 bytes: Visual C++ starts every x64 function 16 bytes from the last
        for (const auto& [at, original] : context.originals) {
            if (at == entry) continue;
            Bytes jump = {0xFF, 0x25, 0, 0, 0, 0};
            const Bytes to = le64(original);
            jump.insert(jump.end(), to.begin(), to.end());
            uc.mem_write(at, jump);
        }
    }
    // every import slot lands on one `ret` of its own
    for (const auto& [slot, name] : context.original.slots) {
        uc.mem_write(slot, le64(Win64Lane::kLandingBase + 16 * r.landed.size()));
        r.landed.push_back(name);
    }
    r.landing_size = page_up(std::max<std::uint64_t>(16 * r.landed.size(), 16));
    uc.mem_map(Win64Lane::kLandingBase, r.landing_size);
    uc.mem_write(Win64Lane::kLandingBase, Bytes(r.landing_size, 0xC3));
    uc.mem_map(Win64Lane::kReturnAddress, kPage);
    uc.mem_write(Win64Lane::kReturnAddress, Bytes(kPage, 0xC3));
    uc.mem_map(Win64Lane::kScratchBase, Win64Lane::kScratchSize);
    uc.mem_write(Win64Lane::kScratchBase, scratch_bytes(v.fill));
    // the input's own bytes: over the scratch memory, or the original's globals
    for (const guided::MemInit& m : v.mem) {
        const std::uint64_t end = m.address + m.data.size();
        const bool scratch = m.address >= Win64Lane::kScratchBase && end <= Win64Lane::kScratchBase + Win64Lane::kScratchSize;
        // (only the writable sections: both sides read those at the original's addresses, but the rebuilt DLL keeps its own constants)
        const bool global = std::ranges::any_of(context.original.regions, [&](const Win64Lane::Image::Region& r) {
            return r.writable && m.address >= r.va && end <= r.va + r.bytes.size();
        });
        if ((scratch || global) && end > m.address) uc.mem_write(m.address, m.data);
    }
    // the thread's and the process's environment blocks (GS's base) and the data the kernel shares
    uc.mem_map(Win64Lane::kTebBase, Win64Lane::kTebSize + Win64Lane::kPebSize);
    uc.mem_write(Win64Lane::kTebBase, teb_bytes());
    uc.mem_write(Win64Lane::kPebBase, peb_bytes(context.original.base));
    uc.mem_map(Win64Lane::kSharedUserData, Win64Lane::kSharedUserDataSize);
    uc.reg_write(reg("GS_BASE"), Win64Lane::kTebBase);
    uc.mem_map(Win64Lane::kStackBase, Win64Lane::kStackSize);
    // the stack as a call leaves it: the return address, the 32 bytes kept for the register arguments, the rest above them
    r.rsp = Win64Lane::kStackBase + Win64Lane::kStackSize - 0x10000 - 8;
    const std::size_t n = std::min(std::max(v.args.size(), Win64Lane::kArguments), Win64Lane::kMostArguments);
    Bytes frame(8 + 32 + 8 * (n - 4), 0);
    for (int k = 0; k < 8; ++k) frame[static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(Win64Lane::kReturnAddress >> (8 * k));
    for (std::size_t a = 4; a < v.args.size() && a < n; ++a)
        for (int k = 0; k < 8; ++k) frame[40 + 8 * (a - 4) + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(static_cast<std::uint64_t>(v.args[a]) >> (8 * k));
    uc.mem_write(r.rsp, frame);
    uc.reg_write(reg("RSP"), r.rsp);
    for (std::size_t a = 0; a < 4 && a < v.args.size(); ++a) uc.reg_write(reg(kArgRegisters[a]), static_cast<std::uint64_t>(v.args[a]));
    for (const Saved& s : kSaved) uc.reg_write(reg(s.name), s.sentinel);
    uc.reg_write(reg("RAX"), 0);
    return r;
}

}  // namespace

compare::Fingerprint Win64Lane::fingerprint(const Context& context, std::uint64_t entry, const Vector& v, bool rebuilt, bool novalue,
                                            std::optional<std::pair<std::uint64_t, std::uint64_t>> own) {
    compare::Fingerprint fp;
    try {
        Ready made = ready(context, entry, v, rebuilt);
        core16::Uc* uc = made.uc.get();
        const std::vector<std::string>& landed = made.landed;
        const std::uint64_t rsp = made.rsp;

        const std::uint64_t rebuilt_lo = rebuilt && context.rebuilt ? context.rebuilt->base : 0, rebuilt_hi = rebuilt && context.rebuilt ? context.rebuilt->end : 0;
        const auto argument = [&](std::uint64_t value) -> std::string {
            if (value >= kStackBase && value < kStackBase + kStackSize) return "stack";   // (the two frames are laid out differently)
            if (value >= rebuilt_lo && value < rebuilt_hi) {   // a rebuilt function stands for the original's (a callback handed out)
                if (const auto o = context.originals.find(value); o != context.originals.end()) return std::format("{:x}", o->second);
                return "rebuilt";
            }
            return std::format("{:x}", value);
        };
        constexpr std::size_t kMaxDeps = 64;
        const std::uint64_t result = static_cast<std::uint64_t>(v.stub_ret.value_or(0));
        const auto record = [&](const std::function<std::string()>& call) {   // (a call past the cap is counted as "...")
            if (fp.deps.size() < kMaxDeps) fp.deps.push_back(call());
            else if (fp.deps.size() == kMaxDeps) fp.deps.emplace_back("...");
        };
        uc->hook_add_code(
            [&](core16::Uc& m, std::uint64_t address, std::uint32_t) {
                const std::size_t k = static_cast<std::size_t>((address - kLandingBase) / 16);
                if (k >= landed.size()) return;
                const std::string& name = landed[k];
                record([&] {
                    std::string call = name + "(";
                    if (const auto n = context.argc.find(name); n != context.argc.end()) {
                        for (int a = 0; a < std::min(n->second, 4); ++a) call += (a == 0 ? "" : ",") + argument(m.reg_read(reg(kArgRegisters[a])));
                    } else {
                        call += "?";
                    }
                    return call + ")";
                });
                m.reg_write(reg("RAX"), answer(context, name, v));
            },
            kLandingBase, kLandingBase + made.landing_size - 1);
        // a system call: its number (EAX) and, where the stub's declaration says how many there are, its arguments as the kernel reads
        // them (R10 for RCX, then RDX, R8, R9, and the first two on the stack) -- else the number alone, compared as a call whose
        // arguments are not known; it gives back what the input says imports give
        uc->hook_add_insn_syscall([&](core16::Uc& m) {
            record([&] {
                const auto number = static_cast<std::uint32_t>(m.reg_read(reg("EAX")));
                const std::uint64_t sp = m.reg_read(reg("RSP"));
                // (made by the function judged itself, a stub, with the stack as its caller left it: every argument it can pass on)
                const bool own = sp == rsp;
                const auto n = context.syscall_argc.find(number);
                if (!own && n == context.syscall_argc.end()) return std::format("syscall {:x}", number);
                const int count = own ? 6 : n->second;
                std::string call = std::format("syscall({:x}", number);
                static constexpr const char* kSyscallRegisters[] = {"R10", "RDX", "R8", "R9"};
                for (int a = 0; a < std::min(count, 4); ++a) call += "," + argument(m.reg_read(reg(kSyscallRegisters[a])));
                for (int slot = 0; slot < std::min(count - 4, 2); ++slot) {
                    const std::optional<std::uint64_t> word = read_stack(m, sp + 0x28 + 8 * static_cast<std::uint64_t>(slot));
                    call += "," + (word ? argument(*word) : std::string("?"));
                }
                return call + ")";
            });
            m.reg_write(reg("RAX"), result);
        });
        std::set<std::int64_t> covered;
        if (own) uc->hook_add_code([&](core16::Uc&, std::uint64_t address, std::uint32_t) { covered.insert(static_cast<std::int64_t>(address)); }, own->first, own->second - 1);
        std::string exit;
        try {
            uc->emu_start(entry, kReturnAddress, 2'000'000, kMaxSteps);
            exit = uc->reg_read(reg("RIP")) == kReturnAddress ? std::string(compare::kExitReturned) : std::string(compare::kExitTruncated);
        } catch (const emulate::unicorn_uc::UcFault& e) {
            // (a fault is a fault: which access faults first -- a read, a write, a fetch -- is the emulated environment's, not the code's)
            exit = "exception: fault";
            if (fs::get_env("CB_WIN64_TRACE"))   // (where it faulted, for the one looking into a verdict)
                std::fprintf(stderr, "win64: %s side, entry %llx: %s at rip %llx (rax %llx rcx %llx rdx %llx rsp %llx)\n", rebuilt ? "rebuilt" : "original",
                             static_cast<unsigned long long>(entry), e.what(), static_cast<unsigned long long>(uc->reg_read(reg("RIP"))),
                             static_cast<unsigned long long>(uc->reg_read(reg("RAX"))), static_cast<unsigned long long>(uc->reg_read(reg("RCX"))),
                             static_cast<unsigned long long>(uc->reg_read(reg("RDX"))), static_cast<unsigned long long>(uc->reg_read(reg("RSP"))));
        }
        fp.exit = exit;
        fp.truncated = exit == compare::kExitTruncated;
        fp.cov.assign(covered.begin(), covered.end());
        if (exit == compare::kExitReturned) {
            fp.ret = novalue ? 0 : static_cast<std::int64_t>(uc->reg_read(reg("RAX")));
            fp.pop = static_cast<std::int64_t>(uc->reg_read(reg("RSP")) - rsp);
            bool saved = true;
            for (const Saved& s : kSaved) saved = saved && uc->reg_read(reg(s.name)) == s.sentinel;
            fp.saved = saved;
        }
        // what it left: the module's writable data, the memory behind the pointer arguments, the environment blocks
        Bytes left = uc->mem_read(kScratchBase, kScratchSize);
        const Bytes blocks = uc->mem_read(kTebBase, kTebSize + kPebSize);
        left.insert(left.end(), blocks.begin(), blocks.end());
        for (const Image::Region& r : context.original.regions) {
            if (!r.writable) continue;
            const Bytes now = uc->mem_read(r.va, r.bytes.size());
            left.insert(left.end(), now.begin(), now.end());
        }
        fp.writes = crypto::to_hex(crypto::sha1(left));
    } catch (const emulate::unicorn_uc::UcFault& e) {   // (the machine could not be set up: no observation)
        fp.err = std::string("emulator: ") + e.what();
    }
    return fp;
}

// ---- guided inputs -------------------------------------------------------------------------------------------------------------

namespace {

constexpr std::uint64_t kRetStub = Win64Lane::kScratchBase + Win64Lane::kScratchSize - 0x10;   // a `ret` a repaired code pointer leads to
constexpr std::size_t kRepairSlots = 15;          // places a repair points at, 0x800 apart from kRepairBase (below the `ret`)
constexpr int kRepairSteps = 6;                   // repairs in a row on one input
constexpr std::size_t kRepairSeeds = 8;           // inputs repaired at most
constexpr int kSwitchRuns = 40;                   // runs spent on the cases of switches
constexpr std::uint64_t kMostCases = 64;          // a bound above this is not taken for a switch's
constexpr std::size_t kMostBranches = 4096;       // decisions a trace keeps (a loop makes one each time round)

// One instruction of the original as the trace reads it.
struct Decoded {
    enum class What : std::uint8_t { Other, Compare, Decision, Load, Call, Neutral };
    What what = What::Other;
    guided::CompareOp op = guided::CompareOp::Other;
    std::string cc;                                     // Decision: the condition
    bool jump = false;                                  // Decision: a conditional jump (not setcc/cmovcc)
    bool writes = false;                                // Compare: it writes its first operand (all but cmp and test)
    std::uint32_t size = 0;
    std::vector<std::optional<Win64Lane::Operand>> ops;
    bool indirect = false;                              // an indirect jmp
};

Decoded decode(const platforms::Insn& insn) {
    Decoded d;
    d.size = insn.size;
    const std::string_view m = insn.mnemonic;
    d.ops = Win64Lane::operands(insn.op_str);
    const auto first_is = [&](std::string_view full) {
        if (d.ops.empty() || !d.ops[0] || d.ops[0]->kind != Win64Lane::Operand::Kind::Reg) return false;
        const std::optional<Gpr> g = gpr(d.ops[0]->reg);
        return g && g->full == full;
    };
    if (m == "jmp") {
        d.indirect = !d.ops.empty() && d.ops[0] && d.ops[0]->kind != Win64Lane::Operand::Kind::Imm;
    } else if (m.starts_with("j") || m.starts_with("set") || m.starts_with("cmov")) {
        const std::string_view cc = m.substr(m.starts_with("j") ? 1 : m.starts_with("set") ? 3 : 4);
        if (Win64Lane::condition(cc, 0, 0)) {
            d.what = Decoded::What::Decision;
            d.cc = std::string(cc);
            d.jump = m.starts_with("j");
        }
    } else if (m == "cmp" || m == "test" || m == "sub" || m == "and" || m == "or" || m == "xor" || m == "add" || m == "dec" || m == "inc") {
        const bool zeroing = m == "xor" && d.ops.size() == 2 && d.ops[0] && d.ops[1] && d.ops[0]->kind == Win64Lane::Operand::Kind::Reg &&
                             d.ops[1]->kind == Win64Lane::Operand::Kind::Reg && d.ops[0]->reg == d.ops[1]->reg;
        if (!first_is("RSP") && !first_is("RBP") && !zeroing) {
            d.what = Decoded::What::Compare;
            d.op = m == "cmp" || m == "sub" ? guided::CompareOp::Other : guided::parse_compare_op(m);
            d.writes = m != "cmp" && m != "test";
        }
    } else if ((m == "mov" || m == "movzx" || m == "movsx" || m == "movsxd") && d.ops.size() == 2 && d.ops[0] && d.ops[1] &&
               d.ops[0]->kind == Win64Lane::Operand::Kind::Reg && d.ops[1]->kind == Win64Lane::Operand::Kind::Mem) {
        d.what = Decoded::What::Load;
    } else if (m == "call") {
        d.what = Decoded::What::Call;
    } else if (m.starts_with("mov") || m == "lea" || m == "nop" || m == "push" || m == "pop") {
        d.what = Decoded::What::Neutral;   // (leaves the flags as they were)
    }
    return d;
}

// The register's value as wide as it is (AH: bits 8-15).
std::uint64_t register_value(core16::Uc& uc, std::string_view name) {
    const std::optional<Gpr> g = gpr(name);
    if (!g) return 0;
    std::uint64_t v = uc.reg_read(reg(g->full));
    if (g->high) v >>= 8;
    return v & mask_of(g->size);
}

std::uint64_t address_of(core16::Uc& uc, const Win64Lane::Operand& o, std::uint64_t next) {
    std::uint64_t a = o.imm;
    if (o.base == "RIP") a += next;
    else if (!o.base.empty()) a += register_value(uc, o.base);
    if (!o.index.empty()) a += register_value(uc, o.index) * o.scale;
    return a;
}

std::optional<std::uint64_t> read_le(core16::Uc& uc, std::uint64_t at, unsigned size) {
    try {
        const Bytes b = uc.mem_read(at, size);
        std::uint64_t v = 0;
        for (unsigned k = 0; k < size; ++k) v |= static_cast<std::uint64_t>(b[k]) << (8 * k);
        return v;
    } catch (const emulate::unicorn_uc::UcFault&) {
        return std::nullopt;
    }
}

}  // namespace

std::optional<Win64Lane::Trace> Win64Lane::trace(const Context& context, std::uint64_t entry, std::pair<std::uint64_t, std::uint64_t> span, const Vector& v) {
    thread_local platforms::DecoderSet decoders;
    const platforms::Decoder& decoder = decoders.get(platforms::Arch::X86, 64);
    Trace t;
    try {
        Ready made = ready(context, entry, v, false);
        core16::Uc& uc = *made.uc;
        const std::uint64_t result = static_cast<std::uint64_t>(v.stub_ret.value_or(0));
        uc.hook_add_code(
            [&](core16::Uc& m, std::uint64_t address, std::uint32_t) {
                const std::size_t k = static_cast<std::size_t>((address - kLandingBase) / 16);
                if (k < made.landed.size()) m.reg_write(reg("RAX"), answer(context, made.landed[k], v));
            },
            kLandingBase, kLandingBase + made.landing_size - 1);
        uc.hook_add_insn_syscall([&](core16::Uc& m) { m.reg_write(reg("RAX"), result); });

        std::unordered_map<std::uint64_t, Decoded> code;   // the instructions met, by address
        const auto decoded = [&](std::uint64_t address) -> const Decoded* {
            if (const auto it = code.find(address); it != code.end()) return &it->second;
            const std::optional<Bytes> bytes = [&]() -> std::optional<Bytes> {
                try {
                    return uc.mem_read(address, 16);
                } catch (const emulate::unicorn_uc::UcFault&) {
                    return std::nullopt;
                }
            }();
            if (!bytes) return nullptr;
            const std::vector<platforms::Insn> one = decoder.disasm(*bytes, address, 1);
            if (one.empty()) return nullptr;
            return &code.emplace(address, decode(one[0])).first->second;
        };
        // where a register's value was loaded from, while it holds it: (address, size)
        std::unordered_map<std::string_view, std::pair<std::uint64_t, unsigned>> loaded_from;
        std::unordered_map<std::uint64_t, std::uint64_t> loads;   // an 8-byte value loaded -> where it was last loaded from
        std::optional<guided::Compare> pending;                   // the comparison the next decision reads
        guided::Observation& o = t.observation;
        o.args_at = made.rsp + 8;                                 // the home of argument 0; the others follow, eight bytes apart
        o.stack_low = kStackBase;
        o.stack_high = made.rsp + 8;

        const auto value_of = [&](const Operand& op, unsigned size, std::uint64_t next, std::optional<std::uint64_t>& at) -> std::optional<std::uint64_t> {
            switch (op.kind) {
            case Operand::Kind::Imm: return op.imm & mask_of(size);
            case Operand::Kind::Reg: {
                const std::optional<Gpr> g = gpr(op.reg);
                if (!g->high)
                    if (const auto f = loaded_from.find(g->full); f != loaded_from.end() && f->second.second >= size) at = f->second.first;
                return register_value(uc, op.reg) & mask_of(size);
            }
            case Operand::Kind::Mem: {
                at = address_of(uc, op, next);
                return read_le(uc, *at, size);
            }
            }
            return std::nullopt;
        };
        uc.hook_add_code(
            [&](core16::Uc& m, std::uint64_t address, std::uint32_t) {
                o.reach.insert(address);
                const Decoded* d = decoded(address);
                if (!d) return;
                const std::uint64_t next = address + d->size;
                switch (d->what) {
                case Decoded::What::Decision: {
                    const std::optional<bool> taken = condition(d->cc, m.reg_read(reg("RFLAGS")), m.reg_read(reg("RCX")));
                    if (o.branches.size() < kMostBranches) o.branches.push_back({address, taken.value_or(false), pending});
                    break;
                }
                case Decoded::What::Compare: {
                    pending.reset();
                    if (d->ops.empty() || !d->ops[0]) break;
                    guided::Compare c;
                    c.op = d->op;
                    const Operand& a = *d->ops[0];
                    const std::optional<Operand> b = d->ops.size() > 1 ? d->ops[1] : std::optional<Operand>(Operand{Operand::Kind::Imm, 0, "", 1});
                    if (!b) break;
                    c.size = a.size != 0 ? a.size : b->size != 0 ? b->size : 8;
                    const std::optional<std::uint64_t> av = value_of(a, c.size, next, c.a_at), bv = value_of(*b, c.size, next, c.b_at);
                    if (!av || !bv) break;
                    c.a = *av;
                    c.b = *bv;
                    pending = c;
                    if (d->writes && a.kind == Operand::Kind::Reg) loaded_from.erase(gpr(a.reg)->full);   // (all but cmp and test write it)
                    break;
                }
                case Decoded::What::Load: {
                    const Operand& to = *d->ops[0];
                    const Operand& from = *d->ops[1];
                    const unsigned size = from.size != 0 ? from.size : to.size;
                    const std::uint64_t at = address_of(m, from, next);
                    const std::optional<Gpr> g = gpr(to.reg);
                    if (g->high) break;
                    loaded_from[g->full] = {at, size};
                    if (size == 8)
                        if (const std::optional<std::uint64_t> value = read_le(m, at, 8)) loads[*value] = at;
                    break;
                }
                case Decoded::What::Call:
                    pending.reset();
                    for (const std::string_view r : {"RAX", "RCX", "RDX", "R8", "R9", "R10", "R11"}) loaded_from.erase(r);
                    break;
                case Decoded::What::Neutral:
                case Decoded::What::Other:
                    if (d->what == Decoded::What::Other) pending.reset();
                    if (!d->ops.empty() && d->ops[0] && d->ops[0]->kind == Operand::Kind::Reg)   // (what it writes holds another value now)
                        if (const std::optional<Gpr> g = gpr(d->ops[0]->reg)) loaded_from.erase(g->full);
                    break;
                }
            },
            span.first, span.second - 1);
        try {
            uc.emu_start(entry, kReturnAddress, 2'000'000, kMaxSteps);
        } catch (const emulate::unicorn_uc::UcFault&) {
            t.faulted = true;
            // where the faulting address came from: the register it was made of, and where that register's value came from
            const std::uint64_t rip = uc.reg_read(reg("RIP"));
            std::vector<std::uint64_t> suspects;
            const Decoded* d = decoded(rip);
            if (d == nullptr) {
                suspects.push_back(rip);   // a call or jump to an address that holds nothing: a code pointer
                t.fetch = true;
            } else {
                for (const std::optional<Operand>& op : d->ops)
                    if (op && op->kind == Operand::Kind::Mem) {
                        if (!op->base.empty() && op->base != "RIP") suspects.push_back(register_value(uc, op->base));
                        if (!op->index.empty()) suspects.push_back(register_value(uc, op->index));
                        break;
                    }
            }
            for (const std::uint64_t x : suspects) {
                if (const auto l = loads.find(x); l != loads.end()) {
                    t.fix = guided::Place{guided::PlaceKind::Addr, l->second, 0};
                } else if (const auto a = std::ranges::find(v.args, static_cast<std::int64_t>(x)); a != v.args.end()) {
                    t.fix = guided::Place{guided::PlaceKind::Arg, static_cast<std::uint64_t>(a - v.args.begin()), 0};
                } else if (static_cast<std::uint64_t>(v.stub_ret.value_or(0)) == x && x != 0) {
                    t.fix = guided::Place{guided::PlaceKind::StubRet, 0, 0};
                }
                if (t.fix) break;
            }
        }
    } catch (const emulate::unicorn_uc::UcFault&) {   // (the machine could not be set up)
        return std::nullopt;
    }
    return t;
}

Win64Lane::Vector Win64Lane::repaired(const Vector& v, const guided::Place& fix, bool code) {
    Vector w = v;
    std::uint64_t to = kRepairBase + 0x800 * (v.mem.size() % kRepairSlots);
    if (code) {
        to = kRetStub;
        if (std::ranges::none_of(w.mem, [](const guided::MemInit& m) { return m.address == kRetStub; })) w.mem.push_back({kRetStub, Bytes{0xC3}});
    }
    switch (fix.kind) {
    case guided::PlaceKind::Arg:
        if (fix.index >= kMostArguments) return w;
        if (w.args.size() <= fix.index) w.args.resize(static_cast<std::size_t>(fix.index) + 1, 0);
        w.args[static_cast<std::size_t>(fix.index)] = static_cast<std::int64_t>(to);
        break;
    case guided::PlaceKind::StubRet: w.stub_ret = static_cast<std::int64_t>(to); break;
    case guided::PlaceKind::Addr:
    case guided::PlaceKind::Mem: w.mem.push_back({fix.index, le64(to)}); break;
    }
    return w;
}

std::vector<Win64Lane::Vector> Win64Lane::guided_inputs(const Context& context, std::pair<std::uint64_t, std::uint64_t> span, std::span<const Vector> seeds) {
    if (!lane_common::guided_on()) return {};
    const guided::Machine machine{guided::ByteOrder::Little, 8};
    std::set<std::pair<std::uint64_t, bool>> edges;
    std::set<std::uint64_t> reach;
    const auto fresh = [&](const guided::Observation& o) {
        bool any = false;
        for (const guided::Branch& b : o.branches) any |= edges.emplace(b.address, b.taken).second;
        for (const std::uint64_t a : o.reach) any |= reach.insert(a).second;
        return any;
    };
    std::vector<guided::Seen> seen;
    std::vector<Vector> out;
    std::vector<std::pair<Vector, Trace>> faulted;   // seeds whose run faulted on an address the input made
    for (const Vector& v : seeds) {
        const std::optional<Trace> t = trace(context, span.first, span, v);
        if (!t) continue;
        fresh(t->observation);
        seen.push_back({to_guided(v), t->observation});
        if (t->faulted && t->fix && faulted.size() < kRepairSeeds) faulted.emplace_back(v, *t);
    }
    if (seen.empty()) return {};

    // 1. pointers where a run faulted: the place the faulting address came from is given a pointer, until the run gets past
    for (auto& [v, t] : faulted) {
        Vector w = v;
        std::set<std::pair<int, std::uint64_t>> fixed;
        Trace at = t;
        for (int step = 0; step < kRepairSteps && at.fix; ++step) {
            if (!fixed.emplace(static_cast<int>(at.fix->kind), at.fix->index).second) break;   // (the same place again: not the cause)
            w = repaired(w, *at.fix, at.fetch);
            const std::optional<Trace> next = trace(context, span.first, span, w);
            if (!next) break;
            if (fresh(next->observation)) {
                out.push_back(w);
                seen.push_back({to_guided(w), next->observation});
            }
            if (!next->faulted) break;
            at = *next;
        }
    }

    // 2. each case of a switch: a bound compared before an indirect jump, every value up to it in the place the value compared was
    const bool has_switch = [&] {
        thread_local platforms::DecoderSet decoders;
        ByteView code;
        for (const Image::Region& r : context.original.regions)
            if (span.first >= r.va && span.first < r.va + r.bytes.size())
                code = ByteView(r.bytes).subspan(static_cast<std::size_t>(span.first - r.va),
                                                 static_cast<std::size_t>(std::min<std::uint64_t>(span.second, r.va + r.bytes.size()) - span.first));
        for (const platforms::Insn& insn : decoders.get(platforms::Arch::X86, 64).disasm(code, span.first))
            if (insn.mnemonic == "jmp" && !insn.op_str.starts_with("0x")) return true;
        return false;
    }();
    if (has_switch) {
        int spent = 0;
        std::set<std::uint64_t> bounds;
        const std::size_t observed = seen.size();
        for (std::size_t s = 0; s < observed && spent < kSwitchRuns; ++s) {
            const guided::Input from = seen[s].input;
            const guided::Observation obs = seen[s].observation;
            for (const guided::Branch& b : obs.branches) {
                if (!b.compare || b.compare->op != guided::CompareOp::Other || b.compare->b < 2 || b.compare->b > kMostCases) continue;
                if (!bounds.insert(b.address).second) continue;
                const guided::Compare& c = *b.compare;
                std::vector<guided::Place> where = guided::places(from, obs, c, 0, c.a, c.size, machine);
                std::uint64_t delta = 0;   // (the value compared may be an argument less a constant: `sub ecx, 3 ; cmp ecx, 7`)
                if (where.empty())
                    for (std::size_t i = 0; i < from.args.size() && where.empty(); ++i)
                        if (((from.args[i] - c.a) & mask_of(c.size)) < 0x100) {
                            where.push_back({guided::PlaceKind::Arg, i, 0});
                            delta = from.args[i] - c.a;
                        }
                if (where.empty()) continue;
                for (std::uint64_t k = 0; k <= c.b && spent < kSwitchRuns; ++k) {
                    const guided::Input g = guided::put(from, where[0], k + delta, c.size, machine);
                    const Vector w = from_guided(g);
                    ++spent;
                    const std::optional<Trace> t = trace(context, span.first, span, w);
                    if (t && fresh(t->observation)) {
                        out.push_back(w);
                        seen.push_back({g, t->observation});
                    }
                }
            }
        }
    }

    // 3. each comparison before a jump turned the other way (verify/guided)
    const guided::Run run = [&](const guided::Input& in) -> std::optional<guided::Observation> {
        const std::optional<Trace> t = trace(context, span.first, span, from_guided(in));
        if (!t) return std::nullopt;
        return t->observation;
    };
    guided::Options options;
    options.budget = lane_common::kGuidedBudget;
    options.keep = lane_common::kMaxGuided;
    options.seed = span.first & 0xFFFF;
    options.machine = machine;
    options.observed = seen;
    for (const guided::Input& g : guided::guided_inputs(run, {}, options)) out.push_back(from_guided(g));
    return out;
}

store::Judgement Win64Lane::judge_function(const Context& context, const Item& item) {
    const store::Judgement made = lane_common::judged([&]() -> store::Judgement {
        const std::function<compare::Fingerprint(const Vector&)> original = [&](const Vector& v) {
            std::optional<std::string> key;
            if (context.sha) key = compare::cache_key_text(std::format("win64 {} {} {:x} {:x} {} | {}", kRev, *context.sha, item.va, item.end, item.novalue, vector_text(v)));
            return lane_common::remembered(context.fingerprints, key, [&] {
                return fingerprint(context, item.va, v, false, item.novalue, std::pair{item.va, item.end});
            });
        };
        const std::function<compare::Fingerprint(const Vector&)> rebuilt = [&](const Vector& v) {
            return fingerprint(context, item.rebuilt_va, v, true, item.novalue);
        };
        const std::vector<Vector> fixed = vectors();
        const std::int64_t insns = std::max<std::int64_t>(static_cast<std::int64_t>(item.end - item.va) / 4, 1);
        ByteView code;
        for (const Image::Region& r : context.original.regions)
            if (item.va >= r.va && item.va < r.va + r.bytes.size())
                code = ByteView(r.bytes).subspan(static_cast<std::size_t>(item.va - r.va), static_cast<std::size_t>(std::min<std::uint64_t>(item.end, r.va + r.bytes.size()) - item.va));
        const compare::Outcome outcome = compare::compare<Vector>(original, rebuilt, fixed, {}, {.insns = insns},
                                                                  [&](const std::set<std::int64_t>&, const std::set<std::int64_t>&) {
                                                                      std::vector<Vector> extra = directed(code, item.va);
                                                                      std::vector<Vector> seeds = fixed;
                                                                      seeds.insert(seeds.end(), extra.begin(), extra.end());
                                                                      for (Vector& v : guided_inputs(context, {item.va, item.end}, seeds)) extra.push_back(std::move(v));
                                                                      return extra;
                                                                  });
        return {outcome.verdict, outcome.reason, outcome.limit_codes(), outcome.detail};
    });
    return made;
}

// ---- the tools -----------------------------------------------------------------------------------------------------------------

std::optional<build::functions::X64Tools> Win64Lane::find_tools(const std::vector<stdfs::path>& roots) {
    std::vector<stdfs::path> where = roots;
    if (where.empty()) where = {"C:/Program Files/Microsoft Visual Studio", "C:/Program Files (x86)/Microsoft Visual Studio"};
    // (versions compare as numbers, part by part: 14.51.36231 after 14.9)
    const auto parts = [](const std::string& v) {
        std::vector<long> out;
        for (std::string_view p : text::split(v, '.')) out.push_back(text::parse_int<long>(p).value_or(0));
        return out;
    };
    std::optional<build::functions::X64Tools> best;
    std::vector<long> best_version;
    std::error_code ec;
    for (const stdfs::path& root : where) {
        for (const auto& release : stdfs::directory_iterator(root, ec)) {
            for (const auto& edition : stdfs::directory_iterator(release.path(), ec)) {
                for (const auto& version : stdfs::directory_iterator(edition.path() / "VC" / "Tools" / "MSVC", ec)) {
                    const stdfs::path bin = version.path() / "bin" / "Hostx64" / "x64";
                    if (!stdfs::is_regular_file(bin / "cl.exe", ec) || !stdfs::is_regular_file(bin / "link.exe", ec)) continue;
                    const std::vector<long> v = parts(fs::to_text(version.path().filename()));
                    if (!best || v > best_version) {
                        best = build::functions::X64Tools{bin / "cl.exe", bin / "link.exe"};
                        best_version = v;
                    }
                }
            }
        }
    }
    return best;
}

// ---- the kind ------------------------------------------------------------------------------------------------------------------

const std::string& Win64Lane::name() const {
    static const std::string kName = "win64";
    return kName;
}

reconstruct::KindTraits Win64Lane::traits() {
    reconstruct::KindTraits t;
    t.per_function = true;   // a verdict holds while the function and what it reaches are unchanged
    return t;
}

Profile Win64Lane::profile(const stdfs::path&) const {
    std::string settings = std::format("win64 rev {} compare {} options", kRev, compare::kRevision);
    for (const std::string& o : build::functions::cflags_x64()) settings += " " + o;
    settings += " vectors";
    for (const Vector& v : vectors()) settings += " [" + vector_text(v) + "]";
    return {crypto::to_hex(crypto::sha1(as_bytes(settings))).substr(0, 12), settings};
}

std::unique_ptr<Prepared> Win64Lane::prepare(const stdfs::path& module_path, const stdfs::path& work) const {
    auto p = std::make_unique<PreparedX64>();
    p->module = decompile::c_text::compilable_x64(services_.decompiler, module_path, work);
    const translate::l2::Made& made = p->module.made;
    p->header = made.header();
    p->order = made.order();
    std::string all = p->header;
    for (const auto& [name, text] : made.texts()) p->texts[name] = text;
    for (const std::string& name : p->order)
        if (const auto text = p->texts.find(name); text != p->texts.end()) all += text->second;
    for (const auto& [va, name] : made.by_va()) p->key[name] = std::format("{:x}", va);
    const formats::pe::Image image = formats::pe::Image::open(module_path);
    for (const std::uint32_t rva : image.x64_function_starts()) p->all_keys.push_back(std::format("{:x}", image.header().image_base + rva));
    std::ranges::sort(p->all_keys);
    p->all_keys.erase(std::ranges::unique(p->all_keys).begin(), p->all_keys.end());
    p->sig = crypto::to_hex(crypto::sha1(as_bytes(all)));
    return p;
}

Built Win64Lane::build(const Prepared& prepared, const stdfs::path& module_path, const stdfs::path& work) const {
    const auto& p = static_cast<const PreparedX64&>(prepared);
    const decompile::c_text::ModuleX64& m = p.module;
    auto ctx = std::make_shared<Context>();
    ctx->original = read_image(module_path);
    const std::uint64_t base = (ctx->original.end + kRebuiltGap + 0xFFFF) & ~std::uint64_t{0xFFFF};
    const OrderedMap<std::string, std::string> written = m.made.texts();
    build::functions::Texts texts;
    for (const auto& [name, text] : written) texts[name] = text;
    const build::functions::BuiltPowerPc b = services_.builder.build_x64(services_.tools, m.made.header(), texts, p.order, work, base);
    if (b.dll) ctx->rebuilt = read_image(*b.dll);
    ctx->sha = services_.content_sha ? services_.content_sha(module_path) : std::nullopt;
    ctx->fingerprints = services_.fingerprints;
    if (services_.import_argc)
        for (const auto& [slot, name] : ctx->original.slots)
            if (const std::optional<int> n = services_.import_argc(name)) ctx->argc[name] = *n;
    // the module's own system call stubs: each one's number, and the arguments its declaration gives it
    if (services_.import_argc) {
        const formats::pe::Image pe = formats::pe::Image::open(module_path);
        for (const formats::pe::Export& e : pe.exports()) {
            if (!e.name || (!e.name->starts_with("Nt") && !e.name->starts_with("Zw"))) continue;
            const std::optional<ByteView> code = pe.read_rva(e.rva, 32);
            if (!code) continue;
            const std::optional<std::uint32_t> number = decompile::c_text::system_call_number(*code);
            if (!number || ctx->syscall_argc.contains(*number)) continue;
            std::optional<int> n = services_.import_argc(*e.name);
            if (!n) n = services_.import_argc((e.name->starts_with("Nt") ? "Zw" : "Nt") + e.name->substr(2));
            if (n) ctx->syscall_argc[*number] = *n;
        }
    }
    if (services_.import_returns)
        for (const auto& [slot, name] : ctx->original.slots)
            if (const std::optional<std::string> t = services_.import_returns(name); t && is_pointer_type(*t)) ctx->pointer_results.insert(name);
    // where each function ends: at the next function the module states, or its image's end
    const formats::pe::Image image = formats::pe::Image::open(module_path);
    std::set<std::uint64_t> starts;
    for (const std::uint32_t rva : image.x64_function_starts()) starts.insert(image.header().image_base + rva);

    Built built;
    std::map<std::string, Item> items;
    for (const auto& [name, why] : b.dropped) built.missing[name] = why;
    for (const auto& [va, name] : m.made.by_va()) {
        if (built.missing.contains(name)) continue;
        const auto placed = b.where.find(name);
        if (placed == b.where.end()) {
            built.missing[name] = "not in the build";
            continue;
        }
        Item item;
        item.key = p.key.at(name);
        item.va = va;
        const auto next = starts.upper_bound(va);
        item.end = next != starts.end() ? *next : ctx->original.end;
        item.rebuilt_va = placed->second;
        ctx->originals[placed->second] = va;
        item.novalue = m.novalue.contains(name);
        built.items.push_back(name);
        items[name] = item;
    }
    built.judge = [ctx, items = std::move(items)](const std::string& name) { return judge_function(*ctx, items.at(name)); };
    return built;
}

}  // namespace cb::verify::lanes
