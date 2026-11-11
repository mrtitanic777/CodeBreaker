// Tests of cb/symbols/prototypes: small header folders written into a temporary directory, shaped like the real ones
// (OpenWatcom's h/nt and h/win, Microsoft's SDK and DDK). The same reader was also run over the real folders of the
// corpus (8,292 Win32 and 1,874 Win16 declarations) and gave the table the old engine gave, name for name and in the
// same order.
//
// DIVERGES from the Python module's reader: the character classes are ASCII (a byte above 0x7F in a header is not a
// word character), and a header's names are sorted as written. No header of the real folders differs by it.
#include "cb/symbols/prototypes.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/knowledge/reads.hpp"

namespace proto = cb::symbols::prototypes;

namespace {

void put(const std::filesystem::path& dir, const std::string& name, const std::string& text) {
    cb::fs::write_text(dir / name, text);
}

proto::Parameter P(std::string type, std::string name) { return {std::move(type), std::move(name)}; }

}  // namespace

CB_TEST("symbols.prototypes.reads_a_header", [] {
    cb::fs::ScopedTempDir tmp("cb-proto");
    put(tmp.path() / "nt", "winuser.h", R"(/* user functions */
#define WINUSERAPI __declspec(dllimport)
#define LONG_MACRO(a, \
                   b) 1
typedef int (WINAPI *WNDENUMPROC)(HWND, LPARAM);
WINUSERAPI HWND WINAPI GetDlgItem(HWND hDlg, int nIDDlgItem);
extern int WINAPI MessageBoxA(HWND, LPCSTR lpText, LPCSTR, UINT);   // unnamed in places
int WINAPIV wsprintfA(LPSTR, LPCSTR, ...);
VOID WINAPI NoArgs(VOID);
void WINAPI Gone(void) {}
LONG WINAPI SetIt(HWND hWnd, LONG dwNew[4]);
)");
    proto::HeaderFolders folders;
    folders.w32 = {tmp.path() / "nt"};
    const proto::Headers h = proto::read_headers(folders);
    CB_CHECK_EQ(h.w16.size(), std::size_t{0});

    const proto::Prototype* get = h.w32.find("GetDlgItem");
    CB_CHECK(get != nullptr);
    CB_CHECK_EQ(get->returns, "HWND");   // WINUSERAPI is linkage, not part of the type
    CB_CHECK_EQ(get->convention, "stdcall");
    CB_CHECK_EQ(get->marker, "WINAPI");
    CB_CHECK_EQ(get->header, "winuser.h");
    CB_CHECK(get->parameters == (std::vector<proto::Parameter>{P("HWND", "hDlg"), P("int", "nIDDlgItem")}));

    const proto::Prototype* box = h.w32.find("MessageBoxA");
    CB_CHECK(box != nullptr);
    CB_CHECK_EQ(box->returns, "int");   // `extern` is dropped
    CB_CHECK(box->parameters ==
             (std::vector<proto::Parameter>{P("HWND", ""), P("LPCSTR", "lpText"), P("LPCSTR", ""), P("UINT", "")}));

    const proto::Prototype* wsprintf = h.w32.find("wsprintfA");
    CB_CHECK(wsprintf != nullptr);
    CB_CHECK_EQ(wsprintf->convention, "cdecl");   // WINAPIV
    CB_CHECK(wsprintf->parameters == (std::vector<proto::Parameter>{P("LPSTR", ""), P("LPCSTR", ""), P("...", "")}));

    const proto::Prototype* none = h.w32.find("NoArgs");
    CB_CHECK(none != nullptr);
    CB_CHECK(none->parameters.empty());   // (VOID) is no parameters
    CB_CHECK_EQ(none->returns, "VOID");

    CB_CHECK(h.w32.find("Gone") == nullptr);          // a definition, not a declaration
    CB_CHECK(h.w32.find("WNDENUMPROC") == nullptr);   // a typedef of a function pointer
    CB_CHECK(h.w32.find("LONG_MACRO") == nullptr);    // a #define across lines

    const proto::Prototype* set = h.w32.find("SetIt");
    CB_CHECK(set != nullptr);
    CB_CHECK(set->parameters == (std::vector<proto::Parameter>{P("HWND", "hWnd"), P("LONG[4]", "dwNew")}));
});

