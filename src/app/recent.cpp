// app/recent.cpp -- see recent.hpp.
#include "app/recent.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <ctime>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"

namespace cb::app::recent {

namespace {

namespace stdfs = std::filesystem;

constexpr std::int64_t kDay = 86400;

std::int64_t rc_floor_div(std::int64_t a, std::int64_t b) { return a / b - ((a % b != 0) && ((a < 0) != (b < 0)) ? 1 : 0); }

// The calendar date of a day number (days since 1970-01-01): Howard Hinnant's civil_from_days.
void rc_civil(std::int64_t days, int& year, int& month, int& day) {
    days += 719468;
    const std::int64_t era = rc_floor_div(days, 146097);
    const std::int64_t doe = days - era * 146097;
    const std::int64_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    const std::int64_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const std::int64_t mp = (5 * doy + 2) / 153;
    day = static_cast<int>(doy - (153 * mp + 2) / 5 + 1);
    month = static_cast<int>(mp < 10 ? mp + 3 : mp - 9);
    year = static_cast<int>(yoe + era * 400 + (month <= 2 ? 1 : 0));
}

}  // namespace

void add(std::vector<Entry>& list, Entry e) {
    list.erase(std::remove_if(list.begin(), list.end(), [&](const Entry& o) { return o.folder == e.folder; }), list.end());
    list.insert(list.begin(), std::move(e));
    if (list.size() > kMaxRecent) list.resize(kMaxRecent);
}

void remove_at(std::vector<Entry>& list, std::size_t index) {
    if (index < list.size()) list.erase(list.begin() + static_cast<std::ptrdiff_t>(index));
}

stdfs::path store_file(const stdfs::path& settings_file) {
    if (settings_file.empty()) return {};
    return settings_file.parent_path() / (settings_file.stem().string() + ".recent.json");
}

std::vector<Entry> load(const stdfs::path& file) {
    std::vector<Entry> out;
    try {
        std::error_code ec;
        if (file.empty() || !stdfs::is_regular_file(file, ec)) return out;
        const Json doc = json::load(file);
        if (!doc.is_object() || !doc.contains("recent") || !doc["recent"].is_array()) return out;
        for (const Json& item : doc["recent"]) {
            if (!item.is_object() || !item.contains("folder") || !item["folder"].is_string()) continue;
            Entry e;
            e.folder = item["folder"].get<std::string>();
            e.name = item.value("name", std::string());
            e.chip = item.value("chip", std::string());
            e.files = item.value("files", std::uint64_t{0});
            e.when = item.value("when", std::int64_t{0});
            if (!e.folder.empty() && out.size() < kMaxRecent) out.push_back(std::move(e));
        }
    } catch (const std::exception&) {   // (an unreadable list is an empty one)
        out.clear();
    }
    return out;
}

void save(const stdfs::path& file, const std::vector<Entry>& list) {
    try {
        if (file.empty()) return;
        Json items = Json::array();
        for (const Entry& e : list) items.push_back(Json{{"name", e.name}, {"chip", e.chip}, {"folder", e.folder}, {"files", e.files}, {"when", e.when}});
        Json doc = Json::object();
        doc["recent"] = std::move(items);
        fs::make_dirs(file.parent_path());
        json::save(file, doc);
    } catch (const std::exception&) {   // (not being able to remember is not an error)
    }
}

std::int64_t now_seconds() {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int utc_offset_seconds(std::int64_t at) {
    const std::time_t t = static_cast<std::time_t>(at);
    std::tm local{};
#ifdef _WIN32
    if (localtime_s(&local, &t) != 0) return 0;
#else
    if (localtime_r(&t, &local) == nullptr) return 0;
#endif
    // the local clock's reading as if it were UTC, less the real time: the offset
    std::int64_t y = local.tm_year + 1900, m = local.tm_mon + 1;
    // days from civil
    y -= m <= 2 ? 1 : 0;
    const std::int64_t era = rc_floor_div(y, 400);
    const std::int64_t yoe = y - era * 400;
    const std::int64_t doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + local.tm_mday - 1;
    const std::int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    const std::int64_t day_number = era * 146097 + doe - 719468;
    const std::int64_t reading = day_number * kDay + local.tm_hour * 3600 + local.tm_min * 60 + local.tm_sec;
    return static_cast<int>(reading - at);
}

std::string when_words(std::int64_t now, std::int64_t then, int utc_offset) {
    if (then > now) then = now;
    const std::int64_t age = now - then;
    if (age < 60) return "just now";
    if (age < 3600) {
        const std::int64_t minutes = age / 60;
        return std::to_string(minutes) + (minutes == 1 ? " minute ago" : " minutes ago");
    }
    const std::int64_t local_then = then + utc_offset, local_now = now + utc_offset;
    const std::int64_t day_then = rc_floor_div(local_then, kDay), day_now = rc_floor_div(local_now, kDay);
    if (day_then == day_now) {
        const std::int64_t second = local_then - day_then * kDay;
        const int hour = static_cast<int>(second / 3600), minute = static_cast<int>(second / 60 % 60);
        char text[32];
        std::snprintf(text, sizeof text, "today %d:%02d %s", hour % 12 == 0 ? 12 : hour % 12, minute, hour < 12 ? "AM" : "PM");
        return text;
    }
    const std::int64_t days = day_now - day_then;
    if (days == 1) return "yesterday";
    if (days < 7) return std::to_string(days) + " days ago";
    static const char* const kMonths[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int year = 0, month = 0, day = 0, year_now = 0, month_now = 0, day_now_of_month = 0;
    rc_civil(day_then, year, month, day);
    rc_civil(day_now, year_now, month_now, day_now_of_month);
    std::string text = std::string(kMonths[month - 1]) + " " + std::to_string(day);
    if (year != year_now) text += ", " + std::to_string(year);
    return text;
}

}  // namespace cb::app::recent
