"""heavyiron.py -- Heavy Iron Studios' GoodEngine archives (.HO, "HEB"), as Up, WALL-E, Ratatouille
and the SpongeBob games ship them on the Xbox 360 and Wii; and their textures.

    python -m codebreaker heavyiron <info|list|layers|dups|extract|replace|textures> <file.ho> ...
    from codebreaker.formats.xbox import heavyiron
    P = heavyiron.parse(data); heavyiron.all_assets(P); heavyiron.asset_bytes(data, a)
    heavyiron.replace(P, data, asset_id, new_bytes)      # any size: in its slot, or the layer moved to the end
    heavyiron.texture(data, P, descriptor_id)            # a texture asset as a Pillow image

The reader follows the engine's own (read from the game's decompiled archive library); the writer
patches the file, keeping the regions it does not understand byte for byte and fixing the offsets
an edit moves. Archives are also media volumes (`codebreaker media list|extract`): each asset as
layerNN/<id>_<type>.bin.

Layout:
  [0,2048)          HEL  - opaque header blob (preserved verbatim)
  [2048,+96)        MAST - 24 x int32(BE); int[15] = num -> SECT at num*2048
  [num*2048, ...)   SECT - 8 x int32(BE): [1] = layerCount; then layerCount x LayerEntry(64B)
  per layer:        data block at offToStart*0x800: AssetList + AssetData + Padding
  AssetEntry(32B):  totalDataSize, relOff, actualSize, unk0C, assetID(u64), assetType, unk1C
All multi-byte ints are big-endian.
"""
import os
import struct

# Known asset-type IDs (by total bytes they usually dominate)
ASSET_TYPES = {
    0xBDE21A8D: "RawBlob",          # pixel/geometry data blobs
    693241887: "TextureDescriptor",
    0x86EF2978: "StaticGeometry",
}


class R:
    __slots__ = ("d", "p")

    def __init__(self, d):
        self.d = d
        self.p = 0

    def i32(self):
        v = struct.unpack_from(">i", self.d, self.p)[0]; self.p += 4; return v

    def u32(self):
        v = struct.unpack_from(">I", self.d, self.p)[0]; self.p += 4; return v

    def u64(self):
        v = struct.unpack_from(">Q", self.d, self.p)[0]; self.p += 8; return v

    def raw(self, n):
        v = self.d[self.p:self.p + n]; self.p += n; return v


def parse(data):
    """Parse a .HO archive into a dict tree (header, layers, assets).

    Raises ValueError if the bytes don't look like a valid HEB/.HO archive (so non-archive or
    truncated input fails with a clear message rather than an opaque struct/index error).
    """
    if len(data) < 2048 + 96:
        raise ValueError("too small to be a .HO archive (%d bytes)" % len(data))
    r = R(data)
    hel = r.raw(2048)
    mast_off = r.p
    mast_ints = [r.i32() for _ in range(24)]
    num = mast_ints[15]
    sect_off = num * 2048
    if num <= 0 or sect_off + 32 > len(data):
        raise ValueError("not a valid .HO archive (bad MAST/SECT pointer)")
    r.p = sect_off
    sect_head = [r.i32() for _ in range(8)]
    nlayers = sect_head[1]
    if not 0 <= nlayers < 100000:
        raise ValueError("not a valid .HO archive (implausible layer count %d)" % nlayers)
    layers = [_layer(r, sect_off) for _ in range(nlayers)]
    for li, L in enumerate(layers):
        _get_assets(r, L, L["offToStart"] * 2048)
        if L["sub"]["kind"] == "PSL":
            for a in L["sub"]["assets"]:
                a["layer_idx"] = li             # tag each asset with its layer (for dup-id handling)
    return {"hel": hel, "mast_off": mast_off, "mast_ints": mast_ints,
            "sect_off": sect_off, "sect_head": sect_head, "layers": layers, "size": len(data)}


def _layer(r, sect_off):
    entry_pos = r.p
    lt = r.raw(4)
    r.i32()
    [r.i32() for _ in range(5)]
    offToStart = r.i32()
    totalLayerSize = r.i32()
    totalLayerSize2 = r.i32()
    [r.i32() for _ in range(4)]
    offToPart2 = r.i32()
    r.i32()
    save = r.p
    r.p = sect_off + offToPart2
    tag = r.raw(4)
    if tag == b"PSL\x00":
        sub = _psl(r)
    elif tag == b"PSLD":
        sub = {"kind": "PSLD", "sectionSize": r.i32(), "vals": [r.i32() for _ in range(6)]}
    else:
        raise ValueError("bad sublayer tag %r at 0x%X" % (tag, r.p - 4))
    r.p = save
    return {"entry_pos": entry_pos, "type": lt, "offToStart": offToStart,
            "totalLayerSize": totalLayerSize, "totalLayerSize2": totalLayerSize2,
            "offToPart2": offToPart2, "sub": sub}


