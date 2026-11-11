#include "cb/analyze/native.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"

namespace cb::analyze {

namespace capstone = cb::disasm::capstone;

namespace {

constexpr std::int64_t kMaxTableEntries = 1024;
constexpr int kDecodeChunk = 48;           // bytes read at a time while following a run of code
constexpr int kLongestInstruction = 16;    // bytes per instruction, for sizing reads

// Words that cannot start real code (looks_like_code), and instructions only the system may run.
constexpr std::array<std::string_view, 26> kNever = {
    "arpl", "daa", "das", "aaa", "aas", "aam", "aad", "into", "bound", "salc", "xlatb", "les", "lds", "lcall",
    "ljmp", "iretd", "iret", "int1", "insb", "insd", "insw", "outsb", "outsd", "outsw", "cmc", "hlt"};
constexpr std::array<std::string_view, 11> kPrivileged = {"in", "out", "cli", "sti", "lgdt", "lidt",
                                                          "ltr", "lmsw", "clts", "invd", "wbinvd"};

bool one_of(std::string_view s, std::initializer_list<std::string_view> words) {
    return std::find(words.begin(), words.end(), s) != words.end();
}
template <std::size_t N>
bool one_of(std::string_view s, const std::array<std::string_view, N>& words) {
    return std::find(words.begin(), words.end(), s) != words.end();
}

bool is_return(std::string_view mn) { return one_of(mn, {"ret", "retf", "retn", "lret", "iret", "iretd"}); }

// `value` read as a signed number of `size` bytes.
Va signed_of(Va value, int size) {
    const int bits = 8 * size;
    if (bits <= 0 || bits > 63) return value;
    return (value >> (bits - 1)) != 0 ? value - (Va{1} << bits) : value;
}

// text.split(None, 1)[1]: what follows the mnemonic.
std::string_view after_mnemonic(std::string_view text) {
    constexpr std::string_view ws = " \t\r\n\f\v";
    const std::size_t a = text.find_first_not_of(ws);
    if (a == std::string_view::npos) return {};
    const std::size_t b = text.find_first_of(ws, a);
    if (b == std::string_view::npos) return {};
    const std::size_t c = text.find_first_not_of(ws, b);
    return c == std::string_view::npos ? std::string_view{} : text.substr(c);
}

std::string trimmed(std::string_view s) {
    constexpr std::string_view ws = " \t\r\n\f\v";
    const std::size_t a = s.find_first_not_of(ws);
    if (a == std::string_view::npos) return {};
    return std::string(s.substr(a, s.find_last_not_of(ws) - a + 1));
}

}  // namespace

// ---- registers -------------------------------------------------------------------------------------------------

std::string_view family(std::string_view reg) {
    struct Row {
        std::string_view whole;
        std::array<std::string_view, 5> parts;
    };
    static constexpr std::array<Row, 8> rows = {{{"eax", {"rax", "eax", "ax", "al", "ah"}},
                                                 {"ecx", {"rcx", "ecx", "cx", "cl", "ch"}},
                                                 {"edx", {"rdx", "edx", "dx", "dl", "dh"}},
                                                 {"ebx", {"rbx", "ebx", "bx", "bl", "bh"}},
                                                 {"esp", {"rsp", "esp", "sp", "spl", ""}},
                                                 {"ebp", {"rbp", "ebp", "bp", "bpl", ""}},
                                                 {"esi", {"rsi", "esi", "si", "sil", ""}},
                                                 {"edi", {"rdi", "edi", "di", "dil", ""}}}};
    for (const Row& row : rows)
        for (std::string_view part : row.parts)
            if (!part.empty() && part == reg) return row.whole;
    return reg;
}

// ---- the decoder -----------------------------------------------------------------------------------------------

// Capstone's handle for one image, and the names of the registers it has been asked for.
struct Image::Decoder {
    const capstone::Api& cs = capstone::api();
    csh handle = 0;
    int bits;
    std::unordered_map<unsigned, std::string> reg_names;

