// cb/formats/xbox/heavyiron.hpp -- Heavy Iron Studios' GoodEngine archives (.HO, "HEB"), as Up, WALL-E, Ratatouille and the
// SpongeBob games ship them on the Xbox 360 and Wii; and the textures inside them.
//
// The reader follows the engine's own (read from the game's decompiled archive library); the writer patches the file,
// keeping the regions it does not understand byte for byte and fixing the offsets an edit moves.
//
// Layout (all multi-byte integers are big-endian):
//   [0,2048)          HEL  - opaque header blob (preserved verbatim)
//   [2048,+96)        MAST - 24 x int32; int[15] = n -> SECT at n*2048
//   [n*2048, ...)     SECT - 8 x int32: [1] = layer count; then one 64-byte layer entry per layer, then the section tables
//   per layer:        a data block at start_block*0x800: AssetList + asset data + padding
//   asset entry (32): slot size, offset in the block, actual size, unknown, id (u64), type, unknown
//
// A texture is a 72-byte descriptor asset naming a separate pixel blob (the blob's id is the descriptor's with bit 44 set):
// its format byte at 0x33 (0x52 DXT1, 0x54 DXT5) and its size packed at 0x34 (width - 1 in the low 13 bits, height - 1 in the
// next 13). The pixels are tiled and 16-bit byte-swapped as the Xbox 360 keeps them (xbox/texture decodes them).
//
// Damage: parse() throws FormatError (TruncatedError where a read runs off the end) for bytes that are not an archive;
// try_parse() answers with a Result instead, for probing. An asset whose data does not lie inside the file is parsed (the
// archive is still listable) and reported by asset_bytes()/replace() as a FormatError.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/formats/xbox/texture.hpp"

namespace cb::formats::xbox::heavyiron {

inline constexpr std::size_t kBlock = 0x800;        // archive blocks (and layer data blocks) are 2048 bytes
inline constexpr std::size_t kHeaderSize = 2048;    // HEL
inline constexpr std::size_t kMastSize = 96;        // MAST: 24 x int32
inline constexpr std::uint32_t kTextureType = 693241887;   // asset type of a texture descriptor

// The name of a well-known asset type ("RawBlob", "TextureDescriptor", "StaticGeometry"), empty for any other.
std::string_view type_name(std::uint32_t asset_type);

// One entry of a layer's AssetList.
struct Asset {
    std::uint64_t id = 0;                  // the engine's 64-bit hash of the asset's name
    std::uint32_t type = 0;
    std::size_t layer = 0;                 // index of the layer it is in
    std::uint32_t slot_size = 0;           // bytes reserved for the data (a multiple of 64 in files the engine wrote)
    std::uint32_t actual_size = 0;         // bytes of data in the slot
    std::uint64_t entry_pos = 0;           // file offset of this entry
    std::uint64_t size_pos = 0;            // file offset of its actual_size field
    std::uint64_t block_offset = 0;        // offset of the data from the start of the layer's data block
    std::uint64_t block_start = 0;         // file offset of the layer's data block
    std::uint64_t data_pos() const { return block_start + block_offset; }
};

// One item of a PSL section's table (kind 0: the AssetList; kind 1: the asset data).
struct Section {
    std::uint64_t pos = 0;                 // file offset of the entry
    std::int32_t kind = 0;
    std::int32_t start_offset = 0;
    std::int32_t size = 0;
    std::int32_t minus_one = 0;            // always -1 in the files seen
};

struct Layer {
    std::uint64_t entry_pos = 0;           // file offset of the layer's 64-byte entry
    std::array<char, 4> type{};            // the four tag bytes of the entry
    std::uint32_t start_block = 0;         // its data block, in units of kBlock
    std::uint32_t total_size = 0;
    std::uint32_t total_size2 = 0;
    std::int32_t part2_offset = 0;         // where its section table is, from the SECT

    enum class Kind { Psl, Psld };
    Kind kind = Kind::Psl;
    std::int32_t section_size = 0;
    // Psl: the section table and the layer's assets, in the order the AssetList gives them
    std::int32_t unknown = 0;
    std::vector<Section> sections;
    std::vector<Asset> assets;
    // Psld: six words of unknown meaning
    std::array<std::int32_t, 6> psld_values{};

    std::uint64_t block_pos() const { return std::uint64_t{start_block} * kBlock; }
};

class Archive {
public:
    // Parse the bytes of a .HO archive. The Archive keeps no reference to them: the accessors that read data take them again.
    static Archive parse(ByteView data);
    static Result<Archive> try_parse(ByteView data);

