// cb/app/host_hook.cpp -- what codebreaker.exe does before the translated program runs (pyengine --dyn-main-hook): started as one of
// the lane kit's or analyze's worker processes, it does that share of the work and exits; otherwise it is the host, and the native
// commands share judging, building and observing out over processes of it.
#include <optional>

#include "cb/base/config.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/jobs/workers.hpp"
#include "cb/store/analyze_command.hpp"

extern "C" int cb_host_main_hook(int argc, char** argv) {
    cb::jobs::WorkerKinds kinds;
    cb::engine::LaneKit::register_worker_kinds(kinds);
    cb::store::analyze_command::register_worker_kinds(kinds, cb::store::analyze_command::Environment{cb::config::current(), nullptr});
    if (const std::optional<int> code = cb::jobs::run_as_worker(kinds, argc, argv)) return *code;   // a worker: its share, then exit
    return cb::engine::host_main_hook(argc, argv);   // not a worker (-1): marks this process the host of the kit's workers
}
