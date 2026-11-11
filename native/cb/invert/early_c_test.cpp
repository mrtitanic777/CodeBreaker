// Tests of cb/invert/early_c.hpp. The compiler model itself is native/invert's; these pin the typed API over it: the
// settings and hooks reaching the model, the findings and results coming back, the text helpers, and the errors.
//
// testdata/early_c.golden.json holds 397 calls of recover / recover_cfg with the answers the Python module gave: the
// default-settings call for each input (the ten seed functions of the oracle harness -- an add, calls, a max, a loop, a
// copy chain, a long, a switch, near/far/Pascal calls -- junk and one-byte inputs, a load base, call names) and, for every
// setting and hook, the calls whose answer it changes, so each entry documents what a setting does; plus a spread of
// mutated and truncated inputs. The hooks are the harness's: SWITCH answers at addresses divisible by 6, CALLINFO and
// CALLREGS answer by a formula of the call's address and target.
//
// DIVERGES from the Python: registers a statement writes are listed in the fixed order ax bx cx dx si di es, where the
// Python's followed the hash of each register name in its process (so two runs could differ); a hook that throws
// propagates its exception, where the Python's ctypes callback swallowed it as "no answer"; errors are
// FormatError/UnsupportedError/InternalError, not the Python's exception names; to_c builds its local declarations with
// early_c_frame, where the model carried a copy of that logic.
#include "cb/invert/early_c.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
namespace ec = cb::invert::early_c;
namespace frame = cb::invert::early_c_frame;

