// cb-unity: alone
// cb/emulate/windows16.cpp -- see windows16.hpp.
//
// Layout of this file: the GDT and the descriptors; the module as loaded (segments, relocations, imports); the 16-bit branch
// log; one run (the machine, the recorded system's LDT, the events the recorder hands up, the end of the run); the Runner.
#include "cb/emulate/windows16.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <mutex>
#include <set>
#include <unordered_map>

#include "cb/core/capstone_api.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/emulate/unicorn_uc.hpp"
#include "cb/formats/ne.hpp"
#include "cb/knowledge/package.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::emulate::windows16 {

using core16::Uc;
using unicorn_uc::UcFault;

namespace {

constexpr std::size_t kWindow = 0x10000;               // a segment's window, and the stack's
constexpr std::int64_t kBranchCap = 4096;              // conditional jumps kept per run (the 32-bit recorder's cap)
constexpr std::uint32_t kLimit = 0xFFFF;

// ---- registers ------------------------------------------------------------------------------------------------------------------
struct RegIds {
    int cs, ds, ss, es, esp, eip, eax, ebx, ecx, edx, esi, edi, ebp, gdtr, ldtr;
};
const RegIds& regs() {
    static const RegIds ids = [] {
        auto id = [](std::string_view name) {
            const std::optional<int> r = unicorn_uc::x86_register(name);
            CB_ASSERT(r.has_value(), "an x86 register Unicorn does not name");
            return *r;
        };
        return RegIds{id("CS"), id("DS"), id("SS"), id("ES"), id("ESP"), id("EIP"), id("EAX"), id("EBX"),
                      id("ECX"), id("EDX"), id("ESI"), id("EDI"), id("EBP"), id("GDTR"), id("LDTR")};
    }();
    return ids;
}

std::uint16_t word_at(Uc& machine, std::uint64_t at) {
    const Bytes b = machine.mem_read(at, 2);
    return static_cast<std::uint16_t>(b[0] | (b[1] << 8));
}
void put_word(Uc& machine, std::uint64_t at, std::uint32_t value) {
    const std::array<std::uint8_t, 2> b = {static_cast<std::uint8_t>(value & 0xFF), static_cast<std::uint8_t>((value >> 8) & 0xFF)};
    machine.mem_write(at, ByteView(b));
}
std::uint16_t low16(std::uint64_t v) { return static_cast<std::uint16_t>(v & 0xFFFF); }

// Maps a window; one already mapped (the scratch's, a heap block over it) is not an error.
void map_if_free(Uc& machine, std::uint64_t base, std::uint64_t size) {
    try {
        machine.mem_map(base, size);
    } catch (const UcFault&) {   // mapped already
    }
}

std::uint32_t base_of(const std::array<std::uint8_t, 8>& d) {
    return static_cast<std::uint32_t>(d[2] | (d[3] << 8) | (d[4] << 16)) | (static_cast<std::uint32_t>(d[7]) << 24);
}
std::uint32_t base_of(ByteView d) {   // a descriptor at the start of `d` (8 bytes)
    return static_cast<std::uint32_t>(d[2] | (d[3] << 8) | (d[4] << 16)) | (static_cast<std::uint32_t>(d[7]) << 24);
}

void put_descriptor(Bytes& table, unsigned index, const std::array<std::uint8_t, 8>& d) {
    if ((index + 1) * 8 > table.size()) table.resize((index + 1) * 8, 0);
    std::copy(d.begin(), d.end(), table.begin() + static_cast<std::ptrdiff_t>(index * 8));
}

// A conforming code descriptor of the sentinel segment, or of a window of the module's own code: how a far call to code that is not
// run is let through from level 0 to an RPL 3 selector.
std::array<std::uint8_t, 8> conforming_code(std::uint32_t base) {
    std::array<std::uint8_t, 8> e = descriptor(base, kLimit, true);
    e[5] = 0x80 | 0x10 | 0x0E;
    return e;
}

// ---- the module as loaded --------------------------------------------------------------------------------------------------------
struct Loaded {
    formats::ne::Module module;
    std::string name;                                  // the file's name
    std::vector<formats::ne::Segment> segments;        // numbered 1..n
    std::vector<formats::ne::Entry> entries;
    std::vector<std::uint16_t> selectors;              // by segment - 1
    std::vector<std::uint32_t> bases;                  // by segment - 1
    std::uint16_t autodata = 0;                        // the data segment's number, 0: none
    std::vector<Bytes> images;                         // each segment as the loader leaves it, relocated (trailing zeros of the window dropped)
    std::unordered_map<std::uint32_t, ImportHit> imports;   // by place in the sentinel segment (slot * kImportStride)
    Bytes gdt;                                         // the whole descriptor table

    explicit Loaded(formats::ne::Module m) : module(std::move(m)) {}

    std::uint16_t selector_of(std::uint32_t segment) const {   // 0 for a segment the module does not have
        return segment >= 1 && segment <= selectors.size() ? selectors[segment - 1] : std::uint16_t{0};
    }
    std::uint32_t base_of_segment(std::uint32_t segment) const {
        if (segment < 1 || segment > bases.size()) throw UsageError(std::format("the module has no segment {}", segment));
        return bases[segment - 1];
    }
    std::optional<std::uint32_t> data_base() const {
        if (autodata < 1 || autodata > bases.size()) return std::nullopt;
        return bases[autodata - 1u];
    }
};

// The value of one of KERNEL's exports that is a plain number: the loader writes it in as a constant.
std::optional<std::uint16_t> kernel_constant(std::string_view api) {
    const std::string up = text::upper(api);
    const std::size_t dot = up.find('.');
    if (dot == std::string::npos || up.substr(0, dot) != "KERNEL") return std::nullopt;
    const std::string fn = up.substr(dot + 1);
    if (fn == "@113" || fn == "__AHSHIFT") return std::uint16_t{3};
    if (fn == "@114" || fn == "__AHINCR") return std::uint16_t{8};
    return std::nullopt;
}

// The name of an import as the module's reference table gives it. The Win16 ordinal-to-name table the SDK libraries make is not asked:
// an ordinal is always "MODULE.@ordinal" (the knowledge base's own tables name them where a stand-in is wanted).
class ImportNamer {
public:
    explicit ImportNamer(const formats::ne::Module& m) : module_(m) {
        try {
            refs_ = m.module_refs();
        } catch (const FormatError&) {   // a damaged reference table: every module is "?"
        }
    }
    std::string module_name(std::uint16_t index) const {
        return index >= 1 && index <= refs_.size() ? refs_[index - 1u] : std::string("?");
    }
    std::string api(const formats::ne::Relocation& r) const {
        if (const auto* ord = std::get_if<formats::ne::ImportOrdinalRef>(&r.target))
            return std::format("{}.@{}", module_name(ord->module), ord->ordinal);
        if (const auto* name = std::get_if<formats::ne::ImportNameRef>(&r.target)) {
            // imported_name gives "?" for an offset outside the file
            return std::format("{}.{}", module_name(name->module), module_.imported_name(name->name_offset));
        }
        return "?.OSFIXUP";
    }

private:
    const formats::ne::Module& module_;
    std::vector<std::string> refs_;
};

// One segment's window while its fix-ups are applied.
struct Window {
    Bytes bytes = Bytes(kWindow, 0);
    std::size_t used = 0;   // the end of what the segment holds or a fix-up wrote

