// codebreaker/decompile/jar.hpp -- typed C++ from codebreaker.decompile.jar.
//
// RetroMCP decompiler backend.
//
// CodeBreaker drives the RetroMCP CLI (native/retromcp: MIT, MCPHackers; built from that source,
// settings.json `retromcp` names another jar) as a headless backend so a
// Minecraft version can be decompiled straight into Java source and fed into the transpiler -- no manual
// round-trip through the RetroMCP GUI. The decompile step needs a Java runtime and network access
// (RetroMCP downloads the version + mappings on first setup). The C++ transpiler itself needs neither.
//
// Pipeline, run inside a workspace directory:
//     java -jar retromcp-cli.jar setup setup=<version>     # download + prepare the workspace
//     java -jar retromcp-cli.jar decompile                 # decompile -> <workspace>/minecraft_*/src
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::jar {

// (the callback the Python takes as on_output: called with each progress line)
using OnOutput = std::function<void(const py::str&)>;

struct DecompileError : py::Exception {
    DecompileError() : py::Exception() {}
    explicit DecompileError(const std::string& m) : py::Exception(m) {}
    explicit DecompileError(const char* m) : py::Exception(m) {}
    explicit DecompileError(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "DecompileError"; }
};

// The RetroMCP CLI jar (settings.json `retromcp`); "" when it is not set.
py::str backend_jar();

// Locate a `java` executable. Order: explicit path, settings.json `java`, $JAVA_HOME/bin/java, PATH.
// Returns path or None.
std::optional<py::str> find_java(const std::optional<py::str>& explicit_ = std::nullopt);

// True if both a Java runtime and the bundled backend jar are present.
bool available();

// Locate the decompiled source dir: the `src` directory holding the most `.java` files.
std::optional<py::str> _find_src(const py::str& workspace);

// Runs `java -jar jar args` in cwd, passing each output line (stdout and stderr together) to on_output and
// to the workspace's codebreaker-decompile.log. -> (exit code, the lines).
std::tuple<int64_t, py::list<py::str>> _run(const py::str& java, const py::str& jar, const py::list<py::str>& args,
                                            const py::str& cwd, const std::optional<OnOutput>& on_output = std::nullopt,
                                            const std::optional<py::str>& stdin_text = std::nullopt);

// Decompile a Minecraft ``version`` (e.g. ``"a1.2.6"``, ``"c0.30_01c"``) into ``workspace``.
//
// Returns the absolute path to the decompiled Java source directory, suitable as the ``java_dir``
// input to :func:`codebreaker.java.transpile.convert`. Raises :class:`DecompileError` on any failure.
//
// Set ``setup=False`` to skip the download/setup step (re-decompile an already-set-up workspace).
py::str decompile(const py::str& version, const py::str& workspace, const std::optional<py::str>& java = std::nullopt,
                  const std::optional<OnOutput>& on_output = std::nullopt, bool setup = true);

// RetroMCP names an array parameter from its JVM descriptor in some versions (Indev 20100213:
// `Block[] block;4`, `byte[] [B4`), which is not Java, while Fernflower's body still calls it `var4`
// (`var4/* $VF was: block;4 */`). The parameter takes the name the body uses.
extern const py::re::Pattern _MANGLED_PARAM;
extern const py::re::Pattern _VF_WAS;

// Rename mangled array parameters in every .java under `src` to the `varN` their bodies use.
// -> how many parameters were renamed.
int64_t repair_array_params(const py::str& src);

}  // namespace codebreaker::decompile::jar
