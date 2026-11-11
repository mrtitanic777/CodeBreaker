#include "cb/build/functions.hpp"

#include <chrono>
#include <mutex>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::build::functions;
namespace stdfs = std::filesystem;

namespace {

// ---- stand-ins for OpenWatcom: batch files that act like the tools do on what the builds ask of them ---------------

// The compiler: the last argument is the source file. It logs its whole command line to calls.txt (in the folder it
// runs in), and answers by what the source says: FAIL_WITH_LINE is an error with a line number (the third line of the
// file, the text's first), FAIL_NO_LINE a message without one, SLOW takes five seconds, UNSET warns about variables read
// and never set; a text that is only a stand-in (it returns 0) leaves an object saying STANDIN, any other REAL -- or
// the file template.obj beside the script if there is one.
const char* const kFakeCompiler =
    "@echo off\r\n"
    "set SRC=\r\n"
    "for %%a in (%*) do set SRC=%%a\r\n"
    "set OBJ=%SRC:.c=.obj%\r\n"
    "echo %*>> calls.txt\r\n"
    "findstr /C:\"SLOW\" %SRC% >nul\r\n"
    "if not errorlevel 1 ping -n 6 127.0.0.1 >nul\r\n"
    "findstr /C:\"FAIL_WITH_LINE\" %SRC% >nul\r\n"
    "if not errorlevel 1 goto line\r\n"
    "findstr /C:\"FAIL_NO_LINE\" %SRC% >nul\r\n"
    "if not errorlevel 1 goto noline\r\n"
    "findstr /C:\"UNSET\" %SRC% >nul\r\n"
    "if not errorlevel 1 goto unset\r\n"
    "if exist %~dp0template.obj goto template\r\n"
    "findstr /C:\"return 0;\" %SRC% >nul\r\n"
    "if not errorlevel 1 goto standin\r\n"
    "echo REAL> %OBJ%\r\n"
    "exit /b 0\r\n"
    ":standin\r\n"
    "echo STANDIN> %OBJ%\r\n"
    "exit /b 0\r\n"
    ":template\r\n"
    "copy /y %~dp0template.obj %OBJ% >nul\r\n"
    "exit /b 0\r\n"
    ":line\r\n"
    "echo %SRC%(3): Error! E1002: bad thing\r\n"
    "exit /b 1\r\n"
    ":noline\r\n"
    "echo the compiler fell over\r\n"
    "exit /b 1\r\n"
    ":unset\r\n"
    "echo %SRC%(5): Warning! W200: 'in_BX' has been referenced but never assigned a value\r\n"
    "echo %SRC%(6): Warning! W200: 'in_AX' has been referenced but never assigned a value\r\n"
    "echo %SRC%(7): Warning! W200: 'in_BX' has been referenced but never assigned a value\r\n"
    "exit /b 0\r\n";

// The linker, run as `wlink @m.lnk` in the build folder. Its behaviour is set by files beside the script:
// map.txt is the map it writes; fail_near.txt makes it refuse a link in which f0001.obj is not a stand-in (a function
// that reaches near across segments); fail_64k.txt makes it refuse while any function is not a stand-in (a segment over
// 64K); undefined.txt makes it warn about an undefined reference the first time; nolink.txt makes it fail outright.
const char* const kFakeLinker =
    "@echo off\r\n"
    "echo %*>> links.txt\r\n"
    "if exist %~dp0nolink.txt goto nolink\r\n"
    "if exist %~dp0fail_near.txt if exist f0001.obj findstr /C:\"REAL\" f0001.obj >nul\r\n"
    "if exist %~dp0fail_near.txt if exist f0001.obj if not errorlevel 1 goto near\r\n"
    "if exist %~dp0fail_64k.txt findstr /C:\"REAL\" f0000.obj f0001.obj f0002.obj >nul 2>nul\r\n"
    "if exist %~dp0fail_64k.txt if not errorlevel 1 goto big\r\n"
    "echo DLL> m.dll\r\n"
    "if exist %~dp0map.txt copy /y %~dp0map.txt m.map >nul\r\n"
    "if exist %~dp0undefined.txt findstr /C:\"original.obj\" m.lnk >nul\r\n"
    "if exist %~dp0undefined.txt if errorlevel 1 echo Warning! W1028: _missing is an undefined reference\r\n"
    "exit /b 0\r\n"
    ":near\r\n"
    "echo Error! E2028: file f0001.obj(f0001.c): not in the same segment\r\n"
    "exit /b 1\r\n"
    ":big\r\n"
    "echo Error! E2038: size of segment CODE1 exceeds 64k by 100 bytes\r\n"
    "exit /b 1\r\n"
    ":nolink\r\n"
    "echo Error! E3000: cannot open something\r\n"
    "echo and another line\r\n"
    "exit /b 1\r\n";

const char* const kFakeAssembler =
    "@echo off\r\n"
    "echo %*>> assembled.txt\r\n"
    "echo ASM> original.obj\r\n"
    "exit /b 0\r\n";

struct Kit {
    fs::ScopedTempDir dir{"cbfn"};
    stdfs::path cc, wlink, wasm;
    Compiler compiler;
    fs::ScopedEnv no_tz{"TZ", std::nullopt};
    Kit() {
        cc = dir.path() / "fakecc.cmd";
        wlink = dir.path() / "fakelink.cmd";
        wasm = dir.path() / "fakeasm.cmd";
        fs::write_text(cc, kFakeCompiler);
        fs::write_text(wlink, kFakeLinker);
        fs::write_text(wasm, kFakeAssembler);
        compiler.exe = cc;
    }
    stdfs::path work(const char* name) {
        stdfs::path p = dir.path() / name;
        fs::make_dirs(p);
        return p;
    }
    void flag(const char* file) { fs::write_text(dir.path() / file, "on"); }
    int lines_of(const stdfs::path& file) {
        std::error_code ec;
        if (!stdfs::exists(file, ec)) return 0;
        return static_cast<int>(text::lines(fs::read_text(file)).size());
    }
};

// A cache in memory that counts what it is asked.
class MemoryCache : public BuildCache {
public:
    std::optional<Bytes> get(std::string_view kind, std::string_view key) override {
        std::lock_guard lock(mutex_);
        ++gets;
        const auto it = items_.find(std::string(kind) + "/" + std::string(key));
        if (it == items_.end()) return std::nullopt;
        ++hits;
        return it->second;
    }
    void put(std::string_view kind, std::string_view key, ByteView value) override {
        std::lock_guard lock(mutex_);
        items_[std::string(kind) + "/" + std::string(key)] = Bytes(value.begin(), value.end());
        ++puts;
    }
    int gets = 0, hits = 0, puts = 0;

private:
    std::mutex mutex_;
    std::map<std::string, Bytes> items_;
};

// A text of a function that the fake compiler takes, with a marker word in its body or head.
std::string fn(const std::string& name, const std::string& marker = "") {
    return "int " + name + "(void)\n{ /* " + marker + " */ return 1; }";
}

std::string native_text(const stdfs::path& p) {
    std::string out = fs::to_text(stdfs::absolute(p).lexically_normal());
    std::replace(out.begin(), out.end(), '/', '\\');
    return out;
}

// ---- OMF objects as OpenWatcom writes them ------------------------------------------------------------------------

Bytes omf_record(std::uint8_t kind, const Bytes& body, bool checksum = true) {
    Bytes r = {kind, static_cast<std::uint8_t>((body.size() + 1) & 0xFF), static_cast<std::uint8_t>((body.size() + 1) >> 8)};
    r.insert(r.end(), body.begin(), body.end());
    std::uint8_t sum = 0;
    for (std::uint8_t b : r) sum = static_cast<std::uint8_t>(sum + b);
    r.push_back(checksum ? static_cast<std::uint8_t>(0u - sum) : 0);
    return r;
}

Bytes pascal(const std::string& s) {
    Bytes b = {static_cast<std::uint8_t>(s.size())};
    b.insert(b.end(), s.begin(), s.end());
    return b;
}

// THEADR <folder>\<stem>.c, a dependency record for <folder>\m.h (with a date), some code, a module end.
Bytes make_object(const stdfs::path& folder, const std::string& stem, std::uint8_t code_byte = 0x90) {
    const std::string dir = native_text(folder);
    Bytes out;
    const auto add = [&out](const Bytes& r) { out.insert(out.end(), r.begin(), r.end()); };
    add(omf_record(0x80, pascal(dir + "\\" + stem + ".c")));
    Bytes dep = {0x80, 0xE9, 0x11, 0x11, 0x22, 0x22};
    const Bytes name = pascal(dir + "\\m.h");
    dep.insert(dep.end(), name.begin(), name.end());
    add(omf_record(0x88, dep));
    add(omf_record(0xA0, {0x01, 0x00, 0x00, code_byte, 0xC3}));   // LEDATA: segment 1, offset 0, two bytes of code
    add(omf_record(0x8A, {0x00}));
    return out;
}

struct Parsed {
    std::string theadr;
    std::string dep;
    std::uint16_t dep_time = 0, dep_date = 0;
    bool checksums_ok = true;
};

Parsed parse_object(const Bytes& obj) {
    Parsed p;
    std::size_t i = 0;
    while (i + 3 <= obj.size()) {
        const std::size_t size = obj[i + 1] | (obj[i + 2] << 8);
        std::uint8_t sum = 0;
        for (std::size_t k = i; k < i + 3 + size; ++k) sum = static_cast<std::uint8_t>(sum + obj[k]);
        if (sum != 0) p.checksums_ok = false;
        if (obj[i] == 0x80) p.theadr.assign(obj.begin() + static_cast<std::ptrdiff_t>(i) + 4, obj.begin() + static_cast<std::ptrdiff_t>(i) + 4 + obj[i + 3]);
        if (obj[i] == 0x88 && obj[i + 4] == 0xE9) {
            p.dep_time = static_cast<std::uint16_t>(obj[i + 5] | (obj[i + 6] << 8));
            p.dep_date = static_cast<std::uint16_t>(obj[i + 7] | (obj[i + 8] << 8));
            p.dep.assign(obj.begin() + static_cast<std::ptrdiff_t>(i) + 10, obj.begin() + static_cast<std::ptrdiff_t>(i) + 10 + obj[i + 9]);
        }
        i += 3 + size;
    }
    return p;
}

void set_utc_time(const stdfs::path& p, int y, unsigned m, unsigned d, int h, int min, int s) {
    using namespace std::chrono;
    const sys_seconds when = sys_days{year{y} / month{m} / day{d}} + hours{h} + minutes{min} + seconds{s};
    stdfs::last_write_time(p, clock_cast<stdfs::file_time_type::clock>(when));
}

}  // namespace

