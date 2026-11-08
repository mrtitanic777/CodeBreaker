// codebreaker/formats/xbox/xwb.hpp -- typed C++ from codebreaker.formats.xbox.xwb.
//
// XACT Wave Bank (.xwb) extractor -- Xbox 360 game audio.
//
// Parses XWB wave banks (the XACT container games ship audio in) and extracts
// each embedded stream with its format decoded. PCM streams are written as real
// .wav files; XMA / ADPCM / WMA streams are extracted raw with a printed format
// table (full XMA -> PCM decode needs the proprietary codec -- feed the raw .xma
// to vgmstream/ffmpeg). Big-endian (Xbox 360) and little-endian banks are both
// handled via version-sanity endian detection.
//
// Layout facts (clean-room from the documented XACT format / MonoGame WaveBank):
//   header:   'WBND', version, [header-version], segment table (off,len x N)
//   seg[0]    WaveBankData: flags, entry_count, name[64], meta_elem_size, ...
//   seg[1]    per-entry metadata (FlagsDuration, Format, PlayRegion off+len, loop)
//   seg[last] ENTRYWAVEDATA
//   Format:   codec=fmt&3, channels=(fmt>>2)&7, rate=(fmt>>5)&0x3FFFF, align=(fmt>>23)&0xFF
//
// Commands:
//     xwb info    <bank.xwb>              header + per-entry format table
//     xwb extract <bank.xwb> [outdir]     dump every stream (PCM->wav, else raw)
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::xbox::xwb {

extern const py::dict<int64_t, py::str> CODECS;
extern const py::dict<int64_t, py::str> _EXT;

// One wave of the bank (the dict XWB._decode makes in Python).
struct Entry {
    int64_t index = 0;
    int64_t codec = 0;
    py::str codec_name;
    int64_t channels = 0;
    int64_t rate = 0;
    int64_t align = 0;
    int64_t offset = 0;
    int64_t length = 0;
};
template <class F> void for_each_field_mut(Entry& e, F&& f) {
    f("index", e.index, true);
    f("codec", e.codec, true);
    f("codec_name", e.codec_name, true);
    f("channels", e.channels, true);
    f("rate", e.rate, true);
    f("align", e.align, true);
    f("offset", e.offset, true);
    f("length", e.length, true);
}

class XWB {
  public:
    py::bytes d;
    py::str en = py::str(">");   // ">" (big-endian, the Xbox 360's) or "<"
    int64_t version = 0;
    std::optional<int64_t> header_version;   // only banks of version 42 and later have one
    py::list<std::tuple<int64_t, int64_t>> segments;   // (offset, length)
    int64_t flags = 0;
    int64_t entry_count = 0;
    py::str bank_name;
    int64_t meta_elem_size = 0;
    int64_t name_elem_size = 0;
    int64_t alignment = 0;
    int64_t wavedata_off = 0;
    py::list<Entry> entries;

    explicit XWB(const py::bytes& data);
    int64_t _u32(int64_t o) const;
    void _parse();
    Entry _decode(int64_t i, int64_t fmt, int64_t play_off, int64_t play_len) const;
    py::bytes stream(const Entry& e) const;
    int64_t extract(const py::str& outdir) const;
};

// Wrap raw PCM in a RIFF/WAVE header. Xbox 360 PCM is big-endian, so the
// samples are byte-swapped to little-endian WAV order.
py::bytes pcm_to_wav(const py::bytes& pcm, int64_t channels, int64_t rate, int64_t bits = 16);

extern const py::str DESCRIPTION;

// main(): the command line without the program name (sys.argv[1:]); prints to stdout.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::xwb
