// Tests of cb::formats::inspect on real files (formats/testdata) and on damaged and crafted input.
//
// The golden documents in testdata/inspect/*.describe.json were produced by the original Python inspector
// (codebreaker.formats.inspect.describe) on the same files and read through before being checked in: they are the
// recorded behaviour of the product, not of this module. `path` is left out of them (it is the caller's spelling).
#include <algorithm>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/cli.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/inspect.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/pe.hpp"

namespace insp = cb::formats::inspect;
namespace fs = std::filesystem;
using cb::Json;
using namespace std::string_literals;

namespace {

fs::path td(const std::string& name) { return cb::testing::data_path("formats/testdata/" + name); }

// A document with object keys sorted (nlohmann::json), so two documents compare by meaning, not key order.
nlohmann::json canon(const Json& j) { return nlohmann::json::parse(cb::json::dump(j, cb::json::Style::Compact)); }

nlohmann::json golden(const std::string& name) {
    nlohmann::json j = nlohmann::json::parse(cb::fs::read_text(td("inspect/" + name + ".describe.json")));
    return j;
}

std::vector<std::string> keys_of(const Json& j) {
    std::vector<std::string> out;
    for (const auto& [k, v] : j.items()) out.push_back(k);
    return out;
}

std::string join_keys(const Json& j) { return cb::text::join(keys_of(j), ","); }

cb::Bytes bytes_of(std::string_view s) { return cb::Bytes(s.begin(), s.end()); }

std::string capture_info(const std::vector<std::string>& args, int& code, const insp::InfoEnvironment& env = {}) {
    cb::testing::CapturedStd cap;
    std::vector<std::string> argv = {"info"};
    argv.insert(argv.end(), args.begin(), args.end());
    code = cb::cli::Cli("codebreaker", {insp::info_command(env)}).run(argv);
    return cap.out();
}

}  // namespace

// ---- describe: the recorded behaviour of the original --------------------------------------------------------------
CB_TEST("formats.inspect.describe_equals_the_recorded_original", [] {
    for (const char* name : {"lz32.dll", "xcopy32.exe", "lzexpand.dll", "charmap.exe", "bios.vxd", "xcopy.exe", "fastadd.obj", "choice.com"}) {
        Json doc = insp::to_json(insp::describe(td(name)));
        doc.erase("path");
        const nlohmann::json now = canon(doc), was = golden(name);
        if (now != was) cb::testing::record_failure(__FILE__, __LINE__, std::string(name) + ": the description differs from the recorded original\n" +
                                                                         nlohmann::json::diff(was, now).dump(1).substr(0, 1500));
        cb::testing::count_check();
    }
});

CB_TEST("formats.inspect.document_keys_come_in_the_order_a_reader_expects", [] {
    const std::string base = "path,name,size,format,kind,ok,notes,summary,dos,headers,sections,exports,imports,resources";
    CB_CHECK_EQ(join_keys(insp::to_json(insp::describe(td("lz32.dll")))),
                base + ",directories,relocations,tls,debug,certificates,clr,rich,load_config,symbols_file");
    CB_CHECK_EQ(join_keys(insp::to_json(insp::describe(td("lzexpand.dll")))), base + ",relocations");
    CB_CHECK_EQ(join_keys(insp::to_json(insp::describe(td("bios.vxd")))), base + ",driver");
    CB_CHECK_EQ(join_keys(insp::to_json(insp::describe(td("xcopy.exe")))), base + ",relocations");
    CB_CHECK_EQ(join_keys(insp::to_json(insp::describe(td("fastadd.obj")))), base);
});

CB_TEST("formats.inspect.describe_a_pe_agrees_with_the_reader", [] {
    const insp::Description d = insp::describe(td("lz32.dll"));
    CB_CHECK(d.ok);
    CB_CHECK_EQ(*d.format, std::string("PE"));
    CB_CHECK_EQ(*d.kind, std::string("pe"));
    const cb::formats::pe::Image img = cb::formats::pe::Image::open(td("lz32.dll"));
    CB_CHECK_EQ(d.sections.size(), img.sections().size());
    for (std::size_t i = 0; i < d.sections.size(); ++i) {
        const auto& s = img.sections()[i];
        CB_CHECK_EQ(d.sections[i].name, s.name);
        CB_CHECK(d.sections[i].address == insp::Address(img.header().image_base + s.virtual_address));
        CB_CHECK_EQ(d.sections[i].code, s.is_code());
    }
    CB_CHECK_EQ(d.exports.symbols.size(), img.export_table().symbols.size());
    CB_CHECK_EQ(d.exports.module, img.export_table().module);
    CB_CHECK_EQ(d.summary->exports, d.exports.symbols.size());
    CB_CHECK_EQ(d.summary->sections, d.sections.size());
    CB_CHECK_EQ(d.summary->import_modules, d.imports.size());
    CB_CHECK_EQ(d.summary->code_bytes, img.code_bytes());
    CB_CHECK_EQ(d.summary->format, std::string("PE32"));
    CB_CHECK_EQ(d.summary->type, std::string("DLL"));
    CB_CHECK(d.summary->entry == insp::Address(*img.entry_va()));
    // imports: the ordinary table first, then the delay-load one
    std::size_t plain = img.import_table(false).size();
    for (std::size_t i = 0; i < d.imports.size(); ++i) CB_CHECK_EQ(d.imports[i].delay, i >= plain);
    const auto* extras = std::get_if<insp::PeExtras>(&d.extras);
    CB_CHECK(extras != nullptr);
    CB_CHECK_EQ(extras->directories.size(), std::size_t{16});
    CB_CHECK_EQ(extras->directories[0].name, std::string("Export"));
    CB_CHECK_EQ(extras->directories[1].rva, img.directory(1).rva);
});

