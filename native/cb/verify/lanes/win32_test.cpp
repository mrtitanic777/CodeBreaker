// Tests of the 32-bit judge lane (win32), against what the Python lane (codebreaker/verify/lanes/win32.py) answered.
//
// The inputs: the fixed vectors and the confirm vectors, a member function's shaping, the names of imports a fingerprint compares, which
// are noise, and the bits of a return value that are leftovers -- each compared with the Python's value (testdata/win32/inputs.json,
// made by running the module's functions).
//
// The judge, on real modules: lz32.dll (19 flat-thunk stubs, imports of KERNEL32) and the Windows 95 version.dll (a flat thunk too, a larger
// one). The Python lane decompiled and rebuilt each with OpenWatcom (the rebuilt DLL is testdata/win32/<module>/rebuilt.dll) and judged every
// function it built, 20 and 32 of them; this runs the C++ judge on the same original and the same rebuilt DLL and compares verdict, reason,
// limits (coverage, paths=N / paths-all, argc-unknown) and the line saying what differed, function by function. What the Python asked of its
// environment -- how many arguments each import takes, the headers' declaration of each import it stubbed, which functions are the thunk
// compiler's stubs -- is recorded beside it (verdicts.json) and given here as answers: the lane's own work is what is compared. The fingerprint
// cache is off on both sides.
//
// They need the symbolic lifter (native/decompiler: pcodecheck.exe and its languages) and the emulator's libraries (unicorn, z3, cbtrace);
// without the lifter they say so and pass.
//
// DIVERGES from win32.py:
//   * the cache key of an original's fingerprint is made from text, not the Python's repr of a tuple (nothing reads the old keys: a C++ cache
//     is rebuilt once);
//   * the profile's digest is made from this lane's own settings text (the revision, the comparer's, the inputs, the flags, the compiler's
//     stamp), not the Python's JSON; results filed under the old digest are made again once;
//   * a trace that fails is a fingerprint with `err` naming the error's kind (the Python's exception class name); a missing module goes on;
//   * the thunk stubs of a module come from Services (the symbol tables' recognition of the thunk compiler's functions), not from a module-level
//     table keyed by path.
#include "cb/verify/lanes/win32.hpp"

#include <algorithm>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/emulate/pcode.hpp"
#include "cb/formats/pe.hpp"
#include "cb/symbols/prototypes.hpp"
#include "cb/verify/lanes/win32_internal.hpp"

namespace {

using cb::Json;
using cb::verify::lanes::Win32Lane;
using Vector = Win32Lane::Vector;
namespace stdfs = std::filesystem;

stdfs::path data(const std::string& relative) { return cb::testing::data_path("verify/lanes/testdata/win32/" + relative); }

// ---- the inputs ------------------------------------------------------------------------------------------------------------------

void check_vector(const Vector& v, const Json& want) {
    std::vector<std::int64_t> args = want["args"].get<std::vector<std::int64_t>>();
    CB_CHECK(v.args == args);
    CB_CHECK_EQ(v.stub_ret.value_or(0), want["stub_ret"].get<std::int64_t>());
    CB_CHECK_EQ(v.mem_init ? v.mem_init->size() : std::size_t(0), want["mem"].size());
    if (!v.mem_init || v.mem_init->size() != want["mem"].size()) return;
    for (std::size_t i = 0; i < want["mem"].size(); ++i) {
        const Json& m = want["mem"][i];
        const cb::verify::guided::MemInit& got = (*v.mem_init)[i];
        CB_CHECK_EQ(got.address, m[0].get<std::uint64_t>());
        CB_CHECK_EQ(got.data.size(), m[1].get<std::size_t>());
        CB_CHECK_EQ(cb::crypto::to_hex(cb::crypto::sha1(got.data)), m[2].get<std::string>());
    }
}

Json inputs() { return cb::json::load(data("inputs.json")); }

}  // namespace

CB_TEST("verify.win32.fixed_vectors", [] {
    const Json want = inputs();
    const std::vector<Vector> got = Win32Lane::vectors();
    CB_CHECK_EQ(got.size(), want["vectors"].size());
    for (std::size_t i = 0; i < got.size() && i < want["vectors"].size(); ++i) check_vector(got[i], want["vectors"][i]);
});

