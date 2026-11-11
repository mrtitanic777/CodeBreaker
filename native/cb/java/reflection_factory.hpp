// cb/java/reflection_factory.hpp -- turn Java reflection registries into real C++ factories.
//
// Java code registers a class table and then instantiates by id via reflection:
//
//     addMapping(EntityPig.class, "Pig", 90);                        // register
//     (Entity) cls.getConstructor(World.class).newInstance(world);   // reflect-construct
//
// C++ has no `newInstance`, so the converter stubs it to NULL (the classic TileEntity-load / Packet-parse /
// entity-spawn crash). But the registration table lists EVERY concrete class, so the factory is fully recoverable:
// this pass reads the `addMapping(j2xClassOf(typeid(X)), ...)` / `addIdClassMapping(..., j2xClassOf(typeid(X)))`
// table out of the emitted .cpp, generates a `<Registry>_createByReflection(Class* c[, Arg* a0 ...])` that switches
// the Class* to a real `new X(args)`, and rewrites the reflection call site to invoke it. NULL stub -> working
// factory.
//
// Runs as a post-emit pass over the converted tree (also `codebreaker java reflect-factories <dir>`), and is wired
// into the conversion so every port gets it automatically.
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::reflection_factory {

// The calls that map ids/names to classes, whose lines carry the concrete class table. With no names, any call to
// a function whose name starts with `register`. Names are plain identifiers, matched whole.
struct Registrars {
    std::vector<std::string> names;
    bool matches_line(std::string_view line) const;
};

// A factory generated for one registry file.
struct Factory {
    std::string registry;               // the registry's class (its file's stem)
    std::string ret;                    // the base type the factory returns
    std::vector<std::string> args;      // constructor argument types (each passed as a pointer)
    std::vector<std::string> classes;   // the concrete classes it switches over, in table order
};

// A factory as other files see it: enough to declare it and to rewrite a call.
struct FactoryRef {
    std::string name;   // `<Registry>_createByReflection`
    std::string ret;
    std::vector<std::string> args;
};

struct ProcessResult {
    std::string text;
    std::size_t sites = 0;              // reflection call sites rewritten
    std::optional<Factory> factory;     // set when a factory was generated
};

// Rewrite the reflection sites in one .cpp (`registry` is its class name) to a generated factory, inserted after
// the include block.
ProcessResult process(std::string_view text, std::string_view registry, const Registrars& registrars = {});

// Rewrite the reflection sites of a NON-registry file to use the shared factories. Returns the new text and how
// many sites it rewrote.
std::pair<std::string, std::size_t> reuse(std::string_view text, const std::vector<FactoryRef>& factories);

// ReflectionFactory.h: the declarations of every factory.
std::string header_text(const std::vector<FactoryRef>& factories);

struct RegistryResult {
    std::string file;
    std::size_t sites = 0;
    Factory factory;
};
struct ReusedResult {
    std::string file;
    std::size_t sites = 0;
};
struct RunResult {
    std::vector<RegistryResult> registries;
    std::vector<ReusedResult> reused;
    std::size_t total_sites = 0;
    std::size_t total_classes = 0;
};

// Two passes over the .cpp files directly in out_dir: (1) generate a factory for each registry class from its
// registration table; (2) rewrite reflection sites in OTHER files to reuse those factories via ReflectionFactory.h
// (written when any factory exists). Files are rewritten in place. NotFoundError if out_dir is not a folder.
RunResult run(const std::filesystem::path& out_dir, const Registrars& registrars = {});

}  // namespace cb::java::reflection_factory
