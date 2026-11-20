// Replays the cases of a recorded corpus through cb::exact::draft::candidates and compares each result, and the tables
// the passes filled, with the recorded answers. The corpus is a folder of
//
//     texts.txt   draft bodies, one a line (\\ \t \n \r escaped)
//     cases.txt   op TAB text-index TAB context [TAB extra..]
//     expected.txt one JSON line a case: {"r": result} | {"same":true} | {"error":T}, then ,"t":{tables}
//                 (after the first 200 texts a result is only its length and crc32: {"crc":..,"len":..})
//     codes.txt   16-bit code, hex, a line each (for byte_fields)
//     kit/        the fake kit (ctype.h, stdio.h)
//
// made by a recording harness. The environment variable
// CB_CANDIDATES_REPLAY names the folder (the test is skipped without it); CB_CANDIDATES_REPLAY_FILTER keeps only the
// cases whose op it names. src/exact/testdata/draft_candidates.* is the part of that corpus checked in (see
// candidates_test.cpp).
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <map>
#include <set>
#include <sstream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "exact/draft/candidates.hpp"

using namespace cb;
using namespace cb::exact;
using namespace cb::exact::draft;
namespace cand = cb::exact::draft::candidates;
namespace dt = cb::exact::draft::text;
namespace ct = cb::text;

namespace {

using Strings = std::vector<std::string>;

std::string replay_unescape(std::string_view s) {
    std::string out;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            const char c = s[++i];
            out += c == 't' ? '\t' : c == 'n' ? '\n' : c == 'r' ? '\r' : c;
        } else {
            out += s[i];
        }
    }
    return out;
}

