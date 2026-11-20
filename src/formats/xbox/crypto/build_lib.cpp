#include "formats/xbox/crypto/build_lib.hpp"

#include <algorithm>
#include <chrono>
#include <format>

#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

namespace cb::formats::xbox::crypto::build_lib {

namespace {

namespace stdfs = std::filesystem;

constexpr const char* kReader = "build_lib";
constexpr std::uint32_t kPatchEnd = 0xFFFFFFFF;
constexpr std::uint32_t kJumpAnchor = 0x4C000024;   // the word the 4BL's loader branches through
constexpr std::size_t kSignatureAt = 0x20, kSignatureSize = 0x100, kHashedFrom = 0x120;
constexpr std::size_t kSeHeaderSize = 0x30, kSeSizeAt = 0xC, kSeFlagsAt = 0x28;
constexpr std::uint32_t kSeFlags = 0x280000;
constexpr std::chrono::minutes kToolTimeout{10};

std::size_t at_most(std::size_t value, std::size_t limit) { return std::min(value, limit); }

void put_u32_be(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

// What a hash covers in a signed bootloader (an SD, offset 0): the header (0x10) and the body after the signature block.
Bytes signed_part(ByteView image, std::size_t offset, std::size_t body_size) {
    const ByteView head = io::checked_range(image, offset, 0x10);
    const ByteView body = io::checked_range(image, offset + kHashedFrom, body_size);
    Bytes data(head.begin(), head.end());
    data.insert(data.end(), body.begin(), body.end());
    return data;
}

}  // namespace

// ---- the layouts ---------------------------------------------------------------------------------------------------------

FlashHeader FlashHeader::parse(ByteView data) {
    io::ByteReader r(data, kReader);
    FlashHeader h;
    h.magic = r.u16_be();
    h.build = r.u16_be();
    h.qfe = r.u16_be();
    h.flags = r.u16_be();
    h.entry = r.u32_be();
    h.size = r.u32_be();
    const ByteView copyright = r.bytes(h.copyright.size());
    std::copy(copyright.begin(), copyright.end(), reinterpret_cast<std::uint8_t*>(h.copyright.data()));
    const ByteView padding = r.bytes(h.padding.size());
    std::copy(padding.begin(), padding.end(), h.padding.begin());
    h.kv_length = r.u32_be();
    h.sys_upd_addr = r.u32_be();
    h.patch_slots = r.u16_be();
    h.kv_version = r.u16_be();
    h.kv_offset = r.u32_be();
    h.patch_slot_size = r.u32_be();
    h.smc_config_offset = r.u32_be();
    h.smc_length = r.u32_be();
    h.smc_offset = r.u32_be();
    return h;
}

Bytes FlashHeader::to_bytes() const {
    io::ByteWriter w;
    w.u16_be(magic);
    w.u16_be(build);
    w.u16_be(qfe);
    w.u16_be(flags);
    w.u32_be(entry);
    w.u32_be(size);
    w.bytes(ByteView(reinterpret_cast<const std::uint8_t*>(copyright.data()), copyright.size()));
    w.bytes(padding);
    w.u32_be(kv_length);
    w.u32_be(sys_upd_addr);
    w.u16_be(patch_slots);
    w.u16_be(kv_version);
    w.u32_be(kv_offset);
    w.u32_be(patch_slot_size);
    w.u32_be(smc_config_offset);
    w.u32_be(smc_length);
    w.u32_be(smc_offset);
    return w.take();
}

BootloaderHeader BootloaderHeader::parse(ByteView data, bool with_nonce) {
    io::ByteReader r(data, kReader);
    BootloaderHeader h;
    const ByteView magic = r.bytes(2);
    std::copy(magic.begin(), magic.end(), h.magic.begin());
    h.build = r.u16_be();
    h.qfe = r.u16_be();
    h.flags = r.u16_be();
    h.entry = r.u32_be();
    h.size = r.u32_be();
    if (with_nonce) {
        std::array<std::uint8_t, 0x10> nonce{};
        const ByteView bytes = r.bytes(nonce.size());
        std::copy(bytes.begin(), bytes.end(), nonce.begin());
        h.nonce = nonce;
    }
    return h;
}

Bytes BootloaderHeader::to_bytes() const {
    io::ByteWriter w;
    w.bytes(magic);
    w.u16_be(build);
    w.u16_be(qfe);
    w.u16_be(flags);
    w.u32_be(entry);
    w.u32_be(size);
    if (nonce) w.bytes(*nonce);
    return w.take();
}

// ---- patch tables -----------------------------------------------------------------------------------------------------------

std::vector<Patch> patch_records(ByteView table) {
    io::ByteReader r(table, "patch table");
    std::vector<Patch> records;
    for (;;) {
        const std::uint32_t address = r.u32_be();
        if (address == kPatchEnd) break;
        const std::uint32_t words = r.u32_be();
        if (words > r.remaining() / 4) throw TruncatedError("patch table", r.pos(), std::uint64_t{words} * 4, r.remaining());
        records.push_back({address, r.bytes(std::size_t{words} * 4)});
    }
    return records;
}

Bytes apply_patches(ByteView image, ByteView table) {
    Bytes out(image.begin(), image.end());
    patch_in_place(out, table);
    return out;
}

std::size_t patch_in_place(Bytes& image, ByteView table) {
    std::size_t changed = 0;
    for (const Patch& p : patch_records(table)) {
        const std::size_t end = std::size_t{p.address} + p.words.size();
        const bool same = end <= image.size() && std::equal(p.words.begin(), p.words.end(), image.begin() + p.address);
        if (same) continue;
        if (end > image.size()) image.resize(end, 0);   // an address past the end grows the image
        std::copy(p.words.begin(), p.words.end(), image.begin() + p.address);
        ++changed;
    }
    return changed;
}

std::size_t calc_patch_size(ByteView table, std::size_t size) {
    std::size_t total = 0;
    for (const Patch& p : patch_records(table))
        if (p.address >= size) total += p.words.size();
    return total;
}

Bytes apply_jump_sd_4bl(ByteView image, std::size_t size) {
    for (std::size_t at = 0; at + 4 <= image.size(); at += 4) {
        const std::uint32_t word = (std::uint32_t{image[at]} << 24) | (std::uint32_t{image[at + 1]} << 16) | (std::uint32_t{image[at + 2]} << 8) | image[at + 3];
        if (word != kJumpAnchor) continue;
        // the branch's distance, with the addresses taken without their high bit (the loader runs them uncached)
        const std::int64_t distance = static_cast<std::int64_t>(size & ~std::size_t{0x80000000}) - static_cast<std::int64_t>(at & ~std::size_t{0x80000000});
        const std::uint32_t branch = 0x48000000 | (static_cast<std::uint32_t>(distance) & 0x3FFFFFC);
        Bytes out(image.begin(), image.end());
        put_u32_be(out, at, branch);
        return out;
    }
    throw FormatError(kReader, image.size(), "no 0x4C000024 in the image: nothing to branch through");
}

// ---- bootloaders in an image -----------------------------------------------------------------------------------------------------

std::size_t bootloader_size(ByteView image, std::size_t offset) {
    const ByteView head = io::checked_range(image, offset, 0x10);
    const std::string_view magic(reinterpret_cast<const char*>(head.data()), 2);
    if (magic != "SB" && magic != "SC" && magic != "SD" && magic != "SE") throw FormatError(kReader, offset, "invalid bootloader magic");
    const std::uint32_t size = (std::uint32_t{head[0xC]} << 24) | (std::uint32_t{head[0xD]} << 16) | (std::uint32_t{head[0xE]} << 8) | head[0xF];
    if (size < 0x20) throw FormatError(kReader, offset + 0xC, "a bootloader smaller than its header");
    const std::size_t body = size - 0x20;
    return body + calc_bldr_pad_size(body);
}

void encrypt_bootloader_in_place(ByteView key, Bytes& image, std::size_t offset) {
    const std::size_t size = bootloader_size(image, offset);
    const ByteView body = io::checked_range(image, offset + 0x20, size);
    const Bytes encrypted = xecrypt::rc4(key, body);
    std::copy(encrypted.begin(), encrypted.end(), image.begin() + static_cast<std::ptrdiff_t>(offset + 0x20));
}

Bytes calc_se_hash(ByteView image, std::size_t offset) {
    const std::size_t size = bootloader_size(image, offset);
    const ByteView head = io::checked_range(image, offset, 0x10);
    const ByteView body = io::checked_range(image, offset + 0x20, size);
    Bytes data(head.begin(), head.end());
    data.insert(data.end(), body.begin(), body.end());
    return xecrypt::rot_sum_sha(data);
}

void sign_bootloader_in_place(Bytes& image, std::size_t offset, const xecrypt::RsaKey& key) {
    const std::size_t size = bootloader_size(image, offset);
    if (size < kSignatureSize) throw FormatError(kReader, offset, "a bootloader too small to hold its signature");
    const Bytes hash = xecrypt::rot_sum_sha(signed_part(image, offset, size - kSignatureSize));
    const Bytes signature = key.sign(hash, as_bytes(kSdSalt));
    std::copy(signature.begin(), signature.end(), image.begin() + static_cast<std::ptrdiff_t>(offset + kSignatureAt));
}

// ---- the 4BL --------------------------------------------------------------------------------------------------------------------------

Bytes sign_sd_4bl(const xecrypt::RsaKey& key, ByteView salt, ByteView image) {
    if (image.size() < kHashedFrom) throw FormatError(kReader, image.size(), "an SD shorter than 0x120 bytes");
    Bytes data(image.begin(), image.end());
    const Bytes hash = xecrypt::rot_sum_sha(signed_part(image, 0, image.size() - kHashedFrom));
    const Bytes signature = key.sign(hash, salt);
    std::copy(signature.begin(), signature.end(), data.begin() + static_cast<std::ptrdiff_t>(kSignatureAt));
    return data;
}

bool verify_sd_4bl(const xecrypt::RsaKey& key, ByteView salt, ByteView image) {
    if (image.size() < kHashedFrom) throw FormatError(kReader, image.size(), "an SD shorter than 0x120 bytes");
    const ByteView signature = image.subspan(kSignatureAt, kSignatureSize);
    const Bytes hash = xecrypt::rot_sum_sha(signed_part(image, 0, image.size() - kHashedFrom));
    return key.verify(signature, hash, salt);
}

Bytes encrypt_bl(ByteView key, ByteView data, bool skip_header) {
    Bytes out(data.begin(), data.end());
    const std::size_t from = skip_header ? at_most(0x20, data.size()) : 0;
    const Bytes encrypted = xecrypt::rc4(key, data.subspan(from));
    std::copy(encrypted.begin(), encrypted.end(), out.begin() + static_cast<std::ptrdiff_t>(from));
    return out;
}

// ---- the SE's LZX ----------------------------------------------------------------------------------------------------------------------

Bytes compress_se(LzxCodec& codec, ByteView data, bool include_header) {
    Bytes packed = codec.compress_continuous(data);
    if (!include_header) return packed;
    Bytes out(kSeHeaderSize, 0);
    out.insert(out.end(), packed.begin(), packed.end());
    put_u32_be(out, kSeSizeAt, static_cast<std::uint32_t>(out.size()));
    put_u32_be(out, kSeFlagsAt, kSeFlags);
    return out;
}

Bytes decompress_se(LzxCodec& codec, ByteView data, bool skip_header) {
    if (skip_header) {
        if (data.size() < kSeHeaderSize) throw TruncatedError(kReader, 0, kSeHeaderSize, data.size());
        data = data.subspan(kSeHeaderSize);
    }
    return codec.decompress_continuous(data);
}

// ---- sources and tools -------------------------------------------------------------------------------------------------------------------

std::optional<Bytes> first_source(std::span<const Source> sources) {
    for (const Source& source : sources) {
        if (const auto* path = std::get_if<stdfs::path>(&source)) {
            std::error_code ec;
            if (path->empty() || !stdfs::is_regular_file(*path, ec)) continue;
            return fs::read_file(*path);
        }
        const Bytes& bytes = std::get<Bytes>(source);
        if (!bytes.empty()) return bytes;
    }
    return std::nullopt;
}

std::vector<std::string> target_defines(PatchTarget target) {
    switch (target) {
        case PatchTarget::Plain: return {};
        case PatchTarget::Devkit: return {"DEVKIT"};
        case PatchTarget::Retail: return {"RETAIL"};
        case PatchTarget::Flash: return {"FLASH"};
        case PatchTarget::VfusesFlash: return {"VFUSES", "FLASH"};
        case PatchTarget::Hdd: return {"HDD"};
        case PatchTarget::VfusesHdd: return {"VFUSES", "HDD"};
    }
    return {};
}

void assemble_patch(const PatchTools& tools, const stdfs::path& asm_file, const stdfs::path& bin_file, PatchTarget target,
                    std::span<const std::string> defines) {
    fs::ScopedTempDir work("xenon_as");
    const stdfs::path elf = work.path() / "temp.elf";
    process::ProcessSpec assemble;
    assemble.argv = {fs::to_text(tools.bin_dir / "xenon-as.exe"), "-be", "-many", "-mregnames", fs::to_text(asm_file), "-o", fs::to_text(elf),
                     "-I", fs::to_text(stdfs::absolute(asm_file).parent_path()), "-I", fs::to_text(stdfs::absolute(tools.include_dir))};
    for (const std::string& d : target_defines(target)) {
        assemble.argv.push_back("--defsym");
        assemble.argv.push_back(d + "=1");
    }
    for (const std::string& d : defines) {
        assemble.argv.push_back("--defsym");
        assemble.argv.push_back(d + "=1");
    }
    assemble.out = process::Capture::Discard;
    assemble.timeout = kToolTimeout;
    process::ProcessResult ran = process::run(assemble);
    if (!ran.ok()) throw ToolError("xenon-as", ran.exit_code, text::from_latin1(ran.err), "patch assembly failed");

    process::ProcessSpec convert;
    convert.argv = {fs::to_text(tools.bin_dir / "xenon-objcopy.exe"), fs::to_text(elf), "-O", "binary", fs::to_text(bin_file)};
    convert.out = process::Capture::Discard;
    convert.timeout = kToolTimeout;
    ran = process::run(convert);
    if (!ran.ok()) throw ToolError("xenon-objcopy", ran.exit_code, text::from_latin1(ran.err), "ELF conversion failed");
}

CommandResult run_command(const stdfs::path& program, std::span<const std::string> arguments) {
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(stdfs::absolute(program))};
    spec.argv.insert(spec.argv.end(), arguments.begin(), arguments.end());
    spec.timeout = kToolTimeout;
    const process::ProcessResult ran = process::run(spec);
    CommandResult out;
    out.exit_code = ran.exit_code;
    out.output = text::sanitize_utf8(std::string(as_text(ran.out)));
    return out;
}

}  // namespace cb::formats::xbox::crypto::build_lib
