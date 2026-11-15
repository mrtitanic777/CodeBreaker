// cb/build/module_scan.cpp -- the passes over the original's instructions that decide which functions of a 16-bit module cannot
// be rebuilt from C: C has no way to say what the original does with a register, with DS or with a pointer to code.

#include <algorithm>
#include <map>
#include <set>

#include "cb/build/module_internal.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/describe.hpp"

namespace cb::build::module::internal {

namespace desc = decompile::describe;

namespace {

bool is_ax(std::uint16_t reg) {
    const std::string name = desc::register_name16(reg);
    return name == "ah" || name == "al" || name == "ax" || name == "eax";
}

// The registers a C function's text reads as they come in, by the whole register they are part of.
const std::map<std::string, std::string, std::less<>>& whole_register() {
    static const std::map<std::string, std::string, std::less<>> m = {
        {"al", "a"}, {"ah", "a"}, {"ax", "a"}, {"eax", "a"}, {"bl", "b"}, {"bh", "b"}, {"bx", "b"}, {"ebx", "b"}, {"cl", "c"}, {"ch", "c"},
        {"cx", "c"}, {"ecx", "c"}, {"dl", "d"}, {"dh", "d"}, {"dx", "d"}, {"edx", "d"}, {"si", "si"}, {"esi", "si"}, {"di", "di"}, {"edi", "di"}};
    return m;
}

// The text before the first comma of an instruction's operands, trimmed: the register it writes.
std::string first_operand(std::string_view op_str) {
    return std::string(text::trim(op_str.substr(0, op_str.find(','))));
}

}  // namespace

// Whether a function's instructions load DS and do not put it back: a `mov ds, x` from anything but a copy taken of DS (`mov dx, ds`),
// with no `pop ds` or restoring `mov ds` after it. `own_data_loads`: the addresses of the `mov ax, imm` instructions whose immediate
// the loader fixes to the module's own automatic data segment -- `mov ax, DGROUP / mov ds, ax` (USER's sub_1_5D4F, which every USER
// function that calls it has already done): the caller's DS is that segment already, so nothing is left changed.
bool leaves_ds(const std::vector<desc::Insn>& ins, const std::set<std::int64_t>& own_data_loads) {
    std::set<std::string> saved;
    bool changed = false;
    bool ax_is_own_data = false;
    for (const desc::Insn& x : ins) {
        const bool copy_of_ds = x.mn == "mov" && x.op_str.size() > 4 && x.op_str.ends_with(", ds") &&
                                x.op_str.find(',') == x.op_str.size() - 4 &&
                                text::is_identifier(std::string_view(x.op_str).substr(0, x.op_str.size() - 4));
        if (x.mn == "mov" && x.op_str.starts_with("ax,") && own_data_loads.contains(x.address)) {
            ax_is_own_data = true;
            continue;
        }
        if (copy_of_ds) {
            saved.insert(x.op_str.substr(0, x.op_str.find(',')));
        } else if (x.mn == "mov" && x.op_str.starts_with("ds,")) {
            const std::string_view source = text::trim(std::string_view(x.op_str).substr(x.op_str.find(',') + 1));
            changed = !saved.contains(std::string(source)) && !(ax_is_own_data && source == "ax");
        } else if (x.mn == "pop" && x.op_str == "ds") {
            changed = false;
        } else if (x.mn == "lds") {
            changed = true;
        }
        if (x.mn != "push" && first_operand(x.op_str) == "ax") ax_is_own_data = false;   // (any other write of AX)
        if (x.mn == "call" || x.mn == "lcall") ax_is_own_data = false;
    }
    return changed;
}

// Whether, before instruction `k`, the function has loaded DS from the module's own data segment (`mov ax, DGROUP / mov ds, ax`): its DS
// is then that segment, and a callee that loads the same one changes nothing for the rest of it.
bool loads_own_ds_before(const std::vector<desc::Insn>& ins, std::size_t k, const std::set<std::int64_t>& own_data_loads) {
    bool ax_is_own_data = false;
    for (std::size_t j = 0; j < k && j < ins.size(); ++j) {
        const desc::Insn& x = ins[j];
        if (x.mn == "mov" && x.op_str.starts_with("ax,") && own_data_loads.contains(x.address)) {
            ax_is_own_data = true;
            continue;
        }
        if (x.mn == "mov" && x.op_str == "ds, ax" && ax_is_own_data) return true;
        if (x.mn != "push" && first_operand(x.op_str) == "ax") ax_is_own_data = false;
        if (x.mn == "call" || x.mn == "lcall") ax_is_own_data = false;
    }
    return false;
}


std::optional<std::uint32_t> result_in_ax(ByteView code, std::uint32_t start, std::uint32_t end) {
    const std::size_t to = std::min<std::size_t>(end, code.size());
    const std::vector<desc::Insn> ins = desc::decode16(code.subspan(std::min<std::size_t>(start, to), to - std::min<std::size_t>(start, to)), start, true);
    std::int64_t prologue = start;
    for (std::size_t i = 0; i < std::min<std::size_t>(6, ins.size()); ++i) {
        if (ins[i].mn == "mov" && ins[i].op_str == "ds, ax") {
            prologue = ins[i].address + ins[i].size;
            break;
        }
    }
    std::set<std::int64_t> targets;
    for (const desc::Insn& i : ins) {
        if (i.in_group(desc::kGroupJump) && i.imm_first()) targets.insert(i.first_imm());
    }
    for (std::size_t k = 0; k < ins.size(); ++k) {
        if (ins[k].mn != "ret" && ins[k].mn != "retf") continue;
        for (std::size_t j = k; j-- > 0;) {
            const desc::Insn& p = ins[j];
            if (p.in_group(desc::kGroupCall) && p.address >= prologue) return static_cast<std::uint32_t>(p.address);   // the callee's result, passed on as it came
            if (p.address < prologue || p.mn == "jmp" || p.mn == "ret" || p.mn == "retf") break;
            if (std::any_of(p.regs_write.begin(), p.regs_write.end(), is_ax)) {
                if (p.mn != "pop") return static_cast<std::uint32_t>(p.address);
                break;
            }
            if (targets.contains(p.address)) break;
        }
    }
    return std::nullopt;
}

bool segment_number_fixups(const formats::ne::Module& ne) {
    bool seen = false;
    for (const formats::ne::Segment& s : ne.segments()) {
        if (!(s.flags & formats::ne::kRelocInfo) || s.file_offset == 0) continue;
        for (const formats::ne::Relocation& r : ne.relocations(s)) {
            if ((r.flags & 3) != 0) continue;   // not a place of the module's own
            seen = true;
            const auto* own = std::get_if<formats::ne::InternalRef>(&r.target);
            if ((own != nullptr && own->segment == 0xFF) || r.source != 2) return false;
        }
    }
    return seen;
}

std::optional<std::uint32_t> resident_limit_of(ByteView code) {
    // mov cx, N / push cs / push dx / push cx / push dx
    static constexpr std::uint8_t kTail[5] = {0x0E, 0x52, 0x51, 0x52, 0x0E};
    for (std::size_t at = 0; at + 8 < code.size(); ++at) {
        if (code[at] == 0xB9 && std::equal(std::begin(kTail), std::end(kTail), code.begin() + static_cast<std::ptrdiff_t>(at + 3)))
            return static_cast<std::uint32_t>(code[at + 1]) | static_cast<std::uint32_t>(code[at + 2]) << 8;
    }
    return std::nullopt;
}

// Functions whose C cannot say what the original passes or returns in a register.
//  - It sets a register just before calling one that reads that register as it comes in (USER's CallMsgFilter, sub_1_80CD: `mov bx, 6` /
//    `mov bx, -1`, then the hook caller sub_1_800A, which takes the hook type in BX; the C passed no BX, the caller looked up a hook record
//    at 1 and jumped to a selector D400 -- a GP on Setup's License page).
//  - It returns a word, but the original sets DX just before it returns: a DX:AX result (sub_1_81C8 returns es:[38h]:1Eh, the C 1Eh alone).
std::map<std::string, std::string> registers_c_cannot_carry(const formats::ne::Module& ne, const Segments& segs, const Places& at,
                                                            const std::map<std::string, std::string>& texts) {
    static const Regex register_in(R"(\b(?:in|unaff)_(AL|AH|AX|BL|BH|BX|CL|CH|CX|DL|DH|DX|SI|DI)\b)");
    static const Regex wide_result(R"(\b(?:int4|uint4|xunknown4|undefined4)\b|__far\s*\*)");
    static const Regex reads_es(R"(\bes\b)");
    static const Regex es_first(R"(\s*es\s*,)");
    std::map<std::uint32_t, std::vector<std::uint32_t>> starts;                    // segment -> offsets
    struct Taker {
        std::uint32_t segment;
        std::string name;
        std::set<std::string> registers;
    };
    std::map<std::uint32_t, std::vector<Taker>> takes;                              // offset -> who takes registers in there
    std::map<std::uint32_t, std::vector<std::pair<std::uint32_t, std::string>>> by_offset;   // offset -> [(segment, name)]
    for (const auto& [place, name] : at) {
        starts[place.seg].push_back(place.off);
        by_offset[place.off].emplace_back(place.seg, name);
        std::set<std::string> regs;
        const auto text = texts.find(name);
        const std::string_view body = text == texts.end() ? std::string_view() : after_brace(text->second);
        register_in.for_each_match(body, [&](const RegexMatch& m) {
            regs.insert(whole_register().find(text::lower(m.group(1)))->second);
            return true;
        });
        if (!regs.empty()) takes[place.off].push_back({place.seg, name, regs});
    }
    for (auto& [seg, offsets] : starts) std::sort(offsets.begin(), offsets.end());
    std::map<std::uint32_t, Bytes> images;
    std::map<std::string, std::string> out;
    std::set<std::string> leaving_ds, leaving_own;
    std::set<std::pair<std::string, std::string>> own_ds_ready;   // {callee, caller}: the caller had its own data in DS before the call
    // (the places where the loader makes a `mov ax, imm` the module's own automatic data segment, by segment)
    std::map<std::uint32_t, std::set<std::uint32_t>> own_data_offsets;
    const auto own_data_loads_of = [&](std::uint32_t sg, const Bytes& code, const std::vector<desc::Insn>& ins) {
        auto found = own_data_offsets.find(sg);
        if (found == own_data_offsets.end()) {
            std::set<std::uint32_t> offsets;
            if (const auto it = segs.find(sg); it != segs.end() && ne.header().autodata_segment != 0) {
                for (const formats::ne::Relocation& r : ne.relocations(it->second)) {
                    const auto* ref = std::get_if<formats::ne::InternalRef>(&r.target);
                    if (r.source != 2 || ref == nullptr || ref->segment != ne.header().autodata_segment) continue;
                    // (a fix-up that is not additive names the first place only: the stored word at it is the offset of the next, 0xFFFF the last)
                    std::uint32_t at_site = r.offset;
                    for (int hops = 0; hops < 65536 && at_site + 1 < code.size(); ++hops) {
                        offsets.insert(at_site);
                        if (r.additive()) break;
                        const std::uint32_t next = static_cast<std::uint32_t>(code[at_site]) | static_cast<std::uint32_t>(code[at_site + 1]) << 8;
                        if (next == 0xFFFF || offsets.contains(next)) break;
                        at_site = next;
                    }
                }
            }
            found = own_data_offsets.emplace(sg, std::move(offsets)).first;
        }
        std::set<std::int64_t> loads;
        for (const desc::Insn& x : ins) {
            if (x.mn == "mov" && x.op_str.starts_with("ax,") && found->second.contains(static_cast<std::uint32_t>(x.address + 1))) loads.insert(x.address);
        }
        return loads;
    };
    std::map<std::string, std::set<std::string>> calls_of;   // (and who calls each: a caller of one that leaves DS changed counts on that too)
    const auto setdefault = [&](const std::string& k, std::string v) { out.emplace(k, std::move(v)); };
    for (const auto& [place, n] : at) {
        const std::uint32_t sg = place.seg, o = place.off;
        const auto text = texts.find(n);
        const auto seg = segs.find(sg);
        if (text == texts.end() || seg == segs.end()) continue;
        if (!images.contains(sg)) {
            const ByteView data = ne.data();
            const formats::ne::Segment& s = seg->second;
            const std::size_t from = std::min<std::size_t>(s.file_offset, data.size());
            images[sg] = Bytes(data.begin() + static_cast<std::ptrdiff_t>(from),
                               data.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(from + s.length, data.size())));
        }
        const Bytes& code = images.at(sg);
        std::uint32_t stop = static_cast<std::uint32_t>(code.size());
        for (std::uint32_t x : starts.at(sg)) {
            if (x > o) {
                stop = x;
                break;
            }
        }
        const std::uint32_t from = std::min<std::uint32_t>(o, static_cast<std::uint32_t>(code.size()));
        const std::vector<desc::Insn> ins = desc::decode16(ByteView(code).subspan(from, std::min<std::size_t>(stop, code.size()) - std::min<std::size_t>(from, stop)), o, false);
        const bool wide = wide_result.test(std::string_view(text->second).substr(0, text->second.find('(')));
        // (it loads DS and never puts it back: it leaves the caller another DS -- sub_1_222B's `mov ds, ax; ret`, KERNEL's sub_2_0170
        // `mov ds, cs:[16Eh]; ret`. Not USER's copy helper sub_1_39E2: `mov dx, ds` ... `mov ds, dx` puts it back)
        const std::set<std::int64_t> own_loads = own_data_loads_of(sg, code, ins);
        if (leaves_ds(ins)) {
            leaving_ds.insert(n);
            setdefault(n, "it leaves DS changed for its caller");
            // (A callee that leaves only the module's own data segment, for a caller that has loaded it itself before the call, changes
            // nothing -- `leaves_ds(ins, own_loads)` and `loads_own_ds_before` say so, and are tested -- but USER's callers of sub_1_5D4F
            // rebuilt as C on that reading fail the all-15 boot (FAIL(dos)). So no caller is excused unless CB_DS_OWN_CALLERS names it
            // (a comma list), for finding the one that breaks.)
            if (!leaves_ds(ins, own_loads)) leaving_own.insert(n);
        }
        for (std::size_t k = 0; k < ins.size(); ++k) {
            const desc::Insn& i = ins[k];
            const std::size_t first_prev = k >= 4 ? k - 4 : 0;   // ins[max(0, k - 4):k]
            if (i.mn == "call" || i.mn == "lcall") {
                const std::string_view last = text::trim(std::string_view(i.op_str).substr(i.op_str.rfind(',') == std::string::npos ? 0 : i.op_str.rfind(',') + 1));
                const std::optional<std::int64_t> parsed = text::parse_int<std::int64_t>(last, 0);
                if (!parsed) continue;
                const std::uint32_t target = static_cast<std::uint32_t>(*parsed);
                const auto bo = by_offset.find(target);
                if (bo != by_offset.end()) {
                    for (const auto& [callee_seg, callee] : bo->second) {
                        if (!(i.mn == "call" && callee_seg != sg) && callee != n) {
                            calls_of[callee].insert(n);
                            if (loads_own_ds_before(ins, k, own_loads)) own_ds_ready.emplace(callee, n);
                        }
                    }
                }
                if (const auto tk = takes.find(target); tk != takes.end()) {
                    for (const Taker& t : tk->second) {
                        if ((i.mn == "call" && t.segment != sg) || t.name == n) continue;
                        // (by the whole register: SUWIN's 7:0056 `mov ax, es:[si]` / `call 0`, and 7:0000 compares AL with the character
                        // looked for -- the C passed no AL, found no dot, and Setup set MSDOS.SYS aside as WINBOOT.---: Windows never started)
                        const desc::Insn* last_set = nullptr;
                        for (std::size_t pi = k - first_prev > 3 ? k - 3 : first_prev; pi < k; ++pi) {
                            const desc::Insn& p = ins[pi];
                            const auto w = whole_register().find(first_operand(p.op_str));
                            if (w != whole_register().end() && t.registers.contains(w->second) &&
                                (p.mn == "mov" || p.mn == "xor" || p.mn == "sub" || p.mn == "lea" || p.mn == "les" || p.mn == "lds" ||
                                 p.mn == "add" || p.mn == "or"))
                                last_set = &p;
                        }
                        if (last_set != nullptr) {
                            setdefault(n, std::format("it passes {} to {} in a register ({:04x})", text::upper(first_operand(last_set->op_str)),
                                                      t.name, i.address));
                        }
                    }
                }
                const desc::Insn* next = k + 1 < ins.size() ? &ins[k + 1] : nullptr;
                // ... and the caller branches on the flags the callee leaves: a result no C returns, and a test no C can write (USER's
                // sub_1_222B: `or ax, ax; ret`, its callers `call 222B; je` -- the C returned nothing, its callers tested AX left from elsewhere)
                if (next != nullptr && !next->mn.empty() && next->mn[0] == 'j' && next->mn != "jmp" && next->mn != "jcxz" && next->mn != "jecxz" &&
                    bo != by_offset.end()) {
                    for (const auto& [callee_seg, callee] : bo->second) {
                        if (!(i.mn == "call" && callee_seg != sg) && callee != n) {
                            setdefault(callee, std::format("it returns in the flags ({} branches on them, {:04x})", n, next->address));
                            setdefault(n, std::format("it branches on the flags {} leaves ({:04x})", callee, next->address));
                        }
                    }
                }
                // ... and the caller reads ES the callee left: an ES result no C returns (USER's sub_1_281C leaves ES = the task's queue, and
                // its caller clears a flag at es:[30h]; the C left ES at another segment, the flag stayed set and Setup sat idle on its Options page)
                if (next != nullptr && reads_es.test(next->op_str) && !es_first.match(next->op_str) && next->mn != "les" && next->mn != "pop" &&
                    bo != by_offset.end()) {
                    for (const auto& [callee_seg, callee] : bo->second) {
                        if (!(i.mn == "call" && callee_seg != sg) && callee != n)
                            setdefault(callee, std::format("its callers read the ES it returns ({}, {:04x})", n, next->address));
                    }
                }
            } else if ((i.mn == "ret" || i.mn == "retf") && !wide) {
                bool dx = false;
                for (std::size_t pi = first_prev; pi < k; ++pi) {
                    const desc::Insn& p = ins[pi];
                    if (((p.mn == "mov" || p.mn == "xor" || p.mn == "sub" || p.mn == "les" || p.mn == "lds") && p.op_str.starts_with("dx,")) || p.mn == "cwd")
                        dx = true;
                }
                if (dx) setdefault(n, std::format("it returns DX:AX where its C returns a word ({:04x})", i.address));
            }
        }
    }
    // (a caller of one that leaves DS changed: the original puts its own DS back after it -- KERNEL's sub_2_2437 `push ds; call 0170 ...
    // pop ds`; its C kept the kernel's DS and the caller after it faulted, "Invalid DPMI return from 0367:2BC3")
    std::set<std::string> excused_callers;
    const std::string excused_text = fs::get_env("CB_DS_OWN_CALLERS").value_or("");
    for (std::string_view x : text::split(excused_text, ',')) {
        if (!x.empty()) excused_callers.emplace(x);
    }
    for (const std::string& callee : leaving_ds) {
        const auto it = calls_of.find(callee);
        if (it == calls_of.end()) continue;
        for (const std::string& caller : it->second) {
            if (leaving_own.contains(callee) && own_ds_ready.contains({callee, caller}) && excused_callers.contains(caller)) continue;
            setdefault(caller, "it calls " + callee + ", which leaves DS changed");
        }
    }
    return out;
}

