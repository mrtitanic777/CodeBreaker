#include "cb/verify/gate16/setup16.hpp"

#include <algorithm>
#include <deque>
#include <format>
#include <set>
#include <thread>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "cb/base/runs.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/cab.hpp"
#include "cb/formats/iso9660.hpp"

namespace cb::verify::gate16::setup16 {
namespace {

namespace stdfs = std::filesystem;

// The 16261 the Python's Setup tooling gave a rebuilt MINI.CAB (a cabinet set's identity: any number does, the file's
// own cabinet is the whole set).
constexpr std::uint16_t kMiniSetId = 16261;
constexpr std::string_view kCdFolder = "/WIN95";

// The License page's grey, and the three points of the page that show it.
constexpr Rgb kLicenseGrey{192, 199, 200};
constexpr std::pair<std::uint32_t, std::uint32_t> kLicensePoints[] = {{60, 350}, {580, 350}, {300, 395}};
// An error box is white where its window stands.
constexpr std::pair<std::uint32_t, std::uint32_t> kErrorBoxPoints[] = {{97, 310}, {320, 175}, {540, 310}};
constexpr std::uint32_t kScreenWidth = 640;      // Windows' graphics screen
constexpr std::uint32_t kTextScreenWidth = 720;  // DOS's text screen

std::string upper(std::string_view s) { return text::upper(s); }

// ---- Winsock ------------------------------------------------------------------------------------------------------------

// The process-wide start of Winsock (the OS's reference count; it is never stopped).
void winsock_ready() {
    static const bool started = [] {
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d) != 0)
            throw Error("Winsock could not be started");
        return true;
    }();
    (void)started;
}

class Socket {
public:
    Socket() {
        winsock_ready();
        s_ = ::socket(AF_INET, SOCK_STREAM, 0);
    }
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    ~Socket() {
        if (s_ != INVALID_SOCKET)
            ::closesocket(s_);
    }
    SOCKET get() const { return s_; }
    bool valid() const { return s_ != INVALID_SOCKET; }

    // Connect to 127.0.0.1:port within `timeout`.
    bool connect_local(std::uint16_t port, std::chrono::milliseconds timeout) {
        if (!valid())
            return false;
        u_long nonblocking = 1;
        ioctlsocket(s_, FIONBIO, &nonblocking);
        sockaddr_in a{};
        a.sin_family = AF_INET;
        a.sin_port = htons(port);
        inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
        bool connected = ::connect(s_, reinterpret_cast<sockaddr*>(&a), sizeof a) == 0;
        if (!connected && WSAGetLastError() == WSAEWOULDBLOCK) {
            fd_set writable, failed;
            FD_ZERO(&writable);
            FD_ZERO(&failed);
            FD_SET(s_, &writable);
            FD_SET(s_, &failed);
            const auto ms = timeout.count();
            timeval tv{static_cast<long>(ms / 1000), static_cast<long>((ms % 1000) * 1000)};
            connected = ::select(0, nullptr, &writable, &failed, &tv) > 0 && FD_ISSET(s_, &writable);
        }
        nonblocking = 0;
        ioctlsocket(s_, FIONBIO, &nonblocking);
        return connected;
    }

    void set_receive_timeout(std::chrono::milliseconds timeout) {
        const DWORD ms = static_cast<DWORD>(timeout.count());
        setsockopt(s_, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
    }

    bool send_all(std::string_view data) {
        while (!data.empty()) {
            const int n = ::send(s_, data.data(), static_cast<int>(data.size()), 0);
            if (n <= 0)
                return false;
            data.remove_prefix(static_cast<std::size_t>(n));
        }
        return true;
    }

private:
    SOCKET s_ = INVALID_SOCKET;
};

// Copy a file over another; the errors are ours.
void copy_over(const stdfs::path& from, const stdfs::path& to) {
    std::error_code ec;
    stdfs::copy_file(from, to, stdfs::copy_options::overwrite_existing, ec);
    if (!ec)
        return;
    if (ec == std::errc::no_such_file_or_directory)
        throw NotFoundError(from, ec.value());
    throw IoError(from, ec.value(), "cannot be copied to " + path_text(to) + ": " + ec.message());
}

// A path as QEMU's option parser takes it: a comma is doubled (a comma ends an option's value).
std::string option_path(const stdfs::path& p) { return text::replace_all(fs::to_text(p), ",", ",,"); }

}  // namespace

