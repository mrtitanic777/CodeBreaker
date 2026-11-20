// java/projects/minecraft/prep/fixes.hpp -- RDI's fixes to the remapped classes: the global transformations (strip the local variable table and the
// signatures, decompose variables, restore the source file names, guess the inner classes, fix access, merge two jars, set the class version), the
// visitors RetroMCP chains after them (the implicit constructors, the exceptions of conf/exceptions.exc, the access of conf/<side>.access) and the
// generics guess.
//
//     prep::ClassStorage storage(read_jar(path));
//     prep::Transform::strip_lvt(storage);
//     prep::Transform::fix_inner_classes(storage);
//     auto exceptions = prep::load_exceptions("conf/exceptions.exc");                 // optional: empty when the file cannot be read
//     auto chain = std::make_unique<prep::AddExceptions>(std::move(*exceptions), std::move(chain_so_far));
//     chain->visit(storage);
//     prep::GuessGenerics(storage).transform(storage);
//
// Ported from RDI (org.mcphackers.rdi.injector.transform.Transform / GuessGenerics, injector.visitors.ClassInitAdder / AddExceptions / AccessFixer,
// injector.data.Access / Exceptions / Data, util.NodeHelper / DescString / Util / Pair), MIT License, (c) MCPHackers.
//
// Transform::replace_common_constants is RDI's entry point only: the pass is prep/constants.hpp's replace_common_constants (area D), defined in
// fixes_constants.cpp so that this header does not include constants.hpp.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "java/bytecode/tree.hpp"
#include "java/projects/minecraft/prep/storage.hpp"

namespace cb::java::minecraft::prep {

// ---- util: Pair, DescString, NodeHelper, Util

// RDI's Pair<T, U>: two values, equal when both are.
template <class T, class U>
struct Pair {
    T left;
    U right;
    static Pair of(T a, U b) { return Pair{std::move(a), std::move(b)}; }
    const T& get_left() const { return left; }
    const U& get_right() const { return right; }
    void set_left(T v) { left = std::move(v); }
    void set_right(U v) { right = std::move(v); }
    bool operator==(const Pair& o) const { return left == o.left && right == o.right; }
};

// Utility for dissecting a method descriptor's argument types (RDI's DescString).
class DescString {
public:
    explicit DescString(const std::string& desc);
    bool has_next() const { return desc_.size() != start_index_; }
    std::string next_type();
    static std::string get_return_type(const std::string& desc) { return desc.substr(desc.rfind(')') + 1); }
    void reset() { start_index_ = 0; }

private:
    std::string desc_;
    std::size_t start_index_ = 0;
};

// RDI's NodeHelper: finding and testing members.
struct NodeHelper {
    static bc::FieldNode* get_field(bc::ClassNode& node, const std::string& name, const std::string& desc);
    static bc::MethodNode* get_method(bc::ClassNode& node, const std::string& name, const std::string& desc);
    static std::vector<bc::MethodNode*> get_constructors(bc::ClassNode& node);
    static bool is_abstract(const bc::MethodNode& m) { return (m.access & bc::op::ACC_ABSTRACT) != 0; }
    static bool is_static(const bc::MethodNode& m) { return (m.access & bc::op::ACC_STATIC) != 0; }
    static bool is_static(const bc::FieldNode& f) { return (f.access & bc::op::ACC_STATIC) != 0; }
};

// RDI's Util.arrayCopy: `original` cut or padded (with the default value) to `new_length`.
template <class T>
std::vector<T> array_copy(const std::vector<T>& original, std::size_t new_length) {
    std::vector<T> copy(new_length);
    for (std::size_t i = 0; i < original.size() && i < new_length; ++i) copy[i] = original[i];
    return copy;
}

// ---- data: Access, Exceptions and their loaders

// The access a conf/<side>.access file gives (RDI's Access): a level for a class, for a field (by owner and name) and for a method.
class Access {
public:
    enum class Level { PRIVATE, DEFAULT, PROTECTED, PUBLIC };
    static Level get_from_bytecode(int acc);
    static int set_access(Level level, int acc);   // RDI's Level.setAccess: the visibility bits replaced by the level's

    std::optional<Level> get_level(const std::string& class_name) const;
    std::optional<Level> get_field_level(const FieldReference& field) const;    // looked up without the descriptor
    std::optional<Level> get_method_level(const MethodReference& method) const;

