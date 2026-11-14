#include "cb/translate/dotnet/pairs.hpp"

#include "cb/core/fs.hpp"
#include "cb/translate/backends/csharp.hpp"
#include "cb/translate/backends/csharp_java.hpp"

namespace cb::translate::dotnet::pairs {

namespace stdfs = std::filesystem;
namespace cs = cb::translate::backends::csharp;

namespace {

std::vector<std::string> texts(const std::vector<Refusal>& refused) {
    std::vector<std::string> out;
    for (const Refusal& r : refused) out.push_back(r.text());
    return out;
}

Verdict judged(const std::vector<Refusal>& refused, judge::Run original, judge::Run translation, std::vector<stdfs::path> files) {
    const std::vector<std::string> shown = texts(refused);
    const judge::Judgement j = judge::compare(original, translation, shown);
    return {j.verdict, j.detail, refused, std::move(files), std::move(original), std::move(translation)};
}

void add_ledger(std::vector<Refusal>& refused, const std::vector<cs::Gap>& ledger) {
    for (const cs::Gap& g : ledger) refused.push_back({g.where, g.kind, g.why});
}

void add_unsupported(std::vector<Refusal>& refused, const std::vector<ir::Node*>& units) {
    for (ir::Node* unit : units) {
        for (ir::Node* n : ir::unsupported(unit)) {
            const auto* u = ir::get<ir::Unsupported>(n);
            const std::string kind = u->kind.value_or("None");
            refused.push_back({ir::get<ir::Unit>(unit)->path.value_or(""), kind, u->raw.value_or(kind)});
        }
    }
}

}  // namespace

frontends::csharp::Program Pairs::units(const std::vector<stdfs::path>& files, const stdfs::path& work) const {
    judge::fresh_folder(work);
    return csharp.lower(arena, files, work);
}

Verdict Pairs::csharp_to_csharp(const std::vector<stdfs::path>& files, const stdfs::path& work, const std::optional<std::string>& main) const {
    frontends::csharp::Program program = units(files, work);
    std::vector<Refusal> refused;
    for (const auto& r : program.refused) refused.push_back({r.where, r.kind, r.why});
    cs::Program out(program.units);
    const std::vector<stdfs::path> paths = out.write(work / "cs");
    add_ledger(refused, out.ledger());
    const judge::CSharpToolchain tools = judge::CSharpToolchain::of(sdk);
    judge::Run original = judge::csharp(tools, files, work / "run-orig", {.main = main});
    judge::Run translation = judge::csharp(tools, paths, work / "run-trans", {.main = main});
    return judged(refused, std::move(original), std::move(translation), paths);
}

Verdict Pairs::java_to_csharp(const stdfs::path& src_dir, const std::string& main, const stdfs::path& work, const JavaSide& java) const {
    judge::fresh_folder(work);
    std::vector<ir::Node*> units = cb::java::frontend::lower_program(arena, src_dir, java.tools);
    for (ir::Node* u : units) ir::get<ir::Unit>(u)->lang = "java";
    cs::Options options;
    if (java.registry != nullptr) options.libraries["java"] = backends::csharp_java::factory(*java.registry);
    options.runtime_dir = java.runtime_dir;
    cs::Program out(units, options);
    const std::vector<stdfs::path> paths = out.write(work / "cs");
    std::vector<Refusal> refused;
    add_ledger(refused, out.ledger());
    add_unsupported(refused, units);
    const judge::CSharpToolchain tools = judge::CSharpToolchain::of(sdk);
    judge::Run original = judge::java(java.jdk, src_dir, main, work / "run-orig");
    judge::Run translation = judge::csharp(tools, paths, work / "run-trans", {.main = main});
    return judged(refused, std::move(original), std::move(translation), paths);
}

Verdict Pairs::csharp_to_cpp(const std::vector<stdfs::path>& files, const stdfs::path& work, const CppSide& cpp, const std::string& main_class) const {
    frontends::csharp::Program program = units(files, work);
    std::vector<Refusal> refused;
    for (const auto& r : program.refused) refused.push_back({r.where, r.kind, r.why});
    const cs2cpp::Conversion converted = cs2cpp::convert(arena, program.units, work, cpp.converter);
    for (const cs2cpp::Refusal& r : converted.refused) refused.push_back({r.where.value_or("None"), r.kind, r.why});
    const judge::CSharpToolchain tools = judge::CSharpToolchain::of(sdk);
    judge::Run original = judge::csharp(tools, files, work / "run-orig");
    if (!refused.empty()) {
        Verdict v = judged(refused, std::move(original), {}, {converted.cpp_dir});
        v.translation.reset();
        return v;
    }
    judge::Run translation = judge::cpp(cpp.host, converted.cpp_dir, main_class, work / "run-trans");
    return judged(refused, std::move(original), std::move(translation), {converted.cpp_dir});
}

}  // namespace cb::translate::dotnet::pairs
