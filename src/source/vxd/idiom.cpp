// cb-unity: alone (file-local helpers named for this module)
#include "source/vxd/idiom.hpp"

#include "source/vxd/corpus.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <set>

#include "core/capstone_api.hpp"
#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "platforms/decoder.hpp"

namespace cb::source::vxd::idiom {

namespace le = formats::le;
namespace stdfs = std::filesystem;

namespace {

constexpr std::uint32_t kHighLinearAlias = 0x80000000;   // a fix-up target in the shared arena of Win95: the low 31 bits are the offset
constexpr std::uint32_t kOffsetMask = 0x7FFFFFFF;
constexpr std::string_view kAliasText = " + 80000000h";

bool is_hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
bool is_word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

const le::InternalFixup* internal_at(const Emission& code, const Place& at) {
    const le::Fixup* f = code.fixups().find(at);
    return f ? std::get_if<le::InternalFixup>(f) : nullptr;
}

std::string label_text(const Emission& code, const Place& at) {
    const std::string* label = code.label_at(at);
    return label ? *label : std::string();
}

std::optional<std::uint32_t> word_at(ByteView bytes, std::uint64_t at) {
    if (at + 4 > bytes.size()) return std::nullopt;
    return static_cast<std::uint32_t>(bytes[at] | (bytes[at + 1] << 8) | (bytes[at + 2] << 16) | (static_cast<std::uint32_t>(bytes[at + 3]) << 24));
}

std::string sub_label(const Place& at) { return std::format("sub_{}_{:04X}", at.object, at.offset); }
std::string loc_label(const Place& at) { return std::format("loc_{}_{:04X}", at.object, at.offset); }

// The number after the comma of `eax, 0x10`, which the control-dispatch scans read (base prefixes honoured).
std::optional<std::int64_t> second_operand(std::string_view operands) {
    const auto parts = text::split(operands, ',');
    if (parts.size() < 2) return std::nullopt;
    return text::parse_int<std::int64_t>(text::trim(parts[1]), 0);
}

bool starts_with_eax(std::string_view operands) {
    std::string squeezed;
    for (const char c : operands) {
        if (c != ' ') squeezed += c;
    }
    return squeezed.starts_with("eax,");
}

bool is_one_of(std::string_view s, std::initializer_list<std::string_view> list) { return std::find(list.begin(), list.end(), s) != list.end(); }

}  // namespace

// ---- the DDK's names --------------------------------------------------------------------------------------------------------

std::optional<std::string_view> device_include(std::uint32_t device) {
    switch (device) {
        case 0x0001: return "VMM";      case 0x0003: return "VPICD";    case 0x0004: return "VDMAD";   case 0x0005: return "VTD";
        case 0x0006: return "V86MMGR";  case 0x0007: return "PAGESWAP"; case 0x000B: return "VSD";     case 0x000C: return "VMD";
        case 0x000D: return "VKD";      case 0x000E: return "VCD";      case 0x0010: return "BLOCKDEV"; case 0x0011: return "VMCPD";
        case 0x0014: return "VNETBIOS"; case 0x0015: return "DOSMGR";   case 0x0016: return "VMPOLL";  case 0x0017: return "SHELL";
        case 0x001E: return "VXDLDR";   case 0x0021: return "PAGEFILE"; case 0x0028: return "VPOWERD"; case 0x002C: return "VWIN32";
        case 0x002D: return "VCOMM";    case 0x0033: return "CONFIGMG"; case 0x0036: return "VFBACKUP"; case 0x0038: return "VCOND";
        case 0x0040: return "IFSMGR";   case 0x0442: return "VTDAPI";
        default: return std::nullopt;
    }
}

std::vector<std::string> device_includes(const Emission& code, std::uint32_t object) {
    std::set<std::string> incs = {"VMM"};
    for (const auto& [at, insn] : code.instructions()) {
        if (at.object != object || !insn.service) continue;
        if (const auto inc = device_include(insn.service->device)) incs.insert(std::string(*inc));
    }
    return {incs.begin(), incs.end()};
}

std::optional<std::string> service_macro(const DdkNames& names, std::uint32_t device, std::uint32_t service) {
    if (!device_include(device)) return std::nullopt;   // no .INC to define the service equate: keep the raw int 20h and DWORD
    const auto device_text = device_name(device);
    if (!device_text) return std::nullopt;
    const auto name = names.services().name(*device_text, service & 0x7FFF);
    if (!name || name->empty()) return std::nullopt;
    const bool jump = (service & 0x8000) != 0;   // DL_Jmp_Mask
    const char* verb = device == 0x0001 ? (jump ? "VMMJmp" : "VMMcall") : (jump ? "VxDJmp" : "VxDcall");
    return std::format("        {} {}", verb, *name);
}

std::string declare_virtual_device(const Emission& code, const le::Ddb& ddb, bool loadable) {
    std::vector<std::string> args = {ddb.name, std::to_string(ddb.major), std::to_string(ddb.minor), ddb.name + "_Control",
                                     std::format("0{:04X}h", ddb.device_id), std::format("0{:08X}h", ddb.init_order)};
    if (loadable) {
        const auto handler = [&](std::int64_t field) -> std::string {   // the label a DDB pointer field targets
            const le::InternalFixup* f = internal_at(code, {ddb.object, ddb.rva + field});
            if (f == nullptr || !f->target) return {};
            return label_text(code, {f->object, *f->target});
        };
        const std::string v86 = handler(0x1C), pm = handler(0x20), ref = handler(0x2C);
        if (!v86.empty() || !pm.empty() || !ref.empty()) {   // trailing arguments only as far as needed, earlier ones blank (the macro's default)
            args.push_back(v86);
            if (!pm.empty() || !ref.empty()) {
                args.push_back(pm);
                if (!ref.empty()) args.push_back(ref);   // a bare label: the macro wraps it (Ref_Data_Offset EQU <OFFSET32 ..>)
            }
        }
    }
    return "Declare_Virtual_Device " + text::join(args, ", ");
}

std::optional<ServiceTable> service_table(const Emission& code, const PlaceSet& emittable) {
    const auto& ddb = code.ddb();
    if (!ddb) return std::nullopt;
    std::uint32_t object = ddb->object;
    const std::int64_t r = ddb->rva;
    // Service_Table_Ptr (+0x30) is stored as a zero-addend relocation on many VxDs: the raw dword is 0 and the real base lives in the
    // fix-up. Prefer the fix-up, fall back to the raw dword. The table's object may differ from the DDB's, so the size is read from the
    // DDB's object and the table indexed in the object the fix-up names.
    std::optional<std::uint32_t> ptr;
    if (const le::Fixup* st = code.fixups().find({object, r + 0x30}); st != nullptr && std::holds_alternative<le::InternalFixup>(*st)) {
        const auto& f = std::get<le::InternalFixup>(*st);
        object = f.object;
        ptr = f.target;
    } else {
        ptr = word_at(code.code(object), static_cast<std::uint64_t>(r + 0x30));
    }
    const ByteView table_code = code.code(object);
    const auto count = word_at(code.code(ddb->object), static_cast<std::uint64_t>(r + 0x34));
    if (!ptr || !count || !(0 < *count && *count < 8192 && std::uint64_t{*ptr} + 4 * std::uint64_t{*count} <= table_code.size())) return std::nullopt;

    ServiceTable out;
    out.at = {object, *ptr};
    out.count = *count;
    std::set<std::string> used;
    for (std::uint32_t i = 0; i < *count; ++i) {
        const Place slot{object, std::int64_t{*ptr} + 4 * i};
        const le::Fixup* f = code.fixups().find(slot);
        if (f == nullptr) {
            if (word_at(table_code, slot.offset).value_or(1) == 0) {
                out.names.push_back("RESERVED");   // a genuine null slot: the macro writes dd 0
                continue;
            }
            return std::nullopt;   // non-zero without a fix-up: unexpected, stays raw
        }
        const auto* internal = std::get_if<le::InternalFixup>(f);
        if (internal == nullptr || !internal->target) return std::nullopt;   // an import or a selector is no handler
        const Place target{internal->object, *internal->target};
        if (!emittable.contains(target)) return std::nullopt;   // a handler left mid-instruction cannot be labelled
        // keyed off the DDB's name
        std::optional<std::string> known = code.names().services().name(ddb->name, i);
        const std::string name = known && !known->empty() ? *known : std::format("{}_Service_{:03}", ddb->name, i);
        if (!used.insert(name).second) return std::nullopt;   // two services with the identical name
        out.names.push_back(name);
        if (name != label_text(code, target)) out.extra[target].push_back(name);   // unless it already IS the label there
    }
    return out;
}

namespace {

// The emitted label a direct branch targets, or empty. Resolves a same-object target and a cross-object one (through the fix-up on
// the branch's operand); the target must be an emittable label so the macro's reference resolves.
std::string branch_label(const Emission& code, std::uint32_t object, std::uint32_t a, const Instruction& insn, const PlaceSet& emittable) {
    if (const auto t = insn.direct_target(); t && emittable.contains({object, *t})) return label_text(code, {object, *t});
    for (std::uint32_t k = a + 1; k < a + insn.size; ++k) {
        const le::Fixup* f = code.fixups().find({object, k});
        if (f == nullptr) continue;
        const auto* internal = std::get_if<le::InternalFixup>(f);
        if (internal != nullptr && internal->target && emittable.contains({internal->object, *internal->target})) {
            return label_text(code, {internal->object, *internal->target});
        }
    }
    return {};
}

bool is_stub_instruction(std::string_view mnemonic) { return is_one_of(mnemonic, {"clc", "stc", "cmc", "ret", "retn", "retf"}); }

void skip_bytes(PlaceSet& skip, std::uint32_t object, std::int64_t from, std::int64_t to) {
    for (std::int64_t k = from; k < to; ++k) skip.insert({object, k});
}

}  // namespace

ControlDispatch control_dispatch(const Emission& code, const PlaceSet& emittable) {
    const auto& ddb = code.ddb();
    if (!ddb) return {};
    const std::string control = ddb->name + "_Control";
    const auto entry = std::find_if(code.labels().begin(), code.labels().end(), [&](const auto& kv) { return kv.second == control; });
    if (entry == code.labels().end()) return {};
    const auto& messages = code.names().control_messages();
    const std::uint32_t object = entry->first.object;
    const std::uint32_t start = static_cast<std::uint32_t>(entry->first.offset);

    // --- the jump-table form ---
    std::uint32_t a = start;
    std::int64_t base = 0;
    std::optional<Place> default_target;   // the range check's (jae/ja) target: the do-nothing stub the many UNHANDLED messages fall
                                           // through to. Real handlers (even one SHARED by several messages) are kept; only this stub goes.
    for (int step = 0; step < 8; ++step) {
        const Instruction* row = code.instruction_at({object, a});
        if (row == nullptr) break;
        if (row->mnemonic == "sub" && starts_with_eax(row->operands)) {
            if (const auto n = second_operand(row->operands)) base = *n;
        }
        if (!default_target && is_one_of(row->mnemonic, {"jae", "ja", "jnb", "jnbe", "jbe", "jb", "jg", "jge"})) {
            if (const auto t = row->direct_target()) default_target = Place{object, *t};
        }
        if (row->mnemonic == "jmp" && row->operands.find("*4") != std::string::npos) {
            ControlDispatch out;
            skip_bytes(out.skip, object, start, a + row->size);   // the prologue bytes
            const std::vector<Place>* table = nullptr;
            for (std::uint32_t k = a; k < a + row->size; ++k) {
                const le::InternalFixup* f = internal_at(code, {object, k});
                if (f != nullptr && f->object == object) {
                    if (f->target) {
                        if (const auto it = code.jump_tables().find({f->object, *f->target}); it != code.jump_tables().end() && !it->second.empty()) {
                            table = &it->second;
                            skip_bytes(out.skip, object, *f->target, std::int64_t{*f->target} + 4 * static_cast<std::int64_t>(table->size()));   // the table's bytes
                        }
                    }
                    break;
                }
            }
            std::uint32_t b = a + row->size;   // a trailing unhandled stub
            while (const Instruction* stub = code.instruction_at({object, b})) {
                if (!is_stub_instruction(stub->mnemonic)) break;
                skip_bytes(out.skip, object, b, b + stub->size);
                b += stub->size;
            }
            if (table != nullptr) {
                for (std::size_t i = 0; i < table->size(); ++i) {
                    const Place& handler = (*table)[i];
                    const std::int64_t message = static_cast<std::int64_t>(i) + base;
                    // keep every real handler, INCLUDING one shared by several messages; drop only the default stub. Fall back to the
                    // uniqueness test when the stub cannot be identified.
                    const bool keep = default_target ? handler != *default_target : std::count(table->begin(), table->end(), handler) == 1;
                    const auto name = message >= 0 ? messages.find(static_cast<std::uint32_t>(message)) : messages.end();
                    if (name != messages.end() && keep && code.instruction_at(handler) != nullptr) {
                        const std::string* label = code.label_at(handler);
                        out.handlers.emplace_back(name->second, label ? *label : loc_label(handler));
                    }
                }
            }
            if (out.handlers.empty()) return {};
            return out;
        }
        a += row->size;
    }

    // --- the compare-chain form: cmp eax,N ; je handler (repeated) ---
    a = start;
    ControlDispatch out;
    std::set<std::int64_t> seen;
    while (const Instruction* row = code.instruction_at({object, a})) {
        if (row->mnemonic != "cmp" || !starts_with_eax(row->operands)) break;
        const auto m = second_operand(row->operands);
        if (!m) break;
        const Instruction* jump = code.instruction_at({object, a + row->size});
        if (jump == nullptr || (jump->mnemonic != "je" && jump->mnemonic != "jz")) break;
        const std::string label = branch_label(code, object, a + row->size, *jump, emittable);
        const auto name = *m >= 0 ? messages.find(static_cast<std::uint32_t>(*m)) : messages.end();
        if (name == messages.end() || label.empty() || seen.contains(*m)) return {};   // this arm cannot be reproduced: emit verbatim
        seen.insert(*m);
        out.handlers.emplace_back(name->second, label);
        skip_bytes(out.skip, object, a, a + row->size + jump->size);
        a += row->size + jump->size;
    }
    if (out.handlers.size() < 2) return {};   // a lone cmp/je is not a dispatch chain
    while (const Instruction* tail = code.instruction_at({object, a})) {   // the trailing default (fall through or a jmp)
        if (!is_stub_instruction(tail->mnemonic) && tail->mnemonic != "jmp") break;
        skip_bytes(out.skip, object, a, a + tail->size);
        a += tail->size;
        if (tail->mnemonic == "jmp") break;
    }
    return out;
}

// ---- pieces of the listing --------------------------------------------------------------------------------------------------

std::size_t ddb_size(const Emission& code, const le::Ddb& ddb) {
    const ByteView bytes = code.code(ddb.object);
    const std::uint64_t r = static_cast<std::uint64_t>(ddb.rva);
    if (r + 0x40 <= bytes.size() && std::equal(bytes.begin() + static_cast<std::ptrdiff_t>(r + 0x3C), bytes.begin() + static_cast<std::ptrdiff_t>(r + 0x40), as_bytes("verP").begin())) {
        const std::uint32_t size = word_at(bytes, r + 0x40).value_or(0);
        return 0x34 <= size && size <= 0x80 ? size : 0x50;
    }
    return 0x34;
}

std::vector<std::string> data_lines(ByteView bytes, std::size_t width) {
    const auto printable = [&](std::size_t i) { return bytes[i] >= 0x20 && bytes[i] <= 0x7E; };
    // the end of the printable run at `from`, and whether it is long enough (four) to be a string
    const auto run_end = [&](std::size_t from) {
        std::size_t j = from;
        while (j < bytes.size() && printable(j)) ++j;
        return j;
    };
    std::vector<std::string> out;
    const std::size_t n = bytes.size();
    std::size_t i = 0;
    while (i < n) {
        if (printable(i)) {   // a printable run of four or more becomes a string literal
            const std::size_t j = run_end(i);
            if (j - i >= 4) {
                for (std::size_t c = i; c < j; c += width) {
                    std::string literal;
                    for (std::size_t k = c; k < std::min(c + width, j); ++k) {
                        if (bytes[k] == '\'') literal += '\'';
                        literal += static_cast<char>(bytes[k]);
                    }
                    out.push_back("        db  '" + literal + "'");
                }
                i = j;
                continue;
            }
        }
        std::size_t j = i;   // else a hex chunk up to the next such run
        while (j < n) {
            if (printable(j) && run_end(j) - j >= 4) break;
            ++j;
        }
        for (std::size_t c = i; c < j; c += 16) out.push_back(masm::db_line(bytes.subspan(c, std::min<std::size_t>(16, j - c))));
        i = j;
    }
    return out;
}

SegmentName object_segment(std::uint32_t object, std::uint32_t flags, std::optional<std::uint32_t> ddb_object) {
    if (ddb_object && object == *ddb_object) return {"_LTEXT", "LCODE"};   // the DDB's object: the DDK's own segment
    return {std::format("VSEG{:02}", object), std::format("S{:02}{}", object, (flags & 0x0004) ? "CODE" : "DATA")};
}

void label_fixup_targets(Emission& code) {
    std::unordered_set<Place, PlaceHash> inside;   // offsets INSIDE an instruction
    for (const auto& [at, insn] : code.instructions()) {
        for (std::uint32_t k = 1; k < insn.size; ++k) inside.insert({at.object, at.offset + k});
    }
    std::map<std::uint32_t, std::uint32_t> sizes;
    for (const le::Object& o : code.driver().objects()) sizes[o.index] = o.vsize;
    for (const auto& [at, fixup] : code.fixups()) {
        const auto* f = std::get_if<le::InternalFixup>(&fixup);
        if (f == nullptr || !f->target) continue;
        const Place target{f->object, static_cast<std::int64_t>(*f->target & kOffsetMask)};
        if (code.labels().contains(target)) continue;
        if (const auto size = sizes.find(target.object); size != sizes.end() && target.offset >= size->second) continue;   // beyond the object: raw
        if (code.instructions().contains(target) || !inside.contains(target)) code.labels()[target] = loc_label(target);
    }
}

// ---- the loadable emission --------------------------------------------------------------------------------------------------

namespace {

// What the loadable emission knows of the DDB the macro regenerates, and of the regions it owns.
struct LoadContext {
    std::string name;
    std::uint32_t ddb_object = 0;
    std::int64_t ddb_rva = 0, ddb_end = 0;
    const PlaceSet* skip = nullptr;   // places the control-dispatch and service-table macros regenerate
};

const char* jcc_name(std::uint8_t opcode) {
    static constexpr std::array<const char*, 16> names = {"jo", "jno", "jb", "jae", "je", "jne", "jbe", "ja", "js", "jns", "jp", "jnp", "jl", "jge", "jle", "jg"};
    return opcode >= 0x80 && opcode <= 0x8F ? names[opcode - 0x80] : nullptr;
}

// The emitted label an INTERNAL fix-up at `at` points to, or empty (an import, no label).
std::string fixup_label(const Emission& code, const Place& at) {
    const le::InternalFixup* f = internal_at(code, at);
    if (f == nullptr || !f->target) return {};
    return label_text(code, {f->object, *f->target});
}

// The `dd` operand for a fix-up target: `offset <name>_DDB [+ N]` when it lands inside the DDB the macro regenerates (no label exists
// there), else `offset <label>`; nullopt when there is none. A target with the alias bit is written `offset <label> + 80000000h` so
// LINK writes a relocation whose loaded value carries the alias, as retail does; dropping these as out-of-bounds left the pointer
// unrelocated and the VxD dereferencing low garbage (the multi-VxD init hang).
std::optional<std::string> dd_target_expr(const Emission& code, std::uint32_t target_object, const std::optional<std::uint32_t>& target_offset, const LoadContext* ctx) {
    if (!target_offset) return std::nullopt;   // a selector fix-up has no offset to point at
    const std::string_view alias = (*target_offset & kHighLinearAlias) ? kAliasText : std::string_view();
    const std::int64_t base = *target_offset & kOffsetMask;
    if (ctx != nullptr && target_object == ctx->ddb_object && ctx->ddb_rva <= base && base < ctx->ddb_end) {
        const std::int64_t d = base - ctx->ddb_rva;
        return std::format("offset {}_DDB{}{}", ctx->name, d ? std::format(" + {:02X}h", d) : std::string(), alias);
    }
    // a target in a region a macro regenerates has no label: stay raw. The pointer loses its relocation, but the region is the macro's
    // and the build assembles (a symbolic `offset loc_..` would be undefined, A2006)
    if (ctx != nullptr && ctx->skip != nullptr && ctx->skip->contains({target_object, base})) return std::nullopt;
    // A target BEYOND the object's size is a reference to the object's base plus a large offset (a VxD's DOS/V86 memory window at
    // obj+0x400000): no label exists that far in, so anchor on the object's base label and write the full offset, and LINK writes a
    // relocation with the addend, as retail does (dropping them left the pointer unrelocated: an init protection fault in dosmgr, vfat).
    for (const le::Object& o : code.driver().objects()) {
        if (o.index == target_object && base >= o.vsize) {
            const std::string* anchor = code.label_at({target_object, 0});
            if (anchor == nullptr) return std::nullopt;
            return std::format("offset {} + {:X}h{}", *anchor, base, alias);
        }
    }
    const std::string* label = code.label_at({target_object, base});
    if (label == nullptr) return std::nullopt;
    return std::format("offset {}{}", *label, alias);
}

std::string branch_text(std::string_view mnemonic, std::string_view label) { return std::format("        {:<6} {}", mnemonic, label); }

// An instruction's INTERNAL fix-ups emitted symbolically, so LINK writes real LE relocations (the byte-exact path writes them raw, which
// drops the relocation and gives a wrong pointer on load). A near call or jmp whose displacement carries a self-relative fix-up becomes
// `call/jmp <label>` (the assembler recomputes the displacement after LINK places the segments); any other 32-bit offset field becomes
// `dd offset <label>` (or `offset <name>_DDB + N` into the DDB), splitting the instruction into db, dd and db. nullopt falls through:
// no fix-up, a service call, an import, an unresolved target.
std::optional<std::vector<std::string>> loadable_instruction(const Emission& code, std::uint32_t object, std::uint32_t a, const Instruction& insn,
                                                             const LoadContext* ctx) {
    std::vector<std::uint32_t> fixes;
    for (std::uint32_t k = a; k < a + insn.size; ++k) {
        if (code.fixups().contains({object, k})) fixes.push_back(k);
    }
    if (fixes.empty() || insn.service) return std::nullopt;
    const ByteView bytes = code.code(object);
    const std::uint8_t op = bytes[a];
    if (insn.size == 5 && (op == 0xE8 || op == 0xE9) && fixes == std::vector<std::uint32_t>{a + 1}) {   // a self-relative near branch
        const std::string label = fixup_label(code, {object, a + 1});
        if (label.empty()) return std::nullopt;
        return std::vector<std::string>{branch_text(op == 0xE8 ? "call" : "jmp", label)};
    }
    if (insn.size == 6 && op == 0x0F && jcc_name(bytes[a + 1]) != nullptr && fixes == std::vector<std::uint32_t>{a + 2}) {
        const std::string label = fixup_label(code, {object, a + 2});
        if (label.empty()) return std::nullopt;
        return std::vector<std::string>{branch_text(jcc_name(bytes[a + 1]), label)};
    }
    std::map<std::uint32_t, std::string> expressions;   // the absolute 32-bit offset field(s)
    for (const std::uint32_t k : fixes) {
        const le::InternalFixup* f = internal_at(code, {object, k});
        const auto e = f != nullptr ? dd_target_expr(code, f->object, f->target, ctx) : std::nullopt;
        if (!e) return std::nullopt;
        expressions[k] = *e;
    }
    std::vector<std::string> out;
    std::uint32_t prev = a;
    for (const std::uint32_t k : fixes) {
        if (k > prev) out.push_back(masm::db_line(bytes.subspan(prev, k - prev)));
        out.push_back("        dd  " + expressions[k]);
        prev = k + 4;
    }
    if (prev < a + insn.size) out.push_back(masm::db_line(bytes.subspan(prev, a + insn.size - prev)));
    return out;
}

// code[a, b) as raw db with `<name> label near` at each interior offset of `labels` (a fix-up target that fell mid-instruction):
// byte-preserving, and it gives the target an address. `near` (a code address) and not `byte`: a branch target needs a near label
// (`je <byte>` is A2024); `dd offset` and data pointers accept a near label too. The directive emits no bytes.
std::vector<std::string> db_split_labels(ByteView bytes, std::uint32_t a, std::uint32_t b, const std::map<std::uint32_t, std::string>& labels) {
    std::vector<std::string> out;
    std::uint32_t run = a;
    for (const auto& [offset, name] : labels) {
        if (offset < a || offset >= b) continue;
        if (offset > run) out.push_back(masm::db_line(bytes.subspan(run, offset - run)));
        out.push_back(name + " label near");
        run = offset;
    }
    if (run < b) out.push_back(masm::db_line(bytes.subspan(run, b - run)));
    return out;
}

// A data span [a, b): each INTERNAL-fix-up DWORD becomes `dd offset <label>` / `dd offset <name>_DDB + N` (a real relocation), the rest goes
// through data_lines.
//
// A SELF-RELATIVE fix-up (source type 8) whose opcode byte (E8 call, E9 jmp, or 0F 8x jcc) sits just before it is a cross-object branch the
// descent left in a data gap. It is emitted as the branch, which LINK relocates self-relative, and not as `dd offset` (absolute, source
// type 7): that corrupts the branch target and faults the VxD on init (the residual multi-VxD hang in dosmgr, vxdldr and vshare).
std::vector<std::string> loadable_data(const Emission& code, std::uint32_t object, std::uint32_t a, std::uint32_t b, const LoadContext* ctx) {
    std::vector<std::string> out;
    const ByteView bytes = code.code(object);
    // Labels INSIDE this span (a fix-up target elsewhere that lands here: common when the span is an in-place DDB or service-table block
    // holding a pointer table, each entry both a `dd offset` and the target of a `call [loc_..]`). data_lines defines no labels, so these
    // would be undefined (A2006). Offset `a` is excluded: the caller defines it.
    std::map<std::uint32_t, std::string> interior;
    for (auto it = code.labels().upper_bound({object, a}); it != code.labels().end() && it->first.object == object && it->first.offset < b; ++it) {
        interior[static_cast<std::uint32_t>(it->first.offset)] = it->second;
    }
    // readable data for [lo, hi), the interior labels defined inside (lo, hi) -- never inside a dd, where the run jumps past them
    const auto flush = [&](std::uint32_t lo, std::uint32_t hi) {
        std::uint32_t r = lo;
        for (const auto& [offset, name] : interior) {
            if (offset <= lo || offset >= hi) continue;
            if (offset > r) {
                for (std::string& line : data_lines(bytes.subspan(r, offset - r))) out.push_back(std::move(line));
            }
            out.push_back(name + " label near");
            r = offset;
        }
        if (r < hi) {
            for (std::string& line : data_lines(bytes.subspan(r, hi - r))) out.push_back(std::move(line));
        }
    };
    std::uint32_t run = a, k = a;
    while (k < b) {
        const le::InternalFixup* f = internal_at(code, {object, k});
        if (f != nullptr) {
            const std::int64_t target = f->target ? (*f->target & kOffsetMask) : 0;
            const auto branch = [&](std::uint32_t opcode_at, const std::string& mnemonic) -> bool {
                const std::string label = label_text(code, {f->object, target});
                if (label.empty()) return false;
                if (opcode_at > run) flush(run, opcode_at);
                if (const auto it = interior.find(opcode_at); it != interior.end()) out.push_back(it->second + " label near");   // a label on the opcode byte
                out.push_back(branch_text(mnemonic, label));
                k += 4;
                run = k;
                return true;
            };
            if (f->source_type == 8 && k >= a + 1 && (bytes[k - 1] == 0xE8 || bytes[k - 1] == 0xE9) && k + 4 <= b) {
                if (branch(k - 1, bytes[k - 1] == 0xE8 ? "call" : "jmp")) continue;
            }
            // a two-byte jcc (0F 8x rel32), self-relative, in a data gap: the jcc, not dd offset
            if (f->source_type == 8 && k >= a + 2 && bytes[k - 2] == 0x0F && jcc_name(bytes[k - 1]) != nullptr && k + 4 <= b) {
                if (branch(k - 2, jcc_name(bytes[k - 1]))) continue;
            }
            const auto expression = dd_target_expr(code, f->object, f->target, ctx);
            if (expression && k + 4 <= b) {
                if (k > run) flush(run, k);
                if (const auto it = interior.find(k); it != interior.end()) out.push_back(it->second + " label near");   // at a pointer table entry's start
                out.push_back("        dd  " + *expression);
                k += 4;
                run = k;
                continue;
            }
        }
        ++k;
    }
    if (run < b) flush(run, b);
    return out;
}

// ---- the encodings ML will not reproduce ------------------------------------------------------------------------------------

bool is_prefix_byte(std::uint8_t b) {
    return b == 0x66 || b == 0x67 || b == 0xF0 || b == 0xF2 || b == 0xF3 || b == 0x2E || b == 0x26 || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65;
}

// Whether a prefix `wanted` leads the instruction (among up to four prefix bytes).
bool has_prefix(const Instruction& insn, std::uint8_t wanted) {
    for (std::size_t i = 0; i < insn.raw.size() && i < 4; ++i) {
        if (insn.raw[i] == wanted) return true;
        if (!is_prefix_byte(insn.raw[i])) break;
    }
    return false;
}

// A mnemonic the period ML (MASM 6.11) rejects or re-encodes -- almost all are byte runs the linear sweep reads as exotic or system
// instructions where the real bytes are data. Byte-preserving keeps the exact bytes and keeps ML out of trouble:
//   aam/aad (an explicit 0Ah operand ML errors on), int1/int3 (capstone's one-token INT), lcall/ljmp (its far-branch spelling), ud0/ud1/ud2,
//   lar/lsl (a 16-bit selector into a 32-bit register: operand size mismatch), salc (0xD6), les/lds/lfs/lgs/lss (far-pointer loads,
//   capstone's `ptr [..]`), the SSE-era x87 the 1994 assembler predates (fisttp, fxsave, fxrstor), the descriptor-table loads (capstone drops
//   the `fword ptr` size), and the post-1994 fast system calls (data misread).
bool is_raw_mnemonic(std::string_view mnemonic) {
    return is_one_of(mnemonic, {"aam", "aad", "lcall", "ljmp", "lar", "lsl", "int3", "int1", "ud0", "ud1", "ud2", "salc", "les", "lds", "lfs", "lgs", "lss",
                                "fisttp", "fxsave", "fxrstor", "lidt", "lgdt", "sidt", "sgdt", "lldt", "sldt", "sysret", "sysenter", "sysexit"});
}

// A shift or rotate with a single operand: the implicit `,1` (the D0/D1 encoding) capstone drops and ML requires as an explicit count.
// Byte-preserved, to keep the D0/D1 byte rather than risk ML re-encoding `,1` to the C0/C1 imm8 form.
bool is_shift_without_count(const Instruction& insn) {
    return is_one_of(insn.mnemonic, {"rcl", "rcr", "rol", "ror", "shl", "shr", "sal", "sar"}) && insn.operands.find(',') == std::string::npos;
}

// What the 1994 MASM 6.11 predates: SSE (xmm registers, 1999), MMX (mm registers, 1997) and the P6 conditional moves (1995). In a stripped
// ring-0 VxD these are invariably data the linear sweep misread.
bool is_unknown_to_ml611(const Instruction& insn) {
    if (insn.mnemonic.starts_with("cmov") || insn.mnemonic.starts_with("fcmov")) return true;
    const std::string_view ops = insn.operands;
    for (std::size_t i = 0; i < ops.size(); ++i) {   // \b(?:xmm|mm)[0-7]\b
        if (i > 0 && is_word_char(ops[i - 1])) continue;
        const std::size_t name = ops.compare(i, 3, "xmm") == 0 ? 3 : (ops.compare(i, 2, "mm") == 0 ? 2 : 0);
        if (name == 0 || i + name >= ops.size()) continue;
        const char digit = ops[i + name];
        if (digit >= '0' && digit <= '7' && (i + name + 1 >= ops.size() || !is_word_char(ops[i + name + 1]))) return true;
    }
    return false;
}

// An x87 op with a stack-register operand (`fadd st(3)`, capstone's `fstpnce`): capstone's spelling the DDK ML rejects. Rare in ring-0 code
// and almost always data misdecoded.
bool is_x87_stack_operand(const Instruction& insn) { return insn.mnemonic.starts_with('f') && insn.operands.find("st(") != std::string::npos; }

// A jmp or call through memory (`call [edx+4]`): capstone writes a sizeless `ptr [..]` the DDK ML rejects (A2016). It has no label target
// either; the recovered dispatch tables are handled separately.
bool is_indirect_branch(const Instruction& insn) { return (insn.mnemonic == "jmp" || insn.mnemonic == "call") && insn.operands.find('[') != std::string::npos; }

// capstone's 80-bit memory sizes `xword` and `tword ptr`: the DDK ML says `tbyte ptr` and reads `xword` as an undefined symbol.
bool has_capstone_size(const Instruction& insn) { return insn.operands.find("xword") != std::string::npos || insn.operands.find("tword") != std::string::npos; }

// A 0x67 address-size prefix: 16-bit addressing (`[di]`, `[bp+si]`) in a use32 segment, which ML rejects (A2155). Almost always data misread.
bool has_address16(const Instruction& insn) { return has_prefix(insn, 0x67); }

// A 0x66 operand-size prefix: a 16-bit operand in a use32 segment (`push word ..`, `mov word ptr .., cx`). The readable re-emit loses the
// prefix (a word `push imm` comes back `push 2`, 32 bits, `6a 02`), so the rebuilt operation moves 4 bytes where the original moved 2,
// corrupting the stack or data at run time: this faulted the from-source BIOS VxD, which drives real-mode BIOS with 16-bit values. Fix-up
// bearing operations never get here (the loadable path takes them first), so no relocation is lost.
bool has_operand16(const Instruction& insn) { return has_prefix(insn, 0x66); }

// An instruction whose ORIGINAL encoding carries an explicit zero displacement: a memory operand with ModRM mod 01 or 10 and disp 0
// (`mov [esp],ecx` encoded `89 4C 24 00` as `[esp+0]`). The assembler re-encodes it minimally (`89 0C 24`), making our code a byte shorter and
// shifting every later offset, so an internal call or branch target drifts and, a few handlers in, faults. ([ebp+0] is mod 01 disp8 0 too but
// the assembler keeps it, so preserving is harmless.) Decoding with detail is costly and instruction bytes repeat heavily, so the answers are
// kept by the instruction's bytes for the life of the emission.
class ZeroDisplacement {
public:
    bool in(const Instruction& insn) {
        const Bytes& key = insn.raw;
        if (const auto hit = known_.find(key); hit != known_.end()) return hit->second;
        const bool found = decode(key);
        known_.emplace(key, found);
        return found;
    }

private:
    bool decode(ByteView raw) {
        if (!decoder_) decoder_.emplace(platforms::Decoder::open(platforms::Arch::X86, 32, true));   // opened on first use: the loadable emission never needs it
        bool found = false;
        try {
            decoder_->each(raw, 0, 1, [&](const cs_insn& insn) {
                const cs_x86& x86 = insn.detail->x86;
                const unsigned mod = (x86.modrm >> 6) & 3;
                if (mod == 1 || mod == 2) {
                    for (std::uint8_t i = 0; i < x86.op_count; ++i) {
                        if (x86.operands[i].type == X86_OP_MEM && x86.operands[i].mem.disp == 0) found = true;
                    }
                }
                return false;
            });
        } catch (const Error&) {
            found = false;   // a byte run capstone cannot decode is not one we can say anything of
        }
        return found;
    }
    std::optional<platforms::Decoder> decoder_;
    std::map<Bytes, bool> known_;   // by the instruction's bytes; a handful of thousand
};

// A branch operand that is a bare number (maybe SHORT or NEAR PTR in front) -- ML's "jump destination must specify a label": a self-relative
// call or jmp (e8/e9 rel32 = 0, a PIC idiom) our disassembly writes numerically. Byte-preserved.
bool is_number_target(std::string_view operand) {
    for (const std::string_view prefix : {"SHORT ", "NEAR PTR ", ""}) {
        if (!operand.starts_with(prefix)) continue;
        std::string_view rest = operand.substr(prefix.size());
        if (rest.empty() || rest.front() < '0' || rest.front() > '9') continue;
        std::size_t i = 1;
        while (i < rest.size() && is_hex_digit(rest[i])) ++i;
        if (i < rest.size() && rest[i] == 'h') ++i;
        if (i == rest.size()) return true;
    }
    return false;
}

// The line of a branch (`call`, `jmp`, `jcc`) as MASM should see it, or nullopt to byte-preserve it: a target we do not emit a label for,
// or a bare number.
std::optional<std::string> branch_line(std::string line, const Instruction& insn, std::uint32_t object, const PlaceSet& emitted) {
    const std::size_t space = line.find_first_not_of(' ');
    const std::size_t gap = space == std::string::npos ? std::string::npos : line.find(' ', space);
    const std::string_view operand = gap == std::string::npos ? std::string_view() : text::trim(std::string_view(line).substr(gap));
    const auto target = insn.direct_target();
    if (!operand.empty() && is_number_target(operand)) return std::nullopt;
    if (target && !emitted.contains({object, *target})) return std::nullopt;
    line = text::replace_all(text::replace_all(line, "SHORT ", ""), "NEAR PTR ", "");
    // Keep the ORIGINAL near or short encoding. A near branch (jcc rel32, 6 bytes; jmp rel32, 5) is written `near ptr` so the assembler keeps
    // rel32 instead of shrinking it to rel8, reproducing the original's bytes (the dominant cause of the from-source object-size drift and of the
    // jump-displacement cascade) with none of the out-of-range risk pinning SHORT carries: rel32 always reaches. A short original stays unsized: the
    // assembler picks, and may legitimately widen it if the layout moved. loop and jcxz have no near form, so gate on jmp (5 or more) and jcc (6).
    const bool near = (insn.mnemonic == "jmp" && insn.size >= 5) || (insn.mnemonic.starts_with('j') && insn.mnemonic != "jmp" && insn.size >= 6);
    if (near) {
        const std::string_view trimmed = text::trim(line);
        const std::size_t blank = trimmed.find(' ');
        const std::string_view mnemonic = trimmed.substr(0, blank);
        const std::string_view rest = blank == std::string_view::npos ? std::string_view() : text::trim(trimmed.substr(blank + 1));
        line = std::format("        {:<6} near ptr {}", mnemonic, rest);
    }
    return line;
}

// ---- the emission -----------------------------------------------------------------------------------------------------------

class Emitter {
public:
    Emitter(Emission& code, bool loadable) : code_(code), loadable_(loadable), ddb_(code.ddb()), name_(ddb_ ? ddb_->name : "DEVICE") {}

