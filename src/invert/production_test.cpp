// Tests of the production environment of `codebreaker invert` (invert/production): the functions hook reads a real module through the describer, the batch
// pass puts every reading to a compiler in one call per switch set, and the hooks whose tool is not set up stay empty.
//
// CB_INVERT_REAL="<word>;<word>;..." runs the command with those words in the production environment of the checkout CB_INVERT_CHECKOUT names (else the
// one this program runs from) -- the real compilers -- and prints what it said and how long it took, for timing.
#include "invert/production.hpp"

#include "build/dos_tools.hpp"
#include "build/msc6.hpp"

#include <chrono>
#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

using namespace cb;
namespace inv = cb::invert::command;
namespace mp = cb::invert::module_pass;

namespace {

std::string data(const char* name) { return fs::to_text(testing::data_path(std::string("invert/testdata/") + name)); }

// A checkout with no settings at all: no compiler is set up.
config::Config bare_config(const fs::ScopedTempDir& dir) { return config::Config::load(dir.path()); }

}  // namespace

CB_TEST("invert.production.nothing_set_up_binds_no_compiler", [] {
    const fs::ScopedTempDir dir;
    const inv::Environment e = inv::production(bare_config(dir));
    CB_CHECK(!e.compile_msc6);
    CB_CHECK(!e.compile_msc6_many);
    CB_CHECK(!e.compile_msvc152);
    CB_CHECK(!e.compile_msvc152_many);
    CB_CHECK(!e.compile_msvc2);
    CB_CHECK(!e.compile_mc203);
    CB_CHECK(static_cast<bool>(e.retail));   // (the corpus of the configuration, here an empty one)
    CB_CHECK(!e.retail("no_such_module.dll"));
    CB_CHECK(static_cast<bool>(e.functions));
});

CB_TEST("invert.production.the_functions_hook_reads_a_real_module", [] {
    const fs::ScopedTempDir dir;
    const inv::Environment e = inv::production(bare_config(dir));
    const std::vector<mp::Function> functions = e.functions(fs::from_text(data("LZEXPAND.DLL")));
    CB_CHECK(functions.size() > 10);
    std::size_t with_fixups = 0;
    for (const mp::Function& f : functions) {
        CB_CHECK(!f.name.empty());
        CB_CHECK(!f.code.empty());
        for (const std::size_t at : f.fixups) CB_CHECK(at < f.code.size());
        if (!f.fixups.empty()) ++with_fixups;
    }
    CB_CHECK(with_fixups > 0);   // a module's far calls are relocated
    // in code order within a segment, each function ending where the next one starts
    for (std::size_t i = 1; i < functions.size(); ++i)
        if (functions[i].segment == functions[i - 1].segment) CB_CHECK(functions[i].offset >= functions[i - 1].offset + functions[i - 1].code.size());
    CB_CHECK_THROWS_AS(FormatError, (void)e.functions(fs::from_text(data("HELLO.OBJ"))));
});

CB_TEST("invert.production.the_batch_pass_asks_a_compiler_once_per_switch_set", [] {
    const fs::ScopedTempDir dir;
    const inv::Environment e = inv::production(bare_config(dir));
    const std::vector<mp::Function> functions = e.functions(fs::from_text(data("LZEXPAND.DLL")));
    std::size_t calls = 0, texts_given = 0;
    std::vector<std::string> sets;
    const mp::CompileMany refuses = [&](std::span<const std::string> texts, std::string_view flags) {
        ++calls;
        texts_given += texts.size();
        sets.emplace_back(flags);
        return std::vector<std::optional<Bytes>>(texts.size());
    };
    const auto results = mp::run_batched(functions, refuses, refuses);
    CB_CHECK_EQ(results.size(), functions.size());
    CB_CHECK(calls >= 1);
    CB_CHECK(calls <= 8);   // (a call per switch set and compiler, not per text)
    CB_CHECK(texts_given >= calls);
    for (const auto& [name, r] : results) CB_CHECK(r.verdict != mp::Verdict::Identical);
    // with no compiler at all the readings stay what the models made of them
    const auto without = mp::run_batched(functions, {}, {});
    CB_CHECK_EQ(without.size(), functions.size());
});

