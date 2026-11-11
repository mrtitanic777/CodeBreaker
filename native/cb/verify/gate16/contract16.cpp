#include "cb/verify/gate16/contract16.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <tuple>

#include "cb/core/capstone_api.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/intmath.hpp"
#include "cb/core/text.hpp"

namespace cb::verify::gate16::contract16 {
namespace {

// Functions whose argument count is their own (wsprintf: its own count): a call is compared on what the callee removes.
bool is_variadic(std::string_view callee) {
    return callee == "USER_420" || callee == "wsprintf" || callee == "USER_421" || callee == "wvsprintf";
}

// Functions that take their arguments in registers -- DOS3Call, NetBIOSCall: AH, DS:DX, ES:DI, CX loaded just before the
// call, and the carry flag back. C sets none of them: a C caller of one is not the original's. (VER's sub_3_0BD6 renames
// a file through DOS3Call; its C called it with nothing loaded, and the isolated check, answering DOS3Call alike on both
// sides, saw no difference.)
bool is_in_registers(std::string_view callee) {
    return callee == "KERNEL_102" || callee == "DOS3CALL" || callee == "KERNEL_103" || callee == "NETBIOSCALL";
}

// ---- reading what the decoder printed ----------------------------------------------------------------------------

// "0x1f" or "31"; the decoder prints lowercase hex and a bare 0.
std::optional<std::int64_t> parse_number(std::string_view s) {
    if (s.starts_with("0x"))
        return text::parse_int<std::int64_t>(s.substr(2), 16);
    return text::parse_int<std::int64_t>(s, 10);
}
bool is_number(std::string_view s) {
    return parse_number(s).has_value() && s.find_first_of("+- ") == std::string_view::npos;
}
// "-0x10" or "-4" as well.
std::optional<std::int64_t> parse_signed(std::string_view s) {
    if (s.starts_with('-')) {
        const auto v = parse_number(s.substr(1));
        if (v && s.find_first_of("+- ", 1) == std::string_view::npos)
            return -*v;
        return std::nullopt;
    }
    return is_number(s) ? parse_number(s) : std::nullopt;
}

// One decoded instruction, with what this module reads of Capstone's record.
struct Insn {
    std::uint64_t address = 0;
    std::uint64_t size = 0;
    std::string mnemonic;      // with "notrack " removed (OpenWatcom's `3E E8`, a call with a DS prefix)
    std::string op_str;
    bool is_call = false, is_jump = false, is_ret = false;
    bool first_is_imm = false;
    std::int64_t first_imm = 0;
    unsigned first_size = 0;
    // Whether the instruction writes SP; none when the decoder cannot say (or it was not asked: see decode()).
    std::optional<bool> writes_sp;
};

std::string without_notrack(std::string m) {
    constexpr std::string_view kPrefix = "notrack ";
    for (std::size_t at; (at = m.find(kPrefix)) != std::string::npos;)
        m.erase(at, kPrefix.size());
    return m;
}

// The instructions the LEAVE scan and the stack walk decide on their mnemonic alone, before asking what they write.
bool is_stack_op(const std::string& m) {
    return m == "push" || m == "pushf" || m == "pop" || m == "popf" || m == "enter" || m == "ret" || m == "retf";
}

std::vector<Insn> decode(const platforms::Decoder& decoder, ByteView code, std::uint64_t base) {
    std::vector<Insn> out;
    decoder.each(code, base, 0, [&](const cs_insn& c) {
        Insn i;
        i.address = c.address;
        i.size = c.size;
        i.mnemonic = without_notrack(c.mnemonic);
        i.op_str = c.op_str;
        const cs_detail* d = c.detail;
        for (int g = 0; d && g < d->groups_count; ++g) {
            i.is_call |= d->groups[g] == CS_GRP_CALL;
            i.is_jump |= d->groups[g] == CS_GRP_JUMP;
            i.is_ret |= d->groups[g] == CS_GRP_RET;
        }
        if (d && d->x86.op_count > 0) {
            const cs_x86_op& op = d->x86.operands[0];
            i.first_is_imm = op.type == X86_OP_IMM;
            i.first_imm = op.imm;
            i.first_size = op.size;
        }
        // (the registers an instruction writes are asked for where the stack walk and the LEAVE scan read them)
        if (!i.is_call && !i.is_jump && !i.is_ret && !is_stack_op(i.mnemonic)) {
            if (const auto access = decoder.register_access(c))
                i.writes_sp = std::find(access->written.begin(), access->written.end(),
                                        static_cast<std::uint16_t>(X86_REG_SP)) != access->written.end();
        }
        out.push_back(std::move(i));
        return true;
    });
    return out;
}

// ("near", seg, off) / ("far", address) of a direct call, or none (through a pointer).
std::optional<CalleeKey> callee_key(const Insn& i, unsigned segment) {
    if (i.mnemonic == "call" && is_number(i.op_str))
        return NearCall{segment, static_cast<std::uint16_t>(*parse_number(i.op_str) & 0xFFFF)};
    if (i.mnemonic == "lcall") {
        const std::vector<std::string_view> parts = text::split(i.op_str, ", ");
        if (parts.size() == 2 && is_number(parts[0]) && is_number(parts[1]))
            return FarCall{static_cast<std::uint32_t>((*parse_number(parts[0]) << 16) | *parse_number(parts[1]))};
    }
    return std::nullopt;
}

bool starts_with_sp_comma(const Insn& i) { return i.op_str.starts_with("sp, "); }

// `sp, <number>`: the number.
std::optional<std::int64_t> sp_amount(const Insn& i) {
    if (!starts_with_sp_comma(i))
        return std::nullopt;
    const std::string_view rest = std::string_view(i.op_str).substr(4);
    return is_number(rest) ? parse_number(rest) : std::nullopt;
}

// A call's pushed bytes tracked down the code: {index of a call -> (bytes pushed and not yet taken off, whether that is
// all of them)} -- the pushes since the prologue, less what was popped, taken off (`add sp, n`) or removed by a callee
// that removes its own (`removes`). A call's pushes are not all its own: the bytes before them can be an enclosing
// call's -- `GlobalAlloc(0x2042, GlobalSize(h))` pushes the flags, then GlobalSize's handle -- and counted back only to
// the call before, USER's GETCLIPBOARDDATA "pushed four" for GlobalAlloc's six; counted back to a join, a call taking no
// arguments was given the next call's (USER's sub_9_0907 pushes sub_9_0930's far pointer before calling sub_1_8270,
// whose result is sub_9_0930's first argument). A join of paths (a jump's target) has what every jump to it had, if they
// agree; else (or one not seen: a jump back, a table's) the count starts over there, as no longer all of them.
struct Pushed {
    std::int64_t bytes = 0;
    bool exact = false;
};

// Remove `n` bytes from the pending pushes, last first. A pending entry of 0 (the half of `push cs / call`) counts as 2.
void take_off(std::vector<std::int64_t>& pend, std::int64_t n) {
    while (n > 0 && !pend.empty()) {
        const std::int64_t last = pend.back();
        pend.pop_back();
        n -= last ? last : 2;
    }
}

std::int64_t total_of(const std::vector<std::int64_t>& pend) {
    std::int64_t sum = 0;
    for (std::int64_t p : pend)
        sum += p;
    return sum;
}

std::map<std::size_t, Pushed> stack_at_calls(const std::vector<Insn>& ins, unsigned segment, std::uint64_t prologue,
                                              const Removes& removes) {
    std::map<std::size_t, Pushed> out;
    struct Path {
        std::vector<std::int64_t> pend;
        bool exact;
    };
    std::map<std::uint64_t, std::vector<Path>> saved;   // jump target -> the state at each jump to it (lookup by address)
    std::vector<std::int64_t> pend;
    bool exact = false, live = false;
    for (std::size_t k = 0; k < ins.size(); ++k) {
        const Insn& i = ins[k];
        const std::uint64_t a = i.address;
        if (a < prologue)
            continue;
        if (a == prologue) {
            pend.clear();
            exact = true;
            live = true;
        } else if (saved.contains(a) || !live) {
            std::vector<Path> seen;
            if (const auto it = saved.find(a); it != saved.end()) {
                seen = std::move(it->second);
                saved.erase(it);
            }
            if (live)
                seen.push_back({pend, exact});
            const bool agree = !seen.empty() &&
                               std::all_of(seen.begin(), seen.end(), [](const Path& p) { return p.exact; }) &&
                               std::all_of(seen.begin(), seen.end(), [&](const Path& p) {
                                   return total_of(p.pend) == total_of(seen.front().pend);
                               });
            if (agree) {
                pend = seen.front().pend;
                exact = true;
            } else {
                pend.clear();
                exact = false;
            }
            live = true;
        }
        const std::string& m = i.mnemonic;
        if (m == "push") {
            pend.push_back(i.first_size == 4 ? 4 : 2);
            if (i.op_str == "cs" && k + 1 < ins.size() && ins[k + 1].mnemonic == "call")
                pend.back() = 0;   // (`push cs / call near`: the return address's half, no argument)
            continue;
        }
        if (m == "pushf" || m == "pushfw") {
            pend.push_back(2);
            continue;
        }
        if (m == "pusha" || m == "pushaw") {
            pend.push_back(16);
            continue;
        }
        if (m == "pop" || m == "popf" || m == "popfw" || m == "popa" || m == "popaw") {
            take_off(pend, m.starts_with("popa") ? 16 : 2);
            continue;
        }
        if (m == "add" || m == "sub") {
            if (const auto n = sp_amount(i)) {
                if (m == "sub")
                    pend.push_back(*n);   // (room for a structure passed by value)
                else
                    take_off(pend, *n);
                continue;
            }
        }
        if (i.is_call) {
            if (!pend.empty() && pend.back() == 0)
                pend.pop_back();
            out[k] = {total_of(pend), exact};
            const auto key = callee_key(i, segment);
            if (!key) {
                const Insn* nxt = k + 1 < ins.size() ? &ins[k + 1] : nullptr;
                if (!(nxt && nxt->mnemonic == "add" && nxt->op_str.starts_with("sp"))) {
                    pend.clear();   // (through a pointer: it may remove what was pushed, or not)
                    exact = false;
                }
                continue;
            }
            take_off(pend, removes ? removes(*key).value_or(0) : 0);
            continue;
        }
        if (i.is_jump) {
            if (i.first_is_imm) {
                const std::uint64_t t = static_cast<std::uint64_t>(i.first_imm) & 0xFFFF;
                if (t > a)
                    saved[t].push_back({pend, exact});
            }
            if (m == "jmp" || m == "ljmp")
                live = false;
            continue;
        }
        if (i.is_ret || m == "ret" || m == "retf" || m == "iret" || m == "int3") {
            live = false;
            continue;
        }
        if (!i.writes_sp || *i.writes_sp) {
            pend.clear();   // (`mov sp, bp`, LEAVE: the frame's end; or an instruction the decoder cannot read)
            exact = false;
        }
    }
    return out;
}

// What the prologue is: the instructions at the start of a function that set up its frame, not its arguments.
std::uint64_t prologue_end(const std::vector<Insn>& ins, std::uint64_t lo) {
    std::uint64_t prologue = lo;
    bool loaded = false;   // (DS set from AX: the Windows prolog done)
    const std::size_t limit = std::min<std::size_t>(ins.size(), 16);
    for (std::size_t j = 0; j < limit; ++j) {
        const Insn& i = ins[j];
        const Insn* nxt = j + 1 < ins.size() ? &ins[j + 1] : nullptr;
        // (`mov ax, ds` and `mov ax, <data segment>` are the prolog's only before DS is loaded from AX: after it, GDI's
        // sub_1_59D2 rebuilt -- `mov ds, ax / mov ax, ds / push ax / push 594h`, a far pointer to its data -- "pushed two
        // bytes" for the four it pushes)
        const bool in_prologue =
            i.mnemonic == "nop" || i.mnemonic == "enter" || i.op_str == "bp, sp" || i.op_str == "ds, ax" ||
            (!loaded && i.op_str == "ax, ds") ||
            // (and OpenWatcom's room for locals, `push ax` a word at a time -- counted as an argument, sub_3_7E58's
            // allocator call "pushed six bytes" for its two)
            (i.mnemonic == "push" && (i.op_str == "ax" || i.op_str == "cx" || i.op_str == "dx") && nxt && !nxt->is_call) ||
            ((i.mnemonic == "push" || i.mnemonic == "inc") &&
             (i.op_str == "bp" || i.op_str == "ds" || i.op_str == "si" || i.op_str == "di")) ||
            (i.mnemonic == "pop" && i.op_str == "ax") ||
            (!loaded && i.mnemonic == "mov" && i.op_str.starts_with("ax, 0x")) ||
            (i.mnemonic == "sub" && i.op_str.starts_with("sp,"));
        if (!in_prologue)
            break;
        prologue = i.address + i.size;
        loaded = loaded || (i.mnemonic == "mov" && i.op_str == "ds, ax");
    }
    return prologue;
}

}  // namespace

std::string TakenOff::text() const {
    switch (kind) {
    case Kind::Bytes:
        return std::to_string(bytes);
    case Kind::Operand:
        return "?" + operand;
    case Kind::Leave:
        return "leave";
    }
    return {};
}

std::vector<Call> calls(const platforms::Decoder& decoder, ByteView image, unsigned segment, std::size_t lo,
                        std::size_t hi, const NameOf& name_of, const Removes& removes) {
    // (a function's end can lie past the segment's image: what is there is all there is)
    hi = std::min(hi, image.size());
    lo = std::min(lo, hi);
    const std::vector<Insn> ins = decode(decoder, image.subspan(lo, hi - lo), lo);
    std::vector<Call> out;

    // (and a push a jump lands on starts a new count -- as describe counts the bytes pushed for a call through a pointer)
    const std::uint64_t prologue = prologue_end(ins, lo);
    const std::map<std::size_t, Pushed> stack = stack_at_calls(ins, segment, prologue, removes);

    for (std::size_t k = 0; k < ins.size(); ++k) {
        const Insn& i = ins[k];
        // (a target of 0 is printed `0`, not `0x0`: every call to a segment's first function -- SUWIN's helpers at 7:0000
        // and 9:0000 -- went unchecked, and a C caller passing one word of a far pointer's two was never seen)
        const std::optional<CalleeKey> key = callee_key(i, segment);
        if (!key)
            continue;   // (through a pointer: the callee is not known here)
        const std::optional<std::string> callee = name_of(*key);
        std::size_t after = k + 1;
        while (after < ins.size() && ins[after].mnemonic == "nop")
            ++after;   // (OpenWatcom's `nop` after a near call)
        const Insn* nxt = after < ins.size() ? &ins[after] : nullptr;
        // (pushed and not yet taken off; not exact: counted only from a join on)
        const Pushed pushed_here = stack.contains(k) ? stack.at(k) : Pushed{};
        const std::int64_t total = pushed_here.bytes;
        TakenOff off;
        std::size_t rest = after;   // (the first instruction past the cleanup)
        if (nxt && nxt->mnemonic == "add" && nxt->op_str.starts_with("sp, ")) {
            const std::vector<std::string_view> parts = text::split(nxt->op_str, ", ");
            const std::string_view amount = parts.size() > 1 ? parts[1] : std::string_view{};
            if (const auto n = parse_signed(amount)) {
                off.bytes = *n;
            } else {
                off.kind = TakenOff::Kind::Operand;
                off.operand = std::string(amount);
            }
            rest = after + 1;
        } else {
            // (or taken off by popping them into a register no one reads: `pop bx`, `pop cx` -- the original's way after a
            // one-word cdecl call; read as nothing, the build's `add sp, 2` after the same call "took off two bytes the
            // original did not")
            const std::size_t room = static_cast<std::size_t>(std::max<std::int64_t>(0, total / 2));
            for (std::size_t q = after; q < std::min(ins.size(), after + room); ++q) {
                if (ins[q].mnemonic == "pop" && (ins[q].op_str == "bx" || ins[q].op_str == "cx") && off.bytes < total) {
                    off.bytes += 2;
                    ++rest;
                } else {
                    break;
                }
            }
        }
        bool at_leave = false;
        if (total) {
            // (and then the frame's LEAVE, nothing touching the stack before it: SP is set back from BP whatever was taken
            // off -- Microsoft C 8 itself leaves a call's arguments there when the function ends next (SETUPX's 2:5A3C,
            // `lcall _SUFORMATMESSAGEBOX / xor ax, ax / leave`), OpenWatcom takes them off: the same. Told here; compared as
            // the same.)
            for (std::size_t q = rest; q < std::min(ins.size(), rest + 12); ++q) {
                const Insn& s = ins[q];
                if (s.mnemonic == "leave" || (s.mnemonic == "mov" && s.op_str == "sp, bp") ||
                    (s.mnemonic == "lea" && s.op_str.starts_with("sp, [bp"))) {   // (OpenWatcom's, before its pops)
                    at_leave = true;
                    break;
                }
                if (s.mnemonic == "push" || s.mnemonic == "pop" || s.mnemonic == "pushf" || s.mnemonic == "popf" ||
                    s.mnemonic == "enter" || s.mnemonic == "ret" || s.mnemonic == "retf" || s.is_call || s.is_jump)
                    break;
                if (!s.writes_sp || *s.writes_sp)
                    break;
            }
        }
        if (!callee || callee->empty())
            continue;
        // (the bytes that are this call's: what the callee removes itself, else what the caller takes off after it, else
        // -- the frame's LEAVE taking them -- all still pushed; a callee that removes none and is followed by no cleanup
        // says nothing of its own)
        const std::int64_t took = removes ? removes(*key).value_or(0) : 0;
        std::optional<std::int64_t> args;
        if (took > 0)
            args = took;
        else if (off.kind == TakenOff::Kind::Bytes && off.bytes > 0)
            args = off.bytes;
        else if (at_leave)
            args = total;
        std::optional<std::int64_t> pushed;
        if (args) {
            if (*args <= total) {
                pushed = *args;
            } else if (pushed_here.exact) {
                // (negative: at least that -- and none counted from a join is no count at all: USER's sub_6_1A72 jumps into
                // its call to sub_1_38E4 from three places, "pushing none")
                pushed = total;
            } else if (total) {
                pushed = -total;
            }
        }
        out.push_back({*callee, std::move(off), pushed, at_leave});
    }
    return out;
}

// ---- the two modules -------------------------------------------------------------------------------------------------

Place place_of(std::uint32_t address) {
    const std::int64_t selector_step = static_cast<std::int64_t>(address >> 16) - static_cast<std::int64_t>(kFirstSelector);
    return {floor_div<std::int64_t>(selector_step, 8) + 1, address & 0xFFFF};
}

DescribedFacts parse_description(std::string_view description) {
    DescribedFacts facts;
    for (const std::string_view line : text::lines(description)) {
        const std::vector<std::string_view> p = text::split(line, '\t');
        if (p.size() < 3 || (p[0] != "X" && p[0] != "F"))
            continue;
        const auto address = text::parse_int<std::uint32_t>(p[1], 16);
        if (!address)
            continue;
        facts.names[*address] = std::string(p[2]);
        if (p.size() > 3) {
            // (a count: a negative one says the function's is not known)
            if (const auto pop = parse_signed(p[3]); pop && p[3].find_first_not_of("-0123456789") == std::string_view::npos)
                facts.pops[*address] = *pop >= 0 ? std::optional<std::int64_t>(*pop) : std::nullopt;
        }
    }
    return facts;
}

namespace {

// removes(key) for `calls`: what the callee at a call's target removes, as described -- or, one the describer did not
// find (the build's functions are found as the original's are, and not all of them: USER's sub_1_2293 rebuilt at 1:5810
// was not, and every call to it "took none"), as its own nearest return says.
Removes removes_of(const Described& d, const DescribedFacts& facts) {
    return [&d, &facts](const CalleeKey& key) -> std::optional<std::int64_t> {
        const bool far = std::holds_alternative<FarCall>(key);
        const std::uint32_t va = far ? std::get<FarCall>(key).address
                                     : address_of(std::get<NearCall>(key).segment, std::get<NearCall>(key).offset);
        if (const auto it = facts.pops.find(va); it != facts.pops.end())
            return it->second;
        const Place where = place_of(va);
        if (where.segment <= 0 || !d.returns)
            return std::nullopt;
        const auto img = d.images.find(static_cast<unsigned>(where.segment));
        if (img == d.images.end())
            return std::nullopt;
        return d.returns(img->second, where.offset, far);
    };
}

// A link map's symbols: for each segment (in the order the map first names them), its (offset, name) pairs, sorted.
struct MapSymbols {
    std::vector<std::pair<std::int64_t, std::vector<std::pair<std::uint32_t, std::string>>>> by_segment;
    std::map<Place, std::string> at;   // (segment, offset) -> name; the later of two at one place wins
};

// One line of the map: "ssss:oooo<non-space>*<spaces>name<spaces>", everything hex.
std::optional<std::tuple<std::int64_t, std::uint32_t, std::string>> map_line(std::string_view line) {
    if (line.size() < 10 || line[4] != ':')
        return std::nullopt;
    const auto segment = text::parse_int<std::int64_t>(line.substr(0, 4), 16);
    const auto offset = text::parse_int<std::uint32_t>(line.substr(5, 4), 16);
    if (!segment || !offset)
        return std::nullopt;
    const std::string_view rest = text::rtrim(line.substr(9));
    const std::vector<std::string_view> tokens = text::split_ws(rest);
    // "ssss:oooo" then optional attached text (a "+" flag), then spaces, then the name, and nothing after it
    std::string_view name;
    if (rest.empty())
        return std::nullopt;
    if (text::is_space(rest.front())) {
        if (tokens.size() != 1)
            return std::nullopt;
        name = tokens[0];
    } else {
        if (tokens.size() != 2)
            return std::nullopt;
        name = tokens[1];
    }
    // the linker's decoration: "@n" at the end (the stack bytes of a __stdcall name), then a leading "_"
    if (const std::size_t at = name.rfind('@'); at != std::string_view::npos && at + 1 < name.size() &&
                                                 name.find_first_not_of("0123456789", at + 1) == std::string_view::npos)
        name = name.substr(0, at);
    if (name.starts_with('_'))
        name.remove_prefix(1);
    return std::make_tuple(*segment, *offset, std::string(name));
}

MapSymbols read_map(std::string_view link_map) {
    MapSymbols out;
    std::map<std::int64_t, std::size_t> index;   // segment -> its place in by_segment (lookup only)
    for (const std::string_view line : text::lines(link_map)) {
        const auto parsed = map_line(line);
        if (!parsed)
            continue;
        const auto& [segment, offset, name] = *parsed;
        const auto [it, fresh] = index.try_emplace(segment, out.by_segment.size());
        if (fresh)
            out.by_segment.push_back({segment, {}});
        out.by_segment[it->second].second.emplace_back(offset, name);
    }
    for (auto& [segment, list] : out.by_segment) {
        std::sort(list.begin(), list.end());
        for (const auto& [offset, name] : list)
            out.at[{segment, offset}] = name;
    }
    return out;
}

std::string list_text(const std::set<TakenOff>& taken) {
    std::string out = "[";
    for (const TakenOff& t : taken)
        out += std::format("{}{}", out.size() > 1 ? ", " : "", t.text());
    return out + "]";
}

std::string list_text(const std::set<std::int64_t>& values) {
    std::string out = "[";
    for (std::int64_t v : values)
        out += std::format("{}{}", out.size() > 1 ? ", " : "", v);
    return out + "]";
}

}  // namespace

std::map<std::string, std::string> mismatches(const Described& original, const Described& built, std::string_view link_map,
                                              std::span<const std::string> names) {
    const DescribedFacts facts_o = parse_description(original.description);
    const DescribedFacts facts_u = parse_description(built.description);
    platforms::DecoderSet decoders;
    const platforms::Decoder& cs = decoders.get(platforms::Arch::X86, 16, true);

    // The original's functions: where each starts and ends, by name; and the name at each place.
    std::map<std::int64_t, std::vector<std::uint32_t>> starts;   // segment -> offsets, sorted
    for (const auto& [va, name] : original.names) {
        const Place w = place_of(va);
        starts[w.segment].push_back(w.offset);
    }
    for (auto& [segment, offsets] : starts)
        std::sort(offsets.begin(), offsets.end());
    struct Extent {
        std::int64_t segment;
        std::uint32_t offset, end;
    };
    std::map<std::string, Extent> original_fn;   // lookup only
    std::map<Place, std::string> original_at;
    for (const auto& [va, name] : original.names) {
        const Place w = place_of(va);
        const std::vector<std::uint32_t>& offsets = starts.at(w.segment);
        const auto later = std::upper_bound(offsets.begin(), offsets.end(), w.offset);
        std::uint32_t end = 0;
        if (later != offsets.end()) {
            end = *later;
        } else if (const auto img = original.images.find(static_cast<unsigned>(w.segment));
                   w.segment > 0 && img != original.images.end()) {
            end = static_cast<std::uint32_t>(img->second.size());
        }
        original_fn[name] = {w.segment, w.offset, end};
        original_at[w] = name;
    }
    const auto original_name = [&](const CalleeKey& key) -> std::optional<std::string> {
        if (const auto* far = std::get_if<FarCall>(&key)) {
            if (const auto it = facts_o.names.find(far->address); it != facts_o.names.end())
                return it->second;
            if (const auto it = original.names.find(far->address); it != original.names.end())
                return it->second;
            return std::nullopt;
        }
        const NearCall& n = std::get<NearCall>(key);
        const auto it = original_at.find(Place{n.segment, n.offset});
        return it == original_at.end() ? std::nullopt : std::optional<std::string>(it->second);
    };

    const MapSymbols symbols = read_map(link_map);
    const auto built_name = [&](const CalleeKey& key) -> std::optional<std::string> {
        if (const auto* far = std::get_if<FarCall>(&key)) {
            // (the build's own map first: the describer names a function of the build by where it is in the build --
            // SETUPX's allocator sub_24_0F1E was "sub_24_052E" there, no call to it compared, and sub_3_7E58 calling it
            // with no size was never seen. An import has no place in the map: its name is the describer's)
            if (const auto it = symbols.at.find(place_of(far->address)); it != symbols.at.end())
                return it->second;
            if (const auto it = facts_u.names.find(far->address); it != facts_u.names.end())
                return it->second;
            return std::nullopt;
        }
        const NearCall& n = std::get<NearCall>(key);
        const auto it = symbols.at.find(Place{n.segment, n.offset});
        return it == symbols.at.end() ? std::nullopt : std::optional<std::string>(it->second);
    };

    const Removes removes_o = removes_of(original, facts_o);
    const Removes removes_u = removes_of(built, facts_u);
    const std::set<std::string_view> wanted(names.begin(), names.end());
    std::map<std::string, std::string> out;

    for (const auto& [segment, list] : symbols.by_segment) {
        const auto built_image = built.images.find(static_cast<unsigned>(segment));
        const ByteView image = built_image == built.images.end() ? ByteView{} : ByteView(built_image->second);
        for (std::size_t k = 0; k < list.size(); ++k) {
            const auto& [offset, name] = list[k];
            const auto fn = original_fn.find(name);
            if (!wanted.contains(name) || fn == original_fn.end())
                continue;
            const std::size_t hi = k + 1 < list.size() ? list[k + 1].first : image.size();
            const Extent& o = fn->second;
            const ByteView original_image = [&] {
                const auto it = original.images.find(static_cast<unsigned>(o.segment));
                return o.segment > 0 && it != original.images.end() ? ByteView(it->second) : ByteView{};
            }();

            std::map<std::string, std::set<TakenOff>> a, b;     // callee -> what the caller takes off after it
            std::map<std::string, std::set<std::int64_t>> pa, pb;   // callee -> the argument bytes pushed for it
            for (const Call& c : calls(cs, original_image, static_cast<unsigned>(o.segment), o.offset, o.end, original_name,
                                       removes_o)) {
                TakenOff t = c.at_leave ? TakenOff{TakenOff::Kind::Leave, 0, {}} : c.taken_off;
                a[c.callee].insert(std::move(t));
                // (at-least counts, negative, say too little to compare)
                if (c.pushed && *c.pushed >= 0)
                    pa[c.callee].insert(*c.pushed);
            }
            for (const Call& c : calls(cs, image, static_cast<unsigned>(segment), offset, hi, built_name, removes_u)) {
                TakenOff t = c.at_leave ? TakenOff{TakenOff::Kind::Leave, 0, {}} : c.taken_off;
                b[c.callee].insert(std::move(t));
                if (c.pushed && *c.pushed >= 0)   // (ours counted from a join: not compared)
                    pb[c.callee].insert(*c.pushed);
            }

            for (const auto& [callee, taken_a] : a) {
                const auto in_b = b.find(callee);
                if (in_b == b.end())
                    continue;
                const std::set<TakenOff>& taken_b = in_b->second;
                if (is_in_registers(callee)) {
                    out[name] = std::format("it calls {}, which takes its arguments in registers", callee);
                    break;
                }
                // (before a LEAVE: whatever was taken off)
                const auto without_leave = [](const std::set<TakenOff>& s) {
                    std::set<TakenOff> r;
                    for (const TakenOff& t : s)
                        if (t.kind != TakenOff::Kind::Leave)
                            r.insert(t);
                    return r;
                };
                const std::set<TakenOff> ta = without_leave(taken_a), tb = without_leave(taken_b);
                if (!ta.empty() && !tb.empty() && ta != tb) {
                    out[name] = std::format("after calling {} the original takes off {} bytes, ours {}", callee,
                                            list_text(taken_a), list_text(taken_b));
                    break;
                }
                // (and fewer argument bytes pushed than the original pushes: SUWIN's sub_8_0634 gave sub_8_0A6A one near
                // pointer for its two far ones -- the second, a 260-byte buffer the caller never reads, dropped -- and a
                // path was written over its frame)
                // (or more: a call is the callee's, the same bytes from any caller -- unless the callee takes a varying
                // number, `...`, as wsprintf does)
                const auto push_a = pa.find(callee), push_b = pb.find(callee);
                if (push_a != pa.end() && push_b != pb.end() && !push_a->second.empty() && !push_b->second.empty()) {
                    const std::set<std::int64_t>& sa = push_a->second;
                    const std::set<std::int64_t>& sb = push_b->second;
                    const bool fewer = *sb.rbegin() < *sa.begin();
                    const bool other = sa != sb && sa.size() == 1 && sb.size() == 1 && !is_variadic(callee);
                    if (fewer || other) {
                        out[name] = std::format("it pushes {} argument bytes for {}, the original {}", list_text(sb),
                                                callee, list_text(sa));
                        break;
                    }
                }
            }
        }
    }
    return out;
}

std::map<std::string, std::string> mismatches(const Describer& describe, const std::filesystem::path& original,
                                              const std::filesystem::path& folder, const std::filesystem::path& image,
                                              std::span<const std::string> names) {
    const Described o = describe(original);
    const Described u = describe(image);   // (a build made again: the same)
    const std::filesystem::path map = folder / "m.map";
    if (!std::filesystem::is_regular_file(map))
        return {};
    return mismatches(o, u, fs::read_text(map), names);
}

}  // namespace cb::verify::gate16::contract16
