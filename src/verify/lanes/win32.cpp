// cb-unity: alone
// verify/lanes/win32.cpp -- the 32-bit judge lane as a Kind: its profile, the module prepared and built, and the verdict of one function
// (see win32.hpp).

#include "verify/lanes/win32.hpp"

#include <algorithm>
#include <format>
#include <iterator>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/decompiler.hpp"
#include "formats/pe_functions.hpp"
#include "verify/lanes/win32_internal.hpp"
#include "verify/stage_timer.hpp"

namespace cb::verify::lanes {

namespace stdfs = std::filesystem;

namespace {

std::string hex_key(std::uint64_t va) { return std::format("{:x}", va); }

// The file's size and modification time: what tells one build of a tool from another.
std::string file_stamp(const stdfs::path& file) {
    std::error_code ec;
    const auto size = stdfs::file_size(file, ec);
    if (ec) return "";
    const auto time = cb::fs::last_write_time(file, ec);
    return std::format("{}:{}:{}", file.filename().string(), size, ec ? 0 : time.time_since_epoch().count());
}

}  // namespace

Win32Lane::Win32Lane(Services services) : services_(std::move(services)) {}
Win32Lane::~Win32Lane() = default;

const std::string& Win32Lane::name() const {
    static const std::string kName = "pe";
    return kName;
}

reconstruct::KindTraits Win32Lane::traits() {
    reconstruct::KindTraits t;
    t.per_function = true;   // a verdict holds while the function and what it reaches are unchanged
    return t;
}

std::vector<std::uint32_t> Win32Lane::functions(const stdfs::path& module_path) {
    const formats::pe_functions::PeCarve carve = formats::pe_functions::PeCarve::open(module_path);
    std::vector<std::uint32_t> out;
    for (const auto& [start, function] : carve.functions()) out.push_back(start);   // (the map's order: ascending)
    return out;
}

// ---- the profile -----------------------------------------------------------------------------------------------------------------

Profile Win32Lane::profile(const stdfs::path&) const {
    // Everything OTHER than a body's own source that a verdict depends on: the pipeline and oracle revisions, the inputs each body is run on, the
    // compiler and its flags. The digest is part of every body's signature, so changing any of these re-verifies the corpus without anyone having to
    // remember to bump a number; the settings are stored beside it.
    emulate::pcode::Lifter* lifter = services_.lifters != nullptr ? services_.lifters->get(emulate::pcode::Mode::X86Flat32) : nullptr;
    const bool paths = lane_common::guided_on() && lane_common::concolic_on() && lifter != nullptr;
    std::string settings = std::format("win32 rev {} compare {} max_calls {} max_support {} directed {} {} guided {} {}", kRev, compare::kRevision, kMaxCalls,
                                       kMaxSupport, kMaxDirected, kHarvestDepth, lane_common::kGuidedBudget, lane_common::kMaxGuided);
    // (and whether the paths are explored, and with what: the lifter and its specification)
    settings += std::format(" paths {} {} {} {} {}", paths, lane_common::kPathBudget, lane_common::kMaxPaths, lane_common::kGuidedRest,
                            paths && services_.lifters != nullptr ? file_stamp(services_.lifters->config().exe) : std::string());
    settings += " inputs";
    for (const Vector& v : vectors()) settings += " [" + win32::vector_text(v) + "]";
    settings += " confirm";
    for (const Vector& v : confirm_vectors()) settings += " [" + win32::vector_text(v) + "]";
    settings += " cflags";
    for (const std::string_view flag : decompile::c_text::cflags()) settings += std::format(" {}", flag);
    settings += " runtime";
    for (const std::string& line : services_.tools.runtime_lines) settings += std::format(" {}", line);
    // (not the decompiler's revision: a verdict is for a text, whichever revision wrote it)
    settings += std::format(" compiler {}", file_stamp(services_.tools.wcc.exe));
    return {crypto::to_hex(crypto::sha1(as_bytes(settings))).substr(0, 12), settings};
}

// ---- the module as source --------------------------------------------------------------------------------------------------------

std::unique_ptr<Prepared> Win32Lane::prepare(const stdfs::path& module_path, const stdfs::path& work) const {
    auto p = std::make_unique<PreparedModule>();
    const std::vector<std::uint32_t> addresses = functions(module_path);
    const std::vector<decompile::c_text::Va> starts(addresses.begin(), addresses.end());
    p->module = decompile::c_text::compilable32(services_.decompiler, module_path, starts, work);
    const decompile::c_text::Compilable32& m = p->module;
    p->header = m.preamble;
    p->order = m.order;
    for (const auto& [name, text] : m.texts) p->texts[name] = text;
    for (const auto& [va, name] : m.by_va) {
        p->va[name] = va;
        p->key[name] = hex_key(static_cast<std::uint64_t>(va));
    }
    for (const std::uint32_t va : addresses) p->all_keys.push_back(hex_key(va));
    std::ranges::sort(p->all_keys);
    // The module is one translation unit: any change to it may change any function's build.
    std::string all = std::to_string(decompile::decompiler::kRevision);
    all += '\0';
    all += p->header;
    all += '\0';
    for (const std::string& name : p->order) {
        if (const auto text = p->texts.find(name); text != p->texts.end()) all += text->second;
    }
    p->sig = crypto::to_hex(crypto::sha1(as_bytes(all)));
    return p;
}

// ---- the build -------------------------------------------------------------------------------------------------------------------

Built Win32Lane::build(const Prepared& prepared, const stdfs::path& module_path, const stdfs::path& work) const {
    const auto& p = static_cast<const PreparedModule&>(prepared);
    const decompile::c_text::Compilable32& m = p.module;
    auto ctx = std::make_shared<Context>(services_, module_path);

    // A flat thunk's stubs are the thunk compiler's, not C: not rebuilt, so a call to one goes to the original's (build32: a jump to it), which the
    // judge enters as an import.
    std::set<std::uint64_t> stub_addresses;
    if (services_.flat_thunk_stubs) {
        const formats::pe_functions::PeCarve carve = formats::pe_functions::PeCarve::open(module_path);
        for (const std::uint64_t va : services_.flat_thunk_stubs(module_path)) {
            stub_addresses.insert(va);
            const std::string stub_name = std::format("stub_{:x}", va);
            ctx->stubs.at[va] = stub_name;
            for (const formats::pe_functions::CodeSection& section : carve.sections()) {
                if (va < section.va || va >= section.end()) continue;
                const std::size_t at = va - section.va;
                for (const platforms::Insn& ins : ctx->decoder().disasm(section.data.subspan(at, std::min<std::size_t>(512, section.data.size() - at)), va)) {
                    if (ins.mnemonic != "ret" && ins.mnemonic != "retn") continue;
                    const std::int64_t n = ins.op_str.empty() ? 0 : std::stoll(ins.op_str, nullptr, 0);
                    ctx->stubs.argc[stub_name] = n % 4 == 0 && n <= 256 ? std::optional<std::int64_t>(n / 4) : std::nullopt;
                    break;
                }
                break;
            }
        }
    }

    // Only functions with a known address are built. The rest are library routines recognised by signature (__allshl, _memcpy): they belong to the
    // C runtime, which is linked.
    std::vector<std::string> placed;
    for (const std::string& name : p.order) {
        const auto va = p.va.find(name);
        if (va != p.va.end() && !stub_addresses.contains(static_cast<std::uint64_t>(va->second))) placed.push_back(name);
    }
    // where the functions the build lacks are jumped to: every function the module has, by the name the decompiler gives it
    std::map<std::string, std::uint64_t> originals;
    {
        const formats::pe_functions::PeCarve carve = formats::pe_functions::PeCarve::open(module_path);
        std::set<std::string> imported;
        for (const auto& [slot, import] : carve.imports()) imported.insert(import.name);
        const std::vector<std::uint32_t> addresses = functions(module_path);
        const std::vector<decompile::c_text::Va> starts(addresses.begin(), addresses.end());
        for (const auto& [va, name] : decompile::describe::function_names(carve.image(), starts, imported)) originals[name] = static_cast<std::uint64_t>(va);
    }
    build::functions::Texts texts;
    for (const auto& [name, text] : m.texts) texts[name] = text;
    std::vector<std::string> cflags;
    for (const std::string_view flag : decompile::c_text::cflags()) cflags.emplace_back(flag);
    const build::functions::Build32Input input{.header = m.preamble, .texts = texts, .order = placed, .outdir = work, .cflags = cflags, .originals = originals};
    build::functions::Tools32 tools = services_.tools;
    if (services_.import_lines_for) tools.import_lines = services_.import_lines_for(module_path, m.imports);
    const build::functions::Built32 b = services_.builder.build32(tools, input);

    Built built;
    std::map<std::string, Item> items;
    for (const auto& [name, why] : b.dropped) built.missing[name] = why;
    for (const std::string& name : placed) {
        if (built.missing.contains(name)) continue;
        const auto where = b.where.find(name);
        if (!b.dll || where == b.where.end()) {
            built.missing[name] = "not in the build";
            continue;
        }
        Item item;
        item.key = p.key.at(name);
        item.va = static_cast<std::uint32_t>(p.va.at(name));
        item.dll = *b.dll;
        item.entry = where->second;
        item.regs = m.in_edx.contains(name) ? 2 : m.in_ecx.contains(name) ? 1 : 0;
        built.items.push_back(name);
        items[name] = std::move(item);
    }
    built.judge = [ctx, items = std::move(items)](const std::string& name) { return judge_function(*ctx, items.at(name)); };
    return built;
}

// ---- one function ----------------------------------------------------------------------------------------------------------------

store::Judgement Win32Lane::judge_function(const Context& context, const Item& item) {
    const StageTimer total("judge.function (total)");
    const store::Judgement made = lane_common::judged([&]() -> store::Judgement {
        const Shape shape = item.regs == 2 ? Shape(&Win32Lane::in_ecx_edx) : item.regs == 1 ? Shape(&Win32Lane::in_ecx) : Shape();
        const auto shaped = [&](const Vector& v) { return shape ? shape(v) : v; };

        const RunOriginal original = [&](const Vector& v) { return win32::fingerprint(context, context.original, item.va, shaped(v)); };
        const RunOriginal rebuilt = [&](const Vector& v) {
            // A rebuild may spend many more instructions than the original on the same work -- a C loop where the original filled memory with one
            // `rep stosd` (dnr.exe's memset over 64 KB: 16,418 steps for the original, 229,414 for the loop, the same 16,387 writes) -- and stop at
            // the step limit where the original returned. Such a run is tried again with room before it counts as different -- also where the
            // original itself stopped at the limit: the rebuild must reach every call the original made by then.
            compare::Fingerprint b = win32::fingerprint(context, item.dll, item.entry, shaped(v), context.original);
            if (b.truncated && b.exit != compare::kExitWildCall) {
                const compare::Fingerprint a = win32::fingerprint(context, context.original, item.va, shaped(v));
                if (a.exit == compare::kExitReturned || a.exit == compare::kExitTruncated) {
                    Vector roomy = shaped(v);
                    roomy.room = kRoom;
                    b = win32::fingerprint(context, item.dll, item.entry, roomy, context.original);
                }
            }
            return b;
        };

        std::shared_ptr<concolic::Stats> found;
        const std::vector<Vector> fixed = vectors();
        const std::vector<Vector> confirm = confirm_vectors();
        const compare::Outcome outcome = compare::compare<Vector>(
            original, rebuilt, fixed, confirm, {.insns = context.insn_count(item.va)},
            [&](const std::set<std::int64_t>&, const std::set<std::int64_t>& reached) { return win32::directed_and_guided(context, item.va, reached, shape, &found); },
            [](const RunOriginal& retail, const Vector& v, const compare::Fingerprint& a) { return undefined_ret_bits(retail, v, a); });
        std::vector<std::string> limits = outcome.limit_codes();
        if (outcome.verdict != store::Verdict::Divergent) {
            const std::vector<std::string> paths = lane_common::path_limits(found.get());
            limits.insert(limits.end(), paths.begin(), paths.end());
        }
        return {outcome.verdict, outcome.reason, limits, outcome.detail};
    });
    return made;
}

}  // namespace cb::verify::lanes
