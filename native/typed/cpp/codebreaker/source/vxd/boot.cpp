// codebreaker/source/vxd/boot.cpp -- typed C++ of codebreaker.source.vxd.boot.

#include <winsock2.h>
#include <ws2tcpip.h>

#include "codebreaker/source/vxd/boot.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/base/runs.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/formats/media.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"
#include "codebreaker/source/vxd/le_writer.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"
#include "py_zlib.hpp"

#pragma comment(lib, "ws2_32.lib")

namespace codebreaker::source::vxd::boot {

namespace le = codebreaker::formats::le;
namespace media = codebreaker::formats::media;

const py::str DEFAULT_DISK("\\\\wsl$\\Ubuntu\\home\\mrtit\\win95_install.img");
const py::str DOC(R"BOOTDOC(boot.py -- does Windows 95 start with these drivers? A rebuilt driver's real test.

    python -m codebreaker vxd boot [--drivers NAME,...] [--vmm32] [--retail] [--bisect]
                                   [--disk IMAGE] [--wait SECONDS] [--tag NAME]

Each driver named (vtd, vpicd, drvspacx ...: a file of the corpus) is built from the recovered
disassembly with le_writer.build_le, and with --vmm32 Windows' own VMM32.VXD with build_w3; they
are put into a copy of an installed Windows 95 disk, and the copy is booted in QEMU with no
window. Windows reaching its desktop (or a logon box over it) is a pass; a blue screen, a screen
that stays black or in text (a "Windows protection error" is one), or no desktop in --wait
seconds (default 300) is a fail. --retail puts the corpus originals in instead of the rebuilt
files: the baseline a rebuilt run is measured against. --bisect, when the run fails, boots
halves of the set until it finds the drivers that fail by themselves.

Where each file goes: a driver kept in WINDOWS\SYSTEM of the corpus goes into
WINDOWS\SYSTEM\VMM32 -- Windows loads a driver found there in place of the copy inside VMM32.VXD
-- and one kept in a folder below it (IOSUBSYS) or with another extension (.386) where the corpus
has it. VMM32.VXD goes into WINDOWS\SYSTEM. The corpus VMM32.VXD holds VMM alone, where the
installed one (W4, compressed) holds 43 drivers; so with --vmm32 every other driver of the
installed bundle the disk has no file of in VMM32 is put there from the corpus too (Windows takes
a driver it does not find in the bundle from there).

The disk is never changed: `--disk`, or the setting "win95_installed_hd" (a raw image of a hard
disk Windows 95 is installed on; default the one the Phase 3 install left in WSL), is copied into
a run folder of its own (base/runs.py), which is deleted when the run ends -- and a killed run's,
by the next run, once it is a day old. BootLog=1 is set in the copy's MSDOS.SYS; after the run the
copy's BOOTLOG.TXT says which of the files Windows loaded, and it is kept with the last screen,
one of each per tag (state/vxdboot/shots/<tag>.png and <tag>.bootlog.txt; the tag is "rebuilt"
or "retail" unless --tag names another, and a run overwrites the last of its tag). That
image has FIX95CPU's drivers in VMM32 already: without them a fast host fails at IOS, whatever
the drivers (the classic timing-loop fault of Windows 95), and those files are left alone.

QEMU runs at below-normal priority, one machine at a time; its process number is printed.
)BOOTDOC");
const py::list<Pixel> GREYS = {Pixel(192, 192, 192), Pixel(192, 199, 200)};   // the taskbar: the palette's grey, and on QEMU's VGA

namespace {

void say(const Log& log, const py::str& line) {
    if (log) log(line);
    else std::cout << line.utf8() << std::endl;
}

// open(path, "w", encoding="latin1", newline="").write(text)
void write_latin1(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("latin1"), std::nullopt, py::str(""));
    f.write(text);
    f.close();
}

py::bytes read_all(const py::str& path) {
    py::file f = py::open_rb(path);
    py::bytes b = f.read();
    f.close();
    return b;
}

}  // namespace

// ---- what goes where -----------------------------------------------------------------------------------------------------------------------
py::str corpus_file(const py::str& name) {
    py::list<py::str> names;
    if (name.contains(py::str("."))) {
        names.append(name);
    } else {
        for (const char* ext : {".vxd", ".386", ".pdr"}) names.append(name + py::str(ext));
    }
    for (const py::str& n : names) {
        const std::optional<py::str> path = base::corpus::retail(n);
        if (path && !path->empty()) return *path;
    }
    throw py::ValueError("no driver " + py::repr(name).utf8() + " in the corpus");
}

