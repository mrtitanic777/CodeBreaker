// LOCAL harness for the W1-B4 refactor (never committed): compile_many against the REAL compilers (Microsoft C 6.0a under DOSBox-X,
// Visual C++ 1.52c on Windows) with the cases of the old harness's section 9, for comparison with the Python's expected.txt. Does
// nothing unless CB_W1B4_COMPILE is set.
#include <nlohmann/json.hpp>

#include "cb/base/cache.hpp"
#include "cb/build/msc6.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
using nlohmann::ordered_json;
namespace msc6 = cb::build::msc6;

// build/dos_tools.py's run(), for this harness only
class DosboxRunner : public build::DosRunner {
public:
    explicit DosboxRunner(std::filesystem::path dosbox) : dosbox_(std::move(dosbox)) {}
    build::DosRunResult run(const build::DosRunSpec& spec) const override {
        const std::filesystem::path work = *spec.work;
        fs::make_dirs(work);
        std::string bat = "@echo off\r\n";
        for (const std::string& c : spec.commands) bat += c + "\r\n";
        bat += "echo _DONE_ > done.txt\r\n";
        fs::write_file(work / "BLD.BAT", to_bytes(bat));
        std::string conf = "[cpu]\n";
        for (const auto& [k, v] : spec.cpu) conf += k + "=" + v + "\n";
        if (!spec.dos.empty()) {
            conf += "[dos]\n";
            for (const auto& [k, v] : spec.dos) conf += k + "=" + v + "\n";
        }
        conf += "[autoexec]\n";
        for (const auto& m : spec.mounts) conf += "mount " + m.drive + " \"" + text::replace_all(m.folder.string(), "/", "\\") + "\"\n";
        for (const auto& [k, v] : spec.variables) conf += "set " + k + "=" + v + "\n";
        conf += spec.mounts.back().drive + ":\ncall BLD.BAT\nexit\n";
        fs::write_file(work / "r.conf", to_bytes(conf));
        for (const std::string& name : spec.artifacts) std::filesystem::remove(work / name);
        std::filesystem::remove(work / "done.txt");
        process::ProcessSpec p;
        p.argv = {dosbox_.string(), "-conf", (work / "r.conf").string(), "-exit"};
        p.env = {{"SDL_VIDEODRIVER", "dummy"}};
        p.out = process::Capture::Discard;
        p.err = process::Capture::Discard;
        p.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(spec.timeout);
        (void)process::run(p);
        build::DosRunResult out;
        out.work = work;
        out.ok = true;
        return out;
    }

private:
    std::filesystem::path dosbox_;
};

ordered_json code_json(const std::optional<msc6::Code>& c) {
    if (!c) return nullptr;
    return ordered_json::array({crypto::to_hex(c->code), crypto::to_hex(c->mask)});
}

ordered_json codes(const std::vector<std::optional<msc6::Code>>& v) {
    ordered_json a = ordered_json::array();
    for (const auto& c : v) a.push_back(code_json(c));
    return a;
}

}  // namespace