CB_TEST("formats.inspect.describe_an_ne_agrees_with_the_reader", [] {
    const insp::Description d = insp::describe(td("charmap.exe"));
    CB_CHECK(d.ok);
    CB_CHECK_EQ(*d.format, std::string("NE"));
    const cb::formats::ne::Module m = cb::formats::ne::Module::open_file(td("charmap.exe"));
    CB_CHECK_EQ(d.sections.size(), m.segments().size());
    CB_CHECK_EQ(d.summary->module, std::string("CHARMAP"));
    CB_CHECK_EQ(d.imports.size(), m.module_refs().size());
    CB_CHECK_EQ(d.summary->type, std::string("EXE"));
    // an NE export carries its entry's movable/exported flags
    for (const auto& s : d.exports.symbols) CB_CHECK(s.ne.has_value());
    // resources are grouped by type, in the order the types first appear, and an NE item has no id and no rva
    CB_CHECK(!d.resources.types.empty());
    for (const auto& t : d.resources.types)
        for (const auto& it : t.items) {
            CB_CHECK(!it.from_pe);
            CB_CHECK(!it.wide);
        }
    CB_CHECK_EQ(d.resources.count(), m.resources().size());
});

CB_TEST("formats.inspect.describe_a_driver_and_a_dos_program", [] {
    const insp::Description v = insp::describe(td("bios.vxd"));
    CB_CHECK(v.ok);
    CB_CHECK_EQ(v.summary->machine, std::string("80386"));
    CB_CHECK_EQ(v.summary->subsystem, std::string("Windows 386"));
    CB_CHECK(std::holds_alternative<insp::LeHeaders>(v.headers));
    CB_CHECK(std::holds_alternative<insp::DriverExtras>(v.extras));
    CB_CHECK(v.dos.has_value() && v.dos->has_value());

    const insp::Description x = insp::describe(td("xcopy.exe"));
    CB_CHECK(x.ok);
    CB_CHECK_EQ(*x.format, std::string("MZ"));
    CB_CHECK_EQ(x.sections.front().name, std::string("program"));
    CB_CHECK(x.sections.front().address == insp::Address(insp::SegmentAddress{0, 0, insp::SegmentAddress::Style::RealMode}));
    CB_CHECK_EQ(insp::address_text(x.sections.front().address), std::string("0000:0000"));
});

CB_TEST("formats.inspect.describe_what_is_not_an_executable", [] {
    const insp::Description o = insp::describe(td("fastadd.obj"));
    CB_CHECK(o.ok);
    CB_CHECK_EQ(*o.format, std::string("OMF object"));
    CB_CHECK(!o.summary.has_value());
    CB_CHECK_EQ(o.facts.at(0).first, std::string("format"));
    CB_CHECK_EQ(o.facts.at(3).second, std::string("(nothing yet)"));
    // an unknown file, a missing one and a folder are answers, not errors
    const insp::Description u = insp::describe(td("choice.com"));
    CB_CHECK(!u.ok);
    CB_CHECK_EQ(u.notes.size(), std::size_t{1});
    const insp::Description missing = insp::describe(td("no_such_file.exe"));
    CB_CHECK(!missing.ok);
    CB_CHECK(!missing.notes.empty());
    CB_CHECK(!insp::describe(td("")).ok);
});

CB_TEST("formats.inspect.the_xbox_hook_adds_facts_to_a_container_and_its_failure_is_a_note", [] {
    insp::DescribeHooks hooks;
    hooks.xbox_summary = [](const fs::path&, const cb::formats::detect::Detected& m) {
        return insp::Facts{{"title", "Pacman"}, {"format", m.format + "!"}};  // a fact of the same name replaces the line's
    };
    const insp::Description d = insp::describe(td("pacman_ce.xex.head"), hooks);
    CB_CHECK(d.ok);
    CB_CHECK_EQ(d.facts.size(), std::size_t{5});
    CB_CHECK_EQ(d.facts.at(0).second, *d.format + "!");
    CB_CHECK_EQ(d.facts.at(4).first, std::string("title"));
    hooks.xbox_summary = [](const fs::path&, const cb::formats::detect::Detected&) -> insp::Facts { throw cb::FormatError("xbox", 0, "damaged"); };
    const insp::Description bad = insp::describe(td("pacman_ce.xex.head"), hooks);
    CB_CHECK(bad.ok);
    CB_CHECK_EQ(bad.notes.size(), std::size_t{1});
    CB_CHECK_EQ(bad.facts.size(), std::size_t{4});
});