// ---- texts ------------------------------------------------------------------------------------------------------------

CB_TEST("build.functions.prototype_and_stand_in", [] {
    CB_CHECK_EQ(prototype("int f(int a, char *b)\n{ return a; }"), "int f(int a, char *b);");
    CB_CHECK_EQ(prototype("static   unsigned long\n g ( void )  {"), "static unsigned long g ( void );");
    // old-style argument lines are not part of the declaration
    CB_CHECK_EQ(prototype("int f(a, b)\nint a;\nchar *b;\n{ }"), "int f(a, b);");
    CB_CHECK_EQ(prototype("no brace or paren"), "no brace or paren;");
    CB_CHECK_EQ(stand_in("int f(int a)\n{ return a; }"), "int f(int a)\n{\n  return 0;\n}\n");
    CB_CHECK_EQ(stand_in("void g(void)\n{ x = 1; }"), "void g(void)\n{\n}\n");
    // the old-style argument lines are kept in the stand-in: a parameter they declare, left out, would be an int
    CB_CHECK_EQ(stand_in("int f(a)\nchar *a;\n{ return 1; }"), "int f(a)\nchar *a;\n{\n  return 0;\n}\n");
    CB_CHECK_EQ(plain_symbol("_NAME@8"), "NAME");
    CB_CHECK_EQ(plain_symbol("NAME_"), "NAME");
    CB_CHECK_EQ(plain_symbol("_NAME"), "NAME");
    CB_CHECK_EQ(plain_symbol("__x__@4"), "x");
    CB_CHECK_EQ(plain_symbol(""), "");
});

