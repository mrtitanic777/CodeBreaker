// Tests of cb::translate::frontends::python::package: a small program of two modules through the whole front end (inference,
// lowering, units), and what the units hold -- the functions translated and refused (with why), the records and classes
// declared, the module names lowered or refused, and the modules a unit names. The expectations are what the original
// (codebreaker/translate/frontends/python/__init__.py) made of the same sources, read through by hand.
//
// DIVERGES from the original: (1) a unit's path for a module made from a string is "<name>", as the Python wrote it, and a
// module read from a file keeps its own path text with '/' separators; (2) the translation keeps the arena its trees live in:
// a `Translation` outlives them, where the Python's garbage collector did; (3) a refusal is a `Refusal` (class, detail, line),
// not a pair of strings.
#include "cb/translate/frontends/python/package.hpp"

#include <algorithm>
#include <filesystem>
#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/methods.hpp"

namespace {

namespace pk = cb::translate::frontends::python::package;
namespace in = cb::translate::frontends::python::infer;
namespace lo = cb::translate::frontends::python::lower;
namespace me = cb::translate::frontends::python::methods;
namespace ir = cb::translate::ir;

constexpr const char* kShapes =
    "LIMIT = 100\n"
    "NAME = 'shapes'\n"
    "ROOMS = [1, 2]\n"
    "ROOMS = [3]\n"
    "class Point:\n"
    "    scale = 3\n"
    "    def __init__(self, label: str, size: bytes):\n"
    "        self.label = label\n"
    "        self.size = len(size)\n"
    "    def width(self) -> int:\n"
    "        return self.size + len(self.label)\n"
    "def origin() -> Point:\n"
    "    return Point('o', b'')\n"
    "def limited(s: str) -> int:\n"
    "    if len(s) > LIMIT:\n"
    "        return LIMIT\n"
    "    return len(s)\n"
    "def rooms() -> list[int]:\n"
    "    return ROOMS\n"
    "def uses_frob(n: str) -> int:\n"
    "    return frob(n)\n"
    "def calls_it(n: str) -> int:\n"
    "    return uses_frob(n)\n";

constexpr const char* kApp =
    "from shapes import Point\n"
    "def far(p: Point) -> int:\n"
    "    return p.width()\n";

struct Built {
    std::unique_ptr<cb::translate::frontends::python::program::Program> program;
    pk::Translation done;
};

// Options for the modules (and functions) named.
pk::Options only(std::set<std::string> modules, std::optional<std::set<std::string>> functions = std::nullopt) {
    pk::Options o;
    o.modules = std::move(modules);
    o.functions = std::move(functions);
    return o;
}

Built build(std::vector<std::pair<std::string, std::string>> sources, pk::Options options = {}) {
    Built b;
    b.program = pk::program_of_sources(sources);
    in::Inference inference(me::library());
    b.program->solve(inference);
    b.done = pk::translate(*b.program, options);
    return b;
}

const ir::ClassDecl* declared(const lo::ModuleResult& r, const std::string& qname) {
    for (ir::Node* t : ir::get<ir::Unit>(r.unit)->types) {
        const auto* c = ir::get<ir::ClassDecl>(t);
        if (c && c->qname == qname)
            return c;
    }
    return nullptr;
}

}  // namespace

