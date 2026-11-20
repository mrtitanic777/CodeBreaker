// source/vxd/corpus.cpp -- the whole VxD corpus through assemble-and-compare (see corpus.hpp).
// cb-unity: alone
#include "source/vxd/corpus.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <format>
#include <nlohmann/json.hpp>

#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"
#include "formats/le.hpp"
#include "jobs/pool.hpp"

namespace cb::source::vxd::corpus {

namespace stdfs = std::filesystem;
namespace le = formats::le;

namespace {

constexpr std::array<std::string_view, 4> kDriverExtensions = {".vxd", ".386", ".pdr", ".mpd"};
constexpr std::array<std::string_view, 3> kExtraFiles = {"csmapper.sys", "bigmem.drv", "drvspace.bin"};

struct Todo {
    std::string key, name;
    const le::Module* module;
};

double seconds_since(std::chrono::steady_clock::time_point start) {
    return std::round(std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() * 10.0) / 10.0;
}

// One module through the object-level test, as the record the results file keeps.
nlohmann::ordered_json compare_one(const le::Module& module, const DdkNames& names, const masm::Toolchain& tools) {
    const auto start = std::chrono::steady_clock::now();
    try {
        const masm::Emission code(Disassembly::run(module, names), names);
        const masm::ModuleReport report = masm::compare_module(code, tools);
        nlohmann::ordered_json objects = nlohmann::ordered_json::array();
        std::size_t exact = 0;
        for (const masm::ObjectReport& o : report.objects) {
            exact += o.match;
            objects.push_back({{"obj", o.object},
                               {"match", o.match},
                               {"ok", true},
                               {"len", o.original_length},
                               {"symbolic", o.symbolic},
                               {"forced_raw", o.forced_raw},
                               {"raw_fallback", o.symbolic == 0 && o.symbolic + o.forced_raw > 0},
                               {"diffs", o.match ? 0 : 1},
                               {"err", nullptr}});
        }
        return {{"byte_exact", report.byte_exact},       {"objects", std::move(objects)},    {"n_obj", report.objects.size()},
                {"n_exact", exact},                      {"symbolic", report.symbolic},      {"forced_raw", report.forced_raw},
                {"raw_fallback_objs", report.all_raw_objects}, {"seconds", seconds_since(start)}};
    } catch (const std::exception& e) {   // one module of the corpus: its failure is its record, the run goes on
        return {{"error", std::string(e.what()).substr(0, 120)}};
    }
}

nlohmann::ordered_json load_results(const stdfs::path& file) {
    std::error_code ec;
    if (!stdfs::exists(file, ec)) return nlohmann::ordered_json::object();
    std::string raw = fs::read_text(file);
    return nlohmann::ordered_json::parse(raw);
}

// A copy beside the file, then renamed over it: a reader never sees half of it. Laid out with indent 1, non-ASCII escaped.
void save_results(const stdfs::path& file, const nlohmann::ordered_json& results) {
    fs::make_dirs(file.parent_path().empty() ? stdfs::path(".") : file.parent_path());
    fs::write_file_atomic(file, as_bytes(results.dump(1, ' ', true)));
}

void summarise(const nlohmann::ordered_json& results, double seconds) {
    std::size_t modules = 0, full = 0, total_objects = 0, exact_objects = 0, symbolic = 0, fallback = 0, errors = 0;
    for (const auto& [key, r] : results.items()) {
        if (r.contains("error")) {
            ++errors;
            continue;
        }
        ++modules;
        full += r.value("byte_exact", false);
        total_objects += r.value("n_obj", std::size_t{0});
        exact_objects += r.value("n_exact", std::size_t{0});
        symbolic += r.value("symbolic", std::size_t{0});
        fallback += r.value("raw_fallback_objs", std::size_t{0});
    }
    out("\n== corpus: {} modules | {} fully byte-exact | {}/{} objects byte-exact | {} symbolic re-encodes | {} raw-fallback objs | {} errors | {:.0f}s ==\n",
        modules, full, exact_objects, total_objects, symbolic, fallback, errors, seconds);
}

}  // namespace

std::vector<stdfs::path> driver_files(const stdfs::path& folder) {
    std::vector<stdfs::path> candidates;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(folder, ec)) {
        const std::string extension = text::lower(fs::to_text(entry.path().extension()));
        if (std::find(kDriverExtensions.begin(), kDriverExtensions.end(), extension) != kDriverExtensions.end()) candidates.push_back(entry.path());
    }
    for (const std::string_view extra : kExtraFiles) candidates.push_back(folder / extra);
    std::sort(candidates.begin(), candidates.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) < fs::to_text(b); });
    candidates.erase(std::unique(candidates.begin(), candidates.end(),
                                 [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) == fs::to_text(b); }),
                     candidates.end());
    std::vector<stdfs::path> files;
    for (const stdfs::path& p : candidates)
        if (stdfs::exists(p, ec) && formats::detect::holds_drivers(p)) files.push_back(p);   // a stray real-mode .sys or .drv is skipped
    return files;
}

