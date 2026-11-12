#include "cb/formats/pe_functions.hpp"

#include <algorithm>
#include <charconv>
#include <cstring>
#include <format>
#include <set>
#include <unordered_set>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"

namespace cb::formats::pe_functions {

namespace {

namespace cs = cb::disasm::capstone;

constexpr const char* kReader = "pe_functions";
constexpr std::size_t kMaxInstructionBytes = 15;

Va to_va(std::uint64_t value, const char* what) {
    if (value > 0xFFFFFFFFull) throw UnsupportedError(kReader, 0, std::format("{} 0x{:x} is not a 32-bit address", what, value));
    return static_cast<Va>(value);
}

// A 32-bit x86 Capstone decoder. Not thread-safe: Capstone keeps decoding state in the handle.
class Decoder {
public:
    explicit Decoder(bool detail) {
        const cs::Api& api = cs::api();
        cs::check(api.cs_open(CS_ARCH_X86, CS_MODE_32, &handle_), "cs_open");
        if (detail) cs::check(api.cs_option(handle_, CS_OPT_DETAIL, CS_OPT_ON), "cs_option");
        insn_ = api.cs_malloc(handle_);
    }
    Decoder(const Decoder&) = delete;
    Decoder& operator=(const Decoder&) = delete;
    ~Decoder() {
        const cs::Api& api = cs::api();
        if (insn_ != nullptr) api.cs_free(insn_, 1);
        api.cs_close(&handle_);
    }

