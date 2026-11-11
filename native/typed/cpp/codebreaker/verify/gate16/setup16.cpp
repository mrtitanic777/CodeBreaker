// codebreaker/verify/gate16/setup16.cpp -- typed C++ port of codebreaker.verify.gate16.setup16 (see setup16.hpp).

#include "codebreaker/verify/gate16/setup16.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <set>

#include "codebreaker/base/runs.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"
#include "py_zlib.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

namespace codebreaker::verify::gate16::setup16 {

namespace {

using py::str;

str S(const char* s) { return str(s); }

constexpr int64_t PRECISION_BITS = 32 - 8 - 2;

// ---- Pillow's bicubic resample (libImaging/Resample.c, 8 bits a channel) -------------------------------------------------------------------
double bicubic(double x) {
    constexpr double a = -0.5;
    if (x < 0.0) x = -x;
    if (x < 1.0) return ((a + 2.0) * x - (a + 3.0)) * x * x + 1;
    if (x < 2.0) return (((x - 5) * x + 8) * x - 4) * a;
    return 0.0;
}

struct Coeffs {
    int64_t ksize = 0;
    std::vector<int64_t> bounds;   // (min, count) per output pixel
    std::vector<int64_t> kk;       // ksize coefficients per output pixel, 22-bit fixed point
};

Coeffs precompute(int64_t in_size, int64_t out_size) {
    const double scale = static_cast<double>(in_size) / static_cast<double>(out_size);
    const double filterscale = scale < 1.0 ? 1.0 : scale;
    const double support = 2.0 * filterscale;
    Coeffs c;
    c.ksize = static_cast<int64_t>(std::ceil(support)) * 2 + 1;
    c.bounds.assign(static_cast<size_t>(out_size) * 2, 0);
    c.kk.assign(static_cast<size_t>(out_size * c.ksize), 0);
    std::vector<double> k(static_cast<size_t>(c.ksize));
    const double ss = 1.0 / filterscale;
    for (int64_t xx = 0; xx < out_size; ++xx) {
        const double center = (xx + 0.5) * scale;
        double ww = 0.0;
        int64_t xmin = static_cast<int64_t>(center - support + 0.5);
        if (xmin < 0) xmin = 0;
        int64_t xmax = static_cast<int64_t>(center + support + 0.5);
        if (xmax > in_size) xmax = in_size;
        xmax -= xmin;
        std::fill(k.begin(), k.end(), 0.0);
        for (int64_t x = 0; x < xmax; ++x) {
            const double w = bicubic((x + xmin - center + 0.5) * ss);
            k[static_cast<size_t>(x)] = w;
            ww += w;
        }
        for (int64_t x = 0; x < xmax; ++x)
            if (ww != 0.0) k[static_cast<size_t>(x)] /= ww;
        c.bounds[static_cast<size_t>(xx) * 2] = xmin;
        c.bounds[static_cast<size_t>(xx) * 2 + 1] = xmax;
        for (int64_t x = 0; x < c.ksize; ++x) {
            const double v = k[static_cast<size_t>(x)];
            c.kk[static_cast<size_t>(xx * c.ksize + x)] = static_cast<int64_t>(v < 0 ? -0.5 + v * (1 << PRECISION_BITS) : 0.5 + v * (1 << PRECISION_BITS));
        }
    }
    return c;
}

uint8_t clip8(int64_t ss) {
    const int64_t v = ss >> PRECISION_BITS;   // (arithmetic shift)
    return static_cast<uint8_t>(v < 0 ? 0 : v > 255 ? 255 : v);
}

// the pixels as rows of 3-byte pixels
using Pix = std::vector<uint8_t>;

Pix resample_horizontal(const Pix& in, int64_t w, int64_t h, int64_t out_w) {
    const Coeffs c = precompute(w, out_w);
    Pix out(static_cast<size_t>(out_w * h * 3));
    for (int64_t y = 0; y < h; ++y)
        for (int64_t xx = 0; xx < out_w; ++xx) {
            const int64_t xmin = c.bounds[static_cast<size_t>(xx) * 2], xmax = c.bounds[static_cast<size_t>(xx) * 2 + 1];
            for (int ch = 0; ch < 3; ++ch) {
                int64_t ss = int64_t(1) << (PRECISION_BITS - 1);
                for (int64_t x = 0; x < xmax; ++x) ss += static_cast<int64_t>(in[static_cast<size_t>((y * w + xmin + x) * 3 + ch)]) * c.kk[static_cast<size_t>(xx * c.ksize + x)];
                out[static_cast<size_t>((y * out_w + xx) * 3 + ch)] = clip8(ss);
            }
        }
    return out;
}

Pix resample_vertical(const Pix& in, int64_t w, int64_t h, int64_t out_h) {
    const Coeffs c = precompute(h, out_h);
    Pix out(static_cast<size_t>(w * out_h * 3));
    for (int64_t yy = 0; yy < out_h; ++yy) {
        const int64_t ymin = c.bounds[static_cast<size_t>(yy) * 2], ymax = c.bounds[static_cast<size_t>(yy) * 2 + 1];
        for (int64_t x = 0; x < w; ++x)
            for (int ch = 0; ch < 3; ++ch) {
                int64_t ss = int64_t(1) << (PRECISION_BITS - 1);
                for (int64_t y = 0; y < ymax; ++y) ss += static_cast<int64_t>(in[static_cast<size_t>(((ymin + y) * w + x) * 3 + ch)]) * c.kk[static_cast<size_t>(yy * c.ksize + y)];
                out[static_cast<size_t>((yy * w + x) * 3 + ch)] = clip8(ss);
            }
    }
    return out;
}

// ---- PNG -----------------------------------------------------------------------------------------------------------------------------------
void be32(std::string& o, uint32_t v) {
    for (int s = 24; s >= 0; s -= 8) o.push_back(static_cast<char>((v >> s) & 0xFF));
}

void chunk(std::string& o, const char* type, const std::string& data) {
    be32(o, static_cast<uint32_t>(data.size()));
    const std::string body = std::string(type, 4) + data;
    o += body;
    be32(o, static_cast<uint32_t>(py::crc32(py::bytes(body))));
}

// ---- Winsock ---------------------------------------------------------------------------------------------------------------------------------
void winsock() {
    static bool started = [] {
        WSADATA d;
        WSAStartup(MAKEWORD(2, 2), &d);
        return true;
    }();
    (void)started;
}

[[noreturn]] void raise_socket(const char* what, int err) { throw py::OSError(std::string(what) + " failed: WSA error " + std::to_string(err)); }

// socket's ConnectionRefusedError (an OSError)
struct ConnectionRefusedError : py::OSError {
    explicit ConnectionRefusedError(const std::string& m) : py::OSError(m) {}
    explicit ConnectionRefusedError(const char* m) : py::OSError(std::string(m)) {}
    explicit ConnectionRefusedError(const str& m) : py::OSError(m.utf8()) {}
    const char* type() const override { return "ConnectionRefusedError"; }
};

IsoFactory& iso_factory() {
    static IsoFactory f;
    return f;
}
std::shared_ptr<CabTool>& cab_tool() {
    static std::shared_ptr<CabTool> c;
    return c;
}

}  // namespace

const py::list<std::tuple<int64_t, int64_t>> LICENSE_POINTS = {{60, 350}, {580, 350}, {300, 395}};

// ---- Image ---------------------------------------------------------------------------------------------------------------------------------
Image Image::make(int64_t w, int64_t h, const py::bytes& rgb) {
    Image i;
    i.w_ = w;
    i.h_ = h;
    i.rgb_ = rgb;
    return i;
}

Image Image::from_ppm(const py::bytes& data) {
    const std::string& s = data.raw();
    size_t at = 0;
    auto bad = [] { return py::OSError("cannot identify image file"); };
    if (s.size() < 2 || s[0] != 'P' || s[1] != '6') throw bad();
    at = 2;
    auto token = [&]() -> std::string {
        while (at < s.size()) {
            if (s[at] == '#') {
                while (at < s.size() && s[at] != '\n') ++at;
            } else if (std::isspace(static_cast<unsigned char>(s[at]))) {
                ++at;
            } else {
                break;
            }
        }
        std::string t;
        while (at < s.size() && !std::isspace(static_cast<unsigned char>(s[at])) && s[at] != '#') t.push_back(s[at++]);
        return t;
    };
    int64_t w = 0, h = 0, maxval = 0;
    try {
        w = std::stoll(token());
        h = std::stoll(token());
        maxval = std::stoll(token());
    } catch (const std::exception&) {
        throw bad();
    }
    if (w <= 0 || h <= 0 || maxval != 255) throw bad();
    ++at;   // the one white space after the maximum
    const size_t need = static_cast<size_t>(w * h * 3);
    if (s.size() < at + need) throw py::OSError("image file is truncated");
    return make(w, h, py::bytes(s.substr(at, need)));
}

Image Image::open(const py::str& path) {
    py::file f = py::open(path, S("rb"));
    py::bytes data = f.read();
    f.close();
    return from_ppm(data);
}

RGB Image::getpixel(int64_t x, int64_t y) const {
    if (x < 0 || y < 0 || x >= w_ || y >= h_) throw py::IndexError("image index out of range");
    const size_t at = static_cast<size_t>((y * w_ + x) * 3);
    const std::string& s = rgb_.raw();
    return {static_cast<uint8_t>(s[at]), static_cast<uint8_t>(s[at + 1]), static_cast<uint8_t>(s[at + 2])};
}

Image Image::resize(int64_t w, int64_t h) const {
    if (w == w_ && h == h_) return *this;
    Pix px(rgb_.raw().begin(), rgb_.raw().end());
    int64_t cw = w_, ch = h_;
    if (w != cw) {
        px = resample_horizontal(px, cw, ch, w);
        cw = w;
    }
    if (h != ch) {
        px = resample_vertical(px, cw, ch, h);
        ch = h;
    }
    return make(w, h, py::bytes(std::string(px.begin(), px.end())));
}

py::list<RGB> Image::getdata() const {
    py::list<RGB> out;
    for (int64_t y = 0; y < h_; ++y)
        for (int64_t x = 0; x < w_; ++x) out.append(getpixel(x, y));
    return out;
}

void Image::save(const py::str& png) const {
    std::string o = "\x89PNG\r\n\x1a\n";
    std::string ihdr;
    be32(ihdr, static_cast<uint32_t>(w_));
    be32(ihdr, static_cast<uint32_t>(h_));
    ihdr += std::string("\x08\x02\x00\x00\x00", 5);
    chunk(o, "IHDR", ihdr);
    std::string raw;
    for (int64_t y = 0; y < h_; ++y) {
        raw.push_back('\0');
        raw += rgb_.raw().substr(static_cast<size_t>(y * w_ * 3), static_cast<size_t>(w_ * 3));
    }
    chunk(o, "IDAT", py::zlib::compress(py::bytes(raw), 6, 15).raw());
    chunk(o, "IEND", "");
    py::file f = py::open(png, S("wb"));
    f.write(py::bytes(o));
    f.close();
}

// ---- paths, port ---------------------------------------------------------------------------------------------------------------------------
Paths _paths() {
    const str mini = py::os::path::join(py::settings::get_str(S("state")), S("mini"));
    py::os::makedirs(py::os::path::dirname(mini), 0777, true);   // (settings.state makes the folder of the path it joins, not the path itself)
    Paths p;
    str q = py::settings::get_str(S("qemu"));
    p.qemu = q.empty() ? S("C:\\Program Files\\qemu\\qemu-system-i386.exe") : q;
    p.iso = py::settings::get_str(S("win95_setup_iso"));
    p.original = py::os::path::join(mini, S("original"));
    p.cab = py::os::path::join(mini, S("MINI.CAB"));
    p.hd = py::os::path::join(mini, S("prepared_hd.img"));
    const str floppy = py::os::path::join(py::settings::get_str(S("state")), S("bootfloppy"), S("boot95_is.img"));
    py::os::makedirs(py::os::path::dirname(floppy), 0777, true);
    p.floppy = floppy;
    return p;
}

int64_t _free_port() {
    winsock();
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) raise_socket("socket", WSAGetLastError());
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = 0;
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    if (bind(s, reinterpret_cast<sockaddr*>(&a), sizeof a) != 0) {
        const int e = WSAGetLastError();
        closesocket(s);
        raise_socket("bind", e);
    }
    int len = sizeof a;
    getsockname(s, reinterpret_cast<sockaddr*>(&a), &len);
    const int64_t port = ntohs(a.sin_port);
    closesocket(s);
    return port;
}

