// Tests of the production environment of `codebreaker invert` (invert/production): the functions hook reads a real module through the describer, the batch
// pass puts every reading to a compiler in one call per switch set, and the hooks whose tool is not set up stay empty.
//
// CB_INVERT_REAL="<word>;<word>;..." runs the command with those words in the production environment of the checkout CB_INVERT_CHECKOUT names (else the
// one this program runs from) -- the real compilers -- and prints what it said and how long it took, for timing.
#include "invert/production.hpp"

#include <chrono>
#include <iostream>

#include "core/crypto.hpp"
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

// Microsoft C 6.0a on the DOS executive gives the code DOSBox-X gave for the same texts (its length and SHA-1 were recorded from DOSBox-X on
// 2026-11-21, when the executive was proven against it; skipped where the compiler or the executive is not set up).
CB_TEST("invert.production.msc6_on_the_executive_gives_the_recorded_bytes", [] {
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
    struct Recorded {
        std::size_t size;
        const char* sha1;
    };
    const struct {
        std::string_view flags;
        Recorded code[3];
    } recorded[] = {
        {"/AM /G2 /Gsw /Ox",
         {{30, "c1d5a6a7ba5cc05fe212aad270e15889ed407840"},
          {42, "6d08ddece25ed3aff0877a05eb4265b0ca3b3734"},
          {46, "ed3ecbbabe80ee28142b80bfcc94b304d1abfd1b"}}},
        {"/AM /G2 /Gsw",
         {{26, "71c0a34a66d7f3d5bd9017db811386ffddf00b76"},
          {38, "52940528997470bbbf08b47de76dfa85eb5b5c2a"},
          {38, "d8e585f050e2d803f1495cdfdf230758dc644995"}}},
    };
    for (const auto& set : recorded) {
        const std::vector<std::optional<Bytes>> native = e.compile_msc6_many(texts, set.flags);
        CB_CHECK_EQ(native.size(), texts.size());
        if (native.size() != texts.size()) continue;
        CB_CHECK(!native[3].has_value());
        for (std::size_t i = 0; i < 3; ++i) {
            CB_CHECK(native[i].has_value());
            if (!native[i]) continue;
            CB_CHECK_EQ(native[i]->size(), set.code[i].size);
            CB_CHECK_EQ(crypto::to_hex(crypto::sha1(*native[i])), std::string(set.code[i].sha1));
        }
    }
});