    std::uint16_t read16(std::uint32_t at) const {
        check(at, 2);
        return static_cast<std::uint16_t>(bytes[at] | (bytes[at + 1] << 8));
    }
    void write16(std::uint32_t at, std::uint32_t v) {
        check(at, 2);
        bytes[at] = static_cast<std::uint8_t>(v & 0xFF);
        bytes[at + 1] = static_cast<std::uint8_t>((v >> 8) & 0xFF);
        used = std::max<std::size_t>(used, at + 2u);
    }
    void write8(std::uint32_t at, std::uint32_t v) {
        check(at, 1);
        bytes[at] = static_cast<std::uint8_t>(v & 0xFF);
        used = std::max<std::size_t>(used, at + 1u);
    }

private:
    static void check(std::uint32_t at, std::uint32_t width) {
        if (at + width > kWindow) throw FormatError("NE relocation", at, "a fix-up lies outside its segment's 64K window");
    }
};

// Applies one relocation record to its place. The source types seen in practice: 2 SEGMENT (a selector), 5 OFFSET (16 bits), 3 POINTER
// (far, offset then selector), 0 LOBYTE; any other is written as an offset. A non-additive record starts a chain: each place holds the
// next place's offset, up to 0xFFFF.
void patch_chain(Window& w, const formats::ne::Relocation& r, std::uint32_t target_selector, std::uint32_t target_offset) {
    const unsigned source = r.source;
    auto put = [&](std::uint32_t at) {
        if (source == 2) w.write16(at, target_selector);
        else if (source == 3) {
            w.write16(at, target_offset);
            w.write16(at + 2, target_selector);
        } else if (source == 0) w.write8(at, target_offset);
        else w.write16(at, target_offset);   // 5 OFFSET, and best effort for the rest
    };
    if (r.additive()) {
        const std::uint32_t at = r.offset;
        if (source == 2) w.write16(at, w.read16(at) + target_selector);
        else if (source == 3) {
            w.write16(at, w.read16(at) + target_offset);
            w.write16(at + 2, w.read16(at + 2) + target_selector);
        } else w.write16(at, w.read16(at) + target_offset);
        return;
    }
    std::uint32_t off = r.offset;
    for (int seen = 0; off != 0xFFFF && seen < 4096; ++seen) {   // walk the chain (its length is bounded: a loop is damage)
        const std::uint32_t next = w.read16(off);                // the next link is the low 16 bits
        put(off);
        off = next;
    }
}

std::shared_ptr<const Loaded> load_module(const std::filesystem::path& path) {
    auto loaded = std::make_shared<Loaded>(formats::ne::Module::open(fs::read_file(path)));
    Loaded& m = *loaded;
    m.name = path.filename().generic_string();
    m.segments = m.module.segments();
    m.entries = m.module.entries();
    m.autodata = m.module.header().autodata_segment;
    const std::size_t n = m.segments.size();
    // The module's segments sit high in the GDT, past every selector a plain number given as an input names (20h, 60h, 100h ...): at
    // index n those numbers were selectors of the module's own code, and a function handed one as a far pointer read its own
    // instructions -- which differ between the original and a rebuild.
    if (kModuleSlot - 1 + n > kGdtSlots - kHeapSlots)   // (the top indices are the heaps')
        throw UnsupportedError("NE segments", 0, std::format("{} segments: more than the GDT holds", n));
    // The GDT: a dense table. Real segments occupy their places; every other index up to kGdtSlots is a present descriptor onto the
    // scratch segment, for Win16 code routinely `pop ds` / `pop es` / `retf`s to selectors nobody assigned (a caller's saved DS, a
    // movable segment's selector, a null argument); with a sparse table those loads raise #GP and abort the trace.
    m.gdt.assign(static_cast<std::size_t>(kGdtSlots) * 8, 0);
    const std::array<std::uint8_t, 8> scratch = descriptor(kScratchBase, kLimit, false);
    for (unsigned i = 1; i < kGdtSlots; ++i) put_descriptor(m.gdt, i, scratch);
    // The stack and the import-sentinel segments are at the same selectors whatever the module's segment count: a build with another
    // number of segments had another stack selector, and a word equal to one side's SS (a loop counter at 170h) was "SS" there and a
    // number on the other.
    put_descriptor(m.gdt, kStackSlot, descriptor(kStackBase, kLimit, false));
    put_descriptor(m.gdt, kStackSlot + 1, descriptor(kSentinelBase, kLimit, true));   // code: far calls and returns land here
    std::vector<Window> windows(n);
    for (std::size_t i = 0; i < n; ++i) {
        const formats::ne::Segment& s = m.segments[i];
        CB_ASSERT(s.index == i + 1, "NE segments are numbered 1..n");
        const std::uint32_t base = segment_base(s.index);
        m.bases.push_back(base);
        m.selectors.push_back(selector(kModuleSlot + s.index - 1));
        put_descriptor(m.gdt, kModuleSlot + s.index - 1, descriptor(base, kLimit, !s.is_data()));
        // The segment's bytes as the file holds them (a segment cut off by the end of the file has what is there).
        const ByteView file = m.module.data();
        const std::uint64_t from = std::min<std::uint64_t>(s.file_offset, file.size());
        const std::uint64_t to = std::min<std::uint64_t>(s.file_offset + s.length, file.size());
        const std::size_t body = static_cast<std::size_t>(std::min<std::uint64_t>(to - from, kWindow));
        std::copy_n(file.begin() + static_cast<std::ptrdiff_t>(from), body, windows[i].bytes.begin());
        windows[i].used = body;
    }
    // The fix-ups. INTERNAL references resolve to (selector:offset) in this GDT; external ones get a unique sentinel far address
    // (sentinel selector : slot) recorded in `imports` for the stand-ins.
    const ImportNamer namer(m.module);
    for (std::size_t i = 0; i < n; ++i) {
        for (const formats::ne::Relocation& r : m.module.relocations(m.segments[i])) {
            std::uint32_t target_selector = 0, target_offset = 0;
            if (const auto* internal = std::get_if<formats::ne::InternalRef>(&r.target)) {
                if (internal->segment == 0xFF) {   // moveable: an entry-table ordinal
                    if (internal->entry < 1 || internal->entry > m.entries.size()) continue;
                    const formats::ne::Entry& e = m.entries[internal->entry - 1u];
                    if (e.kind == formats::ne::Entry::Kind::Unused) continue;
                    target_selector = m.selector_of(e.segment);
                    target_offset = e.offset;
                } else {
                    target_selector = m.selector_of(internal->segment);
                    target_offset = internal->entry;
                }
            } else if (r.source == 5 && !std::holds_alternative<formats::ne::OsFixupRef>(r.target) &&
                       kernel_constant(namer.api(r)).has_value()) {
                // KERNEL's plain numbers (__AHSHIFT 3, __AHINCR 8): written in as the loader does
                target_selector = 0;
                target_offset = *kernel_constant(namer.api(r));
            } else {   // IMPORTORD / IMPORTNAME / OSFIXUP
                const std::uint32_t slot = static_cast<std::uint32_t>(m.imports.size()) + 1;
                target_offset = slot * kImportStride;
                target_selector = selector(kStackSlot + 1);
                ImportHit hit;
                hit.api = namer.api(r);
                hit.slot = target_offset;
                if (const auto* ord = std::get_if<formats::ne::ImportOrdinalRef>(&r.target)) {
                    hit.kind = "IMPORTORD";
                    hit.module = namer.module_name(ord->module);
                    hit.ordinal = ord->ordinal;
                } else if (const auto* name = std::get_if<formats::ne::ImportNameRef>(&r.target)) {
                    hit.kind = "IMPORTNAME";
                    hit.module = namer.module_name(name->module);
                    hit.name_off = name->name_offset;
                } else {
                    hit.kind = "OSFIXUP";
                }
                m.imports[target_offset] = std::move(hit);
            }
            patch_chain(windows[i], r, target_selector, target_offset);
        }
    }
    for (Window& w : windows) {
        w.bytes.resize(w.used);
        m.images.push_back(std::move(w.bytes));
    }
    return loaded;
}

// Maps a machine's memory the way a run begins: the segments (relocated), the stack, the sentinel and scratch windows, the GDT.
void lay_out(Uc& machine, const Loaded& m) {
    for (std::size_t i = 0; i < m.bases.size(); ++i) {
        machine.mem_map(m.bases[i], kWindow);
        if (!m.images[i].empty()) machine.mem_write(m.bases[i], m.images[i]);
    }
    machine.mem_map(kStackBase, kWindow);
    machine.mem_map(kSentinelBase, kWindow);
    map_if_free(machine, kScratchBase, kWindow);
    machine.mem_map(kGdtAddress, (m.gdt.size() + emulator::kPage - 1) / emulator::kPage * emulator::kPage);
    machine.mem_write(kGdtAddress, m.gdt);
    machine.reg_write_descriptor(regs().gdtr, {0, kGdtAddress, static_cast<std::uint32_t>(m.gdt.size() - 1), 0});
}

// ---- the 16-bit branch log ----------------------------------------------------------------------------------------------------------
// For each conditional jump of the function itself, which way it went and what the comparison before it compared (for the input
// generator, verify/guided). What differs from 32-bit code: an instruction is decoded at its offset in its segment (a jump's target is
// an offset), and a memory operand is an offset in a segment -- DS, SS for one based on BP, or the one it names -- whose base is read
// from the GDT.
constexpr std::array<std::string_view, 9> kCompares = {"cmp", "test", "sub", "and", "or", "dec", "inc", "add", "xor"};
constexpr std::array<std::string_view, 13> kFlagless = {"mov", "movzx", "movsx", "lea", "push", "pop", "nop", "xchg", "not", "cdq", "cwde", "cbw", "bswap"};

// The 32-bit register a general-purpose Capstone register name is part of; nothing for any other register.
std::optional<std::string> general_parent(std::string_view name) {
    static constexpr std::array<std::pair<std::string_view, std::string_view>, 24> table = {{
        {"al", "eax"}, {"ah", "eax"}, {"ax", "eax"}, {"eax", "eax"}, {"bl", "ebx"}, {"bh", "ebx"}, {"bx", "ebx"}, {"ebx", "ebx"},
        {"cl", "ecx"}, {"ch", "ecx"}, {"cx", "ecx"}, {"ecx", "ecx"}, {"dl", "edx"}, {"dh", "edx"}, {"dx", "edx"}, {"edx", "edx"},
        {"si", "esi"}, {"esi", "esi"}, {"di", "edi"}, {"edi", "edi"}, {"bp", "ebp"}, {"ebp", "ebp"}, {"sp", "esp"}, {"esp", "esp"}}};
    for (const auto& [from, to] : table)
        if (from == name) return std::string(to);
    return std::nullopt;
}
// ... or the name itself.
std::string parent_register(std::string_view name) { return general_parent(name).value_or(std::string(name)); }

std::optional<native_trace::CompareOp> compare_op(std::string_view mnemonic) {
    using native_trace::CompareOp;
    static constexpr std::array<CompareOp, 9> ops = {CompareOp::Cmp, CompareOp::Test, CompareOp::Sub, CompareOp::And, CompareOp::Or,
                                                     CompareOp::Dec, CompareOp::Inc, CompareOp::Add, CompareOp::Xor};
    for (std::size_t i = 0; i < kCompares.size(); ++i)
        if (kCompares[i] == mnemonic) return ops[i];
    return std::nullopt;
}

struct Operand16 {
    enum class Kind : std::uint8_t { Reg, Imm, Mem } kind = Kind::Imm;
    int reg = -1;                  // Reg: Unicorn's id
    std::string parent;            // Reg: the 32-bit register it is part of
    std::int64_t imm = 0;          // Imm
    int base = -1, index = -1, segment = -1;   // Mem: Unicorn's ids (-1: none)
    std::int64_t disp = 0;
    unsigned size = 0;
};

struct Decoded16 {
    enum class Kind : std::uint8_t { Other, Keep, Jcc, Cmp } kind = Kind::Other;
    std::uint32_t target = 0, next = 0;     // Jcc: the offset it jumps to, and the next instruction's
    native_trace::CompareOp op = native_trace::CompareOp::Cmp;
    std::array<Operand16, 2> operands;      // Cmp
    unsigned size = 0;                      // Cmp: the first operand's
    std::vector<std::string> written;       // the 32-bit registers it writes (not for Jcc)
    std::optional<std::string> load_into;   // a load from memory into a general register: its 32-bit register ...
    Operand16 load_from;                    // ... where from ...
    unsigned load_size = 0;                 // ... how much
};

int unicorn_register(std::string_view capstone_name) {
    const std::optional<int> id = unicorn_uc::x86_register(text::upper(capstone_name));
    return id ? *id : -1;
}

std::optional<Operand16> decode_operand(const platforms::Decoder& d, const cs_x86_op& op) {
    Operand16 o;
    o.size = op.size;
    auto name = [&](unsigned reg) { const char* n = d.register_name(reg); return n ? std::string(n) : std::string(); };
    if (op.type == X86_OP_REG) {
        const std::string n = name(op.reg);
        o.kind = Operand16::Kind::Reg;
        o.reg = unicorn_register(n);
        o.parent = parent_register(n);
        return o;
    }
    if (op.type == X86_OP_IMM) {
        o.kind = Operand16::Kind::Imm;
        o.imm = op.imm;
        return o;
    }
    if (op.type == X86_OP_MEM) {
        const x86_op_mem& m = op.mem;
        const std::string base = m.base ? name(m.base) : std::string();
        const std::string segment = m.segment ? name(m.segment) : (base == "bp" || base == "sp" ? "ss" : "ds");
        o.kind = Operand16::Kind::Mem;
        if (m.base) o.base = unicorn_register(base);
        if (m.index) o.index = unicorn_register(name(m.index));
        o.disp = m.disp;
        o.segment = unicorn_register(segment);
        return o;
    }
    return std::nullopt;
}

// What the branch log needs of the instruction at the start of `code` (offset `off` in its segment); Other for anything it cannot make
// sense of.
std::shared_ptr<const Decoded16> decode_branchy(ByteView code, std::uint32_t off) {
    thread_local platforms::DecoderSet decoders;   // a Capstone engine is not shared between threads
    const platforms::Decoder& d = decoders.get(platforms::Arch::X86, 16, true);
    auto out = std::make_shared<Decoded16>();
    d.each(code, off, 1, [&](const cs_insn& ins) {
        const std::string_view mn = ins.mnemonic;
        if (!ins.detail) return false;
        const cs_x86& x = ins.detail->x86;
        const std::size_t count = x.op_count;
        Decoded16 r;
        if (const std::optional<platforms::RegisterAccess> access = d.register_access(ins)) {
            std::set<std::string> written;
            for (const std::uint16_t reg : access->written) {
                const char* n = d.register_name(reg);
                written.insert(parent_register(n ? n : ""));
            }
            r.written.assign(written.begin(), written.end());
        }
        if ((mn == "mov" || mn == "movzx" || mn == "movsx") && count == 2 && x.operands[0].type == X86_OP_REG &&
            x.operands[1].type == X86_OP_MEM) {
            if (const std::optional<Operand16> from = decode_operand(d, x.operands[1])) {
                const char* into = d.register_name(x.operands[0].reg);
                const std::string into_name = into ? into : "";
                r.load_into = general_parent(into_name);
                r.load_from = *from;
                r.load_size = x.operands[1].size;
            }
        }
        const bool flagless = std::find(kFlagless.begin(), kFlagless.end(), mn) != kFlagless.end() || mn.starts_with("cmov") || mn.starts_with("set");
        if (flagless) {
            r.kind = Decoded16::Kind::Keep;
        } else if (mn.starts_with('j') && mn != "jmp" && count > 0 && x.operands[0].type == X86_OP_IMM) {
            r.kind = Decoded16::Kind::Jcc;
            r.target = static_cast<std::uint32_t>(x.operands[0].imm & 0xFFFF);
            r.next = off + ins.size;
            r.written.clear();
            r.load_into.reset();
        } else {
            r.load_into.reset();
            if (const std::optional<native_trace::CompareOp> op = compare_op(mn); op && (count == 1 || count == 2)) {
                std::vector<Operand16> ops;
                bool all_known = true;
                for (std::size_t i = 0; i < count; ++i) {
                    std::optional<Operand16> o = decode_operand(d, x.operands[i]);
                    if (!o || (o->kind == Operand16::Kind::Reg && o->reg < 0)) all_known = false;
                    else ops.push_back(*o);
                }
                if (all_known) {
                    if (ops.size() == 1) {   // dec / inc: compared with what it becomes
                        Operand16 imm;
                        imm.kind = Operand16::Kind::Imm;
                        imm.imm = mn == "dec" ? 1 : -1;
                        imm.size = ops[0].size;
                        ops.push_back(imm);
                    }
                    r.kind = Decoded16::Kind::Cmp;
                    r.op = *op;
                    r.operands = {ops[0], ops[1]};
                    r.size = ops[0].size;
                }
            }
        }
        *out = std::move(r);
        return false;
    });
    return out;
}

// A code hook for the offsets [lo, hi) of the segment at `cs_base`: appends (jump, taken, compare) to `log`, every address a linear one.
class BranchLog {
public:
    BranchLog(std::vector<native_trace::BranchEntry>& log, std::uint32_t cs_base, OffsetRange range)
        : log_(log), cs_base_(cs_base), range_(range) {}

