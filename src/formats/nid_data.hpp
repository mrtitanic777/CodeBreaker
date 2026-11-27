// formats/nid_data.hpp -- the consoles' function-ID tables as data: config/nids/<console>.nids, built into the program by CMake
// (embed_nids.cmake, as config/formats/*.fmt are), read once on first use into a sorted vector and looked up by binary search.
//
// A file is text: `#` comments, `[section]` headers, and one row per line, `key name [extra]` (the key is `0xHEX`, or decimal,
// or -- for the Orbis function table, whose NIDs are base-64 text -- the text itself). Rows are sorted by key; the first of equal keys answers.
#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace cb::formats::nids {

struct Row {
    std::uint64_t key = 0;
    std::string_view name;
    std::string_view extra;   // the library of a PS3 function; empty elsewhere
};

struct TextRow {
    std::string_view key;
    std::string_view name;
};

class NumericTable {
public:
    const Row* find(std::uint64_t key) const;   // nullptr when the table has no such key
    const std::vector<Row>& rows() const { return rows_; }
    std::vector<Row> rows_;
};

class TextTable {
public:
    const TextRow* find(std::string_view key) const;
    const std::vector<TextRow>& rows() const { return rows_; }
    std::vector<TextRow> rows_;
};

// A section of a file of config/nids (`file` is its name, "vita.nids"), parsed on first use and kept. A file or section that is not
// built in gives an empty table. The returned reference stays valid for the life of the program.
const NumericTable& numeric(std::string_view file, std::string_view section);
const TextTable& text(std::string_view file, std::string_view section);

}  // namespace cb::formats::nids
