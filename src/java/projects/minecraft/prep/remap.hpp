// java/projects/minecraft/prep/remap.hpp -- the mappings of a Minecraft version and the remapping of its classes with them (RDI's Remapper).
//
//     auto mappings = prep::get_mappings("conf/mappings.tiny", storage, "client");    // RetroMCP's TaskDecompile.getMappings (nullopt: no file)
//     if (mappings) prep::Remapper().load(storage).load(*mappings).process();         // every class, field and method renamed in place
//
//     prep::Mappings m = prep::MappingsIO::read("mappings.tiny", "official", "named"); // a tiny v1 or v2 file, one namespace to another
//
// The classes' new names (the original names are replaced: the storage's lookup cache is updated), the fields' and methods' (a rename is carried to
// the classes below the owner, as access allows), every reference to them in the code, the descriptors and signatures, the annotations, the inner
// class table; and each method's MethodParameters from the parameter names of the mappings (a name made of the type's where there is none).
//
// Ported from RDI (org.mcphackers.rdi.injector.remapper.Remapper / FieldRenameMap / MethodRenameMap, injector.data.Mappings, nio.MappingsIO, whose
// tiny readers are Fabric's mapping-io: Tiny1FileReader / Tiny2FileReader / MemoryMappingTree) and RetroMCP (tools.mappings.MappingUtil.readNamespaces,
// tasks.TaskDecompile.getMappings), MIT License, (c) MCPHackers (mapping-io: Apache-2.0, (c) FabricMC).
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "java/projects/minecraft/prep/storage.hpp"

