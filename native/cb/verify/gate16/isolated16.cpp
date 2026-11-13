#include "cb/verify/gate16/isolated16.hpp"

#include <algorithm>
#include <format>
#include <mutex>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/ne.hpp"
#include "cb/jobs/pool.hpp"

namespace cb::verify::gate16 {

namespace {

namespace w16 = emulate::windows16;
namespace stdfs = std::filesystem;
using lanes::Vector16;

using Places = std::map<std::pair<std::uint32_t, std::uint32_t>, std::string>;   // (segment, offset) -> the module's function there

// An argument word as what it is, where two builds legitimately differ in its bits.
std::string iso_word(std::int64_t w, const w16::EntryRegisters& entry, std::optional<std::int64_t> next, const Places& places) {
    if (!places.empty() && next) {
        // (the offset half of a far pointer to one of the module's own functions -- a timer's or an enumeration's callback -- is that function: it sits
        // elsewhere in a rebuild. USER's SetCaretBlinkTime passes 13:0D65 in the original, 13:04B6 in the build: the same one)
        const std::int64_t segment = (*next >> 3) - static_cast<std::int64_t>(w16::kModuleSlot) + 1;
        if (segment >= 1 && w >= 0) {
            const auto it = places.find({static_cast<std::uint32_t>(segment), static_cast<std::uint32_t>(w)});
            if (it != places.end()) return "&" + it->second;
        }
    }
    if (w == entry.ds) return "DS";
    if (w == entry.ss) return "SS";
    if ((next && *next == entry.ss) || (0xE000 <= w && w < 0xFFF0)) return "stack";   // a place in the stack (frames differ)
    return std::format("{:x}", w);
}

std::string iso_hex(ByteView bytes) {
    std::string s;
    for (std::uint8_t b : bytes) s += std::format("{:02x}", b);
    return s;
}

std::string iso_list(const std::vector<std::string>& items, std::size_t from) {
    std::vector<std::string> part;
    for (std::size_t i = from; i < items.size() && i < from + 2; ++i) part.push_back(items[i]);
    return "[" + text::join(part, ", ") + "]";
}

std::string iso_result_text(GatePrint::ResultKind kind, std::int64_t value) {
    switch (kind) {
    case GatePrint::ResultKind::Absent: return "none";
    case GatePrint::ResultKind::Number: return std::format("{:x}", value);
    case GatePrint::ResultKind::DataSelector: return "DS";
    case GatePrint::ResultKind::NoResult: return "no result";
    case GatePrint::ResultKind::Byte: return std::format("byte {:x}", value);
    }
    return "?";
}

// The same fingerprints of an input the original and the rebuild made: equal where they agree.
bool iso_same_input(const Vector16& a, const Vector16& b) {
    if (a.args != b.args || a.stub_ret != b.stub_ret || a.mem.size() != b.mem.size()) return false;
    for (std::size_t i = 0; i < a.mem.size(); ++i) {
        if (a.mem[i].data != b.mem[i].data) return false;
        const auto* la = std::get_if<std::uint32_t>(&a.mem[i].where);
        const auto* lb = std::get_if<std::uint32_t>(&b.mem[i].where);
        if ((la == nullptr) != (lb == nullptr)) return false;
        if (la != nullptr ? *la != *lb : std::get<w16::DataOffset>(a.mem[i].where).offset != std::get<w16::DataOffset>(b.mem[i].where).offset) return false;
    }
    return true;
}

}  // namespace

// ---- tables ----------------------------------------------------------------------------------------------------------------------------------

knowledge::Value IsolatedTables::to_value() const {
    using KV = knowledge::Value;
    KV::Map f, p;
    for (const auto& [name, fn] : funcs) f.insert_or_assign(name, KV(KV::List{KV(fn.removes), KV(fn.far)}));
    for (const auto& [name, at] : place) p.insert_or_assign(name, KV(KV::List{KV(static_cast<std::int64_t>(at.seg)), KV(static_cast<std::int64_t>(at.off))}));
    KV::List w;
    for (const std::string& n : wide) w.emplace_back(n);
    return KV(KV::Map{{"funcs", KV(std::move(f))}, {"wide", KV(std::move(w))}, {"place", KV(std::move(p))}});
}

IsolatedTables IsolatedTables::from_value(const knowledge::Value& doc) {
    IsolatedTables out;
    const knowledge::Value* f = doc.get("funcs");
    const knowledge::Value* w = doc.get("wide");
    const knowledge::Value* p = doc.get("place");
    if (f == nullptr || f->map() == nullptr || w == nullptr || w->list() == nullptr || p == nullptr || p->map() == nullptr)
        throw FormatError("gate tables", 0, "the document has no funcs, wide and place");
    for (const auto& [name, row] : *f->map()) {
        const knowledge::Value::List* l = row.list();
        if (l == nullptr || l->size() < 2 || !(*l)[0].integer() || !(*l)[1].boolean()) throw FormatError("gate tables", 0, "a function is not [removes, far]: " + name);
        out.funcs.insert_or_assign(name, Function{*(*l)[0].integer(), *(*l)[1].boolean()});
    }
    for (const knowledge::Value& n : *w->list()) {
        if (n.text() == nullptr) throw FormatError("gate tables", 0, "a wide function is not a name");
        out.wide.push_back(*n.text());
    }
    for (const auto& [name, row] : *p->map()) {
        const knowledge::Value::List* l = row.list();
        if (l == nullptr || l->size() < 2 || !(*l)[0].integer() || !(*l)[1].integer()) throw FormatError("gate tables", 0, "a place is not [segment, offset]: " + name);
        out.place[name] = {static_cast<std::uint32_t>(*(*l)[0].integer()), static_cast<std::uint32_t>(*(*l)[1].integer())};
    }
    return out;
}

std::map<std::string, core16::SegOff> built_places(const stdfs::path& folder) {
    // `0001:0000049e  _NAME@8` -- a line of the linker's map: the segment, the offset, the symbol (its underscore and `@bytes` are the compiler's).
    static const Regex line_re(R"(^([0-9a-f]{4}):([0-9a-f]{4,8})[+*]?\s+_?([A-Za-z_]\w*?)(?:@\d+)?\s*$)");
    const std::string map = fs::read_text(folder / "m.map");
    std::map<std::string, core16::SegOff> out;
    for (const std::string_view raw : text::lines(map)) {
        const std::string_view line = text::trim(raw);
        const auto m = line_re.match(line);
        if (!m) continue;
        const auto seg = text::parse_int<std::uint32_t>(m->group(1), 16);
        const auto off = text::parse_int<std::uint32_t>(m->group(2), 16);
        if (seg && off) out.try_emplace(std::string(m->group(3)), core16::SegOff{*seg, *off});
    }
    return out;
}

// ---- what a run did ---------------------------------------------------------------------------------------------------------------------------

GatePrint gate_print(const emulate::Run& r, bool wide, std::size_t data_length, const Places& places) {
    const auto trace = std::any_cast<std::shared_ptr<const w16::Trace16>>(&r.raw);
    if (trace == nullptr || !*trace) throw InternalError("a 16-bit run without its record");
    const w16::Trace16& t = **trace;
    const w16::EntryRegisters& entry = t.entry_regs;
    GatePrint out;
    out.exit = t.exit_text();
    if (out.exit.find("exception") != std::string::npos || out.exit.find("fault") != std::string::npos) out.exit = "fault";
    for (const w16::CalleeCall& c : t.calls_out) {
        std::vector<std::string> words;
        for (std::size_t i = 0; i < c.args.size(); ++i)
            words.push_back(iso_word(c.args[i], entry, i + 1 < c.args.size() ? std::optional<std::int64_t>(c.args[i + 1]) : std::nullopt, places));
        out.calls.push_back(std::format("{}({})", c.name, text::join(words, ",")));
    }
    for (const emulate::ImportHit& h : t.imports_hit) out.calls.push_back("api " + lanes::win16_call_text(h, {}, entry, {}));
    const std::optional<std::uint32_t> ds = t.layout.data_base;
    // (and what it leaves anywhere else but its stack: a caller's buffer reached through a far pointer, a block it was given -- the same place on both
    // sides, the module's own segments sitting at the same bases. The data segment alone hid a function filling a RECT wrongly)
    std::map<std::int64_t, std::uint8_t> state, before, far, far_before;
    const std::uint64_t gdt_low = w16::kGdtAddress, gdt_high = static_cast<std::uint64_t>(w16::kGdtAddress) + w16::kGdtSlots * 8;
    for (const emulate::MemoryWrite& w : r.writes) {
        std::map<std::int64_t, std::uint8_t>* into = nullptr;
        std::map<std::int64_t, std::uint8_t>* at = nullptr;
        std::uint64_t base = 0;
        if (ds && *ds <= w.address && w.address < static_cast<std::uint64_t>(*ds) + data_length) {
            into = &state;
            at = &before;
            base = *ds;
        } else if (w16::kStackBase <= w.address && w.address < static_cast<std::uint64_t>(w16::kStackBase) + 0x10000) {
            continue;   // (its frame, its arguments)
        } else if (gdt_low <= w.address && w.address < gdt_high) {
            // (the processor marking a descriptor accessed as a selector is loaded -- `lds si, p` sets a bit at GDT+400h for selector 400h -- is not the
            // function's doing: GDI's sub_9_1110 loads DS before a loop that does not run; its C loads nothing)
            continue;
        } else {
            into = &far;
            at = &far_before;
        }
        for (std::uint32_t i = 0; i < std::min<std::uint32_t>(w.size, 4); ++i) {
            const std::int64_t k = static_cast<std::int64_t>(w.address - base) + i;
            at->try_emplace(k, static_cast<std::uint8_t>(w.old.value_or(0) >> (8 * i)));
            (*into)[k] = static_cast<std::uint8_t>(w.value >> (8 * i));
        }
    }
    for (const auto& [k, x] : state) {
        const auto was = before.find(k);
        if (was == before.end() || was->second != x) out.data[k] = x;
    }
    // (through a far pointer: what is written, not whether it changed what was there -- the place may be in the module's own segments, whose bytes the two
    // builds lay out differently: USER's GetCaretPos wrote 0 over 56F8h in the original and over 007Bh in the build, and its high byte "changed" on one side only)
    out.far = far;
    if (out.exit != "ret") return out;
    std::int64_t result = static_cast<std::int64_t>(wide ? t.dx_ax : t.ax);
    out.result_kind = GatePrint::ResultKind::Number;
    out.result = result;
    if (t.unsure) return out;   // a callee removing it is not known how many bytes: the stack after it says nothing
    if ((result & 0xFFFF) == entry.ds && !wide) {
        out.result_kind = GatePrint::ResultKind::DataSelector;
    } else if ((result & 0xFFFF) == w16::kEntryAx && !wide) {
        out.result_kind = GatePrint::ResultKind::NoResult;   // (AX as it came in: no result set)
    } else if ((result & 0xFF00) == (w16::kEntryAx & 0xFF00) && !wide) {
        out.result_kind = GatePrint::ResultKind::Byte;       // (AL set, AH the caller's: a byte result)
        out.result = result & 0xFF;
    }
    out.pop = t.sp_delta;
    out.kept = std::array<bool, 3>{t.exit_regs.si == entry.si, t.exit_regs.di == entry.di, t.exit_regs.ds == entry.ds};
    return out;
}

std::optional<std::string> gate_difference(const GatePrint& a, const GatePrint& b) {
    if (a.exit == "truncated" || b.exit == "truncated") {
        // (a run stopped at the instruction cap says nothing past where it stopped: a long loop -- a count read from the patterned memory -- ran its
        // 1,052nd pass in the original and not in the build, whose body is a few instructions longer. What both did, the calls up to the shorter run's last,
        // is what compares)
        const std::size_t n = std::min(a.calls.size(), b.calls.size());
        for (std::size_t i = 0; i < n; ++i)
            if (a.calls[i] != b.calls[i]) return std::format("calls: original {}, ours {}", iso_list(a.calls, i), iso_list(b.calls, i));
        return std::nullopt;
    }
    if (a.exit != b.exit) return std::format("ends: original {}, ours {}", a.exit, b.exit);
    if (a.calls != b.calls) {
        std::size_t i = 0;
        while (i < a.calls.size() && i < b.calls.size() && a.calls[i] == b.calls[i]) ++i;
        return std::format("calls: original {}, ours {}", iso_list(a.calls, i), iso_list(b.calls, i));
    }
    for (const auto& [what, x, y] : {std::tuple{"leaves in the data segment", &a.data, &b.data}, std::tuple{"leaves through a far pointer", &a.far, &b.far}}) {
        if (*x == *y) continue;
        std::set<std::int64_t> at;
        for (const auto& [o, v] : *x)
            if (!y->contains(o) || y->at(o) != v) at.insert(o);
        for (const auto& [o, v] : *y)
            if (!x->contains(o) || x->at(o) != v) at.insert(o);
        std::vector<std::string> mine, theirs;
        for (const std::int64_t o : at) {
            if (mine.size() == 4) break;
            mine.push_back(std::format("{:04x}={}", o, x->contains(o) ? std::to_string(x->at(o)) : "none"));
            theirs.push_back(std::format("{:04x}={}", o, y->contains(o) ? std::to_string(y->at(o)) : "none"));
        }
        return std::format("{}: original [{}], ours [{}]", what, text::join(mine, ", "), text::join(theirs, ", "));
    }
    // the result: a function that sets none matches anything; a byte result is matched by its low byte
    if (a.result_kind != GatePrint::ResultKind::NoResult) {
        bool same = a.result_kind == b.result_kind && a.result == b.result;
        if (!same && a.result_kind == GatePrint::ResultKind::Byte) {   // (the original returns a byte: AL is all)
            const std::int64_t low = b.result_kind == GatePrint::ResultKind::Byte ? b.result : b.result_kind == GatePrint::ResultKind::Number ? (b.result & 0xFF) : -1;
            same = low == a.result;
        }
        if (!same) return std::format("returns: original {}, ours {}", iso_result_text(a.result_kind, a.result), iso_result_text(b.result_kind, b.result));
    }
    if (a.pop != b.pop) return std::format("removes argument bytes: original {}, ours {}", a.pop ? std::to_string(*a.pop) : "none", b.pop ? std::to_string(*b.pop) : "none");
    if (a.kept != b.kept) {
        const auto show = [](const std::optional<std::array<bool, 3>>& k) {
            return k ? std::format("{}/{}/{}", (*k)[0] ? "kept" : "changed", (*k)[1] ? "kept" : "changed", (*k)[2] ? "kept" : "changed") : std::string("none");
        };
        return std::format("keeps SI, DI, DS: original {}, ours {}", show(a.kept), show(b.kept));
    }
    return std::nullopt;
}

std::string gate_input_text(const Vector16& v) {
    std::vector<std::string> words;
    for (std::size_t i = 0; i < v.args.size() && i < 4; ++i) words.push_back(std::format("{:x}", v.args[i]));
    std::string out = "args " + text::join(words, " ");
    if (v.guided) {
        std::vector<std::string> extra;
        for (std::size_t i = 0; i < v.mem.size() && i < 2; ++i) {
            if (const auto* linear = std::get_if<std::uint32_t>(&v.mem[i].where)) extra.push_back(std::format("{:x}={}", *linear, iso_hex(v.mem[i].data)));
            else extra.push_back(std::format("DS:{:04x}={}", std::get<w16::DataOffset>(v.mem[i].where).offset, iso_hex(v.mem[i].data)));
        }
        if (v.stub_ret) extra.push_back(std::format("windows returns {:x}", *v.stub_ret));
        out = "guided " + out;
        for (const std::string& e : extra) out += ", " + e;
    }
    return out;
}

// ---- the check -----------------------------------------------------------------------------------------------------------------------------------

Isolated16::Isolated16(Isolated16Spec spec) : spec_(std::move(spec)) {}

namespace {

// Everything one share of a check shares: the original's side of each run, kept within the check.
struct IsoShare {
    const Isolated16Spec& spec;
    stdfs::path original, image;
    const IsolatedTables& tables;
    const std::map<std::string, core16::SegOff>& ours;
    const std::optional<std::vector<Vector16>>& given;
    bool chain = false;
};

std::vector<Isolated16Answer> iso_run_share(const IsoShare& s, const std::vector<std::string>& names) {
    std::vector<Isolated16Answer> answers;
    emulate::native_trace::Governor governor;
    w16::Environment env = s.spec.environment ? s.spec.environment(governor) : w16::Environment{};
    env.governor = &governor;
    w16::Runner runner(env);
    const formats::ne::Module nef = formats::ne::Module::open_file(s.original);
    const std::vector<formats::ne::Segment> segments = nef.segments();
    const unsigned auto_seg = nef.header().autodata_segment;
    const formats::ne::Segment* seg = auto_seg >= 1 && auto_seg <= segments.size() ? &segments[auto_seg - 1] : nullptr;   // (a driver may have none)
    const Bytes data = seg != nullptr && seg->file_offset != 0 ? runner.relocated_data(s.original) : Bytes();   // (fixed up, as loaded)
    // (and the scattered inputs once more over a data segment whose zeros are a pattern, the same on both sides: much of it is zeros at load, and a C function
    // reading the global beside the one it should read the zero beside a zero)
    Bytes noisy = data;
    for (std::size_t i = 0; i < noisy.size(); ++i)
        if (noisy[i] == 0) noisy[i] = static_cast<std::uint8_t>((((static_cast<std::uint64_t>(i) * 0x9E3779B1ULL) >> 13) & 0xFF) | 1);
    std::vector<Vector16> noisy_runs;
    if (!data.empty()) {
        const std::vector<Vector16> fixed = lanes::win16_vectors();
        for (std::size_t i = fixed.size() - 4; i < fixed.size(); ++i) {
            Vector16 v = fixed[i];
            v.noisy = true;
            noisy_runs.push_back(std::move(v));
        }
    }
    // (where the original's data ends and its segment ends: past them both sides hold the same)
    std::optional<w16::DataEnd> data_end;
    if (seg != nullptr) {
        const std::uint32_t end = std::max(seg->length, seg->min_alloc);
        data_end = w16::DataEnd{seg->length, end != 0 ? end : 0x10000};
    }
    // (functions both sides know by name: a data symbol -- the build's codebreaker_data at DS:0 -- or a name only one side's map has would make "0, DS" a
    // pointer on one side alone)
    Places places_orig, places_ours;
    for (const auto& [n, fn] : s.tables.funcs) {
        const auto o = s.tables.place.find(n);
        const auto b = s.ours.find(n);
        if (o == s.tables.place.end() || b == s.ours.end()) continue;
        places_orig[{o->second.seg, o->second.off}] = n;
        places_ours[{b->second.seg, b->second.off}] = n;
    }
    w16::ProbeOptions base_orig, base_ours;
    const auto callees_of = [&](const std::map<std::string, core16::SegOff>& where) {
        w16::Callees out;
        for (const auto& [n, fn] : s.tables.funcs) {
            const auto at = where.find(n);
            if (at == where.end()) continue;
            out[at->second] = w16::Callee{n, fn.removes >= 0 ? std::optional<std::uint32_t>(static_cast<std::uint32_t>(fn.removes)) : std::nullopt, fn.far};
        }
        return out;
    };
    // (chained: the module's other functions run for real on both sides -- what one function leaves for another, through the stack, registers or memory, is
    // then seen)
    if (!s.chain) {
        base_orig.callees = callees_of(s.tables.place);
        base_ours.callees = callees_of(s.ours);
    }
    base_orig.insn_cap = base_ours.insn_cap = s.chain ? 200000 : 20000;
    base_orig.data_end = base_ours.data_end = data_end;

    // A recorded call's context for one side: the module's own code its far pointers reach (a callback in the recorded memory) sent to that side's function of
    // the same name.
    std::map<std::pair<const w16::RecordedCall*, bool>, std::shared_ptr<const w16::RecordedCall>> sided;
    const auto side_context = [&](const std::shared_ptr<const w16::RecordedCall>& c, bool original_side) -> std::shared_ptr<const w16::RecordedCall> {
        if (!c || c->own.empty()) return c;
        auto& kept = sided[{c.get(), original_side}];
        if (!kept) {
            auto copy = std::make_shared<w16::RecordedCall>(*c);
            const auto& where = original_side ? s.tables.place : s.ours;
            for (const auto& [n, fn] : s.tables.funcs) {
                const auto o = s.tables.place.find(n);
                const auto b = s.ours.find(n);
                if (o == s.tables.place.end() || b == s.ours.end() || where.find(n) == where.end()) continue;
                copy->redirect[{o->second.seg, o->second.off}] = w16::Redirect{where.at(n).seg, where.at(n).off, n};
            }
            kept = copy;
        }
        return kept;
    };

    const std::set<std::string> wide_set(s.tables.wide.begin(), s.tables.wide.end());
    for (const std::string& name : names) {
        const auto fn = s.tables.funcs.find(name);
        const auto place = s.tables.place.find(name);
        const auto built = s.ours.find(name);
        if (place == s.tables.place.end() || built == s.ours.end() || fn == nullptr) {
            answers.push_back({name, Verdict::Inconclusive, "not found on both sides"});
            continue;
        }
        const bool far = fn->far;
        const bool wide = wide_set.contains(name);
        const std::uint32_t oseg = place->second.seg, ooff = place->second.off, bseg = built->second.seg, boff = built->second.off;
        std::string why;
        // (and inputs aimed at what its own code tests: the constants it compares with)
        std::uint32_t later = 0x10000;
        for (const auto& [other, at] : s.tables.place)
            if (at.seg == oseg && at.off > ooff) later = std::min(later, at.off);
        const lanes::CodeSpan span{ooff, later};
        const std::vector<Vector16> aimed = lanes::win16_directed(nef, oseg, span, 12);
        std::vector<Vector16> seeds = lanes::win16_vectors();
        seeds.insert(seeds.end(), aimed.begin(), aimed.end());
        Bytes code;
        if (oseg >= 1 && oseg <= segments.size() && segments[oseg - 1].file_offset < nef.data().size()) {
            const formats::ne::Segment& sg = segments[oseg - 1];
            const ByteView bytes = nef.data().subspan(sg.file_offset, std::min<std::uint64_t>(sg.length, nef.data().size() - sg.file_offset));
            code.assign(bytes.begin(), bytes.end());
        }
        lanes::JumpTable jumps(std::move(code), oseg);
        lanes::Win16Target original_target{s.original, oseg, ooff, !far, wide, span};
        lanes::Win16Target ours_target{s.image, bseg, boff, !far, wide, std::nullopt};

        // The original's side of a run: what it did, and the path its own jumps took (the explorer's seeds).
        struct Original {
            GatePrint print;
            concolic::Trail trail;
        };
        std::map<std::size_t, concolic::Trail> trails;
        std::shared_ptr<concolic::Exploration<Vector16>> exploration;
        std::vector<std::pair<Vector16, emulate::Run>> explored_runs;

        const auto original_run = [&](const Vector16& input, const Bytes& dimg, const emulate::Run* ran) -> Original {
            Vector16 v = input;
            v.context = side_context(input.context, true);
            lanes::Win16Side side;
            side.data_image = dimg;
            emulate::Run r;
            if (ran != nullptr) r = *ran;
            else r = runner.recorded(s.original, lanes::win16_run_options(base_orig, original_target, v, side));
            Original out;
            out.print = gate_print(r, wide, data.size(), places_orig);
            out.trail = jumps.trail(span, r.path);
            return out;
        };
        const auto ours_run = [&](const Vector16& input, const Bytes& dimg) -> GatePrint {
            Vector16 v = input;
            v.context = side_context(input.context, false);
            lanes::Win16Side side;
            side.data_image = !dimg.empty() ? dimg : data;
            const emulate::Run r = runner.recorded(s.image, lanes::win16_run_options(base_ours, ours_target, v, side));
            return gate_print(r, wide, data.size(), places_ours);
        };

        // One input beside the other: nullopt to go on, else the verdict that ends it.
        const auto judge_input = [&](const Vector16& v, bool is_seed, std::size_t seed_index) -> std::optional<Isolated16Answer> {
            Original fa;
            GatePrint fb;
            try {
                // (a call recorded in the running system keeps its data segment as it was then; a noisy input runs over the patterned data)
                static const Bytes kNone;
                const Bytes& dimg = v.noisy ? noisy : kNone;
                const emulate::Run* ran = nullptr;
                if (v.guided)
                    for (const auto& [made, record] : explored_runs)
                        if (iso_same_input(made, v)) {
                            ran = &record;
                            break;
                        }
                fa = original_run(v, dimg, ran);
                if (is_seed) trails[seed_index] = fa.trail;
                fb = ours_run(v, dimg);
            } catch (const NotFoundError&) {
                throw;
            } catch (const Error& e) {   // isolation boundary: the emulator itself fell over; the function goes on to its next input
                if (why.empty()) why = std::format("the emulator: {}", e.kind());
                return std::nullopt;
            }
            if (fa.print.exit == "fault" && fb.exit != "fault") {
                // An input the original faults on (a selector it loads at once, `lds si, p`, that no caller would pass) is no behavior to match: its C, loading
                // a selector only to use it, went on. What the original did before it faulted must still be done.
                const std::vector<std::string>& x = fa.print.calls;
                const std::vector<std::string>& y = fb.calls;
                if (!std::equal(x.begin(), x.end(), y.begin(), y.begin() + static_cast<std::ptrdiff_t>(std::min(x.size(), y.size()))) || y.size() < x.size()) {
                    std::vector<std::string> x2(x.begin(), x.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(2, x.size())));
                    std::vector<std::string> y2(y.begin(), y.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(2, y.size())));
                    return Isolated16Answer{name, Verdict::Divergent, std::format("{}: calls: original {}, ours {}", gate_input_text(v), iso_list(x2, 0), iso_list(y2, 0))};
                }
                return std::nullopt;
            }
            if (const std::optional<std::string> d = gate_difference(fa.print, fb)) return Isolated16Answer{name, Verdict::Divergent, gate_input_text(v) + ": " + *d};
            return std::nullopt;
        };

