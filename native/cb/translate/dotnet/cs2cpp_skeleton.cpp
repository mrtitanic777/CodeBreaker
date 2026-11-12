// cb/translate/dotnet/cs2cpp_skeleton.cpp -- C# into the C++03 back end: the Java declaration skeleton the converter parses, the conversion
// as a whole, and the host PrintStream for the judge (see cs2cpp.hpp).

#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/dotnet/cs2cpp_internal.hpp"

namespace cb::translate::dotnet::cs2cpp {

using namespace impl;

namespace {
namespace skeleton_impl {

std::string last_part(std::string_view name) {
    const std::size_t dot = name.rfind('.');
    return std::string(dot == std::string_view::npos ? name : name.substr(dot + 1));
}

bool has_mod(const std::vector<std::string>& mods, std::string_view m) { return std::find(mods.begin(), mods.end(), m) != mods.end(); }

}  // namespace skeleton_impl
}  // namespace

// ---------------------------------------------------------------------------------------------- Java text
std::string java_type(const ir::TypeP& t) {
    if (!t) return "void";
    if (const auto it = primitive_java().find(t->kind()); it != primitive_java().end()) return it->second;
    if (const auto* arr = std::get_if<ir::ArrayType>(&t->body)) return java_type(arr->elem) + "[]";
    if (const auto* cls = std::get_if<ir::ClassType>(&t->body)) {
        std::string n = skeleton_impl::last_part(cls->name);
        if (!cls->args.empty()) {
            std::vector<std::string> as;
            for (const ir::TypeP& a : cls->args) as.push_back(java_type(a));
            n += "<" + text::join(as, ", ") + ">";
        }
        return n;
    }
    if (const auto* tv = std::get_if<ir::TypeVarType>(&t->body)) return tv->name;
    return "Object";
}

std::string default_value(const ir::TypeP& t) {
    if (!t || t->kind() == "void") return "";
    if (t->kind() == "bool") return " false";
    if (primitive_java().contains(t->kind())) return " 0";
    return " null";
}

std::string skeleton(const ir::Node& cn) {
    const auto* c = ir::get<ir::ClassDecl>(&cn);
    if (c == nullptr || !c->qname || !c->name) throw Refuse("class", "a class with no name");
    std::string pkg;
    if (c->qname->ends_with(*c->name)) {
        pkg = c->qname->substr(0, c->qname->size() - c->name->size());
        while (pkg.ends_with('.')) pkg.pop_back();
    }
    std::vector<std::string> lines;
    if (!pkg.empty()) lines.push_back("package " + pkg + ";");
    lines.push_back("import java.util.ArrayList;");
    lines.push_back("");
    const bool is_interface = c->kind && *c->kind == "interface";
    std::vector<std::string> head_parts = c->mods;
    head_parts.push_back(is_interface ? "interface" : "class");
    head_parts.push_back(*c->name);
    std::string head = text::join(head_parts, " ");
    if (c->extends) head += " extends " + java_type(c->extends);
    if (!c->implements.empty()) {
        std::vector<std::string> names;
        for (const ir::TypeP& i : c->implements) names.push_back(java_type(i));
        head += (is_interface ? " extends " : " implements ") + text::join(names, ", ");
    }
    lines.push_back(head + " {");
    for (const ir::Node* m : c->members) {
        if (const auto* f = ir::get<ir::FieldDecl>(m)) {
            if (!f->sym) throw Refuse("field", "a field with no symbol");
            lines.push_back(std::format("\t{} {} {};", text::join(f->mods, " "), java_type(f->type), f->sym->name));
        } else if (const auto* md = ir::get<ir::MethodDecl>(m)) {
            std::vector<std::string> ps;
            for (const ir::Node* pn : md->params) {
                const auto* p = ir::get<ir::Param>(pn);
                if (p == nullptr || !p->sym) throw Refuse("method", "a parameter with no symbol");
                ps.push_back(std::format("{} {}", java_type(p->type), p->sym->name));
            }
            const std::string params = text::join(ps, ", ");
            std::vector<std::string> mods;
            for (const std::string& x : md->mods) {
                if (x != "IMPLICIT") mods.push_back(x);
            }
            const std::string ms = text::join(mods, " ");
            if (md->kind && *md->kind == "ctor") {
                if (skeleton_impl::has_mod(md->mods, "IMPLICIT")) continue;
                lines.push_back(std::format("\t{} {}({}) {{\n\t}}", ms, *c->name, params));
            } else if (md->kind && *md->kind == "static_init") {
                lines.push_back("\tstatic {\n\t}");
            } else if (body_absent(md->body)) {
                lines.push_back(std::format("\t{} {} {}({});", ms, java_type(md->ret), or_none(md->name), params));
            } else {
                lines.push_back(std::format("\t{} {} {}({}) {{\n\t\treturn{};\n\t}}", ms, java_type(md->ret), or_none(md->name), params,
                                            default_value(md->ret)));
            }
        }
    }
    lines.push_back("}");
    std::vector<std::string> fixed;
    for (const std::string& l : lines) fixed.push_back(text::replace_all(l, "\t ", "\t"));   // (an empty modifier list leaves a space)
    return text::join(fixed, "\n") + "\n";
}

// ---------------------------------------------------------------------------------------------- the conversion
Conversion convert(Arena& arena, std::span<ir::Node* const> units, const std::filesystem::path& work, const Converter& converter) {
    ToJava to_java(arena, units);
    std::vector<Refusal> refused = to_java.run();
    const std::filesystem::path src = work / "java-skeleton";
    const std::filesystem::path out = work / "cpp";
    for (const std::filesystem::path& d : {src, out}) {
        fs::remove_tree(d);
        std::filesystem::create_directories(d);
    }
    for (const ir::Node* un : units) {
        for (const ir::Node* c : ir::get<ir::Unit>(un)->types) {
            const auto& decl = *ir::get<ir::ClassDecl>(c);
            fs::write_text(src / (decl.name.value_or("None") + ".java"), skeleton(*c));
        }
    }
    Converted got = converter(src, out, units);   // (the C# kit's L2, not javac's: see the header)
    if (got.has_back_end) {
        for (const auto& [reason, count] : got.reasons) refused.push_back({"C++03 back end", "declined", std::format("{} bodies: {}", count, reason)});
        for (const auto& [reason, count] : got.declined) {
            if (reason == "not found" || got.reasons.empty()) refused.push_back({"C++03 back end", "declined", std::format("{} bodies: {}", count, reason)});
        }
    }
    host_print(out);
    return Conversion{out, std::move(refused), std::move(got.stats)};
}

bool host_print(const std::filesystem::path& cpp_dir) {
    const std::filesystem::path path = cpp_dir / "JavaRT.h";
    std::string text = fs::read_text(path);
    // (written with explicit "\n" and "\t": the same text whatever line endings this source file is checked out with)
    static const std::string kOld =
        "\tstruct _PrintStream {\n"
        "\t\ttemplate<class T> void println(const T&) {}\n"
        "\t\tvoid println() {}\n"
        "\t\ttemplate<class T> void print(const T&) {}\n"
        "\t};\n"
        "\tstatic _PrintStream out;\n"
        "\tstatic _PrintStream err;";
    static const std::string kNew =
        "\tstruct _PrintStream {                     // (the judge's host PrintStream: CodeBreaker's C# kit)\n"
        "\t\tFILE* f;\n"
        "\t\texplicit _PrintStream(FILE* to) : f(to) {}\n"
        "\t\tvoid put(const std::string& s) { fwrite(s.data(), 1, s.size(), f); }\n"
        "\t\ttemplate<class T> void print(const T& x) { std::ostringstream o; o << x; put(o.str()); }\n"
        "\t\tvoid print(bool b) { put(b ? \"true\" : \"false\"); }\n"
        "\t\tvoid print(char c) { put(std::string(1, c)); }\n"
        "\t\tvoid print(double d) { put(j2xNumberText(d, false)); }\n"
        "\t\tvoid print(float d) { put(j2xNumberText(d, true)); }\n"
        "\t\ttemplate<class T> void println(const T& x) { print(x); put(\"\\n\"); }\n"
        "\t\tvoid println() { put(\"\\n\"); }\n"
        "\t};\n"
        "\tstatic _PrintStream out(stdout);\n"
        "\tstatic _PrintStream err(stderr);";
    if (text.find(kOld) == std::string::npos) return false;
    text = text::replace_all(text, kOld, kNew);
    if (text.find("#include <sstream>") == std::string::npos) text = "#include <sstream>\n#include <cstdio>\n" + text;
    fs::write_text(path, text);
    return true;
}

}  // namespace cb::translate::dotnet::cs2cpp
