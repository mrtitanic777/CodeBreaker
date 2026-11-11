// cb/base/runs.hpp -- a run's own folder, gone with the run: copies of disks and CDs never pile up.
//
//     runs::Folder work(state, "phase3", tag);          // <state>/phase3/runs/<tag>_<time>_<pid>/
//     ...copy the disk and the CD into work.path(), boot them...
//     path shots = runs::results(state, "phase3");      // <state>/phase3/shots/: one screenshot, one log per tag
//
// A harness that boots a machine works on copies: a 650 MB hard disk, a CD image. The copy is the run's, in a
// folder of its own under <state>/<tool>/runs/, and is deleted when the run ends, however it ends. A run that was
// killed never reaches its end; so every run, as it starts, deletes the run folders of its tool older than a day
// (a run still going holds its files open, and is not deleted under it). What a run leaves for the user -- its last
// screenshot, its log -- goes to the tool's one results folder, under the run's tag, the one before it overwritten.
//
// `state` is CodeBreaker's state folder (the application's configuration says where); nothing here reads settings.
#pragma once
#include <chrono>
#include <filesystem>
#include <string_view>
#include <vector>

namespace cb::base::runs {

// A run folder older than this is a killed run's.
inline constexpr std::chrono::seconds kKilledRunAge{24 * 3600};

// The tool's one folder of what its runs leave: <state>/<tool>/<name> (made if it is not there).
std::filesystem::path results(const std::filesystem::path& state, std::string_view tool,
                              std::string_view name = "shots");

// Delete a run folder. A machine just stopped lets go of its files a moment later, so deletion is retried (half a
// second apart) up to `tries` times. True when the folder is gone.
bool remove(const std::filesystem::path& folder, int tries = 20);

// Delete the tool's run folders older than `age` (the killed runs'). Returns those deleted.
std::vector<std::filesystem::path> sweep(const std::filesystem::path& state, std::string_view tool,
                                         std::chrono::seconds age = kKilledRunAge);

// A new run folder of the tool's, the killed runs' swept first: <tag>_<yyyymmdd-hhmmss>_<pid>, with _<n> added when
// that name is taken. The tag is cut to 40 characters and every run of characters other than letters, digits,
// '.' and '-' in it becomes one '-'. The caller deletes the folder with remove() when the run ends (Folder does both).
std::filesystem::path start(const std::filesystem::path& state, std::string_view tool, std::string_view tag = "run");

// A run folder for the length of a scope, deleted at its end however the scope ends.
class Folder {
public:
    Folder(const std::filesystem::path& state, std::string_view tool, std::string_view tag = "run")
        : path_(start(state, tool, tag)) {}
    Folder(const Folder&) = delete;
    Folder& operator=(const Folder&) = delete;
    ~Folder() { runs::remove(path_); }
    const std::filesystem::path& path() const { return path_; }

private:
    std::filesystem::path path_;
};

}  // namespace cb::base::runs
