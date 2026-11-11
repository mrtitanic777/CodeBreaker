// cb/base/converted.hpp -- where CodeBreaker puts what it makes of a binary: one project folder per binary.
//
//     converted::Store store(converted_root);                       // the Converted folder (setting "converted")
//     converted::Project p = store.project(binary);                  // Converted/<name>/, made (original copied) once
//     fs::write_text(p.src() / "tetris.c", c);                       // ... write the outputs into its folders ...
//     p.record("decompile", {p.src() / "tetris.c"}, "how to build"); // project.json and README.txt say what was done
//
// Every binary decompiled, recompiled or otherwise taken apart gets a folder under `Converted/` at the top of the
// checkout:
//
//     Converted/<name>/
//       README.txt     what the binary is, what was done to it (each command, when), how to build or run what came out
//       project.json   the same for tools: the input, its SHA-1, its kind, every output and the command that made it
//       original/      the binary as it came in (a copy)
//       src/           source: decompiled C, recompiled C++, assembly, build files
//       assets/        resources and data: extracted resources, a title's files
//       build/         programs and libraries built from src/
//       reports/       analysis, listings, disassembly, logs
//
// A project is the binary's, not the command's: it is found again by the SHA-1 of the original, so every command run on
// one file adds to one folder. Another file of the same name gets <name>_2. `Converted/` holds other people's code and
// data: it is never committed (.gitignore).
//
// project.json keeps the layout the Python wrote (the old code reads the same files until it is retired): keys name, file,
// source, sha1, size, kind, steps; indent 1; ASCII escapes. A key this module does not know (one a person added) is kept
// as it was, behind the known ones, in `extra`.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cb::base::converted {

// The folders a project has, in the order they are made.
inline constexpr std::array<std::string_view, 5> kFolders = {"original", "src", "assets", "build", "reports"};

// A name for a project made of any text: runs of anything but letters, digits, '.', '_' and '-' become one '_'; dots and
// underscores are cut from both ends; nothing left is "binary".
std::string clean_name(std::string_view name);

// A project name for a binary: its file name, unless that is a generic one (default.xex, setup.exe), then the name the
// image gives itself (an XEX's header), else its folder's.
std::string default_name(const std::filesystem::path& binary);

// What kind of file it is as CodeBreaker's detection says it ("PE executable", ...); "" when it is not recognised or
// cannot be read.
std::string kind_of(const std::filesystem::path& binary);

// Lower-case hex SHA-1 of the file's bytes (streamed). IoError when it cannot be read.
std::string sha1_of(const std::filesystem::path& file);

// A key of project.json this module does not read, with its JSON text.
using Extra = std::vector<std::pair<std::string, std::string>>;

// One step of project.json: a command, when it ran, what it made, notes.
struct Step {
    std::string command, when;
    std::vector<std::string> outputs;   // relative to the project folder where they lie in it, else as given
    std::string notes;                  // "" is written as no key
    Extra extra;
};

// What project.json says. Every key is optional: the file may be hand-made or damaged, and a reader asks only for what
// is there.
struct Manifest {
    std::optional<std::string> name, file, source, sha1, kind;
    std::optional<std::uint64_t> size;
    std::vector<Step> steps;
    Extra extra;
};

// The clock steps are stamped with: "2026-01-01T00:00:00" (local time, to the second). A test replaces it.
using Clock = std::function<std::string()>;
std::string local_time_now();

class Project {
public:
    // The folder of a project; its five folders are made if they are not there.
    explicit Project(std::filesystem::path folder, Clock clock = local_time_now);

    const std::filesystem::path& root() const { return root_; }
    std::filesystem::path original() const { return root_ / "original"; }
    std::filesystem::path src() const { return root_ / "src"; }
    std::filesystem::path assets() const { return root_ / "assets"; }
    std::filesystem::path build() const { return root_ / "build"; }
    std::filesystem::path reports() const { return root_ / "reports"; }
    std::filesystem::path manifest_path() const { return root_ / "project.json"; }
    std::string name() const;   // the folder's name

    // project.json's contents; an empty Manifest when the file is missing or is not a JSON object.
    Manifest manifest() const;
    // The copy of the binary in original/; nullopt when the manifest names none.
    std::optional<std::filesystem::path> original_file() const;

    // Note a step: the command, the files or folders it made, and anything a reader needs (how to build or run them).
    // Running the same command again replaces what it said before. Rewrites project.json and README.txt.
    void record(std::string_view command, const std::vector<std::filesystem::path>& outputs = {}, std::string_view notes = {}) const;
    // Write project.json and README.txt for `manifest`.
    void write(const Manifest& manifest) const;

private:
    std::filesystem::path root_;
    Clock clock_;
};

// The Converted folder and the projects in it.
class Store {
public:
    explicit Store(std::filesystem::path root, Clock clock = local_time_now) : root_(std::move(root)), clock_(std::move(clock)) {}

    // The folder (made on demand).
    std::filesystem::path root() const;

    // The project folder of a binary: found again by its SHA-1 (and, when `name` is given, that name), else made with
    // the original copied in. `base` keeps it in another folder than the store's.
    Project project(const std::filesystem::path& binary, const std::optional<std::string>& name = std::nullopt,
                    const std::optional<std::filesystem::path>& base = std::nullopt) const;

private:
    std::filesystem::path root_;
    Clock clock_;
};

}  // namespace cb::base::converted
