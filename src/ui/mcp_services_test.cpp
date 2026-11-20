// Tests of the standard services of the MCP server (mcp_standard.cpp and its parts): every service is filled, and the tools that read the code, the
// names and the runtime libraries answer, on real modules (a 32-bit one, a 16-bit one, a DOS program), as recorded
// (testdata/mcp_python.golden.json calls 17 and on): the listing of a function, what refers to an address, the
// names of a module's functions, the functions a runtime library made, where an address is stored, one function of two builds side by side.
//
// Design notes: the command table is cb/app's catalog (a command's synopsis and summary)
// module docstring, so `commands` and `command_help` answer the catalog's words.
#include <cstdio>
#include <nlohmann/json.hpp>

#include "decompile/decompiler.hpp"
#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "ui/mcp_server.hpp"
#include "ui/mcp_wiring.hpp"

using namespace cb;
using namespace cb::ui;
using namespace cb::ui::mcp;
namespace sfs = std::filesystem;

namespace {

const Json& svc_golden() {
    static const Json g = json::load(testing::data_path("ui/testdata/mcp_python.golden.json"));
    return g;
}

// A server over the standard services of the main checkout's settings (its corpus and tools), with a state folder of its own.
struct SvcFixture {
    fs::ScopedTempDir dir{"cb-mcp-svc"};
    fs::ScopedEnv state_env{"CB_STATE", fs::to_text(dir.path() / "state")};
    config::Config config = config::Config::load(config::current().main_checkout());
    std::shared_ptr<base::Corpus> corpus = standard_corpus(config);
    service::Service service{standard_environment(config, *corpus)};
    service::ServiceLink link{service, {.mode = service::Mode::Off, .start = false, .local = true, .owner = "pid-test"}};
    Server server{service, link, standard_services(config, *corpus)};

    // The recorded answers name imports from the Windows 95 corpus: a machine without it skips them.
    bool has_corpus() const {
        const bool here = sfs::is_directory(config.path("corpus").value_or(sfs::path()));
        if (!here) std::fprintf(stderr, "  (the corpus is not here: not run)\n");
        return here;
    }
    std::pair<std::string, bool> call(const char* tool, Json arguments) {
        const Json r = server.handle(Json{{"jsonrpc", "2.0"}, {"id", 1}, {"method", "tools/call"}, {"params", Json{{"name", tool}, {"arguments", std::move(arguments)}}}});
        return {r["result"]["content"][0]["text"].get<std::string>(), r["result"]["isError"].get<bool>()};
    }
    const std::string& recorded(const std::string& key) {
        static std::map<std::string, std::string> kept;
        return kept[key] = svc_golden().at(key)["result"]["content"][0]["text"].get<std::string>();
    }
};

std::string data(const char* name) { return testing::data_path(std::string("formats/testdata/") + name).string(); }

}  // namespace

CB_TEST("ui.mcp.standard_services_fill_every_service", [] {
    SvcFixture f;
    const Services s = standard_services(f.config, *f.corpus);
    CB_CHECK(s.retail && s.lane && s.tree_root && s.placement && s.locate && s.java_classes);
    CB_CHECK(s.inspect && s.strings && s.hexdump && s.resource_body && s.reach_describe);
    CB_CHECK(s.ledger && s.progress && s.names && s.identified && s.ordinal_name && s.stored_candidates && s.set_name && s.modules_with_body);
    CB_CHECK(s.usage && s.command_names && s.command_help);
    CB_CHECK(s.disasm && s.xrefs && s.decompile_function && s.compare && s.diff && s.version);
    CB_CHECK_EQ(s.version(), "2.1.0");
});