py::str guest_path(const py::str& path) {
    py::str rel = py::os::path::relpath(path, base::corpus::root()).replace(py::str("\\"), py::str("/"));
    const auto fn = py::os::path::split(rel);
    py::str folder = std::get<0>(fn);
    const py::str name = std::get<1>(fn);
    if (folder.upper() == py::str("WINDOWS/SYSTEM") && name.upper().endswith(py::str(".VXD")) && name.upper() != py::str("VMM32.VXD")) folder = py::str("WINDOWS/SYSTEM/VMM32");
    return (folder + py::str("/") + name).upper();
}

py::bytes build(const py::str& path) {
    const py::bytes data = read_all(path);
    const auto vs = le::open_vxds(path);
    if (vs.empty()) throw py::IndexError("list index out of range");
    const le::LEFile v = std::get<1>(vs[0]);
    disassembly::VxDDisasm d(v);
    d.run();
    const std::optional<le::Container> c = le::container(data);
    if (c && c->kind == py::str("W3")) {
        if (c->members.size() != 1)
            throw py::ValueError(py::percent(py::str("%s: a bundle of %d drivers; build_w3 rebuilds one"), path, c->members.size()).utf8());
        return le_writer::build_w3(d, v, std::get<0>(c->members[0]));
    }
    return le_writer::build_le(d, v, d.ddb ? d.ddb->name : py::str("DEVICE"));
}

py::bytes _original(const py::str& path) { return read_all(path); }

py::list<py::str> _bundle_members(const py::bytes& data) {
    std::optional<le::Container> c = le::container(data);
    if (c && c->kind == py::str("W4")) c = le::container(le::expand(data));
    py::list<py::str> out;
    if (c)
        for (const auto& m : c->members) out.append(std::get<0>(m));
    return out;
}

const py::list<std::tuple<py::str, py::str>> BOOT_OPTIONS = {{py::str("BootLog"), py::str("1")}, {py::str("AutoScan"), py::str("0")}};

py::str boot_options(const py::str& text, const std::optional<py::list<std::tuple<py::str, py::str>>>& options_in) {
    const py::list<std::tuple<py::str, py::str>> options = (options_in && !options_in->empty()) ? *options_in : BOOT_OPTIONS;
    std::vector<py::str> keys;
    for (const auto& kv : options) keys.push_back(std::get<0>(kv).lower());
    std::vector<py::str> lines;
    for (const py::str& ln : py::split(text, py::str("\r\n"))) {
        const py::str k = py::split(ln, py::str("="))[0].strip().lower();
        if (std::find(keys.begin(), keys.end(), k) == keys.end()) lines.push_back(ln);
    }
    int64_t at = -1;
    for (size_t i = 0; i < lines.size(); ++i)
        if (lines[i].strip().lower() == py::str("[options]")) {
            at = static_cast<int64_t>(i) + 1;
            break;
        }
    if (at < 0) {
        lines.insert(lines.begin(), py::str("[Options]"));
        at = 1;
    }
    std::vector<py::str> ins;
    for (const auto& kv : options) ins.push_back(std::get<0>(kv) + py::str("=") + std::get<1>(kv));
    lines.insert(lines.begin() + at, ins.begin(), ins.end());
    py::list<py::str> all;
    for (const py::str& l : lines) all.append(l);
    return py::join(py::str("\r\n"), all);
}

