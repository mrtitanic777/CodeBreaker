// cb/run_state.cpp -- see run_state.hpp. One run at a time: the in-process host holds a lock (build/x86link/x86link.cpp).
#include "build/x86link/link/run_state.hpp"

#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"

namespace cb_wlink {

Link* current_link = nullptr;

std::size_t RunState::next_slot() {
    static std::size_t count = 0;   // (the slots' numbering: fixed for the process, not a run's state)
    return count++;
}

std::vector<std::shared_ptr<void>>& RunState::slots() {
    static std::vector<std::shared_ptr<void>> all;
    return all;
}

Link& RunState::begin_run() {
    // (newest first: a part made from another's constructor goes before the one it used)
    std::vector<std::shared_ptr<void>>& all = slots();
    while (!all.empty()) all.pop_back();
    current_link = &part<Link>();   // the link's state, new: what were wlink's globals, as a fresh process had them
    return *current_link;
}

}  // namespace cb_wlink