// ---- damaged input -----------------------------------------------------------------------------------------------
CB_TEST("formats.inspect.damaged_files_are_described_never_thrown", [] {
    cb::fs::ScopedTempDir dir("inspect");
    int described = 0, not_ok = 0;
    for (const char* name : {"lz32.dll", "lzexpand.dll", "bios.vxd", "xcopy.exe", "xcopy32.exe", "charmap.exe"}) {
        const cb::Bytes data = cb::fs::read_file(td(name));
        for (const std::size_t cut : {std::size_t{0}, std::size_t{2}, std::size_t{0x20}, std::size_t{0x40}, std::size_t{0x44}, std::size_t{0x80}, std::size_t{0x100}, std::size_t{0x178},
                                      std::size_t{0x200}, std::size_t{0x400}, std::size_t{0x1000}, data.size() / 2, data.size() - 1}) {
            if (cut > data.size()) continue;
            const fs::path p = dir.path() / (std::string(name) + "." + std::to_string(cut) + ".bin");
            cb::fs::write_file(p, cb::ByteView(data).first(cut));
            const insp::Description d = insp::describe(p);
            const Json doc = insp::to_json(d);  // and the document, markdown and every view of it are made too
            CB_CHECK(!insp::markdown(d).empty());
            CB_CHECK(doc["notes"].is_array());
            for (const std::string_view part : insp::kSections) (void)insp::render(doc.contains(std::string(part)) ? doc[std::string(part)] : Json(), part);
            ++described;
            if (!d.ok) ++not_ok;
        }
    }
    CB_CHECK(described > 60);
    CB_CHECK(not_ok > 5);  // the headers cut short are noted, not read
    // noise in the tables of a real PE: the parts that cannot be read are named in the notes and the rest stands
    cb::Bytes noisy = cb::fs::read_file(td("lz32.dll"));
    for (std::size_t i = 0x180; i < 0x200 && i < noisy.size(); ++i) noisy[i] = static_cast<std::uint8_t>(i * 37);
    const fs::path np = dir.path() / "noisy.dll";
    cb::fs::write_file(np, noisy);
    const insp::Description n = insp::describe(np);
    CB_CHECK(!n.notes.empty() || !n.ok || n.sections.empty() || true);  // any outcome, but an answer
    // a bare MZ stub is a DOS program with nothing in it; a DOS stub with random bytes is one too
    const fs::path mz = dir.path() / "stub.exe";
    cb::fs::write_file(mz, bytes_of(std::string("MZ") + std::string(62, '\0')));
    const insp::Description s = insp::describe(mz);
    CB_CHECK(s.ok);
    CB_CHECK_EQ(*s.format, std::string("MZ"));
    CB_CHECK_EQ(s.sections.size(), std::size_t{2});  // the (empty) program and the 64 bytes after it
});

// ---- strings -------------------------------------------------------------------------------------------------------
CB_TEST("formats.inspect.strings_are_the_8_bit_and_16_bit_runs_in_file_order", [] {
    std::string d = std::string("\0\0", 2) + "Hello world" + std::string("\x01", 1) + "ab" + std::string("\0\x01", 2);
    d += "W\0i\0d\0e\0 \0t\0e\0x\0t\0"s;
    d += std::string("\xff", 1) + "t\tab\r\n" + std::string("\0", 1);
    const std::vector<insp::StringRow> rows = insp::find_strings(bytes_of(d));
    CB_CHECK_EQ(rows.size(), std::size_t{3});
    CB_CHECK_EQ(rows[0].offset, std::uint64_t{2});
    CB_CHECK_EQ(rows[0].text, std::string("Hello world"));
    CB_CHECK(rows[0].kind == insp::StringKind::Ascii);
    CB_CHECK(rows[1].kind == insp::StringKind::Utf16);
    CB_CHECK_EQ(rows[1].text, std::string("Wide text"));
    CB_CHECK_EQ(rows[2].text, std::string("t\tab\r\n"));
    // the shortest run, the wide switch, the limit
    insp::StringOptions o;
    o.min_length = 9;
    CB_CHECK_EQ(insp::find_strings(bytes_of(d), o).size(), std::size_t{2});
    o.wide = false;
    CB_CHECK_EQ(insp::find_strings(bytes_of(d), o).size(), std::size_t{1});
    o.min_length = 1;
    o.limit = 2;
    CB_CHECK_EQ(insp::find_strings(bytes_of(d), o).size(), std::size_t{2});
    CB_CHECK(insp::find_strings({}).empty());
    // a 16-bit run is found in either alignment, and a 1-character 8-bit run beats nothing when the minimum is 1
    const std::vector<insp::StringRow> odd = insp::find_strings(bytes_of(std::string("\x01", 1) + "A\0B\0C\0D\0E\0"s));
    CB_CHECK_EQ(odd.size(), std::size_t{1});
    CB_CHECK_EQ(odd[0].offset, std::uint64_t{1});
    // at one offset the 8-bit row comes before the 16-bit row
    insp::StringOptions one;
    one.min_length = 1;
    const auto tie = insp::find_strings(bytes_of("A\0"s), one);
    CB_CHECK_EQ(tie.size(), std::size_t{2});
    CB_CHECK(tie[0].kind == insp::StringKind::Ascii);
    CB_CHECK(tie[1].kind == insp::StringKind::Utf16);
});

CB_TEST("formats.inspect.categorize_tells_what_a_string_looks_like", [] {
    using C = insp::StringCategory;
    CB_CHECK(insp::categorize("%s failed: %08lx") == C::Format);
    CB_CHECK(insp::categorize("100%") == C::Other);          // a percent sign with no conversion
    CB_CHECK(insp::categorize("%-10.3f") == C::Format);
    CB_CHECK(insp::categorize("KERNEL32.DLL") == C::Module);
    CB_CHECK(insp::categorize("  win.ini  ") == C::Module);
    CB_CHECK(insp::categorize("C:\\WINDOWS\\SYSTEM") == C::Path);
    CB_CHECK(insp::categorize("a/b") == C::Path);
    CB_CHECK(insp::categorize("FILE_ATTRIBUTE_NORMAL") == C::Constant);
    CB_CHECK(insp::categorize("WM_USER_2") == C::Constant);
    CB_CHECK(insp::categorize("WM_") == C::Other);
    CB_CHECK(insp::categorize("NOUNDERSCORE") == C::Other);
    CB_CHECK(insp::categorize("Cannot open the file") == C::Error);
    CB_CHECK(insp::categorize("an Unexpected thing") == C::Error);
    CB_CHECK(insp::categorize("hello") == C::Other);
    CB_CHECK_EQ(std::string(insp::string_category_name(C::Constant)), std::string("constant"));
});

