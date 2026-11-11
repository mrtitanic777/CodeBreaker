#include "cb/formats/resources.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::resources {
namespace {

using J = nlohmann::json;   // (unordered: objects compare by content)

std::filesystem::path fixture(std::string_view name) {
    return testing::data_path(std::string("formats/testdata/resources/") + std::string(name));
}
Bytes load(std::string_view name) { return fs::read_file(fixture(name)); }
J load_json(std::string_view name) { return J::parse(fs::read_text(fixture(name))); }

// ---- the typed values in the shape the old reader gave (the golden files are in that shape) -----------------

J ident_json(const Ident& i) {
    if (const auto* n = std::get_if<std::uint32_t>(&i)) return *n;
    return std::get<std::string>(i);
}

std::string bytes_tag(ByteView b) { return std::format("b:{}:{:08x}", b.size(), crypto::crc32(b)); }

J dialog_json(const Dialog& d) {
    J controls = J::array();
    for (const DialogControl& c : d.controls) {
        controls.push_back({{"class", ident_json(c.window_class)}, {"cx", c.cx}, {"cy", c.cy}, {"exstyle", c.exstyle},
                            {"extra", bytes_tag(c.extra)}, {"help", c.help}, {"id", c.id}, {"style", c.style},
                            {"text", ident_json(c.text)}, {"x", c.x}, {"y", c.y}});
    }
    J font = nullptr;
    if (d.font) {
        font = d.extended ? J::array({d.font->points, d.font->weight, d.font->italic, d.font->charset, d.font->face})
                          : J::array({d.font->points, d.font->face});
    }
    return {{"class", ident_json(d.window_class)}, {"controls", controls}, {"cx", d.cx}, {"cy", d.cy}, {"exstyle", d.exstyle},
            {"extended", d.extended}, {"font", font}, {"help", d.help}, {"menu", ident_json(d.menu)}, {"style", d.style},
            {"title", d.title}, {"x", d.x}, {"y", d.y}};
}

J items_json(const std::vector<MenuItem>& items) {
    J out = J::array();
    for (const MenuItem& i : items) {
        out.push_back({{"flags", i.flags}, {"id", i.id}, {"items", i.items ? items_json(*i.items) : J(nullptr)}, {"text", i.text}});
    }
    return out;
}
J items_json(const std::vector<ExtMenuItem>& items) {
    J out = J::array();
    for (const ExtMenuItem& i : items) {
        out.push_back({{"help", i.help}, {"id", i.id}, {"items", i.items ? items_json(*i.items) : J(nullptr)}, {"state", i.state},
                       {"text", i.text}, {"type", i.type}});
    }
    return out;
}

J version_json(const Version& v) {
    J fixed = nullptr;
    if (v.fixed) {
        const FixedFileInfo& f = *v.fixed;
        fixed = J::array({f.signature, f.struct_version, f.file_version_ms, f.file_version_ls, f.product_version_ms, f.product_version_ls,
                          f.file_flags_mask, f.file_flags, f.os, f.type, f.subtype});   // (the date is dropped by decode())
    }
    J strings = J::object();
    for (const VersionStringTable& t : v.strings) {
        J entries = J::array();
        for (const auto& [k, text] : t.entries) entries.push_back(J::array({k, text}));
        strings[t.language_block] = entries;
    }
    return J::array({fixed, strings, J(v.translations)});
}

J decoded_json(const Decoded& d) {
    struct V {
        J operator()(const std::vector<std::string>& s) const { return J(s); }
        J operator()(const Dialog& x) const { return dialog_json(x); }
        J operator()(const Menu& m) const {
            return {{"extended", m.extended}, {"help", m.help},
                    {"items", std::visit([](const auto& i) { return items_json(i); }, m.items)}};
        }
        J operator()(const std::vector<Accelerator>& a) const {
            J out = J::array();
            for (const Accelerator& x : a) out.push_back(J::array({x.flags, x.key, x.id}));
            return out;
        }
        J operator()(const Version& v) const { return version_json(v); }
    };
    return std::visit(V{}, d);
}

Resource resource(std::uint32_t type, Bytes data) {
    Resource r;
    r.type = type;
    r.name = std::uint32_t{1};
    r.data = std::move(data);
    return r;
}

// ---- real resources ---------------------------------------------------------------------------------------

CB_TEST("formats.resources.real_32bit_resources_decode_as_the_old_reader_decoded_them", [] {
    const J golden = load_json("golden.json");
    CB_CHECK_EQ(golden.size(), 20u);
    int checked = 0;
    for (const auto& [file, entry] : golden.items()) {
        const std::string type = entry["type"];
        const std::uint32_t id = type == "DIALOG" ? kDialog : type == "MENU" ? kMenu : type == "STRING" ? kString
                                 : type == "ACCELERATOR" ? kAccelerator : kVersion;
        const Resource r = resource(id, load(file));
        const std::optional<Decoded> got = decode(r, entry["wide"]);
        CB_CHECK(got.has_value());
        if (!got) continue;
        const J want = entry["decoded"];
        const J have = decoded_json(*got);
        if (have != want) {
            testing::record_failure(__FILE__, __LINE__, file + " decoded differently:\n  want " + want.dump().substr(0, 600) + "\n  have " + have.dump().substr(0, 600));
        }
        ++checked;
    }
    CB_CHECK_EQ(checked, 20);
});

CB_TEST("formats.resources.a_dialog_in_korean_and_what_the_controls_are", [] {
    const Dialog d = dialog(load("comctl32_dialog_1006.bin"), true);
    CB_CHECK(!d.extended);
    CB_CHECK(!d.controls.empty());
    // the strings are real UTF-8 text, not code units
    bool non_ascii = false;
    for (const DialogControl& c : d.controls) {
        if (const auto* s = std::get_if<std::string>(&c.text)) {
            for (unsigned char ch : *s) non_ascii |= ch >= 0x80;
        }
    }
    CB_CHECK(non_ascii || !d.title.empty());
    const Dialog w = dialog(load("wmsui32_dialog_2202.bin"), true);
    CB_CHECK_EQ(w.controls.size(), 1u);
    CB_CHECK_EQ(std::get<std::string>(w.controls[0].window_class), "RichEdit");
    CB_CHECK(w.font.has_value());
    CB_CHECK_EQ(w.font->points, 8);
    CB_CHECK_EQ(w.font->face, "MS Shell Dlg");
    CB_CHECK_EQ(w.controls[0].id, 2235u);
});

CB_TEST("formats.resources.the_version_block_of_a_real_module", [] {
    const Version v = version(load("shell32_version_1.bin"), true);
    CB_CHECK(v.fixed.has_value());
    CB_CHECK_EQ(v.fixed->signature, 0xFEEF04BDu);
    CB_CHECK_EQ(v.strings.size(), 1u);
    CB_CHECK_EQ(v.strings[0].language_block, "040904E4");
    const auto company = std::find_if(v.strings[0].entries.begin(), v.strings[0].entries.end(), [](const auto& e) { return e.first == "CompanyName"; });
    CB_CHECK(company != v.strings[0].entries.end());
    CB_CHECK_EQ(company->second, "Microsoft Corporation");
    CB_CHECK_EQ(v.translations.size(), 2u);
    CB_CHECK_EQ(v.translations[0], 0x0409);   // US English, code page 1252
    CB_CHECK_EQ(v.translations[1], 1252);
});

// ---- a real 16-bit module ----------------------------------------------------------------------------------

CB_TEST("formats.resources.the_resource_table_of_a_real_16bit_module", [] {
    const Bytes file = load("SETUP.EXE");
    CB_CHECK(kind_of(file) == ExecutableKind::Ne);
    const J golden = load_json("SETUP.golden.json");
    const std::vector<Resource> got = read_ne(file);
    CB_CHECK_EQ(got.size(), 3u);
    const J rows = golden["read"];
    for (std::size_t i = 0; i < got.size() && i < rows.size(); ++i) {
        // the old row: [type name, type id, name, language, flags, size, crc32, repr, key]
        CB_CHECK_EQ(type_name(got[i].type), rows[i][0].get<std::string>());
        CB_CHECK_EQ(std::get<std::uint32_t>(got[i].type), rows[i][1].get<std::uint32_t>());
        CB_CHECK_EQ(std::get<std::uint32_t>(got[i].name), rows[i][2].get<std::uint32_t>());
        CB_CHECK(!got[i].language.has_value());
        CB_CHECK_EQ(got[i].flags, rows[i][4].get<std::uint32_t>());
        CB_CHECK_EQ(got[i].data.size(), rows[i][5].get<std::size_t>());
        CB_CHECK_EQ(crypto::crc32(got[i].data), rows[i][6].get<std::uint32_t>());
        CB_CHECK_EQ(got[i].describe(), rows[i][7].get<std::string>());
    }
    // the narrow version block
    const J want = golden["decode"][1];
    const auto decoded = decode(got[1], false);
    CB_CHECK(decoded.has_value());
    CB_CHECK(decoded_json(*decoded) == want);
    CB_CHECK(!decode(got[0], false).has_value());   // an icon group is not a kind decode() reads
    // the group and its icon make the .ico the old reader made (774 bytes)
    const Bytes ico = icon_file(got[0].data, [&](std::uint16_t id) {
        for (const Resource& r : got) {
            if (std::get<std::uint32_t>(r.type) == kIcon && std::get<std::uint32_t>(r.name) == id) return r.data;
        }
        throw FormatError("test", 0, "no such icon member");
    });
    const std::string tag = golden["media"][0][1];
    CB_CHECK_EQ(bytes_tag(ico), tag);
    CB_CHECK_EQ(ico.size(), 774u);
    CB_CHECK_EQ(ico[2], 1);   // an icon file
    CB_CHECK_EQ(ico[4], 1);   // with one image
    CB_CHECK_EQ(ico[6], 32);  // 32 x 32
    CB_CHECK_EQ(ico[7], 32);
    const Group g = group(got[0].data);
    CB_CHECK(g.kind == GroupKind::Icons);
    CB_CHECK_EQ(g.entries.size(), 1u);
    CB_CHECK_EQ(g.entries[0].width, 32);
    CB_CHECK_EQ(g.entries[0].colours, 16);
    CB_CHECK_EQ(g.entries[0].bits, 4);
    CB_CHECK_EQ(g.entries[0].bytes_in_resource, 744u);
    CB_CHECK_EQ(g.entries[0].member_id, 1);
});

CB_TEST("formats.resources.a_resource_table_cut_by_the_file", [] {
    // The table is at the start of SETUP.EXE's data; cutting the file inside a resource gives it short, and cutting the
    // table gives the resources before the cut.
    Bytes file = load("SETUP.EXE");
    const std::size_t full = read_ne(file).size();
    file.resize(file.size() - 100);
    const std::vector<Resource> cut = read_ne(file);
    CB_CHECK_EQ(cut.size(), full);
    CB_CHECK(cut.back().data.size() < 752);
    CB_CHECK_THROWS_AS(FormatError, (void)read_ne(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)read_ne(Bytes(100, 0)));
});

// ---- bitmaps, cursors ----------------------------------------------------------------------------------------

CB_TEST("formats.resources.a_bitmap_gets_its_file_header", [] {
    const Bytes dib = load("comctl32_bitmap_133.bin");
    const Bytes bmp = bitmap_file(dib);
    const J gold = load_json("media.golden.json");
    CB_CHECK_EQ(bytes_tag(bmp), gold["bitmap"]["result"].get<std::string>());
    CB_CHECK_EQ(bmp.size(), dib.size() + 14);
    CB_CHECK_EQ(bmp[0], 'B');
    CB_CHECK_EQ(bmp[1], 'M');
    io::ByteReader r(bmp, "test");
    r.seek(2);
    CB_CHECK_EQ(r.u32(), bmp.size());
    r.seek(10);
    const std::uint32_t pixels = r.u32();
    // 14 + the 40-byte header + the palette of a 4-bit or 8-bit image
    CB_CHECK(pixels >= 14 + 40 && pixels < bmp.size());
});

CB_TEST("formats.resources.bitmap_palettes", [] {
    // a core (12-byte) header with 8 bits: 256 colours of 3 bytes
    Bytes core(12 + 4, 0);
    core[0] = 12;
    core[10] = 8;
    io::ByteReader r(bitmap_file(core), "test");
    r.seek(10);
    CB_CHECK_EQ(r.u32(), 14u + 12u + 3u * 256u);
    // a 40-byte header, 24 bits, no colours used: no palette
    Bytes dib(40 + 4, 0);
    dib[0] = 40;
    dib[14] = 24;
    io::ByteReader r2(bitmap_file(dib), "test");
    r2.seek(10);
    CB_CHECK_EQ(r2.u32(), 14u + 40u);
    // bit-fields compression (3) with a 40-byte header: three masks follow
    dib[16] = 3;
    dib[14] = 16;
    io::ByteReader r3(bitmap_file(dib), "test");
    r3.seek(10);
    CB_CHECK_EQ(r3.u32(), 14u + 40u + 12u);
    // data too short for a header: no palette, no error
    io::ByteReader r4(bitmap_file(Bytes{1, 2, 3}), "test");
    r4.seek(10);
    CB_CHECK_EQ(r4.u32(), 14u);
});

CB_TEST("formats.resources.a_cursor_group_makes_a_cur_file", [] {
    const J gold = load_json("media.golden.json");
    const Bytes group_data = load("comctl32_groupcursor_102.bin");
    const Bytes cursor = load("comctl32_cursor_1.bin");
    const Group g = group(group_data);
    CB_CHECK(g.kind == GroupKind::Cursors);
    CB_CHECK_EQ(g.entries[0].width, 32);
    CB_CHECK_EQ(g.entries[0].height, 64);
    CB_CHECK_EQ(g.entries[0].colours, 0);
    CB_CHECK_EQ(g.entries[0].bytes_in_resource, 308u);
    const Bytes cur = icon_file(group_data, [&](std::uint16_t id) {
        CB_CHECK_EQ(id, 1);
        return cursor;
    });
    CB_CHECK_EQ(bytes_tag(cur), gold["cursor_group"]["result"].get<std::string>());
    CB_CHECK_EQ(cur.size(), 326u);          // 6 + 16 + the member without its 4-byte hot spot
    CB_CHECK_EQ(cur[2], 2);                 // a cursor file
    CB_CHECK_EQ(cur[6], 32);                // its directory: 32 wide
    CB_CHECK_EQ(cur[7], 32);                // the cursor resource counts the mask: 64 rows is a 32-high cursor
    CB_CHECK_EQ(cur[10], cursor[0]);        // the hot spot moved into the directory
    CB_CHECK_EQ(cur[12], cursor[2]);
    CB_CHECK(Bytes(cur.begin() + 22, cur.end()) == Bytes(cursor.begin() + 4, cursor.end()));
});

CB_TEST("formats.resources.group_errors", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)group(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)group(Bytes{0, 0, 7, 0, 0, 0}));                        // not icons or cursors
    CB_CHECK_THROWS_AS(FormatError, (void)group(Bytes{0, 0, 1, 0, 5, 0, 1, 2, 3}));              // five entries, none there
    CB_CHECK_THROWS_AS(FormatError, (void)icon_file(load("comctl32_groupcursor_102.bin"), [](std::uint16_t) { return Bytes{1}; }));   // no hot spot
});