CB_TEST("translate.python.package.a_module_becomes_a_unit", [] {
    Built b = build({{"shapes", kShapes}}, only({"shapes"}));
    const lo::ModuleResult* r = b.done.find("shapes");
    CB_CHECK(r != nullptr);
    const ir::Unit* unit = ir::get<ir::Unit>(r->unit);
    CB_CHECK_EQ(unit->package.value_or(""), "shapes");
    CB_CHECK_EQ(unit->path.value_or(""), "<shapes>");
    CB_CHECK_EQ(unit->lang.value_or(""), "python");

    // what was translated, and what was left in Python
    CB_CHECK(r->translated.contains("shapes.origin"));
    CB_CHECK(r->translated.contains("shapes.limited"));
    CB_CHECK(r->translated.contains("shapes.Point.width"));
    CB_CHECK(r->refused.contains("shapes.uses_frob"));
    CB_CHECK_EQ(r->refused.find("shapes.uses_frob")->reason(), "unresolved:frob");
    CB_CHECK(r->refused.contains("shapes.calls_it"));   // it calls a refused function
    CB_CHECK_EQ(r->refused.find("shapes.calls_it")->cls, lo::RefusalClass::CallsRefused);
    CB_CHECK(*r->lines.find("shapes.limited") >= 4u);

    // the declarations: the class with its attributes and methods, the module with its names and functions
    const ir::ClassDecl* point = declared(*r, "shapes.Point");
    CB_CHECK(point != nullptr);
    CB_CHECK_EQ(point->kind.value_or(""), "class");
    std::vector<std::string> members;
    for (ir::Node* m : point->members) {
        if (const auto* f = ir::get<ir::FieldDecl>(m))
            members.push_back("field " + f->sym->name + (f->sym->is_static ? " static" : ""));
        else if (const auto* md = ir::get<ir::MethodDecl>(m))
            members.push_back("method " + md->name.value_or("?"));
    }
    // the attributes in the order __init__ assigns them, the class-level constant no instance assigns, then the methods
    CB_CHECK_EQ(cb::text::join(members, ", "), "field label, field size, field scale static, method __init__, method width");

    const ir::ClassDecl* module = declared(*r, "shapes");
    CB_CHECK(module != nullptr);
    CB_CHECK_EQ(module->kind.value_or(""), "module");
    bool has_limit = false, has_name = false, has_rooms = false, says_refused = false;
    for (ir::Node* m : module->members) {
        if (const auto* f = ir::get<ir::FieldDecl>(m)) {
            has_limit = has_limit || f->sym->name == "LIMIT";
            has_name = has_name || f->sym->name == "NAME";
            has_rooms = has_rooms || f->sym->name == "ROOMS";
        }
        if (const auto* c = ir::get<ir::Comment>(m))
            says_refused = says_refused || c->text.value_or("").starts_with("not translated: uses_frob");
    }
    CB_CHECK(has_limit);       // used by `limited`: a constant
    CB_CHECK(!has_name);       // nothing reads it
    CB_CHECK(!has_rooms);      // assigned twice: it cannot be lowered ...
    CB_CHECK(says_refused);
    CB_CHECK(r->refused.contains("shapes.rooms"));   // ... and the function that reads it is refused with it
    CB_CHECK_EQ(r->refused.find("shapes.rooms")->cls, lo::RefusalClass::CallsRefused);
    CB_CHECK(r->refused.find("shapes.rooms")->detail.starts_with("module name ROOMS ("));
});

CB_TEST("translate.python.package.a_name_that_is_a_constant_is_a_static_const_field", [] {
    Built b = build({{"shapes", kShapes}}, only({"shapes"}));
    const lo::ModuleResult* r = b.done.find("shapes");
    CB_CHECK_EQ(r->global_decls.size(), 1u);
    if (r->global_decls.empty())
        return;
    const auto* limit = ir::get<ir::FieldDecl>(r->global_decls[0]);
    CB_CHECK_EQ(limit->sym->name, "LIMIT");
    CB_CHECK(limit->sym->is_static);
    CB_CHECK((limit->mods == std::vector<std::string>{"static", "const"}));
    CB_CHECK(limit->init != nullptr);
    CB_CHECK_EQ(limit->sym->owner.value_or(""), "shapes");
});

CB_TEST("translate.python.package.the_modules_a_unit_names_are_translated_with_it", [] {
    Built b = build({{"shapes", kShapes}, {"app", kApp}}, only({"app"}));
    const lo::ModuleResult* app = b.done.find("app");
    CB_CHECK(app != nullptr);
    CB_CHECK(app->translated.contains("app.far"));
    const ir::Unit* unit = ir::get<ir::Unit>(app->unit);
    CB_CHECK(unit->imports == std::vector<std::string>{"shapes"});
    CB_CHECK(b.done.find("shapes") != nullptr);   // asked for app only: the unit it names was made too
    CB_CHECK_EQ(b.done.modules().size(), 2u);
    CB_CHECK_EQ(b.done.modules().begin()->first, "app");
});