CB_TEST("verify.win32.confirm_vectors", [] {
    const Json want = inputs();
    const std::vector<Vector> got = Win32Lane::confirm_vectors();
    CB_CHECK_EQ(got.size(), want["confirm"].size());
    for (std::size_t i = 0; i < got.size() && i < want["confirm"].size(); ++i) check_vector(got[i], want["confirm"][i]);
});

CB_TEST("verify.win32.member_function_shapes", [] {
    const Json want = inputs();
    Vector v;
    v.args = {11, 22, 33, 44};
    const Vector a = Win32Lane::in_ecx(v);
    const Json& one = want["in_ecx_full"];
    CB_CHECK_EQ(std::int64_t(*a.ecx), one["ecx"].get<std::int64_t>());
    CB_CHECK(a.args == one["args"].get<std::vector<std::int64_t>>());
    CB_CHECK(a.given && *a.given == one["given"].get<std::vector<std::int64_t>>());
    CB_CHECK(!a.edx);
    const Vector b = Win32Lane::in_ecx_edx(v);
    const Json& two = want["in_ecx_edx_full"];
    CB_CHECK_EQ(std::int64_t(*b.ecx), two["ecx"].get<std::int64_t>());
    CB_CHECK_EQ(std::int64_t(*b.edx), two["edx"].get<std::int64_t>());
    CB_CHECK(b.args == two["args"].get<std::vector<std::int64_t>>());
    CB_CHECK(b.given && *b.given == two["given"].get<std::vector<std::int64_t>>());
    // shaping twice is shaping once: a directed input is shaped when it is made and again when it is run
    CB_CHECK(Win32Lane::in_ecx(a).args == a.args);
    CB_CHECK(Win32Lane::in_ecx_edx(b).args == b.args);
});

CB_TEST("verify.win32.import_names", [] {
    const Json want = inputs();
    for (const Json& pair : want["norm"]) CB_CHECK_EQ(Win32Lane::normalise_import(pair[0].get<std::string>()), pair[1].get<std::string>());
    for (const Json& pair : want["noise"]) CB_CHECK_EQ(Win32Lane::is_noise(pair[0].get<std::string>()), pair[1].get<bool>());
});

CB_TEST("verify.win32.return_value_leftovers", [] {
    using cb::verify::compare::Fingerprint;
    for (const Json& c : inputs()["leftovers"]) {
        std::size_t asked = 0;
        std::vector<std::uint32_t> sentinels;
        const auto make = [](std::int64_t ret, const std::string& exit, const std::vector<std::string>& deps) {
            Fingerprint f;
            f.ret = ret;
            f.exit = exit;
            f.deps = deps;
            return f;
        };
        const Fingerprint a = make(c["a_ret"].get<std::int64_t>(), c["a_exit"].get<std::string>(), c["a_deps"].get<std::vector<std::string>>());
        const Win32Lane::RunOriginal retail = [&](const Vector& v) {
            ++asked;
            if (v.sentinels) sentinels.assign(v.sentinels->begin(), v.sentinels->end());
            return make(c["c_ret"].get<std::int64_t>(), c["c_exit"].get<std::string>(), c["c_deps"].get<std::vector<std::string>>());
        };
        Vector v;
        v.args = {1};
        CB_CHECK_EQ(std::int64_t(Win32Lane::undefined_ret_bits(retail, v, a)), c["bits"].get<std::int64_t>());
        CB_CHECK_EQ(asked, c["asked"].get<std::size_t>());
        if (asked != 0) {
            // the saved registers hold the opposite bits of what they hold at entry
            const std::vector<std::int64_t> want = c["sentinels"].get<std::vector<std::int64_t>>();
            CB_CHECK_EQ(sentinels.size(), want.size());
            for (std::size_t i = 0; i < sentinels.size() && i < want.size(); ++i) CB_CHECK_EQ(std::int64_t(sentinels[i]), want[i]);
        }
    }
});

