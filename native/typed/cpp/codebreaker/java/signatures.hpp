// codebreaker/java/signatures.hpp -- SHIM over cb/java/signatures.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// Only what the `signatures` command calls is here: run(). The corpus's type resolver is still built by the old
// pipeline (java/transpile.hpp), so the shim parses with the new module, hands the old pipeline the classes it
// expects, and renders with the new resolver that the old one stands for.
#pragma once

#include <cstdio>
#include <string>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/signatures.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/transpile.hpp"
#include "codebreaker/java/types.hpp"
#include "py.hpp"

namespace codebreaker::java::signatures {

// Parse every .java under java_dir and write the codebase map to out_path; prints what was written, as the command does.
inline void run(const py::str& java_dir, const py::str& out_path) {
    cb::compat::guard([&] {
        namespace cbs = cb::java::signatures;
        const cbs::Codebase code = cbs::load(cb::fs::from_text(cb::compat::from_py(java_dir)));
        py::list<transpile::ClassRef> old_classes;
        for (const auto& c : code.classes) old_classes.append(ir::from_new(c));
        const py::ref<types::TypeResolver> resolver = transpile::build_resolver(old_classes);
        cbs::PackageRoles roles;
        for (const auto& kv : transpile::active_profile().package_roles.items()) {
            roles[cb::compat::from_py(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
        }
        const auto out = cb::fs::from_text(cb::compat::from_py(out_path));
        cb::fs::write_text(out, cbs::render(code, resolver->resolver(), roles));
        const std::string said = cbs::summary(code, out);
        std::fwrite(said.data(), 1, said.size(), stdout);
        return 0;
    });
}

}  // namespace codebreaker::java::signatures
