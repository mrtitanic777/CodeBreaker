// cb-unity: alone
#include "verify/gate16/scoreboard16.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "build/msc6.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/process.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "platforms/decoder.hpp"
#include "verify/verdicts.hpp"

namespace cb::verify::gate16::scoreboard16 {

namespace {

struct Pattern {
    std::string_view name;
    Regex re;
};

// what the decompiler writes that a programmer does not, and that a compiler turns into more code
const std::vector<Pattern>& patterns() {
    static const std::vector<Pattern> all = [] {
        std::vector<Pattern> v;
        v.push_back({"CONCAT", Regex(R"(\bCONCAT\d\d\()")});
        v.push_back({"MK_FP", Regex(R"(\bMK_FP\()")});
        v.push_back({"segment helper", Regex(R"(\bcodebreaker_(ss|ds|cs|es)\(\))")});
        v.push_back({"split far (>> 0x10)", Regex(R"(>>\s*0x10\b)")});
        v.push_back({"xunknown", Regex(R"(\bxunknown\d\b)")});
        v.push_back({"unaff_/in_/extraout_", Regex(R"(\b(unaff|in|extraout)_[A-Z]{2}\b)")});
        v.push_back({"SUB/ZEXT/SEXT", Regex(R"(\b(SUB|ZEXT|SEXT)\d\d\()")});
        v.push_back({"constant DS address", Regex(R"(\*\s*\(\s*\w+\s*\*\s*\)\s*0x[0-9a-f]{2,4}\b)")});
        return v;
    }();
    return all;
}

std::string hex_text(const Bytes& bytes) {
    std::string out;
    out.reserve(bytes.size() * 2);
    for (const std::uint8_t b : bytes) out += std::format("{:02x}", static_cast<unsigned>(b));
    return out;
}

Bytes bytes_of_hex(std::string_view hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) {
        const auto v = text::parse_int<unsigned>(hex.substr(i, 2), 16);
        if (!v) throw FormatError("score16 board", i, "a function's bytes are not hexadecimal");
        out.push_back(static_cast<std::uint8_t>(*v));
    }
    return out;
}

std::string pad_to(std::string_view s, std::size_t width) {
    std::string out(s);
    if (out.size() < width) out.append(width - out.size(), ' ');
    return out;
}

}  // namespace

std::vector<std::pair<std::string, std::size_t>> count_patterns(std::string_view text) {
    std::vector<std::pair<std::string, std::size_t>> out;
    for (const Pattern& p : patterns()) {
        const std::size_t n = p.re.for_each_match(text, [](const RegexMatch&) { return true; });
        if (n) out.emplace_back(std::string(p.name), n);
    }
    return out;
}

std::vector<std::string> watcom_flags(std::string_view microsoft_switches) {
    // (dos: no Windows prolog -- /Gw gives -zW)
    std::vector<std::string> out = {"-bt=dos", "-zq", "-w0", "-zl", "-ecc", "-zu", "-zmsc", "-j", "-zastd=c99"};
    for (const std::string_view word : text::split_ws(microsoft_switches)) {
        const std::string_view f = text::ltrim(word.substr(std::min(word.size(), word.find_first_not_of("/-"))));
        if (f.starts_with('A')) {
            out.push_back(f == "AS" ? "-ms" : f == "AM" ? "-mm" : f == "AC" ? "-mc" : f == "AL" ? "-ml" : "-mm");
        } else if (f.starts_with('G')) {
            for (const char c : f.substr(1)) {
                if (c == '2') out.push_back("-2");
                else if (c == '3') out.push_back("-3");
                else if (c == 's') out.push_back("-s");
                else if (c == 'w') out.push_back("-zW");
            }
        } else if (f.starts_with('O')) {
            const std::string_view opts = f.substr(1);
            const bool has_x = opts.find('x') != std::string_view::npos;
            out.push_back(opts == "d" ? "-od" : has_x ? "-obmilr" : "-o" + std::string(opts));
            if (has_x && opts.find('s') != std::string_view::npos) out.push_back("-os");
        }
    }
    return out;
}

