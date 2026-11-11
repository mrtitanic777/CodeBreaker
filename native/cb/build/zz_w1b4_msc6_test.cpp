// LOCAL harness for the W1-B4 refactor (never committed): runs the new msc6 text passes over the inputs of the old harness
// (state/work/typed-port/build.msc6) and writes what it prints, for comparison with the Python's expected.txt. Does nothing unless
// CB_W1B4_MSC6 is set.
#include <nlohmann/json.hpp>

#include <chrono>
#include <cstdio>
#include <fstream>
#include <sstream>

#include "cb/build/msc6.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
using nlohmann::ordered_json;
namespace msc6 = cb::build::msc6;
namespace wine16 = cb::symbols::wine16;

std::optional<std::string> env(const char* n) { return fs::get_env(n); }

std::string sha1hex(std::string_view s) { return crypto::to_hex(crypto::sha1(as_bytes(s))); }

ordered_json dig_chars(const std::filesystem::path& out, const std::string& label, const std::string& s) {
    std::string name = label;
    for (char& c : name) {
        if (c == ':') c = '_';
    }
    static const bool write_files = !env("CB_W1B4_NOWRITE");
    if (write_files) fs::write_file(out / (name + ".txt"), to_bytes(s));
    std::size_t chars = 0;
    for (const unsigned char c : s) chars += (c & 0xC0) != 0x80;
    return ordered_json::array({chars, sha1hex(s)});
}

ordered_json hex_code(const std::optional<msc6::Code>& c) {
    if (!c) return nullptr;
    return ordered_json::array({crypto::to_hex(c->code), crypto::to_hex(c->mask)});
}

wine16::ArgKind kind_of(std::string_view s) {
    for (const wine16::ArgKind k : {wine16::ArgKind::Word, wine16::ArgKind::SWord, wine16::ArgKind::Long, wine16::ArgKind::SegPtr,
                                    wine16::ArgKind::SegStr, wine16::ArgKind::Ptr, wine16::ArgKind::Str}) {
        if (wine16::spelling(k) == s) return k;
    }
    throw UsageError("kind " + std::string(s));
}

}  // namespace

