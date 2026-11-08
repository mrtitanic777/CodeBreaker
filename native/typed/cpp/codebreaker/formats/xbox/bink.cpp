// codebreaker/formats/xbox/bink.cpp -- C++ from codebreaker.formats.xbox.bink (see bink.hpp).

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "codebreaker/formats/xbox/bink.hpp"

#include "py_subprocess.hpp"

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

// subprocess.run(args, stdout=PIPE, stderr=STDOUT, text=True) -> (returncode, stdout)
std::tuple<int64_t, py::str> run_(const py::list<py::str>& args) {
    py::subprocess::RunOptions o;
    o.stderr_ = py::subprocess::Stream::Stdout;
    py::subprocess::CompletedProcess r = py::subprocess::run(args, o);
    return {r.returncode, *r.stdout_};
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
