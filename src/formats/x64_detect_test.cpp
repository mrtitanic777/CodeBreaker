// cb-unity: alone
// `info` on the PlayStation 4 / 5 and Xbox One / Series files: what formats::detect says of small files of each.
#include "core/testing.hpp"
#include "formats/detect.hpp"
#include "formats/orbis_test_image.hpp"
#include "formats/xboxone_test_image.hpp"

namespace cb::formats::detect {
namespace {

Detected first_of(const Bytes& file, const char* name) {
    const std::vector<Detected> found = identify(ByteView(file), name);
    return found.empty() ? Detected{} : found.front();
}

}  // namespace

CB_TEST("formats.detect.playstation_4_and_5_programs", [] {
    const Bytes elf = orbis_test::elf();
    const Detected ps4 = first_of(elf, "tool.elf");
    CB_CHECK_EQ(ps4.family, "ps4");
    CB_CHECK_EQ(ps4.format, "PlayStation 4 executable (ELF)");
    CB_CHECK_EQ(ps4.command, "reach");
    const Detected ps5 = first_of(elf, "tool-ps5.elf");
    CB_CHECK_EQ(ps5.family, "ps5");
    CB_CHECK_EQ(ps5.format, "PlayStation 5 executable (ELF)");
    // a program that names libc.so.6 is a Linux one
    CB_CHECK(first_of(orbis_test::elf("libc.so.6"), "linux.elf").family != "ps4");
    CB_CHECK(first_of(orbis_test::elf("libc.so.6"), "linux.elf").family != "ps5");
    // OpenOrbis's fake SELF, and a retail-typed one
    const Detected fake = first_of(orbis_test::fself(elf), "eboot.bin");
    CB_CHECK_EQ(fake.family, "ps4");
    CB_CHECK_EQ(fake.format, "PlayStation 4 SELF (fake-signed)");
    CB_CHECK_EQ(fake.command, "reach");
    const Detected retail = first_of(orbis_test::fself(elf, 4), "eboot.bin");
    CB_CHECK_EQ(retail.family, "ps4");
    CB_CHECK_EQ(retail.command, "gap");
    // a PS5 SELF and a package are recognised and not handled
    Bytes ps5_self = {0x54, 0x14, 0xF5, 0xEE};
    ps5_self.resize(0x100, 0);
    CB_CHECK_EQ(first_of(ps5_self, "eboot.bin").family, "ps5");
    CB_CHECK_EQ(first_of(ps5_self, "eboot.bin").command, "gap");
    Bytes pkg = {0x7F, 'C', 'N', 'T'};
    pkg.resize(0x100, 0);
    CB_CHECK_EQ(first_of(pkg, "game.pkg").family, "ps4");
    CB_CHECK_EQ(first_of(pkg, "game.pkg").command, "gap");
});

CB_TEST("formats.detect.xbox_one_and_series_programs", [] {
    const Detected exe = first_of(xboxone_test::pe(), "app.exe");
    CB_CHECK_EQ(exe.family, "xboxone");
    CB_CHECK_EQ(exe.command, "reach");
    CB_CHECK_EQ(first_of(xboxone_test::pe(false), "plain.exe").family, "windows");   // a Win32 program is not an app-sandbox one
    const Detected package = first_of(xboxone_test::msix(), "demo.msix");
    CB_CHECK_EQ(package.family, "xboxone");
    CB_CHECK_EQ(package.format, "Xbox One / Series MSIX package");
    CB_CHECK_EQ(package.command, "reach");
    CB_CHECK_EQ(first_of(xboxone_test::msixbundle(), "demo.msixbundle").format, "Xbox One / Series MSIX bundle");
    const Detected retail = first_of(Bytes(0x400, 0), "game.msixvc");
    CB_CHECK_EQ(retail.family, "xboxone");
    CB_CHECK_EQ(retail.command, "gap");
});

}  // namespace cb::formats::detect