    void step(Uc& machine, std::uint64_t addr) {
        if (pending_) {
            const Pending p = *pending_;
            pending_.reset();
            if (static_cast<std::int64_t>(log_.size()) < kBranchCap && (addr == p.target || addr == p.next))
                log_.push_back({p.site, addr == p.target, p.compare});
        }
        Bytes raw;
        try {
            raw = machine.mem_read(addr, 16);
        } catch (const UcFault&) {
            raw = machine.mem_read(addr, (addr | 0xFFF) + 1 - addr);   // the last bytes of a window
        }
        const std::shared_ptr<const Decoded16> d = decoded(raw, static_cast<std::uint32_t>(addr - cs_base_));
        if (d->kind == Decoded16::Kind::Cmp) {
            try {
                const auto [a, at_a] = value(machine, d->operands[0]);
                const auto [b, at_b] = value(machine, d->operands[1]);
                compare_ = native_trace::BranchCompare{d->op, d->size, a, b, at_a, at_b};
            } catch (const Error&) {   // an operand that cannot be read: no comparison to report
                compare_.reset();
            }
        } else if (d->kind == Decoded16::Kind::Jcc) {
            if (range_.lo <= d->target && d->target < range_.hi && d->target != d->next)
                pending_ = Pending{addr, cs_base_ + d->target, cs_base_ + d->next, compare_};
        } else if (d->kind == Decoded16::Kind::Other) {
            compare_.reset();
        }
        for (const std::string& r : d->written) loaded_.erase(r);
        if (d->load_into) {
            try {
                loaded_[*d->load_into] = {address(machine, d->load_from), d->load_size};
            } catch (const Error&) {   // a load whose address cannot be formed is not followed
            }
        }
    }

private:
    struct Pending {
        std::uint64_t site, target, next;
        std::optional<native_trace::BranchCompare> compare;
    };

    std::shared_ptr<const Decoded16> decoded(const Bytes& raw, std::uint32_t off) {
        std::string key(reinterpret_cast<const char*>(&off), sizeof off);
        key.append(reinterpret_cast<const char*>(raw.data()), raw.size());
        auto it = cache_.find(key);
        if (it == cache_.end()) it = cache_.emplace(std::move(key), decode_branchy(raw, off)).first;
        return it->second;
    }
    std::uint64_t segment_base_of(Uc& machine, std::uint32_t selector_value) {
        auto it = bases_.find(selector_value);
        if (it == bases_.end()) it = bases_.emplace(selector_value, selector_base(machine, selector_value)).first;
        return it->second;
    }
    std::uint64_t address(Uc& machine, const Operand16& mem) {
        auto reg = [&](int id) -> std::int64_t { return id < 0 ? 0 : static_cast<std::int64_t>(machine.reg_read(id)); };
        if (mem.segment < 0) throw UsageError("a memory operand without a segment register");
        const std::uint64_t off = static_cast<std::uint64_t>(mem.disp + reg(mem.base) + reg(mem.index)) & 0xFFFF;
        return segment_base_of(machine, static_cast<std::uint32_t>(machine.reg_read(mem.segment))) + off;
    }
    // An operand's value, and where it was loaded from when it is that load's register or a memory place.
    std::pair<std::uint64_t, std::optional<std::uint64_t>> value(Uc& machine, const Operand16& op) {
        const std::uint64_t mask = op.size >= 8 ? ~0ULL : ((1ULL << (8 * op.size)) - 1);
        if (op.kind == Operand16::Kind::Imm) return {static_cast<std::uint64_t>(op.imm) & mask, std::nullopt};
        if (op.kind == Operand16::Kind::Reg) {
            std::optional<std::uint64_t> where;
            if (auto at = loaded_.find(op.parent); at != loaded_.end() && at->second.second == op.size) where = at->second.first;
            return {machine.reg_read(op.reg) & mask, where};
        }
        const std::uint64_t a = address(machine, op);
        const Bytes b = machine.mem_read(a, op.size);
        std::uint64_t v = 0;
        for (std::size_t i = 0; i < b.size() && i < 8; ++i) v |= static_cast<std::uint64_t>(b[i]) << (8 * i);
        return {v, a};
    }

