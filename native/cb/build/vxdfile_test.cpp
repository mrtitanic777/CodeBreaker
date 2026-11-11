#include "cb/build/vxdfile.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build;

void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
}
std::uint32_t get32(const Bytes& b, std::size_t at) {
    return b[at] | (b[at + 1] << 8) | (b[at + 2] << 16) | (std::uint32_t{b[at + 3]} << 24);
}

Bytes name_table(const std::vector<std::pair<std::string, unsigned>>& names, bool terminate = true) {
    Bytes t;
    for (const auto& [name, ordinal] : names) {
        t.push_back(static_cast<std::uint8_t>(name.size()));
        t.insert(t.end(), name.begin(), name.end());
        t.push_back(static_cast<std::uint8_t>(ordinal));
        t.push_back(static_cast<std::uint8_t>(ordinal >> 8));
    }
    if (terminate) t.push_back(0);
    return t;
}

// An MZ stub, an LE header at `lfa` (0xC0 bytes of header and tables), pages, and the non-resident names.
Bytes make_vxd(std::size_t lfa, const Bytes& pages, const Bytes& names, bool with_names = true) {
    Bytes b(lfa + 0xC0, 0);
    b[0] = 'M';
    b[1] = 'Z';
    put32(b, 0x3C, static_cast<std::uint32_t>(lfa));
    b[lfa] = 'L';
    b[lfa + 1] = 'E';
    const auto pages_from = static_cast<std::uint32_t>(b.size());
    put32(b, lfa + 0x80, pages_from);
    put32(b, lfa + 0x88, with_names ? pages_from + static_cast<std::uint32_t>(pages.size()) : 0);
    put32(b, lfa + 0x8C, with_names ? static_cast<std::uint32_t>(names.size()) : 0);
    b.insert(b.end(), pages.begin(), pages.end());
    if (with_names) b.insert(b.end(), names.begin(), names.end());
    return b;
}

}  // namespace

CB_TEST("build.vxdfile.definitions_from_the_name_table", [] {
    const Bytes vxd = make_vxd(0x80, Bytes(300, 0xAA),
                               name_table({{"VKD Virtual Keyboard Driver", 0}, {"VKD_Control", 1}, {"VKD_API_Proc", 2}}));
    const std::string def = vxdfile::definitions(vxd, 0x80);
    CB_CHECK_EQ(def,
                "DESCRIPTION 'VKD Virtual Keyboard Driver'\n"
                "EXPORTS\n"
                "    VKD_Control @1\n"
                "    VKD_API_Proc @2\n");
    // a quote in the description is doubled, as the .def syntax has it; names are Latin-1
    const Bytes quoted = make_vxd(0x80, {}, name_table({{"it's \xE9", 0}}));
    CB_CHECK_EQ(vxdfile::definitions(quoted, 0x80), "DESCRIPTION 'it''s \xC3\xA9'\n");
    // no exports: only the description; nothing at all: an empty text
    CB_CHECK_EQ(vxdfile::definitions(make_vxd(0x80, {}, name_table({})), 0x80), "\n");
    // a table with no terminator, ending at the last entry
    CB_CHECK_EQ(vxdfile::definitions(make_vxd(0x80, {}, name_table({{"X", 3}}, false)), 0x80), "EXPORTS\n    X @3\n");
});

CB_TEST("build.vxdfile.definitions_errors", [] {
    CB_CHECK_THROWS_AS(TruncatedError, (void)vxdfile::definitions(Bytes(0x20, 0), 0x80));             // no header there
    Bytes lies = make_vxd(0x80, {}, name_table({{"A", 1}}));
    put32(lies, 0x80 + 0x8C, 100000);                                                                   // a table past the file
    CB_CHECK_THROWS_AS(FormatError, (void)vxdfile::definitions(lies, 0x80));
    Bytes cut = make_vxd(0x80, {}, Bytes{5, 'a', 'b'});                                                // a name running off the table
    CB_CHECK_THROWS_AS(TruncatedError, (void)vxdfile::definitions(cut, 0x80));
});

CB_TEST("build.vxdfile.relay_puts_the_pages_on_a_sector_boundary", [] {
    const Bytes pages(700, 0x5A);
    const Bytes names = name_table({{"D", 0}, {"E", 1}});
    const Bytes built = make_vxd(0x80, pages, names);
    const Bytes resource{1, 2, 3, 4, 5};
    // the LE goes at file offset 0x60: header and tables (0xC0 bytes) are at 0x60..0x120, the pages start at 0x200
    const Bytes out = vxdfile::relay(built, 0x60, resource);
    const std::size_t start = 0x200 - 0x60;
    CB_CHECK_EQ(out.size(), start + pages.size() + names.size() + resource.size());
    CB_CHECK_EQ(out[0], 'L');
    CB_CHECK_EQ(get32(out, 0x80), 0x200u);
    CB_CHECK_EQ(get32(out, 0x88), 0x200u + 700u);
    CB_CHECK_EQ(get32(out, 0x8C), names.size());                         // unchanged
    CB_CHECK_EQ(get32(out, 0xB8), 0x200u + 700u + names.size());
    CB_CHECK_EQ(get32(out, 0xBC), 5u);
    for (std::size_t i = 0xC0; i < start; ++i) CB_CHECK_EQ(out[i], 0);   // padding
    CB_CHECK_EQ(out[start], 0x5A);
    CB_CHECK_EQ(out[start + 699], 0x5A);
    CB_CHECK_EQ(out[start + 700], 1);                                    // the names, then the resource
    CB_CHECK_EQ(out[out.size() - 1], 5);

    // without a resource the header's resource fields are left as they were; without names so is that pointer
    const Bytes bare = make_vxd(0x80, pages, {}, false);
    const Bytes out2 = vxdfile::relay(bare, 0, {});
    CB_CHECK_EQ(get32(out2, 0x80), 0x200u);
    CB_CHECK_EQ(get32(out2, 0x88), 0u);
    CB_CHECK_EQ(out2.size(), 0x200u + 700u);
    // a boundary already met is kept
    const Bytes out3 = vxdfile::relay(built, 0x40, {});
    CB_CHECK_EQ(get32(out3, 0x80), 0x200u);
});

CB_TEST("build.vxdfile.relay_errors", [] {
    CB_CHECK_THROWS_AS(TruncatedError, (void)vxdfile::relay(Bytes(10, 0), 0));
    Bytes backwards = make_vxd(0x80, Bytes(10, 0), name_table({{"A", 1}}));
    put32(backwards, 0x80 + 0x80, 4);                         // pages "before" the header
    CB_CHECK_THROWS_AS(FormatError, (void)vxdfile::relay(backwards, 0));
    Bytes early_names = make_vxd(0x80, Bytes(10, 0), name_table({{"A", 1}}));
    put32(early_names, 0x80 + 0x88, 0x90);                     // names before the pages
    CB_CHECK_THROWS_AS(FormatError, (void)vxdfile::relay(early_names, 0));
});
