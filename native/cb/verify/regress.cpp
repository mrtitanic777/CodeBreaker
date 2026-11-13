#include "cb/verify/regress.hpp"

#include <algorithm>
#include <format>
#include <future>
#include <memory>
#include <mutex>
#include <set>
#include <stdlib.h>

#include <nlohmann/json.hpp>

#include "cb/base/runs.hpp"
#include "cb/build/module.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/engine/lane_kit.hpp"

namespace cb::verify::regress {

namespace stdfs = std::filesystem;
using Clock = std::chrono::steady_clock;

std::string_view word(Tier tier) {
    switch (tier) {
    case Tier::All15: return "all15";
    case Tier::Setup: return "setup";
    case Tier::Slow: return "slow";
    }
    return "?";
}

std::optional<Tier> tier_named(std::string_view name) {
    for (const Tier t : {Tier::All15, Tier::Setup, Tier::Slow})
        if (text::iequals(name, word(t))) return t;
    return std::nullopt;
}

const std::vector<std::string>& all15_names() {
    static const std::vector<std::string> names = {"KRNL386.EXE", "GDI.EXE",      "USER.EXE",     "KEYBOARD.DRV", "VGA.DRV",
                                                   "SYSTEM.DRV",  "MINIKBD.DLL",  "MSMOUS31.DRV", "NOMOUSE.DRV",  "LMOUSE31.DRV",
                                                   "SOUND.DRV",   "COMM.DRV",     "VER.DLL",      "LZEXPAND.DLL", "WIN87EM.DLL"};
    return names;
}

const std::vector<std::string>& setup_names() {
    static const std::vector<std::string> names = {"SUWIN.EXE",    "SETUPX.DLL",  "SYSDETMG.DLL", "NETDI.DLL",
                                                   "SUEXPAND.DLL", "W95INF16.DLL", "SETUP.EXE"};
    return names;
}

namespace {

std::string seconds_text(std::chrono::seconds s) {
    const auto n = s.count();
    if (n >= 3600) return std::format("{}h{:02}m{:02}s", n / 3600, (n / 60) % 60, n % 60);
    if (n >= 60) return std::format("{}m{:02}s", n / 60, n % 60);
    return std::format("{}s", n);
}

std::chrono::seconds since(Clock::time_point t) { return std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - t); }

bool in_list(const std::vector<std::string>& list, const std::string& name) {
    return std::any_of(list.begin(), list.end(), [&](const std::string& s) { return text::iequals(s, name); });
}

// The numbers STATUS.md gives the all-15 boot (USER C 509 / 61 gated, GDI C 118 / 24): looked at, not a verdict.
struct Expected {
    const char* name;
    std::size_t c, gated;
};
constexpr Expected kStatusNumbers[] = {{"USER.EXE", 509, 61}, {"GDI.EXE", 118, 24}};

}  // namespace

std::string Report::text() const {
    std::string out = std::format("regress {}\n", word(tier));
    if (!refused.empty()) {
        out += std::format("NOT RUN: {}\n", refused);
        return out;
    }
    if (!built.empty()) {
        out += "modules built\n";
        for (const Built& b : built) {
            out += std::format("  {:<13} {}  functions {} C {} asm {} stand-ins {} gated {}{}\n", b.name, b.linked ? "linked    " : "NOT LINKED",
                               b.functions, b.with_text >= b.assembled + b.rejected ? b.with_text - b.assembled - b.rejected : 0, b.assembled,
                               b.rejected, b.gated, b.error.empty() ? "" : "  (" + b.error + ")");
        }
    }
    if (!boots.empty()) {
        out += "boots (Setup to the License page)\n";
        for (const Boot& b : boots)
            out += std::format("  {:<28} {:<16} {}  screen {}\n", b.tag, setup16::word(b.verdict), seconds_text(b.took), fs::to_text(b.screenshot));
    }
    if (!cases.empty()) {
        out += "cases\n";
        for (const Case& c : cases) out += std::format("  {:<28} {:<8} {}{}\n", c.name, c.status, seconds_text(c.took), c.why.empty() ? "" : "  " + c.why);
    }
    for (const std::string& n : notes) out += std::format("note: {}\n", n);
    out += std::format("{} in {}\n", pass ? "PASS" : "FAIL", seconds_text(took));
    return out;
}

