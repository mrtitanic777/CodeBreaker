// java/projects/minecraft/prep/constants.hpp -- the constants the preparation puts back, and the instruction helpers they are written on: RDI's
// injector/data/constants (Constant, ConstantBuilder, ConstantPool, InsnConstant, PiConstants, OneDivideByPowerOfTwo, FloatCastedToDouble), the
// util classes InsnHelper, OPHelper and IdentifyCall, and RetroMCP's GLConstants (the OpenGL and keyboard constants).
//
//     prep::replace_common_constants(storage);                       // Transform.replaceCommonConstants: 0.0625F -> 1.0F / 16.0F, 3.14159 -> Math.PI ...
//     auto gl = std::make_shared<prep::GLConstantsData>(prep::GLConstantsData::load("gl_constants.json"));
//     prep::GLConstants visitor(nullptr, gl);                        // a ClassVisitor: glClear(16640) -> GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT
//     visitor.visit(storage);
//
// Differences from the Java, all of ownership (a node belongs to the InsnList that made it, tree.hpp): the helpers that make an instruction
// (int_insn ...) take the list that will own it; clone returns a list that owns the copies; a node is moved between lists whole (insert(list)),
// never one node of a foreign list; the fields of GLConstants' resource are read from a file or text (GLConstantsData), not a class path resource.
//
// Ported from RDI (org.mcphackers.rdi.injector.data.constants.*, org.mcphackers.rdi.util.InsnHelper / OPHelper / IdentifyCall) and RetroMCP
// (org.mcphackers.mcp.tools.injector.GLConstants), MIT License, (c) MCPHackers.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "java/bytecode/tree.hpp"
#include "java/projects/minecraft/prep/storage.hpp"

namespace cb::java::minecraft::prep {

// Double.toString / Float.toString (JDK 19 and later: the shortest decimal that reads back as the value; "1.0E-5", "100.0", "NaN", "Infinity").
std::string java_double_to_string(double value);
std::string java_float_to_string(float value);

// ---- OPHelper

class OPHelper {
public:
    // The stack size change of an instruction (what it pushes less what it pops, in words); std::runtime_error for athrow, multianewarray and
    // invokedynamic (the original throws IllegalArgumentException).
    static int get_stack_size_delta(const bc::InsnNode* insn);
    static bool is_return(int opcode);
    static bool is_array_load(int opcode);
    static bool is_var_load(int opcode);
    static int reverse_var_opcode(int opcode);
    static bool is_var_similar_type(int opcode1, int opcode2);
    static bool is_var_store(int opcode);
};

// ---- InsnHelper

// An operand of compare_insn: what the original passes as an Object (null, an Integer, a String, a LabelNode, a constant, a Handle).
struct Operand {
    enum Kind { NONE, INT, STRING, LABEL, CONSTANT, HANDLE };
    Kind kind = NONE;
    std::int32_t integer = 0;
    std::string text;
    const bc::LabelNode* label = nullptr;
    std::optional<bc::Constant> constant;
    bc::Handle handle;

    Operand() = default;
    Operand(std::nullptr_t) {}
    Operand(int v) : kind(INT), integer(v) {}
    Operand(const char* s) : kind(STRING), text(s) {}
    Operand(std::string s) : kind(STRING), text(std::move(s)) {}
    Operand(const bc::LabelNode* l) : kind(LABEL), label(l) {}
    Operand(bc::LabelNode* l) : kind(LABEL), label(l) {}
    Operand(bc::Constant c) : kind(CONSTANT), constant(std::move(c)) {}
    Operand(bc::Handle h) : kind(HANDLE), handle(std::move(h)) {}
};

class InsnHelper {
public:
    // `size` instructions from `insn` on (labels, line numbers and frames skipped, as next_insn does); the rest of the array stays null.
    static std::vector<bc::InsnNode*> fill(bc::InsnNode* insn, int size);
    static std::vector<bc::InsnNode*> fill_backwards(bc::InsnNode* insn, int size);
    static void remove_range(bc::InsnList& insns, bc::InsnNode* first, bc::InsnNode* last);
    static void remove(bc::InsnList& insns, const std::vector<bc::InsnNode*>& to_remove);
    // start to end, both included, along the list (std::runtime_error if end is not after start)
    static std::vector<bc::InsnNode*> range(bc::InsnNode* start, bc::InsnNode* end);
    // Copies of the instructions in a list of their own; a label is copied to a new label and the jumps and line numbers of the copies follow it.
    static bc::InsnList clone(const bc::InsnList& insn_list);
    static bc::InsnList clone(const std::vector<bc::InsnNode*>& insn_list);
    // The label before `current` (skipping line numbers), or null; can cause frames to be invalid.
    static bc::LabelNode* label_before(bc::InsnNode* current);
    static bc::InsnNode* next_insn(bc::InsnNode* current);
    static bc::InsnNode* previous_insn(bc::InsnNode* current);
    static bool contains_invoke(const bc::InsnList& insns, const bc::MethodInsnNode& invoke);
    // Wraps start_insn..end_insn in a try block of `exception` (none: any) whose handler is `handle` (null: a pop); `handle` is moved into the method.
    static void add_try_catch(bc::MethodNode& method, bc::InsnNode* start_insn, bc::InsnNode* end_insn, std::optional<std::string> exception);
    static void add_try_catch(bc::MethodNode& method, bc::InsnNode* start_insn, bc::InsnNode* end_insn, bc::InsnList* handle,
                              std::optional<std::string> exception);
    static void add_try_catch(bc::MethodNode& method, bc::InsnNode* start_insn, bc::InsnNode* end_insn, bc::InsnList* handle,
                              std::optional<std::string> exception, const bc::TryCatchBlockNode* after_block);
    static int get_free_index(const bc::InsnList& instructions);
    static int get_free_index(const bc::InsnList& instructions, const bc::InsnNode* until);