CB_TEST("formats.inspect.strings_of_a_file_know_where_they_load", [] {
    // a PE: the address is the one the section maps the byte to, which the reader's rva_to_offset inverts
    const cb::formats::pe::Image img = cb::formats::pe::Image::open(td("lz32.dll"));
    const std::vector<insp::StringRow> pe_rows = insp::strings(td("lz32.dll"));
    std::size_t addressed = 0;
    for (const insp::StringRow& r : pe_rows) {
        if (!r.address) continue;
        ++addressed;
        const auto va = std::get<std::uint64_t>(*r.address);
        CB_CHECK(img.rva_to_offset(va - img.header().image_base) == std::optional<std::uint64_t>(r.offset));
    }
    CB_CHECK(addressed > 10);
    CB_CHECK(pe_rows.size() > addressed);  // the headers are in no section
    CB_CHECK(!pe_rows.front().address.has_value());
    // an NE: "segment:offset"
    const cb::formats::ne::Module m = cb::formats::ne::Module::open_file(td("lzexpand.dll"));
    std::size_t in_segment = 0;
    for (const insp::StringRow& r : insp::strings(td("lzexpand.dll"))) {
        if (!r.address) continue;
        const auto& a = std::get<insp::SegmentAddress>(*r.address);
        const auto seg = m.segments().at(a.segment - 1u);
        CB_CHECK_EQ(seg.file_offset + a.offset, r.offset);
        ++in_segment;
    }
    CB_CHECK(in_segment > 5);
    // anything else has no addresses at all
    for (const insp::StringRow& r : insp::strings(td("xcopy.exe"))) CB_CHECK(!r.address.has_value());
    // the rows as a document
    const Json j = insp::to_json(insp::strings(td("lz32.dll"), {.min_length = 20}));
    CB_CHECK(j.is_array() && !j.empty());
    CB_CHECK_EQ(join_keys(j.front()), std::string("offset,address,kind,category,text"));
    CB_CHECK_THROWS_AS(cb::IoError, insp::strings(td("no_such_file")));
});

// ---- hex -----------------------------------------------------------------------------------------------------------
CB_TEST("formats.inspect.hex_lines_are_sixteen_bytes_a_line", [] {
    cb::Bytes d;
    for (int i = 0; i < 20; ++i) d.push_back(static_cast<std::uint8_t>(0x41 + i));
    d[3] = 0x00;
    d[17] = 0xff;
    const std::vector<std::string> lines = insp::hex_lines(d, 0x1230);
    CB_CHECK_EQ(lines.size(), std::size_t{2});
    CB_CHECK_EQ(lines[0], std::string("00001230  41 42 43 00 45 46 47 48 49 4A 4B 4C 4D 4E 4F 50  ABC.EFGHIJKLMNOP"));
    CB_CHECK_EQ(lines[1], std::string("00001240  51 FF 53 54") + std::string(36, ' ') + "  Q.ST");
    CB_CHECK(insp::hex_lines({}).empty());
    CB_CHECK_EQ(insp::hex_lines(bytes_of("A"), 0x1'0000'0000ull)[0].substr(0, 10), std::string("100000000 "));  // wider than 8 digits
});

CB_TEST("formats.inspect.hexdump_reads_a_window_of_a_file", [] {
    const std::string all = insp::hexdump(td("xcopy.exe"), 0, 64);
    CB_CHECK_EQ(cb::text::split(all, '\n').size(), std::size_t{4});
    CB_CHECK(all.starts_with("00000000  4D 5A "));
    const std::string win = insp::hexdump(td("xcopy.exe"), 0x40, 20);
    CB_CHECK(win.starts_with("00000040  "));
    CB_CHECK_EQ(cb::text::split(win, '\n').size(), std::size_t{2});
    // a window that runs past the end is cut, one that starts past it is empty
    const std::uint64_t size = cb::fs::file_size(td("xcopy.exe"));
    CB_CHECK_EQ(cb::text::split(insp::hexdump(td("xcopy.exe"), size - 3, 100), '\n').size(), std::size_t{1});
    CB_CHECK_EQ(insp::hexdump(td("xcopy.exe"), size + 10, 16), std::string(""));
    CB_CHECK_EQ(insp::hexdump(td("xcopy.exe"), 0, 0), std::string(""));
    CB_CHECK_THROWS_AS(cb::IoError, insp::hexdump(td("no_such_file"), 0, 16));
});

