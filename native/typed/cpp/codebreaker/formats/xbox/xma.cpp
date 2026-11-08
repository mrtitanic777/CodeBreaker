// codebreaker/formats/xbox/xma.cpp -- C++17 port of codebreaker.formats.xbox.xma (see the header).

#include "codebreaker/formats/xbox/xma.hpp"

#include <filesystem>
#include <random>

#include "py_settings.hpp"
#include "py_subprocess.hpp"
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
// the program run with stdout and stderr into one pipe: its exit code and its output, as text
// (subprocess.run(argv, stdout=PIPE, stderr=STDOUT, text=True): cp1252, universal newlines; stdin is ours)
static std::pair<int64_t, py::str> run_process_(const py::list<py::str>& argv) {
    py::subprocess::RunOptions o;
    o.stdin_ = py::subprocess::Stream::Inherit;
    o.stderr_ = py::subprocess::Stream::Stdout;
    py::subprocess::CompletedProcess r = py::subprocess::run(argv, o);
    return {r.returncode, *r.stdout_};
}
#endif

using py::tempfile::TemporaryDirectory;

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