    std::string run() {
        rename_colliding_labels();
        if (loadable_) prepare_loadable();
        find_emittable();
        plan_macros();
        lines_ = {".586p"};   // privileged, a superset of .386p: ring-0 486 and Pentium ops (invd, wbinvd, cpuid, cmpxchg, rdmsr, wrmsr, rdtsc) assemble as themselves
        for (const std::string& inc : all_includes()) lines_.push_back("include " + text::lower(inc) + ".inc");
        lines_.push_back("");
        write_interior_aliases();
        if (loadable_) predeclare_segments();
        write_macros();
        const PlaceSet emitted = minus(emittable_, cp_skip_);
        // a control handler whose label we do not emit (a stub VxD whose messages all dispatch into the control-proc region, or a shared
        // do-nothing stub) would make Control_Dispatch reference an undefined symbol: drop it and that message falls to the macro default
        std::set<std::string> emitted_names;
        for (const Place& p : emitted) emitted_names.insert(code_.labels().at(p));
        std::erase_if(dispatch_.handlers, [&](const auto& h) { return !emitted_names.contains(h.second); });
        drop_instructions_in_regions();
        for (const le::Object& o : code_.driver().objects()) write_object(o, emitted);
        lines_.push_back("        end");
        return text::join(lines_, "\r\n") + "\r\n";
    }

private:
    static PlaceSet minus(const PlaceSet& a, const PlaceSet& b) {
        PlaceSet out;
        for (const Place& p : a) {
            if (!b.contains(p)) out.insert(p);
        }
        return out;
    }

