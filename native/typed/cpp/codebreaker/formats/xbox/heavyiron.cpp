// codebreaker/formats/xbox/heavyiron.cpp -- C++17 port of codebreaker.formats.xbox.heavyiron (see heavyiron.hpp).

#include "codebreaker/formats/xbox/heavyiron.hpp"

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include "py_os.hpp"

namespace codebreaker::formats::xbox::heavyiron {

const py::dict<int64_t, py::str> ASSET_TYPES = {
    {0xBDE21A8D, py::str("RawBlob")},            // pixel/geometry data blobs
    {693241887, py::str("TextureDescriptor")},
    {0x86EF2978, py::str("StaticGeometry")},
};

const py::dict<int64_t, py::str> TEXTURE_FORMATS = {{0x52, py::str("dxt1")}, {0x54, py::str("dxt5")}};

namespace {

// "%016X" of an asset id
py::str hex16(uint64_t v) {
    char b[40];
    std::snprintf(b, sizeof b, "%016llX", static_cast<unsigned long long>(v));
    return py::str(b);
}

std::string num(int64_t v) { return std::to_string(v); }

// struct.unpack_from(">...", d, off) on `size` bytes: where the bytes start, or the struct.error CPython raises
const unsigned char* unpack_at(const py::bytes& d, int64_t off, int64_t size) {
    int64_t n = d.size();
    if (off < 0) {
        if (off + size > 0) {
            throw py::StructError("not enough data to unpack " + num(size) + " bytes at offset " + num(off));
        }
        if (off + n < 0) {
            throw py::StructError("offset " + num(off) + " out of range for " + num(n) + "-byte buffer");
        }
        off += n;
    }
    if (n - off < size) {
        throw py::StructError("unpack_from requires a buffer of at least " + num(size + off) + " bytes for unpacking " + num(size) +
                              " bytes at offset " + num(off) + " (actual buffer size is " + num(n) + ")");
    }
    return reinterpret_cast<const unsigned char*>(d.raw().data()) + off;
}

uint64_t be_uint(const unsigned char* p, int size) {
    uint64_t v = 0;
    for (int i = 0; i < size; ++i) {
        v = (v << 8) | p[i];
    }
    return v;
}

// struct.pack_into(">i", buf, off, value)
void pack_i32(const py::bytearray& buf, int64_t off, int64_t value) {
    int64_t n = buf.size();
    if (off < 0) {
        if (off + 4 > 0) {
            throw py::StructError("no space to pack 4 bytes at offset " + num(off));
        }
        if (off + n < 0) {
            throw py::StructError("offset " + num(off) + " out of range for " + num(n) + "-byte buffer");
        }
        off += n;
    }
    if (n - off < 4) {
        throw py::StructError("pack_into requires a buffer of at least " + num(4 + off) + " bytes for packing 4 bytes at offset " +
                              num(off) + " (actual buffer size is " + num(n) + ")");
    }
    if (value < INT32_MIN || value > INT32_MAX) {
        throw py::StructError("'i' format requires -2147483648 <= number <= 2147483647");
    }
    std::string& s = buf.raw_mut();
    uint32_t u = static_cast<uint32_t>(static_cast<int32_t>(value));
    for (int i = 0; i < 4; ++i) {
        s[static_cast<size_t>(off + i)] = static_cast<char>((u >> (8 * (3 - i))) & 0xFF);
    }
}

}  // namespace

R::R(const py::bytes& data) : d(data), p(0) {}

int64_t R::i32() {
    int64_t v = static_cast<int32_t>(static_cast<uint32_t>(be_uint(unpack_at(d, p, 4), 4)));
    p += 4;
    return v;
}

int64_t R::u32() {
    int64_t v = static_cast<int64_t>(be_uint(unpack_at(d, p, 4), 4));
    p += 4;
    return v;
}

uint64_t R::u64() {
    uint64_t v = be_uint(unpack_at(d, p, 8), 8);
    p += 8;
    return v;
}

py::bytes R::raw(int64_t n) {
    py::bytes v = d.slice(p, p + n);
    p += n;
    return v;
}

// ---- the reader -------------------------------------------------------------------------------------

namespace {

Sublayer read_psl(R& r) {
    int64_t sectionSize = r.i32();
    int64_t amt = r.i32();
    int64_t unknown = r.i32();
    py::list<SubEntry> subs;
    for (int64_t k = 0; k < amt; ++k) {
        SubEntry e;
        e.pos = r.p;
        e.type = r.i32();
        e.startOffset = r.i32();
        e.size = r.i32();
        e.minusOne = r.i32();
        subs.append(e);
    }
    Sublayer sub;
    sub.kind = py::str("PSL");
    sub.sectionSize = sectionSize;
    sub.amt = amt;
    sub.unknown = unknown;
    sub.subs = subs;
    return sub;
}

Layer read_layer(R& r, int64_t sect_off) {
    int64_t entry_pos = r.p;
    py::bytes lt = r.raw(4);
    r.i32();
    for (int k = 0; k < 5; ++k) {
        r.i32();
    }
    int64_t offToStart = r.i32();
    int64_t totalLayerSize = r.i32();
    int64_t totalLayerSize2 = r.i32();
    for (int k = 0; k < 4; ++k) {
        r.i32();
    }
    int64_t offToPart2 = r.i32();
    r.i32();
    int64_t save = r.p;
    r.p = sect_off + offToPart2;
    py::bytes tag = r.raw(4);
    Sublayer sub;
    if (tag == py::bytes("PSL\0", 4)) {
        sub = read_psl(r);
    } else if (tag == py::bytes("PSLD")) {
        sub.kind = py::str("PSLD");
        sub.sectionSize = r.i32();
        for (int k = 0; k < 6; ++k) {
            sub.vals.append(r.i32());
        }
    } else {
        throw py::ValueError(py::str("bad sublayer tag ").utf8() + py::repr(tag).utf8() + " at 0x" +
                             py::format(r.p - 4, "X").utf8());
    }
    r.p = save;
    Layer L;
    L.entry_pos = entry_pos;
    L.type = lt;
    L.offToStart = offToStart;
    L.totalLayerSize = totalLayerSize;
    L.totalLayerSize2 = totalLayerSize2;
    L.offToPart2 = offToPart2;
    L.sub = sub;
    return L;
}

void get_assets(R& r, Layer& L, int64_t layerStart) {
    Sublayer& sub = L.sub;
    if (sub.kind != py::str("PSL")) {
        return;
    }
    for (int64_t k = 0; k < py::len(sub.subs); ++k) {
        SubEntry ss = sub.subs[k];
        if (ss.type == 0) {   // AssetList
            r.p = layerStart + ss.startOffset;
            int64_t count = r.i32();
            r.p += 28;
            for (int64_t n = 0; n < count; ++n) {
                Asset a;
                a.hdr_pos = r.p;
                a.totalDataSize = r.i32();
                a.relOff = r.i32();
                a.absSizeOff = r.p;
                a.actualSize = r.i32();
                r.i32();
                a.assetID = r.u64();
                a.assetType = r.u32();
                r.i32();
                a.absDataOff = layerStart + a.relOff;
                a.layerStart = layerStart;
                sub.assets.append(a);
            }
        }
    }
}

}  // namespace

Archive parse(const py::bytes& data) {
    if (py::len(data) < 2048 + 96) {
        throw py::ValueError(py::percent("too small to be a .HO archive (%d bytes)", py::len(data)).utf8());
    }
    R r(data);
    py::bytes hel = r.raw(2048);
    int64_t mast_off = r.p;
    py::list<int64_t> mast_ints;
    for (int k = 0; k < 24; ++k) {
        mast_ints.append(r.i32());
    }
    int64_t num = mast_ints[15];
    int64_t sect_off = num * 2048;
    if (num <= 0 || sect_off + 32 > py::len(data)) {
        throw py::ValueError("not a valid .HO archive (bad MAST/SECT pointer)");
    }
    r.p = sect_off;
    py::list<int64_t> sect_head;
    for (int k = 0; k < 8; ++k) {
        sect_head.append(r.i32());
    }
    int64_t nlayers = sect_head[1];
    if (!(0 <= nlayers && nlayers < 100000)) {
        throw py::ValueError(py::percent("not a valid .HO archive (implausible layer count %d)", nlayers).utf8());
    }
    py::list<Layer> layers;
    for (int64_t k = 0; k < nlayers; ++k) {
        layers.append(read_layer(r, sect_off));
    }
    for (int64_t li = 0; li < py::len(layers); ++li) {
        Layer& L = layers[li];
        get_assets(r, L, L.offToStart * 2048);
        if (L.sub.kind == py::str("PSL")) {
            for (int64_t k = 0; k < py::len(L.sub.assets); ++k) {
                L.sub.assets[k].layer_idx = li;   // tag each asset with its layer (for dup-id handling)
            }
        }
    }
    Archive P;
    P.hel = hel;
    P.mast_off = mast_off;
    P.mast_ints = mast_ints;
    P.sect_off = sect_off;
    P.sect_head = sect_head;
    P.layers = layers;
    P.size = py::len(data);
    return P;
}

py::list<Asset> all_assets(const Archive& P) {
    py::list<Asset> out;
    for (int64_t i = 0; i < py::len(P.layers); ++i) {
        const Layer& L = P.layers[i];
        if (L.sub.kind == py::str("PSL")) {
            out.extend(L.sub.assets);
        }
    }
    return out;
}

py::list<Asset> find_assets(const Archive& P, uint64_t assetID, const std::optional<int64_t>& layer_idx) {
    py::list<Asset> out;
    for (const Asset& a : all_assets(P)) {
        if (a.assetID == assetID && (!layer_idx || a.layer_idx == layer_idx)) {
            out.append(a);
        }
    }
    return out;
}

std::optional<Asset> find_asset(const Archive& P, uint64_t assetID, const std::optional<int64_t>& layer_idx) {
    py::list<Asset> ms = find_assets(P, assetID, layer_idx);
    if (py::len(ms) == 0) {
        return std::nullopt;
    }
    if (py::len(ms) > 1) {
        py::set<int64_t> layers;
        for (const Asset& a : ms) {
            layers.add(a.layer_idx.value_or(0));   // (every asset parse() makes has its layer)
        }
        throw py::ValueError(py::str("asset ").utf8() + hex16(assetID).utf8() + " is ambiguous: " + num(py::len(ms)) +
                             " instances in layers " + py::to_str(py::sorted(layers)).utf8() + " (pass a layer)");
    }
    return ms[0];
}

py::bytes asset_bytes(const py::bytes& data, const Asset& a) {
    return data.slice(a.absDataOff, a.absDataOff + a.actualSize);
}

int64_t align(int64_t n, int64_t a) {
    return py::floordiv(n + a - 1, a) * a;
}

py::bytearray replace(const Archive& P, const py::bytes& data, uint64_t assetID, const py::bytes& newdata,
                      const std::optional<int64_t>& layer_idx) {
    if (!layer_idx && py::len(find_assets(P, assetID)) > 1) {
        throw py::ValueError(py::str("asset ").utf8() + hex16(assetID).utf8() + " is ambiguous (multiple layers); pass a layer index");
    }
    py::bytearray buf(data);
    int64_t found_layer = -1;
    int64_t found_asset = -1;   // the asset's position in its layer's list (the Python's identity)
    for (int64_t li = 0; li < py::len(P.layers); ++li) {
        const Layer& L = P.layers[li];
        if (layer_idx && li != *layer_idx) {
            continue;
        }
        if (L.sub.kind != py::str("PSL")) {
            continue;
        }
        for (int64_t k = 0; k < py::len(L.sub.assets); ++k) {
            if (L.sub.assets[k].assetID == assetID) {
                found_layer = li;
                found_asset = k;
                break;
            }
        }
        if (found_layer >= 0) {
            break;
        }
    }
    if (found_layer < 0) {
        throw py::KeyError(py::str("asset ").utf8() + hex16(assetID).utf8() + " not found");
    }
    const Layer& L = P.layers[found_layer];
    const Asset a = L.sub.assets[found_asset];
    int64_t oldSlot = a.totalDataSize;
    int64_t newActual = py::len(newdata);
    int64_t newSlot = align(newActual, 64);
    if (newSlot == oldSlot) {
        int64_t off = a.absDataOff;
        buf.set_slice(off, off + newActual, newdata);
        for (int64_t i = off + newActual; i < off + oldSlot; ++i) {
            buf[i] = 0;
        }
        pack_i32(buf, a.absSizeOff, newActual);
        return buf;
    }

    int64_t layerStart = L.offToStart * 2048;
    py::list<int64_t> order;   // the layer's assets by position, sorted by relOff (stable)
    for (int64_t k = 0; k < py::len(L.sub.assets); ++k) {
        order.append(k);
    }
    order.sort([&](int64_t k) { return L.sub.assets[k].relOff; });
    int64_t adStart = L.sub.assets[order[0]].relOff;
    py::bytearray block(data.slice(layerStart, layerStart + adStart));
    py::bytearray adata;
    py::dict<int64_t, int64_t> newrel;   // asset position -> its new relOff
    for (int64_t k : order) {
        const Asset& aj = L.sub.assets[k];
        newrel.set(k, adStart + adata.size());
        if (k == found_asset) {
            adata += newdata;
            adata += py::bytes::zeros(newSlot - newActual);
        } else {
            adata += data.slice(aj.absDataOff, aj.absDataOff + aj.totalDataSize);
        }
    }
    block += adata.view();
    int64_t newTotal = align(adStart + adata.size(), 0x800);
    block += py::bytes::zeros(std::max<int64_t>(0, newTotal - block.size()));
    for (int64_t k : order) {
        const Asset& aj = L.sub.assets[k];
        int64_t hr = aj.hdr_pos - layerStart;
        pack_i32(block, hr + 4, newrel.at(k));
        if (k == found_asset) {
            pack_i32(block, hr + 0, newSlot);
            pack_i32(block, hr + 8, newActual);
        }
    }
    while (buf.size() % 0x800 != 0) {
        buf.append(0);
    }
    int64_t newBlockOff = buf.size();
    buf += block.view();
    int64_t ep = L.entry_pos;
    pack_i32(buf, ep + 28, py::floordiv(newBlockOff, 2048));
    pack_i32(buf, ep + 32, newTotal);
    pack_i32(buf, ep + 36, newTotal);
    for (int64_t k = 0; k < py::len(L.sub.subs); ++k) {
        const SubEntry ss = L.sub.subs[k];
        if (ss.type == 1) {
            pack_i32(buf, ss.pos + 8, adata.size());
        }
    }
    return buf;
}

// ---- textures ---------------------------------------------------------------------------------------

std::tuple<uint64_t, int64_t, int64_t, int64_t> texture_descriptor(const py::bytes& desc) {
    uint64_t blob_id = be_uint(unpack_at(desc, 0, 8), 8);
    int64_t size = static_cast<int64_t>(be_uint(unpack_at(desc, 0x34, 4), 4));
    return std::make_tuple(blob_id, desc[0x33], (size & 0x1FFF) + 1, ((size >> 13) & 0x1FFF) + 1);
}

std::tuple<xtexture::Image, int64_t, int64_t, int64_t> texture(const py::bytes& data, const Archive& P, uint64_t descriptor_id,
                                                               const std::optional<int64_t>& layer_idx, bool force_opaque) {
    std::optional<Asset> da = find_asset(P, descriptor_id, layer_idx);
    if (!da) {
        throw py::KeyError(py::str("texture descriptor ").utf8() + hex16(descriptor_id).utf8() + " not found");
    }
    auto [blob_id, fmt, w, h] = texture_descriptor(asset_bytes(data, *da));
    std::optional<Asset> ba = find_asset(P, blob_id, da->layer_idx);   // the descriptor's own layer first
    if (!ba) {
        py::list<Asset> ms = find_assets(P, blob_id);
        if (py::len(ms) > 0) {
            ba = ms[0];
        }
    }
    if (!ba) {
        throw py::KeyError(py::str("pixel blob ").utf8() + hex16(blob_id).utf8() + " not found");
    }
    if (!TEXTURE_FORMATS.contains(fmt)) {
        throw py::ValueError(py::str("texture ").utf8() + hex16(descriptor_id).utf8() + ": format $" +
                             py::percent("%02X", fmt).utf8() + " is not decoded");
    }
    xtexture::Image image = xtexture::decode_dxt(asset_bytes(data, *ba), w, h, TEXTURE_FORMATS.at(fmt), true, true, force_opaque);
    return std::make_tuple(image, w, h, fmt);
}

// ---- as a media volume ------------------------------------------------------------------------------

bool is_archive(const py::str& path) {
    if (!path.lower().endswith(py::str(".ho"))) {
        return false;
    }
    try {
        py::bytes data;
        {
            py::file f = py::open_rb(path);
            data = f.read();
        }
        parse(data);
        return true;
    } catch (const py::OSError&) {
    } catch (const py::ValueError&) {
    } catch (const py::StructError&) {
    } catch (const py::IndexError&) {
    }
    return false;
}

VolumeListing volume_listing(const py::str& path) {
    py::bytes data;
    {
        py::file f = py::open_rb(path);
        data = f.read();
    }
    Archive P = parse(data);
    py::dict<py::str, Asset> by_name;
    for (const Asset& a : all_assets(P)) {
        by_name.set(py::percent("layer%02d/%s_%08X.bin", a.layer_idx.value_or(0), hex16(a.assetID), a.assetType), a);
    }
    VolumeListing out;
    for (const auto& kv : by_name.items()) {
        out.entries.append(std::make_tuple(std::get<0>(kv), std::get<1>(kv).actualSize));
    }
    out.read = [data, by_name](const py::str& name) {
        if (!by_name.contains(name)) {
            throw py::KeyError(name.utf8());   // by_name[name]
        }
        return asset_bytes(data, by_name.at(name));
    };
    return out;
}

// ---- the command ------------------------------------------------------------------------------------

// (as Python 3.13+ keeps a docstring: its continuation lines without their common indent)
const py::str MAIN_DOC = py::str(
    "python -m codebreaker heavyiron info|layers|dups <file.ho>\n"
    "python -m codebreaker heavyiron list <file.ho> [--type 0xTYPE]\n"
    "python -m codebreaker heavyiron extract <file.ho> <folder> [--type 0xTYPE]\n"
    "python -m codebreaker heavyiron replace <file.ho> <asset id hex> <new bytes file> <out.ho> [--layer N]\n"
    "python -m codebreaker heavyiron textures <file.ho> <folder>       every DXT texture as a PNG");

namespace {

void put32(std::string& o, uint32_t v) {
    for (int i = 3; i >= 0; --i) {
        o.push_back(static_cast<char>((v >> (8 * i)) & 0xFF));
    }
}

void put_chunk(std::string& out, const char* type, const std::string& body) {
    put32(out, static_cast<uint32_t>(body.size()));
    std::string tb = std::string(type) + body;
    out += tb;
    put32(out, static_cast<uint32_t>(py::crc32(py::bytes(tb))));
}

// A PNG of an RGBA image: 8 bits a channel, no filter, the pixels in zlib "stored" blocks.
void save_png(const xtexture::Image& image, const py::str& path) {
    const std::string& px = image.pixels.raw();
    std::string raw;
    int64_t row = image.width * 4;
    for (int64_t y = 0; y < image.height; ++y) {
        raw.push_back('\0');
        raw.append(px, static_cast<size_t>(y * row), static_cast<size_t>(row));
    }
    std::string z = "\x78\x01";
    size_t at = 0;
    do {
        size_t n = std::min<size_t>(65535, raw.size() - at);
        bool last = at + n >= raw.size();
        z.push_back(last ? 1 : 0);
        z.push_back(static_cast<char>(n & 0xFF));
        z.push_back(static_cast<char>(n >> 8));
        z.push_back(static_cast<char>(~n & 0xFF));
        z.push_back(static_cast<char>((~n >> 8) & 0xFF));
        z.append(raw, at, n);
        at += n;
    } while (at < raw.size());
    uint32_t a = 1, b = 0;
    for (unsigned char c : raw) {
        a = (a + c) % 65521;
        b = (b + a) % 65521;
    }
    put32(z, (b << 16) | a);
    std::string out("\x89PNG\r\n\x1a\n", 8);
    std::string hdr;
    put32(hdr, static_cast<uint32_t>(image.width));
    put32(hdr, static_cast<uint32_t>(image.height));
    hdr += std::string("\x08\x06\x00\x00\x00", 5);
    put_chunk(out, "IHDR", hdr);
    put_chunk(out, "IDAT", z);
    put_chunk(out, "IEND", "");
    py::file f = py::open(path, py::str("wb"));
    f.write(py::bytes(out));
}

void write_bytes(const py::str& path, const py::bytes& data) {
    py::file f = py::open(path, py::str("wb"));
    f.write(data);
}

// int(text, 16) as an asset id: the whole unsigned 64-bit range (a negative number is one no asset has: its
// two's complement, which the messages below print as Python prints the negative number)
struct IdText {
    uint64_t id;
    bool negative;
};
IdText parse_id(const py::str& text) {
    try {
        int64_t v = py::int_parse(text, 16);
        return {static_cast<uint64_t>(v), v < 0};
    } catch (const py::OverflowError&) {
        // past int64: read the digits again as unsigned (still at most 64 bits)
        std::string t = text.utf8();
        size_t i = 0;
        while (i < t.size() && (t[i] == ' ' || (t[i] >= 9 && t[i] <= 13))) {
            ++i;
        }
        if (i < t.size() && t[i] == '-') {
            throw;   // a negative number past int64: beyond what this port holds
        }
        if (i < t.size() && t[i] == '+') {
            ++i;
        }
        if (i + 1 < t.size() && t[i] == '0' && (t[i + 1] == 'x' || t[i + 1] == 'X')) {
            i += 2;
            if (i < t.size() && t[i] == '_') {
                ++i;
            }
        }
        uint64_t v = 0;
        for (; i < t.size(); ++i) {
            char c = t[i];
            if (c == '_') {
                continue;
            }
            int d = (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'f') ? c - 'a' + 10 : (c >= 'A' && c <= 'F') ? c - 'A' + 10 : -1;
            if (d < 0) {
                break;   // trailing white space
            }
            if (v > (UINT64_MAX - static_cast<uint64_t>(d)) / 16) {
                throw py::OverflowError("int too large for uint64");
            }
            v = v * 16 + static_cast<uint64_t>(d);
        }
        return {static_cast<uint64_t>(v), false};
    }
}

}  // namespace

int64_t main(const py::list<py::str>& argv) {
    if (py::len(argv) < 2) {
        py::print(MAIN_DOC);
        return 2;
    }

    auto option = [&](const char* name, auto conv) -> std::optional<decltype(conv(py::str()))> {
        if (!argv.contains(py::str(name))) {
            return std::nullopt;
        }
        return conv(argv[argv.index(py::str(name)) + 1]);
    };
    py::str command = argv[0];
    py::str path = argv[1];
    py::bytes data;
    {
        py::file f = py::open_rb(path);
        data = f.read();
    }
    Archive P = parse(data);
    py::list<Asset> A = all_assets(P);
    std::optional<int64_t> kind = option("--type", [](const py::str& x) { return py::int_parse(x, 0); });
    if (kind) {
        py::list<Asset> kept;
        for (const Asset& a : A) {
            if (a.assetType == *kind) {
                kept.append(a);
            }
        }
        A = kept;
    }
    auto type_name = [](int64_t t) { return ASSET_TYPES.get(t, py::str("")); };

    if (command == py::str("info")) {
        py::print(py::percent("%s: %d bytes, %d layers, %d assets", py::os::path::basename(path), P.size, py::len(P.layers), py::len(A)));
        py::dict<int64_t, int64_t> count, size;   // collections.Counter
        for (const Asset& a : A) {
            count[a.assetType] += 1;
            size[a.assetType] += a.actualSize;
        }
        py::list<std::tuple<int64_t, int64_t>> most_common;
        for (const auto& kv : size.items()) {
            most_common.append(kv);
        }
        most_common.sort([](const std::tuple<int64_t, int64_t>& kv) { return std::get<1>(kv); }, true);
        for (const auto& [t, n] : most_common) {
            py::print(py::percent("  $%08X %-18s %6d assets %12d bytes", t, type_name(t), count.at(t), n));
        }
    } else if (command == py::str("list")) {
        for (const Asset& a : A) {
            py::str layer = a.layer_idx ? py::to_str(*a.layer_idx) : py::str("-");
            py::print(py::percent("%s  $%08X %-18s layer %-3s %9d bytes (slot %d) at $%X", hex16(a.assetID), a.assetType,
                                  type_name(a.assetType), layer, a.actualSize, a.totalDataSize, a.absDataOff));
        }
    } else if (command == py::str("layers")) {
        for (int64_t i = 0; i < py::len(P.layers); ++i) {
            const Layer& L = P.layers[i];
            const Sublayer& sub = L.sub;
            int64_t n = sub.kind == py::str("PSL") ? py::len(sub.assets) : 0;
            py::print(py::percent("[%2d] %-8s block at $%-7X %9d bytes  %-4s %d assets", i, py::repr(L.type), L.offToStart * 2048,
                                  L.totalLayerSize, sub.kind, n));
        }
    } else if (command == py::str("dups")) {
        py::dict<uint64_t, py::set<int64_t>> seen;   // defaultdict(set)
        for (const Asset& a : all_assets(P)) {
            seen.setdefault(a.assetID, py::set<int64_t>()).add(a.layer_idx.value_or(0));   // (every asset has its layer)
        }
        py::dict<uint64_t, py::set<int64_t>> dups;
        for (const auto& kv : seen.items()) {
            if (std::get<1>(kv).size() > 1) {
                dups.set(std::get<0>(kv), std::get<1>(kv));
            }
        }
        py::print(py::percent("%d asset ids are in more than one layer (name the layer when replacing one):", dups.size()));
        std::vector<uint64_t> ids = dups.keys();
        std::sort(ids.begin(), ids.end());
        for (uint64_t k : ids) {
            py::print(py::str("  ") + hex16(k) + py::str("  layers ") + py::to_str(py::sorted(dups.at(k))));
        }
    } else if (command == py::str("extract") && py::len(argv) > 2) {
        py::os::makedirs(argv[2], 0777, true);
        for (const Asset& a : A) {
            write_bytes(py::os::path::join(argv[2], hex16(a.assetID) + py::percent("_%08X.bin", a.assetType)), asset_bytes(data, a));
        }
        py::print(py::percent("%d assets -> %s", py::len(A), argv[2]));
    } else if (command == py::str("replace") && py::len(argv) > 4) {
        py::bytes neu;
        {
            py::file f = py::open_rb(argv[3]);
            neu = f.read();
        }
        IdText id = parse_id(argv[2]);
        std::optional<int64_t> layer = option("--layer", [](const py::str& x) { return py::int_parse(x, 10); });
        if (id.negative) {   // (no asset has a negative id: "%016X" % -n is the sign and the digits)
            char digits[40];
            std::snprintf(digits, sizeof digits, "%015llX", static_cast<unsigned long long>(0 - id.id));
            throw py::KeyError(py::str("asset ").utf8() + "-" + digits + " not found");
        }
        py::bytearray out = replace(P, data, id.id, neu, layer);
        write_bytes(argv[4], out.view());
        py::print(py::percent("replaced %s (%d bytes): %d -> %d bytes, %s", argv[2], py::len(neu), py::len(data), out.size(), argv[4]));
    } else if (command == py::str("textures") && py::len(argv) > 2) {
        py::os::makedirs(argv[2], 0777, true);
        py::set<uint64_t> done;
        int64_t ok = 0, skipped = 0;
        for (const Asset& da : all_assets(P)) {
            if (da.assetType != TEXTURE_TYPE) {
                continue;
            }
            if (done.contains(da.assetID)) {
                continue;
            }
            done.add(da.assetID);
            try {
                auto [image, w, h, fmt] = texture(data, P, da.assetID, da.layer_idx);
                (void)fmt;
                save_png(image, py::os::path::join(argv[2], hex16(da.assetID) + py::percent("_%dx%d.png", w, h)));
                ok += 1;
            } catch (const py::KeyError&) {
                skipped += 1;
            } catch (const py::ValueError&) {
                skipped += 1;
            }
        }
        py::print(py::percent("%d textures decoded, %d skipped -> %s", ok, skipped, argv[2]));
    } else {
        py::print(MAIN_DOC);
        return 2;
    }
    return 0;
}

}  // namespace codebreaker::formats::xbox::heavyiron
