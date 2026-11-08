// codebreaker/formats/xbox/xma.cpp -- C++17 port of codebreaker.formats.xbox.xma (see the header).

#include "codebreaker/formats/xbox/xma.hpp"

#include <filesystem>
#include <random>

#include "py_settings.hpp"
#include "codebreaker/formats/xbox/xwb.hpp"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace codebreaker::formats::xbox::xma {

namespace xwb = codebreaker::formats::xbox::xwb;

const py::str DESCRIPTION = "Xbox 360 XMA/XMA2 audio -> playable WAV (native RIFF-XMA2 wrap + ffmpeg decode)";

const int64_t WAVE_FORMAT_XMA2 = 0x0166;

py::bytes wrap_xma2(const py::bytes& data, int64_t channels, int64_t rate, int64_t block) {
    channels = channels ? channels : 2;
    rate = rate ? rate : 44100;
    int64_t n_streams = py::floordiv(channels + 1, 2);
    int64_t block_count = py::floordiv(py::len(data) + block - 1, block);
    py::bytes fmtx = py::pack("<HHIIHH", WAVE_FORMAT_XMA2, channels, rate, rate * channels * 2, channels * 2, int64_t(16));
    py::bytes ext = py::pack("<HHIIIIIIIBBH", int64_t(34), n_streams, int64_t(0), int64_t(0), block, int64_t(0), int64_t(0),
                             int64_t(0), int64_t(0), int64_t(0), int64_t(4), block_count);   // XMA2WAVEFORMATEX tail
    py::bytes fmt_chunk = fmtx + ext;
    py::bytes body = py::bytes("WAVE") + py::bytes("fmt ") + py::pack("<I", py::len(fmt_chunk)) + fmt_chunk +
                     py::bytes("data") + py::pack("<I", py::len(data)) + data;
    return py::bytes("RIFF") + py::pack("<I", py::len(body)) + body;
}

// settings.tool(key, name=None): the program settings.json (or CB_<KEY>) names under `key`, else one on
// PATH called `name` (default: `key`). -> its path, or None.
static std::optional<py::str> tool(const py::str& key) {
    py::str p = py::settings::get_str(key);
    if (py::truthy(p) && (py::os::path::exists(p) || py::shutil::which(p))) {
        return p;
    }
    return py::shutil::which(key);
}

py::str _need_ffmpeg() {
    std::optional<py::str> exe = tool(py::str("ffmpeg"));
    if (!exe) {
        throw py::RuntimeError(
            "ffmpeg not found \xE2\x80\x94 install it and/or run: "
            "set \"ffmpeg\" in settings.json");
    }
    return *exe;
}

#ifdef _WIN32
// subprocess.list2cmdline (including its habit of doubling a quoted argument's trailing backslashes)
static std::u32string list2cmdline_(const py::list<py::str>& argv) {
    std::u32string result;
    for (const py::str& arg : argv) {
        std::u32string bs_buf;
        if (!result.empty()) {
            result.push_back(U' ');
        }
        bool needquote = arg.contains(py::str(" ")) || arg.contains(py::str("\t")) || arg.empty();
        if (needquote) {
            result.push_back(U'"');
        }
        for (char32_t c : arg.u32()) {
            if (c == U'\\') {
                bs_buf.push_back(c);
            } else if (c == U'"') {
                result.append(bs_buf.size() * 2, U'\\');
                bs_buf.clear();
                result += U"\\\"";
            } else {
                result += bs_buf;
                bs_buf.clear();
                result.push_back(c);
            }
        }
        if (!bs_buf.empty()) {
            result += bs_buf;
        }
        if (needquote) {
            result += bs_buf;
            result.push_back(U'"');
        }
    }
    return result;
}