void set_iso_cab(const IsoFactory& iso, const std::shared_ptr<CabTool>& cab) {
    iso_factory() = iso;
    cab_tool() = cab;
}

// ---- the CD ----------------------------------------------------------------------------------------------------------------------------------
void make_cd(const str& out, const py::dict<str, str>& replace) {
    if (!iso_factory() || !cab_tool()) throw py::ModuleNotFoundError("No module named 'pycdlib'");
    CabTool& cab = *cab_tool();
    const Paths p = _paths();
    py::list<str> names;
    for (const CabEntry& e : cab.read(p.cab).files) names.append(e.name);
    py::dict<str, str> up;
    for (const auto& kv : replace.items()) up.set(std::get<0>(kv).upper(), std::get<1>(kv));
    py::list<std::tuple<str, py::bytes>> files;
    for (const str& n : names) {
        const str path = up.contains(n.upper()) && !up.at(n.upper()).empty() ? up.at(n.upper()) : py::os::path::join(p.original, n);
        py::file f = py::open(path, S("rb"));
        files.append(std::make_tuple(n, f.read()));
        f.close();
    }
    const str tmp = out + S(".cab");
    cab.write(files, tmp, S("mszip"), 16261);
    py::bytes data;
    {
        py::file f = py::open(tmp, S("rb"));
        data = f.read();
        f.close();
    }
    py::os::remove(tmp);
    std::shared_ptr<Iso> iso = iso_factory()();
    iso->open(p.iso);
    std::optional<str> joliet = iso->has_joliet() ? std::optional<str>(S("/WIN95/MINI.CAB")) : std::nullopt;
    iso->rm_file(S("/WIN95/MINI.CAB;1"), joliet);
    iso->add_fp(data, S("/WIN95/MINI.CAB;1"), joliet);
    // The rest of Setup's own programs -- SUWIN, SETUPX and the others, in PRECOPY2.CAB, and SETUP.EXE loose beside the cabinets -- rebuilt from
    // ours where `replace` names them: a cabinet holding one is written again with the original's other files and its set fields
    py::set<str> have;
    for (const str& n : names) have.add(n.upper());
    py::dict<str, str> rest;
    for (const auto& kv : up.items())
        if (!have.contains(std::get<0>(kv))) rest.set(std::get<0>(kv), std::get<1>(kv));
    if (!rest.empty()) _replace_on_cd(*iso, cab, rest, out);
    iso->write(out);
    iso->close();
}

