// A tiny program for the worker tests: it is the "host" the launcher starts, with a few kinds of share.
#include <chrono>
#include <stdexcept>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/jobs/workers.hpp"

#include <windows.h>

using namespace cb;

int main(int argc, char** argv) {
    jobs::WorkerKinds kinds;
    // exit with the number it is given
    kinds.add("exit", [](const jobs::Args& a) { return std::stoi(a.at(0)); });
    // read the hand-over file `in`, write the integer and the text back doubled to `out`
    kinds.add("double", [](const jobs::Args& a) {
        jobs::TokenReader in(fs::read_text(a.at(0)));
        const std::int64_t n = in.integer();
        const std::string s = in.text();
        jobs::TokenWriter out;
        out.integer(n * 2).text(s + s);
        fs::write_text(a.at(1), out.str());
        return 0;
    });
    // a share that throws
    kinds.add("throw", [](const jobs::Args&) -> int { throw std::runtime_error("the share failed"); });
    // a worker the guest crashed: the process dies with an access-violation code
    kinds.add("crash", [](const jobs::Args&) -> int {
        TerminateProcess(GetCurrentProcess(), 0xC0000005u);
        return 0;
    });
    // a share that never ends
    kinds.add("sleep", [](const jobs::Args&) {
        std::this_thread::sleep_for(std::chrono::minutes(5));
        return 0;
    });
    if (const std::optional<int> rc = jobs::run_as_worker(kinds, argc, argv)) return *rc;
    return 99;   // run directly (not as a worker): nothing to do
}
