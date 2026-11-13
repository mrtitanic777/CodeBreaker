// cb/exact/draft/bind.hpp -- the draft stage's prover bound to a project: its compiler, its comparison, its rewrites.
//
// `prove` (draft/prove) is the search; what it searches with comes in through `Collaborators`, bound to one function. This
// binds them to an open project:
//
//     compile       the source through the project's toolchain, with its headers, include folders and switches
//     judge         the k-th function of the object held to the program (exact/compare), identical or how it differs
//     compile_many  several candidates through one run of the compiler (exact/batch)
//     diagnose      what differs from the program's function, by the compiler's listing (exact/aim)
//     refiners      the rewrites of draft/shapes, draft/forms and draft/candidates, in the order the beam tries them
//     candidates    the model's text made the project's sources (draft/candidates), with the toolchain profile's settings
//
// Set once on the services, it serves every project opened with them:
//
//     services->inverse_model = draft::inverse_model_named;
//     services->prover_for    = draft::bound_prover;
#pragma once

#include <memory>

#include "cb/exact/project.hpp"

namespace cb::exact::draft {

// The prover of `project`: for a function (its address and length, the texts the model gave, the names of the routines the
// program has), the first reading the compiler proves identical, else the nearest. What it keeps between functions (the
// compiler's options, the candidates' caches, the beam's history in <state>/exact/priors.json) is kept in the returned
// function, so it is made once for a project.
project::ProveTexts bound_prover(const std::shared_ptr<project::Project>& project);

}  // namespace cb::exact::draft