std::string_view word(Verdict v) {
    switch (v) {
    case Verdict::Pass: return "PASS";
    case Verdict::FailDos: return "FAIL(dos)";
    case Verdict::FailHang: return "FAIL(hang)";
    case Verdict::FailErrorBox: return "FAIL(error box)";
    case Verdict::FailTimeout: return "FAIL(timeout)";
    case Verdict::FailErrorBoxBeforeWelcome: return "FAIL(error box before Welcome)";
    case Verdict::Slow: return "SLOW";
    }
    return "SLOW";
}

// ---- what is on the screen -------------------------------------------------------------------------------------------

bool is_license(const Picture& screen) {
    if (screen.width() != kScreenWidth)
        return false;
    // (a point the picture does not have -- a short screen -- is not the page's grey)
    return std::all_of(std::begin(kLicensePoints), std::end(kLicensePoints),
                       [&](const auto& xy) { return screen.at(xy.first, xy.second) == std::optional<Rgb>(kLicenseGrey); });
}

bool is_error_box(const Picture& screen) {
    if (screen.width() != kScreenWidth)
        return false;
    return std::all_of(std::begin(kErrorBoxPoints), std::end(kErrorBoxPoints), [&](const auto& xy) {
        const std::optional<Rgb> p = screen.at(xy.first, xy.second);
        return p && p->sum() > 700;
    });
}

unsigned white_pixels(const Picture& screen) {
    const Picture small = screen.resized(160, 120);
    unsigned n = 0;
    const Bytes& px = small.rgb();
    for (std::size_t i = 0; i + 2 < px.size(); i += 3)
        n += px[i] > 200 && px[i + 1] > 200 && px[i + 2] > 200;
    return n;
}

bool is_welcome(const Picture& screen) { return screen.width() == kScreenWidth && white_pixels(screen) > 2000; }

bool is_black(const Picture& screen) {
    const Picture small = screen.resized(90, 50);
    const Bytes& px = small.rgb();
    unsigned dark = 0;
    for (std::size_t i = 0; i + 2 < px.size(); i += 3)
        dark += unsigned{px[i]} + px[i + 1] + px[i + 2] < 60;
    return static_cast<double>(dark) / (90.0 * 50.0) > 0.9;
}

// ---- QEMU's monitor ----------------------------------------------------------------------------------------------------

std::uint16_t free_port() {
    Socket s;
    if (!s.valid())
        throw Error("a socket could not be made");
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = 0;
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    int size = sizeof a;
    if (::bind(s.get(), reinterpret_cast<sockaddr*>(&a), sizeof a) != 0 ||
        ::getsockname(s.get(), reinterpret_cast<sockaddr*>(&a), &size) != 0)
        throw Error("no local port is free");
    return ntohs(a.sin_port);
}

bool Monitor::send(std::span<const std::string> commands) const {
    Socket s;
    if (!s.connect_local(port_, pacing_.connect_timeout))
        return false;
    s.set_receive_timeout(pacing_.banner_timeout);
    char banner[4096];
    (void)::recv(s.get(), banner, sizeof banner, 0);   // (what the monitor says first; hearing nothing is no error)
    for (const std::string& c : commands) {
        if (!s.send_all(c + "\n"))
            return false;
        std::this_thread::sleep_for(pacing_.after_command);
    }
    return true;
}

std::optional<Picture> Monitor::shot(const stdfs::path& png) const {
    stdfs::path ppm = png;
    ppm.replace_extension(".ppm");
    const std::string command = "screendump " + text::replace_all(fs::to_text(ppm), "\\", "/");
    if (!send(std::span<const std::string>(&command, 1)))
        return std::nullopt;
    std::this_thread::sleep_for(pacing_.after_screendump);
    if (!stdfs::is_regular_file(ppm))
        return std::nullopt;
    std::optional<Picture> screen;
    try {
        screen = Picture::try_from_ppm(fs::read_file(ppm));   // (QEMU may still hold the file: the next look will do)
    } catch (const IoError&) {
        return std::nullopt;
    }
    if (!screen)
        return std::nullopt;
    fs::write_file(png, screen->to_png());
    std::error_code ignored;
    stdfs::remove(ppm, ignored);
    return screen;
}

// ---- the CD ---------------------------------------------------------------------------------------------------------

Config Config::in_state(const stdfs::path& state, stdfs::path qemu, stdfs::path setup_iso) {
    Config c;
    c.state = state;
    c.qemu = std::move(qemu);
    c.setup_iso = std::move(setup_iso);
    c.mini_dir = state / "mini";
    c.boot_floppy = state / "bootfloppy" / "boot95_is.img";
    return c;
}

