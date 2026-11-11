// Tests of cb/symbols/wine16: the .spec lines of Wine's 16-bit modules (the shapes Wine's files have, plus the shapes
// that are not lines at all), and a Wine `dlls` folder made in a temporary folder. The expected exports are what the
// Python module read from the same text (the lines below were run through it once, all of them: the same exports, the
// same lines passed over).
//
// DIVERGES from the Python module, by design: the Python's `\s`, `\w` and `\d` were Unicode's, so a no-break space, an
// em space or an Arabic-Indic digit counted as white space or as a number; here white space and digits are ASCII (Wine's
// files are ASCII), and the bytes of a non-ASCII letter are letters of a name. An ordinal that does not fit 32 bits is
// not an export. The exports of a module are in ordinal order (the Python's dict was in the order of the lines, which nothing
// reads as an order). When two spec files of a module declare one ordinal, the later file in name order replaces the earlier
// (the Python's was the file system's listing order).
#include "cb/symbols/wine16.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace w16 = cb::symbols::wine16;
using w16::ArgKind;
using w16::Convention;

namespace {

std::vector<ArgKind> args(std::initializer_list<ArgKind> kinds) { return kinds; }

void write(const std::filesystem::path& path, std::string_view text) {
    std::filesystem::create_directories(path.parent_path());
    cb::fs::write_file(path, cb::as_bytes(text));
}

}  // namespace

CB_TEST("symbols.wine16.read_spec.the_shapes_of_a_wine_file", [] {
    const w16::Exports e = w16::read_spec(
        "33  pascal -ret16 GetClientRect(word ptr) GetClientRect16\n"
        "7    stub     IpFindFirstLine #(word str str ptr)\n"
        "88  pascal   lstrcpy(segptr str) lstrcpy16\n"
        "1 pascal X(double) X16\n");
    CB_CHECK_EQ(e.size(), std::size_t{4});
    CB_CHECK_EQ(e.at(33).name, "GetClientRect");
    CB_CHECK(e.at(33).convention == Convention::Pascal);
    CB_CHECK(e.at(33).args == args({ArgKind::Word, ArgKind::Ptr}));
    CB_CHECK(e.at(33).ret16);
    CB_CHECK(!e.at(33).stub);
    // a stub's arguments are in its comment; it is pascal
    CB_CHECK_EQ(e.at(7).name, "IpFindFirstLine");
    CB_CHECK(e.at(7).stub);
    CB_CHECK(e.at(7).convention == Convention::Pascal);
    CB_CHECK(e.at(7).args == args({ArgKind::Word, ArgKind::Str, ArgKind::Str, ArgKind::Ptr}));
    CB_CHECK(e.at(88).args == args({ArgKind::SegPtr, ArgKind::Str}));
    // a kind that is not read here (double) says nothing of the arguments
    CB_CHECK(!e.at(1).args.has_value());
    // the map is in ordinal order
    CB_CHECK_EQ(e.begin()->first, 1u);
    CB_CHECK_EQ(e.rbegin()->first, 88u);
});

CB_TEST("symbols.wine16.read_spec.conventions", [] {
    const w16::Exports e = w16::read_spec(
        "1 pascal Foo(word)\n2 cdecl Bar(long long)\r\n3 varargs Baz()\n4 stdcall Qux(s_word segstr)\n");
    CB_CHECK(e.at(1).convention == Convention::Pascal);
    CB_CHECK(e.at(2).convention == Convention::Cdecl);
    CB_CHECK(e.at(2).args == args({ArgKind::Long, ArgKind::Long}));   // the CRLF line is read
    CB_CHECK(e.at(3).convention == Convention::Varargs);
    CB_CHECK(e.at(3).args.has_value() && e.at(3).args->empty());      // none, said so: not unknown
    CB_CHECK(e.at(4).convention == Convention::Stdcall);
    CB_CHECK(e.at(4).args == args({ArgKind::SWord, ArgKind::SegStr}));
});

