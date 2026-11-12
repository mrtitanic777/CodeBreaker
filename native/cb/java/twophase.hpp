// cb/java/twophase.hpp -- two-phase construction for classes whose constructor makes a virtual call.
//
// Java dispatches a virtual call made inside a constructor to the most-derived override, even though the subclass's
// own constructor has not run yet. C++ does not: while a base constructor runs, the object IS a base, so the call
// reaches the base's version. Minecraft relies on the Java rule in a few hierarchies -- Block's constructor reads
// isOpaqueCube() into opaqueCubeLookup (every transparent block came out opaque: wrong light and culling), Entity's
// calls entityInit(), World's createChunkProvider() -- so a plain C++ constructor gets them wrong.
//
// For those classes (the set `compute` finds) every Java constructor becomes a method `C* C::j2xInit(params)`; the
// C++ constructor `C::C()` only zeroes the fields. j2xInit calls the parent's j2xInit first (Java's super(...)),
// then runs the field initialisers and the body, and returns `this`. It runs on a fully built C++ object, so its
// virtual calls reach the override as in Java. Creating an object is then `(new C())->j2xInit(args)` --
// `rewrite_news` turns every `new C(args)` in the emitted tree (and in the probe's harness) into that.
#pragma once

#include <cstddef>
#include <filesystem>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/java/ir.hpp"

namespace cb::java::twophase {

using NameSet = std::set<std::string, std::less<>>;

// The classes to build in two phases: each class whose constructor (or instance initialiser) calls one of its own
// instance methods that a subclass overrides (or that is abstract), and every descendant of such a class. Left out
// (with the subtree below it): a value type (one of `values`), and a root whose parent is another user class that it
// constructs with arguments (the parent's constructor would have to run in `C()`).
NameSet compute(std::span<const ir::JClass> classes, const NameSet& values = {});

// A text rewritten, and how many places were.
struct Rewritten {
    std::string text;
    std::size_t count = 0;
};

// `new C(args)` -> `(new C())->j2xInit(args)` for each C in names; nested constructions in args are rewritten too.
// Leaves alone a `new C()` already in that form and `new C[n]` (an array).
Rewritten rewrite_news(std::string_view text, const NameSet& names);

// A stack object `C tmp(args);` / `C tmp;` (the leak-temporary rewrite makes these, named _j2xleakN) ->
// `C tmp; tmp.j2xInit(args);`.
Rewritten rewrite_stack(std::string_view text, const NameSet& names);

struct RunResult {
    std::size_t sites = 0;               // construction sites rewritten
    std::vector<std::string> files;      // the files that changed, by name
};

// The post-pass over the emitted tree: every `new C(...)` of a two-phase class in the .cpp/.h files of `out_dir`.
// Files are rewritten byte for byte (their line ends are kept); only files with a site are written.
RunResult run(const std::filesystem::path& out_dir, const NameSet& names);

}  // namespace cb::java::twophase