namespace {

Report run_slow(const Services& s, const Options& o) {
    Report r;
    r.tier = Tier::Slow;
    const Clock::time_point start = Clock::now();
    std::error_code ec;
    if (o.cases.empty() || !stdfs::is_regular_file(o.cases, ec)) {
        r.refused = "no cases file: " + fs::to_text(o.cases) + " (--cases)";
        return r;
    }
    if (!o.exe || !stdfs::is_regular_file(*o.exe, ec) || !s.run_exe) {
        r.refused = "no codebreaker program to run the cases with (--exe)";
        return r;
    }
    const Json doc = json::load(o.cases);
    if (!doc.contains("cases") || !doc.at("cases").is_array()) throw FormatError(fs::to_text(o.cases), 0, "no \"cases\" list");
    bool all = true;
    for (const Json& item : doc.at("cases")) {
        Case c;
        c.name = item.at("name").get<std::string>();
        const Clock::time_point t0 = Clock::now();
        std::vector<std::string> argv = {fs::to_text(*o.exe)};
        bool skipped = false;
        const std::string setting = item.value("package_setting", std::string());
        std::string package;
        if (!setting.empty()) {
            const std::optional<stdfs::path> p = s.setting_path ? s.setting_path(setting) : std::nullopt;
            if (!p || !stdfs::exists(*p, ec)) {
                c.status = "SKIPPED";
                c.why = "setting " + setting + " names no package on this machine";
                skipped = true;
            } else {
                package = fs::to_text(*p);
            }
        }
        if (!skipped) {
            for (const Json& a : item.at("argv")) argv.push_back(text::replace_all(a.get<std::string>(), "{package}", package));
            const auto [code, output] = s.run_exe(argv, o.case_timeout);
            const int want = item.value("expect_exit", 0);
            std::vector<std::string> lacks;
            if (item.contains("expect_contains"))
                for (const Json& part : item.at("expect_contains"))
                    if (output.find(part.get<std::string>()) == std::string::npos) lacks.push_back(part.get<std::string>());
            if (code != want) c.why = std::format("exit {} (wanted {})", code, want);
            else if (!lacks.empty()) c.why = "output lacks \"" + lacks.front() + "\"";
            c.status = c.why.empty() ? "PASS" : "FAIL";
        }
        c.took = since(t0);
        all = all && c.status != "FAIL";
        r.cases.push_back(std::move(c));
    }
    r.pass = all && !r.cases.empty();
    r.took = since(start);
    return r;
}

}  // namespace