CB_TEST("symbols.wine16.read_spec.arguments_said_or_not", [] {
    const w16::Exports e = w16::read_spec(
        "5 pascal   Name\n"                     // nothing said
        "6 stub Name2\n"                        // a stub with no comment
        "7 stub Name3 #()\n"                    // a comment that says none
        "8 stub Name4 # (word) trailing\n"      // the comment may be set apart, and have more after it
        "9 stub N5 (word) #(long)\n"            // a stub's own parentheses are not its arguments: its comment is
        "25 pascal Foo(word) # (ptr)\n"         // and a function's comment is not
        "24 pascal Foo(word ptr\n"              // no closing parenthesis: nothing said
        "41 pascal Foo(word,ptr)\n"             // a kind that is no kind
        "42 pascal Foo(word\tptr)\n"
        "19 pascal Empty( )\n"
        "14   pascal    Spaced  (  word   ptr  )  \n");
    CB_CHECK(!e.at(5).args.has_value());
    CB_CHECK(!e.at(6).args.has_value());
    CB_CHECK(e.at(7).args.has_value() && e.at(7).args->empty());
    CB_CHECK(e.at(8).args == args({ArgKind::Word}));
    CB_CHECK(e.at(9).args == args({ArgKind::Long}));
    CB_CHECK(e.at(25).args == args({ArgKind::Word}));
    CB_CHECK(!e.at(24).args.has_value());
    CB_CHECK(!e.at(41).args.has_value());
    CB_CHECK(e.at(42).args == args({ArgKind::Word, ArgKind::Ptr}));
    CB_CHECK(e.at(19).args.has_value() && e.at(19).args->empty());
    CB_CHECK(e.at(14).args == args({ArgKind::Word, ArgKind::Ptr}));
    CB_CHECK_EQ(e.at(14).name, "Spaced");
});

CB_TEST("symbols.wine16.read_spec.flags", [] {
    const w16::Exports e = w16::read_spec(
        "10 pascal -ret16 -arch=win16 Zed(word)\n"
        "11 pascal -ret16 -noimport -arch=win32 Zed2(ptr str)\n"
        "13 pascal -arch=win32 Plain(word)\n"
        "37 pascal -ret16=1 Foo(word)\n"
        "39 stub -ret16 Foo\n"
        "40 stub -arch=win32 Foo2 #(long)\n");
    CB_CHECK(e.at(10).ret16);
    CB_CHECK_EQ(e.at(10).name, "Zed");
    CB_CHECK(e.at(11).ret16);
    CB_CHECK(e.at(11).args == args({ArgKind::Ptr, ArgKind::Str}));
    CB_CHECK(!e.at(13).ret16);
    CB_CHECK(e.at(37).ret16);
    CB_CHECK(e.at(39).ret16 && e.at(39).stub);
    CB_CHECK(!e.at(40).ret16);
    CB_CHECK(e.at(40).args == args({ArgKind::Long}));
});

CB_TEST("symbols.wine16.read_spec.lines_that_are_not_exports", [] {
    CB_CHECK(w16::read_spec("").empty());
    CB_CHECK(w16::read_spec("\n\n\n").empty());
    CB_CHECK(w16::read_spec("# comment\n; other\n").empty());
    CB_CHECK(w16::read_spec("27 invalid Foo(word)\n28 PASCAL Foo(word)\nx 29 pascal Foo()\n-1 pascal Foo()\n+1 pascal Foo()\n").empty());
    // no gap where one is needed: after the convention, after a flag, a flag with nothing after its `=`, a flag in a word
    CB_CHECK(w16::read_spec("34 pascal-ret16 Foo()\n12 pascal -ret16(word) Bad\n36 pascal -ret16= Foo()\n38 pascal -x-ret16 Foo(word)\n").empty());
    CB_CHECK(w16::read_spec("5pascal Foo()\n").empty());
    CB_CHECK(w16::read_spec("5 pascal\n").empty());
    CB_CHECK(w16::read_spec("5 pascal \n").empty());   // no name
});

