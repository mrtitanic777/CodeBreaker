// Tests of the names written into a function's source after judging (source/readable): the cases on the
// SDK headers of this machine (a machine without them checks only the reading of calls).
#include "cb/source/readable.hpp"

#include <chrono>
#include <iostream>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/build/watcom.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace stdfs = std::filesystem;
using namespace cb;

namespace {

constexpr std::string_view kSample = R"(int4 sub_1(int4 a1)
{
  int4 iVar1;
  int4 iVar2;
  int4 uVar3;
  iVar1 = LocalAlloc(0x40,100);
  ShowWindow(a1,5);
  iVar2 = GetDC(a1);
  for (uVar3 = 0; uVar3 < 10; uVar3 = uVar3 + 1) {
    MessageBoxA(a1,"keep 0x40 in a string",0,0x24);
  }
  ReleaseDC(a1,iVar2);
  pcRam10094 = GetProcAddress(iVar1,0x1000);
  return iVar1;
})";

constexpr std::string_view kWindowProc = R"(int4 wndproc(int4 a1, uint4 a2, int4 a3, int4 a4)
{
  if (a2 == 0x10) {
    return 0;
  }
  switch (a2) {
  case 2:
    PostQuitMessage(0);
    break;
  case 0xf:
    GetClientRect(a1,iVar1);
    *(int4 *)(iVar1 + 8) = *(int4 *)(iVar1 + 8) - 4;
    wsprintfA(a4,0x1000,0x2000,*(int4 *)iVar1);
    break;
  }
  return DefWindowProcA(a1,a2,a3,a4);
})";

// The SDK of this machine: the headers of OpenWatcom and of the period SDK and DDK; null when there are none to read.
struct Machine {
    config::Config config = config::Config::load(config::current().main_checkout());
    std::unique_ptr<base::Corpus> corpus = std::make_unique<base::Corpus>(config.products());
    std::unique_ptr<symbols::prototypes::PrototypeTable> prototypes;
    std::unique_ptr<source::readable::Sdk> sdk;
    Machine() {
        const symbols::prototypes::HeaderFolders folders = symbols::prototypes::HeaderFolders::standard(
            build::watcom::OpenWatcom::from_settings(config.text("openwatcom")).root(), corpus->reference());
        if (folders.w32.empty()) return;
        prototypes = std::make_unique<symbols::prototypes::PrototypeTable>(symbols::prototypes::read_headers(folders));
        sdk = std::make_unique<source::readable::Sdk>(source::readable::Sdk::read(folders, *prototypes));
    }
};

source::readable::StringAt strings_at(std::map<std::uint32_t, std::string> table) {
    return [table = std::move(table)](std::uint32_t address) -> std::optional<std::string> {
        const auto found = table.find(address);
        if (found == table.end()) return std::nullopt;
        return found->second;
    };
}

}  // namespace

CB_TEST("source.readable.calls_are_read_with_their_arguments", [] {
    const std::string text = "x = f(a, g(b, \"c,d)\"), [1,2]);\nif (h(1)) return;";
    const std::vector<source::readable::Call> found = source::readable::calls(text);
    CB_CHECK_EQ(found.size(), std::size_t{3});   // f, g, h (if is not a call)
    CB_CHECK_EQ(found[0].function, "f");
    CB_CHECK_EQ(found[0].args.size(), std::size_t{3});   // a, g(...), [1,2]: a bracket's comma splits nothing
    CB_CHECK_EQ(found[0].args[0], "a");
    CB_CHECK_EQ(found[1].function, "g");
    CB_CHECK_EQ(found[1].args.size(), std::size_t{2});   // a string's comma and parenthesis, and a bracket's comma, split nothing
    CB_CHECK_EQ(found[1].args[0], "b");
    CB_CHECK_EQ(found[2].function, "h");
    CB_CHECK_EQ(text.substr(found[2].start, found[2].end - found[2].start), "1");
});