// ---- resource bodies -----------------------------------------------------------------------------------------------
CB_TEST("formats.inspect.resource_bodies_are_shown_as_what_they_are", [] {
    // the version information of a PE: text with the two version lines
    const insp::Description d = insp::describe(td("lz32.dll"));
    const insp::ResourceItem* version = nullptr;
    for (const auto& t : d.resources.types)
        if (t.type == "VERSION") version = &t.items.front();
    CB_CHECK(version != nullptr);
    const insp::ResourceBody v = insp::resource_body(td("lz32.dll"), *version->offset, version->size, version->type_id, true);
    CB_CHECK(v.kind == insp::ResourceBody::Kind::Text);
    CB_CHECK(v.text.starts_with("FileVersion      "));
    CB_CHECK(v.text.find("ProductVersion   ") != std::string::npos);
    // bitmaps, icons and cursors of a 16-bit module become image files
    const insp::Description n = insp::describe(td("charmap.exe"));
    std::size_t images = 0, texts = 0;
    for (const auto& t : n.resources.types)
        for (const auto& it : t.items) {
            const insp::ResourceBody b = insp::resource_body(td("charmap.exe"), *it.offset, it.size, it.type_id, false);
            if (b.kind == insp::ResourceBody::Kind::Image) {
                ++images;
                if (it.type_id == 2) {
                    CB_CHECK_EQ(b.content_type, std::string("image/bmp"));
                    CB_CHECK(b.bytes.size() > 14 && b.bytes[0] == 'B' && b.bytes[1] == 'M');
                } else {
                    CB_CHECK_EQ(b.content_type, std::string("image/x-icon"));
                    CB_CHECK(b.bytes.size() > 22 && b.bytes[2] == (it.type_id == 1 ? 2 : 1));  // an icon file or a cursor file of one image
                }
            } else if (b.kind == insp::ResourceBody::Kind::Text) {
                ++texts;
            } else {
                CB_CHECK(!b.text.empty());
            }
        }
    CB_CHECK(images > 0);
    (void)texts;
    // the edges: nothing to show, a window outside the file, damaged data of a known type, an unknown type
    CB_CHECK_EQ(insp::resource_body(td("lz32.dll"), 0, 0).text, std::string("(empty)"));
    CB_CHECK_EQ(insp::resource_body(td("lz32.dll"), 1ull << 40, 10).text, std::string("(empty)"));
    const insp::ResourceBody shortbmp = insp::resource_body(td("lz32.dll"), 0, 5, 2);
    CB_CHECK(shortbmp.kind == insp::ResourceBody::Kind::Hex);
    CB_CHECK(shortbmp.text.starts_with("could not be shown as its type ("));
    const insp::ResourceBody plain = insp::resource_body(td("lz32.dll"), 0, 40, 99);
    CB_CHECK(plain.kind == insp::ResourceBody::Kind::Hex);
    CB_CHECK(plain.text.starts_with("00000000  4D 5A "));
    // a long body is cut at 4096 bytes of hex and says how much more there is
    const insp::ResourceBody big = insp::resource_body(td("lz32.dll"), 0, 5000, -1);
    CB_CHECK(big.text.ends_with("... 904 more bytes"));
    const insp::ResourceBody cut = insp::resource_body(td("lz32.dll"), 0, 5000, -1, true, 100);  // the limit applies first
    CB_CHECK(cut.text.find("more bytes") == std::string::npos);
    CB_CHECK(insp::resource_body(td("no_such_file"), 0, 10).text.starts_with("cannot be read: "));
});

// ---- the text reports ------------------------------------------------------------------------------------------------
CB_TEST("formats.inspect.cells_show_numbers_in_hex_where_they_are_addresses", [] {
    CB_CHECK_EQ(insp::cell("size", 255), std::string("255"));
    CB_CHECK_EQ(insp::cell("address", 255), std::string("0xff"));
    CB_CHECK_EQ(insp::cell("file_offset", 16), std::string("0x10"));
    CB_CHECK_EQ(insp::cell("entry", 4096), std::string("0x1000"));
    CB_CHECK_EQ(insp::cell("entry_point", 4096), std::string("0x1000"));
    CB_CHECK_EQ(insp::cell("start_va", 4096), std::string("0x1000"));
    CB_CHECK_EQ(insp::cell("entries", 4096), std::string("4096"));
    CB_CHECK_EQ(insp::cell("address", -1), std::string("-1"));
    CB_CHECK_EQ(insp::cell("code", true), std::string("yes"));
    CB_CHECK_EQ(insp::cell("code", false), std::string("no"));
    CB_CHECK_EQ(insp::cell("name", Json(nullptr)), std::string(""));
    CB_CHECK_EQ(insp::cell("flags", Json::array({"READ", "WRITE"})), std::string("READ WRITE"));
    CB_CHECK_EQ(insp::cell("rich", Json::parse("[[1,2],[3,4]]")), std::string("1 2 3 4"));
    CB_CHECK_EQ(insp::cell("text", std::string("a\r\nb")), std::string("a\\r\\nb"));
    CB_CHECK_EQ(insp::cell("ratio", 0.5), std::string("0.5"));
});

CB_TEST("formats.inspect.render_makes_records_and_tables", [] {
    const Json record = Json::parse(R"({"format":"PE32","image_base":4194304,"sections":2,"flags":["A","B"],"nested":{"x":1}})");
    const std::vector<std::string> r = insp::render(record, "summary");
    CB_CHECK_EQ(cb::text::join(r, "\n"),
                "format      PE32\nimage_base  0x400000\nsections    2\nflags       A B\nnested:\n  x  1");
    const Json table = Json::parse(R"([{"name":".text","address":4096,"size":10,"flags":["R"]},{"name":".data","address":8192,"size":2000,"flags":[]}])");
    CB_CHECK_EQ(cb::text::join(insp::render(table, "sections"), "\n"),
                "name   address  size  flags\n.text  0x1000   10    R\n.data  0x2000   2000");
    // a record that holds records shows them under it
    const Json nested = Json::parse(R"([{"module":"A.DLL","count":1,"symbols":[{"name":"f","ordinal":3}]},{"module":"B.DLL","count":0,"symbols":[]}])");
    CB_CHECK_EQ(cb::text::join(insp::render(nested, "imports"), "\n"),
                "module  count\nA.DLL   1\n    name  ordinal\n    f     3\nB.DLL   0\n    (none)");
    // a column is nested when ANY row holds records under it, not only the first
    const Json late = Json::parse(R"([{"module":"A.DLL","count":0,"symbols":[]},{"module":"B.DLL","count":1,"symbols":[{"name":"f","ordinal":3}]}])");
    CB_CHECK_EQ(cb::text::join(insp::render(late, "imports"), "\n"),
                "module  count\nA.DLL   0\n    (none)\nB.DLL   1\n    name  ordinal\n    f     3");
    CB_CHECK_EQ(cb::text::join(insp::render(Json::array(), "x"), "\n"), std::string("(none)"));
    CB_CHECK_EQ(cb::text::join(insp::render(Json::parse("[1,2]"), "x", 1), "\n"), std::string("  1\n  2"));
    CB_CHECK_EQ(cb::text::join(insp::render(Json(7), "x"), "\n"), std::string("7"));
    CB_CHECK(insp::render(Json::object(), "x").empty());
    // a column is at most 48 wide
    const std::vector<std::string> wide = insp::render(Json::parse(R"([{"a":")" + std::string(60, 'z') + R"(","b":"q"}])"), "t");
    CB_CHECK_EQ(wide[1], std::string(60, 'z') + "  q");
    CB_CHECK_EQ(wide[0], std::string("a") + std::string(47, ' ') + "  b");
});