CB_TEST("verify.win32.guided_round_trip", [] {
    for (Vector v : Win32Lane::confirm_vectors()) {
        v.ecx = 0x1234;
        v.edx = 0x5678;
        v.given = std::vector<std::int64_t>{1, 2, 3, 4};
        v.room = 99;
        v.sentinels = std::array<std::uint32_t, 4>{1, 2, 3, 4};
        const Vector back = Win32Lane::from_guided(Win32Lane::to_guided(v));
        CB_CHECK(back.args == v.args);
        CB_CHECK(back.stub_ret == v.stub_ret);
        CB_CHECK(back.mem_init == v.mem_init);
        CB_CHECK(back.ecx == v.ecx && back.edx == v.edx);
        CB_CHECK(back.given == v.given);
        CB_CHECK(back.room == v.room);
        CB_CHECK(back.sentinels == v.sentinels);
    }
});

// What a symbolic exploration sets: an argument, ECX, what the imports return, bytes of memory (runs of neighbours made one).
CB_TEST("verify.win32.derive", [] {
    using cb::verify::concolic::InputVar;
    Vector v;
    v.args = {10, 20, 30, 40};
    v.ecx = 7;
    v.given = std::vector<std::int64_t>{7, 10, 20, 30, 40};
    cb::verify::concolic::Vars names;
    names["a1"] = {InputVar::Place::Arg, 1, 32, 20};
    names["rc"] = {InputVar::Place::Reg, 1, 32, 7};
    names["r0"] = {InputVar::Place::Ret, 0, 32, 0};
    names["m0"] = {InputVar::Place::Mem, 0x206000, 8, 0};
    names["m1"] = {InputVar::Place::Mem, 0x206001, 8, 0};
    names["m2"] = {InputVar::Place::Mem, 0x206010, 8, 0};
    names["late"] = {InputVar::Place::Arg, 9, 32, 0};   // an argument the function does not have: ignored
    // nothing differs from what the input already is
    CB_CHECK(!Win32Lane::derive(v, {{"a1", 20}, {"rc", 7}, {"m0", 0}}, names, {}));
    const auto w = Win32Lane::derive(v, {{"a1", 0x1'0000'0005ull}, {"rc", 8}, {"r0", 1}, {"m0", 0xAB}, {"m1", 0xCD}, {"m2", 0xEF}, {"late", 5}}, names, {});
    CB_CHECK(w.has_value());
    if (!w) return;
    CB_CHECK_EQ(w->args[1], std::int64_t(5));      // masked to 32 bits
    CB_CHECK_EQ(std::int64_t(*w->ecx), std::int64_t(8));
    CB_CHECK_EQ(w->stub_ret.value_or(0), std::int64_t(1));
    CB_CHECK(w->mem_init.has_value());
    if (w->mem_init) {
        CB_CHECK_EQ(w->mem_init->size(), std::size_t(2));   // 206000-206001 as one run, 206010 as another
        CB_CHECK_EQ((*w->mem_init)[0].address, std::uint64_t(0x206000));
        CB_CHECK((*w->mem_init)[0].data == (cb::Bytes{0xAB, 0xCD}));
        CB_CHECK_EQ((*w->mem_init)[1].address, std::uint64_t(0x206010));
    }
    CB_CHECK(w->given && (*w->given)[0] == 8 && (*w->given)[2] == 5);   // what the calls' arguments are named by follows
    // the inputs are the shaped ones: a derived input of a member function keeps its ECX
    Vector plain;
    plain.args = {1, 2, 3, 4};
    const auto shaped = Win32Lane::derive(plain, {{"a1", 9}}, names, Win32Lane::in_ecx);
    CB_CHECK(shaped.has_value() && shaped->ecx.has_value());
});

// ---- the judge on real modules ---------------------------------------------------------------------------------------------------

