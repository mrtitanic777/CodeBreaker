// Tests of cb/formats/detect.hpp on the real modules in testdata/ and on small made-up files for the magic numbers.
//
// DIVERGES from the Python detector: the Bink codec letter in `detail` is shown as the letter when it is printable ASCII and
// as \xNN otherwise (the Python showed any printable Latin-1 character); a corrupt header whose machine field is cut short
// reads as machine 0 (the Python raised struct.error).
#include "cb/formats/detect.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace detect = cb::formats::detect;

namespace {

std::filesystem::path data(const char* name) { return testing::data_path(std::string("formats/testdata/") + name); }

void put(Bytes& b, std::size_t at, std::uint64_t v, int width) {
    if (b.size() < at + static_cast<std::size_t>(width)) b.resize(at + static_cast<std::size_t>(width), 0);
    for (int k = 0; k < width; ++k) b[at + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(v >> (8 * k));
}

Bytes with_magic(std::string_view magic, std::size_t size = 0x400) {
    Bytes b(size, 0);
    std::copy(magic.begin(), magic.end(), b.begin());
    return b;
}

// the single thing a file is, or fail
detect::Detected only(const Bytes& file, std::string_view name = "x.bin") {
    auto found = detect::identify(file, name);
    CB_CHECK_EQ(found.size(), std::size_t{1});
    return found.empty() ? detect::Detected{} : found[0];
}

}  // namespace

CB_TEST("formats.detect.lane_of_real_modules", [] {
    struct Case {
        const char* file;
        detect::Lane lane;
        const char* format;
    };
    const Case cases[] = {{"lz32.dll", detect::Lane::Pe, "PE"},          {"lzexpand.dll", detect::Lane::Ne, "NE"},
                          {"splitter.vxd", detect::Lane::Le, "LE"},      {"find.exe", detect::Lane::Dos, "MZ"},
                          {"xcopy32.exe", detect::Lane::Pe, "PE"}};
    for (const Case& c : cases) {
        CB_CHECK(detect::lane(data(c.file)) == c.lane);
        CB_CHECK(detect::kind(data(c.file)) == c.lane);
        CB_CHECK_EQ(detect::format_of(data(c.file)).value(), c.format);
    }
    CB_CHECK(!detect::lane(data("choice.com")).has_value());      // not MZ
    CB_CHECK(detect::kind(data("choice.com")) == detect::Lane::Dos);  // ... but a DOS program by its name and size
    CB_CHECK(!detect::kind(data("fastadd.obj")).has_value());
    CB_CHECK(!detect::lane(data("does-not-exist.exe")).has_value());
    CB_CHECK(!detect::format_of(testing::data_path("formats")).has_value());  // a directory
    CB_CHECK(detect::holds_drivers(data("splitter.vxd")));
    CB_CHECK(!detect::holds_drivers(data("find.exe")));
    CB_CHECK(!detect::holds_drivers(data("lz32.dll")));
    CB_CHECK(detect::flat(detect::Lane::Pe) && !detect::flat(detect::Lane::Ne));
    CB_CHECK(detect::wide_resources(detect::Lane::Pe) && !detect::wide_resources(std::nullopt));
});

CB_TEST("formats.detect.new_header_from_bytes", [] {
    Bytes mz(0x80, 0);
    mz[0] = 'M', mz[1] = 'Z';
    put(mz, 0x3C, 0x40, 4);
    CB_CHECK(detect::new_header(mz) == detect::NewHeader::None);  // a plain DOS program
    for (const auto& [sig, expected] : {std::pair{"PE", detect::NewHeader::Pe}, std::pair{"NE", detect::NewHeader::Ne},
                                        std::pair{"LE", detect::NewHeader::Le}, std::pair{"LX", detect::NewHeader::Lx},
                                        std::pair{"W3", detect::NewHeader::W3}, std::pair{"W4", detect::NewHeader::W4}}) {
        mz[0x40] = static_cast<std::uint8_t>(sig[0]), mz[0x41] = static_cast<std::uint8_t>(sig[1]);
        CB_CHECK(detect::new_header(mz) == expected);
        CB_CHECK_EQ(std::string(detect::signature_text(expected)), sig);
    }
    CB_CHECK(!detect::new_header(Bytes{'M', 'Z'}).has_value());          // too short
    CB_CHECK(!detect::new_header(Bytes(0x80, 0x41)).has_value());        // not MZ
    put(mz, 0x3C, 0xFFFFFFF0u, 4);                                         // a new-header offset past the end
    CB_CHECK(detect::new_header(mz) == detect::NewHeader::None);
    CB_CHECK(detect::lane_of(detect::NewHeader::W3) == detect::Lane::Dos);
    CB_CHECK(detect::lane_of(detect::NewHeader::Lx) == detect::Lane::Le);
});

CB_TEST("formats.detect.identify_real_modules", [] {
    auto pe = detect::identify(data("lz32.dll"));
    CB_CHECK_EQ(pe.size(), std::size_t{1});
    CB_CHECK_EQ(pe[0].format, "PE executable");
    CB_CHECK_EQ(pe[0].detail, "x86");
    CB_CHECK_EQ(pe[0].family, "windows");
    CB_CHECK_EQ(pe[0].command, "info");
    CB_CHECK_EQ(detect::identify(data("lzexpand.dll"))[0].format, "16-bit Windows (NE)");
    CB_CHECK_EQ(detect::identify(data("splitter.vxd"))[0].format, "virtual device driver (LE)");
    const auto dos = detect::identify(data("find.exe"));
    CB_CHECK_EQ(dos[0].format, "DOS program (MZ)");
    CB_CHECK_EQ(dos[0].family, "dos");
    CB_CHECK_EQ(detect::identify(data("fastadd.obj"))[0].format, "OMF object");
    CB_CHECK(detect::identify(data("does-not-exist.exe")).empty());
    CB_CHECK(detect::identify(testing::data_path("formats")).empty());
});

CB_TEST("formats.detect.identify_by_magic", [] {
    CB_CHECK_EQ(only(with_magic("XEX2")).format, "Xbox 360 executable (XEX2)");
    CB_CHECK_EQ(only(with_magic("XEX2")).family, "xbox360");
    CB_CHECK_EQ(only(with_magic("XEX2")).command, "xex");
    CB_CHECK_EQ(only(with_magic("XBEH")).family, "xbox");
    CB_CHECK_EQ(only(with_magic("MSCF")).format, "Microsoft cabinet (CAB)");
    CB_CHECK_EQ(only(with_magic("\x7f" "ELF")).family, "unix");
    CB_CHECK_EQ(only(with_magic("\x89PNG")).format, "PNG image");
    CB_CHECK_EQ(only(with_magic("PK\x03\x04")).family, "archive");
    CB_CHECK_EQ(only(with_magic("OggS")).family, "media");
    CB_CHECK_EQ(only(with_magic("BIKi")).detail, "codec 'i'");
    CB_CHECK_EQ(only(with_magic(std::string_view("BIK\x01", 4))).detail, "codec '\\x01'");
    // Java class file vs Mach-O universal binary: told by the number after the magic
    Bytes fat = with_magic("\xca\xfe\xba\xbe");
    put(fat, 4, 0x02000000u, 4);  // big-endian 2
    CB_CHECK_EQ(only(fat).format, "Mach-O universal binary");
    CB_CHECK_EQ(only(fat).detail, "2 architectures");
    Bytes java = with_magic("\xca\xfe\xba\xbe");
    put(java, 4, 0x34000000u, 4);  // major version 52
    CB_CHECK_EQ(only(java).format, "Java class file");
    // an XDVDFS volume descriptor
    Bytes disc(0x10000 + 0x40, 0);
    std::string_view vol = "MICROSOFT*XBOX*MEDIA";
    std::copy(vol.begin(), vol.end(), disc.begin() + 0x10000);
    CB_CHECK_EQ(only(disc).format, "Xbox disc image (XDVDFS)");
    CB_CHECK_EQ(only(disc).detail, "volume at 0x0");
    // ISO-9660 with 2048-byte sectors
    Bytes iso(18 * 2048, 0);
    std::string_view cd = "CD001";
    std::copy(cd.begin(), cd.end(), iso.begin() + 16 * 2048 + 1);
    CB_CHECK_EQ(only(iso).format, "CD image (ISO-9660)");
    CB_CHECK_EQ(only(iso).detail, "2048-byte sectors");
    // STFS packages carry their content type at 0x344
    Bytes stfs = with_magic("LIVE", 0x400);
    put(stfs, 0x344, 0x00400000u, 4);  // big-endian 0x4000
    CB_CHECK_EQ(only(stfs).format, "STFS package (LIVE)");
    CB_CHECK_EQ(only(stfs).detail, "Xbox 360 title");
    // a FAT boot sector and a partition table
    Bytes boot(512, 0);
    boot[0] = 0xEB, boot[1] = 0x3C, boot[2] = 0x90;
    put(boot, 11, 512, 2);
    boot[510] = 0x55, boot[511] = 0xAA;
    CB_CHECK_EQ(only(boot).format, "disk image (FAT)");
    CB_CHECK_EQ(only(boot).detail, "floppy image");
    // nothing known
    CB_CHECK(detect::identify(Bytes(64, 0x41), "x.bin").empty());
    CB_CHECK(detect::identify(Bytes(), "empty").empty());
    // two names that are told by the extension alone
    CB_CHECK_EQ(only(Bytes(64, 0x41), "game.ZAR").format, "ZArchive (ZAR)");
    CB_CHECK_EQ(only(Bytes(64, 0x41), "x.pck").family, "xbox360");
});

CB_TEST("formats.detect.pe_machine_and_damaged_headers", [] {
    Bytes pe(0x200, 0);
    pe[0] = 'M', pe[1] = 'Z';
    put(pe, 0x3C, 0x80, 4);
    pe[0x80] = 'P', pe[0x81] = 'E';
    put(pe, 0x84, 0x01F2, 2);  // PowerPC big-endian: an Xbox 360 module
    CB_CHECK_EQ(only(pe).detail, "PowerPC big-endian (Xbox 360)");
    CB_CHECK_EQ(only(pe).family, "xbox360");
    put(pe, 0x84, 0x1234, 2);
    CB_CHECK_EQ(only(pe).detail, "machine 0x1234");
    pe.resize(0x85);  // the machine field is cut short
    CB_CHECK_EQ(only(pe).detail, "machine 0x0000");
    Bytes w3(0x80, 0);
    w3[0] = 'M', w3[1] = 'Z';
    put(w3, 0x3C, 0x40, 4);
    w3[0x40] = 'W', w3[0x41] = '3';
    CB_CHECK_EQ(only(w3).format, "driver bundle (W3)");
});

CB_TEST("formats.detect.dos_header", [] {
    const Bytes d = fs::read_file(data("setver.exe"));
    const auto h = detect::dos_header(d);
    CB_CHECK(h.has_value());
    CB_CHECK_EQ(h->relocations, 6);
    CB_CHECK_EQ(h->relocation_entries.size(), std::size_t{6});
    CB_CHECK_EQ(h->header_size, std::uint32_t{h->header_paragraphs} * 16);
    CB_CHECK(h->load_size > 0 && h->load_size <= d.size());
    // a made-up header with three relocations; one lies past the end of the file
    Bytes m(0x20, 0);
    m[0] = 'M', m[1] = 'Z';
    put(m, 2, 0x80, 2), put(m, 4, 1, 2), put(m, 6, 3, 2), put(m, 8, 2, 2), put(m, 0x18, 0x1C, 2);
    put(m, 0x1C, 0x0010, 2), put(m, 0x1E, 0x0002, 2);  // offset 0x10 in segment 2
    const auto small = detect::dos_header(m);
    CB_CHECK(small.has_value());
    CB_CHECK_EQ(small->relocation_entries.size(), std::size_t{1});  // the others are outside the data
    CB_CHECK_EQ(small->relocation_entries[0].segment, 2);
    CB_CHECK_EQ(small->relocation_entries[0].offset, 0x10);
    CB_CHECK(!detect::dos_header(Bytes{'M', 'Z'}).has_value());
});
