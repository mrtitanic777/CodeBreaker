// dotnet/decompiler/csharp/output_visitor/error_tolerant_output_visitor.cpp -- ErrorTolerantOutputVisitor; see
// error_tolerant_output_visitor.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: ErrorTolerantOutputVisitor), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/error_tolerant_output_visitor.hpp"

#include <ios>

#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

using namespace syntax;

void ErrorTolerantOutputVisitor::open_brace(BraceStyle style, bool new_line_after) {
    CSharpOutputVisitor::open_brace(style, new_line_after);
    brace_depth_++;
}

void ErrorTolerantOutputVisitor::close_brace(BraceStyle style, bool unindent) {
    CSharpOutputVisitor::close_brace(style, unindent);
    brace_depth_--;
}

template <class Node>
void ErrorTolerantOutputVisitor::write(Node& node, const std::function<void(Node&)>& write_node) {
    int braces = brace_depth_;
    std::size_t containers = container_stack_.size();
    try {
        write_node(node);
    } catch (const util::OperationCanceledError&) {
        throw;
    } catch (const std::ios_base::failure&) {
        throw;
    } catch (const std::exception& ex) {
        errors_.push_back(std::current_exception());
        while (container_stack_.size() > containers) {
            AstNode* top = container_stack_.back();
            container_stack_.pop_back();
            writer_.end_node(*top);
        }
        while (brace_depth_ > braces) {
            close_brace(BraceStyle::NextLine);
        }
        new_line();
        for (const std::string& line : CSharpDecompiler::get_error_comment_lines(ex)) {
            writer_.write_comment(CommentType::SingleLine, " " + line);
        }
    }
}

#define CB_ETOV_VISIT(Class, name)                                                          \
    void ErrorTolerantOutputVisitor::visit_##name(Class& node) {                            \
        write<Class>(node, [this](Class& n) { CSharpOutputVisitor::visit_##name(n); });      \
    }

CB_ETOV_VISIT(TypeDeclaration, type_declaration)
CB_ETOV_VISIT(DelegateDeclaration, delegate_declaration)
CB_ETOV_VISIT(ConstructorDeclaration, constructor_declaration)
CB_ETOV_VISIT(DestructorDeclaration, destructor_declaration)
CB_ETOV_VISIT(EnumMemberDeclaration, enum_member_declaration)
CB_ETOV_VISIT(ExtensionDeclaration, extension_declaration)
CB_ETOV_VISIT(EventDeclaration, event_declaration)
CB_ETOV_VISIT(CustomEventDeclaration, custom_event_declaration)
CB_ETOV_VISIT(FieldDeclaration, field_declaration)
CB_ETOV_VISIT(FixedFieldDeclaration, fixed_field_declaration)
CB_ETOV_VISIT(IndexerDeclaration, indexer_declaration)
CB_ETOV_VISIT(MethodDeclaration, method_declaration)
CB_ETOV_VISIT(OperatorDeclaration, operator_declaration)
CB_ETOV_VISIT(PropertyDeclaration, property_declaration)

#undef CB_ETOV_VISIT

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