namespace {

namespace emu = cb::emulate::emulator;
namespace c_text = cb::decompile::c_text;

// The decompiler is not asked by a judge: whatever asks it is a mistake of the test.
class NoDecompiler final : public c_text::Services {
public:
    [[noreturn]] static void no() { throw cb::UsageError("the decompiler is not part of this test"); }
    c_text::Decompiled raw(const stdfs::path&, std::span<const c_text::Va>, const stdfs::path&) override { no(); }
    c_text::Decompiled16 decompile16(const stdfs::path&, const std::optional<stdfs::path>&) override { no(); }
    std::optional<std::string> c_from_l2(const std::optional<std::string>&, cb::core16::FunctionNotes&, const std::optional<c_text::SegmentImages>&,
                                         c_text::ModuleFacts*) override { no(); }
    bool recovers_structures(const cb::decompile::describe::NeDescription&) override { no(); }
    bool tree_images() override { no(); }
    c_text::SegmentImages segment_images(const cb::decompile::describe::NeDescription&) override { no(); }
    const cb::translate::native_l2::Library& library() override { no(); }
    std::map<std::string, std::string> import_dlls(const stdfs::path&) override { no(); }
    std::optional<std::uint32_t> argument_bytes(std::string_view, std::string_view) override { no(); }
    std::optional<std::vector<cb::decompile::struct_text::Structure>> recover_structures(const stdfs::path&) override { no(); }
    cb::decompile::describe::PowerPcFacts powerpc_facts(const stdfs::path&) override { no(); }
};

std::string word_of(cb::verify::Verdict v) { return std::string(cb::verify::word(v)); }

// One module: every function the Python lane judged, judged again here.
void check_module(const std::string& folder, const stdfs::path& original) {
    const stdfs::path decompiler_bin = cb::testing::data_path("../decompiler/build/bin");
    const stdfs::path languages = cb::testing::data_path("../decompiler/languages");
    cb::emulate::pcode::LifterSet lifters(cb::emulate::pcode::PcodeConfig::from_environment(decompiler_bin, languages));
    if (lifters.get(cb::emulate::pcode::Mode::X86Flat32) == nullptr) {
        std::cerr << "win32 tests: no x86 lifter (" << decompiler_bin.string() << "); skipped\n";
        return;
    }
    const Json golden = cb::json::load(data(folder + "/verdicts.json"));
    const Json& env = golden["environment"];
    std::map<std::string, std::optional<std::int64_t>> argc;
    for (const auto& [key, value] : env["argc"].items()) argc[key] = value.is_null() ? std::nullopt : std::optional<std::int64_t>(value.get<std::int64_t>());
    std::map<std::string, cb::symbols::prototypes::Prototype> prototypes;
    for (const auto& [name, params] : env["prototypes"].items()) {
        cb::symbols::prototypes::Prototype p;
        for (const Json& param : params) p.parameters.push_back({param[0].get<std::string>(), param[1].get<std::string>()});
        prototypes[name] = std::move(p);
    }

    NoDecompiler decompiler;
    cb::build::functions::FunctionBuilder builder;
    Win32Lane::Services services{decompiler, builder, {}, {}, nullptr, nullptr, &lifters, {}, nullptr};
    services.tracer.resolve = [](std::string_view name) { return stdfs::path(std::string(name)); };
    services.tracer.prototype = [&prototypes](std::string_view name) -> std::optional<cb::symbols::prototypes::Prototype> {
        const auto it = prototypes.find(std::string(name));
        return it != prototypes.end() ? std::optional(it->second) : std::nullopt;
    };
    services.import_argc = [&argc](std::string_view dll, std::string_view name) -> std::optional<std::int64_t> {
        const auto it = argc.find(std::string(dll) + "|" + std::string(name));
        return it != argc.end() ? it->second : std::nullopt;
    };

    cb::verify::lanes::Win32Lane::Context context(services, original);
    for (const auto& [hex, name] : golden["stubs"].items()) context.stubs.at[std::stoull(hex, nullptr, 16)] = name.get<std::string>();
    for (const auto& [name, n] : golden["stub_argc"].items()) context.stubs.argc[name] = n.is_null() ? std::nullopt : std::optional<std::int64_t>(n.get<std::int64_t>());

    std::size_t judged = 0, matched = 0;
    for (const auto& [name, want] : golden["items"].items()) {
        Win32Lane::Item item;
        item.key = want["key"].get<std::string>();
        item.va = want["va"].get<std::uint32_t>();
        item.dll = data(folder + "/rebuilt.dll");
        item.entry = want["entry"].get<std::uint64_t>();
        item.regs = want["regs"].get<int>();
        const cb::verify::Judgement got = Win32Lane::judge_function(context, item);
        ++judged;
        const std::vector<std::string> want_limits = want["limits"].get<std::vector<std::string>>();
        const bool same = word_of(got.verdict) == want["verdict"].get<std::string>() && got.reason == want["reason"].get<std::string>() &&
                          got.limits == want_limits && got.detail == want["detail"].get<std::string>();
        if (same) ++matched;
        else {
            std::string limits;
            for (const std::string& l : got.limits) limits += l + ",";
            cb::testing::count_check();
            cb::testing::record_failure(__FILE__, __LINE__,
                                        std::format("{}.{}: got {} '{}' [{}] '{}'\n    want {} '{}' [{}] '{}'", folder, name, word_of(got.verdict), got.reason, limits,
                                                    got.detail, want["verdict"].get<std::string>(), want["reason"].get<std::string>(),
                                                    cb::text::join(want_limits, ","), want["detail"].get<std::string>()));
        }
    }
    CB_CHECK_EQ(judged, golden["items"].size());
    CB_CHECK_EQ(matched, judged);
}

}  // namespace


