// source/console_writer.hpp -- the one source writer for the consoles: a platform is a description (config/source/platforms/<name>.json), the
// writer reads it and writes the program through the shape it names (docs/SOURCE_WRITER.md).
//
//     image_writer::Written r = console_writer::reconstruct("coleco", "game.col", "out");
//     r.source                  out/game.asm, in the platform's dialect (the description's `dialect`: sjasmplus for the Z80 machines)
//     r.same                    the source, built, is the program byte for byte
//
// What a platform says, as data: its title, its shape (how the program lies in memory: `z80-block`, one block at one address), its dialect,
// which knowledge tables name its hardware (config/knowledge/<platform>/registers.json: `ports` a hardware section for IN/OUT ports, `fixed`
// a table of fixed addresses outside the program -- BIOS or firmware entry points, system variables) and the head comment, one template per
// line over the values the platform's loader read from the file's header.
//
// What stays code is the loader (source/console_platforms.cpp): which bytes of the file are the program, where they sit, its entry points and
// the header's values by name. The file formats themselves are formats/<x>'s.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "core/bytes.hpp"
#include "source/image_writer.hpp"

namespace cb::source::console_writer {

// A value a head template says: a number, a text, or a list of texts (a line with `each` is written once per element).
using Value = std::variant<std::int64_t, std::string, std::vector<std::string>>;
using Values = std::map<std::string, Value, std::less<>>;

// One line of the head comment: written when `when` is empty or names a value that is set, once per element of `each` when given (as `{it}`).
struct HeadLine {
    std::string text;
    std::string when;
    std::string each;
};

// A platform's description (config/source/platforms/<name>.json).
struct Description {
    std::string name;        // the file's name: what reconstruct() is asked for
    std::string platform;    // the knowledge base's platform (config/knowledge/<platform>)
    std::string title;
    std::string shape;       // "z80-block"
    std::string dialect;     // config/dialects/<dialect>.json
    std::string ports;       // the knowledge hardware section of the I/O ports ("" none)
    std::string fixed;       // the knowledge table of the fixed addresses outside the program ("" none)
    std::vector<HeadLine> head;
};

// The description of `name`. UsageError when there is none.
const Description& describe(std::string_view name);

// What a loader read from a file: the program's bytes, the address of the first, its entry points (address -> name) and the header's values.
struct Loaded {
    Bytes image;
    std::uint32_t origin = 0;
    image_writer::Names entries;
    Values values;
};

// The program in `file`, as the platform's loader reads it (IoError when the file cannot be read, FormatError when it is not the platform's).
Loaded load(std::string_view platform, const std::filesystem::path& file);

// `text` with each {name} (a value), {name:spec} (a number in std::format's spec: 04X) and {name?yes|no} (the branch for whether the value is
// set: non-zero, non-empty; the branches are templates too) put in. UsageError for a name `values` does not have.
std::string fill(std::string_view text, const Values& values);

// The program written as source to `folder` (<stem>.asm for a file named `file_name`) and rebuilt. The writer adds the values every program
// has: `file` (file_name), `size` and `kb` (the image's), `origin`.
image_writer::Written write(const Description& description, const Loaded& program, std::string_view file_name, const std::filesystem::path& folder);

// load() and write() for a file.
image_writer::Written reconstruct(std::string_view platform, const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::console_writer
