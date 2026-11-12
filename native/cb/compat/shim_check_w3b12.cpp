// Compile-only check of the shims at the old header paths for store/functions, symbols/names and symbols/debug_build_estimate (the old types they
// take and give: py::str paths, py::dicts, the old function store). Nothing here runs; it is in cb_shim_check so the shims stay honest until their
// last consumers are refactored.
#include "codebreaker/store/functions.hpp"
#include "codebreaker/symbols/debug_build_estimate.hpp"
#include "codebreaker/symbols/names.hpp"
#include "codebreaker/translate/frontends/python/lower.hpp"

namespace shim_check {

inline void touch_w3b12() {
    namespace names = codebreaker::symbols::names;
    (void)&names::key;
    (void)&names::bodies;
    (void)&names::codeview;
    (void)&names::read_sym;
    (void)&names::sym_objects;
    (void)&names::source_files;
    (void)&names::debug_names;
    (void)&names::own_names;
    (void)&names::powerpc_names;
    (void)&names::index;
    (void)&names::set_name;
    (void)&names::of_module;
    (void)&names::main;
    namespace functions = codebreaker::store::functions;
    namespace estimate = codebreaker::symbols::debug_build_estimate;
    (void)&functions::default_path;
    (void)&functions::databases_folder;
    (void)&functions::_ro;
    (void)&functions::FunctionStore;
    (void)&functions::open;
    (void)&functions::overview;
    (void)&functions::latest;
    (void)&functions::ORDER;
    (void)&functions::classify;
    (void)&estimate::estimate_module;
    (void)&estimate::confident_names;
    (void)&estimate::main;
}

}  // namespace shim_check