    explicit Decoder(int bits_in) : bits(bits_in) {
        capstone::check(cs.cs_open(CS_ARCH_X86, bits == 32 ? CS_MODE_32 : CS_MODE_16, &handle), "cs_open");
        capstone::check(cs.cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON), "cs_option");
    }
    Decoder(const Decoder&) = delete;
    Decoder& operator=(const Decoder&) = delete;
    ~Decoder() {
        if (handle != 0) cs.cs_close(&handle);
    }

    const std::string& reg(unsigned id) {
        auto it = reg_names.find(id);
        if (it != reg_names.end()) return it->second;
        const char* n = cs.cs_reg_name(handle, id);
        return reg_names[id] = (n != nullptr && *n != 0) ? n : "?";
    }

    // Disassemble up to `count` instructions (0: all that fit) from `code`, the first at address `va`.
    struct Batch {
        const Decoder* owner = nullptr;
        cs_insn* insns = nullptr;
        std::size_t count = 0;
        Batch() = default;
        Batch(const Batch&) = delete;
        Batch& operator=(const Batch&) = delete;
        ~Batch() {
            if (insns != nullptr) owner->cs.cs_free(insns, count);
        }
    };
    void disassemble(Batch& out, const std::uint8_t* code, std::size_t size, Va va, std::size_t count) const {
        out.owner = this;
        out.count = cs.cs_disasm(handle, code, size, static_cast<std::uint64_t>(va), count, &out.insns);
    }

    Instruction convert(const cs_insn& ci) {
        Instruction ins;
        ins.addr = static_cast<Va>(ci.address);
        ins.size = ci.size;
        ins.mnemonic = ci.mnemonic;
        ins.text = trimmed(std::string(ci.mnemonic) + " " + ci.op_str);
        const std::uint64_t wide = bits == 32 ? 0xFFFFFFFFull : 0xFFFFull;
        const cs_x86& x = ci.detail->x86;
        for (int k = 0; k < x.op_count; ++k) {
            const cs_x86_op& o = x.operands[k];
            Operand op;
            op.size = o.size;
            if (o.type == X86_OP_REG) {
                op.kind = Operand::Kind::Register;
                op.reg = reg(o.reg);
            } else if (o.type == X86_OP_IMM) {
                op.kind = Operand::Kind::Immediate;
                const std::uint64_t mask = (o.size == 1 || o.size == 2 || o.size == 4) ? ((1ull << (8 * o.size)) - 1) : wide;
                op.value = static_cast<Va>(static_cast<std::uint64_t>(o.imm) & mask);
            } else if (o.type == X86_OP_MEM) {
                op.kind = Operand::Kind::Memory;
                if (o.mem.base != 0) op.base = reg(o.mem.base);
                if (o.mem.index != 0) op.index = reg(o.mem.index);
                op.scale = o.mem.scale;
                op.value = o.mem.base != 0 ? static_cast<Va>(o.mem.disp)
                                           : static_cast<Va>(static_cast<std::uint64_t>(o.mem.disp) & wide);
                if (o.mem.segment != 0) op.seg = reg(o.mem.segment);
            } else {
                continue;
            }
            ins.operands.push_back(std::move(op));
        }
        if (ci.id != 0) {
            cs_regs rd{}, wr{};
            std::uint8_t nr = 0, nw = 0;
            if (cs.cs_regs_access(handle, &ci, rd, &nr, wr, &nw) == CS_ERR_OK) {
                for (int k = 0; k < nr; ++k) ins.reads.emplace(family(reg(rd[k])));
                for (int k = 0; k < nw; ++k) ins.writes.emplace(family(reg(wr[k])));
            }
        }
        const std::string& mn = ins.mnemonic;
        if (is_return(mn)) {
            ins.flow = Flow::Return;
        } else if (one_of(mn, {"call", "lcall", "jmp", "ljmp", "loop", "loope", "loopne", "jcxz", "jecxz"}) ||
                   (!mn.empty() && mn[0] == 'j')) {
            ins.flow = mn.ends_with("call") ? Flow::Call : mn.ends_with("jmp") ? Flow::Jump : Flow::CondJump;
            const int n = ci.size;
            const std::uint8_t* code = ci.bytes;
            if (bits == 16 && n >= 5 && (code[n - 5] == 0x9A || code[n - 5] == 0xEA) && mn[0] == 'l') {
                // a far call or jump to a constant segment:offset
                ins.target = (static_cast<Va>(code[n - 2] | (code[n - 1] << 8)) << 16) | (code[n - 4] | (code[n - 3] << 8));
            } else if (!ins.operands.empty() && ins.operands[0].kind == Operand::Kind::Immediate) {
                const Va v = ins.operands[0].value;
                ins.target = bits == 32 ? v : ((ins.addr & ~Va{0xFFFF}) | (v & 0xFFFF));
            }
        }
        return ins;
    }
};

