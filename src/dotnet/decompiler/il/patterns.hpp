// dotnet/decompiler/il/patterns.hpp -- pattern matching over the ILAst: a pattern is an instruction tree whose leaves may be AnyNode
// (matches anything and may capture it in a CaptureGroup); matching a pattern against a tree fills a Match with the captures.
//
//     patterns::CaptureGroup group;
//     auto* pattern = make<StLoc>(v, make<AnyNode>(&group));
//     patterns::Match m = pattern->match(inst);
//     if (m.success()) for (ILInstruction* captured : m.get(&group)) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Patterns: CaptureGroup, Match, ListMatch, AnyNode; ICSharpCode.Decompiler.IL:
// PatternInstruction, from Patterns/Match.cs, ListMatch.cs, AnyNode.cs and Instructions.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Match and ListMatch are the structs upstream has (cheap values, the default Match is a
// failed match); AnyNode and PatternInstruction are in namespace il, as upstream's OpCode.AnyNode is.
#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::il {

namespace patterns {

// Identifies what an AnyNode captured (by address: make one and keep it while matching).
class CaptureGroup {
public:
    CaptureGroup() = default;
    CaptureGroup(const CaptureGroup&) = delete;
    CaptureGroup& operator=(const CaptureGroup&) = delete;

private:
    char identity_ = 0;
};

// Data holder for the overall pattern matching operation. The default value is an unsuccessful match.
struct Match {
    bool success() const { return results_.has_value(); }
    void set_success(bool value) {
        if (value) {
            if (!results_.has_value()) results_.emplace();
        } else {
            results_.reset();
        }
    }
    explicit operator bool() const { return success(); }

    void add(const CaptureGroup* group, ILInstruction* node) {
        if (!results_.has_value()) results_.emplace();
        results_->emplace_back(group, node);
    }
    int check_point() const { return results_.has_value() ? static_cast<int>(results_->size()) : 0; }
    void restore_check_point(int check_point) {
        if (results_.has_value()) results_->resize(static_cast<std::size_t>(check_point));
    }
    // The instructions captured in the group, in the order they were captured.
    std::vector<ILInstruction*> get(const CaptureGroup* capture_group) const {
        std::vector<ILInstruction*> found;
        if (results_.has_value())
            for (const auto& pair : *results_)
                if (pair.first == capture_group) found.push_back(pair.second);
        return found;
    }

private:
    std::optional<std::vector<std::pair<const CaptureGroup*, ILInstruction*>>> results_;
};

// Data holder for a single list matching operation.
//
// Backtracking: perform_match may save backtracking save points in the ListMatch. Each save point is a stack of ints that say how to
// restore the saved state. A save point made by a perform_match call nested in others on the same list gets more state pushed by the
// outer calls as they return (match or not). When the whole list match fails but save points exist, the most recent is restored by
// calling perform_match with the restore stack set to that save point; each perform_match call must pop its state from that stack
// before it calls its child patterns.
struct ListMatch {
    // The main list matching logic. True when the patterns matched the whole list (the captures are then added to `match`); on false
    // the match is left partly updated and must be restored before reuse.
    static bool do_match(const std::vector<ILInstruction*>& patterns, const std::vector<ILInstruction*>& syntax_list, Match& match);
    template <class P, class S>
    static bool do_match(const InstructionCollection<P>& patterns, const InstructionCollection<S>& syntax_list, Match& match) {
        return do_match(std::vector<ILInstruction*>(patterns.begin(), patterns.end()),
                        std::vector<ILInstruction*>(syntax_list.begin(), syntax_list.end()), match);
    }

    // perform_match for a sequence of patterns: true when all matched against a part of the list; syntax_index then points to the next
    // node that was not part of the match. On false list_match and match are left partly updated.
    static bool perform_match_sequence(const std::vector<ILInstruction*>& patterns, ListMatch& list_match, Match& match);

    // The syntax list being matched against, and the current index in it.
    std::vector<ILInstruction*> syntax_list;
    int syntax_index = 0;

    explicit ListMatch(std::vector<ILInstruction*> syntax_list_) : syntax_list(std::move(syntax_list_)) {}

    void add_save_point(Match& match, int data);
    int get_save_point_start_marker() const { return static_cast<int>(backtracking_stack_.size()); }
    void push_to_save_points(int start_marker, int data);
    std::optional<int> pop_from_save_point();
    // Restores the list match state from the most recent save point; false when there is none.
    bool restore_save_point(Match& match);

private:
    struct SavePoint {
        int check_point;
        int syntax_index;
        std::vector<int> stack;  // the top is the back
    };
    std::vector<SavePoint> backtracking_stack_;
    std::vector<int> restore_stack_;
};

}  // namespace patterns

// Base class for pattern matching in ILAst. A pattern is never part of a real tree: it has no flags and cannot be visited.
class PatternInstruction : public ILInstruction {
public:
    StackType result_type() const override { return StackType::Unknown; }
    InstructionFlags direct_flags() const override;

protected:
    explicit PatternInstruction(OpCode op_code) : ILInstruction(op_code) {}
    PatternInstruction(const PatternInstruction&) = default;
    InstructionFlags compute_flags() const override;
};

// Matches any node (and captures it in the group, if there is one).
class AnyNode final : public PatternInstruction {
public:
    static constexpr OpCode opcode = OpCode::AnyNode;
    using ILInstruction::perform_match;

    explicit AnyNode(patterns::CaptureGroup* group = nullptr) : PatternInstruction(OpCode::AnyNode), group_(group) {}

    int get_child_count() const override { return 0; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    patterns::CaptureGroup* group_;
};

}  // namespace cb::dotnet::decompiler::il
