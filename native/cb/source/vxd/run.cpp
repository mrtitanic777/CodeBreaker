// cb/source/vxd/run.cpp -- the `vxd` command's dispatch (see run.hpp).
#include "cb/source/vxd/run.hpp"

namespace cb::source::vxd {

int run(std::span<const std::string> args, const base::commands::ToolTable& tools) {
    return base::commands::dispatch(tools, args, kVxdUsage);
}

}  // namespace cb::source::vxd