bool same_code(const Masked& a, const Masked& b) {
    if (a.code.size() != b.code.size()) return false;
    for (std::size_t i = 0; i < a.code.size(); ++i) {
        const bool filled = (i < a.mask.size() && a.mask[i]) || (i < b.mask.size() && b.mask[i]);
        if (a.code[i] != b.code[i] && !filled) return false;
    }
    return true;
}

Masked trim_padding(const Masked& code) {
    std::size_t n = code.code.size();
    const auto padding = [](std::uint8_t b) { return b == 0xCC || b == 0x90 || b == 0x00; };
    while (n > 1 && !(n - 1 < code.mask.size() && code.mask[n - 1]) && padding(code.code[n - 1])) {
        const std::uint8_t before = code.code[n - 2];
        if (!(before == 0xC3 || padding(before))) break;
        --n;
    }
    Masked out;
    out.code.assign(code.code.begin(), code.code.begin() + static_cast<std::ptrdiff_t>(n));
    out.mask.assign(code.mask.begin(), code.mask.begin() + static_cast<std::ptrdiff_t>(std::min(n, code.mask.size())));
    return out;
}

std::string_view word(Agreement a) {
    switch (a) {
        case Agreement::Same: return "same";
        case Agreement::Differs: return "differs";
        case Agreement::NotApplicable: break;
    }
    return "n/a";
}

WatcomCode compile_watcom(const build::watcom::OpenWatcom& watcom, const std::vector<std::vector<std::string>>& files,
                          const std::map<std::string, std::string>& texts, std::string_view header,
                          const std::function<std::string(const std::string&)>& switches_of, const Log& log) {
    namespace fs = std::filesystem;
    const fs::path wcc = watcom.cc(build::watcom::Target::Win16);
    const auto env = watcom.environment(build::watcom::Target::Win16);
    const cb::fs::ScopedTempDir work("score16");
    cb::fs::write_text(work.path() / "m.h", header);

    struct Compiled {
        std::optional<Bytes> object;
        std::string why;
    };
    // one source file: the functions together under the switches of the first
    const auto compile_one = [&](const std::string& stem, const std::vector<std::string>& names) -> Compiled {
        std::string source = "#include \"m.h\"\n\n";
        for (std::size_t i = 0; i < names.size(); ++i) source += (i ? "\n\n" : "") + texts.at(names[i]);
        source += "\n";
        cb::fs::write_text(work.path() / (stem + ".c"), source);
        process::ProcessSpec spec;
        spec.argv = {wcc.string()};
        for (std::string& f : watcom_flags(switches_of(names[0]))) spec.argv.push_back(std::move(f));
        spec.argv.push_back("-fo=" + stem + ".obj");
        spec.argv.push_back(stem + ".c");
        spec.cwd = work.path();
        spec.env = env;
        spec.timeout = std::chrono::minutes(5);
        const process::ProcessResult r = process::run(spec);
        const fs::path object = work.path() / (stem + ".obj");
        if (r.exit_code == 0 && fs::is_regular_file(object)) return {cb::fs::read_file(object), ""};
        const std::string all = text::from_cp1252(r.out) + text::from_cp1252(r.err);
        for (const std::string_view line : text::lines(all))
            if (line.find("Error!") != std::string_view::npos) return {std::nullopt, std::string(line)};
        const std::string_view tail = text::trim(all);
        return {std::nullopt, std::string(tail.substr(tail.size() > 200 ? tail.size() - 200 : 0))};
    };
    const auto code_of = [](const Bytes& object, const std::string& name) -> std::optional<Masked> {
        const auto c = build::msc6::function_code(object, name);
        if (!c) return std::nullopt;
        return Masked{c->code, c->mask};
    };

    WatcomCode out;
    for (std::size_t k = 0; k < files.size(); ++k) {
        std::vector<std::string> names;
        for (const std::string& n : files[k])
            if (texts.contains(n)) names.push_back(n);
        if (names.empty()) continue;
        const Compiled whole = compile_one(std::format("f{:04}", k), names);
        if (!whole.object && names.size() > 1) {   // (one function's error does not sink its file)
            for (std::size_t j = 0; j < names.size(); ++j) {
                const Compiled one = compile_one(std::format("f{:04}_{:02}", k, j), {names[j]});
                out.code[names[j]] = one.object ? code_of(*one.object, names[j]) : std::nullopt;
                if (!one.object) out.refused[names[j]] = one.why;
            }
            continue;
        }
        for (const std::string& n : names) {
            out.code[n] = whole.object ? code_of(*whole.object, n) : std::nullopt;
            if (!whole.object) out.refused[n] = whole.why;
        }
    }
    if (log && !out.refused.empty()) {
        const auto& first = *out.refused.begin();
        log(std::format("  OpenWatcom refused {} functions, e.g. {}: {}", out.refused.size(), first.first, first.second));
    }
    return out;
}

