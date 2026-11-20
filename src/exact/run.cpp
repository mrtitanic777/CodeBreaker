#include "exact/run.hpp"

#include <algorithm>
#include <format>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "exact/bench.hpp"
#include "exact/bss.hpp"
#include "exact/carve.hpp"
#include "exact/data.hpp"
#include "exact/identify.hpp"
#include "exact/ledger.hpp"
#include "exact/library.hpp"
#include "exact/link.hpp"
#include "exact/modules.hpp"
#include "exact/names.hpp"
#include "exact/repair.hpp"
#include "formats/omf.hpp"

namespace cb::exact::run {

void Stages::add(std::string name, Stage stage) {
    const auto same = std::find_if(stages_.begin(), stages_.end(), [&](const auto& s) { return s.first == name; });
    if (same != stages_.end()) same->second = std::move(stage);
    else stages_.emplace_back(std::move(name), std::move(stage));
}

const Stage* Stages::find(std::string_view name) const {
    const auto it = std::find_if(stages_.begin(), stages_.end(), [&](const auto& s) { return s.first == name; });
    return it == stages_.end() ? nullptr : &it->second;
}

std::vector<std::string> Stages::names() const {
    std::vector<std::string> out;
    for (const auto& [name, stage] : stages_) out.push_back(name);
    return out;
}

Stages standard_stages(const linkwhole::DosHost* dos) {
    Stages s;
    const auto decoder = [](project::Project& p) -> const x86::Decoder16& { return p.services().decoder ? *p.services().decoder : x86::capstone_decoder(); };
    s.add("ledger", [](project::Project& p, const std::vector<std::string>& a) { return ledger::command(*p.as_ledger(), a); });
    s.add("library", [decoder](project::Project& p, const std::vector<std::string>& a) { return library::run(*p.as_library(), a, decoder(p)); });
    s.add("carve", [](project::Project& p, const std::vector<std::string>& a) { return carve::run(p, a); });
    s.add("modules", [decoder](project::Project& p, const std::vector<std::string>& a) { return modules::run(*p.as_modules(), a, decoder(p)); });
    if (dos != nullptr) s.add("link", [dos](project::Project& p, const std::vector<std::string>& a) { return link::run(p, a, *dos); });
    s.add("bss", [](project::Project& p, const std::vector<std::string>& a) { return bss::run(p, a); });
    s.add("data", [](project::Project& p, const std::vector<std::string>& a) { return data::run(*p.as_data(), a); });
    s.add("repair", [](project::Project& p, const std::vector<std::string>& a) { return repair::command(*p.as_ledger(), a); });
    s.add("names", [](project::Project& p, const std::vector<std::string>& a) { return names::run(p, a); });
    s.add("identify", [](project::Project& p, const std::vector<std::string>& a) { return identify::command(*p.as_ledger(), a, p.identify_services()); });
    s.add("bench", [](project::Project& p, const std::vector<std::string>& a) { return bench::main(p, a); });
    return s;
}

std::string usage() {
    return "exact: a compiled program rebuilt byte for byte from its source (docs/EXACT.md).\n\n"
           "    codebreaker exact PROJECT STAGE [options]\n"
           "    codebreaker exact --toolchains             the toolchain profiles there are\n"
           "    codebreaker exact --compile PROFILE[:VARIANT] FILE.c [switches]\n"
           "                                               one file compiled, its functions listed\n\n"
           "PROJECT is a folder with a project.json. Stages:\n\n"
           "    ledger      every one-function source compiled and put against the program\n"
           "    carve       where the program's own functions begin and end\n"
           "    modules     each source file put together, compiled whole, its functions and data checked\n"
           "    draft       functions written by the compiler's inverse model, kept when the compiler proves them\n"
           "    data        a module's initialized data: where it stands, definitions drafted\n"
           "    bss         the variables without a value at their places: src/BSS\n"
           "    link        the program linked from its sources by the period linker, compared, diagnosed\n"
           "    library     the library modules found in the program, in the linker's order; stand-ins\n";
}

namespace {

// --compile PROFILE[:VARIANT] FILE.c [switches]: one file compiled, its functions listed.
int run_compile(const std::vector<std::string>& args, toolchain::Toolchains& toolchains) {
    if (args.size() < 3) throw UsageError("--compile PROFILE[:VARIANT] FILE.c [switches]");
    const std::string& spec = args[1];
    const std::size_t colon = spec.find(':');
    const std::shared_ptr<toolchain::Toolchain> tc = toolchains.load(spec.substr(0, colon), colon == std::string::npos || colon + 1 == spec.size() ? std::nullopt : std::optional<std::string>(spec.substr(colon + 1)));
    const std::filesystem::path file = std::filesystem::absolute(fs::from_text(args[2]));
    modules::CompileOptions options;   // (the headers beside the file)
    for (const auto& entry : std::filesystem::directory_iterator(file.parent_path()))
        if (entry.is_regular_file() && text::iends_with(entry.path().filename().string(), ".h")) options.headers[entry.path().filename().string()] = fs::read_file(entry.path());
    std::vector<std::string> switches(args.begin() + 3, args.end());
    if (!switches.empty()) options.switches = text::join(switches, " ");
    const modules::CompileResult c = tc->compile(fs::read_file(file), options);
    if (!c.object) {
        out("{}\n", c.console);
        return 1;
    }
    const formats::omf::Object o = formats::omf::read(*c.object);
    for (const std::string& segment : formats::omf::code_segments(o)) {
        const Bytes* code = o.segments.find(segment);
        for (const formats::omf::FunctionSpan& f : formats::omf::functions(o, segment)) {
            const std::size_t at = static_cast<std::size_t>(f.offset), len = static_cast<std::size_t>(f.length);
            const ByteView bytes = code != nullptr && at <= code->size() ? ByteView(*code).subspan(at, std::min(len, code->size() - at)) : ByteView();
            out("{:<10} {:<24} {:>5x} {:>5}  {}\n", segment, f.name, f.offset, f.length, crypto::to_hex(bytes));
        }
    }
    return 0;
}

}  // namespace

int main(const std::vector<std::string>& args, const Stages& stages, toolchain::Toolchains& toolchains, const std::shared_ptr<const project::Services>& services) {
    if (args.empty() || args[0] == "-h" || args[0] == "--help") {
        out("{}\n", usage());
        return args.empty() ? 2 : 0;
    }
    if (args[0] == "--toolchains") {
        for (const std::string& name : toolchains.profiles()) {
            const toolchain::Profile p = toolchain::read_profile(toolchains.locations(), name);   // (the profile, not the tool: a plugin's need not be there to be listed)
            std::vector<std::string> variants;
            for (const auto& [v, body] : p.variants) variants.push_back(v);
            std::sort(variants.begin(), variants.end());
            const auto title = p.strings.find("title");
            out("{:<12} {}  variants: {}\n", name, title == p.strings.end() ? "" : title->second, variants.empty() ? "-" : text::join(variants, ", "));
        }
        return 0;
    }
    if (args[0] == "--compile") return run_compile(args, toolchains);
    const std::string stage = args.size() > 1 ? args[1] : "ledger";
    const Stage* run = stages.find(stage);
    if (run == nullptr) {
        out("no stage {} (stages: {})\n", stage, text::join(stages.names(), ", "));
        return 2;
    }
    const std::shared_ptr<project::Project> project = project::Project::open(std::filesystem::absolute(fs::from_text(args[0])), services);
    return (*run)(*project, std::vector<std::string>(args.begin() + std::min<std::size_t>(2, args.size()), args.end()));
}

}  // namespace cb::exact::run
