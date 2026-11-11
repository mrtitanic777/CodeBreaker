#include "cb/build/asm16.hpp"

#include <algorithm>
#include <cstdlib>
#include <format>

#include "cb/build/msc6.hpp"
#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::build::asm16 {

namespace ne = cb::formats::ne;
namespace assembly = cb::source::assembly;

namespace {

constexpr std::uint16_t kSegmentUse32 = 0x2000;   // NE segment flag: a 32-bit code segment (GDI's 36 and 38, its flat thunks)
// Source types of a relocation record.
constexpr unsigned kSourceByte = 0, kSourceSelector = 2, kSourceFar = 3, kSourceOffset = 5;
// What the loader allocates past the stored bytes is zeros it then fixes up: a far jump at the very end of a segment
// has its selector there (GDI's segment 15). Up to this many bytes are taken.
constexpr std::size_t kMaxLoaderPadding = 16;
constexpr std::size_t kLongestInsn = 16;
constexpr int kNear = 24;   // a far pointer's selector is within this many bytes of its offset

// ---- decoding -----------------------------------------------------------------------------------------------------

struct Operand {
    int type = 0;
    std::int64_t imm = 0;
    unsigned segment = 0, base = 0, index = 0;   // of a memory operand
    std::int64_t disp = 0;
};

// What this module reads of Capstone's record.
struct Decoded {
    std::uint32_t size = 0;
    std::array<std::uint8_t, kLongestInsn> bytes{};
    std::string mnemonic, op_str;
    bool jump_or_call = false;
    std::vector<Operand> operands;
};

// The instruction at the start of `window` (which is at address `at`), or nullopt where it does not decode.
std::optional<Decoded> decode_one(const platforms::Decoder& md, ByteView window, std::uint32_t at) {
    std::optional<Decoded> out;
    md.each(window, at, 1, [&](const cs_insn& c) {
        Decoded d;
        d.size = c.size;
        std::copy_n(c.bytes, std::min<std::size_t>(c.size, d.bytes.size()), d.bytes.begin());
        d.mnemonic = c.mnemonic;
        d.op_str = c.op_str;
        if (c.detail) {
            for (int g = 0; g < c.detail->groups_count; ++g) {
                if (c.detail->groups[g] == CS_GRP_JUMP || c.detail->groups[g] == CS_GRP_CALL) d.jump_or_call = true;
            }
            const cs_x86& x = c.detail->x86;
            for (int k = 0; k < x.op_count; ++k) {
                const cs_x86_op& o = x.operands[k];
                d.operands.push_back(Operand{.type = static_cast<int>(o.type),
                                             .imm = o.imm,
                                             .segment = static_cast<unsigned>(o.mem.segment),
                                             .base = static_cast<unsigned>(o.mem.base),
                                             .index = static_cast<unsigned>(o.mem.index),
                                             .disp = o.mem.disp});
            }
        }
        out = std::move(d);
        return false;
    });
    return out;
}

// ---- small text helpers ---------------------------------------------------------------------------------------------

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// Names are not empty when they are names (a symbol_for that says "" says none).
std::optional<std::string> named(std::optional<std::string> s) {
    if (s && s->empty()) return std::nullopt;
    return s;
}

std::string db_bytes(ByteView b) {
    std::string out;
    for (std::size_t i = 0; i < b.size(); ++i) {
        if (i) out += ", ";
        out += std::format("0{:02X}h", b[i]);
    }
    return out;
}

std::string_view linkage_text(Linkage k) {
    switch (k) {
        case Linkage::Near: return "near";
        case Linkage::Far: return "far";
        case Linkage::Byte: return "byte";
    }
    return "near";
}

std::string pad_right(std::string s, std::size_t width) {
    if (s.size() < width) s.append(width - s.size(), ' ');
    return s;
}

// "call near ptr X" / "jmp  near ptr X": the mnemonic padded to four.
std::string transfer_text(bool call, std::string_view symbol) {
    return std::format("{} near ptr {}", pad_right(call ? "call" : "jmp", 4), symbol);
}

// `ops` with every whole-word 0x<hex of disp> (as the decoder writes it, lower case) made the label.
std::string replace_hex_word(std::string_view ops, std::uint32_t disp, std::string_view label) {
    const std::string token = std::format("0x{:x}", disp);
    std::string out;
    std::size_t i = 0;
    while (i < ops.size()) {
        const bool here = ops.compare(i, token.size(), token) == 0 && (i == 0 || !is_word(ops[i - 1])) &&
                          (i + token.size() == ops.size() || !is_word(ops[i + token.size()]));
        if (here) {
            out += label;
            i += token.size();
        } else {
            out += ops[i++];
        }
    }
    return out;
}

// `[0x1234]` with no segment before it is `ds:[0x1234]`.
std::string default_ds(std::string_view ops) {
    std::string out;
    std::size_t i = 0;
    while (i < ops.size()) {
        if (ops[i] == '[') {
            const bool segment_before = i >= 2 && ops[i - 1] == ':' && ops[i - 2] >= 'a' && ops[i - 2] <= 'z';
            std::size_t k = i + 3;
            if (!segment_before && ops.compare(i + 1, 2, "0x") == 0) {
                while (k < ops.size() && ((ops[k] >= '0' && ops[k] <= '9') || (ops[k] >= 'a' && ops[k] <= 'f'))) ++k;
                if (k > i + 3 && k < ops.size() && ops[k] == ']') {
                    out += "ds:";
                    out.append(ops.substr(i, k + 1 - i));
                    i = k + 1;
                    continue;
                }
            }
        }
        out += ops[i++];
    }
    return out;
}

// `text` ending in the immediate `value` written as 0...hhhh h (the assembler's hexadecimal): that immediate made `offset name`.
// nullopt when the text does not end that way.
std::optional<std::string> offset_in_place_of_immediate(std::string_view text, std::uint32_t value, std::string_view name) {
    if (text.empty() || (text.back() != 'h' && text.back() != 'H')) return std::nullopt;
    std::size_t word = text.size();
    while (word > 0 && is_word(text[word - 1])) --word;   // the word that ends the text
    std::string_view token = text.substr(word, text.size() - 1 - word);
    const std::string digits = std::format("{:X}", value);
    std::size_t zeros = 0;
    while (zeros < token.size() && token[zeros] == '0') ++zeros;
    token.remove_prefix(zeros);
    if (!text::iequals(token, digits)) return std::nullopt;
    return std::string(text.substr(0, word)) + "offset " + std::string(name);
}

// ---- relocations ---------------------------------------------------------------------------------------------------

struct Place {
    std::uint32_t offset = 0;
    unsigned source = 0;
    std::uint32_t addend = 0;
    const ne::Relocation* record = nullptr;
};

// Every relocated place of a segment, following the chains the file keeps (each place holds the offset of the next;
// 0xFFFF ends one). An additive record names one place and carries its addend in it.
std::vector<Place> relocated_places(const std::vector<ne::Relocation>& records, ByteView blob) {
    std::vector<Place> out;
    for (const ne::Relocation& r : records) {
        std::uint32_t off = r.offset;
        for (int hops = 0; off != 0xFFFF && off + 1 < blob.size() && hops < 0x10000; ++hops) {
            const std::uint16_t word = static_cast<std::uint16_t>(blob[off] | (blob[off + 1] << 8));
            out.push_back(Place{off, r.source, r.additive() ? word : 0u, &r});
            if (r.additive()) break;
            off = word;
        }
    }
    return out;
}

// KERNEL's huge-pointer constants, __AHSHIFT (113) and __AHINCR (114), taken as a byte or a word: the number the
// loader writes there in protected mode, 3 and 8. (`db low offset _KERNEL_113` does not assemble: SETUPX's segment 28,
// its decompressor, fell back to C over it -- and faulted 15% into copying files.)
std::optional<unsigned> huge_pointer_constant(const Which& what) {
    if (const auto* n = std::get_if<std::uint16_t>(&what)) {
        if (*n == 113) return 3;
        if (*n == 114) return 8;
        return std::nullopt;
    }
    const std::string& s = std::get<std::string>(what);
    if (s == "__AHSHIFT") return 3;
    if (s == "__AHINCR") return 8;
    return std::nullopt;
}

std::string which_text(const Which& w) {
    return std::holds_alternative<std::uint16_t>(w) ? std::to_string(std::get<std::uint16_t>(w)) : std::get<std::string>(w);
}

// The offset of an instruction is in a set of places?  (does [from, to) hold any member)
template <class Set>
bool any_in(const Set& set, std::uint32_t from, std::uint32_t to) {
    auto it = set.lower_bound(from);
    return it != set.end() && *it < to;
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------------

Function::Function(const ne::Module& module, const Request& request, const SymbolFor& symbol_for, platforms::DecoderSet& decoders) {
    build(module, request, symbol_for, decoders);
}

void Function::build(const ne::Module& module, const Request& request, const SymbolFor& symbol_for, platforms::DecoderSet& decoders) {
    if (request.publics.empty()) throw UsageError("asm16::Function: a request needs at least one public symbol");
    const std::vector<ne::Segment> segs = module.segments();
    if (request.seg == 0 || request.seg > segs.size()) {
        throw UsageError(std::format("asm16::Function: segment {} of {} segments", request.seg, segs.size()));
    }
    const std::uint32_t start = request.start;
    std::uint32_t end = request.end;
    const std::uint16_t seg = request.seg;

    // the holes do not overlap: the last one starting at or before x. An empty one has nothing to place.
    for (const Hole& h : request.holes) {
        if (h.start < h.end) holes_.push_back(h);
    }
    std::sort(holes_.begin(), holes_.end());
    const auto hole_at = [&](std::uint32_t x) -> const Hole* {
        auto it = std::upper_bound(holes_.begin(), holes_.end(), x, [](std::uint32_t v, const Hole& h) { return v < h.start; });
        if (it == holes_.begin()) return nullptr;
        --it;
        return x < it->end ? &*it : nullptr;
    };
    publics_ = request.publics;
    std::sort(publics_.begin(), publics_.end());
    const std::string public_symbol = publics_.front().symbol;

    const ne::Segment& s = segs[seg - 1];
    // a 32-bit code segment is read as 32-bit code -- read as 16-bit, `66 B8 iw` took the next two bytes for its
    // immediate and the `mov gs, ax` after it became a near call far past the segment's end. It is written into the
    // 16-bit code class all the same, each instruction as its bytes with its 32-bit spelling beside them: the linker
    // keeps a class's 16- and 32-bit segments apart, and a use32 segment went after all of the original's, renumbering
    // both (and it sets no 2000h flag in a Windows module either way -- docs/NATIVE.md, the linker's backlog)
    bits_ = (s.flags & kSegmentUse32) ? 32 : 16;
    if (s.file_offset != 0) {
        // (a file cut short gives a shorter segment: the range check below says so)
        const ByteView data = module.data();
        const std::uint64_t from = std::min<std::uint64_t>(s.file_offset, data.size());
        const std::uint64_t to = std::min<std::uint64_t>(s.file_offset + s.length, data.size());
        blob_.assign(data.begin() + static_cast<std::ptrdiff_t>(from), data.begin() + static_cast<std::ptrdiff_t>(to));
    }
    if (!blob_.empty() && s.min_alloc > blob_.size() && s.min_alloc - blob_.size() <= kMaxLoaderPadding) {
        if (end == blob_.size()) end = s.min_alloc;
        blob_.resize(s.min_alloc, 0);
    }
    if (!(start < end && end <= blob_.size())) {
        why_ = "its bytes are not in the file";
        return;
    }
    start_ = start;
    end_ = end;
    for (const ne::Segment& x : segs) {
        if (!x.is_data()) code_segments_[x.index] = x.length;
    }
    std::map<std::uint32_t, ne::Entry> entries;   // by ordinal
    for (const ne::Entry& e : module.entries()) {
        if (e.kind == ne::Entry::Kind::Fixed || e.kind == ne::Entry::Kind::Moveable) entries[e.ordinal] = e;
    }
    const std::vector<std::string> refs = module.module_refs();

    std::map<std::uint32_t, std::uint16_t> internal_selector;   // offset -> segment: a selector of the module's own
    const std::vector<ne::Relocation> records = module.relocations(s);
    for (const Place& place : relocated_places(records, blob_)) {
        const std::uint32_t off = place.offset;
        if (off < start || off >= end || hole_at(off)) continue;
        const unsigned src = place.source;
        std::uint32_t width = 0;
        switch (src) {
            case kSourceByte: width = 1; break;
            case kSourceSelector: width = 2; break;
            case kSourceFar: width = 4; break;
            case kSourceOffset: width = 2; break;
            default: break;
        }
        if (width == 0 || off + width > end) {
            why_ = std::format("a fix-up of kind {} at {:04x}", src, off);
            return;
        }
        const ne::Relocation& r = *place.record;
        const std::uint32_t add = place.addend;
        std::string sym;
        bool internal = false;
        std::uint16_t tseg = 0;
        if (const auto* ord = std::get_if<ne::ImportOrdinalRef>(&r.target); ord || std::holds_alternative<ne::ImportNameRef>(r.target)) {
            const std::uint16_t module_index = ord ? ord->module : std::get<ne::ImportNameRef>(r.target).module;
            std::optional<std::string> mod;
            if (module_index > 0 && module_index <= refs.size()) mod = refs[module_index - 1];
            const Which what = ord ? Which(ord->ordinal) : Which(module.imported_name(std::get<ne::ImportNameRef>(r.target).name_offset));
            if (mod && *mod == "KERNEL" && (src == kSourceByte || src == kSourceOffset)) {
                if (const auto konst = huge_pointer_constant(what)) {
                    const std::uint32_t value = (*konst + (r.additive() ? add : 0)) & (src == kSourceByte ? 0xFFu : 0xFFFFu);
                    sites_[off] = Directive{width, src == kSourceByte ? std::format("db   0{:02X}h", value) : std::format("dw   0{:04X}h", value)};
                    continue;
                }
            }
            std::optional<std::string> found;
            if (mod && !mod->empty()) found = named(symbol_for(ImportRef{*mod, what}));
            if (!found) {
                why_ = std::format("an import ({}.{}) with no symbol", mod.value_or("?"), which_text(what));
                return;
            }
            sym = *found;
            imports_[sym] = Import{*mod, what};
            externs_[sym] = Linkage::Far;
        } else if (const auto* in = std::get_if<ne::InternalRef>(&r.target)) {
            std::uint32_t toff = 0;
            if (in->segment == 0xFF) {
                const auto e = entries.find(in->entry);
                if (e == entries.end()) {
                    why_ = std::format("a fix-up to entry {}, which the module does not have", in->entry);
                    return;
                }
                tseg = e->second.segment;
                toff = e->second.offset;
            } else {
                tseg = in->segment;
                toff = in->entry;
            }
            toff = (toff + add) & 0xFFFF;
            const bool is_data = tseg > 0 && tseg <= segs.size() && segs[tseg - 1].is_data();
            std::optional<std::string> found;
            if (is_data) {   // a data segment: the auto one or one carried
                found = named(symbol_for(DataRef{tseg}));
                if (found) {
                    externs_[*found] = Linkage::Byte;
                    if (src != kSourceSelector && toff) *found += std::format(" + {}", toff);
                }
            } else if (src == kSourceSelector) {
                found = named(symbol_for(SegmentRef{tseg}));
                if (found) externs_[*found] = Linkage::Far;
            } else {
                found = named(symbol_for(FunctionRef{tseg, toff}));
                if (found) externs_[*found] = Linkage::Far;
            }
            if (!found) {
                why_ = std::format("a fix-up to {}:{:04x}, where nothing has a symbol", tseg, toff);
                return;
            }
            sym = *found;
            internal = true;
        } else {
            why_ = std::format("an operating-system fix-up (floating point) at {:04x}", off);
            return;
        }
        std::string text;
        switch (src) {
            case kSourceSelector: text = "dw   seg " + sym; break;
            case kSourceFar: text = "dd   " + sym; break;
            case kSourceOffset: text = "dw   offset " + sym; break;
            default: text = "db   low offset " + sym; break;
        }
        if (add && !internal && src != kSourceSelector) text += std::format(" + {}", add);
        sites_[off] = Directive{width, text};
        if (internal && src == kSourceSelector) internal_selector[off] = tseg;
    }

    // the instructions: near calls and jumps leaving the function, and references to its own segment
    const platforms::Decoder& md = decoders.get(platforms::Arch::X86, bits_, true);
    std::map<std::uint32_t, std::string> sym_of;   // site offset -> the symbol text of its directive
    for (const auto& [off, d] : sites_) {
        const std::size_t cut = d.text.find(' ');
        if (cut == std::string::npos) {
            sym_of[off] = d.text;
        } else {
            sym_of[off] = std::string(text::ltrim(std::string_view(d.text).substr(cut)));
        }
    }
    // The instructions are found by following the code from its entry points (each function of the run, and any
    // other place called into from outside): what nothing reaches is data and stays bytes. A jump through a table in
    // the run is followed into the table.
    std::set<std::uint32_t> entry_points;
    for (const PublicSymbol& p : publics_) entry_points.insert(p.offset);
    for (const std::uint32_t e : request.extra_entries) entry_points.insert(e);
    std::vector<std::uint32_t> work(entry_points.begin(), entry_points.end());
    std::set<std::uint32_t> seen;
    missing_.clear();
    tables_.clear();
    const ByteView blob(blob_);
    const auto site_in = [&](std::uint32_t from, std::uint32_t to) {
        const auto it = sites_.lower_bound(from);
        return it != sites_.end() && it->first < to;
    };
    const std::uint32_t mask_address = bits_ == 16 ? 0xFFFFu : 0xFFFFFFFFu;
    while (!work.empty()) {
        std::uint32_t at = work.back();
        work.pop_back();
        while (start <= at && at < end && !seen.contains(at) && !hole_at(at)) {
            const std::size_t room = std::min<std::size_t>(end, at + kLongestInsn) - at;
            const std::optional<Decoded> decoded = decode_one(md, blob.subspan(at, room), at);
            if (!decoded || at + decoded->size > end || hole_at(at + decoded->size - 1)) break;   // (nothing runs on into a function rebuilt from C)
            const Decoded& ins = *decoded;
            seen.insert(at);
            const bool covered = site_in(at, at + ins.size);   // (its few bytes, looked up)
            const std::uint8_t b0 = ins.bytes[0];
            const std::string& mn = ins.mnemonic;
            if (covered) {
                // the forms with a symbolic spelling: a far call or jump to a symbol, a symbol's selector or offset
                // loaded or pushed
                const std::uint32_t site = at + 1;
                const auto site_at = sites_.find(site);
                if ((b0 == 0x9A || b0 == 0xEA) && ins.size == 5 && site_at != sites_.end() && site_at->second.size == 4) {
                    symbolic_[at] = Directive{5, std::format("{} far ptr {}", b0 == 0x9A ? "call" : "jmp", sym_of[site])};
                } else if ((b0 == 0x9A || b0 == 0xEA) && ins.size == 5 && internal_selector.contains(at + 3)) {
                    // a far call into the module: the file relocates only its selector; its offset is plain bytes --
                    // the target's, which moves with it: the whole is the name
                    const std::uint16_t target_seg = internal_selector[at + 3];
                    const std::uint32_t target_off = blob_u16(at + 1);
                    const std::optional<std::string> name = named(symbol_for(FunctionRef{target_seg, target_off}));
                    if (!name) {
                        far_missing_.insert({target_seg, target_off});
                        why_ = std::format("a far {} to {}:{:04x}, where no function has a symbol", mn, target_seg, target_off);
                        if (target_seg == seg) missing_.insert(target_off);
                        return;
                    }
                    externs_[*name] = Linkage::Far;
                    symbolic_[at] = Directive{5, std::format("{} far ptr {}", b0 == 0x9A ? "call" : "jmp", *name)};
                    sites_.erase(at + 3);
                } else if (((b0 >= 0xB8 && b0 <= 0xBF) || b0 == 0x68) && ins.size == 3 && site_at != sites_.end() && site_at->second.size == 2) {
                    static constexpr std::string_view kRegisters[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
                    const std::string& what = sym_of[site];   // `seg X` or `offset X`
                    symbolic_[at] = Directive{3, b0 != 0x68 ? std::format("mov  {}, {}", kRegisters[b0 - 0xB8], what) : "push " + what};
                }
            } else {
                for (const Operand& op : ins.operands) {
                    if (op.type != X86_OP_MEM || op.segment != X86_REG_CS) continue;
                    const std::uint32_t disp = static_cast<std::uint32_t>(op.disp) & 0xFFFF;
                    const bool plain = op.base == 0 && op.index == 0;
                    if (plain && disp < request.cs_ok) continue;   // the segment's lead-in: where it was
                    if (request.whole && plain) continue;           // the whole segment is where it was
                    if (hole_at(disp)) {
                        why_ = std::format("it reads a function rebuilt from C through CS ({} {} at {:04x})", mn, ins.op_str, at);
                        reaches_ = disp;   // (that one is the assembly's own: a table)
                        return;
                    }
                    if (start <= disp && disp < end && ins.size >= 3) {
                        cs_labels_[at] = disp;   // a table or a variable of the run: a label
                        if ((mn == "jmp" || mn == "call") && op.index == 0 && op.base != 0 && bits_ == 16) {
                            // a jump or call table: its words are places in the run to go on from (it ends, at the
                            // latest, where the first place it sends to after it begins: past that is the code it
                            // goes on to)
                            std::uint32_t t = disp, first = end;
                            while (start <= t && t < std::min(end - 1, first) && !seen.contains(t)) {
                                const std::uint32_t target = blob_u16(t);
                                if (!(start <= target && target < end)) break;
                                if (target > disp) first = std::min(first, target);
                                tables_.insert(t);
                                work.push_back(target);
                                t += 2;
                            }
                        }
                        continue;
                    }
                    if (request.whole) continue;
                    why_ = std::format("it reads its own segment through CS ({} {} at {:04x})", mn, ins.op_str, at);
                    return;
                }
            }
            std::optional<std::uint32_t> target;
            if (!covered && ins.jump_or_call && !ins.operands.empty() && ins.operands[0].type == X86_OP_IMM && b0 != 0x9A && b0 != 0xEA) {
                const std::uint32_t t = static_cast<std::uint32_t>(ins.operands[0].imm) & mask_address;
                target = t;
                const Hole* h = hole_at(t);
                if (h && t == h->start && (b0 == 0xE8 || b0 == 0xE9)) {
                    externs_[h->symbol] = Linkage::Near;   // a function rebuilt from C: by its name
                    calls_[at] = Directive{ins.size, transfer_text(b0 == 0xE8, h->symbol)};
                } else if (h) {
                    why_ = std::format("a jump into the middle of {}, rebuilt from C ({} at {:04x})", h->symbol, mn, at);
                    reaches_ = t;
                    return;
                } else if (start <= t && t < end) {
                    ins_[at] = Insn{Bytes(ins.bytes.begin(), ins.bytes.begin() + ins.size), mn, ins.op_str, t};
                    work.push_back(t);
                } else if (t >= blob_.size()) {
                    // (past the end of the segment: no code jumps or calls there -- bytes read as code. NETDI's segment
                    // 54 ends in strings far pointers name ("NETDI: ", "netlog.txt"), described as functions: `ja
                    // 316C` in a segment 310A long refused all 666 of its functions. Followed no further: bytes. A
                    // near call there was taken for one to a function at that place: an entry no piece defines, a
                    // stand-in in another segment, and a link that failed -- "relocation not in the same segment")
                    break;
                } else if (b0 != 0xE8 && b0 != 0xE9) {
                    why_ = std::format("a short jump out of itself ({} at {:04x})", mn, at);
                    reaches_ = t;
                    return;
                } else {
                    const std::optional<std::string> sym = named(symbol_for(FunctionRef{seg, t}));
                    if (!sym) {
                        missing_.insert(t);
                    } else {
                        externs_[*sym] = Linkage::Near;
                        calls_[at] = Directive{ins.size, transfer_text(b0 == 0xE8, *sym)};
                    }
                }
            } else if (!covered) {
                ins_[at] = Insn{Bytes(ins.bytes.begin(), ins.bytes.begin() + ins.size), mn, ins.op_str, std::nullopt};
            }
            if (mn == "ret" || mn == "retf" || mn == "iret" || mn == "jmp" || mn == "ljmp" || mn == "hlt") break;   // the code does not go on past it
            at += ins.size;
        }
    }
    if (!missing_.empty()) {
        std::string list;
        for (const std::uint32_t t : missing_) list += (list.empty() ? "" : ", ") + std::format("{:04x}", t);
        why_ = "a near call or jump to " + list + ", where no function has a symbol";
        reaches_ = *missing_.begin();
        return;
    }
    if (!calls_.empty() && bits_ == 32) {
        // (a near call or jump out of the run, by its name, would be written as 16-bit code)
        why_ = std::format("a near call or jump out of a 32-bit segment's run ({:04x})", calls_.begin()->first);
        return;
    }
    if (bits_ == 16) far_pointer_offsets(md, symbol_for, internal_selector, seg);   // (its numbers are imm16s: see there)
    raw_far_pointers(symbol_for, internal_selector);
    module_ = module.module_name();
    seg_ = seg;
    public_ = public_symbol;
    // a jump into the middle of an instruction: that instruction is not written as one (each instruction's own few
    // bytes looked up in the set: not every target for each)
    std::set<std::uint32_t> targets;
    for (const auto& [a, i] : ins_) {
        if (i.target) targets.insert(*i.target);
    }
    const auto erase_over = [&](const std::set<std::uint32_t>& places, std::uint32_t skip) {
        // an instruction a place falls inside is not written as one; `skip` is how far into it the check begins
        for (auto it = ins_.begin(); it != ins_.end();) {
            const std::uint32_t a = it->first;
            if (any_in(places, a + skip, a + static_cast<std::uint32_t>(it->second.bytes.size()))) {
                it = ins_.erase(it);
            } else {
                ++it;
            }
        }
    };
    erase_over(targets, 1);
    // a place read through CS is a label too: no instruction may run over it
    std::set<std::uint32_t> read;
    for (const auto& [a, d] : cs_labels_) read.insert(d);
    erase_over(read, 1);
    for (auto it = cs_labels_.begin(); it != cs_labels_.end();) it = ins_.contains(it->first) ? std::next(it) : cs_labels_.erase(it);
    for (const auto& [a, i] : ins_) {
        if (i.target && ins_.contains(*i.target)) labels_.insert(*i.target);
    }
    for (auto it = ins_.begin(); it != ins_.end();) {   // (its target is not an instruction start)
        it = it->second.target && !labels_.contains(*it->second.target) ? ins_.erase(it) : std::next(it);
    }
    for (const auto& [a, d] : cs_labels_) labels_.insert(d);
    std::set<std::uint32_t> held;
    for (const std::uint32_t t : tables_) {   // what a jump table holds: labels, as words
        labels_.insert(blob_u16(t));
        held.insert(t);
        held.insert(t + 1);
    }
    erase_over(held, 0);
    text_ = write(std::set<std::uint32_t>{});
}

std::uint16_t Function::blob_u16(std::uint32_t at) const {
    CB_ASSERT(at + 1 < blob_.size(), "asm16: a word read past the segment");
    return static_cast<std::uint16_t>(blob_[at] | (blob_[at + 1] << 8));
}

// The same far pointers where the frame is written as bytes (code not followed, a table): a selector fix-up to one of
// the module's code segments, and in the two bytes before it the offset -- of `call far` (9A) or of a pointer stored --
// is a number. If it is where a function begins, it is that function's offset, by its name (USER's 25:23DF `call far
// 12:085E`, LoadIcon's, in code reached only through a table: with 085E rebuilt from C and placed after its frame, the
// call ran its filler into the next function).
void Function::raw_far_pointers(const SymbolFor& symbol_for, const std::map<std::uint32_t, std::uint16_t>& internal_selector) {
    std::set<std::uint32_t> covered;
    for (const auto& [a, i] : ins_) {
        for (std::size_t k = 0; k < i.bytes.size(); ++k) covered.insert(a + static_cast<std::uint32_t>(k));
    }
    for (const auto& [a, d] : symbolic_) {
        for (std::uint32_t k = 0; k < d.size; ++k) covered.insert(a + k);
    }
    for (const auto& [off, tseg] : internal_selector) {
        if (!sites_.contains(off)) continue;
        // the offset before the selector (a stored pointer, `call far`), or pushed right after it (`push seg X` / `push
        // 0FF1h`: 68 ss ss 68 oo oo)
        std::int64_t place = static_cast<std::int64_t>(off) - 2;
        if (off >= 1 && blob_[off - 1] == 0x68 && off + 2 < blob_.size() && blob_[off + 2] == 0x68) place = static_cast<std::int64_t>(off) + 3;
        const std::int64_t p = place;
        if (p < start_ || p + 2 > end_) continue;
        const std::uint32_t q = static_cast<std::uint32_t>(p);
        if (sites_.contains(q) || sites_.contains(q + 1) || covered.contains(q) || covered.contains(q + 1)) continue;
        const std::uint16_t value = blob_u16(q);
        if (const auto name = named(symbol_for(FunctionRef{tseg, value}))) {
            externs_.emplace(*name, Linkage::Far);
            sites_[q] = Directive{2, "dw   offset " + *name};
            continue;
        }
        if (q == off + 3 || (q >= 1 && (blob_[q - 1] == 0x9A || blob_[q - 1] == 0xEA))) {
            // a call or a pushed pointer into the middle of a function: that place is an entry of its own (USER 35:035A
            // `call far 43:0234`, inside sub_43_01D4 -- rebuilt from C, the call ran its filler)
            raw_far_entries_.insert({tseg, value});
        }
    }
}

// The code segment whose selector makes a far pointer with the number the instruction at `at` gives, in one of the two
// shapes a far pointer is built in -- `push seg X` right before `push n`, or the two words stored side by side (`mov
// [m], n` / `mov [m+2], seg X`, either order) -- or nullopt.
std::optional<std::uint16_t> Function::pointer_selector(const platforms::Decoder& md, std::uint32_t at, const Insn& data,
                                                        const std::map<std::uint32_t, std::uint16_t>& selectors) const {
    if (data.bytes[0] == 0x68 && data.bytes.size() == 3) {
        if (at >= 3 && blob_[at - 3] == 0x68) {
            const auto it = selectors.find(at - 2);
            if (it != selectors.end()) return it->second;
        }
        return std::nullopt;
    }
    const auto decode_at = [&](std::uint32_t where) {
        const std::size_t room = std::min<std::size_t>(blob_.size() - where, kLongestInsn);
        return decode_one(md, ByteView(blob_).subspan(where, room), where);
    };
    const std::optional<Decoded> me = decode_at(at);
    if (!me) return std::nullopt;
    const Operand* m = nullptr;
    for (const Operand& o : me->operands) {
        if (o.type == X86_OP_MEM) {
            m = &o;
            break;
        }
    }
    if (!m) return std::nullopt;
    std::map<std::uint32_t, std::uint32_t> sizes;
    for (const auto& [a, i] : ins_) sizes[a] = static_cast<std::uint32_t>(i.bytes.size());
    for (const auto& [a, d] : symbolic_) sizes[a] = d.size;   // (the selector's own: a fix-up)
    std::vector<std::uint32_t> others{at + static_cast<std::uint32_t>(data.bytes.size())};
    for (const auto& [a, n] : sizes) {
        if (a + n == at) others.push_back(a);
    }
    for (const std::uint32_t other : others) {
        const auto sz = sizes.find(other);
        if (sz == sizes.end()) continue;
        const std::uint32_t site = other + sz->second - 2;
        const auto sel = selectors.find(site);
        if (sel == selectors.end()) continue;
        const std::optional<Decoded> ins = decode_at(other);
        const Operand* om = nullptr;
        if (ins) {
            for (const Operand& o : ins->operands) {
                if (o.type == X86_OP_MEM) {
                    om = &o;
                    break;
                }
            }
        }
        if (om && om->base == m->base && om->index == m->index && om->segment == m->segment && om->disp == m->disp + 2) return sel->second;
    }
    return std::nullopt;
}

// A far pointer to one of the module's functions put together in the code: its selector is a fix-up (or CS), its offset
// a plain number -- the function's, in the original. Built from the number, the pointer would still go where the
// function WAS (USER's 3:1505 `mov word ptr [bp-18h], 1587h` with segment 10's selector: the window procedure BEAR306,
// rebuilt from C and placed after its frame, entered at its old place -- the frame's filler). So that number is written
// as the function's offset, by its name.
// selectors: {offset of the fix-up: the segment it is the selector of}
void Function::far_pointer_offsets(const platforms::Decoder& md, const SymbolFor& symbol_for,
                                   const std::map<std::uint32_t, std::uint16_t>& selectors, std::uint16_t seg) {
    // (the selector is the one beside it -- a few instructions away: a frame may hold selectors of several segments,
    // each with a function at the same offset: USER's `push seg 12; push 0FF1h`, LoadDIBCursorHandler, where 35:0FF1 is
    // a function too)
    std::vector<std::uint32_t> own;
    for (const auto& [a, i] : ins_) {
        if ((i.mnemonic == "push" && i.operands == "cs") || (i.mnemonic == "mov" && std::string_view(i.operands).ends_with(", cs"))) own.push_back(a);
    }
    if (selectors.empty() && own.empty()) return;
    const auto near_to = [](std::uint32_t a, std::uint32_t b) { return std::abs(static_cast<std::int64_t>(a) - static_cast<std::int64_t>(b)) <= kNear; };
    std::vector<std::pair<std::uint32_t, Insn>> candidates(ins_.begin(), ins_.end());
    for (const auto& [at, i] : candidates) {
        const Bytes& data = i.bytes;
        if (i.target || (i.mnemonic != "mov" && i.mnemonic != "push") || data.size() < 3) continue;
        const bool imm16_form = data[0] == 0x68 || (data[0] >= 0xB8 && data[0] <= 0xBF) || data[0] == 0xC7 ||
                                ((data[0] == 0x26 || data[0] == 0x2E || data[0] == 0x36 || data[0] == 0x3E) && data.size() > 3 && data[1] == 0xC7);
        if (!imm16_form) continue;   // (an imm16 form: push, mov reg / mem)
        const std::uint16_t value = static_cast<std::uint16_t>(data[data.size() - 2] | (data[data.size() - 1] << 8));
        if (!value) continue;
        std::set<std::uint16_t> segs;
        for (const auto& [o, t] : selectors) {
            if (near_to(o, at)) segs.insert(t);
        }
        if (std::any_of(own.begin(), own.end(), [&](std::uint32_t a) { return near_to(a, at); })) segs.insert(seg);
        std::set<std::string> names;
        for (const std::uint16_t t : segs) {
            if (const auto n = named(symbol_for(FunctionRef{t, value}))) names.insert(*n);
        }
        if (names.empty()) {
            // a far pointer to where no function is known to begin: a function nothing else calls (USER's callback at
            // 1:3452, `push seg 1 / push 3452h`, passed to an enumeration -- taken for the tail of sub_1_3412, and gone
            // with it when that was rebuilt from C): that place is an entry of its own. Only in the exact shapes of a
            // far pointer -- a number merely near a selector is not one (taken loosely, 126 of USER's functions went
            // back to assembly for constants)
            const auto t = pointer_selector(md, at, i, selectors);
            if (t) {
                const auto limit = code_segments_.find(*t);
                if (value < (limit == code_segments_.end() ? 0u : limit->second)) raw_far_entries_.insert({*t, value});
            }
            continue;
        }
        if (names.size() != 1) continue;
        const std::string& name = *names.begin();
        const std::string line = mnemonic(at);
        const auto replaced = offset_in_place_of_immediate(line, value, name);
        if (!replaced) continue;
        externs_.emplace(name, Linkage::Far);
        symbolic_[at] = Directive{static_cast<std::uint32_t>(data.size()), *replaced};   // (written by its name: see write)
    }
}

std::string Function::mnemonic(std::uint32_t at) const {
    const Insn& i = ins_.at(at);
    const std::string& ops = i.operands;
    if (const auto op = assembly::string_op(i.mnemonic + " ")) {
        std::string without = text::replace_all(ops, "es:[di]", "");
        if (without.find(':') == std::string::npos) {
            return std::string(text::trim(std::string(op->prefix) + std::string(op->op) + op->width));
        }
    }
    if (i.target) {
        const std::string label = std::format("L_{:04X}", *i.target);
        const std::string& mn = i.mnemonic;
        if (mn == "loop" || mn == "loope" || mn == "loopne" || mn == "jcxz" || mn == "jecxz") return std::format("{} {}", pad_right(mn, 6), label);
        return std::format("{} {} {}", pad_right(mn, 6), i.bytes.size() == 2 ? "short" : "near ptr", label);
    }
    std::string shown = ops;
    if (const auto cs = cs_labels_.find(at); cs != cs_labels_.end()) {   // what it reads through CS, by its label
        const std::uint32_t disp = cs->second;
        if (disp) {
            shown = replace_hex_word(shown, disp, std::format("L_{:04X}", disp));
        } else {
            shown = text::replace_first(shown, "cs:[", "cs:[L_0000 + ");
        }
    }
    shown = default_ds(shown);
    const std::string line = std::format("{} {}", pad_right(std::string(assembly::mnemonic(i.mnemonic)), 6), assembly::operands(shown));
    return std::string(text::rtrim(line));
}

bool Function::stays_inside(std::uint32_t at, std::uint32_t lo, std::uint32_t hi) const {
    const Insn& i = ins_.at(at);
    if (i.target && !(lo <= *i.target && *i.target < hi)) return false;
    const auto disp = cs_labels_.find(at);
    return disp == cs_labels_.end() || (lo <= disp->second && disp->second < hi);
}

// Whether a label is referred to from inside [lo, hi) (so a piece writes it).
bool Function::used(std::uint32_t label, std::uint32_t lo, std::uint32_t hi) {
    if (!(lo <= label && label < hi)) return false;
    if (!refs_) {   // (who refers to each label, once)
        refs_.emplace();
        for (const auto& [a, i] : ins_) {
            if (i.target) (*refs_)[*i.target].push_back(a);
        }
        for (const auto& [a, d] : cs_labels_) (*refs_)[d].push_back(a);
        for (const std::uint32_t t : tables_) (*refs_)[blob_u16(t)].push_back(t);
    }
    const auto it = refs_->find(label);
    if (it == refs_->end()) return false;
    return std::any_of(it->second.begin(), it->second.end(), [&](std::uint32_t a) { return lo <= a && a < hi; });
}

std::map<std::uint32_t, assembly::Candidate> Function::candidates() const {
    std::map<std::uint32_t, assembly::Candidate> out;
    if (bits_ != 16) return out;   // (see write)
    for (const auto& [at, i] : ins_) {
        if (!i.target && !cs_labels_.contains(at)) {   // (a CS label is checked in the whole)
            out[at] = assembly::Candidate{mnemonic(at), i.bytes, Bytes(i.bytes.size(), 0)};
        }
    }
    return out;
}

std::map<std::string, std::uint32_t> Function::raw_calls_into_holes() {
    std::map<std::string, std::uint32_t> out;
    const platforms::Decoder md = platforms::Decoder::open(platforms::Arch::X86, bits_, false);
    const std::size_t size = bits_ == 16 ? 3 : 5;
    std::set<std::uint32_t> decoded;
    for (const auto& [a, i] : ins_) {
        for (std::size_t k = 0; k < i.bytes.size(); ++k) decoded.insert(a + static_cast<std::uint32_t>(k));
    }
    std::map<std::uint32_t, std::string> hole_symbol;
    for (const Hole& h : holes_) hole_symbol[h.start] = h.symbol;
    const auto in_hole = [&](std::uint32_t x) {
        return std::any_of(holes_.begin(), holes_.end(), [&](const Hole& h) { return h.start <= x && x < h.end; });
    };
    std::uint32_t at = start_;
    while (at < end_) {
        if (decoded.contains(at) || in_hole(at)) {
            ++at;
            continue;
        }
        std::uint32_t stop = at;
        while (stop < end_ && !decoded.contains(stop) && !in_hole(stop)) ++stop;
        for (const platforms::Insn& ins : md.disasm(ByteView(blob_).subspan(at, stop - at), at)) {
            if ((ins.bytes[0] == 0xE8 || ins.bytes[0] == 0xE9) && ins.size == size && bits_ == 16) {
                const auto rel = static_cast<std::int16_t>(ins.bytes[1] | (ins.bytes[2] << 8));
                const std::uint32_t target = static_cast<std::uint32_t>(static_cast<std::int64_t>(ins.address) + 3 + rel) & 0xFFFF;
                const auto h = hole_symbol.find(target);
                if (h != hole_symbol.end()) {
                    out.emplace(h->second, static_cast<std::uint32_t>(ins.address));
                    raw_hole_calls_[static_cast<std::uint32_t>(ins.address)] = HoleCall{h->second, ins.bytes[0]};
                }
            }
        }
        at = stop;
    }
    return out;
}

std::string Function::write(const Good& good_in, std::optional<std::pair<std::uint32_t, std::uint32_t>> piece) {
    const std::uint32_t lo = piece ? piece->first : start_;
    const std::uint32_t hi = piece ? piece->second : end_;
    Good good = good_in;
    if (bits_ == 32) good = std::set<std::uint32_t>{};   // (its instructions as their bytes: see the constructor)
    if (!holes_.empty() && !raw_scanned_) {
        // (a call in bytes not followed as code to a function rebuilt from C is written by its name: the function is
        // placed elsewhere, and the displacement in the bytes would not)
        raw_scanned_ = true;
        raw_calls_into_holes();
        for (const auto& [a, c] : raw_hole_calls_) externs_[c.symbol] = Linkage::Near;
    }
    const auto inside = [&](std::uint32_t x) { return lo <= x && x < hi; };
    const auto good_has = [&](std::uint32_t a) { return !good || good->contains(a); };
    std::set<std::uint32_t> covered;   // (inside an instruction)
    if (!good) {
        for (const auto& [a, i] : ins_) {
            for (std::size_t k = 1; k < i.bytes.size(); ++k) covered.insert(a + static_cast<std::uint32_t>(k));
        }
    } else {
        for (const std::uint32_t a : *good) {
            const auto i = ins_.find(a);
            if (i == ins_.end()) continue;
            for (std::size_t k = 1; k < i->second.bytes.size(); ++k) covered.insert(a + static_cast<std::uint32_t>(k));
        }
    }
    std::vector<std::string> out;
    out.push_back(std::format("; {}, {}: {}:{:04x}-{:04x} -- written in assembly, as its author wrote it: what the", module_, public_, seg_, lo, hi));
    out.push_back("; loader fixes and the calls out of the function are written as what they refer to;");
    out.push_back("; an instruction the assembler would encode differently is given as bytes.");
    out.push_back(".386p");
    out.push_back(std::format("CODE{} segment byte public use16 'MCODE'", seg_));
    out.push_back(std::format("        assume cs:CODE{0}, ds:nothing, es:nothing, ss:nothing", seg_));
    std::set<std::string> mine;
    std::map<std::string, Linkage> links;   // other pieces of the frame are near; what the code names comes after and wins
    for (const PublicSymbol& p : publics_) {
        if (inside(p.offset)) {
            mine.insert(p.symbol);
        } else {
            links[p.symbol] = Linkage::Near;
        }
    }
    for (const auto& [sym, k] : externs_) links[sym] = k;
    for (const auto& [sym, k] : links) {
        if (!mine.contains(sym)) out.push_back(std::format("        extrn {}:{}", sym, linkage_text(k)));
    }
    std::map<std::uint32_t, std::string> entry;
    for (const PublicSymbol& p : publics_) {
        if (inside(p.offset)) {
            out.push_back("        public " + p.symbol);
            entry[p.offset] = p.symbol;
        }
    }
    // an instruction something enters in its middle (hand-written overlapping entries: `cmp ax, 0B1xxh` chains entered
    // one byte in, where they read `mov cl, xx`) is written as its bytes -- the same bytes, and the place inside it gets
    // its label
    std::set<std::uint32_t> targets;
    for (const auto& [o, name] : entry) targets.insert(o);
    for (const std::uint32_t x : labels_) {
        if (inside(x)) targets.insert(x);
    }
    std::set<std::uint32_t> split;
    for (const auto& [a, i] : ins_) {
        if (inside(a) && any_in(targets, a + 1, a + static_cast<std::uint32_t>(i.bytes.size()))) split.insert(a);
    }
    Bytes run;
    const auto flush = [&] {
        for (std::size_t i = 0; i < run.size(); i += 16) {
            out.push_back("        db   " + db_bytes(ByteView(run).subspan(i, std::min<std::size_t>(16, run.size() - i))));
        }
        run.clear();
    };
    std::uint32_t at = lo;
    while (at < hi) {
        const auto hole = std::find_if(holes_.begin(), holes_.end(), [&](const Hole& h) { return h.start == at; });
        if (hole != holes_.end()) {
            flush();
            out.push_back(std::format("        db   {} dup (0CCh)                         ; {}, rebuilt from C, is placed after this", hole->end - hole->start, hole->symbol));
            at = hole->end;
            continue;
        }
        if (const auto e = entry.find(at); e != entry.end()) {
            flush();
            out.push_back(e->second + ":");
        }
        if (labels_.contains(at) && used(at, lo, hi)) {
            flush();
            out.push_back(std::format("L_{:04X}:", at));
        }
        if (const auto d = symbolic_.find(at); d != symbolic_.end() && good) {
            flush();
            out.push_back("        " + d->second.text);
            at += d->second.size;
        } else if (const auto s = sites_.find(at); s != sites_.end()) {
            flush();
            out.push_back("        " + s->second.text);
            at += s->second.size;
        } else if (tables_.contains(at) && inside(blob_u16(at))) {
            flush();
            const std::uint32_t t = blob_u16(at);
            // an entry is a label where an instruction of this piece is written; anywhere else (a function rebuilt from
            // C, the middle of an instruction: a table guessed too long reads on into data) it is the number it is -- the
            // same bytes
            const bool held = ins_.contains(t) && good_has(t) && !covered.contains(t) &&
                              !std::any_of(holes_.begin(), holes_.end(), [&](const Hole& h) { return h.start <= t && t < h.end; });
            out.push_back(held ? std::format("        dw   L_{:04X}", t) : std::format("        dw   0{:04X}h", t));
            at += 2;
        } else if (const auto c = calls_.find(at); c != calls_.end()) {
            flush();
            out.push_back("        " + c->second.text);
            at += c->second.size;
        } else if (ins_.contains(at) && good_has(at) && !split.contains(at) && stays_inside(at, lo, hi)) {
            flush();
            out.push_back("        " + mnemonic(at));
            at += static_cast<std::uint32_t>(ins_.at(at).bytes.size());
        } else if (ins_.contains(at) && !split.contains(at)) {
            flush();
            const Insn& i = ins_.at(at);
            out.push_back(std::format("        db   {} ; {} {}", pad_right(db_bytes(i.bytes), 40), i.mnemonic, i.operands));
            at += static_cast<std::uint32_t>(i.bytes.size());
        } else if (const auto h = raw_hole_calls_.find(at); h != raw_hole_calls_.end()) {
            flush();
            out.push_back("        " + transfer_text(h->second.opcode == 0xE8, h->second.symbol));
            at += bits_ == 16 ? 3 : 5;
        } else {
            run.push_back(blob_[at]);
            ++at;
        }
    }
    flush();
    out.push_back(std::format("CODE{} ends", seg_));
    out.push_back("end");
    out.push_back("");
    // (a table read on into what is code or data: an entry whose place got no label is the number it is -- the same two
    // bytes)
    std::set<std::string> defined;
    for (const std::string& line : out) {
        if (line.size() == 7 && line.starts_with("L_") && line.back() == ':') defined.insert(line.substr(0, 6));
    }
    constexpr std::string_view kTableWord = "        dw   ";
    for (std::string& line : out) {
        if (line.size() == kTableWord.size() + 6 && line.starts_with(kTableWord) && line.compare(kTableWord.size(), 2, "L_") == 0) {
            const std::string label = line.substr(kTableWord.size());
            const bool four_hex = std::all_of(label.begin() + 2, label.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'); });
            if (four_hex && !defined.contains(label)) line = std::string(kTableWord) + "0" + label.substr(2) + "h";
        }
    }
    return text::join(out, "\n");
}

namespace {

// differences() for the piece [lo, hi) of the function's segment.
std::vector<std::uint32_t> differences_in(ByteView code, ByteView mask, const Function& f, std::uint32_t lo, std::uint32_t hi) {
    const ByteView want = f.blob().subspan(lo, hi - lo);
    std::set<std::uint32_t> fixed;   // offsets in the piece (a place before it wraps to one that is never looked up)
    for (const auto& [at, d] : f.sites()) {
        for (std::uint32_t k = 0; k < d.size; ++k) fixed.insert(at - lo + k);
    }
    for (const auto* group : {&f.calls(), &f.symbolic()}) {
        for (const auto& [at, d] : *group) {
            for (std::uint32_t k = 1; k < d.size; ++k) fixed.insert(at - lo + k);
        }
    }
    for (const std::uint32_t t : f.tables()) {
        fixed.insert(t - lo);
        fixed.insert(t - lo + 1);
    }
    for (const Hole& h : f.holes()) {
        for (std::uint32_t k = h.start; k < h.end; ++k) fixed.insert(k - lo);
    }
    std::vector<std::uint32_t> out;
    const std::size_t n = std::min(code.size(), want.size());
    for (std::size_t i = 0; i < n; ++i) {
        const bool masked = i < mask.size() && mask[i] != 0;
        if (code[i] != want[i] && !fixed.contains(static_cast<std::uint32_t>(i)) && !masked) out.push_back(lo + static_cast<std::uint32_t>(i));
    }
    return out;
}

}  // namespace

std::vector<std::uint32_t> differences(ByteView code, ByteView mask, const Function& f) {
    return differences_in(code, mask, f, f.start(), f.end());
}

std::optional<std::string> check_piece(ByteView object, const Function& f, std::uint32_t lo, std::uint32_t hi) {
    const msc6::Code code = msc6::object_code(object);
    if (code.code.size() != hi - lo) return std::format("{} bytes, the original {}", code.code.size(), hi - lo);
    // the places the loader sets are those of the piece: the function's own table, with the range narrowed
    const std::vector<std::uint32_t> bad = differences_in(code.code, code.mask, f, lo, hi);
    if (!bad.empty()) return std::format("differs at {:04x}", bad.front());
    return std::nullopt;
}

std::optional<std::string> check(ByteView object, const Function& f) {
    const msc6::Code code = msc6::object_code(object);
    const std::size_t want = f.end() - f.start();
    if (code.code.size() != want) return std::format("{} bytes, the original {}", code.code.size(), want);
    const std::vector<std::uint32_t> bad = differences(code.code, code.mask, f);
    if (!bad.empty()) return std::format("differs at {:04x}", bad.front());
    return std::nullopt;
}

}  // namespace cb::build::asm16
