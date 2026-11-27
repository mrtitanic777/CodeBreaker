// app/convert.cpp -- the conversion state machine (see convert.hpp): stages, honest percentages, staging, cancel and clean-up.
#include "app/convert.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <deque>
#include <fstream>
#include <set>
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
        case State::Building: return "building";
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
        case State::Building: return "Building";
        case State::Done: return "Done";
        case State::Unsupported: return "Cannot convert this file";
        case State::Failed: return "Something went wrong";
        case State::Cancelled: return "Cancelled";
    }
    return "";
}

FunctionVerdict verdict_of(store::Class cls) {
    switch (cls) {
        case store::Class::Verified:
        case store::Class::Weak: return FunctionVerdict::Matches;
        case store::Class::Unverified: return FunctionVerdict::Differs;
        default: return FunctionVerdict::NotChecked;
    }
}

bool is_stage(State state) { return state >= State::Identifying && state <= State::Building; }
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
        case State::Building: return 40;
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

// ---- the character stream (Progress::lines) ----
// How many lines ride on one report, how many may wait, how much of one source file is shown, and how long a shown line may be.
constexpr std::size_t kCvLinesPerReport = 12;
constexpr std::size_t kCvLinesWaiting = 4000;
constexpr std::size_t kCvLinesPerFile = 160;
constexpr std::size_t kCvLineWidth = 100;
constexpr std::uintmax_t kCvBytesPerFile = 64 * 1024;
constexpr auto kCvScanEvery = std::chrono::milliseconds(250);    // the staging folder is looked at no more often than this
constexpr auto kCvTrickleEvery = std::chrono::milliseconds(80);  // a route that only calls check() still lets the stream move this often

// "0000  4D 5A 90 00 03 00 00 00  04 00 00 00 FF FF 00 00  MZ..............": the first bytes of a file, sixteen to a line.
std::vector<std::string> cv_hex_lines(const stdfs::path& file, std::size_t bytes) {
    std::vector<std::string> lines;
    std::ifstream in(file, std::ios::binary);
    std::vector<unsigned char> data(bytes);
    in.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(bytes));
    data.resize(static_cast<std::size_t>(std::max<std::streamsize>(0, in.gcount())));
    for (std::size_t at = 0; at < data.size(); at += 16) {
        char buf[8];
        std::snprintf(buf, sizeof buf, "%04X", static_cast<unsigned>(at));
        std::string line = buf;
        line += "  ";
        std::string text;
        for (std::size_t i = 0; i < 16; ++i) {
            if (i == 8) line += ' ';
            if (at + i < data.size()) {
                const unsigned char b = data[at + i];
                std::snprintf(buf, sizeof buf, "%02X ", b);
                line += buf;
                text += (b >= 0x20 && b < 0x7F) ? static_cast<char>(b) : '.';
            } else {
                line += "   ";
            }
        }
        lines.push_back(line + " " + text);
    }
    return lines;
}

// The text of a source file as stream lines: its name, then its first non-blank lines (tabs as spaces, cut to a width). A file that is not text
// (it has a zero byte in its first 64 KB) gives only its name.
std::vector<std::string> cv_text_lines(const stdfs::path& file, const std::string& name) {
    std::vector<std::string> lines{"=== " + name + " ==="};
    std::ifstream in(file, std::ios::binary);
    std::string data(kCvBytesPerFile, '\0');
    in.read(data.data(), static_cast<std::streamsize>(data.size()));
    data.resize(static_cast<std::size_t>(std::max<std::streamsize>(0, in.gcount())));
    if (data.find('\0') != std::string::npos) return lines;
    std::size_t at = 0;
    while (at < data.size() && lines.size() <= kCvLinesPerFile) {
        std::size_t end = data.find('\n', at);
        if (end == std::string::npos) end = data.size();
        std::string line;
        for (std::size_t i = at; i < end && line.size() < kCvLineWidth; ++i) {
            const char c = data[i];
            if (c == '\t') line += "    ";
            else if (c != '\r') line += c;
        }
        while (!line.empty() && line.back() == ' ') line.pop_back();
        if (!line.empty()) lines.push_back(std::move(line));
        at = end + 1;
    }
    return lines;
}

// The machine's side of a run: the order of stages, the weights, the percentage, and what the route may tell it.
class cv_Machine final : public Reporter {
public:
    cv_Machine(const ProgressFn& progress, const std::atomic<bool>& cancel) : progress_(progress), cancel_(cancel) {}

    // The staging folder: the text of each source file that appears in it joins the stream.
    void watch(const stdfs::path& folder) { watch_ = folder; }

    // The first bytes of the input join the stream, and are sent at once (Identifying has no counts to carry them).
    void stream_hex(const stdfs::path& file) {
        for (std::string& line : cv_hex_lines(file, 256)) push(std::move(line));
        emit(last_done_, last_total_, last_detail_);
    }

    void preview(std::string_view line) override { push(std::string(line.substr(0, kCvLineWidth))); }

    // The stages that will be passed through (Identifying first, Writing last) and their weights.
    void set_route(const std::vector<State>& stages, const std::map<State, double>& weights, bool builds = false) {
        order_.clear();
        order_.push_back(State::Identifying);
        for (const State s : stages)
            if (s != State::Identifying && s != State::Writing && s != State::Building) order_.push_back(s);
        order_.push_back(State::Writing);
        if (builds) order_.push_back(State::Building);
        total_weight_ = 0;
        for (const State s : order_) total_weight_ += weight_of(s, weights);
        weights_ = weights;
    }

