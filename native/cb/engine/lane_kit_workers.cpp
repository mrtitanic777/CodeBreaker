// cb/engine/lane_kit_workers.cpp -- judging and building in worker processes (see lane_kit.hpp).
//
// Unicorn keeps state per process and cb::emulate serialises every call into it, so judging on threads never uses more than about one core. The
// reconstruction path therefore offers `Services::judge_batch`: here the names to judge are shared out over worker processes of the host program
// (jobs/workers.hpp), each of which makes its own kit, prepares and builds the module again in a folder of its own (the decompiler's and the
// compiler's results are kept by content, so that is quick) and judges its share. What travels in files is a line of tokens each way.
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/parallel.hpp"
#include "cb/engine/lane_kit_internal.hpp"
#include "cb/engine/machine_atari.hpp"
#include "cb/engine/machine_pce.hpp"
#include "cb/engine/machine_snes.hpp"
#include "cb/engine/machine_nds.hpp"
#include "cb/engine/machine_gekko.hpp"
#include "cb/engine/machine_n64.hpp"
#include "cb/engine/machine_ps2.hpp"
#include "cb/engine/machine_psp.hpp"
#include "cb/engine/machine_n3ds.hpp"
#include "cb/engine/machine_vita.hpp"
#include "cb/engine/machine_ps3.hpp"
#include "cb/engine/machine_switch.hpp"
#include "cb/engine/machine_m68k.hpp"
#include "cb/engine/machine_dreamcast.hpp"
#include "cb/engine/machine_threedo.hpp"
#include "cb/engine/machine_wonderswan.hpp"
#include "cb/engine/machine_cdi.hpp"
#include "cb/engine/machine_vectrex.hpp"
#include "cb/engine/machine_chf.hpp"
#include "cb/engine/machine_o2.hpp"
#include "cb/engine/machine_intv.hpp"
#include "cb/engine/machine_vb.hpp"
#include "cb/engine/machine_ngp.hpp"
#include "cb/engine/machine_sh2.hpp"
#include "cb/jobs/workers.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
using namespace kit_detail;

namespace {

// A share must be worth a process: starting one and making the module again costs a few seconds.
constexpr std::size_t kNamesPerShare = 4;

int kit_judge_share(const jobs::Args& args) {
    jobs::TokenReader in(fs::read_text(fs::from_text(args.at(0))));
    const std::string checkout = in.text(), lane_name = in.text();
    const stdfs::path module = fs::from_text(in.text());
    const stdfs::path work = fs::from_text(in.text()) / ("share" + (args.size() > 3 ? args.at(3) : std::string("0")));   // (a folder of each worker's own)
    std::vector<std::string> names;
    for (std::int64_t n = in.integer(); n > 0; --n) names.push_back(in.text());
    const auto t0 = std::chrono::steady_clock::now();
    const bool trace = fs::get_env("CB_LANE_KIT_TRACE").has_value();
    const auto since = [&] { return std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count(); };
    LaneKit kit(config::Config::load(fs::from_text(checkout)));
    const std::shared_ptr<const verify::lanes::Kind> lane = kit.lane_named(lane_name);
    if (!lane) throw UsageError("worker: no lane '" + lane_name + "' is set up here");
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane->prepare(module, work);
    const verify::lanes::Built built = lane->build(*prepared, module, work);
    const std::set<std::string_view> have(built.items.begin(), built.items.end());
    if (trace) std::fprintf(stderr, "judge share: %zu names, prepared and built after %.2f s\n", names.size(), since());
    // The workers of one batch share its names: each takes the next one nobody has (a folder made under `claims` is the claim: only one process
    // can make it), so a worker that is given quick functions takes more of them and all finish together.
    const stdfs::path claims = args.size() > 2 ? fs::from_text(args.at(2)) : stdfs::path();
    std::vector<std::string> mine;
    jobs::TokenWriter body;
    for (std::size_t i = 0; i < names.size(); ++i) {
        const std::string& name = names[i];
        if (!claims.empty()) {
            std::error_code ec;
            if (!stdfs::create_directory(claims / std::to_string(i), ec)) continue;   // (already taken, or the folder is gone: not this worker's)
        }
        mine.push_back(name);
        jobs::TokenWriter& out = body;
        verify::Judgement j = verify::not_run();
        if (have.contains(name)) {
            try {
                j = built.judge(name);
            } catch (const std::exception& e) {   // isolation boundary: one function of a share
                log::warn(std::format("worker: judging {} failed: {}", name, e.what()));
            }
        }
        if (trace) std::fprintf(stderr, "judge share: %s %s at %.2f s\n", name.c_str(), std::string(verify::word(j.verdict)).c_str(), since());
        out.text(name).text(verify::word(j.verdict)).text(j.reason).integer(static_cast<std::int64_t>(j.limits.size()));
        for (const std::string& limit : j.limits) out.text(limit);
        out.text(j.detail);
    }
    jobs::TokenWriter out;
    out.integer(static_cast<std::int64_t>(mine.size()));
    fs::write_text(fs::from_text(args.at(1)), out.str() + " " + body.str());
    if (trace) std::fprintf(stderr, "judge share: done after %.2f s\n", since());
    return 0;
}

// One build's answer, as lines of tokens: the report, the image, the folder, and what the builds found.
int kit_build_share(const jobs::Args& args) {
    jobs::TokenReader in(fs::read_text(fs::from_text(args.at(0))));
    const std::string checkout = in.text(), module = in.text(), outdir = in.text();
    const bool call = in.integer() != 0;
    LaneKit kit(config::Config::load(fs::from_text(checkout)));
    const build::module::ModuleBuilder builder(kit.module_services());
    build::module::BuildOptions options;
    if (!outdir.empty()) options.outdir = fs::from_text(outdir);
    options.call = call;
    options.log = [](std::string_view) {};
    const build::module::Built b = builder.build(module, options);
    jobs::TokenWriter out;
    out.text(b.report()).text(b.image ? fs::to_text(*b.image) : std::string()).text(b.folder ? fs::to_text(*b.folder) : std::string());
    const auto pairs = [&](const auto& map) {
        out.integer(static_cast<std::int64_t>(map.size()));
        for (const auto& [k, v] : map) out.text(k).text(v);
    };
    pairs(b.gated);
    pairs(b.rejected);
    out.integer(static_cast<std::int64_t>(b.placed_c.size()));
    for (const std::string& n : b.placed_c) out.text(n);
    out.integer(static_cast<std::int64_t>(b.moved_c.size()));
    for (const std::string& n : b.moved_c) out.text(n);
    fs::write_text(fs::from_text(args.at(1)), out.str());
    return 0;
}

}  // namespace