    // a label that collides with a symbol the macros generate (<name>_DDB from Declare_Virtual_Device, <name>_Service_Table from
    // Begin_Service_Table): e.g. VDD's .SYM names a routine VDD_DDB at an offset that is NOT the real DDB. Renamed to the offset-based
    // name, in the labels themselves so references follow, or ML reports A2005.
    void rename_colliding_labels() {
        for (auto& [at, label] : code_.labels()) {
            if (label == name_ + "_DDB" || label == name_ + "_Service_Table") label = sub_label(at);
        }
    }

    std::vector<std::string> all_includes() const {
        std::set<std::string> incs;
        for (const le::Object& o : code_.driver().objects()) {
            if (!o.executable()) continue;
            for (std::string& inc : device_includes(code_, o.index)) incs.insert(std::move(inc));
        }
        incs.erase("VMM");   // VMM.INC MUST come first: it defines the macros (Begin_Service_Table, ...) the other device INCs invoke when included
        std::vector<std::string> out = {"VMM"};
        out.insert(out.end(), incs.begin(), incs.end());
        return out;
    }

    // Loadable: every internal fix-up target needs a label the symbolic emission can reference; a target that lands mid-instruction forces its
    // instruction to raw db so the label sits on the exact byte.
    void prepare_loadable() {
        label_fixup_targets(code_);
        if (ddb_) load_ctx_ = LoadContext{name_, ddb_->object, ddb_->rva, ddb_->rva + static_cast<std::int64_t>(ddb_size(code_, *ddb_)), nullptr};
        std::unordered_set<Place, PlaceHash> interior;
        for (const auto& [at, insn] : code_.instructions()) {
            for (std::uint32_t k = 1; k < insn.size; ++k) interior.insert({at.object, at.offset + k});
        }
        std::map<std::uint32_t, std::uint32_t> sizes;
        for (const le::Object& o : code_.driver().objects()) sizes[o.index] = o.vsize;
        for (const auto& [at, fixup] : code_.fixups()) {
            const auto* f = std::get_if<le::InternalFixup>(&fixup);
            if (f == nullptr || !f->target) continue;
            const Place target{f->object, static_cast<std::int64_t>(*f->target & kOffsetMask)};   // the low 31 bits are the real object offset
            if (const auto size = sizes.find(target.object); size != sizes.end() && target.offset >= size->second) {
                code_.labels().emplace(Place{target.object, 0}, loc_label({target.object, 0}));   // beyond the object: anchor the reference on its base label
                continue;
            }
            const std::string* existing = code_.label_at(target);
            const std::string label = existing != nullptr ? *existing : loc_label(target);
            code_.labels().emplace(target, label);
            if (interior.contains(target)) {
                // Done even when label_fixup_targets already named it: a misdecoded pointer table decodes run to run into different instruction
                // boundaries, so a target skipped as a data byte there can be mid-instruction here, and without the split the
                // `dd offset loc_..` is undefined. A start or a data target is untouched.
                force_labels_[target] = label;
            }
        }
        // A NARROW overlapping-pointer case: a label strictly inside the source dword [S+1, S+4) of an internal fix-up (two relocated pointers overlap in
        // misdecoded data: a `dd offset` whose target lands mid-another-pointer). The label cannot be emitted inline (that would split the dd and drop a
        // relocation). It is recorded, the dword's start is anchored, and it is written later as `EQU <S-label> + delta`, so the dd stays whole and every
        // referrer still works. Gated on an existing label: a byte-exact module without the overlap is unaffected.
        for (const auto& [at, fixup] : code_.fixups()) {
            const auto* f = std::get_if<le::InternalFixup>(&fixup);
            if (f == nullptr || at.offset < 0) continue;   // a source dword that straddles in from the previous object is recorded at a negative offset: no anchor here
            const ByteView bytes = code_.code(at.object);
            // The emittable ANCHOR of a fix-up unit: for a data pointer (type 7) its dword start; for a self-relative BRANCH (type 8) the branch OPCODE (E8/E9
            // one byte before the rel32, or 0F 8x two before), because the whole branch is re-emitted from the opcode. A label inside the unit
            // (not at the anchor) becomes an EQU alias.
            std::int64_t anchor = at.offset;
            if (f->source_type == 8) {
                if (at.offset >= 1 && at.offset - 1 < static_cast<std::int64_t>(bytes.size()) && (bytes[at.offset - 1] == 0xE8 || bytes[at.offset - 1] == 0xE9)) {
                    anchor = at.offset - 1;
                } else if (at.offset >= 2 && at.offset - 1 < static_cast<std::int64_t>(bytes.size()) && bytes[at.offset - 2] == 0x0F && jcc_name(bytes[at.offset - 1]) != nullptr) {
                    anchor = at.offset - 2;
                }
            }
            for (std::int64_t o = anchor + 1; o < at.offset + 4; ++o) {
                if (code_.labels().contains({at.object, o}) && !interior_alias_.contains({at.object, o})) {
                    interior_alias_[{at.object, o}] = anchor;
                    code_.labels().emplace(Place{at.object, anchor}, loc_label({at.object, anchor}));
                }
            }
        }
    }