    void enter(State stage) override {
        check();
        if (stage == State::Writing && progress_) scan(true);
        current_ = stage;
        passed_.push_back(stage);
        emit(0, 0, {});
    }
    void count(std::uint64_t done, std::uint64_t total, std::string_view detail) override {
        check();
        if (current_ == State::Writing && !detail.empty()) push("-> " + std::string(detail));
        emit(done, total, detail);
    }
    void check() override {
        if (cancel_.load()) throw Cancelled();
        // a route busy between counts (a writer that only checks for cancel) still lets the stream move
        if (progress_ && current_ != State::Idle && std::chrono::steady_clock::now() - last_emit_ >= kCvTrickleEvery) {
            scan();
            if (!waiting_.empty()) emit(last_done_, last_total_, last_detail_);
        }
    }
    void note(std::string_view text) override { note_ = std::string(text); }
    const std::string& noted() const { return note_; }
    void functions(std::vector<FunctionRecord> records, bool can_recheck) override {
        functions_ = std::move(records);
        can_recheck_ = can_recheck;
    }
    const std::vector<FunctionRecord>& recorded() const { return functions_; }
    bool can_recheck() const { return can_recheck_; }

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

    void push(std::string line) {
        waiting_.push_back(std::move(line));
        while (waiting_.size() > kCvLinesWaiting) waiting_.pop_front();
    }

    // New regular files under the staging folder join the stream, at most every kCvScanEvery (`always`: now, as Writing begins, before the
    // files move out of the staging folder).
    void scan(bool always = false) {
        if (watch_.empty()) return;
        const auto now = std::chrono::steady_clock::now();
        if (!always && now - last_scan_ < kCvScanEvery) return;
        last_scan_ = now;
        std::error_code ec;
        std::vector<stdfs::path> fresh;
        for (stdfs::recursive_directory_iterator it(watch_, ec), end; !ec && it != end; it.increment(ec)) {
            if (!it->is_regular_file(ec) || seen_.contains(it->path())) continue;
            if (it->file_size(ec) == 0) continue;   // (still being written: taken on a later look)
            fresh.push_back(it->path());
        }
        std::sort(fresh.begin(), fresh.end());
        for (const stdfs::path& file : fresh) {
            seen_.insert(file);
            for (std::string& line : cv_text_lines(file, stdfs::relative(file, watch_, ec).generic_string())) push(std::move(line));
        }
    }

    void emit(std::uint64_t done, std::uint64_t total, std::string_view detail) {
        last_done_ = done;
        last_total_ = total;
        last_detail_ = std::string(detail);
        last_emit_ = std::chrono::steady_clock::now();
        if (progress_) scan();
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
        for (std::size_t n = 0; n < kCvLinesPerReport && !waiting_.empty(); ++n) {
            p.lines.push_back(std::move(waiting_.front()));
            waiting_.pop_front();
        }
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
    std::vector<FunctionRecord> functions_;
    bool can_recheck_ = false;
    // the character stream
    stdfs::path watch_;
    std::set<stdfs::path> seen_;
    std::deque<std::string> waiting_;
    std::chrono::steady_clock::time_point last_scan_{}, last_emit_{};
    std::uint64_t last_done_ = 0, last_total_ = 0;
    std::string last_detail_;
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
        } else {
            // a folder is converted only when a route takes it whole (a Java game's folder); otherwise it is refused as one
            const bool folder = stdfs::is_directory(request.input, ec);
            if (progress && !folder) machine.stream_hex(request.input);
            Plan plan = planner(request.input);
            outcome.what = plan.what;
            if (!plan.route && folder && plan.refusal.empty()) {
                outcome.state = State::Unsupported;
                outcome.what = "folder";
                outcome.message = "This is a folder. CodeBreaker converts one file at a time: pick the program itself.";
            } else if (!plan.route) {
                outcome.state = State::Unsupported;
                if (!plan.refusal.empty()) outcome.message = plan.refusal;
                else
                    outcome.message = plan.what.empty() ? "CodeBreaker does not recognise this file, so it cannot convert it."
                                                        : "This looks like " + with_article(plan.what) + ". CodeBreaker cannot convert it.";
            } else {
                const Route& route = *plan.route;
                outcome.route = route.name;
                machine.set_route(route.stages, route.weights, static_cast<bool>(route.build));
                if (request.out_dir) {
                    const stdfs::path parent = request.out_dir->parent_path().empty() ? stdfs::path(".") : request.out_dir->parent_path();
                    out = unique_output_folder(parent, request.out_dir->filename().string());
                } else {
                    const std::string stem = plan.output_name.empty() ? request.input.stem().string() : plan.output_name;
                    if (request.converted_root.empty()) {   // (no "converted" setting: next to the input, <name>-converted)
                        const stdfs::path beside = request.input.parent_path().empty() ? stdfs::path(".") : request.input.parent_path();
                        out = unique_output_folder(beside, stem + plan.folder_suffix + "-converted");
                    } else {
                        out = unique_output_folder(request.converted_root, stem + plan.folder_suffix);
                    }
                }
                staging = out;
                staging += ".partial";
                cv_remove(staging);
                stdfs::create_directories(staging);
                machine.watch(staging);

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
                    if (route.build) {   // Building: in the output folder itself; a failure keeps the source and says so
                        machine.enter(State::Building);
                        try {
                            route.build(machine, out);
                        } catch (const Cancelled&) {
                            throw;
                        } catch (const std::exception& e) {
                            outcome.state = State::Failed;
                            outcome.message = std::string(e.what()) + " The source is in " + std::string(reinterpret_cast<const char*>(out.u8string().c_str())) + ".";
                            out_made = false;   // (kept: the source is good, only its build is not)
                        }
                    }
                    if (outcome.state != State::Failed) {
                        outcome.state = State::Done;
                        outcome.output = out;
                        outcome.files = files.size();
                        outcome.checked = machine.noted();
                        outcome.functions = machine.recorded();
                        outcome.can_recheck = machine.can_recheck();
                    }
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
