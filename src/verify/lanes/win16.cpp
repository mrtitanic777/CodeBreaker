#include "verify/lanes/win16.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "base/cache.hpp"
#include "core/capstone_api.hpp"
#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "emulate/native_shadow.hpp"
#include "platforms/win16.hpp"
#include "translate/backends/c.hpp"

namespace cb::verify::lanes {

namespace {

namespace w16 = emulate::windows16;
namespace stdfs = std::filesystem;

constexpr std::size_t kW16ArgWords = platforms::kWin16ArgumentWords;
constexpr std::uint16_t kW16Safe = w16::kSafeSelector;

std::int64_t w16_signed(std::uint64_t v) { return static_cast<std::int64_t>(v); }

// ---- the scattered inputs ---------------------------------------------------------------------------------------------------

// The small generator the four scattered inputs draw from (SplitMix64, seeded with 0x16: the same on every run and machine).
class W16Rng {
public:
    explicit W16Rng(std::uint64_t seed) : state_(seed) {}
    std::uint64_t next() {
        std::uint64_t z = (state_ += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }
    std::uint64_t below(std::uint64_t n) { return next() % n; }

private:
    std::uint64_t state_;
};

Vector16 w16_input(const std::vector<std::uint64_t>& words) {
    Vector16 v;
    for (std::uint64_t w : words) v.args.push_back(w16_signed(w));
    return v;
}

// ---- the explorer's view of an input ------------------------------------------------------------------------------------------

// The explorer reads an input's arguments as words rising up the stack from `args_at`; the tracer pushes them in Pascal order, the first deepest. So to
// the explorer an input's words are the other way round, and its memory is linear addresses of the original's run -- in the data segment (offset there in
// whichever build), or the scratch segment pointers go to (at one place in every build); a place elsewhere is not something an input sets.
guided::Input w16_to_explorer(const Vector16& v, std::optional<std::uint32_t> ds) {
    guided::Input g;
    for (auto it = v.args.rbegin(); it != v.args.rend(); ++it) g.args.push_back(static_cast<std::uint64_t>(*it));
    if (!v.mem.empty()) {
        std::vector<guided::MemInit> blocks;
        for (const w16::MemoryInit& m : v.mem) {
            if (const auto* at = std::get_if<std::uint32_t>(&m.where)) blocks.push_back({*at, m.data});
            else if (ds) blocks.push_back({static_cast<std::uint64_t>(*ds) + std::get<w16::DataOffset>(m.where).offset, m.data});
            else blocks.push_back({std::get<w16::DataOffset>(m.where).offset, m.data});
        }
        g.mem_init = std::move(blocks);
    }
    if (v.stub_ret) g.stub_ret = static_cast<std::uint64_t>(*v.stub_ret);
    return g;
}

Vector16 w16_from_explorer(const guided::Input& g, std::optional<std::uint32_t> ds) {
    Vector16 v;
    for (auto it = g.args.rbegin(); it != g.args.rend(); ++it) v.args.push_back(static_cast<std::int64_t>(*it & 0xFFFF));
    if (g.mem_init) {
        for (const guided::MemInit& m : *g.mem_init) {
            if (ds && *ds <= m.address && m.address < static_cast<std::uint64_t>(*ds) + 0x10000)
                v.mem.push_back({w16::DataOffset{static_cast<std::uint32_t>(m.address - *ds)}, m.data});
            else if (w16::kScratchBase <= m.address && m.address < static_cast<std::uint64_t>(w16::kScratchBase) + 0x10000)
                v.mem.push_back({static_cast<std::uint32_t>(m.address), m.data});
        }
    }
    if (g.stub_ret) v.stub_ret = static_cast<std::int64_t>(*g.stub_ret & 0xFFFFFFFF);
    return v;
}

guided::CompareOp w16_compare_op(emulate::native_trace::CompareOp op) {
    return guided::parse_compare_op(emulate::native_trace::to_text(op));
}

// The original's run as the explorer reads it: its branches (and what each compared), every address that ran, where the arguments are.
guided::Observation w16_observed(const w16::Trace16& r, const Vector16& v) {
    guided::Observation o;
    if (r.branches) {
        for (const emulate::native_trace::BranchEntry& b : *r.branches) {
            guided::Branch branch;
            branch.address = b.address;
            branch.taken = b.taken;
            if (b.compare) {
                guided::Compare c;
                c.op = w16_compare_op(b.compare->op);
                c.size = static_cast<unsigned>(b.compare->size);
                c.a = b.compare->a;
                c.b = b.compare->b;
                c.a_at = b.compare->at_a;
                c.b_at = b.compare->at_b;
                branch.compare = c;
            }
            o.branches.push_back(std::move(branch));
        }
    }
    o.reach.insert(r.observed.path.begin(), r.observed.path.end());
    o.args_at = static_cast<std::uint64_t>(w16::kStackBase) + w16::kFarReturn - 2 * v.args.size();
    o.stack_low = w16::kStackBase;
    o.stack_high = static_cast<std::uint64_t>(w16::kStackBase) + 0x10000;
    return o;
}

concolic::PathEntry w16_path_entry(const emulate::native_shadow::PathEntry& e) {
    concolic::PathEntry p;
    p.site = w16_signed(e.site);
    p.taken = e.taken;
    p.cond = e.condition;
    switch (e.kind) {
    case emulate::native_shadow::PathKind::Jump: p.kind = concolic::Kind::Jump; break;
    case emulate::native_shadow::PathKind::Inner: p.kind = concolic::Kind::Inner; break;
    case emulate::native_shadow::PathKind::Pin: p.kind = concolic::Kind::Pin; break;
    }
    return p;
}

// The module's data segment's base (the NE header's autodata segment), when it has one.
std::optional<std::uint32_t> w16_data_base(const stdfs::path& module) {
    const formats::ne::Module ne = formats::ne::Module::open_file(module);
    const unsigned auto_seg = ne.header().autodata_segment;
    if (auto_seg == 0) return std::nullopt;
    return w16::segment_base(auto_seg);
}

// Input `v` with the named inputs (the shadow's variables) set to `values`: an argument word, what an unmodelled call returns, bytes of the data segment
// or of the scratch segment.
std::optional<Vector16> w16_derive(const Vector16& v, const concolic::Values& values, const concolic::Vars& names, std::optional<std::uint32_t> ds) {
    Vector16 out = v;
    std::map<std::uint64_t, std::uint8_t> mem;   // by linear address, sorted: the runs below are made of neighbours
    for (const w16::MemoryInit& m : v.mem) {
        std::uint64_t at = 0;
        if (const auto* linear = std::get_if<std::uint32_t>(&m.where)) {
            at = *linear;
        } else {
            if (!ds) continue;
            at = static_cast<std::uint64_t>(*ds) + std::get<w16::DataOffset>(m.where).offset;
        }
        for (std::size_t k = 0; k < m.data.size(); ++k) mem[at + k] = m.data[k];
    }
    bool changed = false;
    for (const auto& [name, value] : values) {
        const auto known = names.find(name);
        if (known == names.end()) continue;
        const concolic::InputVar& var = known->second;
        using Place = concolic::InputVar::Place;
        if (var.place == Place::Arg && var.where >= 0 && static_cast<std::size_t>(var.where) < out.args.size()) {
            std::int64_t& word = out.args[static_cast<std::size_t>(var.where)];
            changed |= word != static_cast<std::int64_t>(value & 0xFFFF);
            word = static_cast<std::int64_t>(value & 0xFFFF);
        } else if (var.place == Place::Ret) {
            changed |= static_cast<std::uint64_t>(v.stub_ret.value_or(0)) != value;
            out.stub_ret = static_cast<std::int64_t>(value & 0xFFFFFFFF);
        } else if (var.place == Place::Mem) {
            const std::uint64_t at = static_cast<std::uint64_t>(var.where);
            const auto have = mem.find(at);
            const std::uint64_t then = have != mem.end() ? have->second : static_cast<std::uint64_t>(var.value);
            changed |= then != (value & 0xFF);
            mem[at] = static_cast<std::uint8_t>(value & 0xFF);
        }
    }
    if (!changed) return std::nullopt;
    out.mem.clear();
    for (auto it = mem.begin(); it != mem.end();) {
        const std::uint64_t at = it->first;
        Bytes run;
        std::uint64_t next = at;
        for (; it != mem.end() && it->first == next; ++it, ++next) run.push_back(it->second);
        if (ds && *ds <= at && at < static_cast<std::uint64_t>(*ds) + 0x10000)
            out.mem.push_back({w16::DataOffset{static_cast<std::uint32_t>(at - *ds)}, std::move(run)});
        else if (w16::kScratchBase <= at && at < static_cast<std::uint64_t>(w16::kScratchBase) + 0x10000)
            out.mem.push_back({static_cast<std::uint32_t>(at), std::move(run)});
    }
    return out;
}

// The words a frame helper's C is entered with: a far pointer into the stack segment to each place, the last parameter deepest (its callers push them so).
std::vector<std::int64_t> w16_frame_arguments(const Frame16& frame) {
    std::vector<std::int64_t> words;
    for (auto it = frame.rbegin(); it != frame.rend(); ++it) {
        words.push_back(w16::kStackSelector);
        words.push_back((kWin16CallerBp - it->first) & 0xFFFF);
    }
    return words;
}

// "name(a,b)" or "name" as lane_common writes a recorded call.
std::string w16_hex(ByteView bytes) {
    std::string s;
    for (std::uint8_t b : bytes) s += std::format("{:02x}", b);
    return s;
}

}  // namespace

// ---- inputs -----------------------------------------------------------------------------------------------------------------------

std::vector<std::uint16_t> Vector16::words() const {
    std::vector<std::uint16_t> out;
    for (std::int64_t a : args) out.push_back(static_cast<std::uint16_t>(a));
    return out;
}

std::vector<Vector16> win16_vectors() {
    const std::size_t n = kW16ArgWords;
    const std::uint64_t sel = kW16Safe;
    std::vector<Vector16> out = {
        w16_input(std::vector<std::uint64_t>(n, 0)),
        w16_input(std::vector<std::uint64_t>(n, 0x0400)),
        w16_input(std::vector<std::uint64_t>(n, 0x0080)),
        w16_input(lane_common::counting(n)),
        w16_input(lane_common::counting(n, 0x100, 0x100)),
        w16_input(lane_common::descending(n, 0xFFFF)),
        w16_input(lane_common::alternating(n, lane_common::Pattern(std::function<std::uint64_t(std::size_t)>([](std::size_t i) { return 0x20 * (i + 1); })),
                                           lane_common::Pattern(sel))),   // offset, selector, ...
        w16_input(lane_common::alternating(n, lane_common::Pattern(sel),
                                           lane_common::Pattern(std::function<std::uint64_t(std::size_t)>([](std::size_t i) { return 0x20 * (i + 1); })))),   // selector, offset, ...
        w16_input(std::vector<std::uint64_t>(n, sel)),
        w16_input({1, 0x40, sel, 2, 0x80, sel, 3, 0x100}),
    };
    // Four more, every word a different one spread over the segment: with the same word in each argument, or small ones pointing at the quiet start of the
    // data segment, a function reading the wrong argument or the wrong field read what it should have.
    W16Rng rng(0x16);
    for (int k = 0; k < 4; ++k) {
        std::vector<std::uint64_t> words;
        for (std::size_t i = 0; i < n; ++i) words.push_back((0x0100 + rng.below(0x7F00 - 0x0100)) & ~std::uint64_t{1});
        if (k % 2 != 0)   // (and as far pointers: offset, selector)
            for (std::size_t i = 0; i < n; ++i)
                if (i % 2 != 0) words[i] = sel;
        out.push_back(w16_input(words));
    }
    return out;
}

std::vector<Vector16> win16_directed(const formats::ne::Module& module, std::uint32_t segment, CodeSpan span, std::size_t most) {
    const std::vector<formats::ne::Segment> segments = module.segments();
    if (segment == 0 || segment > segments.size()) throw FormatError("win16 lane", 0, std::format("the module has no segment {}", segment));
    const formats::ne::Segment& s = segments[segment - 1];
    const ByteView file = module.data();
    if (s.file_offset >= file.size()) return {};
    const ByteView code = file.subspan(s.file_offset, std::min<std::uint64_t>(s.length, file.size() - s.file_offset));
    if (span.start >= code.size()) return {};
    const ByteView body = code.subspan(span.start, std::min<std::size_t>(span.end > span.start ? span.end - span.start : 0, code.size() - span.start));
    // (a switch written as a chain of subtractions -- `dec ax / dec ax / sub ax, 4 / sub ax, 9 / sub ax, 72h`, each case's `je` after its step: a window
    // procedure's messages 1, 2, 6, 0Fh, 81h -- tests the running total, not each step's number: the totals are the cases)
    std::vector<std::uint32_t> seen;
    auto note = [&](std::uint32_t c) {
        if (c != 0 && c != 0xFFFF && std::find(seen.begin(), seen.end(), c) == seen.end()) seen.push_back(c);
    };
    std::map<unsigned, std::uint32_t> total;   // by register: lookup only
    platforms::DecoderSet decoders;
    decoders.get(platforms::Arch::X86, 16, true).each(body, span.start, 0, [&](const cs_insn& ins) {
        const std::string mnemonic = ins.mnemonic;
        const cs_x86& x = ins.detail->x86;
        const bool reg_first = x.op_count > 0 && x.operands[0].type == X86_OP_REG;
        const unsigned reg = reg_first ? static_cast<unsigned>(x.operands[0].reg) : 0;
        const bool counts = mnemonic == "dec" || mnemonic == "inc" || mnemonic == "sub" || mnemonic == "add";
        if (reg_first && counts && (mnemonic == "dec" || mnemonic == "inc" || (x.op_count > 1 && x.operands[1].type == X86_OP_IMM))) {
            std::int64_t step = (mnemonic == "dec" || mnemonic == "inc") ? 1 : x.operands[1].imm;
            step = (mnemonic == "dec" || mnemonic == "sub") ? step : -step;
            std::uint32_t& sum = total[reg];
            sum = static_cast<std::uint32_t>(static_cast<std::int64_t>(sum) + step) & 0xFFFF;
            note(sum);
            return true;
        }
        if (reg_first && mnemonic != "cmp" && mnemonic != "test" && mnemonic != "or") total.erase(reg);   // (the register set anew: a chain ends)
        if (mnemonic != "cmp" && mnemonic != "test" && mnemonic != "and") return true;
        for (int i = 0; i < x.op_count; ++i)
            if (x.operands[i].type == X86_OP_IMM) note(static_cast<std::uint32_t>(x.operands[i].imm) & 0xFFFF);
        return true;
    });
    std::vector<Vector16> out;
    const std::uint64_t sel = kW16Safe;
    const std::size_t take = std::max<std::size_t>(most / 2, 24);
    for (std::size_t i = 0; i < seen.size() && i < take; ++i) {
        const std::uint64_t c = seen[i];
        out.push_back(w16_input(std::vector<std::uint64_t>(kW16ArgWords, c)));
        out.push_back(w16_input(lane_common::alternating(kW16ArgWords, lane_common::Pattern(c), lane_common::Pattern(sel))));
    }
    return out;
}

std::optional<Frame16> win16_frame_slots(std::string_view text, std::string_view name) {
    // The head of the function: everything before its body.
    const std::string_view head = text.substr(0, text.find('{'));
    // `name(frame_XX,frame_YY)` -- the parameters, each a place in the caller's frame.
    std::size_t at = std::string_view::npos;
    for (std::size_t from = head.find(name); from != std::string_view::npos; from = head.find(name, from + 1)) {
        const bool word_start = from == 0 || !(std::isalnum(static_cast<unsigned char>(head[from - 1])) || head[from - 1] == '_');
        if (word_start && from + name.size() < head.size() && head[from + name.size()] == '(') {
            at = from + name.size() + 1;
            break;
        }
    }
    if (at == std::string_view::npos) return std::nullopt;
    const std::size_t close = head.find(')', at);
    if (close == std::string_view::npos) return std::nullopt;
    const std::string_view params = head.substr(at, close - at);
    if (params.empty()) return std::nullopt;
    // `type __far *frame_XX;` -- what each is a pointer to.
    static const Regex typed(R"((\w+) __far \*frame_([0-9a-f]+);)");
    std::map<std::uint32_t, std::string> types;
    typed.for_each_match(head, [&](const RegexMatch& m) {
        if (const auto less = text::parse_int<std::uint32_t>(m.group(2), 16)) types[*less] = std::string(m.group(1));
        return true;
    });
    Frame16 out;
    for (std::string_view p : text::split(params, ',')) {
        if (!p.starts_with("frame_") && !p.empty()) return std::nullopt;
        if (p.empty()) continue;   // (a trailing comma)
        const auto less = text::parse_int<std::uint32_t>(p.substr(6), 16);
        if (!less) return std::nullopt;
        const auto ty = types.find(*less);
        if (ty == types.end()) return std::nullopt;
        const std::optional<std::size_t> bytes = translate::backends::c::scalar_size(ty->second);
        out.emplace_back(*less, static_cast<std::uint32_t>(bytes.value_or(2)));
    }
    return out;
}

// ---- the jumps of a code segment ---------------------------------------------------------------------------------------------------

JumpTable::JumpTable(Bytes segment_code, std::uint32_t segment) : code_(std::move(segment_code)), segment_(segment) {}

concolic::Trail JumpTable::trail(CodeSpan span, std::span<const std::uint64_t> path) {
    const std::uint64_t base = w16::segment_base(segment_);
    const std::uint64_t lo = base + span.start, hi = base + span.end;
    concolic::Trail out;
    for (std::size_t i = 0; i + 1 < path.size(); ++i) {
        const std::uint64_t a = path[i];
        if (a < lo || a >= hi) continue;
        const std::uint32_t off = static_cast<std::uint32_t>(a - base);
        auto known = known_.find(off);
        if (known == known_.end()) {
            std::optional<Jump> jump;
            if (off < code_.size()) {
                const ByteView window = ByteView(code_).subspan(off, std::min<std::size_t>(8, code_.size() - off));
                const std::vector<platforms::Insn> decoded = decoders_.get(platforms::Arch::X86, 16).disasm(window, off, 1);
                if (!decoded.empty()) {
                    const platforms::Insn& ins = decoded.front();
                    const bool conditional = (ins.mnemonic.starts_with('j') && ins.mnemonic != "jmp") || ins.mnemonic.starts_with("loop");
                    if (conditional && ins.op_str.starts_with("0x")) {
                        if (const auto target = text::parse_int<std::uint32_t>(ins.op_str, 16)) jump = Jump{*target, off + ins.size};
                    }
                }
            }
            known = known_.emplace(off, jump).first;
        }
        const std::optional<Jump>& j = known->second;
        const std::uint64_t then = path[i + 1] - base;
        if (j && j->target != j->next && (then == j->target || then == j->next)) out.push_back({w16_signed(a), then == j->target});
    }
    return out;
}

// ---- what a run is asked, and what it did -------------------------------------------------------------------------------------------

w16::ProbeOptions win16_judge_base() {
    w16::ProbeOptions o;
    o.ports = true;
    return o;
}

w16::ProbeOptions win16_run_options(const w16::ProbeOptions& base, const Win16Target& target, const Vector16& v, const Win16Side& side) {
    w16::ProbeOptions o = base;
    o.function = w16::CodePoint{target.segment, target.offset};
    o.args = v.words();
    o.near_entry = target.near_entry;
    o.data_image = !side.data_image.empty() ? side.data_image : base.data_image;
    o.stub_ret = v.stub_ret ? std::optional<std::uint32_t>(static_cast<std::uint32_t>(*v.stub_ret)) : std::nullopt;
    o.mem_init = v.mem;
    if (v.context) o.context = v.context.get();
    if (side.steps) o.insn_cap = *side.steps;
    if (side.frame) {   // (a caller-frame helper: see kWin16CallerBp)
        o.bp = kWin16CallerBp;
        for (const auto& [less, size] : *side.frame)
            o.mem_init.push_back({static_cast<std::uint32_t>(w16::kStackBase + ((kWin16CallerBp - less) & 0xFFFF)), Bytes(size, kWin16FrameFill)});
    }
    return o;
}

std::string win16_call_text(const emulate::ImportHit& hit, std::span<const std::int64_t> inputs, const w16::EntryRegisters& entry,
                            const std::map<std::uint16_t, std::string>& segs) {
    const std::string name = hit.api.empty() ? "?" : hit.api;
    if (!hit.arg_bytes) return lane_common::call(name);
    const std::size_t count = std::min<std::size_t>(hit.args_stack.size(), static_cast<std::size_t>(*hit.arg_bytes) / 2);
    std::vector<std::string> out;
    for (std::size_t i = 0; i < count; ++i) {
        const std::int64_t w = hit.args_stack[i];
        const std::optional<std::int64_t> next = i + 1 < count ? std::optional<std::int64_t>(hit.args_stack[i + 1]) : std::nullopt;
        const Bytes* pointed = nullptr;
        for (const auto& [place, bytes] : hit.pointed)
            if (place == i) pointed = &bytes;
        const auto in_segs = [&](std::int64_t selector) { return segs.find(static_cast<std::uint16_t>(selector & ~std::int64_t{3})); };
        if (w == entry.ds) {
            out.push_back("DS");
        } else if (!segs.empty() && in_segs(w) != segs.end()) {
            out.push_back(in_segs(w)->second);
        } else if (!segs.empty() && next && in_segs(*next) != segs.end() && pointed != nullptr) {
            out.push_back("&" + w16_hex(*pointed));   // (a far pointer into a code segment: what is there)
        } else if (w == entry.ss) {
            out.push_back("SS");
        } else if (next && *next == entry.ss) {
            out.push_back("stack");   // offset part of a far pointer into the stack
        } else {
            lane_common::ArgumentContext context;
            context.inputs.assign(inputs.begin(), inputs.end());
            context.above = 0xFF;
            out.push_back(lane_common::argument(w, context));
        }
    }
    return lane_common::call(name, out);
}

Win16Print win16_fingerprint(w16::Runner& runner, const w16::ProbeOptions& base, const Win16Target& target, const Vector16& v, const Win16Side& side,
                             const emulate::Run* ran) {
    emulate::Run r;
    if (ran != nullptr) {
        r = *ran;
    } else {
        try {
            r = runner.recorded(target.module, win16_run_options(base, target, v, side));
        } catch (const NotFoundError&) {
            throw;   // a module that is missing is not an observation
        } catch (const Error& e) {   // isolation boundary: a run that cannot be made is a run that was not an observation
            Win16Print bad;
            bad.fp.err = std::string(e.kind());
            return bad;
        }
    }
    const auto trace_ptr = std::any_cast<std::shared_ptr<const w16::Trace16>>(&r.raw);
    if (trace_ptr == nullptr || !*trace_ptr) throw InternalError("a 16-bit run without its record");
    const w16::Trace16& t = **trace_ptr;
    const std::string ex = r.ended();
    const std::optional<std::uint32_t> ds = t.layout.data_base;
    std::map<std::uint64_t, std::pair<std::string, std::int64_t>> frame_places;   // a frame helper's places in the stack: byte -> its own
    if (side.frame)
        for (const auto& [less, size] : *side.frame)
            for (std::uint32_t i = 0; i < size; ++i)
                frame_places[w16::kStackBase + ((kWin16CallerBp - less + i) & 0xFFFF)] = {"F", -static_cast<std::int64_t>(less) + i};
    lane_common::ChangedOptions where;
    where.most = 4;
    where.everything = false;   // a byte left holding what it held on entry is not an effect (one compiler clears a flag with a word write, another with a byte write)
    where.place = [&](std::uint64_t a) -> lane_common::Where {
        if (!frame_places.empty()) {
            bool any = false;
            for (std::uint32_t i = 0; i < 4; ++i) any |= frame_places.contains(a + i);
            if (any)
                return lane_common::ByteNamer([&frame_places, a](std::size_t i) -> std::optional<lane_common::Place> {
                    const auto it = frame_places.find(a + i);
                    if (it == frame_places.end()) return std::nullopt;
                    return lane_common::Place(lane_common::TaggedPlace{it->second.first, it->second.second});
                });
        }
        if (ds && *ds <= a && a < static_cast<std::uint64_t>(*ds) + 0x10000) {
            const std::uint64_t offset = a - *ds;
            for (const w16::OffsetRange& range : side.own)
                if (range.lo <= offset && offset < range.hi) return lane_common::Skip{};
            return lane_common::Place(lane_common::TaggedPlace{"D", static_cast<std::int64_t>(offset)});
        }
        if (w16::kHeapBase <= a && a < static_cast<std::uint64_t>(w16::kHeapBase) + 64 * w16::kHeapStride)
            return lane_common::Place(lane_common::TaggedPlace{"H", static_cast<std::int64_t>(a - w16::kHeapBase)});
        return lane_common::Skip{};   // the stack, or somewhere wild
    };
    const std::optional<std::string> writes = lane_common::left(r, where);
    const CodeSpan span = target.span.value_or(CodeSpan{target.offset, target.offset});
    const std::uint64_t code_base = t.code_base;
    std::set<std::int64_t> covered;
    for (std::uint64_t a : r.path)
        if (a >= code_base + span.start && a < code_base + span.end) covered.insert(w16_signed(a - code_base - span.start));

    lane_common::FingerprintParts parts;
    if (ex == "ret" && target.wide) {   // nullopt: the function returns nothing
        const auto reg = [&](const char* name) -> std::uint64_t {
            const auto* got = r.results.find(name);
            if (got == nullptr) return 0;
            const auto* n = std::get_if<std::uint64_t>(got);
            return n != nullptr ? *n : 0;
        };
        std::int64_t ret = static_cast<std::int64_t>(*target.wide ? reg("DX:AX") : reg("AX"));
        // An exported function starts `push ds / pop ax`; one that sets no result returns with that still in AX. Its own data selector is a leftover, not a result.
        if (t.entry_regs.ds != 0 && (ret & 0xFFFF) == t.entry_regs.ds && !*target.wide) ret = kWin16Leftover;
        parts.ret = ret;
    }
    parts.writes = writes;
    static const std::array<std::string, 3> kPreserved = {"SI", "DI", "DS"};
    parts.saved = r.kept(kPreserved);
    parts.cov.assign(covered.begin(), covered.end());
    // The run's effects outside itself, in the order it had them: each Windows call and each access to a device (`out(3fb,83)`, `in(3fd)`). What a function
    // did to the UART is what it is for: a rebuild writing another divisor, or reading the line status of another port, did something else.
    struct Effect {
        std::uint64_t step = 0;
        int order = 0;
        std::size_t index = 0;
        std::string text;
    };
    std::vector<Effect> effects;
    for (std::size_t k = 0; k < r.calls.size(); ++k) {
        const auto* hit = std::get_if<emulate::ImportHit>(&r.calls[k].detail);
        if (hit == nullptr) continue;
        effects.push_back({static_cast<std::uint64_t>(hit->n), 0, k, win16_call_text(*hit, v.args, t.entry_regs, side.segs)});
    }
    for (std::size_t k = 0; k < r.io.size(); ++k) {
        const emulate::IoAccess& io = r.io[k];
        const std::string text = io.kind == "..." ? "..." : io.kind == "in" ? std::format("in({:x})", io.port) : std::format("out({:x},{:x})", io.port, io.value);
        effects.push_back({io.step.value_or(0), 1, k, text});
    }
    std::sort(effects.begin(), effects.end(), [](const Effect& a, const Effect& b) {
        return std::tie(a.step, a.order, a.index, a.text) < std::tie(b.step, b.order, b.index, b.text);
    });
    std::vector<std::string> deps;
    for (Effect& e : effects) deps.push_back(std::move(e.text));
    Win16Print out;
    out.fp = lane_common::fingerprint(r, std::move(deps), std::move(parts));
    if (target.span && side.jumps != nullptr) out.trail = side.jumps->trail(*target.span, r.path);
    return out;
}

// ---- the path explorer ------------------------------------------------------------------------------------------------------------------------

bool win16_concolic_ready(emulate::pcode::LifterSet* lifters) {
    return lifters != nullptr && lane_common::guided_on() && lane_common::concolic_on() && lifters->get(emulate::pcode::Mode::X86Real16) != nullptr;
}

Win16Explored win16_explore(w16::Runner& runner, emulate::pcode::LifterSet* lifters, const w16::ProbeOptions& base, const Win16Target& target,
                            std::span<const Vector16> seeds_in, const std::map<std::size_t, concolic::Trail>& known) {
    Win16Explored out;
    const CodeSpan span = target.span.value_or(CodeSpan{target.offset, target.offset});
    const std::optional<std::uint32_t> ds = w16_data_base(target.module);
    const Win16Side bare;
    // The original run on `v` with its branches logged, no shadow; nullopt when the run is not an observation. Every run is kept in `out.made`.
    auto traced = [&](const Vector16& v, emulate::native_shadow::Shadow* shadow) -> std::optional<w16::Trace16> {
        w16::ProbeOptions o = win16_run_options(base, target, v, bare);
        o.branches = w16::OffsetRange{span.start, span.end};
        o.shadow = shadow;
        try {
            return runner.probe(target.module, o);
        } catch (const NotFoundError&) {
            throw;
        } catch (const Error&) {   // isolation boundary: a run that cannot be made is no observation
            return std::nullopt;
        }
    };
    std::vector<Vector16> seeds(seeds_in.begin(), seeds_in.end());
    emulate::pcode::Lifter* lifter = win16_concolic_ready(lifters) ? lifters->get(emulate::pcode::Mode::X86Real16) : nullptr;

    // What the guessing explorer is given of a run.
    auto plain = [&](const guided::Input& g) -> std::optional<guided::Observation> {
        const Vector16 v = w16_from_explorer(g, ds);
        const std::optional<w16::Trace16> r = traced(v, nullptr);
        if (!r) return std::nullopt;
        out.made.emplace_back(v, w16::record(std::make_shared<const w16::Trace16>(*r)));
        return w16_observed(*r, v);
    };

    if (lifter == nullptr) {   // guessed: branch-guided inputs (verify/guided)
        std::vector<guided::Input> explorer_seeds;
        for (const Vector16& v : seeds) explorer_seeds.push_back(w16_to_explorer(v, ds));
        const guided::Run run = [&](const guided::Input& g) -> std::optional<guided::Observation> {
            const Vector16 v = w16_from_explorer(g, ds);
            const std::optional<w16::Trace16> r = traced(v, nullptr);
            if (!r) return std::nullopt;
            return w16_observed(*r, v);
        };
        for (const guided::Input& g : lane_common::guided(run, explorer_seeds, target.offset, {guided::ByteOrder::Little, 2})) out.inputs.push_back(w16_from_explorer(g, ds));
        return out;
    }

    const std::uint64_t base_address = w16::segment_base(target.segment);
    const std::uint64_t lo = base_address + span.start, hi = base_address + span.end;
    std::vector<std::pair<std::uint64_t, std::uint64_t>> inputs;
    if (ds) inputs.emplace_back(*ds, static_cast<std::uint64_t>(*ds) + 0x10000);
    inputs.emplace_back(w16::kScratchBase, static_cast<std::uint64_t>(w16::kScratchBase) + 0x10000);
    std::vector<std::shared_ptr<const emulate::native_shadow::Run>> kept_runs;   // the conditions' nodes live in these until the exploration ends

    const std::function<std::optional<concolic::Observation>(Vector16&)> run = [&](Vector16& v) -> std::optional<concolic::Observation> {
        emulate::native_shadow::Shadow shadow(emulate::native_shadow::Setup::for_lifter(emulate::native_shadow::Kind::Real16, *lifter, inputs,
                                                                                         std::pair<std::uint64_t, std::uint64_t>(lo, hi)));
        const std::optional<w16::Trace16> r = traced(v, &shadow);
        if (!r) return std::nullopt;
        const auto shared = std::make_shared<const w16::Trace16>(*r);
        out.made.emplace_back(v, w16::record(shared));
        const std::shared_ptr<const emulate::native_shadow::Run> made = shadow.run();
        kept_runs.push_back(made);
        concolic::Observation ob;
        for (const emulate::native_shadow::Step& s : made->trail) ob.trail.push_back({w16_signed(s.site), s.taken});
        for (const emulate::native_shadow::PathEntry& e : made->path) ob.path.push_back(w16_path_entry(e));
        for (const auto& [var_name, var] : made->vars) {
            using Kind = emulate::native_shadow::VarKind;
            concolic::InputVar iv;
            iv.bits = var.bits;
            iv.value = static_cast<std::int64_t>(var.value);
            if (var.kind == Kind::Arg && std::holds_alternative<std::uint64_t>(var.where)) {
                iv.place = concolic::InputVar::Place::Arg;
                iv.where = static_cast<std::int64_t>(std::get<std::uint64_t>(var.where));
            } else if (var.kind == Kind::Mem && std::holds_alternative<std::uint64_t>(var.where)) {
                iv.place = concolic::InputVar::Place::Mem;
                iv.where = static_cast<std::int64_t>(std::get<std::uint64_t>(var.where));
            } else if (var.kind == Kind::Ret) {
                iv.place = concolic::InputVar::Place::Ret;
            } else {
                continue;
            }
            ob.vars[var_name] = iv;
        }
        ob.lost = made->lost;
        ob.cut = r->observed.truncated || r->ending == w16::Ending::Truncated;
        if (ob.cut) ob.lost.insert("truncated");
        return ob;
    };
    const std::function<std::optional<Vector16>(const Vector16&, const concolic::Values&, const concolic::Vars&)> derive =
        [&](const Vector16& v, const concolic::Values& values, const concolic::Vars& names) { return w16_derive(v, values, names, ds); };
    const auto got = std::make_shared<concolic::Exploration<Vector16>>(lane_common::explored<Vector16>(
        run, std::span<Vector16>(seeds), derive,
        [lo, hi](std::int64_t site) { return static_cast<std::int64_t>(lo) <= site && site < static_cast<std::int64_t>(hi); }, known));
    out.inputs = got->kept;
    std::map<std::int64_t, int> ways;
    for (const concolic::Step& e : got->edges) ways[e.site] |= 1 << (e.taken ? 1 : 0);
    const bool one_way = std::any_of(ways.begin(), ways.end(), [](const auto& w) { return w.second != 3; });
    if (!got->complete && one_way && lane_common::kGuidedRest > 0 && !out.made.empty()) {
        // (a condition over what the shadow does not follow is no question for the solver, and guessing still reaches some of them)
        std::vector<guided::Seen> seen;
        for (const auto& [v, record] : out.made) {
            const auto trace = std::any_cast<std::shared_ptr<const w16::Trace16>>(&record.raw);
            if (trace != nullptr && *trace) seen.push_back({w16_to_explorer(v, ds), w16_observed(**trace, v)});
        }
        guided::Options options;
        options.budget = lane_common::kGuidedRest;
        options.keep = lane_common::kMaxGuided;
        options.seed = target.offset;
        options.machine = {guided::ByteOrder::Little, 2};
        options.observed = seen;
        const std::vector<guided::Input> extra = guided::guided_inputs(plain, {}, options);
        for (const guided::Input& g : extra) out.inputs.push_back(w16_from_explorer(g, ds));
        got->guessed = static_cast<int>(extra.size());
    }
    out.exploration = got;
    return out;
}

// ---- the lane --------------------------------------------------------------------------------------------------------------------------------------

struct Win16Lane::State {
    std::mutex lock;
};

Win16Lane::Win16Lane(Win16LaneSpec spec) : state_(std::make_shared<State>()), spec_(std::move(spec)) {}

const std::string& Win16Lane::name() const {
    static const std::string kName = "ne";
    return kName;
}

bool Win16Lane::concolic_ready() const { return win16_concolic_ready(spec_.lifters); }

std::string Win16Lane::key(std::uint32_t segment, std::uint32_t offset) { return std::format("{}:{:04x}", segment, offset); }

Profile Win16Lane::profile(const stdfs::path&) const {
    const bool ready = concolic_ready();
    std::string stamp;
    if (ready) {
        const emulate::pcode::PcodeConfig& config = spec_.lifters->config();
        stamp = base::cache::tool_id(config.exe) + "," + base::cache::tool_id(config.sla(emulate::pcode::Mode::X86Real16));
    }
    std::string inputs;
    for (const Vector16& v : win16_vectors()) {
        for (std::int64_t a : v.args) inputs += std::format("{:x} ", a);
        inputs += "| ";
    }
    const std::string settings = std::format("kind=ne judge={} comparer={} options={} inputs=[{}] guided={},{} paths={},{},{},{}", kRev, compare::kRevision,
                                             spec_.compiler_options, inputs, lane_common::kGuidedBudget, lane_common::kMaxGuided, ready,
                                             lane_common::kPathBudget, lane_common::kMaxPaths, stamp);
    const Bytes bytes(settings.begin(), settings.end());
    return {crypto::to_hex(crypto::sha1(bytes)).substr(0, 12), settings};
}

std::unique_ptr<Prepared> Win16Lane::prepare(const stdfs::path& module_path, const stdfs::path& work) const {
    if (!spec_.compilable) throw UsageError("the 16-bit lane has no way to make a module compilable");
    auto m = std::make_unique<PreparedWin16>();
    m->module = spec_.compilable(module_path, work);
    const decompile::c_text::Module16& module = *m->module;
    m->header = module.made.header();
    m->order = module.made.order();
    const OrderedMap<std::string, std::string> texts = module.made.texts();
    for (const auto& [name, text] : texts) m->texts[name] = text;
    for (const auto& [va, name] : module.made.by_va()) {
        const auto where = module.where.find(static_cast<std::int64_t>(va));
        if (where != module.where.end()) m->key[name] = key(where->second.seg, where->second.off);
    }
    for (const auto& [name, k] : m->key) m->all_keys.push_back(k);
    std::sort(m->all_keys.begin(), m->all_keys.end());
    std::string whole = m->header;
    for (const std::string& n : m->order)
        if (const auto text = texts.find(n)) whole += *text;
    const Bytes bytes(whole.begin(), whole.end());
    m->sig = crypto::to_hex(crypto::sha1(bytes));
    return m;
}

std::map<std::uint64_t, std::pair<CodeSpan, std::int64_t>> Win16Lane::spans(const formats::ne::Module& module,
                                                                           const std::map<std::uint64_t, core16::SegOff>& where) {
    const std::vector<formats::ne::Segment> segments = module.segments();
    std::map<std::uint32_t, std::vector<std::pair<std::uint32_t, std::uint64_t>>> by_segment;   // segment -> (offset, address), sorted below
    for (const auto& [va, at] : where) by_segment[at.seg].emplace_back(at.off, va);
    platforms::DecoderSet decoders;
    const platforms::Decoder& decoder = decoders.get(platforms::Arch::X86, 16);
    std::map<std::uint64_t, std::pair<CodeSpan, std::int64_t>> out;
    for (auto& [segment, list] : by_segment) {
        std::sort(list.begin(), list.end());
        if (segment == 0 || segment > segments.size()) continue;
        const formats::ne::Segment& s = segments[segment - 1];
        const ByteView file = module.data();
        ByteView code;
        if (s.file_offset < file.size()) code = file.subspan(s.file_offset, std::min<std::uint64_t>(s.length, file.size() - s.file_offset));
        for (std::size_t i = 0; i < list.size(); ++i) {
            const std::uint32_t off = list[i].first;
            const std::uint32_t end = i + 1 < list.size() ? list[i + 1].first : static_cast<std::uint32_t>(code.size());
            std::int64_t count = 0;
            if (off < code.size() && end > off) count = static_cast<std::int64_t>(decoder.disasm(code.subspan(off, std::min<std::size_t>(end - off, code.size() - off)), off).size());
            out[list[i].second] = {CodeSpan{off, end}, std::max<std::int64_t>(count, 1)};
        }
    }
    return out;
}

std::map<std::uint16_t, std::string> Win16Lane::named_segments(const std::vector<std::uint32_t>& original_segments,
                                                               const std::map<std::uint32_t, std::uint32_t>* order) {
    std::map<std::uint16_t, std::string> out;
    if (order != nullptr) {
        for (const auto& [mine, theirs] : *order) out[w16::selector(w16::kModuleSlot + mine - 1)] = std::format("seg{}", theirs);
    } else {
        for (std::uint32_t n : original_segments) out[w16::selector(w16::kModuleSlot + n - 1)] = std::format("seg{}", n);
    }
    return out;
}

std::map<int, Bytes> Win16Lane::code_images(const decompile::c_text::Module16& module, const formats::ne::Module& ne) {
    std::set<int> want;
    const auto env = fs::get_env("CB_CODE_DATA");
    if (env && *env == "0") {   // every segment whose C reads through CS
        const OrderedMap<std::string, std::string> texts = module.made.texts();
        for (const auto& [va, name] : module.made.by_va()) {
            const auto where = module.where.find(static_cast<std::int64_t>(va));
            const std::string* fn = texts.find(name);
            if (where != module.where.end() && fn != nullptr && fn->find("codebreaker_cs()") != std::string::npos) want.insert(static_cast<int>(where->second.seg));
        }
    } else {   // what c_text could not name
        for (const auto& [segment, report] : module.code_reads)
            if (!report.left.empty()) want.insert(static_cast<int>(segment));
    }
    const std::vector<formats::ne::Segment> segments = ne.segments();
    std::map<int, Bytes> out;
    for (int sg : want) {
        if (sg < 1 || static_cast<std::size_t>(sg) > segments.size()) continue;
        const formats::ne::Segment& s = segments[static_cast<std::size_t>(sg) - 1];
        if (s.file_offset == 0 || s.file_offset >= ne.data().size()) continue;
        const ByteView bytes = ne.data().subspan(s.file_offset, std::min<std::uint64_t>(s.length, ne.data().size() - s.file_offset));
        out[sg] = Bytes(bytes.begin(), bytes.end());
    }
    return out;
}

std::vector<w16::OffsetRange> Win16Lane::own_data(const stdfs::path& work) {
    constexpr std::uint32_t kInterruptRegisters = 20;   // sizeof(struct codebreaker_registers): eight words, CF, ZF, a word
    std::error_code ec;
    if (!stdfs::is_regular_file(work / "m.map", ec)) return {};
    const std::string map = fs::read_text(work / "m.map");
    static const Regex int_helper(R"(^[0-9A-Fa-f]{4}:([0-9A-Fa-f]{4,8})[+*]?\s+_codebreaker_int\s*$)", RegexFlags::Multiline);
    const auto m = int_helper.search(map);
    if (!m) return {};
    const auto at = text::parse_int<std::uint32_t>(m->group(1), 16);
    if (!at) return {};
    return {w16::OffsetRange{*at, *at + kInterruptRegisters}};
}

Built Win16Lane::build(const Prepared& prepared, const stdfs::path& module_path, const stdfs::path& work) const {
    const auto& m = static_cast<const PreparedWin16&>(prepared);
    const decompile::c_text::Module16& module = *m.module;
    if (!spec_.build_dll) throw UsageError("the 16-bit lane has no way to build the functions");
    const formats::ne::Module nef = formats::ne::Module::open_file(module_path);
    const Win16LaneSpec::Rebuilt rebuilt = spec_.build_dll(module, work, code_images(module, nef));
    Built built;
    std::map<std::string, std::string> dropped(rebuilt.dropped.begin(), rebuilt.dropped.end());
    // the original's data segment, as the file holds it
    Bytes data_image;
    if (const unsigned auto_seg = nef.header().autodata_segment; auto_seg != 0) {
        const std::vector<formats::ne::Segment> segments = nef.segments();
        if (auto_seg <= segments.size() && segments[auto_seg - 1].file_offset != 0 && segments[auto_seg - 1].file_offset < nef.data().size()) {
            const formats::ne::Segment& s = segments[auto_seg - 1];
            const ByteView bytes = nef.data().subspan(s.file_offset, std::min<std::uint64_t>(s.length, nef.data().size() - s.file_offset));
            data_image.assign(bytes.begin(), bytes.end());
        }
    }
    std::map<std::uint64_t, core16::SegOff> where;
    for (const auto& [va, name] : module.made.by_va()) {
        const auto at = module.where.find(static_cast<std::int64_t>(va));
        if (at != module.where.end()) where[va] = at->second;
    }
    const auto spans_of = spans(nef, where);
    auto items = std::make_shared<std::map<std::string, Item>>();
    const auto has_text = module.made.texts();
    for (const auto& [va, fn] : module.made.by_va()) {
        if (dropped.contains(fn)) continue;
        const auto placed = rebuilt.where.find(fn);
        if (placed == rebuilt.where.end()) {
            dropped[fn] = "not in the build";
            continue;
        }
        const auto at = where.find(va);
        const auto span = spans_of.find(va);
        if (at == where.end() || span == spans_of.end()) continue;
        Item item;
        item.key = key(at->second.seg, at->second.off);
        item.segment = at->second.seg;
        item.offset = at->second.off;
        item.near_entry = module.near.contains(fn);
        item.wide = module.novalue.contains(fn) ? std::nullopt : std::optional<bool>(module.wide.contains(fn));
        item.span = span->second.first;
        item.insns = span->second.second;
        item.built_segment = placed->second.segment;
        item.built_offset = placed->second.offset;
        if (const std::string* text = has_text.find(fn)) item.frame = win16_frame_slots(*text, fn);   // (a caller-frame helper)
        (*items)[fn] = std::move(item);
        built.items.push_back(fn);
    }
    auto context = std::make_shared<Context>();
    context->module = module_path;
    if (rebuilt.dll) context->dll = *rebuilt.dll;
    context->data_image = std::move(data_image);
    context->own = own_data(work);
    for (const std::string& fn : built.items) context->order.try_emplace(items->at(fn).built_segment, items->at(fn).segment);   // rebuilt code segment -> the original's
    built.judge = [this, items, context](const std::string& name) { return judge_function(*context, items->at(name)); };
    for (const auto& [fn, why] : dropped) built.missing[fn] = why;
    return built;
}

// ---- judging one function ----------------------------------------------------------------------------------------------------------------------------

store::Judgement Win16Lane::judge_function(const Context& c, const Item& item) const {
    using Said = lane_common::Judgement;
    const Said made = lane_common::judged([&]() -> Said {
        emulate::native_trace::Governor governor;
        w16::Environment env = spec_.environment ? spec_.environment(governor) : w16::Environment{};
        env.governor = &governor;
        w16::Runner runner(env);
        const w16::ProbeOptions base = win16_judge_base();
        const formats::ne::Module nef = formats::ne::Module::open_file(c.module);
        const std::vector<formats::ne::Segment> segments = nef.segments();
        const auto code_of = [&](std::uint32_t segment) {
            Bytes code;
            if (segment >= 1 && segment <= segments.size() && segments[segment - 1].file_offset < nef.data().size()) {
                const formats::ne::Segment& s = segments[segment - 1];
                const ByteView bytes = nef.data().subspan(s.file_offset, std::min<std::uint64_t>(s.length, nef.data().size() - s.file_offset));
                code.assign(bytes.begin(), bytes.end());
            }
            return code;
        };
        JumpTable jumps(code_of(item.segment), item.segment);
        std::vector<std::uint32_t> original_segments;
        for (const auto& [mine, theirs] : c.order) original_segments.push_back(theirs);
        const std::map<std::uint16_t, std::string> original_names = named_segments(original_segments);
        const std::map<std::uint16_t, std::string> rebuilt_names = named_segments({}, &c.order);

        Win16Target original_target{c.module, item.segment, item.offset, item.near_entry, item.wide, item.span};
        Win16Target rebuilt_target{c.dll, item.built_segment, item.built_offset, item.near_entry, item.wide, std::nullopt};
        Win16Side original_side;
        original_side.own = c.own;
        original_side.frame = item.frame;
        original_side.segs = original_names;
        original_side.jumps = &jumps;
        Win16Side rebuilt_side;
        rebuilt_side.data_image = c.data_image;
        rebuilt_side.own = c.own;
        rebuilt_side.frame = item.frame;
        rebuilt_side.segs = rebuilt_names;

        // the original's run depends only on the original: it is kept within this judgement (the path explorer's runs too)
        std::map<std::string, Win16Print> runs;
        std::optional<compare::Fingerprint> last;
        const auto run_key = [](const Vector16& v) {
            std::string k;
            for (std::int64_t a : v.args) k += std::format("{:x},", a);
            k += v.stub_ret ? std::format("r{:x}", *v.stub_ret) : "-";
            for (const w16::MemoryInit& m : v.mem) {
                k += std::holds_alternative<std::uint32_t>(m.where) ? std::format(";{:x}=", std::get<std::uint32_t>(m.where))
                                                                    : std::format(";D{:x}=", std::get<w16::DataOffset>(m.where).offset);
                k += w16_hex(m.data);
            }
            return k;
        };
        const auto original = [&](const Vector16& v, const emulate::Run* ran = nullptr) -> Win16Print {
            const std::string k = run_key(v);
            auto it = runs.find(k);
            if (it == runs.end()) {   // (a run the explorer made is the original's run of that input: no other is made)
                Win16Print p = win16_fingerprint(runner, base, original_target, v, original_side, ran);
                it = runs.emplace(k, std::move(p)).first;
            }
            last = it->second.fp;
            return it->second;
        };
        const auto rebuilt = [&](const Vector16& input) -> compare::Fingerprint {
            Vector16 v = input;
            if (item.frame) v.args = w16_frame_arguments(*item.frame);   // (pointers to the places, not the input's words)
            compare::Fingerprint fp = win16_fingerprint(runner, base, rebuilt_target, v, rebuilt_side).fp;
            if (fp.exit == "truncated" && last && last->exit == "ret") {
                // (the original returned and its C was still going: C takes more instructions than the code it was made from. Given RETRY times the steps;
                // one still going then does not end)
                Win16Side again = rebuilt_side;
                again.steps = kWin16Retry * emulate::emulator::kMaxInsns;
                fp = win16_fingerprint(runner, base, rebuilt_target, v, again).fp;
            }
            if (last && last->ret == kWin16Leftover && !fp.err && fp.ret) fp.ret = kWin16Leftover;   // the original returned no value here
            return fp;
        };
        std::shared_ptr<concolic::Exploration<Vector16>> found;
        const std::vector<Vector16> fixed = win16_vectors();
        const auto aimed = [&](const std::set<std::int64_t>&, const std::set<std::int64_t>&) -> std::vector<Vector16> {
            // the directed inputs, then those found path by path (or guessed) seeded with every input -- of the fixed ones the paths they took are known:
            // one per path is run again
            const std::vector<Vector16> directed = win16_directed(nef, item.segment, item.span);
            std::vector<Vector16> seeds = fixed;
            seeds.insert(seeds.end(), directed.begin(), directed.end());
            std::map<std::size_t, concolic::Trail> known;
            for (std::size_t i = 0; i < fixed.size(); ++i) {
                const Win16Print p = original(fixed[i]);
                if (!p.fp.err) known[i] = p.trail;
            }
            Win16Explored got = win16_explore(runner, spec_.lifters, base, original_target, seeds, known);
            if (!item.frame)   // (the explorer ran the original as the judge does)
                for (const auto& [v, record] : got.made) original(v, &record);
            found = got.exploration;
            std::vector<Vector16> out = directed;
            out.insert(out.end(), got.inputs.begin(), got.inputs.end());
            return out;
        };
        const std::function<compare::Fingerprint(const Vector16&)> run_original = [&](const Vector16& v) { return original(v).fp; };
        const std::function<compare::Fingerprint(const Vector16&)> run_rebuilt = rebuilt;
        compare::Options options;
        options.insns = item.insns;
        const compare::Outcome outcome = compare::compare<Vector16>(run_original, run_rebuilt, fixed, {}, options, aimed);
        Said j;
        j.verdict = outcome.verdict;
        j.reason = outcome.reason;
        j.limits = outcome.limit_codes();
        if (outcome.verdict != store::Verdict::Divergent) {
            const std::vector<std::string> paths = lane_common::path_limits(found.get());
            j.limits.insert(j.limits.end(), paths.begin(), paths.end());
        }
        j.detail = outcome.detail;
        return j;
    });
    return store::Judgement{made.verdict, made.reason, made.limits, made.detail};
}

}  // namespace cb::verify::lanes
