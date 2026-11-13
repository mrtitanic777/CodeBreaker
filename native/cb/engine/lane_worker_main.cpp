// A program of the lane kit's worker kinds, for the tests of judging in processes: it is the host the launcher starts (codebreaker.exe registers
// the same kinds at the top of its main).
#include "cb/engine/lane_kit.hpp"
#include "cb/jobs/workers.hpp"

int main(int argc, char** argv) {
    cb::jobs::WorkerKinds kinds;
    cb::engine::LaneKit::register_worker_kinds(kinds);
    if (const std::optional<int> rc = cb::jobs::run_as_worker(kinds, argc, argv)) return *rc;
    return 99;   // run directly (not as a worker): nothing to do
}
