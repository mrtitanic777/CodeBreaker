// cb/compat/old_lane_kit.hpp -- the one lane kit (engine/lane_kit) the shims at the old header paths share. The old ports that still need a lane, a
// gate, a module builder or a decompiled module ask this for the kit, which is made over the settings of the checkout the program runs from the
// first time it is asked and given the program's worker launcher (jobs/workers) from then on. COMPAT(py): deleted with the last shim that uses it.
#pragma once

#include <mutex>

#include "cb/base/config.hpp"
#include "cb/engine/lane_kit.hpp"
#include "codebreaker/jobs/workers.hpp"

namespace cb::compat {

inline cb::engine::LaneKit& old_lane_kit() {
    static cb::engine::LaneKit kit(cb::config::Config::load(cb::config::current().checkout_root()));
    static std::mutex lock;
    {
        std::lock_guard<std::mutex> hold(lock);
        if (codebreaker::jobs::workers::enabled()) kit.set_worker_launcher(codebreaker::jobs::workers::launcher());
    }
    return kit;
}

}  // namespace cb::compat