namespace {

using Offsets = std::vector<frame::Offset>;

Bytes from_hex(std::string_view hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(*text::parse_int<std::uint8_t>(hex.substr(i, 2), 16));
    return out;
}

// The seed functions of the harness (small MC 2.03 functions).
const Bytes kAdd = from_hex("558bec8b46040346065dc3");
const Bytes kMax = from_hex("558bec8b46043b46067e058b4604eb038b46065dc3");
const Bytes kLoop = from_hex("558bec83ec04c746fe0000c746fc0000eb0b8b46fe0146fcff46fe8b46fe3b46047cf08b46fc8be55dc3");
const Bytes kCopy = from_hex("558bec83ec06c746fe01008b46fe8946fc8b46fc8946fa8b46fa8be55dc3");

std::vector<std::string> strs(const Json& j) {
    std::vector<std::string> out;
    for (const Json& s : j) out.push_back(s.get<std::string>());
    return out;
}

// The harness's deterministic hooks.
ec::Hooks harness_hooks(const std::string& which) {
    ec::Hooks h;
    if (which.find('s') != std::string::npos) {
        h.switch_targets = [](std::uint32_t address) -> std::optional<std::vector<std::uint32_t>> {
            if (address % 6 == 0) return std::vector<std::uint32_t>{address + 2, address + 8};
            return std::nullopt;
        };
    }
    if (which.find('i') != std::string::npos) {
        h.callee_pop_bytes = [](const ec::CallSite& call) -> std::uint32_t {
            std::uint32_t address = 0, target = 0;
            if (const auto* near = std::get_if<ec::NearCall>(&call)) {
                if (!near->target) return 0;
                address = near->address, target = *near->target;
            } else {
                const auto& far = std::get<ec::FarCall>(call);
                if (!far.target) return 0;
                address = far.address, target = far.target->seg + far.target->off;
            }
            const std::uint32_t n = (address + target) % 5;
            return n > 1 && n != 4 ? n * 2 : 0;   // (n == 4: the Python's hook raised, which the model took as no answer)
        };
    }
    if (which.find('r') != std::string::npos) {
        h.callee_register_args = [](const ec::CallSite& call) -> std::vector<ec::Reg> {
            const std::uint32_t address = std::visit([](const auto& c) { return c.address; }, call);
            switch (address % 4) {
                case 0: return {ec::Reg::Ax};
                case 1: return {ec::Reg::Ax, ec::Reg::Dx};
                default: return {};
            }
        };
    }
    return h;
}

ec::Options options_of(const Json& c) {
    ec::Options o;
    const Json& flags = c["flags"];
    auto flag = [&](const char* name, bool fallback) { return flags.contains(name) ? flags[name].get<bool>() : fallback; };
    o.mark = flag("MARK", false);
    o.far_pointers = flag("FARPTR", false);
    o.dead_registers = flag("DEADREGS", false);
    o.unsigned_compares = flag("UNSIGNED_CAST", false);
    o.fp_casts = flag("FPCASTS", false);
    o.eager_register_variables = flag("REGVARS_EAGER", false);
    o.frame_after_bp = flag("FRAME_AFTER_BP", false);
    o.reread_register_params = flag("REGPARAM_REREAD", true);
    o.compare_switches = flag("CMPSWITCH", false);
    const std::string returns = c["returns"].get<std::string>();
    if (returns != "N") o.returns_value = returns == "T";
    o.hooks = harness_hooks(c["hooks"].get<std::string>());
    return o;
}

std::unordered_map<std::uint32_t, std::string> names_of(const std::string& text) {   // "9=foo|20=bar"
    std::unordered_map<std::uint32_t, std::string> out;
    if (text.empty()) return out;
    for (const std::string_view pair : text::split(text, '|')) {
        const auto parts = text::split(pair, '=');
        out[*text::parse_int<std::uint32_t>(parts[0])] = std::string(parts[1]);
    }
    return out;
}

Json json_of(const ec::Recovered& r) {
    return Json::array({r.frame_size, r.params, r.statements, r.return_type, r.locals});
}

// What the Python's LAST held, as the harness recorded it, compared field by field (absent keys are empty).
bool findings_match(const Json& last, const ec::Findings& f) {
    if (strs(last.value("saved", Json::array())) != f.saved_registers) return false;
    std::vector<std::pair<std::string, std::uint32_t>> rp;
    for (const Json& kv : last.value("regparam", Json::array())) rp.emplace_back(kv[0].get<std::string>(), kv[1].get<std::uint32_t>());
    std::vector<std::pair<std::string, std::uint32_t>> have(f.register_params.begin(), f.register_params.end());
    if (rp != have) return false;
    std::vector<std::pair<std::string, std::string>> ft;
    for (const Json& kv : last.value("fptypes", Json::array())) ft.emplace_back(kv[0].get<std::string>(), kv[1].get<std::string>());
    std::vector<std::pair<std::string, std::string>> haveft(f.fp_types.begin(), f.fp_types.end());
    if (ft != haveft) return false;
    if (last.value("fpret", std::string()) != f.fp_return) return false;
    const auto ul = strs(last.value("unsigned_longs", Json::array()));
    return std::set<std::string>(ul.begin(), ul.end()) == f.unsigned_longs;
}

}  // namespace

CB_TEST("invert.early_c.golden_cases", [] {
    const Json cases = json::load(testing::data_path("invert/testdata/early_c.golden.json"));
    CB_CHECK_EQ(cases.size(), std::size_t{397});
    std::string failures;
    std::size_t errors = 0, answers = 0;
    for (const Json& c : cases) {
        const Bytes code = from_hex(c["code"].get<std::string>());
        const std::uint32_t base = c["base"].get<std::uint32_t>();
        const ec::Options options = options_of(c);
        const auto names = names_of(c["names"].get<std::string>());
        const bool structured = c["which"].get<int>() == 1;
        const std::string label = std::format("{} {} base {} names '{}' flags {} returns {} hooks {}", structured ? "recover_cfg" : "recover",
                                              c["code"].get<std::string>(), base, c["names"].get<std::string>(),
                                              c["flags"].dump(), c["returns"].get<std::string>(), c["hooks"].get<std::string>());
        try {
            const ec::Recovered r = structured ? ec::recover_cfg(code, base, names, options) : ec::recover(code, base, names, options);
            ++answers;
            if (c.contains("exc")) {
                failures += label + ": the Python raised " + c["exc"].get<std::string>() + "\n";
            } else if (json_of(r) != c["rec"]) {
                failures += label + "\n  got      " + json_of(r).dump() + "\n  expected " + c["rec"].dump() + "\n";
            } else if (!findings_match(c["last"], r.findings)) {
                failures += label + ": findings differ\n";
            }
        } catch (const Error& e) {
            ++errors;
            if (!c.contains("exc")) failures += label + ": threw " + e.what() + "\n";
        }
    }
    CB_CHECK(answers > 300);
    CB_CHECK(errors >= 1);
    CB_CHECK_EQ(failures, std::string());
});

