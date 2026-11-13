// The real exact projects of the Microsoft-Assemblers-Compilers research (C 3.00's P0-P3, LIB, LINK, MASM ...), as the tests of carve, bss, names, link
// and run open them, and the golden (testdata/w5b3.golden.json) the live Python made of them (testdata/gen_w5b3.py). Included by those *_test.cpp
// files only. The research folder is the sibling of the checkout (CB_RESEARCH names another), the kits and profiles are the checkout's
// (settings.json; CB_MAIN_CHECKOUT names another checkout): a machine that has none of it passes these tests without running them, and says so.
#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/carve.hpp"
#include "cb/exact/project.hpp"
#include "cb/exact/toolchain.hpp"

namespace cb::exact::test {

inline std::filesystem::path research_root() {
    if (const auto env = fs::get_env("CB_RESEARCH")) return fs::from_text(*env);
    return "C:/Users/mrtit/Documents/GitHub/Microsoft-Assemblers-Compilers/research";
}

inline std::filesystem::path main_checkout() {
    if (const auto env = fs::get_env("CB_MAIN_CHECKOUT")) return fs::from_text(*env);
    return "C:/Users/mrtit/Documents/GitHub/CodeBreaker";
}

inline const Json& golden() {
    static const Json doc = json::load(testing::data_path("exact/testdata/w5b3.golden.json"));
    return doc;
}

// A text of every key and value in order, so that two documents compare as documents.
inline std::string canon(const Json& j) {
    if (j.is_null()) return "null";
    if (j.is_boolean()) return j.get<bool>() ? "true" : "false";
    if (j.is_number_integer()) return std::to_string(j.get<std::int64_t>());
    if (j.is_string()) return "\"" + j.get<std::string>() + "\"";
    if (j.is_array()) {
        std::string out = "[";
        for (const Json& x : j) out += canon(x) + ",";
        return out + "]";
    }
    std::map<std::string, std::string> sorted;
    for (auto it = j.begin(); it != j.end(); ++it) sorted[it.key()] = canon(it.value());
    std::string out = "{";
    for (const auto& [k, x] : sorted) out += k + ":" + x + ",";
    return out + "}";
}

inline void check(bool ok, const std::string& what) {
    testing::count_check();
    if (!ok) testing::record_failure(__FILE__, __LINE__, what);
}

struct Real {
    fs::ScopedTempDir state{"w5b3-state"};
    toolchain::Locations locations;
    std::unique_ptr<toolchain::Toolchains> toolchains;
    std::shared_ptr<project::Services> services;
    bool ok = false;

    Real() {
        const std::filesystem::path root = main_checkout();
        std::error_code ec;
        if (!std::filesystem::is_directory(research_root() / "msc-3.0", ec) || !std::filesystem::is_regular_file(root / "settings.json", ec)) return;
        locations = toolchain::Locations::of_checkout(root);
        locations.profiles = testing::data_path("../../codebreaker/exact/toolchains");
        locations.state = state.path();   // (a stage's files, if any: not the checkout's)
        try {
            (void)locations.kit("msc300");
        } catch (const Error&) {
            return;
        }
        toolchains = std::make_unique<toolchain::Toolchains>(locations, nullptr, nullptr);
        services = std::make_shared<project::Services>();
        services->toolchains = toolchains.get();
        services->carve = carve::service();
        ok = true;
    }

    std::shared_ptr<project::Project> open(const std::filesystem::path& folder) { return project::Project::open(folder, services); }
    std::shared_ptr<project::Project> open_named(const std::string& name) { return open(research_root() / name); }
};

// What a test says when the real projects are not there.
inline bool skipped(const Real& real, const char* test) {
    if (real.ok) return false;
    std::fprintf(stderr, "  (%s: the research projects or the kits are not here: not run)\n", test);
    return true;
}

// A copy of a project folder that can be written, its program's path made absolute.
inline std::filesystem::path copy_project(const std::string& name, const std::filesystem::path& to) {
    const std::filesystem::path from = research_root() / name;
    fs::copy_tree(from, to);
    Json cfg = json::load(to / "project.json");
    cfg["image"] = fs::to_text(std::filesystem::absolute(from / fs::from_text(cfg.at("image").get<std::string>())).lexically_normal());
    json::save(to / "project.json", cfg, json::Style::PythonCompatiblePretty);
    return to;
}

}  // namespace cb::exact::test
