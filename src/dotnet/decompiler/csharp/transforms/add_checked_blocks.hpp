// dotnet/decompiler/csharp/transforms/add_checked_blocks.hpp -- AddCheckedBlocks: adds checked/unchecked blocks and expressions where the
// arithmetic the ExpressionBuilder marked needs a different overflow context than the surrounding code has. Placing them is an
// optimization problem: as few blocks and expressions as possible, expressions preferred over blocks, expressions as small as possible,
// and blocks opened and closed as late as possible.
//
//     expression->add_annotation(AddCheckedBlocks::checked_annotation());       // this arithmetic must be checked
//     AddCheckedBlocks transform;
//     transform.run(root_node, context);
//
// The three annotations are shared objects that live for the whole process, as upstream's static readonly ones.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: AddCheckedBlocks, from AddCheckedBlocks.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <memory>

#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

// Add checked/unchecked blocks.
class AddCheckedBlocks final : public IAstTransform {
public:
    // Marks an expression that must be evaluated in a checked or unchecked context (the annotation is only ever looked up by its class).
    class CheckedUncheckedAnnotation final : public il::ArenaObject {
    public:
        CheckedUncheckedAnnotation(bool checked_, bool explicit_) : is_checked(checked_), is_explicit(explicit_) {}
        // true=checked, false=unchecked
        bool is_checked;
        // Require an explicit unchecked block (can't rely on the project-level unchecked context)
        bool is_explicit;
    };

    static il::ArenaObject* checked_annotation();
    static il::ArenaObject* unchecked_annotation();
    static il::ArenaObject* explicit_unchecked_annotation();

    void run(syntax::AstNode* node, TransformContext& context) override;

private:
    // The cost of a solution: the checked/unchecked blocks and expressions it adds.
    struct Cost {
        int blocks = 0;
        int expressions = 0;

        Cost() = default;
        Cost(int blocks_, int expressions_) : blocks(blocks_), expressions(expressions_) {}

        // highest possible cost so that the blocks + expressions addition doesn't overflow
        static Cost infinite() { return Cost(0x3fffffff, 0x3fffffff); }

        friend bool operator<(const Cost& a, const Cost& b) {
            return a.blocks + a.expressions < b.blocks + b.expressions ||
                   (a.blocks + a.expressions == b.blocks + b.expressions && a.blocks < b.blocks);
        }
        friend bool operator>(const Cost& a, const Cost& b) { return b < a; }
        friend bool operator<=(const Cost& a, const Cost& b) {
            return a.blocks + a.expressions < b.blocks + b.expressions ||
                   (a.blocks + a.expressions == b.blocks + b.expressions && a.blocks <= b.blocks);
        }
        friend bool operator>=(const Cost& a, const Cost& b) { return b <= a; }
        friend Cost operator+(const Cost& a, const Cost& b) { return Cost(a.blocks + b.blocks, a.expressions + b.expressions); }
        Cost& operator+=(const Cost& other) { return *this = *this + other; }

        // Gets the new cost if an expression with this cost is wrapped in a checked/unchecked expression.
        Cost wrap_in_checked_expr() const;
    };

    // Holds the blocks and expressions that should be inserted.
    class InsertedNode {
    public:
        virtual ~InsertedNode() = default;
        virtual void insert(TransformContext& context) = 0;
    };
    using InsertedNodePtr = std::shared_ptr<InsertedNode>;
    // upstream's `a + b` on two (possibly null) InsertedNodes.
    static InsertedNodePtr combine(const InsertedNodePtr& a, const InsertedNodePtr& b);

    class InsertedNodeList;
    class InsertedExpression;
    class InsertedBlock;

    // Holds the result of an insertion operation.
    struct Result {
        Cost cost_in_checked_context;
        InsertedNodePtr nodes_to_insert_in_checked_context;
        Cost cost_in_unchecked_context;
        InsertedNodePtr nodes_to_insert_in_unchecked_context;
    };

    Result get_result_from_block(syntax::BlockStatement* block);
    Result get_result(syntax::AstNode* node);
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