CB_TEST("symbols.wine16.read_spec.ordinals", [] {
    const w16::Exports e = w16::read_spec("0 pascal Zero(word)\n007 pascal Lead(word)\n13 pascal Foo(word)\n13 pascal Over(long)\n");
    CB_CHECK_EQ(e.size(), std::size_t{3});
    CB_CHECK_EQ(e.at(0).name, "Zero");
    CB_CHECK_EQ(e.at(7).name, "Lead");
    CB_CHECK_EQ(e.at(13).name, "Over");   // the later line wins
    CB_CHECK(w16::read_spec("99999999999 pascal Big(word)\n").empty());   // (DIVERGES: more than any ordinal is)
});

CB_TEST("symbols.wine16.read_spec.spacing_and_what_follows", [] {
    const w16::Exports e = w16::read_spec(
        "30 pascal Foo()\r\n31 stub Bar #(word)\r\n\r\n"
        "44 pascal Foo()Bar\n"
        "45 pascal Baz()#(word)\n"
        "46 pascal Quux # (word)\n"
        "\t47\tcdecl\tTabbed(word)\n"
        "48 pascal Vt(word)\x0b\x0c\n");
    CB_CHECK(e.at(30).args.has_value() && e.at(30).args->empty());
    CB_CHECK(e.at(31).stub && e.at(31).args == args({ArgKind::Word}));
    CB_CHECK_EQ(e.at(44).name, "Foo");   // what follows the arguments is not read
    CB_CHECK(e.at(44).args.has_value() && e.at(44).args->empty());
    CB_CHECK(e.at(45).args.has_value() && e.at(45).args->empty());   // a function's comment is not its arguments
    CB_CHECK(!e.at(46).args.has_value());
    CB_CHECK(e.at(47).convention == Convention::Cdecl);
    CB_CHECK(e.at(48).args == args({ArgKind::Word}));
});

CB_TEST("symbols.wine16.read_spec.non_ascii_names", [] {
    // Latin-1 text as UTF-8: the bytes of a letter are letters of the name; a comment kind with one is no kind
    const w16::Exports e = w16::read_spec("20 pascal N\xC3\xA4me(word)\n22 stub Uni #(w\xC3\xB6rd)\n");
    CB_CHECK_EQ(e.at(20).name, "N\xC3\xA4me");
    CB_CHECK(e.at(20).args == args({ArgKind::Word}));
    CB_CHECK(!e.at(22).args.has_value());
    // DIVERGES: a no-break space is no white space, and an Arabic-Indic digit no ordinal
    CB_CHECK(w16::read_spec("\xC2\xA0" "49 pascal Foo()\n").empty());
    CB_CHECK(w16::read_spec("\xD9\xA3 pascal ArabicDigit(word)\n").empty());
});

CB_TEST("symbols.wine16.kinds", [] {
    CB_CHECK_EQ(w16::width(ArgKind::Word), 2u);
    CB_CHECK_EQ(w16::width(ArgKind::SWord), 2u);
    for (const ArgKind k : {ArgKind::Long, ArgKind::SegPtr, ArgKind::SegStr, ArgKind::Ptr, ArgKind::Str}) CB_CHECK_EQ(w16::width(k), 4u);
    CB_CHECK(w16::is_pointer(ArgKind::Ptr) && w16::is_pointer(ArgKind::Str) && w16::is_pointer(ArgKind::SegPtr) && w16::is_pointer(ArgKind::SegStr));
    CB_CHECK(!w16::is_pointer(ArgKind::Long) && !w16::is_pointer(ArgKind::Word));
    CB_CHECK_EQ(w16::c_type(ArgKind::Ptr), "void __far *");
    CB_CHECK_EQ(w16::c_type(ArgKind::SegStr), "void __far *");
    CB_CHECK_EQ(w16::c_type(ArgKind::Word), "uint2");
    CB_CHECK_EQ(w16::c_type(ArgKind::SWord), "int2");
    CB_CHECK_EQ(w16::c_type(ArgKind::Long), "uint4");
    CB_CHECK_EQ(w16::spelling(ArgKind::SWord), "s_word");
    CB_CHECK_EQ(w16::spelling(ArgKind::SegStr), "segstr");
});