std::set<std::string> near_pointer_tables(const formats::ne::Module& ne, const Places& at) {
    const std::uint32_t auto_seg = ne.header().autodata_segment;
    std::map<std::uint32_t, formats::ne::Segment> segs;
    for (const formats::ne::Segment& s : ne.segments()) segs[s.index] = s;
    const auto auto_it = segs.find(auto_seg);
    if (auto_seg == 0 || auto_it == segs.end() || auto_it->second.file_offset == 0) return {};
    const ByteView file = ne.data();
    const auto slice = [&](const formats::ne::Segment& s) {
        const std::size_t from = std::min<std::size_t>(s.file_offset, file.size());
        return file.subspan(from, std::min<std::size_t>(s.length, file.size() - from));
    };
    const ByteView data = slice(auto_it->second);
    std::map<std::uint32_t, std::map<std::uint32_t, std::string>> starts;   // segment -> {offset: name}
    for (const auto& [place, name] : at) starts[place.seg][place.off] = name;
    static const std::set<std::string, std::less<>> address_registers = {"bx", "si", "di", "ax", "cx", "dx"};
    std::set<std::string> out;
    for (const auto& [sg, names] : starts) {
        const auto seg = segs.find(sg);
        if (seg == segs.end() || seg->second.file_offset == 0 || names.size() < 2) continue;
        bool indirect = false;
        std::set<std::int64_t> immediates;
        for (const desc::Insn& i : desc::decode16(slice(seg->second), 0, false)) {
            if ((i.mn == "call" || i.mn == "jmp") && i.op_str.starts_with("word ptr [")) indirect = true;
            if (i.mn == "mov") {
                // exactly one ", 0x" and a register first: `mov di, 0x776`
                const std::size_t first = i.op_str.find(", 0x");
                if (first != std::string::npos && i.op_str.find(", 0x", first + 1) == std::string::npos &&
                    address_registers.contains(i.op_str.substr(0, 2))) {
                    const std::size_t comma = i.op_str.find(", ");
                    if (const auto value = text::parse_int<std::int64_t>(i.op_str.substr(comma + 2, i.op_str.find(", ", comma + 2) - (comma + 2)), 16))
                        immediates.insert(*value);
                }
            }
        }
        if (!indirect) continue;
        for (std::size_t align : {0u, 1u}) {
            std::vector<std::pair<std::size_t, std::string>> run;
            const auto flush = [&] {
                if (run.size() >= 3 || (run.size() >= 2 && immediates.contains(static_cast<std::int64_t>(run[0].first)))) {
                    for (const auto& r : run) out.insert(r.second);
                }
                run.clear();
            };
            for (std::size_t p = align; p + 1 < data.size(); p += 2) {
                const std::uint32_t w = static_cast<std::uint32_t>(data[p]) | static_cast<std::uint32_t>(data[p + 1]) << 8;
                const auto name = names.find(w);
                if (w != 0 && name != names.end()) {
                    run.emplace_back(p, name->second);
                    continue;
                }
                flush();
            }
            flush();
        }
    }
    return out;
}

