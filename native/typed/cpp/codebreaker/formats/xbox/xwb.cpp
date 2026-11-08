// codebreaker/formats/xbox/xwb.cpp -- typed C++ from codebreaker.formats.xbox.xwb.

#include "codebreaker/formats/xbox/xwb.hpp"

#include "py_os.hpp"

namespace codebreaker::formats::xbox::xwb {

const py::dict<int64_t, py::str> CODECS = {{0, py::str("PCM")}, {1, py::str("XMA")}, {2, py::str("ADPCM")}, {3, py::str("WMA")}};
const py::dict<int64_t, py::str> _EXT = {{0, py::str("wav")}, {1, py::str("xma")}, {2, py::str("adpcm")}, {3, py::str("wma")}};
const py::str DESCRIPTION("extract Xbox 360 XACT wave banks (.xwb): PCM -> wav, XMA/ADPCM raw");

XWB::XWB(const py::bytes& data) {
    if (!(data.slice(0, 4) == py::bytes("WBND") || data.slice(0, 4) == py::bytes("DNBW")))
        throw py::ValueError(py::percent(py::str("not an XWB (magic %r)"), data.slice(0, 4)));
    d = data;
    // endian detection: the version is a small number in the correct order
    bool found = false;
    for (const char* e : {">I", "<I"}) {
        int64_t ver = py::unpack_from1<int64_t>(e, data, 4);
        if (1 <= ver && ver <= 0x10000) {
            en = py::str(std::string(1, e[0]));
            version = ver;
            found = true;
            break;
        }
    }
    if (!found) throw py::ValueError("cannot determine XWB endianness");
    _parse();
}

int64_t XWB::_u32(int64_t o) const {
    return py::unpack_from1<int64_t>(en == py::str(">") ? ">I" : "<I", d, o);
}

void XWB::_parse() {
    bool big = en == py::str(">");
    int64_t o = 8;
    if (version >= 42) {
        header_version = _u32(o);
        o += 4;
    }
    int64_t nseg = version >= 42 ? 5 : 4;
    segments = py::list<std::tuple<int64_t, int64_t>>{};
    for (int64_t k = 0; k < nseg; ++k) {
        segments.append(py::unpack_from<int64_t, int64_t>(big ? ">II" : "<II", d, o));
        o += 8;
    }
    // WaveBankData @ seg[0]
    int64_t b = std::get<0>(segments[0]);
    flags = _u32(b);
    entry_count = _u32(b + 4);
    int64_t name_len = version >= 42 ? 64 : 16;
    // d[b+8:b+8+name_len].split(b"\x00")[0].decode("ascii", "replace")
    py::bytes raw_name = d.slice(b + 8, b + 8 + name_len);
    int64_t nul = raw_name.find(py::bytes("\0", 1));
    bank_name = (nul < 0 ? raw_name : raw_name.slice(0, nul)).decode("ascii", "replace");
    int64_t p = b + 8 + name_len;
    meta_elem_size = _u32(p);
    name_elem_size = _u32(p + 4);
    alignment = _u32(p + 8);
    // per-entry metadata @ seg[1]
    wavedata_off = std::get<0>(segments[-1]);
    entries = py::list<Entry>{};
    int64_t meta = std::get<0>(segments[1]);
    int64_t esz = meta_elem_size ? meta_elem_size : 24;
    for (int64_t i = 0; i < entry_count; ++i) {
        int64_t eo = meta + i * esz;
        int64_t fmt = _u32(eo + 4);
        int64_t play_off = _u32(eo + 8);
        int64_t play_len = _u32(eo + 12);
        entries.append(_decode(i, fmt, play_off, play_len));
    }
}

Entry XWB::_decode(int64_t i, int64_t fmt, int64_t play_off, int64_t play_len) const {
    int64_t codec = fmt & 0x3;
    Entry e;
    e.index = i;
    e.codec = codec;
    e.codec_name = CODECS.get(codec, py::str("?"));
    e.channels = (fmt >> 2) & 0x7;
    e.rate = (fmt >> 5) & 0x3FFFF;
    e.align = (fmt >> 23) & 0xFF;
    e.offset = play_off;
    e.length = play_len;
    return e;
}

py::bytes XWB::stream(const Entry& e) const {
    int64_t s = wavedata_off + e.offset;
    return d.slice(s, s + e.length);
}

int64_t XWB::extract(const py::str& outdir) const {
    py::os::makedirs(outdir, 0777, true);
    int64_t n = 0;
    for (const Entry& e : entries) {
        py::bytes raw = stream(e);
        py::str name = py::percent(py::str("%s_%03d.%s"), bank_name.empty() ? py::str("bank") : bank_name, e.index,
                                   _EXT.get(e.codec, py::str("bin")));
        py::str path = py::os::path::join(outdir, name);
        if (e.codec == 0) {   // PCM -> real WAV
            py::open(path, py::str("wb")).write(pcm_to_wav(raw, e.channels ? e.channels : 2, e.rate ? e.rate : 44100, 16));
        } else {
            py::open(path, py::str("wb")).write(raw);
        }
        n += 1;
    }
    return n;
}

// a * b as Python's unbounded ints would have it, None (nullopt) when it is past int64: such a value
// cannot be packed into the header's fields, and 0 times it is still 0
static std::optional<int64_t> mul_(const std::optional<int64_t>& a, int64_t b) {
    if (b == 0) return 0;
    if (!a) return std::nullopt;
    try {
        return py::checked_mul(*a, b);
    } catch (const py::OverflowError&) {
        return std::nullopt;
    }
}

py::bytes pcm_to_wav(const py::bytes& pcm_in, int64_t channels, int64_t rate, int64_t bits) {
    py::bytes pcm = pcm_in;
    if (bits == 16) {
        // b[0::2], b[1::2] = b[1::2], b[0::2]   # BE -> LE samples
        if (py::len(pcm) % 2 != 0)
            throw py::ValueError(py::percent(py::str("attempt to assign bytes of size %d to extended slice of size %d"),
                                             py::len(pcm) / 2, (py::len(pcm) + 1) / 2));
        std::string b = pcm.raw();
        for (size_t i = 0; i + 1 < b.size(); i += 2) std::swap(b[i], b[i + 1]);
        pcm = py::bytes(b);
    }
    std::optional<int64_t> rc = mul_(rate, channels);
    std::optional<int64_t> byte_rate_wide = mul_(rc, bits);
    std::optional<int64_t> block_wide = mul_(channels, bits);
    // (a value past int64 is past the fields' range too: the pack below fails as Python's does)
    int64_t byte_rate = byte_rate_wide ? py::floordiv(*byte_rate_wide, 8) : -1;
    int64_t block_align = block_wide ? py::floordiv(*block_wide, 8) : -1;
    // fmt chunk body (16 bytes): audioFormat, channels, rate, byteRate, blockAlign, bits
    py::bytes fmt = py::pack("<HHIIHH", 1, channels, rate, byte_rate, block_align, bits);
    return py::bytes("RIFF") + py::pack("<I", 36 + py::len(pcm)) + py::bytes("WAVE") + py::bytes("fmt ") +
           py::pack("<I", 16) + fmt + py::bytes("data") + py::pack("<I", py::len(pcm)) + pcm;
}

// ------------------------------------------------------------------------ CLI
void main(const py::list<py::str>& a) {
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        py::print(py::str("usage: xwb {info|extract} <bank.xwb> [outdir]"));
        return;
    }
    bool is_op = a[0] == py::str("info") || a[0] == py::str("extract");
    py::str op = is_op ? a[0] : py::str("info");
    py::str path = is_op ? a[1] : a[0];
    XWB x(py::open_rb(path).read());
    if (op == py::str("info")) {
        py::print(py::percent(py::str("bank        : %s"), x.bank_name.empty() ? py::str("(unnamed)") : x.bank_name));
        py::print(py::percent(py::str("version     : %d  endian: %s  entries: %d"), x.version,
                              x.en == py::str(">") ? py::str("big (360)") : py::str("little"), x.entry_count));
        py::print(py::str("entries:"));
        for (const Entry& e : x.entries)
            py::print(py::percent(py::str("  [%3d] %-5s %dch %6dHz  align=%-3d  %8d bytes"), e.index, e.codec_name,
                                  e.channels, e.rate, e.align, e.length));
    } else {
        py::str outdir = py::len(a) > 2 ? a[2] : py::str(std::get<0>(py::os::path::splitext(path)) + py::str("_xwb"));
        py::print(py::percent(py::str("extracted %d streams -> %s"), x.extract(outdir), outdir));
    }
}

}  // namespace codebreaker::formats::xbox::xwb