CB_TEST("source.readable.constants", [] {
    Machine m;
    if (!m.sdk) return;
    const source::readable::Sdk& c = *m.sdk;
    CB_CHECK_EQ(c.constant("MessageBoxA", "uType", 0x24).value_or("-"), "MB_ICONQUESTION | MB_YESNO");
    CB_CHECK_EQ(c.constant("LocalAlloc", "uFlags", 0x40).value_or("-"), "LMEM_ZEROINIT");
    CB_CHECK_EQ(c.constant("ShowWindow", "nCmdShow", 0).value_or("-"), "SW_HIDE");
    CB_CHECK_EQ(c.constant("RegOpenKeyExA", "hKey", 0x80000002).value_or("-"), "HKEY_LOCAL_MACHINE");
    CB_CHECK_EQ(c.constant("CreateWindowExA", "dwStyle", 0x50010000).value_or("-"), "WS_CHILD | WS_VISIBLE | WS_TABSTOP");
    CB_CHECK_EQ(c.constant("CreateWindowExA", "dwStyle", 0x10CF0000).value_or("-"), "WS_VISIBLE | WS_OVERLAPPEDWINDOW");
    CB_CHECK_EQ(c.constant("GetWindowLongA", "nIndex", -16).value_or("-"), "GWL_STYLE");
    CB_CHECK(!c.constant("ShowWindow", "nCmdShow", 99));   // no such value: the number stays
    CB_CHECK(!c.constant("LocalAlloc", "uBytes", 0x40));   // a size is not a flag
});

CB_TEST("source.readable.one_function", [] {
    Machine m;
    if (!m.sdk) return;
    translate::l2::Made stored("m", "extern int4 pcRam10094;");
    stored.add("sub_1", std::string(kSample));
    const auto pass = source::readable::make_pass(*m.sdk, stored, strings_at({{0x1000, "DllGetVersion"}}));
    const std::string text = pass->function("sub_1");
    for (const std::string_view wanted : {"int4 sub_1(int4 hWnd)", "hMem = LocalAlloc(LMEM_ZEROINIT,100);", "ShowWindow(hWnd,SW_SHOW);", "hdc = GetDC(hWnd);",
                                          "for (i = 0; i < 10; i = i + 1)", "MB_ICONQUESTION | MB_YESNO", "\"keep 0x40 in a string\"",
                                          "g_pfnDllGetVersion = GetProcAddress(hMem,0x1000 /* \"DllGetVersion\" */);"}) {
        CB_CHECK(text.find(wanted) != std::string::npos);
    }
    CB_CHECK(pass->constants_used());
    const std::string definitions = pass->definitions();
    CB_CHECK(definitions.find("#define MB_YESNO 0x4\n") != std::string::npos);
    CB_CHECK(definitions.find("#define LMEM_ZEROINIT 0x40\n") != std::string::npos);
    CB_CHECK(definitions.find("#define SW_SHOW 0x5\n") != std::string::npos);
    CB_CHECK(definitions.find("#define MB_ICONQUESTION 0x20\n") != std::string::npos);
    CB_CHECK(pass->rename_globals("extern int4 pcRam10094;").find("g_pfnDllGetVersion") != std::string::npos);
    CB_CHECK(source::readable::calls_hint(*m.sdk, kSample).starts_with("memory and windows: LocalAlloc, ShowWindow"));
});

CB_TEST("source.readable.comparisons_fields_and_strings", [] {
    Machine m;
    if (!m.sdk) return;
    translate::l2::Made stored("m", "");
    stored.add("w", std::string(kWindowProc));
    const auto pass = source::readable::make_pass(*m.sdk, stored, strings_at({{0x1000, "%s = %ld"}, {0x2000, "count"}}));
    const std::string text = pass->function("w");
    for (const std::string_view wanted : {"if (Msg == WM_CLOSE)", "case WM_DESTROY:", "case WM_PAINT:", "((RECT *)lpRect)->right = ((RECT *)lpRect)->right - 4;",
                                          "0x1000 /* \"%s = %ld\" */", "0x2000 /* %s: \"count\" */", "((RECT *)lpRect)->left /* %ld */"}) {
        CB_CHECK(text.find(wanted) != std::string::npos);
    }
    const std::string structures = pass->struct_definitions();
    CB_CHECK(structures.find("typedef struct RECT") != std::string::npos);
    CB_CHECK(structures.find("int4 right;") != std::string::npos);
});

// CB_READABLE_PROOF="<function text file>;<out file>" writes the pass over that function (the first of a one-function module), for a comparison with the
// recorded output on a real function; the strings of the module are not known there.
CB_TEST("source.readable.proof", [] {
    const std::optional<std::string> proof = fs::get_env("CB_READABLE_PROOF");
    if (!proof || proof->empty()) return;
    const std::vector<std::string_view> files = text::split(*proof, ';');
    Machine m;
    if (!m.sdk || files.size() != 2) return;
    const auto started = std::chrono::steady_clock::now();
    translate::l2::Made stored("m", "");
    stored.add("f", fs::read_text(fs::from_text(std::string(files[0]))));
    const auto pass = source::readable::make_pass(*m.sdk, stored, strings_at({}));
    fs::write_text(fs::from_text(std::string(files[1])), pass->function("f"));
    std::cout << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << " s\n";
});