Report run(Tier tier, const Services& s, const Options& o) {
    if (o.vms < 1 || o.vms > kMaxVms) throw UsageError(std::format("--vms is 1 to {}: no more than {} machines at once", kMaxVms, kMaxVms));
    if (o.runs < 1) throw UsageError("--runs is at least 1");
    if (tier == Tier::Slow) return run_slow(s, o);

    std::vector<std::string> own = all15_names();
    if (tier == Tier::Setup) own.insert(own.end(), setup_names().begin(), setup_names().end());
    for (const std::string& n : o.only)
        if (!in_list(own, n)) throw UsageError(std::format("--only {}: not a module of {}", n, word(tier)));

    Report r;
    r.tier = tier;
    const Clock::time_point start = Clock::now();
    if (const double free = s.free_gb ? s.free_gb() : 1e9; free < o.min_free_gb) {
        r.refused = std::format("{:.1f} GB free on the state drive, under the {:.0f} GB a run needs (the boots copy 650 MB each)", free, o.min_free_gb);
        return r;
    }
    if (s.missing) {
        const std::vector<std::string> lacks = s.missing(tier);
        if (!lacks.empty()) {
            r.refused = "missing: " + text::join(lacks, "; ");
            return r;
        }
    }
    if (!o.only.empty()) r.notes.push_back("--only: the other modules stay the originals; this is a look, not the regression");
    if (o.runs < 2 && o.only.empty()) r.notes.push_back("fewer than two runs: the regression's rule is two");

    // the run's own folder (built images); removed however the run ends
    stdfs::create_directories(s.work);
    const stdfs::path folder = s.work / std::format("{}-{}-{}", o.tag, word(tier), std::chrono::duration_cast<std::chrono::seconds>(Clock::now().time_since_epoch()).count());
    stdfs::create_directories(folder);
    struct Cleanup {
        stdfs::path folder;
        ~Cleanup() { base::runs::remove(folder); }
    } cleanup{folder};

    // build
    setup16::Replace replace;
    bool all_linked = true;
    const auto build_set = [&](const std::vector<std::string>& names, const stdfs::path& originals, bool all_assembly) {
        std::vector<std::string> wanted;
        for (const std::string& n : names)
            if (o.only.empty() || in_list(o.only, n)) wanted.push_back(n);
        if (wanted.empty()) return;
        for (Built& b : s.build(wanted, originals, folder, all_assembly)) {
            if (b.linked) replace[b.name] = b.image;
            else all_linked = false;
            r.built.push_back(std::move(b));
        }
    };
    build_set(all15_names(), s.mini_original, false);
    if (tier == Tier::Setup) build_set(setup_names(), s.setup_original, true);
    if (o.only.empty()) {
        for (const Expected& e : kStatusNumbers) {
            for (const Built& b : r.built) {
                if (!text::iequals(b.name, e.name)) continue;
                const std::size_t c = b.with_text >= b.assembled + b.rejected ? b.with_text - b.assembled - b.rejected : 0;
                r.notes.push_back(std::format("{}: C {} / gated {} (STATUS.md of the milestone: {} / {}){}", b.name, c, b.gated, e.c, e.gated,
                                              c == e.c && b.gated == e.gated ? "" : " -- differs; the boot verdict decides"));
            }
        }
    }
    if (o.keep_built) {
        std::error_code ec;
        stdfs::create_directories(*o.keep_built, ec);
        for (const auto& [name, image] : replace) stdfs::copy_file(image, *o.keep_built / (name), stdfs::copy_options::overwrite_existing, ec);
    }
    if (!all_linked) {
        r.notes.push_back("a module did not link: nothing was booted");
        r.took = since(start);
        return r;
    }

    // boot: `runs` boots, `vms` at once, each its own CD and disk copy
    std::vector<std::string> tags;
    for (std::size_t i = 1; i <= o.runs; ++i) tags.push_back(std::format("{}-{}-{}", o.tag, word(tier), i));
    for (std::size_t first = 0; first < tags.size(); first += o.vms) {
        std::vector<std::future<Boot>> batch;
        for (std::size_t i = first; i < std::min(tags.size(), first + o.vms); ++i) {
            batch.push_back(std::async(std::launch::async, [&s, &replace, tag = tags[i]]() {
                Boot b;
                b.tag = tag;
                const Clock::time_point t0 = Clock::now();
                try {
                    const setup16::Outcome out = s.boot(replace, tag);
                    b.verdict = out.verdict;
                    b.screenshot = out.screenshot;
                } catch (const std::exception& e) {
                    log::warn(std::format("regress: boot {} failed to run: {}", tag, e.what()));
                    b.verdict = setup16::Verdict::Slow;
                }
                b.took = since(t0);
                return b;
            }));
        }
        for (auto& f : batch) r.boots.push_back(f.get());
    }
    r.pass = std::all_of(r.boots.begin(), r.boots.end(), [](const Boot& b) { return b.verdict == setup16::Verdict::Pass; }) &&
             r.boots.size() == o.runs;
    r.took = since(start);
    return r;
}

// ---- the services over the settings --------------------------------------------------------------------------------------

