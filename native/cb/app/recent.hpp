// cb/app/recent.hpp -- the window's list of recent conversions: the last few files turned into source, newest first, kept per user in a small JSON file
// beside the remembered language (convert.hpp's settings_file), and the plain words for when each happened ("2 minutes ago", "today 11:52 PM",
// "yesterday"). No operating-system API: tested on every system.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace cb::app::recent {

inline constexpr std::size_t kMaxRecent = 5;

struct Entry {
    std::string name;                  // the file that was converted: "lz32.dll"
    std::string chip;                  // what it became: "C", "C++17", "Assembly"
    std::string folder;                // the output folder
    std::uint64_t files = 0;           // files written
    std::int64_t when = 0;             // seconds since 1970 (UTC)
};

// `e` first; an earlier entry for the same folder is replaced; at most kMaxRecent are kept (the oldest go).
void add(std::vector<Entry>& list, Entry e);
// Without the entry at `index` (out of range: nothing).
void remove_at(std::vector<Entry>& list, std::size_t index);

// The file the list is kept in for a settings file: beside it, named <its stem>.recent.json (app.recent.json). Empty for an empty path.
std::filesystem::path store_file(const std::filesystem::path& settings_file);
// Read and write the list; an unreadable or missing file is an empty list, a failed write is ignored.
std::vector<Entry> load(const std::filesystem::path& file);
void save(const std::filesystem::path& file, const std::vector<Entry>& list);

std::int64_t now_seconds();
// Seconds the local clock is ahead of UTC at the moment `at` (daylight saving included).
int utc_offset_seconds(std::int64_t at);

// When `then` was, said for `now` on a clock `utc_offset` seconds ahead of UTC: "just now", "1 minute ago", "12 minutes ago", "today 11:52 PM" (earlier
// today), "yesterday", "3 days ago", then the date ("Nov 2", with the year when it is another one).
std::string when_words(std::int64_t now, std::int64_t then, int utc_offset);

}  // namespace cb::app::recent