py::dict<py::str, py::bytes> plan(const py::str& disk, const py::list<py::str>& drivers, bool vmm32, bool retail, const Log& log) {
    const auto vols = media::volumes(disk);
    if (vols.empty()) throw py::IndexError("list index out of range");
    const std::shared_ptr<media::Volume> vol = vols[0];
    py::dict<py::str, media::Entry> files;
    for (const media::Entry& e : vol->files()) files.set(e.path.upper(), e);
    py::dict<py::str, py::bytes> out;
    py::list<py::str> names = drivers;   // (a copy: py::list shares; list(drivers) + ... in the Python)
    names = drivers + py::list<py::str>();
    if (vmm32) names.append(py::str("vmm32.vxd"));
    for (const py::str& name : names) {
        const py::str path = corpus_file(name);
        out.set(guest_path(path), retail ? _original(path) : build(path));
        say(log, py::percent(py::str("  %-34s %s"), guest_path(path), retail ? py::str("original") : py::str("rebuilt")));
    }
    if (vmm32) {
        const py::str own_key("WINDOWS/SYSTEM/VMM32.VXD");
        const bool has_own = files.contains(own_key);
        py::list<py::str> fallback;
        if (has_own) {
            for (const py::str& m : _bundle_members(vol->read(files.at(own_key)))) {
                const py::str at = py::percent(py::str("WINDOWS/SYSTEM/VMM32/%s.VXD"), m.upper());
                if (m.upper() == py::str("VMM") || out.contains(at) || files.contains(at)) continue;
                out.set(at, _original(corpus_file(m.lower() + py::str(".vxd"))));
                fallback.append(m);
            }
        }
        if (!fallback.empty())
            say(log, py::percent(py::str("  + %d drivers of the installed bundle as their own files (originals): %s"), fallback.size(), py::join(py::str(" "), fallback)));
    }
    if (files.contains(py::str("MSDOS.SYS"))) {
        const py::str text = vol->read(files.at(py::str("MSDOS.SYS"))).decode("latin1");
        out.set(py::str("MSDOS.SYS"), boot_options(text).encode("latin1"));
    }
    return out;
}

// ---- the images ------------------------------------------------------------------------------------------------------------------------------
Image read_ppm(const py::bytes& data) {
    const std::string& s = data.raw();
    size_t pos = 0;
    auto skip_ws = [&] {
        while (pos < s.size()) {
            if (s[pos] == '#') {
                while (pos < s.size() && s[pos] != '\n') ++pos;
            } else if (std::isspace(static_cast<unsigned char>(s[pos]))) {
                ++pos;
            } else {
                break;
            }
        }
    };
    auto number = [&]() -> int64_t {
        skip_ws();
        size_t st = pos;
        while (pos < s.size() && std::isdigit(static_cast<unsigned char>(s[pos]))) ++pos;
        if (st == pos) throw py::OSError("cannot identify image file");
        return std::stoll(s.substr(st, pos - st));
    };
    if (s.size() < 3 || s[0] != 'P' || s[1] != '6') throw py::OSError("cannot identify image file");
    pos = 2;
    Image img;
    img.width = number();
    img.height = number();
    const int64_t maxval = number();
    if (maxval != 255 || img.width <= 0 || img.height <= 0) throw py::OSError("cannot identify image file");
    ++pos;   // the one white space after maxval
    const size_t need = static_cast<size_t>(img.width * img.height * 3);
    if (s.size() - pos < need) throw py::OSError("image file is truncated");
    img.rgb = s.substr(pos, need);
    return img;
}

namespace {
void put32(std::string& o, uint32_t v) {
    o.push_back(static_cast<char>(v >> 24));
    o.push_back(static_cast<char>(v >> 16));
    o.push_back(static_cast<char>(v >> 8));
    o.push_back(static_cast<char>(v));
}
void chunk(std::string& o, const char* type, const std::string& data) {
    put32(o, static_cast<uint32_t>(data.size()));
    const std::string body = std::string(type, 4) + data;
    o += body;
    put32(o, static_cast<uint32_t>(py::crc32(py::bytes(body))));
}
}  // namespace

py::bytes png_bytes(const Image& img) {
    std::string o = "\x89PNG\r\n\x1a\n";
    std::string ihdr;
    put32(ihdr, static_cast<uint32_t>(img.width));
    put32(ihdr, static_cast<uint32_t>(img.height));
    ihdr += std::string("\x08\x02\x00\x00\x00", 5);
    chunk(o, "IHDR", ihdr);
    std::string raw;
    for (int64_t y = 0; y < img.height; ++y) {
        raw.push_back('\0');
        raw += img.rgb.substr(static_cast<size_t>(y * img.width * 3), static_cast<size_t>(img.width * 3));
    }
    chunk(o, "IDAT", py::zlib::compress(py::bytes(raw)).raw());
    chunk(o, "IEND", "");
    return py::bytes(o);
}

