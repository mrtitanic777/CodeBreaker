// Tests of cb/formats/pe.hpp on real Windows 95 modules (testdata/) and on small synthetic images that reach what no small
// real module has (forwarders, delay imports, TLS, a CLR header, a Rich record, certificates, PE32+).
//
// DIVERGES from the Python reader (state/work/typed-port/_bugs/LIST.md): a trailing odd byte of a UTF-16 resource name is
// dropped (the Python replaced it with U+FFFD); a corrupt export directory whose address table is missing is an empty
// list unless it claims more than 2^26 entries.
#include "cb/formats/pe.hpp"

#include <algorithm>

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace pe = cb::formats::pe;

namespace {

Bytes testdata(const char* name) { return fs::read_file(testing::data_path(std::string("formats/testdata/") + name)); }

void put(Bytes& b, std::size_t at, std::uint64_t v, int width) {
    for (int k = 0; k < width; ++k) b[at + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(v >> (8 * k));
}
void put_str(Bytes& b, std::size_t at, std::string_view s) {
    std::copy(s.begin(), s.end(), b.begin() + static_cast<std::ptrdiff_t>(at));
}

// A PE image with one section (RVA 0x1000, file offset 0x400, 0x1000 bytes) whose data directories hold what the
// loader-facing readers look at: an export directory with a forwarder, an import and a delay-import directory, TLS, a CLR
// header, a certificate, and a Rich record in the DOS stub.
Bytes synthetic_pe(bool plus) {
    const std::size_t w = plus ? 8 : 4;
    Bytes f(0x1400 + 16, 0);
    auto rva_off = [](std::size_t rva) { return rva - 0x1000 + 0x400; };
    // DOS header and the Rich record
    f[0] = 'M', f[1] = 'Z';
    put(f, 0x3C, 0x100, 4);
    const std::uint32_t key = 0x11223344;
    put(f, 0x80, 0x536E6144u ^ key, 4);  // "DanS"
    put(f, 0x84, key, 4), put(f, 0x88, key, 4), put(f, 0x8C, key, 4);
    put(f, 0x90, ((0x00ABu << 16) | 0x1234u) ^ key, 4);
    put(f, 0x94, 5u ^ key, 4);
    put_str(f, 0x98, "Rich");
    put(f, 0x9C, key, 4);
    // PE signature, COFF header, optional header
    put_str(f, 0x100, std::string_view("PE\0\0", 4));
    put(f, 0x104, plus ? 0x8664 : 0x014C, 2);
    put(f, 0x106, 1, 2);
    put(f, 0x108, 0x5F5E1000, 4);
    put(f, 0x114, plus ? 0xF0 : 0xE0, 2);
    put(f, 0x116, 0x2002, 2);  // executable image + DLL
    const std::size_t opt = 0x118;
    put(f, opt, plus ? 0x20B : 0x10B, 2);
    put(f, opt + 2, 0x0E, 1), put(f, opt + 3, 0x14, 1);
    put(f, opt + 16, 0x1200, 4);  // entry
    if (plus) put(f, opt + 24, 0x180000000ull, 8);
    else put(f, opt + 28, 0x10000000, 4);
    put(f, opt + 32, 0x1000, 4), put(f, opt + 36, 0x200, 4);
    put(f, opt + 68, 3, 2);  // subsystem: console
    const std::size_t dd_count = opt + (plus ? 108 : 92);
    put(f, dd_count, 16, 4);
    auto dir = [&](int index, std::uint32_t rva, std::uint32_t size) {
        put(f, dd_count + 4 + 8 * static_cast<std::size_t>(index), rva, 4);
        put(f, dd_count + 8 + 8 * static_cast<std::size_t>(index), size, 4);
    };
    dir(0, 0x1000, 0x200);   // exports
    dir(1, 0x1300, 40);      // imports
    dir(4, 0x1400, 16);      // certificates (a file offset)
    dir(9, 0x1500, 4 * static_cast<std::uint32_t>(w));  // TLS
    dir(13, 0x1400, 64);     // delay imports -- at 0x1400 in the section, see below
    dir(14, 0x1600, 72);     // CLR
    // the section header
    const std::size_t sec = opt + (plus ? 0xF0 : 0xE0);
    put_str(f, sec, ".rdata");
    put(f, sec + 8, 0x1000, 4), put(f, sec + 12, 0x1000, 4), put(f, sec + 16, 0x1000, 4), put(f, sec + 20, 0x400, 4);
    put(f, sec + 36, 0x40000040, 4);
    f.resize(0x1400 + 0x1000 + 16, 0);
    // export directory
    const std::size_t ex = rva_off(0x1000);
    put(f, ex + 4, 0x1234, 4), put(f, ex + 12, 0x1100, 4), put(f, ex + 16, 1, 4), put(f, ex + 20, 2, 4), put(f, ex + 24, 1, 4);
    put(f, ex + 28, 0x1040, 4), put(f, ex + 32, 0x1050, 4), put(f, ex + 36, 0x1058, 4);
    put(f, rva_off(0x1040), 0x1200, 4), put(f, rva_off(0x1044), 0x1110, 4);  // ordinal 1: code; ordinal 2: forwarder text
    put(f, rva_off(0x1050), 0x1120, 4), put(f, rva_off(0x1058), 1, 2);       // the one name belongs to slot 1
    put_str(f, rva_off(0x1100), "SYN.DLL");
    put_str(f, rva_off(0x1110), "OTHER.Func");
    put_str(f, rva_off(0x1120), "Fwd");
    // import directory: KERNEL32 by ordinal 5 and by name Sleep (hint 7)
    const std::size_t im = rva_off(0x1300);
    put(f, im, 0x1340, 4), put(f, im + 12, 0x1380, 4), put(f, im + 16, 0x1360, 4);
    const std::uint64_t by_ordinal = plus ? (1ull << 63) | 5 : 0x80000005ull;
    put(f, rva_off(0x1340), by_ordinal, static_cast<int>(w)), put(f, rva_off(0x1340 + w), 0x1390, static_cast<int>(w));
    put(f, rva_off(0x1360), by_ordinal, static_cast<int>(w)), put(f, rva_off(0x1360 + w), 0x1390, static_cast<int>(w));
    put_str(f, rva_off(0x1380), "KERNEL32.dll");
    put(f, rva_off(0x1390), 7, 2);
    put_str(f, rva_off(0x1392), "Sleep");
    // delay import directory (RVA based): USER32 -> MessageBeep (hint 3)
    const std::size_t dl = rva_off(0x1400);
    put(f, dl, 1, 4), put(f, dl + 4, 0x1480, 4), put(f, dl + 12, 0x1460, 4), put(f, dl + 16, 0x1440, 4);
    put(f, rva_off(0x1440), 0x1490, static_cast<int>(w));
    put_str(f, rva_off(0x1480), "USER32.dll");
    put(f, rva_off(0x1490), 3, 2);
    put_str(f, rva_off(0x1492), "MessageBeep");
    // TLS
    for (std::size_t k = 0; k < 4; ++k) put(f, rva_off(0x1500 + k * w), 0x100 * (k + 1), static_cast<int>(w));
    // CLR header
    const std::size_t clr = rva_off(0x1600);
    put(f, clr, 72, 4), put(f, clr + 4, 2, 2), put(f, clr + 6, 5, 2), put(f, clr + 16, 1, 4), put(f, clr + 20, 0x06000001, 4);
    // certificate table at file offset 0x1400 (the same bytes as the delay directory: the test cares only that its
    // length, revision and type fields are read from there) -- so give it its own place at the very end
    const std::size_t cert = f.size() - 16;
    put(f, cert, 16, 4), put(f, cert + 4, 0x200, 2), put(f, cert + 6, 2, 2);
    put(f, dd_count + 4 + 8 * 4, cert, 4);
    return f;
}

}  // namespace

CB_TEST("formats.pe.lz32_headers_and_sections", [] {
    auto im = pe::Image::parse(testdata("lz32.dll"), "lz32.dll");
    const auto& h = im.header();
    CB_CHECK_EQ(im.machine_name(), "i386");
    CB_CHECK_EQ(h.timestamp, 839713624u);
    CB_CHECK_EQ(h.entry_rva, 0x1000u);
    CB_CHECK_EQ(h.image_base, 0xBFEA0000u);
    CB_CHECK(!h.is_plus);
    CB_CHECK(h.is_dll());
    CB_CHECK_EQ(im.sections().size(), std::size_t{6});
    const auto names = std::vector<std::string>{".text", ".data", ".idata", ".edata", ".rsrc", ".reloc"};
    for (std::size_t i = 0; i < names.size(); ++i) CB_CHECK_EQ(im.sections()[i].name, names[i]);
    const pe::Section& text = im.sections()[0];
    CB_CHECK_EQ(text.virtual_size, 0x46Au);
    CB_CHECK_EQ(text.virtual_address, 0x1000u);
    CB_CHECK_EQ(text.raw_size, 0x600u);
    CB_CHECK_EQ(text.raw_ptr, 0x400u);
    CB_CHECK(text.is_code());
    CB_CHECK(!im.sections()[1].is_code());
    CB_CHECK_EQ(im.code_bytes(), std::uint64_t{0x600});
    const auto code = im.code_sections();
    CB_CHECK_EQ(code.size(), std::size_t{1});
    CB_CHECK_EQ(code[0].va, std::uint64_t{0xBFEA1000});
    CB_CHECK_EQ(im.section_data(text).size(), std::size_t{0x600});
    const pe::OptionalHeader oh = im.optional_header();
    CB_CHECK_EQ(oh.magic, 0x10B);
    CB_CHECK_EQ(oh.entry_point, 0x1000u);
    CB_CHECK_EQ(oh.section_alignment, 0x1000u);
    CB_CHECK_EQ(oh.file_alignment, 0x200u);
    CB_CHECK_EQ(pe::subsystem_label(oh.subsystem), std::string("GUI"));
    CB_CHECK_EQ(im.entry_va().value(), std::uint64_t{0xBFEA1000});
    CB_CHECK(!im.is_dotnet());
    CB_CHECK(!im.rich().has_value());
});

CB_TEST("formats.pe.lz32_addressing", [] {
    auto im = pe::Image::parse(testdata("lz32.dll"));
    CB_CHECK_EQ(im.rva_to_offset(0x1000).value(), std::uint64_t{0x400});
    CB_CHECK_EQ(im.rva_to_offset(0x1010).value(), std::uint64_t{0x410});
    CB_CHECK(!im.rva_to_offset(0x200).has_value());  // headers are not in a section
    CB_CHECK_EQ(im.file_offset(0x200).value(), std::uint64_t{0x200});  // ... but they are their own file offset
    CB_CHECK(!im.file_offset(0x7000).has_value());
    auto bytes = im.read_rva(0x1000, 4);
    CB_CHECK(bytes.has_value());
    CB_CHECK(!im.read_rva(0x1000, 0x100000).has_value());
});

CB_TEST("formats.pe.lz32_exports", [] {
    auto im = pe::Image::parse(testdata("lz32.dll"));
    CB_CHECK_EQ(im.module_name(), "LZ32.dll");
    const auto exports = im.exports();
    CB_CHECK_EQ(exports.size(), std::size_t{12});
    CB_CHECK_EQ(exports[0].ordinal, std::uint64_t{1});
    CB_CHECK_EQ(exports[0].name.value(), "LZCopy");
    CB_CHECK_EQ(exports[0].rva, 0x11AAu);
    CB_CHECK_EQ(exports[11].ordinal, std::uint64_t{12});
    CB_CHECK_EQ(exports[11].name.value(), "GetExpandedNameW");
    CB_CHECK_EQ(exports[11].rva, 0x1198u);
    const pe::ExportTable table = im.export_table();
    CB_CHECK_EQ(table.module, "LZ32.dll");
    CB_CHECK_EQ(table.ordinal_base, 1u);
    CB_CHECK_EQ(table.symbols.size(), exports.size());
    for (std::size_t i = 0; i < exports.size(); ++i) {
        CB_CHECK(table.symbols[i].name == exports[i].name);
        CB_CHECK_EQ(table.symbols[i].rva, exports[i].rva);
        CB_CHECK(table.symbols[i].hint.has_value());
        CB_CHECK(!table.symbols[i].forward.has_value());
    }
});

CB_TEST("formats.pe.lz32_imports", [] {
    auto im = pe::Image::parse(testdata("lz32.dll"));
    const auto imports = im.imports();
    CB_CHECK_EQ(imports.size(), std::size_t{17});
    CB_CHECK_EQ(imports[0].slot_va, std::uint64_t{0xBFEA3070});
    CB_CHECK_EQ(imports[0].dll, "KERNEL32.dll");
    CB_CHECK_EQ(imports[0].name, "#21");  // KERNEL32 ordinal 21
    CB_CHECK_EQ(imports[9].name, "DisableThreadLibraryCalls");
    CB_CHECK_EQ(imports[9].slot_va, std::uint64_t{0xBFEA3094});
    for (std::size_t i = 1; i < imports.size(); ++i) CB_CHECK_EQ(imports[i].slot_va, imports[i - 1].slot_va + 4);
    const auto table = im.import_table(false);
    CB_CHECK_EQ(table.size(), std::size_t{1});
    CB_CHECK_EQ(table[0].module, "KERNEL32.dll");
    CB_CHECK_EQ(table[0].symbols.size(), imports.size());
    CB_CHECK_EQ(table[0].symbols[0].ordinal.value(), std::uint16_t{21});
    CB_CHECK(!table[0].symbols[0].name.has_value());
    CB_CHECK_EQ(table[0].symbols[9].name.value(), "DisableThreadLibraryCalls");
    CB_CHECK(im.import_table(true).empty());  // no delay-load directory
});

CB_TEST("formats.pe.lz32_relocations_and_resources", [] {
    auto im = pe::Image::parse(testdata("lz32.dll"));
    const auto blocks = im.reloc_blocks();
    CB_CHECK_EQ(blocks.size(), std::size_t{2});
    CB_CHECK_EQ(blocks[0].page_rva, 0x1000u);
    CB_CHECK_EQ(blocks[0].entries, 38u);
    CB_CHECK_EQ(blocks[1].page_rva, 0x2000u);
    CB_CHECK_EQ(blocks[1].entries, 2u);
    const auto relocs = im.base_relocs();
    CB_CHECK(!relocs.empty() && relocs.size() <= 40);
    CB_CHECK(std::is_sorted(relocs.begin(), relocs.end()));
    for (std::uint64_t r : relocs) CB_CHECK(r >= 0x1000 && r < 0x3000);
    const auto tree = im.resource_tree();
    CB_CHECK_EQ(tree.size(), std::size_t{1});
    CB_CHECK_EQ(tree[0].id.value(), 16u);  // RT_VERSION
    CB_CHECK_EQ(tree[0].items.size(), std::size_t{1});
    CB_CHECK(tree[0].items[0].file_offset.has_value());
    CB_CHECK(!im.tls().has_value());
    CB_CHECK(im.debug_entries().empty());
    CB_CHECK(im.certificates().empty());
});

CB_TEST("formats.pe.resources_of_a_control_panel", [] {
    auto im = pe::Image::parse(testdata("desk.cpl"));
    const auto tree = im.resource_tree();
    CB_CHECK_EQ(tree.size(), std::size_t{4});
    CB_CHECK_EQ(tree[0].id.value(), 3u);  // RT_ICON, two images
    CB_CHECK_EQ(tree[0].items.size(), std::size_t{2});
    CB_CHECK_EQ(tree[0].items[0].id.value(), 1u);
    CB_CHECK_EQ(tree[0].items[0].language, 1033u);
    CB_CHECK_EQ(tree[0].items[0].rva, 0x5160u);
    CB_CHECK_EQ(tree[0].items[0].file_offset.value(), std::uint64_t{5472});
    CB_CHECK_EQ(tree[0].items[0].size, 744u);
    CB_CHECK_EQ(tree[1].id.value(), 6u);   // RT_STRING
    CB_CHECK_EQ(tree[1].items[0].id.value(), 7u);
    CB_CHECK_EQ(tree[2].id.value(), 14u);  // RT_GROUP_ICON
    CB_CHECK_EQ(tree[3].id.value(), 16u);  // RT_VERSION
    CB_CHECK_EQ(tree[3].items[0].size, 808u);
});

CB_TEST("formats.pe.debug_directory", [] {
    auto im = pe::Image::parse(testdata("comcat.dll"));
    const auto entries = im.debug_entries();
    CB_CHECK_EQ(entries.size(), std::size_t{1});
    CB_CHECK_EQ(entries[0].type, 4u);  // IMAGE_DEBUG_TYPE_MISC
    CB_CHECK_EQ(entries[0].size, 272u);
    CB_CHECK(!im.debug_codeview().has_value());  // no CodeView record
});

CB_TEST("formats.pe.synthetic_pe32_every_directory", [] {
    auto im = pe::Image::parse(synthetic_pe(false));
    CB_CHECK_EQ(im.module_name(), "SYN.DLL");
    const auto exports = im.exports();
    CB_CHECK_EQ(exports.size(), std::size_t{2});
    CB_CHECK_EQ(exports[0].ordinal, std::uint64_t{1});
    CB_CHECK(!exports[0].name.has_value());  // by ordinal only
    CB_CHECK_EQ(exports[1].name.value(), "Fwd");
    const auto table = im.export_table();
    CB_CHECK_EQ(table.timestamp, 0x1234u);
    CB_CHECK_EQ(table.symbols.size(), std::size_t{2});
    CB_CHECK(!table.symbols[0].forward.has_value());
    CB_CHECK_EQ(table.symbols[1].forward.value(), "OTHER.Func");
    const auto imports = im.imports();
    CB_CHECK_EQ(imports.size(), std::size_t{2});
    CB_CHECK_EQ(imports[0].name, "#5");
    CB_CHECK_EQ(imports[1].name, "Sleep");
    CB_CHECK_EQ(imports[1].slot_va, std::uint64_t{0x10001364});
    const auto by_name = im.import_table(false);
    CB_CHECK_EQ(by_name[0].symbols[1].hint.value(), 7u);
    const auto delayed = im.import_table(true);
    CB_CHECK_EQ(delayed.size(), std::size_t{1});
    CB_CHECK_EQ(delayed[0].module, "USER32.dll");
    CB_CHECK_EQ(delayed[0].symbols[0].name.value(), "MessageBeep");
    CB_CHECK_EQ(delayed[0].symbols[0].hint.value(), 3u);
    const auto tls = im.tls();
    CB_CHECK(tls.has_value());
    CB_CHECK_EQ(tls->start_va, std::uint64_t{0x100});
    CB_CHECK_EQ(tls->callbacks_va, std::uint64_t{0x400});
    CB_CHECK(im.is_dotnet());
    const auto clr = im.clr();
    CB_CHECK(clr.has_value());
    CB_CHECK_EQ(clr->runtime.major, 2);
    CB_CHECK_EQ(clr->runtime.minor, 5);
    CB_CHECK_EQ(clr->entry_token, 0x06000001u);
    const auto certs = im.certificates();
    CB_CHECK_EQ(certs.size(), std::size_t{1});
    CB_CHECK_EQ(certs[0].length, 16u);
    CB_CHECK_EQ(certs[0].revision, 0x200);
    const auto rich = im.rich();
    CB_CHECK(rich.has_value());
    CB_CHECK_EQ(rich->key, 0x11223344u);
    CB_CHECK_EQ(rich->entries.size(), std::size_t{1});
    CB_CHECK_EQ(rich->entries[0].product, 0xABu);
    CB_CHECK_EQ(rich->entries[0].build, 0x1234u);
    CB_CHECK_EQ(rich->entries[0].count, 5u);
});

CB_TEST("formats.pe.synthetic_pe32_plus", [] {
    auto im = pe::Image::parse(synthetic_pe(true));
    CB_CHECK(im.header().is_plus);
    CB_CHECK_EQ(im.header().image_base, std::uint64_t{0x180000000ull});
    CB_CHECK_EQ(im.machine_name(), "x64");
    const auto imports = im.imports();
    CB_CHECK_EQ(imports.size(), std::size_t{2});
    CB_CHECK_EQ(imports[0].name, "#5");
    CB_CHECK_EQ(imports[1].slot_va, std::uint64_t{0x180001368});  // 8-byte slots
    CB_CHECK_EQ(im.import_table(false)[0].symbols[1].name.value(), "Sleep");
    CB_CHECK_EQ(im.tls()->callbacks_va, std::uint64_t{0x400});
    CB_CHECK_EQ(im.entry_va().value(), std::uint64_t{0x180001200ull});
});

CB_TEST("formats.pe.not_a_pe_is_an_error", [] {
    CB_CHECK_THROWS_AS(FormatError, pe::Image::parse(Bytes{1, 2, 3, 4}));
    CB_CHECK_THROWS_AS(FormatError, pe::Image::parse(Bytes{'M', 'Z'}));  // too short for e_lfanew
    Bytes no_pe = testdata("lz32.dll");
    no_pe[pe::Image::parse(testdata("lz32.dll")).header().e_lfanew] = 'X';  // spoil the PE signature
    CB_CHECK_THROWS_AS(FormatError, pe::Image::parse(no_pe));
    CB_CHECK(!pe::Image::try_parse(Bytes{'M', 'Z'}).has_value());
    CB_CHECK(!pe::Image::try_parse(testdata("lzexpand.dll")).has_value());  // an NE module
    CB_CHECK(!pe::is_dotnet_file(testing::data_path("formats/testdata/lz32.dll")));
    CB_CHECK(!pe::is_dotnet_file(testing::data_path("formats/testdata/no-such-file.dll")));
});

// Every prefix of a real module either fails to parse with a FormatError or parses, and then every inspector reader gives a
// (possibly shorter) answer without throwing.
CB_TEST("formats.pe.truncation_never_crashes_and_inspectors_never_throw", [] {
    const Bytes whole = testdata("lz32.dll");
    for (std::size_t n = 0; n <= whole.size(); n += 61) {
        const Bytes prefix(whole.begin(), whole.begin() + static_cast<std::ptrdiff_t>(n));
        auto im = pe::Image::try_parse(prefix);
        if (!im) continue;
        (void)im->export_table();
        (void)im->import_table(false);
        (void)im->import_table(true);
        (void)im->resource_tree();
        (void)im->reloc_blocks();
        (void)im->tls();
        (void)im->debug_entries();
        (void)im->certificates();
        (void)im->clr();
        (void)im->rich();
        (void)im->optional_header();
    }
});

// Damage in the data directories: the loader-facing readers report FormatError, never read outside the data.
CB_TEST("formats.pe.damaged_tables_are_format_errors", [] {
    Bytes damaged = testdata("lz32.dll");
    // the export directory's address-table RVA points outside the image: nothing to export, not an error
    auto im = pe::Image::parse(damaged);
    const std::uint64_t edata = *im.rva_to_offset(0x4000);
    Bytes bad = damaged;
    for (int k = 0; k < 4; ++k) bad[edata + 28 + static_cast<std::size_t>(k)] = 0x7F;  // address table RVA 0x7F7F7F7F
    CB_CHECK(pe::Image::parse(bad).exports().empty());
    // ... but a name table running off the end of the file is
    bad = damaged;
    bad[edata + 24] = 0xFF, bad[edata + 25] = 0xFF, bad[edata + 26] = 0xFF;  // 16M names
    CB_CHECK_THROWS_AS(FormatError, pe::Image::parse(bad).exports());
    // a file cut inside the import directory: the DLL name lies beyond the end
    const std::uint64_t idata = *im.rva_to_offset(0x3000);
    const Bytes cut(damaged.begin(), damaged.begin() + static_cast<std::ptrdiff_t>(idata + 0x20));
    CB_CHECK_THROWS_AS(FormatError, pe::Image::parse(cut).imports());
});
