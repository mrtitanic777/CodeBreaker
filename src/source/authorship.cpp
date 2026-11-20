// source/authorship.cpp -- whose file is it? (authorship.hpp)

#include "source/authorship.hpp"

#include <algorithm>
#include <array>

#include "core/error.hpp"
#include "core/random_file.hpp"

namespace cb::source {

namespace {

constexpr std::array<std::string_view, 6> kMarks = {"reconstructed source",
                                                    "behavioral reconstruction",
                                                    "authored-source reconstruction",
                                                    "(functional group, reconstructed)",
                                                    "decompiled by CodeBreaker",
                                                    "by CodeBreaker"};

}  // namespace

bool carries_our_mark(std::string_view head) {
    return std::ranges::any_of(kMarks, [&](std::string_view mark) { return head.find(mark) != std::string_view::npos; });
}

bool made_by_us(const std::filesystem::path& path) {
    std::error_code ec;
    if (!std::filesystem::exists(path, ec)) return true;   // not there: ours to write
    try {
        const fs::RandomFile file(path, fs::FileMode::Read);
        return carries_our_mark(as_text(file.read_at(0, kHeadBytes)));   // (as_text of a temporary is used within the statement)
    } catch (const IoError&) {
        return false;   // a file that cannot be read is not ours to replace
    }
}

}  // namespace cb::source
