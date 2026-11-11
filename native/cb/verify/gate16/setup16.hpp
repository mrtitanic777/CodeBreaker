// cb/verify/gate16/setup16.hpp -- does Windows 95 Setup run with these modules? The real system's test of a rebuilt
// module.
//
// Setup's own CD is copied with WIN95\MINI.CAB rebuilt -- the original's files, with the ones given in their place --
// and booted in QEMU from a disk Setup has already prepared and a floppy that starts `SETUP /IS`. At the Welcome
// dialog Enter is pressed (Continue); Setup then shows its License Agreement page. That page on the screen is a pass;
// Windows back at the DOS prompt, an error box, or Windows' screen with nothing on it is a fail.
//
// Everything a run copies (the CD, the disk -- 650 MB) is made in a folder of its own (cb/base/runs.hpp:
// <state>/phase3/runs/) and gone when the run ends -- a killed run's, by the next run once it is a day old; only the
// last screen of each tag is kept (<state>/phase3/shots/<tag>.png), for whoever looks at a failure.
//
//     Config config = Config::in_state(state, qemu_exe, setup_iso);
//     Outcome o = continue_test(config, {{"USER.EXE", "built/user.exe"}}, "user-1");
//     if (o.verdict == Verdict::Pass) ...
//
// The machine is QEMU's, driven through its monitor on a TCP port of this computer (`Monitor`): `screendump` for what is
// on the screen, `sendkey` for the Enter. What the screen shows is told by a few pixels and by the share of white and
// black in a shrunk copy of it (`is_welcome`, `is_license`, `is_error_box`, `is_black`).
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "cb/verify/gate16/screen.hpp"

namespace cb::verify::gate16::setup16 {

// ---- what the run says -----------------------------------------------------------------------------------------------

enum class Verdict {
    Pass,                       // the License Agreement page is on the screen
    FailDos,                    // Windows went back to DOS (a 720-wide text screen, or a black one)
    FailHang,                   // Windows' screen, and nothing happened
    FailErrorBox,               // an error box, twice in a row, after Continue
    FailTimeout,                // after Continue, nothing the gate knows came up in time
    FailErrorBoxBeforeWelcome,  // an error box before the Welcome dialog (it is white like the Welcome dialog: not it)
    Slow,                       // Setup's DOS part never finished: the run says nothing
};

// "PASS", "FAIL(dos)", "FAIL(hang)", "FAIL(error box)", "FAIL(timeout)", "FAIL(error box before Welcome)", "SLOW".
std::string_view word(Verdict v);

struct Outcome {
    Verdict verdict = Verdict::Slow;
    std::filesystem::path screenshot;   // the last screen of the run (the file may not exist if QEMU never answered)
};

// ---- what is on the screen -------------------------------------------------------------------------------------------

// The License Agreement page: a 640-wide screen with the dialog's grey at three points.
bool is_license(const Picture& screen);
// An error box: a 640-wide screen that is white at three points where the box's window stands.
bool is_error_box(const Picture& screen);
// How many of the 160x120 shrunk screen's pixels are white (all three channels above 200).
unsigned white_pixels(const Picture& screen);
// The Welcome dialog: a 640-wide screen that is mostly white (more than 2000 of those pixels).
bool is_welcome(const Picture& screen);
// A black screen: more than 90% of the 90x50 shrunk screen's pixels are black (the three channels sum under 60).
bool is_black(const Picture& screen);

// ---- QEMU's monitor ----------------------------------------------------------------------------------------------------

// A TCP port of this computer nothing is listening on, for a monitor to be started on.
std::uint16_t free_port();

class Monitor {
public:
    struct Pacing {
        std::chrono::milliseconds after_command{300};      // QEMU needs a moment to take a command
        std::chrono::milliseconds after_screendump{1200};  // and to write a screen
        std::chrono::milliseconds connect_timeout{5000};
        std::chrono::milliseconds banner_timeout{2000};    // how long to listen for what the monitor says first
    };

    explicit Monitor(std::uint16_t port, Pacing pacing = {}) : port_(port), pacing_(pacing) {}
    std::uint16_t port() const { return port_; }

    // Connect, hear what the monitor says first, send each command (a line) and wait after it. False when the monitor
    // cannot be reached or a command cannot be sent: QEMU is not listening yet, or is gone.
    bool send(std::span<const std::string> commands) const;
    // The screen: `screendump` into a PPM beside `png`, read it, save it as `png`, remove the PPM. Nothing when no
    // usable picture came (the monitor did not answer, the file was not there or not all there yet).
    std::optional<Picture> shot(const std::filesystem::path& png) const;

private:
    std::uint16_t port_;
    Pacing pacing_;
};

// ---- the machine -----------------------------------------------------------------------------------------------------

// Where the pieces are. Everything is under the state folder but the emulator and the Setup CD.
struct Config {
    std::filesystem::path state;        // runs and screenshots go under <state>/phase3
    std::filesystem::path qemu;         // qemu-system-i386
    std::filesystem::path setup_iso;    // Windows 95's Setup CD
    std::filesystem::path mini_dir;     // <state>/mini: original/ (MINI.CAB's files), MINI.CAB, prepared_hd.img
    std::filesystem::path boot_floppy;  // <state>/bootfloppy/boot95_is.img: starts `SETUP /IS`

    // The usual places under `state`.
    static Config in_state(const std::filesystem::path& state, std::filesystem::path qemu, std::filesystem::path setup_iso);
    std::filesystem::path original() const { return mini_dir / "original"; }
    std::filesystem::path cabinet() const { return mini_dir / "MINI.CAB"; }
    std::filesystem::path prepared_disk() const { return mini_dir / "prepared_hd.img"; }
};

// The files to put in place of Setup's own, by name (case does not matter): MINI.CAB's modules, or SUWIN, SETUPX and
// the others of PRECOPY2.CAB, or SETUP.EXE beside the cabinets.
using Replace = std::map<std::string, std::filesystem::path>;

// Setup's CD at `out`, with MINI.CAB rebuilt from the original's files, `replace` in place of some. The rest of
// `replace` -- files of the other cabinets, and SETUP.EXE loose beside them -- goes in place on the CD too: a loose file
// replaced, a file in a cabinet by the cabinet written again, with the whole set it belongs to (PRECOPY1's last three
// files run on into PRECOPY2, and PRECOPY2 rewritten alone left PRECOPY1 pointing into a cabinet that no longer
// continued them: "Could not decompress this Setup (.CAB) file"). Each file is whole in the cabinet it starts in.
void make_cd(const Config& config, const std::filesystem::path& out, const Replace& replace);

struct Timing {
    int welcome_polls = 60;                              // looks at the screen for the Welcome dialog
    std::chrono::milliseconds welcome_poll{4000};
    std::chrono::milliseconds welcome_settle{8000};      // after the dialog shows, before Enter
    std::chrono::milliseconds continue_poll{3000};
    std::chrono::seconds wait{150};                      // after Enter, to the License page
    Monitor::Pacing monitor;
};

struct Options {
    Timing timing;
    std::optional<std::filesystem::path> keep_shot;      // the screenshot's folder (default: <state>/phase3/shots)
    // What makes the CD (default: make_cd above): a test puts a small one here.
    std::function<void(const std::filesystem::path& out, const Replace& replace)> make_cd;
};

// Boot Setup with `replace` and press Continue.
Outcome continue_test(const Config& config, const Replace& replace, std::string_view tag, const Options& options = {});

}  // namespace cb::verify::gate16::setup16
