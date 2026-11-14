// Tests of the compiler models on the functions of two real 16-bit modules of Windows 95 -- LZEXPAND.DLL (every function its describer finds, 46) and
// OLECLI.DLL (its first 150 framed functions): what each model says of a function, against the recorded answers for the same bytes
// (testdata/msc6_modules.json, read over for sense).
//
//     msc6            the Microsoft C 6.0a model: the text, or the reason it declined, and the notes (calls, register variables, globals, near calls)
//     early_c         recover and recover_cfg (the 1983-84 compiler's model, here run on code it was not written for)
//     flow16          the blocks of the control-flow graph and the structure tree, with and without compare switches
//     msc6_segment    the segments' parameters from how their functions are called, the thunks, the one-shape functions
//
// A function a model fails on is a decline, with a reason in this module's words; the
// registers a statement writes are listed in the model's fixed order.
#include <nlohmann/json.hpp>

#include <chrono>
#include <format>
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/ne.hpp"
#include "cb/invert/early_c.hpp"
#include "cb/invert/flow16.hpp"
#include "cb/invert/msc6.hpp"
#include "cb/invert/module_pass.hpp"
#include "cb/invert/msc6_segment.hpp"
#include "cb/platforms/decoder.hpp"

using namespace cb;
namespace ec = cb::invert::early_c;
namespace flow = cb::invert::flow16;
namespace m6 = cb::invert::msc6;
namespace seg = cb::invert::msc6_segment;

namespace {

Bytes from_hex(std::string_view hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(*text::parse_int<std::uint8_t>(hex.substr(i, 2), 16));
    return out;
}

const Json& golden() {
    static const Json g = json::load(testing::data_path("invert/testdata/msc6_modules.json"));
    return g;
}

std::string label(const char* module, const Json& row) {
    return std::string(module) + " " + std::to_string(row.at("seg").get<int>()) + ":" + std::format("{:x}", row.at("off").get<std::uint32_t>());
}

void fail(const std::string& what) { testing::record_failure(__FILE__, __LINE__, what); }

// ---- msc6 ------------------------------------------------------------------------------------------------------------------------------

m6::Kind kind_of(const std::string& k) { return k == "far" ? m6::Kind::Far : k == "long" ? m6::Kind::Long : m6::Kind::Int; }

std::vector<m6::Param> params_of(const Json& j) {
    std::vector<m6::Param> out;
    for (const Json& p : j) out.push_back({p.at(0).get<std::string>(), kind_of(p.at(1).get<std::string>())});
    return out;
}

void check_msc6_function(const char* module, const Json& row, const std::map<std::uint32_t, std::string>& nears, std::size_t& read, std::size_t& declined,
                         std::size_t& failed) {
    const Bytes code = from_hex(row.at("code").get<std::string>());
    for (const Json& run : row.at("runs")) {
        const std::string cfg = run.at("cfg").get<std::string>();
        m6::Context ctx;
        for (const auto& [k, v] : golden().at("imports").items()) ctx.imports[static_cast<std::uint32_t>(std::stoul(k))] = v.get<std::string>();
        for (const auto& [off, g] : row.at("syms").items())
            ctx.globals[static_cast<std::uint32_t>(std::stoul(off))] = {g.at(0).get<std::string>(),
                                                                         g.at(1).get<std::string>() == "long" ? cb::invert::msc6_small::Width::Long : cb::invert::msc6_small::Width::Word};
        ctx.near_functions = nears;
        ctx.function_base = row.at("off").get<std::uint32_t>();
        if (cfg == "sdk") {
            ctx.params = params_of(run.at("params"));
            ctx.return_type = run.at("rtype").get<std::string>();
        } else if (cfg != "none-int") {
            ctx.params = params_of(row.at("sig"));
            ctx.return_type = cfg == "sig-void" ? "void" : cfg == "sig-long" ? "long" : "int";
        }
        const auto got = m6::invert(code, ctx);
        const std::string where = label(module, row) + " " + cfg;
        if (run.contains("exc")) {
            ++declined;
            if (got.ok()) fail(where + ": the golden expects the error " + run.at("exc").get<std::string>() + "; this one read it");
            continue;
        }
        if (run.contains("why")) {
            ++declined;
            if (got.ok()) fail(where + ": read, where a decline is expected: " + run.at("why").get<std::string>());
            else if (std::string(got.error().what()) != run.at("why").get<std::string>())
                fail(where + ": declined with '" + got.error().what() + "', expected '" + run.at("why").get<std::string>() + "'");
            testing::count_check();
            continue;
        }
        ++read;
        if (!got.ok()) {
            ++failed;
            fail(where + ": declined with '" + got.error().what() + "', where it should read");
            continue;
        }
        const m6::Reading& r = got.value();
        if (r.text != run.at("text").get<std::string>()) {
            ++failed;
            fail(where + ": read differently:\n" + r.text + "\n--- expected:\n" + run.at("text").get<std::string>());
            continue;
        }
        const Json& meta = run.at("meta");
        const bool same = meta.at("calls").get<std::size_t>() == r.notes.calls && meta.at("regvars").get<std::vector<std::string>>() == r.notes.register_variables &&
                          meta.at("cdecl").get<bool>() == r.notes.plain_return && meta.at("has_nearcall").get<bool>() == r.notes.has_near_call &&
                          meta.at("nears").get<std::vector<std::string>>() == r.notes.near_calls && meta.at("globals").size() == r.notes.globals.size();
        bool globals_same = same;
        for (const auto& [name, type] : r.notes.globals)
            if (globals_same && (!meta.at("globals").contains(name) || meta.at("globals").at(name).get<std::string>() != type)) globals_same = false;
        if (!globals_same) fail(where + ": the notes differ from the expected ones");
        testing::count_check();
    }
}

std::map<std::uint32_t, std::string> near_map(const Json& pairs) {
    std::map<std::uint32_t, std::string> out;
    for (const Json& p : pairs) out[p.at(0).get<std::uint32_t>()] = p.at(1).get<std::string>();
    return out;
}

}  // namespace