Image resize_nearest(const Image& img, int64_t w, int64_t h) {
    Image out;
    out.width = w;
    out.height = h;
    out.rgb.resize(static_cast<size_t>(w * h * 3));
    const double ax = static_cast<double>(img.width) / static_cast<double>(w), ay = static_cast<double>(img.height) / static_cast<double>(h);
    // PIL (Geometry.c, ImagingScaleAffine) walks the source coordinate: it starts at half a step and adds one step per pixel -- the sum, not the
    // product, whose rounding decides a pixel on a boundary
    std::vector<int64_t> xs(static_cast<size_t>(w));
    {
        double xo = 0.5 * ax;
        for (int64_t x = 0; x < w; ++x, xo += ax) xs[static_cast<size_t>(x)] = std::min<int64_t>(img.width - 1, static_cast<int64_t>(xo));
    }
    double yo = 0.5 * ay;
    for (int64_t y = 0; y < h; ++y, yo += ay) {
        const int64_t sy = std::min<int64_t>(img.height - 1, static_cast<int64_t>(yo));
        for (int64_t x = 0; x < w; ++x) {
            const int64_t sx = xs[static_cast<size_t>(x)];
            std::memcpy(&out.rgb[static_cast<size_t>((y * w + x) * 3)], &img.rgb[static_cast<size_t>((sy * img.width + sx) * 3)], 3);
        }
    }
    return out;
}

Image crop(const Image& img, int64_t left, int64_t top, int64_t right, int64_t bottom) {
    Image out;
    out.width = right - left;
    out.height = bottom - top;
    out.rgb.assign(static_cast<size_t>(out.width * out.height * 3), '\0');
    for (int64_t y = 0; y < out.height; ++y) {
        for (int64_t x = 0; x < out.width; ++x) {
            const int64_t sx = left + x, sy = top + y;
            if (sx >= 0 && sx < img.width && sy >= 0 && sy < img.height)
                std::memcpy(&out.rgb[static_cast<size_t>((y * out.width + x) * 3)], &img.rgb[static_cast<size_t>((sy * img.width + sx) * 3)], 3);
        }
    }
    return out;
}

Pixel getpixel(const Image& img, int64_t x, int64_t y) {
    if (x < 0 || y < 0 || x >= img.width || y >= img.height) throw py::IndexError("image index out of range");
    const size_t at = static_cast<size_t>((y * img.width + x) * 3);
    return Pixel(static_cast<unsigned char>(img.rgb[at]), static_cast<unsigned char>(img.rgb[at + 1]), static_cast<unsigned char>(img.rgb[at + 2]));
}

bool _teal(const Pixel& p) {
    const int64_t r = std::get<0>(p), g = std::get<1>(p), b = std::get<2>(p);
    return std::llabs(g - b) < 12 && 110 < g && g < 190 && r < g - 50;
}

py::list<Pixel> _pixels(const Image& img) {
    py::list<Pixel> out;
    for (int64_t i = 0; i + 2 < static_cast<int64_t>(img.rgb.size()); i += 3)
        out.append(Pixel(static_cast<unsigned char>(img.rgb[static_cast<size_t>(i)]), static_cast<unsigned char>(img.rgb[static_cast<size_t>(i) + 1]),
                         static_cast<unsigned char>(img.rgb[static_cast<size_t>(i) + 2])));
    return out;
}

py::str classify(const Image& img) {
    const int64_t w = img.width, h = img.height;
    const py::list<Pixel> px = _pixels(resize_nearest(img, 160, 120));
    auto share = [&](const std::function<bool(const Pixel&)>& test) {
        int64_t n = 0;
        for (const Pixel& p : px)
            if (test(p)) n += 1;
        return static_cast<double>(n) / static_cast<double>(px.size());
    };
    if (share([](const Pixel& p) { return std::get<2>(p) > 140 && std::get<0>(p) < 40 && std::get<1>(p) < 40; }) > 0.6) return py::str("blue");
    if (w == 720 && h == 400) return py::str("text");
    if (share([](const Pixel& p) { return std::get<0>(p) + std::get<1>(p) + std::get<2>(p) < 40; }) > 0.97) return py::str("black");
    // (the desktop: mostly teal, with whatever box is on it -- the logon, "Welcome", a clock message -- and the taskbar once Explorer has it, in a
    // 640x480 or larger mode)
    if (w >= 640 && h >= 480 && share(_teal) > 0.4) return py::str("desktop");
    if (w >= 640 && h >= 480) {   // (a wallpaper: the taskbar, then)
        const py::list<Pixel> bar = _pixels(resize_nearest(crop(img, 0, h - 24, w, h), 80, 6));
        auto is_grey = [&](const Pixel& p) {
            for (const Pixel& g : GREYS)
                if (g == p) return true;
            return false;
        };
        int64_t n = 0;
        for (const Pixel& p : bar)
            if (is_grey(p)) n += 1;
        if (static_cast<double>(n) > static_cast<double>(bar.size()) * 0.7 && is_grey(getpixel(img, 0, h - 14))) return py::str("desktop");
    }
    return py::str("other");
}

