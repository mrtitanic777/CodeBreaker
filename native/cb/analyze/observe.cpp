#include "cb/analyze/observe.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <set>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/jobs/pool.hpp"
#include "cb/knowledge/value.hpp"

namespace cb::analyze::observe {

namespace emu = emulate::emulator;
using knowledge::Value;
using namespace std::literals;

namespace {

constexpr std::int64_t kObsStride = 0x10000;   // argument k's buffer is kBuffers + k * kObsStride
constexpr std::size_t kObsProbeSize = 64;             // the battery's buffers, padded to this

std::uint32_t obs_buffer_address(int k) { return static_cast<std::uint32_t>(kBuffers + k * kObsStride); }

std::int64_t obs_sign(std::int64_t x) { return (x > 0) - (x < 0); }
// A 32-bit result as the signed number a comparison returns.
std::int64_t obs_s32(std::uint32_t v) { return static_cast<std::int32_t>(v); }

Bytes obs_bytes(std::string_view text) { return Bytes(text.begin(), text.end()); }

// "hey" as the UTF-16 string of a wide-character function, terminated.
Bytes obs_wide(std::string_view ascii) {
    Bytes out;
    for (char c : ascii) {
        out.push_back(static_cast<std::uint8_t>(c));
        out.push_back(0);
    }
    out.push_back(0);
    out.push_back(0);
    return out;
}

bool obs_starts_with(ByteView whole, ByteView prefix) {
    return whole.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), whole.begin());
}

// `prefix` followed by one byte of the filler the battery pads its buffers with (to see a copy that went one byte too far).
Bytes obs_then_filler(ByteView prefix, std::uint8_t filler) {
    Bytes out(prefix.begin(), prefix.end());
    out.push_back(filler);
    return out;
}

constexpr std::uint8_t kObsPadA = 0xEE, kObsPadB = 0xDD;   // buffer A is padded with 0xEE, buffer B with 0xDD

// What one run of the battery left: the result (none when the function did not return) and both buffers afterwards.
struct ObsProbe {
    std::optional<std::uint32_t> ret;
    Bytes a, b;
    bool returned() const { return ret.has_value(); }
};

}  // namespace

std::string_view name_of(Exit exit) {
    switch (exit) {
        case Exit::Returned: return "ret";
        case Exit::Cut: return "cut";
        case Exit::Fault: return "fault";
    }
    return "fault";
}

std::string_view name_of(Idiom idiom) {
    switch (idiom) {
        case Idiom::Strlen: return "strlen";
        case Idiom::Wcslen: return "wcslen";
        case Idiom::Strcpy: return "strcpy";
        case Idiom::Strcat: return "strcat";
        case Idiom::Strcmp: return "strcmp";
        case Idiom::Stricmp: return "stricmp";
        case Idiom::Strchr: return "strchr";
        case Idiom::Strrchr: return "strrchr";
        case Idiom::Strncpy: return "strncpy";
        case Idiom::Memcpy: return "memcpy";
        case Idiom::Memset: return "memset";
        case Idiom::Memcmp: return "memcmp";
        case Idiom::Memicmp: return "memicmp";
        case Idiom::Strncmp: return "strncmp";
    }
    return "?";
}

// ---- the buffers ---------------------------------------------------------------------------------------------------------

Bytes fill(int k) {
    Bytes out;
    out.reserve(static_cast<std::size_t>(kBufferSize));
    for (std::int64_t o = 0; o < kBufferSize; o += 4) {
        const auto v = static_cast<std::uint32_t>(kMark + (static_cast<std::int64_t>(k) << 24) + (o << 12));
        for (int shift = 0; shift < 32; shift += 8) out.push_back(static_cast<std::uint8_t>(v >> shift));
    }
    return out;
}

std::optional<Where> where(std::int64_t address) {
    if (kBuffers <= address && address < kBuffers + kMaxArgs * kObsStride && (address & 0xFFFF) < kBufferSize)
        return Where{Where::Kind::Argument, static_cast<int>((address - kBuffers) >> 16), address & 0xFFFF, 0};
    if (kMark <= address && address < kMark + (static_cast<std::int64_t>(kMaxArgs) << 24))
        return Where{Where::Kind::Field, static_cast<int>((address - kMark) >> 24), (address >> 12) & 0xFFF, address & 0xFFF};
    return std::nullopt;
}