    std::vector<native_trace::BranchEntry>& log_;
    std::uint32_t cs_base_;
    OffsetRange range_;
    std::optional<Pending> pending_;                 // a conditional jump just run, its two landing places
    std::optional<native_trace::BranchCompare> compare_;   // the comparison in force
    std::map<std::string, std::pair<std::uint64_t, unsigned>> loaded_;   // register -> (address, size) it was loaded from; lookup only
    std::map<std::uint32_t, std::uint64_t> bases_;   // a selector's base, read once
    std::unordered_map<std::string, std::shared_ptr<const Decoded16>> cache_;   // (offset, bytes) -> decoded
};

// ---- a recorded call's LDT ---------------------------------------------------------------------------------------------------------
// Builds an LDT holding a recorded call's selectors (each index of it present: a selector not recorded names scratch), maps what they
// name, and loads LDTR. Returns the selectors the system had that name something here; `code_bases` the linear bases of the recorded
// system's code (by selector, RPL 3).
//
// With a snapshot (the whole of memory at the call, and where the LDT was in it) every selector the system had names its bytes there --
// a selector a callee hands back as much as one among the arguments.
std::set<std::uint16_t> load_recorded_ldt(Uc& machine, const RecordedCall& call, std::optional<std::uint32_t> data_base,
                                          std::map<std::uint16_t, std::uint64_t>& code_bases) {
    const bool snap = call.snapshot && !call.snapshot->memory.empty();
    const std::array<std::uint8_t, 8> scratch3 = descriptor(kScratchBase, kLimit, false, 3);
    const unsigned need = static_cast<unsigned>(std::max<std::uint32_t>(call.ss, call.data_selector.value_or(0)) >> 3);
    Bytes table;
    unsigned count = 0;
    auto fill_to = [&](unsigned entries) {
        for (; count < entries; ++count) put_descriptor(table, count, scratch3);
    };
    std::map<std::uint16_t, RecordedSegment> recorded = call.ldt;
    if (snap) {
        const Bytes& mem = call.snapshot->memory;
        machine.mem_map(kSnapshotBase, (mem.size() + 0xFFFF) & ~std::size_t{0xFFFF});
        machine.mem_write(kSnapshotBase, mem);
        const std::uint32_t at = call.snapshot->ldt_at;
        const unsigned n = (call.snapshot->ldt_limit + 1) / 8;
        fill_to(n);
        for (unsigned i = 1; i < n; ++i) {
            if (static_cast<std::uint64_t>(at) + 8u * i + 8 > mem.size()) continue;
            const ByteView d(mem.data() + at + 8u * i, 8);
            if (!((d[5] & 0x80) && (d[5] & 0x10))) continue;   // not present, or not code or data
            const std::uint32_t base = base_of(d);
            const std::uint32_t limit = static_cast<std::uint32_t>(d[0] | (d[1] << 8));
            if (static_cast<std::uint64_t>(base) + limit >= mem.size()) continue;
            if (d[5] & 8) {
                // Code of the running system -- Windows', another module's, a callback: not run, but called as an import is, in the
                // sentinel segment, answered and returned from. Conforming, so a far call from the tracer's level 0 to an RPL 3
                // selector is allowed; run where it was, a level-3 segment, it faulted on both sides.
                put_descriptor(table, i, conforming_code(kSentinelBase));
                continue;
            }
            put_descriptor(table, i, descriptor(kSnapshotBase + base, limit, false, 3));
        }
        recorded.clear();
        for (unsigned i = 1; i < n; ++i) {
            if (static_cast<std::uint64_t>(at) + 8u * i + 8 > mem.size()) continue;
            if ((mem[at + 8u * i + 5] & 0x98) == 0x98)
                code_bases[static_cast<std::uint16_t>((i << 3) | 7)] = kSnapshotBase + base_of(ByteView(mem.data() + at + 8u * i, 8));
        }
    } else {
        unsigned top = 1;
        for (const auto& [sel, segment] : recorded) top = std::max<unsigned>(top, sel >> 3);
        top = std::max<unsigned>({top, static_cast<unsigned>(call.ss >> 3), static_cast<unsigned>(call.data_selector.value_or(0) >> 3)});
        fill_to(top + 1);
    }
    fill_to(need + 1);
    auto put = [&](std::uint32_t sel, std::uint32_t base, bool code) { put_descriptor(table, sel >> 3, descriptor(base, kLimit, code, 3)); };
    unsigned k = 0;
    for (const auto& [sel, segment] : recorded) {   // (sorted by selector: each recorded segment's copy has a window of its own)
        const unsigned index = k++;
        if (!(sel & 4)) continue;
        const std::uint32_t base = kForeignBase + index * kSegmentStride;
        machine.mem_map(base, kSegmentStride);
        const std::size_t keep = std::min<std::size_t>(segment.bytes.size(), kSegmentStride);
        if (keep) machine.mem_write(base, ByteView(segment.bytes.data(), keep));
        put(sel, base, segment.code);
    }
    put(call.ss, kStackBase, false);
    if (call.data_selector && *call.data_selector && data_base) put(*call.data_selector, *data_base, false);
    if (snap) {
        for (const auto& [sel, segment] : call.own) {   // the module's own code segments: a window each, where a far call is sent on by name
            const std::uint32_t base = kOwnBase + segment * kSegmentStride;
            map_if_free(machine, base, kSegmentStride);
            put_descriptor(table, sel >> 3, conforming_code(base));
        }
    }
    machine.mem_map(kLdtAddress, (table.size() + emulator::kPage - 1) / emulator::kPage * emulator::kPage);
    machine.mem_write(kLdtAddress, table);
    machine.reg_write_descriptor(regs().ldtr, {0, kLdtAddress, static_cast<std::uint32_t>(table.size() - 1), 0x82});
    std::set<std::uint16_t> known;
    if (snap) {
        for (unsigned i = 1; i < table.size() / 8; ++i) known.insert(static_cast<std::uint16_t>((i << 3) | 7));
    } else {
        for (const auto& [sel, segment] : recorded)
            if (sel & 4) known.insert(sel);
    }
    known.insert(call.ss);
    if (call.data_selector && *call.data_selector) known.insert(*call.data_selector);
    return known;
}

std::uint32_t hash32(std::string_view text) {   // the first four bytes of the SHA-1 as a number: the made-up result of a call
    const crypto::Sha1Digest d = crypto::sha1(as_bytes(text));
    return (static_cast<std::uint32_t>(d[0]) << 24) | (static_cast<std::uint32_t>(d[1]) << 16) | (static_cast<std::uint32_t>(d[2]) << 8) | d[3];
}

}  // namespace

// ---- module functions ---------------------------------------------------------------------------------------------------------------

std::array<std::uint8_t, 8> descriptor(std::uint32_t base, std::uint32_t limit, bool code, unsigned dpl, bool big) {
    std::array<std::uint8_t, 8> d{};
    d[0] = static_cast<std::uint8_t>(limit & 0xFF);
    d[1] = static_cast<std::uint8_t>((limit >> 8) & 0xFF);
    d[2] = static_cast<std::uint8_t>(base & 0xFF);
    d[3] = static_cast<std::uint8_t>((base >> 8) & 0xFF);
    d[4] = static_cast<std::uint8_t>((base >> 16) & 0xFF);
    d[5] = static_cast<std::uint8_t>(0x80 | ((dpl & 3) << 5) | 0x10 | (code ? 0xA : 0x2));   // present, DPL, code or data
    d[6] = static_cast<std::uint8_t>(((big ? 0x4 : 0x0) << 4) | ((limit >> 16) & 0xF));      // flags nibble | limit 19:16
    d[7] = static_cast<std::uint8_t>((base >> 24) & 0xFF);
    return d;
}

std::uint32_t selector_base(Uc& machine, std::uint32_t selector_value) {
    try {
        return base_of(machine.mem_read(kGdtAddress + static_cast<std::uint64_t>(selector_value >> 3) * 8, 8));
    } catch (const UcFault&) {
        return 0;
    }
}

std::uint16_t Heaps::alloc_heap(std::uint32_t size) {
    // Take a GDT index near the top (the table was padded with scratch there; overwrite with a real data descriptor onto a fresh window)
    // and map that window.
    const unsigned index = kGdtSlots - 8 - made_;
    const std::uint32_t base = kHeapBase + made_ * kHeapStride;
    ++made_;
    map_if_free(machine_, base, (static_cast<std::uint64_t>(size) + 0xFFF) / 0x1000 * 0x1000);
    const std::array<std::uint8_t, 8> d = descriptor(base, kLimit, false);
    machine_.mem_write(kGdtAddress + static_cast<std::uint64_t>(index) * 8, ByteView(d));
    const std::uint16_t sel = selector(index);
    blocks_[sel] = {base, size};
    return sel;
}

std::string Trace16::exit_text() const {
    switch (ending) {
        case Ending::Returned: return "ret";
        case Ending::Truncated: return "truncated";
        case Ending::BadOrdinal: return "bad-ordinal";
        case Ending::Faulted: return "exception: " + fault_text;
    }
    return "ret";
}

ModelLookup models_of(knowledge::Knowledge& kb) {
    return [&kb](std::string_view api) -> std::optional<StubModel> {
        std::string up = text::upper(api);
        const std::size_t dot = up.find('.');
        std::string fn = dot == std::string::npos ? up : up.substr(dot + 1);
        // KERNEL's numbers for the functions modelled, for when the call is reported by number ("KERNEL.@15")
        if (fn.starts_with('@') && up.starts_with("KERNEL")) {
            if (const std::optional<std::int64_t> number = text::parse_int<std::int64_t>(std::string_view(fn).substr(1))) {
                const knowledge::Rows rows = kb.table("win16", "kernel_ordinal");
                if (const knowledge::Value* named = rows.find(std::to_string(*number)); named && named->text()) fn = *named->text();
            }
        }
        const std::optional<knowledge::Value> model = kb.api_model("win16", fn);
        if (!model) return std::nullopt;
        auto word = [&](std::string_view key) -> std::optional<ModelWord> {
            const knowledge::Value* v = model->get(key);
            if (!v) return std::nullopt;
            if (const std::optional<std::int64_t> number = v->integer()) return ModelWord{*number};
            if (const std::string* t = v->text()) {
                if (*t == "heap") return ModelWord{FromHeap{}};
                if (t->starts_with("arg")) {
                    if (const auto index = text::parse_int<std::size_t>(std::string_view(*t).substr(3))) return ModelWord{FromArgument{*index}};
                }
            }
            throw FormatError("win16 api_model", 0, std::format("{}: a stand-in value that is neither a number, \"heap\" nor \"argN\"", fn));
        };
        return StubModel{word("ax"), word("dx"), word("pop")};
    };
}