// ---- type libraries ------------------------------------------------------------------------------------------

CB_TEST("formats.resources.a_type_library_describes_itself", [] {
    const J both = load_json("stdole2_typelib.golden.json");   // [the description, the outline the old reader rendered]
    const J gold = both[0];
    const std::optional<TypeLibrary> lib = type_library(load("stdole2_typelib.bin"));
    CB_CHECK(lib.has_value());
    if (!lib) return;
    CB_CHECK_EQ(lib->format, gold["format"].get<std::string>());
    CB_CHECK_EQ(lib->format, "MSFT");
    CB_CHECK_EQ(lib->name, "stdole");
    CB_CHECK_EQ(lib->guid, "00020430-0000-0000-C000-000000000046");
    const J types = gold["types"];
    CB_CHECK_EQ(lib->types.size(), types.size());
    for (std::size_t i = 0; i < lib->types.size() && i < types.size(); ++i) {
        CB_CHECK_EQ(lib->types[i].kind, types[i][0].get<std::string>());
        CB_CHECK_EQ(lib->types[i].name, types[i][1].get<std::string>());
        CB_CHECK_EQ(lib->types[i].guid, types[i][2].get<std::string>());
    }
    CB_CHECK_EQ(lib->types[4].name, "IDispatch");
    CB_CHECK_EQ(lib->types[4].guid, "00020400-0000-0000-C000-000000000046");
    const std::string outline = type_library_outline(*lib);
    CB_CHECK_EQ(outline, both[1].get<std::string>());   // text for text, as the old reader rendered it
    CB_CHECK(outline.starts_with("// Outline of a type library (MSFT layout)."));
    CB_CHECK(outline.find("[ uuid(00020400-0000-0000-C000-000000000046) ] interface IDispatch;") != std::string::npos);
    CB_CHECK(outline.find("struct GUID;") != std::string::npos);
    CB_CHECK(outline.ends_with("}\n"));
});