std::set<std::string> register_argument_calls(const formats::ne::Module& ne, const Places& at) {
    std::map<std::uint32_t, formats::ne::Segment> segs;
    for (const formats::ne::Segment& s : ne.segments()) segs[s.index] = s;
    std::map<std::uint32_t, std::vector<std::pair<std::uint32_t, std::string>>> by_seg;
    for (const auto& [place, name] : at) by_seg[place.seg].emplace_back(place.off, name);   // (offsets in order, as `at` is)
    std::set<std::string> out;
    const ByteView file = ne.data();
    for (const auto& [sg, functions] : by_seg) {
        const auto seg = segs.find(sg);
        if (seg == segs.end() || seg->second.file_offset == 0) continue;
        const std::size_t from = std::min<std::size_t>(seg->second.file_offset, file.size());
        const ByteView code = file.subspan(from, std::min<std::size_t>(seg->second.length, file.size() - from));
        for (std::size_t k = 0; k < functions.size(); ++k) {
            const std::uint32_t off = functions[k].first;
            const std::size_t end = k + 1 < functions.size() ? functions[k + 1].first : code.size();
            if (off > code.size() || end > code.size() || off > end) continue;
            const desc::Insn* prev = nullptr;
            for (const desc::Insn& i : desc::decode16(code.subspan(off, end - off), off, false)) {
                if (i.mn == "lcall" && i.op_str.starts_with("[bp - ") && prev != nullptr && prev->mn == "mov" &&
                    prev->op_str.starts_with("ax, word ptr [bp - ")) {
                    out.insert(functions[k].second);
                    break;
                }
                prev = &i;
            }
        }
    }
    return out;
}

}  // namespace cb::build::module::internal
