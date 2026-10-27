"""XMA / XMA2 audio — the Xbox 360 game audio codec — to playable WAV.

We already extract raw XMA streams from XACT wave banks (`xwb`); this wraps a
raw XMA2 stream in the standard RIFF-WAVE(XMA2) container natively, and decodes
it to PCM WAV via ffmpeg (found through `config`/PATH — driven not embedded,
like chdman: the XMA2 codec is a WMA-Pro derivative and ffmpeg maintains it).

`xma bank` turns a whole `.xwb` wave bank into playable .wav files in one step.

Commands: wrap / decode / bank.
"""

import os
import struct
import subprocess
import sys

from codebreaker.base import settings

DESCRIPTION = "Xbox 360 XMA/XMA2 audio -> playable WAV (native RIFF-XMA2 wrap + ffmpeg decode)"

WAVE_FORMAT_XMA2 = 0x0166


def wrap_xma2(data, channels, rate, block=0x10000):
    """Wrap a raw XMA2 stream in a RIFF-WAVE(XMA2) container (native, no deps).
    The result is decodable by ffmpeg / any XMA2-aware player."""
    channels = channels or 2
    rate = rate or 44100
    n_streams = (channels + 1) // 2
    block_count = (len(data) + block - 1) // block
    fmtx = struct.pack("<HHIIHH", WAVE_FORMAT_XMA2, channels, rate,
                       rate * channels * 2, channels * 2, 16)
    ext = struct.pack("<HHIIIIIIIBBH", 34, n_streams, 0, 0, block,
                      0, 0, 0, 0, 0, 4, block_count)     # XMA2WAVEFORMATEX tail
    fmt_chunk = fmtx + ext
    body = (b"WAVE" + b"fmt " + struct.pack("<I", len(fmt_chunk)) + fmt_chunk +
            b"data" + struct.pack("<I", len(data)) + data)
    return b"RIFF" + struct.pack("<I", len(body)) + body


def _need_ffmpeg():
    exe = settings.tool("ffmpeg")
    if not exe:
        raise RuntimeError("ffmpeg not found — install it and/or run: "
                           'set "ffmpeg" in settings.json')
    return exe


def decode_to_wav(xma_or_riff, out_wav, channels=2, rate=44100, block=0x10000):
    """Decode a raw XMA2 stream (or an already-wrapped RIFF-XMA2) to PCM WAV via
    ffmpeg. Returns the output size in bytes."""
    exe = _need_ffmpeg()
    riff = xma_or_riff if xma_or_riff[:4] == b"RIFF" else wrap_xma2(xma_or_riff, channels, rate, block)
    import tempfile
    with tempfile.TemporaryDirectory() as folder:
        tmp = os.path.join(folder, "in.xma.wav")
        open(tmp, "wb").write(riff)
        # 16-bit PCM in a plain WAVEFORMATEX: with the decoder's own layout ffmpeg writes WAVE_FORMAT_EXTENSIBLE,
        # which MonoGame's (and many players') WAV readers do not take
        r = subprocess.run([exe, "-y", "-hide_banner", "-loglevel", "error", "-i", tmp, "-c:a", "pcm_s16le",
                            "-ac", str(min(channels, 2)), out_wav],
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode != 0 or not os.path.exists(out_wav):
        raise RuntimeError("ffmpeg XMA2 decode failed:\n%s" % (r.stdout or "").strip())
    return os.path.getsize(out_wav)


def decode_bank(xwb_path, out_dir):
    """Extract every stream from an .xwb wave bank and write playable .wav:
    PCM streams verbatim, XMA streams decoded via ffmpeg. Returns file count."""
    from codebreaker.formats.xbox import xwb
    w = xwb.XWB(open(xwb_path, "rb").read())
    os.makedirs(out_dir, exist_ok=True)
    n = 0
    for e in w.entries:
        data = w.stream(e)
        out = os.path.join(out_dir, "stream_%03d.wav" % e["index"])
        if e["codec_name"] == "PCM":
            open(out, "wb").write(xwb.pcm_to_wav(data, e["channels"] or 2, e["rate"] or 44100))
        elif e["codec_name"] == "XMA":
            try:
                decode_to_wav(data, out, e["channels"] or 2, e["rate"] or 44100)
            except Exception as ex:
                open(os.path.join(out_dir, "stream_%03d.xma" % e["index"]), "wb").write(data)
                print("  stream %d: XMA decode failed (%s); wrote raw .xma" % (e["index"], ex))
                continue
        else:
            open(os.path.join(out_dir, "stream_%03d.%s" % (e["index"], e["codec_name"].lower())),
                 "wb").write(data)
            continue
        n += 1
    return n


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  xma wrap   <raw.xma> <out.wav> --channels N --rate R   RIFF-XMA2 (native)\n"
              "  xma decode <raw.xma> <out.wav> --channels N --rate R   -> PCM WAV (ffmpeg)\n"
              "  xma bank   <bank.xwb> <outdir>                         extract+decode a wave bank")
        return
    ch = int(a[a.index("--channels") + 1]) if "--channels" in a else 2
    rate = int(a[a.index("--rate") + 1]) if "--rate" in a else 44100
    op = a[0]
    if op == "wrap" and len(a) >= 3:
        open(a[2], "wb").write(wrap_xma2(open(a[1], "rb").read(), ch, rate))
        print("wrote RIFF-XMA2 -> %s" % a[2]); return
    if op == "decode" and len(a) >= 3:
        n = decode_to_wav(open(a[1], "rb").read(), a[2], ch, rate)
        print("decoded -> %s (%d bytes PCM WAV)" % (a[2], n)); return
    if op == "bank" and len(a) >= 3:
        n = decode_bank(a[1], a[2])
        print("wrote %d WAV file(s) -> %s" % (n, a[2])); return
    print("unknown/short command; run 'xma' for usage"); sys.exit(2)


if __name__ == "__main__":
    main()