void digest(const emu::Trace* run, Observation& out) {
    if (run == nullptr) {   // nothing could be run: it faulted, unless an earlier run returned
        if (out.exit != Exit::Returned) out.exit = Exit::Fault;
        return;
    }
    const emulate::native_trace::TraceRecord& rec = run->record;
    auto touch = [&out](std::uint64_t address, std::uint64_t size, bool write) {
        const std::optional<Where> w = where(static_cast<std::int64_t>(address));
        if (!w) return;
        Cell& cell = w->kind == Where::Kind::Argument ? out.fields[w->k][w->o] : out.nested[{w->k, w->o}][w->offset];
        cell.size = std::max(cell.size, static_cast<std::int64_t>(size));
        ++(write ? cell.writes : cell.reads);
    };
    for (const auto& m : rec.mem_reads) touch(m.addr, m.size, false);
    for (const auto& m : rec.mem_writes) touch(m.addr, m.size, true);
    for (const auto& c : rec.calls) {   // a call through a register or a table that ended up in the module's own code
        if (!c.indirect() || !c.resolved_target || *c.resolved_target == 0 || c.external) continue;
        const Indirect found{static_cast<Va>(c.pc), static_cast<Va>(*c.resolved_target)};
        if (std::none_of(out.indirect.begin(), out.indirect.end(), [&](const auto& i) { return i == found; })) out.indirect.push_back(found);
    }
    if (rec.api_calls) {
        for (const emulate::native_trace::ApiCall& call : *rec.api_calls) {
            if (!call.args) continue;
            for (std::size_t j = 0; j < call.args->size(); ++j) {
                const std::optional<Where> w = where((*call.args)[j]);
                Pass row{call.api, static_cast<int>(j), 0, 0, Pass::How::Address};
                if (w && w->kind == Where::Kind::Argument) {
                    row.argument = w->k;
                    row.offset = w->o;
                } else if (w && w->offset == 0) {
                    row.argument = w->k;
                    row.offset = w->o;
                    row.how = Pass::How::Value;
                } else {
                    continue;
                }
                if (std::find(out.passes.begin(), out.passes.end(), row) == out.passes.end()) out.passes.push_back(row);
            }
        }
    }
    if (run->exit == emu::Exit::Returned) {
        out.exit = Exit::Returned;
        if (run->esp_delta && *run->esp_delta < 0x100) out.removed = static_cast<std::int64_t>(*run->esp_delta) - 4;
        const std::optional<Where> w = where(run->ret.value_or(0));
        if (w && w->kind == Where::Kind::Argument && w->o == 0) out.returns = Returns{Returns::Kind::Argument, w->k, std::nullopt};
        else if (w && w->kind == Where::Kind::Field && w->offset == 0) out.returns = Returns{Returns::Kind::Field, w->k, w->o};
    } else if (out.exit != Exit::Returned) {
        out.exit = rec.truncated ? Exit::Cut : Exit::Fault;
    }
}

Bytes after(const emu::Trace& run, std::int64_t base, ByteView initial) {
    Bytes buf(initial.begin(), initial.end());
    for (const auto& w : run.record.mem_writes) {
        const std::int64_t off = static_cast<std::int64_t>(w.addr) - base;
        const auto size = static_cast<std::int64_t>(w.size);
        if (off < 0 || off + size > static_cast<std::int64_t>(buf.size())) continue;   // a write outside the buffer is not its content
        for (std::int64_t i = 0; i < size; ++i)
            buf[static_cast<std::size_t>(off + i)] = i < 8 ? static_cast<std::uint8_t>(w.value >> (8 * i)) : 0;
    }
    return buf;
}

// ---- running -------------------------------------------------------------------------------------------------------------

Observer::Observer(Setting setting) : setting_(std::move(setting)) {
    emu::Environment env = setting_.environment;
    const std::filesystem::path module = setting_.module;
    env.resolve = [module](std::string_view) { return module; };
    auto tracer = std::make_unique<emu::Tracer>(std::move(env));
    tracer->load(setting_.module.filename().string());
    idle_.push_back(std::move(tracer));
}

std::optional<emu::Trace> Observer::trace(Va function, const std::vector<std::uint32_t>& args,
                                          const std::vector<emu::MemoryInit>& memory) const {
    std::unique_ptr<emu::Tracer> tracer;
    {
        const std::lock_guard<std::mutex> lock(mutex_);
        if (!idle_.empty()) {
            tracer = std::move(idle_.back());
            idle_.pop_back();
        }
    }
    if (!tracer) {
        emu::Environment env = setting_.environment;
        const std::filesystem::path module = setting_.module;
        env.resolve = [module](std::string_view) { return module; };
        tracer = std::make_unique<emu::Tracer>(std::move(env));
    }
    emu::TraceOptions options;
    options.args = args;
    options.mem_init = memory;
    options.stub_externals = true;
    options.argc = setting_.argc;
    options.insn_cap = kInstructions;
    try {
        emu::Trace done = tracer->trace(setting_.module.filename().string(), emu::FuncSelector{static_cast<std::uint64_t>(function)}, options);
        const std::lock_guard<std::mutex> lock(mutex_);
        idle_.push_back(std::move(tracer));
        return done;
    } catch (const FormatError&) {   // the emulator could not lay the function out or run it at all: that function is a fault
        return std::nullopt;         // (the Tracer is dropped: it may hold a half-made machine)
    } catch (const EmulationError&) {
        return std::nullopt;
    }
}