namespace {

// Environment variables set for a scope and put back.
class ScopedEnv {
public:
    ScopedEnv(std::string name, const char* value) : name_(std::move(name)) {
        if (const std::optional<std::string> old = fs::get_env(name_)) old_ = *old;
        _putenv_s(name_.c_str(), value);
    }
    ~ScopedEnv() { _putenv_s(name_.c_str(), old_ ? old_->c_str() : ""); }
    ScopedEnv(const ScopedEnv&) = delete;
    ScopedEnv& operator=(const ScopedEnv&) = delete;

private:
    std::string name_;
    std::optional<std::string> old_;
};

}  // namespace

Services production_services(const config::Config& config) {
    Services s;
    const stdfs::path state = config.state();
    const stdfs::path qemu = config.path("qemu").value_or(stdfs::path("C:/Program Files/qemu/qemu-system-i386.exe"));
    const std::optional<stdfs::path> iso = config.path("win95_setup_iso");
    s.work = state / "work" / "regress";
    s.mini_original = state / "mini" / "original";
    s.setup_original = state / "setup" / "original";
    auto kit = std::make_shared<engine::LaneKit>(config);

    s.build = [kit](const std::vector<std::string>& names, const stdfs::path& originals, const stdfs::path& outdir, bool all_assembly) {
        // Setup's own programs were proved all assembly with no gate (CB_ASM_KEEP_C names no function, CB_NO_GATE): put back after
        std::unique_ptr<ScopedEnv> keep, nogate;
        if (all_assembly) {
            keep = std::make_unique<ScopedEnv>("CB_ASM_KEEP_C", "codebreaker_none");
            nogate = std::make_unique<ScopedEnv>("CB_NO_GATE", "1");
        }
        std::vector<std::string> paths;
        for (const std::string& n : names) paths.push_back(fs::to_text(originals / n));
        const build::module::ModuleBuilder builder(kit->module_services());
        std::vector<Built> out;
        const Clock::time_point t0 = Clock::now();
        try {
            const auto made = builder.build_many(
                paths, [&](const std::string& path) -> std::optional<stdfs::path> { return outdir / stdfs::path(path).filename(); }, false);
            for (std::size_t i = 0; i < names.size(); ++i) {
                Built b;
                b.name = names[i];
                for (const auto& [path, m] : made) {
                    if (path != paths[i]) continue;
                    b.linked = m.linked();
                    if (b.linked) b.image = *m.image;
                    b.error = m.error;
                    b.functions = m.functions;
                    b.with_text = m.with_text;
                    b.assembled = m.assembled.size();
                    b.rejected = m.rejected.size();
                    b.gated = m.gated.size();
                }
                if (b.image.empty() && b.error.empty()) b.error = "no result";
                out.push_back(std::move(b));
            }
        } catch (const std::exception& e) {
            out.clear();
            for (const std::string& n : names) {
                Built b;
                b.name = n;
                b.error = e.what();
                out.push_back(std::move(b));
            }
        }
        const std::chrono::seconds total = since(t0);
        for (Built& b : out) b.took = total;   // (built side by side: the whole batch's time)
        return out;
    };

    s.boot = [config, state, qemu, iso](const setup16::Replace& replace, const std::string& tag) {
        const setup16::Config machine = setup16::Config::in_state(state, qemu, iso.value_or(stdfs::path()));
        return setup16::continue_test(machine, replace, tag);
    };

    s.free_gb = [state]() {
        std::error_code ec;
        const stdfs::space_info info = stdfs::space(state, ec);
        return ec ? 0.0 : static_cast<double>(info.available) / (1024.0 * 1024.0 * 1024.0);
    };

    s.missing = [qemu, iso, state](Tier tier) {
        std::vector<std::string> lacks;
        std::error_code ec;
        const auto need = [&](const stdfs::path& p, const char* what) {
            if (p.empty() || !stdfs::exists(p, ec)) lacks.push_back(std::format("{} ({})", what, fs::to_text(p)));
        };
        if (tier == Tier::Slow) return lacks;
        need(qemu, "qemu (setting qemu)");
        need(iso.value_or(stdfs::path()), "the Windows 95 Setup CD (setting win95_setup_iso)");
        need(state / "mini" / "prepared_hd.img", "the prepared disk");
        need(state / "mini" / "MINI.CAB", "MINI.CAB");
        need(state / "bootfloppy" / "boot95_is.img", "the boot floppy");
        for (const std::string& n : all15_names()) need(state / "mini" / "original" / n, "an original of MINI.CAB");
        if (tier == Tier::Setup)
            for (const std::string& n : setup_names()) need(state / "setup" / "original" / n, "an original of Setup's programs");
        return lacks;
    };

    s.run_exe = [](const std::vector<std::string>& argv, std::chrono::minutes timeout) {
        const process::ProcessResult r =
            process::run({.argv = argv, .err = process::Capture::Merge, .timeout = std::chrono::duration_cast<std::chrono::milliseconds>(timeout)});
        return std::pair<int, std::string>{r.timed_out ? -1 : static_cast<int>(r.exit_code), text::sanitize_utf8(std::string(r.out.begin(), r.out.end()))};
    };
    s.setting_path = [config](const std::string& key) { return config.path(key); };
    return s;
}