CB_TEST("symbols.prototypes.com_declarations", [] {
    cb::fs::ScopedTempDir tmp("cb-proto");
    put(tmp.path(), "objbase.h", R"(STDAPI_(DWORD) CoBuildVersion(VOID);
WINOLEAPI CoInitialize(LPVOID pvReserved);
STDAPI_(void) CoFreeUnusedLibraries(void);
STDAPI CoGetMalloc(DWORD dwMemContext, LPMALLOC FAR* ppMalloc);
)");
    proto::HeaderFolders folders;
    folders.w32 = {tmp.path()};
    const proto::Headers h = proto::read_headers(folders);
    const proto::Prototype* v = h.w32.find("CoBuildVersion");
    CB_CHECK(v != nullptr);
    CB_CHECK_EQ(v->returns, "DWORD");
    CB_CHECK_EQ(v->marker, "STDAPI");
    CB_CHECK_EQ(v->convention, "stdcall");
    const proto::Prototype* init = h.w32.find("CoInitialize");
    CB_CHECK(init != nullptr);
    CB_CHECK_EQ(init->returns, "HRESULT");   // a COM function with no stated result returns HRESULT
    CB_CHECK_EQ(init->marker, "WINOLEAPI");
    const proto::Prototype* get = h.w32.find("CoGetMalloc");
    CB_CHECK(get != nullptr);
    CB_CHECK_EQ(get->returns, "HRESULT");
    CB_CHECK(get->parameters == (std::vector<proto::Parameter>{P("DWORD", "dwMemContext"), P("LPMALLOC FAR*", "ppMalloc")}));
    CB_CHECK(h.w32.find("CoFreeUnusedLibraries") != nullptr);
});

CB_TEST("symbols.prototypes.later_folder_names_parameters_and_adds_functions", [] {
    cb::fs::ScopedTempDir tmp("cb-proto");
    put(tmp.path() / "ow", "winuser.h", "HWND WINAPI GetDlgItem(HWND, int);\nint WINAPI Two(int, int);\n");
    put(tmp.path() / "ms", "winuser.h",
        "HWND WINAPI GetDlgItem(HWND hDlg, int nIDDlgItem);\nint WINAPI Two(int a);\nBOOL WINAPI OnlyHere(HWND hWnd);\n");
    proto::HeaderFolders folders;
    folders.w32 = {tmp.path() / "ow", tmp.path() / "ms"};
    const proto::Headers h = proto::read_headers(folders);
    CB_CHECK(h.w32.find("GetDlgItem")->parameters == (std::vector<proto::Parameter>{P("HWND", "hDlg"), P("int", "nIDDlgItem")}));
    // a different number of parameters: the first folder's declaration stands
    CB_CHECK(h.w32.find("Two")->parameters == (std::vector<proto::Parameter>{P("int", ""), P("int", "")}));
    CB_CHECK(h.w32.find("OnlyHere") != nullptr);
    // declarations come in the order the headers give them, the first folder's first
    std::vector<std::string> order;
    for (const auto& [name, p] : h.w32) order.push_back(name);
    CB_CHECK(order == (std::vector<std::string>{"GetDlgItem", "Two", "OnlyHere"}));
});

CB_TEST("symbols.prototypes.api_macros_and_windows_line_endings", [] {
    cb::fs::ScopedTempDir tmp("cb-proto");
    put(tmp.path(), "lzexpand.h", "int LZAPI LZRead(HFILE, void FAR *, int);\r\nLONG\r\nLZAPI\r\nLZSeek(HFILE, LONG, int);\r\n");
    put(tmp.path(), "win386.h", "int WINAPI Skipped(int);\n");   // the Win386 headers are not read
    put(tmp.path(), "notes.txt", "int WINAPI AlsoSkipped(int);\n");
    proto::HeaderFolders folders;
    folders.w16 = {tmp.path()};
    const proto::Headers h = proto::read_headers(folders);
    CB_CHECK_EQ(h.w16.size(), std::size_t{2});
    CB_CHECK_EQ(h.w16.find("LZRead")->convention, "stdcall");
    CB_CHECK_EQ(h.w16.find("LZRead")->marker, "LZAPI");
    CB_CHECK(h.w16.find("LZRead")->parameters == (std::vector<proto::Parameter>{P("HFILE", ""), P("void FAR *", ""), P("int", "")}));
    CB_CHECK(h.w16.find("LZSeek") != nullptr);
    CB_CHECK_EQ(h.w16.find("LZSeek")->returns, "LONG");
});

