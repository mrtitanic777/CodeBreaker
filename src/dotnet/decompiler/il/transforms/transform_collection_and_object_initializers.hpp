// dotnet/decompiler/il/transforms/transform_collection_and_object_initializers.hpp -- recognizes the statements that fill a freshly made
// object (calls of its setters and Add methods, stores to its fields, indexer stores) and puts them in one initializer block, which
// becomes `new C { A = 1, B = { 2, 3 } }`. AccessPathElement describes what such a statement reaches: the chain of members from the
// object to the one the statement sets or adds to.
//
//     stloc o(newobj C())                       stloc o(Block CollectionInitializer {
//     callvirt Add(ldloc o, 1)            =>        stloc t(newobj C()); callvirt Add(ldloc t, 1); ...; final: ldloc t })
//     callvirt Add(ldloc o, 2)
//
//     TransformCollectionAndObjectInitializers transform;
//     transform.run(block, pos, context);
//
//     auto info = AccessPathElement::get_access_path(inst, root_type);      // info.kind: Invalid, Setter or Adder
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: TransformCollectionAndObjectInitializers, AccessPathKind, AccessPathElement,
// ILInstructionMatchComparer, from TransformCollectionAndObjectInitializers.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler {
class DecompilerSettings;
}
namespace cb::dotnet::decompiler::csharp::resolver {
class CSharpResolver;
}

namespace cb::dotnet::decompiler::il::transforms {

enum class AccessPathKind : std::uint8_t { Invalid, Setter, Adder };

// One step of the path from the initialized object to a member: the instruction that reaches it (ldflda, callvirt, ldobj ...), the member,
// and for an indexer the index arguments.
struct AccessPathElement {
    AccessPathElement(OpCode op_code, const IMember* member, std::optional<std::vector<ILInstruction*>> indices = std::nullopt)
        : op_code(op_code), member(member), indices(std::move(indices)) {}

    OpCode op_code;
    const IMember* member;
    std::optional<std::vector<ILInstruction*>> indices;

    // What a statement reaches and sets or adds: the kind, the path of members, the values stored or added, the variable the path starts
    // at, and the variables the indices use.
    struct AccessPath {
        AccessPathKind kind = AccessPathKind::Invalid;
        std::vector<AccessPathElement> path;
        std::optional<std::vector<ILInstruction*>> values;
        ILVariable* target = nullptr;
        std::vector<ILVariable*> used_index_variables;
    };

    static AccessPath get_access_path(ILInstruction* instruction, const IType* root_type, const DecompilerSettings* settings = nullptr,
                                      csharp::resolver::CSharpResolver* resolver = nullptr);

    std::string to_string() const;
    bool equals(const AccessPathElement& other) const;
    friend bool operator==(const AccessPathElement& lhs, const AccessPathElement& rhs) { return lhs.equals(rhs); }
    friend bool operator!=(const AccessPathElement& lhs, const AccessPathElement& rhs) { return !lhs.equals(rhs); }
};

// Transforms collection and object initialization patterns.
class TransformCollectionAndObjectInitializers final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "TransformCollectionAndObjectInitializers"; }

    // `call <Clone>$(ldloc record)`: the compiler's copy of a record, the start of a `with` expression.
    static bool is_record_clone_method_call(CallInstruction* ci);

private:
    // The index variable stores seen so far: the variable, where its store is in the block (-1 when an initializer item used it) and
    // its value.
    struct IndexVariable {
        ILVariable* variable;
        int index;
        ILInstruction* value;
    };

    static bool type_contains_init_only_properties(const typesystem::ITypeDefinition* type_definition);
    bool is_method_call_on_variable(ILInstruction* inst, ILVariable* variable);
    bool is_part_of_initializer(InstructionCollection<ILInstruction>& instructions, int pos, ILVariable* target, const IType* root_type,
                                BlockKind* block_kind, bool* initializer_contains_init_only_items, StatementTransformContext& context);
    bool is_valid_object_initializer_target(const std::vector<AccessPathElement>& path);
    IndexVariable* find_possible_index_variable(ILVariable* variable);
    // Adds `element` to the set on top of the path stack; false when it is already in it.
    bool add_to_top_of_path_stack(const AccessPathElement& element);

    std::vector<IndexVariable> possible_index_variables_;
    std::vector<AccessPathElement> current_path_;
    bool is_collection_ = false;
    // The elements of each level of the path seen so far (upstream's Stack of HashSets; only membership and the count are used).
    std::vector<std::vector<AccessPathElement>> path_stack_;
};

}  // namespace cb::dotnet::decompiler::il::transforms
