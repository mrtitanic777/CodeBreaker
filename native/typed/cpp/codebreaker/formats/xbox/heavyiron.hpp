// codebreaker/formats/xbox/heavyiron.hpp -- C++17 port of codebreaker.formats.xbox.heavyiron.
//
// heavyiron.py -- Heavy Iron Studios' GoodEngine archives (.HO, "HEB"), as Up, WALL-E, Ratatouille
// and the SpongeBob games ship them on the Xbox 360 and Wii; and their textures.
//
//     python -m codebreaker heavyiron <info|list|layers|dups|extract|replace|textures> <file.ho> ...
//     P = heavyiron::parse(data); heavyiron::all_assets(P); heavyiron::asset_bytes(data, a)
//     heavyiron::replace(P, data, asset_id, new_bytes)    // any size: in its slot, or the layer moved to the end
//     heavyiron::texture(data, P, descriptor_id)          // a texture asset as an RGBA image
//
// The reader follows the engine's own (read from the game's decompiled archive library); the writer
// patches the file, keeping the regions it does not understand byte for byte and fixing the offsets
// an edit moves. Archives are also media volumes (`codebreaker media list|extract`): each asset as
// layerNN/<id>_<type>.bin.
//
// Layout:
//   [0,2048)          HEL  - opaque header blob (preserved verbatim)
//   [2048,+96)        MAST - 24 x int32(BE); int[15] = num -> SECT at num*2048
//   [num*2048, ...)   SECT - 8 x int32(BE): [1] = layerCount; then layerCount x LayerEntry(64B)
//   per layer:        data block at offToStart*0x800: AssetList + AssetData + Padding
//   AssetEntry(32B):  totalDataSize, relOff, actualSize, unk0C, assetID(u64), assetType, unk1C
// All multi-byte ints are big-endian.
//
// Typing notes:
//   * The Python's dicts with fixed keys are structs: Asset (an asset's entry), SubEntry (one item of a
//     PSL section's table), Sublayer (the "sub" dict of a layer: kind "PSL" fills amt/unknown/subs/assets, kind
//     "PSLD" fills vals; the other fields stay empty), Layer, and Archive (the dict parse() returns).
//   * assetID is a uint64_t (the ids are the engine's 64-bit hashes; they use the whole range, which int64_t
//     does not hold).  Everything else is int64_t.
//   * An Asset is a value; where the Python tests two of them with `is` (replace), the C++ compares their
//     positions in the layer's list.  A Sublayer's lists are shared handles, as the Python's lists are.
//   * Python's struct.unpack_from / pack_into on a bytes / bytearray (with the negative offsets they
//     take from the end) are done here with the messages of CPython's struct.error.
//   * texture() gave a Pillow RGBA image: here the typed formats::xbox::texture::Image (width, height, the
//     pixels as tobytes() gives them); `textures` writes each as a PNG with a small encoder in the .cpp
//     (zlib "stored" blocks: the same pixels as Pillow's file, not the same compressed bytes).
//   * volume_listing returned ([(name, size)], reader function): here a struct holding both.
//   * main() takes sys.argv[1:] and returns the exit status; the Python's `sys.exit(main(sys.argv[1:]))`.
//   * Integers past int64 (an asset id is parsed from hex up to 64 bits) raise OverflowError where Python
//     would go on with a big integer; no real archive gets near it.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/formats/xbox/texture.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::heavyiron {

namespace xtexture = codebreaker::formats::xbox::texture;   // (heavyiron's own `texture` is a function)

// Known asset-type IDs (by total bytes they usually dominate)
extern const py::dict<int64_t, py::str> ASSET_TYPES;

// R -- a big-endian reader over bytes, as the Python's slotted class
class R {
  public:
    py::bytes d;
    int64_t p = 0;

    explicit R(const py::bytes& data);
    int64_t i32();
    int64_t u32();
    uint64_t u64();
    py::bytes raw(int64_t n);
};

// one entry of an AssetList
struct Asset {
    int64_t hdr_pos = 0;
    int64_t totalDataSize = 0;
    int64_t relOff = 0;
    int64_t absSizeOff = 0;
    int64_t actualSize = 0;
    uint64_t assetID = 0;
    int64_t assetType = 0;
    int64_t absDataOff = 0;
    int64_t layerStart = 0;
    std::optional<int64_t> layer_idx;   // set by parse() on every asset of a PSL layer
};