CB_TEST("invert.msc6.every_function_of_two_real_modules_reads_as_recorded", [] {
    std::size_t read = 0, declined = 0, failed = 0;
    for (const Json& row : golden().at("lzexpand").at("rows")) {
        std::map<std::uint32_t, std::string> nears;
        for (const auto& [k, v] : row.at("nears").items()) nears[static_cast<std::uint32_t>(std::stoul(k))] = v.get<std::string>();
        check_msc6_function("lzexpand", row, nears, read, declined, failed);
    }
    std::map<int, std::map<std::uint32_t, std::string>> by_segment;
    for (const Json& s : golden().at("olecli").at("segments"))
        if (s.contains("nears")) by_segment[s.at("seg").get<int>()] = near_map(s.at("nears"));
    for (const Json& row : golden().at("olecli").at("rows")) check_msc6_function("olecli", row, by_segment[row.at("seg").get<int>()], read, declined, failed);
    CB_CHECK(read > 200);
    CB_CHECK(declined > 200);
    CB_CHECK_EQ(failed, std::size_t{0});
});

CB_TEST("invert.msc6.declines_what_it_does_not_cover", [] {
    const auto nothing = m6::invert(Bytes(12, 0x90));
    CB_CHECK(!nothing.ok());
    CB_CHECK_EQ(std::string(nothing.error().what()), std::string("prologue miss"));
    const auto empty = m6::invert(Bytes{});
    CB_CHECK(!empty.ok());
    // a function with a frame and no epilogue
    const auto cut = m6::invert(from_hex("1e589045558bec1e8ed8b80100"));
    CB_CHECK(!cut.ok());
    CB_CHECK_EQ(std::string(cut.error().what()), std::string("no epilogue"));
});

CB_TEST("invert.msc6.the_documented_function", [] {
    // int far pascal F(p0) { register v0 = G(p0); if (!v0) return 0; H(v0); return v0; }
    const Bytes calls = from_hex("1e589045558bec1e8ed883ec0256ff76069a000000008bf00bf675042bc0eb08569a000000008bc65e4d4d8be51f5d4dca020090");
    m6::Context ctx;
    ctx.imports = {{0, "G"}, {1, "H"}};
    const auto r = m6::invert(calls, ctx);
    CB_CHECK(r.ok());
    if (r.ok()) {
        CB_CHECK(r.value().text.find("v0 = G(p0);") != std::string::npos);
        CB_CHECK(r.value().text.find("if (v0 == 0)") != std::string::npos);
        CB_CHECK(r.value().text.find("H(v0);") != std::string::npos);
        CB_CHECK_EQ(r.value().notes.calls, std::size_t{2});
        CB_CHECK(!r.value().notes.plain_return);
    }
});