def _psl(r):
    sectionSize, amt, unknown = r.i32(), r.i32(), r.i32()
    subs = []
    for _ in range(amt):
        pos = r.p
        subs.append({"pos": pos, "type": r.i32(), "startOffset": r.i32(),
                     "size": r.i32(), "minusOne": r.i32()})
    return {"kind": "PSL", "sectionSize": sectionSize, "amt": amt,
            "unknown": unknown, "subs": subs, "assets": []}


def _get_assets(r, L, layerStart):
    sub = L["sub"]
    if sub["kind"] != "PSL":
        return
    for ss in sub["subs"]:
        if ss["type"] == 0:                       # AssetList
            r.p = layerStart + ss["startOffset"]
            count = r.i32()
            r.p += 28
            for _ in range(count):
                hdr_pos = r.p
                totalDataSize = r.i32()
                relOff = r.i32()
                absSizeOff = r.p
                actualSize = r.i32()
                r.i32()
                assetID = r.u64()
                assetType = r.u32()
                r.i32()
                sub["assets"].append({
                    "hdr_pos": hdr_pos, "totalDataSize": totalDataSize, "relOff": relOff,
                    "absSizeOff": absSizeOff, "actualSize": actualSize, "assetID": assetID,
                    "assetType": assetType, "absDataOff": layerStart + relOff,
                    "layerStart": layerStart})


def all_assets(P):
    out = []
    for L in P["layers"]:
        if L["sub"]["kind"] == "PSL":
            out.extend(L["sub"]["assets"])
    return out


def find_assets(P, assetID, layer_idx=None):
    """Every asset matching assetID, optionally restricted to one layer."""
    return [a for a in all_assets(P)
            if a["assetID"] == assetID and (layer_idx is None or a.get("layer_idx") == layer_idx)]


def find_asset(P, assetID, layer_idx=None):
    """The single asset matching assetID; None if absent.

    Raises ValueError if the id appears in multiple layers and no layer_idx is given -- the
    shipping game (e.g. MNUS.ho) has hundreds of duplicate ids, so silently taking the first
    would be a correctness bug. Pass a layer to disambiguate.
    """
    ms = find_assets(P, assetID, layer_idx)
    if not ms:
        return None
    if len(ms) > 1:
        raise ValueError("asset %016X is ambiguous: %d instances in layers %s (pass a layer)"
                         % (assetID, len(ms), sorted(set(a.get("layer_idx") for a in ms))))
    return ms[0]


def asset_bytes(data, a):
    return data[a["absDataOff"]:a["absDataOff"] + a["actualSize"]]


def align(n, a):
    return (n + a - 1) // a * a


def repack_inslot(data, edits):
    """edits: list of (asset, newbytes), each <= the asset's slot. In-place patch, no cascade.
    The slot tail is zero-filled, so this is not byte-identical to the source when the original
    padding was non-zero (harmless to the engine)."""
    buf = bytearray(data)
    for a, nd in edits:
        if len(nd) > a["totalDataSize"]:
            raise ValueError("asset %016X: %d bytes > slot %d (use replace())"
                             % (a["assetID"], len(nd), a["totalDataSize"]))
        off = a["absDataOff"]
        buf[off:off + len(nd)] = nd
        for i in range(off + len(nd), off + a["actualSize"]):
            buf[i] = 0
        struct.pack_into(">i", buf, a["absSizeOff"], len(nd))
    return buf


