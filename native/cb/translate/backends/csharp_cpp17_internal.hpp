// cb/translate/backends/csharp_cpp17_internal.hpp -- what the modules of the C# -> C++17 back end share and nobody else needs.
//
// One Writer over the units of a program: it registers every class, then writes forward declarations, the declarations of the classes
// (bases first), every member's body out of line, the static state and `main`. Expressions come back as text (parenthesised where they
// nest); statements are written line by line. What has no form is thrown as Declined, caught at the member and recorded; the program is
// then not written at all.
//
// The modules: csharp_cpp17.cpp (the program, its classes and types), csharp_cpp17_stmt.cpp (statements), csharp_cpp17_expr.cpp
// (expressions and the library's calls).
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "cb/translate/backends/csharp_cpp17.hpp"

namespace cb::translate::backends::csharp_cpp17::detail {

using ir::Node;
using ir::TypeP;

// What cannot be written, with the kind of construct and why.
struct Declined {
    std::string kind, why;
};

// A C# name as a C++ one (a C++ keyword gets an underscore), and a string as C++ UTF-16 literal pieces: u"abc" u"\x00e9".
std::string ident(std::string_view name);
std::string u16_literal_text(const std::string& utf8);

// A variable by its kind and name: within one function, the symbols of a name and of its declaration may be different objects (and name
// their owner differently), and C# lets no name stand for two variables where one is in the scope of the other.
inline std::string sym_key(const ir::Sym& s) { return std::to_string(static_cast<int>(s.kind)) + ":" + s.name; }

struct PropInfo {
    bool has_get = false, has_set = false, is_auto = false, is_static = false;
    TypeP type;
};

struct ClassInfo {
    Node* node = nullptr;
    const ir::ClassDecl* decl = nullptr;
    std::string package;
    std::string cname;                     // the C++ name, without the package
    std::vector<std::string> typeparams;
    bool is_struct = false, is_interface = false, is_enum = false, is_record = false, is_delegate = false;
};

class Writer {
public:
    Writer(std::span<ir::Node* const> units, std::string main) : units_(units), main_(std::move(main)) {}
    Output run();

private:
    // ---- what the program holds
    std::span<ir::Node* const> units_;
    std::string main_;
    std::map<std::string, ClassInfo> classes_;           // by qualified name
    std::vector<std::string> order_;                     // the qualified names, in source order
    std::map<std::string, PropInfo> props_;              // "Owner.Name"
    std::vector<Gap> gaps_;
    std::string out_;
    int indent_ = 0;
    std::string package_, where_;                         // the namespace being written and the member
    std::set<std::string> member_names_;                  // of the class being written: type names they hide are qualified
    std::set<std::string> boxed_;                    // locals a lambda captures: held through a shared_ptr
    std::set<std::string> hoisted_;                    // pattern variables, declared at the top of the function
    std::vector<TypeP> returns_;                          // the result type of the function being written
    std::vector<TypeP> yields_;                           // the element type of the iterator method being written (`yield return` gives one)
    int temp_ = 0;

    // ---- the program and its classes (csharp_cpp17.cpp)
    void register_class(Node* node, const std::string& package, const std::string& outer);
    const ClassInfo* info(const std::string& qname) const;
    std::string cxx_class(const std::string& qname) const;   // the C++ name as used in package_ (qualified when a member hides it)
    std::vector<std::string> sorted_classes() const;
    void write_forward(const ClassInfo& c);
    void write_class(const ClassInfo& c);
    void write_members_in_class(const ClassInfo& c, const std::string& self);
    void write_bodies(const ClassInfo& c);
    void write_method_body(const ClassInfo& c, const ir::MethodDecl& m, const std::string& self_type);
    void write_ctor_body(const ClassInfo& c, const ir::MethodDecl& m);
    void write_field_init(const ClassInfo& c);
    void write_record_members(const ClassInfo& c);
    void write_statics(const ClassInfo& c);
    void write_enum(const ClassInfo& c);
    void write_main();
    std::string template_head(const ClassInfo& c, const std::vector<std::string>& method_tparams = {}) const;
    std::string self_type(const ClassInfo& c) const;       // Name<T, U> for a generic class
    std::string params_text(const std::vector<Node*>& params, const ir::SymP& sym);
    static std::string accessor_name(const ir::SymP& prop, const std::string& kind);

