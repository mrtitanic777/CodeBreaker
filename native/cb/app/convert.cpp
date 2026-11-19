// cb/app/convert.cpp -- the conversion state machine (see convert.hpp): stages, honest percentages, staging, cancel and clean-up.
#include "cb/app/convert.hpp"

#include <algorithm>
#include <cctype>
#include <system_error>

namespace cb::app::convert {

namespace stdfs = std::filesystem;

std::string_view state_name(State state) {
    switch (state) {
        case State::Idle: return "idle";
        case State::Identifying: return "identifying";
        case State::Unpacking: return "unpacking";
        case State::Opening: return "opening";
        case State::Disassembling: return "disassembling";
        case State::Decompiling: return "decompiling";
        case State::Translating: return "translating";
        case State::Checking: return "checking";
        case State::Writing: return "writing";
        case State::Done: return "done";
        case State::Unsupported: return "unsupported";
        case State::Failed: return "failed";
        case State::Cancelled: return "cancelled";
    }
    return "idle";
}

std::string_view state_label(State state) {
    switch (state) {
        case State::Idle: return "Ready";
        case State::Identifying: return "Identifying the file";
        case State::Unpacking: return "Unpacking";
        case State::Opening: return "Opening";
        case State::Disassembling: return "Disassembling";
        case State::Decompiling: return "Decompiling";
        case State::Translating: return "Translating";
        case State::Checking: return "Checking";
        case State::Writing: return "Writing the source";
        case State::Done: return "Done";
        case State::Unsupported: return "Cannot convert this file";
        case State::Failed: return "Something went wrong";
        case State::Cancelled: return "Cancelled";
    }
    return "";
}

bool is_stage(State state) { return state >= State::Identifying && state <= State::Writing; }
bool is_final(State state) { return state >= State::Done; }

double default_weight(State stage) {
    switch (stage) {
        case State::Identifying: return 2;
        case State::Unpacking: return 25;
        case State::Opening: return 5;
        case State::Disassembling: return 35;
        case State::Decompiling: return 40;
        case State::Translating: return 30;
        case State::Checking: return 40;
        case State::Writing: return 8;
        default: return 0;
    }
}

std::filesystem::path unique_output_folder(const std::filesystem::path& root, std::string_view stem) {
    const std::string base(stem.empty() ? std::string_view("converted") : stem);
    std::error_code ec;
    stdfs::path candidate = root / base;
    for (int n = 2; stdfs::exists(candidate, ec); ++n) candidate = root / (base + "_" + std::to_string(n));
    return candidate;
}

std::string with_article(std::string_view what) {
    if (what.empty()) return {};
    const std::size_t end = what.find_first_of(" -(/");
    const std::string_view word = what.substr(0, end);
    bool acronym = word.size() >= 2;
    for (const char c : word)
        if (!(std::isupper(static_cast<unsigned char>(c)) || std::isdigit(static_cast<unsigned char>(c)))) acronym = false;
    const char first = word[0];
    bool an = false;
    if (acronym) {
        an = std::string_view("AEFHILMNORSX8").find(first) != std::string_view::npos;
    } else if (first == 'X' || first == '8') {
        an = true;   // "an Xbox", "an 8-bit"
    } else {
        an = std::string_view("aeiouAEIOU").find(first) != std::string_view::npos;
    }
    return std::string(an ? "an " : "a ") + std::string(what);
}

namespace {

// The machine's side of a run: the order of stages, the weights, the percentage, and what the route may tell it.
class cv_Machine final : public Reporter {
public:
    cv_Machine(const ProgressFn& progress, const std::atomic<bool>& cancel) : progress_(progress), cancel_(cancel) {}

    // The stages that will be passed through (Identifying first, Writing last) and their weights.
    void set_route(const std::vector<State>& stages, const std::map<State, double>& weights) {
        order_.clear();
        order_.push_back(State::Identifying);
        for (const State s : stages)
            if (s != State::Identifying && s != State::Writing) order_.push_back(s);
        order_.push_back(State::Writing);
        total_weight_ = 0;
        for (const State s : order_) total_weight_ += weight_of(s, weights);
        weights_ = weights;
    }

    void enter(State stage) override {
        check();
        current_ = stage;
        passed_.push_back(stage);
        emit(0, 0, {});
    }
    void count(std::uint64_t done, std::uint64_t total, std::string_view detail) override {
        check();
        emit(done, total, detail);
    }
    void check() override {
        if (cancel_.load()) throw Cancelled();
    }
    void note(std::string_view text) override { note_ = std::string(text); }
    const std::string& noted() const { return note_; }

    // Report a state with no work of its own (the end of the run, or Identifying before a route is known).
    void final_state(State state, int percent, std::string detail = {}) {
        passed_.push_back(state);
        Progress p;
        p.state = state;
        p.label = std::string(state_label(state));
        p.detail = std::move(detail);
        p.percent = last_percent_ = std::max(last_percent_, percent);
        if (progress_) progress_(p);
    }

    const std::vector<State>& passed() const { return passed_; }

private:
    double weight_of(State s, const std::map<State, double>& weights) const {
        const auto it = weights.find(s);
        return it != weights.end() ? it->second : default_weight(s);
    }