    // The shortest instruction that loads the value (the node is made by `owner`).
    static bc::InsnNode* int_insn(bc::InsnList& owner, std::int32_t value);
    static bc::InsnNode* long_insn(bc::InsnList& owner, std::int64_t value);
    static bc::InsnNode* boolean_insn(bc::InsnList& owner, bool value);
    static bc::InsnNode* float_insn(bc::InsnList& owner, float value);
    static bc::InsnNode* double_insn(bc::InsnList& owner, double value);
    static std::int32_t int_value(const bc::InsnNode* insn);
    static std::int64_t long_value(const bc::InsnNode* insn);
    static bool boolean_value(const bc::InsnNode* insn);
    static float float_value(const bc::InsnNode* insn);
    static double double_value(const bc::InsnNode* insn);
    // The invokespecial <init> that follows an aload 0 (the super or this call of a constructor), or null.
    static bc::InsnNode* get_super(bc::InsnNode* first);
    static bc::InsnNode* get_last_return(bc::InsnNode* last);
    // Whether the instruction has that opcode (-1: any) and operands: field and method instructions compare owner, name, descriptor (a null operand
    // matches anything); constants, variables, types, labels and line numbers compare their one operand; with no operand the opcode alone decides.
    static bool compare_insn(const bc::InsnNode* insn, int opcode, const std::vector<Operand>& compare = {});
};

// ---- IdentifyCall

// The instructions that make up each argument of a call (and the receiver of a call that is not static, at index 0), found by walking back from the
// call along the stack sizes.
class IdentifyCall {
public:
    using Argument = std::optional<std::vector<bc::InsnNode*>>;   // none: an argument the walk did not find (null in the original)

    explicit IdentifyCall(bc::MethodInsnNode* invoke);
    const std::vector<Argument>& get_arguments() const { return arguments_; }
    const std::vector<bc::InsnNode*>& get_argument(int index) const;   // std::runtime_error for one not found

