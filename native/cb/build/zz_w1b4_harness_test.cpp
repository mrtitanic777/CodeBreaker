// LOCAL harness for the W1-B4 refactor (never committed): runs the new asm16 / msc6 over the inputs of the old harnesses and
// writes what they print, for comparison with the Python's expected.txt. Does nothing unless CB_W1B4 is set.
#include <nlohmann/json.hpp>

#include <fstream>
#include <sstream>

#include "cb/build/asm16.hpp"
#include "cb/build/msc6.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
using nlohmann::ordered_json;
namespace asm16 = cb::build::asm16;
namespace ne = cb::formats::ne;
namespace msc6 = cb::build::msc6;

std::optional<std::string> env(const char* n) { return fs::get_env(n); }

std::string sha1hex(std::string_view s) { return crypto::to_hex(crypto::sha1(as_bytes(s))); }

ordered_json dig(const std::filesystem::path& out, const std::string& label, const std::string& s) {
    std::string name = label;
    for (char& c : name) {
        if (c == ':') c = '_';
    }
    static const bool write_files = !env("CB_W1B4_NOWRITE");
    if (write_files) fs::write_file(out / (name + ".txt"), to_bytes(s));
    return ordered_json::array({s.size() /* characters: ASCII */, sha1hex(s)});
}

ordered_json dig_chars(const std::filesystem::path& out, const std::string& label, const std::string& s) {
    std::size_t chars = 0;
    for (const unsigned char c : s) chars += (c & 0xC0) != 0x80;
    ordered_json j = dig(out, label, s);
    j[0] = chars;
    return j;
}

int csum(std::string_view s) {
    int t = 0;
    for (const unsigned char c : s) t += c;
    return t;
}

std::optional<std::string> harness_symbols(const asm16::Reference& ref) {
    if (const auto* f = std::get_if<asm16::FunctionRef>(&ref)) {
        if ((f->seg * 31 + static_cast<int>(f->off)) % 13 == 0) return std::nullopt;
        return std::format("_f{}_{:x}", f->seg, f->off);
    }
    if (const auto* s = std::get_if<asm16::SegmentRef>(&ref)) return std::format("_seg{}", s->seg);
    if (const auto* d = std::get_if<asm16::DataRef>(&ref)) return std::format("_d{}", d->seg);
    const auto& i = std::get<asm16::ImportRef>(ref);
    const std::string mod = i.module.value_or("None");
    const int w = std::holds_alternative<std::uint16_t>(i.which) ? std::get<std::uint16_t>(i.which) : csum(std::get<std::string>(i.which));
    if ((csum(mod) + w) % 17 == 0) return std::nullopt;
    return "_IMP_" + mod + "_" + (std::holds_alternative<std::uint16_t>(i.which) ? std::to_string(std::get<std::uint16_t>(i.which)) : std::get<std::string>(i.which));
}

Bytes object_of(ByteView code) {
    const auto record = [](std::uint8_t kind, ByteView body) {
        Bytes r{kind, static_cast<std::uint8_t>((body.size() + 1) & 0xFF), static_cast<std::uint8_t>((body.size() + 1) >> 8)};
        append(r, body);
        r.push_back(0);
        return r;
    };
    Bytes out = record(0x80, to_bytes(std::string("\x03" "f.c", 4)));
    Bytes data{1, 0, 0};
    append(data, code);
    append(out, record(0xA0, data));
    append(out, record(0x8A, Bytes{0}));
    return out;
}

std::string hexs(ByteView b) { return crypto::to_hex(b); }

ordered_json pairs(const std::set<std::pair<std::uint16_t, std::uint32_t>>& s) {
    ordered_json a = ordered_json::array();
    for (const auto& [x, y] : s) a.push_back({x, y});
    return a;
}

ordered_json externs_json(const asm16::Function& f) {
    ordered_json a = ordered_json::array();
    for (const auto& [k, v] : f.externs()) a.push_back({k, v == asm16::Linkage::Far ? "far" : v == asm16::Linkage::Near ? "near" : "byte"});
    return a;
}