std::uint32_t replay_crc32(std::string_view s) {
    static const auto table = [] {
        std::array<std::uint32_t, 256> t{};
        for (std::uint32_t i = 0; i < 256; ++i) {
            std::uint32_t c = i;
            for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            t[i] = c;
        }
        return t;
    }();
    std::uint32_t c = 0xFFFFFFFFu;
    for (const unsigned char b : s) c = table[(c ^ b) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

// A text of bytes as JSON with non-ASCII escaped.
std::string replay_json(std::string_view s) {
    std::string out = "\"";
    for (const unsigned char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else if (c == '\b') out += "\\b";
        else if (c == '\f') out += "\\f";
        else if (c < 0x20 || c >= 0x7f) out += std::format("\\u{:04x}", c);
        else out += static_cast<char>(c);
    }
    return out + "\"";
}

std::string replay_json(const Strings& v) {
    std::string out = "[";
    for (std::size_t i = 0; i < v.size(); ++i) out += (i ? "," : "") + replay_json(v[i]);
    return out + "]";
}

std::string replay_opt(const std::optional<std::string>& v) { return v ? replay_json(*v) : "null"; }

// ---- the recorded projects ---------------------------------------------------------------------------------------

Bytes replay_image() {
    Bytes img(0x40, 0);
    const auto add = [&](std::string_view s) { img.insert(img.end(), s.begin(), s.end()); };
    add(std::string_view("\0Hello, world\0%s: error\0\x01" "ab\0path\\to\0\0", 40));
    for (int i = 1; i < 40; ++i) img.push_back(static_cast<std::uint8_t>(i));
    img.insert(img.end(), 64, 0);
    add(std::string_view("x\0no newline", 12));
    return img;
}

const std::map<std::string, std::uint32_t>& replay_libdata() {
    static const std::map<std::string, std::uint32_t> data = {{"__iob", 0x1000}, {"_ctype", 0x2000}, {"_ctype_", 0x2000}};
    return data;
}

struct ReplayProject {
    std::string dir;
    cand::Settings settings;
    Bytes image;
};

std::vector<std::unique_ptr<ReplayProject>> replay_projects(const std::filesystem::path& kit) {
    SymbolTable symbols;
    const auto sym = [&](const char* n, SymKind k, std::int64_t a) { symbols.insert_or_assign(n, SymPlace{k, a}); };
    sym("_buf", SymKind::Data, 0x2000);
    sym("_counter", SymKind::Data, 4);
    sym("g_0010", SymKind::Data, 0x10);
    sym("_tab", SymKind::Data, 0x300);
    sym("sym:odd", SymKind::Data, 0x400);
    sym("_f", SymKind::Code, 0x1234);
    sym("_arr", SymKind::Data, 0x100);
    sym("buf2", SymKind::Data, 0x2020);
    sym("_weird name", SymKind::Data, 0x500);
    sym("x", SymKind::Data, 0x600);
    std::vector<std::unique_ptr<ReplayProject>> out;
    for (int which = 0; which < 4; ++which) {
        auto p = std::make_unique<ReplayProject>();
        p->dir = std::format("PRJ{}", which + 1);
        p->image = replay_image();
        cand::Settings& s = p->settings;
        s.kit.project = p->dir;
        s.kit.toolchain = "msc300";
        s.kit.library_data = [] { return std::map<std::string, std::uint32_t>(); };
        s.image = p->image;
        s.data_segment = 0x40;
        s.data_names = cand::data_names_of(symbols);
        if (which == 0) {
            s.kit.kit = kit;
            s.frame_order = forms::FrameRule{16};
            s.table_macros = {{"ctype.h"}};
            forms::DataMacroSpec dm;
            dm.base = "__iob";
            dm.size = 8;
            dm.names = {"stdin", "stdout", "stderr"};
            dm.fields[7] = "fileno(%s)";
            dm.include = "stdio.h";
            s.data_macros = {dm};
            forms::LibraryMacroSpec a;
            a.call = "_filbuf";
            a.as = "getc(%s)";
            a.type = "FILE";
            a.include = "stdio.h";
            forms::LibraryMacroSpec b;
            b.call = "_flsbuf";
            b.as = "putc(%s, %s)";
            b.include = "stdio.h";
            b.byte_args = {0};
            s.macros = {a, b};
            s.fp_conversions = {"ftol", "__ftol"};
            s.long_helpers = {{"lmul", "*"}, {"ldiv", "/"}, {"lrem", "%"}, {"almul", "*="}, {"aldiv", "/="}, {"ulshr", ">>"}, {"aulshr", ">>="}, {"ulrem", "%"}, {"ulless", "<"}};
        } else if (which == 2) {
            s.kit.kit = kit;
            s.frame_order = forms::FrameRule{7};
            forms::LibraryMacroSpec a;
            a.call = "_filbuf";
            a.as = "getc(%s)";
            s.macros = {a};
            s.long_helpers = {{"lmul", "*"}};
        } else if (which == 3) {
            forms::LibraryMacroSpec a;
            a.call = "zz";
            a.as = "$%s";
            s.macros = {a};
        }
        out.push_back(std::move(p));
    }
    return out;
}

// ---- the recorded contexts (CTXS) ---------------------------------------------------------------------------------

struct ReplayContext {
    std::map<std::string, std::string> arrays;
    std::vector<std::pair<std::string, std::pair<std::int64_t, std::string>>> struct_arrays;
    std::vector<std::pair<std::string, std::optional<std::string>>> streams;   // "#include", "#include2" included
    std::set<std::string> word_pointers;
    std::map<std::string, std::string> struct_pointers;
    std::set<dt::Site> unsigned_sites, byte_sites;
    std::set<std::string> unsigned_globals;
    dt::LongInfo longs;
    std::map<std::string, std::string> fp_types;
    std::optional<std::string> fp_ret;
    std::set<std::pair<std::int64_t, std::string>> frame_slots;
    bool register_slots = false;
};

ReplayContext replay_context(int i) {
    ReplayContext c;
    if (i == 1) {
        c.longs.fields = {2, 6};
        c.longs.globals = {"g_0010", "g_0100"};
        c.longs.funcs = {"foo", "sub_1234"};
        c.longs.ret = true;
        c.longs.unsigned_names = {"v4", "a"};
        c.unsigned_sites = {{"p", 2}};
        c.unsigned_globals = {"g_0004"};
        c.byte_sites = {{"*", 4}, {"bx*", 2}, {"bxw", 0}, {"r_si", 4}};
        c.register_slots = true;
        c.fp_types = {{"*p", "long"}, {"v2", "double"}, {"g_0300", "float"}};
        c.fp_ret = "double";
        c.frame_slots = {{-2, "byte"}, {4, "byte"}, {6, "word"}, {-4, "zx"}, {-4, "byte"}, {8, "byte"}};
        c.arrays = {{"g_0100", "int"}, {"g_0200", "char"}, {"g_0102", "int"}};
        c.struct_arrays = {{"g_0100", {6, "int"}}, {"g_0300", {4, "char"}}};
        c.streams = {{"stdin", "FILE"}, {"#include", "stdio.h"}};
        c.word_pointers = {"p", "q"};
        c.struct_pointers = {{"g_0100", "s_p"}};
    } else if (i == 2) {
        c.longs.fields = {4};
        c.longs.ret = false;
        c.longs.funcs = {"bar"};
        c.unsigned_sites = {{"*", 0}};
        c.byte_sites = {{"bx*", 0}, {"siw", 0}, {"p", 0}, {"q", 2}, {"*", 2}, {"data", 4660}};
        c.fp_ret = "float";
        c.frame_slots = {{-6, "word"}, {4, "word"}, {-8, "byte"}};
        c.arrays = {{"g_0100", "int"}, {"g_0110", "int"}, {"g_0400", "int"}};
        c.struct_arrays = {{"g_0100", {4, "int"}}, {"g_0102", {4, "int"}}};
        c.word_pointers = {"p"};
    } else if (i == 3) {
        c.longs.ret = true;
        c.longs.fields = {0, 2, 4};
        c.longs.unsigned_names = {"v6"};
        c.unsigned_globals = {"g_0010"};
        c.unsigned_sites = {{"p", 4}};
        c.byte_sites = {{"*", 4}, {"*", 5}, {"data", 4660}, {"sir", 0}, {"bx*", 4}, {"bxw", 4}};   // ("g", 0) is no site: a global
        c.register_slots = true;
        c.fp_types = {{"v2", "float"}, {"a", "long"}};
        c.arrays = {{"g_0100", "char"}, {"g_0200", "int"}};
        c.struct_arrays = {{"g_0200", {8, "int"}}};
        c.streams = {{"f", "None"}, {"#include", "stdio.h"}, {"#include2", "ctype.h"}};
        c.word_pointers = {"r_si"};
        c.struct_pointers = {{"g_0200", "s_q"}};
    } else if (i == 4) {
        c.arrays = {{"g_0100", "struct s_g_0100"}, {"g_0102", "struct s_g_0102"}, {"g_0104", "struct s_g_0104"}, {"g_0200", "struct s_g_0200"}, {"g_0202", "struct s_g_0202"}};
        c.struct_arrays = {{"g_0100", {8, "int"}}, {"g_0102", {8, "char"}}, {"g_0104", {8, "struct s_g_0104 *"}}, {"g_0200", {6, "int"}}, {"g_0202", {6, "int"}}};
    }
    return c;
}

cand::State replay_state(const ReplayContext& c, int ctx) {
    cand::State s;
    dt::FunctionFacts& f = s.facts;
    f.arrays = c.arrays;
    for (const auto& [k, v] : c.struct_arrays) f.struct_arrays[k] = dt::StructArray{static_cast<std::uint32_t>(v.first), v.second, {}};
    for (const auto& [k, v] : c.streams) {
        if (k == "#include") f.include = v;
        else if (k == "#include2") f.include2 = v;
        else f.streams.insert_or_assign(k, v.value_or("None"));
    }
    f.word_pointers = c.word_pointers;
    f.unsigned_sites = c.unsigned_sites;
    f.unsigned_globals = c.unsigned_globals;
    f.byte_sites = c.byte_sites;
    if (ctx == 1) f.byte_globals = {"g_0010"};
    f.longs = c.longs;
    s.struct_pointers = c.struct_pointers;
    s.fp.types = c.fp_types;
    s.fp.ret = c.fp_ret;
    for (const auto& [off, w] : c.frame_slots) s.frame_slots.insert({off, w == "byte" ? cand::SlotWidth::Byte : w == "word" ? cand::SlotWidth::Word : cand::SlotWidth::ZeroExtended});
    return s;
}

// ---- the tables as the recording dumps them ----------------------------------------------------------------------

std::string replay_tables(const cand::State& s) {
    const dt::FunctionFacts& f = s.facts;
    std::vector<std::string> parts;
    const auto add = [&](std::string_view key, const std::string& value) { parts.push_back("\"" + std::string(key) + "\":" + value); };
    if (!f.arrays.empty()) {
        std::string v = "[";
        for (const auto& [k, t] : f.arrays) v += std::string(v.size() > 1 ? "," : "") + "[" + replay_json(k) + "," + replay_opt(t) + "]";
        add("a", v + "]");
    }
    if (!f.struct_arrays.empty()) {
        std::string v = "[";
        for (const auto& [k, a] : f.struct_arrays) {
            std::string type;
            if (a.fields.empty()) {
                type = replay_json(a.first_type);
            } else {
                type = "[";
                for (std::size_t i = 0; i < a.fields.size(); ++i) type += std::string(i ? "," : "") + std::format("[{},{}]", a.fields[i].offset, replay_json(a.fields[i].type));
                type += "]";
            }
            v += std::string(v.size() > 1 ? "," : "") + "[" + replay_json(k) + "," + std::format("[{},{}]", a.size, type) + "]";
        }
        add("s", v + "]");
    }
    if (!f.word_pointers.empty()) {
        std::string v = "[";
        for (const std::string& k : f.word_pointers) v += std::string(v.size() > 1 ? "," : "") + "[" + replay_json(k) + ",2]";
        add("p", v + "]");
    }
    {
        std::vector<std::pair<std::string, std::optional<std::string>>> fi;
        if (f.include) fi.emplace_back("#include", *f.include);
        if (f.include2) fi.emplace_back("#include2", *f.include2);
        for (const auto& kv : f.streams) fi.emplace_back(kv.first, kv.second == "None" ? std::nullopt : std::optional<std::string>(kv.second));
        std::ranges::sort(fi);
        if (!fi.empty()) {
            std::string v = "[";
            for (const auto& [k, t] : fi) v += std::string(v.size() > 1 ? "," : "") + "[" + replay_json(k) + "," + replay_opt(t) + "]";
            add("f", v + "]");
        }
    }
    if (!s.struct_pointers.empty()) {
        std::string v = "[";
        for (const auto& [k, t] : s.struct_pointers) v += std::string(v.size() > 1 ? "," : "") + "[" + replay_json(k) + "," + replay_opt(t) + "]";
        add("sp", v + "]");
    }
    {
        std::vector<std::string> li;
        const auto list = [&](std::string_view key, const auto& set) {
            if (set.empty()) return;
            std::string v = "[";
            bool first = true;
            for (const auto& x : set) {
                if constexpr (std::is_arithmetic_v<std::decay_t<decltype(x)>>) v += std::string(first ? "" : ",") + std::format("{}", x);
                else v += std::string(first ? "" : ",") + replay_json(x);
                first = false;
            }
            li.push_back("\"" + std::string(key) + "\":" + v + "]");
        };
        list("fields", f.longs.fields);
        list("globals", f.longs.globals);
        list("funcs", f.longs.funcs);
        list("unsigned", f.longs.unsigned_names);
        if (f.longs.ret) li.push_back(std::string("\"ret\":") + (*f.longs.ret ? "true" : "false"));
        if (!li.empty()) add("l", "{" + ct::join(li, ",") + "}");
    }
    {
        std::vector<std::string> fp;
        if (!s.fp.types.empty()) {
            std::string v = "[";
            for (const auto& [k, t] : s.fp.types) v += std::string(v.size() > 1 ? "," : "") + "[" + replay_json(k) + "," + replay_opt(t) + "]";
            fp.push_back("\"t\":" + v + "]");
        }
        if (s.fp.ret && !s.fp.ret->empty()) fp.push_back("\"r\":" + replay_json(*s.fp.ret));
        if (!fp.empty()) add("fp", "{" + ct::join(fp, ",") + "}");
    }
    if (!s.frame_slots.empty()) {
        std::vector<std::pair<std::int64_t, std::string>> slots;
        for (const cand::FrameSlot& slot : s.frame_slots) slots.emplace_back(slot.offset, slot.width == cand::SlotWidth::Byte ? "byte" : slot.width == cand::SlotWidth::Word ? "word" : "zx");
        std::ranges::sort(slots);
        std::string v = "[";
        for (const auto& [o, w] : slots) v += std::string(v.size() > 1 ? "," : "") + std::format("[{},{}]", o, replay_json(w));
        add("fw", v + "]");
    }
    if (!f.byte_globals.empty()) add("bg", replay_json(Strings(f.byte_globals.begin(), f.byte_globals.end())));
    if (!f.ranges.empty()) {
        std::string v = "[";
        for (const dt::Range& r : f.ranges) v += std::string(v.size() > 1 ? "," : "") + std::format("[{},{},{}]", r.lo, r.hi, r.above ? "true" : "false");
        add("r", v + "]");
    }
    if (!f.unsigned_sites.empty() || !f.unsigned_globals.empty()) {
        Strings u;
        for (const dt::Site& s2 : f.unsigned_sites) u.push_back(std::format("[{},{}]", replay_json(s2.pointer), s2.offset));
        for (const std::string& g : f.unsigned_globals) u.push_back(replay_json(g));
        std::ranges::sort(u);
        add("u", replay_json(u));   // (each a JSON text: dumped twice, as the recording does)
    }
    return "{" + ct::join(parts, ",") + "}";
}

// ---- the operations -------------------------------------------------------------------------------------------------

const std::vector<std::vector<std::pair<std::string, std::int64_t>>>& replay_regparams() {
    static const std::vector<std::vector<std::pair<std::string, std::int64_t>>> v = {{}, {{"si", 0}}, {{"di", 1}}, {{"si", 1}, {"di", 0}}, {{"si", 0}, {"di", 1}}, {{"si", 30}}, {{"di", 2}}};
    return v;
}

std::vector<std::set<dt::Site>> replay_widths() {
    return {{},
            {{"p", 2}, {"*", 4}, {"bx*", 2}, {"q", 0}},
            {{"*", 0}, {"r_si", 4}, {"si*", 4}, {"siw", 0}},
            {{"bx*", 0}, {"p", 4}, {"*", 4}, {"*", 6}, {"bx*", 2}, {"bxw", 2}, {"r_di", 2}},
            {{"si*", 0}, {"di*", 0}, {"p", 0}, {"r_si*", 2}, {"si*", 2}}};
}
std::vector<std::set<std::string>> replay_width_globals() { return {{}, {"g_0010"}, {}, {"g_0100"}, {}}; }

const std::vector<std::map<std::string, std::string>>& replay_helpers() {
    static const std::vector<std::map<std::string, std::string>> v = {
        {},
        {},
        {{"lmul", "*"}, {"ldiv", "/"}, {"lrem", "%"}, {"almul", "*="}, {"aldiv", "/="}, {"ulshr", ">>"}, {"aulshr", ">>="}, {"ulrem", "%"}, {"ulless", "<"}},
        {{"lmul", "*"}, {"almul", "*="}, {"ulless", "<"}, {"ulrem", "%"}}};
    return v;
}
const std::vector<Strings>& replay_routines() {
    static const std::vector<Strings> v = {{}, {}, {"ftol", "__ftol"}, {"ftol"}};
    return v;
}

const char* const kReplayByteTexts[] = {"", "x = BYTE(p->f2);", "BYTE(  (q->f4) )\nBYTE(*p)\nBYTE(*++r_si)\nBYTE(*(q++))\nBYTE(r_di->f10)", "BYTE(p->f0) BYTE(*(--s))", "y = BYTE(*s--) + BYTE(bx->f3);"};

class ReplayModel final : public cand::MarkingModel {
public:
    ReplayModel(std::string mode, ByteView code) : mode_(std::move(mode)), code_(code) {}
    std::optional<std::string> marked_c(ByteView) const override {
        if (mode_ == "fail" || mode_ == "failc") return std::nullopt;
        return std::string(kReplayByteTexts[replay_crc32(as_text(code_)) % 5]);
    }

private:
    std::string mode_;
    ByteView code_;
};

Bytes replay_hex(std::string_view h) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(h.substr(i, 2)), nullptr, 16)));
    return out;
}

