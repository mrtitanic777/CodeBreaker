#include "cb/formats/lzx.hpp"

#include <algorithm>
#include <cstdio>
#include <map>
#include <random>

#include "basefile.h"      // native/xex: the xex program's own decoder, an independent implementation (the oracle)
#include "crypto/aes.h"
#include "crypto/keys.h"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::lzx {
namespace {

// ---- a small LZX encoder, for the tests -------------------------------------------------------------------
// Every literal and every match symbol gets a 9-bit code (at most 512 symbols are in use), the pre-tree gives every
// symbol 5 bits; matches are at most 8 long (no length tree) and never cross a frame or a block. It is not a good
// compressor and it does not need to be: it produces the layouts the decoder has to read (the stream header, block
// headers, delta-coded lengths, verbatim/uncompressed blocks, repeated offsets, 32 KB frames padded to words).

constexpr std::size_t kFrame = 32768;

struct BitWriter {
    Bytes out;
    std::uint32_t acc = 0;
    unsigned n = 0;
    void put(std::uint32_t value, unsigned bits) {   // MSB first, into 16-bit little-endian words
        for (unsigned i = bits; i-- > 0;) {
            acc = (acc << 1) | ((value >> i) & 1);
            if (++n == 16) {
                out.push_back(static_cast<std::uint8_t>(acc & 0xFF));
                out.push_back(static_cast<std::uint8_t>(acc >> 8));
                acc = 0;
                n = 0;
            }
        }
    }
    void align16() {
        while (n != 0) put(0, 1);
    }
    void raw_u32(std::uint32_t v) {
        for (unsigned i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
    }
};

struct Slots {
    std::vector<std::uint32_t> base;
    std::vector<unsigned> extra;
    explicit Slots(unsigned window_bits) {
        unsigned j = 0;
        std::vector<unsigned> e;
        for (unsigned i = 0; i < 51; i += 2) {
            e.push_back(j);
            if (i + 1 < 51) e.push_back(j);
            if (i != 0 && j < 17) ++j;
        }
        std::uint32_t b = 0;
        for (unsigned i = 0; i < 51 && b < (1u << window_bits); ++i) {
            base.push_back(b);
            extra.push_back(e[i]);
            b += 1u << e[i];
        }
    }
    unsigned slot_of(std::uint32_t offset) const {   // the slot whose range holds `offset` (slot >= 3)
        for (unsigned s = 3; s < base.size(); ++s) {
            if (offset >= base[s] - 2 && offset < base[s] - 2 + (1u << extra[s])) return s;
        }
        throw std::logic_error("offset beyond the window");
    }
};

struct Token {
    bool match = false;
    std::uint8_t literal = 0;
    std::uint32_t offset = 0;
    unsigned length = 0;
};

enum class BlockKind { Verbatim, Uncompressed };

struct EncodeOptions {
    unsigned window_bits = 15;
    std::size_t block_size = 0;                  // 0: one block
    std::vector<BlockKind> kinds = {BlockKind::Verbatim};   // cycled through block by block
    std::uint32_t e8_filesize = 0;               // sets the stream header's E8 flag (the data is not translated)
};

Bytes encode(const Bytes& plain, const EncodeOptions& opt = {}) {
    const Slots slots(opt.window_bits);
    const std::size_t main_elements = 256 + slots.base.size() * 8;
    BitWriter w;
    if (opt.e8_filesize == 0) {
        w.put(0, 1);
    } else {
        w.put(1, 1);
        w.put(opt.e8_filesize >> 16, 16);
        w.put(opt.e8_filesize & 0xFFFF, 16);
    }
    std::vector<unsigned> prev_main(main_elements, 0), prev_len(250, 0);
    std::uint32_t r0 = 1, r1 = 1, r2 = 1;
    std::map<std::uint32_t, std::vector<std::size_t>> seen;   // 3 bytes -> positions, for the parse

    // the pre-tree: all 20 symbols 5 bits long, so a symbol's code is its value
    auto write_lengths = [&](const std::vector<unsigned>& prev, const std::vector<unsigned>& now, std::size_t first,
                             std::size_t last) {
        for (int i = 0; i < 20; ++i) w.put(5, 4);
        for (std::size_t i = first; i < last; ++i) w.put((prev[i] + 17 - now[i]) % 17, 5);
    };

    std::size_t block_index = 0;
    const std::size_t block_size = opt.block_size ? opt.block_size : std::max<std::size_t>(plain.size(), 1);
    for (std::size_t start = 0; start < plain.size(); start += block_size, ++block_index) {
        const std::size_t end = std::min(plain.size(), start + block_size);
        const BlockKind kind = opt.kinds[block_index % opt.kinds.size()];
        if (kind == BlockKind::Uncompressed) {
            w.put(3, 3);
            w.put(static_cast<std::uint32_t>(end - start), 24);
            w.align16();
            w.raw_u32(r0);
            w.raw_u32(r1);
            w.raw_u32(r2);
            // frames: an uncompressed block's frame ends need no padding beyond the byte parity below
            w.out.insert(w.out.end(), plain.begin() + static_cast<std::ptrdiff_t>(start), plain.begin() + static_cast<std::ptrdiff_t>(end));
            if ((end - start) & 1) w.out.push_back(0);
            continue;
        }
        // parse the block into tokens
        std::vector<Token> tokens;
        std::uint32_t t0 = r0, t1 = r1, t2 = r2;
        for (std::size_t pos = start; pos < end;) {
            const std::size_t limit = std::min({end, (pos / kFrame + 1) * kFrame, pos + 8});
            unsigned best_len = 0;
            std::uint32_t best_offset = 0;
            if (pos + 3 <= plain.size()) {
                const std::uint32_t key = plain[pos] | (plain[pos + 1] << 8) | (plain[pos + 2] << 16);
                const auto it = seen.find(key);
                if (it != seen.end()) {
                    int tried = 0;
                    for (auto p = it->second.rbegin(); p != it->second.rend() && tried < 6; ++p, ++tried) {
                        const std::size_t offset = pos - *p;
                        if (offset >= (1u << opt.window_bits) - 3) break;
                        unsigned len = 0;
                        while (pos + len < limit && plain[*p + len] == plain[pos + len]) ++len;
                        if (len > best_len) {
                            best_len = len;
                            best_offset = static_cast<std::uint32_t>(offset);
                        }
                    }
                }
            }
            // prefer a repeated offset when it reaches as far
            for (std::uint32_t r : {t0, t1, t2}) {
                if (r != 0 && r <= pos) {
                    unsigned len = 0;
                    while (pos + len < limit && plain[pos - r + len] == plain[pos + len]) ++len;
                    if (len >= 2 && len >= best_len) {
                        best_len = len;
                        best_offset = r;
                        break;
                    }
                }
            }
            const std::size_t advance = best_len >= 2 ? best_len : 1;
            if (best_len >= 2) {
                tokens.push_back({true, 0, best_offset, best_len});
                if (best_offset == t1) std::swap(t0, t1);
                else if (best_offset == t2) std::swap(t0, t2);
                else if (best_offset != t0) { t2 = t1; t1 = t0; t0 = best_offset; }
            } else {
                tokens.push_back({false, plain[pos], 0, 0});
            }
            for (std::size_t k = 0; k < advance; ++k) {
                if (pos + k + 3 <= plain.size()) {
                    seen[plain[pos + k] | (plain[pos + k + 1] << 8) | (plain[pos + k + 2] << 16)].push_back(pos + k);
                }
            }
            pos += advance;
        }
        // which main symbols the block uses; the repeat state evolves exactly as the decoder's will
        auto symbol_of = [&](const Token& t, std::uint32_t& a, std::uint32_t& b, std::uint32_t& c) -> unsigned {
            if (!t.match) return t.literal;
            unsigned slot;
            if (t.offset == a) slot = 0;
            else if (t.offset == b) { slot = 1; std::swap(a, b); }
            else if (t.offset == c) { slot = 2; std::swap(a, c); }
            else { slot = slots.slot_of(t.offset); c = b; b = a; a = t.offset; }
            return static_cast<unsigned>(256 + slot * 8 + (t.length - 2));
        };
        std::vector<unsigned> main_len(main_elements, 0);
        {
            std::uint32_t a = r0, b = r1, c = r2;
            for (const Token& t : tokens) main_len[symbol_of(t, a, b, c)] = 9;
            for (unsigned i = 0; i < 256; ++i) main_len[i] = 9;
        }
        std::vector<std::uint32_t> code(main_elements, 0);
        std::uint32_t next = 0;
        for (std::size_t s = 0; s < main_elements; ++s) {
            if (main_len[s] == 9) code[s] = next++;
        }
        CB_ASSERT(next <= 512, "the test encoder's code is over-subscribed");

        w.put(1, 3);
        w.put(static_cast<std::uint32_t>(end - start), 24);
        write_lengths(prev_main, main_len, 0, 256);
        write_lengths(prev_main, main_len, 256, main_elements);
        write_lengths(prev_len, std::vector<unsigned>(250, 0), 0, 249);   // no length tree
        prev_main = main_len;
        std::size_t pos = start;
        for (const Token& t : tokens) {
            const unsigned sym = symbol_of(t, r0, r1, r2);
            w.put(code[sym], 9);
            if (t.match) {
                const unsigned slot = (sym - 256) / 8;
                if (slot >= 3) w.put(t.offset - (slots.base[slot] - 2), slots.extra[slot]);
            }
            pos += t.match ? t.length : 1;
            if (pos % kFrame == 0 && pos < plain.size()) w.align16();
        }
    }
    w.align16();
    return w.out;
}

unsigned bits_of(std::uint32_t window) { return static_cast<unsigned>(std::bit_width(window) - 1); }

// Some bytes that look like a program: repeated phrases (matches), runs, and noise (literals).
Bytes sample(std::size_t n, unsigned seed) {
    std::mt19937 rng(seed);
    Bytes out;
    const Bytes phrase = fs::read_file(testing::data_path("formats/testdata/pacman_ce.xex.head"));
    while (out.size() < n) {
        switch (rng() % 4) {
            case 0: out.insert(out.end(), 1 + rng() % 200, static_cast<std::uint8_t>(rng())); break;   // a run
            case 1: {                                                                                  // a phrase of the real header
                const std::size_t at = rng() % (phrase.size() - 300);
                out.insert(out.end(), phrase.begin() + static_cast<std::ptrdiff_t>(at),
                           phrase.begin() + static_cast<std::ptrdiff_t>(at + 1 + rng() % 250));
                break;
            }
            case 2:
                for (unsigned i = 0, k = 1 + rng() % 100; i < k; ++i) out.push_back(static_cast<std::uint8_t>(rng()));   // noise
                break;
            default:
                if (out.size() > 600) {   // a far back-reference (may overlap itself, like an LZ match)
                    const std::size_t distance = 1 + rng() % std::min<std::size_t>(out.size() - 1, 30000);
                    const std::size_t length = 1 + rng() % 250;
                    for (std::size_t i = 0; i < length; ++i) out.push_back(out[out.size() - distance]);
                }
        }
    }
    out.resize(n);
    return out;
}

CB_TEST("formats.lzx.verbatim_blocks", [] {
    for (std::size_t n : {std::size_t{1}, std::size_t{2}, std::size_t{100}, std::size_t{32768}, std::size_t{32769},
                          std::size_t{100000}}) {
        const Bytes plain = sample(n, static_cast<unsigned>(n));
        CB_CHECK(decompress(encode(plain), 15, plain.size()) == plain);
    }
});

CB_TEST("formats.lzx.blocks_that_end_inside_a_frame", [] {
    const Bytes plain = sample(90000, 4);
    for (std::size_t block : {std::size_t{1000}, std::size_t{7001}, std::size_t{32768}, std::size_t{40000}}) {
        CB_CHECK(decompress(encode(plain, {.block_size = block}), 15, plain.size()) == plain);
    }
});

CB_TEST("formats.lzx.uncompressed_blocks_mixed_with_verbatim", [] {
    const Bytes plain = sample(80001, 8);   // odd length: the stored block is padded to a word
    CB_CHECK(decompress(encode(plain, {.block_size = 5001, .kinds = {BlockKind::Verbatim, BlockKind::Uncompressed, BlockKind::Uncompressed}}),
                        15, plain.size()) == plain);
    CB_CHECK(decompress(encode(plain, {.kinds = {BlockKind::Uncompressed}}), 15, plain.size()) == plain);
});

CB_TEST("formats.lzx.larger_windows", [] {
    const Bytes plain = sample(300000, 7);
    for (unsigned bits : {16u, 17u, 18u, 20u, 21u}) {
        CB_CHECK(decompress(encode(plain, {.window_bits = bits}), bits, plain.size()) == plain);
    }
});

CB_TEST("formats.lzx.intel_e8_translation_is_undone", [] {
    // A stream whose header says E8 translation was on, filesize 1000. The data holds E8 operands in the encoder's
    // (translated, absolute) form; the decoder turns each back into a relative displacement.
    Bytes stored(100, 0);
    auto put32 = [&](std::size_t at, std::int32_t v) {
        for (unsigned i = 0; i < 4; ++i) stored[at + 1 + i] = static_cast<std::uint8_t>(static_cast<std::uint32_t>(v) >> (8 * i));
        stored[at] = 0xE8;
    };
    put32(10, 100);     // absolute 100 at position 10: relative 90
    put32(30, -20);     // negative, within -pos: the encoder added the file size... decoder adds it back: -20 + 1000
    put32(50, 5000);    // not below the file size: left alone
    put32(70, -500);    // below -pos: left alone
    Bytes expected = stored;
    auto set32 = [&](std::size_t at, std::int32_t v) {
        for (unsigned i = 0; i < 4; ++i) expected[at + 1 + i] = static_cast<std::uint8_t>(static_cast<std::uint32_t>(v) >> (8 * i));
    };
    set32(10, 100 - 10);
    set32(30, -20 + 1000);
    EncodeOptions opt;
    opt.kinds = {BlockKind::Uncompressed};
    opt.e8_filesize = 1000;
    CB_CHECK(decompress(encode(stored, opt), 15, stored.size()) == expected);
    // the last 10 bytes of a frame are never translated
    Bytes tail(40, 0);
    tail[35] = 0xE8;
    tail[36] = 7;
    CB_CHECK(decompress(encode(tail, opt), 15, tail.size()) == tail);
});

CB_TEST("formats.lzx.output_length_is_exact", [] {
    const Bytes plain = sample(5000, 11);
    const Bytes packed = encode(plain);
    CB_CHECK(decompress(packed, 15, 1234) == Bytes(plain.begin(), plain.begin() + 1234));
    CB_CHECK_EQ(decompress(packed, 15, 0).size(), 0u);
});

CB_TEST("formats.lzx.reference_data_extends_the_history_not_the_output", [] {
    // A stream made without a reference decodes to the same bytes with one: matches are relative to the output
    // position, the reference only lengthens the history before it, and none of it appears in the result.
    const Bytes plain = sample(60000, 5);
    const Bytes packed = encode(plain);
    CB_CHECK(decompress(packed, 15, plain.size(), sample(20000, 6)) == plain);
    CB_CHECK(decompress(packed, 15, plain.size(), Bytes(40000, 1)) == plain);   // longer than the window
});

CB_TEST("formats.lzx.damaged_streams_are_errors_not_garbage", [] {
    const Bytes plain = sample(50000, 9);
    const Bytes packed = encode(plain);
    // cut short: runs out of data
    const Bytes cut(packed.begin(), packed.begin() + static_cast<std::ptrdiff_t>(packed.size() / 3));
    CB_CHECK_THROWS_AS(FormatError, (void)decompress(cut, 15, plain.size()));
    CB_CHECK_THROWS_AS(FormatError, (void)decompress(Bytes{}, 15, 100));
    // a bad window
    CB_CHECK_THROWS_AS(UnsupportedError, (void)decompress(packed, 14, 100));
    CB_CHECK_THROWS_AS(UnsupportedError, (void)decompress(packed, 22, 100));
    // an unknown block type
    Bytes badtype = {0x00, 0xE0, 0, 0};   // E8 flag 0, block type 7
    CB_CHECK_THROWS_AS(FormatError, (void)decompress(badtype, 15, 100));
    // an uncompressed block longer than the data
    Bytes stored = encode(Bytes(100, 5), {.kinds = {BlockKind::Uncompressed}});
    stored.resize(stored.size() - 20);
    CB_CHECK_THROWS_AS(FormatError, (void)decompress(stored, 15, 100));
    // bit flips everywhere: every outcome is an exception or output, never a crash (checked under the sanitizer)
    std::mt19937 rng(1);
    for (int trial = 0; trial < 300; ++trial) {
        Bytes bad = packed;
        bad[rng() % bad.size()] ^= static_cast<std::uint8_t>(1u << (rng() % 8));
        try {
            (void)decompress(bad, 15, plain.size());
        } catch (const FormatError&) {
        }
    }
});

// ---- the block chain of an XEX ----------------------------------------------------------------------------

// One block: the next block's size and hash, then chunks (16-bit length + bytes) ended by a zero length.
Bytes block(std::uint32_t next_size, const crypto::Sha1Digest& next_hash, const std::vector<Bytes>& chunks) {
    io::ByteWriter w;
    w.u32_be(next_size);
    w.bytes(next_hash);
    for (const Bytes& c : chunks) {
        w.u16_be(static_cast<std::uint16_t>(c.size()));
        w.bytes(c);
    }
    w.u16_be(0);
    return w.take();
}

CB_TEST("formats.lzx.deblock_follows_the_chain", [] {
    const Bytes a1(100, 0xA1), a2(50, 0xA2), b1(200, 0xB1), c1(10, 0xC1);
    const Bytes last = block(0, crypto::Sha1Digest{}, {c1});
    const Bytes second = block(static_cast<std::uint32_t>(last.size()), crypto::sha1(last), {b1});
    const Bytes first = block(static_cast<std::uint32_t>(second.size()), crypto::sha1(second), {a1, a2});
    Bytes image = first;
    append(image, second);
    append(image, last);
    const crypto::Sha1Digest head = crypto::sha1(first);
    Bytes want = a1;
    append(want, a2);
    append(want, b1);
    append(want, c1);
    CB_CHECK(deblock(image, static_cast<std::uint32_t>(first.size()), head, true) == want);
    CB_CHECK(deblock(image, static_cast<std::uint32_t>(first.size()), head, false) == want);
    CB_CHECK(deblock(image, 0, head, true).empty());   // an image with no blocks
});

CB_TEST("formats.lzx.deblock_checks_the_hashes", [] {
    const Bytes last = block(0, crypto::Sha1Digest{}, {Bytes(10, 1)});
    const Bytes first = block(static_cast<std::uint32_t>(last.size()), crypto::sha1(last), {Bytes(10, 2)});
    Bytes image = first;
    append(image, last);
    const std::uint32_t first_size = static_cast<std::uint32_t>(first.size());
    const crypto::Sha1Digest head = crypto::sha1(first);
    // a flipped byte in the second block: its hash (named in the first) no longer matches
    Bytes bad = image;
    bad[first.size() + 30] ^= 1;   // inside the chunk data
    CB_CHECK_THROWS_AS(FormatError, (void)deblock(bad, first_size, head, true));
    CB_CHECK_NOTHROW((void)deblock(bad, first_size, head, false));
    // the wrong first hash, no hash at all
    crypto::Sha1Digest wrong = head;
    wrong[0] ^= 1;
    CB_CHECK_THROWS_AS(FormatError, (void)deblock(image, first_size, wrong, true));
    CB_CHECK_THROWS_AS(FormatError, (void)deblock(image, first_size, ByteView{}, true));
    // a block the image is too short to hold
    CB_CHECK_THROWS_AS(FormatError, (void)deblock(ByteView(image).first(first.size() - 1), first_size, head, false));
    // a chunk that claims more than the block holds
    Bytes lying = block(0, crypto::Sha1Digest{}, {Bytes(10, 1)});
    lying[24] = 0x7F;
    CB_CHECK_THROWS_AS(FormatError, (void)deblock(lying, static_cast<std::uint32_t>(lying.size()), crypto::sha1(lying), true));
});

CB_TEST("formats.lzx.decompress_xex_runs_the_whole_chain", [] {
    const Bytes plain = sample(90000, 21);
    const Bytes stream = encode(plain);
    std::vector<Bytes> parts;   // the stream cut into block-sized pieces
    for (std::size_t at = 0; at < stream.size(); at += 20000) {
        parts.emplace_back(stream.begin() + static_cast<std::ptrdiff_t>(at),
                           stream.begin() + static_cast<std::ptrdiff_t>(std::min(stream.size(), at + 20000)));
    }
    std::vector<Bytes> made(parts.size());
    crypto::Sha1Digest next_hash{};
    std::uint32_t next_size = 0;
    for (std::size_t i = parts.size(); i-- > 0;) {   // built from the last block back, each naming the next
        made[i] = block(next_size, next_hash, {parts[i]});
        next_hash = crypto::sha1(made[i]);
        next_size = static_cast<std::uint32_t>(made[i].size());
    }
    Bytes image;
    for (const Bytes& b : made) append(image, b);
    CB_CHECK(decompress_xex(image, 0x8000, next_size, next_hash, plain.size()) == plain);
    CB_CHECK_THROWS_AS(FormatError, (void)decompress_xex(image, 0x8001, next_size, next_hash, plain.size()));
});

// ---- a real image, against the xex program's own decoder -------------------------------------------------------

// Pac-Man Championship Edition (Xbox Live Arcade), a retail title: encrypted, LZX compressed. It is a game binary, so it
// is not checked in; the test runs where the file is (the typed-port harness inputs, or CB_TEST_XEX) and is skipped
// where it is not.
std::filesystem::path real_xex() {
    if (const auto env = fs::get_env("CB_TEST_XEX")) return fs::from_text(*env);
    return "C:/Users/mrtit/Documents/GitHub/CodeBreaker/state/work/typed-port/formats.xex/inputs/Pac-Man_C.E.xex";
}

CB_TEST("formats.lzx.a_real_title_decodes_as_the_xex_program_decodes_it", [] {
    const std::filesystem::path path = real_xex();
    if (!std::filesystem::is_regular_file(path)) {
        std::printf("      (skipped: %s is not here)\n", path.string().c_str());
        return;
    }
    const Bytes file = fs::read_file(path);
    const ::xex::XexFile x = ::xex::XexFile::parse(std::vector<std::uint8_t>(file.begin(), file.end()));
    CB_CHECK(x.is_compressed() && x.is_encrypted());
    // the oracle: the xex program's full pipeline
    const std::vector<std::uint8_t> oracle = ::xex::reconstruct_basefile(x);
    // ours: decrypt the way it does (retail key unwraps the title key; AES-128-CBC, zero IV), then deblock + decode
    Bytes work(file.begin() + x.data_offset(), file.end());
    std::uint8_t title_key[16];
    ::xex::crypto::Aes128(::xex::detect_kek(x)).decrypt_block(x.security().aes_key.data(), title_key);
    std::uint8_t iv[16] = {0};
    ::xex::crypto::Aes128(title_key).cbc_decrypt(work.data(), work.size() & ~std::size_t{15}, iv);
    const ::xex::OptionalHeader* info = x.find_header(::xex::kHdrFileFormatInfo);
    CB_CHECK(info != nullptr);
    const ByteView first_hash = ByteView(file).subspan(info->value + 16, 20);
    const Bytes ours = decompress_xex(work, x.comp_window_size(), x.comp_first_block_size(), first_hash, x.security().image_size);
    CB_CHECK_EQ(ours.size(), oracle.size());
    CB_CHECK(ours == Bytes(oracle.begin(), oracle.end()));
    CB_CHECK_EQ(ours[0], 'M');   // a PE image: MZ
    CB_CHECK_EQ(ours[1], 'Z');
});

}  // namespace
}  // namespace cb::formats::lzx
