// knowledge/describe.hpp -- description files: what the platforms' APIs, services, registers and constants are, as data.
//
// One file per platform and domain, config/knowledge/<platform>/<domain>.json, in one schema:
//
//     {
//       "cbk": 1,
//       "platform": "dos",
//       "domain": "services",
//       "about": "one line: what the file holds",
//       "source": {"what": "where the facts are from", "license": "facts", "checked": "2026-11-20"},
//       "entries": [
//         {"id": "21.3D", "kind": "service", "vector": "21", "select": {"AH": "3D"}, "label": "open file"},
//         ...
//       ]
//     }
//
// An entry has an `id` (unique in its file: the key of the row in the base), a `kind` (api, service, register, region,
// export, constant, type, helper, signature) and whatever its kind needs; optional on any entry: `names` (spellings by
// dialect, {"rgbds": "rLCDC"}), `doc` (one line), `since` and `until` (versions), `origin` (a note), `model` (what an
// emulator's stand-in does). Numbers that are addresses or select values are hex strings. `select` is {register:
// hex text}; a `constant` has a `set` and a `value`.
//
// A machine's `registers` file holds its hardware: `register` entries (a `table` of the machine: "ports", "tia_write"; a `space`: "cpu"
// or "io"; an `address`; a `width` in bytes), `region` entries (a block of the address space: `address`, `size`, `name`, read as the
// prefix of the registers in it), and, for what is not a register, entries of kind `export` (a BIOS routine's entry point: `table`,
// `address`) or `service` (an OS call by code: `table`, `select`). Knowledge::hardware_registers / hardware_names / table_names /
// hardware_regions read them. An `ordinals` file (xbox360) holds `export` entries `<module>#<number>` with module, ordinal and name
// (Knowledge::export_name).
//
// The folders directly under config/knowledge are platforms, except the two that hold other things (`models`, the
// emulator stand-ins; `kept`, the knowledge people gave). A file's rows in the base are platform / domain / id, so the
// loader (sources.hpp) needs no list of them: a new platform is a new folder.
//
//     const ValidationReport report = validate_descriptions(root);        // every file: schema, ids, shapes
//     const Description d = parse_description(text, "dos/services.json"); // one file, or FormatError
#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "knowledge/sources.hpp"
#include "knowledge/value.hpp"

namespace cb::knowledge {

inline constexpr int kDescriptionVersion = 1;

// The folders of config/knowledge that are not platforms.
bool is_platform_folder(std::string_view name);

// A kind an entry may have.
bool is_entry_kind(std::string_view kind);

struct Issue {
    std::string file;    // "dos/services.json"
    std::string where;   // "entries[12]" or "entries[12].select"
    std::string what;
    std::string text() const;   // "dos/services.json: entries[12]: the id repeats entries[3]"
};

struct ValidationReport {
    std::size_t files = 0;
    std::size_t entries = 0;
    std::vector<Issue> issues;
    bool ok() const { return issues.empty(); }
};

// A description file as it is read: its header and its entries by id (in the file's order).
struct Description {
    std::string platform;
    std::string domain;
    std::string about;
    Value source;
    Rows entries;
};

// What is wrong with a description file's document: nothing when the list is empty. `platform` and `domain` are those
// the file's place says (its folder and its name without the extension); the document must say the same.
std::vector<Issue> check_description(const Value& document, std::string_view file, std::string_view platform, std::string_view domain);

// A description file's text as a Description. Throws ParseError (not JSON) or FormatError (the first thing wrong).
// `platform` and `domain` as for check_description.
Description parse_description(std::string_view text, std::string_view file, std::string_view platform, std::string_view domain);

// The description files under `root` (config/knowledge): <platform>/<domain>.json, in order of name.
std::vector<std::filesystem::path> description_files(const std::filesystem::path& root);

// Every file under `root` checked; a file that cannot be read or parsed is an issue too.
ValidationReport validate_descriptions(const std::filesystem::path& root);

// The number a hex text ("0x0F", "0F", "21") names; nullopt when it is not one.
std::optional<std::uint64_t> hex_value(std::string_view text);

}  // namespace cb::knowledge