CB_TEST("invert.early_c.seed_functions_default_settings", [] {
    // an add: both walkers give the same body
    for (const bool structured : {false, true}) {
        const ec::Recovered r = structured ? ec::recover_cfg(kAdd) : ec::recover(kAdd);
        CB_CHECK_EQ(r.frame_size, 0u);
        CB_CHECK((r.params == std::vector<std::string>{"a", "b"}));
        CB_CHECK((r.statements == std::vector<std::string>{"return (a + b);"}));
        CB_CHECK_EQ(r.return_type, "int");
        CB_CHECK(r.locals.empty());
    }
    // a max: the linear walker keeps the jumps' shape, the structured one a register variable
    const ec::Recovered linear = ec::recover(kMax);
    CB_CHECK((linear.statements == std::vector<std::string>{"if ((a > b)) {", "    return a;", "}", "return b;"}));
    const ec::Recovered cfg = ec::recover_cfg(kMax);
    CB_CHECK_EQ(cfg.statements.front(), "/* register variables: ax */");
    CB_CHECK_EQ(cfg.statements.back(), "return ax;");
    // a loop with two locals at frame offsets 2 and 4
    const ec::Recovered loop = ec::recover(kLoop);
    CB_CHECK((loop.locals == Offsets{2, 4}));
    CB_CHECK_EQ(loop.statements.front(), "v2 = 0;");
});

CB_TEST("invert.early_c.settings_and_hooks_reach_the_model", [] {
    // helper operators: a long multiply is a call of the library's helper at offset 18 (`e8 00 00` at 15); the driver says
    // that helper divides (the answers are the Python module's for the same input)
    const Bytes lmul = from_hex("558bec8b46048b5e068b4e088b560ae800008bc35dc3");
    CB_CHECK((ec::recover(lmul).statements == std::vector<std::string>{"return (a * b);"}));
    CB_CHECK((ec::recover(lmul, 0, {{18, "/"}}).statements == std::vector<std::string>{"return (a / b);"}));
    CB_CHECK((ec::recover_cfg(lmul, 0, {{18, "%"}}).statements == std::vector<std::string>{"return (a % b);"}));
    CB_CHECK((ec::recover(lmul, 0, {{99, "/"}}).statements == std::vector<std::string>{"return (a * b);"}));   // another helper
    CB_CHECK_EQ(ec::recover(lmul).return_type, "long");
    CB_CHECK((ec::recover(lmul, 0x100, {{0x112, "/"}}).statements == std::vector<std::string>{"return (a / b);"}));   // base included
    const Bytes call = from_hex("558bec8b46045068100090e8f9ff83c4048be55dc3");
    CB_CHECK((ec::recover(call).statements == std::vector<std::string>{"return sub_7(0x10, a);"}));
    // a hook is asked about the calls the walker meets
    std::vector<ec::CallSite> asked;
    ec::Options watch;
    watch.hooks.callee_pop_bytes = [&](const ec::CallSite& site) -> std::uint32_t {
        asked.push_back(site);
        return 0;
    };
    (void)ec::recover(call, 0, {}, watch);
    CB_CHECK(!asked.empty());
    CB_CHECK(std::holds_alternative<ec::NearCall>(asked.front()));
    if (const auto* near = std::get_if<ec::NearCall>(&asked.front())) {
        CB_CHECK_EQ(near->address, 11u);
        CB_CHECK(near->target.has_value() && *near->target == 7u);
    }
    // a hook's exception propagates
    ec::Options angry;
    angry.hooks.callee_pop_bytes = [](const ec::CallSite&) -> std::uint32_t { throw UsageError("the hook says no"); };
    CB_CHECK_THROWS_AS(UsageError, ec::recover(call, 0, {}, angry));
});