Observation Observer::observe(const Candidate& c) const {
    Observation out;
    const int n = static_cast<int>(std::min<std::int64_t>(c.args.value_or(4), kMaxArgs));
    std::vector<emu::MemoryInit> memory;
    std::vector<std::uint32_t> every, pointers_only, zeros(static_cast<std::size_t>(n), 0);
    for (int k = 0; k < n; ++k) {
        memory.push_back({obs_buffer_address(k), fill(k)});
        every.push_back(obs_buffer_address(k));
        pointers_only.push_back(std::find(c.pointers.begin(), c.pointers.end(), k) != c.pointers.end() ? obs_buffer_address(k) : 0);
    }
    auto run_and_digest = [&](const std::vector<std::uint32_t>& args, const std::vector<emu::MemoryInit>& mem) {
        const std::optional<emu::Trace> t = trace(c.function, args, mem);
        digest(t ? &*t : nullptr, out);
    };
    run_and_digest(every, memory);
    if (n != 0 && !c.pointers.empty() && static_cast<int>(c.pointers.size()) < n) run_and_digest(pointers_only, memory);
    if (n == 0 || out.exit != Exit::Returned) run_and_digest(zeros, {});
    if (c.battery) out.idiom = idiom(c.function, c.args);
    return out;
}

std::optional<Idiom> Observer::idiom(Va function, std::optional<std::int64_t> nargs) const {
    const std::uint32_t A = static_cast<std::uint32_t>(kTextA), B = static_cast<std::uint32_t>(kTextB);
    // One run with buffer A holding `a` (padded with 0xEE) and B holding `b` (padded with 0xDD, so a byte copied too far shows).
    auto run = [&](const std::vector<std::uint32_t>& args, ByteView a = {}, ByteView b = {}) {
        Bytes pa(a.begin(), a.end()), pb(b.begin(), b.end());
        if (pa.size() < kObsProbeSize) pa.resize(kObsProbeSize, kObsPadA);
        if (pb.size() < kObsProbeSize) pb.resize(kObsProbeSize, kObsPadB);
        const std::optional<emu::Trace> t = trace(function, args, {{A, pa}, {B, pb}});
        ObsProbe p;
        p.a = pa;
        p.b = pb;
        if (t && t->exit == emu::Exit::Returned) {
            p.ret = t->ret.value_or(0);
            p.a = after(*t, kTextA, pa);
            p.b = after(*t, kTextB, pb);
        }
        return p;
    };
    auto B_ = [](std::string_view s) { return obs_bytes(s); };
    // the buffers still start with what they were given
    auto unchanged = [](const ObsProbe& r, ByteView a, ByteView b) { return obs_starts_with(r.a, a) && obs_starts_with(r.b, b); };
    auto has = [&](std::int64_t n) { return !nargs || *nargs == n; };

    struct Test {
        Idiom idiom;
        std::function<bool()> passes;
    };
    std::vector<Test> tests;

    if (has(1)) {
        tests.push_back({Idiom::Strlen, [&] {
            for (const auto& [s, n] : std::vector<std::pair<std::string_view, std::uint32_t>>{{"hello\0"sv, 5}, {"\0"sv, 0}, {"abcdefghijk\0"sv, 11}}) {
                const ObsProbe r = run({A}, B_(s));
                if (!(r.ret == n && unchanged(r, B_(s), {}))) return false;
            }
            return true;
        }});
        tests.push_back({Idiom::Wcslen, [&] {
            for (const auto& [s, n] : std::vector<std::pair<std::string_view, std::uint32_t>>{{"hey"sv, 3}, {""sv, 0}, {"abcdefg"sv, 7}})
                if (run({A}, obs_wide(s)).ret != n) return false;
            return true;
        }});
    }
    if (has(2)) {
        tests.push_back({Idiom::Strcpy, [&] {
            for (std::string_view s : {"hello\0"sv, "\0"sv, "a longer string\0"sv}) {
                const ObsProbe r = run({B, A}, B_(s));
                if (!r.returned() || !obs_starts_with(r.b, obs_then_filler(B_(s), kObsPadB))) return false;
            }
            return true;
        }});
        tests.push_back({Idiom::Strcat, [&] {
            for (const auto& [s, d] : std::vector<std::pair<std::string_view, std::string_view>>{{"cd\0"sv, "ab\0"sv}, {"xyz\0"sv, "\0"sv}, {"\0"sv, "q\0"sv}}) {
                const ObsProbe r = run({B, A}, B_(s), B_(d));
                Bytes want = B_(d);
                want.pop_back();   // the destination's own terminator is overwritten by the source
                want.insert(want.end(), s.begin(), s.end());
                if (!r.returned() || !obs_starts_with(r.b, obs_then_filler(want, kObsPadB))) return false;
            }
            return true;
        }});
        struct Order {
            std::string_view a, b;
            int want;
        };
        const std::vector<Order> ordered = {{"abc\0"sv, "abc\0"sv, 0}, {"abc\0"sv, "abd\0"sv, -1}, {"abd\0"sv, "abc\0"sv, 1}, {"ab\0"sv, "abc\0"sv, -1}, {"\0"sv, "\0"sv, 0}};
        auto compares = [&, A, B](std::vector<Order> cases) {
            for (const Order& c : cases) {
                const ObsProbe r = run({A, B}, B_(c.a), B_(c.b));
                if (!r.returned() || obs_sign(obs_s32(*r.ret)) != c.want || !unchanged(r, B_(c.a), B_(c.b))) return false;
            }
            return true;
        };
        tests.push_back({Idiom::Strcmp, [&, ordered, compares] {
            std::vector<Order> cases = ordered;
            cases.push_back({"ABC\0"sv, "abc\0"sv, -1});
            return compares(cases);
        }});
        tests.push_back({Idiom::Stricmp, [&, ordered, compares] {
            std::vector<Order> cases = ordered;
            cases.push_back({"ABC\0"sv, "abc\0"sv, 0});
            cases.push_back({"aBd\0"sv, "Abc\0"sv, 1});
            return compares(cases);
        }});
        auto finds = [&, A](std::vector<std::pair<char, std::uint32_t>> cases) {
            for (const auto& [c, want] : cases)
                if (run({A, static_cast<std::uint32_t>(static_cast<unsigned char>(c))}, B_("hello\0"sv)).ret != want) return false;
            return true;
        };
        tests.push_back({Idiom::Strchr, [&, finds, A] { return finds({{'l', A + 2}, {'h', A}, {'z', 0}}); }});
        tests.push_back({Idiom::Strrchr, [&, finds, A] { return finds({{'l', A + 3}, {'h', A}, {'z', 0}}); }});
    }
    if (has(3)) {
        Bytes src;
        for (int i = 1; i <= 32; ++i) src.push_back(static_cast<std::uint8_t>(i));
        auto first = [](ByteView v, std::size_t n) { return Bytes(v.begin(), v.begin() + static_cast<std::ptrdiff_t>(std::min(n, v.size()))); };
        struct Counted {
            std::string_view a, b;
            std::uint32_t n;
            int want;
        };
        const std::vector<Counted> sized = {{"abcd"sv, "abcd"sv, 4, 0}, {"abcd"sv, "abce"sv, 4, -1}, {"abce"sv, "abcd"sv, 4, 1}, {"abcd"sv, "abce"sv, 3, 0}};
        auto counted = [&, A, B](std::vector<Counted> cases) {
            for (const Counted& c : cases) {
                const ObsProbe r = run({A, B, c.n}, B_(c.a), B_(c.b));
                if (!r.returned() || obs_sign(obs_s32(*r.ret)) != c.want) return false;
            }
            return true;
        };
        tests.push_back({Idiom::Strncpy, [&, first, A, B] {
            for (const auto& [s, n] : std::vector<std::pair<std::string_view, std::uint32_t>>{{"hello\0"sv, 3}, {"hi\0"sv, 6}, {"abc\0"sv, 4}}) {
                const ObsProbe r = run({B, A, n}, B_(s));
                Bytes want = first(B_(s), n);   // the source's first n bytes, then zeros up to n
                want.resize(n, 0);
                if (!r.returned() || !obs_starts_with(r.b, obs_then_filler(want, kObsPadB))) return false;
            }
            return true;
        }});
        tests.push_back({Idiom::Memcpy, [&, first, src, A, B] {
            struct Copy {
                Bytes s;
                std::uint32_t n;
            };
            for (const Copy& c : std::vector<Copy>{{src, 7}, {src, 1}, {src, 16}, {src, 0}, {B_("ab\0cdef"sv), 6}}) {
                const ObsProbe r = run({B, A, c.n}, c.s);
                if (!r.returned() || !obs_starts_with(r.b, obs_then_filler(first(c.s, c.n), kObsPadB))) return false;
            }
            return true;
        }});
        tests.push_back({Idiom::Memset, [&, B] {
            for (const auto& [c, n] : std::vector<std::pair<std::uint32_t, std::uint32_t>>{{0x5A, 9}, {0, 4}, {0x11, 1}, {0x7F, 0}}) {
                const ObsProbe r = run({B, c, n});
                Bytes want(n, static_cast<std::uint8_t>(c));
                if (!r.returned() || !obs_starts_with(r.b, obs_then_filler(want, kObsPadB))) return false;
            }
            return true;
        }});
        tests.push_back({Idiom::Memcmp, [&, sized, counted] {
            std::vector<Counted> cases = sized;
            cases.push_back({"\0b"sv, "\0c"sv, 2, -1});
            cases.push_back({"ABCD"sv, "abcd"sv, 4, -1});
            return counted(cases);
        }});
        tests.push_back({Idiom::Memicmp, [&, sized, counted] {
            std::vector<Counted> cases = sized;
            cases.push_back({"\0b"sv, "\0c"sv, 2, -1});
            cases.push_back({"ABCD"sv, "abcd"sv, 4, 0});
            return counted(cases);
        }});
        tests.push_back({Idiom::Strncmp, [&, sized, counted] {
            std::vector<Counted> cases = sized;
            cases.push_back({"\0b"sv, "\0c"sv, 2, 0});
            cases.push_back({"ABCD"sv, "abcd"sv, 4, -1});
            return counted(cases);
        }});
    }
    for (const Test& t : tests)
        if (t.passes()) return t.idiom;
    return std::nullopt;
}