ordered_json describe(const std::filesystem::path& out, const std::string& label, const asm16::Function& f) {
    ordered_json d = ordered_json::object();
    d["text"] = f.text() ? dig_chars(out, "text_" + label, *f.text()) : ordered_json(nullptr);
    d["why"] = f.why();
    ordered_json imports = ordered_json::array();
    for (const auto& [k, v] : f.imports()) {
        imports.push_back({k, v.module, std::holds_alternative<std::uint16_t>(v.which) ? ordered_json(std::get<std::uint16_t>(v.which)) : ordered_json(std::get<std::string>(v.which))});
    }
    d["imports"] = imports;
    d["externs"] = externs_json(f);
    d["missing"] = f.missing();
    d["tables"] = f.tables();
    d["far_missing"] = pairs(f.far_missing());
    d["reaches"] = f.reaches() ? ordered_json(*f.reaches()) : ordered_json(nullptr);
    ordered_json holes = ordered_json::array();
    for (const auto& h : f.holes()) holes.push_back({h.start, h.end, h.symbol});
    d["holes"] = holes;
    ordered_json publics = ordered_json::array();
    for (const auto& p : f.publics()) publics.push_back({p.offset, p.symbol});
    d["publics"] = publics;
    ordered_json cs = ordered_json::array();
    for (const auto& [a, b] : f.cs_labels()) cs.push_back({a, b});
    d["cs_labels"] = cs;
    d["raw_far_entries"] = pairs(f.raw_far_entries());
    ordered_json rhc = ordered_json::array();
    for (const auto& [a, c] : f.raw_hole_calls()) rhc.push_back({a, c.symbol, c.opcode});
    d["raw_hole_calls"] = rhc;
    if (!f.text()) return d;
    d["bits"] = f.bits();
    d["start"] = f.start();
    d["end"] = f.end();
    d["seg"] = f.seg();
    d["module"] = f.module_name();
    d["public"] = f.first_public();
    const auto directives = [](const std::map<std::uint32_t, asm16::Directive>& m) {
        ordered_json a = ordered_json::array();
        for (const auto& [at, x] : m) a.push_back({at, x.size, x.text});
        return a;
    };
    d["sites"] = directives(f.sites());
    d["symbolic"] = directives(f.symbolic());
    d["calls"] = directives(f.calls());
    d["labels"] = f.labels();
    ordered_json ins = ordered_json::array();
    for (const auto& [a, i] : f.insns()) ins.push_back({a, hexs(i.bytes), i.mnemonic, i.operands, i.target ? ordered_json(*i.target) : ordered_json(nullptr)});
    d["ins"] = ins;
    ordered_json segs = ordered_json::array();
    for (const auto& [s, n] : f.code_segments()) segs.push_back({s, n});
    d["code_segments"] = segs;
    return d;
}

ordered_json full(const std::filesystem::path& out, const std::string& label, asm16::Function& f) {
    ordered_json d = describe(out, label, f);
    if (!f.text()) return d;
    std::string mn;
    for (const auto& [a, i] : f.insns()) mn += (mn.empty() ? "" : "\n") + f.mnemonic(a);
    d["mn"] = dig_chars(out, "mn_" + label, mn);
    ordered_json cand = ordered_json::array();
    for (const auto& [a, c] : f.candidates()) cand.push_back({a, c.text, hexs(c.original), hexs(c.mask)});
    d["cand"] = cand;
    ordered_json raw = ordered_json::array();
    for (const auto& [name, at] : f.raw_calls_into_holes()) raw.push_back({name, at});
    d["raw_calls"] = raw;
    std::set<std::uint32_t> good, every;
    std::size_t i = 0;
    for (const auto& [a, ins] : f.insns()) {
        if (i++ % 3) good.insert(a);
        every.insert(a);
    }
    d["w_none"] = dig_chars(out, "wnone_" + label, f.write(std::nullopt));
    d["w_all"] = dig_chars(out, "wall_" + label, f.write(every));
    d["w_good"] = dig_chars(out, "wgood_" + label, f.write(good));
    const std::uint32_t lo = f.start() + (f.end() - f.start()) / 4;
    const std::uint32_t hi = f.end() - (f.end() - f.start()) / 4;
    const std::pair<std::uint32_t, std::uint32_t> piece{lo, hi};
    d["w_piece"] = dig_chars(out, "wpiece_" + label, f.write(good, piece));
    d["w_piece_none"] = dig_chars(out, "wpiecen_" + label, f.write(every, piece));
    d["w_piece_none2"] = dig_chars(out, "wpiecen2_" + label, f.write(std::nullopt, piece));
    d["w_none_is_all"] = ordered_json::array({f.write(std::nullopt) == f.write(every), f.write(std::nullopt, piece) == f.write(every, piece)});
    d["externs_after"] = externs_json(f);
    ordered_json rhc = ordered_json::array();
    for (const auto& [a, c] : f.raw_hole_calls()) rhc.push_back({a, c.symbol, c.opcode});
    d["raw_hole_calls_after"] = rhc;
    const ByteView want = f.blob().subspan(f.start(), f.end() - f.start());
    d["check"] = asm16::check(object_of(want), f).value_or("");
    Bytes flipped(want.begin(), want.end());
    if (!flipped.empty()) flipped[0] ^= 0xFF;
    d["check_flip"] = asm16::check(object_of(flipped), f).value_or("");
    d["check_len"] = asm16::check(object_of(want.subspan(0, want.size() ? want.size() - 1 : 0)), f).value_or("");
    d["diff"] = asm16::differences(want, Bytes(want.size(), 0), f);
    d["diff_flip"] = asm16::differences(flipped, Bytes(flipped.size(), 0), f);
    d["check_piece"] = asm16::check_piece(object_of(f.blob().subspan(lo, hi - lo)), f, lo, hi).value_or("");
    Bytes pf(f.blob().begin() + lo, f.blob().begin() + hi);
    pf[0] ^= 1;
    d["check_piece_flip"] = asm16::check_piece(object_of(pf), f, lo, hi).value_or("");
    d["start_end_after"] = ordered_json::array({f.start(), f.end()});
    return d;
}

