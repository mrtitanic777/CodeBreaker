#include "cb/build/mzfile.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build;

void put16(Bytes& b, std::size_t at, unsigned v) {
    b[at] = static_cast<std::uint8_t>(v);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}
unsigned get16(const Bytes& b, std::size_t at) { return b[at] | (b[at + 1] << 8); }

// A linker's header: 0x40 bytes (4 paragraphs) with a relocation table of three entries at 0x1E, a message kept
// behind it, and every build-decided field set to something.
Bytes original_header() {
    Bytes h(0x40, 0);
    h[0] = 'M';
    h[1] = 'Z';
    put16(h, 0x02, 0x123);                // last page
    put16(h, 0x04, 7);                    // pages
    put16(h, 0x06, 3);                    // relocations
    put16(h, 0x08, 4);                    // header paragraphs (kept)
    put16(h, 0x0A, 0x55);                 // min alloc
    put16(h, 0x0C, 0xFFFF);               // max alloc (kept)
    put16(h, 0x0E, 0x10);                 // ss
    put16(h, 0x10, 0x200);                // sp
    put16(h, 0x12, 0xABCD);               // checksum (kept)
    put16(h, 0x14, 0x20);                 // ip
    put16(h, 0x16, 0x1);                  // cs
    put16(h, 0x18, 0x1E);                 // relocation table at 0x1E
    for (std::size_t i = 0; i < 12; ++i) h[0x1E + i] = static_cast<std::uint8_t>(0x40 + i);   // 3 relocations
    const std::string message = "Incorrect MS-DOS version";
    for (std::size_t i = 0; i < message.size() && 0x2A + i < h.size(); ++i) h[0x2A + i] = static_cast<std::uint8_t>(message[i]);
    return h;
}

}  // namespace

CB_TEST("build.mzfile.template_blanks_what_the_build_decides", [] {
    const Bytes tpl = mzfile::make_template(original_header());
    CB_CHECK_EQ(tpl.size(), 0x40u);
    for (const std::size_t at : {0x02, 0x04, 0x06, 0x0A, 0x0E, 0x10, 0x14, 0x16}) CB_CHECK_EQ(get16(tpl, at), 0u);
    for (std::size_t i = 0x1E; i < 0x2A; ++i) CB_CHECK_EQ(tpl[i], 0);   // the relocation table
    CB_CHECK_EQ(get16(tpl, 0x08), 4u);                                    // what the linker's options chose stays
    CB_CHECK_EQ(get16(tpl, 0x0C), 0xFFFFu);
    CB_CHECK_EQ(get16(tpl, 0x12), 0xABCDu);
    CB_CHECK_EQ(get16(tpl, 0x18), 0x1Eu);
    CB_CHECK_EQ(tpl[0x2A], 'I');                                          // the message behind the table stays
    CB_CHECK_THROWS_AS(TruncatedError, (void)mzfile::make_template(Bytes(10, 0)));
    Bytes lies = original_header();
    put16(lies, 0x06, 0x400);                                             // a table that runs past the header
    CB_CHECK_THROWS_AS(FormatError, (void)mzfile::make_template(lies));
});

CB_TEST("build.mzfile.compose_fills_the_blanks", [] {
    const Bytes tpl = mzfile::make_template(original_header());
    const Bytes module(1000, 0x90);
    const std::vector<mzfile::Relocation> relocs{{0x10, 0x1}, {0x22, 0x2}, {0x1234, 0x56}};
    const Bytes file = mzfile::compose(tpl, module, relocs, {.cs = 0x7, .ip = 0x8, .ss = 0x9, .sp = 0xA}, 0x33);
    CB_CHECK_EQ(file.size(), 0x40u + 1000u);
    const std::size_t total = file.size();                  // 1064 = 2 pages and 40 bytes
    CB_CHECK_EQ(get16(file, 0x02), total % 512);
    CB_CHECK_EQ(get16(file, 0x04), (total + 511) / 512);
    CB_CHECK_EQ(get16(file, 0x06), 3u);
    CB_CHECK_EQ(get16(file, 0x0A), 0x33u);
    CB_CHECK_EQ(get16(file, 0x0E), 9u);
    CB_CHECK_EQ(get16(file, 0x10), 0xAu);
    CB_CHECK_EQ(get16(file, 0x14), 8u);
    CB_CHECK_EQ(get16(file, 0x16), 7u);
    CB_CHECK_EQ(get16(file, 0x1E), 0x10u);                  // offset, segment pairs
    CB_CHECK_EQ(get16(file, 0x20), 0x1u);
    CB_CHECK_EQ(get16(file, 0x26), 0x1234u);
    CB_CHECK_EQ(get16(file, 0x28), 0x56u);
    CB_CHECK_EQ(file[0x40], 0x90);                          // the module follows
    CB_CHECK_EQ(get16(file, 0x12), 0xABCDu);                // the kept words are untouched

    // a last page that is exactly full: 0, or 512 when the linker wrote it so
    const Bytes full(512 - 0x40 + 512, 0);
    CB_CHECK_EQ(get16(mzfile::compose(tpl, full, {}, {}, 0), 0x02), 0u);
    CB_CHECK_EQ(get16(mzfile::compose(tpl, full, {}, {}, 0, true), 0x02), 512u);
    CB_CHECK_EQ(get16(mzfile::compose(tpl, full, {}, {}, 0, true), 0x04), 2u);
});

CB_TEST("build.mzfile.compose_errors", [] {
    const Bytes tpl = mzfile::make_template(original_header());
    const Bytes module(10, 0);
    // the template has room for three relocations (to 0x2A); a fourth runs into the message
    const std::vector<mzfile::Relocation> four(4, mzfile::Relocation{1, 2});
    CB_CHECK_THROWS_AS(FormatError, (void)mzfile::compose(tpl, module, four, {}, 0));
    // a table that is not blank is not room
    Bytes dirty = tpl;
    dirty[0x1E] = 1;
    CB_CHECK_THROWS_AS(FormatError, (void)mzfile::compose(dirty, module, std::vector<mzfile::Relocation>(1), {}, 0));
    // sizes that do not fit the header's words
    CB_CHECK_THROWS_AS(FormatError, (void)mzfile::compose(tpl, Bytes(512 * 70000ull, 0), {}, {}, 0));
    CB_CHECK_THROWS_AS(TruncatedError, (void)mzfile::compose(Bytes(4, 0), module, {}, {}, 0));
});
