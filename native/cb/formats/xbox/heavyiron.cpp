#include "cb/formats/xbox/heavyiron.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <map>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "zlib.h"

namespace cb::formats::xbox::heavyiron {

namespace {

constexpr std::string_view kReader = "heavyiron";
constexpr std::int32_t kMaxLayers = 100000;   // a larger count is damage, not an archive
constexpr std::uint32_t kSlotAlign = 64;      // asset slots
constexpr std::size_t kAssetEntrySize = 32;
constexpr std::size_t kAssetListHeader = 32;  // the count, then 28 bytes the reader skips

[[noreturn]] void bad(std::uint64_t offset, const std::string& what) { throw FormatError(kReader, offset, what); }

// A signed field of the file that must not be negative (an offset, a size or a count).
std::uint32_t non_negative(std::int32_t v, std::uint64_t at, std::string_view what) {
    if (v < 0) bad(at, std::format("negative {} ({})", what, v));
    return static_cast<std::uint32_t>(v);
}

std::uint64_t align_up(std::uint64_t n, std::uint64_t a) { return (n + a - 1) / a * a; }

Layer read_layer(io::ByteReader& r, std::uint64_t sect_pos) {
    Layer layer;
    layer.entry_pos = r.pos();
    const ByteView tag = r.bytes(4);
    std::memcpy(layer.type.data(), tag.data(), 4);
    r.skip(4 + 5 * 4);
    layer.start_block = non_negative(r.i32_be(), layer.entry_pos + 28, "block number");
    layer.total_size = non_negative(r.i32_be(), layer.entry_pos + 32, "layer size");
    layer.total_size2 = non_negative(r.i32_be(), layer.entry_pos + 36, "layer size");
    r.skip(4 * 4);
    layer.part2_offset = r.i32_be();
    r.skip(4);
    const std::size_t next_entry = r.pos();

    const std::int64_t part2 = static_cast<std::int64_t>(sect_pos) + layer.part2_offset;
    if (layer.part2_offset < 0 || part2 > static_cast<std::int64_t>(r.size())) bad(layer.entry_pos + 56, std::format("section table offset {} is outside the file", layer.part2_offset));
    r.seek(static_cast<std::uint64_t>(part2));
    const ByteView section_tag = r.bytes(4);
    if (section_tag.size() == 4 && std::memcmp(section_tag.data(), "PSL\0", 4) == 0) {
        layer.kind = Layer::Kind::Psl;
        layer.section_size = r.i32_be();
        const std::int32_t count = r.i32_be();
        layer.unknown = r.i32_be();
        if (count < 0) bad(r.pos() - 8, std::format("negative section count ({})", count));
        for (std::int32_t i = 0; i < count; ++i) {
            Section s;
            s.pos = r.pos();
            s.kind = r.i32_be();
            s.start_offset = r.i32_be();
            s.size = r.i32_be();
            s.minus_one = r.i32_be();
            layer.sections.push_back(s);
        }
    } else if (std::memcmp(section_tag.data(), "PSLD", 4) == 0) {
        layer.kind = Layer::Kind::Psld;
        layer.section_size = r.i32_be();
        for (std::int32_t& v : layer.psld_values) v = r.i32_be();
    } else {
        bad(r.pos() - 4, "unknown section tag");
    }
    r.seek(next_entry);
    return layer;
}

// The AssetList of a PSL layer: for every section of kind 0, a count and then that many 32-byte entries.
void read_assets(io::ByteReader& r, Layer& layer, std::size_t layer_index) {
    if (layer.kind != Layer::Kind::Psl) return;
    const std::uint64_t block_start = layer.block_pos();
    for (const Section& s : layer.sections) {
        if (s.kind != 0) continue;
        const std::int64_t list = static_cast<std::int64_t>(block_start) + s.start_offset;
        if (list < 0 || list > static_cast<std::int64_t>(r.size())) bad(s.pos, std::format("asset list offset {} is outside the file", s.start_offset));
        r.seek(static_cast<std::uint64_t>(list));
        const std::int32_t count = r.i32_be();
        if (count < 0) bad(static_cast<std::uint64_t>(list), std::format("negative asset count ({})", count));
        r.skip(kAssetListHeader - 4);
        for (std::int32_t i = 0; i < count; ++i) {
            Asset a;
            a.layer = layer_index;
            a.entry_pos = r.pos();
            a.slot_size = non_negative(r.i32_be(), a.entry_pos, "asset slot size");
            a.block_offset = non_negative(r.i32_be(), a.entry_pos + 4, "asset offset");
            a.size_pos = r.pos();
            a.actual_size = non_negative(r.i32_be(), a.size_pos, "asset size");
            r.skip(4);
            a.id = r.u64_be();
            a.type = r.u32_be();
            r.skip(4);
            a.block_start = block_start;
            layer.assets.push_back(a);
        }
    }
}

void put_be32(Bytes& b, std::uint64_t at, std::uint32_t v) {
    if (at > b.size() || b.size() - at < 4) bad(at, "an offset to patch lies outside the data");
    for (int i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

std::uint32_t fits_int32(std::uint64_t v, std::string_view what) {
    if (v > 0x7FFFFFFFu) throw UnsupportedError(kReader, 0, std::format("{} ({}) does not fit the archive's 32-bit fields", what, v));
    return static_cast<std::uint32_t>(v);
}

std::string hex_id(std::uint64_t id) { return std::format("{:016X}", id); }

}  // namespace

std::string_view type_name(std::uint32_t asset_type) {
    switch (asset_type) {
        case 0xBDE21A8D: return "RawBlob";   // pixel / geometry data blobs
        case kTextureType: return "TextureDescriptor";
        case 0x86EF2978: return "StaticGeometry";
        default: return {};
    }
}

// ---- reading ---------------------------------------------------------------------------------------------------

Archive Archive::parse(ByteView data) {
    if (data.size() < kHeaderSize + kMastSize) bad(0, std::format("too small to be a .HO archive ({} bytes)", data.size()));
    io::ByteReader r(data, kReader);
    Archive a;
    a.size_ = data.size();
    r.seek(kHeaderSize);
    for (std::int32_t& v : a.mast_) v = r.i32_be();
    const std::int32_t num = a.mast_[15];
    a.sect_pos_ = static_cast<std::uint64_t>(std::max(num, 0)) * kBlock;
    if (num <= 0 || a.sect_pos_ + 32 > data.size()) bad(kHeaderSize + 15 * 4, "bad MAST/SECT pointer");
    r.seek(a.sect_pos_);
    for (std::int32_t& v : a.sect_head_) v = r.i32_be();
    const std::int32_t layer_count = a.sect_head_[1];
    if (layer_count < 0 || layer_count >= kMaxLayers) bad(a.sect_pos_ + 4, std::format("implausible layer count {}", layer_count));
    a.layers_.reserve(static_cast<std::size_t>(layer_count));
    for (std::int32_t i = 0; i < layer_count; ++i) a.layers_.push_back(read_layer(r, a.sect_pos_));
    for (std::size_t i = 0; i < a.layers_.size(); ++i) read_assets(r, a.layers_[i], i);
    return a;
}

Result<Archive> Archive::try_parse(ByteView data) {
    try {
        return parse(data);
    } catch (const FormatError& e) {   // an isolation boundary: damage is this probe's answer
        return Failure(e.clone());
    }
}

std::vector<const Asset*> Archive::assets() const {
    std::vector<const Asset*> out;
    for (const Layer& l : layers_) {
        for (const Asset& a : l.assets) out.push_back(&a);
    }
    return out;
}

std::vector<const Asset*> Archive::find_all(std::uint64_t id, std::optional<std::size_t> layer) const {
    std::vector<const Asset*> out;
    for (const Asset* a : assets()) {
        if (a->id == id && (!layer || a->layer == *layer)) out.push_back(a);
    }
    return out;
}

const Asset* Archive::find(std::uint64_t id, std::optional<std::size_t> layer) const {
    const std::vector<const Asset*> found = find_all(id, layer);
    if (found.empty()) return nullptr;
    if (found.size() > 1) {
        std::string where;
        std::size_t last = static_cast<std::size_t>(-1);
        for (const Asset* a : found) {   // (layers come in order)
            if (a->layer == last) continue;
            last = a->layer;
            if (!where.empty()) where += ", ";
            where += std::to_string(a->layer);
        }
        throw UsageError(std::format("asset {} is ambiguous: {} instances in layers {} (name a layer)", hex_id(id), found.size(), where));
    }
    return found.front();
}

ByteView asset_bytes(ByteView file, const Asset& asset) { return io::checked_range(file, asset.data_pos(), asset.actual_size); }

// ---- writing ---------------------------------------------------------------------------------------------------

Bytes replace(ByteView file, const Archive& archive, std::uint64_t id, ByteView new_data, std::optional<std::size_t> layer) {
    const std::vector<const Asset*> candidates = archive.find_all(id, layer);
    if (!layer && candidates.size() > 1) throw UsageError(std::format("asset {} is ambiguous (several layers); name a layer", hex_id(id)));
    if (candidates.empty()) throw UsageError(std::format("asset {} not found", hex_id(id)));
    const Asset& target = *candidates.front();
    const Layer& owner = archive.layers()[target.layer];

    Bytes buf(file.begin(), file.end());
    const std::uint64_t old_slot = target.slot_size;
    const std::uint64_t new_actual = new_data.size();
    const std::uint64_t new_slot = align_up(new_actual, kSlotAlign);
    const std::uint32_t new_actual32 = fits_int32(new_actual, "the new data");

    if (new_slot == old_slot) {   // it fits its slot: write it in place, clear the rest of the slot
        io::checked_range(buf, target.data_pos(), old_slot);
        std::copy(new_data.begin(), new_data.end(), buf.begin() + static_cast<std::ptrdiff_t>(target.data_pos()));
        std::fill(buf.begin() + static_cast<std::ptrdiff_t>(target.data_pos() + new_actual), buf.begin() + static_cast<std::ptrdiff_t>(target.data_pos() + old_slot), std::uint8_t{0});
        put_be32(buf, target.size_pos, new_actual32);
        return buf;
    }

    // Otherwise rebuild the layer's block (its assets in the order of their offsets), and move it to the end of the file.
    const std::uint64_t block_start = owner.block_pos();
    std::vector<const Asset*> order;
    for (const Asset& a : owner.assets) order.push_back(&a);
    std::stable_sort(order.begin(), order.end(), [](const Asset* x, const Asset* y) { return x->block_offset < y->block_offset; });
    const std::uint64_t data_start = order.front()->block_offset;   // where the asset data begins in the block
    const ByteView head = io::checked_range(file, block_start, data_start);
    Bytes block(head.begin(), head.end());
    Bytes asset_data;
    std::map<const Asset*, std::uint64_t> new_offset;
    for (const Asset* a : order) {
        new_offset[a] = data_start + asset_data.size();
        if (a == &target) {
            append(asset_data, new_data);
            asset_data.resize(asset_data.size() + (new_slot - new_actual), 0);
        } else {
            append(asset_data, io::checked_range(file, a->data_pos(), a->slot_size));
        }
    }
    append(block, asset_data);
    const std::uint64_t new_total = align_up(data_start + asset_data.size(), kBlock);
    block.resize(new_total, 0);
    for (const Asset* a : order) {
        if (a->entry_pos < block_start) bad(a->entry_pos, "an asset entry lies before its layer's block");
        const std::uint64_t at = a->entry_pos - block_start;
        put_be32(block, at + 4, fits_int32(new_offset[a], "an asset offset"));
        if (a == &target) {
            put_be32(block, at, fits_int32(new_slot, "the new slot"));
            put_be32(block, at + 8, new_actual32);
        }
    }
    buf.resize(align_up(buf.size(), kBlock), 0);
    const std::uint64_t new_block_pos = buf.size();
    append(buf, block);
    put_be32(buf, owner.entry_pos + 28, fits_int32(new_block_pos / kBlock, "the layer's new block number"));
    put_be32(buf, owner.entry_pos + 32, fits_int32(new_total, "the layer's size"));
    put_be32(buf, owner.entry_pos + 36, fits_int32(new_total, "the layer's size"));
    for (const Section& s : owner.sections) {
        if (s.kind == 1) put_be32(buf, s.pos + 8, fits_int32(asset_data.size(), "the layer's data size"));
    }
    return buf;
}

// ---- textures --------------------------------------------------------------------------------------------------

TextureDescriptor texture_descriptor(ByteView descriptor) {
    io::ByteReader r(descriptor, "heavyiron texture descriptor");
    TextureDescriptor d;
    d.blob_id = r.u64_be();
    r.seek(0x33);
    d.format_byte = r.u8();
    const std::uint32_t size = r.u32_be();
    d.width = (size & 0x1FFF) + 1;
    d.height = ((size >> 13) & 0x1FFF) + 1;
    return d;
}

std::optional<texture::Format> texture_format(std::uint8_t format_byte) {
    if (format_byte == 0x52) return texture::Format::Dxt1;
    if (format_byte == 0x54) return texture::Format::Dxt5;
    return std::nullopt;
}

Result<Texture> decode_texture(ByteView file, const Archive& archive, std::uint64_t descriptor_id, std::optional<std::size_t> layer, bool force_opaque) {
    try {
        const Asset* desc_asset = archive.find(descriptor_id, layer);
        if (!desc_asset) return UsageError(std::format("texture descriptor {} not found", hex_id(descriptor_id)));
        const TextureDescriptor d = texture_descriptor(asset_bytes(file, *desc_asset));
        const Asset* blob = archive.find(d.blob_id, desc_asset->layer);   // the descriptor's own layer first
        if (!blob) {
            const std::vector<const Asset*> anywhere = archive.find_all(d.blob_id);
            if (!anywhere.empty()) blob = anywhere.front();
        }
        if (!blob) return UsageError(std::format("pixel blob {} not found", hex_id(d.blob_id)));
        const std::optional<texture::Format> format = texture_format(d.format_byte);
        if (!format) return UnsupportedError(kReader, desc_asset->data_pos() + 0x33, std::format("texture {}: format 0x{:02X} is not decoded", hex_id(descriptor_id), d.format_byte));
        texture::DecodeOptions options;
        options.tiled = true;
        options.swapped = true;
        options.force_opaque = force_opaque;
        return Texture{texture::decode_dxt(asset_bytes(file, *blob), d.width, d.height, *format, options), d.format_byte};
    } catch (const Error& e) {   // one texture failing is that texture's answer; the callers skip it
        return Failure(e.clone());
    }
}

// ---- as a media volume -----------------------------------------------------------------------------------------

bool is_archive(const std::filesystem::path& path) {
    if (text::lower(fs::to_text(path.extension())) != ".ho") return false;
    try {
        return Archive::try_parse(fs::read_file(path)).ok();
    } catch (const IoError&) {   // unreadable is "not an archive" to a probe
        return false;
    }
}

std::vector<VolumeEntry> volume_entries(const Archive& archive) {
    std::vector<VolumeEntry> out;
    std::map<std::string, std::size_t> position;   // the same name twice: the later asset, in the earlier place
    for (const Asset* a : archive.assets()) {
        VolumeEntry e{std::format("layer{:02}/{}_{:08X}.bin", a->layer, hex_id(a->id), a->type), a->actual_size, a};
        const auto [it, fresh] = position.emplace(e.name, out.size());
        if (fresh) out.push_back(std::move(e));
        else out[it->second] = std::move(e);
    }
    return out;
}

// ---- PNG -------------------------------------------------------------------------------------------------------

namespace {

void png_chunk(Bytes& out, std::string_view type, ByteView body) {
    io::ByteWriter w;
    w.u32_be(static_cast<std::uint32_t>(body.size()));
    Bytes tagged = to_bytes(type);
    append(tagged, body);
    w.bytes(tagged);
    w.u32_be(cb::crypto::crc32(tagged));
    append(out, w.data());
}

}  // namespace

Bytes png_bytes(const texture::Image& image) {
    const std::size_t row = std::size_t{image.width} * 4;
    Bytes raw;
    raw.reserve((row + 1) * image.height);
    for (std::uint32_t y = 0; y < image.height; ++y) {
        raw.push_back(0);   // filter: none
        const auto first = image.rgba.begin() + static_cast<std::ptrdiff_t>(y * row);
        raw.insert(raw.end(), first, first + static_cast<std::ptrdiff_t>(row));
    }
    z_stream zs;
    std::memset(&zs, 0, sizeof zs);
    if (deflateInit(&zs, 6) != Z_OK) throw InternalError("zlib: deflateInit failed");
    Bytes z(deflateBound(&zs, static_cast<uLong>(raw.size())));
    zs.next_in = raw.data();
    zs.avail_in = static_cast<uInt>(raw.size());
    zs.next_out = z.data();
    zs.avail_out = static_cast<uInt>(z.size());
    const int rc = deflate(&zs, Z_FINISH);
    z.resize(z.size() - zs.avail_out);
    deflateEnd(&zs);
    if (rc != Z_STREAM_END) throw InternalError("zlib: deflate did not finish");
    Bytes out = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n'};
    io::ByteWriter header;
    header.u32_be(image.width);
    header.u32_be(image.height);
    header.u8(8);   // 8 bits a channel
    header.u8(6);   // RGBA
    header.u8(0);   // deflate
    header.u8(0);   // filter method 0
    header.u8(0);   // not interlaced
    png_chunk(out, "IHDR", header.data());
    png_chunk(out, "IDAT", z);
    png_chunk(out, "IEND", {});
    return out;
}

// ---- the command -----------------------------------------------------------------------------------------------

namespace {

std::string quote_tag(const std::array<char, 4>& tag) {
    std::string s = "'";
    for (char c : tag) s += (c >= 0x20 && c < 0x7F) ? c : '.';
    return s + "'";
}

// "[1, 2]"
std::string list_text(const std::vector<std::size_t>& v) {
    std::string s = "[";
    for (std::size_t i = 0; i < v.size(); ++i) s += (i ? ", " : "") + std::to_string(v[i]);
    return s + "]";
}

// The asset id as the command line gives it: hexadecimal, the whole unsigned 64-bit range.
std::uint64_t parse_id(const std::string& s) {
    const std::optional<std::uint64_t> v = text::parse_int<std::uint64_t>(s, 16);
    if (!v) throw UsageError(std::format("not an asset id: '{}'", s));
    return *v;
}

}  // namespace

int command(const std::vector<std::string>& args) {
    // options may sit anywhere; the rest are the words in order
    std::vector<std::string> words;
    std::optional<std::string> type_option, layer_option;
    for (std::size_t i = 0; i < args.size(); ++i) {
        std::optional<std::string>* slot = args[i] == "--type" ? &type_option : args[i] == "--layer" ? &layer_option : nullptr;
        if (!slot) {
            words.push_back(args[i]);
            continue;
        }
        if (i + 1 >= args.size()) throw UsageError(args[i] + " needs a value");
        *slot = args[++i];
    }
    if (words.size() < 2) {
        out_text(kUsage);
        return 2;
    }
    const std::string& cmd = words[0];
    const std::filesystem::path path = fs::from_text(words[1]);
    const Bytes data = fs::read_file(path);
    const Archive archive = Archive::parse(data);
    std::vector<const Asset*> list = archive.assets();
    if (type_option) {
        const std::optional<std::int64_t> kind = text::parse_int<std::int64_t>(*type_option, 0);
        if (!kind) throw UsageError("--type wants a number: " + *type_option);
        std::erase_if(list, [&](const Asset* a) { return a->type != *kind; });
    }
    std::optional<std::size_t> layer;
    if (layer_option) {
        layer = text::parse_int<std::size_t>(*layer_option, 10);
        if (!layer) throw UsageError("--layer wants a number: " + *layer_option);
    }

    if (cmd == "info") {
        out("{}: {} bytes, {} layers, {} assets\n", fs::to_text(path.filename()), archive.file_size(), archive.layers().size(), list.size());
        struct Tally { std::uint32_t type; std::size_t count; std::uint64_t bytes; };
        std::vector<Tally> tally;   // by first appearance, so equal totals keep that order
        for (const Asset* a : list) {
            auto it = std::find_if(tally.begin(), tally.end(), [&](const Tally& t) { return t.type == a->type; });
            if (it == tally.end()) it = tally.insert(tally.end(), Tally{a->type, 0, 0});
            it->count += 1;
            it->bytes += a->actual_size;
        }
        std::stable_sort(tally.begin(), tally.end(), [](const Tally& x, const Tally& y) { return x.bytes > y.bytes; });
        for (const Tally& t : tally) out("  ${:08X} {:<18} {:>6} assets {:>12} bytes\n", t.type, type_name(t.type), t.count, t.bytes);
    } else if (cmd == "list") {
        for (const Asset* a : list) {
            out("{}  ${:08X} {:<18} layer {:<3} {:>9} bytes (slot {}) at ${:X}\n", hex_id(a->id), a->type, type_name(a->type), a->layer, a->actual_size, a->slot_size, a->data_pos());
        }
    } else if (cmd == "layers") {
        for (std::size_t i = 0; i < archive.layers().size(); ++i) {
            const Layer& l = archive.layers()[i];
            const bool psl = l.kind == Layer::Kind::Psl;
            out("[{:>2}] {:<8} block at ${:<7X} {:>9} bytes  {:<4} {} assets\n", i, quote_tag(l.type), l.block_pos(), l.total_size, psl ? "PSL" : "PSLD", psl ? l.assets.size() : 0);
        }
    } else if (cmd == "dups") {
        std::map<std::uint64_t, std::vector<std::size_t>> layers_of;   // ordered by id: the report is sorted
        for (const Asset* a : archive.assets()) {
            std::vector<std::size_t>& v = layers_of[a->id];
            if (std::find(v.begin(), v.end(), a->layer) == v.end()) v.push_back(a->layer);
        }
        std::erase_if(layers_of, [](const auto& kv) { return kv.second.size() < 2; });
        out("{} asset ids are in more than one layer (name the layer when replacing one):\n", layers_of.size());
        for (auto& [id, v] : layers_of) {
            std::sort(v.begin(), v.end());
            out("  {}  layers {}\n", hex_id(id), list_text(v));
        }
    } else if (cmd == "extract" && words.size() > 2) {
        const std::filesystem::path folder = fs::from_text(words[2]);
        fs::make_dirs(folder);
        for (const Asset* a : list) fs::write_file(folder / std::format("{}_{:08X}.bin", hex_id(a->id), a->type), asset_bytes(data, *a));
        out("{} assets -> {}\n", list.size(), words[2]);
    } else if (cmd == "replace" && words.size() > 4) {
        const Bytes replacement = fs::read_file(fs::from_text(words[3]));
        const Bytes result = replace(data, archive, parse_id(words[2]), replacement, layer);
        fs::write_file(fs::from_text(words[4]), result);
        out("replaced {} ({} bytes): {} -> {} bytes, {}\n", words[2], replacement.size(), data.size(), result.size(), words[4]);
    } else if (cmd == "textures" && words.size() > 2) {
        const std::filesystem::path folder = fs::from_text(words[2]);
        fs::make_dirs(folder);
        std::vector<std::uint64_t> done;
        std::size_t ok = 0, skipped = 0;
        for (const Asset* d : archive.assets()) {
            if (d->type != kTextureType || std::find(done.begin(), done.end(), d->id) != done.end()) continue;
            done.push_back(d->id);
            const Result<Texture> t = decode_texture(data, archive, d->id, d->layer);
            if (!t.ok()) {
                ++skipped;
                continue;
            }
            const texture::Image& image = t.value().image;
            fs::write_file(folder / std::format("{}_{}x{}.png", hex_id(d->id), image.width, image.height), png_bytes(image));
            ++ok;
        }
        out("{} textures decoded, {} skipped -> {}\n", ok, skipped, words[2]);
    } else {
        out_text(kUsage);
        return 2;
    }
    return 0;
}

}  // namespace cb::formats::xbox::heavyiron