namespace {
std::atomic<bool> g_hosting{false};
}

int host_main_hook(int argc, char** argv) {
    jobs::WorkerKinds kinds;
    LaneKit::register_worker_kinds(kinds);
    if (const std::optional<int> code = jobs::run_as_worker(kinds, argc, argv)) return *code;
    g_hosting = true;
    return -1;
}

bool hosting_workers() { return g_hosting.load(); }

void LaneKit::register_worker_kinds(jobs::WorkerKinds& kinds) {
    register_m68k_lanes();    // (the Neo Geo and Sega CD lanes: the same table, the same reason)
    register_atari_lanes();   // (the Atari machines' lanes, in the table of engine/machine_lanes.hpp: the host and its workers make the same registrations)
    register_pce_lane();      // (the PC Engine lane, likewise)
    register_snes_lane();     // (the Super Nintendo lane, likewise)
    register_nds_lane();   // (the Nintendo DS lane, likewise)
    register_ps2_lane();   // (the PlayStation 2 and PSP lanes, likewise)
    register_psp_lane();
    register_n3ds_lane();   // (the Nintendo 3DS lane, likewise)
    register_vita_lane();   // (the PlayStation Vita lane, likewise)
    register_ps3_lane();    // (the PlayStation 3 lane, likewise)
    register_switch_lane(); // (the Nintendo Switch lane, likewise)
    register_n64_lane();   // (the Nintendo 64 lane, likewise)
    register_gekko_lanes();   // (the GameCube, Wii and Wii U lanes, likewise)
    register_sh2_lanes();   // (the Sega 32X and Saturn lanes, likewise)
    register_dreamcast_lane();   // (the Sega Dreamcast lane, likewise)
    register_threedo_lane();     // (the 3DO lane, likewise)
    register_wonderswan_lane();  // (the WonderSwan lane, likewise)
    register_cdi_lane();         // (the Philips CD-i lane, likewise)
    register_vectrex_lane();     // (the Vectrex lane, likewise)
    register_chf_lane();     // (and this one)
    register_o2_lane();     // (and this one)
    register_intv_lane();     // (and this one)
    register_vb_lane();     // (and this one)
    register_ngp_lane();     // (the Neo Geo Pocket lane, likewise)
    kinds.add("lane-judge", kit_judge_share);
    kinds.add("lane-build", kit_build_share);
}

void LaneKit::set_worker_launcher(jobs::WorkerLauncher launcher) { impl_->launcher = std::move(launcher); }

