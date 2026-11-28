// The C# back end over real programs (the old writer's harness, test-inputs/csharp-harness, moved onto the one C# back end): the units
// of the translator's test programs as their front ends lowered them, checked in under translate/testdata, written by L2CSharpProgram --
// the Java programs through the rule set java, the C# ones as C#'s own. Each program is written whole, and each of its units alone; the
// answer to each is every file's name, length and CRC-32 (a runtime file's name alone), and the ledger (or the error). The recorded answers are
// dotnet/testdata/l2_csharp_harness.txt.
//
// The recording was reviewed: every whole program was built with Roslyn and the Java ones run against the JVM on their sources
// (work/stage15/judge_harness.sh). With CB_L2CS_OUT set to a folder the files are written there (harness/PROGRAM/...) to read or build;
// with CB_L2CS_HARNESS_RECORD=1 the answers are written to the recording instead of compared with it.
#include "dotnet/decompiler/csharp/l2_syntax_builder.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <map>

#include <nlohmann/json.hpp>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "translate/l2.hpp"

namespace {

namespace dcs = cb::dotnet::decompiler::csharp;
namespace ir = cb::translate::ir;
namespace stdfs = std::filesystem;

struct HarnessProgram {
    std::string name;
    std::string lang;   // "java" or "csharp"
    std::vector<stdfs::path> unit_files;   // one JSON document of Units, or a Unit, each
};

// The programs: translate/testdata/cpp03/* (Java), c_java/zoo and mix (Java), cs2cpp/* (C#).
std::vector<HarnessProgram> harness_programs() {
    std::vector<HarnessProgram> out;
    const stdfs::path base = cb::testing::data_path("translate/testdata");
    const auto numbered = [](const stdfs::path& dir) {   // units/0.json, 1.json ... in their number's order
        std::vector<stdfs::path> files;
        for (const auto& e : stdfs::directory_iterator(dir))
            if (e.path().extension() == ".json") files.push_back(e.path());
        std::sort(files.begin(), files.end(), [](const stdfs::path& a, const stdfs::path& b) { return std::stoi(a.stem().string()) < std::stoi(b.stem().string()); });
        return files;
    };
    const auto folders = [](const stdfs::path& dir) {
        std::vector<stdfs::path> found;
        for (const auto& e : stdfs::directory_iterator(dir))
            if (e.is_directory() && stdfs::exists(e.path() / "units")) found.push_back(e.path());
        std::sort(found.begin(), found.end());
        return found;
    };
    for (const stdfs::path& d : folders(base / "cpp03")) out.push_back({d.filename().string(), "java", numbered(d / "units")});
    for (const char* n : {"zoo", "mix"}) out.push_back({n, "java", {base / "c_java" / n / "units.l2.json"}});
    for (const stdfs::path& d : folders(base / "cs2cpp")) out.push_back({"cs_" + d.filename().string(), "csharp", numbered(d / "units")});
    return out;
}

const cb::java::apimap::Registry& harness_registry() {
    static const cb::java::apimap::Registry reg = cb::java::apimap::Registry::load(cb::testing::data_path("../config/java/api_maps"));
    return reg;
}

// One question's answer: {"files": [[name, length, crc32]], "ledger": [[where, kind, why]]}, or {"error": what}.
cb::Json harness_answer(std::span<ir::Node* const> units, bool java, const std::optional<stdfs::path>& out) {
    dcs::L2CSharpOptions options;
    if (java) {
        options.java_registry = &harness_registry();
        options.runtime_dir = cb::testing::data_path("../config/translate/dotnet/runtime");
    }
    cb::Json answer = cb::Json::object();
    try {
        dcs::L2CSharpProgram p(units, options);
        cb::Json files = cb::Json::array();
        for (const dcs::L2File& f : p.files()) {
            const bool runtime = java && (f.name == "JavaRt.cs" || f.name == "JavaCollections.cs" || f.name == "JavaStreams.cs" || f.name == "JavaCharTypes.cs");
            if (runtime) files.push_back({f.name});   // (a runtime file is copied, not written: which ones are used is the answer)
            else files.push_back({f.name, f.text.size(), cb::crypto::crc32(cb::as_bytes(f.text))});
        }
        cb::Json ledger = cb::Json::array();
        for (const dcs::L2Gap& g : p.ledger()) ledger.push_back({g.where, g.kind, g.why});
        answer["files"] = std::move(files);
        answer["ledger"] = std::move(ledger);
        if (out) p.write(*out);
    } catch (const cb::Error& e) {
        answer = cb::Json::object();
        answer["error"] = e.what();
    }
    return answer;
}

}  // namespace

CB_TEST("dotnet.l2_csharp.harness.recorded_programs_agree", [] {
    const stdfs::path recording = cb::testing::data_path("dotnet/testdata/l2_csharp_harness.txt");
    const bool record = cb::fs::get_env("CB_L2CS_HARNESS_RECORD") == std::optional<std::string>("1");
    const std::optional<std::string> out_env = cb::fs::get_env("CB_L2CS_OUT");
    std::map<std::string, std::string> want;   // key -> answer (one JSON line)
    const std::string recorded_text = record ? std::string() : cb::fs::read_text(recording);
    if (!record) {
        for (const std::string_view line : cb::text::split(recorded_text, '\n')) {
            const auto tab = line.find('\t');
            if (tab != std::string_view::npos) want.emplace(std::string(line.substr(0, tab)), std::string(line.substr(tab + 1)));
        }
    }
    std::string recorded;
    std::size_t asked = 0, differ = 0;
    std::int64_t total_us = 0;
    for (const HarnessProgram& prog : harness_programs()) {
        cb::Arena arena;
        std::vector<ir::Node*> units;
        for (const stdfs::path& f : prog.unit_files) {
            const cb::Json doc = cb::json::load(f);
            if (doc.is_array()) {
                for (const cb::Json& d : doc) units.push_back(cb::translate::l2::from_json(arena, d));
            } else {
                units.push_back(cb::translate::l2::from_json(arena, doc));
            }
        }
        for (ir::Node* u : units) ir::get<ir::Unit>(u)->lang = prog.lang;
        const bool java = prog.lang == "java";
        std::vector<std::pair<std::string, std::vector<ir::Node*>>> questions = {{prog.name + "|whole", units}};
        if (units.size() > 1) {
            for (std::size_t i = 0; i < units.size(); ++i) questions.push_back({prog.name + "|unit|" + std::to_string(i), {units[i]}});
        }
        for (const auto& [key, asked_units] : questions) {
            std::optional<stdfs::path> out;
            if (out_env && key.ends_with("|whole")) out = stdfs::path(*out_env) / "harness" / prog.name;
            const auto t0 = std::chrono::steady_clock::now();
            const std::string answer = cb::json::dump(harness_answer(asked_units, java, out), cb::json::Style::Compact);
            total_us += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count();
            ++asked;
            if (record) {
                recorded += key + "\t" + answer + "\n";
                continue;
            }
            const auto it = want.find(key);
            if (it == want.end() || it->second != answer) {
                ++differ;
                std::fprintf(stderr, "%s: %s\n  recorded %s\n", key.c_str(), answer.substr(0, 400).c_str(), it == want.end() ? "nothing" : it->second.substr(0, 400).c_str());
            }
        }
    }
    if (record) cb::fs::write_text(recording, recorded);
    std::fprintf(stderr, "l2_csharp harness: %zu questions, %zu differ from the recording, writing took %lld us\n", asked, differ, static_cast<long long>(total_us));
    CB_CHECK_EQ(differ, std::size_t{0});
    CB_CHECK(record || asked == want.size());
    CB_CHECK(asked > 40);
});
