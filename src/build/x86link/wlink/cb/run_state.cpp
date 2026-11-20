// cb/run_state.cpp -- see run_state.hpp. One run at a time: the in-process host holds a lock (build/x86link/x86link.cpp).
#include "run_state.hpp"

namespace cb_wlink {

std::size_t RunState::next_slot() {
    static std::size_t count = 0;   // (the slots' numbering: fixed for the process, not a run's state)
    return count++;
}

std::vector<std::shared_ptr<void>>& RunState::slots() {
    static std::vector<std::shared_ptr<void>> all;
    return all;
}

void RunState::begin_run() {
    // (newest first: a part made from another's constructor goes before the one it used)
    std::vector<std::shared_ptr<void>>& all = slots();
    while (!all.empty()) all.pop_back();
}

}  // namespace cb_wlink