// ---- construction and the address space ------------------------------------------------------------------------

Image::Image(ImageSpec spec)
    : kind_(spec.kind),
      bits_(spec.bits),
      flat_(spec.flat),
      has_pointers_(spec.has_pointers),
      data_segment_(spec.data_segment),
      spans_(std::move(spec.spans)),
      function_names_(std::move(spec.function_names)),
      imports_(std::move(spec.imports)),
      pointer_list_(std::move(spec.pointers)),
      roots_(std::move(spec.roots)),
      decoder_(std::make_unique<Decoder>(spec.bits)) {
    std::sort(spans_.begin(), spans_.end(), [](const Span& a, const Span& b) { return a.va < b.va; });
    for (const Span& s : spans_) span_starts_.push_back(s.va);
    for (const auto& [start, limit] : spec.functions) {
        functions_.push_back(start);
        limits_[start] = limit;
    }
    std::sort(functions_.begin(), functions_.end());
    for (const auto& [site, p] : pointer_list_) pointers_[site] = p;
}

Image::Image(Image&&) noexcept = default;
Image& Image::operator=(Image&&) noexcept = default;
Image::~Image() = default;

const Pointer* Image::pointer_at(Va va) const {
    const auto it = pointers_.find(va);
    return it == pointers_.end() ? nullptr : &it->second;
}

const Span* Image::span(Va va) const {
    const auto it = std::upper_bound(span_starts_.begin(), span_starts_.end(), va);
    if (it == span_starts_.begin()) return nullptr;
    const Span& s = spans_[static_cast<std::size_t>(it - span_starts_.begin()) - 1];
    return va < s.va + static_cast<Va>(s.data.size()) ? &s : nullptr;
}

Bytes Image::read(Va va, std::int64_t n) const {
    const Span* s = span(va);
    if (s == nullptr || n <= 0) return {};
    const Va off = va - s->va;
    const Va size = static_cast<Va>(s->data.size());
    const Va end = std::min(off + n, size);
    if (end <= off) return {};
    return Bytes(s->data.begin() + off, s->data.begin() + end);
}

std::optional<std::uint32_t> Image::u16(Va va) const {
    const Bytes b = read(va, 2);
    if (b.size() != 2) return std::nullopt;
    return static_cast<std::uint32_t>(b[0] | (b[1] << 8));
}

std::optional<std::uint32_t> Image::u32(Va va) const {
    const Bytes b = read(va, 4);
    if (b.size() != 4) return std::nullopt;
    return static_cast<std::uint32_t>(b[0]) | (static_cast<std::uint32_t>(b[1]) << 8) |
           (static_cast<std::uint32_t>(b[2]) << 16) | (static_cast<std::uint32_t>(b[3]) << 24);
}

bool Image::is_code(Va va) const {
    const Span* s = span(va);
    return s != nullptr && s->executable;
}

bool Image::is_data(Va va) const { return span(va) != nullptr && !imports_.contains(va); }

// ---- functions and names ---------------------------------------------------------------------------------------

Va Image::function_limit(Va start) const {
    const auto it = limits_.find(start);
    return it == limits_.end() ? start : it->second;
}