Run record(const std::shared_ptr<const Trace16>& trace) {
    const Trace16& t = *trace;
    Run run;
    run.exit = t.exit_text();
    const bool ran = t.ending != Ending::BadOrdinal;
    run.results.insert_or_assign("AX", ran ? Register{static_cast<std::uint64_t>(t.ax)} : Register{});
    run.results.insert_or_assign("DX:AX", ran ? Register{static_cast<std::uint64_t>(t.dx_ax)} : Register{});
    for (const native_trace::MemWrite& w : t.observed.mem_writes)
        run.writes.push_back({w.addr, static_cast<std::uint32_t>(w.size), w.value, w.old, false});
    for (const ImportHit& h : t.imports_hit) run.calls.push_back({h.api, h.args_stack, CallDetail{h}});
    if (t.io) {
        for (const ports16::LogEntry& e : *t.io) {
            const char* kind = e.kind == ports16::Access::In ? "in" : e.kind == ports16::Access::Out ? "out" : "...";
            run.io.push_back({e.step, kind, e.port, e.value, std::nullopt});
        }
    }
    if (ran) {
        run.stack = t.sp_delta;
        Preserved kept;   // SI DI DS as they went in and came out
        kept.insert_or_assign("SI", SavedRegister{t.entry_regs.si, t.exit_regs.si});
        kept.insert_or_assign("DI", SavedRegister{t.entry_regs.di, t.exit_regs.di});
        kept.insert_or_assign("DS", SavedRegister{t.entry_regs.ds, t.exit_regs.ds});
        run.preserved = std::move(kept);
        run.steps = static_cast<std::uint64_t>(t.observed.insn_count);
    }
    run.path = t.observed.path;
    run.truncated = t.observed.truncated;
    run.endian = Endian::Little;
    run.raw = trace;
    return run;
}

// ---- one run --------------------------------------------------------------------------------------------------------------------------

struct Runner::Impl {
    Environment env;
    std::mutex lock;   // guards the three tables below, which every thread asking this Runner shares
    std::map<std::tuple<std::string, std::uint64_t, std::int64_t>, std::shared_ptr<const Loaded>> loaded;   // (path, size, mtime) -> the module; lookup only
    std::map<std::pair<std::uint32_t, std::uint32_t>, Bytes> tails;                                         // (data base, segment end) -> the pattern to 64K
    std::map<std::pair<std::string, std::uint64_t>, std::uint32_t> retf;                                    // (recording, address) -> bytes its `retf n` removes

    std::unique_ptr<Uc> new_machine() const {
        std::unique_ptr<Uc> machine = env.open_engine ? env.open_engine() : unicorn_uc::open_x86(32);
        machine->speed_up();   // the same reads, for less
        return machine;
    }

    std::shared_ptr<const Loaded> module(const std::filesystem::path& path) {
        std::error_code ec;
        const std::uint64_t size = std::filesystem::file_size(path, ec);
        if (ec) throw NotFoundError(path, ec.value());
        const std::int64_t stamp = std::filesystem::last_write_time(path, ec).time_since_epoch().count();
        if (ec) throw NotFoundError(path, ec.value());
        const auto key = std::make_tuple(path.lexically_normal().generic_string(), size, stamp);
        {
            const std::lock_guard<std::mutex> hold(lock);
            if (const auto it = loaded.find(key); it != loaded.end()) return it->second;
        }
        std::shared_ptr<const Loaded> made = load_module(path);   // (two threads may both load it: the later keeps the earlier's)
        const std::lock_guard<std::mutex> hold(lock);
        return loaded.emplace(key, std::move(made)).first->second;
    }

    // The bytes of arguments the recorded system's code at selector:offset removes: its first `retf n` (0 for a plain retf, or none
    // found in the next 8K).
    std::uint32_t retf_bytes(const RecordedCall& call, std::uint64_t code_base, std::uint32_t offset) {
        if (!call.snapshot || call.snapshot->memory.empty()) return 0;
        const Bytes& mem = call.snapshot->memory;
        const std::uint64_t at = code_base - kSnapshotBase + offset;
        const auto key = std::make_pair(call.key, at);
        if (!call.key.empty()) {
            const std::lock_guard<std::mutex> hold(lock);
            if (const auto it = retf.find(key); it != retf.end()) return it->second;
        }
        std::uint32_t got = 0;
        if (at < mem.size()) {
            const ByteView chunk(mem.data() + at, std::min<std::size_t>(0x2000, mem.size() - static_cast<std::size_t>(at)));
            thread_local platforms::DecoderSet decoders;
            decoders.get(platforms::Arch::X86, 16, false).each(chunk, offset, 0, [&](const cs_insn& ins) {
                const std::string_view mn = ins.mnemonic;
                if (mn != "retf" && mn != "lret") return true;
                const std::string_view ops = ins.op_str;
                got = ops.empty() ? 0 : text::parse_int<std::uint32_t>(ops, 0).value_or(0);
                return false;
            });
        }
        if (!call.key.empty()) {
            const std::lock_guard<std::mutex> hold(lock);
            retf[key] = got;
        }
        return got;
    }

    // The pattern a data segment's tail is filled with: bytes [from, 64K) of a hash of the address.
    const Bytes& tail(std::uint32_t base, std::uint32_t from) {
        const std::lock_guard<std::mutex> hold(lock);
        auto it = tails.find({base, from});
        if (it == tails.end()) {
            Bytes t;
            for (std::uint32_t a = from; a < kWindow; ++a)
                t.push_back(static_cast<std::uint8_t>(((static_cast<std::uint64_t>(base) + a) * 0x9E3779B1ULL >> 13) & 0xFF));
            it = tails.emplace(std::make_pair(base, from), std::move(t)).first;
        }
        return it->second;
    }
};

namespace {

// Everything one run is: its machine, the module on it, the record being made, and the events the recorder hands up.
class Execution {
public:
    Execution(Runner::Impl& runner, std::shared_ptr<const Loaded> module, const Setup& setup, const ImportStub& stub)
        : runner_(runner), mod_(std::move(module)), setup_(setup), stub_(stub), machine_(runner.new_machine()), uc_(*machine_) {}

    // The function as the setup names it: its segment and offset; nullopt for an ordinal the entry table does not have.
    std::optional<CodePoint> locate(const Function& f) const {
        if (const auto* point = std::get_if<CodePoint>(&f)) {
            mod_->base_of_segment(point->segment);   // (UsageError for a segment the module does not have)
            return *point;
        }
        const std::uint32_t ordinal = std::get<Ordinal>(f).number;
        if (ordinal < 1 || ordinal > mod_->entries.size()) return std::nullopt;
        const formats::ne::Entry& e = mod_->entries[ordinal - 1];
        if (e.kind == formats::ne::Entry::Kind::Unused) return std::nullopt;
        mod_->base_of_segment(e.segment);
        return CodePoint{e.segment, e.offset};
    }