CB_TEST("verify.win32.judge_lz32", [] { check_module("lz32", cb::testing::data_path("formats/testdata/lz32.dll")); });
CB_TEST("verify.win32.judge_version", [] { check_module("version", cb::testing::data_path("build/testdata/flat_thunk/version.dll")); });

// The module's functions are the carve's starts, ascending (what the lane prepares, and what every verdict's key is made from).
CB_TEST("verify.win32.functions_of_a_module", [] {
    const Json want = inputs()["functions"];
    const std::vector<std::uint32_t> lz32 = Win32Lane::functions(cb::testing::data_path("formats/testdata/lz32.dll"));
    const std::vector<std::uint32_t> version = Win32Lane::functions(cb::testing::data_path("build/testdata/flat_thunk/version.dll"));
    CB_CHECK(std::vector<std::uint64_t>(lz32.begin(), lz32.end()) == want["lz32"].get<std::vector<std::uint64_t>>());
    CB_CHECK(std::vector<std::uint64_t>(version.begin(), version.end()) == want["version"].get<std::vector<std::uint64_t>>());
    CB_CHECK_THROWS_AS(cb::FormatError, (void)Win32Lane::functions(data("inputs.json")));   // not a PE image
});

// A run asks for the argument count of each import it meets, thousands of times a module: the knowledge behind it is asked once per import (a
// judge of version.dll took 3 s instead of 10 s for it).
CB_TEST("verify.win32.an_imports_argument_count_is_asked_once", [] {
    NoDecompiler decompiler;
    cb::build::functions::FunctionBuilder builder;
    Win32Lane::Services services{decompiler, builder, {}, {}, nullptr, nullptr, nullptr, {}, nullptr};
    int asked = 0;
    services.import_argc = [&asked](std::string_view dll, std::string_view name) -> std::optional<std::int64_t> {
        ++asked;
        if (name == "CloseHandle") return 1;
        (void)dll;
        return std::nullopt;   // (an unknown count is an answer too)
    };
    const Win32Lane::Context context(services, "lz32.dll");
    for (int i = 0; i < 5; ++i) {
        CB_CHECK(context.import_argc("KERNEL32", "CloseHandle") == std::optional<std::int64_t>(1));
        CB_CHECK(!context.import_argc("KERNEL32", "NoSuchCall").has_value());
    }
    CB_CHECK_EQ(asked, 2);
    CB_CHECK(!context.import_argc("USER32", "CloseHandle").has_value() || asked == 3);   // (the module is part of the question)
});

// What the registry is told, and the profile every verdict's signature carries.
CB_TEST("verify.win32.the_kind", [] {
    NoDecompiler decompiler;
    cb::build::functions::FunctionBuilder builder;
    Win32Lane::Services services{decompiler, builder, {}, {}, nullptr, nullptr, nullptr, {}, nullptr};
    Win32Lane lane(services);
    CB_CHECK_EQ(lane.name(), "pe");
    CB_CHECK(Win32Lane::traits().per_function);
    const cb::verify::lanes::Profile a = lane.profile("lz32.dll"), b = lane.profile("version.dll");
    CB_CHECK_EQ(a.digest.size(), std::size_t(12));
    CB_CHECK_EQ(a.digest, b.digest);   // one profile for the kind
    CB_CHECK(a.settings.starts_with("win32 rev 48 compare "));
    CB_CHECK(a.settings.find("paths false ") != std::string::npos);   // no lifter: the inputs are guessed, and the profile says so
    // the inputs are part of it: a profile is what a verdict depends on besides the source
    CB_CHECK(a.settings.find("[args 0 0 0 0 ret 0]") != std::string::npos);
});