CB_TEST("w1b4_harness.compile", [] {
    const auto root = fs::get_env("CB_W1B4_COMPILE");
    if (!root) return;
    const std::filesystem::path out = fs::from_text(fs::get_env("CB_W1B4_OUT").value()) / "compile";
    fs::remove_tree(out);
    fs::make_dirs(out);
    const std::filesystem::path msc_dir = fs::from_text(fs::get_env("CB_W1B4_MSC6").value());   // the old harness's folder
    msc6::Tools tools;
    tools.dosbox_x = fs::from_text(fs::get_env("CB_W1B4_DOSBOX").value());
    tools.msc6 = fs::from_text(fs::get_env("CB_W1B4_MSC6_HOME").value());
    tools.msvc152 = fs::from_text(fs::get_env("CB_W1B4_MSVC152").value());
    const DosboxRunner runner(tools.dosbox_x);
    base::cache::Config config;
    config.state_dir = out / "cache-state";
    config.shared_dir = out / "cache-shared";
    base::cache::Cache cache(config);
    const msc6::Context ctx{tools, runner, &cache};
    std::string actual;
    const auto emit = [&](const std::string& label, const ordered_json& v) { actual += label + "\t" + v.dump() + "\n"; };

    const std::string hdr_raw = "typedef short int2;\ntypedef unsigned short uint2;\ntypedef long int4;\nextern int2 __far __stdcall ext(int2);\nint2 g(int2);\n";
    const std::string hdr = msc6::dialect(hdr_raw);
    const std::string s_f1 = "int2 f1(int2 a)\n{\n  return a + 1;\n}";
    const std::string s_f2 = "int2 f2(int2 a, int2 b)\n{\n  return a * b + g(a);\n}";
    const std::string s_f3 = "int2 f3(int2 a)\n{\n  int2 i;\n  int2 s = 0;\n  for (i = 0; i < a; i++) {\n    s += i;\n  }\n  return s;\n}";
    const std::string s_bad = "int2 bad(int2 a)\n{\n  return undefined_thing(a);\n}";
    const std::string s_bad2 = "int2 bad2(int2 a)\n{\n  return a +;\n}";
    const std::string s_g = "int2 g(int2 a)\n{\n  return f1(a) + f2(a, a);\n}";
    const std::string flags = "/AM /Gcsw /Os";
    const auto options = [&](msc6::Compiler c) {
        msc6::CompileOptions o;
        o.compiler = c;
        o.work = out / (c == msc6::Compiler::Msc6 ? "work_msc6" : "work_msvc152");
        return o;
    };
    using S = msc6::Source;
    const auto run = [&](const std::string& label, std::vector<S> sources, const std::string& h, const std::string& f,
                         msc6::CompileOptions o) {
        if (const auto only = fs::get_env("CB_W1B4_ONLY"); only && label.find(*only) == std::string::npos) return;
        try {
            emit(label, codes(msc6::compile_many(ctx, sources, h, f, o)));
        } catch (const Error& e) {
            emit(label, ordered_json{{"raises", std::string(e.kind())}});
        }
    };
    const auto named = [](std::string text, std::string fn) { return S{std::move(text), std::move(fn)}; };

    run("compile:single", {{s_f1}, {s_f2}, {s_bad}, {s_f3}}, hdr, flags, options(msc6::Compiler::Msc6));
    run("compile:single-again", {{s_f1}, {s_f2}, {s_bad}, {s_f3}}, hdr, flags, options(msc6::Compiler::Msc6));
    run("compile:keys", {{s_f1}, {s_f2}, {s_f3}}, hdr, "/AL /Gcsw /Ox", options(msc6::Compiler::Msc6));
    const std::vector<S> src{named(s_f1, "f1"), named(s_f2, "f2"), named(s_f3, "f3"), named(s_bad, "bad"), named(s_bad2, "bad2"), named(s_g, "g")};
    run("compile:named", src, hdr, flags, options(msc6::Compiler::Msc6));
    run("compile:named-again", src, hdr, flags, options(msc6::Compiler::Msc6));
    {
        auto o = options(msc6::Compiler::Msc6);
        o.batch = 2;
        run("compile:named-batch2", src, hdr, "/AM /Gcsw /Od", o);
        o.batch = 1;
        run("compile:named-batch1", src, hdr, "/AS /Gcsw /Os", o);
        o.batch = msc6::kBatch;
        o.together = {{0, 1}, {2, 3}, {4, 5}};
        run("compile:together", src, hdr, "/AM /Gc2sw /Os", o);
    }
    const std::string s_x = "int2 x(void)\n{\n  return 3;\n}";
    run("compile:partial", {named(s_f1, "f1"), named(s_f2, "f2"), named(s_x, "x")}, hdr, flags, options(msc6::Compiler::Msc6));
    run("compile:partial2", {named(s_f1, "f1"), named(s_f2, "f2"), named(s_x, "x")}, hdr, flags, options(msc6::Compiler::Msc6));
    run("compile:msvc152", {{s_f1}, {s_f2}, {s_bad}}, hdr, "/AM /Gsw /Os", options(msc6::Compiler::Msvc152));
    run("compile:msvc152-named", src, hdr, "/AM /Gsw /Os", options(msc6::Compiler::Msvc152));
    run("compile:empty", {}, hdr, flags, options(msc6::Compiler::Msc6));
    fs::write_file(out / "actual.txt", to_bytes(actual));
});