CB_TEST("build.functions.options_a_text_asks_for", [] {
    const std::vector<std::string> base = {"-zq", "-3"};
    CB_CHECK(adjust_flags(base, "int f(void) { return 1; }") == base);
    const auto exported = adjust_flags(base, "int __export f(void) { }");
    CB_CHECK((exported == std::vector<std::string>{"-zq", "-3", "-zW"}));
    const std::vector<std::string> already = {"-zW"};
    CB_CHECK(adjust_flags(already, "int __export f(void) { }") == already);
    const auto frame = adjust_flags(base, "int f(void) { return codebreaker_entry_sp(); }");
    CB_CHECK((frame == std::vector<std::string>{"-zq", "-3", "-of+"}));
    const auto both = adjust_flags(base, "int __export f(void) { return codebreaker_stack(0); }");
    CB_CHECK((both == std::vector<std::string>{"-zq", "-3", "-zW", "-of+"}));
    CB_CHECK(cflags16().front() == "-bt=windows");
    CB_CHECK(std::find(cflags16().begin(), cflags16().end(), "-ecd") != cflags16().end());
    CB_CHECK(cflags_powerpc().back() == "/FIm.h");
});

// ---- objects ----------------------------------------------------------------------------------------------------------

CB_TEST("build.functions.an_object_kept_from_another_folder_is_rewritten_for_this_one", [] {
    Kit k;
    const stdfs::path was = k.work("the_old_folder"), now = k.work("now");
    fs::write_text(now / "m.h", "header");
    set_utc_time(now / "m.h", 2026, 11, 11, 13, 45, 30);
    const Bytes kept = make_object(was, "f0003");
    const auto here = omf_here(kept, now);
    CB_CHECK(here.has_value());
    const Parsed p = parse_object(*here);
    CB_CHECK_EQ(p.theadr, native_text(now) + "\\f0003.c");
    CB_CHECK_EQ(p.dep, native_text(now) + "\\m.h");
    CB_CHECK_EQ(p.dep_time, (13 << 11) | (45 << 5) | 15);                    // as DOS keeps a time: seconds in twos
    CB_CHECK_EQ(p.dep_date, ((2026 - 1980) << 9) | (11 << 5) | 11);
    CB_CHECK(p.checksums_ok);
    // the same folder: nothing changes but the date of m.h
    const auto same = omf_here(make_object(now, "f0003"), now);
    CB_CHECK(same.has_value());
    CB_CHECK_EQ(parse_object(*same).theadr, native_text(now) + "\\f0003.c");
    // the two names change length with the folder's, and nothing else does
    CB_CHECK_EQ(static_cast<std::ptrdiff_t>(here->size()) - static_cast<std::ptrdiff_t>(kept.size()),
                2 * (static_cast<std::ptrdiff_t>(native_text(now).size()) - static_cast<std::ptrdiff_t>(native_text(was).size())));
});

CB_TEST("build.functions.an_object_that_cannot_be_rewritten_is_not_guessed_at", [] {
    Kit k;
    const stdfs::path was = k.work("was"), now = k.work("now");
    const Bytes kept = make_object(was, "f0000");
    // the file it depends on is not there (m.h was not written before this was asked)
    CB_CHECK(!omf_here(kept, now).has_value());
    fs::write_text(now / "m.h", "header");
    CB_CHECK(omf_here(kept, now).has_value());
    // with a time zone set the tool's dates are of that zone: it is run instead
    {
        fs::ScopedEnv tz("TZ", "PST8PDT");
        CB_CHECK(!omf_here(kept, now).has_value());
    }
    // not an object file whole
    Bytes cut = kept;
    cut.pop_back();
    CB_CHECK(!omf_here(cut, now).has_value());
    CB_CHECK(!omf_here(Bytes{}, now).has_value());
    CB_CHECK(!omf_here(Bytes{0x01, 0x02, 0x03, 0x04}, now).has_value());
    // a header that names no folder
    Bytes plain = omf_record(0x80, pascal("f0000.c"));
    const Bytes tail = omf_record(0x8A, {0});
    plain.insert(plain.end(), tail.begin(), tail.end());
    CB_CHECK(!omf_here(plain, now).has_value());
});

CB_TEST("build.functions.messages_are_kept_without_the_folder_they_name", [] {
    Kit k;
    const stdfs::path a = k.work("one"), b = k.work("two");
    const std::string message = "f0001.c(3): Error! E1002: cannot open " + native_text(a) + "\\m.h";
    const Bytes kept = message_kept(message, a);
    CB_CHECK(as_text(kept).find(native_text(a)) == std::string_view::npos);
    CB_CHECK_EQ(message_here(kept, a), message);
    CB_CHECK_EQ(message_here(kept, b), "f0001.c(3): Error! E1002: cannot open " + native_text(b) + "\\m.h");
    CB_CHECK_EQ(message_here(message_kept("no folder here", a), b), "no folder here");
});

CB_TEST("build.functions.an_objects_digest_is_what_it_is_not_where_or_when_it_was_made", [] {
    Kit k;
    const stdfs::path a = k.work("a"), b = k.work("b");
    fs::write_file(a / "x.obj", make_object(a, "f0000"));
    fs::write_file(b / "x.obj", make_object(b, "f0007"));   // another folder, another name, the same code
    CB_CHECK(object_digest(a / "x.obj").has_value());
    CB_CHECK_EQ(object_digest(a / "x.obj").value(), object_digest(b / "x.obj").value());
    CB_CHECK_EQ(object_digest(a / "x.obj")->size(), 40u);   // SHA-1
    fs::write_file(b / "y.obj", make_object(b, "f0007", 0x91));   // other code
    CB_CHECK_NE(object_digest(a / "x.obj").value(), object_digest(b / "y.obj").value());
    CB_CHECK(!object_digest(a / "missing.obj").has_value());
    // COFF: all but the time stamp (bytes 4 to 8)
    Bytes coff1(64, 7), coff2(64, 7);
    for (int i = 4; i < 8; ++i) coff2[static_cast<std::size_t>(i)] = 99;
    fs::write_file(a / "c1.obj", coff1);
    fs::write_file(a / "c2.obj", coff2);
    CB_CHECK_EQ(object_digest(a / "c1.obj").value(), object_digest(a / "c2.obj").value());
    coff2[20] = 0;
    fs::write_file(a / "c3.obj", coff2);
    CB_CHECK_NE(object_digest(a / "c1.obj").value(), object_digest(a / "c3.obj").value());
});

// ---- the compiler -----------------------------------------------------------------------------------------------------

