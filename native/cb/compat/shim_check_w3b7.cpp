// Compile-only check of the shims at the old header paths for java/transpile, java/runtime and java/emit/source (the old types they take and
// give: py::ref classes and ledgers, the old back end's Bodies, the old registries). Nothing here runs; it is in cb_shim_check so the shims stay
// honest until their last consumers are refactored.
#include "codebreaker/java/emit/source.hpp"
#include "codebreaker/java/runtime.hpp"
#include "codebreaker/java/transpile.hpp"

namespace shim_check {

inline void touch_w3b7() {
    namespace transpile = codebreaker::java::transpile;
    namespace runtime = codebreaker::java::runtime;
    namespace source = codebreaker::java::emit::source;
    (void)&transpile::convert;
    (void)&transpile::build_resolver;
    (void)&transpile::discover;
    (void)&transpile::dump_trees;
    (void)&transpile::set_tree_dumper;
    (void)&transpile::use_profile;
    (void)&transpile::active_profile;
    (void)&transpile::profile_names;
    (void)&runtime::STUB_MEMBERS;
    (void)&runtime::FILES;
    (void)&runtime::generate_files;
    (void)&source::render_source;
}

}  // namespace shim_check