def replace(P, data, assetID, newdata, layer_idx=None):
    """Replace an asset with newdata of ANY size. Same slot -> in-place; else rebuild that
    layer's 0x800-aligned block, relocate to EOF, and fix offToStart + sizes. Pass layer_idx
    to target one instance of a duplicated id (see find_asset)."""
    if layer_idx is None and len(find_assets(P, assetID)) > 1:
        raise ValueError("asset %016X is ambiguous (multiple layers); pass a layer index" % assetID)
    buf = bytearray(data)
    found = None
    for li, L in enumerate(P["layers"]):
        if layer_idx is not None and li != layer_idx:
            continue
        if L["sub"]["kind"] != "PSL":
            continue
        for a in L["sub"]["assets"]:
            if a["assetID"] == assetID:
                found = (L, a)
                break
        if found:
            break
    if not found:
        raise KeyError("asset %016X not found" % assetID)
    L, a = found
    oldSlot, newActual = a["totalDataSize"], len(newdata)
    newSlot = align(newActual, 64)
    if newSlot == oldSlot:
        off = a["absDataOff"]
        buf[off:off + newActual] = newdata
        for i in range(off + newActual, off + oldSlot):
            buf[i] = 0
        struct.pack_into(">i", buf, a["absSizeOff"], newActual)
        return buf

    layerStart = L["offToStart"] * 2048
    assets = sorted(L["sub"]["assets"], key=lambda x: x["relOff"])
    adStart = assets[0]["relOff"]
    block = bytearray(data[layerStart:layerStart + adStart])
    adata = bytearray()
    newrel = {}
    for aj in assets:
        newrel[id(aj)] = adStart + len(adata)
        if aj is a:
            adata += bytes(newdata) + b"\x00" * (newSlot - newActual)
        else:
            adata += data[aj["absDataOff"]:aj["absDataOff"] + aj["totalDataSize"]]
    block += adata
    newTotal = align(adStart + len(adata), 0x800)
    block += b"\x00" * (newTotal - len(block))
    for aj in assets:
        hr = aj["hdr_pos"] - layerStart
        struct.pack_into(">i", block, hr + 4, newrel[id(aj)])
        if aj is a:
            struct.pack_into(">i", block, hr + 0, newSlot)
            struct.pack_into(">i", block, hr + 8, newActual)
    while len(buf) % 0x800 != 0:
        buf.append(0)
    newBlockOff = len(buf)
    buf += block
    ep = L["entry_pos"]
    struct.pack_into(">i", buf, ep + 28, newBlockOff // 2048)
    struct.pack_into(">i", buf, ep + 32, newTotal)
    struct.pack_into(">i", buf, ep + 36, newTotal)
    for ss in L["sub"]["subs"]:
        if ss["type"] == 1:
            struct.pack_into(">i", buf, ss["pos"] + 8, len(adata))
    return buf




# ---- textures ---------------------------------------------------------------------------------------
#
# A texture is a 72-byte descriptor asset naming a separate pixel blob (the blob's id is the
# descriptor's with bit 44 set): its format byte at 0x33 ($52 DXT1, $54 DXT5) and its size packed
# at 0x34 (width - 1 in the low 13 bits, height - 1 in the next 13). The pixels are tiled and
# 8-in-16 byte-swapped as the Xbox 360 keeps them (formats/xbox/texture.py decodes them).

TEXTURE_TYPE = 693241887
TEXTURE_FORMATS = {0x52: "dxt1", 0x54: "dxt5"}


def texture_descriptor(desc):
    """(blob id, format byte, width, height) of a texture descriptor."""
    blob_id = struct.unpack_from(">Q", desc, 0)[0]
    size = struct.unpack_from(">I", desc, 0x34)[0]
    return blob_id, desc[0x33], (size & 0x1FFF) + 1, ((size >> 13) & 0x1FFF) + 1


def texture(data, P, descriptor_id, layer_idx=None, force_opaque=False):
    """The texture a descriptor names, as a Pillow RGBA image: (image, width, height, format)."""
    from codebreaker.formats.xbox import texture as xtexture
    da = find_asset(P, descriptor_id, layer_idx)
    if da is None:
        raise KeyError("texture descriptor %016X not found" % descriptor_id)
    blob_id, fmt, w, h = texture_descriptor(asset_bytes(data, da))
    ba = find_asset(P, blob_id, da.get("layer_idx"))          # the descriptor's own layer first
    if ba is None:
        ms = find_assets(P, blob_id)
        ba = ms[0] if ms else None
    if ba is None:
        raise KeyError("pixel blob %016X not found" % blob_id)
    if fmt not in TEXTURE_FORMATS:
        raise ValueError("texture %016X: format $%02X is not decoded" % (descriptor_id, fmt))
    image = xtexture.decode_dxt(asset_bytes(data, ba), w, h, TEXTURE_FORMATS[fmt], tiled=True,
                                force_opaque=force_opaque)
    return image, w, h, fmt


# ---- as a media volume ------------------------------------------------------------------------------

def is_archive(path):
    if not path.lower().endswith(".ho"):
        return False
    try:
        with open(path, "rb") as f:
            parse(f.read())
        return True
    except (OSError, ValueError, struct.error, IndexError):
        return False


def volume_listing(path):
    """[(name, size)] and a reader, for formats/xbox/volumes.py."""
    with open(path, "rb") as f:
        data = f.read()
    P = parse(data)
    by_name = {}
    for a in all_assets(P):
        by_name["layer%02d/%016X_%08X.bin" % (a.get("layer_idx", 0), a["assetID"], a["assetType"])] = a
    return [(n, a["actualSize"]) for n, a in by_name.items()], lambda name: asset_bytes(data, by_name[name])


# ---- the command ------------------------------------------------------------------------------------

def main(argv):
    """python -m codebreaker heavyiron info|layers|dups <file.ho>
       python -m codebreaker heavyiron list <file.ho> [--type 0xTYPE]
       python -m codebreaker heavyiron extract <file.ho> <folder> [--type 0xTYPE]
       python -m codebreaker heavyiron replace <file.ho> <asset id hex> <new bytes file> <out.ho> [--layer N]
       python -m codebreaker heavyiron textures <file.ho> <folder>       every DXT texture as a PNG"""
    if len(argv) < 2:
        print(main.__doc__)
        return 2
    from collections import Counter, defaultdict

    def option(name, conv):
        return conv(argv[argv.index(name) + 1]) if name in argv else None
    command, path = argv[0], argv[1]
    with open(path, "rb") as f:
        data = f.read()
    P = parse(data)
    A = all_assets(P)
    kind = option("--type", lambda x: int(x, 0))
    if kind is not None:
        A = [a for a in A if a["assetType"] == kind]
    if command == "info":
        print("%s: %d bytes, %d layers, %d assets" % (os.path.basename(path), P["size"], len(P["layers"]), len(A)))
        count, size = Counter(), Counter()
        for a in A:
            count[a["assetType"]] += 1
            size[a["assetType"]] += a["actualSize"]
        for t, n in size.most_common():
            print("  $%08X %-18s %6d assets %12d bytes" % (t, ASSET_TYPES.get(t, ""), count[t], n))
    elif command == "list":
        for a in A:
            print("%016X  $%08X %-18s layer %-3s %9d bytes (slot %d) at $%X" % (
                a["assetID"], a["assetType"], ASSET_TYPES.get(a["assetType"], ""), a.get("layer_idx", "-"),
                a["actualSize"], a["totalDataSize"], a["absDataOff"]))
    elif command == "layers":
        for i, L in enumerate(P["layers"]):
            sub = L["sub"]
            print("[%2d] %-8r block at $%-7X %9d bytes  %-4s %d assets" % (
                i, L["type"], L["offToStart"] * 2048, L["totalLayerSize"], sub["kind"],
                len(sub["assets"]) if sub["kind"] == "PSL" else 0))
    elif command == "dups":
        seen = defaultdict(set)
        for a in all_assets(P):
            seen[a["assetID"]].add(a.get("layer_idx"))
        dups = {k: v for k, v in seen.items() if len(v) > 1}
        print("%d asset ids are in more than one layer (name the layer when replacing one):" % len(dups))
        for k in sorted(dups):
            print("  %016X  layers %s" % (k, sorted(dups[k])))
    elif command == "extract" and len(argv) > 2:
        os.makedirs(argv[2], exist_ok=True)
        for a in A:
            with open(os.path.join(argv[2], "%016X_%08X.bin" % (a["assetID"], a["assetType"])), "wb") as f:
                f.write(asset_bytes(data, a))
        print("%d assets -> %s" % (len(A), argv[2]))
    elif command == "replace" and len(argv) > 4:
        with open(argv[3], "rb") as f:
            new = f.read()
        out = replace(P, data, int(argv[2], 16), new, layer_idx=option("--layer", int))
        with open(argv[4], "wb") as f:
            f.write(out)
        print("replaced %s (%d bytes): %d -> %d bytes, %s" % (argv[2], len(new), len(data), len(out), argv[4]))
    elif command == "textures" and len(argv) > 2:
        os.makedirs(argv[2], exist_ok=True)
        done, ok, skipped = set(), 0, 0
        for da in (a for a in all_assets(P) if a["assetType"] == TEXTURE_TYPE):
            if da["assetID"] in done:
                continue
            done.add(da["assetID"])
            try:
                image, w, h, _fmt = texture(data, P, da["assetID"], da.get("layer_idx"))
                image.save(os.path.join(argv[2], "%016X_%dx%d.png" % (da["assetID"], w, h)))
                ok += 1
            except (KeyError, ValueError):
                skipped += 1
        print("%d textures decoded, %d skipped -> %s" % (ok, skipped, argv[2]))
    else:
        print(main.__doc__)
        return 2
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv[1:]))