CB_TEST("build.functions.run_compile_makes_the_object_or_names_the_error", [] {
    Kit k;
    const stdfs::path dir = k.work("out");
    const std::vector<std::string> flags = {"-zq", "-w0"};
    const CompileOutcome ok = run_compile(k.compiler, flags, dir, "f0000", fn("good"));
    CB_CHECK(ok.why.empty());
    CB_CHECK(stdfs::exists(dir / "f0000.obj"));
    CB_CHECK_EQ(fs::read_text(dir / "f0000.c"), "#include \"m.h\"\n\nint good(void)\n{ /*  */ return 1; }\n");
    CB_CHECK(fs::read_text(dir / "calls.txt").find("-zq -w0 f0000.c") != std::string::npos);

    // an error with a line: the line it is about is added (line 3 of the file is the text's first)
    const CompileOutcome bad = run_compile(k.compiler, flags, dir, "f0001", "int FAIL_WITH_LINE(void)\n{ return 1; }");
    CB_CHECK_EQ(bad.why, "E1002: bad thing -- int FAIL_WITH_LINE(void)");
    CB_CHECK(bad.keepable);
    CB_CHECK(!stdfs::exists(dir / "f0001.obj"));

    // a compiler that fell over without a line: what it said, and it is not worth remembering
    const CompileOutcome fell = run_compile(k.compiler, flags, dir, "f0002", fn("x", "FAIL_NO_LINE"));
    CB_CHECK_EQ(fell.why, "the compiler fell over");
    CB_CHECK(!fell.keepable);

    // a stale object from an earlier run is not mistaken for this one's
    fs::write_text(dir / "f0003.obj", "stale");
    CB_CHECK(!run_compile(k.compiler, flags, dir, "f0003", fn("y", "FAIL_NO_LINE")).why.empty());
    CB_CHECK(!stdfs::exists(dir / "f0003.obj"));
});

CB_TEST("build.functions.a_compiler_that_runs_too_long_is_stopped_and_the_function_rejected", [] {
    Kit k;
    const stdfs::path dir = k.work("out");
    const auto start = std::chrono::steady_clock::now();
    const CompileOutcome r = run_compile(k.compiler, {}, dir, "f0000", fn("slow", "SLOW"), std::chrono::milliseconds(400));
    CB_CHECK(std::chrono::steady_clock::now() - start < std::chrono::seconds(4));
    CB_CHECK(r.why.find("time limit") != std::string::npos);
    CB_CHECK(!r.keepable);
});

CB_TEST("build.functions.run_never_set_lists_each_variable_once_sorted", [] {
    Kit k;
    const stdfs::path dir = k.work("out");
    const std::vector<std::string> flags = {"-zq", "-w0", "-bt=windows"};
    const auto names = run_never_set(k.compiler, flags, dir, "f0004", fn("v", "UNSET"));
    CB_CHECK((names == std::vector<std::string>{"in_AX", "in_BX"}));
    CB_CHECK(!stdfs::exists(dir / "f0004_w.c"));   // its source is taken away again
    // the warnings are asked for: -w0 is replaced by -w2, and -zs added
    const std::string calls = fs::read_text(dir / "calls.txt");
    CB_CHECK(calls.find("-zq -bt=windows -w2 -zs f0004_w.c") != std::string::npos);
    CB_CHECK(run_never_set(k.compiler, flags, dir, "f0005", fn("clean")).empty());
});

CB_TEST("build.functions.a_compile_is_made_once_and_remembered", [] {
    Kit k;
    MemoryCache cache;
    FunctionBuilder builder({.cache = &cache});
    const stdfs::path one = k.work("one"), two = k.work("two");
    builder.write_header(one, "header text");
    builder.write_header(two, "header text");
    // the fake compiler leaves objects as OpenWatcom writes them, naming the folder it ran in
    fs::write_file(k.dir.path() / "template.obj", make_object(one, "f0000"));
    const std::vector<std::string> flags = {"-zq"};
    CB_CHECK(builder.compile(k.compiler, flags, one, "f0000", fn("a")).empty());
    CB_CHECK_EQ(k.lines_of(one / "calls.txt"), 1);
    CB_CHECK_EQ(cache.puts, 1);
    // again: from the cache, the compiler is not started, and the object is there
    stdfs::remove(one / "f0000.obj");
    CB_CHECK(builder.compile(k.compiler, flags, one, "f0000", fn("a")).empty());
    CB_CHECK_EQ(k.lines_of(one / "calls.txt"), 1);
    CB_CHECK(stdfs::exists(one / "f0000.obj"));
    // in another folder: the object is made again from what was kept, for this folder
    CB_CHECK(builder.compile(k.compiler, flags, two, "f0000", fn("a")).empty());
    CB_CHECK_EQ(k.lines_of(two / "calls.txt"), 0);
    CB_CHECK_EQ(parse_object(fs::read_file(two / "f0000.obj")).theadr, native_text(two) + "\\f0000.c");
    // another text, other options, another stem or another header are other compiles
    CB_CHECK(builder.compile(k.compiler, flags, one, "f0000", fn("b")).empty());
    CB_CHECK(builder.compile(k.compiler, std::vector<std::string>{"-zq", "-3"}, one, "f0000", fn("a")).empty());
    CB_CHECK(builder.compile(k.compiler, flags, one, "f0001", fn("a")).empty());
    CB_CHECK_EQ(k.lines_of(one / "calls.txt"), 4);
    builder.write_header(one, "another header");
    CB_CHECK(builder.compile(k.compiler, flags, one, "f0000", fn("a")).empty());
    CB_CHECK_EQ(k.lines_of(one / "calls.txt"), 5);
});

CB_TEST("build.functions.a_rejection_is_remembered_but_a_compiler_that_fell_over_is_not", [] {
    Kit k;
    MemoryCache cache;
    FunctionBuilder builder({.cache = &cache});
    const stdfs::path dir = k.work("out");
    builder.write_header(dir, "h");
    const std::string rejected = "int FAIL_WITH_LINE(void)\n{ return 1; }";
    const std::string fell = fn("z", "FAIL_NO_LINE");
    const std::vector<std::string> flags = {};
    CB_CHECK_EQ(builder.compile(k.compiler, flags, dir, "f0000", rejected), "E1002: bad thing -- int FAIL_WITH_LINE(void)");
    CB_CHECK_EQ(builder.compile(k.compiler, flags, dir, "f0000", rejected), "E1002: bad thing -- int FAIL_WITH_LINE(void)");
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 1);
    CB_CHECK_EQ(builder.compile(k.compiler, flags, dir, "f0001", fell), "the compiler fell over");
    CB_CHECK_EQ(builder.compile(k.compiler, flags, dir, "f0001", fell), "the compiler fell over");
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 3);
    // the kept message names the folder it ran in by a mark: in another folder it names that one
    const stdfs::path other = k.work("other");
    builder.write_header(other, "h");
    const std::string with_folder = "int FAIL_WITH_LINE(void)\n{ return 2; }";
    (void)builder.compile(k.compiler, flags, dir, "f0002", with_folder);
    CB_CHECK_EQ(builder.compile(k.compiler, flags, other, "f0002", with_folder), "E1002: bad thing -- int FAIL_WITH_LINE(void)");
    CB_CHECK_EQ(k.lines_of(other / "calls.txt"), 0);
});