CB_TEST("ui.mcp.the_code_is_read_as_recorded", [] {
    SvcFixture f;
    if (!f.has_corpus()) return;   // (the 16-bit modules' names come from the corpus's KERNEL, USER and GDI)
    const std::string lz32 = data("lz32.dll"), expand = data("lzexpand.dll"), find = data("find.exe");
    // a 32-bit function whole, and from an address for a number of instructions
    CB_CHECK_EQ(f.call("disasm", Json{{"module", lz32}, {"addr", "bfea1000"}}).first, f.recorded("call 17 disasm"));
    CB_CHECK_EQ(f.call("disasm", Json{{"module", lz32}, {"addr", "bfea1000"}, {"count", 12}}).first, f.recorded("call 18 disasm"));
    // a 16-bit one, by segment:offset and by its name
    CB_CHECK_EQ(f.call("disasm", Json{{"module", expand}, {"addr", "2:0000"}}).first, f.recorded("call 19 disasm"));
    CB_CHECK_EQ(f.call("disasm", Json{{"module", expand}, {"addr", "LZINIT"}, {"count", 8}}).first, f.recorded("call 20 disasm"));
    // a DOS program is not one the analyses read
    const auto dos = f.call("disasm", Json{{"module", find}, {"addr", "100"}});
    CB_CHECK(dos.second);
    CB_CHECK_EQ(dos.first, f.recorded("call 21 disasm"));
    // what refers to an address, an import, a 16-bit function
    CB_CHECK_EQ(f.call("xrefs", Json{{"module", lz32}, {"addr", "bfea1000"}}).first, f.recorded("call 22 xrefs"));
    CB_CHECK_EQ(f.call("xrefs", Json{{"module", lz32}, {"addr", "KERNEL32!OpenFile"}}).first, f.recorded("call 23 xrefs"));
    CB_CHECK_EQ(f.call("xrefs", Json{{"module", expand}, {"addr", "LZINIT"}}).first, f.recorded("call 24 xrefs"));
    CB_CHECK_EQ(f.call("xrefs", Json{{"module", expand}, {"addr", "4:0000"}}).first, f.recorded("call 25 xrefs"));
    // where an address is stored in the file (a 32-bit module only)
    CB_CHECK_EQ(f.call("hex_dump", Json{{"module", lz32}, {"addr", "bfea1000"}, {"length", 32}}).first, f.recorded("call 30 hex_dump"));
    const auto not_stored = f.call("hex_dump", Json{{"module", expand}, {"addr", "2:0000"}});
    CB_CHECK(not_stored.second);
    CB_CHECK_EQ(not_stored.first, f.recorded("call 31 hex_dump"));
    // a name that is nowhere
    const auto nowhere = f.call("disasm", Json{{"module", lz32}, {"addr", "nosuchfunction"}});
    CB_CHECK(nowhere.second);
    CB_CHECK(nowhere.first.find("no address or function named 'nosuchfunction' in lz32.dll") != std::string::npos);
});

CB_TEST("ui.mcp.the_names_and_the_runtime_libraries_are_read_as_recorded", [] {
    SvcFixture f;
    const std::string lz32 = data("lz32.dll"), expand = data("lzexpand.dll");
    CB_CHECK_EQ(f.call("names", Json{{"module", lz32}, {"limit", 10}}).first, f.recorded("call 26 names"));
    CB_CHECK_EQ(f.call("names", Json{{"module", expand}, {"limit", 12}}).first, f.recorded("call 27 names"));
    CB_CHECK_EQ(f.call("identify", Json{{"module", lz32}}).first, f.recorded("call 28 identify"));
    CB_CHECK_EQ(f.call("identify", Json{{"module", expand}}).first, f.recorded("call 29 identify"));
});

CB_TEST("ui.mcp.two_builds_are_compared_and_one_function_shown_side_by_side", [] {
    SvcFixture f;
    const std::string lz32 = data("lz32.dll");
    CB_CHECK_EQ(f.call("diff_function", Json{{"module_a", lz32}, {"module_b", lz32}, {"addr", "bfea1000"}}).first, f.recorded("call 32 diff_function"));
    const auto none = f.call("diff_function", Json{{"module_a", lz32}, {"module_b", lz32}, {"addr", "nosuchfunction"}});
    CB_CHECK(none.second);
    CB_CHECK_EQ(none.first, f.recorded("call 33 diff_function"));
    // a build against itself: every function is identical, none added or removed
    const auto [text, error] = f.call("compare_versions", Json{{"module_a", lz32}, {"module_b", lz32}, {"limit", 3}});
    CB_CHECK(!error);
    const Json doc = Json::parse(text);
    CB_CHECK_EQ(doc["counts"]["changed"].get<int>(), 0);
    CB_CHECK_EQ(doc["counts"]["added"].get<int>(), 0);
    CB_CHECK_EQ(doc["counts"]["removed"].get<int>(), 0);
    CB_CHECK(doc["counts"]["identical"].get<int>() > 0);
    CB_CHECK_EQ(doc["rows"].size(), std::size_t{3});
});