    Trace16 run(const CodePoint& where, const std::optional<std::uint32_t>& ordinal) {
        seg_ = where.segment;
        offset_ = where.offset;
        cs_base_ = mod_->base_of_segment(seg_);
        lay_out(uc_, *mod_);
        ds_sel_ = mod_->autodata ? mod_->selector_of(mod_->autodata) : selector(kStackSlot);
        if (ds_sel_ == 0) ds_sel_ = selector(kStackSlot);   // DGROUP; the stack's where the module has none
        const std::optional<std::uint32_t> data_base = mod_->data_base();
        std::vector<std::uint16_t> args = setup_.args;
        if (args.empty()) args.assign(setup_.arity, 0);
        const RecordedCall* context = setup_.context;
        if (context) known_ = load_recorded_ldt(uc_, *context, data_base, code_bases_);

        write_data(data_base);
        enter(where, args);

        rec_.module = mod_->name;
        rec_.ordinal = ordinal;
        rec_.segment = seg_;
        rec_.offset = offset_;
        rec_.args = args;
        rec_.layout.segment_bases = mod_->bases;
        rec_.layout.data_base = data_base;
        rec_.layout.autodata_segment = mod_->autodata;
        for (std::uint32_t i = 1; i <= mod_->bases.size(); ++i) {
            if (i != mod_->autodata) code_at_[static_cast<std::uint16_t>(mod_->selector_of(i) & ~3)] = mod_->bases[i - 1];
        }
        for (const auto& [place, callee] : setup_.callees) {
            places_[place] = callee.name;
            if (place.seg >= 1 && place.seg <= mod_->bases.size() && !(place.seg == seg_ && place.off == offset_))
                intercept_[mod_->bases[place.seg - 1u] + place.off] = &callee;
        }

        native_win16::Options o;
        o.eip_id = regs().eip;
        o.near_entry = setup_.near_entry;
        o.fill = setup_.fill_pages;
        o.cap = setup_.insn_cap;
        o.cs_base = cs_base_;
        o.near_ret = static_cast<std::uint64_t>(cs_base_) + kNearReturn;
        o.far_ret = static_cast<std::uint64_t>(kSentinelBase) + kFarReturn;
        o.sent_lo = kSentinelBase;
        o.sent_hi = static_cast<std::uint64_t>(kSentinelBase) + 0x10000;
        o.snap_base = kSnapshotBase;
        o.governor = runner_.env.governor;
        for (const auto& [address, callee] : intercept_) o.intercept.push_back(address);
        if (setup_.shadow) o.shadow = static_cast<tracer::Shadow*>(setup_.shadow->handle());
        if (context) o.own = native_trace::AddressRange{kOwnBase, static_cast<std::uint64_t>(kOwnBase) + 0x1000000};
        o.handlers.on_sentinel = [this](std::int64_t address, std::int64_t n) { sentinel(address, n); };
        o.handlers.on_intercept = [this](std::int64_t address, std::int64_t n) { answer(address, n); };
        o.handlers.on_interrupt = [this](std::int64_t number, std::int64_t n) { interrupt(number, n); };
        o.handlers.on_own = [this](std::int64_t address, std::int64_t n) { return own(address, n); };
        native_win16::Run recorder(static_cast<uc_struct*>(uc_.handle()), std::move(o));
        // (the branch log and the ports' device model are this module's own hooks, attached after the recorder's)
        std::unique_ptr<BranchLog> branch_log;
        if (setup_.branches) {
            rec_.branches.emplace();
            branch_log = std::make_unique<BranchLog>(*rec_.branches, cs_base_, *setup_.branches);
            BranchLog* log = branch_log.get();
            uc_.hook_add_code([log](Uc& machine, std::uint64_t address, std::uint32_t) { log->step(machine, address); },
                              static_cast<std::uint64_t>(cs_base_) + setup_.branches->lo, static_cast<std::uint64_t>(cs_base_) + setup_.branches->hi - 1);
        }
        std::unique_ptr<ports16::Devices> devices;
        std::unique_ptr<ports16::PortHooks> port_hooks;
        if (setup_.ports) {
            devices = std::make_unique<ports16::Devices>([&recorder] { return static_cast<std::uint64_t>(recorder.count()); });
            port_hooks = std::make_unique<ports16::PortHooks>(static_cast<uc_struct*>(uc_.handle()), *devices);
        }

        // The prelude: setup calls first (sharing this machine's DGROUP and the import stand-ins), their trace then discarded so only the
        // main function's memory and imports are captured.
        for (const PreludeCall& call : setup_.prelude) {
            const std::optional<CodePoint> at = locate(call.function);
            if (!at) throw UsageError("a prelude call by an ordinal the entry table does not have");
            std::vector<std::uint16_t> pargs = call.args;
            enter(*at, pargs);
            try {
                uc_.emu_start(at->offset, kSentinelBase, 0, static_cast<std::uint64_t>(setup_.insn_cap) + 1);
            } catch (const UcFault&) {   // a prelude that faults has done what it did
            }
            if (std::exception_ptr e = recorder.raised()) std::rethrow_exception(e);   // what a callback raised in it
        }
        if (!setup_.prelude.empty()) {
            recorder.reset();
            rec_.imports_hit.clear();
            rec_.prelude_ran = setup_.prelude.size();
            if (devices) devices->clear_log();   // the devices keep their state
            enter(where, args);
        }

        // The register file the main function sees at entry (the implicit inputs behind the decompiler's in_ / unaff_ pseudo-variables).
        const RegIds& r = regs();
        rec_.entry_regs = {low16(uc_.reg_read(r.eax)), low16(uc_.reg_read(r.ebx)), low16(uc_.reg_read(r.ecx)), low16(uc_.reg_read(r.edx)),
                           low16(uc_.reg_read(r.esi)), low16(uc_.reg_read(r.edi)), low16(uc_.reg_read(r.ebp)), low16(uc_.reg_read(r.cs)),
                           low16(uc_.reg_read(r.ds)), low16(uc_.reg_read(r.es)), low16(uc_.reg_read(r.ss))};
        if (setup_.shadow) {
            std::vector<native_shadow::Place> places;
            if (!context) {
                for (const auto& [at, value] : arg_places_) places.push_back({std::uint64_t{at}, std::uint64_t{value}, std::nullopt});
            }
            setup_.shadow->enter(places);
        }

        std::optional<std::string> failed;
        try {
            uc_.emu_start(offset_, kSentinelBase, 0, static_cast<std::uint64_t>(setup_.insn_cap) + 1);
        } catch (const UcFault& e) {
            failed = e.what();
        }
        rec_.observed = recorder.collect();   // the record, then what a callback raised in the run
        if (std::exception_ptr e = recorder.raised()) std::rethrow_exception(e);
        if (!failed) {
            // Neither returned nor stopped by a hook: emu_start's own instruction count ran out -- just short of the cap, the skipped
            // calls counting too. That is a run cut off, as much as one the hook stops: "stopped" made a long loop in each build look
            // like an ending.
            rec_.ending = rec_.observed.returned ? Ending::Returned : Ending::Truncated;
            if (rec_.observed.limit_fault) {
                rec_.ending = Ending::Faulted;
                rec_.fault_text = "past the end of a segment";
            }
            if (rec_.interrupt) failed = "Unhandled CPU exception (UC_ERR_EXCEPTION)";   // (the hook stopped it as Unicorn says it unhooked)
        }
        if (failed) {
            rec_.ending = Ending::Faulted;
            rec_.fault_text = *failed;
            describe_fault();
        }
        rec_.ax = low16(uc_.reg_read(r.eax));
        rec_.sp_delta = static_cast<std::uint16_t>(((uc_.reg_read(r.esp) & 0xFFFF) - entry_sp_) & 0xFFFF);   // the return address, plus any arguments removed
        rec_.code_base = cs_base_;
        rec_.exit_regs = {low16(uc_.reg_read(r.esi)), low16(uc_.reg_read(r.edi)), low16(uc_.reg_read(r.ds))};
        rec_.dx_ax = (static_cast<std::uint32_t>(low16(uc_.reg_read(r.edx))) << 16) | rec_.ax;
        rec_.n_imports = mod_->imports.size();
        if (devices) rec_.io = std::vector<ports16::LogEntry>(devices->log().begin(), devices->log().end());
        return std::move(rec_);
    }

    Uc& machine() { return uc_; }
    Trace16& record() { return rec_; }

private:
    // The data segment as the setup wants it: the image over its start, the original's tail, the controlled inputs.
    void write_data(const std::optional<std::uint32_t>& data_base) {
        if (!setup_.data_image.empty() && data_base) {
            const std::size_t keep = std::min<std::size_t>(setup_.data_image.size(), kWindow);
            uc_.mem_write(*data_base, ByteView(setup_.data_image.data(), keep));
        }
        if (setup_.data_end && data_base) {
            const std::uint32_t base = *data_base;
            const std::uint32_t start = setup_.data_image.empty() ? setup_.data_end->image_end : static_cast<std::uint32_t>(setup_.data_image.size());
            for (std::uint64_t page = (static_cast<std::uint64_t>(base) + start) & ~std::uint64_t{0xFFF}; page < static_cast<std::uint64_t>(base) + kWindow; page += 0x1000)
                map_if_free(uc_, page, 0x1000);
            if (setup_.data_end->segment_end > start) uc_.mem_write(base + start, Bytes(setup_.data_end->segment_end - start, 0));
            if (setup_.data_end->segment_end < kWindow) {
                const Bytes& tail = runner_.tail(base, setup_.data_end->segment_end);
                uc_.mem_write(base + setup_.data_end->segment_end, tail);
            }
        }
        for (const MemoryInit& init : setup_.mem_init) {
            std::uint64_t at = 0;
            if (const auto* in_data = std::get_if<DataOffset>(&init.where)) {
                if (!data_base || in_data->offset + init.data.size() > kWindow) continue;   // no data segment, or past its end
                at = static_cast<std::uint64_t>(*data_base) + in_data->offset;
            } else {
                at = std::get<std::uint32_t>(init.where);
            }
            try {
                uc_.mem_write(at, init.data);
            } catch (const UcFault&) {   // nothing mapped there: not an input
            }
        }
    }

    // Sets up the frame of one call and points CS:IP at it: Pascal arguments pushed left to right, then a far-return place in the
    // sentinel segment (a near return offset for a near function), DS and ES at DGROUP.
    void enter(const CodePoint& at, const std::vector<std::uint16_t>& args) {
        const RegIds& r = regs();
        if (setup_.context) return enter_recorded(at);
        std::uint32_t sp = 0xFFF0;
        arg_places_.clear();
        for (const std::uint16_t a : args) {
            sp = (sp - 2) & 0xFFFF;
            put_word(uc_, kStackBase + sp, a);
            arg_places_.emplace_back(kStackBase + sp, a);
        }
        if (setup_.near_entry) {
            sp = (sp - 2) & 0xFFFF;
            put_word(uc_, kStackBase + sp, kNearReturn);
        } else {
            // a far return goes to a place of its own in the sentinel segment (it once went to offset 0 of the function's own segment,
            // which ran whatever was there)
            sp = (sp - 2) & 0xFFFF;
            put_word(uc_, kStackBase + sp, selector(kStackSlot + 1));
            sp = (sp - 2) & 0xFFFF;
            put_word(uc_, kStackBase + sp, kFarReturn);
        }
        entry_sp_ = sp;
        uc_.reg_write(r.ss, selector(kStackSlot));
        uc_.reg_write(r.esp, sp);
        uc_.reg_write(r.ds, ds_sel_);
        uc_.reg_write(r.es, ds_sel_);
        uc_.reg_write(r.eax, kEntryAx);   // a function that sets no result returns it
        if (setup_.bp) uc_.reg_write(r.ebp, *setup_.bp);
        uc_.reg_write(r.cs, mod_->selector_of(at.segment));
        uc_.reg_write(r.eip, at.offset);
    }

