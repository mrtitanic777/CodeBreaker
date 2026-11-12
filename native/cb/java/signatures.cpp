// cb-unity: alone (helpers named for what they do here)
#include "cb/java/signatures.hpp"

#include <algorithm>
#include <format>
#include <map>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/java/parse.hpp"

namespace cb::java::signatures {

namespace {

namespace stdfs = std::filesystem;

std::string kind_word(const ir::JClass& cls) {
    if (cls.is_interface) return "interface";
    if (cls.is_enum) return "enum";
    return cls.is_abstract ? "abstract class" : "class";
}

// ` : extends Base implements A, B`, or nothing.
std::string inheritance_suffix(const ir::JClass& cls) {
    std::vector<std::string> bits;
    if (cls.parent && !cls.parent->empty()) bits.push_back("extends " + *cls.parent);
    if (!cls.interfaces.empty()) bits.push_back("implements " + text::join(cls.interfaces, ", "));
    return bits.empty() ? "" : " : " + text::join(bits, " ");
}

// The resolver's answer, or a note in the map saying it could not give one: one odd type must not hide the rest.
// (An isolation boundary: the map is a report about the whole corpus.)
template <class Resolve>
std::string resolved_or_note(Resolve resolve) {
    try {
        return resolve();
    } catch (const Error& e) {
        return std::format("<resolve-error: {}>", e.what());
    }
}

std::string field_line(const ir::Field& field, const types::TypeResolver& resolver) {
    const std::string cpp = resolved_or_note([&] { return resolver.field_type(field.type); });
    return std::format("  - {}{}{} {}   // C++: {}", field.is_static ? "static " : "", field.is_final ? "final " : "",
                       field.type.to_string(), field.name, cpp);
}

std::string method_line(const ir::Method& method, const types::TypeResolver& resolver) {
    std::vector<std::string> flags;
    if (method.is_static) flags.push_back("static");
    if (method.is_abstract) flags.push_back("abstract");
    const std::string prefix = flags.empty() ? "" : "[" + text::join(flags, " ") + "] ";

    std::string java_return = "void";
    std::string cpp_return = "void";
    if (method.is_ctor) {
        java_return = cpp_return = "(ctor)";
    } else if (method.return_type) {
        java_return = method.return_type->to_string();
        cpp_return = resolved_or_note([&] { return resolver.return_type(*method.return_type); });
    }
    std::vector<std::string> java_params, cpp_params;
    for (const ir::Param& p : method.params) {
        java_params.push_back(std::format("{} {}", p.type.to_string(), p.name));
        cpp_params.push_back(std::format("{} {}", resolved_or_note([&] { return resolver.param_type(p.type); }), p.name));
    }
    return std::format("  - {}{} {}({})   // C++: {} {}({})", prefix, java_return, method.name, text::join(java_params, ", "),
                       cpp_return, method.name, text::join(cpp_params, ", "));
}

// The classes of each package, by package name with the default package last, each package's classes by name.
struct PackageList {
    std::string name;
    std::vector<const ir::JClass*> classes;
};

std::vector<PackageList> group_by_package(const std::vector<ir::JClass>& classes) {
    std::map<std::string, std::vector<const ir::JClass*>> by_name;   // sorted: the order is the output's
    for (const ir::JClass& c : classes) {
        const std::string pkg = c.package && !c.package->empty() ? *c.package : std::string(kDefaultPackage);
        by_name[pkg].push_back(&c);
    }
    std::vector<PackageList> out;
    for (auto& [name, list] : by_name) {
        std::stable_sort(list.begin(), list.end(), [](const ir::JClass* a, const ir::JClass* b) { return a->name < b->name; });
        out.push_back({name, std::move(list)});
    }
    std::stable_sort(out.begin(), out.end(), [](const PackageList& a, const PackageList& b) {
        return (a.name == kDefaultPackage) < (b.name == kDefaultPackage);   // the default package goes last
    });
    return out;
}

std::vector<stdfs::path> java_files_by_name(const stdfs::path& java_dir) {
    if (!stdfs::is_directory(java_dir)) throw NotFoundError(java_dir, 2, "not a folder");
    std::vector<stdfs::path> all;
    for (const stdfs::directory_entry& e : stdfs::recursive_directory_iterator(java_dir)) {
        if (e.is_regular_file() && e.path().extension() == ".java") all.push_back(e.path());
    }
    std::sort(all.begin(), all.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) < fs::to_text(b); });
    std::map<std::string, stdfs::path> by_file_name;   // the first path met for a name wins
    for (const stdfs::path& p : all) by_file_name.emplace(fs::to_text(p.filename()), p);
    std::vector<stdfs::path> out;
    for (const auto& [name, path] : by_file_name) out.push_back(path);
    return out;
}

}  // namespace

