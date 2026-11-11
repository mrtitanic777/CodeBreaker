// codebreaker/decompile/jar.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/jar.{hpp,cpp}; this header keeps the old py:: surface for java/run and verify/lanes/java
// (settings.json is read here, where the old layer holds it, and handed to the module). Conversion and forwarding
// only; deleted with its last py:: consumer.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/decompile/jar.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::decompile::jar {

namespace nj = ::cb::decompile::jar;

// (the callback the Python takes as on_output: called with each progress line)
using OnOutput = std::function<void(const py::str&)>;

struct DecompileError : py::Exception {
    DecompileError() : py::Exception() {}
    explicit DecompileError(const std::string& m) : py::Exception(m) {}
    explicit DecompileError(const char* m) : py::Exception(m) {}
    explicit DecompileError(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "DecompileError"; }
};

namespace detail {
inline std::string native_text(const std::filesystem::path& p) {
    std::string s = ::cb::fs::to_text(p);
    for (char& c : s) {
        if (c == '/') c = '\\';
    }
    return s;
}
inline py::str to_str(const std::filesystem::path& p) { return ::cb::compat::to_py(native_text(p)); }
inline std::filesystem::path to_path(const py::str& s) { return ::cb::fs::from_text(s.utf8()); }
inline nj::Settings settings() {
    return {to_path(py::settings::get_str("retromcp")), to_path(py::settings::get_str("java"))};
}
inline nj::OutputFn output(const std::optional<OnOutput>& on_output) {
    if (!on_output) return {};
    return [on_output](std::string_view line) { (*on_output)(::cb::compat::to_py(line)); };
}
template <class F>
decltype(auto) guarded(F&& f) {
    try {
        return std::forward<F>(f)();
    } catch (const nj::DecompileError& e) {
        throw DecompileError(std::string(e.what()));
    } catch (const ::cb::Error& e) {
        ::cb::compat::rethrow_as_py(e);
    }
}
}  // namespace detail

// The RetroMCP CLI jar (settings.json `retromcp`); "" when it is not set.
inline py::str backend_jar() { return py::settings::get_str("retromcp"); }

// Locate a `java` executable. Order: explicit path, settings.json `java`, $JAVA_HOME/bin/java, PATH.
inline std::optional<py::str> find_java(const std::optional<py::str>& explicit_ = std::nullopt) {
    std::optional<std::filesystem::path> wanted;
    if (explicit_) wanted = detail::to_path(*explicit_);
    auto found = nj::find_java(detail::settings(), wanted);
    if (!found) return std::nullopt;
    return detail::to_str(*found);
}

// True if both a Java runtime and the bundled backend jar are present.
inline bool available() { return nj::available(detail::settings()); }

// Locate the decompiled source dir: the `src` directory holding the most `.java` files.
inline std::optional<py::str> _find_src(const py::str& workspace) {
    auto found = nj::find_src(detail::to_path(workspace));
    if (!found) return std::nullopt;
    return detail::to_str(*found);
}

// Runs `java -jar jar args` in cwd, passing each output line to on_output and to the workspace's
// codebreaker-decompile.log. -> (exit code, the lines).
inline std::tuple<int64_t, py::list<py::str>> _run(const py::str& java, const py::str& jar, const py::list<py::str>& args,
                                                   const py::str& cwd, const std::optional<OnOutput>& on_output = std::nullopt,
                                                   const std::optional<py::str>& stdin_text = std::nullopt) {
    std::optional<std::string> input;
    if (stdin_text) input = stdin_text->utf8();
    nj::RunResult r = detail::guarded([&] {
        return nj::run_java(detail::to_path(java), detail::to_path(jar), ::cb::compat::from_py(args), detail::to_path(cwd),
                            detail::output(on_output), input);
    });
    return {static_cast<int64_t>(r.exit_code), ::cb::compat::to_py(r.lines)};
}

// Decompile a Minecraft ``version`` into ``workspace``; the absolute path of the decompiled Java source directory.
// Raises DecompileError on any failure.
inline py::str decompile(const py::str& version, const py::str& workspace, const std::optional<py::str>& java = std::nullopt,
                         const std::optional<OnOutput>& on_output = std::nullopt, bool setup = true) {
    nj::DecompileOptions options;
    options.settings = detail::settings();
    if (java) options.java = detail::to_path(*java);
    options.on_output = detail::output(on_output);
    options.setup = setup;
    return detail::guarded([&] { return detail::to_str(nj::decompile(version.utf8(), detail::to_path(workspace), options)); });
}

// Rename mangled array parameters in every .java under `src` to the `varN` their bodies use.
// -> how many parameters were renamed.
inline int64_t repair_array_params(const py::str& src) {
    return detail::guarded([&] { return static_cast<int64_t>(nj::repair_array_params(detail::to_path(src))); });
}

}  // namespace codebreaker::decompile::jar