namespace cb::java::minecraft::prep {

// The hash of a reference, for the maps that are only looked up.
struct MemberReferenceHash {
    std::size_t operator()(const MemberReference& r) const noexcept {
        const std::hash<std::string> h;
        std::size_t v = h(r.owner);
        v = v * 1000003u ^ h(r.name);
        v = v * 1000003u ^ h(r.desc);
        return v;
    }
};

// A field's new name by (owner, descriptor, old name).
class FieldRenameMap {
public:
    void clear() { renames_.clear(); }
    std::optional<std::string> get(const std::string& owner, const std::string& descriptor, const std::string& old_name) const;
    std::optional<std::string> get_or_default(const std::string& owner, const std::string& descriptor, const std::string& old_name,
                                              const std::optional<std::string>& default_value) const;
    std::string opt_get(const std::string& owner, const std::string& descriptor, const std::string& old_name) const;   // the old name if none
    void put(const std::string& owner, const std::string& descriptor, const std::string& name, const std::string& new_name);
    void put_all(const FieldRenameMap& other);   // `other`'s entries into this one
    std::size_t size() const { return renames_.size(); }

private:
    std::unordered_map<FieldReference, std::string, MemberReferenceHash> renames_;
};

// A method's parameter names, by local variable index (an empty name: none for that index).
using ParameterNames = std::vector<std::optional<std::string>>;

// A method's new name by (owner, descriptor, old name), and its parameters' names (the mappings' local variable names).
class MethodRenameMap {
public:
    void clear() { renames_.clear(); }
    std::optional<std::string> get(const std::string& owner, const std::string& descriptor, const std::string& old_name) const;
    std::optional<std::string> get_or_default(const std::string& owner, const std::string& descriptor, const std::string& old_name,
                                              const std::optional<std::string>& default_value) const;
    std::string opt_get(const std::string& owner, const std::string& descriptor, const std::string& old_name) const;
    void put(const std::string& owner, const std::string& descriptor, const std::string& name, const std::string& new_name);
    // Removes a method's remapping (a SINGLE method of a SINGLE class and its references).
    void remove(const std::string& owner, const std::string& desc, const std::string& name);
    void put_all(const MethodRenameMap& other);   // the names and the parameters' names
    std::size_t size() const { return renames_.size(); }
    // nullptr: none (never set, or set to none)
    const ParameterNames* get_lv_mappings(const std::string& owner, const std::string& descriptor, const std::string& old_name) const;
    void set_lv_mappings(const std::string& owner, const std::string& descriptor, const std::string& old_name, const ParameterNames* params);

private:
    std::unordered_map<MethodReference, std::string, MemberReferenceHash> renames_;
    std::unordered_map<MethodReference, std::optional<ParameterNames>, MemberReferenceHash> lvt_;
};

// What a mappings file gives: the renames of the fields and methods, and of the classes (by internal name).
struct Mappings {
    enum class Provider { TINY1, TINY2 };
    FieldRenameMap fields;
    MethodRenameMap methods;
    std::unordered_map<std::string, std::string> classes;
};

// RDI's nio.MappingsIO: a tiny v1 ("v1" header) or v2 ("tiny 2 0" header) file read as the names of one namespace to those of another.
class MappingsIO {
public:
    // Empty mappings when the header is not a tiny one or the file is damaged (the error is printed to stderr, as RDI prints its stack trace).
    // A namespace the file does not have names nothing.
    static Mappings read(const std::filesystem::path& path, const std::string& src_namespace, const std::string& target_namespace);
    // The namespaces by index: -1 the source namespace (the first column), 0.. the destination namespaces
    static Mappings read(const std::filesystem::path& path, int src_namespace, int target_namespace);
    // nullopt: not a tiny file this reads
    static std::optional<Mappings::Provider> get_provider(const std::filesystem::path& mappings);
};

// RetroMCP's MappingUtil.readNamespaces: the namespaces of a tiny file's header (source first). std::runtime_error ("No valid tiny header in <path>")
// for another file.
std::vector<std::string> read_namespaces(const std::filesystem::path& mappings);

// RetroMCP's TaskDecompile.getMappings: the mappings of `mappings_path` from "official" (a file that has that namespace) or else from the side's
// own ("client" or "server": Side.name) to "named"; a class of the storage with no package and no mapping goes to net/minecraft/src/<name>.
// nullopt when the file does not exist.
std::optional<Mappings> get_mappings(const std::filesystem::path& mappings_path, const ClassStorage& storage, const std::string& side_name);

// CodeBreaker's, not RDI's: the mappings completed with what the decompiled source needs to compile -- a method named in a class and not in the
// class or interface above it that declares it is named there too; and of two methods of one name and the same parameters that differ only in
// what they return (the bytecode allows it; Java and C++ do not), the later is <name>_<return type> (`a_V`). Run on the storage before the remapper.
Mappings completed_for_source(const Mappings& given, const ClassStorage& storage);

// RDI's Remapper: the in-memory remapping of a storage's classes (the nodes are changed, the storage's cache is updated).
class Remapper {
public:
    Remapper& load(ClassStorage& classes) {
        storage_ = &classes;
        return *this;
    }
    Remapper& load(const Mappings& mapping_data) {
        mappings_ = &mapping_data;
        return *this;
    }
    // Every class remapped; the class names are the last to change.
    void process();

private:
    void create_method_hierarchy();
    void create_field_hierarchy();
    template <class A>
    void remap_annotations(std::vector<A>& annotations);
    void remap_annotation(bc::AnnotationNode& annotation);
    void remap_annotation_value(bc::AnnotationValue& value);
    void remap_bsm_arg(std::vector<bc::Constant>& bsm_args, std::size_t index);
    void remap_field(const std::string& owner, bc::FieldNode& field);
    std::string remap_internal_name(const std::string& internal_name);
    void remap_method(bc::ClassNode& owner, bc::MethodNode& method);
    static std::optional<bc::Type> get_param_type(const std::vector<bc::Type>& parameters, bool is_static, int lv_index);
    bool remap_signature(const std::string& signature, std::string& out);
    bool remap_signature(std::string& signature_out, const std::string& signature, int start, int end);
    std::string remap_single_desc(const std::string& input);
    std::optional<std::string> mapped_class(const std::string& name) const;

    MethodRenameMap hierarchised_method_renames_;
    FieldRenameMap hierarchised_field_renames_;
    ClassStorage* storage_ = nullptr;
    const Mappings* mappings_ = nullptr;
};

}  // namespace cb::java::minecraft::prep