std::vector<Candidate> candidates(Image& image, const conventions::Facts& facts, const std::vector<structures::Structure>& found,
                                  std::optional<std::size_t> limit) {
    std::map<Va, std::set<int>> pointer_args;   // function -> the arguments the static recovery found a structure behind
    for (const structures::Structure& s : found)
        for (const structures::Root& r : s.roots)
            if (r.kind == structures::Root::Kind::Arg) pointer_args[r.at].insert(static_cast<int>(r.index));
    std::vector<Candidate> items;
    for (Va f : image.functions()) {
        const auto fact_at = facts.find(f);
        if (fact_at == facts.end()) continue;
        const conventions::Fact& fact = fact_at->second;
        using conventions::Shape;
        if (fact.shape == Shape::ImportThunk || fact.shape == Shape::JumpThunk || fact.shape == Shape::Empty || fact.shape == Shape::NotCode) continue;
        if (std::any_of(fact.reg_args.begin(), fact.reg_args.end(), [](const std::string& r) { return r != "ecx"; })) continue;
        // worth the battery: small, calls nothing, loops (or is a few instructions in front of the function after it, as
        // hand-written entry points are), and takes one to three arguments
        const bool battery = fact.shape == Shape::Leaf && fact.reg_args.empty() &&
                             (fact.loops != 0 || !fact.string_ops.empty() || image.function(f).insns.size() <= 3) &&
                             (!fact.args || (1 <= *fact.args && *fact.args <= 3));
        Candidate c{f, fact.args, {}, battery};
        if (const auto p = pointer_args.find(f); p != pointer_args.end()) c.pointers.assign(p->second.begin(), p->second.end());
        items.push_back(std::move(c));
    }
    // the informative ones first
    std::sort(items.begin(), items.end(), [](const Candidate& a, const Candidate& b) {
        if (a.battery != b.battery) return a.battery;
        if (a.pointers.empty() != b.pointers.empty()) return !a.pointers.empty();
        return a.function < b.function;
    });
    if (limit && items.size() > *limit) items.resize(*limit);
    return items;
}

