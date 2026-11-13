// codebreaker/ui/service.hpp -- COMPAT SHIM over cb/ui/service.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module (the jobs, what the store holds for a module, the service and its client, the link that says which process answers) is
// native/cb/ui/service.{hpp,cpp}, service_jobs.cpp and service_client.cpp: typed, `Service` and `ServiceLink` objects where the Python had a
// module's globals (START, LOCAL, OWNER, launcher()), `cb::Json` at the edge. The old py:: port (a Json value type of its own, `call()`, the
// process-wide state) is gone with its one consumer, the MCP server, which is native/cb/ui/mcp_server.
//
// Forwarding only: the names of the new module under the old namespace, for a not-yet-refactored port that names it. Deleted with the last one.
#pragma once

#include "cb/ui/service.hpp"

namespace codebreaker::ui::service {
using namespace ::cb::ui::service;
}  // namespace codebreaker::ui::service
