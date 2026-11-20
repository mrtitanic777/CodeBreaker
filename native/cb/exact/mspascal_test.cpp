// cb-unity: alone
// Tests of cb::exact::mspascal: the Microsoft Pascal compiler as a toolchain. The pure parts -- the order a source's procedures are
// emitted in, the PUBDEF written into an object, naming the procedures from a listing or from where the code calls the entry helper --
// are tested on small objects made here; the orchestration (the kit's folder, the two passes, the errors, the cache) with a pass runner
// that is not the DOS executive. With the real kit and executive (CB_KIT_MSPASCAL names the kit) the 3.11 compiler compiles a
// small program and its procedures come out named.
//
// Hand-checked: the helpers' bytes, the order of emit_order for nested procedures, forward declarations, records and
// case blocks, the PUBDEF's layout (group 0, the segment's index, each name with its offset and a zero type), "(helper)", the command
// lines `T,T,T,T;` (and `NAME,T,T,T;` for another file name), the 1500 characters of listing an error shows.
//
// Design notes: an unterminated procedure heading ends the scan; passes run `fast` (no block
// hook: the blocks are not used here); the 1.10 kit's zips are extracted by a hook (Hooks::extract_zip).
#include "cb/exact/mspascal.hpp"

#include <nlohmann/json.hpp>

#include "cb/base/cache.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/omf.hpp"
#include "cb/exact/plugin.hpp"

namespace {

namespace pas = cb::exact::mspascal;
namespace tc = cb::exact::toolchain;
namespace omf = cb::formats::omf;
namespace stdfs = std::filesystem;
namespace cfs = cb::fs;
using cb::Bytes;
using cb::as_bytes;
using cb::to_bytes;

bool has(std::string_view hay, std::string_view needle) { return hay.find(needle) != std::string_view::npos; }

const char* const kSource =
    "{ a comment with procedure x begin end }\n"
    "program demo;\n"
    "type r = record a: integer; b: record c: integer end end;\n"
    "procedure outer(x: integer; (* skip ( *) y: integer);\n"
    "  procedure inner;\n"
    "  begin\n"
    "    case x of\n"
    "      1: begin end;\n"
    "    end;\n"
    "  end;\n"
    "begin\n"
    "  inner;\n"
    "end;\n"
    "procedure later; forward;\n"
    "function f(z: integer): integer;\n"
    "begin\n"
    "  f := z;\n"
    "end;\n"
    "procedure later;\n"
    "begin writeln('end of begin');\n"
    "end;\n"
    "begin\n"
    "end.\n";

constexpr const char* kProfile = R"JSON({
  "name": "mspascal", "plugin": "codebreaker.exact.mspascal:MSPascal", "kit": "mspascal", "kit110": "mspascal110", "emulator": "dos",
  "default_variant": "1.10", "helper_data": 3,
  "variants": {"1.10": {"version": "1.10"}, "3.04": {"version": "3.04"}, "3.11": {"version": "3.11", "helper_data": 2}}
})JSON";

// An object whose code segment holds, at `starts`, the entry helper's call (a near call to `helper`), as a compiler writes it.
struct Code3 {
    Bytes code;
    std::size_t helper = 0;
    std::vector<std::size_t> starts;
};

Code3 make_code(std::size_t procedures) {
    Code3 c;
    const std::size_t each = 8;
    c.helper = each * procedures + 4;
    for (std::size_t i = 0; i < procedures; ++i) {
        const std::size_t at = c.code.size();
        c.starts.push_back(at);
        const std::int16_t rel = static_cast<std::int16_t>(c.helper - (at + 3));   // E8 rel16: a near call
        for (const std::uint8_t b : {std::uint8_t{0xE8}, static_cast<std::uint8_t>(rel & 0xFF), static_cast<std::uint8_t>((rel >> 8) & 0xFF), std::uint8_t{0x90},
                                     std::uint8_t{0x90}, std::uint8_t{0x90}, std::uint8_t{0x90}, std::uint8_t{0xC3}})
            c.code.push_back(b);
    }
    c.code.resize(c.helper, 0x90);
    const Bytes& h = pas::entry_helpers()[1];   // 3.0x: only the frame size
    c.code.insert(c.code.end(), h.begin(), h.end());
    c.code.push_back(0);
    c.code.push_back(0);
    return c;
}