std::vector<std::string> fields(const std::string& line) {
    std::vector<std::string> out;
    for (const std::string_view p : text::split(line, '\t')) out.emplace_back(p);
    return out;
}

}  // namespace

CB_TEST("w1b4_harness.asm16", [] {
    const auto from = env("CB_W1B4_ASM16");
    if (!from) return;
    const std::filesystem::path dir = fs::from_text(*from);
    const std::filesystem::path mini = fs::from_text(env("CB_W1B4_MINI").value());
    const std::filesystem::path out = fs::from_text(env("CB_W1B4_OUT").value()) / "asm16";
    fs::remove_tree(out);
    fs::make_dirs(out);
    std::map<std::string, ne::Module> modules;
    platforms::DecoderSet decoders;
    std::string actual;
    std::istringstream cases(fs::read_text(dir / "cases.txt"));
    for (std::string line; std::getline(cases, line);) {
        if (line.empty()) continue;
        const auto f = fields(line);
        const std::string& label = f[0];
        const std::string& mod = f[1];
        if (!modules.contains(mod)) {
            try {
                modules.emplace(mod, ne::Module::open_file(mini / mod));
            } catch (const Error&) {
                continue;
            }
        }
        asm16::Request r;
        r.seg = static_cast<std::uint16_t>(std::stoi(f[2]));
        r.start = static_cast<std::uint32_t>(std::stoi(f[3]));
        r.end = static_cast<std::uint32_t>(std::stoi(f[4]));
        for (const std::string_view p : text::split(std::string_view(f[5]).substr(1), ',')) {
            const std::size_t colon = p.find(':');
            r.publics.push_back({static_cast<std::uint32_t>(std::stoi(std::string(p.substr(0, colon)))), std::string(p.substr(colon + 1))});
        }
        r.cs_ok = static_cast<std::uint32_t>(std::stoi(f[6]));
        r.whole = f[7] == "1";
        if (f[8] != "-") {
            for (const std::string_view e : text::split(f[8], ',')) r.extra_entries.push_back(static_cast<std::uint32_t>(std::stoi(std::string(e))));
        }
        if (f[9] != "-") {
            for (const std::string_view h : text::split(f[9], ',')) {
                const auto p = text::split(h, ':');
                r.holes.push_back({static_cast<std::uint32_t>(std::stoi(std::string(p[0]))), static_cast<std::uint32_t>(std::stoi(std::string(p[1]))), std::string(p[2])});
            }
        }
        ordered_json value;
        try {
            asm16::Function fn(modules.at(mod), r, harness_symbols, decoders);
            value = full(out, label, fn);
        } catch (const Error& e) {
            value = ordered_json{{"raises", std::string(e.kind())}};
        }
        actual += "fn:" + label + "\t" + value.dump() + "\n";
    }
    fs::write_file(out / "actual.txt", to_bytes(actual));
}
);