std::optional<Va> Image::owner(Va va) const {
    const auto it = std::upper_bound(functions_.begin(), functions_.end(), va);
    if (it == functions_.begin()) return std::nullopt;
    const Va f = *(it - 1);
    const auto l = limits_.find(f);
    if (l != limits_.end() && va < l->second) return f;
    return std::nullopt;
}

const std::string& Image::import_name(Va va) const {
    static const std::string none;
    const auto it = imports_.find(va);
    return it == imports_.end() ? none : it->second;
}

const std::string& Image::function_name(Va va) const {
    static const std::string none;
    const auto it = function_names_.find(va);
    return it == function_names_.end() ? none : it->second;
}

void Image::set_function_name(Va va, std::string name) { function_names_[va] = std::move(name); }

std::string Image::label(Va va) const {
    if (const std::string& n = function_name(va); !n.empty()) return n;
    if (const std::string& i = import_name(va); !i.empty()) return i;
    return std::format("{:x}", va);
}

// ---- decoding --------------------------------------------------------------------------------------------------

const Function& Image::function(Va start) {
    auto it = decoded_.find(start);
    if (it == decoded_.end()) it = decoded_.emplace(start, decode(start)).first;
    return *it->second;
}

Function Image::linear(Va va, int n) {
    Function fn;
    fn.start = fn.limit = fn.end = va;
    const Span* s = span(va);
    if (s == nullptr || n <= 0) return fn;
    const Va off = va - s->va;
    const Va len = std::min<Va>(static_cast<Va>(kLongestInstruction) * n, static_cast<Va>(s->data.size()) - off);
    Decoder::Batch batch;
    decoder_->disassemble(batch, s->data.data() + off, static_cast<std::size_t>(len), va, static_cast<std::size_t>(n));
    for (std::size_t k = 0; k < batch.count; ++k) fn.insns.push_back(decoder_->convert(batch.insns[k]));
    return fn;
}

const Instruction* Image::covering(Va va) {
    const std::optional<Va> f = owner(va);
    if (!f) return nullptr;
    const Function& fn = function(*f);
    const auto it = std::upper_bound(fn.insns.begin(), fn.insns.end(), va,
                                     [](Va v, const Instruction& i) { return v < i.addr; });
    if (it == fn.insns.begin()) return nullptr;
    const Instruction& prev = *(it - 1);
    return va < prev.addr + prev.size ? &prev : nullptr;
}

std::unique_ptr<Function> Image::decode(Va start) {
    auto fn = std::make_unique<Function>();
    fn->start = start;
    fn->limit = function_limit(start);
    std::map<Va, Instruction> seen;
    std::vector<Va> work{start};
    const Span* s = span(start);
    while (!work.empty() && s != nullptr) {
        Va at = work.back();
        work.pop_back();
        if (seen.contains(at) || !(start <= at && at < fn->limit)) continue;
        bool more = true;
        while (more && at < fn->limit) {   // a run of code, read a piece at a time
            const Va off = at - s->va;
            const Va size = static_cast<Va>(s->data.size());
            const Va hi = std::min({off + kDecodeChunk, fn->limit - s->va, size});
            bool has_next = false;
            Va next = 0;
            if (hi > off) {
                Decoder::Batch batch;
                decoder_->disassemble(batch, s->data.data() + off, static_cast<std::size_t>(hi - off), at, 0);
                for (std::size_t k = 0; k < batch.count; ++k) {
                    const cs_insn& ci = batch.insns[k];
                    if (seen.contains(static_cast<Va>(ci.address))) {
                        has_next = false;
                        break;
                    }
                    Instruction& ins = seen[static_cast<Va>(ci.address)] = decoder_->convert(ci);
                    next = static_cast<Va>(ci.address) + ci.size;
                    has_next = true;
                    if (ins.flow == Flow::Return || ins.mnemonic == "hlt" || ins.mnemonic == "ud2") {
                        has_next = false;
                        break;
                    }
                    if (ins.flow == Flow::Jump) {
                        if (ins.target) {
                            work.push_back(*ins.target);
                        } else if (!ins.operands.empty()) {
                            JumpTable table;
                            if (jump_table(*fn, ins, seen, table)) {
                                for (Va v : table.targets) work.push_back(v);
                                fn->tables.push_back(std::move(table));
                            }
                        }
                        has_next = false;
                        break;
                    }
                    if (ins.flow == Flow::CondJump && ins.target) work.push_back(*ins.target);
                }
            }
            at = next;
            more = has_next;
        }
    }
    fn->insns.reserve(seen.size());
    for (auto& kv : seen) fn->insns.push_back(std::move(kv.second));
    fn->end = fn->insns.empty() ? start : fn->insns.back().addr + fn->insns.back().size;
    return fn;
}