    void emit(std::uint64_t done, std::uint64_t total, std::string_view detail) {
        double before = 0;
        double own = 0;
        bool found = false;
        for (const State s : order_) {
            if (s == current_) {
                own = weight_of(s, weights_);
                found = true;
                break;
            }
            before += weight_of(s, weights_);
        }
        double fraction = 0;
        if (total > 0) fraction = std::min(1.0, static_cast<double>(done) / static_cast<double>(total));
        Progress p;
        p.state = current_;
        p.label = std::string(state_label(current_));
        p.detail = std::string(detail);
        p.done = done;
        p.total = total;
        const double at = found && total_weight_ > 0 ? (before + own * fraction) / total_weight_ * 100.0 : static_cast<double>(last_percent_);
        // 100 is Done's alone: a stage that is the last one at its end shows 99
        p.percent = last_percent_ = std::max(last_percent_, std::min(99, static_cast<int>(at)));
        if (progress_) progress_(p);
    }

    const ProgressFn& progress_;
    const std::atomic<bool>& cancel_;
    std::vector<State> order_{State::Identifying, State::Writing};
    std::map<State, double> weights_;
    double total_weight_ = default_weight(State::Identifying) + default_weight(State::Writing);
    State current_ = State::Idle;
    int last_percent_ = 0;
    std::vector<State> passed_;
    std::string note_;
};

void cv_remove(const stdfs::path& folder) {
    std::error_code ec;
    stdfs::remove_all(folder, ec);
}

// The files under `folder`, relative, in name order.
std::vector<stdfs::path> cv_files_under(const stdfs::path& folder) {
    std::vector<stdfs::path> files;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(folder, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_regular_file(ec)) files.push_back(stdfs::relative(it->path(), folder, ec));
    std::sort(files.begin(), files.end());
    return files;
}

}  // namespace

Outcome convert(const Request& request, const Planner& planner, const ProgressFn& progress, const std::atomic<bool>& cancel) {
    Outcome outcome;
    cv_Machine machine(progress, cancel);
    stdfs::path out, staging;
    bool out_made = false;
    try {
        machine.enter(State::Identifying);
        std::error_code ec;
        if (request.input.empty() || !stdfs::exists(request.input, ec)) {
            outcome.state = State::Failed;
            outcome.message = "The file was not found: " + request.input.string();
        } else if (stdfs::is_directory(request.input, ec)) {
            outcome.state = State::Unsupported;
            outcome.what = "folder";
            outcome.message = "This is a folder. CodeBreaker converts one file at a time: pick the program itself.";
        } else {
            Plan plan = planner(request.input);
            outcome.what = plan.what;
            if (!plan.route) {
                outcome.state = State::Unsupported;
                if (!plan.refusal.empty()) outcome.message = plan.refusal;
                else
                    outcome.message = plan.what.empty() ? "CodeBreaker does not recognise this file, so it cannot convert it."
                                                        : "This looks like " + with_article(plan.what) + ". CodeBreaker cannot convert it.";
            } else {
                const Route& route = *plan.route;
                outcome.route = route.name;
                machine.set_route(route.stages, route.weights);
                if (request.out_dir) {
                    const stdfs::path parent = request.out_dir->parent_path().empty() ? stdfs::path(".") : request.out_dir->parent_path();
                    out = unique_output_folder(parent, request.out_dir->filename().string());
                } else {
                    out = unique_output_folder(request.converted_root, request.input.stem().string() + plan.folder_suffix);
                }
                staging = out;
                staging += ".partial";
                cv_remove(staging);
                stdfs::create_directories(staging);

                route.run(machine, request.input, staging);

                // Writing: the finished files move from the staging folder into the output folder, one at a time.
                machine.enter(State::Writing);
                const std::vector<stdfs::path> files = cv_files_under(staging);
                if (files.empty()) {
                    outcome.state = State::Failed;
                    outcome.message = "The conversion produced no source code.";
                } else {
                    stdfs::create_directories(out);
                    out_made = true;
                    std::uint64_t done = 0;
                    machine.count(0, files.size(), {});
                    for (const stdfs::path& rel : files) {
                        stdfs::create_directories((out / rel).parent_path());
                        std::error_code move_ec;
                        stdfs::rename(staging / rel, out / rel, move_ec);
                        if (move_ec) {
                            stdfs::copy_file(staging / rel, out / rel, stdfs::copy_options::overwrite_existing);
                            stdfs::remove(staging / rel, move_ec);
                        }
                        machine.count(++done, files.size(), rel.generic_string());
                    }
                    outcome.state = State::Done;
                    outcome.output = out;
                    outcome.files = files.size();
                    outcome.checked = machine.noted();
                }
            }
        }
    } catch (const Cancelled&) {
        outcome.state = State::Cancelled;
        outcome.message = "The conversion was cancelled.";
        if (out_made) cv_remove(out);
    } catch (const NotConvertible& e) {
        outcome.state = State::Unsupported;
        outcome.message = e.what();
    } catch (const std::exception& e) {
        outcome.state = State::Failed;
        outcome.message = e.what();
        if (out_made) cv_remove(out);
    }
    if (!staging.empty()) cv_remove(staging);
    if (outcome.state != State::Done) outcome.output.clear();
    machine.final_state(outcome.state, outcome.state == State::Done ? 100 : 0, outcome.message);
    outcome.stages = machine.passed();
    return outcome;
}

}  // namespace cb::app::convert
