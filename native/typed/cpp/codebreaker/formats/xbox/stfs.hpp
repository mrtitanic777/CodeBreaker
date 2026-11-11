// codebreaker/formats/xbox/stfs.hpp -- C++ port of codebreaker.formats.xbox.stfs.
//
// STFS -- Xbox 360 secure content package reader (CON / LIVE / PIRS).
//
// A clean-room implementation of the STFS container format, written to the public/documented format
// algorithm (Free60 and community references). It gives CodeBreaker native read access to Xbox 360
// content packages as original code, so the capability lives in the tool without any GPL source.
//
// STFS stores a small filesystem in fixed 0x1000-byte blocks with hash blocks interleaved among the data:
// one level-0 hash block per 0xAA (170) data blocks, one level-1 per 0x70E4, optionally a level-2.
// `_cbdbn` turns a logical data-block index into its real ("backing") position past those hash blocks;
// the hash entry for each block records the *next* block, forming a chain. Packages come in two hash
// layouts ("female"/"male", a header bit) which shift the table spacing, and each level uses one of two
// shadow tables selected by a status bit in the table above it.
//
// Resigning: a CON package carries a console certificate (from a keyvault) and an RSA signature of its
// header. The signature is standard PKCS#1 v1.5 RSA-SHA1 over header[0x22C:0x344] using the console's
// 1024-bit private key, stored byte-reversed at 0x1AC; the certificate (0x1A8 bytes) is copied verbatim
// from the keyvault. Both were reverse-engineered and confirmed byte-exact against genuine
// console-signed packages, so `resign` binds a package to a console you own using its KV_dec.bin. A
// LIVE/PIRS package (Microsoft-signed -- unforgeable) is converted to CON and console-signed the same way
// tools like Velocity do. See docs/STFS.md.
//
// Commands:
//     stfs info    <package>            header + content metadata
//     stfs list    <package>            the contained file listing
//     stfs extract <package> [outdir]   extract every contained file
//     stfs resign  <package> <KV> [out] resign CON / convert LIVE|PIRS -> console CON
//
// Typing notes:
//   * `bytes | bytearray` parameters are py::bytes (a py::bytearray converts to it). STFS keeps a py::bytes
//     copy of the data (the Python keeps the caller's object). The buffers the writer changes in place
//     (`_header_hash`) are py::bytearray handles.
//   * `build`'s `template` and `resign`'s `package` / `kv` are "bytes or a path": the Python tests
//     isinstance; here `build` has two overloads (bytes, path) and `resign` takes std::variant<py::bytes,
//     py::str> (the one place the Python mixes types in a parameter).
//   * The RSA arithmetic is MemCrypto's BigInt (the port of MemCrypto is the codebreaker module that has
//     an arbitrary-precision integer; used here rather than writing a second one).
//   * `files` is the Python dict {path: (start block, blocks, size)}; struct.error is py::StructError.
//   * main() takes the argument list (sys.argv[1:]); sys.exit(code) is MemCrypto::SystemExit.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <variant>

#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"
#include "py.hpp"

namespace codebreaker::formats::xbox::stfs {

using BigInt = codebreaker::formats::xbox::crypto::MemCrypto::BigInt;
using SystemExit = codebreaker::formats::xbox::crypto::MemCrypto::SystemExit;

extern const py::str DESCRIPTION;   // "read/resign Xbox 360 STFS packages (CON/LIVE/PIRS)"

inline constexpr int64_t BLOCK = 0x1000;
// MAGICS = (b"CON ", b"LIVE", b"PIRS")
bool is_magic(const py::bytes& four);   // `x in MAGICS`

// a little-endian 24-bit integer at d[o]
int64_t _u24_le(const py::bytes& d, int64_t o);

class STFS {
public:
    py::bytes d;
    py::bytes magic;

    int64_t header_size = 0;
    int64_t first_hash = 0;

    // STFS volume descriptor
    int64_t block_sep = 0;
    int64_t ft_block_count = 0;
    int64_t ft_block_number = 0;
    int64_t alloc_blocks = 0;
    int64_t unalloc_blocks = 0;

    int64_t sex = 0;               // 0 = female, 1 = male
    py::list<int64_t> step;
    int64_t top_level = 0;

    // content metadata
    int64_t content_type = 0;
    py::str title_id;
    py::str profile_id;
    py::str display_name;
    py::str title_name;

    py::list<int64_t> top_entries;   // the top hash table's per-entry status bytes
    py::dict<py::str, std::tuple<int64_t, int64_t, int64_t>> files;   // {path: (start block, blocks, size)}

    explicit STFS(py::bytes data);   // by value: a caller that hands over a temporary gives the package without a copy

    // util
    py::str _wstr(int64_t off, int64_t maxlen = 0x80);

