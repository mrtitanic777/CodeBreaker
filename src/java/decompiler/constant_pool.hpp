// java/decompiler/constant_pool.hpp -- a class's constant pool as the decompiler uses it: each constant resolved (a class, string or method type to its
// text; a field, method or interface-method reference, a name-and-type, an invoke-dynamic or a method handle to its class, name and descriptor, with the
// parameter count and the kind of value it returns), and the renamer's new names applied when one is set.
//
//     const ConstantPool pool(class_file, nullptr);
//     const LinkConstant* m = pool.link(12);   // m->classname "java/io/PrintStream", m->elementname "println", m->descriptor "(I)V", m->param_count 1
//
// Ported from Fernflower (de.fernflower.struct.consts: ConstantPool, PooledConstant, PrimitiveConstant, LinkConstant, VariableTypeEnum), Copyright
// 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). The pool is read from java/classfile (its Modified UTF-8 as UTF-8);
// writing a pool back is java/classfile's.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "java/classfile.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

// The kinds a loadable constant has on the stack (VariableTypeEnum).
namespace vte {
inline constexpr int BOOLEAN = 1, BYTE = 2, CHAR = 3, SHORT = 4, INT = 5, FLOAT = 6, LONG = 7, DOUBLE = 8, RETURN_ADDRESS = 9, REFERENCE = 10,
                     INSTANCE_UNINITIALIZED = 11, VALUE_UNKNOWN = 12, VOID = 13;
}

// The renamer's names: "class", "class name descriptor" -> the new one (for a member: "class newname descriptor"); nullopt keeps it.
class PoolInterceptor {
public:
    virtual ~PoolInterceptor() = default;
    virtual std::optional<std::string> get_name(const std::string& old_name) const = 0;
};

class ConstantPool;

struct PooledConstant {
    int type = 0;
    bool own = false;
    int return_type = 0;

    virtual ~PooledConstant() = default;
    virtual void resolve(const ConstantPool& pool) { (void)pool; }
    // The stack kind of a loadable constant's tag; std::runtime_error for one that cannot be loaded.
    static int pool_type_to_intern(int type);
};

// Integer, Long, Float, Double, String, Class, MethodType, Utf8.
struct PrimitiveConstant : PooledConstant {
    using Value = std::variant<std::monostate, std::int32_t, std::int64_t, float, double, std::string>;
    int index = 0;
    Value value;
    bool is_array = false;

    PrimitiveConstant(int type, Value value);
    // A Class, String or MethodType constant naming its Utf8 by index (resolved later).
    static std::unique_ptr<PrimitiveConstant> indexed(int type, int index);

    std::int32_t get_int() const { return std::get<std::int32_t>(value); }
    std::int64_t get_long() const { return std::get<std::int64_t>(value); }
    float get_float() const { return std::get<float>(value); }
    double get_double() const { return std::get<double>(value); }
    const std::string& get_string() const { return std::get<std::string>(value); }

    void resolve(const ConstantPool& pool) override;
    // Equal: the same tag, the same array-ness and the same value (a float or double compared by its bits, as Java's boxed equals does).
    bool equals(const PrimitiveConstant& other) const;

private:
    void init_constant();
};

// NameAndType, Fieldref, Methodref, InterfaceMethodref, InvokeDynamic, MethodHandle.
struct LinkConstant : PooledConstant {
    int index1 = 0, index2 = 0;
    std::string classname, elementname, descriptor;
    int param_count = 0;
    bool is_void = false;
    bool return_category2 = false;

    LinkConstant(int type, std::string classname, std::string elementname, std::string descriptor);
    LinkConstant(int type, int index1, int index2);

    void resolve(const ConstantPool& pool) override;
    bool equals(const LinkConstant& other) const;

private:
    void init_constant();
    void resolve_descriptor(const std::string& descriptor);
};

class ConstantPool {
public:
    static constexpr int FIELD = 1, METHOD = 2;

    // The pool of a class file, every constant resolved. `interceptor`: the renamer's names (may be null).
    ConstantPool(const classfile::ClassFile& c, std::shared_ptr<const PoolInterceptor> interceptor = nullptr);

    std::size_t size() const { return pool_.size(); }
    // The constant at `index` (null for index 0, the slot after a long or a double, or a tag the decompiler does not read).
    const PooledConstant* constant(std::size_t index) const;
    // The constant as a primitive or a link constant, renamed when there is an interceptor; null when it is not one. A renamed one is new: the pool
    // keeps it for as long as it lives.
    const PrimitiveConstant* primitive(std::size_t index) const;
    const LinkConstant* link(std::size_t index) const;
    // A member's name and descriptor (renamed), for a member of `class_name` named by these two Utf8 indexes.
    std::pair<std::string, std::string> class_element(int element_type, const std::string& class_name, int name_index, int descriptor_index) const;
    // The renamer's name for a class (in descriptor form for an array); nullopt when it keeps its name.
    std::optional<std::string> build_new_classname(const std::string& class_name) const;

private:
    std::optional<std::string> build_new_descriptor(bool is_field, const std::string& descriptor) const;

    std::vector<std::unique_ptr<PooledConstant>> pool_;
    std::shared_ptr<const PoolInterceptor> interceptor_;
    mutable std::vector<std::unique_ptr<PooledConstant>> renamed_;   // the renamed copies handed out
};

}  // namespace cb::java::decompiler
