// native_modules.hpp -- a module of the program replaced by its typed C++ (pyengine --emit-native-module,
// pyengine --dynamic --dyn-native-typed DIR).
//
// When every function of a module is proven (translated by the typed path, compiling, agreeing with the
// Python under the judge), the module stops being Python: its typed C++ is committed as source in DIR
// (DIR/cpp/<path>.hpp/.cpp, with DIR/<module>.json naming its functions and their parameters), its .py can go,
// and the dynamic build registers it as a native module of that name -- each function converts its arguments
// exactly (the islands' boundary, native/pyengine/islands), calls the typed function and boxes the result, or
// raises the exception the typed body raised, as Python's exception of that name. There is no dynamic body
// to fall back to: a value of another kind than the proof took is a TypeError.
#pragma once
#include <string>
#include <vector>

#include "islands.hpp"

namespace pyengine {

struct NativeModule {
    std::string module, ns, rel;        // codebreaker.compare.image, codebreaker::compare::image, codebreaker/compare/image
    std::vector<Island> functions;      // every function of the module, as an island (params, result)
};

// the module's typed C++ and manifest from a typed run (pyengine <package> --out RUN --cpp RUN/cpp), into
// out_dir: refused, with why, unless every function of the module is a provable island, the module has no
// other top-level names, and its C++ needs no other module of the program. The Python source is read for
// its top-level names (it must still be there when the module is emitted).
// judge: the judge's judge.json (its "agree" list): every function of the module must be in it.
bool emit_native_module(const std::string& root, const std::string& typed_dir, const std::string& module, const std::string& out_dir,
                        const std::string& judge, std::string& err);

// every manifest in dir
bool load_native_modules(const std::string& dir, std::vector<NativeModule>& out, std::string& err);

std::string native_exports(const std::vector<NativeModule>& mods);      // native_exports.hpp: both sides
std::string native_typed_glue(const std::vector<NativeModule>& mods);   // the typed side (py renamed pyt)
std::string native_dyn_glue(const std::vector<NativeModule>& mods);     // the dynamic side: the modules registered

}  // namespace pyengine