void _replace_on_cd(Iso& iso, CabTool& cab, const py::dict<str, str>& rest, const str& out) {
    const str work = py::tempfile::mkdtemp(std::nullopt, std::optional<str>(S("cd_")), std::optional<str>(py::os::path::dirname(py::os::path::abspath(out))));
    try {
        py::dict<str, str> idents;   // {NAME: identifier as the CD has it}
        for (const IsoChild& c : iso.list_children(S("/WIN95"))) {
            const py::bytes& id = c.file_identifier;
            if (id.raw() == "." || id.raw() == "..") continue;
            const str text = id.decode("utf-8");
            idents.set(py::split(text, S(";"))[0].upper(), text);
        }
        py::dict<str, Cab> cabs;
        for (const str& name : idents.keys()) {   // (every cabinet, beside the others: a file one cabinet continues into the next is read from both)
            if (name.endswith(S(".CAB"))) {
                const py::bytes raw = iso.get_file_from_iso(S("/WIN95/") + idents.at(name));
                {
                    py::file f = py::open(py::os::path::join(work, name), S("wb"));
                    f.write(raw);
                    f.close();
                }
                cabs.set(name, cab.read(py::os::path::join(work, name)));
            }
        }
        py::dict<str, py::bytes> nw;   // (loose: SETUP.EXE)
        for (const str& name : rest.keys())
            if (idents.contains(name)) {
                py::file f = py::open(rest.at(name), S("rb"));
                nw.set(name, f.read());
                f.close();
            }
        // A cabinet holding one of ours is written again -- with the whole set it belongs to: PRECOPY1's last three files run on into PRECOPY2, and
        // PRECOPY2 rewritten alone left PRECOPY1 pointing into a cabinet that no longer continued them ("Could not decompress this Setup (.CAB)
        // file"). Each file is now whole in the cabinet it starts in.
        py::list<str> todo;
        auto in_todo = [&](const str& k) {
            for (const str& t : todo)
                if (t == k) return true;
            return false;
        };
        for (const auto& kv : cabs.items()) {
            const str& name = std::get<0>(kv);
            const Cab& c = std::get<1>(kv);
            bool holds = false;
            for (const CabEntry& e : c.files)
                if (rest.contains(e.name.upper())) holds = true;
            if (holds) {
                str k = name;
                while (cabs.contains(k) && cabs.at(k).info.prev) k = cabs.at(k).info.prev->upper();
                while (cabs.contains(k)) {
                    if (!in_todo(k)) todo.append(k);
                    k = cabs.at(k).info.next ? cabs.at(k).info.next->upper() : S("");
                }
            }
        }
        for (const str& name : todo) {
            const Cab& c = cabs.at(name);
            py::dict<str, str> got = cab.extract(py::os::path::join(work, name), py::os::path::join(work, name + S(".d")));
            py::list<std::tuple<str, py::bytes>> files;
            for (const CabEntry& e : c.files) {
                if (e.folder == 0xFFFD || e.folder == 0xFFFF) continue;   // (continued: in the one before)
                const str path = rest.contains(e.name.upper()) && !rest.at(e.name.upper()).empty() ? rest.at(e.name.upper()) : got.at(e.name);
                py::file f = py::open(path, S("rb"));
                files.append(std::make_tuple(e.name, f.read()));
                f.close();
            }
            const CabInfo& i = c.info;
            cab.write(files, py::os::path::join(work, name + S(".new")), S("mszip"), i.set_id, i.index, i.prev, i.next);
            py::file f = py::open(py::os::path::join(work, name + S(".new")), S("rb"));
            nw.set(name, f.read());
            f.close();
        }
        for (const auto& kv : nw.items()) {
            const str& name = std::get<0>(kv);
            const py::bytes& data = std::get<1>(kv);
            const std::optional<str> joliet = iso.has_joliet() ? std::optional<str>(S("/WIN95/") + name) : std::nullopt;
            iso.rm_file(S("/WIN95/") + idents.at(name), joliet);
            iso.add_fp(data, S("/WIN95/") + idents.at(name), joliet);
        }
    } catch (...) {
        py::shutil::rmtree(work, true);   // (no copies left behind)
        throw;
    }
    py::shutil::rmtree(work, true);
}