    // The offsets that will carry an emitted label: one on an instruction start or on a data byte, not one inside an instruction.
    void find_emittable() {
        PlaceSet starts, inside;
        for (const auto& [at, insn] : code_.instructions()) {
            starts.insert(at);
            for (std::uint32_t k = 1; k < insn.size; ++k) inside.insert({at.object, at.offset + k});
        }
        for (const auto& [at, label] : code_.labels()) {
            if (starts.contains(at) || !inside.contains(at)) emittable_.insert(at);
        }
    }

    // The control dispatch and the DDB's skip first, so the service table is validated against what the walk will emit (a handler inside the
    // dispatch region would be referenced and skipped).
    void plan_macros() {
        dispatch_ = control_dispatch(code_, emittable_);
        cp_skip_ = dispatch_.skip;
        if (!dispatch_.handlers.empty()) {
            const std::string control = name_ + "_Control";
            const auto entry = std::find_if(code_.labels().begin(), code_.labels().end(), [&](const auto& kv) { return kv.second == control; });
            if (entry != code_.labels().end()) control_at_ = entry->first;
        }
        if (ddb_ && !loadable_) {   // the macro re-emits the DDB: skip it in the walk (the loadable emission writes it in place, at its real
            skip_bytes(cp_skip_, ddb_->object, ddb_->rva, ddb_->rva + static_cast<std::int64_t>(ddb_size(code_, *ddb_)));   // offset, so a pointer handed to a VxD
        }                                                                                                                    // service carries the original's)
        service_ = service_table(code_, minus(emittable_, cp_skip_));   // handlers must be emitted, not skipped
        if (service_ && !loadable_) skip_bytes(cp_skip_, service_->at.object, service_->at.offset, service_->at.offset + 4 * static_cast<std::int64_t>(service_->count));
        if (load_ctx_) load_ctx_->skip = &cp_skip_;   // so a pointer into a macro-owned region is written raw
    }

