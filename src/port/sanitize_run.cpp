// cb-unity: alone (helpers named for what they do here)
#include "port/sanitize_run.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <thread>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/parallel.hpp"
#include "core/text.hpp"
#include "port/tool_build.hpp"

namespace cb::port::tool {

namespace stdfs = std::filesystem;

std::string sanitize_key(const stdfs::path& out, const stdfs::path& cases, const stdfs::path& cc) {
    std::vector<cache::KeyPart> parts;
    parts.emplace_back(fs::to_text(stdfs::absolute(cc)));
    std::vector<stdfs::path> files;
    std::error_code ec;
    for (const stdfs::directory_entry& e : stdfs::directory_iterator(out, ec)) {
        const std::string ext = text::lower(fs::to_text(e.path().extension()));
        if (e.is_regular_file(ec) && (ext == ".c" || ext == ".h" || ext == ".asm")) files.push_back(e.path());
    }
    std::sort(files.begin(), files.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a.filename()) < fs::to_text(b.filename()); });
    for (const stdfs::path& f : files) {
        parts.emplace_back(fs::to_text(f.filename()));
        parts.emplace_back(fs::read_file(f));
    }
    try {
        // the cases file with its keys in order (the same cases written another way are the same cases)
        const nlohmann::json config = nlohmann::json::parse(fs::read_text(cases));
        parts.emplace_back(config.dump());
        const stdfs::path base = stdfs::absolute(cases).parent_path();
        const auto at = [&](const std::string& p) { return stdfs::path(p).is_absolute() ? stdfs::path(p) : base / fs::from_text(p); };
        if (const auto inputs = config.find("inputs"); inputs != config.end() && inputs->is_object()) {
            for (const auto& [name, value] : inputs->items()) {
                if (!value.is_string()) continue;
                const stdfs::path p = at(value.get<std::string>());
                if (stdfs::is_regular_file(p, ec)) parts.emplace_back(fs::read_file(p));
            }
        }
        if (const auto programs = config.find("programs"); programs != config.end() && programs->is_object()) {
            for (const auto& [name, value] : programs->items()) {
                const std::string port = value.is_object() && value.contains("port") && value["port"].is_string() ? value["port"].get<std::string>() : std::string();
                const stdfs::path p = at(port);
                if (!port.empty() && stdfs::is_regular_file(p, ec)) {
                    parts.emplace_back(name);
                    parts.emplace_back(static_cast<std::uint64_t>(stdfs::file_size(p, ec)));
                    parts.emplace_back(static_cast<std::int64_t>(cb::fs::last_write_time(p, ec).time_since_epoch().count()));
                }
            }
        }
    } catch (const std::exception&) {
        parts.emplace_back("?");   // cases that cannot be read: a key of their own, never the same as a good one
    }
    return cache::key(parts);
}