// ---- Process ---------------------------------------------------------------------------------------------------------------------------------
Process::Process(const py::list<str>& args) {
    const str line = py::subprocess::list2cmdline(args);
    std::wstring cmd = py::subprocess::to_wide(line);
    cmd.push_back(L'\0');
    HANDLE nul = CreateFileW(L"NUL", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE inherit = nullptr;
    DuplicateHandle(GetCurrentProcess(), nul, GetCurrentProcess(), &inherit, 0, TRUE, DUPLICATE_SAME_ACCESS);
    CloseHandle(nul);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = inherit;
    si.hStdError = inherit;
    PROCESS_INFORMATION pi{};
    (void)sa;
    const BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);
    const DWORD err = GetLastError();
    CloseHandle(inherit);
    if (!ok) {
        if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) throw py::FileNotFoundError(py::percent(S("[WinError %d] The system cannot find the file specified"), static_cast<int64_t>(err)));
        throw py::OSError(py::percent(S("[WinError %d] CreateProcess failed"), static_cast<int64_t>(err)));
    }
    CloseHandle(pi.hThread);
    handle_ = pi.hProcess;
}

Process::~Process() {
    if (handle_) CloseHandle(static_cast<HANDLE>(handle_));
}

std::optional<int64_t> Process::poll() {
    DWORD code = 0;
    if (!GetExitCodeProcess(static_cast<HANDLE>(handle_), &code)) return std::nullopt;
    if (code == STILL_ACTIVE) return std::nullopt;
    return static_cast<int64_t>(code);
}

