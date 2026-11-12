#include "cb/java/projects/minecraft/applet_ping.hpp"

#include <map>
#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/java/projects/minecraft/converted_tree.hpp"
#include "cb/java/projects/minecraft/ir_edit.hpp"

namespace cb::java::projects::minecraft::applet_ping {

namespace ir = translate::ir;
namespace edit = ir_edit;
namespace tree = converted_tree;

namespace {

// The setLevel ping: a try that (1) opens getDocumentBase()->...->openStream(), (2) guards a
// `this-><field> = <var>;` assignment inside an if on the host check, (3) is followed by an empty catch.
// `[^{}]*?` keeps each span inside the brace-free prefix so the match does not cross into the if/catch bodies wrongly.
const Regex& ping_site() {
    static const Regex re(
        R"(try\s*\{[^{}]*?getDocumentBase\(\)[^{}]*?openStream\b[^{}]*?)"
        R"(if\s*\([^{}]*?\)\s*\{\s*(this->\w+\s*=\s*\w+\s*;)\s*\}\s*)"
        R"(\}\s*catch\s*\([^)]*\)\s*\{\s*\})");
    return re;
}

// Fail loud: an openStream() on getDocumentBase() still guarded by a try that survived the pass.
const Regex& ping_survivor() {
    static const Regex re(R"(try\s*\{[^{}]*?getDocumentBase\(\)[^{}]*?openStream\b)");
    return re;
}

constexpr std::string_view kNote =
    "   // J2X applet-ping-bypass: console has no applet -- getDocumentBase()->openStream() is a NULL-returning stub "
    "and readLine() on it FAULTS (an access violation, NOT caught by try/catch). Drop the ping and set the level "
    "directly (matches the OG Indev hand-port). [generator: applet_ping]";

// Text that cannot hold a ping is skipped without running the regexes.
bool mentions_ping(std::string_view text) {
    return text.find("getDocumentBase") != std::string_view::npos && text.find("openStream") != std::string_view::npos;
}

}  // namespace

Rewritten rewrite(std::string_view text) {
    Rewritten out;
    out.text = ping_site().replace_all(text, [&](const RegexMatch& m) {
        ++out.sites;
        return std::string(m.group(1)) + std::string(kNote);
    });
    return out;
}

Report run(const std::filesystem::path& out_dir) {
    Report report;
    for (const std::string& name : tree::cpp_files(out_dir)) {
        const std::filesystem::path path = out_dir / fs::from_text(name);
        auto file = tree::read_cpp(path);
        if (!file || !mentions_ping(file->text)) continue;
        Rewritten r = rewrite(file->text);
        if (r.text != file->text) {
            tree::write_cpp(path, r.text, file->crlf);
            report.files.push_back({name, r.sites});
            report.sites += r.sites;
        }
        if (ping_survivor().test(r.text)) report.stranded.push_back(name);
    }
    return report;
}

namespace {

// The `this.f = v` assignment statement a ping try reduces to, or nullptr when the try is not the ping. The try must
// call getDocumentBase() and openStream(), have only empty catches and no finally/resources, and hold exactly one if
// (no else) whose only statement is the assignment.
ir::Node* ping_assignment(ir::Node* stmt) {
    const ir::Try* t = ir::get<ir::Try>(stmt);
    if (t == nullptr || t->finally_body != nullptr || !t->resources.empty()) return nullptr;
    std::set<std::string, std::less<>> calls;
    for (ir::Node* n : ir::walk(t->body)) {
        const ir::Call* c = ir::get<ir::Call>(n);
        if (c != nullptr && c->sym != nullptr) calls.insert(c->sym->name);
    }
    if (!calls.contains("getDocumentBase") || !calls.contains("openStream")) return nullptr;
    for (ir::Node* cn : t->catches) {
        const ir::Catch* c = ir::get<ir::Catch>(cn);
        const ir::Block* b = c != nullptr ? ir::get<ir::Block>(c->body) : nullptr;
        if (b != nullptr && !b->stmts.empty()) return nullptr;
    }
    const ir::Block* try_body = ir::get<ir::Block>(t->body);
    if (try_body == nullptr) return nullptr;
    const ir::If* guard = nullptr;
    for (ir::Node* x : try_body->stmts) {
        if (const ir::If* i = ir::get<ir::If>(x)) {
            if (guard != nullptr) return nullptr;   // more than one guard
            guard = i;
        }
    }
    if (guard == nullptr || ir::present(guard->orelse)) return nullptr;
    if (std::holds_alternative<std::vector<ir::Node*>>(guard->then)) return nullptr;   // a bare list is not one statement
    const std::vector<ir::Node*> then = edit::block_or_single(guard->then);
    if (then.size() != 1) return nullptr;
    const ir::ExprStmt* es = ir::get<ir::ExprStmt>(then[0]);
    if (es == nullptr) return nullptr;
    const ir::Assign* a = ir::get<ir::Assign>(edit::strip(es->expr));
    if (a == nullptr || a->op) return nullptr;
    const ir::Select* target = ir::get<ir::Select>(a->target);
    if (target == nullptr || !ir::is<ir::This>(target->obj)) return nullptr;
    if (!ir::is<ir::Name>(edit::strip(a->value))) return nullptr;
    return then[0];
}

}  // namespace

Report patch(const std::vector<ir::Node*>& units) {
    std::map<std::string, std::size_t> per_class;   // sorted: the report lists classes by name
    for (const edit::MethodInClass& m : edit::methods(units)) {
        const std::string cls = m.cls->name.value_or("(anonymous)");
        for (ir::Node* root : ir::statements(m.method->body)) {
            for (std::vector<ir::Node*>* list : edit::stmt_lists(root)) {
                for (ir::Node*& s : *list) {
                    if (ir::Node* assignment = ping_assignment(s)) {
                        s = assignment;   // set the level directly: no applet on console
                        ++per_class[cls];
                    }
                }
            }
        }
    }
    Report report;
    for (const auto& [cls, n] : per_class) {
        report.files.push_back({cls + ".cpp", n});
        report.sites += n;
    }
    return report;
}

std::vector<std::string> stranded(const std::filesystem::path& out_dir) {
    std::vector<std::string> out;
    for (const std::string& name : tree::cpp_files(out_dir)) {
        auto file = tree::read_cpp(out_dir / fs::from_text(name));
        if (file && ping_survivor().test(file->text)) out.push_back(name);
    }
    return out;
}

}  // namespace cb::java::projects::minecraft::applet_ping
