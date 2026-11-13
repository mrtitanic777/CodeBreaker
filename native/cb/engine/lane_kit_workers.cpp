// cb/engine/lane_kit_workers.cpp -- judging and building in worker processes (see lane_kit.hpp).
//
// Unicorn keeps state per process and cb::emulate serialises every call into it, so judging on threads never uses more than about one core. The
// reconstruction path therefore offers `Services::judge_batch`: here the names to judge are shared out over worker processes of the host program
// (jobs/workers.hpp), each of which makes its own kit, prepares and builds the module again in a folder of its own (the decompiler's and the
// compiler's results are kept by content, so that is quick) and judges its share. What travels in files is a line of tokens each way.
#include <algorithm>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/parallel.hpp"
#include "cb/engine/lane_kit_internal.hpp"
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
    const stdfs::path module = fs::from_text(in.text()), work = fs::from_text(in.text());
    std::vector<std::string> names;
    for (std::int64_t n = in.integer(); n > 0; --n) names.push_back(in.text());
    LaneKit kit(config::Config::load(fs::from_text(checkout)));
    const std::shared_ptr<const verify::lanes::Kind> lane = kit.lane_named(lane_name);
    if (!lane) throw UsageError("worker: no lane '" + lane_name + "' is set up here");
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane->prepare(module, work);
    const verify::lanes::Built built = lane->build(*prepared, module, work);
    const std::set<std::string_view> have(built.items.begin(), built.items.end());
    jobs::TokenWriter out;
    out.integer(static_cast<std::int64_t>(names.size()));
    for (const std::string& name : names) {
        verify::Judgement j = verify::not_run();
        if (have.contains(name)) {
            try {
                j = built.judge(name);
            } catch (const std::exception& e) {   // isolation boundary: one function of a share
                log::warn(std::format("worker: judging {} failed: {}", name, e.what()));
            }
        }
        out.text(name).text(verify::word(j.verdict)).text(j.reason).integer(static_cast<std::int64_t>(j.limits.size()));
        for (const std::string& limit : j.limits) out.text(limit);
        out.text(j.detail);
    }
    fs::write_text(fs::from_text(args.at(1)), out.str());
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

void LaneKit::register_worker_kinds(jobs::WorkerKinds& kinds) {
    kinds.add("lane-judge", kit_judge_share);
    kinds.add("lane-build", kit_build_share);
}

void LaneKit::set_worker_launcher(jobs::WorkerLauncher launcher) { impl_->launcher = std::move(launcher); }

std::optional<std::map<std::string, verify::Judgement>> LaneKit::judge_apart(const verify::reconstruct::Services::JudgeBatch& batch) {
    if (!impl_->launcher || !impl_->launcher->enabled()) return std::nullopt;
    const std::size_t workers = static_cast<std::size_t>(parallel::workers(impl_->config.worker_policy()));
    const std::size_t shares = std::min(workers, batch.names.size() / kNamesPerShare);
    if (shares < 2) return std::nullopt;   // (too few to share out: the caller's threads judge)
    std::vector<std::vector<std::string>> parts(shares);
    for (std::size_t i = 0; i < batch.names.size(); ++i) parts[i % shares].push_back(batch.names[i]);   // (round robin: the heavy ones spread)
    struct Share {
        std::unique_ptr<jobs::Handover> hand;
        bool ok = false;
    };
    std::vector<Share> made(shares);
    std::vector<std::thread> running;
    for (std::size_t s = 0; s < shares; ++s) {
        made[s].hand = std::make_unique<jobs::Handover>("judge");
        jobs::TokenWriter in;
        in.text(fs::to_text(impl_->config.checkout_root())).text(std::string(batch.kind.name())).text(fs::to_text(batch.module))
            .text(fs::to_text(batch.work / ("share" + std::to_string(s)))).integer(static_cast<std::int64_t>(parts[s].size()));
        for (const std::string& n : parts[s]) in.text(n);
        made[s].hand->write("in.txt", in.str());
        running.emplace_back([&, s] {
            try {
                const jobs::WorkerResult r = impl_->launcher->run("lane-judge", {fs::to_text(made[s].hand->file("in.txt")), fs::to_text(made[s].hand->file("out.txt"))});
                made[s].ok = r.ok();
            } catch (const Error& e) {
                log::warn(std::format("reconstruct: a judging worker did not start: {}", e.what()));
            }
        });
    }
    for (std::thread& t : running) t.join();
    std::map<std::string, verify::Judgement> out;
    for (std::size_t s = 0; s < shares; ++s) {
        if (!made[s].ok) continue;   // (its functions are given no answer: not run, as a dead worker's were)
        try {
            jobs::TokenReader answer(made[s].hand->read("out.txt"));
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