// ---- early_c on the same functions ---------------------------------------------------------------------------------------------------

namespace {

Json json_of(const ec::Recovered& r) { return Json::array({r.frame_size, r.params, r.statements, r.return_type, r.locals}); }

void check_early(const char* module, const Json& row, std::size_t& answered, std::size_t& failed) {
    const Bytes code = from_hex(row.at("code").get<std::string>());
    for (const char* which : {"recover", "recover_cfg"}) {
        const Json& want = row.at("early").at(which);
        const std::string where = label(module, row) + " " + which;
        try {
            const ec::Recovered r = std::string(which) == "recover" ? ec::recover(code) : ec::recover_cfg(code);
            if (want.is_object()) {
                fail(where + ": the golden expects the error " + want.at("exc").get<std::string>() + "; this one answered");
                ++failed;
            } else if (json_of(r) != want) {
                fail(where + "\n  got      " + json_of(r).dump() + "\n  expected " + want.dump());
                ++failed;
            } else {
                ++answered;
            }
        } catch (const Error& e) {
            if (!want.is_object()) {
                fail(where + ": threw " + e.what() + ", where an answer is expected");
                ++failed;
            }
        }
        testing::count_check();
    }
}

}  // namespace

CB_TEST("invert.early_c.every_function_of_two_real_modules_reads_as_recorded", [] {
    std::size_t answered = 0, failed = 0;
    for (const char* module : {"lzexpand", "olecli"})
        for (const Json& row : golden().at(module).at("rows")) check_early(module, row, answered, failed);
    CB_CHECK(answered > 300);
    CB_CHECK_EQ(failed, std::size_t{0});
});

// ---- flow16 --------------------------------------------------------------------------------------------------------------------------

namespace {

const char* end_name(flow::End e) {
    switch (e) {
        case flow::End::Return: return "ret";
        case flow::End::Jump: return "jmp";
        case flow::End::Branch: return "cond";
        case flow::End::Fall: return "fall";
        case flow::End::TailCall: return "tailcall";
        case flow::End::Switch: return "switch";
        case flow::End::Dead: return "dead";
    }
    return "?";
}

Json opt_block(int b) { return b == flow::kNone ? Json(nullptr) : Json(b); }

Json tree_json(const flow::Tree& tree);

Json node_json(const flow::Node& n) {
    using K = flow::Node::Kind;
    switch (n.kind) {
        case K::Block: return Json::array({"block", n.block});
        case K::Break: return Json::array({"break"});
        case K::Continue: return Json::array({"continue"});
        case K::Goto: return Json::array({"goto", nullptr, n.block});
        case K::If: return Json::array({"if", n.block, tree_json(n.body), opt_block(n.join), n.negated});
        case K::IfElse: return Json::array({"ifelse", n.block, tree_json(n.body), tree_json(n.else_body), opt_block(n.join)});
        case K::While: return Json::array({"while", n.block, tree_json(n.body), opt_block(n.join), n.negated});
        case K::DoWhile: return Json::array({"dowhile", n.block, tree_json(n.body), opt_block(n.join), n.negated});
        case K::Loop: return Json::array({"loop", n.block, tree_json(n.body), opt_block(n.join)});
        case K::Switch: {
            Json arms = Json::array();
            for (const auto& [values, body] : n.arms) {
                Json vs = Json::array();
                for (const flow::CaseValue& v : values) vs.push_back(v.is_default ? Json("default") : Json(v.value));
                arms.push_back(Json::array({vs, tree_json(body)}));
            }
            return Json::array({"switch", n.block, arms, opt_block(n.join)});
        }
    }
    return Json();
}

Json tree_json(const flow::Tree& tree) {
    Json out = Json::array();
    for (const flow::Node& n : tree) out.push_back(node_json(n));
    return out;
}

Json blocks_json(const flow::Cfg& cfg) {
    Json out = Json::array();
    for (const flow::Block& b : cfg.blocks) {
        Json succ = Json::array();
        for (const int s : b.successors) succ.push_back(s);
        Json table = nullptr, cases = nullptr;
        if (b.table) {
            table = Json::array();
            for (const int t : *b.table) table.push_back(t);
        }
        if (b.cases) {
            cases = Json::array();
            for (const flow::Case& c : *b.cases) cases.push_back(Json::array({c.value, c.target}));
        }
        out.push_back(Json::array({b.first, b.last, end_name(b.end), succ, b.branch ? Json(*b.branch) : Json(nullptr), table, cases,
                                   opt_block(b.default_target), b.selector.empty() ? Json(nullptr) : Json(b.selector),
                                   b.selector_end == flow::kNone ? Json(nullptr) : Json(b.selector_end)}));
    }
    return out;
}

}  // namespace

