#include "cb/build/dos_sessions.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::build::dos_sessions {

namespace stdfs = std::filesystem;
using namespace std::chrono_literals;

namespace {

constexpr std::string_view kPoll =
    "rescan >nul\r\n"
    "if exist C:\\GO goto work\r\n"
    "wait\r\n";

// A stem is a DOS file name's base: the batch file is built from it.
bool is_stem(std::string_view s) {
    return !s.empty() && s.size() <= 8 &&
           std::all_of(s.begin(), s.end(), [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '$'; });
}

// LF line ends become CRLF; a CRLF already there stays one.
std::string dos_text(std::string_view s) {
    return text::replace_all(text::replace_all(s, "\r\n", "\n"), "\n", "\r\n");
}

bool file_exists(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::exists(p, ec);
}

std::optional<Bytes> read_if_exists(const stdfs::path& p) {
    if (!file_exists(p)) return std::nullopt;
    return fs::read_file(p);
}

}  // namespace

std::string worker_script() {
    std::string s =
        "echo hi > C:\\BOOTED\r\n"
        ":loop\r\n";
    for (int i = 0; i < 4; ++i) s += kPoll;
    s +=
        "choice /C:YN /T:Y,1 /N >nul\r\n"
        "goto loop\r\n"
        ":work\r\n"
        "del C:\\GO\r\n"
        "rescan >nul\r\n"
        "call C:\\WORK.BAT\r\n"
        "echo done > C:\\ACK\r\n"
        "goto loop\r\n";
    return s;
}

std::string dosbox_config(const stdfs::path& work) {
    return std::format(
        "[sdl]\r\nautolock=false\r\n"
        "[cpu]\r\ncputype=386\r\ncore=normal\r\ncycles=max\r\n"
        "[dos]\r\nver=6.22\r\n"
        "[autoexec]\r\nmount c \"{}\"\r\nset TMP=C:\\\r\nc:\r\ncall WORKER.BAT\r\n",
        fs::to_text(work));
}

// ------------------------------------------------------------------------------------------ DosboxWorker

DosboxWorker::DosboxWorker(const stdfs::path& masm_root, const stdfs::path& dosbox, std::chrono::milliseconds boot_timeout)
    : work_(std::make_unique<fs::ScopedTempDir>("dosw")) {
    // Colocate ONLY MASM.EXE -- the reconstructor's asm uses no INCLUDE, so the mount stays tiny, which is what
    // makes DOSBox's RESCAN reliably see host-created trigger files (a large mount breaks that).
    stdfs::copy_file(masm_root / "BIN" / "MASM.EXE", path("MASM.EXE"));
    fs::write_file(path("WAIT.COM"), kWaitCom);
    fs::write_text(path("WORKER.BAT"), worker_script());
    const stdfs::path conf = path("dbx.conf");
    fs::write_text(conf, dosbox_config(work_->path()));
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(dosbox), "-conf", fs::to_text(conf), "-nolog"};
    spec.env["SDL_VIDEODRIVER"] = "dummy";
    spec.out = process::Capture::Discard;
    spec.err = process::Capture::Discard;
    process_ = std::make_unique<process::Process>(process::start(spec));
    wait_boot(boot_timeout);
}

DosboxWorker::~DosboxWorker() { close(); }

void DosboxWorker::wait_boot(std::chrono::milliseconds timeout) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        if (file_exists(path("BOOTED"))) return;
        if (!process_->running()) throw ToolError("dosbox", 1, "", "DOSBox worker died during boot");
        std::this_thread::sleep_for(50ms);
    }
    throw TimeoutError("dosbox", "DOSBox worker boot timed out");
}