CB_TEST("formats.resources.the_older_type_library_layout_is_only_named", [] {
    const std::optional<TypeLibrary> lib = type_library(load("stdole32_typelib.bin"));
    CB_CHECK(lib.has_value());
    CB_CHECK_EQ(lib->format, "SLTG");
    CB_CHECK(lib->name.empty() && lib->guid.empty() && lib->types.empty());
    // as the old reader rendered it
    CB_CHECK_EQ(type_library_outline(*lib),
                "// Outline of a type library (SLTG layout). Not compilable: only what each type is called.\nlibrary unnamed\n{\n}\n");
    CB_CHECK(!type_library(Bytes{}).has_value());
    CB_CHECK(!type_library(Bytes{'M', 'S', 'F'}).has_value());
    CB_CHECK(!type_library(Bytes(100, 7)).has_value());
});

CB_TEST("formats.resources.a_cut_type_library_gives_what_was_read", [] {
    const Bytes data = load("stdole2_typelib.bin");
    const std::size_t full = type_library(data)->types.size();
    // every prefix is either not a library, a bare format, or a shorter list: never an error
    std::size_t last = 0;
    for (std::size_t n : {std::size_t{0}, std::size_t{3}, std::size_t{4}, std::size_t{0x53}, std::size_t{0x54}, std::size_t{0x100}, std::size_t{0x400}, std::size_t{0x1000}, std::size_t{0x2000}, data.size() - 1, data.size()}) {
        const auto lib = type_library(ByteView(data).first(n));
        if (!lib) continue;
        CB_CHECK(lib->types.size() <= full);
        CB_CHECK(lib->types.size() >= last || n < 0x1000);   // (a longer prefix never reads fewer, once the tables are in)
        last = lib->types.size();
    }
    CB_CHECK_EQ(type_library(data)->types.size(), full);
});

