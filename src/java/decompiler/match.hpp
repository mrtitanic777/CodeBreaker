// java/decompiler/match.hpp -- the small pattern language the decompiler matches statement/expression trees with (the 1.4-style `Class.forName` inlining
// uses it): a description of nested statement and expression nodes with properties (type, position, size, constant value, invoked class and signature,
// variable index, field name ...), `$name$` values that bind to what they meet, and the engine that parses a description and matches an object against it.
//
//     MatchEngine engine;
//     engine.parse("statement type:if iftype:if\n exprent position:head type:if\n  exprent type:field name:$field$");
//     if (engine.match(stat)) { const MatchValue v = engine.get_variable_value("$field$"); }   // stat: a Statement* (an IMatchable)
//
// Ported from Fernflower (de.fernflower.struct.match: IMatchable, MatchNode, MatchEngine), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see
// NOTICE in this folder). Statement and Exprent derive from IMatchable (the original's `implements IMatchable`) and override find_object, match and (for
// the expressions, whose equals the original uses to compare two bound values) equals_matchable. Match nodes are plain objects owned by their parent, not
// arena objects: the original builds its descriptions in a static initializer, before any run (and so any arena) exists. The original's Object-typed
// values (an Integer, a String, a VarType, a constant's value, an expression or statement) are one MatchValue; its null is std::monostate.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

class MatchNode;
class MatchEngine;
class IMatchable;

// What a rule or a bound variable holds: null, an Integer, a Long, a Float, a Double, a String, a VarType, or an object being matched (a statement or an
// expression: held by identity, compared by IMatchable::equals_matchable).
using MatchValue = std::variant<std::monostate, std::int32_t, std::int64_t, float, double, std::string, VarType, IMatchable*>;

// Java's equals between two Objects of MatchValue (the same alternative and equal contents; objects by equals_matchable).
bool equal_match_values(const MatchValue& a, const MatchValue& b);
// Java's toString of the value; std::runtime_error for null (the original's NullPointerException).
std::string match_value_string(const MatchValue& v);

class IMatchable {
public:
    enum class MatchProperties {
        STATEMENT_TYPE,
        STATEMENT_RET,
        STATEMENT_STATSIZE,
        STATEMENT_EXPRSIZE,
        STATEMENT_POSITION,
        STATEMENT_IFTYPE,

        EXPRENT_TYPE,
        EXPRENT_RET,
        EXPRENT_POSITION,
        EXPRENT_FUNCTYPE,
        EXPRENT_EXITTYPE,
        EXPRENT_CONSTTYPE,
        EXPRENT_CONSTVALUE,
        EXPRENT_INVOCATION_CLASS,
        EXPRENT_INVOCATION_SIGNATURE,
        EXPRENT_INVOCATION_PARAMETER,
        EXPRENT_VAR_INDEX,
        EXPRENT_FIELD_NAME,
    };

    virtual ~IMatchable() = default;

    // The child object of the kind `match_node` describes (a statement or an expression), the `index`-th of its kind; null when there is none.
    virtual IMatchable* find_object(const MatchNode* match_node, int index) = 0;
    // This object has the properties `match_node` asks for (binding the `$variables$` in `engine`).
    virtual bool match(const MatchNode* match_node, MatchEngine& engine) = 0;
    // The original's Object.equals as the engine uses it on two bound values: identity here; the expressions override it with Exprent::equals.
    virtual bool equals_matchable(const IMatchable* other) const { return this == other; }
};

using MatchProperties = IMatchable::MatchProperties;

class MatchNode {
public:
    // A rule's value (and, for an invocation's `parameter:N:$x$`, which parameter it is for).
    struct RuleValue {
        int parameter = 0;
        MatchValue value;

        RuleValue(int parameter, MatchValue value) : parameter(parameter), value(std::move(value)) {}
        // The value's text starts and ends with '$' (std::runtime_error for a null value, as the original's NullPointerException).
        bool is_variable() const;
        std::string to_string() const { return match_value_string(value); }
    };

    static constexpr int MATCHNODE_STATEMENT = 0;
    static constexpr int MATCHNODE_EXPRENT = 1;

    explicit MatchNode(int type) : type_(type) {}

    void add_child(std::unique_ptr<MatchNode> child) { children_.push_back(std::move(child)); }
    // A rule put again keeps its place (the reference build's LinkedHashMap).
    void add_rule(MatchProperties property, RuleValue value) {
        for (auto& r : rules_) {
            if (r.first == property) { r.second = std::move(value); return; }
        }
        rules_.emplace_back(property, std::move(value));
    }
    int get_type() const { return type_; }
    const std::vector<std::unique_ptr<MatchNode>>& get_children() const { return children_; }
    // The rules in the order they were parsed (the reference build's LinkedHashMap).
    const std::vector<std::pair<MatchProperties, RuleValue>>& get_rules() const { return rules_; }
    // The rule for the property (the original's getRules().get(property)); null when there is none.
    const RuleValue* find_rule(MatchProperties property) const {
        for (const auto& r : rules_) if (r.first == property) return &r.second;
        return nullptr;
    }
    // The rule's value; null (std::monostate) when the node has no such rule.
    MatchValue get_rule_value(MatchProperties property) const;

private:
    int type_;
    std::vector<std::pair<MatchProperties, RuleValue>> rules_;
    std::vector<std::unique_ptr<MatchNode>> children_;
};

class MatchEngine {
public:
    MatchEngine() = default;
    MatchEngine(const MatchEngine&) = delete;
    MatchEngine& operator=(const MatchEngine&) = delete;

    // Reads a description: one node per line, a child indented one space deeper than its parent; `statement` or `exprent`, then `property:value` words.
    // std::runtime_error "Unknown matching property" / "Unhandled matching property".
    void parse(const std::string& description);
    // The object matches the parsed description (clears the variables first).
    bool match(IMatchable* object);
    // Binds `name` to `value` when it has none (true), or says whether the value it has equals `value`.
    bool check_and_set_variable_value(const std::string& name, const MatchValue& value);
    // What `name` is bound to; null (std::monostate) when it is not.
    MatchValue get_variable_value(const std::string& name) const;

private:
    bool match(const MatchNode* match_node, IMatchable* object);

    // (The original's static name -> property/type tables are built once in match.cpp.)
    std::unique_ptr<MatchNode> root_node_;
    std::map<std::string, MatchValue> variables_;
};

}  // namespace cb::java::decompiler