// ---- the machine -----------------------------------------------------------------------------------------------------------------------------
namespace {

struct Winsock {
    Winsock() {
        WSADATA d;
        WSAStartup(MAKEWORD(2, 2), &d);
    }
};
void ensure_winsock() { static Winsock w; }

// setup16._free_port()
int64_t free_port() {
    ensure_winsock();
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    a.sin_port = 0;
    bind(s, reinterpret_cast<sockaddr*>(&a), sizeof a);
    int len = sizeof a;
    getsockname(s, reinterpret_cast<sockaddr*>(&a), &len);
    const int64_t port = ntohs(a.sin_port);
    closesocket(s);
    return port;
}

// setup16._Monitor
struct Monitor {
    int64_t port;
    explicit Monitor(int64_t p) : port(p) {}

    void send_cmds(const std::vector<std::string>& cmds) const {
        ensure_winsock();
        SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in a{};
        a.sin_family = AF_INET;
        a.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        a.sin_port = htons(static_cast<u_short>(port));
        DWORD to = 5000;
        setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&to), sizeof to);
        if (connect(s, reinterpret_cast<sockaddr*>(&a), sizeof a) != 0) {
            closesocket(s);
            throw py::OSError("[WinError 10061] No connection could be made because the target machine actively refused it");
        }
        to = 2000;
        setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&to), sizeof to);
        char buf[4096];
        (void)recv(s, buf, sizeof buf, 0);   // (a timeout is fine)
        for (const std::string& c : cmds) {
            const std::string line = c + "\n";
            send(s, line.data(), static_cast<int>(line.size()), 0);
            py::time::sleep(0.3);
        }
        closesocket(s);
    }

    Image shot(const py::str& png) const {
        const py::str ppm = png.slice(0, py::len(png) - 4) + py::str(".ppm");
        send_cmds({"screendump " + ppm.replace(py::str("\\"), py::str("/")).utf8()});
        py::time::sleep(1.2);
        const Image img = read_ppm(read_all(ppm));
        {
            py::file f = py::open(png, py::str("wb"));
            f.write(png_bytes(img));
            f.close();
        }
        py::os::remove(ppm);
        return img;
    }
};

// subprocess.Popen of QEMU: below-normal priority, its output to NUL
struct Qemu {
    HANDLE process = nullptr;
    DWORD pid = 0;

    Qemu(const py::list<py::str>& args) {
        std::wstring cmd = py::subprocess::to_wide(py::subprocess::list2cmdline(args));
        SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
        HANDLE nul = CreateFileW(L"NUL", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        STARTUPINFOW si{};
        si.cb = sizeof si;
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si.hStdOutput = nul;
        si.hStdError = nul;
        PROCESS_INFORMATION pi{};
        const BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, BELOW_NORMAL_PRIORITY_CLASS, nullptr, nullptr, &si, &pi);
        const DWORD err = GetLastError();
        CloseHandle(nul);
        if (!ok) py::raise_winerror(err);
        CloseHandle(pi.hThread);
        process = pi.hProcess;
        pid = pi.dwProcessId;
    }
    ~Qemu() {
        if (process) CloseHandle(process);
    }
    Qemu(const Qemu&) = delete;
    Qemu& operator=(const Qemu&) = delete;

    bool exited() const {
        DWORD code = 0;
        return GetExitCodeProcess(process, &code) && code != STILL_ACTIVE;
    }
    void kill() const { TerminateProcess(process, 1); }
    void wait(double seconds) const { WaitForSingleObject(process, static_cast<DWORD>(seconds * 1000)); }
};

}  // namespace