// ---- built templates, the layouts no real module in the fixtures has ---------------------------------------

void wide_text(std::string_view s, io::ByteWriter& w) {
    w.bytes(text::to_utf16le(s));
    w.u16(0);
}

CB_TEST("formats.resources.strings", [] {
    auto counted = [](io::ByteWriter& w, std::string_view s) {
        const Bytes u = text::to_utf16le(s);
        w.u16(static_cast<std::uint16_t>(u.size() / 2));
        w.bytes(u);
    };
    // a 32-bit block: sixteen counted UTF-16 strings
    io::ByteWriter w;
    for (int i = 0; i < 16; ++i) counted(w, i == 3 ? "h\xC3\xA9llo" : i == 15 ? "" : std::string(static_cast<std::size_t>(i), 'a'));
    const std::vector<std::string> got = strings(w.data(), true);
    CB_CHECK_EQ(got.size(), 16u);
    CB_CHECK_EQ(got[3], "h\xC3\xA9llo");
    CB_CHECK_EQ(got[5], "aaaaa");
    CB_CHECK_EQ(got[15], "");
    // padding after the sixteenth string is zeros: fine; anything else is not
    Bytes padded = w.data();
    padded.insert(padded.end(), 6, 0);
    CB_CHECK_EQ(strings(padded, true).size(), 16u);
    padded.back() = 1;
    CB_CHECK_THROWS_AS(FormatError, (void)strings(padded, true));
    // a block that ends after three strings: the rest are empty
    io::ByteWriter few;
    counted(few, "one");
    counted(few, "");
    counted(few, "three");
    const std::vector<std::string> shorter = strings(few.data(), true);
    CB_CHECK_EQ(shorter.size(), 16u);
    CB_CHECK_EQ(shorter[0], "one");
    CB_CHECK_EQ(shorter[2], "three");
    CB_CHECK_EQ(shorter[3], "");
    // a 16-bit block: counted single-byte strings
    Bytes narrow = {3, 'a', 'b', 'c', 0};
    narrow.insert(narrow.end(), 14, 0);
    const std::vector<std::string> n = strings(narrow, false);
    CB_CHECK_EQ(n[0], "abc");
    CB_CHECK_EQ(n[1], "");
    CB_CHECK_THROWS_AS(FormatError, (void)strings(Bytes{200, 'a'}, false));   // a string longer than the data
    CB_CHECK_THROWS_AS(FormatError, (void)strings(Bytes{1}, true));            // half a count
});