Codebase load(const stdfs::path& java_dir) {
    Codebase code;
    for (const stdfs::path& path : java_files_by_name(java_dir)) {
        try {   // isolation boundary: a file that will not parse is listed at the bottom of the map
            code.classes.push_back(parse::parse_file(path));
        } catch (const Error& e) {
            code.failures.push_back({fs::to_text(path.filename()), e.what()});
        }
    }
    return code;
}

std::string package_role(std::string_view package, const PackageRoles& roles, std::size_t class_count) {
    if (const auto it = roles.find(package); it != roles.end()) return it->second;
    if (package == kDefaultPackage) return "Classes declared without a package.";
    const std::size_t dot = package.rfind('.');
    const std::string_view segment = dot == std::string_view::npos ? package : package.substr(dot + 1);
    return std::format("Subsystem: {} ({} classes).", segment, class_count);
}

std::string render(const Codebase& code, const types::TypeResolver& resolver, const PackageRoles& roles) {
    const std::vector<PackageList> packages = group_by_package(code.classes);
    std::vector<std::string> out;
    auto w = [&](std::string line) { out.push_back(std::move(line)); };

    w("# Game codebase signatures");
    w("");
    w("Generated by `codebreaker java signatures` from the Java source \xE2\x80\x94 regenerate with `codebreaker java signatures <java_dir>`.");
    w("");
    w(std::format("Classes mapped: {} (across {} subsystems/packages).", code.classes.size(), packages.size()));
    if (!code.failures.empty()) w(std::format("Parse failures: {} (listed at the bottom).", code.failures.size()));
    w("");
    w("Each field/method shows the Java spelling and, after `// C++:`, the type the converter resolves it to (value vs `T*` "
      "pointer \xE2\x80\x94 i.e. `.` vs `->`).");
    w("");

    w("## Subsystem overview");
    w("");
    for (const PackageList& pkg : packages) {
        std::vector<std::string> names;
        for (const ir::JClass* c : pkg.classes) names.push_back(c->name);
        w("### " + pkg.name);
        w(package_role(pkg.name, roles, names.size()));
        w("");
        w(std::format("Classes ({}): {}", names.size(), text::join(names, ", ")));
        w("");
    }

    w("## Classes");
    w("");
    for (const PackageList& pkg : packages) {
        for (const ir::JClass* cls : pkg.classes) {
            w("### " + cls->name);
            w(kind_word(*cls) + inheritance_suffix(*cls));
            w("package: " + pkg.name);
            w("");

            if (cls->is_enum && !cls->enum_consts.empty()) {
                std::vector<std::string> consts;
                for (const ir::EnumConst& ec : cls->enum_consts) {
                    consts.push_back(ec.args.empty() ? ec.name : std::format("{}({})", ec.name, ec.args.size()));
                }
                w(std::format("Enum constants ({}): {}", cls->enum_consts.size(), text::join(consts, ", ")));
                w("");
            }

            w(std::format("Fields ({}):", cls->fields.size()));
            if (cls->fields.empty()) w("  - (none)");
            for (const ir::Field& f : cls->fields) w(field_line(f, resolver));
            w("");

            w(std::format("Methods ({}):", cls->methods.size()));
            if (cls->methods.empty()) w("  - (none)");
            for (const ir::Method& m : cls->methods) w(method_line(m, resolver));
            w("");
        }
    }

    if (!code.failures.empty()) {
        w("## Parse failures");
        w("");
        for (const ParseFailure& f : code.failures) w(std::format("  - {}: {}", f.file, f.message));
        w("");
    }
    return text::join(out, "\n") + "\n";
}

std::string summary(const Codebase& code, const stdfs::path& out_path) {
    const std::vector<PackageList> packages = group_by_package(code.classes);
    std::string out = std::format("Wrote {}\nClasses mapped: {} across {} packages.\n", fs::to_text(out_path), code.classes.size(),
                                  packages.size());
    for (const PackageList& pkg : packages) out += std::format("  - {} ({} classes)\n", pkg.name, pkg.classes.size());
    if (code.failures.empty()) {
        out += "Parse failures: 0\n";
    } else {
        out += std::format("Parse failures: {}\n", code.failures.size());
        for (const ParseFailure& f : code.failures) out += std::format("  - {}: {}\n", f.file, f.message);
    }
    return out;
}

}  // namespace cb::java::signatures
