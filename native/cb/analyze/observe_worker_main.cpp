// The host of observe's worker processes, for the tests of running a module's functions apart: it is the program the launcher starts (codebreaker.exe
// registers the same kind at the top of its main, with its own way of preparing a Setting).
#include "cb/analyze/observe.hpp"
#include "cb/jobs/workers.hpp"

int main(int argc, char** argv) {
    cb::jobs::WorkerKinds kinds;
    cb::analyze::observe::register_worker_kinds(kinds, {});   // (no environment: the emulator's defaults, as the tests' Setting has)
    if (const std::optional<int> rc = cb::jobs::run_as_worker(kinds, argc, argv)) return *rc;
    return 99;   // run directly (not as a worker): nothing to do
}
