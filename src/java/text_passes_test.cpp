// Tests of cb/java/finalize, prune and init_driver against the recorded results for the
// same inputs: testdata/{finalize,tree,init}.golden.json.
#include <nlohmann/json.hpp>

#include <map>
#include <string>
#include <vector>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "java/corpus.hpp"
#include "java/finalize.hpp"
#include "java/init_driver.hpp"
#include "java/parse.hpp"
#include "java/prune.hpp"

namespace {

namespace stdfs = std::filesystem;
using cb::Json;

// The text writer writes LF on every platform; the goldens are compared with their line ends normalised.
std::string tp_lf(const std::string& text) { return cb::text::replace_all(text, "\r\n", "\n"); }

Json tp_golden(const char* name) { return cb::json::load(cb::testing::data_path(std::string("java/testdata/") + name)); }

CB_TEST("java.finalize.cases_match_golden", [] {
    const Json cases = tp_golden("finalize.golden.json");
    CB_CHECK_EQ(cases.size(), std::size_t{6});
    const std::set<std::string, std::less<>> escapes = {"Item"};
    for (const Json& c : cases) {
        const std::string kind = c["kind"].get<std::string>();
        const std::string text = c["in"].get<std::string>();
        std::string got;
        if (kind == "gl") {
            got = cb::java::finalize::convert_gl_to_d3d9(text);
        } else if (kind == "trace") {
            got = cb::java::finalize::trace_guest_throws(text);
        } else {
            cb::java::finalize::Options options;
            options.gl_to_d3d9 = c["flags"].value("gl", false);
            options.trace_throws = c["flags"].value("trace", false);
            options.this_escapes = &escapes;
            got = cb::java::finalize::finalize_emitted(text, options);
        }
        CB_CHECK_EQ(got, c["out"].get<std::string>());
    }
});

CB_TEST("java.finalize.discarded_new_keeps_the_blank_lines", [] {
    // the blank lines that follow a rewritten statement are kept
    const std::string got = cb::java::finalize::finalize_emitted("new Foo();\n\n\nint x;\n");
    CB_CHECK_EQ(got, "delete new Foo();  // J2X-BUGFIX discarded-new-leak\n\n\nint x;\n");
});

CB_TEST("java.prune.tree_passes_match_golden", [] {
    const Json golden = tp_golden("tree.golden.json");
    cb::fs::ScopedTempDir tmp("cbprune");
    for (const auto& [name, text] : golden["files"].items()) cb::fs::write_file(tmp.path() / name, cb::ByteView(reinterpret_cast<const std::uint8_t*>(text.get<std::string>().data()), text.get<std::string>().size()));
    std::vector<std::string> messages;
    const auto say = [&messages](std::string_view line) { messages.emplace_back(line); };

    const std::vector<std::string> removed = cb::java::prune::coalesce_forwarders(tmp.path(), say);
    CB_CHECK(removed == golden["coalesce_removed"].get<std::vector<std::string>>());
    for (const auto& [name, text] : golden["after_coalesce"].items()) CB_CHECK_EQ(cb::fs::read_text(tmp.path() / name), tp_lf(text.get<std::string>()));

    const std::vector<std::string> dropped = cb::java::prune::prune_unreferenced(tmp.path(), {"Game", "Needs"}, {"sub/Kernel.cpp", "KernelConfig.h", "x.txt"}, say);
    CB_CHECK(dropped == golden["pruned"].get<std::vector<std::string>>());
    std::vector<std::string> left;
    for (const auto& e : stdfs::directory_iterator(tmp.path())) left.push_back(cb::fs::to_text(e.path().filename()));
    std::ranges::sort(left);
    CB_CHECK(left == golden["after_prune"].get<std::vector<std::string>>());
    CB_CHECK(messages == golden["messages"].get<std::vector<std::string>>());
});

CB_TEST("java.prune.defined_symbols_match_golden", [] {
    const Json golden = tp_golden("tree.golden.json");
    for (const auto& [name, input] : golden["symbol_inputs"].items()) {
        const std::set<std::string> got = cb::java::prune::defined_symbols(input.get<std::string>());
        CB_CHECK(std::vector<std::string>(got.begin(), got.end()) == golden["symbols"][name].get<std::vector<std::string>>());
    }
});

CB_TEST("java.prune.nothing_to_do", [] {
    cb::fs::ScopedTempDir tmp("cbprune");
    cb::fs::write_text(tmp.path() / "Game.h", "#pragma once\nclass Game {};\n");
    CB_CHECK(cb::java::prune::coalesce_forwarders(tmp.path()).empty());
    CB_CHECK(cb::java::prune::prune_unreferenced(tmp.path(), {"Game"}).empty());
    CB_CHECK(cb::java::prune::prune_unreferenced(tmp.path() / "absent", {"Game"}).empty());
});

std::vector<cb::java::ir::JClass> tp_init_classes() {
    std::vector<cb::java::ir::JClass> classes;
    for (const auto& path : cb::java::corpus::discover(cb::testing::data_path("java/testdata/init"))) classes.push_back(cb::java::parse::parse_file(path));
    return classes;
}

CB_TEST("java.init_driver.order_matches_golden", [] {
    const Json golden = tp_golden("init.golden.json");
    const auto classes = tp_init_classes();
    CB_CHECK_EQ(golden["orders"].size(), std::size_t{12});
    for (const Json& c : golden["orders"]) {
        const auto callable = c["callable"].get<std::vector<std::string>>();
        const auto got = cb::java::init_driver::compute_static_init_order(classes, {callable.begin(), callable.end()}, c["seed"].get<std::vector<std::string>>());
        CB_CHECK(got == c["order"].get<std::vector<std::string>>());
    }
});

CB_TEST("java.init_driver.files_match_golden", [] {
    const Json golden = tp_golden("init.golden.json");
    const auto classes = tp_init_classes();
    cb::fs::ScopedTempDir tmp("cbinit");
    cb::java::init_driver::write_init_driver(tmp.path(), {"IA", "IC", "ID", "IJ"}, classes, {"IC", "IA"});
    for (const auto& [name, text] : golden["files"].items()) CB_CHECK_EQ(cb::fs::read_text(tmp.path() / name), tp_lf(text.get<std::string>()));
    CB_CHECK_EQ(golden["files"].size(), std::size_t{4});
});

}  // namespace
