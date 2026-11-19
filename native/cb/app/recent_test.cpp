// Tests of the Recent list (recent.hpp): the list kept per user, and the words for when.
#include "cb/app/recent.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::app::recent {
namespace {

Entry rc_entry(const char* name, const char* folder, std::int64_t when = 0) { return Entry{name, "C", folder, 3, when}; }

// 2026-11-19 11:52 PM on a clock that is UTC: 1795132320.
constexpr std::int64_t kNightTime = 1795132320;
constexpr std::int64_t kHourLen = 3600, kDayLen = 86400;

CB_TEST("app.recent.the_newest_is_first_the_same_folder_replaces_and_five_are_kept", [] {
    std::vector<Entry> list;
    for (int i = 0; i < 7; ++i) add(list, rc_entry("a.dll", ("C:/out/" + std::to_string(i)).c_str()));
    CB_CHECK(list.size() == kMaxRecent);
    CB_CHECK_EQ(list.front().folder, "C:/out/6");
    CB_CHECK_EQ(list.back().folder, "C:/out/2");
    add(list, rc_entry("again.dll", "C:/out/3"));   // already there: moves to the front, the others stay
    CB_CHECK(list.size() == kMaxRecent);
    CB_CHECK_EQ(list.front().name, "again.dll");
    CB_CHECK_EQ(list[1].folder, "C:/out/6");
    remove_at(list, 0);
    CB_CHECK(list.size() == kMaxRecent - 1 && list.front().folder == "C:/out/6");
    remove_at(list, 99);   // (out of range: nothing)
    CB_CHECK(list.size() == kMaxRecent - 1);
});

CB_TEST("app.recent.the_list_is_kept_in_a_file_beside_the_settings_and_survives_a_bad_one", [] {
    fs::ScopedTempDir dir{"cb_recent"};
    const std::filesystem::path settings = dir.path() / "app.json";
    const std::filesystem::path file = store_file(settings);
    CB_CHECK_EQ(file.filename().string(), "app.recent.json");
    CB_CHECK(file.parent_path() == dir.path());
    CB_CHECK(store_file({}).empty());
    CB_CHECK(load(file).empty());   // (nothing kept yet)
    std::vector<Entry> list;
    add(list, Entry{"lz32.dll", "C", "C:/Converted/lz32", 12, 1000});
    add(list, Entry{"Bank.cs", "C++17", "C:/Converted/Bank_cpp17", 3, 2000});
    save(file, list);
    const std::vector<Entry> back = load(file);
    CB_CHECK(back.size() == 2);
    CB_CHECK_EQ(back[0].name, "Bank.cs");
    CB_CHECK_EQ(back[0].chip, "C++17");
    CB_CHECK_EQ(back[0].folder, "C:/Converted/Bank_cpp17");
    CB_CHECK(back[0].files == 3 && back[0].when == 2000);
    CB_CHECK_EQ(back[1].name, "lz32.dll");
    fs::write_text(file, "not json");
    CB_CHECK(load(file).empty());
    fs::write_text(file, R"({"recent": [{"name": "no folder"}, {"folder": "C:/x"}, 7]})");   // (entries without a folder are skipped)
    CB_CHECK(load(file).size() == 1);
});

CB_TEST("app.recent.when_is_said_in_minutes_then_today_yesterday_days_and_dates", [] {
    CB_CHECK_EQ(when_words(kNightTime, kNightTime, 0), "just now");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 59, 0), "just now");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 60, 0), "1 minute ago");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 120, 0), "2 minutes ago");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 59 * 60, 0), "59 minutes ago");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime + 500, 0), "just now");   // (a clock that went back is not "in the future")
    // earlier the same day, on a 12-hour clock: 11:52 PM now, so 9:52 PM and 12:52 AM were today
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 2 * kHourLen, 0), "today 9:52 PM");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 23 * kHourLen, 0), "today 12:52 AM");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 24 * kHourLen, 0), "yesterday");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 2 * kDayLen, 0), "2 days ago");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 6 * kDayLen, 0), "6 days ago");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 7 * kDayLen, 0), "Nov 12");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 400 * kDayLen, 0), "Oct 15, 2025");
    // the clock of the place decides what "today" is: 11:52 PM in UTC is 1:52 AM the next day at +2 hours
    CB_CHECK_EQ(when_words(kNightTime + 2 * kHourLen, kNightTime - 2 * kHourLen, 2 * kHourLen), "yesterday");
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 3 * kHourLen, -9 * kHourLen), "today 11:52 AM");
    // noon and midnight read 12
    CB_CHECK_EQ(when_words(kNightTime, kNightTime - 11 * kHourLen - 52 * 60, 0), "today 12:00 PM");
});

CB_TEST("app.recent.the_local_clock_offset_is_a_whole_number_of_minutes_a_day_apart_at_most", [] {
    const int offset = utc_offset_seconds(now_seconds());
    CB_CHECK(offset % 60 == 0);
    CB_CHECK(offset > -15 * 3600 && offset < 15 * 3600);
    CB_CHECK(now_seconds() > 1795000000);   // (after this was written)
});

}  // namespace
}  // namespace cb::app::recent
