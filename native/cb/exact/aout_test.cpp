#include "cb/exact/aout.hpp"

#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::exact;

namespace {

// A small XENIX-style a.out: 8 bytes of text (a call whose operand is on an odd byte, and a `mov ax, imm16` of
// the data), 4 of data (a word naming the text), 6 of bss and three symbols.
Bytes small_aout(bool with_relocation = true) {
    io::ByteWriter w;
    const std::uint16_t header[] = {0407, 8, 4, 6, 3 * 12, 0, 0, static_cast<std::uint16_t>(with_relocation ? 0 : 1)};
    for (std::uint16_t v : header) w.u16(v);
    const Bytes text = {0xE8, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x90, 0x90};
    w.bytes(text);
    w.bytes(Bytes{0x01, 0x02, 0x03, 0x04});
    if (with_relocation) {
        // text: four words; the call's operand begins on an odd byte (bit 15) and is relative (bit 0) to symbol 1
        // (kind 4, symbol number in bits 4-14); the immediate names the data (2)
        const std::uint16_t text_relocs[] = {0x8000 | (1 << 4) | (4 << 1) | 1, 0, 2 << 1, 0};
        for (std::uint16_t v : text_relocs) w.u16(v);
        for (std::uint16_t v : {std::uint16_t{1 << 1}, std::uint16_t{0}}) w.u16(v);   // data: word 0 names the text
    }
    const auto symbol = [&](std::string_view name, std::uint16_t type, std::uint16_t value) {
        Bytes field(8);
        std::copy(name.begin(), name.end(), field.begin());
        w.bytes(field);
        w.u16(type);
        w.u16(value);
    };
    symbol("_start", 0x20 | 2, 0);    // global, in the text
    symbol("_ext", 0x20, 0);           // global, undefined
    symbol("_common", 0x20, 4);        // undefined with a size: a communal variable
    return w.take();
}

}  // namespace

CB_TEST("exact.aout.is_aout", [] {
    CB_CHECK(aout::is_aout(small_aout()));
    Bytes other = small_aout();
    other[0] = 0x10;
    other[1] = 0x01;   // 0420 octal: not a magic
    CB_CHECK(!aout::is_aout(other));
    CB_CHECK(!aout::is_aout(Bytes(15)));   // shorter than the header
    for (std::uint8_t lo : {std::uint8_t{0x07}, std::uint8_t{0x08}, std::uint8_t{0x09}}) {   // 0407, 0410, 0411
        Bytes b(16);
        b[0] = lo;
        b[1] = 0x01;
        CB_CHECK(aout::is_aout(b));
    }
});

CB_TEST("exact.aout.read_small_object", [] {
    const Bytes file = small_aout();
    const aout::Image image = aout::read(file);
    const ObjectFile& o = image.object;
    CB_CHECK(o.kind == ObjectKind::Aout);
    CB_CHECK_EQ(o.segs.size(), 3u);
    CB_CHECK_EQ(o.segs[0].name, "_TEXT");
    CB_CHECK_EQ(o.segs[0].cls, "CODE");
    CB_CHECK_EQ(o.segs[0].length, 8u);
    CB_CHECK_EQ(o.segs[1].align, 2u);
    CB_CHECK_EQ(o.segs[2].length, 6u);
    CB_CHECK((o.seg.at("_DATA") == (Bytes{1, 2, 3, 4})));
    CB_CHECK((o.seg.at("_BSS") == Bytes(6)));
    CB_CHECK_EQ(o.filled.at("_TEXT"), 8u);
    CB_CHECK_EQ(o.filled.at("_DATA"), 4u);
    CB_CHECK(!o.filled.contains("_BSS"));
    CB_CHECK((o.raw == file));

    CB_CHECK_EQ(image.symbols.size(), 3u);
    CB_CHECK_EQ(image.symbols[1].name, "_ext");
    CB_CHECK_EQ(image.symbols[2].value, 4);
    CB_CHECK_EQ(o.pubs.size(), 1u);
    CB_CHECK_EQ(o.pubs[0].name, "_start");
    CB_CHECK_EQ(*o.pubs[0].segment, "_TEXT");
    CB_CHECK((o.ext == (std::vector<std::string>{"_ext", "_common"})));
    CB_CHECK_EQ(o.comm.size(), 1u);
    CB_CHECK_EQ(o.comm.at("_common").size, 4u);

    CB_CHECK_EQ(o.fix.size(), 3u);
    // the call: operand on byte 1, relative to the word's end, to the external symbol 1; counts from the text's
    // start, so its displacement is the word's end
    CB_CHECK_EQ(o.fix[0].segment, "_TEXT");
    CB_CHECK_EQ(o.fix[0].offset, 1u);
    CB_CHECK(o.fix[0].kind == FixKind::Rel);
    CB_CHECK((o.fix[0].target == Target{TargetKind::External, "_ext"}));
    CB_CHECK_EQ(o.fix[0].displacement, 3);
    CB_CHECK_EQ(o.fix[1].offset, 4u);
    CB_CHECK(o.fix[1].kind == FixKind::Off);
    CB_CHECK((o.fix[1].target == Target{TargetKind::Segment, "_DATA"}));
    CB_CHECK_EQ(o.fix[1].displacement, 0);
    CB_CHECK_EQ(o.fix[2].segment, "_DATA");
    CB_CHECK((o.fix[2].target == Target{TargetKind::Segment, "_TEXT"}));
});