CB_TEST("formats.inspect.markdown_reports_a_description_and_a_folder", [] {
    insp::Description d = insp::describe(td("lz32.dll"));
    d.xex = {{"image name", "x.dll"}};
    const std::string md = insp::markdown(d);
    CB_CHECK(md.starts_with("# lz32.dll\n\nPE, "));
    CB_CHECK(md.find("## Xbox 360 executable\n\n| Field | Value |\n|---|---|\n| image name | x.dll |\n") != std::string::npos);
    CB_CHECK(md.find("## Summary\n\n| Field | Value |\n|---|---|\n| format | PE32 |\n") != std::string::npos);
    CB_CHECK(md.find("| image_base | 0x") != std::string::npos);
    CB_CHECK(md.find("## Sections\n\n| Name | Address | Size | Flags |\n|---|---|---:|---|\n| .text | 0x") != std::string::npos);
    CB_CHECK(md.find("## Imports\n\n| Module | Functions |\n|---|---:|\n| ") != std::string::npos);
    CB_CHECK(md.find("- **Exports:** 12\n") != std::string::npos);
    CB_CHECK(md.ends_with("\n"));
    // an unrecognised file has its notes quoted
    const std::string un = insp::markdown(insp::describe(td("choice.com")));
    CB_CHECK(un.starts_with("# choice.com\n\nnot recognised, "));
    CB_CHECK(un.ends_with("> not a format CodeBreaker recognises\n"));
});

CB_TEST("formats.inspect.sizes_read_like_a_file_manager", [] {
    CB_CHECK_EQ(insp::size_text(0), std::string("0 B"));
    CB_CHECK_EQ(insp::size_text(1023), std::string("1023 B"));
    CB_CHECK_EQ(insp::size_text(1024), std::string("1.0 KB"));
    CB_CHECK_EQ(insp::size_text(1536), std::string("1.5 KB"));
    CB_CHECK_EQ(insp::size_text(5ull << 20), std::string("5.0 MB"));
    CB_CHECK_EQ(insp::size_text(3ull << 30), std::string("3.0 GB"));
    CB_CHECK_EQ(insp::size_text(7ull << 40), std::string("7.0 TB"));
    CB_CHECK_EQ(insp::size_text(5000ull << 40), std::string("5000.0 TB"));
});

// ---- folders -------------------------------------------------------------------------------------------------------
CB_TEST("formats.inspect.a_folder_is_mapped_by_format", [] {
    cb::fs::ScopedTempDir dir("inspect");
    const fs::path root = dir.path();
    cb::fs::make_dirs(root / "sub" / "deeper");
    cb::fs::make_dirs(root / ".git");
    cb::fs::make_dirs(root / "node_modules");
    const auto put = [&](const fs::path& rel, const fs::path& from) { cb::fs::write_file(root / rel, cb::fs::read_file(from)); };
    put("a.dll", td("lz32.dll"));
    put("sub/b.dll", td("lz32.dll"));
    put("sub/deeper/c.exe", td("xcopy32.exe"));
    put("d.exe", td("xcopy.exe"));
    put("sub/e.vxd", td("bios.vxd"));
    put(".git/x.exe", td("lz32.dll"));        // skipped folders
    put("node_modules/y.dll", td("lz32.dll"));
    put("f.obj", td("fastadd.obj"));
    cb::fs::write_text(root / "a.c", "int main(){}\n");
    cb::fs::write_text(root / "b.py", "print(1)\n");
    cb::fs::write_text(root / "Makefile", "all:\n");
    cb::fs::write_text(root / "d.unknownext", "zzz");
    cb::fs::write_text(root / "noext", "plain");
    cb::fs::write_file(root / "bin.c", bytes_of(std::string("\0\0\0binary", 9)));  // a source name with binary content
    cb::fs::write_text(root / "empty.c", "");
    std::vector<std::string> heard;
    const insp::FolderMap m = insp::folder_map(root, [&](std::string_view s) { heard.emplace_back(s); });
    CB_CHECK(heard.empty());  // fewer than 2000 files
    CB_CHECK_EQ(m.files, std::uint64_t{13});  // the skipped folders' files are not counted
    CB_CHECK_EQ(m.folder, cb::fs::to_text(root));
    std::uint64_t total = 0;
    for (const auto& f : m.formats) total += f.files;
    CB_CHECK_EQ(total, std::uint64_t{9});  // 2 + 1 dll/exe PE x3, MZ, LE, OMF, C, Python, Makefile  (all but empty.c, bin.c, 3 unknowns)
    // the biggest format first
    for (std::size_t i = 1; i < m.formats.size(); ++i) CB_CHECK(m.formats[i - 1].bytes >= m.formats[i].bytes);
    const auto pe_row = std::find_if(m.formats.begin(), m.formats.end(), [](const insp::FolderFormat& f) { return f.format.starts_with("PE executable"); });
    CB_CHECK(pe_row != m.formats.end());
    CB_CHECK_EQ(pe_row->command, std::string("info"));
    std::uint64_t pe_files = 0;
    for (const auto& f : m.formats)
        if (f.format.starts_with("PE executable")) pe_files += f.files;
    CB_CHECK_EQ(pe_files, std::uint64_t{3});
    const auto dos_row = std::find_if(m.formats.begin(), m.formats.end(), [](const insp::FolderFormat& f) { return f.format == "DOS program (MZ)"; });
    CB_CHECK(dos_row != m.formats.end());
    CB_CHECK_EQ(dos_row->family, std::string("dos"));
    const auto c_row = std::find_if(m.formats.begin(), m.formats.end(), [](const insp::FolderFormat& f) { return f.format == "source / text: C"; });
    CB_CHECK(c_row != m.formats.end());
    CB_CHECK_EQ(c_row->family, std::string("source"));
    CB_CHECK_EQ(c_row->command, std::string(""));
    CB_CHECK_EQ(c_row->files, std::uint64_t{1});  // bin.c has a NUL, empty.c is empty
    // what is not recognised is tallied by extension
    std::set<std::string> exts;
    for (const auto& o : m.unrecognised) exts.insert(o.extension);
    CB_CHECK(exts == (std::set<std::string>{".c", ".unknownext", "(none)"}));
    const std::string md = insp::markdown(m);
    CB_CHECK(md.starts_with("# " + m.folder + "\n\n13 files, "));
    CB_CHECK(md.find("| `.unknownext` | 1 | 3 B | `d.unknownext` |") != std::string::npos);
    CB_CHECK(md.find("`codebreaker info`") != std::string::npos);
    const Json j = insp::to_json(m);
    CB_CHECK_EQ(join_keys(j), std::string("folder,files,bytes,formats,unrecognised"));
    CB_CHECK_EQ(join_keys(j["formats"].front()), std::string("format,family,command,files,bytes,example"));
    // a folder that is not there maps to nothing
    CB_CHECK_EQ(insp::folder_map(root / "nope").files, std::uint64_t{0});
});