CB_TEST("formats.resources.an_extended_dialog", [] {
    io::ByteWriter w;
    w.u16(1);
    w.u16(0xFFFF);
    w.u32(0x77);              // help id
    w.u32(0x10);              // exstyle
    w.u32(0x80C80040);        // style, with DS_SETFONT
    w.u16(2);                 // two controls
    w.i16(10); w.i16(-20); w.i16(200); w.i16(100);
    w.u16(0);                 // menu: none
    w.u16(0);                 // class: the default
    wide_text("Title", w);
    w.u16(9);                 // points
    w.u16(700);               // weight
    w.u8(1);                  // italic
    w.u8(2);                  // charset
    wide_text("Tahoma", w);
    w.align(4);               // control 1: a button (predefined class atom), text a string
    w.u32(5); w.u32(6); w.u32(0x50000001);
    w.i16(1); w.i16(2); w.i16(3); w.i16(4);
    w.u32(1);
    w.u16(0xFFFF); w.u16(0x0080);
    wide_text("OK", w);
    w.u16(0);
    w.align(4);               // control 2: a named class, text an ordinal, three bytes of creation data
    w.u32(0); w.u32(0); w.u32(0x50800000);
    w.i16(-1); w.i16(-2); w.i16(30); w.i16(40);
    w.u32(0x12345);
    wide_text("Edit", w);
    w.u16(0xFFFF); w.u16(0x0101);
    w.u16(3);
    w.bytes(Bytes{7, 8, 9});
    const Dialog d = dialog(w.data(), true);
    CB_CHECK(d.extended);
    CB_CHECK_EQ(d.help, 0x77u);
    CB_CHECK_EQ(d.exstyle, 0x10u);
    CB_CHECK_EQ(d.style, 0x80C80040u);
    CB_CHECK_EQ(d.x, 10);
    CB_CHECK_EQ(d.y, -20);
    CB_CHECK_EQ(d.cx, 200);
    CB_CHECK_EQ(d.cy, 100);
    CB_CHECK_EQ(std::get<std::string>(d.menu), "");
    CB_CHECK_EQ(std::get<std::string>(d.window_class), "");
    CB_CHECK_EQ(d.title, "Title");
    CB_CHECK(d.font.has_value());
    CB_CHECK_EQ(d.font->points, 9);
    CB_CHECK_EQ(d.font->weight, 700);
    CB_CHECK_EQ(d.font->italic, 1);
    CB_CHECK_EQ(d.font->charset, 2);
    CB_CHECK_EQ(d.font->face, "Tahoma");
    CB_CHECK_EQ(d.controls.size(), 2u);
    CB_CHECK_EQ(d.controls[0].help, 5u);
    CB_CHECK_EQ(d.controls[0].exstyle, 6u);
    CB_CHECK_EQ(d.controls[0].style, 0x50000001u);
    CB_CHECK_EQ(d.controls[0].id, 1u);
    CB_CHECK_EQ(std::get<std::uint32_t>(d.controls[0].window_class), 0x80u);
    CB_CHECK_EQ(*control_class_name(0x80), "BUTTON");
    CB_CHECK_EQ(std::get<std::string>(d.controls[0].text), "OK");
    CB_CHECK(d.controls[0].extra.empty());
    CB_CHECK_EQ(d.controls[1].x, -1);
    CB_CHECK_EQ(d.controls[1].id, 0x12345u);
    CB_CHECK_EQ(std::get<std::string>(d.controls[1].window_class), "Edit");
    CB_CHECK_EQ(std::get<std::uint32_t>(d.controls[1].text), 0x101u);
    CB_CHECK((d.controls[1].extra == Bytes{7, 8, 9}));
    // every prefix is an error or a dialog, never a crash
    for (std::size_t n = 0; n < w.size(); ++n) {
        try {
            (void)dialog(ByteView(w.data()).first(n), true);
        } catch (const FormatError&) {
        }
    }
    CB_CHECK_THROWS_AS(FormatError, (void)dialog(ByteView(w.data()).first(w.size() - 1), true));   // the extra data is short
});

