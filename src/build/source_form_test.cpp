// Tests of the source form (build/source_form.hpp): the renames, which names are free, and the folder written from a build's
// link and its noted objects -- a hand-made build of two C pieces, an assembly piece and the data, with an import by number.
#include "build/source_form.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace {

using namespace cb;
namespace sf = cb::build::source_form;

bool sf_has(std::string_view text, std::string_view part) { return text.find(part) != std::string_view::npos; }

CB_TEST("build.source_form.rename_identifiers", [] {
    const std::map<std::string, std::string> names = {{"KERNEL_18", "GlobalLock"}, {"sub_3_0488", "ReadHeader"}};
    // C: the name itself; the assembler and the linker: its decorated form
    CB_CHECK_EQ(sf::rename_identifiers("x = KERNEL_18(h); sub_3_0488();", names), std::string("x = GlobalLock(h); ReadHeader();"));
    CB_CHECK_EQ(sf::rename_identifiers("        extrn _KERNEL_18:far\n        call far ptr _KERNEL_18", names),
                std::string("        extrn _GlobalLock:far\n        call far ptr _GlobalLock"));
    CB_CHECK_EQ(sf::rename_identifiers("import _KERNEL_18 KERNEL.18", names), std::string("import _GlobalLock KERNEL.18"));
    // a longer identifier that holds one is not it
    CB_CHECK_EQ(sf::rename_identifiers("KERNEL_180 xKERNEL_18 KERNEL_18x __KERNEL_18", names), std::string("KERNEL_180 xKERNEL_18 KERNEL_18x __KERNEL_18"));
});

CB_TEST("build.source_form.free_renames", [] {
    std::set<std::string> taken;
    sf::identifiers_of("int2 GetVersion; KERNEL_18 KERNEL_3 KERNEL_4 KERNEL_5 sub_1 sub_2 OFFSET_X", taken);
    const std::map<std::string, std::string> wanted = {
        {"KERNEL_18", "GlobalLock"},    // free
        {"KERNEL_3", "getversion"},     // taken, without case (the assembler)
        {"KERNEL_4", "offset"},         // the assembler's word
        {"KERNEL_5", "_lopen"},         // free
        {"sub_1", "Twice"}, {"sub_2", "twice"},   // the second to a name already given
    };
    const std::map<std::string, std::string> got = sf::free_renames(wanted, taken);
    CB_CHECK_EQ(got.size(), std::size_t{3});
    CB_CHECK_EQ(got.at("KERNEL_18"), std::string("GlobalLock"));
    CB_CHECK_EQ(got.at("KERNEL_5"), std::string("_lopen"));
    CB_CHECK(got.contains("sub_1") != got.contains("sub_2"));
});