    void write_interior_aliases() {
        // EQU aliases for labels that land inside a relocated pointer's dword (overlapping pointers in misdecoded data): defined here, at the top, so
        // every forward reference resolves, while the dword itself is one whole `dd offset`. `<interior> EQU <dword start> + delta`.
        for (const auto& [at, start] : interior_alias_) {
            const std::string interior = label_text(code_, at), anchor = label_text(code_, {at.object, start});
            if (!interior.empty() && !anchor.empty() && interior != anchor) lines_.push_back(std::format("{} EQU {} + {}", interior, anchor, at.offset - start));
        }
        if (!interior_alias_.empty()) lines_.push_back("");
    }

    // Each object's own FLAT segment is declared first, with a unique class so LINK keeps it a separate object: a writable data object is not merged into
    // the read-only code.
    void predeclare_segments() {
        for (const le::Object& o : code_.driver().objects()) {
            const SegmentName s = object_segment(o.index, o.flags, ddb_ ? std::optional<std::uint32_t>(ddb_->object) : std::nullopt);
            lines_.push_back(std::format("{} SEGMENT DWORD PUBLIC _FLAT '{}'", s.name, s.class_name));
            lines_.push_back(s.name + " ENDS");
        }
        lines_.push_back("");
    }

    // The service table and the DDB, written inline before the code. (Written after it they regressed VMM device init; retail has the code first
    // in the locked object, and the loadable emission gets that from writing them in place.)
    void write_macros() {
        if (service_) {
            std::set<std::string> service_names;
            for (const std::string& n : service_->names) {
                if (n != "RESERVED") service_names.insert(n);
            }
            // Begin_Service_Table makes a PUBLIC caller thunk `_<service>` for each service: a reconstructed label of that exact name (a .SYM ships those thunks)
            // is renamed, or ML reports A2005.
            for (auto& [at, label] : code_.labels()) {
                if (label.starts_with('_') && service_names.contains(label.substr(1))) label = sub_label(at);
            }
            lines_.push_back(std::format("IFNDEF {}_Device_ID", name_));   // the service macro forms each ordinal from <name>_Device_ID, which the
            if (ddb_) lines_.push_back(std::format("{}_Device_ID EQU 0{:04X}h", name_, ddb_->device_id));   // device's own INC (absent, perhaps) would define
            lines_.push_back("ENDIF");
            lines_.push_back(std::format("Create_{}_Service_Table EQU 1", name_));   // before the DDB, so Declare_Virtual_Device fills the table's pointer and size
        }
        if (service_ && !loadable_) {
            lines_.push_back("Begin_Service_Table " + name_);
            for (const std::string& n : service_->names) {   // all LOCAL: every handler is in this one file
                lines_.push_back(n != "RESERVED" ? std::format("        {}_Service {}, LOCAL", name_, n) : std::format("        {}_Service RESERVED", name_));
            }
            lines_.push_back("End_Service_Table " + name_);
            lines_.push_back("");
        }
        if (ddb_ && !loadable_) {
            lines_.push_back(declare_virtual_device(code_, *ddb_, false));
            lines_.push_back("");
        }
    }

