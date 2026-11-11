// cb/formats/xbox/stfs.hpp -- STFS: Xbox 360 secure content packages (CON / LIVE / PIRS): read, pack, resign.
//
// A clean-room implementation of the STFS container format, written to the public/documented format algorithm (Free60
// and community references). STFS stores a small filesystem in fixed 0x1000-byte blocks with hash blocks interleaved
// among the data: one level-0 hash block per 0xAA (170) data blocks, one level-1 per 0x70E4, optionally a level-2.
// The reader turns a logical data-block index into its real ("backing") position past those hash blocks; the hash
// entry for each block records the *next* block, forming a chain. Packages come in two hash layouts ("female"/"male", a
// header bit) which shift the table spacing, and each level uses one of two shadow tables selected by a status bit in
// the table above it.
//
// Resigning: a CON package carries a console certificate (from a keyvault) and an RSA signature of its header. The
// signature is standard PKCS#1 v1.5 RSA-SHA1 over header[0x22C:0x344] using the console's 1024-bit private key, stored
// byte-reversed at 0x1AC; the certificate (0x1A8 bytes) is copied verbatim from the keyvault. Both were
// reverse-engineered and confirmed byte-exact against genuine console-signed packages, so `resign` binds a package to
// a console you own using its KV_dec.bin. A LIVE/PIRS package (Microsoft-signed -- unforgeable) is converted to CON
// and console-signed the same way tools like Velocity do. See docs/STFS.md. (The RSA arithmetic is Windows CNG's.)
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::xbox::stfs {

inline constexpr std::size_t kBlock = 0x1000;

enum class Magic { Con, Live, Pirs };
std::string_view to_string(Magic m);                  // "CON ", "LIVE", "PIRS"
std::optional<Magic> magic_of(ByteView four);         // nullopt for anything else

// One file of the package.
struct FileEntry {
    std::string path;               // "media/font.xpr": folders joined with '/'
    std::uint32_t start_block = 0;
    std::uint32_t block_count = 0;
    std::uint32_t size = 0;
};

class Package {
public:
    // Parse a package, owning its bytes. FormatError if it is not one (bad magic, a header or table cut short, a chain
    // that leaves the package), TruncatedError where a read runs past the end.
    static Package parse(Bytes data);

    Magic magic() const { return magic_; }
    std::uint32_t header_size() const { return header_size_; }
    std::uint32_t first_hash_offset() const { return first_hash_; }   // the header size rounded up to a block: where block data begins

    // the volume descriptor
    std::uint8_t block_separation() const { return block_sep_; }
    std::uint16_t file_table_block_count() const { return ft_block_count_; }
    std::uint32_t file_table_block_number() const { return ft_block_number_; }
    std::uint32_t allocated_blocks() const { return alloc_blocks_; }
    std::uint32_t unallocated_blocks() const { return unalloc_blocks_; }

    bool male_layout() const { return male_; }    // otherwise female
    unsigned top_hash_level() const { return top_level_; }

    // the content metadata
    std::uint32_t content_type() const { return content_type_; }
    const std::string& title_id() const { return title_id_; }         // 8 hex digits
    const std::string& profile_id() const { return profile_id_; }     // 16 hex digits
    const std::string& display_name() const { return display_name_; }
    const std::string& title_name() const { return title_name_; }

    // The files, in the order of the file table. A path met twice is listed once (at its first place, with the later
    // entry's data).
    const std::vector<FileEntry>& files() const { return files_; }
    const FileEntry* find(std::string_view path) const;

    // The bytes of a file; NotFoundError-like FormatError for a name that is not in the package.
    Bytes read_file(const FileEntry& file) const;
    Bytes read_file(std::string_view path) const;

    // Write every file under `outdir` (folders created, '/' or '\\' in a path as separators); returns the count. A path
    // that would leave `outdir` is refused (FormatError).
    std::size_t extract_all(const std::filesystem::path& outdir) const;

    ByteView data() const { return data_; }

private:
    Package() = default;
    std::uint64_t backing_block(std::uint32_t block) const;
    std::uint64_t block_address(std::uint32_t block) const;
    std::uint64_t level0_backing(std::uint32_t block) const;
    std::uint64_t level1_backing(std::uint32_t block) const;
    void load_top_table();
    std::uint64_t hash_address(std::uint32_t block) const;
    std::uint32_t next_block(std::uint32_t block) const;
    Bytes read_chain(std::uint32_t start, std::uint32_t blocks, std::uint32_t size) const;
    void read_file_listing();

    Bytes data_;
    Magic magic_ = Magic::Con;
    std::uint32_t header_size_ = 0, first_hash_ = 0;
    std::uint8_t block_sep_ = 0;
    std::uint16_t ft_block_count_ = 0;
    std::uint32_t ft_block_number_ = 0, alloc_blocks_ = 0, unalloc_blocks_ = 0;
    bool male_ = false;
    unsigned shift_ = 0;                  // 1 for the male layout: hash blocks come in pairs
    std::uint32_t step_[2] = {0, 0};
    unsigned top_level_ = 0;
    std::uint32_t content_type_ = 0;
    std::string title_id_, profile_id_, display_name_, title_name_;
    std::vector<std::uint8_t> top_entries_;   // the top hash table's per-entry status bytes
    std::vector<FileEntry> files_;
    std::unordered_map<std::string, std::size_t> index_;   // path -> position in files_ (lookup only)
};

// ---- packing -----------------------------------------------------------------------------------------

struct SourceFile {
    std::string name;     // a flat name (the root folder of the package); Latin-1 characters only, cut at 40 bytes
    ByteView data;
};

// Pack files into a loadable STFS package, cloning the header skeleton (console cert + metadata layout) from an
// existing `template_package` and rebuilding the payload, hash tree, file table and volume descriptor. Written in the
// 'male' layout. The package is rehashed here but keeps the template's console signature: it loads on emulators /
// RGH-JTAG as-is; to bind it to a specific console (and satisfy a stock retail console), follow with `resign`.
// FormatError if the template is not a package; UsageError if there are no files.
Bytes build(std::span<const SourceFile> files, ByteView template_package);

// The files of a folder (non-recursive, by name) or the one file named, packed from the template package at
// `template_path`; written to `out_path`. Returns `out_path`.
std::filesystem::path build(const std::filesystem::path& folder, const std::filesystem::path& out_path,
                            const std::filesystem::path& template_path);

// ---- resigning ---------------------------------------------------------------------------------------

// A decrypted keyvault (KV_dec.bin, 0x4000 bytes) holds the console certificate and the console's 1024-bit RSA private
// key. Offsets are from the documented XECRYPT_KEYVAULT layout; the certificate copied into the STFS header is the
// 0x1A8-byte region at KV 0x9C8, and the private key is XECRYPT_RSAPRV_1024 at KV 0x298 (cqw, e, reserved, then
// n/p/q/dp/dq/cr as XeCrypt "BnQw" bignums).
inline constexpr std::size_t kKvCertOffset = 0x9C8;
inline constexpr std::size_t kKvCertLength = 0x1A8;
inline constexpr std::size_t kKvPrivateKeyOffset = 0x298;
inline constexpr std::size_t kKvSize = 0x4000;

struct ResignOptions {
    // Also stamp the keyvault's console id into the content metadata (0x36C): off by default so a same-console CON
    // resign stays byte-identical to the console's own output.
    bool bind_console = false;
};

// Re-sign a package with a console keyvault you own so it will load on that console: copies the console certificate
// from the keyvault and RSA-signs the header (PKCS#1 v1.5 over SHA1(header[0x22C:0x344])) with the console private key.
// Works on CON packages (saves, profiles, DLC) and CONVERTS Microsoft-signed LIVE / PIRS packages to console-signed CON
// -- their signature cannot be forged, so the only way to load a modified one is to re-sign it as CON. The metadata from
// 0x22C on is identical across the three, so conversion rewrites only the 0x04..0x22C signature block, the magic and the
// license descriptor. Returns the new package bytes. FormatError for a package or keyvault that is not one, or a
// keyvault whose key is not a valid RSA key or does not match its certificate (the signature must verify against it).
Bytes resign(ByteView package, ByteView keyvault, const ResignOptions& options = {});

// The file form: read `package` and `keyvault`, write the result to `out_path` (in place when not given).
void resign_file(const std::filesystem::path& package, const std::filesystem::path& keyvault,
                 const std::optional<std::filesystem::path>& out_path = std::nullopt, const ResignOptions& options = {});

// The console id the keyvault would stamp (5 bytes of the certificate), as 10 hex digits.
std::string console_id(ByteView keyvault);

}  // namespace cb::formats::xbox::stfs