CB_TEST("exact.aout.no_relocation_words", [] {
    // the last header word says there is none: the symbols follow the data at once
    const aout::Image image = aout::read(small_aout(false));
    CB_CHECK(image.object.fix.empty());
    CB_CHECK_EQ(image.symbols.size(), 3u);
    CB_CHECK_EQ(image.symbols[0].name, "_start");
});

// A real-shaped object found by the fuzzing of the old harness (case 1 of exact.aout): magic 0410, text 33, data 5,
// no bss, a header word that makes the file carry relocation words for both; relocation kinds 0 and 5-7 name
// nothing known and stay as segment "?", the linker being the one to refuse them.
CB_TEST("exact.aout.unknown_relocation_kinds_are_kept", [] {
    const char* hex =
        "08012100050000000c0021a1c43c00009d069da111d6fe8418060a614131c2e58c98772098d52acf796849b4aef951856ba5070bc555"
        "00000000000000000e000c80000000000000000000000000040200000680000007000b0000000200650026905f00000023000000";
    Bytes file;
    for (const char* p = hex; *p != '\0'; p += 2) file.push_back(static_cast<std::uint8_t>(std::stoi(std::string(p, 2), nullptr, 16)));
    const aout::Image image = aout::read(file);
    const ObjectFile& o = image.object;
    CB_CHECK_EQ(o.seg.at("_TEXT").size(), 33u);
    CB_CHECK_EQ(o.seg.at("_DATA").size(), 5u);
    CB_CHECK_EQ(o.fix.size(), 7u);
    CB_CHECK_EQ(o.fix[0].offset, 8u);
    CB_CHECK((o.fix[0].target == Target{TargetKind::Segment, "?"}));
    CB_CHECK_EQ(o.fix[2].target.name, "_DATA");
    CB_CHECK_EQ(o.fix[3].target.name, "_BSS");
    CB_CHECK(o.fix[4].kind == FixKind::Rel);
    CB_CHECK_EQ(o.fix[4].displacement, 34);
});

CB_TEST("exact.aout.damaged_input", [] {
    const Bytes file = small_aout();
    // every prefix shorter than the whole file is a read past the end
    for (std::size_t n = 0; n < file.size(); ++n)
        CB_CHECK_THROWS_AS(TruncatedError, (void)aout::read(ByteView(file).first(n)));
    // a relocation naming a symbol the table does not have
    Bytes bad = file;
    bad[16 + 8 + 4 + 1] = 0x80 | 0x7F;   // the call's relocation word: symbol number 0x7ff
    bad[16 + 8 + 4] = 0xF9;
    CB_CHECK_THROWS_AS(FormatError, (void)aout::read(bad));
});