// Recognise the jump through a table at `ins`, a `jmp [table + reg*4]` (32-bit) or `jmp cs:[table + reg]` (16-bit),
// from the bound check and index arithmetic before it.
bool Image::jump_table(const Function& fn, const Instruction& ins, const std::map<Va, Instruction>& seen, JumpTable& out) {
    const Operand& op = ins.operands[0];
    if (op.kind != Operand::Kind::Memory) return false;
    Va table = 0;
    int width = 0;
    std::string r;
    if (bits_ == 32) {
        if (!op.base.empty() || op.index.empty() || op.scale != 4 || !is_data(op.value)) return false;
        table = op.value;
        width = 4;
        r = op.index;
    } else {
        r = !op.base.empty() ? op.base : op.index;
        if (op.seg != "cs" || r.empty() || op.size != 2) return false;
        table = (ins.addr & ~Va{0xFFFF}) | (op.value & 0xFFFF);
        width = 2;
    }
    // the (up to ten) instructions before the jump, oldest first
    std::vector<const Instruction*> back;
    {
        auto it = seen.lower_bound(ins.addr);
        while (it != seen.begin() && back.size() < 10) {
            --it;
            back.push_back(&it->second);
        }
        std::reverse(back.begin(), back.end());
    }
    std::optional<Va> count, low, default_target, byte_table;
    std::string selector(family(r));
    for (int i = static_cast<int>(back.size()) - 1; i >= 0; --i) {
        const Instruction& b = *back[static_cast<std::size_t>(i)];
        const bool has_next = static_cast<std::size_t>(i) + 1 < back.size();
        if ((b.mnemonic == "mov" || b.mnemonic == "movzx") && b.operands.size() == 2 &&
            b.operands[0].kind == Operand::Kind::Register && b.operands[1].kind == Operand::Kind::Memory &&
            family(b.operands[0].reg) == selector && b.operands[1].size == 1 && !byte_table &&
            !b.operands[1].base.empty() && bits_ == 32 && is_data(b.operands[1].value & 0xFFFFFFFF)) {
            // a byte table maps the selector to an entry: movzx eax, byte [index + BYTES]
            byte_table = b.operands[1].value & 0xFFFFFFFF;
            selector = family(b.operands[1].base);
        } else if (b.mnemonic == "cmp" && b.operands.size() == 2 && b.operands[1].kind == Operand::Kind::Immediate &&
                   !count && has_next &&
                   one_of(back[static_cast<std::size_t>(i) + 1]->mnemonic, {"ja", "jae", "jnbe", "jnb", "jg", "jge"})) {
            const Instruction& branch = *back[static_cast<std::size_t>(i) + 1];
            count = b.operands[1].value + (one_of(branch.mnemonic, {"ja", "jnbe", "jg"}) ? 1 : 0);
            default_target = branch.target;
            const std::string_view rest = after_mnemonic(b.text);
            const std::size_t comma = rest.rfind(',');
            selector = std::string(comma == std::string_view::npos ? rest : rest.substr(0, comma));
            for (int k = std::max(0, i - 3); k < i; ++k) {   // `sub reg, K` / `dec reg` before the bound
                const Instruction& p = *back[static_cast<std::size_t>(k)];
                if (!p.operands.empty() && p.operands[0].kind == Operand::Kind::Register &&
                    b.operands[0].kind == Operand::Kind::Register && family(p.operands[0].reg) == family(b.operands[0].reg)) {
                    const bool imm2 = p.operands.size() > 1 && p.operands[1].kind == Operand::Kind::Immediate;
                    if (p.mnemonic == "sub" && imm2) low = p.operands[1].value;
                    else if (p.mnemonic == "dec") low = 1;
                    else if (p.mnemonic == "add" && imm2) low = -signed_of(p.operands[1].value, p.operands[1].size);
                    else if (p.mnemonic == "lea" && p.operands.size() > 1 && p.operands[1].kind == Operand::Kind::Memory &&
                             !p.operands[1].base.empty() && p.operands[1].index.empty())
                        low = -p.operands[1].value;
                }
            }
            break;
        }
    }
    if (count && !(0 < *count && *count <= kMaxTableEntries)) count.reset();
    std::optional<std::vector<std::uint8_t>> index;
    std::optional<std::int64_t> entries;
    if (byte_table && count && *count != 0) {
        index = read(*byte_table, *count);
        entries = static_cast<std::int64_t>(index->size()) == *count ? (*std::max_element(index->begin(), index->end())) + 1 : 0;
    } else {
        entries = count;
    }
    std::vector<Va> targets;
    const std::int64_t n = entries ? *entries : kMaxTableEntries;
    for (std::int64_t i = 0; i < n; ++i) {
        const Va at = table + width * i;
        const std::optional<std::uint32_t> raw = width == 4 ? u32(at) : u16(at);
        if (!raw) break;
        Va v = *raw;
        if (width == 2) v |= ins.addr & ~Va{0xFFFF};
        // a table of unknown length ends where something else begins: a function start or code already decoded
        if (!(fn.start <= v && v < fn.limit) || (!entries && i != 0 && (is_function(at) || seen.contains(at)))) break;
        targets.push_back(v);
    }
    if (targets.empty() || (entries && static_cast<std::int64_t>(targets.size()) != *entries)) return false;
    out.site = ins.addr;
    out.table = table;
    out.targets = std::move(targets);
    out.low = low.value_or(0);
    out.selector = selector;
    out.default_target = default_target;
    out.index_bytes = std::move(index);
    out.bounded = count.has_value();
    return true;
}

