// exact/draft/early_c_model.hpp -- the early-C inverse model (cb/invert/early_c) as the draft stage's InverseModel.
//
// The draft stage reads a function through `InverseModel`; the model of the compilers of 1983-85 (Microsoft C 2.03 / 3.00,
// Lattice C 2.12) is `cb::invert::early_c`. This is the adapter: what the stage asks (a linear walk, a structured recovery
// with its switches, the text printed, the ladders folded, what the recovery read of the 8087's values and the register
// parameters) answered from the model's calls, each with the options it needs -- nothing is kept between two calls but what
// `last()` reports.
//
//     auto model = early_c_model();                    // as the profile's "inverse": "early_c" names it
//     draft::ModelTexts texts = draft::model_texts(*model, code, "f00102", 0x102);
//
// The same object is a candidates::MarkingModel: the model's checker mode, which prints the byte-wide places as BYTE(x).
#pragma once

#include <memory>
#include <string_view>

#include "exact/draft/candidates.hpp"
#include "exact/draft/package.hpp"

namespace cb::exact::draft {

class EarlyCModel : public InverseModel, public candidates::MarkingModel {};

// The model a toolchain profile's "inverse" names: "early_c". UsageError for a name that is no model of this build.
std::shared_ptr<EarlyCModel> early_c_model();
std::shared_ptr<InverseModel> inverse_model_named(std::string_view name);

}  // namespace cb::exact::draft