// ---- worker processes ------------------------------------------------------------------------------------------------------------

namespace {

void write_cells(jobs::TokenWriter& out, const Cells& cells) {
    out.integer(static_cast<std::int64_t>(cells.size()));
    for (const auto& [offset, cell] : cells) out.integer(static_cast<std::int64_t>(offset)).integer(cell.size).integer(cell.reads).integer(cell.writes);
}

Cells read_cells(jobs::TokenReader& in) {
    Cells cells;
    for (std::int64_t n = in.integer(); n > 0; --n) {
        const Va offset = static_cast<Va>(in.integer());
        Cell c;
        c.size = in.integer();
        c.reads = in.integer();
        c.writes = in.integer();
        cells[offset] = c;
    }
    return cells;
}

void write_optional(jobs::TokenWriter& out, const std::optional<std::int64_t>& v) { out.integer(v ? 1 : 0).integer(v.value_or(0)); }

std::optional<std::int64_t> read_optional(jobs::TokenReader& in) {
    const bool present = in.integer() != 0;
    const std::int64_t v = in.integer();
    return present ? std::optional<std::int64_t>(v) : std::nullopt;
}

}  // namespace

void write_observation(jobs::TokenWriter& out, const Observation& o) {
    out.integer(static_cast<std::int64_t>(o.fields.size()));
    for (const auto& [k, cells] : o.fields) {
        out.integer(k);
        write_cells(out, cells);
    }
    out.integer(static_cast<std::int64_t>(o.nested.size()));
    for (const auto& [key, cells] : o.nested) {
        out.integer(key.first).integer(static_cast<std::int64_t>(key.second));
        write_cells(out, cells);
    }
    out.integer(static_cast<std::int64_t>(o.indirect.size()));
    for (const Indirect& i : o.indirect) out.integer(static_cast<std::int64_t>(i.site)).integer(static_cast<std::int64_t>(i.target));
    out.integer(static_cast<std::int64_t>(o.passes.size()));
    for (const Pass& p : o.passes)
        out.text(p.import).integer(p.parameter).integer(p.argument).integer(static_cast<std::int64_t>(p.offset)).integer(p.how == Pass::How::Value ? 1 : 0);
    write_optional(out, o.exit ? std::optional<std::int64_t>(static_cast<std::int64_t>(*o.exit)) : std::nullopt);
    write_optional(out, o.removed);
    out.integer(o.returns ? 1 : 0);
    if (o.returns) {
        out.integer(o.returns->kind == Returns::Kind::Field ? 1 : 0).integer(o.returns->k);
        write_optional(out, o.returns->offset ? std::optional<std::int64_t>(static_cast<std::int64_t>(*o.returns->offset)) : std::nullopt);
    }
    write_optional(out, o.idiom ? std::optional<std::int64_t>(static_cast<std::int64_t>(*o.idiom)) : std::nullopt);
}