        std::optional<Isolated16Answer> verdict;
        if (s.given) {
            for (const Vector16& v : *s.given)
                if ((verdict = judge_input(v, false, 0))) break;
        } else {
            for (std::size_t i = 0; i < seeds.size() && !verdict; ++i) verdict = judge_input(seeds[i], true, i);
            for (std::size_t i = 0; i < noisy_runs.size() && !verdict; ++i) verdict = judge_input(noisy_runs[i], false, 0);
            // (and the guided ones the judge runs: a branch no fixed or aimed input takes -- a global tested before anything is done, a Windows call's result --
            // hid the path behind it. Looked for only once the others all agree: a function found to differ needs no more)
            if (!verdict && lane_common::guided_on()) {
                try {
                    lanes::Win16Explored got = lanes::win16_explore(runner, s.spec.lifters, base_orig, original_target, seeds, trails);
                    exploration = got.exploration;
                    explored_runs = std::move(got.made);
                    for (Vector16& g : got.inputs) {
                        g.guided = true;
                        if (!verdict) verdict = judge_input(g, false, 0);
                    }
                } catch (const NotFoundError&) {
                    throw;
                } catch (const Error& e) {   // isolation boundary: the explorer fell over; what the fixed inputs said stands, with the reason
                    if (why.empty()) why = std::format("the emulator: {}", e.kind());
                }
            }
        }
        if (verdict) {
            answers.push_back(*verdict);
            continue;
        }
        // (and, when it passes, how much of the original that was: the paths of its own jumps the runs took -- all there are, when the explorer found no more)
        std::string seen;
        if (exploration && !s.given) seen = std::format("paths {}{}", exploration->paths, exploration->complete ? " (all)" : "");
        answers.push_back({name, why.empty() ? Verdict::Equivalent : Verdict::Inconclusive, why.empty() ? seen : why});
    }
    return answers;
}

}  // namespace