    // Removes the call, its receiver and its arguments.
    static void remove_call(bc::InsnList& insns, bc::MethodInsnNode* invoke);
    // The first call of that opcode, owner, name and descriptor in the list.
    static std::optional<IdentifyCall> identify_call(const bc::InsnList& insns, int opcode, const std::string& owner, const std::string& name,
                                                     const std::string& desc);

private:
    std::vector<Argument> init_arguments();
    bc::MethodInsnNode* invoke_insn_;
    std::vector<Argument> arguments_;
};

// ---- the constants

// A constant of the source (a value, a field, a division) the bytecode has as a loaded value.
class Constant {
public:
    virtual ~Constant() = default;
    // Replaces `constant` in the list (an ldc, for most) by the instructions that make its value; whether it did.
    virtual bool replace(bc::InsnList& source_list, bc::InsnNode* constant);
    virtual bool replace(bc::InsnList& source_list, bc::LdcInsnNode* constant) = 0;
    virtual std::unique_ptr<Constant> copy() const = 0;
};

class FloatCastedToDouble : public Constant {
public:
    bool replace(bc::InsnList& source_list, bc::LdcInsnNode* constant) override;
    using Constant::replace;
    std::unique_ptr<Constant> copy() const override { return std::make_unique<FloatCastedToDouble>(); }
};

class OneDivideByPowerOfTwo : public Constant {
public:
    bool replace(bc::InsnList& source_list, bc::LdcInsnNode* constant) override;
    using Constant::replace;
    std::unique_ptr<Constant> copy() const override { return std::make_unique<OneDivideByPowerOfTwo>(); }
};

// A value (an Integer, Long, Float or Double) and the instructions that make it; replaces a constant equal to the value, or to its negative (then
// followed by a negate).
class InsnConstant : public Constant {
public:
    InsnConstant(bc::InsnList insns, bc::Constant value) : insn_list_(std::move(insns)), value_(std::move(value)) {}
    bool replace(bc::InsnList& source_list, bc::InsnNode* constant) override;
    bool replace(bc::InsnList& source_list, bc::LdcInsnNode* constant) override;
    void replace(bc::InsnList& source_list, bc::InsnNode* constant, bc::InsnList& replace_list);
    std::unique_ptr<Constant> copy() const override { return std::make_unique<InsnConstant>(InsnHelper::clone(insn_list_), value_); }

protected:
    std::optional<bc::Constant> get_negative() const;
    bc::InsnNode* get_negate_instruction(bc::InsnList& owner) const;

private:
    bc::InsnList insn_list_;
    bc::Constant value_;
};

// A constant written as an expression: set a value (or a field with that value), then multiply, divide, add, subtract and cast; the builder
// follows the value as Java computes it and the instructions as they read in source.
class ConstantBuilder {
public:
    static constexpr int INT = 0, LONG = 1, FLOAT = 2, DOUBLE = 3;

    ConstantBuilder() = default;
    ConstantBuilder(ConstantBuilder&&) = default;
    ConstantBuilder& operator=(ConstantBuilder&&) = default;
    // (Java's is one builder every call clears; a new one is the same)
    static ConstantBuilder new_instance() { return ConstantBuilder(); }

    // Initial value
    ConstantBuilder& set(std::int32_t value);
    ConstantBuilder& set(std::int64_t value);
    ConstantBuilder& set(float value);
    ConstantBuilder& set(double value);
    // Initial field value (a getstatic of a constant field)
    ConstantBuilder& set(const std::string& owner, const std::string& name, std::int8_t value);
    ConstantBuilder& set(const std::string& owner, const std::string& name, std::int16_t value);
    ConstantBuilder& set(const std::string& owner, const std::string& name, std::int32_t value);
    ConstantBuilder& set(const std::string& owner, const std::string& name, std::int64_t value);
    ConstantBuilder& set(const std::string& owner, const std::string& name, float value);
    ConstantBuilder& set(const std::string& owner, const std::string& name, double value);

    ConstantBuilder& multiply(std::int32_t value);
    ConstantBuilder& multiply(std::int64_t value);
    ConstantBuilder& multiply(float value);
    ConstantBuilder& multiply(double value);
    ConstantBuilder& multiply(const std::string& owner, const std::string& name, std::int32_t value);
    ConstantBuilder& multiply(const std::string& owner, const std::string& name, std::int64_t value);
    ConstantBuilder& multiply(const std::string& owner, const std::string& name, float value);
    ConstantBuilder& multiply(const std::string& owner, const std::string& name, double value);
    ConstantBuilder& multiply(const ConstantBuilder& builder);

    ConstantBuilder& divide(std::int32_t value);
    ConstantBuilder& divide(std::int64_t value);
    ConstantBuilder& divide(float value);
    ConstantBuilder& divide(double value);
    ConstantBuilder& divide(const std::string& owner, const std::string& name, std::int32_t value);
    ConstantBuilder& divide(const std::string& owner, const std::string& name, std::int64_t value);
    ConstantBuilder& divide(const std::string& owner, const std::string& name, float value);
    ConstantBuilder& divide(const std::string& owner, const std::string& name, double value);
    ConstantBuilder& divide(const ConstantBuilder& builder);

    ConstantBuilder& add(std::int32_t value);
    ConstantBuilder& add(std::int64_t value);
    ConstantBuilder& add(float value);
    ConstantBuilder& add(double value);
    ConstantBuilder& add(const std::string& owner, const std::string& name, std::int32_t value);
    ConstantBuilder& add(const std::string& owner, const std::string& name, std::int64_t value);
    ConstantBuilder& add(const std::string& owner, const std::string& name, float value);
    ConstantBuilder& add(const std::string& owner, const std::string& name, double value);
    ConstantBuilder& add(const ConstantBuilder& builder);