    // A data byte just before the DDB or the table can be misdecoded as an instruction that SPANS the region's start, so the walk would jump over
    // the landing offset: drop any instruction that overlaps an in-place region (start or interior). It is data, and the in-place branch writes the
    // region correctly.
    void drop_instructions_in_regions() {
        if (!loadable_) return;
        std::vector<std::tuple<std::uint32_t, std::int64_t, std::int64_t>> regions;
        if (ddb_) {
            regions.emplace_back(ddb_->object, ddb_->rva, ddb_->rva + static_cast<std::int64_t>(ddb_size(code_, *ddb_)));
            inplace_stops_.insert({ddb_->object, ddb_->rva});
        }
        if (service_) {
            regions.emplace_back(service_->at.object, service_->at.offset, service_->at.offset + 4 * static_cast<std::int64_t>(service_->count));
            inplace_stops_.insert(service_->at);   // the walk must land on these, or a data span coalesces past them and the public <name>_DDB label is lost
        }
        for (const auto& [object, lo, hi] : regions) {
            auto& insns = code_.instructions();
            for (auto it = insns.begin(); it != insns.end();) {
                it = (it->first.object == object && it->first.offset < hi && it->first.offset + it->second.size > lo) ? insns.erase(it) : std::next(it);
            }
        }
    }