    std::size_t file_size() const { return size_; }
    std::uint64_t mast_pos() const { return kHeaderSize; }
    const std::array<std::int32_t, 24>& mast() const { return mast_; }
    std::uint64_t sect_pos() const { return sect_pos_; }
    const std::array<std::int32_t, 8>& sect_head() const { return sect_head_; }
    const std::vector<Layer>& layers() const { return layers_; }

    // Every asset of every PSL layer, in layer order.
    std::vector<const Asset*> assets() const;
    // Every asset with this id, optionally only in one layer.
    std::vector<const Asset*> find_all(std::uint64_t id, std::optional<std::size_t> layer = std::nullopt) const;
    // The one asset with this id; nullptr if there is none. UsageError if the id is in several layers and none was named --
    // shipping archives (MNUS.ho) have hundreds of duplicate ids, so taking the first would be a bug.
    const Asset* find(std::uint64_t id, std::optional<std::size_t> layer = std::nullopt) const;

private:
    std::size_t size_ = 0;
    std::array<std::int32_t, 24> mast_{};
    std::uint64_t sect_pos_ = 0;
    std::array<std::int32_t, 8> sect_head_{};
    std::vector<Layer> layers_;
};

// The data of an asset (a view into `file`). FormatError if it does not lie inside the file.
ByteView asset_bytes(ByteView file, const Asset& asset);

// A copy of `file` with the asset's data replaced, by data of any size: in its slot when the new data needs the same slot
// (a multiple of 64 bytes), otherwise the layer's 0x800-aligned block is rebuilt and moved to the end of the file, and its
// offsets and sizes fixed. `layer` picks one instance of a duplicated id. UsageError if the id is ambiguous or not there;
// FormatError if the archive's own offsets do not hold together.
Bytes replace(ByteView file, const Archive& archive, std::uint64_t id, ByteView new_data, std::optional<std::size_t> layer = std::nullopt);

// ---- textures ------------------------------------------------------------------------------------------

struct TextureDescriptor {
    std::uint64_t blob_id = 0;
    std::uint8_t format_byte = 0;          // 0x52 DXT1, 0x54 DXT5
    std::uint32_t width = 0;
    std::uint32_t height = 0;
};
// The fields of a 72-byte descriptor. TruncatedError if it is shorter than 0x38 bytes.
TextureDescriptor texture_descriptor(ByteView descriptor);
// The DXT format of a format byte; nullopt for one that is not decoded.
std::optional<texture::Format> texture_format(std::uint8_t format_byte);

struct Texture {
    texture::Image image;
    std::uint8_t format_byte = 0;
};
// The texture a descriptor asset names, as RGBA pixels. The pixel blob is looked for in the descriptor's own layer first, then
// anywhere. Errors (in the Result): UsageError for an ambiguous or missing descriptor or blob, UnsupportedError for a format
// byte that is not DXT1/DXT5, FormatError for damaged data.
Result<Texture> decode_texture(ByteView file, const Archive& archive, std::uint64_t descriptor_id, std::optional<std::size_t> layer = std::nullopt,
                        bool force_opaque = false);

// ---- as a media volume ---------------------------------------------------------------------------------

// Whether the file is a .HO archive: the extension and a parse that succeeds.
bool is_archive(const std::filesystem::path& path);

// Each asset as "layerNN/<id>_<type>.bin" with its size; the same name opens its data.
struct VolumeEntry {
    std::string name;
    std::uint32_t size = 0;
    const Asset* asset = nullptr;
};
std::vector<VolumeEntry> volume_entries(const Archive& archive);

// ---- the command ---------------------------------------------------------------------------------------

inline constexpr std::string_view kUsage =
    "codebreaker heavyiron info|layers|dups <file.ho>\n"
    "codebreaker heavyiron list <file.ho> [--type 0xTYPE]\n"
    "codebreaker heavyiron extract <file.ho> <folder> [--type 0xTYPE]\n"
    "codebreaker heavyiron replace <file.ho> <asset id hex> <new bytes file> <out.ho> [--layer N]\n"
    "codebreaker heavyiron textures <file.ho> <folder>       every DXT texture as a PNG\n";

// The `heavyiron` command; `args` are the words after its name. Prints its report with cb::out and returns the exit status
// (0, or 2 with the usage text for a command line it does not understand). Failures to read or parse are thrown.
int command(const std::vector<std::string>& args);

// A PNG file (8-bit RGBA, unfiltered rows) for an image.
Bytes png_bytes(const texture::Image& image);

}  // namespace cb::formats::xbox::heavyiron
