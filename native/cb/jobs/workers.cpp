#include "cb/jobs/workers.hpp"

#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/core/win.hpp"

#include <windows.h>

namespace cb::jobs {

namespace {

constexpr const char* kTokenReader = "worker tokens";

std::filesystem::path handover_base() {
    std::filesystem::path base = std::filesystem::temp_directory_path() / "codebreaker_workers";
    fs::make_dirs(base);
    return base;
}

}  // namespace

// ---- kinds and the worker side ---------------------------------------------------------------------------------
void WorkerKinds::add(std::string kind, WorkerMain run) {
    if (find(kind) != nullptr) throw UsageError(std::format("worker kind '{}' is registered twice", kind));
    kinds_.emplace_back(std::move(kind), std::move(run));
}

const WorkerMain* WorkerKinds::find(std::string_view kind) const {
    for (const auto& [name, run] : kinds_)
        if (name == kind) return &run;
    return nullptr;
}

std::optional<int> run_as_worker(const WorkerKinds& kinds, const Args& argv) {
    if (argv.size() < 3 || argv[1] != kWorkerFlag) return std::nullopt;
    const std::string& kind = argv[2];
    const WorkerMain* run = kinds.find(kind);
    if (run == nullptr) {
        log::error(std::format("worker {}: no such kind", kind));
        return kWorkerUnknownKind;
    }
    // The isolation boundary of a worker's job loop: whatever the share throws is its exit code, said on stderr.
    try {
        return (*run)(Args(argv.begin() + 3, argv.end()));
    } catch (const std::exception& e) {
        log::error(std::format("worker {}: {}", kind, e.what()));
        return kWorkerFailed;
    }
}

std::optional<int> run_as_worker(const WorkerKinds& kinds, int argc, char** argv) {
    return run_as_worker(kinds, Args(argv, argv + argc));
}

// ---- the parent side -----------------------------------------------------------------------------------------------
WorkerLauncher::WorkerLauncher(std::filesystem::path exe) : exe_(std::move(exe)) {
    if (fs::get_env("CB_NO_WORKERS")) exe_.clear();
}

WorkerLauncher WorkerLauncher::for_this_program() {
    std::wstring buf(MAX_PATH, L'\0');
    for (;;) {
        const DWORD n = GetModuleFileNameW(nullptr, buf.data(), static_cast<DWORD>(buf.size()));
        if (n == 0) return WorkerLauncher({});
        if (n < buf.size()) {
            buf.resize(n);
            return WorkerLauncher(std::filesystem::path(buf));
        }
        buf.resize(buf.size() * 2);   // truncated: a long path
    }
}

std::vector<std::string> WorkerLauncher::command(std::string_view kind, const Args& args) const {
    std::vector<std::string> argv{fs::to_text(exe_), std::string(kWorkerFlag), std::string(kind)};
    argv.insert(argv.end(), args.begin(), args.end());
    return argv;
}

WorkerResult WorkerLauncher::run(std::string_view kind, const Args& args, const LaunchOptions& options) const {
    if (!enabled()) throw UsageError("worker processes are not enabled in this program");
    process::ProcessSpec spec;
    spec.argv = command(kind, args);
    spec.out = process::Capture::Discard;
    spec.err = process::Capture::Inherit;
    spec.timeout = options.timeout;
    spec.priority = options.priority;
    const process::ProcessResult r = process::run(spec);
    return {r.exit_code, r.timed_out};
}

// ---- the files' text --------------------------------------------------------------------------------------------------
void TokenWriter::token(std::string_view t) {
    if (!out_.empty()) out_ += ' ';
    out_ += t;
}

TokenWriter& TokenWriter::integer(std::int64_t v) {
    token(std::to_string(v));
    return *this;
}

TokenWriter& TokenWriter::bytes(ByteView v) {
    token(v.empty() ? std::string("-") : crypto::to_hex(v));
    return *this;
}

TokenWriter& TokenWriter::text(std::string_view v) { return bytes(as_bytes(v)); }

TokenReader::TokenReader(std::string_view text) {
    std::size_t at = 0;
    while (at < text.size()) {
        while (at < text.size() && (text[at] == ' ' || text[at] == '\n' || text[at] == '\r' || text[at] == '\t')) ++at;
        std::size_t end = at;
        while (end < text.size() && text[end] != ' ' && text[end] != '\n' && text[end] != '\r' && text[end] != '\t') ++end;
        if (end > at) tokens_.emplace_back(text.substr(at, end - at));
        at = end;
    }
}

std::string_view TokenReader::next(const char* what) {
    if (at_ >= tokens_.size()) throw FormatError(kTokenReader, at_, std::format("the tokens ended where {} was expected", what));
    return tokens_[at_++];
}

std::int64_t TokenReader::integer() {
    const std::size_t index = at_;
    const std::string_view t = next("an integer");
    const std::optional<std::int64_t> v = text::parse_int<std::int64_t>(t);
    if (!v) throw FormatError(kTokenReader, index, std::format("'{}' is not an integer", t));
    return *v;
}

Bytes TokenReader::bytes() {
    const std::size_t index = at_;
    const std::string_view t = next("bytes");
    if (t == "-") return {};
    try {
        return crypto::from_hex(t);
    } catch (const FormatError&) {
        throw FormatError(kTokenReader, index, std::format("'{}' is not hex", t));
    }
}

std::string TokenReader::text() {
    const Bytes b = bytes();
    return std::string(as_text(b));
}

// ---- the hand-over folder -----------------------------------------------------------------------------------------
Handover::Handover(std::string_view purpose) : dir_(purpose, handover_base()) {}

void Handover::write(std::string_view name, std::string_view text) const { fs::write_text(file(name), text); }
std::string Handover::read(std::string_view name) const { return fs::read_text(file(name)); }

}  // namespace cb::jobs
