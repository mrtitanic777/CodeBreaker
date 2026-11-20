// java/projects/minecraft/prep/storage.hpp -- the classes of a Minecraft jar as the preparation passes see them: the storage, the references to a
// member, and the visitor chain (the model of RetroMCP's RDI library: org.mcphackers.rdi.injector.data.ClassStorage, util.FieldReference /
// MethodReference, injector.visitors.ClassVisitor, injector.transform.Injection; RDI and RetroMCP are MIT-licensed, (c) MCPHackers).
//
//     prep::ClassStorage storage(read_jar(path));                 // every class of the jar, in its order
//     bytecode::ClassNode* c = storage.get_class("net/minecraft/src/Block");
//     for (bytecode::ClassNode* node : storage) ...
//
// The preparation (prep.hpp) is what RetroMCP's setup and decompile tasks do before and after Fernflower: download the version, remap its classes
// with the version's mappings, fix them up (RDI), decompile, patch the source. CodeBreaker does it natively; RetroMCP is retired.
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "java/bytecode/tree.hpp"

namespace cb::java::minecraft::prep {

namespace bc = cb::java::bytecode;

// A field or a method by owner, name and descriptor (RDI's FieldReference / MethodReference). `hash_code` is RDI's, for a JavaHashMap that is iterated.
struct MemberReference {
    std::string owner, name, desc;
    bool operator==(const MemberReference& o) const { return owner == o.owner && name == o.name && desc == o.desc; }
};
using FieldReference = MemberReference;
using MethodReference = MemberReference;

// String.hashCode (the names here are ASCII: each byte one char; a non-ASCII name would hash by its UTF-8 bytes, not Java's UTF-16 units).
inline std::int32_t java_string_hash(const std::string& s) {
    std::uint32_t h = 0;
    for (const unsigned char c : s) h = 31 * h + c;
    return static_cast<std::int32_t>(h);
}
// RDI's hashCode of a reference: (owner.hashCode() & 0xFFFF0000 | name.hashCode() & 0x0000FFFF) ^ desc.hashCode()
inline std::int32_t reference_hash(const MemberReference& r) {
    return static_cast<std::int32_t>((static_cast<std::uint32_t>(java_string_hash(r.owner)) & 0xFFFF0000u) |
                                     (static_cast<std::uint32_t>(java_string_hash(r.name)) & 0x0000FFFFu)) ^
           java_string_hash(r.desc);
}

// The jar's classes in order, each looked up by name (RDI's ClassStorage). The storage owns them.
class ClassStorage {
public:
    ClassStorage() = default;
    explicit ClassStorage(std::vector<std::unique_ptr<bc::ClassNode>> nodes) : nodes_(std::move(nodes)) { update_cache(); }
    ClassStorage(ClassStorage&&) = default;
    ClassStorage& operator=(ClassStorage&&) = default;

    struct iterator {
        std::vector<std::unique_ptr<bc::ClassNode>>::const_iterator at;
        bc::ClassNode* operator*() const { return at->get(); }
        iterator& operator++() { ++at; return *this; }
        bool operator!=(const iterator& o) const { return at != o.at; }
    };
    iterator begin() const { return {nodes_.begin()}; }
    iterator end() const { return {nodes_.end()}; }
    std::size_t size() const { return nodes_.size(); }
    const std::vector<std::unique_ptr<bc::ClassNode>>& get_classes() const { return nodes_; }

