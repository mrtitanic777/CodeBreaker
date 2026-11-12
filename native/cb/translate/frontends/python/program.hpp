// cb/translate/frontends/python/program.hpp -- the Python modules being translated, their symbols, and the facts
// inference gathers about them.
//
// A Program is a set of Python modules parsed by pyfront (CPython's `ast` shape), each with its symbol table: its
// functions, classes, module-level names, and its imports resolved to the program's own modules or to outside libraries.
// The facts are what inference learns, program-wide, to a fixpoint (`solve`):
//
//     params[fn]       the type of each parameter: its annotation, else the join of every call site's argument
//     returns[fn]      the join of every value it returns
//     attrs[class]     each attribute of a class: the join of every value stored to it
//     globals[module]  each module-level name
//     sites            per container object class (the sites that flow together are one class: union-find): a list's
//                      or set's element, a dict's keys and values, a record's fields and which fields every creation sets
//
// The inference itself is not here: `solve` drives an `Evaluator` (a function or a module's top level run on types), so
// this header does not depend on the inference that reads and writes the facts.
//
// Identity: a node's identity is its address, a `const syntax::Node*`; every node lives in its module's tree, which the
// Program owns, so a pointer stays valid as long as the Program does. The Program is neither copied nor moved (the symbols
// point back at their module).
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/translate/frontends/python/pyast.hpp"
#include "cb/translate/frontends/python/pytypes.hpp"

namespace cb::translate::frontends::python::program {

namespace syntax = ::pyfront;
using pytypes::Type;
using StringSet = std::set<std::string>;
using SiteId = std::uint32_t;   // a container site, 1-based; 0 is "none"

struct Module;
struct Class;
class Program;

// ---- names the program resolves ------------------------------------------------------------------------

// What a module-level name refers to, or what an import binds.
struct Ref {
    enum class Kind {
        Function,     // `name` is the function's qualified name
        Class,        // `name` is the class's qualified name
        Global,       // a module-level name: `name` is the module, `member` the name
        Module,       // a module of the program
        StdModule,    // a module of the standard library
        Extern,       // a library the front end does not map
        StdObject,    // an object of a standard-library module (`from os import path`)
        Missing,      // `name.member` is not a module or a name of the program (`from mod import absent`)
        Member        // an import of `member` from the program's module `name`, not yet resolved (import tables only)
    };
    Kind kind = Kind::Extern;
    std::string name;
    std::string member;
    bool operator==(const Ref&) const = default;
};

// The builtin exceptions the program's classes may derive from.
bool is_builtin_exception(std::string_view name);
// A module of the standard library, by its top-level name.
bool is_stdlib_module(std::string_view top);

// ---- symbols ----------------------------------------------------------------------------------------------

enum class FnKind { Function, Method, Static, ClassMethod, Property, Async };
std::string_view fn_kind_word(FnKind kind);

// A function or method of the program.
struct Fn {
    std::string qname;
    const syntax::FunctionDef* node = nullptr;
    Module* module = nullptr;
    Class* cls = nullptr;                                       // the class it is a method of
    std::string name;
    FnKind kind = FnKind::Function;
    std::vector<std::string> decorators;                        // each decorator's last name ("?" for any other form)
    std::vector<std::string> params;                            // positional parameters, positional-only first
    std::vector<std::string> kwonly;
    OrderedMap<std::string, const syntax::Expr*> annotations;   // parameter -> its annotation (those that have one)
    OrderedMap<std::string, const syntax::Expr*> defaults;      // parameter -> its default value
    std::optional<std::string> varargs;
    std::optional<std::string> kwargs;
    std::size_t lines = 0;
    // The parameters the function is ready to get None for: a default of None, or a test of it against None or of its
    // truth. Any other parameter is not None by contract -- a caller that may pass None is refused there, where the None
    // comes from. Set by Program::index.
    StringSet none_aware;

    Fn(std::string qualified_name, const syntax::FunctionDef& def, Module& owner, Class* owner_class = nullptr);
    // The parameters a caller passes (a method's self, a classmethod's cls not among them).
    std::vector<std::string> bound() const;
};

struct Class {
    std::string qname;
    const syntax::ClassDef* node = nullptr;
    Module* module = nullptr;
    std::string name;
    std::vector<const syntax::Expr*> base_nodes;
    std::vector<std::string> bases;                       // qualified names of the program's classes it derives from
    std::optional<std::string> exception;                 // the builtin exception it derives from, if it is one
    OrderedMap<std::string, Fn*> methods;
    OrderedMap<std::string, const syntax::Expr*> class_attrs;   // name -> the value assigned in the class body
    std::vector<std::string> subclasses;
    std::vector<std::string> decorators;
    std::optional<std::string> foreign_base;              // a base the program does not have (a library class), as text