CB_TEST("invert.flow16.blocks_and_structure_of_real_functions_as_recorded", [] {
    platforms::DecoderSet decoders;
    const platforms::Decoder& cs = decoders.get(platforms::Arch::X86, 16);
    std::size_t checked = 0, with_loops = 0, with_switches = 0;
    for (const char* module : {"lzexpand", "olecli"}) {
        for (const Json& row : golden().at(module).at("rows")) {
            if (row.at("flow").is_null()) continue;
            const Bytes code = from_hex(row.at("code").get<std::string>());
            const auto insns = cs.disasm(code, 0);
            for (const bool compare_switches : {false, true}) {
                const Json& want = row.at("flow").at(compare_switches ? "cmp" : "plain");
                const std::string where = label(module, row) + (compare_switches ? " cmp" : " plain");
                try {
                    flow::Options options;
                    options.compare_switches = compare_switches;
                    const flow::Cfg cfg = flow::build_cfg(insns, options);
                    if (want.contains("exc")) {
                        fail(where + ": the golden expects the error " + want.at("exc").get<std::string>());
                        continue;
                    }
                    if (blocks_json(cfg) != want.at("blocks")) fail(where + ": the blocks differ:\n" + blocks_json(cfg).dump() + "\n" + want.at("blocks").dump());
                    const Json tree = tree_json(flow::structure(cfg));
                    if (tree != want.at("tree")) fail(where + ": the tree differs:\n" + tree.dump() + "\n" + want.at("tree").dump());
                    ++checked;
                    const std::string dumped = want.at("tree").dump();
                    with_loops += dumped.find("\"while\"") != std::string::npos || dumped.find("\"dowhile\"") != std::string::npos ? 1 : 0;
                    with_switches += dumped.find("\"switch\"") != std::string::npos ? 1 : 0;
                } catch (const Error& e) {
                    if (!want.contains("exc")) fail(where + ": threw " + e.what());
                }
                testing::count_check();
            }
        }
    }
    CB_CHECK(checked > 100);
    CB_CHECK(with_loops > 3);
});

CB_TEST("invert.flow16.errors", [] {
    CB_CHECK_THROWS_AS(UsageError, flow::build_cfg(std::vector<platforms::Insn>{}));
    platforms::DecoderSet decoders;
    const auto insns = decoders.get(platforms::Arch::X86, 16).disasm(from_hex("c3"), 0);
    const flow::Cfg cfg = flow::build_cfg(insns);
    CB_CHECK_EQ(cfg.blocks.size(), std::size_t{1});
    CB_CHECK(cfg.blocks[0].end == flow::End::Return);
    CB_CHECK_THROWS_AS(UsageError, flow::structure(cfg, 7));
    CB_CHECK_EQ(flow::structure(cfg).size(), std::size_t{1});
});

// ---- msc6_segment -----------------------------------------------------------------------------------------------------------------------

