// app/batch_queue.cpp -- see batch_queue.hpp.
#include "app/batch_queue.hpp"

#include <algorithm>

#include "core/fs.hpp"

namespace cb::app::batch {

std::size_t Queue::add(const std::vector<std::filesystem::path>& files) {
    for (const std::filesystem::path& file : files) {
        Item item;
        item.file = file;
        item.name = fs::to_text(file.filename());
        items_.push_back(std::move(item));
    }
    return files.size();
}

int Queue::next_waiting() const {
    for (std::size_t i = 0; i < items_.size(); ++i)
        if (items_[i].state == ItemState::Waiting) return static_cast<int>(i);
    return -1;
}

bool Queue::running() const {
    return std::any_of(items_.begin(), items_.end(), [](const Item& i) { return i.state == ItemState::Running; });
}

void Queue::start(std::size_t index) {
    if (index >= items_.size()) return;
    items_[index].state = ItemState::Running;
    items_[index].percent = 0;
}

void Queue::progress(std::size_t index, int percent, const std::string& stage, const std::string& detail) {
    if (index >= items_.size() || items_[index].state != ItemState::Running) return;
    items_[index].percent = std::clamp(percent, 0, 100);
    items_[index].stage = stage;
    items_[index].detail = detail;
}

void Queue::finish(std::size_t index, ItemState state, Item details) {
    if (index >= items_.size()) return;
    Item& item = items_[index];
    item.state = state;
    item.percent = state == ItemState::Done ? 100 : 0;
    item.stage.clear();
    item.detail.clear();
    item.output = std::move(details.output);
    item.files = details.files;
    item.message = std::move(details.message);
    item.checked = std::move(details.checked);
    item.chip = std::move(details.chip);
    item.functions = std::move(details.functions);
    item.can_recheck = details.can_recheck;
}

void Queue::cancel_waiting() {
    for (Item& item : items_)
        if (item.state == ItemState::Waiting) item.state = ItemState::Cancelled;
}

std::size_t Queue::settled() const {
    return static_cast<std::size_t>(std::count_if(items_.begin(), items_.end(), [](const Item& i) { return i.state != ItemState::Waiting && i.state != ItemState::Running; }));
}

std::size_t Queue::done() const {
    return static_cast<std::size_t>(std::count_if(items_.begin(), items_.end(), [](const Item& i) { return i.state == ItemState::Done; }));
}

int Queue::last_done() const {
    for (std::size_t i = items_.size(); i-- > 0;)
        if (items_[i].state == ItemState::Done) return static_cast<int>(i);
    return -1;
}

std::string Queue::progress_words() const { return std::to_string(settled()) + " of " + std::to_string(items_.size()) + " done"; }

int Queue::overall_percent() const {
    if (items_.empty()) return 0;
    double sum = 0;
    for (const Item& i : items_) {
        if (i.state == ItemState::Running) sum += i.percent;
        else if (i.state != ItemState::Waiting) sum += 100;
    }
    return static_cast<int>(sum / static_cast<double>(items_.size()));
}

std::string Queue::status_words(const Item& item) {
    switch (item.state) {
        case ItemState::Waiting: return "Waiting";
        case ItemState::Running: return std::to_string(item.percent) + "%";
        case ItemState::Done: return "Done";
        case ItemState::CannotConvert: return "Can't convert";
        case ItemState::Failed: return "Failed";
        case ItemState::Cancelled: return "Cancelled";
    }
    return {};
}

}  // namespace cb::app::batch
