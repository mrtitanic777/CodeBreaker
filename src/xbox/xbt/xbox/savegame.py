"""Xbox 360 save-game toolkit — edit STFS-wrapped game saves and repack.

A save is an STFS package (usually CON) whose payload is the title's save blob.
We already read STFS, rebuild it (cloning the original header/metadata), and
resign it, so editing a save is: extract -> patch the inner blob -> rebuild
against the original as template (preserving title id / content type / display
name / thumbnail) -> resign to a console's keyvault.

Commands:
  savegame info    <save>                             metadata + inner file list
  savegame extract <save> <outdir>                    dump the inner save files
  savegame repack  <save> <folder> <out> [kv]         rebuild from edited files, resign
  savegame patch   <save> <inner> <off> <hex> <out> [kv]
                                                       poke bytes in an inner file, rebuild, resign

Note: many titles embed their own checksum over the save data; this fixes the
STFS wrapper (hash tree + signature) but not a title-specific inner checksum.
"""

import os
import shutil
import struct
import sys
import tempfile

from stfs import STFS, build, resign

CONTENT_TYPES = {
    0x00000001: "Saved Game", 0x00000002: "Marketplace Content",
    0x00000003: "Publisher", 0x00001000: "Xbox 360 Title",
    0x00002000: "IPTV Pause Buffer", 0x00004000: "Installed Game",
    0x00005000: "Xbox Original Game", 0x00007000: "Game on Demand",
    0x00008000: "Avatar Item", 0x00009000: "Profile", 0x0000A000: "Gamer Picture",
    0x0000B000: "Theme", 0x0000D000: "Cache File", 0x0000E000: "Storage Download",
    0x0000F000: "Xbox Saved Game", 0x00010000: "Xbox Download",
    0x00020000: "Game Demo", 0x00030000: "Video", 0x00040000: "Game Title",
    0x00050000: "Installer", 0x00080000: "Game Trailer", 0x000A0000: "Arcade Title",
    0x00100000: "XNA", 0x00200000: "License Store", 0x00300000: "Movie",
    0x00400000: "TV", 0x00500000: "Music Video", 0x00600000: "Game Video",
    0x00700000: "Podcast Video", 0x00800000: "Viral Video",
}

DESCRIPTION = "edit Xbox 360 STFS game saves: info/extract/patch/repack (+ resign)"


def info(data: bytes) -> dict:
    s = STFS(data)
    return {
        "magic": data[:4].decode("latin-1"),
        "content_type": s.content_type,
        "content_type_name": CONTENT_TYPES.get(s.content_type, "0x%X" % s.content_type),
        "title_id": s.title_id,
        "profile_id": s.profile_id,
        "display_name": s.display_name,
        "title_name": s.title_name,
        "files": list(s.files),
    }


def _extract_to(data: bytes, folder: str) -> STFS:
    s = STFS(data)
    os.makedirs(folder, exist_ok=True)
    for name in s.files:
        p = os.path.join(folder, name.replace("/", os.sep).replace("\\", os.sep))
        os.makedirs(os.path.dirname(p) or folder, exist_ok=True)
        open(p, "wb").write(s.read_file(name))
    return s


def repack(save_path: str, folder: str, out_path: str, kv: bytes = None):
    """Rebuild an STFS save from a folder of (edited) files, cloning the original
    save's header/metadata, then optionally resign with a keyvault."""
    template = open(save_path, "rb").read()
    build(folder, out_path, template)
    if kv:
        data = resign(open(out_path, "rb").read(), kv, out_path)
    return out_path


def patch(save_path: str, inner: str, offset: int, blob: bytes,
          out_path: str, kv: bytes = None):
    """Poke `blob` at `offset` in an inner save file, then rebuild + resign."""
    data = open(save_path, "rb").read()
    tmp = tempfile.mkdtemp()
    try:
        _extract_to(data, tmp)
        target = os.path.join(tmp, inner.replace("/", os.sep).replace("\\", os.sep))
        if not os.path.isfile(target):
            raise FileNotFoundError("inner file not found: %s" % inner)
        buf = bytearray(open(target, "rb").read())
        if offset + len(blob) > len(buf):
            raise ValueError("patch past end of %s (size %d)" % (inner, len(buf)))
        buf[offset:offset + len(blob)] = blob
        open(target, "wb").write(buf)
        build(tmp, out_path, data)
        if kv:
            resign(open(out_path, "rb").read(), kv, out_path)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return out_path


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print(__doc__.split("Commands:")[1].split("Note:")[0].rstrip())
        return
    op = a[0]
    if op == "info":
        d = info(open(a[1], "rb").read())
        print("magic        : %s" % d["magic"])
        print("content type : %s (0x%X)" % (d["content_type_name"], d["content_type"]))
        print("title id     : %s   profile: %s" % (d["title_id"], d["profile_id"]))
        print("display name : %r" % d["display_name"])
        print("title        : %r" % d["title_name"])
        print("inner files  : %d" % len(d["files"]))
        for f in d["files"]:
            print("   %s" % f)
        return
    if op == "extract":
        s = _extract_to(open(a[1], "rb").read(), a[2])
        print("extracted %d inner file(s) -> %s" % (len(s.files), a[2])); return
    if op == "repack":
        kv = open(a[4], "rb").read() if len(a) > 4 else None
        repack(a[1], a[2], a[3], kv)
        print("repacked -> %s%s" % (a[3], " (resigned)" if kv else "")); return
    if op == "patch":
        kv = open(a[6], "rb").read() if len(a) > 6 else None
        blob = bytes.fromhex(a[4].replace(" ", ""))
        patch(a[1], a[2], int(a[3], 0), blob, a[5], kv)
        print("patched %s @%s -> %s%s" % (a[2], a[3], a[5], " (resigned)" if kv else "")); return
    print("unknown op: %s" % op)


if __name__ == "__main__":
    main()
