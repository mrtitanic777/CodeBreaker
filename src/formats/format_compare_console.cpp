// formats/format_compare_console.cpp -- format_compare for the console ROM headers (gb gba nds ngp vb pce atari7800 sms
// genesis n64 ...): the engine's tree of a file against the reader of the same name, field by field.
//
// A reader that first puts the file in its own order (a copier's header taken off, an .smd de-interleaved, a .v64 swapped)
// hands the engine that normalised image: for those the engine runs when the reader accepts the file, and what the
// engine reads is the reader's `data`. The others are compared on the file itself, both ways (what one refuses the other
// refuses).
#include <algorithm>
#include <format>
#include <functional>
#include <optional>

#include "core/error.hpp"
#include "core/text.hpp"
#include "formats/atari7800.hpp"
#include "formats/coleco.hpp"
#include "formats/msx.hpp"
#include "formats/format_compare_kit.hpp"
#include "formats/gb.hpp"
#include "formats/gba.hpp"
#include "formats/genesis.hpp"
#include "formats/intv.hpp"
#include "formats/lynx.hpp"
#include "formats/n64.hpp"
#include "formats/snes.hpp"
#include "formats/nds.hpp"
#include "formats/ngp.hpp"
#include "formats/pce.hpp"
#include "formats/sms.hpp"
#include "formats/vb.hpp"

