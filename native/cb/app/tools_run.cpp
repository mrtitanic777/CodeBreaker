// cb/app/tools_run.cpp -- see tools_run.hpp.
#include "cb/app/tools_run.hpp"

#include <cctype>

#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"

namespace cb::app::tools {

namespace {
constexpr std::size_t kTrRetained = 256 * 1024;
}

int percent_of_line(std::string_view line) {
    std::size_t i = 0;
    while (i < line.size() && line[i] == ' ') ++i;
    std::size_t digits = 0;
    int value = 0;
    while (i < line.size() && std::isdigit(static_cast<unsigned char>(line[i])) && digits < 4) {
        value = value * 10 + (line[i] - '0');
        ++i;
        ++digits;
    }
    if (digits == 0 || i >= line.size() || line[i] != '%' || value > 100) return -1;
    return value;
}

struct Run::Shared {
    mutable std::mutex lock;
    RunState state;
    std::unique_ptr<process::Process> child;   // owned by the reader thread's lifetime; stop() kills through it under the lock
    std::string partial_line;                  // the line being read (a percentage is read when the line is whole)
};

Run::Run(std::vector<std::string> argv, std::filesystem::path exe, std::filesystem::path cwd) : shared_(std::make_shared<Shared>()) {
    process::ProcessSpec spec;
    spec.argv.push_back(fs::to_text(exe.empty() ? fs::executable_path() : exe));
    spec.argv.insert(spec.argv.end(), argv.begin(), argv.end());
    if (!cwd.empty()) spec.cwd = cwd;
    spec.out = process::Capture::Collect;
    spec.err = process::Capture::Merge;
    try {
        shared_->child = std::make_unique<process::Process>(process::start(spec));
    } catch (const std::exception& e) {
        shared_->state.running = false;
        shared_->state.failure = e.what();
        shared_->state.exit_code = 1;
        shared_->state.version = 1;
        return;
    }
    std::shared_ptr<Shared> shared = shared_;
    reader_ = std::thread([shared] {
        Bytes buffer(4096);
        for (;;) {
            std::size_t n = 0;
            try {
                n = shared->child->read(process::Process::Stream::Out, ByteSpan(buffer.data(), buffer.size()));
            } catch (const std::exception&) {
                n = 0;
            }
            if (n == 0) break;
            std::lock_guard hold(shared->lock);
            RunState& s = shared->state;
            for (std::size_t i = 0; i < n; ++i) {
                const char c = static_cast<char>(buffer[i]);
                if (c == '\r') continue;
                s.output += c;
                if (c == '\n') {
                    if (const int p = percent_of_line(shared->partial_line); p >= 0) s.percent = p;
                    shared->partial_line.clear();
                } else {
                    shared->partial_line += c;
                }
            }
            if (s.output.size() > kTrRetained) s.output.erase(0, s.output.size() - kTrRetained);
            ++s.version;
        }
        std::optional<std::uint32_t> code;
        try {
            code = shared->child->wait();
        } catch (const std::exception&) {
        }
        std::lock_guard hold(shared->lock);
        shared->state.running = false;
        shared->state.exit_code = code ? static_cast<int>(*code) : 1;
        if (shared->state.exit_code == 0 && shared->state.percent >= 0) shared->state.percent = 100;
        ++shared->state.version;
    });
}

Run::~Run() {
    if (shared_ && reader_.joinable()) {
        stop();
        reader_.join();
    }
}

RunState Run::state() const {
    std::lock_guard hold(shared_->lock);
    return shared_->state;
}

void Run::stop() {
    std::lock_guard hold(shared_->lock);
    if (!shared_->state.running || !shared_->child) return;
    shared_->state.stopped = true;
    shared_->child->kill();   // (this child's job only)
    ++shared_->state.version;
}

}  // namespace cb::app::tools