CB_TEST("formats.resources.a_plain_32bit_dialog", [] {
    io::ByteWriter w;
    w.u32(0x90C80040);   // style (DS_SETFONT)
    w.u32(0x08);         // exstyle
    w.u16(1);
    w.i16(1); w.i16(2); w.i16(3); w.i16(4);
    w.u16(0xFFFF); w.u16(77);   // menu: ordinal 77
    wide_text("MyClass", w);
    wide_text("Caption", w);
    w.u16(8);
    wide_text("MS Sans Serif", w);
    w.align(4);
    w.u32(0x50010000); w.u32(0x20);
    w.i16(5); w.i16(6); w.i16(7); w.i16(8);
    w.u16(99);
    wide_text("Static", w);
    wide_text("hello", w);
    w.u16(0);
    const Dialog d = dialog(w.data(), true);
    CB_CHECK(!d.extended);
    CB_CHECK_EQ(d.exstyle, 8u);
    CB_CHECK_EQ(std::get<std::uint32_t>(d.menu), 77u);
    CB_CHECK_EQ(std::get<std::string>(d.window_class), "MyClass");
    CB_CHECK_EQ(d.title, "Caption");
    CB_CHECK_EQ(d.font->points, 8);
    CB_CHECK_EQ(d.font->weight, 0);   // plain templates have no weight, italic, charset
    CB_CHECK_EQ(d.font->face, "MS Sans Serif");
    CB_CHECK_EQ(d.controls.size(), 1u);
    CB_CHECK_EQ(d.controls[0].style, 0x50010000u);
    CB_CHECK_EQ(d.controls[0].exstyle, 0x20u);
    CB_CHECK_EQ(d.controls[0].id, 99u);
    CB_CHECK_EQ(std::get<std::string>(d.controls[0].text), "hello");
});

CB_TEST("formats.resources.a_16bit_dialog", [] {
    io::ByteWriter w;
    w.u32(0x90C80040);
    w.u8(2);   // two controls
    w.i16(4); w.i16(5); w.i16(60); w.i16(70);
    w.u8(0);                      // menu: empty text
    w.u8(0);                      // class: empty text
    w.cstr("Dialog");
    w.u16(10);                    // font: points, face
    w.cstr("Helv");
    w.i16(1); w.i16(2); w.i16(3); w.i16(4);    // control 1: rect, id, style, class atom, text, extra count
    w.u16(1);
    w.u32(0x50010001);
    w.u8(0x80);
    w.cstr("OK");
    w.u8(0);
    w.i16(5); w.i16(6); w.i16(7); w.i16(8);    // control 2: a named class and a text that is an ordinal
    w.u16(2);
    w.u32(0x50000000);
    w.cstr("MyCtl");
    w.u8(0xFF); w.u16(0x1234);
    w.u8(2);
    w.bytes(Bytes{9, 9});
    const Dialog d = dialog(w.data(), false);
    CB_CHECK(!d.extended);
    CB_CHECK_EQ(d.title, "Dialog");
    CB_CHECK_EQ(d.font->points, 10);
    CB_CHECK_EQ(d.font->face, "Helv");
    CB_CHECK_EQ(d.controls.size(), 2u);
    CB_CHECK_EQ(d.controls[0].id, 1u);
    CB_CHECK_EQ(d.controls[0].style, 0x50010001u);
    CB_CHECK_EQ(std::get<std::uint32_t>(d.controls[0].window_class), 0x80u);
    CB_CHECK_EQ(std::get<std::string>(d.controls[0].text), "OK");
    CB_CHECK_EQ(std::get<std::string>(d.controls[1].window_class), "MyCtl");
    CB_CHECK_EQ(std::get<std::uint32_t>(d.controls[1].text), 0x1234u);
    CB_CHECK((d.controls[1].extra == Bytes{9, 9}));
});