std::map<std::string, std::int64_t> publics_of(const Bytes& object) {
    std::map<std::string, std::int64_t> out;
    for (const omf::Public& p : omf::read(object).publics) out[p.name] = p.offset;
    return out;
}

// A pass runner that is not the executive: PAS1 says what it was told to, PAS2 leaves the object and its listing.
class FakeRunner final : public tc::PassRunner {
public:
    mutable std::vector<std::string> calls;       // "PROGRAM tail"
    mutable std::string pass_one_says = "Pass One\r\nNo Errors Detected\r\n";
    mutable std::string listing;                  // T.LST written by PAS1
    mutable std::string code_listing;             // T.COD written by PAS2 (empty: none)
    mutable Bytes object;                         // T.OBJ written by PAS2
    cb::exact::emu::dos::Result run(const tc::PassRun& pass) const override {
        calls.push_back(pass.exe.filename().string() + " " + pass.command_tail);
        cb::exact::emu::dos::Result r;
        if (pass.exe.filename() == "PAS1.EXE") {
            r.console = to_bytes(pass_one_says);
            if (!listing.empty()) cfs::write_text(pass.workdir / "T.LST", listing);
        } else {
            r.console = to_bytes("Pass Two\r\n");
            if (!object.empty()) cfs::write_file(pass.workdir / "T.OBJ", object);
            if (!code_listing.empty()) cfs::write_text(pass.workdir / "T.COD", code_listing);
        }
        r.exit = 0;
        return r;
    }
};

struct Fixture {
    cfs::ScopedTempDir dir{"cbpascal"};
    cfs::ScopedEnv no_kit{"CB_KIT_MSPASCAL", std::nullopt};
    cfs::ScopedEnv no_kit110{"CB_KIT_MSPASCAL110", std::nullopt};
    tc::Locations locations;
    FakeRunner runner;
    cb::base::cache::Config cache_config;
    std::unique_ptr<cb::base::cache::Cache> cache;
    Fixture() {
        locations.profiles = dir.path() / "profiles";
        locations.state = dir.path() / "state";
        locations.checkout_root = dir.path();
        locations.kits["mspascal"] = dir.path() / "kit";
        locations.kits["mspascal110"] = dir.path() / "kit110";
        cfs::write_text(locations.profiles / "mspascal.json", kProfile);
        for (const char* v : {"3.04", "3.11"})
            for (const char* f : {"PAS1.EXE", "PAS2.EXE", "PASKEY"}) cfs::write_text(dir.path() / "kit" / v / f, std::string(f) + " of " + v);
        cache_config.state_dir = dir.path() / "cache";
        cache_config.shared_dir = dir.path() / "shared";
        cache = std::make_unique<cb::base::cache::Cache>(cache_config);
        cache->declare_kind(tc::kCacheKind, {cb::base::cache::Scope::State, "1"});
    }
    pas::MsPascal make(const std::string& variant = "3.04", pas::Hooks hooks = {}, bool cached = true) {
        tc::Context c;
        c.locations = &locations;
        c.runner = &runner;
        if (cached) c.cache = cache.get();
        return pas::MsPascal(tc::read_profile(locations, "mspascal"), variant, c, std::move(hooks));
    }
};

}  // namespace

CB_TEST("exact.mspascal.the_order_procedures_are_emitted_in", [] {
    const std::vector<std::string> expect = {"INNER", "OUTER", "F", "LATER"};
    CB_CHECK((pas::emit_order(kSource) == expect));
    CB_CHECK(pas::emit_order("program p;\nbegin\nend.\n").empty());
    CB_CHECK(pas::emit_order("").empty());
    // a procedure declared external or extern has no code of its own
    CB_CHECK(pas::emit_order("procedure x; extern;\nprocedure y; external;\nprocedure z;\nbegin end;\n") == std::vector<std::string>{"Z"});
    // a heading that never ends: the scan stops with what it found
    CB_CHECK(pas::emit_order("procedure a;\nbegin end;\nprocedure b(x: integer").size() == 1);
    // records and cases open `end`s that are not a procedure's
    CB_CHECK(pas::emit_order("procedure p;\ntype t = record a: integer end;\nbegin\n  case 1 of 1: ; end;\nend;\n") == std::vector<std::string>{"P"});
});

