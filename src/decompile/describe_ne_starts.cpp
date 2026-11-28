// decompile/describe_ne_starts.cpp -- NE: where a 16-bit module's functions start, and how each one returns (see
// describe.hpp).
// cb-unity: alone

#include <deque>

#include "decompile/data.hpp"
#include "decompile/describe_internal.hpp"

namespace cb::decompile::describe {

using namespace detail;
using formats::ne::Segment;

namespace {

// The bytes of a segment as the file holds them (cut to the file: a table that lies about a segment's length gets what
// is there, the way the loader's reader has always been lenient about it).
ByteView stored_bytes(const formats::ne::Module& m, const Segment& s) {
    ByteView whole = m.data();
    if (s.file_offset == 0 || s.file_offset >= whole.size()) return {};
    return whole.subspan(s.file_offset, std::min<std::uint64_t>(s.length, whole.size() - s.file_offset));
}

// Offsets inside one code segment that two or more `call rel16` sites agree on. A stray 0xE8 in data does not
// coincide with another, so an agreed target is a real function start.
std::vector<std::int64_t> near_call_targets(ByteView code) {
    std::map<std::int64_t, std::set<std::int64_t>> sites;   // iterated: ascending
    const std::int64_t n = static_cast<std::int64_t>(code.size());
    for (std::int64_t i = 0; i < n - 2; ++i) {
        if (code[static_cast<std::size_t>(i)] == 0xE8) {
            const auto rel = static_cast<std::int16_t>(word_at(code, i + 1));
            const std::int64_t tgt = (i + 3 + rel) & 0xFFFF;
            if (tgt < n) sites[tgt].insert(i);
        }
    }
    std::vector<std::int64_t> out;
    for (const auto& [target, from] : sites)
        if (from.size() >= 2) out.push_back(target);
    return out;
}

// The entry sequences of Win16 functions (the compilers' and the hand-written ones' alike)
constexpr std::uint8_t kPrologueBytes[4][4] = {{0x55, 0x8B, 0xEC, 0},         // push bp; mov bp, sp
                                               {0x45, 0x55, 0x8B, 0xEC},      // inc bp; push bp; mov bp, sp   (a far frame)
                                               {0x1E, 0x58, 0x90, 0},         // push ds; pop ax; nop          (exported, as in the file)
                                               {0x8C, 0xD8, 0x90, 0}};        // mov ax, ds; nop
constexpr std::size_t kPrologueLength[4] = {3, 4, 3, 3};

bool bytes_at(ByteView code, std::int64_t o, const std::uint8_t* p, std::size_t n) {
    if (o < 0 || static_cast<std::size_t>(o) + n > code.size()) return false;
    return std::equal(p, p + n, code.begin() + o);
}

bool is_prologue(ByteView code, std::int64_t o) {
    for (int k = 0; k < 4; ++k)
        if (bytes_at(code, o, kPrologueBytes[k], kPrologueLength[k])) return true;
    const std::int64_t n = static_cast<std::int64_t>(code.size());
    if (o >= 0 && o < n && code[static_cast<std::size_t>(o)] == 0xC8 && o + 4 <= n &&
        (code[static_cast<std::size_t>(o) + 3] == 0 || code[static_cast<std::size_t>(o) + 3] == 1))
        return true;   // enter n, 0|1
    static constexpr std::uint8_t kFarFrame[4] = {0x45, 0x55, 0x8B, 0xEC};
    return o >= 0 && o < n && code[static_cast<std::size_t>(o)] == 0xB8 && bytes_at(code, o + 3, kFarFrame, 4);   // mov ax, DGROUP; inc bp ...
}

bool ends_function(std::string_view m) { return eq_any(m, {"ret", "retf", "iret", "jmp", "ljmp"}); }

// Near call targets in the code reached from `start` (fall-through and jumps followed).
std::set<std::int64_t> calls_followed(const Code16& code, std::int64_t start, std::size_t limit = 20000) {
    std::set<std::int64_t> calls, seen;
    std::vector<std::int64_t> work{start};
    const std::int64_t n = code.size();
    while (!work.empty() && seen.size() < limit) {
        std::int64_t at = work.back();
        work.pop_back();
        while (0 <= at && at < n && !seen.count(at)) {
            const Insn* ins = code.one(at);
            if (!ins) break;
            seen.insert(at);
            const int b0 = ins->bytes[0];
            if (b0 == 0xE8 && ins->size == 3) {
                const auto rel = static_cast<std::int16_t>(ins->bytes[1] | (ins->bytes[2] << 8));
                const std::int64_t t = (at + 3 + rel) & 0xFFFF;
                if (t < n) calls.insert(t);
            } else if (b0 == 0xEB || b0 == 0xE9 || (0x70 <= b0 && b0 <= 0x7F) || (0xE0 <= b0 && b0 <= 0xE3) ||
                       (b0 == 0x0F && 0x80 <= ins->bytes[1] && ins->bytes[1] <= 0x8F)) {
                if (auto v = hex_of(ins->op_str)) {
                    const std::int64_t t = *v & 0xFFFF;
                    if (t < n) work.push_back(t);
                }
            }
            if (ends_function(ins->mn)) break;
            at += ins->size;
        }
    }
    return calls;
}

// Places in the module's code segments a far pointer names whose offset is a plain number beside a selector fix-up:
// `call far` / `jmp far` (9A/EA, the offset inline), a pointer pushed (`push seg X` / `push n`), or stored as two words
// side by side (`mov [bp+d], n` / `mov [bp+d+2], seg X`, either order). Such a place is a function -- often one nothing
// calls by a near call and the module does not export: a callback (USER's 1:3452, passed to an enumeration from 1:36E8).
// Not seen, it was taken for the tail of the function before it.
std::set<SegOff> far_pointer_targets(const formats::ne::Module& m, const Segment& s, ByteView code,
                                     const std::map<std::uint16_t, Segment>& segs) {
    std::set<SegOff> out;
    const std::int64_t n = static_cast<std::int64_t>(code.size());
    const std::vector<formats::ne::Relocation> relocations = m.relocations(s);
    auto u16 = [&](std::int64_t at) { return word_at(code, at); };
    auto b = [&](std::int64_t at) -> int { return code[static_cast<std::size_t>(at)]; };
    for (const decompile::data::RelocationPlace& place : decompile::data::places(relocations, code)) {
        const std::int64_t o = place.offset;
        const formats::ne::Relocation& r = relocations[place.record];
        const auto* internal = std::get_if<formats::ne::InternalRef>(&r.target);
        if (place.source != 2 || !internal || internal->segment == 0xFF) continue;
        const auto sit = segs.find(internal->segment);
        if (sit == segs.end() || sit->second.is_data()) continue;
        std::vector<std::int64_t> offs;
        if (o >= 3 && (b(o - 3) == 0x9A || b(o - 3) == 0xEA)) offs.push_back(u16(o - 2));   // call/jmp far off:seg
        if (o >= 1 && b(o - 1) == 0x68 && o + 5 <= n && b(o + 2) == 0x68) offs.push_back(u16(o + 3));   // push seg / push off
        if (o >= 3 && b(o - 3) == 0xC7 && b(o - 2) == 0x46) {   // mov [bp+d], seg ...
            const int d = b(o - 1);
            if (o >= 8 && b(o - 8) == 0xC7 && b(o - 7) == 0x46 && b(o - 6) == ((d - 2) & 0xFF))
                offs.push_back(u16(o - 5));   // ... after mov [bp+d-2], off
            if (o + 7 <= n && b(o + 2) == 0xC7 && b(o + 3) == 0x46 && b(o + 4) == ((d - 2) & 0xFF))
                offs.push_back(u16(o + 5));   // ... before it
        }
        for (std::int64_t v : offs)
            if (0 <= v && v < static_cast<std::int64_t>(sit->second.length)) out.insert({internal->segment, static_cast<std::uint32_t>(v)});
    }
    return out;
}

}  // namespace

std::vector<std::int64_t> prologues_after_ends(const Code16& code) {
    std::vector<std::int64_t> out;
    std::int64_t at = 0;
    const std::int64_t n = code.size();
    const ByteView bytes = code.bytes();
    while (at < n) {
        const Insn* ins = code.one(at);
        if (!ins) {
            at += 1;
            continue;
        }
        at += ins->size;
        if (ends_function(ins->mn)) {
            std::int64_t p = at;
            while (p < n && (bytes[static_cast<std::size_t>(p)] == 0x00 || bytes[static_cast<std::size_t>(p)] == 0x90 || bytes[static_cast<std::size_t>(p)] == 0xCC)) ++p;
            if (p < n && is_prologue(bytes, p)) out.push_back(p);
        }
    }
    return out;
}

// How the function at `off` returns: read from the function's own first return instruction: `retf` is a far function,
// `ret` a near one, and the operand is what it removes. That is the function's own statement of how it must be called;
// where it was found (entry table, a relocation, a near call) only says how SOMEONE calls it. No kind when no return is
// met within `limit` instructions.
Return returns(const Code16& code, std::int64_t off, std::int64_t limit, std::optional<Distance> want) {
    const std::int64_t len = code.size();
    const ByteView bytes = code.bytes();
    // The code is followed, not read straight down: functions share epilogues (Windows exports beside each other jump
    // into one `retf 8` that may lie BEFORE them), and straight down from a jump is often someone else's code and
    // someone else's return. The nearest return by the paths the function takes is its own.
    // (one instruction at a time, every path abreast: the return fewest instructions away)
    std::deque<std::int64_t> work{off};
    std::set<std::int64_t> seen;
    std::int64_t n = 0;
    while (!work.empty() && n < limit) {
        const std::int64_t at = work.front();
        work.pop_front();
        if (!(0 <= at && at < len) || seen.count(at)) continue;
        seen.insert(at);
        const Insn* insp = code.one(at);
        if (!insp) continue;
        const Insn& ins = *insp;
        ++n;
        if (ins.mn == "ret" || ins.mn == "retf" || ins.mn == "lret") {
            const Distance kind = ins.mn == "ret" ? Distance::Near : Distance::Far;
            const std::int64_t pop = ins.op_str.empty() ? 0 : number_of(ins.op_str).value_or(0);
            if ((pop & 1) || pop > 0x7E) continue;   // (no function removes an odd count or so many: bytes that are not code, decoded)
            if (!want || kind == *want) return {kind, pop};
            continue;
        }
        std::optional<std::int64_t> target;
        if (ins.op_str.size() > 2 && ins.op_str.starts_with("0x") &&
            std::all_of(ins.op_str.begin() + 2, ins.op_str.end(), [](unsigned char c) { return std::isalnum(c) != 0; }))
            target = number_of(ins.op_str);
        if (ins.mn == "push" && target && off < *target && *target < len) {
            // a return address it pushes -- the stubs push the place of a `retf N` (KRNL386's LoadResource: push 13Dh ...
            // jmp worker; at 13Dh, retf 4) -- only when a return is what is there: a constant pushed as an argument is not
            // one (USER sub_24_00F6's `push 98Bh` was followed into the function after it, to a `ret 0EEBh` in its bytes,
            // and the dialog builder was called with two words fewer than it takes)
            // (or a few straight instructions before it: GetMessage's is `mov ax, -1` / `retf 0Ah`; or a shared tail with
            // a frame of its own: GetWindowText pushes AB36h, `push bp / mov bp, sp ...`, which ends in its `retf 8`)
            if (is_prologue(bytes, *target)) work.push_back(*target);
            const std::vector<Insn> there = code.decode(*target, *target + 16, false, 4);
            int k = 0;
            for (const Insn& t : there) {
                if (t.mn == "ret" || t.mn == "retf") {
                    work.push_back(*target);
                    break;
                }
                if (k >= 2 || t.mn.starts_with("j") || t.mn == "call" || t.mn == "lcall") break;
                ++k;
            }
        }
        if (ins.mn == "jmp") {
            if (target) work.push_back(*target);   // (through a table or register: not followed)
            continue;
        }
        if (ins.mn == "ljmp" || ins.mn == "iret") continue;   // (a far jump leaves: what is after it is someone else's)
        if ((ins.mn.starts_with("j") || eq_any(ins.mn, {"loop", "loope", "loopne", "jcxz"})) && target) work.push_back(*target);
        work.push_back(ins.address + ins.size);
    }
    return {std::nullopt, -1};
}

// Every function start that can be established.
std::map<SegOff, Distance> find_functions(const formats::ne::Module& m) {
    const std::vector<Segment> seglist = m.segments();
    std::map<std::uint16_t, Segment> segs;
    for (const Segment& s : seglist) segs[s.index] = s;
    auto is_code = [&](std::uint32_t idx) {
        const auto it = segs.find(static_cast<std::uint16_t>(idx));
        return it != segs.end() && !it->second.is_data();
    };
    std::map<SegOff, Distance> out;
    for (const formats::ne::Entry& e : m.entries())
        if (e.kind != formats::ne::Entry::Kind::Unused && is_code(e.segment)) out[SegOff{e.segment, e.offset}] = Distance::Far;
    // the module's entry point (a library's LibEntry: what Windows runs when it loads it -- it makes the local heap and
    // sets the module up) is a function, called far by the loader
    const std::uint32_t cs = m.header().cs_ip >> 16, ip = m.header().cs_ip & 0xFFFF;
    if (cs != 0 && is_code(cs)) out[SegOff{cs, ip}] = Distance::Far;
    std::set<std::uint32_t> used;   // (segments some export starts in: below)
    for (const auto& [place, how] : out) used.insert(place.seg);
    std::map<std::uint16_t, std::unique_ptr<Code16>> codes;
    for (const Segment& s : seglist) {
        if (s.is_data()) continue;
        for (const formats::ne::Relocation& r : m.relocations(s)) {
            const auto* internal = std::get_if<formats::ne::InternalRef>(&r.target);
            if (internal && internal->segment != 0xFF && is_code(internal->segment) && r.source == 3)
                out.try_emplace(SegOff{internal->segment, internal->entry}, Distance::Far);
        }
        const ByteView stored = stored_bytes(m, s);
        for (std::int64_t off : near_call_targets(stored)) out.try_emplace(SegOff{s.index, static_cast<std::uint32_t>(off)}, Distance::Near);
        for (const SegOff& place : far_pointer_targets(m, s, stored, segs)) out.try_emplace(place, Distance::Far);
        // a function nothing visibly reaches (through a table, a pointer computed or kept in data): its entry sequence
        // right after another function's end. Taken for that one's tail, it was erased with it when that one was rebuilt
        // from C (USER had 95)
        auto& slot = codes[s.index];
        slot = std::make_unique<Code16>(Bytes(stored.begin(), stored.end()));
        for (std::int64_t off : prologues_after_ends(*slot)) {
            const Return how = returns(*slot, off, 600);
            out.try_emplace(SegOff{s.index, static_cast<std::uint32_t>(off)}, how.kind.value_or(Distance::Far));
        }
    }
    // a code segment nothing above names a function in (reached by its number from outside, or dead) still is one of the
    // module's segments: what it holds starts at 0 -- far or near as how it is reached says (a far fix-up, a near call:
    // above), else as its own return says.
    // (SUWIN's 7:0000 is a near helper -- `ret`, called `call 0` -- and taken for far, its C caller called it far and lost
    // a word to the stack on every call: ten in a loop, then a `retf` into nothing, Illegal Instruction)
    for (const Segment& s : seglist) {
        if (!s.is_data() && s.length != 0 && !used.count(s.index) && !out.contains(SegOff{s.index, 0})) {
            const Code16 code(s.file_offset != 0 ? Bytes(stored_bytes(m, s).begin(), stored_bytes(m, s).end()) : Bytes{});
            out[SegOff{s.index, 0}] = returns(code, 0, 600).kind.value_or(Distance::Far);
        }
    }
    // what the code calls, followed from every start known -- one call is enough when it is in code that is followed (a
    // stray E8 in data is not)
    std::set<SegOff> done;
    for (;;) {
        std::vector<SegOff> fresh;
        for (const auto& [place, how] : out)
            if (!done.count(place) && codes.count(static_cast<std::uint16_t>(place.seg))) fresh.push_back(place);
        if (fresh.empty()) break;
        for (const SegOff& k : fresh) {
            done.insert(k);
            for (std::int64_t t : calls_followed(*codes.at(static_cast<std::uint16_t>(k.seg)), k.off))
                out.try_emplace(SegOff{k.seg, static_cast<std::uint32_t>(t)}, Distance::Near);
        }
    }
    return out;
}

// The guard: entry sequences after an end that are not among `known` -- functions the description does not have, so
// that the build would take them for the tail of the function before.
std::vector<SegOff> hidden_functions(const formats::ne::Module& m, const std::set<SegOff>& known) {
    std::vector<SegOff> out;
    for (const Segment& s : m.segments()) {
        if (s.is_data() || s.file_offset == 0) continue;
        const ByteView stored = stored_bytes(m, s);
        const Code16 code{Bytes(stored.begin(), stored.end())};
        for (std::int64_t o : prologues_after_ends(code))
            if (!known.contains(SegOff{s.index, static_cast<std::uint32_t>(o)})) out.push_back({s.index, static_cast<std::uint32_t>(o)});
    }
    return out;
}

}  // namespace cb::decompile::describe
