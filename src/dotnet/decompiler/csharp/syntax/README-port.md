# Writing a node class on the syntax core (csharp/syntax)

A node is `final : public Expression/Statement/AstType/EntityDeclaration/AstNode`, made with `make<X>(...)` (`using il::make` is in ast_node.hpp), with the generated members of upstream's source generator written by hand, from `work/ilspy-port/nuget-decompiled/.../<Node>.cs`:
- **kind**: `static constexpr AstNodeKind kind = AstNodeKind::X;` plus `node_kind()`; the value, its placeholder and `kind_of<X>` already exist in ast_node_kind.hpp. `as<X>(n)` / `is<X>(n)` work from it.
- **slots**: one `static constexpr CSharpSlotInfoT<Child> x_slot{"X", is_collection, &Slots::kind, is_optional}` per slot (the generated `XSlot`), in declaration order, and `get_child_count / get_child_by_index / set_child_by_index / get_child_slot_info` (a single slot is one index even when empty, a collection a run) + `get_collection_by_kind` for collections. A single child is a `ChildPtr<T>` field written with `set_child_node(field, v, index)` (index = its fixed position; omit it for a slot after a collection); read-only `T* x() const`. A string slot (`[Slot] string Name`) is a getter/setter over an Identifier child (`Identifier::create`, `create_if_not_empty` for optional names).
- **collections**: `mutable AstNodeCollection<T> xs_{this, Slots::kind, first_index, incremental};` (incremental = the node's only collection and its last slot), `AstNodeCollection<T>& xs() const`. Use the untyped `nodes()` / `set_node` for a T that is not ported yet (see composed_type.cpp).
- **visitor**: `void dispatch_to(IAstVisitor& v) override { v.visit_x(*this); }` (protected); the visit_ names are in ast_visitor.hpp; the typed visitors work through AstNode::accept_visitor.
- **clone**: `AstNode* shallow_clone() const override { return make<X>(*this); }` (protected) needs no copy constructor: the implicit one copies the scalars, `ChildPtr` fields and collections copy as empty. `clone_children_into(copy)` sets each child to `clone_as(child.get())` and calls `copy->xs_.clone_from(copy, xs_)` per collection, in slot order.
- **do_match**: `as<X>(other)`, then each child with `match_optional` (optional slot) or `match_required`, collections with `xs_.do_match(o->xs_, match)`, strings with `match_string`, scalars with `==` (a `[ExcludeFromMatch]` one is left out).
- **no Null objects** in ILSpy 11: an absent child is nullptr. A class with `hasPatternPlaceholder` adds `static X* from_pattern(Pattern*)` (pattern_placeholder.hpp). Override `to_string` / `check_invariant` where upstream does; `check_invariant` calls `AstNode::check_invariant()` first.

```cpp
class ParenthesizedExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ParenthesizedExpression;
    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    ParenthesizedExpression() = default;
    explicit ParenthesizedExpression(Expression* expression) { set_expression(expression); }

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value) { set_child_node(expression_, value, 0); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override {
        auto* o = as<ParenthesizedExpression>(other);
        return o != nullptr && match_required(expression_, o->expression_, match);
    }

    int get_child_count() const override { return 1; }
    AstNode* get_child_by_index(int index) const override {
        if (index == 0) return expression_;
        throw std::out_of_range("index");
    }
    void set_child_by_index(int index, AstNode* value) override {
        if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
        throw std::out_of_range("index");
    }
    const CSharpSlotInfo& get_child_slot_info(int index) const override {
        if (index == 0) return expression_slot;
        throw std::out_of_range("index");
    }
    void clone_children_into(AstNode* copy_node) const override {
        auto* copy = static_cast<ParenthesizedExpression*>(copy_node);
        if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
    }

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_parenthesized_expression(*this); }
    AstNode* shallow_clone() const override { return make<ParenthesizedExpression>(*this); }

private:
    ChildPtr<Expression> expression_;
};
```

Complete examples with a collection and a scalar: simple_type.hpp/.cxx (identifier + type arguments), composed_type.hpp/.cxx (two collections, scalars, to_string, check_invariant). Slot kinds that do not exist in slots.hpp (`Slots::`) must be added there with their child type; operator_declaration.hpp must define `OperatorType` as `std::uint8_t` (documentation_reference.hpp declares it opaque until the header exists), and syntax_extensions.hpp / documentation_reference.cpp / pattern_matching/identifier_expression_backreference.cpp start defining their parts as soon as operator_declaration.hpp, direction_expression.hpp, identifier_expression.hpp (`type_arguments()`, `identifier()`), attribute.hpp, assignment_expression.hpp, attribute_section.hpp and parameter_declaration.hpp exist.