// ---- discovery -------------------------------------------------------------------------------------------------

std::vector<Va> Image::discover() {
    std::vector<Va> added;
    std::vector<Va> pending = functions_;
    while (true) {
        std::set<Va> fresh;
        for (Va start : pending)
            for (const Instruction& ins : function(start).insns)
                if (ins.flow == Flow::Call && ins.target && !is_function(*ins.target) && !is_import(*ins.target) &&
                    is_code(*ins.target))
                    fresh.insert(*ins.target);
        if (fresh.empty()) {
            fresh = held_code_addresses();
            if (fresh.empty()) break;
        }
        std::set<Va> redo(fresh);
        for (Va va : fresh) {
            discovered_order_.push_back(va);
            added.push_back(va);
            const auto pos = std::lower_bound(functions_.begin(), functions_.end(), va);
            const std::size_t i = static_cast<std::size_t>(pos - functions_.begin());
            const Span* s = span(va);
            CB_ASSERT(s != nullptr, "a discovered start lies in a span");
            Va end = s->va + static_cast<Va>(s->data.size());
            if (kind_ == ImageKind::Ne) end = std::min(end, s->va + std::max<Va>(s->initialised, 1));
            limits_[va] = (i < functions_.size() && functions_[i] < end) ? functions_[i] : end;
            if (i != 0 && limits_[functions_[i - 1]] > va) {   // the function before now ends where this one starts
                const Va prev = functions_[i - 1];
                limits_[prev] = va;
                decoded_.erase(prev);
                redo.insert(prev);
            }
            functions_.insert(functions_.begin() + static_cast<std::ptrdiff_t>(i), va);
        }
        discovered_set_.insert(fresh.begin(), fresh.end());
        pending.assign(redo.begin(), redo.end());
    }
    return added;
}

