// cb/port/views_case.hpp -- test support: the cases of testdata/views (make_views_cases.py): a program's sources, and what
// the Python original made of them. Used by program_prep_test.cpp, old_c_text_test.cpp and views_test.cpp.
#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/old_c_data.hpp"
#include "cb/port/pre_ansi_types.hpp"

namespace cb::port::pre_ansi::vcase {

using J = nlohmann::ordered_json;

struct Case {
    std::string name;
    std::filesystem::path dir;
    Files originals;
    J expected;
    bool real = false;   // (the big trees: files are compared by CRC)
};

inline std::filesystem::path root() { return testing::data_path("port/testdata/views"); }

inline Files read_dir_files(const std::filesystem::path& dir) {
    Files out;
    std::vector<std::filesystem::path> names;
    for (const auto& e : std::filesystem::directory_iterator(dir))
        if (e.is_regular_file()) names.push_back(e.path());
    std::sort(names.begin(), names.end());
    for (const auto& p : names) out.insert_or_assign(p.filename().string(), fs::read_text(p));
    return out;
}

inline Case load(const std::string& name) {
    Case c;
    c.name = name;
    c.dir = root() / name;
    c.real = !name.starts_with("syn-");
    c.originals = read_dir_files(c.real ? testing::data_path("port/testdata/old_c") / name / "originals" : c.dir / "originals");
    c.expected = json::load(c.dir / "expected.json");
    return c;
}

inline const std::vector<std::string>& synthetic_names() {
    static const std::vector<std::string> names = {"syn-anon", "syn-tails", "syn-link", "syn-fnptr", "syn-cast", "syn-twin",
                                                   "syn-views", "syn-columns", "syn-misc", "syn-bss", "syn-passthru", "syn-offsets"};
    return names;
}
inline const std::vector<std::string>& real_names() {
    static const std::vector<std::string> names = {"msc-3.00-p0", "msc-3.00-p3"};
    return names;
}

inline Profile profile_of(const Case& c) {
    return Profile::load((testing::data_path("port/testdata") / (c.expected["profile"].get<std::string>() + ".json")).string());
}

// [code points, CRC-32 of the UTF-8 bytes] of a text, as the generator writes them.
inline J crc(std::string_view t) {
    std::size_t points = 0;
    for (const char ch : t) points += (static_cast<unsigned char>(ch) & 0xC0) != 0x80;
    return J::array({points, crypto::crc32(as_bytes(t))});
}

inline J notes_json(const Notes& notes) {
    J out = J::array();
    for (const auto& kv : notes) out.push_back(J::array({kv.first, kv.second}));
    return out;
}

}  // namespace cb::port::pre_ansi::vcase

// (JSON values are compared as their text: the test macros cannot print a nlohmann value)
#define CHECK_J(actual, expected) \
    CB_CHECK_EQ(::cb::port::pre_ansi::vcase::J(actual).dump(), ::cb::port::pre_ansi::vcase::J(expected).dump())
