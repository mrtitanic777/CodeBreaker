// app/layers_test.cpp -- the layering check of docs/SDK_DESIGN.md (rule 3: layers point one way), run with every gate.
//
// Every #include in product code under src/ whose target lives in a higher layer than the includer is an upward include. The ones
// still in the tree are listed in tools/sdk/layer_baseline.txt. The test fails on an upward include not in the list (a new one: move
// the shared piece down, or invert the dependency with an interface the lower area owns) and on a listed one that no longer occurs
// (fixed: take it off, so the list only shrinks). CB_LAYERS_UPDATE=1 rewrites the list from the tree; use it only after removing
// upward includes.
//
// It sits in app, the top layer, because it reads the whole tree and depends on nothing but core.

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace {

namespace stdfs = std::filesystem;

// Lowest first: "The corrected layering" of docs/SDK_DESIGN.md. An area not listed is not checked.
const std::vector<std::vector<std::string_view>>& layers() {
    static const std::vector<std::vector<std::string_view>> l = {
        {"core", "base"},
        {"formats", "platforms", "l2", "symbols", "store", "knowledge", "jobs"},
        {"decompile", "emulate", "analyze", "invert", "exact", "coverage", "compare"},
        {"translate", "source", "build", "naming", "dotnet", "java", "xna", "recompile", "port", "verify"},
        {"engine"},
        {"app", "ui", "sdk"},
    };
    return l;
}

int layer_of(std::string_view area) {
    for (std::size_t i = 0; i < layers().size(); ++i)
        if (std::find(layers()[i].begin(), layers()[i].end(), area) != layers()[i].end()) return static_cast<int>(i);
    return -1;
}

// The area a file belongs to: its first folder, except translate's typed tree and documents, which are the L2 hub everything emits into.
std::string area_of(std::string_view rel) {
    for (std::string_view l2 : {"translate/ir/", "translate/ir.hpp", "translate/ir.cpp", "translate/l2.hpp", "translate/l2.cpp"})
        if (rel.starts_with(l2)) return "l2";
    return std::string(rel.substr(0, rel.find('/')));
}

// Tests, their helpers (*_test_image.hpp: test inputs built in code, included only by tests) and programs of their own (*_main.cpp:
// harnesses and helper hosts, built apart from the area's library) sit above every layer.
bool is_test(std::string_view rel) {
    return rel.ends_with("_main.cpp") || rel.ends_with("_test_image.hpp") || rel.ends_with("_test.cpp") || rel.ends_with("_test.cc") || rel.ends_with("_test.hpp") || rel.find("/tests/") != std::string_view::npos;
}

bool is_source(const stdfs::path& p) {
    static const std::set<std::string> ext = {".cpp", ".cc", ".cxx", ".c", ".hpp", ".hh", ".h", ".inc"};
    return ext.contains(p.extension().string());
}

std::string normal(const stdfs::path& p) { return p.lexically_normal().generic_string(); }

std::set<std::string> upward_includes(const stdfs::path& src) {
    std::set<std::string> files;
    for (auto it = stdfs::recursive_directory_iterator(src); it != stdfs::recursive_directory_iterator(); ++it) {
        const std::string name = it->path().filename().string();
        if (it->is_directory() && (name == "third_party" || name == "testdata" || name == "tests")) {
            it.disable_recursion_pending();
            continue;
        }
        if (it->is_regular_file() && is_source(it->path())) files.insert(normal(stdfs::relative(it->path(), src)));
    }
    std::set<std::string> found;
    for (const std::string& rel : files) {
        if (is_test(rel)) continue;
        const std::string from_area = area_of(rel);
        const int from_layer = layer_of(from_area);
        if (from_layer < 0) continue;
        std::ifstream in(src / rel);
        const stdfs::path here = stdfs::path(rel).parent_path();
        for (std::string line; std::getline(in, line);) {
            const std::size_t hash = line.find_first_not_of(" \t");
            if (hash == std::string::npos || line[hash] != '#') continue;
            const std::size_t word = line.find_first_not_of(" \t", hash + 1);
            if (word == std::string::npos || line.compare(word, 7, "include") != 0) continue;
            const std::size_t open = line.find('"', word + 7);
            const std::size_t close = open == std::string::npos ? open : line.find('"', open + 1);
            if (close == std::string::npos) continue;
            const std::string inc = line.substr(open + 1, close - open - 1);
            std::string to;
            for (const std::string& cand : {normal(here / inc), normal(stdfs::path(inc))})
                if (files.contains(cand)) {
                    to = cand;
                    break;
                }
            if (to.empty()) continue;
            const std::string to_area = area_of(to);
            const int to_layer = layer_of(to_area);
            if (to_area != from_area && to_layer > from_layer) found.insert(rel + " -> " + to);
        }
    }
    return found;
}

}  // namespace

CB_TEST("app.layers.includes_point_down_except_the_listed_ones", [] {
    const stdfs::path src = stdfs::path(CB_SOURCE_DIR);
    const stdfs::path baseline = src.parent_path() / "tools" / "sdk" / "layer_baseline.txt";
    const std::set<std::string> now = upward_includes(src);
    if (cb::fs::get_env("CB_LAYERS_UPDATE").value_or("") == "1") {
        std::ofstream out(baseline, std::ios::binary);
        out << "# Upward includes still in the tree (app.layers test, docs/SDK_DESIGN.md rule 3). This list only shrinks.\n";
        for (const std::string& line : now) out << line << "\n";
        return;
    }
    std::set<std::string> known;
    std::ifstream in(baseline);
    for (std::string line; std::getline(in, line);) {
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ')) line.pop_back();
        if (!line.empty() && line[0] != '#') known.insert(line);
    }
    std::vector<std::string> added, fixed;
    std::set_difference(now.begin(), now.end(), known.begin(), known.end(), std::back_inserter(added));
    std::set_difference(known.begin(), known.end(), now.begin(), now.end(), std::back_inserter(fixed));
    std::ostringstream why;
    for (const std::string& a : added) why << "\n  new upward include (move the shared piece down, or invert the dependency): " << a;
    for (const std::string& f : fixed) why << "\n  fixed (take it off tools/sdk/layer_baseline.txt, or run with CB_LAYERS_UPDATE=1): " << f;
    if (!added.empty() || !fixed.empty()) ::cb::testing::record_failure(__FILE__, __LINE__, "the layering moved:" + why.str());
});