CB_TEST("build.functions.without_a_header_key_or_a_cache_everything_is_compiled", [] {
    Kit k;
    MemoryCache cache;
    FunctionBuilder with_cache({.cache = &cache});
    const stdfs::path dir = k.work("out");   // no header written: no key
    CB_CHECK(with_cache.compile(k.compiler, {}, dir, "f0000", fn("a")).empty());
    CB_CHECK(with_cache.compile(k.compiler, {}, dir, "f0000", fn("a")).empty());
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 2);
    CB_CHECK_EQ(cache.puts, 0);
    FunctionBuilder without({});
    without.write_header(dir, "h");
    CB_CHECK(without.compile(k.compiler, {}, dir, "f0001", fn("a")).empty());
    CB_CHECK(without.compile(k.compiler, {}, dir, "f0001", fn("a")).empty());
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 4);
    CB_CHECK(!without.header_key(k.dir.path() / "never").has_value());
    CB_CHECK_EQ(*without.header_key(dir), FunctionBuilder::header_digest("h"));
    CB_CHECK_NE(FunctionBuilder::header_digest("h"), FunctionBuilder::header_digest("h2"));
});

CB_TEST("build.functions.flow_checks_are_kept_too", [] {
    Kit k;
    MemoryCache cache;
    FunctionBuilder builder({.cache = &cache});
    const stdfs::path dir = k.work("out");
    builder.write_header(dir, "h");
    const std::vector<std::string> flags = {"-zq", "-w0"};
    const auto first = builder.never_set(k.compiler, flags, dir, "f0000", fn("v", "UNSET"));
    CB_CHECK((first == std::vector<std::string>{"in_AX", "in_BX"}));
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 1);
    CB_CHECK((builder.never_set(k.compiler, flags, dir, "f0009", fn("v", "UNSET")) == first));   // the stem is not in the key
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 1);
    // a function with nothing to report is a fact too
    CB_CHECK(builder.never_set(k.compiler, flags, dir, "f0001", fn("c")).empty());
    CB_CHECK(builder.never_set(k.compiler, flags, dir, "f0001", fn("c")).empty());
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 2);
});

CB_TEST("build.functions.what_the_engine_served_answers_before_the_compiler", [] {
    Kit k;
    FunctionBuilder builder({});
    const stdfs::path dir = k.work("out");
    builder.write_header(dir, "h");
    const std::vector<std::string> flags = {"-zq"};
    const std::string key = FunctionBuilder::served_key(*builder.header_key(dir), flags, fn("a"));
    const std::string bad_key = FunctionBuilder::served_key(*builder.header_key(dir), flags, fn("b"));
    // not serving: the compiler runs
    CB_CHECK(builder.compile(k.compiler, flags, dir, "f0000", fn("a")).empty());
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 1);
    builder.serve({{key, ServedObject{"", Bytes{'O', 'B', 'J'}}}, {bad_key, ServedObject{"E9: served rejection", {}}}},
                  {{FunctionBuilder::served_key(*builder.header_key(dir), flags, fn("a")), {"in_ES"}}});
    CB_CHECK(builder.compile(k.compiler, flags, dir, "f0001", fn("a")).empty());
    CB_CHECK_EQ(fs::read_text(dir / "f0001.obj"), "OBJ");
    CB_CHECK_EQ(fs::read_text(dir / "f0001.c"), "#include \"m.h\"\n\n" + fn("a") + "\n");
    CB_CHECK_EQ(builder.compile(k.compiler, flags, dir, "f0002", fn("b")), "E9: served rejection");
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 1);
    CB_CHECK((builder.never_set(k.compiler, flags, dir, "f0003", fn("a")) == std::vector<std::string>{"in_ES"}));
    // what the engine did not make is compiled, and counted
    CB_CHECK(builder.compile(k.compiler, flags, dir, "f0004", fn("c")).empty());
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 2);
    CB_CHECK_EQ(builder.served_counts().served, 2);
    CB_CHECK_EQ(builder.served_counts().compiled, 1);
    // the facts go when the serving ends
    builder.unserve();
    CB_CHECK(builder.compile(k.compiler, flags, dir, "f0005", fn("a")).empty());
    CB_CHECK_EQ(fs::read_text(dir / "f0005.obj"), "REAL\r\n");
    CB_CHECK_EQ(k.lines_of(dir / "calls.txt"), 3);
    // calls nest: it goes on until the last one is undone
    builder.serve({{key, ServedObject{"", Bytes{'X'}}}}, {});
    builder.serve({}, {});
    builder.unserve();
    CB_CHECK(builder.compile(k.compiler, flags, dir, "f0006", fn("a")).empty());
    CB_CHECK_EQ(fs::read_text(dir / "f0006.obj"), "X");
    builder.unserve();
    CB_CHECK(builder.compile(k.compiler, flags, dir, "f0007", fn("a")).empty());
    CB_CHECK_EQ(fs::read_text(dir / "f0007.obj"), "REAL\r\n");
});

