// cb/translate/backends/csharp.hpp -- the translator's C# back end: whole programs from the IR, in C# as a person writes it.
//
// It reads any front end's units -- each says its source language (Unit::lang) -- and writes one .cs file per unit. Where
// the languages line up the output is idiomatic: properties, `using`, `lock`, generics, lambdas and delegates, `foreach`,
// `is` patterns, switch expressions, extension-method calls written on their receiver, operators written as operators. Where
// they do not it writes the faithful form: from Java a method Java would dispatch virtually is `virtual`, labelled break and
// continue are gotos to labels after and inside the loop, a switch section that falls through ends in `goto case`, a constant
// expression that Java lets wrap is `unchecked`; from C# the source's own forms.
//
// What it cannot write faithfully it declines, never guesses: a method body holding something it does not carry becomes
// `throw new System.NotSupportedException("CB-GAP: ...")`, listed in the ledger with the reason; a declaration it cannot carry
// (a Java inner class that uses its outer instance, an anonymous class) is a ledger entry and the unit is declined.
//
// A source language's own library -- Java's types, calls and semantics in C# -- is not in this module: it is a `Library`
// (below), registered by language in the Options. A Java unit written without one is declined class by class.
//
//     ir::Node* units[] = {unit};                                       // Unit nodes, from any front end
//     csharp::Program program(units, {.runtime_dir = "dotnet/runtime"});
//     for (const csharp::File& f : program.files()) ...                 // {"Shapes.cs", text}, then the runtime files used
//     for (const csharp::Gap& g : program.ledger()) ...                 // what was declined, and why
//     program.write("out");                                             // the files into a folder
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::backends::csharp {

namespace ir = cb::translate::ir;

// Precedence of an expression's outermost operator (C# 12 spec, highest first).
inline constexpr int kPrimary = 16, kUnary = 15, kMul = 14, kAdd = 13, kShift = 12, kRel = 11, kEq = 10, kAnd = 9, kXor = 8,
                     kOr = 7, kLand = 6, kLor = 5, kCoalesce = 4, kCond = 3, kAssign = 2;

// An expression's text and the precedence of its outermost operator.
struct Ex {
    std::string text;
    int prec = kPrimary;
};

// This construct is not written; the method (or unit) becomes a ledgered gap. `kind` is a short name for the ledger
// ("lambda", "anonymous-class", an Unsupported node's kind), `why` what a person reads.
class Decline : public Error {
public:
    Decline(std::string kind, std::string why) : Error(why.empty() ? kind : why), kind_(std::move(kind)), why_(why.empty() ? kind_ : std::move(why)) {}
    const std::string& decline_kind() const { return kind_; }
    const std::string& why() const { return why_; }
    CB_ERROR_BOILERPLATE(Decline)

private:
    std::string kind_;
    std::string why_;
};

// One thing declined: where (class and method), what kind of construct, why.
struct Gap {
    std::string where;
    std::string kind;
    std::string why;
    friend bool operator==(const Gap&, const Gap&) = default;
};

// One file of the program: its name and its text.
struct File {
    std::string name;
    std::string text;
};

// A name written as C# takes it: a keyword gets an `@`.
std::string ident(std::string_view name);
// Whether a type is the language's string (System.String, java.lang.String).
bool is_string(const ir::TypeP& type);
// An expression with its Paren wrappers taken off.
ir::Node* strip(ir::Node* expr);

class Program;

// What a library may ask of the writer of the unit it is called from.
class Context {
public:
    virtual ~Context() = default;
    virtual Program& program() = 0;
    // The source language of the unit being written ("java", "csharp" ...).
    virtual const std::string& lang() const = 0;
    // A type as C# spells it (through the library's own type mapping, for a class of its language).
    virtual std::string ctype(const ir::TypeP& type) = 0;
    // An expression's text and precedence.
    virtual Ex expr(ir::Node* expr, bool statement = false) = 0;
    // An expression, in parentheses where its operator binds less than `prec` (or as much, for a right operand).
    virtual std::string wrap(ir::Node* expr, int prec, bool right = false) = 0;
    // `expr` as a value of type `type`: a char from an int, as Java converts a constant implicitly.
    virtual std::string coerce(ir::Node* expr, const ir::TypeP& type) = 0;
    // A `using` the unit needs.
    virtual void add_using(std::string_view ns) = 0;
};

// A source language's knowledge of its library and its semantics, in C#: the Java library maps Java's types onto .NET's, its
// calls through the API-mapping registry, and Java's dispatch, construction order and text conversions onto what C# says.
// The writer asks it at each place the source language differs; every answer that is empty (or false) means "as the source
// is written". One Library serves a whole Program (what it notes of one unit holds for the next); `prepare` runs once before
// anything is written. A method may throw Decline.
class Library {
public:
    virtual ~Library() = default;

    // Before writing: look at the whole program (an abstract Java class that leaves an interface's method to its subclasses
    // declares it abstract).
    virtual void prepare(Program&) {}

    // ---- types
    // A class type's C# name, where the library maps it.
    virtual std::optional<std::string> type_name(Context&, const ir::TypeP&) { return std::nullopt; }
    // Whether an interface a class implements is written (a library interface with no C# counterpart is not).
    virtual bool keep_interface(const ir::TypeP&) { return true; }

    // ---- declarations
    virtual void check_class(Context&, ir::ClassDecl&) {}
    virtual std::vector<std::string> class_mods(Context&, ir::ClassDecl& c) = 0;
    virtual std::vector<std::string> field_mods(Context&, ir::FieldDecl& f, ir::ClassDecl& c) = 0;
    virtual std::vector<std::string> method_mods(Context&, ir::MethodDecl& m, ir::ClassDecl& c) = 0;
    virtual std::string method_name(Context&, ir::MethodDecl& m, ir::ClassDecl& c) = 0;
    virtual std::string member_name(Context&, const ir::Sym& s) = 0;
    // The members in the order they are written, with what the source's initialisation order moves between them (instance
    // initialisers run after the base constructor in Java, before it in C#).
    virtual std::vector<ir::Node*> arrange_members(Context&, ir::ClassDecl&, std::vector<ir::Node*> members) { return members; }
    // A constructor the source did not write but its language runs one for (one that must hold the moved initialisers).
    virtual bool ctor_needed(Context&, ir::ClassDecl&, ir::MethodDecl&) { return false; }
    // Statements to run after a constructor's base call.
    virtual std::vector<ir::Node*> ctor_prologue(Context&, ir::ClassDecl&, ir::MethodDecl&, ir::CtorCall*) { return {}; }

    // ---- statements and expressions: an answer is the C# text, or nothing for "as written"
    virtual std::optional<Ex> select(Context&, ir::Node*) { return std::nullopt; }
    virtual std::optional<Ex> call(Context&, ir::Node*, bool /*statement*/) { return std::nullopt; }
    virtual std::optional<std::string> stmt_call(Context&, ir::Node*) { return std::nullopt; }
    virtual std::optional<std::string> concat_part(Context&, ir::Node*) { return std::nullopt; }
    virtual std::optional<Ex> new_object(Context&, ir::Node*) { return std::nullopt; }
    virtual std::optional<Ex> binary(Context&, ir::Node*) { return std::nullopt; }
    virtual std::optional<std::string> coerce(Context&, ir::Node*, const ir::TypeP&) { return std::nullopt; }
    // The collection a for-each walks, where the library writes it other than as it is.
    virtual std::optional<std::string> iterable(Context&, ir::Node*) { return std::nullopt; }
    // What a local holds, as the unit declares it (a Map that is a TreeMap).
    virtual void record(Context&, const ir::SymP&, ir::Node* /*init*/) {}
    virtual void check_lambda(Context&, ir::Lambda&) {}
};

// A library for a language, made once per Program.
using LibraryFactory = std::function<std::unique_ptr<Library>()>;

struct Options {
    // The libraries by source language ("java"). A language with none is written as C# only for "csharp"; any other is
    // declined.
    std::map<std::string, LibraryFactory, std::less<>> libraries;
    // Where the runtime files (`JavaRt.cs` ...) a library asks for are. Reading one that is not there is an IoError.
    std::filesystem::path runtime_dir;
};

// The units to write, what they declare, and what came of writing them.
class Program {
public:
    // `units`: Unit nodes (anything else is a UsageError). They must outlive the Program.
    explicit Program(std::span<ir::Node* const> units, Options options = {});
    ~Program();
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    // The files: one per unit (named by its path's stem; a stem met twice gets `_2`, `_3` ...), then the runtime files used,
    // by name. Written on first use.
    const std::vector<File>& files();
    // What was declined, in the order it was met (after files()).
    const std::vector<Gap>& ledger();
    // The files into `dir` (made if need be); their paths.
    std::vector<std::filesystem::path> write(const std::filesystem::path& dir);

    // ---- what a library asks of the program
    std::span<ir::Node* const> units() const { return units_; }
    // The class declared by this qualified name in these units, or null.
    ir::ClassDecl* class_named(std::string_view qname) const;
    // The class a class extends, when it is one of these units'.
    ir::ClassDecl* base_of(const ir::ClassDecl& c) const;
    // Does method `m` of class `c` override one of a class it extends (in these units)?
    bool overrides(const ir::ClassDecl& c, const ir::MethodDecl& m) const;
    // The runtime file (`JavaRt`) the output uses.
    void use_runtime(std::string name) { runtime_.insert(std::move(name)); }
    // A field's initialiser as it is written: another expression, or none where it moved elsewhere. Absent from the map:
    // the field's own.
    void set_init(const ir::Node* field, ir::Node* init) { inits_[field] = init; }
    bool has_init_override(const ir::Node* field) const { return inits_.contains(field); }
    ir::Node* init_override(const ir::Node* field) const { return inits_.at(field); }
    // Nodes a library makes (statements moved into a constructor) live here, as long as the program.
    Arena& arena() { return arena_; }
    void note(Gap gap) { ledger_.push_back(std::move(gap)); }
    const Options& options() const { return options_; }

private:
    friend class Writer;
    void index(ir::Node* class_node);
    Library* library_for(const std::string& lang);

    std::vector<ir::Node*> units_;
    Options options_;
    std::unordered_map<std::string, ir::ClassDecl*> classes_;   // lookup only
    std::vector<Gap> ledger_;
    std::set<std::string> runtime_;
    std::unordered_map<const ir::Node*, ir::Node*> inits_;       // lookup only
    std::map<std::string, std::unique_ptr<Library>, std::less<>> libraries_;
    std::optional<std::vector<File>> files_;
    Arena arena_;
};

}  // namespace cb::translate::backends::csharp