    // The recorded call's own frame: its stack at its SP, its registers, its selectors.
    void enter_recorded(const CodePoint& at) {
        const RegIds& r = regs();
        const RecordedCall& c = *setup_.context;
        Bytes stack(c.stack.begin(), c.stack.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(c.stack.size(), kWindow)));
        stack.resize(kWindow, 0);
        uc_.mem_write(kStackBase, stack);
        const std::uint32_t sp = c.sp;
        if (setup_.near_entry) {
            put_word(uc_, kStackBase + sp, kNearReturn);
        } else {
            put_word(uc_, kStackBase + sp, kFarReturn);
            put_word(uc_, kStackBase + sp + 2, selector(kStackSlot + 1));
        }
        entry_sp_ = sp;
        // AX is the tracer's mark, not the recorded one: a function that sets no result hands back what AX held, and the recorded word
        // read as a result (sub_22_4008, a copy loop, "returned 1228h").
        uc_.reg_write(r.eax, kEntryAx);
        uc_.reg_write(r.ebx, c.regs.bx);
        uc_.reg_write(r.ecx, c.regs.cx);
        uc_.reg_write(r.edx, c.regs.dx);
        uc_.reg_write(r.esi, c.regs.si);
        uc_.reg_write(r.edi, c.regs.di);
        uc_.reg_write(r.ebp, c.regs.bp);
        // SS is the tracer's own: a stack selector must be of the running level, and the run's is 0; the recorded one, in the LDT, names the
        // same bytes for a pointer into the stack.
        uc_.reg_write(r.ss, selector(kStackSlot));
        uc_.reg_write(r.esp, sp);
        uc_.reg_write(r.ds, c.ds && known_.contains(*c.ds) ? *c.ds : ds_sel_);
        uc_.reg_write(r.es, c.es && known_.contains(*c.es) ? *c.es : ds_sel_);
        uc_.reg_write(r.cs, mod_->selector_of(at.segment));
        uc_.reg_write(r.eip, at.offset);
    }

    // A call into another of the module's functions, recorded and answered.
    void answer(std::int64_t address, std::int64_t n) {
        const RegIds& r = regs();
        const Callee& callee = *intercept_.at(static_cast<std::uint64_t>(address));
        std::uint32_t pop = 0;
        if (callee.removes) pop = *callee.removes;
        else rec_.unsure = true;   // how many it removes is not known: the stack after it is not either
        const std::uint32_t spn = static_cast<std::uint32_t>(uc_.reg_read(r.esp) & 0xFFFF);
        const std::uint16_t ip = word_at(uc_, kStackBase + spn);
        const std::uint32_t cs = callee.far ? word_at(uc_, kStackBase + spn + 2) : static_cast<std::uint32_t>(uc_.reg_read(r.cs));
        const std::uint32_t first = spn + (callee.far ? 4 : 2);
        std::vector<std::uint16_t> words;
        for (std::uint32_t k = 0; k < pop / 2; ++k) words.push_back(word_at(uc_, kStackBase + ((first + 2 * k) & 0xFFFF)));
        rec_.calls_out.push_back({callee.name, words, static_cast<std::uint16_t>(spn), n, ip});
        // The made-up result is made from the words as they compare between builds: a place in the stack, or the stack's or data's
        // selector, legitimately differs (USER's eleven RegisterClass callers kept a result made from their frame's address, and
        // "differed"). The text it is hashed from -- the name and the list of those words -- is fixed, so a recorded run replays.
        const std::uint64_t ss_now = uc_.reg_read(r.ss), ds_now = uc_.reg_read(r.ds);
        std::string same = "[";
        for (std::size_t i = 0; i < words.size(); ++i) {
            const std::uint16_t w = words[i];
            const std::optional<std::uint16_t> next = i + 1 < words.size() ? std::optional<std::uint16_t>(words[i + 1]) : std::nullopt;
            std::string item;
            std::string function;   // a callback: the function, wherever it sits
            if (next) {
                const std::int64_t segment = static_cast<std::int64_t>(*next >> 3) - kModuleSlot + 1;
                if (segment >= 0)
                    if (const auto it = places_.find({static_cast<std::uint32_t>(segment), w}); it != places_.end()) function = it->second;
            }
            if (!function.empty()) item = "'&" + function + "'";
            else if (w == ss_now) item = "'SS'";
            else if (w == ds_now) item = "'DS'";
            else if ((next && *next == ss_now) || (0xE000 <= w && w < 0xFFF0)) item = "'stack'";
            else item = std::to_string(w);
            same += (i ? ", " : "") + item;
        }
        same += "]";
        const std::uint32_t h = hash32(callee.name + same);
        // A place in the caller's stack handed over (`&local, SS`), the callee fills: a word made up the same way. Left as it was, the
        // caller read what its frame held before -- another thing in each build (LookupMenuHandle's result, written by sub_10_009C).
        if (setup_.shadow) {
            native_shadow::CallResult shadow_call;
            shadow_call.args_at = kStackBase + first;
            shadow_call.args = static_cast<std::int64_t>(words.size());
            shadow_call.why = "callee";
            setup_.shadow->after_call(uc_, shadow_call);
        }
        for (std::size_t i = 0; i + 1 < words.size(); ++i) {
            if (words[i + 1] == ss_now) {
                put_word(uc_, kStackBase + words[i], ((h >> 7) ^ (0x9E37u * static_cast<std::uint32_t>(i + 1))) & 0xFFFF);
                if (setup_.shadow) setup_.shadow->written(kStackBase + words[i], 2);
            }
        }
        uc_.reg_write(r.eax, h & 0xFFFF);
        uc_.reg_write(r.edx, (h >> 16) & 0xFFFF);
        uc_.reg_write(r.esp, (first + pop) & 0xFFFF);
        uc_.reg_write(r.cs, cs);
        uc_.reg_write(r.eip, ip);
    }

    // An import sentinel landing: a far call or jump reached the sentinel segment; the call is named, answered, and returned from.
    void sentinel(std::int64_t address, std::int64_t n) {
        const RegIds& r = regs();
        const std::uint32_t slot = static_cast<std::uint32_t>(address - kSentinelBase);
        const std::uint32_t spn = static_cast<std::uint32_t>(uc_.reg_read(r.esp) & 0xFFFF);
        const std::uint16_t ip = word_at(uc_, kStackBase + spn);
        const std::uint16_t cs = word_at(uc_, kStackBase + spn + 2);
        // The Pascal arguments pushed above the far-return address (for caller reconstruction).
        std::vector<std::int64_t> args_stack;
        for (std::uint32_t k = 0; k < 12; ++k) args_stack.push_back(word_at(uc_, kStackBase + ((spn + 4 + 2 * k) & 0xFFFF)));
        ImportHit hit;
        if (const auto it = mod_->imports.find(slot); it != mod_->imports.end()) hit = it->second;
        else hit.slot = slot;
        hit.args_stack = args_stack;
        hit.n = n;
        hit.ret_ip = ip;
        hit.ret_cs = cs;   // the caller's own code segment
        // And what a far pointer into one of the module's code segments points at: up to 32 bytes, to the first zero -- a string kept in
        // the code, at its own offset in each build.
        for (std::size_t k = 0; k + 1 < args_stack.size(); ++k) {
            const auto base = code_at_.find(static_cast<std::uint16_t>(args_stack[k + 1] & ~std::int64_t{3}));
            if (base == code_at_.end()) continue;
            try {
                Bytes s = uc_.mem_read(static_cast<std::uint64_t>(base->second) + static_cast<std::uint64_t>(args_stack[k]), 32);
                if (const auto zero = std::find(s.begin(), s.end(), std::uint8_t{0}); zero != s.end()) s.erase(zero, s.end());
                hit.pointed.emplace_back(k, std::move(s));
            } catch (const UcFault&) {   // the pointer is not into mapped code
            }
        }
        std::uint32_t called = static_cast<std::uint32_t>(uc_.reg_read(r.cs));
        std::size_t index = 0;
        StubResult res;
        if (setup_.context && called != selector(kStackSlot + 1)) {
            called |= 3;   // loaded as conforming code: RPL 0
            // A far call into the recorded system's code: named by where it went, the same in any build, and its arguments removed as that
            // code's own `retf n` removes them -- left on the stack, the frame below them came apart (sub_22_33B8 "lost" DI).
            const auto code = code_bases_.find(static_cast<std::uint16_t>(called));
            const std::uint32_t size = code == code_bases_.end() ? 0 : runner_.retf_bytes(*setup_.context, code->second, slot);
            ImportHit foreign;
            foreign.api = std::format("far {:04x}:{:04x}", called, slot);
            foreign.slot = slot;
            foreign.foreign = true;
            foreign.args_stack = args_stack;
            foreign.n = n;
            foreign.ret_ip = ip;
            foreign.arg_bytes = size;
            rec_.imports_hit.push_back(std::move(foreign));
            index = rec_.imports_hit.size() - 1;
            const std::uint32_t h = hash32(rec_.imports_hit[index].api);
            res = StubResult{h & 0xFFFF, (h >> 16) & 0xFFFF, size};
        } else {
            rec_.imports_hit.push_back(std::move(hit));
            index = rec_.imports_hit.size() - 1;
            if (stub_) {
                // (the stand-in may annotate the hit it is given: it stays in the record)
                if (const std::optional<StubResult> answered = stub_(rec_.imports_hit[index], uc_)) res = *answered;
            }
        }
        const std::uint32_t ax = res.ax.value_or(0) & 0xFFFF;
        const std::uint32_t dx = res.dx.value_or(0) & 0xFFFF;
        const std::uint32_t pop = res.pop.value_or(0);
        uc_.reg_write(r.esp, (spn + 4 + pop) & 0xFFFF);   // the far-return address and the callee's Pascal arguments
        uc_.reg_write(r.eax, (dx << 16) | ax);
        uc_.reg_write(r.edx, dx);
        uc_.reg_write(r.cs, cs);
        uc_.reg_write(r.eip, ip);
        if (setup_.shadow) {
            // An unmodelled call's result is an input, `ret`; a modelled one's is made from its arguments, which the shadow does not
            // follow into the model.
            const ImportHit& made = rec_.imports_hit[index];
            native_shadow::CallResult shadow_call;
            shadow_call.symbolic = made.unmodelled;
            shadow_call.value = (dx << 16) | ax;
            shadow_call.args_at = kStackBase + ((spn + 4) & 0xFFFF);
            shadow_call.args = pop / 2;
            if (!made.unmodelled) shadow_call.why = "call:" + made.api;
            setup_.shadow->after_call(uc_, shadow_call);
        }
    }

