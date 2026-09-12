"""XACT Wave Bank (.xwb) extractor — Xbox 360 game audio.

Parses XWB wave banks (the XACT container games ship audio in) and extracts
each embedded stream with its format decoded. PCM streams are written as real
.wav files; XMA / ADPCM / WMA streams are extracted raw with a printed format
table (full XMA -> PCM decode needs the proprietary codec — feed the raw .xma
to vgmstream/ffmpeg). Big-endian (Xbox 360) and little-endian banks are both
handled via version-sanity endian detection.

Layout facts (clean-room from the documented XACT format / MonoGame WaveBank):
  header:   'WBND', version, [header-version], segment table (off,len x N)
  seg[0]    WaveBankData: flags, entry_count, name[64], meta_elem_size, ...
  seg[1]    per-entry metadata (FlagsDuration, Format, PlayRegion off+len, loop)
  seg[last] ENTRYWAVEDATA
  Format:   codec=fmt&3, channels=(fmt>>2)&7, rate=(fmt>>5)&0x3FFFF, align=(fmt>>23)&0xFF

Commands:
    xwb info    <bank.xwb>              header + per-entry format table
    xwb extract <bank.xwb> [outdir]     dump every stream (PCM->wav, else raw)
"""

import os
import struct
import sys

CODECS = {0: "PCM", 1: "XMA", 2: "ADPCM", 3: "WMA"}
_EXT = {0: "wav", 1: "xma", 2: "adpcm", 3: "wma"}


class XWB:
    def __init__(self, data):
        if data[:4] not in (b"WBND", b"DNBW"):
            raise ValueError("not an XWB (magic %r)" % data[:4])
        self.d = data
        # endian detection: the version is a small number in the correct order
        for en in (">", "<"):
            ver = struct.unpack_from(en + "I", data, 4)[0]
            if 1 <= ver <= 0x10000:
                self.en = en
                self.version = ver
                break
        else:
            raise ValueError("cannot determine XWB endianness")
        self._parse()

    def _u32(self, o):
        return struct.unpack_from(self.en + "I", self.d, o)[0]

    def _parse(self):
        en, d = self.en, self.d
        o = 8
        if self.version >= 42:
            self.header_version = self._u32(o)
            o += 4
        nseg = 5 if self.version >= 42 else 4
        self.segments = []
        for _ in range(nseg):
            off, length = struct.unpack_from(en + "II", d, o)
            self.segments.append((off, length))
            o += 8
        # WaveBankData @ seg[0]
        b = self.segments[0][0]
        self.flags = self._u32(b)
        self.entry_count = self._u32(b + 4)
        name_len = 64 if self.version >= 42 else 16
        self.bank_name = d[b + 8:b + 8 + name_len].split(b"\x00")[0].decode("ascii", "replace")
        p = b + 8 + name_len
        self.meta_elem_size = self._u32(p)
        self.name_elem_size = self._u32(p + 4)
        self.alignment = self._u32(p + 8)
        # per-entry metadata @ seg[1]
        self.wavedata_off = self.segments[-1][0]
        self.entries = []
        meta = self.segments[1][0]
        esz = self.meta_elem_size or 24
        for i in range(self.entry_count):
            eo = meta + i * esz
            fmt = self._u32(eo + 4)
            play_off = self._u32(eo + 8)
            play_len = self._u32(eo + 12)
            self.entries.append(self._decode(i, fmt, play_off, play_len))

    def _decode(self, i, fmt, play_off, play_len):
        codec = fmt & 0x3
        channels = (fmt >> 2) & 0x7
        rate = (fmt >> 5) & 0x3FFFF
        align = (fmt >> 23) & 0xFF
        return dict(index=i, codec=codec, codec_name=CODECS.get(codec, "?"),
                    channels=channels, rate=rate, align=align,
                    offset=play_off, length=play_len)

    def stream(self, e):
        s = self.wavedata_off + e["offset"]
        return self.d[s:s + e["length"]]

    def extract(self, outdir):
        os.makedirs(outdir, exist_ok=True)
        n = 0
        for e in self.entries:
            raw = self.stream(e)
            name = "%s_%03d.%s" % (self.bank_name or "bank", e["index"], _EXT.get(e["codec"], "bin"))
            path = os.path.join(outdir, name)
            if e["codec"] == 0:                       # PCM -> real WAV
                open(path, "wb").write(pcm_to_wav(raw, e["channels"] or 2, e["rate"] or 44100, 16))
            else:
                open(path, "wb").write(raw)
            n += 1
        return n


def pcm_to_wav(pcm: bytes, channels: int, rate: int, bits: int = 16) -> bytes:
    """Wrap raw PCM in a RIFF/WAVE header. Xbox 360 PCM is big-endian, so the
    samples are byte-swapped to little-endian WAV order."""
    if bits == 16:
        b = bytearray(pcm)
        b[0::2], b[1::2] = b[1::2], b[0::2]           # BE -> LE samples
        pcm = bytes(b)
    byte_rate = rate * channels * bits // 8
    block_align = channels * bits // 8
    # fmt chunk body (16 bytes): audioFormat, channels, rate, byteRate, blockAlign, bits
    fmt = struct.pack("<HHIIHH", 1, channels, rate, byte_rate, block_align, bits)
    return (b"RIFF" + struct.pack("<I", 36 + len(pcm)) + b"WAVE" +
            b"fmt " + struct.pack("<I", 16) + fmt +
            b"data" + struct.pack("<I", len(pcm)) + pcm)


DESCRIPTION = "extract Xbox 360 XACT wave banks (.xwb): PCM -> wav, XMA/ADPCM raw"


# ------------------------------------------------------------------------ CLI
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: xwb {info|extract} <bank.xwb> [outdir]"); return
    op = a[0] if a[0] in ("info", "extract") else "info"
    path = a[1] if a[0] in ("info", "extract") else a[0]
    x = XWB(open(path, "rb").read())
    if op == "info":
        print("bank        : %s" % (x.bank_name or "(unnamed)"))
        print("version     : %d  endian: %s  entries: %d" % (
            x.version, "big (360)" if x.en == ">" else "little", x.entry_count))
        print("entries:")
        for e in x.entries:
            print("  [%3d] %-5s %dch %6dHz  align=%-3d  %8d bytes" % (
                e["index"], e["codec_name"], e["channels"], e["rate"],
                e["align"], e["length"]))
    else:
        outdir = a[2] if len(a) > 2 else os.path.splitext(path)[0] + "_xwb"
        print("extracted %d streams -> %s" % (x.extract(outdir), outdir))


if __name__ == "__main__":
    main()