CB_TEST("invert.msc6_segment.signatures_thunks_and_shapes_as_recorded", [] {
    std::size_t sigs_checked = 0, shapes_checked = 0;
    for (const char* module : {"lzexpand", "olecli"}) {
        const formats::ne::Module ne = formats::ne::Module::open_file(testing::data_path(std::string("invert/testdata/") + (std::string(module) == "lzexpand" ? "LZEXPAND.DLL" : "OLECLI.DLL")));
        const Json& segments = golden().at(module).at("segments");
        std::size_t k = 0;
        for (const formats::ne::Segment& s : ne.segments()) {
            if (s.length == 0) continue;
            const Json& want = segments.at(k++);
            const ByteView data = ne.data().subspan(s.file_offset, s.length);
            const std::string where = std::string(module) + " segment " + std::to_string(s.index);
            try {
                const auto got = seg::infer_signatures(data);
                if (want.contains("exc")) {
                    fail(where + ": the golden expects the error " + want.at("exc").get<std::string>());
                    continue;
                }
                Json have = Json::array();
                for (const seg::Signature& sig : got) {
                    Json ps = Json::array();
                    for (const m6::Param& p : sig.params) ps.push_back(Json::array({p.name, p.kind == m6::Kind::Far ? "far" : p.kind == m6::Kind::Long ? "long" : "int"}));
                    have.push_back(Json::array({sig.offset, ps}));
                }
                if (have != want.at("sigs")) fail(where + ": signatures differ:\n" + have.dump() + "\n" + want.at("sigs").dump());
                ++sigs_checked;
            } catch (const Error&) {
                if (!want.contains("exc")) fail(where + ": threw where an answer is expected");
            }
            testing::count_check();
        }
    }
    CB_CHECK(sigs_checked >= 5);
    for (const char* module : {"lzexpand", "olecli"}) {
        for (const Json& row : golden().at(module).at("rows")) {
            const Bytes code = from_hex(row.at("code").get<std::string>());
            const std::string where = label(module, row);
            const Json& thunk_want = row.at("thunk");
            const auto thunk = seg::thunk(code);
            if (thunk_want.is_null()) {
                if (thunk) fail(where + ": a thunk, where none is expected");
            } else if (!thunk) {
                fail(where + ": no thunk, where one is expected");
            } else {
                CB_CHECK_EQ(seg::thunk_assembly(*thunk, "T"), thunk_want.at("asm").get<std::string>());
                CB_CHECK_EQ(thunk->kind() == seg::ThunkKind::Forwarder ? std::string("forwarder") : std::string("return-stub"), thunk_want.at("kind").get<std::string>());
            }
            const Json& shapes_want = row.at("shapes");
            const auto shapes = seg::shapes(code);
            Json have = Json::array();
            for (const seg::Shape& s : shapes) have.push_back(Json::array({s.name, s.text}));
            if (have != shapes_want) fail(where + ": the one-shape candidates differ");
            shapes_checked += shapes.size();
            testing::count_check();
        }
    }
});

// ---- the module pass ---------------------------------------------------------------------------------------------------------------------

CB_TEST("invert.module_pass.every_function_is_read_as_recorded", [] {
    namespace mp = cb::invert::module_pass;
    std::size_t recognised = 0, declined = 0, assembly = 0;
    for (const char* module : {"lzexpand", "olecli"}) {
        for (const Json& row : golden().at(module).at("rows")) {
            mp::Function f;
            f.name = row.contains("name") ? row.at("name").get<std::string>() : "sub_" + std::to_string(row.at("seg").get<int>()) + "_" + std::format("{:x}", row.at("off").get<std::uint32_t>());
            f.segment = row.at("seg").get<std::uint32_t>();
            f.offset = row.at("off").get<std::uint32_t>();
            f.code = from_hex(row.at("code").get<std::string>());
            if (row.contains("sdk") && !row.at("sdk").is_null()) f.sdk = mp::Signature{row.at("sdk").at(0).get<std::string>(), params_of(row.at("sdk").at(1))};
            if (!row.at("sig").is_null()) f.called = params_of(row.at("sig"));
            const std::string where = label(module, row);
            const Json& want = row.at("module");
            const mp::Reading got = mp::read_function(f);
            const std::string kind = want.at(0).get<std::string>();
            if (std::string(mp::to_text(got.verdict)) != kind) {
                fail(where + ": " + std::string(mp::to_text(got.verdict)) + ", expected " + kind);
            } else if (got.text != want.at(1).get<std::string>()) {
                fail(where + ": text differs:\n" + got.text + "\n--- expected:\n" + want.at(1).get<std::string>());
            } else if (kind != "assembly" && got.tries.size() != want.at(2).get<std::size_t>()) {
                fail(where + ": " + std::to_string(got.tries.size()) + " texts to try, expected " + std::to_string(want.at(2).get<std::size_t>()));
            }
            recognised += got.verdict == mp::Verdict::Recognised;
            declined += got.verdict == mp::Verdict::Declined;
            assembly += got.verdict == mp::Verdict::Assembly;
            // the variables it names by address
            const auto globals = mp::globals_of(f.code);
            CB_CHECK_EQ(globals.size(), row.at("syms").size());
            for (const auto& [off, g] : globals) {
                const std::string key = std::to_string(off);
                CB_CHECK(row.at("syms").contains(key));
                if (row.at("syms").contains(key)) CB_CHECK_EQ(g.name, row.at("syms").at(key).at(0).get<std::string>());
            }
            // the bytes with the places the linker fills in zeroed
            std::string masked;
            for (const std::uint8_t b : cb::invert::msc6_small::mask(f.code)) masked += std::format("{:02x}", b);
            CB_CHECK_EQ(masked, row.at("mask").get<std::string>());
            testing::count_check();
        }
    }
    CB_CHECK(recognised > 60);
    CB_CHECK(declined > 100);
});

