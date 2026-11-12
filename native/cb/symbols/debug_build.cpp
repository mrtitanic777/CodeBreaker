// cb-unity: alone (file-local helpers named for this module)
#include "cb/symbols/debug_build.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::symbols::debug_build {

namespace segment = source::ne_asm::segment;
namespace ne = formats::ne;

namespace {

bool is_hex_lower(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }
bool is_hex_any(char c) { return is_hex_lower(c) || (c >= 'A' && c <= 'F'); }
bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// How many characters of `s` from `at` are lowercase hex digits.
std::size_t hex_run(std::string_view s, std::size_t at) {
    std::size_t n = 0;
    while (at + n < s.size() && is_hex_lower(s[at + n])) ++n;
    return n;
}

bool is_branch_word(std::string_view word) {
    static constexpr std::array<std::string_view, 31> branches = {
        "call", "jmp", "je", "jne", "jz", "jnz", "jl", "jle", "jg", "jge", "jb", "jbe", "ja", "jae", "js", "jns", "jo", "jno", "jp",
        "jnp", "jpe", "jpo", "jc", "jnc", "jcxz", "jecxz", "loop", "loope", "loopne", "loopz", "loopnz"};
    return std::find(branches.begin(), branches.end(), word) != branches.end();
}

// `0x<hex>` at `at`: its length, 0 when it is not one.
std::size_t hex_number(std::string_view s, std::size_t at) {
    if (at + 2 >= s.size() + 0 || s.compare(at, 2, "0x") != 0) return 0;
    const std::size_t digits = hex_run(s, at + 2);
    return digits ? 2 + digits : 0;
}

// One instruction's text with the offset-dependent operands masked.
std::string masked(std::string_view instruction) {
    std::string a;   // whitespace collapsed to one blank, ends trimmed
    for (const std::string_view word : text::split_ws(instruction)) a += (a.empty() ? "" : " ") + std::string(word);
    // a branch or call target: the mnemonic, a blank, then `0x<hex>`
    const std::size_t blank = a.find(' ');
    if (blank != std::string::npos && is_branch_word(std::string_view(a).substr(0, blank))) {
        if (const std::size_t n = hex_number(a, blank + 1)) a.replace(blank + 1, n, "X");
    }
    // an absolute memory operand `[0x<hex>]`
    std::string b;
    for (std::size_t i = 0; i < a.size();) {
        if (a[i] == '[') {
            if (const std::size_t n = hex_number(a, i + 1); n && i + 1 + n < a.size() && a[i + 1 + n] == ']') {
                b += "[X]";
                i += n + 2;
                continue;
            }
        }
        b += a[i++];
    }
    // a far pointer `0x<hex>:0x<hex>`
    std::string c;
    for (std::size_t i = 0; i < b.size();) {
        if (const std::size_t first = hex_number(b, i)) {
            if (i + first < b.size() && b[i + first] == ':') {
                if (const std::size_t second = hex_number(b, i + first + 1)) {
                    c += "X:X";
                    i += first + 1 + second;
                    continue;
                }
            }
        }
        c += b[i++];
    }
    return c;
}

std::vector<std::uint32_t> code_segments(const ne::Module& m) {
    std::vector<std::uint32_t> out;
    std::uint32_t i = 0;
    for (const ne::Segment& s : m.segments()) {
        ++i;
        if (!s.is_data()) out.push_back(i);
    }
    return out;
}

bool lower_starts_with_sub(std::string_view name) { return text::lower(name).starts_with("sub_"); }

}  // namespace

std::string FunctionKey::text() const { return std::format("{}:{:04x}", segment, offset); }

Signature signature(const segment::Seg& seg, std::uint32_t start) {
    Signature out;
    for (const segment::Row& row : seg.function_rows(start).rows) out.push_back(masked(row.text));
    return out;
}