Outcome DosboxWorker::assemble_many(std::span<const Source> items, std::optional<std::chrono::milliseconds> timeout,
                                    const std::function<bool()>& cancelled) {
    for (const Source& item : items) {
        if (!is_stem(item.stem)) throw UsageError(std::format("'{}' is not a DOS file stem (1-8 letters, digits, _ or $)", item.stem));
    }
    // clear the previous batch
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(work_->path(), ec)) {
        const std::string name = text::upper(fs::to_text(entry.path().filename()));
        if (name.ends_with(".ASM") || name.ends_with(".OBJ") || name.ends_with(".LST") || name.ends_with(".LOG") || name == "ACK" ||
            name == "GO" || name == "WORK.BAT") {
            std::error_code ignored;
            stdfs::remove(entry.path(), ignored);
        }
    }
    std::string commands;
    for (const Source& item : items) {
        fs::write_text(path(item.stem + ".ASM"), dos_text(item.text));
        commands += std::format("MASM {0}.ASM, {0}.OBJ, {0}.LST, NUL; > {0}.LOG\r\n", item.stem);
    }
    fs::write_text(path("WORK.BAT"), commands);
    fs::write_text(path("GO"), "x");   // the trigger: written LAST

    const std::chrono::milliseconds limit = timeout ? *timeout : std::chrono::seconds(60 + 3 * items.size());
    const auto deadline = std::chrono::steady_clock::now() + limit;
    bool acknowledged = false;
    while (std::chrono::steady_clock::now() < deadline) {
        if (file_exists(path("ACK"))) {
            acknowledged = true;
            break;
        }
        if (cancelled && cancelled()) {
            close();
            return Aborted{};
        }
        if (!process_->running()) return Dead{};
        std::this_thread::sleep_for(20ms);
    }
    if (!acknowledged) return Dead{};   // hung: treat as dead

    Finished done;
    done.work = work_->path();
    for (const Source& item : items) {
        Product p;
        p.obj = read_if_exists(path(item.stem + ".OBJ"));
        if (const auto lst = read_if_exists(path(item.stem + ".LST"))) p.lst = text::from_latin1(*lst);
        if (const auto log = read_if_exists(path(item.stem + ".LOG"))) p.log = text::from_latin1(*log);
        done.results.emplace_back(item.stem, std::move(p));
    }
    return done;
}

bool DosboxWorker::alive() { return process_ && process_->running(); }

void DosboxWorker::close() {
    if (process_) {
        process_->kill();
        process_->wait(3s);
        process_.reset();
    }
    work_.reset();
}

// ------------------------------------------------------------------------------------------ pool

WorkerPool::WorkerPool(Factory make, std::size_t max_sessions) : make_(std::move(make)), slots_(static_cast<std::ptrdiff_t>(max_sessions)) {}

std::unique_ptr<WorkerPool> WorkerPool::for_masm(const stdfs::path& masm_root, const stdfs::path& dosbox, std::size_t max_sessions) {
    return std::make_unique<WorkerPool>(
        [masm_root, dosbox] { return std::make_shared<DosboxWorker>(masm_root, dosbox); }, max_sessions);
}

std::shared_ptr<Session> WorkerPool::lease() {
    slots_.acquire();
    {
        std::lock_guard<std::mutex> hold(lock_);
        if (!idle_.empty()) {
            std::shared_ptr<Session> s = std::move(idle_.back());
            idle_.pop_back();
            return s;
        }
    }
    std::shared_ptr<Session> s;
    try {
        s = make_();
    } catch (...) {
        slots_.release();   // the slot was not used: give it back, then let the failure through
        throw;
    }
    std::lock_guard<std::mutex> hold(lock_);
    all_.push_back(s);
    return s;
}

void WorkerPool::release(const std::shared_ptr<Session>& session, bool keep) {
    {
        std::lock_guard<std::mutex> hold(lock_);
        const auto it = std::find(all_.begin(), all_.end(), session);
        if (keep && it != all_.end() && session->alive()) {
            idle_.push_back(session);
        } else {
            if (it != all_.end()) all_.erase(it);
            session->close();
        }
    }
    slots_.release();
}

void WorkerPool::close() {
    std::lock_guard<std::mutex> hold(lock_);
    for (const auto& s : all_) s->close();
    all_.clear();
    idle_.clear();
}

}  // namespace cb::build::dos_sessions
