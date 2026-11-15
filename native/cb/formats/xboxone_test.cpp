// cb-unity: alone
#include "cb/formats/xboxone.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xboxone_test_image.hpp"

namespace cb::formats::xboxone {

namespace xt = xboxone_test;

CB_TEST("formats.xboxone.a_loose_pe32_plus_is_read", [] {
    const Program p = parse(xt::pe());
    CB_CHECK_EQ(p.container, "PE32+ executable");
    CB_CHECK(p.app_container);
    CB_CHECK_EQ(p.member, "");
    const pe::Image image = image_of(p);
    CB_CHECK(image.header().is_plus);
    CB_CHECK_EQ(image.header().machine, 0x8664);
    CB_CHECK_EQ(image.header().entry_rva, xt::kEntry);
    const auto imports = image.imports();
    CB_CHECK_EQ(imports.size(), 1u);
    CB_CHECK_EQ(imports[0].dll, "KERNEL32.dll");
    CB_CHECK_EQ(imports[0].name, "Sleep");
    CB_CHECK_EQ(imports[0].slot_va, xt::kImageBase + xt::kIat);
    CB_CHECK(!parse(xt::pe(false)).app_container);
});

CB_TEST("formats.xboxone.a_package_gives_the_executable_its_manifest_names", [] {
    const Program p = parse(xt::msix(), "demo.msix");
    CB_CHECK_EQ(p.container, "MSIX package");
    CB_CHECK_EQ(p.member, "app.exe");
    CB_CHECK_EQ(p.identity, "Demo.App");
    CB_CHECK_EQ(p.version, "1.2.3.0");
    CB_CHECK_EQ(p.architecture, "x64");
    CB_CHECK_EQ(p.publisher, "CN=Demo");
    CB_CHECK_EQ(p.members.size(), 2u);   // app.exe and lib.dll
    CB_CHECK_EQ(p.other_members, 2u);    // the manifest and resources.pri
    CB_CHECK(p.file == xt::pe());
    // another member by name
    const Program dll = parse(xt::msix(), "demo.msix", "lib.dll");
    CB_CHECK_EQ(dll.member, "lib.dll");
    CB_CHECK(!dll.app_container);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(xt::msix(), "demo.msix", "missing.exe"));
    // the same package under the older name
    CB_CHECK_EQ(parse(xt::msix(), "demo.appx").container, "APPX package");
});

CB_TEST("formats.xboxone.a_bundle_gives_the_x64_package_inside", [] {
    const Program p = parse(xt::msixbundle(), "demo.msixbundle");
    CB_CHECK_EQ(p.container, "MSIX bundle");
    CB_CHECK_EQ(p.member, "app.exe");
    CB_CHECK_EQ(p.identity, "Demo.App");
    CB_CHECK(p.file == xt::pe());
});

CB_TEST("formats.xboxone.refuses_what_it_cannot_read", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(64, 0)));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(xt::zip({{"readme.txt", Bytes(10, 'x')}}), "demo.msix"));                    // no manifest
    CB_CHECK_THROWS_AS(FormatError, (void)parse(xt::zip({{"AppxManifest.xml", xt::text("<Package/>")}}), "demo.msix"));      // no executable
    Bytes x86 = xt::pe();
    xt::put16(x86, 0x84, 0x14C);   // an x86 image is not this
    CB_CHECK_THROWS_AS(FormatError, (void)parse(x86));
    Bytes pe32 = xt::pe();
    xt::put16(pe32, 0x98, 0x10B);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(pe32));
    // a retail package: the XVD signature at 0x200
    Bytes xvd(0x1000, 0);
    xt::put_text(xvd, 0x200, "msft-xvd");
    CB_CHECK_THROWS_AS(FormatError, (void)parse(xvd, "game.xvc"));
});

CB_TEST("formats.xboxone.damaged_files_throw_and_never_overrun", [] {
    for (const Bytes& original : {xt::pe(), xt::msix(), xt::msixbundle()}) {
        for (std::size_t n = 0; n < original.size(); n += 29) {
            try {
                (void)parse(ByteView(original).first(n), "demo.msixbundle");
            } catch (const Error&) {   // (a refusal, as it should be)
            }
        }
    }
    Bytes bad = xt::msix();
    for (std::size_t k = bad.size() - 22; k < bad.size(); ++k) bad[k] = 0xFF;   // an end record of nonsense
    try {
        (void)parse(bad, "demo.msix");
    } catch (const Error&) {
    }
});

CB_TEST("formats.xboxone.probe_takes_uwp_executables_and_packages", [] {
    fs::ScopedTempDir dir("cb_xboxone_probe");
    fs::write_file(dir.path() / "app.exe", xt::pe());
    fs::write_file(dir.path() / "plain.exe", xt::pe(false));
    fs::write_file(dir.path() / "demo.msix", xt::msix());
    fs::write_file(dir.path() / "demo.msixbundle", xt::msixbundle());
    fs::write_file(dir.path() / "empty.msix", xt::zip({{"readme.txt", Bytes(4, 1)}}));
    fs::write_file(dir.path() / "game.xvc", Bytes(0x400, 0));
    fs::write_file(dir.path() / "text.txt", Bytes(100, 'a'));
    CB_CHECK(probe(dir.path() / "app.exe"));
    CB_CHECK(!probe(dir.path() / "plain.exe"));   // (a Win32 program: not an app-sandbox one)
    CB_CHECK(probe(dir.path() / "demo.msix"));
    CB_CHECK(probe(dir.path() / "demo.msixbundle"));
    CB_CHECK(!probe(dir.path() / "empty.msix"));
    CB_CHECK(probe(dir.path() / "game.xvc"));     // (recognised, not handled)
    CB_CHECK(is_encrypted_package(dir.path() / "game.xvc"));
    CB_CHECK(!is_encrypted_package(dir.path() / "demo.msix"));
    CB_CHECK(!probe(dir.path() / "text.txt"));
    CB_CHECK(!probe(dir.path() / "missing.msix"));
});

}  // namespace cb::formats::xboxone