cli::Command command(const config::Config& config) {
    cli::Command c{"regress",
                   "Logan's milestone regression: the all-15 Setup boot, the Setup chain, the slow cases",
                   {cli::Positional{"tier", "all15 | setup | slow"},
                    cli::Option{"--runs", "", "boots, each from a fresh CD and disk (default 2)", cli::Arg::Value, "N"},
                    cli::Option{"--vms", "", "boots at once, 1 to 3 (default 1)", cli::Arg::Value, "N"},
                    cli::Option{"--only", "", "rebuild only these modules, the rest stay the originals (a look, not the regression)", cli::Arg::Value, "NAME,..."},
                    cli::Option{"--keep-built", "", "keep the built images in this folder", cli::Arg::Value, "DIR"},
                    cli::Option{"--tag", "", "the name the boots' screenshots are filed under (default regress)", cli::Arg::Value, "TAG"},
                    cli::Option{"--cases", "", "slow: the cases file (default tests/regress/slow.json)", cli::Arg::Value, "FILE"},
                    cli::Option{"--exe", "", "slow: the codebreaker program the cases run", cli::Arg::Value, "FILE"}},
                   {},
                   {},
                   "Verify",
                   "all15 rebuilds the 15 modules of Setup's MINI.CAB from source, puts them on Setup's CD and boots Windows 95 Setup in QEMU to the License "
                   "page (PASS in every run). setup adds Setup's own seven 16-bit programs, rebuilt as assembly. slow runs the commands of the cases file. "
                   "At most 3 machines at once; only the machines this run started are stopped; the run's copies are deleted; nothing starts with under 30 GB "
                   "free. Exit 0 on PASS, 1 on FAIL or a run that could not start."};
    c.handler = [config](const cli::Args& args) {
        const std::string name = args.required("tier");
        const std::optional<Tier> tier = tier_named(name);
        if (!tier) throw UsageError("regress: '" + name + "' is not all15, setup or slow");
        Options o;
        if (const auto v = args.integer<std::size_t>("runs")) o.runs = *v;
        if (const auto v = args.integer<std::size_t>("vms")) o.vms = *v;
        for (const std::string& list : args.values("only"))
            for (const std::string_view part : text::split_trim(list, ',')) o.only.push_back(text::upper(part));
        if (const auto v = args.value("keep-built")) o.keep_built = fs::from_text(*v);
        if (const auto v = args.value("tag")) o.tag = *v;
        o.cases = args.value("cases") ? fs::from_text(*args.value("cases")) : config.checkout_root() / "tests" / "regress" / "slow.json";
        if (const auto v = args.value("exe")) o.exe = fs::from_text(*v);
        const Report r = run(*tier, production_services(config), o);
        cb::out("{}", r.text());
        return r.pass ? kExitOk : kExitFailed;
    };
    return c;
}

}  // namespace cb::verify::regress