// ---- the shadow and the path explorer on LZ32 (tests/test_concolic.py) ---------------------------------------------------------------------
//
// The lifter is pcodecheck as the pcode tests find it (the environment's, else native/decompiler/build/bin of the checkout, a worktree's build being
// in the main one); without it these say so and pass.

namespace {

stdfs::path w32sh_pcodecheck() {
    if (auto env = cb::fs::get_env("CB_PCODECHECK"); env && !env->empty()) return *env;
    stdfs::path dir = stdfs::path(CB_SOURCE_DIR);
    for (int i = 0; i < 6 && !dir.empty(); ++i, dir = dir.parent_path()) {
        const stdfs::path exe = dir / "native" / "decompiler" / "build" / "bin" / "pcodecheck.exe";
        if (stdfs::exists(exe)) return exe;
    }
    return {};
}

// LZ32 as the judge has it: its lifter, the imports' argument counts and the thunk stubs the Python lane recorded.
struct W32ShRig {
    cb::emulate::pcode::LifterSet lifters;
    NoDecompiler decompiler;
    cb::build::functions::FunctionBuilder builder;
    std::map<std::string, std::optional<std::int64_t>> argc;
    Win32Lane::Services services;
    std::unique_ptr<Win32Lane::Context> context;
    stdfs::path module = cb::testing::data_path("formats/testdata/lz32.dll");

    explicit W32ShRig(const stdfs::path& exe)
        : lifters(cb::emulate::pcode::PcodeConfig::from_environment(exe.parent_path(), stdfs::path(CB_SOURCE_DIR) / "decompiler" / "languages")),
          services{decompiler, builder, {}, {}, nullptr, nullptr, &lifters, {}, nullptr} {
        const Json golden = cb::json::load(data("lz32/verdicts.json"));
        for (const auto& [key, value] : golden["environment"]["argc"].items())
            argc[key] = value.is_null() ? std::nullopt : std::optional<std::int64_t>(value.get<std::int64_t>());
        services.tracer.resolve = [](std::string_view name) { return stdfs::path(std::string(name)); };
        services.import_argc = [this](std::string_view dll, std::string_view name) -> std::optional<std::int64_t> {
            const auto it = argc.find(std::string(dll) + "|" + std::string(name));
            return it != argc.end() ? it->second : std::nullopt;
        };
        context = std::make_unique<Win32Lane::Context>(services, module);
        for (const auto& [hex, name] : golden["stubs"].items()) context->stubs.at[std::stoull(hex, nullptr, 16)] = name.get<std::string>();
        for (const auto& [name, n] : golden["stub_argc"].items())
            context->stubs.argc[name] = n.is_null() ? std::nullopt : std::optional<std::int64_t>(n.get<std::int64_t>());
    }

    bool ready() { return lifters.get(cb::emulate::pcode::Mode::X86Flat32) != nullptr; }
    // The exports (name, address) of the module.
    std::vector<std::pair<std::string, std::uint64_t>> exports() const {
        const cb::formats::pe::Image image = cb::formats::pe::Image::open(module);
        std::vector<std::pair<std::string, std::uint64_t>> out;
        for (const cb::formats::pe::Export& e : image.exports())
            out.emplace_back(e.name.value_or("#" + std::to_string(e.ordinal)), image.optional_header().image_base + e.rva);
        return out;
    }
};

// What the judge compares of a run: its end, steps, the result, the calls out, the stack, the registers kept, the instructions that ran.
std::string w32sh_seen(const cb::emulate::Run& r) {
    std::string out = std::format("{} {} ", r.exit.value_or("-"), r.steps.value_or(0));
    if (const cb::emulate::Register* eax = r.results.find("EAX"); eax != nullptr && std::holds_alternative<std::uint64_t>(*eax))
        out += std::format("eax={:x} ", std::get<std::uint64_t>(*eax));
    for (const cb::emulate::Call& c : r.calls) {
        out += c.name.value_or("?") + "(";
        if (c.args) for (const std::int64_t a : *c.args) out += std::format("{:x},", a);
        out += ");";
    }
    out += std::format(" stack={} kept={} |", r.stack.value_or(-1), r.kept());
    for (const cb::emulate::MemoryWrite& w : r.writes) out += std::format(" {:x}/{}/{:x}", w.address, w.size, w.value);
    out += " |";
    std::vector<std::uint64_t> path = r.path;
    std::sort(path.begin(), path.end());
    for (const std::uint64_t a : path) out += std::format(" {:x}", a);
    return out;
}

}  // namespace

