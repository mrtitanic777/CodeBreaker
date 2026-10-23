"""volumes.py -- Xbox and Xbox 360 containers as media volumes.

    from codebreaker.formats.xbox import volumes
    volumes.volumes(path)       [volume, ...] when `path` is an Xbox container, else None

Each volume has the shape formats/media.py gives every volume (kind, label, where, boot,
files() -> [Entry], read(entry) -> bytes), so `codebreaker media list|extract` reads these as it
reads a floppy or a CD:

    stfs       an STFS package (CON / LIVE / PIRS): saves, DLC, title updates, installs
    xdvdfs     an Xbox / Xbox 360 disc image (XISO, redump XGD), plain or CSO / CCI compressed
    god        a Games-on-Demand folder (its Data#### parts are the disc, de-interleaved)
    fatx       each FATX partition of a hard-disk or memory-unit image
    nand       the flash file system of a console's NAND dump (16 MB, with spare bytes)
    heavyiron  a Heavy Iron GoodEngine archive (.ho): each asset by layer, id and type
    zar        a ZArchive (.zar)
    xzp        a XUI package (XUIZ)
    pck        a Minecraft Legacy Console Edition .pck
"""
import os
import struct

from codebreaker.formats.media import Entry
from codebreaker.formats.xbox import ciso, fatx, gdf, god, heavyiron, nand, pck, stfs, xzp, zar

_STFS = (b"CON ", b"LIVE", b"PIRS")


class _Listed(object):
    """A volume whose files are a list of (path, size) and a way to read one."""
    def __init__(self, kind, label, where, boot, listing, reader):
        self.kind, self.label, self.where, self.boot = kind, label, where, boot
        self._listing, self._reader = listing, reader

    def files(self):
        return [Entry(p, size, False, 0) for p, size in self._listing()]

    def read(self, entry):
        return self._reader(entry.path)


def _stfs(path):
    p = stfs.STFS(open(path, "rb").read())
    boot = {"magic": p.magic.decode("latin-1").strip(), "title id": p.title_id,
            "content type": "0x%X" % p.content_type, "display name": p.display_name}
    return [_Listed("stfs", p.title_name or os.path.basename(path), "", boot,
                    lambda: [(n, v[2]) for n, v in p.files.items()], p.read_file)]


def _xdvdfs(device, label, where, kind="xdvdfs"):
    g = gdf.GDF(device)
    return _Listed(kind, label, where, {"volume at": "0x%X" % g.base},
                   g.files, g.read)


def _god(folder):
    """A Games-on-Demand folder: the `<media id>.data` folder of Data#### parts, or the folder
    of the whole package (the one that holds it)."""
    def parts(d):
        return any(n.lower().startswith("data") and os.path.isfile(os.path.join(d, n)) for n in os.listdir(d))
    data = folder if parts(folder) else None
    if data is None:
        for dp, dn, _fn in os.walk(folder):
            hit = [d for d in dn if d.lower().endswith(".data") and parts(os.path.join(dp, d))]
            if hit:
                data = os.path.join(dp, hit[0])
                break
    if data is None:
        return None
    return [_xdvdfs(god.SvodReader(data), os.path.basename(os.path.dirname(data)) or "GoD", "Games on Demand", "god")]


class FatxVolume(object):
    """One FATX partition of a drive or memory-unit image."""
    kind = "fatx"

    def __init__(self, reader, partition):
        self._r, self._p = reader, partition
        self.label, self.where = partition.name, "partition at 0x%X" % partition.offset
        self.boot = {"cluster": "0x%X bytes" % partition.bytes_per_cluster, "clusters": partition.max_clusters,
                     "byte order": "big (Xbox 360)" if partition.endian == ">" else "little (original Xbox)"}
        self._entries = None

    def _all(self):
        if self._entries is None:
            self._entries = {e.path: e for e in self._r._flatten(self._r.list_directory(self._p))}
        return self._entries

    def files(self):
        return [Entry(e.path, e.file_size, e.is_directory, e.first_cluster,
                      when=e.modified.strftime("%Y-%m-%d %H:%M") if e.modified else "")
                for e in self._all().values()]

    def read(self, entry):
        e, p = self._all()[entry.path], self._p
        out, remaining = [], e.file_size
        for cl in self._r.cluster_chain(p, e.first_cluster):
            if remaining <= 0:
                break
            chunk = self._r.read_cluster(p, cl)[:min(remaining, p.bytes_per_cluster)]
            out.append(chunk)
            remaining -= len(chunk)
        return b"".join(out)


def _fatx(path):
    r = fatx.FatxReader(path)
    found = [FatxVolume(r, p) for p in r.detect_partitions()]
    if not found:
        r.close()
    return found or None


def _nand(path):
    with open(path, "rb") as f:
        fs = nand.Flash(f.read())
    return [_Listed("nand", "flash", "root block %#x" % fs.root, {"files": len(fs.files)},
                    lambda: [(n, v[1]) for n, v in fs.files.items()], fs.read)]


def _zar(path):
    z = zar.ZarReader(path)
    return [_Listed("zar", os.path.basename(path), "", {"files": len(z.files())},
                    z.files, lambda name: b"".join(z.read_iter(name)))]


def _payloads(kind, path, found):
    blobs = dict(found)
    return [_Listed(kind, os.path.basename(path), "", {"files": len(blobs)},
                    lambda: [(n, len(b)) for n, b in blobs.items()], blobs.__getitem__)]


def volumes(path, long_names=True):
    """The volumes of `path` when it is an Xbox container, else None (it is then some other
    kind of image, or not one). `long_names` is media's option; Xbox names are always long."""
    if os.path.isdir(path):
        return _god(path)
    with open(path, "rb") as f:
        head = f.read(16)
    magic = head[:4]
    if magic in _STFS:
        return _stfs(path)
    if magic in (ciso.CSO_MAGIC, ciso.CCI_MAGIC):
        return [_xdvdfs(ciso.open_image(path), os.path.basename(path), magic.decode("latin-1").strip() + " compressed disc")]
    if nand.is_nand(path):
        return _nand(path)
    if path.lower().endswith(".ho") and heavyiron.is_archive(path):
        listing, reader = heavyiron.volume_listing(path)
        return [_Listed("heavyiron", os.path.basename(path), "", {"assets": len(listing)}, lambda: listing, reader)]
    if magic == b"XUIZ":
        return _payloads("xzp", path, xzp.payloads(path))
    if path.lower().endswith(".zar"):
        return _zar(path)
    if path.lower().endswith(".pck"):
        return _payloads("pck", path, pck.payloads(path)[0])
    dev = gdf._FileDevice(path)
    try:
        return [_xdvdfs(dev, os.path.basename(path), "disc image")]
    except (ValueError, AssertionError, struct.error):
        dev.close()
    if magic == b"FATX" or os.path.getsize(path) >= 0x80000:
        try:
            return _fatx(path)
        except (OSError, ValueError, struct.error):
            return None
    return None