namespace cb::formats::engine {
namespace {

std::uint64_t u(const Node& n, std::string_view path) { return n.get(path).value; }

std::string trim_nul(std::string s) {
    while (!s.empty() && s.back() == '\0') s.pop_back();
    return s;
}
std::string trim_pad(std::string s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\0')) s.pop_back();
    return s;
}
std::string latin1_trim(const std::string& s) { return std::string(text::trim(text::from_latin1(as_bytes(s)))); }

// What a run needs of one format.
template <class Rom>
struct Run {
    bool normalized = false;                                            // the engine reads the reader's data, and only when the reader accepts
    std::function<Rom(ByteView)> parse;                                 // the reader
    std::function<ByteView(const Rom&, ByteView)> input;                // the bytes the engine reads (normalised formats)
    std::function<void(Checker&, const Tree&, const Rom&)> check;       // the values
    std::size_t max_size = static_cast<std::size_t>(-1);                // a bigger file is refused before the description is asked (a mapper): not compared
};

template <class Rom>
void run(Comparison& c, const Registry& reg, std::string_view format, ByteView data, const Run<Rom>& r) {
    if (data.size() > r.max_size) return;
    std::optional<Rom> rom;
    try {
        rom = r.parse(data);
        c.reader_accepts = true;
    } catch (const Error&) {
    }
    ByteView in = data;
    if (r.normalized) {
        if (!rom) return;   // nothing to compare: the engine reads what the reader made of the file
        in = r.input(*rom, data);
    }
    c.file_size = in.size();
    std::optional<Tree> tree;
    try {
        tree = reg.read(format, in);
        c.engine_accepts = true;
    } catch (const FormatError& e) {
        c.engine_error = e.what();
    }
    if (!tree) {
        if (c.reader_accepts) c.mismatch_count = 1;
        return;
    }
    if (!c.reader_accepts) {
        c.mismatch_count = 1;
        return;
    }
    Checker k(c);
    try {
        r.check(k, *tree, *rom);
    } catch (const Error& e) {   // a path the description should have given: a mismatch
        k.add("(reading the tree)", e.what(), "");
    }
    c.round_trip = equal_bytes(write(*tree), in);
    c.covered_round_trip = covered_equal(*tree);
    c.covered = covered_bytes(*tree);
}

// ---- the formats ----

void check_gb(Checker& k, const Tree& t, const gb::Rom& rom) {
    const Node& h = t.root.get("header");
    const std::uint64_t color = u(h, "color");
    k.eq("color", color, static_cast<std::uint64_t>(rom.color));
    const std::string title = h.get("title").text;
    const std::size_t nul = title.find('\0');
    k.eq_text("title", latin1_trim(nul == std::string::npos ? title : title.substr(0, nul)), rom.title);
    k.eq("sgb", u(h, "sgb"), rom.sgb);
    k.eq("cartridge_type", u(h, "cartridge_type"), rom.cartridge_type);
    k.eq("header_checksum", u(h, "header_checksum"), rom.header_checksum);
    k.eq("header_checksum_ok", u(h, "computed_checksum") == u(h, "header_checksum"), rom.header_checksum_ok);
    k.eq("logo_ok", equal_bytes(bytes_of(t, h.get("logo")), gb::kLogo), rom.logo_ok);
}

void check_gba(Checker& k, const Tree& t, const gba::Rom& rom) {
    const Node& h = t.root.get("header");
    const gba::Header& r = rom.header;
    k.eq("entry_word", u(h, "entry_word"), r.entry_word);
    k.eq("entry present", u(h, "is_branch"), r.entry.has_value());
    if (r.entry) k.eq("entry", u(h, "entry"), *r.entry);
    k.eq("logo_ok", equal_bytes(bytes_of(t, h.get("logo")), gba::kLogo), r.logo_ok);
    k.eq_text("title", trim_nul(h.get("title").text), r.title);
    k.eq_text("game_code", trim_nul(h.get("game_code").text), r.game_code);
    k.eq_text("maker_code", trim_nul(h.get("maker_code").text), r.maker_code);
    k.eq("unit_code", u(h, "unit_code"), r.unit_code);
    k.eq("device_type", u(h, "device_type"), r.device_type);
    k.eq("version", u(h, "version"), r.version);
    k.eq("complement", u(h, "complement"), r.complement);
    k.eq("computed_complement", u(h, "computed_complement"), r.computed_complement);
}

void check_ngp(Checker& k, const Tree& t, const ngp::Rom& rom) {
    const Node& h = t.root.get("header");
    const ngp::Header& r = rom.header;
    k.eq("licensed", u(h, "licensed"), r.licensed);
    k.eq("start", u(h, "start"), r.start);
    k.eq("catalog", u(h, "catalog"), r.catalog);
    k.eq("sub_catalog", u(h, "sub_catalog"), r.sub_catalog);
    k.eq("colour", u(h, "colour"), r.colour);
    k.eq_text("title", trim_pad(h.get("title").text), r.title);
}

void check_binary(Checker& k, const Node& h, const std::string& name, const nds::Binary& b) {
    k.eq(name + ".rom_offset", u(h, name + ".rom_offset"), b.rom_offset);
    k.eq(name + ".entry", u(h, name + ".entry"), b.entry);
    k.eq(name + ".ram_address", u(h, name + ".ram_address"), b.ram_address);
    k.eq(name + ".size", u(h, name + ".size_bytes"), b.size);
}

void check_nds(Checker& k, const Tree& t, const nds::Rom& rom) {
    const Node& h = t.root.get("header");
    const nds::Header& r = rom.header;
    k.eq_text("title", trim_nul(h.get("title").text), r.title);
    k.eq_text("game_code", trim_nul(h.get("game_code").text), r.game_code);
    k.eq_text("maker_code", trim_nul(h.get("maker_code").text), r.maker_code);
    k.eq("unit_code", u(h, "unit_code"), r.unit_code);
    k.eq("seed", u(h, "seed"), r.seed);
    k.eq("capacity", u(h, "capacity"), r.capacity);
    k.eq("region", u(h, "region"), r.region);
    k.eq("version", u(h, "version"), r.version);
    k.eq("autostart", u(h, "autostart"), r.autostart);
    check_binary(k, h, "arm9", r.arm9);
    check_binary(k, h, "arm7", r.arm7);
    k.eq("fnt_offset", u(h, "fnt_offset"), r.fnt_offset);
    k.eq("fnt_size", u(h, "fnt_size"), r.fnt_size);
    k.eq("fat_offset", u(h, "fat_offset"), r.fat_offset);
    k.eq("fat_size", u(h, "fat_size"), r.fat_size);
    k.eq("ovl9_offset", u(h, "ovl9_offset"), r.ovl9_offset);
    k.eq("ovl9_size", u(h, "ovl9_size"), r.ovl9_size);
    k.eq("ovl7_offset", u(h, "ovl7_offset"), r.ovl7_offset);
    k.eq("ovl7_size", u(h, "ovl7_size"), r.ovl7_size);
    k.eq("banner_offset", u(h, "banner_offset"), r.banner_offset);
    k.eq("used_size", u(h, "used_size"), r.used_size);
    k.eq("header_size", u(h, "header_size"), r.header_size);
    k.eq("logo_ok", equal_bytes(bytes_of(t, h.get("logo")), gba::kLogo), r.logo_ok);
    k.eq("logo_crc", u(h, "logo_crc"), r.logo_crc);
    k.eq("header_crc", u(h, "header_crc"), r.header_crc);
}

void check_vb(Checker& k, const Tree& t, const vb::Rom& rom) {
    const Node& h = t.root.get("header");
    k.eq_text("title", trim_pad(h.get("title").text), rom.header.title);
    k.eq_text("maker", trim_pad(h.get("maker").text), rom.header.maker);
    k.eq_text("game_code", trim_pad(h.get("game_code").text), rom.header.game_code);
    k.eq("version", u(h, "version"), rom.header.version);
    k.eq("header offset", h.offset, rom.header_offset());
}

void check_pce(Checker& k, const Tree& t, const pce::Rom& rom) {
    const Node& d = t.root;
    k.eq("copier size", d.get("copier").size, rom.header.size());
    k.eq("banks size", d.get("banks").size, rom.data.size());
    k.eq("irq2", u(d, "vectors.irq2"), rom.vectors.irq2);
    k.eq("irq1", u(d, "vectors.irq1"), rom.vectors.irq1);
    k.eq("timer", u(d, "vectors.timer"), rom.vectors.timer);
    k.eq("nmi", u(d, "vectors.nmi"), rom.vectors.nmi);
    k.eq("reset", u(d, "vectors.reset"), rom.vectors.reset);
}

void check_a78(Checker& k, const Tree& t, const atari7800::Rom& rom) {
    const Node& d = t.root;
    const Node& h = d.get("header");
    k.eq("version", u(h, "version"), rom.version);
    k.eq_text("title", trim_pad(h.get("title").text), rom.title);
    k.eq("declared_size", u(h, "declared_size"), rom.declared_size);
    k.eq("cart_type", u(h, "cart_type"), rom.cart_type);
    k.eq("controller1", u(h, "controller1"), rom.controller1);
    k.eq("controller2", u(h, "controller2"), rom.controller2);
    k.eq("tv", u(h, "tv"), rom.tv);
    k.eq("save", u(h, "save"), rom.save);
    k.eq("supergame", u(d, "supergame"), rom.supergame);
    k.eq("base", u(d, "base"), rom.base);
    k.eq("rom size", d.get("rom").size, rom.data.size());
    k.eq_bytes("header bytes", bytes_of(t, h), rom.header);
}

void check_sms(Checker& k, const Tree& t, const sms::Rom& rom) {
    const Node* h = t.root.find("header");
    k.eq("header present", h != nullptr, rom.header.has_value());
    if (!h || !rom.header) return;
    const sms::Header& r = *rom.header;
    k.eq("at", h->offset, r.at);
    k.eq("checksum", u(*h, "checksum"), r.checksum);
    k.eq("version", u(*h, "version"), r.version);
    const std::string code = std::format("{:X}{:02X}{:02X}", u(*h, "product2") >> 4, u(*h, "product1"), u(*h, "product0"));
    const std::size_t first = code.find_first_not_of('0');
    k.eq_text("product", first == std::string::npos ? "0" : code.substr(first), r.product);
    static const char* const regions[] = {nullptr, nullptr, nullptr, "Japan", "export", "Japan", "export", "international"};
    const std::uint64_t region = u(*h, "region");
    k.eq_text("region", region < 8 && regions[region] ? regions[region] : "?", r.region);
    static const std::uint32_t sizes[16] = {0x40000, 0x80000, 0x100000, 0, 0, 0, 0, 0, 0, 0, 0x2000, 0x4000, 0x8000, 0xC000, 0x10000, 0x20000};
    const std::uint64_t code_size = u(*h, "size_code");
    k.eq("size", sizes[code_size], r.size.value_or(0));
}

void check_genesis(Checker& k, const Tree& t, const genesis::Rom& rom) {
    const Node& d = t.root;
    const Node& h = d.get("header");
    const genesis::Header& r = rom.header;
    k.eq_text("system", latin1_trim(h.get("system").text), r.system);
    k.eq_text("copyright", latin1_trim(h.get("copyright").text), r.copyright);
    k.eq_text("domestic_name", latin1_trim(h.get("domestic_name").text), r.domestic_name);
    k.eq_text("overseas_name", latin1_trim(h.get("overseas_name").text), r.overseas_name);
    k.eq_text("serial", latin1_trim(h.get("serial").text), r.serial);
    k.eq_text("devices", latin1_trim(h.get("devices").text), r.devices);
    k.eq_text("region", latin1_trim(h.get("region").text), r.region);
    k.eq("checksum", u(h, "checksum"), r.checksum);
    k.eq("rom_start", u(h, "rom_start"), r.rom_start);
    k.eq("rom_end", u(h, "rom_end"), r.rom_end);
    k.eq("ram_start", u(h, "ram_start"), r.ram_start);
    k.eq("ram_end", u(h, "ram_end"), r.ram_end);
    const Node& v = d.get("vectors");
    k.eq("initial_ssp", v.children[0].value, rom.initial_ssp);
    k.eq("reset", v.children[1].value, rom.reset);
    std::vector<std::pair<std::string, std::uint32_t>> mine;
    for (unsigned n = 0; n < v.children.size(); ++n) {
        std::string name = genesis::vector_name(n);
        const std::uint32_t a = static_cast<std::uint32_t>(v.children[n].value);
        if (!name.empty() && a < rom.data.size() && a % 2 == 0 && a >= 0x200) mine.emplace_back(std::move(name), a);
    }
    k.eq("vector count", mine.size(), rom.vectors.size());
    for (std::size_t i = 0; i < std::min(mine.size(), rom.vectors.size()); ++i) {
        k.eq_text(std::format("vector[{}].name", i), mine[i].first, rom.vectors[i].name);
        k.eq(std::format("vector[{}].address", i), mine[i].second, rom.vectors[i].address);
    }
}

void check_n64(Checker& k, const Tree& t, const n64::Rom& rom) {
    const Node& h = t.root.get("header");
    const n64::Header& r = rom.header;
    k.eq("pi_config", u(h, "pi_config"), r.pi_config);
    k.eq("clock_rate", u(h, "clock_rate"), r.clock_rate);
    k.eq("entry", u(h, "entry"), r.entry);
    k.eq("release", u(h, "release"), r.release);
    k.eq("crc1", u(h, "crc1"), r.crc1);
    k.eq("crc2", u(h, "crc2"), r.crc2);
    k.eq_text("name", trim_pad(h.get("name").text), r.name);
    k.eq("category", u(h, "category"), static_cast<std::uint8_t>(r.category));
    k.eq_text("game_code", h.get("game_code").text, r.game_code);
    k.eq("country", u(h, "country"), static_cast<std::uint8_t>(r.country));
    k.eq("version", u(h, "version"), r.version);
}

void check_snes_vectors(Checker& k, const Node& h, const std::string& name, const snes::Vectors& v) {
    k.eq(name + ".cop", u(h, name + ".cop"), v.cop);
    k.eq(name + ".brk", u(h, name + ".brk"), v.brk);
    k.eq(name + ".abort", u(h, name + ".abort"), v.abort);
    k.eq(name + ".nmi", u(h, name + ".nmi"), v.nmi);
    k.eq(name + ".reset", u(h, name + ".reset"), v.reset);
    k.eq(name + ".irq", u(h, name + ".irq"), v.irq);
}

void check_snes(Checker& k, const Tree& t, const snes::Rom& rom) {
    const bool hirom = u(t.root, "is_hirom") != 0;
    const Node& h = t.root.get(hirom ? "hi" : "lo");
    k.eq("mapping", hirom, rom.mapping == snes::Mapping::HiRom);
    k.eq("header_offset", u(t.root, "header_offset"), rom.header_offset);
    k.eq_text("title", trim_pad(h.get("title").text), rom.title);
    k.eq("map_mode", u(h, "map_mode"), rom.map_mode);
    k.eq("chipset", u(h, "chipset"), rom.chipset);
    k.eq("rom_size_code", u(h, "rom_size_code"), rom.rom_size_code);
    k.eq("ram_size_code", u(h, "ram_size_code"), rom.ram_size_code);
    k.eq("country", u(h, "country"), rom.country);
    k.eq("developer", u(h, "developer"), rom.developer);
    k.eq("version", u(h, "version"), rom.version);
    k.eq("complement", u(h, "complement"), rom.complement);
    k.eq("checksum", u(h, "checksum"), rom.checksum);
    check_snes_vectors(k, h, "native", rom.native);
    check_snes_vectors(k, h, "emulation", rom.emulation);
}

void check_lynx(Checker& k, const Tree& t, const lynx::Program& p) {
    const Node& d = t.root;
    if (p.kind == lynx::Kind::Cartridge) {
        k.eq("kind", u(d, "is_cartridge"), 1);
        const Node& h = d.get("cart");
        auto upto_nul = [](const std::string& s) { return s.substr(0, s.find('\0')); };
        k.eq_text("name", upto_nul(h.get("name").text), p.name);
        k.eq_text("manufacturer", upto_nul(h.get("manufacturer").text), p.manufacturer);
        k.eq("bank0_block", u(h, "bank0_block"), p.bank0_block);
        k.eq("bank1_block", u(h, "bank1_block"), p.bank1_block);
        k.eq("version", u(h, "version"), p.version);
        k.eq("rotation", u(h, "rotation"), p.rotation);
        k.eq("audin", u(h, "audin"), p.audin);
        k.eq("eeprom", u(h, "eeprom"), p.eeprom);
        if (p.encrypted_blocks != 0) k.eq("encrypted_count", u(d, "encrypted_count"), p.encrypted_blocks);
    } else {
        k.eq("kind", u(d, "is_bs93"), 1);
        const Node& h = d.get("object");
        k.eq("load_address", u(h, "load_address"), p.load_address);
        k.eq("block_length", u(h, "block_length"), p.block_length);
    }
}

void check_intv(Checker& k, const Tree& t, const intv::Rom& rom) {
    const Node& segs = t.root.get("segments");
    k.eq("segment count", segs.children.size(), rom.segments.size());
    for (std::size_t i = 0; i < std::min(segs.children.size(), rom.segments.size()); ++i) {
        const Node& s = segs.children[i];
        const intv::Segment& r = rom.segments[i];
        const std::string p = std::format("segment[{}].", i);
        k.eq(p + "start", s.u("first_page") << 8, r.start);
        k.eq(p + "words", s.get("words").size / 2, r.words.size());
        k.eq(p + "crc", s.u("crc"), r.crc);
        const ByteView raw = bytes_of(t, s.get("words"));
        bool same = raw.size() == r.words.size() * 2;
        for (std::size_t w = 0; same && w < r.words.size(); ++w) same = static_cast<std::uint16_t>(raw[2 * w] << 8 | raw[2 * w + 1]) == r.words[w];
        k.eq(p + "word values", same, 1);
    }
    k.eq("tail size", t.root.get("tail").size, rom.tail.size());
}

void check_coleco(Checker& k, const Tree& t, const coleco::Program& p) {
    const Node& h = t.root.get("header");
    k.eq("title_screen", u(h, "title_screen"), p.header.title_screen);
    k.eq("sprite_names", u(h, "sprite_names"), p.header.sprite_names);
    k.eq("sprite_order", u(h, "sprite_order"), p.header.sprite_order);
    k.eq("work_buffer", u(h, "work_buffer"), p.header.work_buffer);
    k.eq("controllers", u(h, "controllers"), p.header.controllers);
    k.eq("start", u(h, "start"), p.header.start);
}

void check_msx(Checker& k, const Tree& t, const msx::Program& p) {
    const Node& h = t.root.get("header");
    k.eq("init", u(h, "init"), p.header.init);
    k.eq("statement", u(h, "statement"), p.header.statement);
    k.eq("device", u(h, "device"), p.header.device);
    k.eq("text", u(h, "text"), p.header.text);
    k.eq("header at", h.offset, t.root.u("at"));
}

}  // namespace

const std::vector<std::string>& compared_formats() {
    static const std::vector<std::string> formats = {"mz",  "pe",  "ne",  "le",     "ines", "gb",      "gba", "ngp",  "nds",    "vb",  "pce",
                                                     "atari7800", "sms", "genesis", "n64", "snes", "lynx", "intv", "coleco", "msx"};
    return formats;
}

bool compare_console(Comparison& c, const Registry& reg, std::string_view format, ByteView data) {
    if (format == "gb") {
        run<gb::Rom>(c, reg, format, data, {false, [](ByteView d) { return gb::parse(d); }, {}, check_gb});
    } else if (format == "gba") {
        run<gba::Rom>(c, reg, format, data, {false, [](ByteView d) { return gba::parse(d); }, {}, check_gba});
    } else if (format == "ngp") {
        run<ngp::Rom>(c, reg, format, data, {false, [](ByteView d) { return ngp::parse(d); }, {}, check_ngp});
    } else if (format == "nds") {
        run<nds::Rom>(c, reg, format, data, {false, [](ByteView d) { return nds::parse(d); }, {}, check_nds});
    } else if (format == "vb") {
        run<vb::Rom>(c, reg, format, data, {false, [](ByteView d) { return vb::parse(d); }, {}, check_vb});
    } else if (format == "pce") {
        run<pce::Rom>(c, reg, format, data, {false, [](ByteView d) { return pce::parse(d); }, {}, check_pce});
    } else if (format == "atari7800") {
        run<atari7800::Rom>(c, reg, format, data, {false, [](ByteView d) { return atari7800::parse(d); }, {}, check_a78});
    } else if (format == "sms") {
        run<sms::Rom>(c, reg, format, data,
                      {true, [](ByteView d) { return sms::parse(d); }, [](const sms::Rom& r, ByteView) { return ByteView(r.data); }, check_sms});
    } else if (format == "genesis") {
        run<genesis::Rom>(c, reg, format, data,
                          {true, [](ByteView d) { return genesis::parse(d); }, [](const genesis::Rom& r, ByteView) { return ByteView(r.data); }, check_genesis});
    } else if (format == "n64") {
        run<n64::Rom>(c, reg, format, data,
                      {true, [](ByteView d) { return n64::parse(d); }, [](const n64::Rom& r, ByteView) { return ByteView(r.data); }, check_n64});
    } else if (format == "snes") {
        run<snes::Rom>(c, reg, format, data,
                       {true, [](ByteView d) { return snes::parse(d); }, [](const snes::Rom& r, ByteView) { return ByteView(r.data); }, check_snes});
    } else if (format == "lynx") {
        run<lynx::Program>(c, reg, format, data, {false, [](ByteView d) { return lynx::parse(d); }, {}, check_lynx});
    } else if (format == "intv") {
        run<intv::Rom>(c, reg, format, data, {false, [](ByteView d) { return intv::parse(d, false); }, {}, check_intv});
    } else if (format == "coleco") {
        run<coleco::Program>(c, reg, format, data, {false, [](ByteView d) { return coleco::parse(d); }, {}, check_coleco, 0x8000});
    } else if (format == "msx") {
        run<msx::Program>(c, reg, format, data, {false, [](ByteView d) { return msx::parse(d); }, {}, check_msx, 0xC000});
    } else {
        return false;
    }
    return true;
}

}  // namespace cb::formats::engine