CB_TEST("build.source_form.written_folder", [] {
    fs::ScopedTempDir dir("cb-source-form");
    const std::filesystem::path b = dir.path();
    fs::write_text(b / "m.h", "extern unsigned long __far __stdcall KERNEL_18();\nint sub_3_0488(void);\n");
    fs::write_text(b / "stub.exe", "MZ");
    const std::vector<std::string> lnk = {"format windows dll", "name m.dll", "libpath C:/somewhere/lib286/win", "option start='_sub_3_0488'",
                                          "file data.obj", "file s03/f0000.obj", "file s03/f0001.obj", "file asm/frame03_01.obj",
                                          "option stub=stub.exe", "import _KERNEL_18 KERNEL.18", "library clibm.lib"};
    std::map<std::filesystem::path, build::functions::MadeObject> made;
    made[b / "data.obj"] = {"wcc", {"-zq", "-nc=XCODE"}, "data.c", "#include \"m.h\"\n\nchar codebreaker_data_segment = 1;\n", b};
    made[b / "s03/f0000.obj"] = {"wcc", {"-zq", "-nt=CODE3"}, "f0000.c", "#include \"m.h\"\n\nint sub_3_0488(void)\n{\n  return KERNEL_18(1);\n}\n", b / "s03"};
    made[b / "s03/f0001.obj"] = {"wcc", {"-zq", "-nt=CODE3"}, "f0001.c", "#include \"m.h\"\n\nint WEP(void)\n{\n  return sub_3_0488();\n}\n", b / "s03"};
    made[b / "asm/frame03_01.obj"] = {"wasm", {"-q", "-zcm"}, "asm/frame03_01.asm", "        extrn _KERNEL_18:far\nend\n", b};
    sf::Input in;
    in.build_folder = b;
    in.module = "ver.dll";
    in.link_lines = lnk;
    in.made = [&](const std::filesystem::path& p) -> std::optional<build::functions::MadeObject> {
        const auto it = made.find(p.lexically_normal());
        return it == made.end() ? std::nullopt : std::optional<build::functions::MadeObject>(it->second);
    };
    in.stem_function = {{"s03/f0000", "sub_3_0488"}, {"s03/f0001", "WEP"}};
    in.names = {{"KERNEL_18", "GlobalLock"}, {"sub_3_0488", "ReadHeader"}};
    in.carried = {"stub.exe"};
    const sf::SourceForm f = sf::write(in);
    CB_CHECK(f.missing.empty());
    CB_CHECK_EQ(f.c_files, std::size_t{3});
    CB_CHECK_EQ(f.asm_files, std::size_t{1});
    CB_CHECK_EQ(f.renamed.size(), std::size_t{2});
    const std::filesystem::path s = b / "source";
    CB_CHECK(sf_has(fs::read_text(s / "s03/ReadHeader.c"), "int ReadHeader(void)"));
    CB_CHECK(sf_has(fs::read_text(s / "s03/ReadHeader.c"), "return GlobalLock(1);"));
    CB_CHECK(sf_has(fs::read_text(s / "s03/WEP.c"), "return ReadHeader();"));
    CB_CHECK(sf_has(fs::read_text(s / "asm/frame03_01.asm"), "extrn _GlobalLock:far"));
    CB_CHECK(sf_has(fs::read_text(s / "m.h"), "__stdcall GlobalLock();"));
    CB_CHECK(std::filesystem::is_regular_file(s / "stub.exe"));
    const std::string link = fs::read_text(s / "m.lnk");
    CB_CHECK(!sf_has(link, "libpath"));   // (the makefile gives it, under %WATCOM%)
    CB_CHECK(sf_has(link, "file s03/ReadHeader.obj"));
    CB_CHECK(sf_has(link, "import _GlobalLock KERNEL.18"));
    CB_CHECK(sf_has(link, "option start='_ReadHeader'"));
    const std::string mk = fs::read_text(s / "makefile");
    CB_CHECK(sf_has(mk, "wlink libpath $(%WATCOM)\\lib286\\win @m.lnk"));
    CB_CHECK(sf_has(mk, "wcc -zq -nt=CODE3 -i=. -fo=s03\\ReadHeader.obj s03\\ReadHeader.c"));
    CB_CHECK(sf_has(mk, "wasm -q -zcm -fo=asm\\frame03_01.obj asm\\frame03_01.asm"));
    CB_CHECK(sf_has(mk, "wcc -zq -nc=XCODE -fo=data.obj data.c"));
    CB_CHECK(!sf_has(mk, "C:/") && !sf_has(mk, "C:\\"));
});

CB_TEST("build.source_form.sdk_parameter_names", [] {
    // DWORD WINAPI GetFileResourceSize(LPCSTR lpszFileName, LPCSTR lpszResType, LPCSTR lpszResID, DWORD FAR* lpdwFileOffset):
    // Pascal, so the last is lowest (a1, a whole far pointer); the others came out of the decompiler as halves
    const std::string text = "int4 __stdcall16far __export GETFILERESOURCESIZE(a1,a3,a4,a5,a6,a7,a8)\nvoid __far *a1;\nint2 a3;\nint2 a4;\n"
                             "int2 a5;\nint2 a6;\nint2 a7;\nint2 a8;\n{\n  return a3;\n}\n";
    const std::vector<std::pair<std::string, std::string>> declared = {
        {"LPCSTR", "lpszFileName"}, {"LPCSTR", "lpszResType"}, {"LPCSTR", "lpszResID"}, {"DWORD FAR*", "lpdwFileOffset"}};
    const std::map<std::string, std::string> got = sf::sdk_parameter_names(text, "GETFILERESOURCESIZE", declared);
    CB_CHECK_EQ(got.size(), std::size_t{7});
    CB_CHECK_EQ(got.at("a1"), std::string("lpdwFileOffset"));
    CB_CHECK_EQ(got.at("a3"), std::string("lpszResID_off"));
    CB_CHECK_EQ(got.at("a4"), std::string("lpszResID_seg"));
    CB_CHECK_EQ(got.at("a8"), std::string("lpszFileName_seg"));
    // words that do not add up to the header's: nothing
    CB_CHECK(sf::sdk_parameter_names(text, "GETFILERESOURCESIZE", {{"LPCSTR", "lpszFileName"}}).empty());
    // a C-convention function: its order is not Pascal's
    CB_CHECK(sf::sdk_parameter_names("int2 __cdecl16far F(a1)\nint2 a1;\n{\n}\n", "F", {{"int", "x"}}).empty());
    CB_CHECK_EQ(sf::sdk_words("HWND").value_or(0), 1);
    CB_CHECK_EQ(sf::sdk_words("LPOFSTRUCT").value_or(0), 2);
    CB_CHECK_EQ(sf::sdk_words("void FAR*").value_or(0), 2);
    CB_CHECK(!sf::sdk_words("struct tagX").has_value());
});

}  // namespace