namespace {

// Waits (a little) while the machine has no memory to spare: the sanitizer's builds and runs want it.
void wait_for_memory(std::string_view what) {
    constexpr double kNeededGb = 2.0;
    for (int i = 0; i < 120; ++i) {   // ten minutes at most
        const double free_gb = parallel::available_gb();
        if (free_gb < 0 || free_gb >= kNeededGb) return;
        if (i == 0) log::info(std::format("port: waiting for memory for the {} ({:.1f} GB free)", what, free_gb));
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

std::vector<std::string> notes_of_mark(const stdfs::path& mark, const std::string& key) {
    try {
        const nlohmann::json last = nlohmann::json::parse(fs::read_text(mark));
        if (last.value("key", std::string()) != key) return {};
        std::vector<std::string> notes;
        for (const auto& n : last.value("notes", nlohmann::json::array()))
            if (n.is_string()) notes.push_back(n.get<std::string>());
        notes.emplace_back("(the port and its cases as the last run: not run again)");
        return notes;
    } catch (const std::exception&) {
        return {};
    }
}

}  // namespace

std::vector<std::string> sanitize_loop(const SanitizeSetup& setup, const SanitizeHooks& hooks) {
    // the compiler with the sanitizer: the port's own, else clang, else gcc (MinGW's gcc has no sanitizer runtime)
    std::vector<std::string> names;
    if (setup.compiler != "msvc") names.push_back(setup.compiler);
    names.insert(names.end(), {"clang", "gcc"});
    std::optional<stdfs::path> cc;
    const Compiler* spec = nullptr;
    std::string name;
    for (const std::string& n : names) {
        const auto it = setup.compilers->find(n);
        if (it == setup.compilers->end()) continue;
        const std::optional<stdfs::path> found = find_compiler(it->second.find);
        if (found && !(n == "gcc")) {   // (this is Windows: gcc has no sanitizer runtime here)
            cc = found;
            spec = &it->second;
            name = n;
            break;
        }
    }
    if (!cc) return {"no compiler with AddressSanitizer found: the cases were not run sanitized"};

    const std::optional<stdfs::path> runtime = sanitize::runtime_dir(*cc);
    const stdfs::path work = setup.out / ".sanitize";
    stdfs::create_directories(work);
    const stdfs::path exe = work / "program.exe";
    std::vector<std::string> notes;
    hooks.write();
    // all it would build and run: as the last time, what it found then, not run again
    const std::string key = sanitize_key(setup.out, setup.cases, *cc);
    const stdfs::path mark = setup.out / ".sanitized.json";
    if (std::vector<std::string> same = notes_of_mark(mark, key); !same.empty()) {
        std::error_code ec;
        stdfs::remove_all(work, ec);
        return same;
    }

    const verify::Config config = verify::load_config(setup.cases);
    const stdfs::path base = stdfs::absolute(setup.cases).parent_path();
    bool clean = false;
    for (int round = 0; round < setup.rounds; ++round) {
        wait_for_memory("sanitized build");
        hooks.write();
        std::vector<std::string> tpl;
        for (const std::string& a : spec->build)
            if (a != "-O1" && a != "/O1") tpl.push_back(a);
        std::vector<std::string> flags = {"-g", "-fsanitize=address"};
        if (name == "clang") flags.emplace_back("-fsanitize=array-bounds");
        tpl.insert(tpl.begin() + (tpl.empty() ? 0 : 1), flags.begin(), flags.end());
        const BuildResult built = build(*setup.results, *spec, *cc, tpl, setup.out, setup.sources, exe, setup.jobs);
        if (!built.ok) {
            const std::string tail = built.log.size() > 300 ? built.log.substr(built.log.size() - 300) : built.log;
            notes.push_back("the sanitized build failed: " + tail);
            break;
        }
        std::map<std::string, std::string> env = {{"ASAN_OPTIONS", "detect_leaks=0"}};
        env["PATH"] = (runtime ? fs::to_text(*runtime) + ";" : std::string()) + fs::get_env("PATH").value_or("");
        std::vector<sanitize::Report> reports;
        for (const verify::CaseConsole& c : verify::run_cases(config, base, exe, env, *setup.services)) reports.push_back({c.name, c.console});
        const std::vector<sanitize::Mended> done = hooks.mend(reports);
        std::vector<std::string> bad;
        for (const sanitize::Report& r : reports)
            if (r.text.find("AddressSanitizer") != std::string::npos || r.text.find("runtime error: index") != std::string::npos) bad.push_back(r.case_name);
        if (done.empty()) {
            notes.push_back(std::format("round {}: {} cases run sanitized, {}", round + 1, reports.size(),
                                        bad.empty() ? "no fault" : "faults not mended: " + text::join(bad, ", ")));
            clean = true;
            break;
        }
        for (const sanitize::Mended& m : done) notes.push_back(std::format("round {}: {}", round + 1, m.why));
    }
    std::error_code ec;
    stdfs::remove_all(work, ec);
    if (clean) {   // settled: kept, with the key of what it ran
        try {
            nlohmann::json j;
            j["key"] = sanitize_key(setup.out, setup.cases, *cc);
            j["notes"] = notes;
            fs::write_text(mark, j.dump());
        } catch (const Error&) {
        }
    }
    return notes;
}

}  // namespace cb::port::tool