static std::wstring wide_(const std::u32string& s) {
    std::wstring w;
    for (char32_t c : s) {
        if (c >= 0x10000) {
            char32_t v = c - 0x10000;
            w.push_back(static_cast<wchar_t>(0xD800 + (v >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + (v & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    return w;
}

// the program run with stdout and stderr into one pipe: its exit code and its output, as text
// (cp1252, universal newlines)
static std::pair<int64_t, py::str> run_process_(const py::list<py::str>& argv) {
    std::wstring cmd = wide_(list2cmdline_(argv));
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof sa;
    sa.bInheritHandle = TRUE;
    HANDLE rd = nullptr, wr = nullptr;
    if (!CreatePipe(&rd, &wr, &sa, 0)) {
        throw py::OSError("CreatePipe failed");
    }
    SetHandleInformation(rd, HANDLE_FLAG_INHERIT, 0);
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (in && in != INVALID_HANDLE_VALUE) {
        SetHandleInformation(in, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    }
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in;
    si.hStdOutput = wr;
    si.hStdError = wr;
    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcessW(nullptr, &cmd[0], nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);
    DWORD err = GetLastError();
    CloseHandle(wr);
    if (!ok) {
        CloseHandle(rd);
        if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) {
            throw py::FileNotFoundError("[WinError " + std::to_string(err) + "] The system cannot find the file specified");
        }
        if (err == ERROR_ACCESS_DENIED) {
            throw py::PermissionError("[WinError 5] Access is denied");
        }
        throw py::OSError("[WinError " + std::to_string(err) + "] CreateProcess failed");
    }
    std::string raw;
    char buf[4096];
    DWORD got = 0;
    while (ReadFile(rd, buf, sizeof buf, &got, nullptr) && got > 0) {
        raw.append(buf, got);
    }
    CloseHandle(rd);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    py::str text = py::bytes(raw).decode("cp1252");
    std::u32string out;
    const std::u32string& u = text.u32();
    for (size_t i = 0; i < u.size(); ++i) {
        if (u[i] == U'\r') {
            out.push_back(U'\n');
            if (i + 1 < u.size() && u[i + 1] == U'\n') {
                ++i;
            }
        } else {
            out.push_back(u[i]);
        }
    }
    return {static_cast<int64_t>(code), py::str(out)};
}
#endif

// tempfile.TemporaryDirectory(): a new folder under the temp folder, removed (with what is in it) when this goes
struct TemporaryDirectory {
    py::str name;
    TemporaryDirectory() {
        std::filesystem::path base = std::filesystem::temp_directory_path();
        std::random_device rd;
        static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789_";
        for (;;) {
            std::string leaf = "tmp";
            for (int i = 0; i < 8; ++i) leaf.push_back(alphabet[rd() % (sizeof alphabet - 1)]);
            std::filesystem::path p = base / leaf;
            std::error_code ec;
            if (std::filesystem::create_directory(p, ec)) {
                name = py::str(p.u8string());
                return;
            }
            if (ec) throw py::OSError("cannot create a temporary folder: " + ec.message());
        }
    }
    ~TemporaryDirectory() {
        std::error_code ec;
        std::filesystem::remove_all(std::filesystem::u8path(name.utf8()), ec);
    }
    TemporaryDirectory(const TemporaryDirectory&) = delete;
    TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;
};

int64_t decode_to_wav(const py::bytes& xma_or_riff, const py::str& out_wav, int64_t channels, int64_t rate, int64_t block) {
    py::str exe = _need_ffmpeg();
    py::bytes riff = xma_or_riff.slice(0, 4) == py::bytes("RIFF") ? xma_or_riff : wrap_xma2(xma_or_riff, channels, rate, block);
    TemporaryDirectory folder;
    py::str tmp = py::os::path::join(folder.name, py::str("in.xma.wav"));
    {
        py::file f = py::open(tmp, py::str("wb"));
        f.write(riff);
        f.close();
    }
    // 16-bit PCM in a plain WAVEFORMATEX: with the decoder's own layout ffmpeg writes WAVE_FORMAT_EXTENSIBLE,
    // which MonoGame's (and many players') WAV readers do not take
    py::list<py::str> argv;
    argv.append(exe);
    for (const char* s : {"-y", "-hide_banner", "-loglevel", "error", "-i"}) argv.append(py::str(s));
    argv.append(tmp);
    for (const char* s : {"-c:a", "pcm_s16le", "-ac"}) argv.append(py::str(s));
    argv.append(py::str(std::to_string(py::min(channels, int64_t(2)))));
    argv.append(out_wav);
#ifdef _WIN32
    auto [returncode, stdout_text] = run_process_(argv);
#else
    throw py::NotImplementedError("xma.decode_to_wav: running a program is implemented for Windows only");
    int64_t returncode = 0;
    py::str stdout_text;
#endif
    if (returncode != 0 || !py::os::path::exists(out_wav)) {
        throw py::RuntimeError(py::str("ffmpeg XMA2 decode failed:\n").utf8() + py::or_(stdout_text, py::str("")).strip().utf8());
    }
    return py::os::path::getsize(out_wav);
}

int64_t decode_bank(const py::str& xwb_path, const py::str& out_dir) {
    xwb::XWB w(py::open_rb(xwb_path).read());
    py::os::makedirs(out_dir, 0777, true);
    int64_t n = 0;
    for (const xwb::Entry& e : w.entries) {
        py::bytes data = w.stream(e);
        py::str out = py::os::path::join(out_dir, py::percent(py::str("stream_%03d.wav"), e.index));
        if (e.codec_name == "PCM") {
            py::file f = py::open(out, py::str("wb"));
            f.write(xwb::pcm_to_wav(data, e.channels ? e.channels : 2, e.rate ? e.rate : 44100));
            f.close();
        } else if (e.codec_name == "XMA") {
            try {
                decode_to_wav(data, out, e.channels ? e.channels : 2, e.rate ? e.rate : 44100);
            } catch (const py::Exception& ex) {
                py::file f = py::open(py::os::path::join(out_dir, py::percent(py::str("stream_%03d.xma"), e.index)), py::str("wb"));
                f.write(data);
                f.close();
                py::print(py::percent(py::str("  stream %d: XMA decode failed (%s); wrote raw .xma"), e.index,
                                      py::str(std::string(ex.what()))));
                continue;
            }
        } else {
            py::file f = py::open(py::os::path::join(out_dir, py::percent(py::str("stream_%03d.%s"), e.index, e.codec_name.lower())),
                                  py::str("wb"));
            f.write(data);
            f.close();
            continue;
        }
        n += 1;
    }
    return n;
}

void main(const py::list<py::str>& sys_argv) {
    py::list<py::str> a = sys_argv.slice_from(1);
    if (!py::truthy(a) || a[0] == "-h" || a[0] == "--help") {
        py::print(py::str("usage:\n"
                          "  xma wrap   <raw.xma> <out.wav> --channels N --rate R   RIFF-XMA2 (native)\n"
                          "  xma decode <raw.xma> <out.wav> --channels N --rate R   -> PCM WAV (ffmpeg)\n"
                          "  xma bank   <bank.xwb> <outdir>                         extract+decode a wave bank"));
        return;
    }
    int64_t ch = a.contains(py::str("--channels")) ? py::int_from_str(a[a.index(py::str("--channels")) + 1]) : 2;
    int64_t rate = a.contains(py::str("--rate")) ? py::int_from_str(a[a.index(py::str("--rate")) + 1]) : 44100;
    py::str op = a[0];
    if (op == "wrap" && py::len(a) >= 3) {
        {
            py::file out = py::open(a[2], py::str("wb"));   // (opened first, as the Python's one expression does)
            py::bytes raw = py::open_rb(a[1]).read();
            out.write(wrap_xma2(raw, ch, rate));
            out.close();
        }
        py::print(py::percent(py::str("wrote RIFF-XMA2 -> %s"), a[2]));
        return;
    }
    if (op == "decode" && py::len(a) >= 3) {
        py::bytes raw = py::open_rb(a[1]).read();
        int64_t n = decode_to_wav(raw, a[2], ch, rate);
        py::print(py::percent(py::str("decoded -> %s (%d bytes PCM WAV)"), a[2], n));
        return;
    }
    if (op == "bank" && py::len(a) >= 3) {
        int64_t n = decode_bank(a[1], a[2]);
        py::print(py::percent(py::str("wrote %d WAV file(s) -> %s"), n, a[2]));
        return;
    }
    py::print(py::str("unknown/short command; run 'xma' for usage"));
    throw SystemExit{2};
}

}  // namespace codebreaker::formats::xbox::xma