void Process::kill() {
    if (poll()) return;
    TerminateProcess(static_cast<HANDLE>(handle_), 1);
}

bool Process::wait_timeout(double timeout) { return WaitForSingleObject(static_cast<HANDLE>(handle_), static_cast<DWORD>(timeout * 1000)) == WAIT_OBJECT_0; }

void Process::wait(double timeout) { wait_timeout(timeout); }

// ---- the monitor -----------------------------------------------------------------------------------------------------------------------------
void _Monitor::send(const py::list<str>& cmds) {
    winsock();
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) raise_socket("socket", WSAGetLastError());
    // socket.create_connection(("127.0.0.1", port), timeout=5)
    u_long nonblock = 1;
    ioctlsocket(s, FIONBIO, &nonblock);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_port = htons(static_cast<u_short>(port));
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    int r = connect(s, reinterpret_cast<sockaddr*>(&a), sizeof a);
    if (r != 0) {
        const int e = WSAGetLastError();
        if (e != WSAEWOULDBLOCK) {
            closesocket(s);
            throw ConnectionRefusedError(py::percent(S("[WinError %d] No connection could be made because the target machine actively refused it"), static_cast<int64_t>(e)));
        }
        fd_set wr, ex;
        FD_ZERO(&wr);
        FD_ZERO(&ex);
        FD_SET(s, &wr);
        FD_SET(s, &ex);
        timeval tv{5, 0};
        const int n = select(0, nullptr, &wr, &ex, &tv);
        if (n == 0) {
            closesocket(s);
            throw py::TimeoutError("timed out");
        }
        if (FD_ISSET(s, &ex)) {
            closesocket(s);
            throw ConnectionRefusedError("[WinError 10061] No connection could be made because the target machine actively refused it");
        }
    }
    nonblock = 0;
    ioctlsocket(s, FIONBIO, &nonblock);
    DWORD ms = 2000;   // s.settimeout(2)
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
    char buf[4096];
    recv(s, buf, sizeof buf, 0);   // (a timeout is no error here)
    for (const str& c : cmds) {
        const std::string line = c.utf8() + "\n";
        size_t sent = 0;
        while (sent < line.size()) {
            const int n = ::send(s, line.data() + sent, static_cast<int>(line.size() - sent), 0);
            if (n <= 0) {
                const int e = WSAGetLastError();
                closesocket(s);
                raise_socket("send", e);
            }
            sent += static_cast<size_t>(n);
        }
        py::time::sleep(0.3);
    }
    closesocket(s);
}

