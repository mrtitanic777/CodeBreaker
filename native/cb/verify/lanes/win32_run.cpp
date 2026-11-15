// cb-unity: alone
// cb/verify/lanes/win32_run.cpp -- what the 32-bit lane asks of a run: the tracers a judge runs on, one run as a fingerprint two builds can
// be compared on (their calls into imports, result, memory left changed, stack, saved registers, coverage), and what the original's code says
// of a function (see win32.hpp).

#include <algorithm>
#include <any>
#include <format>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "cb/core/capstone_api.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/pe.hpp"
#include "cb/verify/lanes/win32_internal.hpp"
#include "cb/verify/stage_timer.hpp"

namespace cb::verify::lanes {

namespace emu = emulate::emulator;
namespace stdfs = std::filesystem;

// ---- the tracers -----------------------------------------------------------------------------------------------------------------

Win32TracerPool::Lease Win32TracerPool::acquire() {
    std::unique_ptr<emu::Tracer> tracer;
    {
        const std::lock_guard<std::mutex> guard(lock_);
        if (!idle_.empty()) {
            tracer = std::move(idle_.back());
            idle_.pop_back();
        }
    }
    if (!tracer) tracer = std::make_unique<emu::Tracer>(environment_);
    return Lease(*this, std::move(tracer));
}

void Win32TracerPool::release(std::unique_ptr<emu::Tracer> tracer) {
    const std::lock_guard<std::mutex> guard(lock_);
    idle_.push_back(std::move(tracer));
}

Win32Lane::Context::Context(const Win32Lane::Services& services, stdfs::path original_path)
    : original(std::move(original_path)),
      fingerprints(services.fingerprints),
      import_argc(services.import_argc),
      tracers(services.tracer) {
    // A run asks for the argument count of every import its function calls, so thousands of times; the answer comes from the knowledge base and
    // the exporting module's code, and is the same each time: asked once.
    if (services.import_argc) {
        struct Memo {
            std::mutex lock;
            std::unordered_map<std::string, std::optional<std::int64_t>> answers;
        };
        auto memo = std::make_shared<Memo>();
        import_argc = [ask = services.import_argc, memo](std::string_view dll, std::string_view name) -> std::optional<std::int64_t> {
            std::string key(dll);
            key += '!';
            key += name;
            {
                const std::lock_guard<std::mutex> guard(memo->lock);
                if (const auto it = memo->answers.find(key); it != memo->answers.end()) return it->second;
            }
            const std::optional<std::int64_t> answer = ask(dll, name);
            const std::lock_guard<std::mutex> guard(memo->lock);
            memo->answers.emplace(std::move(key), answer);
            return answer;
        };
    }
    if (services.content_sha) sha = services.content_sha(original);
    if (services.lifters != nullptr) lifter = services.lifters->get(emulate::pcode::Mode::X86Flat32);
}

const platforms::Decoder& Win32Lane::Context::decoder() const {
    const std::lock_guard<std::mutex> guard(lock);
    return decoders_.get(platforms::Arch::X86, 32, true);
}

std::pair<std::uint64_t, std::uint64_t> Win32Lane::Context::image_span(const stdfs::path& module) const {
    {
        const std::lock_guard<std::mutex> guard(lock);
        if (const auto it = spans_.find(module); it != spans_.end()) return it->second;
    }
    std::pair<std::uint64_t, std::uint64_t> span{0, 0};
    try {
        const formats::pe::Image image = formats::pe::Image::open(module);
        std::uint64_t top = 0;
        for (const formats::pe::Section& s : image.sections()) top = std::max<std::uint64_t>(top, std::uint64_t{s.virtual_address} + std::max(s.virtual_size, s.raw_size));
        const std::uint64_t base = image.optional_header().image_base;
        span = {base, base + top};
    } catch (const Error&) {   // a file that is not a PE image has no span: a run on it is not made from here
    }
    const std::lock_guard<std::mutex> guard(lock);
    spans_[module] = span;
    return span;
}

std::vector<std::pair<std::uint64_t, std::uint64_t>> Win32Lane::Context::writable(const stdfs::path& module) const {
    std::vector<std::pair<std::uint64_t, std::uint64_t>> out;
    try {
        const formats::pe::Image image = formats::pe::Image::open(module);
        const std::uint64_t base = image.optional_header().image_base;
        for (const formats::pe::Section& s : image.sections()) {
            if ((s.characteristics & 0x80000000u) != 0 && (s.characteristics & 0x20000000u) == 0) {
                out.emplace_back(base + s.virtual_address, base + s.virtual_address + std::max(s.virtual_size, s.raw_size));
            }
        }
    } catch (const Error&) {
        out.clear();
    }
    return out;
}

const std::vector<guided::MemInit>& Win32Lane::Context::data_filled() const {
    const std::lock_guard<std::mutex> guard(lock);
    if (filled_) return *filled_;
    // Every word of the original's writable sections a value of its own: the address of a landing pad, so one used as a function pointer is
    // called and returns, and which was called is seen. The emulator does not load a module's data, so its globals all read as zero: two of
    // them passed in each other's places, or a loop that clears a table it never reaches, look the same as the right code while both are zero.
    // (The file's own data is not used: its tables of initialisers would be run, which is the harness's doing, not the body's.)
    std::vector<guided::MemInit> out;
    try {
        const formats::pe::Image image = formats::pe::Image::open(original);
        const std::uint64_t base = image.optional_header().image_base;
        for (const formats::pe::Section& s : image.sections()) {
            if ((s.characteristics & 0x80000000u) == 0 || (s.characteristics & 0x20000000u) != 0 || s.virtual_size == 0) continue;   // writable data only
            const std::uint64_t at = base + s.virtual_address;
            Bytes data;
            const std::uint32_t words = std::min(s.virtual_size, Win32Lane::kFillMax) / 4;
            data.reserve(std::size_t{words} * 4);
            for (std::uint32_t i = 0; i < words; ++i) {
                const std::uint32_t pad = Win32Lane::kSafe + 4 * static_cast<std::uint32_t>(((at >> 2) + i) % 0x400);
                for (int b = 0; b < 4; ++b) data.push_back(static_cast<std::uint8_t>(pad >> (8 * b)));
            }
            out.push_back(guided::MemInit{at, std::move(data)});
        }
    } catch (const Error&) {
        out.clear();
    }
    filled_ = std::move(out);
    return *filled_;
}

// ---- what the original's code says of a function -------------------------------------------------------------------------------------

namespace {

// One decoded instruction, as much of it as the lane reads.
struct Decoded {
    std::string mnemonic;
    std::uint8_t size = 0;
    int op_count = 0;
    int first_type = 0;                 // X86_OP_*
    std::int64_t first_imm = 0;
    std::uint32_t first_base = 0, first_index = 0;
    std::vector<cs_x86_op> operands;
};

Decoded decoded_of(const cs_insn& c) {
    Decoded d;
    d.mnemonic = c.mnemonic;
    d.size = static_cast<std::uint8_t>(c.size);
    if (c.detail != nullptr) {
        d.op_count = c.detail->x86.op_count;
        for (int k = 0; k < d.op_count; ++k) d.operands.push_back(c.detail->x86.operands[k]);
        if (d.op_count > 0) {
            d.first_type = d.operands[0].type;
            d.first_imm = d.operands[0].imm;
            d.first_base = d.operands[0].mem.base;
            d.first_index = d.operands[0].mem.index;
        }
    }
    return d;
}

std::vector<Decoded> decode_all(const platforms::Decoder& decoder, ByteView code, std::uint64_t address) {
    std::vector<Decoded> out;
    decoder.each(code, address, 0, [&](const cs_insn& c) {
        out.push_back(decoded_of(c));
        return true;
    });
    return out;
}

// The instruction at the start of `code`, or none when it does not decode.
std::optional<Decoded> decode_one(const platforms::Decoder& decoder, ByteView code, std::uint64_t address) {
    std::optional<Decoded> out;
    decoder.each(code, address, 1, [&](const cs_insn& c) {
        out = decoded_of(c);
        return false;
    });
    return out;
}

// How many instructions the function has: those reachable from its entry through its own jumps (a decode to the next known function also
// counted what lies after the function's last jump or return -- the rest of an import-thunk table, padding, the next function's data -- and
// so called a one-instruction thunk 2% covered). Where the function jumps through a register or a table, its reach is not known: then every
// instruction to the end of its extent.
std::int64_t reachable_insns(const platforms::Decoder& decoder, ByteView code, std::uint64_t start) {
    const std::uint64_t lo = start, hi = start + code.size();
    std::set<std::uint64_t> seen;
    std::vector<std::uint64_t> todo{start};
    while (!todo.empty()) {
        std::uint64_t a = todo.back();
        todo.pop_back();
        while (lo <= a && a < hi && !seen.contains(a)) {
            const std::size_t at = static_cast<std::size_t>(a - lo);
            const std::optional<Decoded> ins = decode_one(decoder, code.subspan(at, std::min<std::size_t>(16, code.size() - at)), a);
            if (!ins) break;
            seen.insert(a);
            const std::string& mn = ins->mnemonic;
            const bool has_operand = ins->op_count > 0;
            if (mn == "ret" || mn == "retf" || mn == "iret" || mn == "iretd" || mn == "hlt" || mn == "int3" || mn == "ud2") break;
            if (mn == "jmp" || (mn.starts_with('j') && has_operand)) {
                if (has_operand && ins->first_type == X86_OP_IMM) {
                    todo.push_back(static_cast<std::uint64_t>(ins->first_imm) & 0xFFFFFFFFu);
                } else if (mn == "jmp" && !(has_operand && ins->first_type == X86_OP_MEM && ins->first_base == 0 && ins->first_index == 0)) {
                    // through a register or a table: where it goes is not known here
                    return static_cast<std::int64_t>(decode_all(decoder, code, start).size());
                }
                if (mn == "jmp") break;
            }
            a += ins->size;
        }
    }
    return static_cast<std::int64_t>(seen.size());
}

}  // namespace

std::int64_t Win32Lane::Context::insn_count(std::uint64_t func) const {
    {
        const std::lock_guard<std::mutex> guard(lock);
        if (const auto it = insns_.find(func); it != insns_.end()) return it->second;
    }
    std::int64_t n = 0;
    try {
        auto lease = tracers.acquire();
        const emu::ResolvedFunction f = lease.get().resolve(fs::to_text(original), emu::FuncSelector{func});
        const ByteView code(f.module->code.data() + f.off, f.end - f.off);
        n = reachable_insns(decoder(), code, f.va());
    } catch (const Error&) {   // a function the tracer cannot find has no count: the coverage limit then says nothing
        n = 0;
    }
    const std::lock_guard<std::mutex> guard(lock);
    insns_[func] = n;
    return n;
}

const Win32Lane::Context::Harvest& Win32Lane::Context::harvest(std::uint64_t func) const {
    {
        const std::lock_guard<std::mutex> guard(lock);
        if (const auto it = harvest_.find(func); it != harvest_.end()) return it->second;
    }
    Harvest out;
    try {
        const auto [lo, hi] = image_span(original);
        auto lease = tracers.acquire();
        emu::Tracer& tracer = lease.get();
        const platforms::Decoder& dec = decoder();
        std::set<std::uint64_t> iat;
        // The function first, then the functions it calls: a caller often cannot get past `if (!helper(x)) return` until the input satisfies
        // what the HELPER tests.
        std::vector<std::pair<std::uint64_t, std::size_t>> todo{{func, 0}};
        std::set<std::uint64_t> seen;
        std::size_t next = 0;
        while (next < todo.size() && seen.size() < Win32Lane::kHarvestFunctions) {
            const auto [f, depth] = todo[next++];
            if (!seen.insert(f).second) continue;
            std::shared_ptr<const emu::LoadedModule> module;
            emu::ResolvedFunction rf;
            try {
                rf = tracer.resolve(fs::to_text(original), emu::FuncSelector{f});
                module = rf.module;
            } catch (const Error&) {   // a callee the tracer cannot find is not looked into
                continue;
            }
            if (iat.empty()) {
                for (const auto& [slot, name] : module->imports) iat.insert(slot);
            }
            const ByteView code(module->code.data() + rf.off, rf.end - rf.off);
            for (const Decoded& ins : decode_all(dec, code, rf.va())) {
                if (ins.mnemonic == "call" && depth < Win32Lane::kHarvestDepth && ins.op_count > 0 && ins.first_type == X86_OP_IMM) {
                    todo.emplace_back(static_cast<std::uint64_t>(ins.first_imm) & 0xFFFFFFFFu, depth + 1);
                    continue;
                }
                for (const cs_x86_op& op : ins.operands) {
                    if (op.type == X86_OP_IMM && (ins.mnemonic == "cmp" || ins.mnemonic == "test" || ins.mnemonic == "sub" || ins.mnemonic == "and")) {
                        const std::uint32_t v = static_cast<std::uint32_t>(op.imm);
                        if (v != 0 && std::ranges::find(out.constants, v) == out.constants.end()) out.constants.push_back(v);
                    } else if (op.type == X86_OP_MEM && op.mem.base == 0 && op.mem.index == 0) {
                        const std::uint32_t a = static_cast<std::uint32_t>(op.mem.disp);
                        if (lo <= a && a < hi && !iat.contains(a) && std::ranges::find(out.globals, a) == out.globals.end() && ins.mnemonic != "lea")
                            out.globals.push_back(a);
                    }
                }
            }
        }
    } catch (const Error&) {   // what could not be read is not harvested
    }
    if (out.constants.size() > 10) out.constants.resize(10);
    if (out.globals.size() > 24) out.globals.resize(24);
    const std::lock_guard<std::mutex> guard(lock);
    return harvest_.emplace(func, std::move(out)).first->second;
}

// ---- one run -----------------------------------------------------------------------------------------------------------------------

namespace win32 {
namespace {

std::string bytes_hex(ByteView data) { return crypto::to_hex(crypto::sha1(data)); }

// What a run is a function of, as text: its arguments, memory, what imports return, the registers at entry, the step limit.
std::string describe_vector(const Vector& v) {
    std::string out = "args";
    for (const std::int64_t a : v.args) out += std::format(" {}", static_cast<std::uint64_t>(a) & 0xFFFFFFFFu);
    if (v.mem_init) {
        for (const guided::MemInit& m : *v.mem_init) out += std::format(" mem {:x}:{}:{}", m.address, m.data.size(), bytes_hex(m.data));
    }
    out += std::format(" ret {}", v.stub_ret.value_or(0));
    if (v.sentinels) {
        for (const std::uint32_t s : *v.sentinels) out += std::format(" sentinel {:x}", s);
    }
    if (v.ecx) out += std::format(" ecx {:x}", *v.ecx);
    if (v.edx) out += std::format(" edx {:x}", *v.edx);
    if (v.room) out += std::format(" room {}", *v.room);
    return out;
}

bool plain() { return lane_common::plain(); }

emu::TraceOptions trace_options(const Context& context, const Vector& v, bool branches, emulate::native_shadow::Shadow* shadow,
                                const std::optional<stdfs::path>& overlay) {
    emu::TraceOptions o;
    std::vector<std::uint32_t> args;
    for (const std::int64_t a : v.args) args.push_back(static_cast<std::uint32_t>(a));
    o.args = std::move(args);
    if (v.mem_init) {
        for (const guided::MemInit& m : *v.mem_init) o.mem_init.push_back(emu::MemoryInit{static_cast<std::uint32_t>(m.address), m.data});
    }
    o.insn_cap = v.room.value_or(emu::kMaxInsns);
    o.stub_externals = true;
    o.stub_ret = v.stub_ret.value_or(0);
    if (overlay) o.overlay = fs::to_text(*overlay);
    o.sentinels = v.sentinels;
    o.reads = false;
    o.stop_wild = true;
    o.quiet_stack = true;
    o.reuse = !plain();
    o.fast = !plain() && shadow == nullptr;
    o.lean = !plain();
    o.ecx = v.ecx;
    o.edx = v.edx;
    o.pads = emulate::native_trace::AddressRange{Win32Lane::kSafe, Win32Lane::kSafe + 0x1000};
    o.branches = branches;
    o.shadow = shadow;
    // How a run enters the stubs: as imports, each with its argument count. The same for a fingerprint run and a branch-logged one, so that
    // the inputs aimed at branches are chosen on the program the judge runs.
    for (const auto& [address, name] : context.stubs.at) o.stub_at.push_back(emulate::native_trace::Sentinel{address, {"THUNK", name}});
    o.argc = [&context](const emulate::native_trace::ImportName& n) -> std::optional<std::int64_t> {
        if (n.dll == "THUNK") {
            const auto it = context.stubs.argc.find(n.name);
            return it != context.stubs.argc.end() ? it->second : std::nullopt;
        }
        return context.import_argc ? context.import_argc(n.dll, n.name) : std::nullopt;
    };
    return o;
}

std::shared_ptr<const emu::Trace> trace_of(const emulate::Run& run) { return std::any_cast<std::shared_ptr<const emu::Trace>>(run.raw); }

// The memory a function left outside its own frame: a digest of the final value of every byte it wrote (lane_common::left). A function that
// stores to a global or through a pointer argument is judged on what it stored and where -- the return value alone says nothing about either. A
// byte left holding what it held on entry is not an effect: one compiler clears a flag with a 16-bit `and`, another with an 8-bit one, and
// memory ends up the same.
//
// `own` is the address range of a REBUILT image: writes into it are that build's own statics (compiler temporaries, register stand-ins), which
// the original has no counterpart for. The original's globals are reached by their literal addresses on both sides, so they compare. A stored
// stack address is recorded as such (its value differs by build). A call through a pointer that resolves to nothing lands in zero bytes; the
// oracle returns from it after one `add [eax], al`: that write is the harness's (the record says so), not the function's. None: too many
// writes were made to record them all.
std::optional<std::string> effects(const emulate::Run& run, const std::optional<std::pair<std::uint64_t, std::uint64_t>>& own) {
    const auto in_frame = [&](std::uint64_t a) {
        return (Win32Lane::kStackLow <= a && a < Win32Lane::kStackHigh) || (own && own->first <= a && a < own->second);
    };
    lane_common::ChangedOptions options;
    options.place = [&](std::uint64_t a) -> lane_common::Where {
        if (in_frame(a)) return lane_common::Skip{};
        return lane_common::Place(a);
    };
    options.word = [&](std::uint64_t value) -> std::optional<std::string> { return in_frame(value) ? std::optional<std::string>("S") : std::nullopt; };
    return lane_common::left(run, options);
}

// (own, reach): offsets of the function's own instructions that ran, and the addresses of every instruction that ran anywhere in the module
// (the function and the callees it entered).
std::pair<std::vector<std::int64_t>, std::vector<std::int64_t>> coverage(const emulate::Run& run) {
    const emu::Trace& t = *trace_of(run);
    const std::uint64_t lo = t.func_va, hi = t.func_end_va;
    std::vector<std::int64_t> own, reach;
    for (const std::uint64_t a : run.path) {
        reach.push_back(static_cast<std::int64_t>(a));
        if (lo <= a && a < hi) own.push_back(static_cast<std::int64_t>(a - lo));
    }
    std::ranges::sort(own);
    std::ranges::sort(reach);
    if (reach.size() > Win32Lane::kMaxReach) reach.resize(Win32Lane::kMaxReach);
    return {std::move(own), std::move(reach)};
}

// The imported-call part of a fingerprint: every call in order, its name as both builds spell it, toolchain noise left out, and each argument as
// lane_common::argument writes it -- which input it is, a small constant, or a stack address or an address inside the rebuilt image ("S": those
// differ by build; a rebuilt body's locals live in its stand-in frame, inside its own image); otherwise the value itself. Reconstructed code
// refers to the original's data by its literal address, so a pointer into the original image is the same number on both sides and is compared as
// one: passing the wrong string or the wrong table is a difference.
std::vector<std::string> call_sequence(const emulate::Run& run, const std::vector<std::int64_t>& inputs,
                                       const std::optional<std::pair<std::uint64_t, std::uint64_t>>& own) {
    lane_common::ArgumentContext arg;
    arg.inputs = inputs;
    arg.small = true;
    arg.frame.push_back(lane_common::AddressRange{Win32Lane::kStackLow, Win32Lane::kStackHigh});
    if (own) arg.frame.push_back(lane_common::AddressRange{static_cast<std::int64_t>(own->first), static_cast<std::int64_t>(own->second)});
    else arg.frame.emplace_back(std::nullopt);
    const std::vector<lane_common::Called> calls = lane_common::called(run);
    return lane_common::calls(
        calls, [&](std::int64_t a) { return lane_common::argument(a & 0xFFFFFFFFLL, arg); },
        [](const std::optional<std::string>& name) { return Win32Lane::normalise_import(name.value_or("?")); },
        [](const std::optional<std::string>& name) { return name && Win32Lane::is_noise(*name); }, Win32Lane::kMaxCalls);
}

// One uncached run as a fingerprint (an error is a fingerprint with `err` set; a missing module or tool goes on: a broken setup is not an
// observation).
compare::Fingerprint trace_fingerprint(const Context& context, const stdfs::path& module, std::uint64_t func, const Vector& spec,
                                       const std::optional<stdfs::path>& rebuilt_of) {
    const StageTimer timer(rebuilt_of ? "run.rebuilt (uncached)" : "run.original (uncached)");
    emulate::Run run;
    try {
        auto lease = context.tracers.acquire();
        run = lease.get().recorded(fs::to_text(module), emu::FuncSelector{func}, trace_options(context, spec, false, nullptr, rebuilt_of));
    } catch (const NotFoundError&) {
        throw;
    } catch (const Error& e) {   // one run of many: a trace that failed is no observation, and is never kept
        compare::Fingerprint fp;
        fp.err = std::string(e.kind());
        return fp;
    }
    // A call that landed in zero bytes went through a pointer nothing in this environment fills in (a 32->16 thunk entry the loader would set, an
    // uninitialised callback). What the ORIGINAL does after that is the harness's doing, not the function's: the run is not an observation. A
    // REBUILT body that does it where the original did not has simply gone somewhere wrong.
    const emu::Trace& trace = *trace_of(run);
    const bool wild = trace.record.bogus_stubs.value_or(0) != 0 || trace.record.ran_into_zeros;
    if (wild && !rebuilt_of) {
        compare::Fingerprint fp;
        fp.err = "call-into-nothing";
        return fp;
    }
    std::optional<std::pair<std::uint64_t, std::uint64_t>> own;
    if (rebuilt_of) own = context.image_span(module);
    const std::vector<std::int64_t> inputs = spec.given ? *spec.given : spec.args;
    lane_common::FingerprintParts parts;
    if (const emulate::Register* eax = run.results.find("EAX"); eax != nullptr && std::holds_alternative<std::uint64_t>(*eax))
        parts.ret = static_cast<std::int64_t>(std::get<std::uint64_t>(*eax));
    parts.writes = effects(run, own);
    parts.saved = run.kept();
    if (!rebuilt_of) {
        auto [cov, reach] = coverage(run);
        parts.cov = std::move(cov);
        parts.reach = std::move(reach);
    }
    parts.steps = run.steps ? std::optional<std::int64_t>(static_cast<std::int64_t>(*run.steps)) : std::nullopt;
    compare::Fingerprint fp = lane_common::fingerprint(run, call_sequence(run, inputs, own), std::move(parts));
    if (wild) fp.exit = std::string(compare::kExitWildCall);
    return fp;
}

}  // namespace

std::string vector_text(const Vector& v) { return describe_vector(v); }

compare::Fingerprint fingerprint(const Context& context, const stdfs::path& module, std::uint64_t func, const Vector& spec,
                                 const std::optional<stdfs::path>& rebuilt_of) {
    // Kept across runs under the module's content hash: a run is a pure function of the module's bytes, the function and the input, and the
    // original's is the same for every rebuild. A run cut off at the step limit is kept too: a run stops on counts alone, the same on every
    // machine, so a truncated run is as settled as any other -- and they are the slowest to make again. (A rebuilt DLL has no hash of its own.)
    std::optional<std::string> key;
    if (!rebuilt_of && context.sha) {
        key = compare::cache_key_text(std::format("win32 {} {} {} {:x} | {}", Win32Lane::kRev, compare::kRevision, *context.sha, func, describe_vector(spec)));
    }
    return lane_common::remembered(context.fingerprints, key, [&] { return trace_fingerprint(context, module, func, spec, rebuilt_of); });
}

std::optional<emulate::Run> observed_run(const Context& context, std::uint64_t func, const Vector& spec, emulate::native_shadow::Shadow* shadow) {
    const StageTimer timer("run.observed (branch log)");
    emulate::Run run;
    try {
        auto lease = context.tracers.acquire();
        run = lease.get().recorded(fs::to_text(context.original), emu::FuncSelector{func}, trace_options(context, spec, true, shadow, std::nullopt));
    } catch (const NotFoundError&) {
        throw;
    } catch (const Error&) {   // a run that fails is not an observation
        return std::nullopt;
    }
    const emu::Trace& trace = *trace_of(run);
    if (trace.record.bogus_stubs.value_or(0) != 0 || trace.record.ran_into_zeros) return std::nullopt;
    return run;
}

guided::Observation observation_of(const emulate::Run& run) {
    const emu::Trace& trace = *trace_of(run);
    guided::Observation o;
    for (const emulate::native_trace::BranchEntry& b : trace.record.branches) {
        guided::Branch branch{b.address, b.taken, std::nullopt};
        if (b.compare) {
            guided::Compare c;
            c.op = guided::parse_compare_op(emulate::native_trace::to_text(b.compare->op));
            c.size = static_cast<unsigned>(b.compare->size);
            c.a = b.compare->a;
            c.b = b.compare->b;
            c.a_at = b.compare->at_a;
            c.b_at = b.compare->at_b;
            branch.compare = c;
        }
        o.branches.push_back(branch);
    }
    for (const std::int64_t a : coverage(run).second) o.reach.insert(static_cast<std::uint64_t>(a));
    o.args_at = static_cast<std::uint64_t>(trace.entry_esp) + 4;
    o.stack_low = Win32Lane::kStackLow;
    o.stack_high = Win32Lane::kStackHigh;
    return o;
}

std::optional<guided::Observation> branch_run(const Context& context, std::uint64_t func, const Vector& spec) {
    const std::optional<emulate::Run> run = observed_run(context, func, spec);
    return run ? std::optional<guided::Observation>(observation_of(*run)) : std::nullopt;
}

}  // namespace win32

}  // namespace cb::verify::lanes
