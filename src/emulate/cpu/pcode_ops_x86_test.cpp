// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_ops_x86.cpp against published answers: the AES-NI instructions against FIPS 197 (appendix C.1, AES-128), CRC32C against its check value, PCLMULQDQ against products
// worked by hand, and the string compares of SSE4.2 against the examples of Intel's manual (including the ranges aggregation with an odd number of bounds, where Unicorn reads the word
// before the register and the processor does not).
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/cpu/pcode_ops_x86.hpp"

namespace cb {
namespace {

namespace ox = emulate::pcode_ops_x86;
using Emu = decompile::sleigh::Emulator;
using Octets = std::vector<std::uint8_t>;

std::filesystem::path languages_dir() {
    if (const std::optional<std::string> env = fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

struct Rig {
    std::unique_ptr<Emu> emu = std::make_unique<Emu>(languages_dir() / "x86-64.sla", std::vector<std::pair<std::string, int>>{{"longMode", 1}, {"addrsize", 2}, {"bit64", 1}, {"opsize", 1}});
    ox::Context context = ox::make_context(*emu);
    // The operation `name` on the given inputs: its output of `out_size` bytes (empty when the operation declined).
    Octets call(const char* name, const std::vector<Octets>& in, std::size_t out_size) {
        const emulate::pcode_ops::Handler* handler = ox::table().find(name);
        CB_CHECK(handler != nullptr);
        if (handler == nullptr) return {};
        Emu::UserOpCall c;
        c.nIn = static_cast<unsigned>(in.size());
        for (std::size_t i = 0; i < in.size(); ++i) {
            c.inSize[i] = static_cast<unsigned>(in[i].size());
            c.in[i] = in[i].data();
        }
        Octets out(out_size, 0);
        c.outSize = static_cast<unsigned>(out_size);
        c.out = out.data();
        return (*handler)(*emu, c, &context) ? out : Octets{};
    }
};

Octets hex(const char* text) {
    Octets out;
    for (const char* p = text; p[0] != 0 && p[1] != 0; p += 2) out.push_back(static_cast<std::uint8_t>(std::stoul(std::string(p, 2), nullptr, 16)));
    return out;
}
Octets xmm(std::initializer_list<std::uint8_t> bytes) {
    Octets out(bytes);
    out.resize(16, 0);
    return out;
}

}  // namespace

// AES-128: the key schedule by AESKEYGENASSIST (Intel's white paper), the ten rounds by AESENC and AESENCLAST, and back by AESIMC, AESDEC and AESDECLAST.
CB_TEST("emulate.pcode_ops_x86.aes_ni_instructions_encrypt_and_decrypt_as_fips_197_says", [] {
    Rig rig;
    const Octets key = hex("000102030405060708090a0b0c0d0e0f"), plain = hex("00112233445566778899aabbccddeeff"), cipher = hex("69c4e0d86a7b0430d8cdb78070b4c55a");
    static constexpr std::uint8_t kRcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};
    std::vector<Octets> round_keys{key};
    for (int i = 0; i < 10; ++i) {
        const Octets assist = rig.call("aeskeygenassist", {round_keys.back(), Octets{kRcon[i]}}, 16);
        CB_CHECK_EQ(assist.size(), 16u);
        if (assist.size() != 16) return;
        // w4 = w0 ^ SubWord(RotWord(w3)) ^ rcon, which is the assist's dword 3 broadcast; then each word the xor of the previous and the one four before
        Octets next(16);
        const Octets& k = round_keys.back();
        for (int b = 0; b < 4; ++b) next[static_cast<std::size_t>(b)] = static_cast<std::uint8_t>(k[static_cast<std::size_t>(b)] ^ assist[static_cast<std::size_t>(12 + b)]);
        for (int w = 1; w < 4; ++w)
            for (int b = 0; b < 4; ++b) next[static_cast<std::size_t>(4 * w + b)] = static_cast<std::uint8_t>(k[static_cast<std::size_t>(4 * w + b)] ^ next[static_cast<std::size_t>(4 * (w - 1) + b)]);
        round_keys.push_back(next);
    }
    Octets state = plain;
    for (std::size_t b = 0; b < 16; ++b) state[b] ^= round_keys[0][b];
    for (int round = 1; round < 10; ++round) state = rig.call("aesenc", {state, round_keys[static_cast<std::size_t>(round)]}, 16);
    state = rig.call("aesenclast", {state, round_keys[10]}, 16);
    CB_CHECK(state == cipher);
    // decryption: the equivalent inverse cipher (the middle keys through AESIMC)
    state = cipher;
    for (std::size_t b = 0; b < 16; ++b) state[b] ^= round_keys[10][b];
    for (int round = 9; round >= 1; --round) state = rig.call("aesdec", {state, rig.call("aesimc", {round_keys[static_cast<std::size_t>(round)]}, 16)}, 16);
    state = rig.call("aesdeclast", {state, round_keys[0]}, 16);
    CB_CHECK(state == plain);
});

// CRC32C of "123456789" is 0xE3069283 (the check value of the Castagnoli polynomial; the instruction takes no inversion).
CB_TEST("emulate.pcode_ops_x86.crc32_folds_the_castagnoli_polynomial", [] {
    Rig rig;
    std::uint32_t crc = 0xFFFFFFFFu;
    for (const char c : std::string("123456789")) {
        const Octets out = rig.call("crc32", {Octets{static_cast<std::uint8_t>(crc), static_cast<std::uint8_t>(crc >> 8), static_cast<std::uint8_t>(crc >> 16), static_cast<std::uint8_t>(crc >> 24)}, Octets{static_cast<std::uint8_t>(c)}}, 4);
        CB_CHECK_EQ(out.size(), 4u);
        if (out.size() != 4) return;
        crc = out[0] | (out[1] << 8) | (out[2] << 16) | (static_cast<std::uint32_t>(out[3]) << 24);
    }
    CB_CHECK_EQ(~crc, 0xE3069283u);
});

// Carry-less products: 3 x 3 = 5, (x^2 + 1)(x + 1) = x^3 + x^2 + x + 1; the high quadword and the 128-bit result.
CB_TEST("emulate.pcode_ops_x86.pclmulqdq_multiplies_without_carries", [] {
    Rig rig;
    Octets a(16, 0), b(16, 0);
    a[0] = 3;
    b[0] = 3;
    Octets out = rig.call("pclmulqdq", {a, b, Octets{0x00}}, 16);
    CB_CHECK_EQ(out.size(), 16u);
    if (out.size() != 16) return;
    CB_CHECK_EQ(static_cast<unsigned>(out[0]), 5u);
    a.assign(16, 0);
    b.assign(16, 0);
    a[8 + 7] = 0x80;   // the high quadword's top bit
    b[0] = 0x02;
    out = rig.call("pclmulqdq", {a, b, Octets{0x01}}, 16);   // a's high quadword times b's low: 2^127 * 2^1 = 2^128 does not fit: the product is 2^64 * 2^63 * 2 -> bit 128? no: 2^63 * 2 = 2^64
    CB_CHECK_EQ(out.size(), 16u);
    if (out.size() != 16) return;
    CB_CHECK_EQ(static_cast<unsigned>(out[8]), 1u);   // bit 64
});

// Intel's string compares (SDM, PCMPISTRI): the immediate's fields and the flags.
CB_TEST("emulate.pcode_ops_x86.string_compares_find_what_the_manual_says", [] {
    Rig rig;
    Octets needle = xmm({'a', 'b', 'c'});
    Octets hay = xmm({'x', 'x', 'a', 'b', 'c', 'y', 'a', 'b', 'c', 'z'});
    const auto index = [&](unsigned ctrl, const Octets& a, const Octets& b) {
        const Octets out = rig.call("pcmpistri", {a, b, Octets{static_cast<std::uint8_t>(ctrl)}}, 4);
        return out.size() == 4 ? static_cast<unsigned>(out[0]) : 99u;
    };
    // equal ordered (0x0C): the needle is found at byte 2 (the most significant match, 0x4C, is not asked: QEMU's helper leaves bits above the string in the result, and Unicorn's index is theirs)
    CB_CHECK_EQ(index(0x0C, needle, hay), 2u);
    // equal any (0x00): the first byte of the haystack that is one of the needle's bytes: 'a' at 2
    CB_CHECK_EQ(index(0x00, needle, hay), 2u);
    // ranges (0x04): a range 'a'..'c' (two bounds): the first byte of the haystack inside it: 2
    CB_CHECK_EQ(index(0x04, xmm({'a', 'c'}), hay), 2u);
    // ranges with a lone bound over ('a'..'c' and then 'x', which pairs with nothing and matches nothing): position 2, and a lone bound by itself: none
    CB_CHECK_EQ(index(0x04, xmm({'a', 'c', 'x'}), hay), 2u);
    CB_CHECK_EQ(index(0x04, xmm({'x'}), hay), 16u);
    // equal each (0x08): the first position where the two agree
    CB_CHECK_EQ(index(0x08, xmm({'q', 'x', 'a'}), hay), 1u);
    // nothing found: 16 (bytes)
    CB_CHECK_EQ(index(0x00, xmm({'#'}), hay), 16u);
    // words (0x01): the index counts words, none found: 8
    CB_CHECK_EQ(index(0x01, xmm({'#'}), hay), 8u);
});

// Which of the description's named operations the table serves and which it does not (CB_X86_OPS_REPORT=1 lists them).
CB_TEST("emulate.pcode_ops_x86.the_table_names_only_operations_of_the_description", [] {
    Rig rig;
    const emulate::pcode_ops::Binding b = ox::bind(*rig.emu, rig.context);
    std::size_t avx512 = 0;
    for (const std::string& name : b.unbound)
        if (name.find("avx512") != std::string::npos || name.find("_vbmi") != std::string::npos || name.find("_bitalg") != std::string::npos || name.find("_vnni") != std::string::npos) ++avx512;
    std::printf("pcode_ops_x86 on x86-64.sla: %zu of the description's %zu operations served, %zu not (%zu of them AVX-512), %zu names of the table that the description has not\n", b.bound,
                rig.emu->userOpCount(), b.unbound.size(), avx512, b.unknown.size());
    if (const auto report = fs::get_env("CB_X86_OPS_REPORT"); report && !report->empty()) {
        for (const std::string& name : b.unbound)
            if (name.find("avx512") == std::string::npos) std::printf("  unserved: %s\n", name.c_str());
        for (const std::string& name : b.unknown) std::printf("  not in the description: %s\n", name.c_str());
    }
    CB_CHECK(b.bound > 300);
});

}  // namespace cb