    ConstantBuilder& subtract(std::int32_t value);
    ConstantBuilder& subtract(std::int64_t value);
    ConstantBuilder& subtract(float value);
    ConstantBuilder& subtract(double value);
    ConstantBuilder& subtract(const std::string& owner, const std::string& name, std::int32_t value);
    ConstantBuilder& subtract(const std::string& owner, const std::string& name, std::int64_t value);
    ConstantBuilder& subtract(const std::string& owner, const std::string& name, float value);
    ConstantBuilder& subtract(const std::string& owner, const std::string& name, double value);
    ConstantBuilder& subtract(const ConstantBuilder& builder);

    // Casts (note: the original's int to float cast emits l2f)
    ConstantBuilder& to_int();
    ConstantBuilder& to_long();
    ConstantBuilder& to_float();
    ConstantBuilder& to_double();

    // The constant (null when nothing was set).
    std::unique_ptr<Constant> build() const;
    ConstantBuilder copy() const;

    int type = -1;
    std::int32_t int_value = 0;
    std::int64_t long_value = 0;
    float float_value = 0;
    double double_value = 0;

private:
    enum Op { MUL, DIV, ADD, SUB };
    template <class T> ConstantBuilder& apply(Op op, T value);
    template <class T> ConstantBuilder& apply_field(Op op, const std::string& owner, const std::string& name, T value);
    ConstantBuilder& apply_builder(Op op, const ConstantBuilder& builder);
    bc::InsnNode* cast(int target_type);
    void check_type(int required_type) const;

    bc::InsnList insns_;
};

// Math.PI and (float)Math.PI as the source writes them: x * PI, PI / 180 ...
class PiConstants : public Constant {
public:
    PiConstants();
    bool replace(bc::InsnList& source_list, bc::LdcInsnNode* constant) override;
    using Constant::replace;
    std::unique_ptr<Constant> copy() const override { return std::make_unique<PiConstants>(); }

    std::vector<std::unique_ptr<Constant>> pi_doubles, pi_floats;

private:
    void add(const ConstantBuilder& constant);
    ConstantBuilder pi_, pi_float_, pi_float_double_;
};

// The list of constants, in the order they are tried.
class ConstantPool {
public:
    static const std::vector<std::unique_ptr<Constant>>& constants();   // Java's ConstantPool.CONSTANTS
    static ConstantBuilder constant() { return ConstantBuilder::new_instance(); }
};

// Transform.replaceCommonConstants: in every method, each ldc or sipush that a constant of the pool matches (the first to) is replaced by the
// expression of the constant.
void replace_common_constants(ClassStorage& storage);

// ---- GLConstants

// The data of gl_constants.json: for each group of OpenGL methods, the names of the constants their integer arguments are (a combination is written
// with | & ^), and the names of Keyboard's keys.
class GLConstantsData {
public:
    // std::runtime_error if the file is not JSON (the original then leaves the class without constants: pass no data).
    static GLConstantsData load(const std::filesystem::path& file);
    static GLConstantsData parse(std::string_view json_text);

    // The key constant for isKeyDown(n) and so on (a getstatic of org/lwjgl/input/Keyboard), or null.
    bc::InsnNode* get_keyboard_insn(bc::InsnList& owner, std::int32_t constant) const;
    // The expression of the constant as an argument of that OpenGL call (GL_A | GL_B), in a list of its own, or none.
    std::optional<bc::InsnList> get_gl_insn(const bc::MethodInsnNode& invoke, std::int32_t constant) const;
    bool is_gl_package(const std::string& owner) const { return packages_.contains(owner); }

private:
    struct Group {
        std::unordered_map<std::string, std::vector<std::string>> methods;   // package -> method names
        // package -> (value -> constant name), in the order Java's HashMap<String, ...> iterates them
        std::vector<std::pair<std::string, std::unordered_map<std::int32_t, std::string>>> values;
    };
    std::vector<Group> constants_;
    std::unordered_map<std::int32_t, std::string> keyboard_;
    std::unordered_set<std::string> packages_;
};

// The visitor that puts OpenGL and Keyboard constants where a method has their numbers (RetroMCP's GLConstants).
class GLConstants : public ClassVisitor {
public:
    // `data` null: the visitor does nothing (the original when its resource is missing).
    GLConstants(std::unique_ptr<ClassVisitor> inner, std::shared_ptr<const GLConstantsData> data)
        : ClassVisitor(std::move(inner)), data_(std::move(data)) {}

    static bool is_icmp(int opcode);
    // The int an instruction loads (iconst, bipush, sipush, an int ldc), or none.
    static std::optional<std::int32_t> int_value(const bc::InsnNode* insn);

protected:
    void visit_method(bc::MethodNode& node) override;

private:
    std::shared_ptr<const GLConstantsData> data_;
};

}  // namespace cb::java::minecraft::prep
