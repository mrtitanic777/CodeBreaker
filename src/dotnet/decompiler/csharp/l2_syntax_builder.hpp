// dotnet/decompiler/csharp/l2_syntax_builder.hpp -- the C# back end's plain lowering (docs/JAVA_TO_CSHARP_L2.md): L2 units of any front end
// with no IL facts lowered into ILSpy's C# syntax tree, which ILSpy's output visitor writes. The CIL lane's units go through ILSpy's
// builders, which read L2 with the IL facts (l2_body.hpp); every other front end's go through this.
//
//     ir::Node* units[] = {unit};                         // Unit nodes, lang "csharp"
//     L2CSharpProgram program(units);
//     for (const L2File& f : program.files()) ...         // {"Shapes.cs", text}
//     for (const L2Gap& g : program.ledger()) ...         // what was declined, and why
//
// Each L2 node becomes the syntax node C# writes it with; the decisions the old writer (translate/backends/csharp) made from the tree are
// made the same way here (a name an inner block declares before an outer one is renumbered, a constant that overflows is `unchecked`, an
// object initialiser the front end hoisted is put back, a section that falls through ends in `goto case`). The text -- parentheses,
// spacing, layout, literal spelling, `@` before a keyword -- is the output visitor's alone (InsertParenthesesVisitor, the Allman options
// with four spaces), the same visitor that writes ilspycmd's C#.
//
// What it cannot write faithfully it declines, never guesses: a method body holding something it does not carry becomes
// `throw new System.NotSupportedException("CB-GAP: ...")` with a ledger line; a class it cannot carry is a `// CB-GAP:` comment.
// Units of a language that needs a library (Java: the rule set java, not yet ported) are declined class by class.
//
// Not upstream code: ILSpy writes C# from the IL only.
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "translate/ir.hpp"

namespace cb::dotnet::decompiler::csharp {

namespace ir = cb::translate::ir;

// One thing declined: where (class and method), what kind of construct, why.
struct L2Gap {
    std::string where;
    std::string kind;
    std::string why;
    friend bool operator==(const L2Gap&, const L2Gap&) = default;
};

// One file of the program: its name and its text (lines end in "\n").
struct L2File {
    std::string name;
    std::string text;
};

// The units to write, what they declare, and what came of writing them.
class L2CSharpProgram {
public:
    // `units`: Unit nodes (anything else is a UsageError). They must outlive the program.
    explicit L2CSharpProgram(std::span<ir::Node* const> units);
    ~L2CSharpProgram();
    L2CSharpProgram(const L2CSharpProgram&) = delete;
    L2CSharpProgram& operator=(const L2CSharpProgram&) = delete;

    // One file per unit, named by its path's stem (a stem met twice gets `_2`, `_3` ...). Written on first use.
    const std::vector<L2File>& files();
    // What was declined, in the order it was met (after files()).
    const std::vector<L2Gap>& ledger();
    // The files into `dir` (made if need be); their paths.
    std::vector<std::filesystem::path> write(const std::filesystem::path& dir);

    // The class declared by this qualified name in these units, or null.
    ir::ClassDecl* class_named(std::string_view qname) const;
    // The class a class extends, when it is one of these units'.
    ir::ClassDecl* base_of(const ir::ClassDecl& c) const;
    void note(L2Gap gap) { ledger_.push_back(std::move(gap)); }

private:
    void index(ir::Node* class_node);

    std::vector<ir::Node*> units_;
    std::unordered_map<std::string, ir::ClassDecl*> classes_;   // lookup only
    std::vector<L2Gap> ledger_;
    std::optional<std::vector<L2File>> files_;
};

}  // namespace cb::dotnet::decompiler::csharp