struct ReplayResult {
    std::string json;     // the `r` value, or empty for "same"
    bool same = false;
};

ReplayResult replay_text(const std::string& input, const std::string& out) { return out == input ? ReplayResult{"", true} : ReplayResult{replay_json(out), false}; }

// Every case of the corpus in `dir`, compared; what differs is reported and counted.
std::size_t replay_corpus(const std::filesystem::path& dir) {
    const auto filter_env = fs::get_env("CB_CANDIDATES_REPLAY_FILTER");
    const auto read_lines = [&](const char* name) {
        std::ifstream in(dir / name, std::ios::binary);
        Strings lines;
        for (std::string l; std::getline(in, l);) lines.push_back(l);
        return lines;
    };
    Strings texts;
    for (const std::string& l : read_lines("texts.txt")) texts.push_back(replay_unescape(l));
    const Strings cases = read_lines("cases.txt"), expected = read_lines("expected.txt"), codes = read_lines("codes.txt");
    CB_CHECK_EQ(cases.size(), expected.size());
    const auto projects = replay_projects(dir / "kit");
    dt::ProjectCaches caches;
    {   // (the kit's headers are read by the first project that has one, as in the full run)
        for (const char* h : {"stdio.h", "ctype.h"}) (void)forms::header_names(projects[0]->settings.kit, caches, h);
    }
    caches.library_data["PRJ1"] = replay_libdata();
    caches.library_data["PRJ3"] = replay_libdata();
    const std::map<std::uint32_t, std::string> names = {{0x1234, "foo"}, {0x20, "bar"}, {0x1000, "start"}, {0x10, "tiny"}, {0x2234, "far_one"}, {0x1ffff, "edge"}};
    const std::uint32_t ats[] = {0x1000, 0, 0x10000, 0xfff0, 0x1ffff - 2};
    const auto widths_table = replay_widths();
    const auto globals_table = replay_width_globals();
    const Strings regs = {"si", "di"};
    std::map<std::string, std::size_t> failed, checked, error_divergences;
    std::size_t reported = 0;
    for (std::size_t n = 0; n < cases.size(); ++n) {
        const Strings f = [&] {
            Strings parts;
            for (const std::string_view p : ct::split(cases[n], '\t')) parts.push_back(replay_unescape(p));
            return parts;
        }();
        const std::string& op = f[0];
        if (filter_env && op != *filter_env) continue;
        const int index = std::stoi(f[1]), ctx = std::stoi(f[2]);
        const std::string text = index >= 0 ? texts[static_cast<std::size_t>(index)] : std::string();
        const ReplayContext context = replay_context(ctx);
        cand::State state = replay_state(context, ctx);
        const bool slots = context.register_slots;
        ReplayResult result;
        bool threw = false;
        try {
            if (op == "structs") {
                result = replay_text(text, cand::structs(text, widths_table[std::stoul(f[3])], state));
            } else if (op == "register_variables") {
                cand::RegisterParams params;
                for (const auto& [r, i] : replay_regparams()[std::stoul(f[3])]) params.insert_or_assign(r, static_cast<int>(i));
                result = replay_text(text, cand::register_variables(text, regs, params));
            } else if (op == "unsigned_locals") {
                const Strings v = cand::unsigned_locals(text, std::stoul(f[3]));
                result = {replay_json(v), false};
            } else if (op == "fill_frame") {
                result = replay_text(text, cand::fill_frame(text, static_cast<std::uint32_t>(std::stoul(f[3])), f[4] == "1", f[5] == "_p%d" ? cand::FillerName::ByPlace : cand::FillerName::Fixed));
            } else if (op == "string_variants") {
                result = {replay_json(cand::string_variants(projects[std::stoul(f[3])]->settings, text)), false};
            } else if (op == "long_helpers") {
                result = replay_text(text, cand::long_helpers(text, replay_helpers()[std::stoul(f[3])]));
            } else if (op == "fp_conversions") {
                result = replay_text(text, cand::fp_conversions(text, replay_routines()[std::stoul(f[3])], state));
            } else if (op == "localize" || op == "localize2") {
                const cand::Settings& s = projects[std::stoul(f[3])]->settings;
                const std::uint32_t at = ats[std::stoul(f[4])];
                dt::ProjectCaches scratch = caches;   // (project 4's macro has no name: the original raised before it asked for a header, so what it would cache is not kept)
                dt::ProjectCaches& use = f[3] == "3" ? scratch : caches;
                const cand::Candidate a = cand::localize(s, text, at, names, state, use);
                std::string json = "[" + replay_json(a.source) + "," + replay_json(a.registers_left);
                if (op == "localize2") {
                    const cand::Candidate b = cand::localize(s, text, at, names, state, use);
                    json += "," + replay_json(b.source) + "," + replay_json(b.registers_left);
                }
                result = {json + "]", false};
            } else if (op == "candidates") {
                const cand::Settings& s = projects[std::stoul(f[3])]->settings;
                cand::RegisterParams params;
                for (const auto& [r, i] : replay_regparams()[std::stoul(f[4])]) params.insert_or_assign(r, static_cast<int>(i));
                cand::ByteFields widths;
                widths.sites = widths_table[std::stoul(f[5])];
                widths.globals = globals_table[std::stoul(f[5])];
                dt::ProjectCaches scratch = caches;
                const auto made = cand::candidates(s, text, ats[std::stoul(f[6])], names, regs, params, widths, 0, state, f[3] == "3" ? scratch : caches);
                std::string json = "[";
                for (std::size_t i = 0; i < made.size(); ++i) json += std::string(i ? "," : "") + "[" + replay_json(made[i].source) + "," + replay_json(made[i].registers_left) + "]";
                result = {json + "]", false};
            } else if (op == "byte_fields") {
                const Bytes code = replay_hex(codes[std::stoul(f[3])]);
                const ReplayModel model(f[4], code);
                const auto found = cand::byte_fields(f[4] == "nomark" ? nullptr : &model, code);
                Strings items;
                if (found) {
                    for (const dt::Site& s : found->sites) items.push_back(std::format("[{},{}]", replay_json(s.pointer), s.offset));
                    for (const std::string& g : found->globals) items.push_back(std::format("[\"g\",{}]", replay_json(g)));
                    std::ranges::sort(items);
                    state.frame_slots = found->frame_slots;
                    state.facts.ranges = found->ranges;
                }
                std::string json = "[";
                for (std::size_t i = 0; i < items.size(); ++i) json += std::string(i ? "," : "") + items[i];
                result = {json + "]", false};
            } else if (op == "frame_chars") {
                result = replay_text(text, cand::frame_chars(text, state));
            } else if (op == "fp_types") {
                result = replay_text(text, cand::fp_types(text, state.fp));
            } else if (op == "buffers") {
                const auto v = cand::buffers(text);
                result = {replay_opt(v), false};
                if (v && *v == text) result = {"", true};
            } else if (op == "void_variant") {
                const auto v = cand::void_variant(text);
                result = {replay_opt(v), false};
                if (v && *v == text) result = {"", true};
            } else if (op == "registers_last") {
                const auto v = cand::registers_last(text, slots);
                result = {replay_opt(v), false};
                if (v && *v == text) result = {"", true};
            } else if (op == "register_slots") {
                const auto v = cand::register_slots(text, slots);
                result = {replay_opt(v), false};
                if (v && *v == text) result = {"", true};
            } else if (op == "long_returns") {
                result = replay_text(text, cand::long_returns(text));
            } else if (op == "long_pairs") {
                result = replay_text(text, cand::long_pairs(text, state));
            } else if (op == "long_ladders") {
                result = replay_text(text, cand::long_ladders(text));
            } else if (op == "local_pointers") {
                result = replay_text(text, cand::local_pointers(text));
            } else {
                CB_CHECK(false);
                continue;
            }
        } catch (const std::exception&) {
            threw = true;
        }
        if (const auto dump = fs::get_env("CB_CANDIDATES_REPLAY_DUMP"); dump && *dump == std::to_string(n)) fs::write_text(dir / ("mine_" + *dump + ".json"), result.json);
        const std::string& want = expected[n];
        ++checked[op];
        const auto crc_form = [](const std::string& b) { return std::format("{{\"crc\":{},\"len\":{}}}", replay_crc32(b), b.size()); };
        {
            // (the original raised: its dynamic typing - not an answer to compare; an answer after the first 200 texts is a crc, so the
            // error is recognised by the crc of the error names the original raises)
            bool was_error = want.starts_with("{\"error\"");
            for (const char* e : {"AttributeError", "ValueError", "TypeError", "KeyError", "IndexError", "ZeroDivisionError"})
                was_error = was_error || want.starts_with(crc_form(std::string("{\"error\":\"") + e + "\"}").substr(0, 0) + "{\"crc\":" + std::to_string(replay_crc32(std::string("{\"error\":\"") + e + "\"}")) + ",");
            if (was_error) {
                if (!threw) ++error_divergences[op];
                continue;
            }
        }
        std::string body;
        if (threw) body = "{\"threw\":true}";
        else if (result.same) body = "{\"same\":true}";
        else body = "{\"r\":" + result.json + "}";
        if (index >= 200 && !threw) body = crc_form(body);
        const std::string tables = replay_tables(state);
        if (tables != "{}") body = body.substr(0, body.size() - 1) + ",\"t\":" + tables + "}";
        if (body != want) {
            ++failed[op];
            if (reported++ < 12) {
                std::printf("  case %zu: %s\n", n, cases[n].substr(0, 100).c_str());
                CB_CHECK_EQ(body.substr(0, 400), want.substr(0, 400));
            }
        }
    }
    std::size_t total_failed = 0;
    for (const auto& kv : failed) total_failed += kv.second;
    for (const auto& [op, n] : checked) std::printf("  replay %-20s %6zu cases, %zu differ, %zu errors of the original answered\n", op.c_str(), n, failed.contains(op) ? failed.at(op) : 0, error_divergences.contains(op) ? error_divergences.at(op) : 0);
    return total_failed;
}

}  // namespace

// The checked-in part of the corpus (src/exact/testdata/draft_candidates): the smallest texts on which each pass
// changes something, and a few it leaves alone, with the recorded answers and the tables it filled.
CB_TEST("exact.draft.candidates.checked_in_corpus", [] {
    CB_CHECK_EQ(replay_corpus(testing::data_path("exact/testdata/draft_candidates")), 0u);
});

// The whole recorded corpus (about 40,000 cases): CB_CANDIDATES_REPLAY names its folder.
CB_TEST("exact.draft.candidates.replay_of_the_recorded_corpus", [] {
    const auto dir = fs::get_env("CB_CANDIDATES_REPLAY");
    if (!dir) return;
    CB_CHECK_EQ(replay_corpus(fs::from_text(*dir)), 0u);
});
