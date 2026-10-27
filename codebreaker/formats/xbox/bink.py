"""Bink video (.bik) — the most common Xbox 360 cutscene/video format.

`bink info` parses the container header natively (version, frame count,
dimensions, fps, audio tracks) with no external tool. `bink decode` extracts and
decodes via ffmpeg (found through `config`/PATH) — the Bink1/Bink2 video and DCT
audio codecs are RAD-proprietary and ffmpeg maintains the decoders, so the same
driven-not-embedded policy as chdman/xma applies.

Commands: info / decode.
"""

import os
import struct
import subprocess
import sys

from codebreaker.base import settings

DESCRIPTION = "Xbox 360 Bink video (.bik): native header info + ffmpeg-driven decode/extract"

_BINK1 = (b"BIKb", b"BIKd", b"BIKf", b"BIKg", b"BIKh", b"BIKi", b"BIKk")
_BINK2 = tuple((b"KB2" + bytes([c]) for c in range(ord("a"), ord("z") + 1)))


def is_bink(data):
    m = data[:4]
    return m in _BINK1 or m in _BINK2


def read_header(path):
    """Parse a Bink header (little-endian). Returns a dict."""
    with open(path, "rb") as f:
        head = f.read(64)
    magic = head[:4]
    if not is_bink(head):
        raise ValueError("not a Bink file (magic %r)" % magic)
    gen = 2 if magic in _BINK2 else 1
    (file_size, frames, largest, _frames2, width, height,
     fps_num, fps_den, flags, ntracks) = struct.unpack_from("<10I", head, 4)
    fps = (fps_num / fps_den) if fps_den else 0.0
    return {
        "generation": gen, "codec": magic.decode("latin-1"),
        "file_size": file_size + 8, "frames": frames, "largest_frame": largest,
        "width": width, "height": height, "fps": round(fps, 3),
        "video_flags": "0x%08X" % flags, "audio_tracks": ntracks,
    }


def decode(bik_path, out_path, audio_only=False, video_only=False):
    """Decode a .bik via ffmpeg. The output container/codec is chosen by the
    out_path extension (.wav/.mp4/.png-sequence/…). Returns output size."""
    exe = settings.tool("ffmpeg")
    if not exe:
        raise RuntimeError("ffmpeg not found — install it or run: "
                           'set "ffmpeg" in settings.json')
    args = [exe, "-y", "-hide_banner", "-loglevel", "error", "-i", bik_path]
    if audio_only:
        args += ["-vn"]
    if video_only:
        args += ["-an"]
    args += [out_path]
    r = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode != 0:
        raise RuntimeError("ffmpeg Bink decode failed:\n%s" % (r.stdout or "").strip())
    return os.path.getsize(out_path) if os.path.exists(out_path) else 0


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  bink info   <file.bik>                     native header summary\n"
              "  bink decode <file.bik> <out.mp4|.wav|...>  ffmpeg decode (ext picks codec)\n"
              "     --audio  audio track only   |   --video  video only")
        return
    op = a[0]
    if op == "info":
        h = read_header(a[1])
        print("Bink%d (%s): %s" % (h["generation"], h["codec"], a[1]))
        print("  %d x %d  @ %.3f fps   %d frames   %d audio track(s)" % (
            h["width"], h["height"], h["fps"], h["frames"], h["audio_tracks"]))
        print("  file size %d bytes  (largest frame %d)  flags %s" % (
            h["file_size"], h["largest_frame"], h["video_flags"]))
        return
    if op == "decode" and len(a) >= 3:
        n = decode(a[1], a[2], audio_only="--audio" in a, video_only="--video" in a)
        print("decoded -> %s (%d bytes)" % (a[2], n)); return
    print("unknown/short command; run 'bink' for usage"); sys.exit(2)


if __name__ == "__main__":
    main()
