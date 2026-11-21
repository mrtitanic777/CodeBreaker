// cb/run_state.hpp -- the state a file of wlink keeps for one run of the linker, made new at each run's start (no mutable
// global or static: docs/X86LINK_DESIGN.md phase 4b). A file keeps what it carried in statics in a struct of its own and
// reaches it as `cb_wlink::RunState::part<FileState>()`: value-initialized at its first use in a run (zero, as a
// static was in a fresh process, or its members' initializers), destroyed when the next run begins. A function that
// uses it many times takes the reference once.
#pragma once

#include <cstddef>
#include <memory>
#include <vector>

namespace cb_wlink {

class RunState {
public:
    template <class T>
    static T& part() {
        static const std::size_t slot = next_slot();
        std::vector<std::shared_ptr<void>>& all = slots();
        if (slot >= all.size()) all.resize(slot + 1);
        std::shared_ptr<void>& p = all[slot];
        if (!p) p = std::shared_ptr<void>(new T{}, [](void* x) { delete static_cast<T*>(x); });
        return *static_cast<T*>(p.get());
    }
    // A run starts (wlink.cpp: InitSubSystems): what an earlier run kept is gone.
    static Link& begin_run();

private:
    static std::size_t next_slot();
    static std::vector<std::shared_ptr<void>>& slots();
};

}  // namespace cb_wlink