    // ---- text output
    void line(const std::string& text) { out_ += std::string(static_cast<std::size_t>(indent_) * 4, ' ') + text + "\n"; }
    void open(const std::string& text) { line(text.empty() ? "{" : text + " {"); ++indent_; }
    void close(const std::string& tail = "") { --indent_; line("}" + tail); }
    [[noreturn]] static void decline(std::string kind, std::string why) { throw Declined{std::move(kind), std::move(why)}; }

    // ---- types (csharp_cpp17.cpp)
    std::string type(const TypeP& t);
    std::string class_text(const ir::ClassType& c);        // a class's C++ name with its arguments, no Ref
    std::string type_list(const std::vector<TypeP>& args);
    bool is_value(const TypeP& t) const;
    bool is_numeric(const TypeP& t) const;
    bool is_integral(const TypeP& t) const;
    bool is_floating(const TypeP& t) const;
    bool is_string(const TypeP& t) const;
    bool is_nullable(const TypeP& t) const;
    bool is_object_like(const TypeP& t) const;
    bool is_function(const TypeP& t) const;
    bool is_user_value(const TypeP& t) const;
    const ir::ClassType* class_of(const TypeP& t) const;
    static std::string prim_name(const ir::PrimType& p);
    std::string promoted(const TypeP& a, const TypeP& b) const;
    TypeP function_result(const TypeP& t) const;
    std::optional<PropInfo> prop_of(const ir::SymP& sym) const;

    // ---- statements (csharp_cpp17_stmt.cpp)
    static std::vector<Node*> flat(const ir::Suite& suite);   // a block's statements, or the statements themselves
    void body(const ir::Suite& suite);                     // a statement or a list, as the lines of a block
    void stmt(Node* s);
    void stmts(const std::vector<Node*>& list);
    void local_decl(const ir::Local& l);
    void for_each(const ir::ForEach& f);
    void for_loop(const ir::For& f);
    void switch_stmt(const ir::Switch& s);
    void try_stmt(const ir::Try& t);
    void collect_captures(Node* root);
    void collect_hoists(Node* root, std::vector<Node*>& out) const;
    void write_hoists(Node* root);
    std::string capture(const std::function<void()>& write);   // what `write` would write, as text
    void switch_cases(Node* subject, const std::vector<Node*>& cases);
    static bool free_break(Node* n);
    std::string var_decl(const ir::SymP& sym, const TypeP& t, const std::string& init, bool has_init);

    // ---- expressions (csharp_cpp17_expr.cpp)
    std::string expr(Node* e);
    static std::string bare(const std::string& e);
    std::string conv(Node* e, const TypeP& target);
    std::string literal(const ir::Lit& l, const TypeP& t);
    std::string null_of(const TypeP& t) const;
    std::string local_name(const ir::SymP& s) const;
    std::string select(const ir::Select& s, Node* n);
    std::string call(const ir::Call& c, Node* n);
    std::string library_call(const ir::Call& c, Node* n, const std::string& owner);
    std::string new_expr(const ir::New& e, Node* n);
    std::string binary(const ir::Binary& b, Node* n);
    std::string binary_core(const ir::Binary& b, Node* n, const std::string& pt, const std::string& l, const std::string& r);
    std::string binop(const std::string& op, const std::string& l, const std::string& r, const std::string& pt, bool is_float,
                      bool checked);
    std::string operand(Node* n, const std::string& pt);
    std::string unary(const ir::Unary& u, Node* n);
    std::string assign(const ir::Assign& a, Node* n);
    std::string cast(const ir::Cast& c, Node* n);
    std::string lambda(const ir::Lambda& l, Node* n);
    std::string switch_expr(const ir::SwitchExpr& s, Node* n);
    std::string index_expr(const ir::Index& i);
    std::string new_array(const ir::NewArray& a, Node* n);
    std::string args_text(const std::vector<Node*>& args, const ir::SymP& sym);
    std::string receiver(Node* target);                    // `x->` or `x.` for a member access on the node
    std::string instance_of(const ir::InstanceOf& t);
};

}  // namespace cb::translate::backends::csharp_cpp17::detail
