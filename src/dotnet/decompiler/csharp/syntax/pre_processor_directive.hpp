// dotnet/decompiler/csharp/syntax/pre_processor_directive.hpp -- PreProcessorDirective (and the `#line` and `#pragma warning` forms): a
// `#region`, `#if`, `#pragma` ... line attached as trivia.
//
//     auto* d = make<PreProcessorDirective>(PreProcessorDirectiveType::Region, "Fields");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: PreProcessorDirective, LinePreprocessorDirective,
// PragmaWarningPreprocessorDirective, from GeneralScope/PreProcessorDirective.cs with the members ILSpy 11's source generator adds),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/pre_processor_directive.hpp"
#include "dotnet/decompiler/csharp/syntax/trivia.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// Which directive a PreProcessorDirective is.
enum class PreProcessorDirectiveType : std::uint8_t {
    Invalid = 0,
    Region = 1,
    Endregion = 2,
    If = 3,
    Endif = 4,
    Elif = 5,
    Else = 6,
    Define = 7,
    Undef = 8,
    Error = 9,
    Warning = 10,
    Pragma = 11,
    Line = 12,
};

class PreProcessorDirective : public Trivia {
public:
    static constexpr AstNodeKind kind = AstNodeKind::PreProcessorDirective;
    PreProcessorDirective(PreProcessorDirectiveType type_value, TextLocation start_location, TextLocation end_location);
    PreProcessorDirective(PreProcessorDirectiveType type_value, const std::string& argument_value = {});

    PreProcessorDirectiveType type() const { return type_; }
    void set_type(PreProcessorDirectiveType value) { type_ = value; }
    const std::string& argument() const { return argument_; }
    void set_argument(std::string value) { argument_ = std::move(value); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_pre_processor_directive(*this); }
    AstNode* shallow_clone() const override { return make<PreProcessorDirective>(*this); }

private:
    PreProcessorDirectiveType type_{};
    std::string argument_{};
};

class LinePreprocessorDirective final : public PreProcessorDirective {
public:
    static constexpr AstNodeKind kind = AstNodeKind::LinePreprocessorDirective;
    LinePreprocessorDirective(TextLocation start_location, TextLocation end_location);
    explicit LinePreprocessorDirective(const std::string& argument = {});


    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    AstNode* shallow_clone() const override { return make<LinePreprocessorDirective>(*this); }

private:
};

class PragmaWarningPreprocessorDirective final : public PreProcessorDirective {
public:
    static constexpr AstNodeKind kind = AstNodeKind::PragmaWarningPreprocessorDirective;
    static constexpr CSharpSlotInfoT<PrimitiveExpression> warnings_slot{"Warnings", true, &Slots::warning, true};

    PragmaWarningPreprocessorDirective(TextLocation start_location, TextLocation end_location);
    explicit PragmaWarningPreprocessorDirective(const std::string& argument = {});

    AstNodeCollection<PrimitiveExpression>& warnings() const { return warnings_; }

    // Ends where its last warning number ends.
    TextLocation end_location() const override;

    // Whether warning `pragma_warning` is one of the numbers.
    bool is_defined(int pragma_warning) const;

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    AstNode* shallow_clone() const override { return make<PragmaWarningPreprocessorDirective>(*this); }

private:
    mutable AstNodeCollection<PrimitiveExpression> warnings_{this, Slots::warning, 0, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