CB_TEST("build.functions.every_function_is_compiled_and_a_rejected_one_is_stood_in_for", [] {
    Kit k;
    FunctionBuilder builder({.workers = {.workers = 3}});
    const stdfs::path dir = k.work("out");
    Texts texts = {
        {"first", fn("first")},
        {"broken", "int broken(void)\n{ FAIL_WITH_LINE }"},                    // its body is the fault: a stand-in compiles
        {"hopeless", "int FAIL_WITH_LINE hopeless(void)\n{ return 1; }"},       // so is its head: not even a stand-in does
        {"last", fn("last")},
    };
    const std::vector<std::string> order = {"first", "broken", "hopeless", "last"};
    const Compiled c = builder.compile_all(k.compiler, std::vector<std::string>{"-zq"}, "header text", texts, order, dir);
    CB_CHECK_EQ(fs::read_text(dir / "m.h"), "header text");
    CB_CHECK_EQ(c.stems.size(), 3u);
    CB_CHECK_EQ(c.stems.at("first"), "f0000");
    CB_CHECK_EQ(c.stems.at("broken"), "f0001");
    CB_CHECK(!c.stems.contains("hopeless"));
    CB_CHECK_EQ(c.stems.at("last"), "f0003");
    CB_CHECK_EQ(c.dropped.size(), 2u);
    CB_CHECK_EQ(c.dropped.begin()->first, "broken");   // in the order the functions were given
    CB_CHECK_EQ((c.dropped.begin() + 1)->first, "hopeless");
    CB_CHECK(c.dropped.at("broken").starts_with("E1002: bad thing"));
    CB_CHECK_EQ(fs::read_text(dir / "f0001.obj"), "STANDIN\r\n");
    CB_CHECK_EQ(fs::read_text(dir / "f0000.obj"), "REAL\r\n");
    CB_CHECK(!stdfs::exists(dir / "f0002.obj"));
});

CB_TEST("build.functions.groups_share_a_pool_and_keep_their_own_folders_and_options", [] {
    Kit k;
    FunctionBuilder builder({.workers = {.workers = 1}});   // (one at a time: the fake compiler's log is not written by two at once)
    const stdfs::path a = k.work("seg1"), b = k.work("seg2");
    Texts texts = {{"f", fn("f")}, {"g", fn("g")}, {"h", fn("h")}};
    const std::vector<Group> groups = {{{"-zq"}, {"f", "g"}, a}, {{"-zq", "-mm"}, {"h"}, b}};
    const ExtraFlags more = {{"g", {"-nt=CODE9"}}};
    const auto out = builder.compile_groups(k.compiler, "H", texts, groups, std::nullopt, more);
    CB_CHECK_EQ(out.size(), 2u);
    CB_CHECK_EQ(out[0].stems.at("g"), "f0001");
    CB_CHECK_EQ(out[1].stems.at("h"), "f0000");   // numbered within its folder
    const std::string calls_a = fs::read_text(a / "calls.txt");
    CB_CHECK(calls_a.find("-zq -nt=CODE9 f0001.c") != std::string::npos);   // g's own switch, only g
    CB_CHECK(calls_a.find("-zq f0000.c") != std::string::npos);
    CB_CHECK(fs::read_text(b / "calls.txt").find("-zq -mm f0000.c") != std::string::npos);
    CB_CHECK_EQ(fs::read_text(b / "m.h"), "H");
    CB_CHECK(builder.compile_groups(k.compiler, "H", texts, {}).empty());
});

// ---- the builds -------------------------------------------------------------------------------------------------------

namespace {

Tools16 tools16(Kit& k) {
    Tools16 t;
    t.wcc = k.compiler;
    t.wlink = k.wlink;
    t.wasm = k.wasm;
    t.lib_dir = "C:/watcom/lib286/win";
    t.interrupt_routines = [](const std::set<int>& vectors) {
        std::string vs;
        for (int v : vectors) vs += std::to_string(v) + " ";
        return Tools16::InterruptCode{"; interrupts " + vs + "\n", "#include \"m.h\"\nint interrupt_registers(void) { return 5; }\n"};
    };
    return t;
}

}  // namespace

CB_TEST("build.functions.build16_links_and_says_where_each_function_is", [] {
    Kit k;
    k.flag("map.txt");
    fs::write_text(k.dir.path() / "map.txt",
                   "WATCOM LINKER MAP\r\n"
                   "0001:0000  garbage\r\n"
                   "0001:00000010       _alpha\r\n"
                   "0002:0020+  beta_\r\n"
                   "0002:0030*  alpha\r\n"        // (a second alpha: the first one stays)
                   "0002:0040       _stolen\r\n"  // (not one of the functions)
                   "0003:00000050   gamma@4\r\n");
    FunctionBuilder builder({});
    const stdfs::path dir = k.work("out");
    Texts texts = {{"alpha", fn("alpha")}, {"beta", fn("beta")}, {"gamma", fn("gamma")}, {"broken", "int broken(void)\n{ FAIL_WITH_LINE }"}};
    const Build16Input in{.header = "hdr",
                          .texts = texts,
                          .order = {"alpha", "beta", "gamma", "broken"},
                          .imports = {{"GetVersion", {"KERNEL", 3}}, {"Alloc", {"KERNEL", 15}}},
                          .outdir = dir,
                          .segments = {{2, Bytes{1, 2, 3}}},
                          .jobs = 2,
                          .code_segment = {},
                          .interrupts = {0x21},
                          .code_data = {{1, "int code_data_one;"}},
                          .code_image = {{1, Bytes(40, 9)}}};
    const Built16 b = builder.build16(tools16(k), in);
    CB_CHECK(b.dll.has_value());
    CB_CHECK(b.dll && *b.dll == dir / "m.dll");
    CB_CHECK_EQ(b.where.size(), 3u);
    CB_CHECK((b.where.at("alpha") == SegmentOffset{1, 0x10}));
    CB_CHECK((b.where.at("beta") == SegmentOffset{2, 0x20}));
    CB_CHECK((b.where.at("gamma") == SegmentOffset{3, 0x50}));
    CB_CHECK_EQ(b.dropped.size(), 1u);
    CB_CHECK(b.dropped.contains("broken"));
    // the link: what it was told
    const std::string lnk_text = fs::read_text(dir / "m.lnk");
    const auto lnk = text::lines(lnk_text);
    const std::vector<std::string> expected = {
        "system windows dll", "name m.dll", "option quiet, map, nodefaultlibs, caseexact, undefsok",
        "libpath C:/watcom/lib286/win", "option packcode=0", "file image01.obj",
        "file f0000.obj", "file f0001.obj", "file f0002.obj", "file f0003.obj",   // (the broken one is stood in for)
        "file code01.obj", "file data.obj", "library clibm.lib", "file interrupts.obj", "file interrupt_registers.obj",
        "import _Alloc KERNEL.15", "import _GetVersion KERNEL.3"};
    CB_CHECK_EQ(lnk.size(), expected.size());
    for (std::size_t i = 0; i < lnk.size() && i < expected.size(); ++i) CB_CHECK_EQ(std::string(lnk[i]), expected[i]);
    // the data segment, the original's code segment, and the data of the code segment were compiled in
    CB_CHECK(fs::read_text(dir / "data.c").find("char codebreaker_data_segment = 1;") != std::string::npos);
    CB_CHECK(fs::read_text(dir / "data.c").find("CODEBREAKER_SEGMENT_2\")) codebreaker_segment_2[] = {\n1,2,3\n};") != std::string::npos);
    CB_CHECK(fs::read_text(dir / "image01.c").find("codebreaker_code_image_1[] = {\n9,9,9") != std::string::npos);
    CB_CHECK(fs::read_text(dir / "calls.txt").find("-nt=CODE1 image01.c") != std::string::npos);
    CB_CHECK(fs::read_text(dir / "assembled.txt").find("-q interrupts.asm") != std::string::npos);
    CB_CHECK(fs::read_text(dir / "interrupts.asm").find("interrupts 33") != std::string::npos);
    // the digests of the objects, for the engine: a stand-in is marked
    const auto digests = builder.object_digests(dir);
    CB_CHECK(digests.has_value());
    CB_CHECK(digests->at("broken").starts_with("stand-in:"));
    CB_CHECK(!digests->at("alpha").starts_with("stand-in:"));
    CB_CHECK(!builder.object_digests(k.dir.path() / "nowhere").has_value());
});

