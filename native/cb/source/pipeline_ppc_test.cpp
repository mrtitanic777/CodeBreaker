// Tests of `match` on an Xbox 360 image (source/pipeline_ppc): the object reader, its bit-level masks, the comparison, and the original's code up to its
// padding. Four checks: small functions compiled with the Xbox 360 compiler the settings name (powerpc_tools),
// which a machine without one skips.
#include "cb/source/pipeline_ppc.hpp"

#include "cb/base/config.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace stdfs = std::filesystem;
using namespace cb;
namespace ppcm = cb::source::pipeline;

namespace {

constexpr std::string_view ppct_source = R"(
extern int counter;
int helper(int);
int caller(int a, int b) {
    int i, s = 0;
    for (i = 0; i < a; i++) {
        if (i & 1) s += helper(i); else s -= b;
    }
    counter = s;
    return s * 3;
}
)";

std::uint32_t ppct_word(const Bytes& b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) << 24 | static_cast<std::uint32_t>(b[at + 1]) << 16 | static_cast<std::uint32_t>(b[at + 2]) << 8 | b[at + 3];
}

// The function `caller` as the compiler makes it, or nothing on a machine without the compiler.
std::optional<verify::authored::repair::Code> ppct_built(fs::ScopedTempDir& work) {
    const config::Config config = config::Config::load(config::current().main_checkout());
    const std::optional<stdfs::path> folder = config.path("powerpc_tools");
    std::error_code ec;
    if (!folder || !stdfs::is_regular_file(*folder / "cl.exe", ec)) return std::nullopt;
    return ppcm::ppc_compile_function(*folder / "cl.exe", "", ppct_source, "caller", "/O2", work.path());
}

void ppct_put_be(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}
void ppct_put_le(Bytes& b, std::size_t at, std::uint32_t v, int n) {
    for (int i = 0; i < n; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
}
void ppct_put_text(Bytes& b, std::size_t at, std::string_view text) {
    for (std::size_t i = 0; i < text.size(); ++i) b[at + i] = static_cast<std::uint8_t>(text[i]);
}

// A PowerPC image with one function at 0x82003000 that the exception table says is 0x40 bytes: mflr r12; bl +0x10; blr; a zero word (padding);
// then another function's code (li r3,0; blr) and zeros.
Bytes ppct_image() {
    Bytes f(0x700, 0);
    ppct_put_text(f, 0, "MZ");
    ppct_put_le(f, 0x3c, 0x40, 4);
    ppct_put_text(f, 0x40, "PE");
    ppct_put_le(f, 0x44, 0x01F2, 2);
    ppct_put_le(f, 0x46, 2, 2);
    ppct_put_le(f, 0x54, 0xE0, 2);
    ppct_put_le(f, 0x56, 0x0102, 2);
    const std::size_t opt = 0x58;
    ppct_put_le(f, opt, 0x10b, 2);
    ppct_put_le(f, opt + 16, 0x3000, 4);
    ppct_put_le(f, opt + 28, 0x82000000u, 4);
    ppct_put_le(f, opt + 32, 0x1000, 4);
    ppct_put_le(f, opt + 36, 0x200, 4);
    ppct_put_le(f, opt + 56, 0x4000, 4);
    ppct_put_le(f, opt + 60, 0x200, 4);
    ppct_put_le(f, opt + 92, 16, 4);
    ppct_put_le(f, opt + 96 + 3 * 8, 0x2000, 4);   // exception table
    ppct_put_le(f, opt + 96 + 3 * 8 + 4, 0x8, 4);
    struct Sec {
        const char* name;
        std::uint32_t rva, raw, size, chars;
    };
    const Sec secs[] = {{".pdata", 0x2000, 0x400, 0x8, 0x40000040}, {".text", 0x3000, 0x600, 0x80, 0x60000020}};
    for (std::size_t i = 0; i < 2; ++i) {
        const std::size_t h = 0x138 + 40 * i;
        ppct_put_text(f, h, secs[i].name);
        ppct_put_le(f, h + 8, secs[i].size, 4);
        ppct_put_le(f, h + 12, secs[i].rva, 4);
        ppct_put_le(f, h + 16, secs[i].size, 4);
        ppct_put_le(f, h + 20, secs[i].raw, 4);
        ppct_put_le(f, h + 36, secs[i].chars, 4);
    }
    ppct_put_be(f, 0x600, 0x7D8802A6);
    ppct_put_be(f, 0x604, 0x48000011);   // bl +0x10: out of the function, once it ends before its padding
    ppct_put_be(f, 0x608, 0x4E800020);
    ppct_put_be(f, 0x610, 0x38600000);
    ppct_put_be(f, 0x614, 0x4E800020);
    ppct_put_be(f, 0x400, 0x82003000u);
    ppct_put_be(f, 0x404, (0x10u << 8) | 2);   // 0x40 bytes (16 words), prologue 2: as the small image of powerpc_image_test has it
    return f;
}

}  // namespace