std::set<std::string> redo_set(const std::string& text) {
    std::set<std::string> names;
    for (const std::string_view name : text::split(text, ',')) {
        const std::string_view trimmed = text::trim(name);
        if (!trimmed.empty()) names.insert(std::string(trimmed));
    }
    return names;
}

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "corpus",
        "every driver of a folder through assemble-and-compare",
        {cli::Positional{"folder", "the drivers' folder (default: the corpus's)", false},
         cli::Positional{"results", "the results file, kept after every module (default: the state folder's vxd/byte_exact.json)", false},
         cli::Option{"--redo", "", "modules to do again, though the file has them (a,b,c)", cli::Arg::Value, "NAMES"},
         cli::Option{"--workers", "", "how many modules are assembled at once", cli::Arg::Value, "N", "4"}},
        [&](const cli::Args& a) -> int {
            const stdfs::path folder = a.value("folder") ? fs::from_text(*a.value("folder")) : env.corpus.root();
            const stdfs::path results_file = a.value("results") ? fs::from_text(*a.value("results")) : env.results_file;
            const std::set<std::string> redo = redo_set(a.value_or("redo", ""));
            const int workers = a.integer<int>("workers").value_or(4);

            nlohmann::ordered_json results = load_results(results_file);
            std::vector<std::vector<std::pair<std::string, le::Module>>> by_file;
            for (const stdfs::path& file : driver_files(folder)) {
                try {
                    by_file.push_back(le::open_modules(file));
                } catch (const Error& e) {
                    results[fs::to_text(file.filename())] = {{"error", std::string(e.what()).substr(0, 80)}};
                }
            }
            std::vector<Todo> todo;
            for (const auto& modules : by_file) {
                for (const auto& [name, module] : modules) {
                    const std::string key = text::lower(name);
                    if (results.contains(key) && !redo.contains(key) && !results[key].contains("error")) continue;   // resume: recorded already
                    todo.push_back({key, name, &module});
                }
            }
            const auto start = std::chrono::steady_clock::now();
            out("== {} modules to process, {} workers ==\n", todo.size(), workers);
            flush_out();
            jobs::AsDoneOptions<Todo, nlohmann::ordered_json> options;
            options.jobs = workers;
            jobs::as_done(
                todo, [&](const Todo& t) { return compare_one(*t.module, env.names, env.tools); },
                [&](const Todo& t, nlohmann::ordered_json& r) {
                    results[t.key] = r;
                    save_results(results_file, results);
                    if (r.contains("error")) {
                        out("  {:<14} ERROR {}\n", t.name, r["error"].get<std::string>().substr(0, 60));
                    } else {
                        out("  {:<14} {:>2}/{:>2} byte-exact  sym={:<6} raw={:<5} fb={}  {:<10} [{}s]\n", t.name, r["n_exact"].get<std::size_t>(),
                            r["n_obj"].get<std::size_t>(), r["symbolic"].get<std::size_t>(), r["forced_raw"].get<std::size_t>(),
                            r["raw_fallback_objs"].get<std::size_t>(), r["byte_exact"].get<bool>() ? "BYTE-EXACT" : "PARTIAL",
                            static_cast<int>(r["seconds"].get<double>()));
                    }
                    flush_out();
                },
                options);
            summarise(results, std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count());
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("corpus", {command});
    std::vector<std::string> line = {"corpus"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::corpus