CB_TEST("build.functions.build16_failing_to_link_blames_every_function_that_was_built", [] {
    Kit k;
    k.flag("nolink.txt");
    FunctionBuilder builder({});
    const stdfs::path dir = k.work("out");
    Texts texts = {{"alpha", fn("alpha")}, {"broken", "int broken(void)\n{ FAIL_WITH_LINE }"}};
    const Built16 b = builder.build16(tools16(k), {.header = "h", .texts = texts, .order = {"alpha", "broken"}, .outdir = dir});
    CB_CHECK(!b.dll.has_value());
    CB_CHECK(b.where.empty());
    CB_CHECK_EQ(b.dropped.size(), 2u);
    CB_CHECK_EQ(b.dropped.at("alpha"), "link: Error! E3000: cannot open something");   // the first error, not the lines around it
    CB_CHECK(b.dropped.at("broken").starts_with("E1002"));   // its own reason stands
    // a tool that is not there is a mistake of the caller
    Tools16 none = tools16(k);
    none.wlink.clear();
    CB_CHECK_THROWS_AS(UsageError, (void)builder.build16(none, {.header = "h", .texts = texts, .order = {"alpha"}, .outdir = dir}));
});

CB_TEST("build.functions.build16_stands_in_for_a_function_that_reaches_near_across_segments", [] {
    Kit k;
    k.flag("fail_near.txt");
    k.flag("map.txt");
    fs::write_text(k.dir.path() / "map.txt", "0001:0010 _a\r\n0001:0020 _b\r\n0001:0030 _c\r\n");
    FunctionBuilder builder({});
    const stdfs::path dir = k.work("out");
    Texts texts = {{"a", fn("a")}, {"b", fn("b")}, {"c", fn("c")}};
    const Built16 built = builder.build16(tools16(k), {.header = "h", .texts = texts, .order = {"a", "b", "c"}, .outdir = dir});
    CB_CHECK(built.dll.has_value());
    CB_CHECK_EQ(k.lines_of(dir / "links.txt"), 2);   // linked again after the repair
    CB_CHECK_EQ(built.dropped.size(), 1u);
    CB_CHECK_EQ(built.dropped.at("b"), "reaches near for something in another segment");
    CB_CHECK(built.where.contains("a") && built.where.contains("c") && !built.where.contains("b"));
    CB_CHECK_EQ(fs::read_text(dir / "f0001.obj"), "STANDIN\r\n");
});

CB_TEST("build.functions.build16_drops_the_largest_functions_of_a_segment_over_64k", [] {
    Kit k;
    k.flag("fail_64k.txt");
    k.flag("map.txt");
    fs::write_text(k.dir.path() / "map.txt", "0001:0010 _a\r\n0001:0020 _b\r\n0002:0030 _c\r\n");
    FunctionBuilder builder({});
    const stdfs::path dir = k.work("out");
    Texts texts = {{"a", fn("a")}, {"b", fn("b")}, {"c", fn("c")}};
    // the fake linker says the first segment is 100 bytes over and wants every function that is not a stand-in gone; the
    // functions of the other segment are not touched by this repair, so it is only done once all of them are stood in for
    const Built16 built = builder.build16(tools16(k), {.header = "h", .texts = texts, .order = {"a", "b", "c"}, .outdir = dir,
                                                       .code_segment = {{"a", 1}, {"b", 1}, {"c", 2}}});
    CB_CHECK(built.dropped.contains("a"));
    CB_CHECK(built.dropped.contains("b"));
    CB_CHECK_EQ(built.dropped.at("a"), "too large: the functions of its segment do not fit in 64K when rebuilt");
    // c is also not a stand-in, so the link goes on failing; the repair stops when nothing more of segment 1 is left
    CB_CHECK(!built.dll.has_value());
    CB_CHECK(built.dropped.at("c").starts_with("link: "));
});