// The 32-bit shadow only watches: LZ32's exports traced with it (one instruction at a time) and without it (the fast tracer) give the judge the
// same run.
CB_TEST("verify.win32.a_32_bit_run_with_its_shadow_is_the_same_run", [] {
    const stdfs::path exe = w32sh_pcodecheck();
    if (exe.empty()) {
        std::cerr << "win32 shadow test: no pcodecheck (native/decompiler); skipped\n";
        return;
    }
    W32ShRig rig(exe);
    if (!rig.ready()) {
        std::cerr << "win32 shadow test: no x86 flat lift; skipped\n";
        return;
    }
    const std::vector<Vector> fixed = Win32Lane::vectors(), confirm = Win32Lane::confirm_vectors();
    std::vector<Vector> seeds(fixed.begin() + 1, fixed.end());
    for (std::size_t i = 0; i < 3 && i < confirm.size(); ++i) seeds.push_back(confirm[i]);
    const auto exports = rig.exports();
    CB_CHECK(exports.size() >= 8);
    std::size_t compared = 0, observed = 0;
    for (std::size_t k = 0; k < 8 && k < exports.size(); ++k) {
        for (const Vector& v : seeds) {
            const auto fast = cb::verify::lanes::win32::observed_run(*rig.context, exports[k].second, v, nullptr);
            cb::emulate::native_shadow::Shadow shadow(cb::emulate::native_shadow::Setup::for_lifter(
                cb::emulate::native_shadow::Kind::Flat32, *rig.lifters.get(cb::emulate::pcode::Mode::X86Flat32), {{0x200000, 0x210000}}));
            const auto watched = cb::verify::lanes::win32::observed_run(*rig.context, exports[k].second, v, &shadow);
            CB_CHECK_EQ(fast.has_value(), watched.has_value());
            if (fast && watched) {
                CB_CHECK_EQ(w32sh_seen(*fast), w32sh_seen(*watched));
                ++observed;
            }
            ++compared;
        }
    }
    CB_CHECK(compared >= 8 * 5);
    CB_CHECK(observed > compared / 2);   // most runs are observations: the equality is not between two nothings
});

// LZOpenFileA through the x86-32-flat p-code: its paths found, said to be all, and each input kept takes one no seed took.
CB_TEST("verify.win32.the_explorer_finds_every_path_of_lzopenfile", [] {
    const stdfs::path exe = w32sh_pcodecheck();
    if (exe.empty()) {
        std::cerr << "win32 explorer test: no pcodecheck (native/decompiler); skipped\n";
        return;
    }
    W32ShRig rig(exe);
    if (!rig.ready()) {
        std::cerr << "win32 explorer test: no x86 flat lift; skipped\n";
        return;
    }
    std::uint64_t va = 0;
    for (const auto& [name, address] : rig.exports())
        if (name == "LZOpenFileA") va = address;
    CB_CHECK(va != 0);
    std::vector<Vector> seeds = Win32Lane::vectors();
    for (Vector& v : Win32Lane::confirm_vectors()) seeds.push_back(std::move(v));
    const auto [kept, got] = cb::verify::lanes::win32::explore_vectors(*rig.context, va, seeds, {});
    CB_CHECK(got != nullptr);
    if (!got) return;
    CB_CHECK(got->complete);
    CB_CHECK(got->paths >= 4);
    CB_CHECK(!kept.empty());
    const auto trail_of = [&](const Vector& v) {
        std::vector<std::pair<std::uint64_t, bool>> t;
        const auto o = cb::verify::lanes::win32::branch_run(*rig.context, va, v);
        if (o) for (const auto& b : o->branches) t.emplace_back(b.address, b.taken);
        return t;
    };
    std::set<std::vector<std::pair<std::uint64_t, bool>>> trails;
    for (const Vector& v : seeds) trails.insert(trail_of(v));
    for (const Vector& v : kept) CB_CHECK(!trails.contains(trail_of(v)));
});