    // block address maths
    int64_t _cbdbn(int64_t b);   // ComputeBackingDataBlockNumber -- data block -> backing block
    int64_t _block_addr(int64_t b);
    int64_t _cl0(int64_t b);   // ComputeLevel0BackingHashBlockNumber
    int64_t _cl1(int64_t b);
    void _load_top_table();
    int64_t _hash_addr(int64_t b);
    int64_t _next_block(int64_t b);

    // file table
    py::bytes _read_chain(int64_t start, int64_t nblocks, int64_t size);
    py::dict<py::str, std::tuple<int64_t, int64_t, int64_t>> _read_file_listing();
    py::bytes read_file(const py::str& name);
    int64_t extract_all(const py::str& outdir);
};

// ---- writer (pack) ----------------------------------------------------------------------------------
// Block geometry for writing. Packages are written in the 'male' layout (block separation 0), which fixes
// the table spacing at [0xAC, 0x723A].
class _WriteGeom {
public:
    int64_t sex;
    py::list<int64_t> step;
    int64_t alloc_blocks;
    int64_t first_hash;
    int64_t top_level;

    _WriteGeom(int64_t alloc_blocks, int64_t first_hash);
    int64_t cbdbn(int64_t b);
    int64_t block_addr(int64_t b);
    int64_t l0_backing(int64_t b);
    int64_t l1_backing();
};

// Master hash @0x32C = SHA1(0x344 .. first hash table).
void _header_hash(py::bytearray buf, int64_t first_hash);

// Pack a FOLDER of files into a loadable STFS package, cloning the header skeleton (console cert +
// metadata layout) from an existing `template` package and rebuilding the payload, hash tree, file table
// and volume descriptor. Clean-room implementation of the documented STFS write flow.
//
// The package is rehashed here but keeps the template's console signature. It loads on emulators /
// RGH-JTAG as-is; to bind it to a specific console (and satisfy a stock retail console), follow with
// `resign` using that console's keyvault.
py::str build(const py::str& folder, const py::str& out_path, const py::bytes& template_bytes);
py::str build(const py::str& folder, const py::str& out_path, const py::str& template_path);

// ---- resign (CON) -----------------------------------------------------------------------------------
// A decrypted keyvault (KV_dec.bin, 0x4000 bytes) holds the console certificate and the console's 1024-bit
// RSA private key. Offsets are from the documented XECRYPT_KEYVAULT layout; the certificate copied into the
// STFS header is the 0x1A8-byte region at KV 0x9C8, and the private key is XECRYPT_RSAPRV_1024 at KV 0x298
// (cqw, e, reserved, then n/p/q/dp/dq/cr as XeCrypt "BnQw" bignums).
inline constexpr int64_t KV_CERT_OFF = 0x9C8;
inline constexpr int64_t KV_CERT_LEN = 0x1A8;
inline constexpr int64_t KV_PRV_OFF = 0x298;
// PKCS#1 v1.5 DigestInfo prefix for SHA-1
extern const py::bytes _SHA1_DER;

// Decode a XeCrypt big-number (array of big-endian qwords) to an int.
BigInt _bn(const py::bytes& b);

// PKCS#1 v1.5 RSA-SHA1 sign `region` with the KV's console private key. Returns the 0x80-byte signature in
// the byte-reversed form STFS stores, and the key's (e, n).
std::tuple<py::bytes, std::tuple<int64_t, BigInt>> _con_sign(const py::bytes& region, const py::bytes& kv);

// Verify a byte-reversed STFS signature against the certificate's public modulus (a self-check). The
// modulus is a XeCrypt BnQw bignum, like the key.
bool _con_verify(const py::bytes& region, const py::bytes& sig, const py::bytes& modulus_bnqw, int64_t e);

// Re-sign a package with a console keyvault you own so it will load on that console. Copies the console
// certificate from the KV and RSA-signs the header (PKCS#1 v1.5 over SHA1(header[0x22C:0x344])) with the
// console private key.
//
// Works on CON packages (saves, profiles, DLC) and CONVERTS Microsoft-signed LIVE / PIRS packages to
// console-signed CON -- a LIVE/PIRS signature cannot be forged (that needs Microsoft's key), so the only
// way to load a modified one is to re-sign it as CON, exactly as Velocity / Le Fluffie do. The metadata
// from 0x22C on is identical across CON/LIVE/PIRS, so conversion only rewrites the 0x04..0x22C signature
// block, the magic, and the license descriptor.
//
// The certificate copy and the CON signature scheme were verified byte-exact against genuine
// console-signed packages. `bind_console` additionally stamps the KV's console id into the content
// metadata (0x36C) -- off by default so a same-console CON resign stays byte-identical to the console's own
// output. Overwrites in place unless `out_path` is given.
py::bytes resign(const std::variant<py::bytes, py::str>& package, const std::variant<py::bytes, py::str>& kv,
                 const std::optional<py::str>& out_path = std::nullopt, bool bind_console = false);

// ---- CLI --------------------------------------------------------------------------------------------
// `argv` is sys.argv[1:]; sys.exit(n) is a SystemExit.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::stfs
