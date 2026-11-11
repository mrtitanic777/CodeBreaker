// codebreaker/emulate/api_models.hpp -- the knowledge base's api_model, as emulate/emulator.py and
// emulate/windows16.py ask for it (knowledge.api_model(platform, api)).
//
// knowledge/__init__.py's api_model(platform, api) is the row of the authored kind (platform, "api_model") for an
// API: what the emulator's stand-in for a system function does ({ax, dx, pop} for win16, with "arg<N>" and
// "heap" as knowledge/models/win16.json says; {argc} for win32), without the keys that start with an underscore.
// knowledge/__init__.py itself has no C++ port; this reads the same authored file (knowledge/models/<platform>.json,
// its "functions" part) through knowledge/sources.hpp.
#pragma once

#include <optional>

#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"

namespace codebreaker::emulate::api_models {

using knowledge::sources::Json;

// knowledge.api_model(platform, api): a model's fields (underscore-led keys left out), or None when the API is
// not modelled.  `platform`: "win16", "win32" or "vxd".
std::optional<py::dict<py::str, Json>> api_model(const py::str& platform, const py::str& api);

}  // namespace codebreaker::emulate::api_models
