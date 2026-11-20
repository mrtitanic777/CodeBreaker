// IMatchable, MatchNode and MatchEngine: ported from Fernflower (de.fernflower.struct.match), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see
// NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/match.hpp"

#include <cstring>
#include <stdexcept>

#include "java/decompiler/exprents.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler {

namespace {

template <class F>
bool same_bits(F a, F b) {
    // Float.equals / Double.equals compare the bit patterns (NaN equals NaN, 0.0 does not equal -0.0).
    return std::memcmp(&a, &b, sizeof(F)) == 0;
}

// String.split(regex) for a single-character separator, or for runs of whitespace: leading empty string kept, trailing empty strings dropped.
std::vector<std::string> split_on(const std::string& s, char sep) {
    std::vector<std::string> out;
    std::size_t start = 0;
    while (true) {
        const std::size_t pos = s.find(sep, start);
        if (pos == std::string::npos) {
            out.push_back(s.substr(start));
            break;
        }
        out.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    while (!out.empty() && out.back().empty()) out.pop_back();
    if (out.empty() && s.empty()) out.push_back("");   // "".split(..) is [""]
    return out;
}

bool is_space(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }

// line.split("\s+"): a leading run of whitespace gives a leading empty string; trailing empty strings are dropped.
std::vector<std::string> split_whitespace(const std::string& s) {
    std::vector<std::string> out;
    std::size_t i = 0;
    if (!s.empty() && is_space(s[0])) out.push_back("");
    while (i < s.size()) {
        while (i < s.size() && is_space(s[i])) ++i;
        std::size_t j = i;
        while (j < s.size() && !is_space(s[j])) ++j;
        if (j > i) out.push_back(s.substr(i, j - i));
        i = j;
    }
    while (!out.empty() && out.back().empty()) out.pop_back();
    return out;
}

struct Tables {
    std::map<std::string, MatchProperties> stat_properties, expr_properties;
    std::map<std::string, int> stat_type, expr_type, expr_func_type, expr_exit_type, stat_if_type;
    std::map<std::string, VarType> expr_const_type;

    Tables() {
        stat_properties["type"] = MatchProperties::STATEMENT_TYPE;
        stat_properties["ret"] = MatchProperties::STATEMENT_RET;
        stat_properties["position"] = MatchProperties::STATEMENT_POSITION;
        stat_properties["statsize"] = MatchProperties::STATEMENT_STATSIZE;
        stat_properties["exprsize"] = MatchProperties::STATEMENT_EXPRSIZE;
        stat_properties["iftype"] = MatchProperties::STATEMENT_IFTYPE;

        expr_properties["type"] = MatchProperties::EXPRENT_TYPE;
        expr_properties["ret"] = MatchProperties::EXPRENT_RET;
        expr_properties["position"] = MatchProperties::EXPRENT_POSITION;
        expr_properties["functype"] = MatchProperties::EXPRENT_FUNCTYPE;
        expr_properties["exittype"] = MatchProperties::EXPRENT_EXITTYPE;
        expr_properties["consttype"] = MatchProperties::EXPRENT_CONSTTYPE;
        expr_properties["constvalue"] = MatchProperties::EXPRENT_CONSTVALUE;
        expr_properties["invclass"] = MatchProperties::EXPRENT_INVOCATION_CLASS;
        expr_properties["signature"] = MatchProperties::EXPRENT_INVOCATION_SIGNATURE;
        expr_properties["parameter"] = MatchProperties::EXPRENT_INVOCATION_PARAMETER;
        expr_properties["index"] = MatchProperties::EXPRENT_VAR_INDEX;
        expr_properties["name"] = MatchProperties::EXPRENT_FIELD_NAME;

        stat_type["if"] = Statement::TYPE_IF;
        stat_type["do"] = Statement::TYPE_DO;
        stat_type["switch"] = Statement::TYPE_SWITCH;
        stat_type["trycatch"] = Statement::TYPE_TRYCATCH;
        stat_type["basicblock"] = Statement::TYPE_BASICBLOCK;
        stat_type["sequence"] = Statement::TYPE_SEQUENCE;

        expr_type["array"] = Exprent::EXPRENT_ARRAY;
        expr_type["assignment"] = Exprent::EXPRENT_ASSIGNMENT;
        expr_type["constant"] = Exprent::EXPRENT_CONST;
        expr_type["exit"] = Exprent::EXPRENT_EXIT;
        expr_type["field"] = Exprent::EXPRENT_FIELD;
        expr_type["function"] = Exprent::EXPRENT_FUNCTION;
        expr_type["if"] = Exprent::EXPRENT_IF;
        expr_type["invocation"] = Exprent::EXPRENT_INVOCATION;
        expr_type["monitor"] = Exprent::EXPRENT_MONITOR;
        expr_type["new"] = Exprent::EXPRENT_NEW;
        expr_type["switch"] = Exprent::EXPRENT_SWITCH;
        expr_type["var"] = Exprent::EXPRENT_VAR;
        expr_type["annotation"] = Exprent::EXPRENT_ANNOTATION;
        expr_type["assert"] = Exprent::EXPRENT_ASSERT;

        expr_func_type["eq"] = FunctionExprent::FUNCTION_EQ;

        expr_exit_type["return"] = ExitExprent::EXIT_RETURN;
        expr_exit_type["throw"] = ExitExprent::EXIT_THROW;

        stat_if_type["if"] = IfStatement::IFTYPE_IF;
        stat_if_type["ifelse"] = IfStatement::IFTYPE_IFELSE;

        expr_const_type["null"] = VarType::NULL_TYPE;
        expr_const_type["string"] = VarType::STRING;
    }
};

const Tables& tables() {
    static const Tables t;
    return t;
}

MatchValue lookup_int(const std::map<std::string, int>& m, const std::string& key) {
    const auto it = m.find(key);
    return it == m.end() ? MatchValue{} : MatchValue{static_cast<std::int32_t>(it->second)};
}

}  // namespace

bool equal_match_values(const MatchValue& a, const MatchValue& b) {
    if (a.index() != b.index()) return false;
    struct V {
        const MatchValue& other;
        bool operator()(std::monostate) const { return false; }
        bool operator()(std::int32_t x) const { return x == std::get<std::int32_t>(other); }
        bool operator()(std::int64_t x) const { return x == std::get<std::int64_t>(other); }
        bool operator()(float x) const { return same_bits(x, std::get<float>(other)); }
        bool operator()(double x) const { return same_bits(x, std::get<double>(other)); }
        bool operator()(const std::string& x) const { return x == std::get<std::string>(other); }
        bool operator()(const VarType& x) const { return x == std::get<VarType>(other); }
        bool operator()(IMatchable* x) const { return x->equals_matchable(std::get<IMatchable*>(other)); }
    };
    return std::visit(V{b}, a);
}

std::string match_value_string(const MatchValue& v) {
    struct V {
        std::string operator()(std::monostate) const { throw std::runtime_error("NullPointerException"); }
        std::string operator()(std::int32_t x) const { return std::to_string(x); }
        std::string operator()(std::int64_t x) const { return std::to_string(x); }
        std::string operator()(float x) const { return std::to_string(x); }
        std::string operator()(double x) const { return std::to_string(x); }
        std::string operator()(const std::string& x) const { return x; }
        std::string operator()(const VarType& x) const { return x.to_string(); }
        std::string operator()(IMatchable*) const { return ""; }
    };
    return std::visit(V{}, v);
}

bool MatchNode::RuleValue::is_variable() const {
    const std::string str_value = match_value_string(value);
    if (str_value.empty()) throw std::out_of_range("StringIndexOutOfBoundsException");
    return str_value.front() == '$' && str_value.back() == '$';
}

MatchValue MatchNode::get_rule_value(MatchProperties property) const {
    const RuleValue* rule = find_rule(property);
    return rule == nullptr ? MatchValue{} : rule->value;
}

void MatchEngine::parse(const std::string& description) {
    const Tables& t = tables();

    // each line is a separate statement/exprent
    const std::vector<std::string> lines = split_on(description, '\n');

    int depth = 0;
    std::vector<MatchNode*> stack;   // back: the top (the original's LinkedList used as a stack: first = top)
    std::unique_ptr<MatchNode> root;

    for (const std::string& line : lines) {
        std::vector<std::string> properties = split_whitespace(line);   // split on any number of whitespaces
        if (!properties.empty() && properties[0].empty()) {
            properties.erase(properties.begin());
        }
        if (properties.empty()) throw std::out_of_range("IndexOutOfBoundsException");

        const int node_type = properties[0] == "statement" ? MatchNode::MATCHNODE_STATEMENT : MatchNode::MATCHNODE_EXPRENT;

        // create new node
        auto match_node = std::make_unique<MatchNode>(node_type);
        for (std::size_t i = 1; i < properties.size(); ++i) {
            const std::vector<std::string> values = split_on(properties[i], ':');

            const auto& prop_map = node_type == MatchNode::MATCHNODE_STATEMENT ? t.stat_properties : t.expr_properties;
            const auto pit = prop_map.find(values.at(0));
            if (pit == prop_map.end()) {   // unknown property defined
                throw std::runtime_error("Unknown matching property");
            }
            const MatchProperties property = pit->second;

            MatchValue value;
            int parameter = 0;

            std::string str_value = values.at(1);
            if (values.size() == 3) {
                parameter = std::stoi(values[1]);
                str_value = values[2];
            }

            switch (property) {
                case MatchProperties::STATEMENT_TYPE:
                    value = lookup_int(t.stat_type, str_value);
                    break;
                case MatchProperties::STATEMENT_STATSIZE:
                case MatchProperties::STATEMENT_EXPRSIZE:
                    value = static_cast<std::int32_t>(std::stoi(str_value));
                    break;
                case MatchProperties::STATEMENT_POSITION:
                case MatchProperties::EXPRENT_POSITION:
                case MatchProperties::EXPRENT_INVOCATION_CLASS:
                case MatchProperties::EXPRENT_INVOCATION_SIGNATURE:
                case MatchProperties::EXPRENT_INVOCATION_PARAMETER:
                case MatchProperties::EXPRENT_VAR_INDEX:
                case MatchProperties::EXPRENT_FIELD_NAME:
                case MatchProperties::EXPRENT_CONSTVALUE:
                case MatchProperties::STATEMENT_RET:
                case MatchProperties::EXPRENT_RET:
                    value = str_value;
                    break;
                case MatchProperties::STATEMENT_IFTYPE:
                    value = lookup_int(t.stat_if_type, str_value);
                    break;
                case MatchProperties::EXPRENT_FUNCTYPE:
                    value = lookup_int(t.expr_func_type, str_value);
                    break;
                case MatchProperties::EXPRENT_EXITTYPE:
                    value = lookup_int(t.expr_exit_type, str_value);
                    break;
                case MatchProperties::EXPRENT_CONSTTYPE: {
                    const auto cit = t.expr_const_type.find(str_value);
                    if (cit != t.expr_const_type.end()) value = cit->second;
                    break;
                }
                case MatchProperties::EXPRENT_TYPE:
                    value = lookup_int(t.expr_type, str_value);
                    break;
                default:
                    throw std::runtime_error("Unhandled matching property");
            }

            match_node->add_rule(property, MatchNode::RuleValue(parameter, value));
        }

        MatchNode* raw = match_node.get();
        if (stack.empty()) {   // first line, root node
            root = std::move(match_node);
            stack.push_back(raw);
        } else {
            // return to the correct parent on the stack
            const std::size_t from = static_cast<std::size_t>(depth) < line.size() ? static_cast<std::size_t>(depth) : std::string::npos;
            const std::size_t last_space = line.rfind(' ', from);   // String.lastIndexOf(' ', depth)
            const int new_depth = (last_space == std::string::npos ? -1 : static_cast<int>(last_space)) + 1;
            for (int i = new_depth; i <= depth; ++i) {
                stack.pop_back();
            }

            // insert new node
            stack.back()->add_child(std::move(match_node));
            stack.push_back(raw);

            depth = new_depth;
        }
    }

    root_node_ = std::move(root);
}

bool MatchEngine::match(IMatchable* object) {
    variables_.clear();
    return match(root_node_.get(), object);
}

bool MatchEngine::match(const MatchNode* match_node, IMatchable* object) {
    if (!object->match(match_node, *this)) {
        return false;
    }

    int expr_index = 0;
    int stat_index = 0;

    for (const std::unique_ptr<MatchNode>& child_node : match_node->get_children()) {
        const bool is_statement = child_node->get_type() == MatchNode::MATCHNODE_STATEMENT;

        IMatchable* child_object = object->find_object(child_node.get(), is_statement ? stat_index : expr_index);
        if (child_object == nullptr || !match(child_node.get(), child_object)) {
            return false;
        }

        if (is_statement) {
            stat_index++;
        } else {
            expr_index++;
        }
    }

    return true;
}

bool MatchEngine::check_and_set_variable_value(const std::string& name, const MatchValue& value) {
    const auto it = variables_.find(name);
    if (it == variables_.end() || std::holds_alternative<std::monostate>(it->second)) {
        variables_[name] = value;
    } else if (!equal_match_values(it->second, value)) {
        return false;
    }

    return true;
}

MatchValue MatchEngine::get_variable_value(const std::string& name) const {
    const auto it = variables_.find(name);
    return it == variables_.end() ? MatchValue{} : it->second;
}

}  // namespace cb::java::decompiler
