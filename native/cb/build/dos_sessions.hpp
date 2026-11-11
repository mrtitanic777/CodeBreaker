// cb/build/dos_sessions.hpp -- a POOL of persistent, warm DOSBox+MASM sessions.
//
// The DOSBox BOOT (~1.5-2 s) is the dominant per-assemble cost. `masmlift` amortised it by batching many
// functions into one boot; this eliminates the RE-boot entirely: each worker boots DOSBox ONCE and then loops,
// assembling batch after batch without restarting. A batch that took a fresh boot now costs ~0.3 s.
//
// How the warm session works (validated):
//   * a SINGLE small mount (C:) holds MASM.EXE + the work files -- one small mount so DOSBox's `RESCAN` reliably
//     sees host-created files (a big mount does not);
//   * a DOS dispatch loop polls: `rescan` -> if C:\GO exists -> `call C:\WORK.BAT` (the MASM commands the host
//     just wrote) -> write C:\ACK -> loop;
//   * the host drops the .ASM + WORK.BAT + GO, waits for ACK, reads the .OBJ/.LST.
//
// A pool hands out warm sessions to the conductor's fan-out (one session per concurrent chunk). Falls back
// cleanly: a dead session is dropped and a fresh one boots. The DOSBox process is a cb::process::Process, so it
// lives in a kill-on-close Job Object: closing a session (or ending ours) ends exactly its DOSBox.
#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <semaphore>
#include <span>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"

namespace cb::build::dos_sessions {

// WAIT.COM: BIOS wait (INT 15h/86h) ~50ms, then exit. A bounded busy-wait -- used to build a short LOW-LATENCY
// response window before the worker falls back to the CPU-cheap (but 1s-coarse) CHOICE throttle.
// mov ah,86h; cx=0; dx=C350h; int 15h; int 20h
inline constexpr std::array<std::uint8_t, 12> kWaitCom{0xB4, 0x86, 0xB9, 0x00, 0x00, 0xBA, 0x50, 0xC3, 0xCD, 0x15, 0xCD, 0x20};

// WORKER.BAT: a hybrid idle loop: a few fast `wait`+rescan checks (~200ms window that catches a back-to-back next
// batch almost instantly), and only THEN a 1s CHOICE keyboard-wait that DOSBox idle-throttles to ~7% of a core. A
// bare `goto loop` busy-spins a full core (a pool of those pegs the machine and STARVES MASM); a pure 1s CHOICE
// throttles but adds ~1s latency to every assemble. The hybrid gives both: low latency when busy, low CPU when
// genuinely idle. CRLF line ends, as DOS reads them.
std::string worker_script();

// dbx.conf for a session whose work folder is `work` (mounted as C:).
std::string dosbox_config(const std::filesystem::path& work);

// One source to assemble: `stem` names the DOS files (STEM.ASM -> STEM.OBJ, STEM.LST, STEM.LOG): 1-8 letters,
// digits, underscore, dollar sign.
struct Source {
    std::string stem;
    std::string text;
};

// The files an assembly produced; each is present only when the file exists.
struct Product {
    std::optional<Bytes> obj;
    std::optional<std::string> lst;   // Latin-1 text, as UTF-8
    std::optional<std::string> log;
};

// What a batch answers.
struct Finished {
    std::vector<std::pair<std::string, Product>> results;   // in the order the sources were given
    std::filesystem::path work;                              // the session's work folder
};
struct Aborted {};   // the caller cancelled: the session was closed
struct Dead {};      // the session died or hung: drop it
using Outcome = std::variant<Finished, Aborted, Dead>;

// A warm session, as the pool sees it.
class Session {
public:
    virtual ~Session() = default;
    // Assemble the sources in this session. `timeout` defaults to 60 s + 3 s per source. `cancelled` is polled
    // while waiting.
    virtual Outcome assemble_many(std::span<const Source> items, std::optional<std::chrono::milliseconds> timeout = std::nullopt,
                                  const std::function<bool()>& cancelled = nullptr) = 0;
    virtual bool alive() = 0;
    virtual void close() = 0;
};

// One persistent DOSBox session running the MASM dispatch loop.
class DosboxWorker final : public Session {
public:
    // Boots DOSBox and waits for its dispatch loop. ToolError when DOSBox cannot start, dies while booting, or does not
    // come up within `boot_timeout`. `masm_root` holds BIN/MASM.EXE.
    DosboxWorker(const std::filesystem::path& masm_root, const std::filesystem::path& dosbox,
                 std::chrono::milliseconds boot_timeout = std::chrono::seconds(20));
    ~DosboxWorker() override;
    DosboxWorker(const DosboxWorker&) = delete;
    DosboxWorker& operator=(const DosboxWorker&) = delete;

    Outcome assemble_many(std::span<const Source> items, std::optional<std::chrono::milliseconds> timeout = std::nullopt,
                          const std::function<bool()>& cancelled = nullptr) override;
    bool alive() override;
    void close() override;
    const std::filesystem::path& work() const { return work_->path(); }

private:
    std::filesystem::path path(const std::string& name) const { return work_->path() / name; }
    void wait_boot(std::chrono::milliseconds timeout);

    std::unique_ptr<fs::ScopedTempDir> work_;      // gone with the session
    std::unique_ptr<process::Process> process_;
};

// Thread-safe pool of warm sessions, capped at `max_sessions` leased at once.
class WorkerPool {
public:
    using Factory = std::function<std::shared_ptr<Session>()>;

    // `make` boots one session (throwing when it cannot).
    WorkerPool(Factory make, std::size_t max_sessions);
    // A pool of DosboxWorkers over `masm_root` and `dosbox`.
    static std::unique_ptr<WorkerPool> for_masm(const std::filesystem::path& masm_root, const std::filesystem::path& dosbox,
                                                std::size_t max_sessions);

    // A session: an idle one, or a new one if the cap allows, else waits for a release.
    std::shared_ptr<Session> lease();
    // Give a session back. It is kept for the next lease when `keep` and still alive, else closed and dropped.
    void release(const std::shared_ptr<Session>& session, bool keep = true);
    // Close every session.
    void close();

private:
    Factory make_;
    std::counting_semaphore<> slots_;
    std::mutex lock_;
    std::vector<std::shared_ptr<Session>> idle_;
    std::vector<std::shared_ptr<Session>> all_;
};

}  // namespace cb::build::dos_sessions
