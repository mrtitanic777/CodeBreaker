#include "cb/exact/exepack.hpp"

#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::exact;

namespace {

// An EXEPACK-packed program: a 32-byte MZ header whose CS is 0 (so EXEPACK's header begins at the end of the
// header), the program's entry and stack, "RB", an unpacker's stand-in, the message, and the relocation groups.
Bytes packed(const std::vector<std::vector<std::uint16_t>>& groups, bool truncate_last = false) {
    io::ByteWriter w;
    w.u16(0x5A4D);
    for (std::uint16_t v : {std::uint16_t{0x40}, std::uint16_t{2}, std::uint16_t{0}, std::uint16_t{2}, std::uint16_t{0}, std::uint16_t{0xFFFF},
                            std::uint16_t{0}, std::uint16_t{0x100}, std::uint16_t{0}, std::uint16_t{0}, std::uint16_t{0}})
        w.u16(v);   // cblp cp crlc cparhdr(2: 32 bytes) minalloc maxalloc ss sp csum ip cs
    w.zeros(32 - w.size());
    for (std::uint16_t v : {std::uint16_t{0x0010}, std::uint16_t{0x0020}, std::uint16_t{0x0100}, std::uint16_t{0x0800}, std::uint16_t{0x0200},
                            std::uint16_t{0x0030}, std::uint16_t{0x0123}})
        w.u16(v);   // ip cs mem size sp ss dest
    w.bytes(as_bytes("RB"));
    w.zeros(2);
    w.bytes(as_bytes("\xFC\x33\xC0 unpacker "));
    w.bytes(as_bytes("Packed file is corrupt"));
    for (std::size_t g = 0; g < 16; ++g) {
        const std::vector<std::uint16_t> none;
        const auto& offsets = g < groups.size() ? groups[g] : none;
        w.u16(static_cast<std::uint16_t>(offsets.size()));
        for (std::uint16_t o : offsets) w.u16(o);
    }
    Bytes out = w.take();
    if (truncate_last) out.resize(out.size() - 34);   // (group 0 keeps its count and one of its three offsets)
    return out;
}

}  // namespace

CB_TEST("exact.exepack.reads_the_program_it_packed", [] {
    const auto p = exepack::read(packed({{0x0010, 0x0020}, {0x0004}}));
    CB_CHECK(p.has_value());
    CB_CHECK_EQ(p->ip, 0x10);
    CB_CHECK_EQ(p->cs, 0x20);
    CB_CHECK_EQ(p->mem, 0x100);
    CB_CHECK_EQ(p->size, 0x800);
    CB_CHECK_EQ(p->sp, 0x200);
    CB_CHECK_EQ(p->ss, 0x30);
    CB_CHECK_EQ(p->dest, 0x123);
    // each group is one 64K of the image: group 1's offset 4 is image address 0x10004; the listing's order is kept
    CB_CHECK((p->relocs == (std::vector<std::uint32_t>{0x10, 0x20, 0x10004})));
});

CB_TEST("exact.exepack.groups_up_to_the_last", [] {
    std::vector<std::vector<std::uint16_t>> groups(16);
    groups[15] = {0xFFFE};
    const auto p = exepack::read(packed(groups));
    CB_CHECK(p.has_value());
    CB_CHECK((p->relocs == (std::vector<std::uint32_t>{15 * 0x10000 + 0xFFFE})));
    CB_CHECK(exepack::read(packed({}))->relocs.empty());
});

CB_TEST("exact.exepack.not_packed", [] {
    CB_CHECK(!exepack::read(Bytes{}).has_value());
    CB_CHECK(!exepack::read(as_bytes("MZ")).has_value());   // shorter than the header
    Bytes not_mz = packed({});
    not_mz[0] = 'X';
    CB_CHECK(!exepack::read(not_mz).has_value());
    Bytes no_signature = packed({});
    no_signature[32 + 14] = 'X';
    no_signature[32 + 16] = 'X';
    CB_CHECK(!exepack::read(no_signature).has_value());
    // a header that points past the end of the file
    Bytes past_end = packed({});
    past_end.resize(40);
    CB_CHECK(!exepack::read(past_end).has_value());
    // "ZM" is an MZ too
    Bytes zm = packed({});
    zm[0] = 'Z';
    zm[1] = 'M';
    CB_CHECK(exepack::read(zm).has_value());
});

CB_TEST("exact.exepack.damaged_relocation_table", [] {
    CB_CHECK_THROWS_AS(TruncatedError, (void)exepack::read(packed({{1, 2, 3}}, true)));
    // without the message there is no table: the program's header alone
    Bytes no_message = packed({{1}});
    no_message.resize(32 + 18 + 4);
    const auto p = exepack::read(no_message);
    CB_CHECK(p.has_value());
    CB_CHECK(p->relocs.empty());
});