    // `int 21h` in a Windows module is DOS3Call by another door -- the same service, which Windows documents as the faster way in; a
    // module's own code picks one by a flag its start-up sets (LZEXPAND's sub_11_0042 tests CS:[71h]), and a rebuild in C calls DOS3Call.
    // So it is answered as that import is, recorded as that call. Any other interrupt, or a processor exception, ends the run.
    void interrupt(std::int64_t number, std::int64_t n) {
        const RegIds& r = regs();
        if (number == 0x21 && stub_) {
            ImportHit hit;
            hit.api = "KERNEL.@102";
            hit.n = n;
            hit.interrupt = 0x21;
            rec_.imports_hit.push_back(std::move(hit));
            const std::size_t index = rec_.imports_hit.size() - 1;
            const StubResult res = stub_(rec_.imports_hit[index], uc_).value_or(StubResult{});
            const std::uint32_t ax = res.ax.value_or(0) & 0xFFFF, dx = res.dx.value_or(0) & 0xFFFF;
            uc_.reg_write(r.eax, (dx << 16) | ax);
            uc_.reg_write(r.edx, dx);
            if (setup_.shadow) {
                const ImportHit& made = rec_.imports_hit[index];
                native_shadow::CallResult shadow_call;
                shadow_call.symbolic = made.unmodelled;
                shadow_call.value = (dx << 16) | ax;
                if (!made.unmodelled) shadow_call.why = "call:KERNEL.@102";
                setup_.shadow->after_call(uc_, shadow_call);
            }
            return;
        }
        rec_.interrupt = static_cast<std::uint8_t>(number);
        uc_.emu_stop();
    }

    // The module's own code reached through a recorded far pointer (a context's run): that function of this side's build, by name.
    // True when redirected.
    bool own(std::int64_t address, std::int64_t n) {
        const RegIds& r = regs();
        const std::uint64_t at = static_cast<std::uint64_t>(address);
        const auto it = setup_.context->redirect.find({static_cast<std::uint32_t>((at - kOwnBase) >> 16), static_cast<std::uint32_t>(at & 0xFFFF)});
        if (it == setup_.context->redirect.end()) return false;
        ImportHit hit;
        hit.api = "callback " + it->second.name;
        hit.n = n;
        rec_.imports_hit.push_back(std::move(hit));
        mod_->base_of_segment(it->second.segment);
        uc_.reg_write(r.cs, mod_->selector_of(it->second.segment));
        uc_.reg_write(r.eip, it->second.offset);
        return true;
    }

    // The instruction at the faulting CS:EIP (helps triage which 16-bit construct the segmented emulator still cannot handle).
    void describe_fault() {
        const RegIds& r = regs();
        try {
            const std::uint32_t eip = static_cast<std::uint32_t>(uc_.reg_read(r.eip) & 0xFFFF);
            const std::uint32_t cs = static_cast<std::uint32_t>(uc_.reg_read(r.cs));
            for (std::uint32_t i = 1; i <= mod_->bases.size(); ++i) {
                if (mod_->selector_of(i) != cs) continue;
                const Bytes fb = uc_.mem_read(static_cast<std::uint64_t>(mod_->bases[i - 1]) + eip, 16);
                thread_local platforms::DecoderSet decoders;
                const std::vector<platforms::Insn> insns = decoders.get(platforms::Arch::X86, 16, false).disasm(fb, eip, 1);
                Fault f;
                f.cs = cs;
                f.eip = eip;
                if (!insns.empty()) f.insn = std::string(text::trim(insns[0].mnemonic + " " + insns[0].op_str));
                f.bytes = crypto::to_hex(ByteView(fb.data(), 8));
                rec_.fault = std::move(f);
                break;
            }
        } catch (const Error&) {   // the instruction cannot be read: the fault has no detail
        }
    }

    Runner::Impl& runner_;
    std::shared_ptr<const Loaded> mod_;
    const Setup& setup_;
    const ImportStub& stub_;
    std::unique_ptr<Uc> machine_;
    Uc& uc_;
    Trace16 rec_;
    std::uint32_t seg_ = 0, offset_ = 0, cs_base_ = 0;
    std::uint16_t ds_sel_ = 0;
    std::uint32_t entry_sp_ = 0;
    std::vector<std::pair<std::uint32_t, std::uint16_t>> arg_places_;   // where each argument word is pushed, its value
    std::set<std::uint16_t> known_;                                     // the recorded selectors that name something here
    std::map<std::uint16_t, std::uint64_t> code_bases_;                 // the recorded system's code: selector -> linear base
    std::map<std::uint16_t, std::uint32_t> code_at_;                    // the module's code segments: selector (no RPL) -> base
    std::map<core16::SegOff, std::string> places_;                      // callee place -> name
    std::map<std::uint64_t, const Callee*> intercept_;                  // callee linear address -> callee
};

}  // namespace

// ---- the runner -------------------------------------------------------------------------------------------------------------------------

Runner::Runner(Environment environment) : impl_(std::make_unique<Impl>()) {
    if (!environment.governor) throw UsageError("a 16-bit runner needs a governor");
    impl_->env = std::move(environment);
}
Runner::~Runner() = default;

Trace16 Runner::trace(const std::filesystem::path& path, const TraceOptions& options) {
    const std::shared_ptr<const Loaded> module = impl_->module(path);
    Execution run(*impl_, module, options, options.import_stub);
    std::optional<std::uint32_t> ordinal;
    if (const auto* o = std::get_if<Ordinal>(&options.function)) ordinal = o->number;
    const std::optional<CodePoint> where = run.locate(options.function);
    if (!where) {
        Trace16 bad;
        bad.module = module->name;
        bad.ordinal = ordinal;
        bad.ending = Ending::BadOrdinal;
        return bad;
    }
    return run.run(*where, ordinal);
}

Trace16 Runner::probe(const std::filesystem::path& path, const ProbeOptions& options) {
    Setup setup = options;
    if (options.cdecl_args) std::reverse(setup.args.begin(), setup.args.end());   // pushed in Pascal order: C's right to left needs them turned
    std::optional<Heaps> heaps;   // made with the machine, at the first call
    auto wrapped = [&](ImportHit& hit, Uc& machine) -> std::optional<StubResult> {
        if (!heaps) heaps.emplace(machine);
        StubResult res = options.stub ? options.stub(hit, machine, *heaps) : default_stub(hit, *heaps);
        hit.unmodelled = !res.pop.has_value();   // its result: stub_ret, for the path explorer
        if (!res.pop && options.stub_ret) {      // not modelled: the result asked for
            res.ax = *options.stub_ret & 0xFFFF;
            res.dx = (*options.stub_ret >> 16) & 0xFFFF;
        }
        if (!res.pop) {   // remove the arguments, as the real function would
            std::optional<std::uint32_t> size;
            if (impl_->env.argument_bytes) size = impl_->env.argument_bytes(hit.api);
            if (size) hit.arg_bytes = *size;
            else hit.arg_bytes.reset();
            if (size && *size) res.pop = *size;
        } else {
            hit.arg_bytes = *res.pop;
        }
        return res;
    };
    const ImportStub stub = wrapped;
    const std::shared_ptr<const Loaded> module = impl_->module(path);
    Execution run(*impl_, module, setup, stub);
    std::optional<std::uint32_t> ordinal;
    if (const auto* o = std::get_if<Ordinal>(&setup.function)) ordinal = o->number;
    const std::optional<CodePoint> where = run.locate(setup.function);
    if (!where) {
        Trace16 bad;
        bad.module = module->name;
        bad.ordinal = ordinal;
        bad.ending = Ending::BadOrdinal;
        return bad;
    }
    Trace16 rec = run.run(*where, ordinal);
    if (heaps) {
        for (const auto& [sel, block] : heaps->blocks()) rec.heap_sels[sel] = block.base;
    }
    return rec;
}

Run Runner::recorded(const std::filesystem::path& module, const ProbeOptions& options) {
    return record(std::make_shared<const Trace16>(probe(module, options)));
}

StubResult Runner::default_stub(const ImportHit& hit, HeapContext& heap) const {
    const std::optional<StubModel> model = impl_->env.models ? impl_->env.models(hit.api) : std::nullopt;
    if (!model) return StubResult{0, std::nullopt, std::nullopt};   // unknown: a benign 0 (the stack may drift)
    auto value = [&](const std::optional<ModelWord>& word) -> std::optional<std::uint32_t> {
        if (!word) return std::nullopt;
        if (const auto* number = std::get_if<std::int64_t>(&*word)) return static_cast<std::uint32_t>(*number);
        if (std::holds_alternative<FromHeap>(*word)) return heap.alloc_heap();   // a new selector of the stand-in heap
        const std::size_t n = std::get<FromArgument>(*word).index;               // the Nth word of the arguments, as pushed
        return n < hit.args_stack.size() ? static_cast<std::uint32_t>(hit.args_stack[n]) : 0u;
    };
    return StubResult{value(model->ax), value(model->dx), value(model->pop)};
}

Bytes Runner::relocated_data(const std::filesystem::path& path) {
    const std::shared_ptr<const Loaded> module = impl_->module(path);   // (kept by the file's size and time: a replaced file is read again)
    Bytes out;
    if (module->autodata >= 1 && module->autodata <= module->segments.size() && module->segments[module->autodata - 1u].file_offset != 0) {
        const formats::ne::Segment& s = module->segments[module->autodata - 1u];
        out = module->images[module->autodata - 1u];
        out.resize(s.length, 0);   // the segment's length: what the loader keeps of its window
    }
    return out;
}

}  // namespace cb::emulate::windows16