    bc::ClassNode* get_class(const std::string& name) const {
        const auto it = cache_.find(name);
        return it == cache_.end() ? nullptr : it->second;
    }
    bc::FieldNode* get_field(const FieldReference& ref) const {
        if (bc::ClassNode* node = get_class(ref.owner)) {
            for (bc::FieldNode& f : node->fields) {
                if (f.name == ref.name && f.desc == ref.desc) return &f;
            }
        }
        return nullptr;
    }
    bc::MethodNode* get_method(const MethodReference& ref) const {
        if (bc::ClassNode* node = get_class(ref.owner)) {
            for (bc::MethodNode& m : node->methods) {
                if (m.name == ref.name && m.desc == ref.desc) return &m;
            }
        }
        return nullptr;
    }
    // A class of the same name is replaced: the old one leaves the list, the new one goes at its end (as RDI's addClass).
    void add_class(std::unique_ptr<bc::ClassNode> node) {
        if (bc::ClassNode* old = get_class(node->name)) remove_class(old);
        cache_[node->name] = node.get();
        nodes_.push_back(std::move(node));
    }
    void remove_class(bc::ClassNode* node) {
        if (node == nullptr) return;
        cache_.erase(node->name);
        std::erase_if(nodes_, [&](const auto& p) { return p.get() == node; });
    }
    void remove_class(const std::string& name) { remove_class(get_class(name)); }
    // Takes a class out of the storage and gives it to the caller (Java shares a node between two storages, as RDI's merge does; here it moves).
    std::unique_ptr<bc::ClassNode> release_class(bc::ClassNode* node) {
        std::unique_ptr<bc::ClassNode> out;
        for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
            if (it->get() == node) { out = std::move(*it); nodes_.erase(it); break; }
        }
        if (out) cache_.erase(out->name);
        return out;
    }
    // (after a pass renames classes)
    void update_cache() {
        cache_.clear();
        for (const auto& n : nodes_) cache_[n->name] = n.get();
    }
    std::vector<std::string> get_all_classes() const {
        std::vector<std::string> names;
        for (const auto& n : nodes_) names.push_back(n->name);
        return names;
    }
    static bool in_one_package(const std::string& a, const std::string& b) {
        const std::size_t sa = a.rfind('/'), sb = b.rfind('/');
        if (sa != sb) return false;
        if (sa == std::string::npos) return true;
        return a.compare(0, sa, b, 0, sb) == 0;
    }

private:
    std::vector<std::unique_ptr<bc::ClassNode>> nodes_;
    std::unordered_map<std::string, bc::ClassNode*> cache_;
};

// A pass over the whole storage (RDI's Injection).
class Injection {
public:
    virtual ~Injection() = default;
    virtual void transform(ClassStorage& storage) = 0;
};

// RDI's ClassVisitor: a chain. A visitor made around another (`ClassInitAdder(storage, visitorStack)`) and merge(a, b) (`a` goes under the
// innermost of `b`'s chain) both put the wrapped one first: each visit reaches the innermost visitor first, then each outer one. Per class the order
// is: the class, its inner classes, its methods, its fields, the end.
class ClassVisitor {
public:
    explicit ClassVisitor(std::unique_ptr<ClassVisitor> inner = nullptr) : cv_(std::move(inner)) {}
    virtual ~ClassVisitor() = default;

    // merge(first, second): `first` becomes the innermost of `second`'s chain (it visits first); either may be null
    static std::unique_ptr<ClassVisitor> merge(std::unique_ptr<ClassVisitor> first, std::unique_ptr<ClassVisitor> second) {
        if (second == nullptr) return first;
        if (first == nullptr) return second;
        ClassVisitor* v = second.get();
        while (v->cv_ != nullptr) v = v->cv_.get();
        v->cv_ = std::move(first);
        return second;
    }

    void visit(const ClassStorage& storage) {
        for (bc::ClassNode* node : storage) visit(*node);
    }
    void visit(bc::ClassNode& node) {
        visit_class1(node);
        for (bc::InnerClassNode& inner : node.inner_classes) visit_inner1(inner);
        for (bc::MethodNode& method : node.methods) visit_method1(method);
        for (bc::FieldNode& field : node.fields) visit_field1(field);
        visit_end1(node);
    }

protected:
    virtual void visit_class(bc::ClassNode&) {}
    virtual void visit_inner(bc::InnerClassNode&) {}
    virtual void visit_method(bc::MethodNode&) {}
    virtual void visit_field(bc::FieldNode&) {}
    virtual void visit_end(bc::ClassNode&) {}

private:
    void visit_class1(bc::ClassNode& n) { if (cv_) cv_->visit_class1(n); visit_class(n); }
    void visit_inner1(bc::InnerClassNode& n) { if (cv_) cv_->visit_inner1(n); visit_inner(n); }
    void visit_method1(bc::MethodNode& n) { if (cv_) cv_->visit_method1(n); visit_method(n); }
    void visit_field1(bc::FieldNode& n) { if (cv_) cv_->visit_field1(n); visit_field(n); }
    void visit_end1(bc::ClassNode& n) { if (cv_) cv_->visit_end1(n); visit_end(n); }

    std::unique_ptr<ClassVisitor> cv_;
};

}  // namespace cb::java::minecraft::prep
