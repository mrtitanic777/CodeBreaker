// Tests of the module-definition renderers. The expected texts are the forms a .def, the linker's export
// directives and a C header take; each golden below was read and checked line by line.
#include "cb/build/definitions.hpp"

#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build::definitions;

Export code(std::uint32_t ordinal, std::optional<std::string> name, std::uint64_t va = 0x1000) {
    Export e;
    e.ordinal = ordinal;
    e.name = std::move(name);
    e.address = va;
    return e;
}

Interface win32_dll() {
    Interface i;
    i.kind = ModuleKind::Win32;
    i.library = true;
    i.name = "LZ32";
    i.exports = {code(1, "LZStart"), code(2, "GetExpandedNameA"), code(3, std::nullopt)};
    Export data = code(4, "lzValue");
    data.kind = ExportKind::Data;
    Export fwd = code(5, "Forwarded");
    fwd.kind = ExportKind::Forward;
    fwd.forward_target = "OTHER.Target";
    i.exports.push_back(data);
    i.exports.push_back(fwd);
    i.imports = {{"KERNEL32", std::string("lstrcpyA")}, {"KERNEL32", std::uint32_t{10}}, {"KERNEL32", std::uint32_t{9}},
                 {"ADVAPI32", std::string("RegOpenKeyA")}};
    return i;
}

}  // namespace

CB_TEST("build.definitions.win32_definition_file", [] {
    const std::string def = definition_file(win32_dll(), "LZ32.DLL");
    CB_CHECK_EQ(def,
                "; lz32.dll -- module definition, read from the original's own tables.\n"
                "LIBRARY LZ32\n"
                "EXPORTS\n"
                "    LZStart                          @1\n"
                "    GetExpandedNameA                 @2\n"
                "    ordinal_3                        @3 NONAME\n"
                "    lzValue                          @4 DATA\n"
                "    Forwarded = OTHER.Target @5\n"
                "IMPORTS\n"
                "    ADVAPI32.RegOpenKeyA\n"
                "    KERNEL32.10\n"       // ordinals sort as the text they are written as: "10" before "9"
                "    KERNEL32.9\n"
                "    KERNEL32.lstrcpyA\n");
});

CB_TEST("build.definitions.win16_definition_file", [] {
    Interface i;
    i.kind = ModuleKind::Win16;
    i.library = false;
    i.name = "NOTEPAD";
    i.description = "Edit's the text";
    i.heap = 1024;
    i.stack = 0;
    Export e = code(1, "WndProc");
    e.address = cb::Far16{2, 0x30};
    e.resident = true;
    Export d = code(2, std::nullopt);
    d.kind = ExportKind::Data;
    d.address = cb::Far16{0xFE, 5};
    i.exports = {e, d};
    CB_CHECK_EQ(definition_file(i, "Notepad.EXE"),
                "; notepad.exe -- module definition, read from the original's own tables.\n"
                "NAME NOTEPAD\n"
                "DESCRIPTION 'Edit''s the text'\n"
                "EXETYPE WINDOWS\n"
                "HEAPSIZE 1024\n"
                "EXPORTS\n"
                "    WndProc                          @1 RESIDENTNAME\n"
                "    ordinal_2                        @2 NONAME        ; data\n");
    i.stack = 0x2000;
    CB_CHECK(definition_file(i, "n.exe").find("STACKSIZE 8192\n") != std::string::npos);
    // nothing to export or import: only the header lines
    Interface bare;
    bare.name = "BARE";
    CB_CHECK_EQ(definition_file(bare, "bare.exe"),
                "; bare.exe -- module definition, read from the original's own tables.\nNAME BARE\n");
});