CB_TEST("build.functions.build32_exports_what_it_built_and_jumps_to_the_original_for_what_it_lacks", [] {
    Kit k;
    k.flag("undefined.txt");
    FunctionBuilder builder({});
    const stdfs::path dir = k.work("out");
    Texts texts = {{"alpha", fn("alpha")}, {"beta", fn("beta")}, {"fails", "int fails(void)\n{ FAIL_WITH_LINE }"}};
    Tools32 t;
    t.wcc = k.compiler;
    t.wlink = k.wlink;
    t.wasm = k.wasm;
    t.link_head = {"option map", "option offset=0x10000000"};
    t.runtime_lines = {"library clib3r.lib", "", "library math3r.lib"};
    t.import_lines = [](ByteView) { return std::vector<std::string>{"import GetVersion KERNEL32.GetVersion", "", "import Sleep KERNEL32.Sleep"}; };
    t.publics = [](ByteView obj) {   // (the fake objects are text: say what each one defines by what it holds)
        const std::string_view text = as_text(obj);
        if (text.find("REAL") != std::string_view::npos) return std::vector<std::string>{"_alpha", "other", "_beta@8", "_gamma_x"};
        return std::vector<std::string>{};
    };
    t.exports = [](const stdfs::path&) {
        Tools32::PeExports e;
        e.image_base = 0x10000000;
        e.rvas = {{"alpha", 0x1000}, {"beta", 0x2000}, {"fails", 0x3000}};
        return e;
    };
    const Built32 b = builder.build32(t, {.header = "h", .texts = texts, .order = {"alpha", "beta", "fails"}, .outdir = dir,
                                          .cflags = {"-zq", "-bt=nt"}, .jobs = 1,
                                          .originals = {{"fails", 0x401000}, {"missing", 0x00402000}}});
    CB_CHECK(b.dll.has_value());
    // a function that is rejected and that the original has is not stood in for: it is the original's
    CB_CHECK_EQ(b.dropped.size(), 1u);
    CB_CHECK(b.dropped.contains("fails"));
    CB_CHECK_EQ(b.where.size(), 2u);
    CB_CHECK_EQ(b.where.at("alpha"), 0x10001000u);
    CB_CHECK_EQ(b.where.at("beta"), 0x10002000u);
    CB_CHECK_EQ(builder.object_digests(dir)->at("fails"), "the original's");
    // the compiler got the module's options and -e=20
    CB_CHECK(fs::read_text(dir / "calls.txt").find("-zq -bt=nt -e=20 f0000.c") != std::string::npos);
    // linked twice: the second time with the jump object, because the first said `_missing` was undefined
    const std::string links = fs::read_text(dir / "links.txt");
    CB_CHECK_EQ(k.lines_of(dir / "links.txt"), 2);
    const std::string lnk_text = fs::read_text(dir / "m.lnk");
    const auto lnk = text::lines(lnk_text);
    const std::vector<std::string> expected = {
        "system nt_dll", "name m.dll", "option quiet", "option undefsok", "option map", "option offset=0x10000000",
        "file f0000.obj", "file f0001.obj", "library clib3r.lib", "library math3r.lib", "library math387r.lib",
        "import GetVersion KERNEL32.GetVersion", "import Sleep KERNEL32.Sleep",   // (once each, however many objects ask)
        "file original.obj", "export alpha='_alpha'", "export beta='_beta@8'"};
    CB_CHECK_EQ(lnk.size(), expected.size());
    for (std::size_t i = 0; i < lnk.size() && i < expected.size(); ++i) CB_CHECK_EQ(std::string(lnk[i]), expected[i]);
    // the jumps: only the names the linker said were undefined and the original has
    const std::string jumps = fs::read_text(dir / "original.asm");
    CB_CHECK(jumps.find("public _missing\n_missing:\n        push 000402000h\n        ret\n") != std::string::npos);
    CB_CHECK(jumps.find("_TEXT segment dword public use32 'CODE'") != std::string::npos);
    CB_CHECK(fs::read_text(dir / "assembled.txt").find("-zq -fo=original.obj original.asm") != std::string::npos);
});

CB_TEST("build.functions.build_powerpc_compiles_links_at_a_base_and_reads_the_map", [] {
    Kit k;
    k.flag("map.txt");
    fs::write_text(k.dir.path() / "map.txt",
                   " Address         Publics by Value              Rva+Base       Lib:Object\r\n"
                   "\r\n"
                   " 0001:00000000       alpha                      82010000 f   f0000.obj\r\n"
                   " 0001:00000040       alpha                      82010040 f   again.obj\r\n"
                   " 0001:00000080       outside                    82010080 f   externs.obj\r\n"
                   " 0001:000000c0       unwanted                   820100c0 f   x.obj\r\n");
    FunctionBuilder builder({});
    const stdfs::path dir = k.work("out");
    Texts texts = {{"alpha", fn("alpha")}, {"ugly", "int FAIL_WITH_LINE ugly(void)\n{ }"}};
    const PowerPcTools tools{k.cc, k.wlink, "C:/xdk/lib/xboxkrnl.lib"};
    const std::vector<std::string> externs = {"outside", "other"};
    const BuiltPowerPc b = builder.build_powerpc(tools, "ppc header", texts, std::vector<std::string>{"alpha", "ugly"}, externs, dir, 0x82000000, 1);
    CB_CHECK(b.dll.has_value());
    CB_CHECK_EQ(b.where.size(), 2u);
    CB_CHECK_EQ(b.where.at("alpha"), 0x82010000u);
    CB_CHECK_EQ(b.where.at("outside"), 0x82010080u);
    CB_CHECK_EQ(b.dropped.size(), 1u);
    CB_CHECK(b.dropped.at("ugly").find("bad thing") != std::string::npos);   // (what the compiler said)
    CB_CHECK_EQ(fs::read_text(dir / "m.h"), "ppc header");
    CB_CHECK_EQ(fs::read_text(dir / "m.rsp"), "f0000.obj\nexterns.obj\n");
    // each extern is a function of its own, in name order, that returns its number
    CB_CHECK_EQ(fs::read_text(dir / "externs.c"),
                "#pragma optimize(\"\", off)\nuint8 other() { return 0; }\n#pragma optimize(\"\", off)\nuint8 outside() { return 1; }\n");
    const std::string links = fs::read_text(dir / "links.txt");
    CB_CHECK(links.find("/BASE:0x82000000 /OUT:m.dll @m.rsp C:/xdk/lib/xboxkrnl.lib") != std::string::npos);
    // a PowerPC compile has the PowerPC options
    CB_CHECK(fs::read_text(dir / "calls.txt").find("/nologo /c /TC /O2 /GS- /Zl /Gy /W0 /FIm.h /Fof0000.obj f0000.c") != std::string::npos);
    k.flag("nolink.txt");
    const BuiltPowerPc failed = builder.build_powerpc(tools, "ppc header", texts, std::vector<std::string>{"alpha"}, externs, dir, 0x82000000, 1);
    CB_CHECK(!failed.dll.has_value());
    CB_CHECK(failed.dropped.at("alpha").starts_with("link: "));
});

CB_TEST("build.functions.a_cache_is_not_needed_to_build", [] {
    Kit k;
    FunctionBuilder builder({});
    const stdfs::path dir = k.work("out");
    k.flag("map.txt");
    fs::write_text(k.dir.path() / "map.txt", "0001:0010 _alpha\r\n");
    Texts texts = {{"alpha", fn("alpha")}};
    const Built16 b = builder.build16(tools16(k), {.header = "h", .texts = texts, .order = {"alpha"}, .outdir = dir});
    CB_CHECK(b.dll.has_value());
    CB_CHECK_EQ(b.where.size(), 1u);
    CB_CHECK(b.dropped.empty());
});