namespace {

namespace cab = formats::cab;
namespace iso = formats::iso9660;

// The names in `replace` upper-cased (a name is compared without regard to case).
Replace upper_names(const Replace& replace) {
    Replace out;
    for (const auto& [name, path] : replace)
        out[upper(name)] = path;
    return out;
}

// A cabinet of the files, in one folder, MSZIP, carrying the fields that tie it into its set.
Bytes write_cabinet(const std::vector<std::pair<std::string, Bytes>>& files, const cab::WriteOptions& options) {
    std::vector<cab::SourceFile> sources;
    sources.reserve(files.size());
    for (const auto& [name, data] : files)
        sources.push_back({name, data});
    return cab::build(sources, options);
}

// Put the files of `rest` (upper-case names) in place on the open CD under /WIN95: a loose file replaced, a file in a
// cabinet by the cabinet written again.
void replace_on_cd(iso::Volume& cd, const Replace& rest, const stdfs::path& out) {
    fs::ScopedTempDir work("cd", out.parent_path().empty() ? stdfs::path(".") : out.parent_path());   // (no copies left behind)
    std::set<std::string> loose;                         // every name in /WIN95
    for (const iso::Entry& e : cd.list(kCdFolder))
        if (!e.directory)
            loose.insert(upper(e.name));
    // every cabinet, beside the others: a file one cabinet continues into the next is read from both
    std::map<std::string, cab::Cabinet> cabs;
    for (const std::string& name : loose)
        if (name.ends_with(".CAB")) {
            fs::write_file(work.path() / name, cd.read(std::format("{}/{}", kCdFolder, name)));
            cabs.emplace(name, cab::read(work.path() / name));
        }
    std::map<std::string, Bytes> replacements;           // /WIN95 name -> what to put there (sorted: replaced in order)
    for (const auto& [name, path] : rest)
        if (loose.contains(name))
            replacements[name] = fs::read_file(path);    // (a loose file: SETUP.EXE)
    // A cabinet holding one of ours is written again -- with the whole set it belongs to: PRECOPY1's last three files run
    // on into PRECOPY2, and PRECOPY2 rewritten alone left PRECOPY1 pointing into a cabinet that no longer continued them
    // ("Could not decompress this Setup (.CAB) file"). Each file is now whole in the cabinet it starts in.
    std::set<std::string> todo;
    for (const auto& [name, c] : cabs) {
        const bool holds_one = std::any_of(c.files.begin(), c.files.end(), [&](const cab::FileEntry& e) { return rest.contains(upper(e.name)); });
        if (!holds_one)
            continue;
        std::string k = name;
        std::set<std::string> walked;                    // (a set that names itself: stop)
        while (cabs.contains(k) && cabs.at(k).prev && walked.insert(k).second)
            k = upper(cabs.at(k).prev->cabinet);
        walked.clear();
        while (cabs.contains(k) && walked.insert(k).second) {
            todo.insert(k);
            k = cabs.at(k).next ? upper(cabs.at(k).next->cabinet) : std::string();
        }
    }
    for (const std::string& name : todo) {
        const cab::Cabinet& c = cabs.at(name);
        std::map<std::string, stdfs::path> got;          // every file of the cabinet, extracted (continued ones from its neighbours)
        for (const cab::Extracted& e : cab::extract(work.path() / name, work.path() / (name + ".d")))
            got[e.name] = e.path;
        std::vector<std::pair<std::string, Bytes>> files;
        for (const cab::FileEntry& e : c.files) {
            if (e.folder == cab::kFromPrev || e.folder == cab::kPrevAndNext)
                continue;                                // (continued: in the one before)
            const auto ours = rest.find(upper(e.name));
            files.emplace_back(e.name, fs::read_file(ours != rest.end() ? ours->second : got.at(e.name)));
        }
        cab::WriteOptions options;
        options.set_id = c.set_id;
        options.index = c.index;
        options.prev = c.prev;
        options.next = c.next;
        replacements[name] = write_cabinet(files, options);
    }
    for (const auto& [name, data] : replacements)
        cd.replace(std::format("{}/{}", kCdFolder, name), data);
}

}  // namespace

