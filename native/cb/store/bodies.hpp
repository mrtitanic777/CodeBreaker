// cb/store/bodies.hpp -- the body store: the function bodies seen in any module, and the names given to them.
//
// The same function is linked into many modules (the C runtime, helper libraries, code shared between the 16- and 32-bit halves of a
// component). A body is recognised by a digest of its bytes with everything the linker fills in left out, so the digest is the same wherever
// the code sits. The knowledge base keeps one row per body per module; a name given to a body is kept once and read back for every module that
// has it. This is what a name is asked of -- symbols/names records a module's bodies and reads the shared names, compare/versions carries a name
// from one build to the next -- and it is an interface because the knowledge base's own tables are not part of this tree yet: whatever holds
// them (the old engine's knowledge base today, the program database's own tables when they move) implements it.
//
// A body is in a module at a function: `key` is the function store's text for the address ("1:04a2", "bfea1000").
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>

namespace cb::store {

// A function's body in one module: its digest, and the name the module itself gave it with where that name is from (an export, a symbol file).
struct Body {
    std::string digest;
    std::optional<std::string> name, source;
};

// The name the other modules that hold the body give it, and where it comes from ("named by hand", "the same body in kernel32.dll").
struct SharedName {
    std::string name, source;
};

// The module a set of bodies is of, as the store should know it.
struct ModuleRef {
    std::string sha, name, path, kind;   // the content hash, the file's name and path, the kind of file ("pe", "ne", ...; empty: not an executable)
};

// How many bodies the modules share.
struct BodyCounts {
    std::int64_t recorded = 0;           // rows: a body in a module
    std::int64_t distinct = 0;           // different bodies
    std::int64_t in_several = 0;         // bodies in two modules or more
    std::int64_t copies = 0;             // the functions that are such copies
};

class BodyStore {
public:
    virtual ~BodyStore() = default;

    // Replace what the store holds of the module's bodies (by function key); the module is noted by what `module` says of it.
    virtual void set_bodies(const ModuleRef& module, const std::map<std::string, Body>& by_key) = 0;
    // The module's bodies by function key; empty when none are recorded.
    virtual std::map<std::string, Body> bodies(std::string_view module) = 0;
    // Give a body a name: it then holds in every module with that body.
    virtual void name_body(std::string_view digest, std::string_view name, std::string_view source = "named by hand") = 0;
    // The names the module's bodies have from elsewhere, by function key.
    virtual std::map<std::string, SharedName> names_from_bodies(std::string_view module) = 0;
    virtual BodyCounts shared_bodies() = 0;
};

}  // namespace cb::store