    Class(std::string qualified_name, const syntax::ClassDef& def, Module& owner);
};

struct Module {
    std::string name;
    std::filesystem::path path;                           // empty for a module made from a string
    std::string source;
    std::unique_ptr<syntax::Module> tree;
    OrderedMap<std::string, Fn*> functions;
    OrderedMap<std::string, Class*> classes;
    // name -> the values assigned at module level (nullptr: an unpacking, or an annotation with no value)
    OrderedMap<std::string, std::vector<const syntax::Expr*>> assigns;
    // local name -> Module | Member | Extern | StdModule
    OrderedMap<std::string, Ref> imports;
    std::vector<const syntax::Stmt*> statements;          // module-level statements that are not defs, imports or names
    bool is_package = false;                              // an __init__.py

    // Parses `source`; throws ParseError (the file is `path`, or `name` when there is none).
    Module(std::string module_name, std::filesystem::path file, std::string text);
    // Does the module start with a docstring?
    bool has_docstring() const;
};

// ---- facts ---------------------------------------------------------------------------------------------------

// What a running evaluation reads and what a round changes. The Python spelled these ('p', fn), ('d',), ('g', module),
// ('s', root), ('a', class), ('ca',), ('r', fn).
struct FactKey {
    enum class Tag { Params, Defaults, Globals, Site, Attrs, ClassAttrs, Returns };
    Tag tag = Tag::Params;
    std::string name;       // the function, module or class; empty for Defaults, Site, ClassAttrs
    SiteId site = 0;        // for Site
    bool operator==(const FactKey&) const = default;

    static FactKey params(std::string fn) { return {Tag::Params, std::move(fn), 0}; }
    static FactKey defaults() { return {Tag::Defaults, {}, 0}; }
    static FactKey globals(std::string module) { return {Tag::Globals, std::move(module), 0}; }
    static FactKey of_site(SiteId root) { return {Tag::Site, {}, root}; }
    static FactKey attrs(std::string cls) { return {Tag::Attrs, std::move(cls), 0}; }
    static FactKey class_attrs() { return {Tag::ClassAttrs, {}, 0}; }
    static FactKey returns(std::string fn) { return {Tag::Returns, std::move(fn), 0}; }
};
struct FactKeyHash {
    std::size_t operator()(const FactKey& k) const;
};
using FactSet = std::unordered_set<FactKey, FactKeyHash>;

enum class Container { List, Dict, Set };
std::string_view container_word(Container kind);

// Where a container was made: the module and line.
struct Origin {
    std::string module;
    std::uint32_t line = 0;
};

// One container site. The facts of a class of sites live in its root's entry (`find`); an entry that is not a root is
// stale and is only read by the fields noted below.
struct Site {
    SiteId parent = 0;                                  // union-find parent; a root is its own parent
    Container kind = Container::List;
    Type elem;                                          // list/set element; dict values when used as a dict
    Type keys;                                          // dict keys
    OrderedMap<std::string, Type> fields;               // a record's fields, in the order they were first set
    std::optional<StringSet> required;                  // the keys every creation sets (nullopt: no creation seen)
    std::optional<std::string> dynamic;                 // why it is a dict and not a record
    std::vector<std::string> hints;                     // names it was given in the source, for naming the C++ type
    std::optional<Origin> where;                        // nullopt for a site made with no place
    const syntax::Node* node = nullptr;                 // the AST node that is it
    bool mutated = false;                               // changed after it was built (reference semantics)
    std::optional<std::string> poisoned;                // a function that added it an item of no type
    // Not merged by `union_`: set on the site a ** spread creation made.
    std::optional<StringSet> spread_keys;               // keys a ** spread brought, set by every creation
    std::set<SiteId> spread_from;                       // the sites the spreads came from
};

enum class IntPolicy { Strict, Checked };   // Checked: an int beyond int64 raises OverflowError in the translated C++

// Which of a site's two element tables `grow` joins into.
enum class Table { Elem, Keys };

// A program evaluation `Program::solve` runs: a function, or a module's top level, on types. Each reads facts through
// `Program::read` and changes them through `Program::touch`; a function that throws is refused (`Program::crashes`).
class Evaluator {
public:
    virtual ~Evaluator() = default;
    virtual void run_module(Program& program, Module& module) = 0;
    virtual void run_function(Program& program, Fn& fn) = 0;
};

// A fact keyed by (function, parameter) or (class, name).
using NameKey = std::pair<std::string, std::string>;

// A caller that passed a parameter a value of no type, and why.
struct Poison {
    std::string caller;
    std::string why;
};

class Program {
public:
    Program() = default;
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    // ---- symbols (filled by add_module/add_tree, then index)
    OrderedMap<std::string, std::unique_ptr<Module>> modules;   // qualified module name -> Module, in the order added
    OrderedMap<std::string, std::unique_ptr<Fn>> fns;           // qualified name -> Fn
    OrderedMap<std::string, std::unique_ptr<Class>> classes;    // qualified name -> Class