Rows run(const Inputs& in) {
    CB_ASSERT(in.result && in.texts && in.watcom && in.toolchain, "score16: the match result, the texts, OpenWatcom and the toolchain are needed");
    const authored::match16::Result& result = *in.result;
    // the functions Microsoft C compiled (or that are the library's), in the module's order, of the segment asked for
    std::vector<const authored::match16::Function*> chosen;
    for (const authored::match16::Function& f : in.functions) {
        if (!result.code.contains(f.name)) continue;
        if (in.segment && f.segment != *in.segment) continue;
        chosen.push_back(&f);
    }
    std::vector<std::vector<std::string>> files;   // match16's source files, the chosen functions of each
    for (const std::vector<std::string>& file : result.files) {
        std::vector<std::string> names;
        for (const std::string& n : file)
            if (std::any_of(chosen.begin(), chosen.end(), [&](const auto* f) { return f->name == n; })) names.push_back(n);
        if (!names.empty()) files.push_back(std::move(names));
    }
    const WatcomCode ow = compile_watcom(
        *in.watcom, files, *in.texts, in.header,
        [&](const std::string& n) {
            const auto it = result.setting.find(n);
            return it == result.setting.end() ? std::string() : it->second.switches;
        },
        in.log);
    Rows rows;
    for (const authored::match16::Function* f : chosen) {
        const std::string& n = f->name;
        const std::optional<Masked>& ms = result.code.at(n);
        const auto ow_it = ow.code.find(n);
        const std::optional<Masked> o = ow_it == ow.code.end() ? std::nullopt : ow_it->second;
        const authored::match16::Verdict msv = ms ? in.toolchain->distance(*ms, f->original) : authored::match16::Verdict{};
        const Masked orig = trim_padding(f->original);
        Row r;
        r.segment = f->segment;
        r.original = orig.code.size();
        std::optional<Masked> ms_trim, o_trim;
        if (ms) ms_trim = trim_padding(*ms);
        if (o) o_trim = trim_padding(*o);
        if (ms_trim) r.msc8 = ms_trim->code.size();
        r.msc8_vs_original = ms ? msv.match : ByteMatch::NoCompile;
        r.msc8_bytes_off = ms ? msv.differing : 0;
        if (o_trim) r.watcom = o_trim->code.size();
        r.watcom_vs_msc8 = ms_trim && o_trim ? (same_code(*o_trim, *ms_trim) ? Agreement::Same : Agreement::Differs) : Agreement::NotApplicable;
        r.fits = o_trim && o_trim->code.size() <= r.original;
        const auto text = in.texts->find(n);
        r.c = text == in.texts->end() ? std::string() : text->second;
        r.patterns = count_patterns(r.c);
        const auto verdict = result.verdicts.find(n);
        r.library = verdict != result.verdicts.end() && verdict->second.match == ByteMatch::Identified;
        r.hex.original = hex_text(f->original.code);
        r.hex.msc8 = ms ? hex_text(ms->code) : "";
        r.hex.watcom = o ? hex_text(o->code) : "";
        rows.insert_or_assign(n, std::move(r));
    }
    return rows;
}