std::map<std::uint32_t, std::map<std::uint32_t, std::string>> parse_map(std::string_view map_text) {
    std::map<std::uint32_t, std::map<std::uint32_t, std::string>> out;
    for (const std::string_view line : text::lines(map_text)) {   // ` SSSS:OOOO  name`
        if (line.size() < 12 || line[0] != ' ' || line[5] != ':') continue;
        const std::string_view seg = line.substr(1, 4), off = line.substr(6, 4);
        if (!std::all_of(seg.begin(), seg.end(), is_hex_any) || !std::all_of(off.begin(), off.end(), is_hex_any)) continue;
        std::size_t at = 10;
        if (!text::is_space(line[at])) continue;
        while (at < line.size() && text::is_space(line[at])) ++at;
        std::size_t end = at;
        while (end < line.size() && is_word(line[end])) ++end;
        if (end == at) continue;
        out[*text::parse_int<std::uint32_t>(seg, 16)][*text::parse_int<std::uint32_t>(off, 16)] = std::string(line.substr(at, end - at));
    }
    return out;
}

SignatureIndex build_index(const ne::Module& debug_build, std::string_view map_text, const segment::SharedOrdinals& ordinals, const source::ne_asm::fixups::Objdump& objdump) {
    const auto names = parse_map(map_text);
    SignatureIndex index;
    std::set<Signature> ambiguous;
    for (const std::uint32_t i : code_segments(debug_build)) {
        const auto named = names.find(i);
        if (named == names.end()) continue;
        const segment::Seg seg(debug_build, i, ordinals, {}, objdump);
        for (const auto& [start, name] : named->second) {
            if (!seg.is_instruction(start)) continue;
            Signature sig = signature(seg, start);
            if (sig.empty()) continue;
            const auto known = index.find(sig);
            if (known != index.end() && known->second != name) ambiguous.insert(sig);
            else index[sig] = name;
        }
    }
    for (const Signature& sig : ambiguous) index.erase(sig);
    return index;
}

Match match_module(const ne::Module& retail, const SignatureIndex& index, const segment::SharedOrdinals& ordinals, std::size_t min_length,
                   const source::ne_asm::fixups::Objdump& objdump) {
    std::map<std::uint32_t, segment::Seg> segs;
    for (const std::uint32_t i : code_segments(retail)) segs.emplace(i, segment::Seg(retail, i, ordinals, {}, objdump));

    // export validation: the signatures the retail exports own, and where each export is
    std::map<Signature, std::set<std::string>> export_sig;
    std::map<std::string, FunctionKey> export_at;   // UPPERCASE name -> where
    for (const auto& [i, seg] : segs) {
        for (const auto& [start, func] : seg.functions()) {
            const std::string name = seg.name_at(start);
            if (name.empty() || lower_starts_with_sub(name)) continue;
            export_sig[signature(seg, start)].insert(text::upper(name));
            export_at[text::upper(name)] = {i, start};
        }
    }
    SignatureIndex checked;   // a signature an export owns under another name is dropped
    for (const auto& [sig, name] : index) {
        const auto owners = export_sig.find(sig);
        if (owners != export_sig.end() && !owners->second.empty() && !owners->second.contains(text::upper(name))) continue;
        checked.emplace(sig, name);
    }

    Match out;
    for (const auto& [i, seg] : segs) {
        for (const auto& [start, func] : seg.functions()) {
            const Signature sig = signature(seg, start);
            if (sig.size() < min_length) continue;
            const auto found = checked.find(sig);
            if (found == checked.end()) continue;
            const std::string& real = found->second;
            // never shadow a real export at a different offset (case-insensitive)
            if (const auto clash = export_at.find(text::upper(real)); clash != export_at.end() && !(clash->second == FunctionKey{i, start})) continue;
            const std::string current = seg.name_at(start);
            if (!current.empty() && !lower_starts_with_sub(current)) {
                const bool same = text::upper(current) == text::upper(real);
                out.agree += same;
                out.disagree += !same;
            }
            out.names[{i, start}] = real;
        }
    }
    // a real name lives at one address: every offset under a duplicated name goes
    std::map<std::string, std::size_t> claims;
    for (const auto& [key, name] : out.names) ++claims[name];
    std::erase_if(out.names, [&](const auto& kv) { return claims.at(kv.second) > 1; });
    return out;
}