CB_TEST("symbols.wine16.specs.a_wine_folder", [] {
    cb::fs::ScopedTempDir tmp("wine16");
    const auto dlls = tmp.path() / "dlls";
    write(dlls / "krnl386.exe16" / "krnl386.exe16.spec", "1 pascal -ret16 InitTask()\n2 pascal GetVersion()\n3 stub X #(word)\n");
    write(dlls / "krnl386.exe16" / "other.txt", "9 pascal Never()\n");                      // not a .spec file
    write(dlls / "setupx.dll16" / "setupx.dll16.spec", "7 stub IpFindFirstLine #(word str str ptr)\n");
    write(dlls / "setupx.dll16" / "setupx2.spec", "8 pascal Other(word)\n");                // another module (SETUPX2)
    write(dlls / "user.exe16" / "user.exe16.spec", "1 pascal Old(word)\n2 pascal Two()\n");
    write(dlls / "user.exe16" / "user.spec", "1 pascal New(long)\n");                       // replaces ordinal 1 of the same module
    write(dlls / "gdi.drv16" / "gdi.spec", "5 pascal SetPixel(word word word long)\n");
    write(dlls / "x.dll16y" / "x.spec", "1 pascal NotAWineFolder()\n");                     // the folder name does not end in 16
    write(dlls / "old.dll32" / "old.spec", "1 pascal ThirtyTwo()\n");
    write(dlls / "notwine" / "foo.spec", "1 pascal Foo()\n");
    write(dlls / "empty.dll16" / "readme", "nothing\n");
    write(dlls / "loose.spec", "1 pascal Loose()\n");                                      // a file, not a module folder

    const w16::Specs wine(dlls);
    CB_CHECK_EQ(wine.exports("KERNEL").size(), std::size_t{3});   // KRNL386 is KERNEL
    CB_CHECK_EQ(wine.exports("kernel").at(1).name, "InitTask");
    CB_CHECK(wine.exports("KERNEL").at(1).ret16);
    CB_CHECK_EQ(wine.exports("krnl386").size(), std::size_t{0});  // (asked for by the module's name, not the file's)
    CB_CHECK_EQ(wine.exports("SETUPX").at(7).name, "IpFindFirstLine");
    CB_CHECK(wine.exports("SETUPX").at(7).stub);
    CB_CHECK_EQ(wine.exports("SETUPX2").at(8).name, "Other");
    CB_CHECK_EQ(wine.exports("USER").size(), std::size_t{2});
    CB_CHECK_EQ(wine.exports("USER").at(1).name, "New");          // user.spec comes after user.exe16.spec
    CB_CHECK_EQ(wine.exports("USER").at(2).name, "Two");
    CB_CHECK_EQ(wine.exports("GDI").at(5).args->size(), std::size_t{4});
    for (const char* none : {"X", "OLD", "FOO", "EMPTY", "LOOSE", "NOPE", ""}) CB_CHECK(wine.exports(none).empty());
});

CB_TEST("symbols.wine16.specs.no_folder", [] {
    CB_CHECK(w16::Specs({}).exports("KERNEL").empty());
    CB_CHECK(w16::Specs("no/such/wine/folder").exports("USER").empty());
    cb::fs::ScopedTempDir tmp("wine16");
    CB_CHECK(w16::Specs(tmp.path()).exports("USER").empty());   // a folder with nothing in it
});

CB_TEST("symbols.wine16.specs.a_spec_file_that_cannot_be_read", [] {
    cb::fs::ScopedTempDir tmp("wine16");
    std::filesystem::create_directories(tmp.path() / "dlls" / "user.exe16" / "user.spec");   // a folder where a file was
    const w16::Specs wine(tmp.path() / "dlls");
    CB_CHECK_THROWS_AS(cb::IoError, (void)wine.exports("USER"));
});