    // ---- facts
    std::unordered_map<std::string, OrderedMap<std::string, Type>> params;    // fn -> {param: type}
    std::unordered_map<std::string, Type> returns;                            // fn -> the join of what it returns
    std::unordered_map<std::string, OrderedMap<std::string, Type>> attrs;     // class -> {attr: type}, in the order stored
    std::unordered_map<std::string, OrderedMap<std::string, Type>> globals;   // module -> {name: type}
    std::map<NameKey, Type> class_attr_types;                                 // (class, name) -> the type of a class-level value
    std::map<NameKey, Type> default_types;                                    // (fn, param) -> the type of its default
    OrderedMap<std::string, std::string> crashes;                             // fn -> the error that stopped it
    std::map<NameKey, Poison> param_poison;                                   // (fn, param) -> the caller that passed no type
    std::map<NameKey, std::string> poisoned;                                  // (class, attribute) -> a function that stored no type
    std::unordered_map<std::string, StringSet> index_params;                  // fn -> the parameters it indexes with at once
    std::unordered_map<const syntax::Node*, std::vector<std::string>> literal_keys;   // dict display -> its constant keys
    std::unordered_set<const syntax::Node*> builder_targets;   // item stores that fill a record right after its creation
    IntPolicy int_policy = IntPolicy::Strict;
    int round = 0;                                       // the round `solve` is in

    // ---- loading
    // Parses `source` as module `name` and adds it. Throws ParseError.
    Module& add_module(const std::string& name, std::string source, const std::filesystem::path& path = {});
    // Reads and adds the file. Throws IoError (cannot read) or ParseError.
    Module& add_file(const std::string& name, const std::filesystem::path& path);
    // Adds every .py file under `root` (the folder of package `package`), a folder's files before its subfolders, each
    // in name order. A file that does not parse is skipped; the skipped paths are returned.
    std::vector<std::filesystem::path> add_tree(const std::filesystem::path& root, const std::string& package);
    // Builds every module's symbol table, then resolves class bases and imports.
    void index();

    // ---- type lookups
    // The type a parameter's annotation gives (Bottom when it has none).
    Type annotation_type(const Fn& fn, const std::string& param);
    Type ann_to_type(Module& module, const syntax::Expr* annotation);
    // The parameter's fact joined with its annotation and its default.
    Type param_type(const Fn& fn, const std::string& param);
    Type global_type(const std::string& module, const std::string& name);
    Fn* find_method(const std::string& cls, const std::string& name);
    // The attributes the methods of the class assign on their first parameter.
    const StringSet& assigned_attrs(const std::string& cls);
    // ... of the class or any class derived from it.
    StringSet assigned_attrs_any(const std::string& cls);
    // The most basic class that has the attribute (where the C++ declares the member).
    std::optional<std::string> attr_owner(const std::string& cls, const std::string& attr);
    // nullopt: no such attribute.
    std::optional<Type> attr_type(const std::string& cls, const std::string& attr);
    Type class_attr_type(const std::string& cls, const std::string& attr);
    // The statement list a statement is in, once `map_blocks` has seen its function.
    const std::vector<syntax::Stmt*>* block_of(const syntax::Stmt* stmt) const;
    void map_blocks(const syntax::FunctionDef& fn);

    // ---- solving
    // Runs every function and module, then round after round only those that read a fact the last round changed, until
    // none changed. Returns the rounds run. `log` gets one line per round.
    std::size_t solve(Evaluator& evaluator, std::size_t max_rounds = 40,
                      const std::function<void(const std::string&)>& log = {});
    // Runs `body` (the recording pass of one function) with the suspension cleared before and after.
    template <class Body>
    decltype(auto) evaluate(Body&& body) {
        SuspendReset reset(*this);
        return std::forward<Body>(body)();
    }

    // What a module-level name refers to; nullopt for a name that is not defined there.
    std::optional<Ref> resolve_global(Module& module, const std::string& name);
    std::optional<Ref> resolve_expr_name(Module& module, const syntax::Expr* expr);

