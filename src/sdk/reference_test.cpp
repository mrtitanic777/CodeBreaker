// sdk/reference_test.cpp -- what keeps the SDK honest: the reference is what the headers say (docs/sdk/reference), every function has a
// sample or a test, no exported name of a release goes away (tools/sdk/abi_baseline.txt), and src/sdk includes only the engines'
// public faces.
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "sdk/reference.hpp"

namespace {

namespace stdfs = std::filesystem;
namespace ref = cb::sdk_impl::reference;

stdfs::path reference_checkout() { return stdfs::path(CB_SOURCE_DIR).parent_path(); }

std::string reference_file_text(const stdfs::path& p) {
    std::ifstream in(p, std::ios::binary);
    std::ostringstream o;
    o << in.rdbuf();
    std::string s = o.str();
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());   // (a checkout may have CRLF lines)
    return s;
}

}  // namespace

CB_TEST("sdk.reference.is_current", [] {
    const auto pages = ref::of_checkout(reference_checkout());
    const stdfs::path out = reference_checkout() / "docs" / "sdk" / "reference";
    if (cb::fs::get_env("CB_SDK_REFERENCE_UPDATE").value_or("") == "1") {
        ref::write(pages, out);
        return;
    }
    std::vector<std::string> stale;
    for (const auto& [name, text] : pages)
        if (reference_file_text(out / cb::fs::from_text(name)) != text) stale.push_back(name);
    std::error_code ec;
    for (auto it = stdfs::recursive_directory_iterator(out, ec); it != stdfs::recursive_directory_iterator(); ++it)
        if (it->is_regular_file() && !pages.contains(cb::fs::to_text(stdfs::relative(it->path(), out)))) stale.push_back(cb::fs::to_text(it->path().filename()) + " (no longer made)");
    if (!stale.empty())
        ::cb::testing::record_failure(__FILE__, __LINE__,
                                      "docs/sdk/reference is not what include/cb says (run tools\\sdk\\reference.cmd, or CB_SDK_REFERENCE_UPDATE=1): " + stale.front() +
                                          (stale.size() > 1 ? " and " + std::to_string(stale.size() - 1) + " more" : ""));
});

CB_TEST("sdk.reference.every_function_is_documented_and_called", [] {
    const auto headers = ref::read(reference_checkout() / "include" / "cb");
    const ref::Uses samples = ref::uses(headers, reference_checkout(), {reference_checkout() / "samples"});
    const ref::Uses tests = ref::uses(headers, reference_checkout(), {reference_checkout() / "src" / "sdk"});
    std::size_t functions = 0;
    for (const ref::Header& h : headers)
        for (const ref::Entry& e : h.entries) {
            CB_CHECK(!e.summary.empty());
            if (e.kind != "function") continue;
            ++functions;
            CB_CHECK(!e.returns.empty() || e.declaration.starts_with("void"));
            std::size_t callers = 0;
            if (const auto s = samples.find(e.name); s != samples.end()) callers += s->second.size();
            if (const auto t = tests.find(e.name); t != tests.end())
                callers += static_cast<std::size_t>(std::count_if(t->second.begin(), t->second.end(), [](const std::string& f) {
                    return f.ends_with("_test.cpp") && !f.ends_with("/reference_test.cpp");   // (this file names functions as text)
                }));
            if (callers == 0) ::cb::testing::record_failure(__FILE__, __LINE__, e.name + " has no sample or test that calls it");
        }
    CB_CHECK(functions >= 30);
});

CB_TEST("sdk.reference.strict", [] {
    cb::fs::ScopedTempDir dir("cb-sdk-ref");
    auto header = [&](std::string_view text) {
        std::ofstream(dir.path() / "x.h", std::ios::binary) << text;
        try {
            (void)ref::read(dir.path());
            return std::string("read");
        } catch (const cb::ParseError& e) {
            return std::string(e.what());
        }
    };
    CB_CHECK_EQ(header("/* x.h */\n/** cb_a: does a.\n * @return 0.\n * @since 0.1\n */\nCB_API int cb_a(void);\n"), std::string("read"));
    CB_CHECK(header("/* x.h */\nCB_API int cb_a(void);\n").find("cb_a has no /** comment") != std::string::npos);
    CB_CHECK(header("/** cb_b: does a.\n * @since 0.1\n */\nCB_API int cb_a(void);\n").find("names cb_b but the declaration after it is cb_a") != std::string::npos);
    CB_CHECK(header("/** cb_a: does a.\n */\nCB_API int cb_a(void);\n").find("has no @since") != std::string::npos);
    CB_CHECK(header("/** cb_a: does a.\n * @sinse 0.1\n */\nCB_API int cb_a(void);\n").find("@sinse is not a tag") != std::string::npos);
    const auto headers = ref::read(reference_checkout() / "include" / "cb");
    const auto pages = ref::pages(headers, {}, {});
    CB_CHECK(pages.contains("README.md") && pages.contains("binary_h.md") && pages.contains("functions/cb_binary_open.md"));
});

CB_TEST("sdk.abi.no_name_of_a_release_is_gone", [] {
    // (a major version may remove names: then the baseline is made again, and the reference says what moved)
    std::set<std::string> now;
    for (const ref::Header& h : ref::read(reference_checkout() / "include" / "cb"))
        for (const ref::Entry& e : h.entries)
            if (e.kind == "function") now.insert(e.name);
    std::ifstream in(reference_checkout() / "tools" / "sdk" / "abi_baseline.txt");
    std::size_t names = 0;
    for (std::string line; std::getline(in, line);) {
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ')) line.pop_back();
        if (line.empty() || line[0] == '#') continue;
        ++names;
        if (!now.contains(line)) ::cb::testing::record_failure(__FILE__, __LINE__, line + " was exported by a release and is gone from include/cb");
    }
    CB_CHECK(names > 0);
});

CB_TEST("sdk.includes.only_public_faces", [] {
    // src/sdk is a thin layer: what it may include of the engines (docs/SDK_DESIGN.md, phase 2)
    const std::set<std::string> faces = {"base/config.hpp", "formats/inspect.hpp", "source/processors.hpp", "decompile/decompiler.hpp",
                                         "engine/lane_kit.hpp"};
    const stdfs::path sdk = stdfs::path(CB_SOURCE_DIR) / "sdk";
    for (const auto& entry : stdfs::directory_iterator(sdk)) {
        const std::string name = cb::fs::to_text(entry.path().filename());
        if (name.ends_with("_test.cpp") || name.ends_with("_main.cpp") || !(name.ends_with(".cpp") || name.ends_with(".hpp"))) continue;
        std::ifstream in(entry.path());
        for (std::string line; std::getline(in, line);) {
            if (!line.starts_with("#include \"")) continue;
            const std::string inc = line.substr(10, line.find('"', 10) - 10);
            const bool ok = inc.starts_with("core/") || inc.starts_with("sdk/") || inc.starts_with("cb/") || faces.contains(inc);
            if (!ok) ::cb::testing::record_failure(__FILE__, __LINE__, "src/sdk/" + name + " includes " + inc + ", which is not an engine's public face");
        }
    }
});