CB_TEST("source.pipeline_ppc.one_function_whole", [] {
    fs::ScopedTempDir work("ppc-match");
    const auto built = ppct_built(work);
    if (!built) return;
    const std::size_t n = built->bytes.size() / 4;
    const std::uint32_t last = ppct_word(built->bytes, 4 * (n - 1));
    // it ends returning: blr, or a jump into the register-restore helper (the compiler's epilogue)
    CB_CHECK(last == 0x4E800020 || ((last >> 26) == 18 && !(last & 1)));
    CB_CHECK_EQ(built->word_mask.size(), n);
    CB_CHECK(n > 10);   // a label inside the function must not cut it
});

CB_TEST("source.pipeline_ppc.masks_are_fields", [] {
    fs::ScopedTempDir work("ppc-match");
    const auto built = ppct_built(work);
    if (!built) return;
    const std::size_t n = built->bytes.size() / 4;
    std::size_t calls = 0, highs = 0;
    for (std::size_t i = 0; i < n; ++i) {
        const std::uint32_t w = ppct_word(built->bytes, 4 * i);
        const std::uint32_t mask = built->word_mask[i];
        if ((w >> 26) == 18 && (w & 1)) {   // a bl's distance, and only it
            ++calls;
            CB_CHECK_EQ(mask, ppcm::kPpcMaskLi);
        } else if ((w >> 26) == 15 && ((w >> 16) & 31) == 0) {   // the high half of the global's address
            ++highs;
            CB_CHECK_EQ(mask, ppcm::kPpcMaskHalf);
        } else if ((w >> 26) != 15 && (w >> 26) != 18 && mask != ppcm::kPpcMaskHalf) {   // nothing else is masked (a low half rides on its own instruction)
            CB_CHECK_EQ(mask, 0u);
        }
    }
    CB_CHECK(calls > 0);
    CB_CHECK(highs > 0);
});

CB_TEST("source.pipeline_ppc.distance_masks_places_and_counts_registers", [] {
    fs::ScopedTempDir work("ppc-match");
    const auto built = ppct_built(work);
    if (!built) return;
    using verify::ByteMatch;
    verify::authored::repair::Code other = *built;
    other.word_mask.assign(built->word_mask.size(), 0);
    auto d = ppcm::ppc_compare(*built, other);
    CB_CHECK(d.match == ByteMatch::Identical);
    CB_CHECK_EQ(d.differing, 0u);
    std::size_t call = 0;
    while (!((ppct_word(other.bytes, call) >> 26) == 18 && (ppct_word(other.bytes, call) & 1))) call += 4;
    other.bytes[call + 2] ^= 0x40;   // another callee address: masked
    CB_CHECK(ppcm::ppc_compare(*built, other).match == ByteMatch::Identical);
    other.bytes[0] ^= 0x01;   // another register: not masked
    d = ppcm::ppc_compare(*built, other);
    CB_CHECK(d.match == ByteMatch::SameSize);
    CB_CHECK_EQ(d.differing, 1u);
    verify::authored::repair::Code shorter = *built;
    shorter.bytes.resize(shorter.bytes.size() - 4);
    shorter.word_mask.assign(built->word_mask.size() - 1, 0);
    CB_CHECK(ppcm::ppc_compare(*built, shorter).match == ByteMatch::Different);
});

CB_TEST("source.pipeline_ppc.the_original_stops_at_padding", [] {
    const formats::powerpc_image::Image image = formats::powerpc_image::Image::parse(ppct_image());
    const auto codes = ppcm::ppc_original_code(image, {0x82003000});
    CB_CHECK(codes.contains(0x82003000));
    const auto& code = codes.at(0x82003000);
    CB_CHECK_EQ(code.bytes.size(), 12u);                 // the zero word ends it, though the record says 0x40 bytes
    CB_CHECK_EQ(code.word_mask.size(), 3u);
    CB_CHECK_EQ(code.word_mask[1], ppcm::kPpcMaskLi);    // bl out of the function: masked
    CB_CHECK_EQ(code.word_mask[0], 0u);
});