    // ---- containers
    SiteId find(SiteId id);                           // the root of the site's class
    Site& site(SiteId id) { return sites_.at(id); }
    const Site& site(SiteId id) const { return sites_.at(id); }
    Site& root(SiteId id) { return sites_.at(find(id)); }
    std::size_t site_count() const { return sites_.size() - 1; }   // sites are 1..site_count()
    // The container site of an AST node (the same node is the same site every round).
    SiteId new_site(const syntax::Node* node, Container kind, std::optional<Origin> where = std::nullopt);
    SiteId union_sites(SiteId a, SiteId b);
    void mark_mutated(SiteId id);
    // Joins `type` into one of the site's tables and notes a change. (Not while a loop is still being run to its
    // fixpoint: its heads are not final, and a widened bound would stick.) Returns the root.
    SiteId grow(Table table, SiteId id, const Type& type);
    void grow_field(SiteId id, const std::string& key, const Type& type);
    // A creation of the container set exactly these keys.
    void created(SiteId id, const StringSet& keys);
    void make_dynamic(SiteId id, const std::string& why);
    bool is_record(SiteId id);
    void hint(SiteId id, const std::string& name);

    // ---- facts' bookkeeping
    // A fact changed: what read it runs again.
    void touch(const FactKey& key);
    // The running evaluation reads a fact.
    void read(const FactKey& key);
    Type ret(const std::string& fn);
    // The names a function binds (its parameters among them).
    const StringSet& fn_locals(const Fn& fn);
    // The subscripts and calls of an expression or statement, outside nested scopes, in source order (cached).
    const std::vector<const syntax::Expr*>& access_nodes(pyast::AnyNode node);
    // While a loop body runs toward its fixpoint its facts are not final: nothing is recorded.
    bool suspended() const { return suspend_ > 0; }
    class SuspendScope {
    public:
        explicit SuspendScope(Program& program) : program_(program) { ++program_.suspend_; }
        ~SuspendScope() { --program_.suspend_; }
        SuspendScope(const SuspendScope&) = delete;
        SuspendScope& operator=(const SuspendScope&) = delete;

    private:
        Program& program_;
    };
    // A fact that keeps growing round after round: its integer bounds widened (to thresholds), a type that keeps
    // nesting deeper given up -- so the rounds end.
    Type settle(const Type& old_type, const Type& new_type, int depth = 0);
    const FactSet& dirty() const { return dirty_; }

    // ---- the join
    Type join(const Type& a, const Type& b);
    std::optional<std::string> common_base(const std::string& x, const std::string& y);
    // The class and its bases, breadth first, the program's classes only.
    std::vector<std::string> mro(const std::string& cls);
    std::vector<std::string> descendants(const std::string& cls);
    // `type` with every container named by its root site (for comparing facts between rounds).
    Type canon(const Type& type);

private:
    class SuspendReset {
    public:
        explicit SuspendReset(Program& program) : program_(program) { program_.suspend_ = 0; }
        ~SuspendReset() { program_.suspend_ = 0; }
        SuspendReset(const SuspendReset&) = delete;
        SuspendReset& operator=(const SuspendReset&) = delete;

    private:
        Program& program_;
    };

    void index_stmt(Module& module, const syntax::Stmt& stmt);
    Ref module_ref(const std::string& name) const;
    std::string from_base(const Module& module, const syntax::ImportFrom& stmt) const;
    std::optional<Ref> resolve_global_(Module& module, const std::string& name, int depth);
    Type annotation_(Module& module, const syntax::Expr* annotation);
    Type& table_of(Table table, SiteId root);

    std::vector<Site> sites_ = std::vector<Site>(1);    // index 0 is unused
    std::unordered_map<const syntax::Node*, SiteId> site_of_node_;
    std::unordered_map<const syntax::Expr*, Type> ann_cache_;
    std::vector<std::unique_ptr<syntax::Module>> annotation_trees_;   // trees parsed from string annotations
    std::unordered_map<const syntax::Stmt*, const std::vector<syntax::Stmt*>*> blocks_;
    std::unordered_map<std::string, StringSet> assigned_attrs_;
    std::unordered_map<std::string, StringSet> fn_locals_;
    std::unordered_map<const syntax::Node*, std::vector<const syntax::Expr*>> access_;
    FactSet* reads_ = nullptr;                          // the facts the running evaluation reads
    FactSet dirty_;                                     // the facts changed this round
    int suspend_ = 0;
};

}  // namespace cb::translate::frontends::python::program
