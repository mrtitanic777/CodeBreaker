// codebreaker/base/converted.hpp -- typed C++ of codebreaker.base.converted
//
// converted.py -- where CodeBreaker puts what it makes of a binary: one project folder per binary.
//
//     Project p = converted::project(path);       // Converted/<name>/, made (and its original copied in) once
//     py::str out = os::path::join(p.src, "tetris.c");   // ... write the outputs into its folders ...
//     p.record("decompile", {out}, "...");        // project.json and README.txt say what was done
//
// Every binary decompiled, recompiled or otherwise taken apart gets a folder under `Converted/` at
// the top of the checkout (setting `converted`):
//
//     Converted/<name>/
//       README.txt     what the binary is, what was done to it (each command, when), how to build or
//                      run what came out
//       project.json   the same for tools: the input, its SHA-1, its kind, every output and the
//                      command that made it
//       original/      the binary as it came in (a copy)
//       src/           source: decompiled C, recompiled C++, assembly, build files
//       assets/        resources and data: extracted resources, a title's files
//       build/         programs and libraries built from src/
//       reports/       analysis, listings, disassembly, logs
//
// A project is the binary's, not the command's: it is found again by the SHA-1 of the original, so
// every command run on one file adds to one folder. Another file of the same name gets <name>_2.
// `Converted/` holds other people's code and data: it is never committed (.gitignore).
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * project.json's dict (Project.data(), what _write takes) is the struct ProjectData: its keys are
//     std::optional (absent keys are what `d.get(key)` answered None to).  The keys as the file has them are
//     kept in `order`, and a key this module does not read (or one whose value is not of the type below) in
//     `other`, so a rewrite keeps both as the Python's dict did; a key new to the dict goes behind the others.
//     A step of "steps" is the struct Step (the same, with `not_a_dict` for a step that is no object).
//     `size` is an int in every file this module wrote.  Two corners of a hand-damaged project.json are not
//     followed to the letter: a "steps" value that is no list is a TypeError whatever it is, and README.txt of a
//     non-scalar value (a list or object where a text belongs) is a NotImplementedError.
//   * json.dump(d, indent=1) is written by the module (the same text); project.json and README.txt are written
//     through the runtime's text file in UTF-8, README.txt with "\r\n" line ends, as the Python's open() calls say.
//   * `Project.name` and `Project.original_file` are properties in Python: the methods name() and original_file().
//   * datetime.datetime.now().isoformat(timespec="seconds") is `_clock()`; a test replaces it.
//   * Exceptions swallowed by the Python's `except Exception` (xex / detect asked about an arbitrary file) are
//     swallowed here (py::Exception and std::exception).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"

namespace codebreaker::base::converted {

extern const py::list<py::str> FOLDERS;
// file names that say nothing about the program (a console's default.xex, a setup.exe): the
// project is named after what the binary calls itself, or its folder, instead
extern const py::set<py::str> _GENERIC;

// The current time as datetime.datetime.now().isoformat(timespec="seconds") writes it (a test hook).
std::function<py::str()>& _clock();

// The Converted folder (created on demand).
py::str root();

py::str _sha1(const py::str& path);
py::str _clean(const py::str& name);

// A project name for a binary: its file name, unless that is a generic one (default.xex), then the name the
// image gives itself (an XEX's header), else its folder's.
py::str default_name(const py::str& path);
py::str _kind(const py::str& path);

using Json = knowledge::sources::Json;                            // a JSON value as json.load gives it (a key this module does not read)

// One step of project.json: a command, when it ran, what it made, notes.
struct Step {
    std::optional<py::str> command, when;
    std::optional<py::list<py::str>> outputs;
    std::optional<py::str> notes;
    py::list<py::str> order;                                      // the keys as the file has them
    py::dict<py::str, Json> other;                                // keys it does not read (or of another type), kept as they are
    std::optional<Json> not_a_dict;                               // a step of the file that is not an object (s.get raises)
};

// project.json's dict
struct ProjectData {
    std::optional<py::str> name, file, source, sha1, kind;
    std::optional<int64_t> size;
    std::optional<py::list<Step>> steps;
    py::list<py::str> order;                                      // the keys as the file has them (new ones go behind)
    py::dict<py::str, Json> other;                                // keys it does not read (or of another type), kept as they are
};

class Project {
  public:
    explicit Project(const py::str& folder);
    py::str root, original, src, assets, build, reports, manifest;

    // project.json's contents ({} -- no key set -- when it is missing or damaged)
    ProjectData data() const;
    py::str name() const;
    // The copy of the binary in original/.
    std::optional<py::str> original_file() const;
    // Note a step: the command, the files or folders it made, and anything a reader needs (how to build or
    // run them). Rewrites project.json and README.txt.
    void record(const py::str& command, const py::list<py::str>& outputs = py::list<py::str>(), const py::str& notes = py::str()) const;
    void _write(const ProjectData& d) const;
};

// The project folder of a binary: found again by its SHA-1, else made (its original copied in).
// `base`: another folder to keep it in (default: the Converted folder).
Project project(const py::str& path, const std::optional<py::str>& name = std::nullopt, const std::optional<py::str>& base = std::nullopt);

}  // namespace codebreaker::base::converted