CB_TEST("formats.resources.menus", [] {
    // 16-bit: a popup "File" (flag 0x10, the only top-level item so also the last, 0x80) holding Open and Exit (last)
    io::ByteWriter n;
    n.u16(0); n.u16(0);                       // version 0, offset 0
    n.u16(0x10 | 0x80); n.cstr("File");
    n.u16(0);    n.u16(100); n.cstr("Open");
    n.u16(0x80); n.u16(101); n.cstr("Exit");
    const Menu m = menu(n.data(), false);
    CB_CHECK(!m.extended);
    const auto& top = std::get<std::vector<MenuItem>>(m.items);
    CB_CHECK_EQ(top.size(), 1u);
    CB_CHECK_EQ(top[0].text, "File");
    CB_CHECK_EQ(top[0].flags, 0x10);            // without the "last" bit
    CB_CHECK(top[0].items.has_value());
    CB_CHECK_EQ(top[0].items->size(), 2u);
    CB_CHECK_EQ((*top[0].items)[0].id, 100);
    CB_CHECK_EQ((*top[0].items)[1].text, "Exit");
    CB_CHECK_EQ((*top[0].items)[1].flags, 0);

    // 32-bit extended: a popup holding one item
    io::ByteWriter e;
    e.u16(1); e.u16(4); e.u32(0x55);            // version 1, offset to the items, help id
    e.align(4);
    e.u32(0); e.u32(0); e.u32(0); e.u16(0x01 | 0x80); wide_text("&View", e);   // type, state, id, popup+last
    e.align(4);
    e.u32(0x55);                                // the popup's help id
    e.align(4);
    e.u32(2); e.u32(3); e.u32(1500); e.u16(0x80); wide_text("&Zoom", e);       // the only item
    const Menu x = menu(e.data(), true);
    CB_CHECK(x.extended);
    CB_CHECK_EQ(x.help, 0x55u);
    const auto& ext = std::get<std::vector<ExtMenuItem>>(x.items);
    CB_CHECK_EQ(ext.size(), 1u);
    CB_CHECK_EQ(ext[0].text, "&View");
    CB_CHECK_EQ(ext[0].help, 0x55u);
    CB_CHECK(ext[0].items.has_value());
    CB_CHECK_EQ((*ext[0].items)[0].id, 1500u);
    CB_CHECK_EQ((*ext[0].items)[0].type, 2u);
    CB_CHECK_EQ((*ext[0].items)[0].state, 3u);

    // a version this reader does not know; version 1 is a 32-bit form only
    CB_CHECK_THROWS_AS(FormatError, (void)menu(Bytes{2, 0, 0, 0}, true));
    CB_CHECK_THROWS_AS(FormatError, (void)menu(Bytes{1, 0, 0, 0}, false));
    // a menu that never ends its popups: nesting is bounded, not a stack overflow
    io::ByteWriter deep;
    deep.u16(0); deep.u16(0);
    for (int i = 0; i < 5000; ++i) { deep.u16(0x10); deep.cstr(""); }
    CB_CHECK_THROWS_AS(FormatError, (void)menu(deep.data(), false));
});

CB_TEST("formats.resources.accelerators", [] {
    // 16-bit: five bytes each (flags, key, id), the last marked 0x80
    const std::vector<Accelerator> n = accelerators(Bytes{0x01, 0x70, 0, 0x64, 0, 0x83, 0x71, 0, 0x65, 0}, false);
    CB_CHECK_EQ(n.size(), 2u);
    CB_CHECK_EQ(n[0].flags, 1);
    CB_CHECK_EQ(n[0].key, 0x70);
    CB_CHECK_EQ(n[0].id, 100);
    CB_CHECK_EQ(n[1].flags, 3);       // 0x83 without the mark
    CB_CHECK_EQ(n[1].id, 101);
    // 32-bit: eight bytes each; reading stops at the mark and ignores what follows
    const std::vector<Accelerator> w = accelerators(Bytes{0x81, 0, 0x41, 0, 0x0A, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8}, true);
    CB_CHECK_EQ(w.size(), 1u);
    CB_CHECK_EQ(w[0].key, 0x41);
    CB_CHECK(accelerators(Bytes{}, true).empty());
    CB_CHECK_EQ(accelerators(Bytes{1, 2, 3}, true).size(), 0u);   // a partial entry is not an entry
});

CB_TEST("formats.resources.version_block_errors", [] {
    Bytes blob = load("shell32_version_1.bin");
    blob[6] = 'X';   // the key is "VS_VERSION_INFO" in UTF-16 at offset 6
    CB_CHECK_THROWS_AS(FormatError, (void)version(blob, true));
    CB_CHECK_THROWS_AS(FormatError, (void)version(Bytes{}, true));
    CB_CHECK_THROWS_AS(FormatError, (void)version(Bytes{2, 0, 0, 0, 0, 0}, true));   // a block shorter than its own header
    // a version block whose children nest without end
    io::ByteWriter w;
    for (int i = 0; i < 100; ++i) { w.u16(0x7FFF); w.u16(0); w.u16(0); wide_text("a", w); w.align(4); }
    CB_CHECK_THROWS_AS(FormatError, (void)version(w.data(), true));
});

