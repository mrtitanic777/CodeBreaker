// cb/java/runtime.hpp -- the support files a conversion writes beside the translated classes.
//
// These stand in for the Java runtime types that are not part of the decompiled program (java.util.Random, the
// java.io streams, the java.awt and LWJGL names a desktop game reaches ...). A class that uses one #includes its header
// (typemap::runtime_headers()). Three kinds of file make the runtime:
//
//   * canonical headers -- hand-written, real implementations kept as files in the lane's platform/canonical folder
//     (byte codecs, the string and collection types, File, the regex engine ...), read verbatim;
//   * texts -- headers written out in this module (the GL11 stub, java.lang.Class, the Inflater ...);
//   * stubs -- symbol-only classes and namespaces made from a specification (`NsStub`, `ClassStub`): every method is a
//     no-op or returns a default, marked `J2X-STUB[...]` so one grep lists the whole placeholder surface.
//
// A `Runtime` is built from the lane's data folder and answers the same for the life of the process: the files, the
// member types of the stub classes (for the translator's `.` / `->` decisions), and `generate_files`, which applies a
// C++ standard and a target platform to the set.
//
//     Runtime runtime(JavaData::locate());
//     for (const auto& [name, text] : runtime.generate_files({.cpp_std = header::CppStd::of_year(2011), .platform = "xbox360"}))
//         fs::write_text(out_dir / name, text);
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/java/emit/header.hpp"
#include "cb/java/paths.hpp"

namespace cb::java::runtime {

// ---------------------------------------------------------------------------------------------- stub specifications

// `const int <name> = <value>;`
struct Constant {
    std::string name;
    std::int64_t value = 0;
};

// A function or method that answers a default: `<type> <name>(...) { return <value>; }`.
struct Function {
    std::string name;
    std::string type;
    std::string value;
};

// A namespace of static constants and functions (GL15, Keyboard), accessed `Name::X`.
struct NsStub {
    std::string name;
    std::vector<Constant> constants;
    std::vector<std::string> void_fns;      // no-ops
    std::vector<Function> ret_fns;          // return a default
    std::vector<std::string> includes;      // class names whose X.h is included
    std::vector<std::string> forward_decls;
    std::vector<std::string> prelude;       // lines written right after `#include <string>`
    std::vector<std::string> extra_lines;   // lines written verbatim at the end of the namespace
};

// A class with variadic stub methods (SoundSystem, DataInput), accessed `var->X`.
struct ClassStub {
    std::string name;
    std::vector<std::string> void_methods;   // virtual, no-ops
    std::vector<Function> ret_methods;       // virtual, return a default
    bool ctors = true;                       // a variadic constructor
    std::vector<std::string> bases;          // public bases (the first is the stub's parent for member lookup)
    std::vector<std::string> includes;
    std::vector<std::string> forward_decls;
    std::vector<std::string> extra;          // verbatim member lines (conversion operators, static factories)
    std::vector<std::string> prelude;        // lines written right after `#include <string>`
    std::vector<std::string> trailer;        // lines written after the class, before the closing #endif
};

// The header text of a stub, as it is emitted (with its banner naming the external library).
std::string ns_stub(const NsStub& spec);
std::string class_stub(const ClassStub& spec);
// The NIO buffers (FloatBuffer, IntBuffer, ByteBuffer): real classes whose methods chain, so `.clear().put()` compiles.
std::string buffer_stub(std::string_view name);

// The banner that marks a symbol-only stub of an external Java library type.
std::vector<std::string> external_banner(std::string_view name);

// ---------------------------------------------------------------------------------------------- the runtime

// The member types of a stub class, so an expression chain through it gets `.` or `->` right.
struct StubMembers {
    std::optional<std::string> parent;
    std::map<std::string, std::string, std::less<>> methods;   // method -> its C++ return spelling
};

struct GenerateOptions {
    std::optional<emit::header::CppStd> cpp_std;   // none: no standard-specific rewrites, C++03 in banners
    std::string platform;                          // a platform key (platforms.hpp); empty: no platform
};

class Runtime {
public:
    // Reads the canonical headers from `data`'s platform/canonical folder. NotFoundError if one the runtime needs is
    // missing; FormatError if one is not valid UTF-8.
    explicit Runtime(const JavaData& data);

    // Every runtime header by file name, before a standard or a platform is applied.
    const std::map<std::string, std::string, std::less<>>& files() const { return files_; }

    // The stub classes' member types by class name.
    const std::map<std::string, StubMembers, std::less<>>& stub_members() const { return members_; }

    // The canonical headers that are in the folder but are not part of the runtime: one left out would silently fall
    // back to its no-op stub. Empty means every one is wired.
    std::vector<std::string> unwired_canonicals() const;

    // The full file set for a conversion: the runtime headers with the standard's rewrites and the platform's banners
    // and timers applied, plus stdafx.h/.cpp and, where no platform layer supplies it, j2x_host_io.cpp. A fresh set on
    // every call, so one session can convert with different settings.
    std::map<std::string, std::string, std::less<>> generate_files(const GenerateOptions& options = {}) const;

private:
    void build_base();
    void build_stubs();             // runtime_data.cpp: the specifications, as the module's data
    void build_repeated_stubs();    // the stubs that come in families (listeners, events, layouts, exceptions)

    void put_file(std::string name, std::string_view text);
    void add_canonical(const std::string& name);
    void add_ns_stub(const std::string& file, const NsStub& spec);
    void add_class_stub(const std::string& file, const ClassStub& spec);

    std::filesystem::path canonical_dir_;
    std::map<std::string, std::string, std::less<>> files_;
    std::map<std::string, StubMembers, std::less<>> members_;
};

}  // namespace cb::java::runtime