std::vector<std::string> report_lines(const Rows& rows) {
    std::vector<const Row*> c;   // the compiler's run-time library aside
    for (const auto& kv : rows)
        if (!kv.second.library) c.push_back(&kv.second);
    const std::size_t n = c.size();
    std::size_t ident = 0, same_ow = 0, fits = 0, off = 0;
    for (const Row* r : c) {
        ident += r->msc8_vs_original == ByteMatch::Identical;
        same_ow += r->watcom_vs_msc8 == Agreement::Same;
        fits += r->fits;
        off += r->watcom.value_or(0) > r->original ? *r->watcom - r->original : 0;
    }
    std::vector<std::string> out;
    out.push_back(std::format("{} functions (runtime library aside)", n));
    out.push_back(std::format("  C        Microsoft C 8 gives the original's bytes: {:4}   (decompiler's work: {})", ident, n - ident));
    out.push_back(std::format("  compiler OpenWatcom -zmsc gives Microsoft C 8's:   {:4}   (compiler's work: {})", same_ow, n - same_ow));
    out.push_back(std::format("  fits     OpenWatcom's no longer than the original:  {:4}   (would move: {})", fits, n - fits));
    out.push_back(std::format("  bytes over the original's length, all together: {}", off));
    struct Tally {
        std::string pattern;
        std::size_t functions = 0, bytes = 0;
    };
    std::vector<Tally> tally;   // in the order the patterns were first met; the largest are listed first
    for (const Row* r : c) {
        if (r->msc8_vs_original == ByteMatch::Identical) continue;
        for (const auto& p : r->patterns) {
            auto it = std::find_if(tally.begin(), tally.end(), [&](const Tally& t) { return t.pattern == p.first; });
            if (it == tally.end()) {
                tally.push_back({p.first, 0, 0});
                it = tally.end() - 1;
            }
            it->functions += 1;
            it->bytes += r->msc8_bytes_off;
        }
    }
    out.push_back("  the decompiler's patterns in functions Microsoft C 8 does not match (functions, bytes off):");
    std::stable_sort(tally.begin(), tally.end(), [](const Tally& a, const Tally& b) { return a.bytes > b.bytes; });
    for (const Tally& t : tally) out.push_back(std::format("    {} {:4}  {:6}", pad_to(t.pattern, 24), t.functions, t.bytes));
    return out;
}

std::vector<std::string> show_lines(const Row& row) {
    platforms::DecoderSet decoders;
    const platforms::Decoder& cs = decoders.get(platforms::Arch::X86, 16);
    std::vector<std::string> out;
    out.push_back(row.c);
    std::vector<std::vector<std::string>> cols;
    for (const std::string* hex : {&row.hex.original, &row.hex.msc8, &row.hex.watcom}) {
        std::vector<std::string> col;
        for (const platforms::Insn& i : cs.disasm(bytes_of_hex(*hex), 0)) col.push_back(i.mnemonic + " " + i.op_str);
        cols.push_back(std::move(col));
    }
    out.push_back(std::format("  {}| {}| {}", pad_to("original", 32), pad_to("Microsoft C 8", 32), "OpenWatcom -zmsc"));
    std::size_t most = 0;
    for (const auto& col : cols) most = std::max(most, col.size());
    for (std::size_t i = 0; i < most; ++i) {
        const auto at = [&](std::size_t k) { return i < cols[k].size() ? cols[k][i] : std::string(); };
        out.push_back(std::format("  {}| {}| {}", pad_to(at(0), 32), pad_to(at(1), 32), at(2)));
    }
    return out;
}

