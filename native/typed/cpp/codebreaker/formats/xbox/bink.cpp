// codebreaker/formats/xbox/bink.cpp -- C++ from codebreaker.formats.xbox.bink (see bink.hpp).

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "codebreaker/formats/xbox/bink.hpp"

#include <windows.h>

#include <charconv>
#include <cstdio>

namespace codebreaker::formats::xbox::bink {

const py::str DESCRIPTION("Xbox 360 Bink video (.bik): native header info + ffmpeg-driven decode/extract");

namespace {

py::list<py::bytes> bink1_() {
    py::list<py::bytes> out;
    for (const char* m : {"BIKb", "BIKd", "BIKf", "BIKg", "BIKh", "BIKi", "BIKk"}) {
        out.append(py::bytes(m));
    }
    return out;
}

py::list<py::bytes> bink2_() {
    py::list<py::bytes> out;
    for (int64_t c = 'a'; c <= 'z'; ++c) {
        out.append(py::bytes(std::string("KB2") + static_cast<char>(c)));
    }
    return out;
}

// round(x, ndigits): the correctly rounded decimal of x's exact value, read back as a float
double round_digits_(double x, int ndigits) {
    char buf[512];
    std::snprintf(buf, sizeof buf, "%.*f", ndigits, x);
    double r = 0;
    std::from_chars(buf, buf + std::strlen(buf), r);
    return r;
}

// settings.tool(key): a program -- the one settings.json names under `key`, else one on PATH called `key`.
// -> its path, or None.
std::optional<py::str> tool_(const py::str& key) {
    py::str p = py::settings::get_str(key);
    if (!p.empty() && (py::os::path::exists(p) || py::shutil::which(p))) {
        return p;
    }
    return py::shutil::which(key);
}

// subprocess.list2cmdline
std::wstring list2cmdline_(const py::list<py::str>& args) {
    std::u32string result;
    bool first = true;
    for (const py::str& arg : args) {
        const std::u32string& a = arg.u32();
        std::u32string bs;
        if (!first) {
            result.push_back(U' ');
        }
        first = false;
        bool needquote = a.find(U' ') != std::u32string::npos || a.find(U'\t') != std::u32string::npos || a.empty();
        if (needquote) {
            result.push_back(U'"');
        }
        for (char32_t c : a) {
            if (c == U'\\') {
                bs.push_back(c);
            } else if (c == U'"') {
                result.append(bs.size() * 2, U'\\');
                bs.clear();
                result += U"\\\"";
            } else {
                result += bs;
                bs.clear();
                result.push_back(c);
            }
        }
        result += bs;
        if (needquote) {
            result += bs;
            result.push_back(U'"');
        }
    }
    std::string utf8 = py::str(result).utf8();
    int n = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), w.data(), n);
    return w;
}

// subprocess.run(args, stdout=PIPE, stderr=STDOUT, text=True) -> (returncode, stdout)
std::tuple<int64_t, py::str> run_(const py::list<py::str>& args) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE rd = nullptr, wr = nullptr;
    if (!CreatePipe(&rd, &wr, &sa, 0)) {
        throw py::OSError("CreatePipe failed");
    }
    SetHandleInformation(rd, HANDLE_FLAG_INHERIT, 0);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = wr;
    si.hStdError = wr;
    PROCESS_INFORMATION pi{};
    std::wstring cmd = list2cmdline_(args);
    if (!CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi)) {
        DWORD e = GetLastError();
        CloseHandle(rd);
        CloseHandle(wr);
        std::string path = args[0].utf8();
        if (e == 2 || e == 3) {
            throw py::FileNotFoundError("[WinError " + std::to_string(e) + "] The system cannot find the file specified");
        }
        throw py::OSError("[WinError " + std::to_string(e) + "] cannot start " + path);
    }
    CloseHandle(wr);
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
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    // text=True: the locale's codec (cp1252) and universal newlines
    std::u32string text = py::bytes(raw).decode("cp1252").u32();
    std::u32string out;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == U'\r') {
            out.push_back(U'\n');
            if (i + 1 < text.size() && text[i + 1] == U'\n') {
                ++i;
            }
        } else {
            out.push_back(text[i]);
        }
    }
    return {static_cast<int64_t>(static_cast<int32_t>(code)), py::str(out)};
}

}  // namespace

