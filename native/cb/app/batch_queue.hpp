// cb/app/batch_queue.hpp -- the queue behind dropping several files on the window: the files wait, run one after another, and each ends Done, Can't
// convert, Failed or Cancelled. Files dropped while the queue runs join its end. The model only keeps the books (what each file is doing and how far the
// whole queue has got); the window runs the files. No operating-system API: tested on every system.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace cb::app::batch {

enum class ItemState { Waiting, Running, Done, CannotConvert, Failed, Cancelled };

struct Item {
    std::filesystem::path file;
    std::string name;                  // the file's name
    ItemState state = ItemState::Waiting;
    int percent = 0;                   // while Running; 100 once Done
    std::string stage;                 // "Decompiling"
    std::string detail;                // "17 of 39 functions"
    // what the run ended with
    std::filesystem::path output;
    std::uint64_t files = 0;           // files written
    std::string message;               // why a file could not be converted
    std::string checked;               // how a translation was proved
    std::string chip;                  // what it became ("C", "C++17")
};

class Queue {
public:
    // The files join the end of the queue; the count added.
    std::size_t add(const std::vector<std::filesystem::path>& files);
    void clear() { items_.clear(); }

    const std::vector<Item>& items() const { return items_; }
    std::size_t total() const { return items_.size(); }
    bool empty() const { return items_.empty(); }
    // The first file still waiting; -1 when none is.
    int next_waiting() const;
    // A file is running now.
    bool running() const;
    // Something is running or waiting.
    bool active() const { return running() || next_waiting() >= 0; }

    void start(std::size_t index);
    void progress(std::size_t index, int percent, const std::string& stage, const std::string& detail);
    // The run of `index` ended with `state` (Done, CannotConvert, Failed or Cancelled) and these details (state, file, name and percent are kept).
    void finish(std::size_t index, ItemState state, Item details);
    // Every file still waiting becomes Cancelled.
    void cancel_waiting();

    // Files that have ended in any way, and how many ended Done.
    std::size_t settled() const;
    std::size_t done() const;
    // The last file that ended Done; -1 when none did.
    int last_done() const;
    // "2 of 5 done": settled of total.
    std::string progress_words() const;
    // The share of the whole queue done, 0..100: the settled files and the running one's percentage.
    int overall_percent() const;

    // The status text of one row: "Done", "62%", "Waiting", "Can't convert", "Failed", "Cancelled".
    static std::string status_words(const Item& item);

private:
    std::vector<Item> items_;
};

}  // namespace cb::app::batch
