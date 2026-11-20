// build/module_build.cpp -- build a module whole: prepare it, link it, run the gate, link again where the gate found a function that is not
// what the original is, check the image, run its exports; several modules side by side; the `build` command.

#include <algorithm>
#include <format>
#include <mutex>

#include "base/cache.hpp"
#include "build/module_internal.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/parallel.hpp"
#include "core/text.hpp"

namespace cb::build::module {

using namespace internal;

namespace {

void say(const std::function<void(std::string_view)>& log, std::string_view line) {
    if (log) log(line);
    else cb::out("{}\n", line);
}

}  // namespace

Built ModuleBuilder::build(std::string_view module, const BuildOptions& options) const {
    const Services& s = services_;
    const std::function<void(std::string_view)>& log = options.log ? options.log : s.log;
    const std::optional<stdfs::path> found = s.retail ? s.retail(module) : std::nullopt;
    if (!found || found->empty()) throw UsageError(std::format("no module '{}' in the corpus", module));
    const stdfs::path path = *found;
    Built b(path, s.kind_of ? s.kind_of(path) : std::nullopt);
    if (!b.kind || (*b.kind != "ne" && *b.kind != "pe"))
        throw UsageError(std::format("{}: a {} module is not linked whole here", b.name, b.kind ? *b.kind : "None"));
    std::optional<OrderedMap<std::string, std::string>> gated = options.gated;
    if (gated) b.gated = *gated;
    b.folder = s.build_folder(b.name);   // (the working files live here; `outdir` is given the image alone)
    const stdfs::path folder = *b.folder;
    std::error_code ec;
    stdfs::create_directories(folder, ec);
    say(log, std::format("{}: decompiling", b.name));
    const Prepared prepared = s.prepare(path, folder / "decompiled");
    b.functions = prepared.functions;
    bool from_cache = false;
    const std::shared_ptr<decompile::c_text::Module16>& m16 = prepared.win16;
    const std::shared_ptr<decompile::c_text::Compilable32>& m32 = prepared.win32;
    if (*b.kind == "ne" ? !m16 : !m32) throw UsageError(std::format("{}: the module was not made ready for a build", b.name));
    if (m16 && !gated && !env_is_set("CB_NO_GATE")) {
        // every C function's verdict already known (kept by the gate): the ones that differ go to assembly now, and the module is built once instead of twice
        if (auto known = gate_known(path, *m16)) {
            b.gated = *known;
            gated = std::move(known);
            from_cache = true;
        }
    }
    if (m16 && m16->description) b.disputes = disputes(*m16->description);
    say(log, std::format("{}: compiling {} functions, data and resources", b.name, m16 ? m16->made.order().size() : m32->order.size()));
    const std::optional<stdfs::path> image = m16 ? build16(b, path, *m16, folder) : build32(b, path, *m32, folder);
    // (what the build carried that the module did not have is the module's now, for whoever builds from it again)
    const auto again = [&](OrderedMap<std::string, std::string> merged, bool gate) {
        BuildOptions next = options;
        next.gated = std::move(merged);
        next.gate = gate;
        return build(module, next);
    };
    if (!b.resident_overflow.empty()) {
        say(log, std::format("{}: {} placed past where a segment is cut: rebuilt from their instructions", b.name, b.resident_overflow.size()));
        // (and the gate still runs on what is left as C, if this build was to run it: the overflow is found before the gate, and a build handed its gated set runs none)
        OrderedMap<std::string, std::string> merged = b.gated;
        for (const auto& [name, why] : b.resident_overflow) merged.insert_or_assign(name, why);
        return again(std::move(merged), options.gate || !gated || from_cache);
    }
    if (!image || image->empty()) return b;
    b.image = folder / text::lower(path.filename().string());
    stdfs::copy_file(*image, *b.image, stdfs::copy_options::overwrite_existing);
    if (*b.kind == "ne") {
        whole_file_image(path, *b.image);
        if ((!gated || from_cache || options.gate) && !env_is_set("CB_NO_GATE")) {
            // (built with the kept verdicts: only a C function not among them is run now)
            std::set<std::string> c_functions = b.placed_c;
            c_functions.insert(b.moved_c.begin(), b.moved_c.end());
            // (the function CB_BISECT edits is not run beside the original: it is meant to differ -- module_build16.cpp)
            if (const std::optional<std::string> bisect = env_value("CB_BISECT")) c_functions.erase(bisect->substr(0, bisect->find(':')));
            const std::vector<std::string> c(c_functions.begin(), c_functions.end());
            say(log, std::format("{}: running {} functions rebuilt from C beside the original's", b.name, c.size()));
            std::vector<std::pair<std::string, std::string>> not_run;
            OrderedMap<std::string, std::string> differ = gate(path, folder, c, *m16, &not_run);
            std::vector<std::pair<std::string, std::string>> failed;
            for (const auto& nw : not_run) {
                if (nw.second.starts_with("the checker failed")) failed.push_back(nw);
            }
            if (!failed.empty()) {
                b.gate_error = std::format("{} functions ({})", failed.size(), failed[0].second.substr(0, 120));
                say(log, std::format("{}: the gate could not run {} functions: {}", b.name, failed.size(), failed[0].second.substr(0, 120)));
            }
            // (and its calls beside the original's: what each call is followed by -- the check above answers a call as the description believes; this
            // reads both as they are)
            if (s.gate_contract) {
                try {
                    for (const auto& [name, why] : s.gate_contract(path, folder, *b.image, c)) {
                        if (!differ.contains(name)) differ.insert_or_assign(name, "its calls are not the original's: " + why);
                    }
                } catch (const std::exception& e) {   // (a check that fails says so: never "all agree")
                    b.contract_error = e.what();
                    say(log, std::format("{}: the call-contract check FAILED: {}", b.name, *b.contract_error));
                }
            }
            if (s.cache != nullptr) {
                std::set<std::string> all(c.begin(), c.end());
                if (gated) {
                    for (const auto& [name, why] : *gated) all.insert(name);
                }
                const std::string kept = text::join(std::vector<std::string>(all.begin(), all.end()), "\n");
                s.cache->put("gate16_set", base::cache::key({fs::read_file(path), m16->made.header()}),
                             ByteView(reinterpret_cast<const std::uint8_t*>(kept.data()), kept.size()));
            }
            if (!differ.empty()) {
                say(log, std::format("{}: {} do not do what the original does: rebuilt from their instructions", b.name, differ.size()));
                OrderedMap<std::string, std::string> merged = gated ? *gated : OrderedMap<std::string, std::string>();
                for (const auto& [name, why] : differ) merged.insert_or_assign(name, why);
                return again(std::move(merged), false);
            }
        }
    }
    check(b);
    if (options.outdir) {
        stdfs::create_directories(*options.outdir, ec);
        const stdfs::path delivered = *options.outdir / b.image->filename();
        stdfs::copy_file(*b.image, delivered, stdfs::copy_options::overwrite_existing);
        b.image = delivered;
    }
    if (options.call) run(b);
    return b;
}

std::vector<std::pair<std::string, Built>> ModuleBuilder::build_many(const std::vector<std::string>& modules,
                                                                       const std::function<std::optional<stdfs::path>(const std::string&)>& outdir_of,
                                                                       bool call) const {
    const Services& s = services_;
    const auto outdir = [&](const std::string& m) { return outdir_of ? outdir_of(m) : std::nullopt; };
    const int jobs = s.workers ? s.workers() : parallel::workers();
    // (the gate and the judge run Unicorn and the tracer: builds side by side in one process would share them, so each module gets a process of its own when
    // processes can be started; else one at a time, unless CB_BUILD_THREADS asks for threads)
    const bool as_processes = static_cast<bool>(s.build_apart);
    const std::size_t procs = env_is_set("CB_BUILD_THREADS") || as_processes ? std::max<std::size_t>(1, std::min<std::size_t>(static_cast<std::size_t>(jobs), modules.size())) : 1;
    std::vector<std::pair<std::string, Built>> out;
    BuildOptions options;
    options.call = call;
    // (a module whose build throws -- a file it could not write, a tool that died -- is that module's failure: its Built says so, the rest are built)
    const auto isolated = [&](const std::string& m, const std::function<Built()>& make) -> Built {
        try {
            return make();
        } catch (const std::exception& e) {
            const std::optional<stdfs::path> found = s.retail ? s.retail(m) : std::nullopt;
            Built b(found && !found->empty() ? *found : stdfs::path(m), std::nullopt);
            b.error = e.what();
            return b;
        }
    };
    if (procs == 1) {
        for (const std::string& m : modules) {
            options.outdir = outdir(m);
            out.emplace_back(m, isolated(m, [&] { return build(m, options); }));
        }
        return out;
    }
    // the largest first, so that the longest is not the one started last
    const auto size_of = [&](const std::string& m) -> std::uint64_t {
        const auto p = s.retail ? s.retail(m) : std::nullopt;
        std::error_code ec;
        return p && stdfs::is_regular_file(*p, ec) ? fs::file_size(*p) : 0;
    };
    std::vector<std::string> by_size = modules;
    std::stable_sort(by_size.begin(), by_size.end(), [&](const std::string& a, const std::string& b) { return size_of(a) > size_of(b); });
    std::mutex log_lock;
    parallel::ThreadPool pool(procs);
    const std::vector<Built> made = parallel::parallel_map(pool, by_size, [&](const std::string& m) -> Built {
        return isolated(m, [&]() -> Built {
        {
            std::lock_guard<std::mutex> lock(log_lock);
            say(s.log, std::format("{}: building (one of {} at once)", stdfs::path(m).filename().string(), procs));
        }
        if (as_processes) {
            if (std::optional<Built> apart = s.build_apart(m, outdir(m), call)) return std::move(*apart);
        }
        BuildOptions one;
        one.call = call;
        one.outdir = outdir(m);
        one.log = [](std::string_view) {};
        return build(m, one);
        });
    });
    std::map<std::string, const Built*> by_name;
    for (std::size_t i = 0; i < by_size.size(); ++i) by_name[by_size[i]] = &made[i];
    for (const std::string& m : modules) out.emplace_back(m, *by_name.at(m));
    return out;
}

cli::Command ModuleBuilder::command() const {
    return cli::Command{
        "build",
        "build a whole module from its recovered source and check what came out",
        {cli::Positional{"module", "a corpus name or a file", true, true},
         cli::Option{"--out", "", "where the build goes (default: the module's project folder, build)", cli::Arg::Value, "DIR"},
         cli::Option{"--no-run", "", "do not call the built module's exports in the emulator", cli::Arg::Flag},
         cli::Option{"--engine", "", "accepted, as the old command line had it; the direct path is the one taken", cli::Arg::Flag},
         cli::Option{"--no-engine", "", "accepted, as the old command line had it; the direct path is the one taken", cli::Arg::Flag}},
        [this](const cli::Args& args) {
            const Services& s = services_;
            const std::vector<std::string> names = args.values("module");
            const std::optional<std::string> out = args.value("out");
            const bool call = !args.flag("no-run");
            std::map<std::string, std::optional<stdfs::path>> where_of;
            std::map<std::string, stdfs::path> originals;
            for (const std::string& name : names) {
                if (out) {
                    where_of[name] = fs::from_text(*out);
                    continue;
                }
                const auto path = s.retail ? s.retail(name) : std::nullopt;
                if (path && s.project_build_folder) {
                    originals[name] = *path;
                    where_of[name] = s.project_build_folder(*path);
                }
            }
            // (several: side by side, as many at once as the budget -- build_many; one: here)
            std::vector<std::pair<std::string, Built>> made;
            if (names.size() > 1 && !out) {
                made = build_many(names, [&](const std::string& m) { return where_of[m]; }, call);
            }
            int bad = 0;
            for (const std::string& name : names) {
                const auto done = std::find_if(made.begin(), made.end(), [&](const auto& nb) { return nb.first == name; });
                BuildOptions options;
                options.call = call;
                options.outdir = where_of[name];
                const Built b = done != made.end() ? done->second : build(name, options);
                cb::out("{}\n", b.report());
                if (s.record_build && originals.contains(name)) {
                    s.record_build(originals.at(name), std::string("the whole module built from its recovered source: ") +
                                                           (b.linked() ? "linked: " + fs::to_text(*b.image) : "did not link"));
                }
                if (b.linked()) cb::out("  image      {}\n", fs::to_text(*b.image));
                bad += b.linked() ? 0 : 1;
            }
            return bad != 0 ? 1 : 0;
        },
        {},
        "Build",
        ""};
}

}  // namespace cb::build::module