BootResult boot(const py::str& disk, const py::dict<py::str, py::bytes>& files, const py::str& tag, double wait, const Log& log) {
    const std::optional<py::str> qopt = py::settings::get_opt_str(py::str("qemu"));
    const py::str qemu = (qopt && !qopt->empty()) ? *qopt : py::str("C:\\Program Files\\qemu\\qemu-system-i386.exe");
    const py::str shots = base::runs::results(py::str("vxdboot"));
    py::os::makedirs(shots, 0777, true);
    const py::str png = py::os::path::join(shots, tag + py::str(".png"));
    double min_gb = 0.0;   // float(settings.get("min_free_gb") or 6): the setting is a number, or text (the environment)
    {
        const py::settings::Value v = py::settings::get(py::str("min_free_gb"));
        if (std::holds_alternative<py::str>(v)) {
            const py::str s = std::get<py::str>(v).strip();
            if (!std::get<py::str>(v).empty()) {
                try {
                    min_gb = std::stod(s.utf8());
                } catch (const std::exception&) {
                    throw py::ValueError("could not convert string to float: " + py::repr(std::get<py::str>(v)).utf8());
                }
            }
        } else if (std::holds_alternative<int64_t>(v)) {
            min_gb = static_cast<double>(std::get<int64_t>(v));
        } else if (std::holds_alternative<double>(v)) {
            min_gb = std::get<double>(v);
        } else if (std::holds_alternative<bool>(v)) {
            min_gb = std::get<bool>(v) ? 1.0 : 0.0;
        }
    }
    if (min_gb == 0.0) min_gb = 6;
    const double need = static_cast<double>(py::os::path::getsize(disk)) + min_gb * 1073741824.0;
    const py::str state_dir = py::os::path::join(py::settings::get_str(py::str("state")), py::str("vxdboot"));
    py::os::makedirs(py::os::path::dirname(state_dir), 0777, true);
    if (static_cast<double>(std::filesystem::space(state_dir.utf8()).available) < need) throw py::RuntimeError("not enough free disk space for a copy of " + disk.utf8());
    const py::str work = base::runs::start(py::str("vxdboot"), tag);   // (the disk copy: this run's alone)
    std::unique_ptr<Qemu> q;
    py::str verdict("FAIL(timeout)");
    py::str bootlog;
    try {
        const py::str hd = py::os::path::join(work, py::str("hd.img"));
        py::shutil::copyfile(disk, hd);
        media::put(hd, files);
        const int64_t port = free_port();
        q = std::make_unique<Qemu>(py::list<py::str>{qemu, py::str("-M"), py::str("pc"), py::str("-m"), py::str("64"), py::str("-drive"),
                                                    py::percent(py::str("file=%s,format=raw,if=ide,index=0"), hd), py::str("-boot"), py::str("c"), py::str("-display"),
                                                    py::str("none"), py::str("-monitor"), py::percent(py::str("tcp:127.0.0.1:%d,server,nowait"), port)});
        say(log, py::percent(py::str("  QEMU process %d (below-normal priority), up to %ds"), static_cast<int64_t>(q->pid), static_cast<int64_t>(wait)));
        const Monitor mon(port);
        std::vector<py::str> seen;
        int64_t same = 0;
        std::optional<py::str> last;
        const double until = py::time::time() + wait;
        while (py::time::time() < until) {
            py::time::sleep(4);
            if (q->exited()) {
                verdict = py::str("FAIL(qemu exited)");
                break;
            }
            Image img;
            try {
                img = mon.shot(png);
            } catch (const py::OSError&) {
                continue;
            } catch (const py::ValueError&) {
                continue;
            }
            const py::str kind = classify(img);
            seen.push_back(kind);
            if (!last || kind != *last)
                say(log, py::percent(py::str("    %3ds  %s screen %dx%d"), static_cast<int64_t>(wait - (until - py::time::time())), kind, img.width, img.height));
            same = (last && kind == *last) ? same + 1 : 0;
            last = kind;
            if (kind == py::str("desktop") && same >= 2) {   // (three screens running: settled)
                verdict = py::str("PASS");
                break;
            }
            if (kind == py::str("blue") && same >= 8) {   // (ScanDisk is blue too, for a while)
                verdict = py::str("FAIL(blue screen)");
                break;
            }
            // a text screen or a black one after Windows' own graphic screens is a stop (the protection error; a fault with the screen left dark), but only
            // once it has stayed
            if ((kind == py::str("text") || kind == py::str("black")) && std::find(seen.begin(), seen.end(), py::str("other")) != seen.end() && same >= 15) {
                verdict = py::str("FAIL(") + kind + py::str(" screen)");
                break;
            }
        }
    } catch (...) {
        if (q) {
            q->kill();
            q->wait(20);
        }
        try {
            bootlog = _read(py::os::path::join(work, py::str("hd.img")), py::str("BOOTLOG.TXT"));
        } catch (const py::OSError&) {
        } catch (const py::ValueError&) {
        } catch (const py::KeyError&) {
        }
        base::runs::remove(work);   // (QEMU lets go of its files a moment later)
        throw;
    }
    if (q) {
        q->kill();
        q->wait(20);
    }
    try {
        bootlog = _read(py::os::path::join(work, py::str("hd.img")), py::str("BOOTLOG.TXT"));
    } catch (const py::OSError&) {
    } catch (const py::ValueError&) {
    } catch (const py::KeyError&) {
    }
    base::runs::remove(work);   // (QEMU lets go of its files a moment later)
    write_latin1(py::os::path::join(shots, tag + py::str(".bootlog.txt")), bootlog);
    return BootResult{verdict, png, bootlog};
}

