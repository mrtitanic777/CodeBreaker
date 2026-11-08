// codebreaker/formats/xbox/xma.hpp -- C++17 port of codebreaker.formats.xbox.xma.
//
// XMA / XMA2 audio -- the Xbox 360 game audio codec -- to playable WAV.
//
// We already extract raw XMA streams from XACT wave banks (`xwb`); this wraps a
// raw XMA2 stream in the standard RIFF-WAVE(XMA2) container natively, and decodes
// it to PCM WAV via ffmpeg (found through `config`/PATH -- driven not embedded,
// like chdman: the XMA2 codec is a WMA-Pro derivative and ffmpeg maintains it).
//
// `xma bank` turns a whole `.xwb` wave bank into playable .wav files in one step.
//
// Commands: wrap / decode / bank.
//
// Notes on the port (the same ones the chd port makes):
//   * the setting `ffmpeg` is read through py::settings::get_str (py_settings.hpp);
//   * decode_to_wav starts ffmpeg with the Windows API (subprocess.run with stdout=PIPE,
//     stderr=STDOUT, text=True): its output is decoded as cp1252 (the locale codec of the Python it
//     was ported from) with universal newlines;
//   * tempfile.TemporaryDirectory is a folder made under the system temp folder and removed at the
//     end of the call (tempfile has no runtime port);
//   * main() takes sys.argv as its argument (the program name first); sys.exit(2) is SystemExit;
//   * channels / rate / block are int64_t: a Python caller's None for them is not representable
//     (the Python only ever passes ints or the defaults).
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::xma {

extern const py::str DESCRIPTION;

extern const int64_t WAVE_FORMAT_XMA2;

// sys.exit(code): not a py::Exception (it is not an Exception in Python either)
using py::SystemExit;

// Wrap a raw XMA2 stream in a RIFF-WAVE(XMA2) container (native, no deps).
// The result is decodable by ffmpeg / any XMA2-aware player.
py::bytes wrap_xma2(const py::bytes& data, int64_t channels, int64_t rate, int64_t block = 0x10000);

// the ffmpeg program, or RuntimeError when there is none
py::str _need_ffmpeg();

// Decode a raw XMA2 stream (or an already-wrapped RIFF-XMA2) to PCM WAV via
// ffmpeg. Returns the output size in bytes.
int64_t decode_to_wav(const py::bytes& xma_or_riff, const py::str& out_wav, int64_t channels = 2, int64_t rate = 44100,
                      int64_t block = 0x10000);

// Extract every stream from an .xwb wave bank and write playable .wav:
// PCM streams verbatim, XMA streams decoded via ffmpeg. Returns file count.
int64_t decode_bank(const py::str& xwb_path, const py::str& out_dir);

// `sys_argv`: sys.argv, the program name first. Throws SystemExit where the Python calls sys.exit.
void main(const py::list<py::str>& sys_argv);

}  // namespace codebreaker::formats::xbox::xma