CB_TEST("exact.mspascal.entry_helpers", [] {
    const auto& h = pas::entry_helpers();
    CB_CHECK_EQ(h.size(), std::size_t{3});
    CB_CHECK_EQ(h[0].size(), std::size_t{29});
    CB_CHECK_EQ(h[1].size(), std::size_t{11});
    CB_CHECK_EQ(h[2].size(), std::size_t{11});
    CB_CHECK_EQ(h[1][0], 0x5B);
    CB_CHECK_EQ(h[2][0], 0x5E);
    CB_CHECK(pas::inline_frame() == Bytes({0x55, 0x8B, 0xEC, 0x81, 0xEC}));
});

CB_TEST("exact.mspascal.publish_writes_a_pubdef_before_the_modend", [] {
    const Code3 c = make_code(2);
    const Bytes obj = omf::raw_object("T", c.code, {});
    const cb::OrderedMap<std::string, std::uint32_t> procs = {{"BETA", 8}, {"ALPHA", 0}, {"(helper)", static_cast<std::uint32_t>(c.helper)}};
    const Bytes out = pas::publish(obj, procs, "_TEXT");
    const auto pubs = publics_of(out);
    CB_CHECK_EQ(pubs.size(), std::size_t{3});
    CB_CHECK_EQ(pubs.at("ALPHA"), 0);
    CB_CHECK_EQ(pubs.at("BETA"), 8);
    CB_CHECK_EQ(pubs.at("(helper)"), static_cast<std::int64_t>(c.helper));
    // the code is where it was, the module still ends with its MODEND, and the PUBDEF names the code segment
    CB_CHECK(omf::read(out).segments.at("_TEXT") == omf::read(obj).segments.at("_TEXT"));
    const auto records = omf::records(out);
    CB_CHECK(records.back().type == 0x8A || records.back().type == 0x8B);
    for (const omf::Public& p : omf::read(out).publics) CB_CHECK(p.segment.value_or("") == "_TEXT");
    // the names are written in the order of their offsets: ALPHA, BETA, (helper)
    std::vector<std::string> order;
    for (const omf::Public& p : omf::read(out).publics) order.push_back(p.name);
    CB_CHECK((order == std::vector<std::string>{"ALPHA", "BETA", "(helper)"}));
    // a segment the object does not define
    CB_CHECK_THROWS_AS(cb::FormatError, (void)pas::publish(obj, procs, "NOPE"));
});

CB_TEST("exact.mspascal.procedures_are_named_from_where_the_code_calls_the_helper", [] {
    const Code3 c = make_code(2);
    const Bytes obj = omf::raw_object("T", c.code, {});
    // a source of two procedures and a main block: the block is no code (one call fewer than names), so A and B get the two calls
    const Bytes named = pas::name_procedures(obj, "", "program p;\nprocedure a;\nbegin\nend;\nprocedure b;\nbegin\nend;\nbegin\nend.\n");
    const auto pubs = publics_of(named);
    CB_CHECK_EQ(pubs.at("A"), 0);
    CB_CHECK_EQ(pubs.at("B"), 8);
    CB_CHECK_EQ(pubs.at("(helper)"), static_cast<std::int64_t>(c.helper));
    CB_CHECK_EQ(pubs.size(), std::size_t{3});
    // a call count that matches no number of names: the names are paired with the calls in order, as far as they go
    const Bytes odd = pas::name_procedures(obj, "", "program p;\nbegin\nend.\n");
    CB_CHECK_EQ(publics_of(odd).size(), std::size_t{2});
    CB_CHECK_EQ(publics_of(odd).at("P"), 0);
    CB_CHECK(publics_of(odd).contains("(helper)"));
    // a name the object already publishes is not published again
    const Bytes again = pas::name_procedures(named, "", "program p;\nprocedure a;\nbegin\nend;\nprocedure b;\nbegin\nend;\nbegin\nend.\n");
    CB_CHECK_EQ(publics_of(again).size(), std::size_t{3});
    // an object that has no code is returned as it is
    const Bytes empty = omf::raw_object("T", Bytes{}, {});
    CB_CHECK(pas::name_procedures(empty, "", "x") == empty);
});

CB_TEST("exact.mspascal.procedures_are_named_from_the_code_listing", [] {
    const Code3 c = make_code(3);
    const Bytes obj = omf::raw_object("T", c.code, {});
    const std::string listing =
        "Microsoft Pascal code listing\r\n\f"
        "  Procedure/Function : first\r\n"
        "  ** 000000 55 8BEC\r\n"
        "  ** 000004 90\r\n"
        "  Procedure/Function : SECOND\r\n"
        "  ** 000008 E8\r\n"
        "  Procedure/Function : third\r\n"
        "no address here\r\n"
        "  ** 000010 90\r\n";
    const Bytes named = pas::name_procedures(obj, listing, "");
    const auto pubs = publics_of(named);
    CB_CHECK_EQ(pubs.at("FIRST"), 0);       // the first address of each: the second line of FIRST is not taken
    CB_CHECK_EQ(pubs.at("SECOND"), 8);
    CB_CHECK_EQ(pubs.at("THIRD"), 0x10);
    CB_CHECK(pubs.contains("(helper)"));
});