Image _Monitor::shot(const str& png) {
    const str ppm = png.slice(0, png.size() - 4) + S(".ppm");
    send(py::list<str>{S("screendump ") + ppm.replace(S("\\"), S("/"))});
    py::time::sleep(1.2);
    Image img = Image::open(ppm);
    img.save(png);
    py::os::remove(ppm);
    return img;
}

// ---- what is on the screen -----------------------------------------------------------------------------------------------------------------
bool _license(const Image& img) {
    if (std::get<0>(img.size()) != 640) return false;
    for (const auto& xy : LICENSE_POINTS)
        if (img.getpixel(std::get<0>(xy), std::get<1>(xy)) != LICENSE_GREY) return false;
    return true;
}

bool _error_box(const Image& img) {
    if (std::get<0>(img.size()) != 640) return false;
    for (const auto& xy : {std::make_tuple(97, 310), std::make_tuple(320, 175), std::make_tuple(540, 310)}) {
        const RGB p = img.getpixel(std::get<0>(xy), std::get<1>(xy));
        if (!(std::get<0>(p) + std::get<1>(p) + std::get<2>(p) > 700)) return false;
    }
    return true;
}

int64_t _welcome_count(const Image& img) {
    const Image small = img.resize(160, 120);
    int64_t n = 0;
    for (const RGB& p : small.getdata())
        if (std::get<0>(p) > 200 && std::get<1>(p) > 200 && std::get<2>(p) > 200) ++n;
    return n;
}

bool _welcome(const Image& img) {
    if (std::get<0>(img.size()) != 640) return false;
    return _welcome_count(img) > 2000;
}