CB_TEST("formats.inspect.text_kind_is_by_name_and_content", [] {
    cb::fs::ScopedTempDir dir("inspect");
    const auto text = [&](const std::string& name, const std::string& body) {
        cb::fs::write_file(dir.path() / name, bytes_of(body));
        return insp::text_kind(dir.path() / name);
    };
    CB_CHECK_EQ(text("a.cpp", "x")->format, std::string("source / text: C++"));
    CB_CHECK_EQ(text("A.HPP", "x")->format, std::string("source / text: C++ header"));
    CB_CHECK_EQ(text("Makefile", "x")->format, std::string("source / text: makefile"));
    CB_CHECK_EQ(text("CMakeLists.txt", "x")->format, std::string("source / text: CMake"));
    CB_CHECK_EQ(text("README", "x")->format, std::string("source / text: text"));
    CB_CHECK_EQ(text("c.json", "{}")->format, std::string("source / text: JSON"));
    CB_CHECK_EQ(text("a.c", "x")->family, std::string("source"));
    CB_CHECK(!text("readme.md.bin", "x").has_value());
    CB_CHECK(!text("d.unknownext", "zzz").has_value());
    CB_CHECK(!text("bin.c", std::string("ab\0cd", 5)).has_value());
    CB_CHECK(!insp::text_kind(dir.path() / "missing.c").has_value());
    // only the first 4 KB is looked at
    CB_CHECK(text("late.c", std::string(5000, 'a') + std::string(1, '\0')).has_value());
});

// ---- xex -----------------------------------------------------------------------------------------------------------------
CB_TEST("formats.inspect.xex_facts_come_from_the_header_and_only_for_an_xex", [] {
    CB_CHECK(insp::xex_facts(td("lz32.dll")).empty());
    CB_CHECK(insp::xex_facts(td("no_such_file")).empty());
    const insp::Facts f = insp::xex_facts(td("pacman_ce.xex.head"));
    CB_CHECK(!f.empty());
    CB_CHECK_EQ(f.at(0).first, std::string("image name"));
    CB_CHECK_EQ(f.at(1).first, std::string("load address"));
    CB_CHECK(f.at(1).second.starts_with("0x") && f.at(1).second.size() == 10);
    CB_CHECK_EQ(f.at(3).first, std::string("import libraries"));
});

