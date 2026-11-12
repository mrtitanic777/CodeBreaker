// Compile-only check of the shim at the old header path of compare/versions: the old py:: surface (Side and Function, the Comparison with its
// py::list members, Product, main) over cb/compare/versions. The shim calls the old function store and corpus, which cannot be linked into
// cb_tests, so nothing here runs; it is in cb_shim_check so the shim stays honest until its last consumer is refactored. (The shim is
// header-only: including it compiles every definition in it.)
#include "codebreaker/compare/versions.hpp"

namespace shim_check_w3b1 {

inline void touch() {
    namespace v = codebreaker::compare::versions;
    (void)&v::load;
    (void)&v::compare;
    (void)&v::likeness;
    (void)&v::shared;
    (void)&v::product;
    (void)&v::main;
    (void)&v::set_capstone_dll;
    (void)&v::clock;
    (void)v::LEVELS;
    (void)v::ENGINE;
    (void)v::DOC;
    v::Product p;
    (void)v::to_value(p);
    (void)v::to_value(v::ModuleRow());
    std::shared_ptr<v::Comparison> none;
    if (none) {
        (void)none->counts();
        (void)none->code();
        (void)none->names();
        (void)none->reconstructions();
        (void)none->carry_names();
        (void)none->carry();
        (void)none->report();
        (void)none->as_json();
    }
}

}  // namespace shim_check_w3b1