std::string rows_to_json(const Rows& rows) {
    Json doc = Json::object();
    for (const auto& [name, r] : rows) {
        Json patterns = Json::object();
        for (const auto& [p, k] : r.patterns) patterns[p] = k;
        Json row = Json::object();
        row["segment"] = r.segment ? Json(*r.segment) : Json(nullptr);
        row["original"] = r.original;
        row["msc8"] = r.msc8 ? Json(*r.msc8) : Json(nullptr);
        row["msc8_vs_original"] = std::string(verify::word(r.msc8_vs_original));
        row["msc8_bytes_off"] = r.msc8_bytes_off;
        row["watcom"] = r.watcom ? Json(*r.watcom) : Json(nullptr);
        row["watcom_vs_msc8"] = std::string(word(r.watcom_vs_msc8));
        row["fits"] = r.fits;
        row["patterns"] = std::move(patterns);
        row["library"] = r.library;
        row["c"] = r.c;
        row["hex"] = Json{{"original", r.hex.original}, {"msc8", r.hex.msc8}, {"watcom", r.hex.watcom}};
        doc[name] = std::move(row);
    }
    return doc.dump(1);
}

Rows rows_from_json(std::string_view json) {
    Json doc;
    if (!json::try_parse(json, doc) || !doc.is_object()) throw FormatError("score16 board", 0, "not a JSON object");
    Rows rows;
    try {
        for (const auto& [name, v] : doc.items()) {
            Row r;
            if (!v.at("segment").is_null()) r.segment = v.at("segment").get<std::uint32_t>();
            r.original = v.at("original").get<std::size_t>();
            if (!v.at("msc8").is_null()) r.msc8 = v.at("msc8").get<std::size_t>();
            r.msc8_vs_original = verify::parse_byte_match(v.at("msc8_vs_original").get<std::string>()).value_or(ByteMatch::NoCompile);
            r.msc8_bytes_off = v.at("msc8_bytes_off").get<std::size_t>();
            if (!v.at("watcom").is_null()) r.watcom = v.at("watcom").get<std::size_t>();
            const std::string agree = v.at("watcom_vs_msc8").get<std::string>();
            r.watcom_vs_msc8 = agree == "same" ? Agreement::Same : agree == "differs" ? Agreement::Differs : Agreement::NotApplicable;
            r.fits = v.at("fits").get<bool>();
            for (const auto& [p, k] : v.at("patterns").items()) r.patterns.emplace_back(p, k.get<std::size_t>());
            r.library = v.at("library").get<bool>();
            r.c = v.at("c").get<std::string>();
            r.hex = {v.at("hex").at("original").get<std::string>(), v.at("hex").at("msc8").get<std::string>(),
                     v.at("hex").at("watcom").get<std::string>()};
            rows.insert_or_assign(name, std::move(r));
        }
    } catch (const nlohmann::json::exception& e) {
        throw FormatError("score16 board", 0, e.what());
    }
    return rows;
}

std::optional<Arguments> parse_arguments(const std::vector<std::string>& args) {
    Arguments out;
    bool showing = false;
    for (std::size_t i = 0; i < args.size(); ++i) {
        const std::string& a = args[i];
        if (a == "--show" || a == "--json" || a == "--segment") {
            if (i + 1 >= args.size()) return std::nullopt;
            const std::string& value = args[++i];
            if (a == "--show") {
                showing = true;
                for (const std::string_view f : text::split(value, ',')) out.show.emplace_back(f);
            } else if (a == "--json") {
                out.json = value;
            } else {
                const auto n = text::parse_int<std::uint32_t>(value);
                if (!n) return std::nullopt;
                out.segment = *n;
            }
        } else if (a.starts_with("--")) {
            continue;
        } else if (out.module.empty()) {
            out.module = a;
        }
    }
    if (showing) return out.json.empty() ? std::nullopt : std::optional<Arguments>(out);
    return out.module.empty() ? std::nullopt : std::optional<Arguments>(out);
}

}  // namespace cb::verify::gate16::scoreboard16
