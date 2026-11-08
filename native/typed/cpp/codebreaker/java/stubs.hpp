// codebreaker/java/stubs.hpp -- C++17 port of codebreaker.java.stubs.
//
// stubs.py -- classify converter stubs by target subsystem.
//
// Every stub header the converter emits needs a REAL implementation backed by
// platform-specific APIs. This module inspects the stub headers, classifies them
// by subsystem (rendering, audio, input, io, networking, ...), and annotates each
// with what SDK/platform feature the stub needs.
//
// When an SDK directory is provided, the classifier cross-references the stub's
// requirements against the SDK headers to suggest which SDK APIs map to which
// stub methods.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::java::stubs {

// One subsystem's record (the Python dict with the keys label, description, sdk_hint, members).
struct Subsystem {
    py::str label;
    py::str description;
    py::str sdk_hint;
    py::set<py::str> members;
};

// ---------------------------------------------------------------------------
// Subsystem definitions: (subsystem_id, label, description, sdk_hint, members)
// ---------------------------------------------------------------------------
// subsystem id -> its record, in definition order (an OrderedDict)
extern const py::dict<py::str, Subsystem> SUBSYSTEMS;

// Return (subsystem_id, subsystem_info) for a stub class name, or ('unknown', None).
std::tuple<py::str, std::optional<Subsystem>> classify_stub(const py::str& class_name);

}  // namespace codebreaker::java::stubs