py::str _read(const py::str& image, const py::str& name) {
    const auto vols = media::volumes(image);
    if (vols.empty()) throw py::IndexError("list index out of range");
    const std::shared_ptr<media::Volume> vol = vols[0];
    py::dict<py::str, media::Entry> entry;
    for (const media::Entry& e : vol->files()) entry.set(e.path.upper(), e);
    return vol->read(entry.at(name.upper())).decode("latin1");
}

py::str loaded(const py::str& bootlog, const py::str& path) {
    const py::list<py::str> parts = py::split(path, py::str("/"));
    const py::str name = parts[parts.size() - 1].upper();
    const py::str stem = py::split(name, py::str("."))[0];
    py::list<py::str> steps;
    for (const py::str& ln : py::splitlines(bootlog)) {
        const int64_t rb = ln.find(py::str("]"));
        const py::str tail = rb >= 0 ? ln.slice(rb + 1, py::len(ln)) : ln;   // ln.split("]", 1)[-1]
        const auto pv = tail.partition(py::str("="));
        py::str value = std::get<2>(pv).strip().upper();
        const py::str what = std::get<0>(pv);
        if (value.endswith(py::str("\\") + name) || value == name || value == stem) {
            const py::str step = what.strip();
            bool present = false;
            for (const py::str& s : steps) present = present || s == step;
            if (step != py::str("Loading Vxd") && !present) steps.append(step);
        }
    }
    py::list<py::str> out;
    for (const py::str& s : steps) {
        bool ended = false;
        for (const py::str& t : steps) ended = ended || t == s + py::str("SUCCESS");
        if (!ended) out.append(s);   // (a phase that ended: once)
    }
    return py::join(py::str(" "), out);
}

py::str run(const py::str& disk, const py::list<py::str>& drivers, bool vmm32, bool retail, const py::str& tag, double wait, const Log& log) {
    py::list<py::str> units = drivers + py::list<py::str>();
    if (vmm32) units.append(py::str("VMM32"));
    const py::str label = units.empty() ? py::str("(nothing put in)") : py::join(py::str(", "), units);
    say(log, py::percent(py::str("%s: %s %s"), tag, retail ? py::str("originals of") : py::str("rebuilt"), label));
    const py::dict<py::str, py::bytes> files = plan(disk, drivers, vmm32, retail, log);
    const BootResult br = boot(disk, files, tag, wait, log);
    for (const py::str& d : drivers) {
        const py::str p = guest_path(corpus_file(d));
        const py::str l = loaded(br.bootlog, p);
        say(log, py::percent(py::str("    %-34s %s"), p, !l.empty() ? l : py::str("(not in BOOTLOG.TXT: not loaded)")));
    }
    say(log, py::percent(py::str("  %s   screen: %s"), br.verdict, br.png));
    if (br.verdict != py::str("PASS") && !br.bootlog.empty()) {
        const py::list<py::str> ls = py::splitlines(br.bootlog.strip());
        py::list<py::str> last;
        for (int64_t i = std::max<int64_t>(0, ls.size() - 6); i < ls.size(); ++i) last.append(ls[i]);
        say(log, py::str("  BOOTLOG.TXT ends:\n    ") + py::join(py::str("\n    "), last));
    }
    return br.verdict;
}