Observation read_observation(jobs::TokenReader& in) {
    Observation o;
    for (std::int64_t n = in.integer(); n > 0; --n) {
        const int k = static_cast<int>(in.integer());
        o.fields[k] = read_cells(in);
    }
    for (std::int64_t n = in.integer(); n > 0; --n) {
        const int k = static_cast<int>(in.integer());
        const Va at = static_cast<Va>(in.integer());
        o.nested[{k, at}] = read_cells(in);
    }
    for (std::int64_t n = in.integer(); n > 0; --n) {
        Indirect i;
        i.site = static_cast<Va>(in.integer());
        i.target = static_cast<Va>(in.integer());
        o.indirect.push_back(i);
    }
    for (std::int64_t n = in.integer(); n > 0; --n) {
        Pass p;
        p.import = in.text();
        p.parameter = static_cast<int>(in.integer());
        p.argument = static_cast<int>(in.integer());
        p.offset = static_cast<Va>(in.integer());
        p.how = in.integer() != 0 ? Pass::How::Value : Pass::How::Address;
        o.passes.push_back(std::move(p));
    }
    if (const auto exit = read_optional(in)) o.exit = static_cast<Exit>(*exit);
    o.removed = read_optional(in);
    if (in.integer() != 0) {
        Returns r;
        r.kind = in.integer() != 0 ? Returns::Kind::Field : Returns::Kind::Argument;
        r.k = static_cast<int>(in.integer());
        if (const auto off = read_optional(in)) r.offset = static_cast<Va>(*off);
        o.returns = r;
    }
    if (const auto idiom = read_optional(in)) o.idiom = static_cast<Idiom>(*idiom);
    return o;
}

namespace {

// The worker's share: the module and the functions to run (address, argument count, pointers, battery); the answer is each one's observation.
int observe_share(const jobs::Args& args, const PrepareSetting& prepare) {
    jobs::TokenReader in(fs::read_text(fs::from_text(args.at(0))));
    Setting setting;
    setting.module = fs::from_text(in.text());
    if (prepare) prepare(setting);
    std::vector<Candidate> items;
    for (std::int64_t n = in.integer(); n > 0; --n) {
        Candidate c;
        c.function = static_cast<Va>(in.integer());
        c.args = read_optional(in);
        for (std::int64_t k = in.integer(); k > 0; --k) c.pointers.push_back(static_cast<int>(in.integer()));
        c.battery = in.integer() != 0;
        items.push_back(std::move(c));
    }
    const Observer observer(setting);
    jobs::TokenWriter out;
    out.integer(static_cast<std::int64_t>(items.size()));
    for (const Candidate& c : items) {
        out.integer(static_cast<std::int64_t>(c.function));
        write_observation(out, observer.observe(c));
    }
    fs::write_text(fs::from_text(args.at(1)), out.str());
    return 0;
}

// The candidates run in worker processes, `shares` of them at a time; the functions a share lost (it crashed, or timed out) are run again one to a
// process, and one that crashes again is lost alone. nullopt where the launcher is not there or the list is too short to share out.
std::optional<Seen> run_apart(const std::vector<Candidate>& items, const Setting& setting) {
    if (!setting.launcher || !setting.launcher->enabled()) return std::nullopt;
    const std::size_t workers = static_cast<std::size_t>(std::max(1, setting.jobs));
    const std::size_t shares = std::min(workers, items.size() / std::max<std::size_t>(1, setting.per_process));
    if (shares < 2) return std::nullopt;
    const auto start = [&](const std::vector<Candidate>& part, Seen& into) {
        jobs::Handover hand("observe");
        jobs::TokenWriter in;
        in.text(fs::to_text(setting.module)).integer(static_cast<std::int64_t>(part.size()));
        for (const Candidate& c : part) {
            in.integer(static_cast<std::int64_t>(c.function));
            write_optional(in, c.args);
            in.integer(static_cast<std::int64_t>(c.pointers.size()));
            for (const int k : c.pointers) in.integer(k);
            in.integer(c.battery ? 1 : 0);
        }
        hand.write("in.txt", in.str());
        jobs::LaunchOptions options;
        options.timeout = std::chrono::minutes(30);
        const jobs::WorkerResult r = setting.launcher->run("observe", {fs::to_text(hand.file("in.txt")), fs::to_text(hand.file("out.txt"))}, options);
        if (!r.ok()) return false;
        try {
            jobs::TokenReader answer(hand.read("out.txt"));
            for (std::int64_t n = answer.integer(); n > 0; --n) {
                const Va f = static_cast<Va>(answer.integer());
                into.emplace(f, read_observation(answer));
            }
        } catch (const Error&) {
            return false;
        }
        return true;
    };
    std::vector<std::vector<Candidate>> parts(shares);
    for (std::size_t i = 0; i < items.size(); ++i) parts[i % shares].push_back(items[i]);   // (round robin: the heavy ones spread)
    std::vector<Seen> done(shares);
    std::vector<char> ok(shares, 0);
    {
        std::vector<std::thread> running;
        for (std::size_t s = 0; s < shares; ++s)
            running.emplace_back([&, s] {
                try {
                    ok[s] = start(parts[s], done[s]) ? 1 : 0;
                } catch (const Error&) {   // (a worker that would not start: its functions are run again below)
                }
            });
        for (std::thread& t : running) t.join();
    }
    Seen out;
    std::vector<Candidate> again;
    for (std::size_t s = 0; s < shares; ++s) {
        if (ok[s]) {
            for (auto& [f, o] : done[s]) out.emplace(f, std::move(o));
        } else {
            again.insert(again.end(), parts[s].begin(), parts[s].end());
        }
    }
    for (const Candidate& c : again) {
        Seen one;
        bool good = false;
        try {
            good = start(std::vector<Candidate>{c}, one);
        } catch (const Error&) {
        }
        if (good) {
            for (auto& [f, o] : one) out.emplace(f, std::move(o));
        } else {
            out.emplace(c.function, std::nullopt);   // lost alone
        }
    }
    return out;
}

}  // namespace