CB_TEST("ui.mcp.one_function_is_decompiled_now_as_recorded", [] {
    SvcFixture f;
    // (the decompiler is the checkout's program: without it there is nothing to ask)
    if (!sfs::exists(cb::decompile::decompiler::engine_bin(f.config) / "decompiler.exe")) return;
    if (!f.has_corpus()) return;
    const std::string lz32 = data("lz32.dll"), expand = data("lzexpand.dll");
    CB_CHECK_EQ(f.call("decompile_function", Json{{"module", lz32}, {"addr", "bfea1000"}}).first, f.recorded("call 34 decompile_function"));   // a 32-bit function by itself
    CB_CHECK_EQ(f.call("decompile_function", Json{{"module", expand}, {"addr", "1:0000"}}).first, f.recorded("call 35 decompile_function"));   // cut out of its module's C
    const auto missing = f.call("decompile_function", Json{{"module", expand}, {"addr", "9:9999"}});
    CB_CHECK(missing.second);
    CB_CHECK_EQ(missing.first, f.recorded("call 36 decompile_function"));
});

CB_TEST("ui.mcp.the_command_table_is_the_catalog", [] {
    SvcFixture f;
    const Json commands = Json::parse(f.call("commands", Json::object()).first);
    CB_CHECK(commands["usage"].get<std::string>().starts_with("usage: codebreaker <command> [arguments]"));
    bool has_decompile = false, has_ui = false;
    for (const Json& c : commands["runnable"]) {
        has_decompile = has_decompile || c == "decompile";
        has_ui = has_ui || c == "app";   // (the interfaces are not run as jobs)
    }
    CB_CHECK(has_decompile);
    CB_CHECK(!has_ui);
    const Json help = Json::parse(f.call("command_help", Json{{"command", "decompile"}}).first);
    CB_CHECK(help["help"].get<std::string>().find("decompile <name> [--out FILE]") != std::string::npos);
    CB_CHECK(f.call("command_help", Json{{"command", "nosuchcommand"}}).second);
});

CB_TEST("ui.mcp.what_a_file_is_and_where_its_source_goes", [] {
    SvcFixture f;
    const Json d = Json::parse(f.call("describe", Json{{"path", data("lz32.dll")}}).first);
    CB_CHECK_EQ(d["platform"].get<std::string>(), "32-bit Windows");
    CB_CHECK_EQ(d["lane"].get<std::string>(), "pe");
    CB_CHECK_EQ(d["summary"]["image_base"].get<std::string>(), "0xbfea0000");
    const Json tree = Json::parse(f.call("source_tree", Json{{"module", data("lz32.dll")}}).first);
    CB_CHECK(!tree["relative"].get<std::string>().empty());
    CB_CHECK(tree.contains("decided"));
});

// A fresh state folder (what a new user has: no functions.sqlite) still has the verifier settings in force: they come from the lanes, not the
// store, and asking for them creates nothing in the state folder.
CB_TEST("ui.mcp.verifier_settings_in_force_need_no_function_store", [] {
    SvcFixture f;
    CB_CHECK(!sfs::exists(f.config.state() / "functions.sqlite"));
    const auto settings = wiring::verifier_profiles(f.config)();
    CB_CHECK(settings.at("pe").has_value() && !settings.at("pe")->empty());
    CB_CHECK(settings.at("ne").has_value() && !settings.at("ne")->empty());
    CB_CHECK(!sfs::exists(f.config.state() / "functions.sqlite"));
});