std::optional<std::map<std::string, verify::Judgement>> LaneKit::judge_apart(const verify::reconstruct::Services::JudgeBatch& batch) {
    if (!impl_->launcher || !impl_->launcher->enabled()) return std::nullopt;
    // Only the lanes that run Unicorn have a lock to get round; the real-mode and cartridge lanes compare images or run CodeBreaker's own interpreters
    // (cheap, and a worker would assemble the whole program again for nothing).
    const std::string_view kind = batch.kind.name();
    if (kind != "pe" && kind != "ne" && kind != "le" && kind != "powerpc") return std::nullopt;
    const std::size_t workers =static_cast<std::size_t>(parallel::workers(impl_->config.worker_policy()));
    const std::size_t shares = std::min(workers, batch.names.size() / kNamesPerShare);
    if (shares < 2) return std::nullopt;   // (too few to share out: the caller's threads judge)
    // One hand-over for the batch: the names, and the folder the workers claim them in (see kit_judge_share).
    jobs::Handover hand("judge");
    jobs::TokenWriter in;
    in.text(fs::to_text(impl_->config.checkout_root())).text(std::string(batch.kind.name())).text(fs::to_text(batch.module)).text(fs::to_text(batch.work))
        .integer(static_cast<std::int64_t>(batch.names.size()));
    for (const std::string& n : batch.names) in.text(n);
    hand.write("in.txt", in.str());
    const stdfs::path claims = hand.file("claims");
    fs::make_dirs(claims);
    std::vector<char> ok(shares, 0);
    std::vector<std::thread> running;
    for (std::size_t s = 0; s < shares; ++s) {
        running.emplace_back([&, s] {
            try {
                const jobs::WorkerResult r = impl_->launcher->run("lane-judge", {fs::to_text(hand.file("in.txt")), fs::to_text(hand.file("out" + std::to_string(s) + ".txt")),
                                                                                  fs::to_text(claims), std::to_string(s)});
                ok[s] = r.ok();
            } catch (const Error& e) {
                log::warn(std::format("reconstruct: a judging worker did not start: {}", e.what()));
            }
        });
    }
    // The parent judges too, from the build it already has (the workers are still making theirs), taking names from the same claims.
    std::map<std::string, verify::Judgement> out;
    for (std::size_t i = 0; i < batch.names.size(); ++i) {
        std::error_code ec;
        if (!stdfs::create_directory(claims / std::to_string(i), ec)) continue;
        try {
            out.emplace(batch.names[i], batch.built.judge(batch.names[i]));
        } catch (const std::exception& e) {   // isolation boundary: one function
            log::warn(std::format("reconstruct: judging {} failed: {}", batch.names[i], e.what()));
            out.emplace(batch.names[i], verify::not_run());
        }
    }
    for (std::thread& t : running) t.join();
    for (std::size_t s = 0; s < shares; ++s) {
        if (!ok[s]) continue;   // (the functions it took are given no answer: not run, as a dead worker's were)
        try {
            jobs::TokenReader answer(hand.read("out" + std::to_string(s) + ".txt"));
            for (std::int64_t n = answer.integer(); n > 0; --n) {
                const std::string name = answer.text();
                verify::Judgement j;
                j.verdict = verify::parse_verdict(answer.text()).value_or(verify::Verdict::Inconclusive);
                j.reason = answer.text();
                for (std::int64_t k = answer.integer(); k > 0; --k) j.limits.push_back(answer.text());
                j.detail = answer.text();
                out.emplace(name, std::move(j));
            }
        } catch (const Error& e) {
            log::warn(std::format("reconstruct: a judging worker's answer did not read: {}", e.what()));
        }
    }
    return out;
}

std::optional<build::module::Built> LaneKit::build_apart(const std::string& module, const std::optional<stdfs::path>& outdir, bool call) {
    if (!impl_->launcher || !impl_->launcher->enabled()) return std::nullopt;
    const std::optional<stdfs::path> found = impl_->corpus->retail(module);
    if (!found) return std::nullopt;
    jobs::Handover hand("build");
    jobs::TokenWriter in;
    in.text(fs::to_text(impl_->config.checkout_root())).text(module).text(outdir ? fs::to_text(*outdir) : std::string()).integer(call ? 1 : 0);
    hand.write("in.txt", in.str());
    const jobs::WorkerResult r = impl_->launcher->run("lane-build", {fs::to_text(hand.file("in.txt")), fs::to_text(hand.file("out.txt"))});
    std::string answer;
    try {
        answer = hand.read("out.txt");
    } catch (const Error&) {
    }
    if (!r.ok() || answer.empty()) throw ToolError("build worker", r.exit_code, "", std::format("{}: the build stopped", module));
    const std::optional<base::Lane> lane = impl_->corpus->lane(*found);
    build::module::Built b(*found, lane ? std::optional<std::string>(*lane == base::Lane::Ne ? "ne" : *lane == base::Lane::Pe ? "pe" : *lane == base::Lane::Le ? "le" : "dos") : std::nullopt);
    jobs::TokenReader t(answer);
    b.carried_report = t.text();
    if (const std::string image = t.text(); !image.empty()) b.image = fs::from_text(image);
    if (const std::string folder = t.text(); !folder.empty()) b.folder = fs::from_text(folder);
    for (std::int64_t n = t.integer(); n > 0; --n) {
        std::string k = t.text();
        b.gated.insert_or_assign(std::move(k), t.text());
    }
    for (std::int64_t n = t.integer(); n > 0; --n) {
        std::string k = t.text();
        b.rejected.insert_or_assign(std::move(k), t.text());
    }
    for (std::int64_t n = t.integer(); n > 0; --n) b.placed_c.insert(t.text());
    for (std::int64_t n = t.integer(); n > 0; --n) b.moved_c.insert(t.text());
    return b;
}

}  // namespace cb::engine