CB_TEST("symbols.prototypes.table_lookups", [] {
    cb::fs::ScopedTempDir tmp("cb-proto");
    put(tmp.path() / "w32", "winuser.h",
        "HWND WINAPI GetDlgItem(HWND hDlg, int nIDDlgItem);\nint WINAPIV wsprintfA(LPSTR, LPCSTR, ...);\nvoid WINAPI Nothing(void);\n");
    put(tmp.path() / "w16", "winuser.h", "int FAR PASCAL wsprintf(LPSTR, LPCSTR, ...);\nint WINAPI LZRead(HFILE, void FAR *, int);\n");
    proto::HeaderFolders folders;
    folders.w32 = {tmp.path() / "w32"};
    folders.w16 = {tmp.path() / "w16"};
    const proto::PrototypeTable table(proto::read_headers(folders));

    CB_CHECK(table.of("GetDlgItem") != nullptr);
    CB_CHECK(table.of("GETDLGITEM") != nullptr);   // capitals do not matter
    CB_CHECK_EQ(table.of("GETDLGITEM")->returns, "HWND");
    CB_CHECK(table.of("GetDlgItem", proto::Bits::W16) == nullptr);
    CB_CHECK(table.of("LZREAD", proto::Bits::W16) != nullptr);
    CB_CHECK(table.of("NoSuchFunction") == nullptr);

    CB_CHECK_EQ(table.spelling("GETDLGITEM"), "GetDlgItem");
    CB_CHECK_EQ(table.spelling("_WSPRINTF"), "wsprintf");   // a leading underscore is tried without
    CB_CHECK_EQ(table.spelling("GetDlgItem"), "GetDlgItem");   // not all capitals: as it is
    CB_CHECK_EQ(table.spelling("UNKNOWNNAME"), "UNKNOWNNAME");
    CB_CHECK_EQ(table.spelling(""), "");

    CB_CHECK_EQ(*table.declaration("GetDlgItem"), "HWND WINAPI GetDlgItem(HWND hDlg, int nIDDlgItem)");
    CB_CHECK_EQ(*table.declaration("GETDLGITEM"), "HWND WINAPI GetDlgItem(HWND hDlg, int nIDDlgItem)");   // spelled as the headers do
    CB_CHECK_EQ(*table.declaration("Nothing"), "void WINAPI Nothing(void)");
    CB_CHECK_EQ(*table.declaration("LZRead", proto::Bits::W16), "int WINAPI LZRead(HFILE, void FAR *, int)");
    CB_CHECK(!table.declaration("NoSuchFunction").has_value());
});

CB_TEST("symbols.prototypes.lookups_are_recorded_as_knowledge_reads", [] {
    cb::fs::ScopedTempDir tmp("cb-proto");
    put(tmp.path() / "w16", "winuser.h", "HWND WINAPI GetDlgItem(HWND, int);\n");
    proto::HeaderFolders folders;
    folders.w16 = {tmp.path() / "w16"};
    const proto::PrototypeTable table(proto::read_headers(folders));
    cb::knowledge::Recording recording;
    (void)table.of("GETDLGITEM", proto::Bits::W16);
    std::vector<std::string> keys;
    for (const auto& read : recording.reads()->sorted()) keys.push_back(read.text());
    // the name asked, then the headers' spelling of it
    CB_CHECK(keys == (std::vector<std::string>{"win16:prototype:GETDLGITEM", "win16:prototype:GetDlgItem"}));
});

CB_TEST("symbols.prototypes.missing_folders_and_stamp", [] {
    cb::fs::ScopedTempDir tmp("cb-proto");
    proto::HeaderFolders none;
    none.w32 = {tmp.path() / "does-not-exist"};
    const proto::Headers h = proto::read_headers(none);
    CB_CHECK_EQ(h.w32.size(), std::size_t{0});

    put(tmp.path() / "a", "one.h", "int WINAPI One(int);\n");
    put(tmp.path() / "a", "two.h", "int WINAPI Two(int);\n");
    proto::HeaderFolders folders;
    folders.w32 = {tmp.path() / "a"};
    const std::string stamp = proto::stamp(folders);
    CB_CHECK(stamp.starts_with(std::to_string(proto::kReaderRevision) + "|"));
    CB_CHECK(stamp.find(":2:") != std::string::npos);   // the folder holds two headers
    put(tmp.path() / "a", "three.h", "int WINAPI Three(int);\n");
    CB_CHECK(proto::stamp(folders) != stamp);

    // the standard folders are those that exist
    cb::fs::make_dirs(tmp.path() / "ref" / "Microsoft Windows 95 DDK" / "INC16");
    const proto::HeaderFolders standard = proto::HeaderFolders::standard({}, tmp.path() / "ref");
    CB_CHECK_EQ(standard.w32.size(), std::size_t{0});
    CB_CHECK_EQ(standard.w16.size(), std::size_t{1});
});