// Addresses the file marks as addresses that point at code no known function reaches.
std::set<Va> Image::held_code_addresses() {
    std::set<Va> fresh;
    if (!has_pointers_ || pointers_.empty() || bits_ != 32) return fresh;
    for (const auto& entry : pointer_list_) {
        const Va site = entry.first;
        const Pointer& p = pointers_.at(site);
        if (p.kind != PointerKind::Abs32 || !p.target) continue;
        const Va t = *p.target;
        if (is_function(t) || is_import(t) || !is_code(t)) continue;
        if (covering(t) != nullptr) continue;   // a label inside a function, or not code at all
        if (is_code(site)) {
            const Instruction* ins = covering(site);
            if (ins == nullptr) {
                // an address in the code that no instruction holds: a table of addresses among the code, believed
                // when it is a run of them and one of them is a function we know
                std::vector<Va> run{site};
                for (const Va step : {Va{-4}, Va{4}}) {
                    Va at = site + step;
                    while (pointers_.contains(at) && run.size() < 256) {
                        run.push_back(at);
                        at += step;
                    }
                }
                bool known = false;
                for (Va a : run) {
                    const Pointer& q = pointers_.at(a);
                    if (q.target && is_function(*q.target)) known = true;
                }
                if (run.size() < 2 || !known) continue;
            } else {
                // an instruction in the code holds the address as an immediate and is not a branch: a handler handed on
                bool held = false;
                for (const Operand& op : ins->operands)
                    if (op.kind == Operand::Kind::Immediate && op.value == t) held = true;
                if (!held || ins->flow != Flow::None) continue;
            }
        }
        if (looks_like_code(t)) fresh.insert(t);
    }
    return fresh;
}

// Does the code at `va` read like a function: no instruction a compiler never emits, no run of zero bytes, no
// address the loader writes in the middle of an instruction, and a return or a jump within `most` instructions.
bool Image::looks_like_code(Va va, int most) {
    const Span* s = span(va);
    const Bytes head = read(va, 8);
    if (s == nullptr || head.size() < 8) return false;
    if ((head[0] == 0 && head[1] == 0) || (head[0] == 0xCC && head[1] == 0xCC) || (head[0] == 0x90 && head[1] == 0x90))
        return false;
    bool printable = true;
    for (std::size_t k = 0; k < 4; ++k)
        if (!(0x20 <= head[k] && head[k] < 0x7F)) printable = false;
    if (printable) return false;
    const Va off = va - s->va;
    const Va len = std::min<Va>(static_cast<Va>(kLongestInstruction) * most, static_cast<Va>(s->data.size()) - off);
    Decoder::Batch batch;
    decoder_->disassemble(batch, s->data.data() + off, static_cast<std::size_t>(len), va, 0);
    int n = 0;
    for (std::size_t k = 0; k < batch.count; ++k) {
        const cs_insn& ci = batch.insns[k];
        const std::string_view mn = ci.mnemonic;
        const std::size_t sp = mn.find_last_of(" \t");
        const std::string_view last = sp == std::string_view::npos ? mn : mn.substr(sp + 1);   // after a rep/lock prefix
        if (one_of(last, kNever) || (flat_ && one_of(mn, kPrivileged)) || (ci.size >= 2 && ci.bytes[0] == 0 && ci.bytes[1] == 0))
            return false;
        if (has_pointers_ && !pointers_.empty()) {   // an address the loader writes must be an operand
            const cs_x86_encoding& e = ci.detail->x86.encoding;
            const Va a0 = static_cast<Va>(ci.address);
            const Va fit1 = e.disp_size == 4 ? a0 + e.disp_offset : -1;
            const Va fit2 = e.imm_size == 4 ? a0 + e.imm_offset : -1;
            for (Va a = a0; a < a0 + ci.size; ++a)
                if (pointers_.contains(a) && a != fit1 && a != fit2) return false;
        }
        ++n;
        if (is_return(mn) || mn == "jmp") return mn != "retf";
        if (n >= most) return true;
    }
    return false;   // ran out of bytes without a verdict
}