    void write_object(const le::Object& o, const PlaceSet& emitted) {
        const std::uint32_t object = o.index;
        SegmentName segment;
        std::string macro = "VxD_LOCKED_CODE";
        if (loadable_) {   // each object is its own FLAT segment
            segment = object_segment(object, o.flags, ddb_ ? std::optional<std::uint32_t>(ddb_->object) : std::nullopt);
            lines_.push_back(segment.name + " SEGMENT");
            lines_.push_back("   ASSUME cs:FLAT, ds:FLAT, es:FLAT, ss:FLAT");
        } else {
            macro = segment_macro(o.flags);
            lines_.push_back(macro + "_SEG");
        }
        const LoadContext* ctx = load_ctx_ ? &*load_ctx_ : nullptr;
        const std::uint32_t n = static_cast<std::uint32_t>(code_.code(object).size());
        std::string current_proc;
        std::uint32_t a = 0;
        while (a < n) {
            const Place here{object, a};
            if (control_at_ && here == *control_at_) {   // the control dispatch macro block
                lines_.push_back("Begin_Control_Dispatch " + name_);
                for (const auto& [message, handler] : dispatch_.handlers) lines_.push_back(std::format("        Control_Dispatch {:<22} {}", message + ",", handler));
                lines_.push_back("End_Control_Dispatch " + name_);
                ++a;
                continue;
            }
            if (loadable_ && ddb_ && object == ddb_->object && a == ddb_->rva) {
                // The DDB struct written IN PLACE at its real offset as relocatable data (dd offset for the pointer fields), with the public label: the DDB
                // lands where the original has it and a pointer to it (passed to a VxD service) is right.
                const std::uint32_t end = static_cast<std::uint32_t>(ddb_->rva + static_cast<std::int64_t>(ddb_size(code_, *ddb_)));
                lines_.push_back("public " + name_ + "_DDB");
                lines_.push_back(name_ + "_DDB label byte");
                for (std::string& line : loadable_data(code_, object, a, end, ctx)) lines_.push_back(std::move(line));
                a = end;
                continue;
            }
            if (loadable_ && service_ && object == service_->at.object && a == service_->at.offset) {   // the service table in place: each entry a fix-up, `dd offset <handler>`
                const std::string label = label_text(code_, service_->at);
                if (!label.empty()) lines_.push_back(label + ":");
                const std::uint32_t end = a + 4 * static_cast<std::uint32_t>(service_->count);
                for (std::string& line : loadable_data(code_, object, a, end, ctx)) lines_.push_back(std::move(line));
                a = end;
                continue;
            }
            if (cp_skip_.contains(here)) {   // the rest of the control procedure and its table: the macros above generate them
                ++a;
                continue;
            }
            if (service_) {
                if (const auto extra = service_->extra.find(here); extra != service_->extra.end()) {   // several services may share one handler
                    for (const std::string& name : extra->second) lines_.push_back(name + ":");
                }
            }
            const bool aliased = interior_alias_.contains(here);   // an interior label is an EQU, not defined inline
            const std::string* label = aliased ? nullptr : code_.label_at(here);
            if (label != nullptr && (label->starts_with("sub_") || label->starts_with(name_ + "_")) && !label->ends_with("_Control")) {
                if (!current_proc.empty()) lines_.push_back("EndProc " + current_proc);
                current_proc = *label;
                lines_.push_back("BeginProc " + *label);
            } else if (label != nullptr) {
                lines_.push_back(*label + ":");
            }
            const Instruction* row = code_.instruction_at(here);
            if (row == nullptr) {
                a = write_data(object, a, n, ctx);
                continue;
            }
            a = write_instruction(object, a, *row, emitted, ctx);
        }
        if (!current_proc.empty()) lines_.push_back("EndProc " + current_proc);
        // The BSS tail: a size beyond the physical bytes is uninitialised data. Reserved, with any label there defined: a data-only object may be purely a
        // relocation target (a DDB, service or jump-table pointer -> loc_<obj>_0000), whose label must exist or the `dd offset ..` is undefined and the segment
        // links empty (combuff's 4-byte PDATA).
        for (std::uint32_t k = n; k < o.vsize;) {
            if (const std::string* label = code_.label_at({object, k})) lines_.push_back(*label + ":");
            std::uint32_t next = k + 1;
            while (next < o.vsize && code_.label_at({object, next}) == nullptr) ++next;
            lines_.push_back(std::format("        db {} dup (0)", next - k));
            k = next;
        }
        lines_.push_back(loadable_ ? segment.name + " ENDS" : macro + "_ENDS");
        lines_.push_back("");
    }

    // The DDK segment macro of an object: discardable code is the init segment, pageable code the pageable one, everything else (data, the
    // message-hook table, preloaded code) the locked one.
    static std::string segment_macro(std::uint32_t flags) {
        if (!(flags & 0x04) || (flags & 0x8000)) return "VxD_LOCKED_CODE";
        if (flags & 0x10) return "VxD_ICODE";
        if (flags & 0x40) return "VxD_LOCKED_CODE";
        return "VxD_PAGEABLE_CODE";
    }

    // Data from `a` to the next instruction, label or region: readable `db 'text'` for the carved strings; in loadable mode data pointers and jump
    // tables become real relocations.
    std::uint32_t write_data(std::uint32_t object, std::uint32_t a, std::uint32_t n, const LoadContext* ctx) {
        std::uint32_t b = a;
        while (b < n) {
            const Place at{object, b};
            const bool stops = b != a && (inplace_stops_.contains(at) || (code_.label_at(at) != nullptr && !interior_alias_.contains(at)) ||
                                         (service_ && service_->extra.contains(at)));
            if (code_.instruction_at(at) != nullptr || cp_skip_.contains(at) || stops) break;
            ++b;
        }
        if (loadable_) {
            for (std::string& line : loadable_data(code_, object, a, b, ctx)) lines_.push_back(std::move(line));
        } else {
            for (std::string& line : data_lines(code_.code(object).subspan(a, b - a))) lines_.push_back(std::move(line));
        }
        return b;
    }

    // One instruction; the new position.
    std::uint32_t write_instruction(std::uint32_t object, std::uint32_t a, const Instruction& row, const PlaceSet& emitted, const LoadContext* ctx) {
        const ByteView bytes = code_.code(object);
        if (loadable_) {
            const auto inside = force_labels_.lower_bound({object, std::int64_t{a} + 1});
            if (inside != force_labels_.end() && inside->first.object == object && inside->first.offset < std::int64_t{a} + row.size) {
                std::map<std::uint32_t, std::string> labels;   // a target sits mid-instruction
                for (auto it = force_labels_.lower_bound({object, std::int64_t{a}}); it != force_labels_.end() && it->first.object == object && it->first.offset < std::int64_t{a} + row.size; ++it) {
                    labels[static_cast<std::uint32_t>(it->first.offset)] = it->second;
                }
                for (std::string& line : db_split_labels(bytes, a, a + row.size, labels)) lines_.push_back(std::move(line));
                return a + row.size;
            }
            // A relocation whose 4-byte field runs PAST this (misdecoded, short) instruction is a data pointer decoded as code. It is written as ONE
            // 4-byte dd: otherwise the `dd offset` would overflow and shift the table, corrupting the data and faulting init.
            std::optional<std::uint32_t> past;
            for (std::uint32_t k = a; k < a + row.size && !past; ++k) {
                if (code_.fixups().contains({object, k}) && k + 4 > a + row.size) past = k;
            }
            if (past) {
                const std::uint32_t k = *past, end = k + 4;
                std::map<std::uint32_t, std::string> buried;
                for (std::uint32_t j = a + 1; j < end; ++j) {
                    if (const std::string* label = code_.label_at({object, j}); label != nullptr) buried[j] = *label;
                }
                if (!buried.empty()) {
                    // This single-dd span would bury a referenced label (an overlapping or misdecoded pointer table: another dword points AT a byte inside this
                    // pointer, or a branch or `dd offset` targets an instruction start the jump steps over). The walk's `a` jumps to k+4 in one step and never
                    // lands on that offset, so neither the label nor the split fires and the `dd offset loc_..` / `jcc loc_..` elsewhere is undefined (A2006).
                    // The whole span is raw db WITH the label(s): every target is defined and byte-exact, and only this one pointer loses its relocation, in a
                    // region the disassembly already misswept. The build assembles and links.
                    for (std::string& line : db_split_labels(bytes, a, end, buried)) lines_.push_back(std::move(line));   // a's own label is already defined
                    return end;
                }
                if (k > a) lines_.push_back(masm::db_line(bytes.subspan(a, k - a)));
                const le::InternalFixup* f = internal_at(code_, {object, k});
                const auto expression = f != nullptr ? dd_target_expr(code_, f->object, f->target, ctx) : std::nullopt;
                lines_.push_back(expression ? "        dd  " + *expression : masm::db_line(bytes.subspan(k, std::min<std::size_t>(4, bytes.size() - k))));
                return end;   // the instructions the pointer overlaps are skipped
            }
        }
        const auto symbolic = loadable_ ? loadable_instruction(code_, object, a, row, ctx) : std::nullopt;
        if (row.service) {
            const auto macro = service_macro(code_.names(), row.service->device, row.service->service);
            lines_.push_back(macro ? *macro : masm::db_line(row.raw));
        } else if (symbolic) {
            for (const std::string& line : *symbolic) lines_.push_back(line);
        } else if (loadable_) {
            // Byte-preserve EVERY remaining (non-relocation) instruction. One that gets here carries no internal fix-up (those went the symbolic way), so its exact
            // bytes are reproducible as `db` and need no symbol. Re-encoding through ML can pick a shorter form (a misdecoded string or data region is the usual
            // case: "HTREE\RESERVED" decodes to instructions ML then shrinks), shifting every later label and leaving data pointers wrong: a mid-init fault
            // (configmg, ios). Byte-preserving makes the object layout exact by construction, so branch displacements and data pointers stay right without the
            // near-ptr re-encoding heuristic.
            lines_.push_back(masm::db_line(row.raw));
        } else if (row.fixup || masm::has_absolute_memory(row) || masm::has_segment_prefix(row) || has_address16(row) || has_operand16(row) ||
                   zero_displacement_.in(row) || is_shift_without_count(row) || is_unknown_to_ml611(row) || is_x87_stack_operand(row) || is_indirect_branch(row) ||
                   has_capstone_size(row) || is_raw_mnemonic(row.mnemonic)) {
            lines_.push_back(masm::db_line(row.raw));   // the encodings ML will not reproduce (absolute memory, segment override, 16-bit address, aam/aad, int3, far ...)
        } else {
            std::optional<std::string> line = masm::masm_line(code_, object, row);
            if (line && masm::is_branch(row.mnemonic)) line = branch_line(*line, row, object, emitted);   // a target we emit no label for: raw
            lines_.push_back(line ? *line : masm::db_line(row.raw));
        }
        return a + row.size;
    }