std::vector<Isolated16Answer> Isolated16::check(const stdfs::path& original, const stdfs::path& folder, const std::vector<std::string>& names,
                                                const Isolated16Options& options) const {
    IsolatedTables tables;
    if (options.tables) {
        tables = *options.tables;
    } else {
        if (!spec_.tables) throw UsageError("the 16-bit gate is not given the tables of the module");
        tables = spec_.tables(original);
    }
    const std::map<std::string, core16::SegOff> ours = built_places(folder);
    stdfs::path image;
    if (options.image) {
        image = *options.image;
    } else {
        const std::string want = text::lower(fs::to_text(original.filename()));
        std::error_code ec;
        for (const auto& entry : stdfs::directory_iterator(folder, ec))
            if (text::lower(fs::to_text(entry.path().filename())) == want) image = entry.path();
        if (image.empty()) throw NotFoundError(folder / original.filename(), 0);
    }
    const IsoShare share{spec_, original, image, tables, ours, options.inputs, options.chain};
    const int workers = std::max(1, options.workers > 0 ? options.workers : jobs::default_jobs());
    std::map<std::string, Isolated16Answer> by_name;
    if (workers > 1 && names.size() > static_cast<std::size_t>(2 * workers) && !options.inputs && !options.chain) {
        // (shares of a few functions each, `workers` running at once, the next started as one ends: one function can take a minute and another a second, and
        // equal shares made at the start had the others idle while the slowest went on -- USER, 646 functions)
        const std::size_t count = std::min<std::size_t>(names.size(), static_cast<std::size_t>(workers) * 4);
        std::vector<std::vector<std::string>> shares(count);
        for (std::size_t k = 0; k < names.size(); ++k) shares[k % count].push_back(names[k]);
        const auto done = jobs::threads(shares, [&](const std::vector<std::string>& part) { return iso_run_share(share, part); }, workers);
        for (const std::vector<Isolated16Answer>& part : done)
            for (const Isolated16Answer& a : part) by_name[a.name] = a;
    } else {
        for (const Isolated16Answer& a : iso_run_share(share, names)) by_name[a.name] = a;
    }
    std::vector<Isolated16Answer> out;
    for (const std::string& n : names) out.push_back(by_name.at(n));
    return out;
}

}  // namespace cb::verify::gate16