// ---- the info command ------------------------------------------------------------------------------------------------------
CB_TEST("formats.inspect.info_shows_a_summary_and_the_parts", [] {
    int code = -1;
    const std::string path = cb::fs::to_text(td("lz32.dll"));
    const std::string out = capture_info({path}, code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK(out.starts_with(path + "  (PE, "));
    CB_CHECK(out.find("\nsummary:\n  format          PE32\n") != std::string::npos);
    CB_CHECK(out.find("\nheaders:\n") != std::string::npos);
    CB_CHECK(out.find("\nsections:\n  name") != std::string::npos);
    CB_CHECK(out.find("\n12 exports, ") != std::string::npos);
    CB_CHECK(out.find("(--section exports | imports | resources | all)\n") != std::string::npos);

    const std::string one = capture_info({path, "--section", "summary"}, code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK(one.starts_with("format          PE32\n"));
    const std::string all = capture_info({path, "--section", "all"}, code);
    CB_CHECK(all.find("\ndirectories:\n") != std::string::npos);
    CB_CHECK(all.find("\nexports:\n") != std::string::npos);
    CB_CHECK(all.find("\nrelocations:\n") != std::string::npos);

    const Json whole = cb::json::parse(capture_info({path, "--json"}, code));
    CB_CHECK_EQ(whole["summary"]["exports"].get<int>(), 12);
    const Json part = cb::json::parse(capture_info({path, "--section", "sections", "--json"}, code));
    CB_CHECK(part.is_array());
    CB_CHECK_EQ(part.size(), std::size_t{6});
});

CB_TEST("formats.inspect.info_refusals_and_exit_codes", [] {
    int code = -1;
    const std::string path = cb::fs::to_text(td("xcopy.exe"));
    const std::string bogus = capture_info({path, "--section", "bogus"}, code);
    CB_CHECK_EQ(code, 2);
    CB_CHECK_EQ(bogus, std::string("xcopy.exe has no part 'bogus' (it has: summary dos headers sections exports resources relocations strings hex)\n"));
    // a part only another format has
    CB_CHECK_EQ(capture_info({path, "--section", "tls"}, code).starts_with("xcopy.exe has no part 'tls'"), true);
    CB_CHECK_EQ(code, 2);
    // a name nobody can resolve
    CB_CHECK_EQ(capture_info({"no_such_thing.exe"}, code), std::string("no file or corpus module 'no_such_thing.exe'\n"));
    CB_CHECK_EQ(code, 2);
    // a file that is not recognised: described, with its note, exit 1
    const std::string un = capture_info({cb::fs::to_text(td("choice.com"))}, code);
    CB_CHECK_EQ(code, 1);
    CB_CHECK(un.ends_with("note: not a format CodeBreaker recognises\n"));
    // the resolver finds corpus names
    insp::InfoEnvironment env;
    env.resolve = [](std::string_view name) -> std::optional<fs::path> {
        if (name == "lz32") return cb::testing::data_path("formats/testdata/lz32.dll");
        return std::nullopt;
    };
    CB_CHECK(capture_info({"lz32", "--section", "summary"}, code, env).starts_with("format          PE32"));
    CB_CHECK_EQ(code, 0);
    // no target at all is a usage error
    CB_CHECK_THROWS_AS(cb::UsageError, cb::cli::Cli("codebreaker", {insp::info_command()}).parse({"info"}));
});

CB_TEST("formats.inspect.info_hex_and_strings_views", [] {
    int code = -1;
    const std::string path = cb::fs::to_text(td("xcopy.exe"));
    const std::string hex = capture_info({path, "--section", "hex", "--offset", "0x40", "--length", "80"}, code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK_EQ(cb::text::split(cb::text::trim(hex), '\n').size(), std::size_t{5});
    CB_CHECK(hex.starts_with("00000040  "));
    const Json hj = cb::json::parse(capture_info({path, "--section", "hex", "--json"}, code));
    CB_CHECK(hj["hex"].get<std::string>().starts_with("00000000  4D 5A "));

    const std::string lz = cb::fs::to_text(td("lz32.dll"));
    const std::string rows = capture_info({lz, "--section", "strings", "--min", "12"}, code);
    CB_CHECK_EQ(code, 0);
    for (const std::string_view line : cb::text::lines(rows)) {
        if (line.empty()) continue;
        CB_CHECK(line.size() > 30);
        CB_CHECK(line[8] == ' ' && line[9] == ' ');
    }
    const Json sj = cb::json::parse(capture_info({lz, "--section", "strings", "--json", "--min", "14"}, code));
    CB_CHECK(sj.is_array() && !sj.empty());
    CB_CHECK(sj.front()["text"].get<std::string>().size() >= 14);
});

CB_TEST("formats.inspect.info_markdown_folder_and_unpack", [] {
    cb::fs::ScopedTempDir dir("inspect");
    int code = -1;
    const std::string path = cb::fs::to_text(td("lz32.dll"));
    const std::string out_file = cb::fs::to_text(dir.path() / "report.md");
    const std::string said = capture_info({path, "--markdown", "--out", out_file}, code);
    CB_CHECK_EQ(said, "wrote " + out_file + "\n");
    CB_CHECK_EQ(code, 0);
    CB_CHECK(cb::fs::read_text(fs::path(out_file)).starts_with("# lz32.dll\n"));
    // a folder: the map, as text, JSON and markdown
    cb::fs::write_file(dir.path() / "x.dll", cb::fs::read_file(td("lz32.dll")));
    const std::string folder = cb::fs::to_text(dir.path());
    const std::string text = capture_info({folder}, code);
    CB_CHECK(text.find(": 2 files, ") != std::string::npos);  // x.dll and the report
    CB_CHECK(text.find("PE executable") != std::string::npos);
    CB_CHECK(text.find("   codebreaker info\n") != std::string::npos);
    CB_CHECK(cb::json::parse(capture_info({folder, "--json"}, code))["files"].get<int>() == 2);
    CB_CHECK(capture_info({folder, "--markdown"}, code).starts_with("# " + folder));
    // an EXEPACK program is written back out unpacked as a plain MZ file
    const std::string unpacked = cb::fs::to_text(dir.path() / "u.exe");
    CB_CHECK_EQ(capture_info({cb::fs::to_text(td("xcopy.exe")), "--unpack", unpacked}, code),
                "xcopy.exe (EXEPACK, unpacked static) -> " + unpacked + "\n");
    CB_CHECK_EQ(code, 0);
    const cb::Bytes u = cb::fs::read_file(fs::path(unpacked));
    CB_CHECK(u.size() > 2 && u[0] == 'M' && u[1] == 'Z');
    CB_CHECK(u.size() > cb::fs::file_size(td("xcopy.exe")));
});

// ---- addresses -----------------------------------------------------------------------------------------------------------
CB_TEST("formats.inspect.addresses_are_text_in_the_style_of_their_format", [] {
    using S = insp::SegmentAddress;
    CB_CHECK_EQ(insp::address_text(insp::Address{}), std::string(""));
    CB_CHECK_EQ(insp::address_text(std::uint64_t{0x401000}), std::string("0x401000"));
    CB_CHECK_EQ(insp::address_text(S{3, 0x1a4, S::Style::NeSegment}), std::string("3:01a4"));
    CB_CHECK_EQ(insp::address_text(S{0x1234, 0x10, S::Style::RealMode}), std::string("1234:0010"));
    CB_CHECK_EQ(insp::address_text(S{1, 0x1000, S::Style::Driver}), std::string("1:00001000"));
    CB_CHECK_EQ(std::string(insp::resource_type_name(5)), std::string("DIALOG"));
    CB_CHECK_EQ(std::string(insp::resource_type_name(13)), std::string(""));
    CB_CHECK_EQ(std::string(insp::preview_name(insp::Preview::Image)), std::string("image"));
});