bool _black(const Image& img) {
    const py::list<RGB> px = img.resize(90, 50).getdata();
    int64_t n = 0;
    for (const RGB& p : px)
        if (std::get<0>(p) + std::get<1>(p) + std::get<2>(p) < 60) ++n;
    return static_cast<double>(n) / static_cast<double>(px.size()) > 0.9;
}

// ---- the test --------------------------------------------------------------------------------------------------------------------------------
std::tuple<str, str> continue_test(const py::dict<str, str>& replace, const str& tag, int64_t wait, const std::optional<str>& keep_shot, const MakeCd& make_cd_fn) {
    const Paths p = _paths();
    const str shots = keep_shot && !keep_shot->empty() ? *keep_shot : base::runs::results(S("phase3"));
    py::os::makedirs(shots, 0777, true);
    const str png = py::os::path::join(shots, tag + S(".png"));
    const str work = base::runs::start(S("phase3"), tag);   // (the disk and CD copies: this run's alone)
    std::unique_ptr<Process> q;
    str verdict = S("SLOW");
    try {
        const str iso = py::os::path::join(work, S("cd.iso"));
        if (make_cd_fn) make_cd_fn(iso, replace);
        else make_cd(iso, replace);
        const str hd = py::os::path::join(work, S("hd.img"));
        py::shutil::copy(p.hd, hd);
        const int64_t port = _free_port();
        q = std::make_unique<Process>(py::list<str>{p.qemu, S("-M"), S("pc"), S("-m"), S("64"), S("-drive"),
                                                    py::percent(S("file=%s,format=raw,if=floppy,index=0,snapshot=on"), p.floppy), S("-drive"),
                                                    py::percent(S("file=%s,format=raw,if=ide,index=0"), hd), S("-cdrom"), iso, S("-boot"), S("a"), S("-display"),
                                                    S("none"), S("-monitor"), py::percent(S("tcp:127.0.0.1:%d,server,nowait"), port)});
        _Monitor mon(port);
        std::optional<Image> img;
        bool broke = false;
        for (int64_t i = 0; i < 60; ++i) {   // (to the Welcome dialog)
            py::time::sleep(4);
            if (q->poll()) {
                broke = true;
                break;
            }
            try {
                img = mon.shot(png);
            } catch (const py::OSError&) {
                continue;
            } catch (const py::ValueError&) {
                continue;
            }
            if (_error_box(*img)) {   // (white like the Welcome dialog: not it)
                verdict = S("FAIL(error box before Welcome)");
                broke = true;
                break;
            }
            if (_welcome(*img)) {
                py::time::sleep(8);
                mon.send(py::list<str>{S("sendkey ret")});
                verdict = S("FAIL(timeout)");
                int64_t bad = 0;
                const double until = py::time::time() + static_cast<double>(wait);
                while (py::time::time() < until) {
                    py::time::sleep(3);
                    try {
                        img = mon.shot(png);
                    } catch (const py::OSError&) {
                        continue;
                    } catch (const py::ValueError&) {
                        continue;
                    }
                    if (std::get<0>(img->size()) == 720) {
                        verdict = S("FAIL(dos)");
                        break;
                    }
                    if (_license(*img)) {
                        verdict = S("PASS");
                        break;
                    }
                    bad = _error_box(*img) ? bad + 1 : 0;
                    if (bad >= 2) {
                        verdict = S("FAIL(error box)");
                        break;
                    }
                }
                broke = true;
                break;
            }
            // (no keys while Setup's DOS part runs: it asks for none from a prepared disk, and an Enter it did not need reached Windows and hid a
            // fault -- USER's 1:007F)
        }
        if (!broke && img)
            verdict = _black(*img) ? S("FAIL(dos)") : std::get<0>(img->size()) == 640 ? S("FAIL(hang)") : S("SLOW");
    } catch (...) {
        if (q) {
            q->kill();
            q->wait_timeout(20);
        }
        base::runs::remove(work);   // (QEMU lets go of its files a moment later)
        throw;
    }
    if (q) {
        q->kill();
        q->wait_timeout(20);
    }
    base::runs::remove(work);   // (QEMU lets go of its files a moment later)
    return {verdict, png};
}

}  // namespace codebreaker::verify::gate16::setup16