CB_TEST("invert.module_pass.compilers_decide_identical", [] {
    namespace mp = cb::invert::module_pass;
    // a function whose text the stand-in compiler turns back into its own bytes is identical, under the first switch set that does
    mp::Function f;
    f.name = "F";
    f.code = from_hex("1e589045558bec1e8ed8b80700" "4d4d8be51f5d4dcb");
    std::vector<std::string> asked;
    const mp::Compile compile = [&](std::string_view, std::string_view flags) -> std::optional<Bytes> {
        asked.emplace_back(flags);
        if (flags == "/AM /G2 /Gsw /Os") return f.code;
        return std::nullopt;
    };
    std::vector<std::string> log;
    const auto results = mp::run({f}, compile, {}, {}, [&](const std::string& line) { log.push_back(line); });
    CB_CHECK_EQ(std::string(mp::to_text(results.at("F").verdict)), std::string("identical"));
    CB_CHECK_EQ(results.at("F").flags, std::string("/AM /G2 /Gsw /Os"));
    CB_CHECK_EQ(asked.size(), std::size_t{2});
    CB_CHECK_EQ(log.size(), std::size_t{2});
    // without a compiler the function stays recognised and the log says so
    std::vector<std::string> log2;
    const auto none = mp::run({f}, {}, {}, {}, [&](const std::string& line) { log2.push_back(line); });
    CB_CHECK_EQ(std::string(mp::to_text(none.at("F").verdict)), std::string("recognised"));
    CB_CHECK_EQ(log2.size(), std::size_t{1});
    CB_CHECK(mp::compiler_of(from_hex("8cd89045558bec1e8ed8")) == mp::Compiler::Msvc152);
    CB_CHECK(mp::compiler_of(from_hex("1e589045558bec1e8ed8")) == mp::Compiler::Msc6);
});

// CB_INVERT_BENCH=<passes> times the models over the golden's functions (for the old-against-new timing of the report).
CB_TEST("invert.models.bench", [] {
    const std::optional<std::string> passes = fs::get_env("CB_INVERT_BENCH");
    if (!passes || passes->empty()) return;
    const std::size_t n = std::stoul(*passes);
    std::vector<Bytes> codes;
    for (const char* module : {"lzexpand", "olecli"})
        for (const Json& row : golden().at(module).at("rows")) codes.push_back(from_hex(row.at("code").get<std::string>()));
    const auto time = [&](const char* what, const auto& each) {
        const auto start = std::chrono::steady_clock::now();
        std::size_t sink = 0;
        for (std::size_t i = 0; i < n; ++i)
            for (const Bytes& c : codes) sink += each(c);
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        std::cout << what << ": " << seconds / static_cast<double>(n * codes.size()) * 1e6 << " us per function (" << sink << ")\n";
    };
    time("msc6", [](const Bytes& c) { const auto r = m6::invert(c); return r.ok() ? r.value().text.size() : 0; });
    time("early_c.recover", [](const Bytes& c) { try { return ec::recover(c).statements.size(); } catch (const Error&) { return std::size_t{0}; } });
    time("early_c.recover_cfg", [](const Bytes& c) { try { return ec::recover_cfg(c).statements.size(); } catch (const Error&) { return std::size_t{0}; } });
});
