// cpp17_jmodel.hpp -- a Java program's object model read off L2 (codebreaker/translate/backends/jmodel.py),
// for the C++17 back end's Java rules: every class (nested, local, anonymous and enum-constant bodies
// included) with its C name, its supertypes, whether it needs its outer instance, what a local or
// anonymous class captures; which method a method overrides (so its erased signature is the one the
// root declares, as javac's bridge methods make it); the one abstract method of a functional
// interface; which classes can form a reference cycle (where reference counting leaks).
//
// Library types are named by their qualified Java name; lib_supers() gives their supertypes and
// lib_methods() the methods a program class can override, each as javac would type it.
#pragma once
#include <optional>
#include <unordered_map>

#include "cpp17.hpp"

namespace cpp17 {
namespace jm {

extern const char* const OBJECT;

struct Sam {
    std::string name;
    std::vector<TypeP> params;
    TypeP ret;
};

const std::vector<std::pair<std::string, Sam>>& sams();                 // in jmodel.SAMS' order
const Sam* sam_of_lib(const std::string& q);
const std::vector<std::pair<std::string, std::vector<std::string>>>& lib_supers();
const std::vector<std::string>* lib_supers_of(const std::string& q);
const std::vector<Sam>* lib_methods_of(const std::string& q);

struct ClassInfo {
    NodeP decl;
    std::string qname, cname;
    ClassInfo* outer = nullptr;
    bool local = false, anon = false, inner = false, needs_outer = false;
    std::string kind;
    std::vector<std::pair<std::string, TypeP>> captures;
    std::string base;                  // '' = None
    std::vector<std::string> ifaces;
    NodeP enum_const;
    std::string package;
    NodeP method;                      // the method a local/anonymous class sits in
    NodeP new_;                        // the New of an anonymous class
    std::vector<NodeP> methods(const std::string* name = nullptr) const;
    std::vector<NodeP> fields() const;
    const std::vector<NodeP>& members() const { return nodes(decl, "members"); }
};

TypeP erase(const TypeP& t);

class Model {
public:
    explicit Model(const std::vector<NodeP>& units);
    void name_anonymous();
    ClassInfo* info(const std::string& qname) const;
    ClassInfo* of_type(const TypeP& t) const;
    ClassInfo* by_decl(const l2::Node* n) const;
    std::vector<std::string> supers(const std::string& q) const;
    std::vector<std::string> ancestors(const std::string& q) const;
    bool is_sub(const std::string& a, const std::string& b) const;
    bool is_interface(const std::string& q) const;
    bool subclassed(const std::string& q) const;
    std::pair<ClassInfo*, NodeP> method_decl(const SymP& sym) const;
    bool overrides(ClassInfo* ci, const NodeP& m, std::vector<TypeP>& params, TypeP& ret) const;
    std::optional<Sam> sam(const std::string& q) const;
    std::vector<std::string> cycles() const;
    const std::vector<ClassInfo*>& all() const { return order_; }

private:
    std::vector<std::unique_ptr<ClassInfo>> owned_;
    std::vector<ClassInfo*> order_;                       // self.classes.values(), in insertion order
    std::unordered_map<std::string, size_t> index_;       // key -> position in order_
    std::unordered_map<const l2::Node*, ClassInfo*> by_decl_;
    int anon_ = 0;
    std::string cname_of(const std::string& qname, const std::string& package) const;
    ClassInfo* add(const NodeP& c, ClassInfo* outer, const std::string& package, bool local = false, bool anon = false,
                   const NodeP& method = nullptr);
    void nested_in(const NodeP& m, ClassInfo* ci, const std::string& package);
    void supers_of(ClassInfo* ci);
    void capture(ClassInfo* ci);
};

}  // namespace jm
}  // namespace cpp17