void register_worker_kinds(jobs::WorkerKinds& kinds, PrepareSetting prepare) {
    kinds.add("observe", [prepare = std::move(prepare)](const jobs::Args& args) { return observe_share(args, prepare); });
}

Seen run(Image& image, const conventions::Facts& facts, const std::vector<structures::Structure>& found, const Setting& setting) {
    if (!image.flat()) return {};
    const std::vector<Candidate> items = candidates(image, facts, found, setting.limit);
    if (std::optional<Seen> apart = run_apart(items, setting)) return std::move(*apart);
    const Observer observer(setting);
    jobs::RunOptions<Candidate, std::optional<Observation>> options;
    options.jobs = setting.jobs;
    options.small = 12;   // fewer than this are not worth the threads
    options.crashed = [](const Candidate&) { return std::optional<Observation>(); };
    auto done = jobs::run(
        items, [&observer](const Candidate& c) { return std::optional<Observation>(observer.observe(c)); },
        [](const Candidate& c) { return c.function; }, options);
    Seen out;
    for (auto& entry : done) out.emplace(entry.first, std::move(entry.second));
    return out;
}

// ---- the analysis ---------------------------------------------------------------------------------------------------------

std::string describe(const Pass& pass) {
    return std::format("{} parameter {} gets {} argument {} +{:x}", pass.import, pass.parameter,
                       pass.how == Pass::How::Address ? "the address of" : "the value at", pass.argument, pass.offset);
}

Report analyse(Image& image, const conventions::Facts& facts, const std::vector<structures::Structure>& found, const Seen& seen) {
    Report report;
    Counts& counts = report.counts;
    std::set<Va> functions(image.functions().begin(), image.functions().end());
    std::set<Va> new_targets;
    std::int64_t edges = 0;
    // (function, argument) -> the structure the static recovery found behind it (the later one of equal keys)
    std::map<std::pair<Va, Va>, const structures::Structure*> static_fields;
    for (const structures::Structure& s : found)
        for (const structures::Root& r : s.roots)
            if (r.kind == structures::Root::Kind::Arg) static_fields[{r.at, r.index}] = &s;

    std::vector<std::pair<std::string_view, Va>> idioms;   // (name, function), to be sorted
    for (const auto& [f, maybe] : seen) {
        if (!maybe) continue;   // the pool lost it
        const Observation& o = *maybe;
        const Exit exit = o.exit.value_or(Exit::Fault);
        ++counts.functions_run;
        counts.returned += exit == Exit::Returned;
        counts.faulted += exit == Exit::Fault;
        counts.cut_off += exit == Exit::Cut;
        counts.with_pointer_arguments += !o.fields.empty();
        for (const auto& [k, cells] : o.fields) counts.fields_observed += static_cast<std::int64_t>(cells.size());
        counts.nested_pointers_followed += static_cast<std::int64_t>(o.nested.size());
        counts.fields_passed_to_system_calls += static_cast<std::int64_t>(o.passes.size());

        ObservedFact fact;
        fact.addr = f;
        fact.exit = exit;
        fact.fields = o.fields;
        fact.nested = o.nested;
        fact.passes = o.passes;
        for (const Indirect& i : o.indirect) {
            const std::optional<Va> owner = image.owner(i.site);
            if (!owner || !image.is_code(i.target)) continue;
            fact.indirect_calls.push_back(std::format("{} at {:x} -> {}", image.label(*owner), i.site, image.label(i.target)));
            ++edges;
            if (!functions.contains(i.target)) new_targets.insert(i.target);
        }
        if (o.removed) {
            fact.removed = o.removed;
            const auto fc = facts.find(f);
            if (fc != facts.end() && fc->second.removes) {
                counts.removed_bytes_agree_with_static += *fc->second.removes == *o.removed;
                counts.removed_bytes_differ += *fc->second.removes != *o.removed;
            }
        }
        fact.returns = o.returns;
        if (o.idiom) {
            fact.behaves_like = o.idiom;
            idioms.emplace_back(name_of(*o.idiom), f);
        }
        for (const auto& [k, cells] : o.fields) {
            const auto s = static_fields.find({f, k});
            if (s == static_fields.end()) continue;
            for (const auto& [offset, cell] : cells) {
                if (s->second->fields.contains(offset)) ++counts.static_fields_confirmed;
                else ++counts.fields_only_seen_running;
            }
        }
        report.observed.push_back(std::move(fact));
    }

    // what the runs say about each recovered structure
    std::map<std::string, std::pair<const structures::Structure*, std::set<Va>>> by_structure;
    for (const auto& [key, s] : static_fields) {
        const auto o = seen.find(key.first);
        if (o == seen.end() || !o->second) continue;
        const auto cells = o->second->fields.find(static_cast<int>(key.second));
        if (cells == o->second->fields.end() || cells->second.empty()) continue;
        auto& entry = by_structure.try_emplace(s->name, s, std::set<Va>{}).first->second;
        for (const auto& [offset, cell] : cells->second) entry.second.insert(offset);
    }
    for (const auto& [name, entry] : by_structure) {
        const structures::Structure& s = *entry.first;
        StructureObservedFact fact;
        fact.name = name;
        for (Va offset : entry.second) (s.fields.contains(offset) ? fact.fields_confirmed : fact.fields_added).push_back(offset);
        for (const auto& [offset, field] : s.fields) fact.fields_not_reached += !entry.second.contains(offset);
        counts.structures_with_a_field_confirmed += !fact.fields_confirmed.empty();
        report.structures.push_back(std::move(fact));
    }
    counts.indirect_calls_resolved = edges;
    counts.function_starts_found = static_cast<std::int64_t>(new_targets.size());
    counts.library_idioms = static_cast<std::int64_t>(idioms.size());

    std::sort(idioms.begin(), idioms.end());
    for (std::size_t i = 0; i < idioms.size() && i < 16; ++i)
        report.text.push_back(std::format("behaves like {}: {}", idioms[i].first, image.label(idioms[i].second)));
    std::size_t shown = 0;
    for (const ObservedFact& fact : report.observed)
        for (const std::string& line : fact.indirect_calls)
            if (shown++ < 8) report.text.push_back(line);
    if (!new_targets.empty()) {
        std::string line = "function starts reached only by running: ";
        std::size_t n = 0;
        for (Va t : new_targets) {
            if (n++ == 8) break;
            line += std::format("{}{:x}", n == 1 ? "" : ", ", t);
        }
        report.text.push_back(line);
    }
    return report;
}