    void put_class(const std::string& name, Level level) { classes_[name] = level; }
    void put_field(const std::string& owner, const std::string& name, Level level) { fields_[FieldReference{owner, name, ""}] = level; }
    void put_method(const MethodReference& m, Level level) { methods_[m] = level; }

private:
    struct RefHash {
        std::size_t operator()(const MemberReference& r) const { return static_cast<std::size_t>(static_cast<std::uint32_t>(reference_hash(r))); }
    };
    std::unordered_map<std::string, Level> classes_;
    std::unordered_map<FieldReference, Level, RefHash> fields_;
    std::unordered_map<MethodReference, Level, RefHash> methods_;
};

// The exceptions conf/exceptions.exc adds to methods (RDI's Exceptions).
class Exceptions {
public:
    std::vector<std::string> get_exceptions(const MethodReference& ref) const;   // empty for a method with none
    void set_exceptions(const MethodReference& ref, std::vector<std::string> excs) { exceptions_[ref] = std::move(excs); }

private:
    struct RefHash {
        std::size_t operator()(const MemberReference& r) const { return static_cast<std::size_t>(static_cast<std::uint32_t>(reference_hash(r))); }
    };
    std::unordered_map<MethodReference, std::vector<std::string>, RefHash> exceptions_;
};

// RDI's Data.loadExceptions / loadAccess. The original prints the stack trace of an IOException and returns null: here, empty (a message to stderr).
// A line the original's parsing throws on (no owner, an unknown level) throws std::runtime_error.
std::optional<Exceptions> load_exceptions(const std::filesystem::path& file);
std::optional<Access> load_access(const std::filesystem::path& file);

// ---- the global transformations (RDI's Transform)

class Transform {
public:
    static constexpr int VISIBILITY_MODIFIERS = bc::op::ACC_PRIVATE | bc::op::ACC_PROTECTED | bc::op::ACC_PUBLIC;

    // Guesses the inner classes from class nodes
    static void fix_inner_classes(ClassStorage& storage);
    // Restores the SwitchMaps to how they should be (deprecated in RDI); the amount of classes identified as switch maps
    static int fix_switch_maps(ClassStorage& storage);
    // Guesses anonymous inner classes from a synthetic field and a single "parent" that makes them; the amount of guessed anonymous inner classes
    static int guess_anonymous_inner_classes(ClassStorage& storage);
    static void fix_access(ClassStorage& storage);
    static void decompose_vars(ClassStorage& storage);
    static void remove_var_self_assignment(ClassStorage& storage);
    // (fixes_constants.cpp) each LDC / SIPUSH offered to every constant of ConstantPool in turn, until one replaces it
    static void replace_common_constants(ClassStorage& storage);
    // Sets class version for every class in storage
    static void set_major_version(ClassStorage& storage, int version);
    // Changes sourceFile property of all classes to their name
    static void restore_source_file(ClassStorage& storage);
    // Clears local variable table for every method in each class
    static void strip_lvt(ClassStorage& storage);
    static void strip_signatures(ClassStorage& storage);
    // Adds all missing classes, methods and fields from storage2 to storage (storage2's classes that are added are moved into storage)
    static void merge(ClassStorage& storage, ClassStorage& storage2);
};

// ---- the visitors

// A class with no constructor gets the first one of its superclass, calling it (RDI's ClassInitAdder).
class ClassInitAdder : public ClassVisitor {
public:
    ClassInitAdder(const ClassStorage& storage, std::unique_ptr<ClassVisitor> cv) : ClassVisitor(std::move(cv)), storage_(storage) {}
    static bc::MethodNode create_method(const std::string& super_class, const std::string& name, const std::string& desc);

protected:
    void visit_class(bc::ClassNode& node) override;

private:
    const ClassStorage& storage_;
};

// The exceptions of the context added to each method but <clinit>, sorted (RDI's AddExceptions).
class AddExceptions : public ClassVisitor {
public:
    AddExceptions(Exceptions context, std::unique_ptr<ClassVisitor> cv) : ClassVisitor(std::move(cv)), context_(std::move(context)) {}

protected:
    void visit_class(bc::ClassNode& node) override { class_node_ = &node; }
    void visit_method(bc::MethodNode& node) override;

private:
    std::vector<std::string> process_exceptions(const MethodReference& ref, const std::vector<std::string>& exceptions);
    Exceptions context_;
    bc::ClassNode* class_node_ = nullptr;
};

// The access levels of the context given to the classes, fields and methods it names (RDI's AccessFixer).
class AccessFixer : public ClassVisitor {
public:
    AccessFixer(Access context, std::unique_ptr<ClassVisitor> cv) : ClassVisitor(std::move(cv)), context_(std::move(context)) {}

protected:
    void visit_class(bc::ClassNode& node) override;
    void visit_field(bc::FieldNode& node) override;
    void visit_method(bc::MethodNode& node) override;

private:
    Access context_;
    bc::ClassNode* class_node_ = nullptr;
};

// ---- GuessGenerics

// Guesses the generic signatures of the classes from their bridge methods, and renames the bridges' targets after them (RDI's GuessGenerics).
// `transform` renames methods and calls in the storage it is given.
class GuessGenerics : public Injection {
public:
    explicit GuessGenerics(ClassStorage& storage);
    ~GuessGenerics() override;
    void transform(ClassStorage& storage) override;

    static std::string get_generics(int size);
    static std::string get_generics(const std::vector<std::optional<std::string>>& generics, bool parameterized);   // (a null entry reads "null")


private:
    struct Impl;
    ClassStorage* storage_;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::java::minecraft::prep