// one item of a PSL section's table
struct SubEntry {
    int64_t pos = 0;
    int64_t type = 0;
    int64_t startOffset = 0;
    int64_t size = 0;
    int64_t minusOne = 0;
};

struct Sublayer {
    py::str kind;                 // "PSL" or "PSLD"
    int64_t sectionSize = 0;
    int64_t amt = 0;              // PSL
    int64_t unknown = 0;          // PSL
    py::list<SubEntry> subs;      // PSL
    py::list<Asset> assets;       // PSL
    py::list<int64_t> vals;       // PSLD
};

struct Layer {
    int64_t entry_pos = 0;
    py::bytes type;
    int64_t offToStart = 0;
    int64_t totalLayerSize = 0;
    int64_t totalLayerSize2 = 0;
    int64_t offToPart2 = 0;
    Sublayer sub;
};

struct Archive {
    py::bytes hel;
    int64_t mast_off = 0;
    py::list<int64_t> mast_ints;
    int64_t sect_off = 0;
    py::list<int64_t> sect_head;
    py::list<Layer> layers;
    int64_t size = 0;
};

// Parse a .HO archive into a tree (header, layers, assets).
//
// Raises ValueError if the bytes don't look like a valid HEB/.HO archive (so non-archive or
// truncated input fails with a clear message rather than an opaque struct/index error).
Archive parse(const py::bytes& data);

py::list<Asset> all_assets(const Archive& P);

// Every asset matching assetID, optionally restricted to one layer.
py::list<Asset> find_assets(const Archive& P, uint64_t assetID, const std::optional<int64_t>& layer_idx = std::nullopt);

// The single asset matching assetID; None if absent.
//
// Raises ValueError if the id appears in multiple layers and no layer_idx is given -- the
// shipping game (e.g. MNUS.ho) has hundreds of duplicate ids, so silently taking the first
// would be a correctness bug. Pass a layer to disambiguate.
std::optional<Asset> find_asset(const Archive& P, uint64_t assetID, const std::optional<int64_t>& layer_idx = std::nullopt);

py::bytes asset_bytes(const py::bytes& data, const Asset& a);

int64_t align(int64_t n, int64_t a);

// Replace an asset with newdata of ANY size. Same slot -> in-place; else rebuild that
// layer's 0x800-aligned block, relocate to EOF, and fix offToStart + sizes. Pass layer_idx
// to target one instance of a duplicated id (see find_asset).
py::bytearray replace(const Archive& P, const py::bytes& data, uint64_t assetID, const py::bytes& newdata,
                      const std::optional<int64_t>& layer_idx = std::nullopt);

// ---- textures ---------------------------------------------------------------------------------------
//
// A texture is a 72-byte descriptor asset naming a separate pixel blob (the blob's id is the
// descriptor's with bit 44 set): its format byte at 0x33 ($52 DXT1, $54 DXT5) and its size packed
// at 0x34 (width - 1 in the low 13 bits, height - 1 in the next 13). The pixels are tiled and
// 8-in-16 byte-swapped as the Xbox 360 keeps them (formats/xbox/texture.py decodes them).

inline constexpr int64_t TEXTURE_TYPE = 693241887;
extern const py::dict<int64_t, py::str> TEXTURE_FORMATS;

// (blob id, format byte, width, height) of a texture descriptor.
std::tuple<uint64_t, int64_t, int64_t, int64_t> texture_descriptor(const py::bytes& desc);

// The texture a descriptor names, as an RGBA image: (image, width, height, format).
std::tuple<xtexture::Image, int64_t, int64_t, int64_t> texture(const py::bytes& data, const Archive& P, uint64_t descriptor_id,
                                                              const std::optional<int64_t>& layer_idx = std::nullopt,
                                                              bool force_opaque = false);

// ---- as a media volume ------------------------------------------------------------------------------

bool is_archive(const py::str& path);

// [(name, size)] and a reader, for formats/xbox/volumes.py.
struct VolumeListing {
    py::list<std::tuple<py::str, int64_t>> entries;
    std::function<py::bytes(const py::str&)> read;
};
VolumeListing volume_listing(const py::str& path);

// ---- the command ------------------------------------------------------------------------------------

// main's docstring: the usage text
extern const py::str MAIN_DOC;

// python -m codebreaker heavyiron ...; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::heavyiron
