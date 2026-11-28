// build/l2cc/l2cc.hpp -- L2 compiled by x86cc: a C unit's functions, held as translate::ir trees, compiled with their bodies
// lowered through the front end's own semantic actions rather than written as C and read back (docs/L2_COMPILER.md, seam 1).
//
//     l2cc::Unit unit{.header = "", .functions = {add}};               // `add`: an ir MethodDecl
//     l2cc::Result r = l2cc::compile(x86cc::Target::i386, unit, "add.c", {"-bt=nt", "-zq"}, folder);
//     if (!r.declined.empty()) ...   // a construct the lowering has not got yet: nothing was compiled
//     if (!r.failed.empty()) ...     // the front end refused a call: the object is not to be used
//
// The unit's text is its header and, for each function, the head of its definition with an empty body (`int add(int a, int b)
// {}`): the declarations go through the front end's declaration parser as the text path's do, and each body comes from the
// tree. A function whose tree matches the text path's C gives the same object as it.
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "build/x86cc/x86cc.hpp"
#include "translate/ir.hpp"

namespace cb::build::l2cc {

struct Unit {
    std::string header;                                // the unit's declarations, as C text (an L2 Module's `header`)
    std::vector<translate::ir::Node*> functions;       // MethodDecls
};

struct Result {
    std::vector<std::string> declined;   // "<function>: <what>", one for each function the lowering cannot take yet
    std::string failed;                  // "<function>: <why>": a call the front end refused
    std::string text;                    // the unit's text, as compiled
    x86cc::Run run;                      // the compile (when nothing was declined)
};

// What of a function the lowering cannot take yet (the first construct, in words), or empty when it can take all of it.
std::string declines(const translate::ir::Node* function, x86cc::Target target);

// The head of a function's definition as C (`int add(int a, int b)`), from its L2 signature; empty when a type has no C
// spelling here yet.
std::string head(const translate::ir::Node* function, x86cc::Target target);

// The unit compiled in `folder` from a file named `file` (written there), with x86cc's `args` (the file's name is added).
Result compile(x86cc::Target target, const Unit& unit, const std::string& file, const std::vector<std::string>& args,
               const std::filesystem::path& folder, const std::map<std::string, std::string>& environment = {});

}  // namespace cb::build::l2cc