CB_TEST("translate.python.package.only_the_functions_asked_for_are_lowered", [] {
    Built b = build({{"shapes", kShapes}}, only({"shapes"}, std::set<std::string>{"shapes.limited"}));
    const lo::ModuleResult* r = b.done.find("shapes");
    CB_CHECK(r->translated.contains("shapes.limited"));
    CB_CHECK(!r->translated.contains("shapes.origin"));
    CB_CHECK(!r->refused.contains("shapes.origin"));
});

CB_TEST("translate.python.package.a_module_that_is_not_there_is_not_translated", [] {
    Built b = build({{"shapes", kShapes}}, only({"shapes", "nothing"}));
    CB_CHECK(b.done.find("nothing") == nullptr);
    CB_CHECK(b.done.find("shapes") != nullptr);
});

CB_TEST("translate.python.package.types_of_names_the_programs_own_types", [] {
    const auto point = ir::class_type("shapes.Point");
    const auto list = ir::class_type("py.list", {point});
    const auto outer = ir::class_type("py.tuple", {ir::class_type("shapes.Other"), list, ir::i32_type()});
    CB_CHECK((pk::types_of(outer) == std::set<std::string>{"shapes.Other", "shapes.Point"}));
    CB_CHECK(pk::types_of(ir::i32_type()).empty());
    CB_CHECK(pk::types_of(ir::TypeP()).empty());
});

// A census of real open-source Python (docs/TESTED_SOFTWARE.md, "The universal translator"): a dozen small modules of CPython's own standard
// library (PSF License 2.0, read in place from the installed Python's Lib folder, never copied into the checkout), each through the whole
// front end alone: how many of its functions lower to L2 and how many are refused, with the reason. It asserts only that every module
// goes through without an error; the numbers are the report. Where no Python is installed it does nothing.
CB_TEST("translate.python.package.the_standard_library_census", [] {
    std::vector<std::filesystem::path> libs;
    if (const auto env = cb::fs::get_env("CB_PYTHON_STDLIB")) libs.emplace_back(*env);
    for (const char* v : {"C:/Python314/Lib", "C:/Python313/Lib", "C:/Python312/Lib", "/usr/lib/python3/lib", "/usr/lib/python3.12"}) libs.emplace_back(v);
    std::filesystem::path lib;
    for (const auto& c : libs) {
        if (std::filesystem::is_regular_file(c / "bisect.py")) {
            lib = c;
            break;
        }
    }
    if (lib.empty()) return;
    std::size_t translated = 0, refused = 0, modules = 0;
    std::map<std::string, std::size_t> why;
    for (const char* name : {"bisect", "heapq", "colorsys", "keyword", "operator", "fnmatch", "genericpath", "posixpath", "shlex", "textwrap", "string", "reprlib"}) {
        const std::filesystem::path file = lib / (std::string(name) + ".py");
        if (!std::filesystem::is_regular_file(file)) continue;
        const Built b = build({{name, cb::fs::read_text(file)}}, only({name}));
        const lo::ModuleResult* r = b.done.find(name);
        CB_CHECK(r != nullptr);
        if (r == nullptr) continue;
        ++modules;
        translated += r->translated.size();
        refused += r->refused.size();
        for (const auto& entry : r->refused) ++why[entry.second.reason().substr(0, entry.second.reason().find(':'))];
        std::fprintf(stderr, "  census %-12s %3zu translated, %3zu refused\n", name, r->translated.size(), r->refused.size());
    }
    std::string reasons;
    for (const auto& [reason, n] : why) reasons += " " + reason + "=" + std::to_string(n);
    std::fprintf(stderr, "  census total: %zu modules, %zu functions translated, %zu refused (%s )\n", modules, translated, refused, reasons.c_str());
    CB_CHECK(modules > 0);
});