CB_TEST("w1b4_harness.msc6", [] {
    const auto from = env("CB_W1B4_MSC6");
    if (!from) return;
    const std::filesystem::path dir = fs::from_text(*from);
    const std::filesystem::path out = fs::from_text(env("CB_W1B4_OUT").value()) / "msc6";
    fs::remove_tree(out);
    fs::make_dirs(out);
    msc6::Tools tools;
    tools.msvc152 = fs::from_text(env("CB_W1B4_MSVC152").value_or(""));
    std::string actual;
    const auto emit = [&](const std::string& label, const ordered_json& v) { actual += label + "\t" + v.dump() + "\n"; };
    auto last = std::chrono::steady_clock::now();
    const auto lap = [&](const char* what) {
        const auto now = std::chrono::steady_clock::now();
        if (env("CB_W1B4_TIME")) std::fprintf(stderr, "  %-28s %7.3f s\n", what, std::chrono::duration<double>(now - last).count());
        last = now;
    };
    const std::string filter = env("CB_W1B4_FILTER").value_or("");
    const auto want = [&](const std::string& label) { return filter.empty() || label.find(filter) != std::string::npos; };
    std::map<std::string, double> spent;   // seconds in the pass itself, by the label's kind (the text before ':')
    const auto text_call = [&](const std::string& label, const auto& make) {
        if (!want(label)) return;
        const auto begin = std::chrono::steady_clock::now();
        const std::string result = make();
        spent[label.substr(0, label.find(':'))] += std::chrono::duration<double>(std::chrono::steady_clock::now() - begin).count();
        emit(label, dig_chars(out, label, result));
    };

    // the ordinal names (the real Python's) and Wine's table (seeded)
    std::map<std::pair<std::string, std::uint32_t>, std::string> ordinal;
    const std::string ordinals_text = fs::read_text(dir / "ordinals.txt");
    for (const std::string_view line : text::lines(ordinals_text)) {
        const auto f = text::split(line, '\t');
        if (f.size() >= 3 && f[2].starts_with('=')) ordinal[{std::string(f[0]), *text::parse_int<std::uint32_t>(f[1])}] = std::string(f[2].substr(1));
    }
    const msc6::OrdinalName names_of = [&](std::string_view m, std::uint32_t n) -> std::optional<std::string> {
        const auto it = ordinal.find({std::string(m), n});
        if (it == ordinal.end()) return std::nullopt;
        return it->second;
    };
    std::map<std::pair<std::string, std::uint32_t>, wine16::Export> exports;
    const std::string exports_text = fs::read_text(dir / "exports.txt");
    for (const std::string_view line : text::lines(exports_text)) {
        const auto f = text::split(line, '\t');
        if (f.size() < 6) continue;
        wine16::Export e;
        e.name = "Fn";
        e.convention = f[2] == "pascal" ? wine16::Convention::Pascal : f[2] == "cdecl" ? wine16::Convention::Cdecl : wine16::Convention::Stdcall;
        if (f[3] != "-") {
            std::vector<wine16::ArgKind> kinds;
            if (f[3] != "+") {
                for (const std::string_view k : text::split(f[3], ',')) kinds.push_back(kind_of(k));
            }
            e.args = kinds;
        }
        e.ret16 = f[4] == "1";
        e.stub = f[5] == "1";
        exports[{std::string(f[0]), *text::parse_int<std::uint32_t>(f[1])}] = e;
    }
    const msc6::ExportLookup look = [&](std::string_view m, std::uint32_t n) -> const wine16::Export* {
        const auto it = exports.find({std::string(m), n});
        return it == exports.end() ? nullptr : &it->second;
    };
    wine16::Export e1;
    e1.name = "IpFindFirstLine";
    e1.convention = wine16::Convention::Pascal;
    e1.args = std::vector<wine16::ArgKind>{wine16::ArgKind::Word, wine16::ArgKind::Str, wine16::ArgKind::Str, wine16::ArgKind::Ptr};
    e1.stub = true;

    lap("setup (ordinals, exports)");
    // the fuzzed objects
    for (int i = 0; i < 400; ++i) {
        if (!want("object_code:")) break;
        const Bytes blob = fs::read_file(dir / "fuzz" / std::format("{:03}.bin", i));
        const std::string label = std::format("object_code:fuzz{:03}", i);
        try {
            emit(label, hex_code(msc6::object_code(blob)));
        } catch (const Error& e) {
            emit(label, ordered_json{{"raises", std::string(e.kind())}});
        }
    }

    // the sample C of the Win95 modules
    std::set<std::string> stems;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::pair<std::string, std::string>> texts;   // label -> (stem, text), sorted
    for (const auto& entry : std::filesystem::directory_iterator(dir / "data")) {
        const std::string name = entry.path().filename().string();
        const std::string stem = name.substr(0, name.find_first_of("_."));
        stems.insert(stem);
        if (name.ends_with(".h")) headers[stem] = fs::read_text(entry.path());
        if (name.ends_with(".c")) texts[name.substr(0, name.size() - 2)] = {stem, fs::read_text(entry.path())};
    }
    lap("fuzzed objects + data load");
    const std::string sdk = msc6::windows_h(tools).value_or("");
    lap("windows_h");
    const msc6::Prototypes real_protos = msc6::sdk_prototypes(sdk);
    const std::string kSdk =
        "LRESULT WINAPI DefWindowProc(HWND, UINT, WPARAM, LPARAM);\nvoid    WINAPI MessageBeep(UINT);\n"
        "LPSTR   WINAPI lstrcpy(LPSTR, LPCSTR);\nint WINAPI  wsprintf(LPSTR, LPCSTR, ...);\nvoid WINAPI Nothing(void);\n"
        "BOOL WINAPI GetClientRect(HWND, RECT FAR*);\nint WINAPI Plain();\n  VOID WINAPI Foo(int a, int b);\n#define WINAPI x WINAPI y(a);\n"
        "DWORD WINAPI Bar(void);\n";
    lap("sdk_prototypes(WINDOWS.H)");
    const msc6::Prototypes test_protos = msc6::sdk_prototypes(kSdk);
    {
        std::string joined;
        std::size_t k = 0;
        for (const auto& [name, params] : real_protos.params) {
            joined += (k++ ? "\n" : "") + name + "|" + (params ? text::join(*params, ",") : std::string("None"));
        }
        std::vector<std::string> first(real_protos.no_result.begin(), real_protos.no_result.end());
        if (first.size() > 20) first.resize(20);
        emit("sdk_prototypes:default", ordered_json::array({real_protos.params.size(), first, real_protos.no_result.size(), sha1hex(joined)}));
    }
    for (const std::string& s : stems) {
        text_call("dialect:" + s, [&] { return msc6::dialect(headers[s]); });
        text_call("cdecl_imports:" + s, [&] { return msc6::cdecl_imports(headers[s], sdk, names_of); });
    }
    lap("dialect + cdecl_imports");
    std::map<std::string, std::set<std::string>> all_names;
    for (const std::string& s : stems) {
        std::vector<std::string> in{headers[s]};
        for (const auto& [label, st] : texts) {
            if (st.first == s) in.push_back(st.second);
        }
        all_names[s] = msc6::pascal_names(in);
    }
    lap("pascal_names over all texts");
    for (const auto& [label, st] : texts) {
        const auto& [s, t] = st;
        const std::set<std::string>& names = all_names[s];
        if (want("names:")) {
            const std::vector<std::string> in{headers[s], t};
            const auto n = msc6::pascal_names(in);
            emit("names:" + label, ordered_json(std::vector<std::string>(n.begin(), n.end())));
        }
        const auto po_begin = std::chrono::steady_clock::now();
        const std::string ordered = (want("po:") || want("sdkcalls") || want("pipeline")) ? msc6::pascal_order(t, names) : std::string();
        spent["pascal_order"] += std::chrono::duration<double>(std::chrono::steady_clock::now() - po_begin).count();
        text_call("po:" + label, [&] { return ordered; });
        text_call("fold:" + label, [&] { return msc6::fold_temporaries(t); });
        text_call("join:" + label, [&] { return msc6::join_far_parameters(t); });
        text_call("frame:" + label, [&] { return msc6::split_frame(t); });
        if (want("ds:")) {
            const auto ds_begin = std::chrono::steady_clock::now();
            const msc6::DsGlobals ds = msc6::ds_globals(t);
            spent["ds_globals"] += std::chrono::duration<double>(std::chrono::steady_clock::now() - ds_begin).count();
            emit("ds:" + label, ordered_json::array({dig_chars(out, "ds_" + label + "_d", ds.declarations), dig_chars(out, "ds_" + label + "_t", ds.text)}));
        }
        text_call("sdkcalls:" + label, [&] { return msc6::sdk_calls(ordered, real_protos, names_of); });
        text_call("sdkcalls2:" + label, [&] { return msc6::sdk_calls(ordered, test_protos, names_of); });
        text_call("wdef:" + label, [&] { return msc6::wine_definition(t, t.find("USER") != std::string::npos ? look("USER", 33) : &e1); });
        text_call("pipeline:" + label, [&] { return msc6::ds_globals(msc6::split_frame(msc6::fold_temporaries(msc6::join_far_parameters(ordered)))).text; });
    }
    for (const std::string& s : stems) {
        if (!want("wine_typing")) break;
        OrderedMap<std::string, std::string> po;
        for (const auto& [label, st] : texts) {
            if (st.first == s) po.insert_or_assign(label, msc6::pascal_order(st.second, all_names[s]));
        }
        const auto wt_begin = std::chrono::steady_clock::now();
        const msc6::WineTyped typed = msc6::wine_typing(headers[s], po, look);
        spent["wine_typing"] += std::chrono::duration<double>(std::chrono::steady_clock::now() - wt_begin).count();
        ordered_json list = ordered_json::array();
        list.push_back(dig_chars(out, "wt_" + s + "_h", typed.header));
        for (const auto& [k, v] : typed.texts) list.push_back(dig_chars(out, "wt_" + s + "_" + k, v));
        emit("wine_typing:" + s, list);
        emit("wine_typing_exact:" + s, ordered_json::array({dig_chars(out, "wt2_" + s + "_h", typed.header)}));
    }
    lap("wine_typing per stem");
    // the mutated texts
    const std::string manifest_text = fs::read_text(dir / "manifest.txt");
    for (const std::string_view line : text::lines(manifest_text)) {
        const auto f = text::split(line, '\t');
        if (f[0] != "mut") continue;
        const std::string s(f[1]);
        const std::string name(f[2].substr(0, f[2].size() - 2));
        const std::string t = fs::read_text(dir / "mut" / std::string(f[2]));
        std::set<std::string> names = all_names[s];
        if (want("mpo:")) {
            const std::vector<std::string> own{t};
            const auto extra = msc6::pascal_names(own);
            names.insert(extra.begin(), extra.end());
        }
        text_call("mpo:" + name, [&] { return msc6::pascal_order(t, names); });
        text_call("mfold:" + name, [&] { return msc6::fold_temporaries(t); });
        text_call("mjoin:" + name, [&] { return msc6::join_far_parameters(t); });
        text_call("mframe:" + name, [&] { return msc6::split_frame(t); });
        if (want("mds:")) emit("mds:" + name, dig_chars(out, "mds_" + name, msc6::ds_globals(t).text));
        text_call("msdk:" + name, [&] { return msc6::sdk_calls(t, real_protos, names_of); });
        text_call("mwdef:" + name, [&] { return msc6::wine_definition(t, &e1); });
        if (want("mwt:")) {
            OrderedMap<std::string, std::string> one;
            one.insert_or_assign("t", t);
            const msc6::WineTyped typed = msc6::wine_typing(headers[s], one, look);
            emit("mwt:" + name, ordered_json::array({dig_chars(out, "mwt_" + name + "_h", typed.header), dig_chars(out, "mwt_" + name + "_t", typed.texts.at("t"))}));
        }
    }
    lap("mutated texts");
    if (env("CB_W1B4_TIME")) {
        for (const auto& [k, v] : spent) std::fprintf(stderr, "  pass %-12s %8.4f s\n", k.c_str(), v);
    }
    fs::write_file(out / "actual.txt", to_bytes(actual));
});