    // The instruction at the front of `code`; `code` and `address` move past it. nullptr when nothing decodes there
    // (the next call would try the same bytes again). The result is valid until the next call.
    const cs_insn* next(const std::uint8_t*& code, std::size_t& size, std::uint64_t& address) {
        return cs::api().cs_disasm_iter(handle_, &code, &size, &address, insn_) ? insn_ : nullptr;
    }

private:
    csh handle_ = 0;
    cs_insn* insn_ = nullptr;
};

Instruction make_instruction(const cs_insn& c) {
    Instruction out;
    out.address = static_cast<Va>(c.address);
    out.size = static_cast<std::uint8_t>(c.size);
    out.mnemonic = c.mnemonic;
    out.op_str = c.op_str;
    if (c.detail != nullptr) {
        const cs_x86& x = c.detail->x86;
        out.disp = x.disp;
        out.disp_size = x.encoding.disp_size;
        out.disp_offset = x.encoding.disp_offset;
        out.imm_size = x.encoding.imm_size;
        out.imm_offset = x.encoding.imm_offset;
        for (int k = 0; k < x.op_count; ++k) {
            const cs_x86_op& o = x.operands[k];
            Operand op;
            switch (o.type) {
            case X86_OP_REG: op.kind = OperandKind::Register; break;
            case X86_OP_IMM:
                op.kind = OperandKind::Immediate;
                op.imm = o.imm;
                break;
            case X86_OP_MEM:
                op.kind = OperandKind::Memory;
                op.base = o.mem.base;
                op.index = o.mem.index;
                op.disp = o.mem.disp;
                break;
            default: break;
            }
            out.operands.push_back(op);
        }
    }
    return out;
}

// The target of `call imm`. With detail the first operand is read; without it, the operand text is a lone "0x..." number
// (how Capstone prints a direct near call).
std::optional<Va> call_target_of(const cs_insn& c, bool detail) {
    if (std::strcmp(c.mnemonic, "call") != 0) return std::nullopt;
    if (detail) {
        if (c.detail == nullptr || c.detail->x86.op_count < 1 || c.detail->x86.operands[0].type != X86_OP_IMM) return std::nullopt;
        const std::int64_t imm = c.detail->x86.operands[0].imm;
        if (imm < 0 || imm > 0xFFFFFFFFll) return std::nullopt;
        return static_cast<Va>(imm);
    }
    const std::string_view op = c.op_str;
    if (op.size() < 3 || op[0] != '0' || op[1] != 'x') return std::nullopt;
    std::uint64_t value = 0;
    const auto [end, ec] = std::from_chars(op.data() + 2, op.data() + op.size(), value, 16);
    if (ec != std::errc() || end != op.data() + op.size() || value > 0xFFFFFFFFull) return std::nullopt;
    return static_cast<Va>(value);
}

struct Carved {
    std::vector<Va> addresses, targets;   // ascending; the sweep's decoded addresses and every direct call target
    std::vector<Function> functions;
};

const CodeSection* section_holding(std::span<const CodeSection> sections, Va va) {
    for (const CodeSection& s : sections)
        if (s.contains(va)) return &s;
    return nullptr;
}

// The linear sweep, the function starts (exports, the entry point, direct-call targets in code, standard prologues) and
// the extents start-to-next-start. `detail`: read call targets from Capstone's operand detail (and keep the lists of
// decoded addresses and call targets); without it only the functions are wanted and the target is read off the operand text.
Carved carve_sections(std::span<const CodeSection> sections, const std::map<Va, std::string>& exports,
                      std::optional<Va> entry, bool detail) {
    Decoder decoder(detail);
    std::unordered_set<Va> decoded, called;

    // The sweep: every section from its start; a byte that does not decode is stepped over.
    for (const CodeSection& s : sections) {
        const std::uint8_t* p = s.data.data();
        std::size_t left = s.data.size();
        std::uint64_t address = s.va;
        while (left > 0) {
            const cs_insn* ins = decoder.next(p, left, address);
            if (ins == nullptr) {
                ++p;
                --left;
                ++address;
                continue;
            }
            decoded.insert(static_cast<Va>(ins->address));
            if (auto t = call_target_of(*ins, detail)) called.insert(*t);
        }
    }

    std::set<Va> starts;
    for (const auto& [va, name] : exports) starts.insert(va);
    if (entry) starts.insert(*entry);
    for (Va t : called)
        if (section_holding(sections, t) != nullptr) starts.insert(t);
    for (const CodeSection& s : sections) {
        const std::size_t n = s.data.size();
        const std::uint8_t* d = s.data.data();
        for (std::size_t i = 0; i + 4 < n; ++i) {   // a prologue starts at an index with four bytes behind it
            if (d[i] == 0x55 && d[i + 1] == 0x8b && d[i + 2] == 0xec) starts.insert(s.va + static_cast<Va>(i));
            else if (d[i] == 0x8b && d[i + 1] == 0xff && d[i + 2] == 0x55 && d[i + 3] == 0x8b && d[i + 4] == 0xec)
                starts.insert(s.va + static_cast<Va>(i));
        }
    }

    // A start the sweep did not decode (it began inside another instruction): decode from it until the sweep's listing.
    // Only a start something vouches for -- an export, the entry point, a call target -- is followed; a prologue is not.
    std::vector<Va> pending;
    for (Va s : starts)
        if (!decoded.contains(s)) pending.push_back(s);
    for (Va start : pending) {
        if (!(exports.contains(start) || called.contains(start) || (entry && start == *entry))) continue;
        const CodeSection* sec = section_holding(sections, start);
        if (sec == nullptr) continue;
        const std::size_t off = start - sec->va;
        const std::uint8_t* p = sec->data.data() + off;
        std::size_t left = sec->data.size() - off;
        std::uint64_t address = start;
        while (left > 0) {
            const cs_insn* ins = decoder.next(p, left, address);
            if (ins == nullptr) break;
            const Va at = static_cast<Va>(ins->address);
            if (decoded.contains(at) && at != start) break;
            if (detail) {
                if (auto t = call_target_of(*ins, detail)) called.insert(*t);
            }
            decoded.insert(at);
        }
    }

    Carved out;
    std::vector<Va> order;
    for (Va s : starts)
        if (decoded.contains(s)) order.push_back(s);
    if (detail) {
        out.addresses.assign(decoded.begin(), decoded.end());
        std::ranges::sort(out.addresses);
        out.targets.assign(called.begin(), called.end());
        std::ranges::sort(out.targets);
    }
    for (std::size_t i = 0; i < order.size(); ++i) {
        const CodeSection* s = section_holding(sections, order[i]);
        Va end = s->end();
        if (i + 1 < order.size() && order[i + 1] < end) end = order[i + 1];
        Function f;
        f.start = order[i];
        f.end = end;
        if (auto it = exports.find(order[i]); it != exports.end()) f.name = it->second;
        out.functions.push_back(std::move(f));
    }
    return out;
}

std::vector<CodeSection> code_sections_of(const pe::Image& image) {
    std::vector<CodeSection> out;
    for (const auto& c : image.code_sections()) {
        CodeSection s;
        s.va = to_va(c.va, "section address");
        s.data = image.section_data(*c.section);
        to_va(c.va + s.data.size(), "section end");
        s.name = c.section->name;
        out.push_back(std::move(s));
    }
    return out;
}

std::map<Va, std::string> exports_of(const pe::Image& image) {
    std::map<Va, std::string> out;
    for (const pe::Export& e : image.exports()) {
        const Va va = to_va(image.header().image_base + e.rva, "export address");
        out[va] = (e.name && !e.name->empty()) ? *e.name : std::format("Ord{}", e.ordinal);
    }
    return out;
}

void require_pe32(const pe::Image& image) {
    if (image.header().is_plus) throw UnsupportedError(kReader, 0, "a PE32+ image has no 32-bit code to carve");
    to_va(image.header().image_base, "image base");
}

std::optional<Va> entry_of(const pe::Image& image) {
    if (auto e = image.entry_va()) return to_va(*e, "entry point");
    return std::nullopt;
}

}  // namespace

std::optional<Va> Instruction::call_target() const {
    if (mnemonic != "call" || operands.empty() || operands[0].kind != OperandKind::Immediate) return std::nullopt;
    if (operands[0].imm < 0 || operands[0].imm > 0xFFFFFFFFll) return std::nullopt;
    return static_cast<Va>(operands[0].imm);
}

// ---- PeCarve ------------------------------------------------------------------------------------------------------

struct PeCarve::Lazy {
    std::mutex mutex;
    Decoder decoder{true};
    bool refs_done = false;
    std::map<Va, std::vector<Va>> refs;       // function start -> data addresses
    std::map<Va, std::vector<Va>> referrers;  // data address -> function starts
};

PeCarve::PeCarve(PeCarve&&) noexcept = default;
PeCarve& PeCarve::operator=(PeCarve&&) noexcept = default;
PeCarve::~PeCarve() = default;

PeCarve PeCarve::open(const std::filesystem::path& file) { return from_image(pe::Image::open(file)); }

PeCarve PeCarve::from_image(pe::Image image) {
    require_pe32(image);
    PeCarve c;
    c.image_ = std::make_unique<pe::Image>(std::move(image));   // sections point into the image's bytes: it stays put
    const pe::Image& img = *c.image_;
    c.image_base_ = to_va(img.header().image_base, "image base");
    c.sections_ = code_sections_of(img);
    for (const pe::Import& i : img.imports()) {
        if (i.slot_va > 0xFFFFFFFFull) continue;
        c.imports_.insert_or_assign(static_cast<Va>(i.slot_va), ImportName{i.dll, i.name});
    }
    c.exports_ = exports_of(img);
    Carved carved = carve_sections(c.sections_, c.exports_, entry_of(img), true);
    c.addresses_ = std::move(carved.addresses);
    c.call_targets_ = std::move(carved.targets);
    for (Function& f : carved.functions) {
        const Va start = f.start;
        c.functions_.emplace(start, std::move(f));
    }
    c.lazy_ = std::make_unique<Lazy>();
    return c;
}

const CodeSection* PeCarve::section_of(Va address) const { return section_holding(sections_, address); }

bool PeCarve::has_instruction(Va address) const { return std::ranges::binary_search(addresses_, address); }

namespace {

// The instruction at `address` of a section: Capstone reads at most 15 bytes.
std::optional<Instruction> decode_at(Decoder& decoder, const CodeSection& s, Va address) {
    const std::size_t off = address - s.va;
    const std::uint8_t* p = s.data.data() + off;
    std::size_t left = std::min(kMaxInstructionBytes, s.data.size() - off);
    std::uint64_t at = address;
    const cs_insn* ins = decoder.next(p, left, at);
    if (ins == nullptr) return std::nullopt;
    return make_instruction(*ins);
}

}  // namespace

std::optional<Instruction> PeCarve::instruction_at(Va address) const {
    if (!has_instruction(address)) return std::nullopt;
    const CodeSection* s = section_of(address);
    if (s == nullptr) return std::nullopt;
    std::lock_guard lock(lazy_->mutex);
    return decode_at(lazy_->decoder, *s, address);
}

std::optional<std::string> PeCarve::resolve_call(const Instruction& ins) const {
    if ((ins.mnemonic != "call" && ins.mnemonic != "jmp") || ins.operands.empty()) return std::nullopt;
    const Operand& op = ins.operands[0];
    if (op.kind == OperandKind::Immediate) {
        if (op.imm < 0 || op.imm > 0xFFFFFFFFll) return std::nullopt;
        const Va t = static_cast<Va>(op.imm);
        if (auto it = exports_.find(t); it != exports_.end()) return it->second;
        if (functions_.contains(t) || section_of(t) != nullptr) return std::format("sub_{:x}", t);
        return std::nullopt;
    }
    if (op.kind == OperandKind::Memory && op.base == 0 && op.index == 0) {
        // An absolute [disp] with no base or index is an IAT slot (or a global / loader-connected thunk pointer). Capstone
        // gives the displacement signed, so it is masked to 32 bits before matching the IAT map. A hit is a named import;
        // a miss is an internal indirect (the per-module QT_Thunk pointer) and honestly stays unresolved.
        if (const ImportName* imp = imports_.find(static_cast<Va>(op.disp))) return imp->dll + "!" + imp->name;
    }
    return std::nullopt;
}

std::vector<std::string> PeCarve::calls_in(Va start) const {
    std::vector<std::string> out;
    const auto f = functions_.find(start);
    if (f == functions_.end()) return out;
    for (Va va = start; va < f->second.end;) {
        const std::optional<Instruction> ins = instruction_at(va);
        if (!ins) {
            ++va;
            continue;
        }
        if (auto name = resolve_call(*ins)) out.push_back(std::move(*name));
        va += ins->size;
    }
    return out;
}

std::vector<DisasmLine> PeCarve::disassemble(Va start) const {
    std::vector<DisasmLine> rows;
    const auto f = functions_.find(start);
    if (f == functions_.end()) return rows;
    for (Va va = start; va < f->second.end;) {
        std::optional<Instruction> ins = instruction_at(va);
        if (!ins) {
            ++va;
            continue;
        }
        va += ins->size;
        rows.push_back({ins->address, std::move(ins->mnemonic), std::move(ins->op_str)});
    }
    return rows;
}

bool PeCarve::in_image(std::uint64_t address) const {
    const std::uint64_t base = image_->header().image_base;
    for (const pe::Section& s : image_->sections()) {
        const std::uint64_t lo = base + s.virtual_address;
        if (address >= lo && address < lo + std::max(s.virtual_size, s.raw_size)) return true;
    }
    return false;
}

const std::map<Va, std::vector<Va>>& PeCarve::data_refs() const {
    std::lock_guard lock(lazy_->mutex);
    if (lazy_->refs_done) return lazy_->refs;
    std::map<Va, std::set<Va>> found;
    for (Va addr : addresses_) {
        // the function the instruction lies in: the last start at or before it, if its extent reaches it
        auto it = functions_.upper_bound(addr);
        if (it == functions_.begin()) continue;
        --it;
        const Function& fn = it->second;
        if (addr >= fn.end) continue;
        const CodeSection* sec = section_of(addr);
        if (sec == nullptr) continue;
        const std::optional<Instruction> ins = decode_at(lazy_->decoder, *sec, addr);
        if (!ins) continue;
        const bool branch = ins->mnemonic == "call" || ins->mnemonic == "jmp" || ins->mnemonic.starts_with('j');
        for (const Operand& op : ins->operands) {
            std::optional<Va> target;
            if (op.kind == OperandKind::Immediate) {
                if (branch) continue;
                target = static_cast<Va>(op.imm);
            } else if (op.kind == OperandKind::Memory && op.base == 0 && op.index == 0) {
                const Va candidate = static_cast<Va>(op.disp);
                if (imports_.contains(candidate)) continue;
                target = candidate;
            }
            if (target && in_image(*target)) found[fn.start].insert(*target);
        }
    }
    for (const auto& [fs, vas] : found) {
        lazy_->refs[fs].assign(vas.begin(), vas.end());
        for (Va v : vas) lazy_->referrers[v].push_back(fs);   // `found` iterates functions ascending: each list is sorted
    }
    lazy_->refs_done = true;
    return lazy_->refs;
}

std::vector<Va> PeCarve::xrefs_to(Va address) const {
    data_refs();
    const auto it = lazy_->referrers.find(address);
    return it == lazy_->referrers.end() ? std::vector<Va>() : it->second;
}

std::map<Va, std::vector<std::pair<Va, std::string>>> PeCarve::string_refs(
    const std::unordered_map<Va, std::string>& text_at) const {
    std::map<Va, std::vector<std::pair<Va, std::string>>> out;
    for (const auto& [fs, vas] : data_refs()) {
        std::vector<std::pair<Va, std::string>> hits;
        for (Va v : vas)
            if (auto t = text_at.find(v); t != text_at.end()) hits.emplace_back(v, t->second);
        if (!hits.empty()) out.emplace(fs, std::move(hits));
    }
    return out;
}

// ---- the rest -------------------------------------------------------------------------------------------------------

std::shared_ptr<const PeCarve> CarveCache::get(const std::filesystem::path& file) {
    const std::string key = file.lexically_normal().generic_string();
    {
        std::lock_guard lock(mutex_);
        if (auto it = kept_.find(key); it != kept_.end()) return it->second;
    }
    auto carve = std::make_shared<const PeCarve>(PeCarve::open(file));
    std::lock_guard lock(mutex_);
    return kept_.emplace(key, std::move(carve)).first->second;
}

std::vector<Function> function_spans(const pe::Image& image) {
    require_pe32(image);
    const std::vector<CodeSection> sections = code_sections_of(image);
    return carve_sections(sections, exports_of(image), entry_of(image), false).functions;
}

std::vector<ListingRow> disassemble_listing(ByteView code, Va base) {
    std::vector<ListingRow> rows;
    Decoder decoder(false);
    std::size_t pos = 0;
    while (pos < code.size()) {
        const std::uint8_t* p = code.data() + pos;
        std::size_t left = std::min(kMaxInstructionBytes, code.size() - pos);
        std::uint64_t address = base + pos;
        const cs_insn* ins = decoder.next(p, left, address);
        ListingRow row;
        row.at = static_cast<std::uint32_t>(base + pos);
        if (ins == nullptr) {
            row.text = "(bad)";
            pos += 1;
        } else {
            row.text = ins->op_str[0] == '\0' ? std::string(ins->mnemonic) : std::format("{} {}", ins->mnemonic, ins->op_str);
            pos += ins->size;
        }
        rows.push_back(std::move(row));
    }
    return rows;
}

PeSeg PeSeg::open(const std::filesystem::path& file, std::string_view module, const std::optional<std::string>& section,
                  std::shared_ptr<const PeCarve> carve) {
    if (!carve) carve = std::make_shared<const PeCarve>(PeCarve::open(file));
    const pe::Image& image = carve->image();
    const std::span<const CodeSection> secs = carve->sections();
    if (secs.empty()) throw FormatError(kReader, 0, std::format("{}: no executable section", module));
    const CodeSection* pick = nullptr;
    if (!section) {   // the ".text" section, else the largest (the first of the largest)
        for (const CodeSection& s : secs)
            if (s.name == ".text") {
                pick = &s;
                break;
            }
        if (pick == nullptr) {
            pick = &secs[0];
            for (const CodeSection& s : secs)
                if (s.data.size() > pick->data.size()) pick = &s;
        }
    } else {
        for (const CodeSection& s : secs)
            if (s.name == *section) {
                pick = &s;
                break;
            }
        if (pick == nullptr) throw FormatError(kReader, 0, std::format("{}: no section '{}'", module, *section));
    }

    PeSeg seg;
    seg.module = module;
    seg.path = file;
    seg.base = carve->image_base();
    seg.va = pick->va;
    seg.code.assign(pick->data.begin(), pick->data.end());
    seg.name = pick->name;
    seg.rows = disassemble_listing(seg.code, 0);
    for (std::size_t i = 0; i < seg.rows.size(); ++i) seg.row_at.emplace(seg.rows[i].at, i);

    // The carve's functions that fall inside THIS section, as offsets from its start.
    const Va lo = pick->va, hi = pick->end();
    for (const auto& [start, f] : carve->functions()) {
        if (start < lo || start >= hi) continue;
        Function local;
        local.start = start - lo;
        local.end = std::min(f.end, hi) - lo;
        local.name = f.name;
        if (f.name && !f.name->empty()) seg.export_names.emplace(local.start, *f.name);
        seg.funcs.emplace(local.start, std::move(local));
    }
    seg.carve = std::move(carve);

    // The base-relocation mask, section-relative (each HIGHLOW covers 4 bytes).
    seg.reloc_mask.assign(seg.code.size(), false);
    const std::int64_t section_rva = static_cast<std::int64_t>(seg.va) - static_cast<std::int64_t>(seg.base);
    for (std::uint64_t rva : image.base_relocs()) {
        const std::int64_t o = static_cast<std::int64_t>(rva) - section_rva;
        if (o < 0 || o >= static_cast<std::int64_t>(seg.code.size())) continue;
        for (std::int64_t k = 0; k < 4 && o + k < static_cast<std::int64_t>(seg.code.size()); ++k)
            seg.reloc_mask[static_cast<std::size_t>(o + k)] = true;
    }
    return seg;
}

}  // namespace cb::formats::pe_functions
