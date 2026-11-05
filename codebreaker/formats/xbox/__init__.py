"""formats/xbox -- the Xbox and Xbox 360 file formats: containers, discs, filesystems, assets.

    from codebreaker.formats.xbox import summary
    summary(path, match)            {field: value} for `codebreaker info` (match: detect.identify's)

Containers are read through formats/media.py (volumes.py adapts each one), so
`codebreaker media list|extract` takes all of them:

    stfs.py      STFS packages (CON / LIVE / PIRS): read, pack, resign with your console's keyvault
    gdf.py       XDVDFS / GDF disc filesystem (XISO, redump XGD1-3)
    god.py       Games on Demand (SVOD): read, and ISO / XEX / NXE install -> GoD
    ciso.py      CSO and CCI compressed disc images (their LZ4 blocks are formats.dll's)
    chd.py       CHD disc images (header read here; conversion through chdman)
    fatx.py      FATX filesystems of drives and memory units: list, extract, deleted-file recovery, carving
    zar.py       ZArchive (.zar)
    xzp.py       XUI packages (XUIZ)
    pck.py       Minecraft Legacy Console Edition packages

Assets, summarised by `codebreaker info`, converted by each module's own command line
(`python -m codebreaker.formats.xbox.<name> ...`):

    xbe.py       original Xbox executables (XBE): header, certificate, sections
    xdbf.py      XDBF / GPD databases: gamer profiles, achievements
    xpr.py       XPR0 / XPR2 resource bundles -> DDS textures
    texture.py   Xbox 360 texture untiling -> DDS (DXT1/3/5)
    xwb.py       XACT wave banks; xma.py: XMA / XMA2 audio -> WAV (through ffmpeg)
    xur.py       compiled XUI scenes: strings, re-emit
    bink.py      Bink video headers (decode through ffmpeg)

The external tools some conversions use are named in settings.json (`ffmpeg`, `chdman`) or found
on PATH.
"""
import os


def _container(path):
    from codebreaker.formats import media
    vols = media.volumes(path)
    out = {"volumes": len(vols)}
    for i, v in enumerate(vols[:4]):
        files = [e for e in v.files() if not e.is_dir]
        key = "volume" if len(vols) == 1 else "volume %d" % (i + 1)
        out[key] = "%s %r: %d files, %d bytes%s" % (v.kind, v.label, len(files), sum(e.size for e in files),
                                                   (" (%s)" % v.where) if v.where else "")
        for k, val in (v.boot or {}).items():
            out["%s %s" % (key, k) if len(vols) > 1 else k] = val
    return out


def _xbe(path):
    from codebreaker.formats.xbox import xbe
    x = xbe.XBE(open(path, "rb").read())
    return {"title": getattr(x, "title_name", ""), "title id": "%08X" % getattr(x, "title_id", 0),
            "base": "0x%08X" % x.base, "image size": x.size_of_image, "sections": x.section_count,
            "region": "0x%X" % getattr(x, "region", 0), "allowed media": "0x%X" % getattr(x, "allowed_media", 0)}


def _xdbf(path):
    from codebreaker.formats.xbox import xdbf
    x = xdbf.XDBF(open(path, "rb").read())
    out = {"version": "0x%X" % x.version, "entries": x.entry_count, "free": x.free_count}
    out.update({"namespace %s" % xdbf.NS.get(ns, ns): n for ns, n in sorted(x.counts().items())})
    try:
        ach = x.achievements()
        out["achievements"] = "%d (%d unlocked, %dG)" % (len(ach), sum(1 for a in ach if a["unlocked"]),
                                                         sum(a["score"] for a in ach))
    except Exception:
        pass
    return out


def _xpr(path):
    from codebreaker.formats.xbox import xpr
    info = xpr.parse(open(path, "rb").read())
    return {k: v for k, v in info.items() if not isinstance(v, (bytes, bytearray, list, dict))}


def _xwb(path):
    from codebreaker.formats.xbox import xwb
    w = xwb.XWB(open(path, "rb").read())
    out = {"version": getattr(w, "version", ""), "byte order": "big" if getattr(w, "en", ">") == ">" else "little"}
    for k in ("entries", "names", "bank_name"):
        v = getattr(w, k, None)
        if v is not None:
            out[k] = len(v) if isinstance(v, (list, tuple, dict)) else v
    return out


def _xur(path):
    from codebreaker.formats.xbox import xur
    x = xur.XUR(open(path, "rb").read())
    return {"version": x.version, "tool version": getattr(x, "tool_version", ""), "sections": x.section_count,
            "section tags": " ".join(m.decode("latin-1") if isinstance(m, bytes) else str(m) for m, _o, _l in x.sections)}


def _bink(path):
    from codebreaker.formats.xbox import bink
    return bink.read_header(path)


def _chd(path):
    from codebreaker.formats.xbox import chd
    h = chd.read_header(path)
    return {k: (", ".join(v) if isinstance(v, list) else v) for k, v in h.items()}


_BY_FORMAT = (("XBE", _xbe), ("XDBF", _xdbf), ("resource bundle", _xpr), ("wave bank", _xwb),
              ("XUI scene", _xur), ("Bink", _bink), ("CHD", _chd))


def summary(path, match):
    """What `codebreaker info` shows for an Xbox file: {field: value}, or {} when there is nothing
    to add to the format line. `match` is the first of detect.identify(path)."""
    if os.path.isdir(path) or match.get("command") == "media":
        return _container(path)
    for key, fn in _BY_FORMAT:
        if key in match.get("format", ""):
            return fn(path)
    return {}