CB_TEST("exact.mspascal.the_kit_of_a_version", [] {
    Fixture f;
    CB_CHECK(cfs::same_path(f.make("3.11").kit(), f.dir.path() / "kit" / "3.11"));
    CB_CHECK_EQ(f.make("3.11").helper_data(), 2u);   // the variant's own
    CB_CHECK_EQ(f.make("3.04").helper_data(), 3u);   // the profile's
    CB_CHECK_EQ(f.make("3.11").version(), "3.11");
    CB_CHECK(f.make("3.11").plugin_key() != f.make("3.04").plugin_key());
    CB_CHECK_EQ(f.make("3.11").plugin_key(), cb::crypto::to_hex(cb::crypto::sha1(to_bytes("mspascal" "3.11" "pas1"))).substr(0, 12));
    // 1.10: the pre-release kit's zips, extracted once by the hook, in name order
    cfs::write_text(f.dir.path() / "kit110" / "B.ZIP", "zip");
    cfs::write_text(f.dir.path() / "kit110" / "a.zip", "zip");
    cfs::write_text(f.dir.path() / "kit110" / "readme.txt", "not a zip");
    std::vector<std::string> extracted;
    pas::Hooks hooks;
    hooks.extract_zip = [&](const stdfs::path& zip, const stdfs::path& dest) {
        extracted.push_back(zip.filename().string());
        cfs::write_text(dest / "PAS2.EXE", "from " + zip.filename().string());
    };
    const pas::MsPascal old = f.make("1.10", hooks);
    const stdfs::path d = old.kit();
    CB_CHECK(cfs::same_path(d, f.dir.path() / "state" / "exact" / "stage" / "mspascal110"));
    CB_CHECK((extracted == std::vector<std::string>{"B.ZIP", "a.zip"}));   // (path order: capitals first)
    (void)old.kit();
    CB_CHECK_EQ(extracted.size(), std::size_t{2});   // once
    // no hook: said
    cfs::ScopedTempDir other("cbpascal2");
    Fixture g;
    cfs::write_text(g.dir.path() / "kit110" / "x.zip", "zip");
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)g.make("1.10").kit());
});

CB_TEST("exact.mspascal.a_compile_runs_both_passes_and_names_the_procedures", [] {
    Fixture f;
    const Code3 c = make_code(2);
    f.runner.object = omf::raw_object("T", c.code, {});
    const pas::MsPascal t = f.make("3.04");
    const auto r = t.compile(as_bytes(std::string("program p;\nprocedure a;\nbegin\nend;\nprocedure b;\nbegin\nend;\nbegin\nend.\n")), std::string_view("T.PAS"));
    CB_CHECK(r.object.has_value());
    const auto pubs = publics_of(*r.object);
    CB_CHECK(pubs.contains("A") && pubs.contains("B") && pubs.contains("(helper)"));
    CB_CHECK(has(r.console, "Pass One") && has(r.console, "Pass Two"));
    // the passes: PAS1 with the stem, a leading blank; PAS2 with none
    CB_CHECK_EQ(f.runner.calls.size(), std::size_t{2});
    CB_CHECK_EQ(f.runner.calls[0], "PAS1.EXE  T,T,T,T;");
    CB_CHECK_EQ(f.runner.calls[1], "PAS2.EXE ");
    // the same source again is not run again: the cache has it
    const auto again = t.compile(as_bytes(std::string("program p;\nprocedure a;\nbegin\nend;\nprocedure b;\nbegin\nend;\nbegin\nend.\n")), std::string_view("T.PAS"));
    CB_CHECK_EQ(f.runner.calls.size(), std::size_t{2});
    CB_CHECK(again.object == r.object);
    // another file name is another compile, and the stem is the command's
    const auto named = t.compile(as_bytes(std::string("program p;\nbegin\nend.\n")), std::string_view("PROG.PAS"));
    CB_CHECK(named.object.has_value());
    CB_CHECK_EQ(f.runner.calls[2], "PAS1.EXE  PROG,T,T,T;");
    // as modules read it: the headers and switches mean nothing
    const cb::exact::modules::Toolchain& as_modules = t;
    const auto via_modules = as_modules.compile(as_bytes(std::string("program p;\nbegin\nend.\n")), {.switches = "/x"});
    CB_CHECK(via_modules.object.has_value());
});