const py::list<py::bytes> _BINK1 = bink1_();
const py::list<py::bytes> _BINK2 = bink2_();

bool is_bink(const py::bytes& data) {
    py::bytes m = data.slice(0, 4);
    return _BINK1.contains(m) || _BINK2.contains(m);
}

BinkHeader read_header(const py::str& path) {
    py::bytes head;
    {
        py::file f = py::file::open(path, py::str("rb"));
        head = f.read(64);
        f.close();
    }
    py::bytes magic = head.slice(0, 4);
    if (!is_bink(head)) {
        throw py::ValueError(py::percent(py::str("not a Bink file (magic %r)"), magic).utf8());
    }
    int64_t gen = _BINK2.contains(magic) ? 2 : 1;
    auto [file_size, frames, largest, _frames2, width, height, fps_num, fps_den, flags, ntracks] =
        py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>("<10I", head, 4);
    double fps = fps_den ? py::truediv(fps_num, fps_den) : 0.0;
    BinkHeader h;
    h.generation = gen;
    h.codec = magic.decode("latin-1");
    h.file_size = file_size + 8;
    h.frames = frames;
    h.largest_frame = largest;
    h.width = width;
    h.height = height;
    h.fps = round_digits_(fps, 3);
    h.video_flags = py::percent(py::str("0x%08X"), flags);
    h.audio_tracks = ntracks;
    return h;
}

int64_t decode(const py::str& bik_path, const py::str& out_path, bool audio_only, bool video_only) {
    std::optional<py::str> exe = tool_(py::str("ffmpeg"));
    if (!exe || exe->empty()) {
        throw py::RuntimeError(
            "ffmpeg not found \xE2\x80\x94 install it or run: "
            "set \"ffmpeg\" in settings.json");
    }
    py::list<py::str> args{*exe, py::str("-y"), py::str("-hide_banner"), py::str("-loglevel"), py::str("error"), py::str("-i"), bik_path};
    if (audio_only) {
        args.extend(py::list<py::str>{py::str("-vn")});
    }
    if (video_only) {
        args.extend(py::list<py::str>{py::str("-an")});
    }
    args.extend(py::list<py::str>{out_path});
    auto [returncode, output] = run_(args);
    if (returncode != 0) {
        throw py::RuntimeError(py::percent(py::str("ffmpeg Bink decode failed:\n%s"), output.strip()).utf8());
    }
    return py::os::path::exists(out_path) ? py::os::path::getsize(out_path) : 0;
}

int64_t main(const py::list<py::str>& argv) {
    py::list<py::str> a = argv.slice(1, py::len(argv));
    if (!py::len(a) || a[0] == "-h" || a[0] == "--help") {
        py::print(py::str(
            "usage:\n"
            "  bink info   <file.bik>                     native header summary\n"
            "  bink decode <file.bik> <out.mp4|.wav|...>  ffmpeg decode (ext picks codec)\n"
            "     --audio  audio track only   |   --video  video only"));
        return 0;
    }
    py::str op = a[0];
    if (op == "info") {
        BinkHeader h = read_header(a[1]);
        py::print(py::percent(py::str("Bink%d (%s): %s"), h.generation, h.codec, a[1]));
        py::print(py::percent(py::str("  %d x %d  @ %.3f fps   %d frames   %d audio track(s)"), h.width, h.height, h.fps, h.frames,
                              h.audio_tracks));
        py::print(py::percent(py::str("  file size %d bytes  (largest frame %d)  flags %s"), h.file_size, h.largest_frame, h.video_flags));
        return 0;
    }
    if (op == "decode" && py::len(a) >= 3) {
        int64_t n = decode(a[1], a[2], a.contains(py::str("--audio")), a.contains(py::str("--video")));
        py::print(py::percent(py::str("decoded -> %s (%d bytes)"), a[2], n));
        return 0;
    }
    py::print(py::str("unknown/short command; run 'bink' for usage"));
    return 2;
}

}  // namespace codebreaker::formats::xbox::bink