Match recover(const ne::Module& retail, const ne::Module& debug_build, std::string_view map_text, const segment::SharedOrdinals& ordinals,
              const source::ne_asm::fixups::Objdump& objdump) {
    return match_module(retail, build_index(debug_build, map_text, ordinals, objdump), ordinals, kMinLength, objdump);
}

std::string to_json(const Names& names) {
    std::map<std::string, std::string> by_text;   // sorted as text, as the Python's sort_keys did
    for (const auto& [key, name] : names) by_text[key.text()] = name;
    if (by_text.empty()) return "{}";
    std::string out = "{\n";
    bool first = true;
    for (const auto& [key, name] : by_text) {
        if (!first) out += ",\n";
        first = false;
        out += Json(key).dump() + ": " + Json(name).dump();
    }
    return out + "\n}";
}

std::map<std::size_t, segment::NameOverrides> parse_overrides(std::string_view names_json) {
    Json parsed;
    try {
        parsed = Json::parse(names_json);
    } catch (const std::exception& e) {
        throw ParseError("names.json", 1, 1, e.what());
    }
    if (!parsed.is_object()) throw ParseError("names.json", 1, 1, "a JSON object of \"seg:off\" -> name was expected");
    std::map<std::size_t, segment::NameOverrides> out;
    for (const auto& [key, value] : parsed.items()) {
        const std::size_t colon = key.find(':');
        const auto seg = colon == std::string::npos ? std::nullopt : text::parse_int<std::size_t>(std::string_view(key).substr(0, colon), 10);
        const auto off = colon == std::string::npos ? std::nullopt : text::parse_int<std::uint32_t>(std::string_view(key).substr(colon + 1), 16);
        if (!seg || !off || !value.is_string()) throw ParseError("names.json", 1, 1, "the entry '" + key + "' is not \"seg:off\" -> name");
        out[*seg][*off] = value.get<std::string>();
    }
    return out;
}

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "debug-names",
        "the real internal names of KRNL386 from Microsoft's debug build of it",
        {cli::Option{"--ords", "", "the ORDINALS.md to name ordinals from", cli::Arg::Value, "PATH", "ORDINALS.md"}},
        [&](const cli::Args& a) -> int {
            const segment::SharedOrdinals ordinals = std::make_shared<const segment::Ordinals>(segment::load_ordinals(fs::from_text(a.required("--ords"))));
            const std::filesystem::path retail = env.corpus.resolve("krnl386.exe");
            const std::filesystem::path debug_folder = env.corpus.reference({"WIN32SDK", "SCT", "DEBUG16", "I386"});
            const ne::Module retail_module = source::ne_asm::fixups::open_module(retail);
            const ne::Module debug_module = source::ne_asm::fixups::open_module(debug_folder / "KRNL386.EXE");
            const Match found = recover(retail_module, debug_module, fs::read_text(debug_folder / "KRNL386.MAP"), ordinals, env.objdump);
            const std::filesystem::path out = env.state_folder / "idiomatic" / "krnl386" / "names.json";
            fs::make_dirs(out.parent_path());
            fs::write_text(out, to_json(found.names));
            cb::out("export validation: {} agree, {} disagree (must be 0)\n", found.agree, found.disagree);
            cb::out("saved {} exact real internal names -> {}\n", found.names.size(), fs::to_text(out));
            return found.disagree == 0 ? kExitOk : kExitFailed;
        },
        {}, {}, {}};
    const cli::Cli program("debug-names", {command});
    std::vector<std::string> line = {"debug-names"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::symbols::debug_build