CB_TEST("invert.early_c.errors", [] {
    // A max function cut off in the middle of its jump, structured, with the harness's hook that names argument registers:
    // the model reads past the end (the Python's IndexError); here it is a FormatError.
    ec::Options registers;
    registers.hooks = harness_hooks("r");
    const Bytes cut = from_hex("558bec8b46043b46067e058b46");
    CB_CHECK_THROWS_AS(FormatError, ec::recover_cfg(cut, 100, {}, registers));
    // junk and empty input are not errors: the model answers with what it found (an empty function)
    CB_CHECK_NOTHROW(ec::recover(Bytes{}));
    CB_CHECK_NOTHROW(ec::recover_cfg(Bytes{0x90}));
    CB_CHECK(ec::recover(Bytes{}).statements.empty());
});

CB_TEST("invert.early_c.options_from_environment", [] {
    {
        fs::ScopedEnv a("MC2INV_FAR", "1"), b("MC2INV_DEADREGS", "0"), c("MC2INV_UNSIGNED", "1"), d("MASM_CRET", "yes"),
            e("CFG_NOCAP", "");
        const ec::Options o = ec::Options::from_environment();
        CB_CHECK(o.far_pointers);
        CB_CHECK(!o.dead_registers);
        CB_CHECK(o.unsigned_compares);
        CB_CHECK(o.caller_cleans);
        CB_CHECK(!o.uncapped_structures);
        CB_CHECK(!o.mark);
        CB_CHECK(o.reread_register_params);
    }
    CB_CHECK(!ec::Options::from_environment().far_pointers || fs::get_env("MC2INV_FAR").has_value());
});

CB_TEST("invert.early_c.text_helpers", [] {
    // copies: a live local-to-local copy chain, as (dest, src) offsets
    const auto copies = ec::detect_copies(std::vector<std::string>{"v0 = 1;", "v2 = v0;", "v4 = v2;"});
    CB_CHECK_EQ(copies.size(), std::size_t{2});
    CB_CHECK_EQ(copies[0].dest, 2u);
    CB_CHECK_EQ(copies[0].src, 0u);
    CB_CHECK_EQ(copies[1].dest, 4u);
    CB_CHECK_EQ(copies[1].src, 2u);
    const auto three = ec::detect_copies(std::vector<std::string>{"v0 = 1;", "v2 = v0;", "v6 = v2;", "v4 = v6;"});
    CB_CHECK_EQ(three.size(), std::size_t{3});
    CB_CHECK_EQ(three[2].dest, 4u);
    CB_CHECK_EQ(three[2].src, 6u);
    CB_CHECK(ec::detect_copies(std::vector<std::string>{"v0 = 1;", "v2 = v0;", "v0 = 5;"}).empty());   // src overwritten after
    CB_CHECK(ec::detect_copies(std::vector<std::string>{"v0 = 1;", "{", "v2 = v0;"}).empty());          // not straight-line
    CB_CHECK(ec::detect_copies({}).empty());

    // a 32-bit comparison's ladder (high words, then the unsigned low words) folds into one comparison
    const std::vector<std::string> ladder = {"if ((S16(a) <= S16(b))) {", "    if ((S16(a) >= S16(b))) {", "        if (((unsigned)c < d)) {",
                                             "            X();", "        } else {", "            Y();", "        }", "    } else {",
                                             "        X();", "    }", "} else {", "    Y();", "}"};
    CB_CHECK((ec::fold_ladders(ladder) == std::vector<std::string>{"if ((LONG(a, c) < LONG(b, d))) {", "    X();", "} else {", "    Y();", "}"}));
    CB_CHECK((ec::fold_ladders(ladder, true) ==
              std::vector<std::string>{"if ((S32(LONG(a, c)) < S32(LONG(b, d)))) {", "    X();", "} else {", "    Y();", "}"}));
    std::vector<std::string> not_a_ladder = ladder;
    not_a_ladder[2] = "        if (((unsigned)c >= d)) {";   // the bodies no longer agree with the comparison
    CB_CHECK(ec::fold_ladders(not_a_ladder) == not_a_ladder);
    CB_CHECK(ec::fold_ladders({}).empty());

    // local declarations: consecutive ints, gap fillers, longs, copies (a copy chain declares in reverse)
    CB_CHECK_EQ(ec::local_decls(6, Offsets{0, 2, 4}), "    int v0;\n    int v2;\n    int v4;\n");
    CB_CHECK_EQ(ec::local_decls(12, Offsets{0, 8}, {0, 8}), "    long v0;\n    char _p0[4];\n    long v8;\n");
    CB_CHECK_EQ(ec::local_decls(12, Offsets{0, 2, 4}, {}, std::vector<frame::Copy>{{.dest = 2, .src = 0}, {.dest = 4, .src = 2}}),
                "    int v4;\n    int v2;\n    int v0;\n");
    CB_CHECK_EQ(ec::local_decls(12, Offsets{0, 2, 4, 6}, {4}, std::vector<frame::Copy>{{.dest = 2, .src = 0}, {.dest = 6, .src = 4}}),
                "    int v0;\n    int v2;\n    long v4;\n    int v6;\n    char _p6[4];\n");
    CB_CHECK_EQ(ec::local_decls(0, {}), "");
});