CB_TEST("formats.resources.damage_never_crashes", [] {
    // every prefix of every real resource decodes, is opaque, or is a FormatError
    const J golden = load_json("golden.json");
    for (const auto& [file, entry] : golden.items()) {
        const Bytes data = load(file);
        const std::string type = entry["type"];
        const std::uint32_t id = type == "DIALOG" ? kDialog : type == "MENU" ? kMenu : type == "STRING"
                                 ? kString : type == "ACCELERATOR" ? kAccelerator : kVersion;
        for (std::size_t n = 0; n < data.size(); ++n) {
            const Resource r = resource(id, Bytes(data.begin(), data.begin() + static_cast<std::ptrdiff_t>(n)));
            (void)decode(r, true);   // never throws: a damaged body is opaque
        }
        // a flipped byte in the middle
        Bytes flipped = data;
        flipped[flipped.size() / 2] ^= 0xFF;
        (void)decode(resource(id, flipped), true);
    }
});

CB_TEST("formats.resources.decode_by_kind", [] {
    CB_CHECK(!decode(resource(kBitmap, Bytes(100, 1)), true).has_value());      // a kind decode() leaves to the caller
    CB_CHECK(!decode(resource(kRcData, Bytes{1, 2, 3}), true).has_value());
    Resource named;
    named.type = std::string("TYPELIB");
    named.name = std::uint32_t{1};
    named.data = Bytes{1, 2, 3};
    CB_CHECK(!decode(named, true).has_value());                                  // a named type is opaque
    // a dialog that does not parse is opaque
    CB_CHECK(!decode(resource(kDialog, Bytes{1, 2, 3}), true).has_value());
    // the version's date is dropped
    const auto v = decode(resource(kVersion, load("shell32_version_1.bin")), true);
    CB_CHECK(v.has_value());
    CB_CHECK_EQ(std::get<Version>(*v).fixed->date_ms, 0u);
    CB_CHECK_EQ(std::get<Version>(*v).fixed->date_ls, 0u);
});

CB_TEST("formats.resources.names_keys_and_sizes", [] {
    CB_CHECK_EQ(type_name(Ident(std::uint32_t{5})), "DIALOG");
    CB_CHECK_EQ(type_name(Ident(std::uint32_t{14})), "GROUP_ICON");
    CB_CHECK_EQ(type_name(Ident(std::uint32_t{99})), "#99");
    CB_CHECK_EQ(type_name(Ident(std::string("TYPELIB"))), "TYPELIB");
    CB_CHECK_EQ(*member_type_of(kGroupIcon), kIcon);
    CB_CHECK_EQ(*member_type_of(kGroupCursor), kCursor);
    CB_CHECK(!member_type_of(kDialog).has_value());
    CB_CHECK(!control_class_name(0x86).has_value());
    Resource r;
    r.type = std::uint32_t{5};
    r.name = std::string("ABOUT");
    r.language = 1033;
    r.data = Bytes(120, 0);
    CB_CHECK_EQ(r.describe(), "<DIALOG 'ABOUT' lang=1033 120 bytes>");
    r.language.reset();
    r.name = std::uint32_t{7};
    CB_CHECK_EQ(r.describe(), "<DIALOG 7 lang=None 120 bytes>");
    // keys order by type, name, language
    const ResourceKey a{std::uint32_t{5}, std::uint32_t{1}, std::nullopt};
    const ResourceKey b{std::uint32_t{5}, std::uint32_t{1}, 1033u};
    const ResourceKey c{std::uint32_t{6}, std::uint32_t{0}, std::nullopt};
    CB_CHECK(a < b);
    CB_CHECK(b < c);
    CB_CHECK(a == ResourceKey(a));
    // a 16-bit module pads a resource with up to a unit of zeros
    CB_CHECK(same_data(Bytes{1, 2, 3}, Bytes{1, 2, 3, 0, 0}));
    CB_CHECK(same_data(Bytes{1, 2, 3, 0}, Bytes{1, 2, 3}));
    CB_CHECK(!same_data(Bytes{1, 2, 3}, Bytes{1, 2, 3, 0, 1}));
    CB_CHECK(!same_data(Bytes{1, 2, 3}, Bytes{1, 2, 4}));
    CB_CHECK(same_data(Bytes{}, Bytes{}));
});

CB_TEST("formats.resources.entries_of_a_32bit_module", [] {
    const Bytes image(1000, 0x5A);
    std::vector<PeResourceEntry> entries;
    entries.push_back({std::uint32_t{3}, std::uint32_t{1}, 1033u, 100, 50});
    entries.push_back({std::string("TYPELIB"), std::uint32_t{1}, 0u, std::nullopt, 10});    // no data in the file: left out
    entries.push_back({std::uint32_t{4}, std::string("MAIN"), std::nullopt, 990, 50});      // runs past the end: left out
    entries.push_back({std::uint32_t{4}, std::uint32_t{2}, 1033u, 950, 50});                // ends exactly at the end
    const std::vector<Resource> got = from_pe_entries(image, entries);
    CB_CHECK_EQ(got.size(), 2u);
    CB_CHECK_EQ(got[0].data.size(), 50u);
    CB_CHECK_EQ(*got[0].language, 1033u);
    CB_CHECK_EQ(got[0].flags, 0);
    CB_CHECK_EQ(std::get<std::uint32_t>(got[1].type), 4u);
    CB_CHECK(kind_of(Bytes{'M', 'Z'}) == ExecutableKind::Other);
});

}  // namespace
}  // namespace cb::formats::resources