// ---- what a function holds at a call ---------------------------------------------------------------------------

namespace {

Argument argument_of(const Operand& op) {
    Argument a;
    a.kind = op.kind == Operand::Kind::Register    ? Argument::Kind::Register
             : op.kind == Operand::Kind::Immediate ? Argument::Kind::Immediate
                                                   : Argument::Kind::Memory;
    a.reg = op.reg;
    a.value = op.value;
    return a;
}

}  // namespace

std::optional<Argument> definition(const Function& fn, int i, std::string_view reg, int depth) {
    const std::string f(family(reg));
    for (int j = i - 1; j > std::max(-1, i - 1 - depth); --j) {
        const Instruction& ins = fn.insns[static_cast<std::size_t>(j)];
        if (ins.flow == Flow::Jump || ins.flow == Flow::Return) return std::nullopt;
        if (ins.flow == Flow::Call) {
            if (f == "eax") {   // the result of the call just made
                Argument a;
                a.kind = Argument::Kind::CallResult;
                if (ins.target) a.call_target = ins.target;
                else if (!ins.operands.empty() && ins.operands[0].kind == Operand::Kind::Memory &&
                         ins.operands[0].base.empty() && ins.operands[0].index.empty())
                    a.call_target = ins.operands[0].value;   // the slot it calls through
                return a;
            }
            if (f == "ecx" || f == "edx") return std::nullopt;   // clobbered by the call
            continue;
        }
        if (!ins.writes.contains(f)) continue;
        const auto& ops = ins.operands;
        if (ops.size() == 2 && ops[0].kind == Operand::Kind::Register && family(ops[0].reg) == f) {
            if (ins.mnemonic == "mov" || ins.mnemonic == "movzx") {
                if (ops[1].kind == Operand::Kind::Register) {
                    if (auto inner = definition(fn, j, ops[1].reg, depth - (i - j))) return inner;
                }
                return argument_of(ops[1]);
            }
            if (ins.mnemonic == "lea" && ops[1].kind == Operand::Kind::Memory) {
                Argument a;
                a.kind = Argument::Kind::LoadAddress;
                return a;
            }
            if ((ins.mnemonic == "xor" || ins.mnemonic == "sub") && ops[1].kind == Operand::Kind::Register &&
                ops[1].reg == ops[0].reg) {
                Argument a;
                a.kind = Argument::Kind::Immediate;
                a.value = 0;
                return a;
            }
        }
        if (ins.mnemonic == "pop" && j != 0 && fn.insns[static_cast<std::size_t>(j) - 1].mnemonic == "push") {   // push K / pop reg
            const Instruction& push = fn.insns[static_cast<std::size_t>(j) - 1];
            if (push.operands.empty()) return std::nullopt;
            return argument_of(push.operands[0]);
        }
        return std::nullopt;
    }
    return std::nullopt;
}

std::vector<Argument> arguments(const Function& fn, int i, std::size_t n) {
    std::vector<Argument> out;
    for (int j = i - 1; j > std::max(-1, i - 60); --j) {
        if (out.size() >= n) break;
        const Instruction& ins = fn.insns[static_cast<std::size_t>(j)];
        if (ins.flow == Flow::Call || ins.flow == Flow::Jump || ins.flow == Flow::Return ||
            one_of(ins.mnemonic, {"pop", "leave", "enter", "pushal", "popal"}))
            break;
        if (ins.mnemonic == "push" && !ins.operands.empty()) {
            const Operand& op = ins.operands[0];
            std::optional<Argument> held;
            if (op.kind == Operand::Kind::Register) held = definition(fn, j, op.reg, 14);
            out.push_back(held ? std::move(*held) : argument_of(op));
        } else if ((ins.mnemonic == "add" || ins.mnemonic == "sub") && !ins.operands.empty() &&
                   ins.operands[0].kind == Operand::Kind::Register && family(ins.operands[0].reg) == "esp") {
            break;   // the stack pointer moved: the pushes before it are another call's
        }
    }
    return out;
}

}  // namespace cb::analyze