CB_TEST("invert.early_c.to_c", [] {
    const std::vector<std::string> none;
    CB_CHECK_EQ(ec::to_c("f", 0, none, none), "f()\n{\n\n}\n");
    CB_CHECK_EQ(ec::to_c("g", 6, std::vector<std::string>{"a", "b"}, std::vector<std::string>{"return (a + b);"}, "int", Offsets{0, 2, 4}),
                "g(a, b)\nint a, b;\n{\n    int v0;\n    int v2;\n    int v4;\n    return (a + b);\n}\n");
    // a long and a gap, and a long return type
    CB_CHECK_EQ(ec::to_c("h", 8, std::vector<std::string>{"a"}, std::vector<std::string>{"v0 = 1;", "v2 = v0;", "return v2;"}, "long",
                         Offsets{0, 2, 6}, {6, 0}),
                "h(a)\nlong a;\n{\n    long v0;\n    int v2;\n    char _p2[2];\n    long v6;\n    v0 = 1;\n    v2 = v0;\n    return v2;\n}\n");
    // a copy chain among the statements: the locals are declared in the order that lands them where they were
    CB_CHECK_EQ(ec::to_c("k", 10, std::vector<std::string>{"p"},
                         std::vector<std::string>{"v0 = 1;", "v2 = v0;", "v4 = v2;", "return v4;"}, "int", Offsets{0, 2, 4}),
                "k(p)\nint p;\n{\n    int v4;\n    int v2;\n    int v0;\n    v0 = 1;\n    v2 = v0;\n    v4 = v2;\n    return v4;\n}\n");
    // a name is taken as it comes
    CB_CHECK_EQ(ec::to_c("caf\xC3\xA9", 2, none, std::vector<std::string>{"return 0;"}), "caf\xC3\xA9()\n{\n    return 0;\n}\n");
    // from a recovery
    const ec::Recovered loop = ec::recover(kLoop);
    const std::string c = ec::to_c("sum", loop.frame_size, loop.params, loop.statements, loop.return_type, loop.locals);
    CB_CHECK(c.starts_with("sum(a)\nint a;\n{\n    int v2;\n    int v4;\n"));
});
