// source/vxd/loadable_corpus.cpp -- the whole VxD corpus through the from-source loadable build (see loadable_corpus.hpp).
// cb-unity: alone
#include "source/vxd/loadable_corpus.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <format>
#include <map>
#include <nlohmann/json.hpp>

#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/le.hpp"
#include "jobs/pool.hpp"
#include "source/vxd/corpus.hpp"

namespace cb::source::vxd::loadable_corpus {

namespace stdfs = std::filesystem;
namespace le = formats::le;

namespace {

struct LoadableTodo {
    std::string key, name;
    const le::Module* module;
};

double loadable_seconds(std::chrono::steady_clock::time_point start) {
    return std::round(std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() * 10.0) / 10.0;
}

nlohmann::ordered_json build_one(const from_source::Environment& env, const le::Module& module) {
    const auto start = std::chrono::steady_clock::now();
    try {
        const from_source::Built built = from_source::build(env, module);
        return {{"ok", true},
                {"stage", nullptr},
                {"seconds", loadable_seconds(start)},
                {"objects", built.objects},
                {"device_id", built.device_id},
                {"device_id_ok", built.device_id_ok},
                {"identity", built.page_map_identity},
                {"resource_len", built.resource_length},
                {"vxd_len", built.vxd_length}};
    } catch (const std::exception& e) {   // one module of the corpus: its failure is its record, the run goes on
        return {{"ok", false}, {"stage", "exception"}, {"log", std::string(e.what()).substr(0, 200)}, {"seconds", loadable_seconds(start)}};
    }
}

nlohmann::ordered_json load_results(const stdfs::path& file) {
    std::error_code ec;
    if (!stdfs::exists(file, ec)) return nlohmann::ordered_json::object();
    return nlohmann::ordered_json::parse(fs::read_text(file));
}

// Laid out with indent 1, non-ASCII escaped, renamed over the old file whole.
void save_results(const stdfs::path& file, const nlohmann::ordered_json& results) {
    fs::make_dirs(file.parent_path().empty() ? stdfs::path(".") : file.parent_path());
    fs::write_file_atomic(file, as_bytes(results.dump(1, ' ', true)));
}

std::string capitalised_bool(bool b) { return b ? "True" : "False"; }

void summarise(const nlohmann::ordered_json& results, double seconds) {
    std::size_t ok = 0, device_ok = 0, identity = 0;
    std::vector<std::pair<std::string, std::size_t>> stages;   // by first appearance
    for (const auto& [key, r] : results.items()) {
        if (r.value("ok", false)) {
            ++ok;
            device_ok += r.value("device_id_ok", false);
            identity += r.value("identity", false);
            continue;
        }
        const std::string stage = r.contains("stage") && r["stage"].is_string() ? r["stage"].get<std::string>() : "None";
        const auto at = std::find_if(stages.begin(), stages.end(), [&](const auto& s) { return s.first == stage; });
        if (at == stages.end()) stages.emplace_back(stage, 1);
        else ++at->second;
    }
    out("\n== loadable corpus: {}/{} BUILD | {} device-id-correct | {} identity-map | {:.0f}s ==\n", ok, results.size(), device_ok, identity, seconds);
    if (!stages.empty()) {
        std::string shown;
        for (const auto& [stage, count] : stages) shown += std::format("{}'{}': {}", shown.empty() ? "" : ", ", stage, count);
        out("   failures by stage: {{{}}}\n", shown);
    }
}

}  // namespace

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "loadable-corpus",
        "every driver of a folder through the from-source build",
        {cli::Positional{"folder", "the drivers' folder (default: the corpus's)", false},
         cli::Positional{"results", "the results file, kept after every module (default: the state folder's vxd/loadable.json)", false},
         cli::Option{"--redo", "", "modules to do again, though the file has them (a,b,c)", cli::Arg::Value, "NAMES"},
         cli::Option{"--workers", "", "how many modules are built at once", cli::Arg::Value, "N", "4"}},
        [&](const cli::Args& a) -> int {
            const stdfs::path folder = a.value("folder") ? fs::from_text(*a.value("folder")) : env.corpus.root();
            const stdfs::path results_file = a.value("results") ? fs::from_text(*a.value("results")) : env.results_file;
            const std::set<std::string> redo = corpus::redo_set(a.value_or("redo", ""));
            const int workers = a.integer<int>("workers").value_or(4);

            nlohmann::ordered_json results = load_results(results_file);
            std::vector<std::vector<std::pair<std::string, le::Module>>> by_file;
            for (const stdfs::path& file : corpus::driver_files(folder)) {
                try {
                    by_file.push_back(le::open_modules(file));
                } catch (const Error& e) {
                    results[fs::to_text(file.filename())] = {{"ok", false}, {"stage", "open"}, {"log", std::string(e.what()).substr(0, 80)}};
                }
            }
            std::vector<LoadableTodo> todo;
            for (const auto& modules : by_file) {
                for (const auto& [name, module] : modules) {
                    const std::string key = text::lower(name.substr(0, name.find('.')));
                    if (results.contains(key) && !redo.contains(key)) continue;   // resume: recorded already
                    todo.push_back({key, name, &module});
                }
            }
            const auto start = std::chrono::steady_clock::now();
            out("== {} modules to build, {} workers ==\n", todo.size(), workers);
            flush_out();
            jobs::AsDoneOptions<LoadableTodo, nlohmann::ordered_json> options;
            options.jobs = workers;
            jobs::as_done(
                todo, [&](const LoadableTodo& t) { return build_one(env.build, *t.module); },
                [&](const LoadableTodo& t, nlohmann::ordered_json& r) {
                    results[t.key] = r;
                    save_results(results_file, results);
                    if (r["ok"].get<bool>()) {
                        out("  {:<14} OK   objs={:<2} dev=0x{:04x} dev_ok={:<5} ident={:<5} res={:<4} [{:.1f}s]\n", t.key, r["objects"].get<std::size_t>(),
                            r["device_id"].get<std::uint32_t>(), capitalised_bool(r["device_id_ok"].get<bool>()), capitalised_bool(r["identity"].get<bool>()),
                            r["resource_len"].get<std::uint32_t>(), r["seconds"].get<double>());
                    } else {
                        std::string log = r.value("log", "");
                        log = log.size() > 90 ? log.substr(log.size() - 90) : log;
                        std::replace(log.begin(), log.end(), '\n', ' ');
                        out("  {:<14} FAIL@{:<10} {}\n", t.key, r["stage"].get<std::string>(), log);
                    }
                    flush_out();
                },
                options);
            summarise(results, std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count());
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("loadable-corpus", {command});
    std::vector<std::string> line = {"loadable-corpus"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::loadable_corpus