CB_TEST("exact.mspascal.errors_from_pass_one_are_shown", [] {
    Fixture f;
    f.runner.pass_one_says = "Pass One\r\n 1 Error Detected\r\n";
    f.runner.listing = "program p;\r\n    ^ Error 12: something\r\nfine line\r\nanother Error here\r\n";
    const pas::MsPascal t = f.make("3.04", {}, false);
    const auto r = t.compile(as_bytes(std::string("bad")), std::string_view("T.PAS"));
    CB_CHECK(!r.object.has_value());
    CB_CHECK(has(r.console, "1 Error Detected"));
    CB_CHECK(has(r.console, "^ Error 12: something\nanother Error here"));
    CB_CHECK(!has(r.console, "fine line"));
    CB_CHECK_EQ(f.runner.calls.size(), std::size_t{1});   // PAS2 is not run
    // "No Errors" before the last banner does not count
    f.runner.pass_one_says = "Pass One\r\nNo Errors\r\nPass One\r\n";
    CB_CHECK(!t.compile(as_bytes(std::string("bad2")), std::string_view("T.PAS")).object.has_value());
    // pass two that leaves no object
    f.runner.pass_one_says = "Pass One\r\nNo Errors Detected\r\n";
    f.runner.object.clear();
    const auto none = t.compile(as_bytes(std::string("none")), std::string_view("T.PAS"));
    CB_CHECK(!none.object.has_value() && has(none.console, "Pass Two"));
});

CB_TEST("exact.mspascal.registered_as_a_plugin_and_as_the_joiner_reads_it", [] {
    Fixture f;
    tc::Toolchains toolchains(f.locations, &f.runner, f.cache.get());
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)toolchains.load("mspascal"));
    pas::register_plugin(toolchains);
    const auto loaded = std::dynamic_pointer_cast<pas::MsPascal>(toolchains.load("mspascal", "3.11"));
    CB_CHECK(loaded != nullptr);
    const cb::exact::pascal_join::Toolchain& joiner = *loaded;
    CB_CHECK_EQ(joiner.helper_data(), 2u);
});

// With the real kit and the real executive: the 3.11 compiler compiles a small program, and its procedures come out named.
CB_TEST("exact.mspascal.the_real_compiler", [] {
    const auto kit = cfs::get_env("CB_KIT_MSPASCAL");
    if (!kit) return;   // (not set up: nothing to run)
    CB_WINDOWS_ONLY("the DOS executive is built on Windows only");
    const auto unicorn = cfs::get_env("CB_UNICORN_DLL");
    const auto executive = cb::exact::emu::dos::Executive::load(unicorn ? cfs::from_text(*unicorn) : cb::testing::data_path("third_party/bin/unicorn.dll"));
    const tc::ExecutiveRunner runner(*executive);
    cfs::ScopedTempDir dir("cbpascalreal");
    tc::Locations locations;
    locations.profiles = cb::testing::data_path("../../data/exact/toolchains");
    locations.state = dir.path() / "state";
    locations.checkout_root = dir.path();
    tc::Context c;
    c.locations = &locations;
    c.runner = &runner;
    const pas::MsPascal t(tc::read_profile(locations, "mspascal"), "3.11", c);
    const std::string program =
        "program demo;\r\nvar total: integer;\r\n\r\nprocedure bump;\r\nbegin\r\n  total := total + 1;\r\nend;\r\n\r\nbegin\r\n  total := 0;\r\n  bump;\r\nend.\r\n";
    const auto r = t.compile(as_bytes(program), std::string_view("T.PAS"));
    CB_CHECK(r.object.has_value());
    if (r.object) {
        const auto pubs = publics_of(*r.object);
        CB_CHECK(pubs.contains("BUMP"));
        CB_CHECK(pubs.contains("(helper)"));
    }
    const auto bad = t.compile(as_bytes(std::string("program demo;\r\nbegin\r\n  nothing here := ;\r\nend.\r\n")), std::string_view("T.PAS"));
    CB_CHECK(!bad.object.has_value());
});