CB_TEST("invert.production.a_compiler_that_agrees_makes_a_function_identical", [] {
    const fs::ScopedTempDir dir;
    const inv::Environment e = inv::production(bare_config(dir));
    const std::vector<mp::Function> functions = e.functions(fs::from_text(data("LZEXPAND.DLL")));
    // a compiler that gives each function its own bytes back: the first reading of every function the models read is then identical
    std::map<std::string, const mp::Function*> by_text;
    const auto read = mp::run_batched(functions, {}, {});
    for (const mp::Function& f : functions)
        for (const std::string& t : read.at(f.name).tries) by_text.emplace(t, &f);
    const mp::CompileMany oracle = [&](std::span<const std::string> texts, std::string_view) {
        std::vector<std::optional<Bytes>> out;
        for (const std::string& t : texts) {
            const auto it = by_text.find(t);
            out.push_back(it == by_text.end() ? std::nullopt : std::optional<Bytes>(it->second->code));
        }
        return out;
    };
    const auto results = mp::run_batched(functions, oracle, oracle);
    std::size_t identical = 0;
    for (const auto& [name, r] : results)
        if (r.verdict == mp::Verdict::Identical) ++identical;
    CB_CHECK(identical > 0);
    for (const auto& [name, r] : results)
        if (r.verdict == mp::Verdict::Identical) CB_CHECK(!r.flags.empty());
});

CB_TEST("invert.production.real", [] {
    const std::optional<std::string> proof = fs::get_env("CB_INVERT_REAL");
    if (!proof || proof->empty()) return;
    const std::optional<std::string> root = fs::get_env("CB_INVERT_CHECKOUT");
    const config::Config config = root && !root->empty() ? config::Config::load(fs::from_text(*root)) : config::current();
    std::vector<std::string> args;
    for (const std::string_view piece : text::split(*proof, ';')) args.emplace_back(piece);
    const inv::Environment e = inv::production(config);
    const auto started = std::chrono::steady_clock::now();
    const int code = inv::run(args, e);
    std::cout << "exit " << code << ", " << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << " s\n";
});

// Microsoft C 6.0a on the DOS executive gives the bytes the emulator's gives (skipped where the compiler, the emulator or the executive is not set up).
CB_TEST("invert.production.msc6_on_the_executive_gives_the_emulators_bytes", [] {
    const std::optional<std::string> root = fs::get_env("CB_INVERT_CHECKOUT");
    if (!root || root->empty()) return;
    const config::Config config = config::Config::load(fs::from_text(*root));
    const inv::Environment e = inv::production(config);
    if (!e.compile_msc6_many) return;
    const std::vector<std::string> texts = {
        "int far pascal f(int a, int b) { return a + b; }\n",
        "long far pascal g(long x) { return x * 3L; }\n",
        "void far pascal h(char far *p) { while (*p) *p++ = 0; }\n",
        "int far pascal broken(int a) { return a +; }\n",   // the compiler refuses it
    };
    for (const std::string_view flags : {"/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw"}) {
        const std::vector<std::optional<Bytes>> native = e.compile_msc6_many(texts, flags);
        CB_CHECK_EQ(native.size(), texts.size());
        CB_CHECK(native[0].has_value() && native[1].has_value() && native[2].has_value());
        CB_CHECK(!native[3].has_value());
        // the emulator's answer for the same texts
        cb::build::msc6::Tools tools;
        tools.dosbox_x = config.path("dosbox_x").value_or("");
        tools.msc6 = config.path("msc6").value_or("");
        if (!cb::build::msc6::available(tools)) continue;
        const cb::build::dos_tools::DosboxRunner runner{cb::build::dos_tools::Emulator::from_config(config)};
        std::vector<cb::build::msc6::Source> sources;
        for (const std::string& t : texts) sources.push_back({t, std::nullopt});
        const fs::ScopedTempDir work;
        cb::build::msc6::CompileOptions options;
        options.work = work.path();
        const auto emulated = cb::build::msc6::compile_many({tools, runner, nullptr}, sources, "", flags, options);
        for (std::size_t i = 0; i < texts.size(); ++i) {
            CB_CHECK_EQ(native[i].has_value(), emulated[i].has_value());
            if (native[i] && emulated[i]) CB_CHECK(*native[i] == emulated[i]->code);
        }
    }
});