namespace {

Value obs_cell_value(const Cell& c) { return Value(Value::List{Value(c.size), Value(c.reads), Value(c.writes)}); }

Value obs_cells_value(const Cells& cells) {
    Value::Map out;
    for (const auto& [offset, cell] : cells) out.insert_or_assign(std::format("+{:x}", offset), obs_cell_value(cell));
    return Value(std::move(out));
}

}  // namespace

Value to_value(const ObservedFact& fact) {
    Value::Map out;
    out.insert_or_assign("addr", Value(static_cast<std::int64_t>(fact.addr)));
    out.insert_or_assign("kind", Value("observed"));
    out.insert_or_assign("exit", Value(std::string(name_of(fact.exit))));
    if (!fact.fields.empty()) {
        Value::Map m;
        for (const auto& [k, cells] : fact.fields) m.insert_or_assign(std::format("argument {}", k), obs_cells_value(cells));
        out.insert_or_assign("fields", Value(std::move(m)));
    }
    if (!fact.nested.empty()) {
        Value::Map m;
        for (const auto& [key, cells] : fact.nested) m.insert_or_assign(std::format("argument {} +{:x}", key.first, key.second), obs_cells_value(cells));
        out.insert_or_assign("nested", Value(std::move(m)));
    }
    if (!fact.passes.empty()) {
        Value::List l;
        for (const Pass& p : fact.passes) l.emplace_back(describe(p));
        out.insert_or_assign("passes", Value(std::move(l)));
    }
    if (!fact.indirect_calls.empty()) {
        Value::List l;
        for (const std::string& s : fact.indirect_calls) l.emplace_back(s);
        out.insert_or_assign("indirect_calls", Value(std::move(l)));
    }
    if (fact.removed) out.insert_or_assign("removed", Value(*fact.removed));
    if (fact.returns) {
        Value::List l;
        l.emplace_back(fact.returns->kind == Returns::Kind::Argument ? "argument" : "field");
        l.emplace_back(static_cast<std::int64_t>(fact.returns->k));
        if (fact.returns->offset) l.emplace_back(static_cast<std::int64_t>(*fact.returns->offset));
        out.insert_or_assign("returns", Value(std::move(l)));
    }
    if (fact.behaves_like) out.insert_or_assign("behaves_like", Value(std::string(name_of(*fact.behaves_like))));
    return Value(std::move(out));
}

std::vector<facts::Row> fact_rows(const Report& report, ImageKind kind) {
    std::vector<facts::Row> rows;
    for (const ObservedFact& f : report.observed) rows.push_back({facts::key(kind, f.addr), "observed", to_value(f)});
    for (const StructureObservedFact& s : report.structures) {
        auto offsets = [](const std::vector<Va>& v) {
            Value::List l;
            for (Va o : v) l.emplace_back(std::format("+{:x}", o));
            return Value(std::move(l));
        };
        rows.push_back({"", "structure_observed",
                        Value(Value::Map{{"addr", Value()},
                                         {"kind", Value("structure_observed")},
                                         {"name", Value(s.name)},
                                         {"fields_confirmed", offsets(s.fields_confirmed)},
                                         {"fields_added", offsets(s.fields_added)},
                                         {"fields_not_reached", Value(s.fields_not_reached)}})});
    }
    return rows;
}

}  // namespace cb::analyze::observe