py::list<py::str> bisect(const py::list<py::str>& units, const std::function<py::str(const py::list<py::str>&, const py::str&)>& trial, const py::str& tag, const Log& log) {
    std::function<py::list<py::str>(const py::list<py::str>&, const py::str&)> go = [&](const py::list<py::str>& group, const py::str& n) -> py::list<py::str> {
        if (group.size() == 1) return group;
        py::list<py::str> found;
        const int64_t half = group.size() / 2;
        const py::list<py::str> parts[2] = {group.slice(0, half), group.slice(half, group.size())};
        for (int i = 0; i < 2; ++i) {
            if (trial(parts[i], py::percent(py::str("%s_b%s%d"), tag, n, static_cast<int64_t>(i))) != py::str("PASS")) found.extend(go(parts[i], n + py::percent(py::str("%d"), static_cast<int64_t>(i))));
        }
        if (found.empty()) {
            say(log, py::percent(py::str("  %s: no half fails alone -- they fail together"), py::join(py::str(", "), group)));
            return group;
        }
        return found;
    };
    return go(units + py::list<py::str>(), py::str(""));
}

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    (void)err;
    if (argv.empty() || argv[0] == py::str("-h") || argv[0] == py::str("--help")) {
        out << DOC.utf8() << "\n";
        return argv.empty() ? 2 : 0;
    }
    const std::optional<py::str> hd = py::settings::get_opt_str(py::str("win95_installed_hd"));
    py::dict<py::str, py::str> opts;
    opts.set(py::str("--drivers"), py::str(""));
    opts.set(py::str("--disk"), (hd && !hd->empty()) ? *hd : DEFAULT_DISK);
    opts.set(py::str("--wait"), py::str("300"));
    opts.set(py::str("--tag"), py::str(""));
    py::set<py::str> flags;
    int64_t i = 0;
    while (i < argv.size()) {
        const py::str a = argv[i];
        if (opts.contains(a) && i + 1 < argv.size()) {
            opts.set(a, argv[i + 1]);
            i += 2;
            continue;
        }
        if (a == py::str("--vmm32") || a == py::str("--retail") || a == py::str("--bisect")) {
            flags.add(a);
        } else {
            out << "unknown argument " << py::repr(a).utf8() << "\n" << DOC.utf8() << "\n";
            return 2;
        }
        i += 1;
    }
    py::list<py::str> drivers;
    for (const py::str& d : py::split(opts.at(py::str("--drivers")), py::str(",")))
        if (!d.strip().empty()) drivers.append(d.strip().lower());
    const py::str disk = opts.at(py::str("--disk"));
    const py::str dl = disk.lower();
    if ((dl.startswith(py::str("\\\\wsl$")) || dl.startswith(py::str("\\\\wsl.localhost"))) && !py::os::path::isfile(disk) && py::shutil::which(py::str("wsl"))) {
        py::subprocess::RunOptions o;
        o.timeout = int64_t(120);
        py::subprocess::run(py::list<py::str>{py::str("wsl"), py::str("-e"), py::str("true")}, o);   // (WSL stops when idle)
    }
    if (!py::os::path::isfile(disk)) {
        out << "no installed Windows 95 disk at " << disk.utf8() << " (set win95_installed_hd in settings.json, or --disk)\n";
        return 1;
    }
    const py::str tag = !opts.at(py::str("--tag")).empty() ? opts.at(py::str("--tag")) : (flags.contains(py::str("--retail")) ? py::str("retail") : py::str("rebuilt"));
    const Log log = [&](const py::str& line) { out << line.utf8() << "\n"; };
    const bool vmm32 = flags.contains(py::str("--vmm32")), retail = flags.contains(py::str("--retail"));
    const py::str verdict = run(disk, drivers, vmm32, retail, tag, static_cast<double>(py::int_parse(opts.at(py::str("--wait")))), log);
    if (verdict != py::str("PASS") && flags.contains(py::str("--bisect"))) {
        py::list<py::str> units = drivers + py::list<py::str>();
        if (vmm32) units.append(py::str("VMM32"));
        if (units.size() > 1) {
            const py::list<py::str> culprits = bisect(
                units,
                [&](const py::list<py::str>& part, const py::str& name) {
                    py::list<py::str> ds;
                    bool has_vmm = false;
                    for (const py::str& u : part) {
                        if (u != py::str("VMM32")) ds.append(u);
                        else has_vmm = true;
                    }
                    return run(disk, ds, has_vmm, retail, name, static_cast<double>(py::int_parse(opts.at(py::str("--wait")))), log);
                },
                tag, log);
            out << "fails: " << py::join(py::str(", "), culprits).utf8() << "\n";
        }
    }
    return verdict == py::str("PASS") ? 0 : 1;
}

}  // namespace codebreaker::source::vxd::boot