void make_cd(const Config& config, const stdfs::path& out, const Replace& replace) {
    const Replace up = upper_names(replace);
    const cab::Cabinet mini = cab::read(config.cabinet());
    std::vector<std::pair<std::string, Bytes>> files;
    std::set<std::string> in_mini;
    for (const cab::FileEntry& e : mini.files) {
        const auto ours = up.find(upper(e.name));
        files.emplace_back(e.name, fs::read_file(ours != up.end() ? ours->second : config.original() / e.name));
        in_mini.insert(upper(e.name));
    }
    cab::WriteOptions options;
    options.set_id = kMiniSetId;
    const Bytes cabinet = write_cabinet(files, options);

    copy_over(config.setup_iso, out);
    iso::Volume cd(out, iso::Access::ReadWrite);
    cd.replace(std::format("{}/MINI.CAB", kCdFolder), cabinet);
    // The rest of Setup's own programs -- SUWIN, SETUPX and the others, in PRECOPY2.CAB, and SETUP.EXE loose beside the
    // cabinets -- rebuilt from ours where `replace` names them.
    Replace rest;
    for (const auto& [name, path] : up)
        if (!in_mini.contains(name))
            rest.emplace(name, path);
    if (!rest.empty())
        replace_on_cd(cd, rest, out);
}

// ---- the test --------------------------------------------------------------------------------------------------------

Outcome continue_test(const Config& config, const Replace& replace, std::string_view tag, const Options& options) {
    const Timing& timing = options.timing;
    const stdfs::path shots = options.keep_shot ? *options.keep_shot : base::runs::results(config.state, "phase3");
    fs::make_dirs(shots);
    Outcome outcome;
    outcome.screenshot = shots / (std::string(tag) + ".png");
    const stdfs::path& png = outcome.screenshot;

    // The disk and the CD are copies, in this run's folder alone; the folder is gone however the run ends, after the
    // machine (declared after it) has let go of them.
    base::runs::Folder work(config.state, "phase3", tag);
    const stdfs::path cd = work.path() / "cd.iso";
    if (options.make_cd)
        options.make_cd(cd, replace);
    else
        make_cd(config, cd, replace);
    const stdfs::path disk = work.path() / "hd.img";
    copy_over(config.prepared_disk(), disk);

    const Monitor monitor(free_port(), timing.monitor);
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(config.qemu), "-M", "pc", "-m", "64",
                 "-drive", std::format("file={},format=raw,if=floppy,index=0,snapshot=on", option_path(config.boot_floppy)),
                 "-drive", std::format("file={},format=raw,if=ide,index=0", option_path(disk)),
                 "-cdrom", fs::to_text(cd), "-boot", "a", "-display", "none",
                 "-monitor", std::format("tcp:127.0.0.1:{},server,nowait", monitor.port())};
    spec.out = process::Capture::Discard;
    spec.err = process::Capture::Discard;
    process::Process machine = process::start(spec);

    Verdict verdict = Verdict::Slow;
    std::optional<Picture> screen;
    bool concluded = false;
    const std::string enter = "sendkey ret";
    for (int look = 0; look < timing.welcome_polls && !concluded; ++look) {   // (to the Welcome dialog)
        std::this_thread::sleep_for(timing.welcome_poll);
        if (!machine.running()) {
            concluded = true;   // (QEMU ended by itself: nothing to say)
            break;
        }
        std::optional<Picture> now = monitor.shot(png);
        if (!now)
            continue;
        screen = std::move(now);
        if (is_error_box(*screen)) {   // (white like the Welcome dialog: not it)
            verdict = Verdict::FailErrorBoxBeforeWelcome;
            concluded = true;
        } else if (is_welcome(*screen)) {
            std::this_thread::sleep_for(timing.welcome_settle);
            // (a key that does not get there leaves the page unchanged: the wait below ends as a timeout)
            (void)monitor.send(std::span<const std::string>(&enter, 1));
            verdict = Verdict::FailTimeout;
            int bad = 0;
            const auto until = std::chrono::steady_clock::now() + timing.wait;
            while (std::chrono::steady_clock::now() < until) {
                std::this_thread::sleep_for(timing.continue_poll);
                std::optional<Picture> after = monitor.shot(png);
                if (!after)
                    continue;
                screen = std::move(after);
                if (screen->width() == kTextScreenWidth) {
                    verdict = Verdict::FailDos;
                    break;
                }
                if (is_license(*screen)) {
                    verdict = Verdict::Pass;
                    break;
                }
                bad = is_error_box(*screen) ? bad + 1 : 0;
                if (bad >= 2) {
                    verdict = Verdict::FailErrorBox;
                    break;
                }
            }
            concluded = true;
        }
        // (no keys while Setup's DOS part runs: it asks for none from a prepared disk, and an Enter it did not need reached
        // Windows and hid a fault -- USER's 1:007F)
    }
    if (!concluded && screen)
        verdict = is_black(*screen) ? Verdict::FailDos : screen->width() == kScreenWidth ? Verdict::FailHang : Verdict::Slow;

    machine.kill();
    (void)machine.wait(std::chrono::seconds(20));
    outcome.verdict = verdict;
    return outcome;
}

}  // namespace cb::verify::gate16::setup16