CB_TEST("build.definitions.export_directives", [] {
    Interface i = win32_dll();
    const Directives d = export_directives(i, [](const Export& e) -> std::optional<std::string> {
        if (e.ordinal == 1) return "_LZStart@12";
        if (e.ordinal == 2) return "sub_401000";
        if (e.ordinal == 3) return "sub_402000";
        return std::nullopt;   // the data export has no symbol of its own
    });
    CB_CHECK_EQ(d.lines.size(), 3u);
    CB_CHECK_EQ(d.lines[0], "export LZStart.1='_LZStart@12'");
    CB_CHECK_EQ(d.lines[1], "export GetExpandedNameA.2='sub_401000'");
    CB_CHECK_EQ(d.lines[2], "export ordinal_3.3='sub_402000'");   // an export by ordinal gets a made-up name
    CB_CHECK_EQ(d.missing.size(), 2u);
    CB_CHECK_EQ(d.missing[0].what.ordinal, 4u);
    CB_CHECK_EQ(d.missing[0].why, "exported data is not a symbol of its own");
    CB_CHECK_EQ(d.missing[1].what.ordinal, 5u);
    CB_CHECK_EQ(d.missing[1].why, "forwarded to OTHER.Target: this linker cannot forward");

    // a function nobody reconstructed
    const Directives none = export_directives(i, [](const Export&) { return std::optional<std::string>(""); });
    CB_CHECK(none.lines.empty());
    CB_CHECK_EQ(none.missing.size(), 5u);
    CB_CHECK_EQ(none.missing[0].why, "no function was reconstructed at its address");

    // names that are not plain identifiers are quoted, a 16-bit export carries its flags
    Interface w;
    w.kind = ModuleKind::Win16;
    Export e = code(7, "?Foo@@YAXXZ");
    e.address = cb::Far16{1, 0};
    e.resident = true;
    e.shared_data = false;
    w.exports = {e};
    const Directives dw = export_directives(w, [](const Export&) { return std::optional<std::string>("foo_impl"); });
    CB_CHECK_EQ(dw.lines.at(0), "export '?Foo@@YAXXZ'.7='foo_impl' resident nodata");
});

CB_TEST("build.definitions.public_header", [] {
    const Interface i = win32_dll();
    const std::string h = public_header(i, "lz32.dll", [](const Export& e) -> std::optional<std::string> {
        if (e.ordinal == 1) return "int __stdcall LZStart(void);";
        return std::nullopt;
    });
    std::string line1 = "/* lz32_public.h -- the functions LZ32 exports, as reconstructed. Argument names and types are the\n";
    CB_CHECK(h.starts_with(line1));
    CB_CHECK(h.find("#ifndef LZ32_PUBLIC_H\n#define LZ32_PUBLIC_H\n\n") != std::string::npos);
    CB_CHECK(h.find("int __stdcall LZStart(void);") != std::string::npos);
    // the declaration is padded to 72 columns before its comment
    const std::string declaration = "int __stdcall LZStart(void);";
    CB_CHECK(h.find(declaration + std::string(72 - declaration.size(), ' ') + " /* LZStart @1 */\n") != std::string::npos);
    CB_CHECK(h.find("/* GetExpandedNameA @2: no reconstruction at its address */\n") != std::string::npos);
    CB_CHECK(h.find("/* (by ordinal only) @3: no reconstruction at its address */\n") != std::string::npos);
    CB_CHECK(h.find("/* lzValue @4: data */\n") != std::string::npos);
    CB_CHECK(h.find("/* Forwarded @5: forwarded to OTHER.Target */\n") != std::string::npos);
    CB_CHECK(h.ends_with("\n#endif\n"));
    // the include guard is the file's stem, made an identifier
    CB_CHECK(public_header(i, "my-module.v2.dll", [](const Export&) { return std::nullopt; }).find("#ifndef MY_MODULE_V2_PUBLIC_H") !=
             std::string::npos);
});

CB_TEST("build.definitions.small_types", [] {
    Export e = code(9, std::nullopt);
    CB_CHECK_EQ(e.describe(), "<export (no name) @9 code>");
    e.name = "Foo";
    e.kind = ExportKind::Forward;
    CB_CHECK_EQ(e.describe(), "<export Foo @9 forward>");
    CB_CHECK(!code(1, "").has_name());
    CB_CHECK_EQ((Import{"K", std::uint32_t{12}}.text()), "12");
    CB_CHECK_EQ((Import{"K", std::string("x")}.text()), "x");
});