    Emission& code_;
    bool loadable_;
    std::optional<le::Ddb> ddb_;
    std::string name_;
    std::map<Place, std::string> force_labels_;      // a fix-up target that lands MID-INSTRUCTION: its instruction is written as db
    std::map<Place, std::int64_t> interior_alias_;   // a label inside a fix-up's source dword -> the dword's anchor
    std::optional<LoadContext> load_ctx_;
    PlaceSet emittable_, cp_skip_, inplace_stops_;
    ControlDispatch dispatch_;
    std::optional<Place> control_at_;
    std::optional<ServiceTable> service_;
    ZeroDisplacement zero_displacement_;
    std::vector<std::string> lines_;
};

}  // namespace

std::string emit(Emission& code, bool loadable) { return Emitter(code, loadable).run(); }

// ---- the trees --------------------------------------------------------------------------------------------------------------

namespace {

constexpr std::string_view kHeader = R"(; {0}.ASM -- {0} reconstructed in idiomatic VMM DDK-macro source form.
;
; Recovered from the stripped LE binary by recursive-descent disassembly and re-emitted with the
; period Virtual Device DDK macros (Declare_Virtual_Device, Begin_Control_Dispatch / Control_
; Dispatch / End_Control_Dispatch, VMMcall / VxDcall <service>, BeginProc / EndProc, VxD_..._SEG) so
; it reads like Microsoft's own VxD source. It assembles through the 1996 DDK ML.EXE (COFF) with the
; stock INC32 headers. Encodings ML will not reproduce byte-for-byte (mov eAX,[disp], segment/16-bit-
; address prefixes, aam/aad, shift-by-1, self-relative call) are kept as `db` to stay byte-exact.
;
; The original source file names, comments and most internal labels are not present in a stripped LE
; image: internal routines are sub_<offset> except where a debug .SYM gives the real name. Exported
; service names and control-dispatch handler names ARE the real DDK names.
)";

}  // namespace

std::filesystem::path write_tree(const le::Module& driver, std::string_view name, const stdfs::path& out_folder, const DdkNames& names) {
    Emission code(Disassembly::run(driver, names), names);
    const std::string lower = text::lower(name);
    const stdfs::path folder = out_folder / lower;
    fs::make_dirs(folder);
    const stdfs::path path = folder / (lower + ".asm");
    fs::write_text(path, text::replace_all(kHeader, "{0}", name) + "\r\n" + emit(code));
    return path;
}

std::map<std::string, bool> verify(std::span<const NamedDriver> drivers, const DdkNames& names, const masm::Toolchain& tools, std::size_t budget) {
    struct Source {
        std::string name, text;
        std::size_t lines = 0;
    };
    std::vector<Source> sources;
    for (const NamedDriver& d : drivers) {
        Emission code(Disassembly::run(*d.driver, names), names);
        std::string text = emit(code);
        const std::size_t lines = static_cast<std::size_t>(std::count(text.begin(), text.end(), '\n'));
        sources.push_back({d.name, std::move(text), lines});
    }
    std::stable_sort(sources.begin(), sources.end(), [](const Source& a, const Source& b) { return a.lines < b.lines; });
    std::vector<std::vector<const Source*>> batches;
    std::vector<const Source*> current;
    std::size_t current_lines = 0;
    for (const Source& s : sources) {
        if (s.lines > budget) {
            batches.push_back({&s});
            continue;
        }
        if (current_lines + s.lines > budget && !current.empty()) {
            batches.push_back(std::move(current));
            current.clear();
            current_lines = 0;
        }
        current.push_back(&s);
        current_lines += s.lines;
    }
    if (!current.empty()) batches.push_back(std::move(current));
    std::map<std::string, bool> result;
    for (const auto& batch : batches) {
        std::vector<std::string> texts;
        for (const Source* s : batch) texts.push_back(s->text);
        const std::vector<masm::Assembled> made = masm::assemble_macro_batch(tools, texts);
        for (std::size_t i = 0; i < batch.size(); ++i) result[batch[i]->name] = i < made.size() && made[i].object.has_value();
    }
    return result;
}

// ---- the command ------------------------------------------------------------------------------------------------------------

namespace {

std::vector<stdfs::path> drivers_in(const stdfs::path& folder) {
    std::vector<stdfs::path> files;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(folder, ec)) {
        const std::string ext = text::lower(fs::to_text(entry.path().extension()));
        if (entry.is_regular_file(ec) && (ext == ".vxd" || ext == ".386")) files.push_back(entry.path());
    }
    std::sort(files.begin(), files.end());
    return files;
}

std::string module_stem(std::string_view member_name) { return std::string(member_name.substr(0, member_name.find('.'))); }

}  // namespace

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "idiom",
        "a driver as kit-macro source",
        {cli::Positional{"path", "a driver (.vxd, .386) or a folder of them", false},
         cli::Positional{"out", "where a folder's sources go", false},
         cli::Option{"--verify", "", "does every driver of the folder assemble?", cli::Arg::Flag}},
        [&](const cli::Args& a) -> int {
            const auto path_argument = a.value("path");
            if (a.flag("--verify")) {
                const stdfs::path folder = path_argument ? fs::from_text(*path_argument) : env.corpus.root();
                std::vector<std::pair<std::string, le::Module>> held;
                // (the whole corpus of drivers: *.vxd, *.386, *.pdr, *.mpd and the few under other names, as the corpus tool takes it)
                for (const stdfs::path& file : corpus::driver_files(folder)) {
                    try {
                        for (auto& member : le::open_modules(file)) held.emplace_back(text::lower(module_stem(member.first)), std::move(member.second));
                    } catch (const Error&) {
                        // a file that is not a driver is not verified
                    }
                }
                std::vector<NamedDriver> drivers;
                for (const auto& [name, module] : held) drivers.push_back({name, &module});
                const std::map<std::string, bool> result = verify(drivers, env.names, env.tools);
                std::vector<std::string> failed;
                std::size_t clean = 0;
                for (const auto& [name, ok] : result) {
                    clean += ok;
                    if (!ok) failed.push_back(name);
                }
                out("== idiomatic assembly: {}/{} clean ==\n", clean, result.size());
                if (!failed.empty()) out("FAILED: {}\n", text::join(failed, ", "));
                return failed.empty() ? kExitOk : kExitFailed;
            }
            if (!path_argument) throw UsageError("give a driver or a folder");
            const std::string ext = text::lower(fs::to_text(fs::from_text(*path_argument).extension()));
            if ((ext == ".vxd" || ext == ".386") && !a.value("out")) {   // a single file: a preview of the source
                const auto modules = le::open_modules(fs::from_text(*path_argument));
                if (modules.empty()) throw FormatError("LE", 0, "the file holds no driver");
                Emission code(Disassembly::run(modules.front().second, env.names), env.names);
                out_text(emit(code).substr(0, 1600));
                out_text("\n");
                return kExitOk;
            }
            const stdfs::path out_folder = a.value("out") ? fs::from_text(*a.value("out")) : env.output_folder;
            std::size_t written = 0;
            for (const stdfs::path& file : drivers_in(fs::from_text(*path_argument))) {
                std::vector<std::pair<std::string, le::Module>> modules;
                try {
                    modules = le::open_modules(file);
                } catch (const Error& e) {
                    out("  {:<16} OPEN-ERROR {}\n", fs::to_text(file.filename()), std::string(e.what()).substr(0, 50));
                    continue;
                }
                for (const auto& [member, module] : modules) {
                    try {
                        write_tree(module, text::upper(module_stem(member)), out_folder, env.names);
                        ++written;
                    } catch (const Error& e) {   // one driver of the folder: its failure is its own line, the rest go on
                        out("  {:<16} EMIT-ERROR {}\n", member, std::string(e.what()).substr(0, 50));
                    }
                }
            }
            out("wrote {} idiomatic VxD sources to {}\n", written, fs::to_text(out_folder));
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("idiom", {command});
    std::vector<std::string> line = {"idiom"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::idiom
