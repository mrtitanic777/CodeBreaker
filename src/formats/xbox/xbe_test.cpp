// Tests of cb/formats/xbox/xbe.hpp. The inputs are two XBEs built for the purpose (a retail XBE with a code section that calls the kernel through
// the thunk table, and a header with no sections); the expected lines are the recorded answers for them
// (testdata/assets/make_asset_goldens), PE image included (its SHA-1), and the PE is also read back by cb/formats/pe.hpp.
//
// Behaviour pinned here: a table or name outside the file is FormatError; a section that starts
// below the image base is FormatError; an image over 1 GiB is FormatError.
#include "formats/xbox/xbe.hpp"

#include <cstring>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/testing.hpp"
#include "formats/pe.hpp"
#include "formats/xbox/asset_dump_kit.hpp"

namespace xbe_test {

using namespace cb;
using namespace cb::formats::xbox;

constexpr std::uint32_t kBase = 0x10000, kText = 0x11000, kData = 0x12000;

// The retail XBE: entry `push offset msg; call [DbgPrint]; add esp,4; xor eax,eax; ret`.
static Bytes make_xbe() {
    io::ByteWriter code;
    code.u8(0x68);
    code.u32(kData + 0x10);
    code.u8(0xFF);
    code.u8(0x15);
    code.u32(kData);
    for (const std::uint8_t b : std::initializer_list<std::uint8_t>{0x83, 0xC4, 0x04, 0x33, 0xC0, 0xC3}) code.u8(b);
    io::ByteWriter data;
    data.u32(0x80000008);
    data.u32(0);
    data.zeros(8);
    data.cstr("hello, kernel");
    Bytes hdr(0x1000, 0);
    auto put = [&](std::size_t at, std::uint32_t v) {
        for (std::size_t i = 0; i < 4; ++i) hdr[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    std::memcpy(hdr.data(), "XBEH", 4);
    put(0x104, kBase);
    put(0x108, 0x1000);
    put(0x10C, 0x3000);
    put(0x118, kBase + 0x200);
    put(0x11C, 2);
    put(0x120, kBase + 0x400);
    put(0x128, kText ^ 0xA8FC57AB);
    put(0x148, kData ^ 0x5B6D40B6);
    put(0x200 + 0x08, 0x4D530001);
    const Bytes title = text::to_utf16le("Demo");
    std::memcpy(hdr.data() + 0x20C, title.data(), title.size());
    struct S {
        std::uint32_t va, flags, name_at;
        const Bytes* body;
        std::uint32_t file_at;
    };
    const Bytes code_bytes = code.data(), data_bytes = data.data();
    const S sections[] = {{kText, 0x6, 0x480, &code_bytes, 0x1000}, {kData, 0x1, 0x488, &data_bytes, 0x2000}};
    for (std::size_t i = 0; i < 2; ++i) {
        const std::size_t at = 0x400 + 0x38 * i;
        put(at, sections[i].flags);
        put(at + 4, sections[i].va);
        put(at + 8, 0x1000);
        put(at + 12, sections[i].file_at);
        put(at + 16, static_cast<std::uint32_t>(sections[i].body->size()));
        put(at + 20, kBase + sections[i].name_at);
    }
    std::memcpy(hdr.data() + 0x480, ".text\0", 6);
    std::memcpy(hdr.data() + 0x488, ".data\0", 6);
    Bytes file = hdr;
    for (const Bytes* body : {&code_bytes, &data_bytes}) {
        Bytes page = *body;
        page.resize(0x1000, 0);
        append(file, page);
    }
    return file;
}

CB_TEST("formats.xbox.xbe.the_fixtures_read", [] {
    for (const char* name : {"xbe_small.bin", "xbe_demo.bin"}) {
        const Bytes file = fs::read_file(fs::from_text(asset_dump::fixture(name)));
        const std::string diff = asset_dump::difference(asset_dump::golden(name), asset_dump::dump_xbe(name, file));
        CB_CHECK(diff.empty());
        if (!diff.empty()) cb::testing::record_failure(__FILE__, __LINE__, std::string(name) + " differs from the recorded dump:\n" + diff);
    }
});

CB_TEST("formats.xbox.xbe.the_xbe_the_test_builds_is_the_fixture", [] {
    CB_CHECK(equal_bytes(make_xbe(), fs::read_file(fs::from_text(asset_dump::fixture("xbe_demo.bin")))));
});

CB_TEST("formats.xbox.xbe.header_certificate_and_keys", [] {
    // a header with the entry scrambled with the retail key and a certificate naming "Halo"
    const xbe::Xbe x = xbe::Xbe::parse(fs::read_file(fs::from_text(asset_dump::fixture("xbe_small.bin"))));
    CB_CHECK_EQ(x.title_name, "Halo");
    CB_CHECK_EQ(x.entry_point().address, 0x11030u);
    CB_CHECK(x.entry_point().keys == xbe::KeySet::Retail);
    CB_CHECK_EQ(x.title_id_text(), "MS-055");
    CB_CHECK_EQ(x.region_text(), "North America");
    CB_CHECK_EQ(x.media_text(), "0x0");
    CB_CHECK(x.sections().empty());

    // a debug-key scrambled entry, a Chihiro one, and one no key lands in the image
    Bytes file = fs::read_file(fs::from_text(asset_dump::fixture("xbe_small.bin")));
    auto put = [&](std::size_t at, std::uint32_t v) {
        for (std::size_t i = 0; i < 4; ++i) file[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    put(0x128, 0x11030 ^ 0x94859D4B);
    CB_CHECK(xbe::Xbe::parse(file).entry_point().keys == xbe::KeySet::Debug);
    put(0x128, 0x11030 ^ 0x40B5C16E);
    CB_CHECK(xbe::Xbe::parse(file).entry_point().keys == xbe::KeySet::Chihiro);
    put(0x128, 0x12345678);
    CB_CHECK(xbe::Xbe::parse(file).entry_point().keys == xbe::KeySet::Unknown);
    CB_CHECK_EQ(xbe::Xbe::parse(file).entry_point().address, 0x12345678u ^ 0xA8FC57ABu);   // the retail key is applied
    put(0x200 + 0xA0, 0x80000003);
    CB_CHECK_EQ(xbe::Xbe::parse(file).region_text(), "North America|Japan|Manufacturing/Debug");
    put(0x200 + 0x08, 0x00010002);   // publisher bytes 00 01 are not letters
    CB_CHECK_EQ(xbe::Xbe::parse(file).title_id_text(), "0001-002");
});

CB_TEST("formats.xbox.xbe.the_pe_image_of_the_test_xbe", [] {
    const Bytes file = make_xbe();
    const xbe::Xbe x = xbe::Xbe::parse(file);
    CB_CHECK_EQ(x.sections().size(), 2u);
    CB_CHECK_EQ(x.sections()[0].name, ".text");
    CB_CHECK_EQ(x.sections()[1].flags, 0x1u);
    const Bytes image = x.pe_image();
    const formats::pe::Image pe = formats::pe::Image::parse(image);
    // the PE: x86, the XBE's base and entry
    CB_CHECK_EQ(pe.header().machine, formats::pe::kMachineI386);
    CB_CHECK_EQ(pe.header().image_base, std::uint64_t{kBase});
    CB_CHECK_EQ(pe.entry_va().value_or(0), std::uint64_t{kText});
    CB_CHECK_EQ(pe.optional_header().subsystem, 14);   // Xbox
    // the kernel thunk table is xboxkrnl.exe's imports (ordinal 8, DbgPrint)
    bool found = false;
    for (const formats::pe::Import& i : pe.imports()) found = found || (i.dll == "xboxkrnl.exe" && i.name == "#8" && i.slot_va == kData);
    CB_CHECK(found);
    // the same bytes at the same addresses
    const auto text = pe.read_rva(kText - kBase, 6 + 5 + 2);
    CB_CHECK(text.has_value());
    CB_CHECK_EQ((*text)[0], 0x68);
    const auto hello = pe.read_rva(kData - kBase + 0x10, 13);
    CB_CHECK(hello.has_value() && as_text(*hello) == "hello, kernel");
    std::vector<std::string> names;
    for (const auto& s : pe.sections()) names.push_back(s.name);
    CB_CHECK_EQ(names.size(), 3u);
    CB_CHECK_EQ(names[0], ".text");
    CB_CHECK_EQ(names[1], ".data");
    CB_CHECK_EQ(names[2], ".cbimp");
    CB_CHECK(pe.sections()[0].is_code());
    CB_CHECK(!pe.sections()[1].is_code());
    CB_CHECK_EQ(pe.sections()[1].characteristics & 0x80000000u, 0x80000000u);   // .data is writable
});

CB_TEST("formats.xbox.xbe.to_pe_writes_the_image_once_named_by_its_contents", [] {
    fs::ScopedTempDir dir("xbe");
    fs::write_file(dir.path() / "Default.XBE", make_xbe());
    const std::filesystem::path out = xbe::to_pe(dir.path() / "Default.XBE", dir.path() / "unpacked");
    CB_CHECK_EQ(out.extension().string(), ".exe");
    CB_CHECK(out.filename().string().starts_with("default-"));
    CB_CHECK_EQ(out.filename().string().size(), std::string("default-").size() + 12 + 4);
    CB_CHECK(equal_bytes(fs::read_file(out), xbe::Xbe::parse(make_xbe()).pe_image()));
    const auto written = cb::fs::last_write_time(out);
    CB_CHECK(xbe::to_pe(dir.path() / "Default.XBE", dir.path() / "unpacked") == out);   // a second call finds it
    CB_CHECK(cb::fs::last_write_time(out) == written);
});

CB_TEST("formats.xbox.xbe.damaged_input_is_a_format_error", [] {
    const Bytes file = make_xbe();
    CB_CHECK_THROWS_AS(FormatError, (void)xbe::Xbe::parse(to_bytes("MZ not an xbe")));
    CB_CHECK_THROWS_AS(TruncatedError, (void)xbe::Xbe::parse(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)xbe::Xbe::parse(ByteView(file).first(0x150)));   // no certificate in the file
    // every prefix is an error or an XBE, and never a crash
    for (std::size_t n = 0; n < 0x500; n += 7) {
        try {
            const xbe::Xbe x = xbe::Xbe::parse(ByteView(file).first(n));
            (void)x.sections();
        } catch (const FormatError&) {
        }
    }
    auto with = [&](std::size_t at, std::uint32_t v) {
        Bytes f = file;
        for (std::size_t i = 0; i < 4; ++i) f[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
        return f;
    };
    CB_CHECK_THROWS_AS(FormatError, (void)xbe::Xbe::parse(with(0x118, 0x100)));              // the certificate below the base
    CB_CHECK_THROWS_AS(FormatError, (void)xbe::Xbe::parse(with(0x118, 0xFFFF0000)));      // and far past the file
    const xbe::Xbe many = xbe::Xbe::parse(with(0x11C, 0xFFFFFFFF));                          // 4 billion sections
    CB_CHECK_THROWS_AS(FormatError, (void)many.sections());
    const xbe::Xbe huge = xbe::Xbe::parse(with(0x10C, 0xFFFFFFF0));                          // an image that would fill memory
    CB_CHECK_THROWS_AS(FormatError, (void)huge.pe_image());
    // a section that starts too low for the PE headers to precede it
    Bytes low = file;
    for (std::size_t i = 0; i < 4; ++i) low[0x400 + 4 + i] = static_cast<std::uint8_t>(((kBase + 0x10) >> (8 * i)));
    CB_CHECK_THROWS_AS(FormatError, (void)xbe::Xbe::parse(low).pe_image());
});

CB_TEST("formats.xbox.xbe.the_command_prints_the_header_and_sections", [] {
    fs::ScopedTempDir dir("xbe");
    fs::write_file(dir.path() / "default.xbe", make_xbe());
    const cli::Cli app("codebreaker", {xbe::command()});
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xbe", "info", fs::to_text(dir.path() / "default.xbe")}), 0);
        const std::string out = cap.out();
        CB_CHECK(out.find("title        : 'Demo'") != std::string::npos);
        CB_CHECK(out.find("title id     : 0x4D530001  (MS-001)") != std::string::npos);
        CB_CHECK(out.find("entry point  : 0x00011000  (retail key)") != std::string::npos);
        CB_CHECK(out.find("sections     : 2") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xbe", "sections", fs::to_text(dir.path() / "default.xbe")}), 0);
        CB_CHECK(cap.out().find("  .text        va=0x00011000 vsize=4096     raw=0x001000 size=17       flags=0x6") != std::string::npos);
        CB_CHECK(cap.out().find("total: 2 sections") != std::string::npos);
    }
});

}  // namespace xbe_test
