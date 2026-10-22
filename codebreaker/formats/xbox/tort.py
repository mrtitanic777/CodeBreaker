"""tort.py -- the packed asset archive some Xbox Live Arcade titles ship: <name>.tort with its <name>.hdr index.

    from codebreaker.formats.xbox import tort
    for entry in tort.entries("bin.hdr"):          # name, raw size, compressed size, offset
        ...
    tort.extract("bin.hdr", "bin.tort", out_dir)   # every asset, decompressed, under its own path

The index is text: three header lines (company, comment, entry count), then one line per asset:
`name, raw size, padded raw size, compressed size, padded compressed size, offset, flag`. Each
asset is zlib data at `offset`, `compressed size` bytes long, inflating to `raw size` bytes.
Tetris Splash's bin.hdr / bin.tort hold its textures, fonts, effects and sounds. A title reads
the archive itself; extracting it is for looking at its assets (a title that finds a devData/
folder of loose files takes a developer path instead).

    python -m codebreaker.formats.xbox.tort <bin.hdr> <bin.tort> <out folder>
"""
import os
import sys
import zlib


def entries(hdr_path):
    """[(name, raw size, compressed size, offset)] from the index."""
    with open(hdr_path, "rb") as f:
        lines = f.read().decode("latin-1").replace("\r", "").split("\n")
    out = []
    for line in lines[3:]:
        parts = [p.strip() for p in line.split(",")]
        if len(parts) >= 6 and parts[0]:
            out.append((parts[0], int(parts[1]), int(parts[3]), int(parts[5])))
    return out


def extract(hdr_path, tort_path, out_dir):
    """Every asset into out_dir (its own sub-path). -> (extracted, failed, bytes)."""
    with open(tort_path, "rb") as f:
        data = f.read()
    ok = failed = total = 0
    for name, raw, compressed, offset in entries(hdr_path):
        try:
            body = zlib.decompress(data[offset:offset + compressed])
        except zlib.error:
            failed += 1
            continue
        if len(body) != raw:
            failed += 1
            continue
        dst = os.path.join(out_dir, *name.replace("\\", "/").split("/"))
        os.makedirs(os.path.dirname(dst) or out_dir, exist_ok=True)
        with open(dst, "wb") as f:
            f.write(body)
        ok += 1
        total += len(body)
    return ok, failed, total


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print(__doc__)
        sys.exit(2)
    n, bad, size = extract(*sys.argv[1:4])
    print("%d assets extracted (%d failed), %.1f MB" % (n, bad, size / 1048576))
