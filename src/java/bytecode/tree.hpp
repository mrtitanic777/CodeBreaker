// java/bytecode/tree.hpp -- a Java class as a tree of named parts that passes can change: the model ASM's tree API gives (org.objectweb.asm.tree), on
// CodeBreaker's own class file reader (java/classfile). Names, descriptors and constants are values (not pool indexes), and a method's code is a
// linked list of instruction nodes with labels, so a pass inserts, removes and replaces instructions as ASM's do; the class is then written with its
// constant pool built again and each method's max stack and locals computed (ASM's ClassWriter with COMPUTE_MAXS).
//
//     bytecode::ClassNode c = bytecode::read(bytes);                    // FormatError for a damaged class
//     for (bytecode::MethodNode& m : c.methods)
//         for (bytecode::InsnNode* i = m.instructions.first(); i; i = i->next) ...
//     const Bytes out = bytecode::write(c);
//
// The names keep ASM's (ClassNode, MethodNode, InsnList, LdcInsnNode ...), their members its fields in snake case (superName -> super_name,
// tryCatchBlocks -> try_catch_blocks), so code ported from ASM-based Java reads as its original. What is not modelled is kept as it was: an
// attribute the tree does not know is carried with its bytes (it names no constants a pass can change: one that does is modelled). Stack map frames
// are not kept: the classes are read by the decompiler, which has no use for them (a class of version 51 or later written here does not verify).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "core/bytes.hpp"

namespace cb::java::bytecode {

// ---- Opcodes (the JVM's, as org.objectweb.asm.Opcodes names them)

namespace op {
inline constexpr int NOP = 0, ACONST_NULL = 1, ICONST_M1 = 2, ICONST_0 = 3, ICONST_1 = 4, ICONST_2 = 5, ICONST_3 = 6, ICONST_4 = 7, ICONST_5 = 8,
                     LCONST_0 = 9, LCONST_1 = 10, FCONST_0 = 11, FCONST_1 = 12, FCONST_2 = 13, DCONST_0 = 14, DCONST_1 = 15, BIPUSH = 16, SIPUSH = 17,
                     LDC = 18, ILOAD = 21, LLOAD = 22, FLOAD = 23, DLOAD = 24, ALOAD = 25, IALOAD = 46, LALOAD = 47, FALOAD = 48, DALOAD = 49,
                     AALOAD = 50, BALOAD = 51, CALOAD = 52, SALOAD = 53, ISTORE = 54, LSTORE = 55, FSTORE = 56, DSTORE = 57, ASTORE = 58,
                     IASTORE = 79, LASTORE = 80, FASTORE = 81, DASTORE = 82, AASTORE = 83, BASTORE = 84, CASTORE = 85, SASTORE = 86, POP = 87,
                     POP2 = 88, DUP = 89, DUP_X1 = 90, DUP_X2 = 91, DUP2 = 92, DUP2_X1 = 93, DUP2_X2 = 94, SWAP = 95, IADD = 96, LADD = 97,
                     FADD = 98, DADD = 99, ISUB = 100, LSUB = 101, FSUB = 102, DSUB = 103, IMUL = 104, LMUL = 105, FMUL = 106, DMUL = 107,
                     IDIV = 108, LDIV = 109, FDIV = 110, DDIV = 111, IREM = 112, LREM = 113, FREM = 114, DREM = 115, INEG = 116, LNEG = 117,
                     FNEG = 118, DNEG = 119, ISHL = 120, LSHL = 121, ISHR = 122, LSHR = 123, IUSHR = 124, LUSHR = 125, IAND = 126, LAND = 127,
                     IOR = 128, LOR = 129, IXOR = 130, LXOR = 131, IINC = 132, I2L = 133, I2F = 134, I2D = 135, L2I = 136, L2F = 137, L2D = 138,
                     F2I = 139, F2L = 140, F2D = 141, D2I = 142, D2L = 143, D2F = 144, I2B = 145, I2C = 146, I2S = 147, LCMP = 148, FCMPL = 149,
                     FCMPG = 150, DCMPL = 151, DCMPG = 152, IFEQ = 153, IFNE = 154, IFLT = 155, IFGE = 156, IFGT = 157, IFLE = 158,
                     IF_ICMPEQ = 159, IF_ICMPNE = 160, IF_ICMPLT = 161, IF_ICMPGE = 162, IF_ICMPGT = 163, IF_ICMPLE = 164, IF_ACMPEQ = 165,
                     IF_ACMPNE = 166, GOTO = 167, JSR = 168, RET = 169, TABLESWITCH = 170, LOOKUPSWITCH = 171, IRETURN = 172, LRETURN = 173,
                     FRETURN = 174, DRETURN = 175, ARETURN = 176, RETURN = 177, GETSTATIC = 178, PUTSTATIC = 179, GETFIELD = 180, PUTFIELD = 181,
                     INVOKEVIRTUAL = 182, INVOKESPECIAL = 183, INVOKESTATIC = 184, INVOKEINTERFACE = 185, INVOKEDYNAMIC = 186, NEW = 187,
                     NEWARRAY = 188, ANEWARRAY = 189, ARRAYLENGTH = 190, ATHROW = 191, CHECKCAST = 192, INSTANCEOF = 193, MONITORENTER = 194,
                     MONITOREXIT = 195, MULTIANEWARRAY = 197, IFNULL = 198, IFNONNULL = 199;
// access flags
inline constexpr int ACC_PUBLIC = 0x0001, ACC_PRIVATE = 0x0002, ACC_PROTECTED = 0x0004, ACC_STATIC = 0x0008, ACC_FINAL = 0x0010,
                     ACC_SUPER = 0x0020, ACC_SYNCHRONIZED = 0x0020, ACC_OPEN = 0x0020, ACC_TRANSITIVE = 0x0020, ACC_VOLATILE = 0x0040,
                     ACC_BRIDGE = 0x0040, ACC_STATIC_PHASE = 0x0040, ACC_VARARGS = 0x0080, ACC_TRANSIENT = 0x0080, ACC_NATIVE = 0x0100,
                     ACC_INTERFACE = 0x0200, ACC_ABSTRACT = 0x0400, ACC_STRICT = 0x0800, ACC_SYNTHETIC = 0x1000, ACC_ANNOTATION = 0x2000,
                     ACC_ENUM = 0x4000, ACC_MANDATED = 0x8000, ACC_MODULE = 0x8000, ACC_RECORD = 0x10000, ACC_DEPRECATED = 0x20000;
// newarray's types
inline constexpr int T_BOOLEAN = 4, T_CHAR = 5, T_FLOAT = 6, T_DOUBLE = 7, T_BYTE = 8, T_SHORT = 9, T_INT = 10, T_LONG = 11;
// method handle kinds
inline constexpr int H_GETFIELD = 1, H_GETSTATIC = 2, H_PUTFIELD = 3, H_PUTSTATIC = 4, H_INVOKEVIRTUAL = 5, H_INVOKESTATIC = 6, H_INVOKESPECIAL = 7,
                     H_NEWINVOKESPECIAL = 8, H_INVOKEINTERFACE = 9;
}  // namespace op

// ---- Type (org.objectweb.asm.Type): a field or method descriptor, or an internal name

class Type {
public:
    enum Sort { VOID = 0, BOOLEAN, CHAR, BYTE, SHORT, INT, FLOAT, LONG, DOUBLE, ARRAY, OBJECT, METHOD };

    static Type get_type(const std::string& descriptor);        // "I", "[Ljava/lang/String;", "(II)V"
    static Type get_object_type(const std::string& internal);   // "java/lang/String" (or an array descriptor "[I")
    static Type get_method_type(const std::string& descriptor);
    static Type get_method_type(const Type& return_type, const std::vector<Type>& argument_types);
    static std::vector<Type> get_argument_types(const std::string& method_descriptor);
    static Type get_return_type(const std::string& method_descriptor);
    // The size of the arguments and of the return value, as ASM packs them: (argumentsSize << 2) | returnSize; argumentsSize counts `this`.
    static int get_arguments_and_return_sizes(const std::string& method_descriptor);

    int get_sort() const { return sort_; }
    int get_dimensions() const;                // an array's
    Type get_element_type() const;             // an array's
    std::string get_class_name() const;        // "java.lang.String", "int", "int[]"
    std::string get_internal_name() const;     // "java/lang/String" (an array: its descriptor)
    std::string get_descriptor() const;
    std::vector<Type> get_argument_types() const { return get_argument_types(get_descriptor()); }
    Type get_return_type() const { return get_return_type(get_descriptor()); }
    int get_size() const { return sort_ == VOID ? 0 : (sort_ == LONG || sort_ == DOUBLE) ? 2 : 1; }
    int get_opcode(int opcode) const;          // the IALOAD/IASTORE/ILOAD/ISTORE/IADD.../IRETURN variant for this type

    bool operator==(const Type& o) const { return sort_ == o.sort_ && get_descriptor() == o.get_descriptor(); }

    static const Type VOID_TYPE, BOOLEAN_TYPE, CHAR_TYPE, BYTE_TYPE, SHORT_TYPE, INT_TYPE, FLOAT_TYPE, LONG_TYPE, DOUBLE_TYPE;

private:
    Type(int sort, std::string descriptor) : sort_(sort), descriptor_(std::move(descriptor)) {}
    int sort_ = VOID;
    std::string descriptor_;   // the whole descriptor ("I", "Ljava/lang/String;", "[I", "(I)V")
};

// A method handle constant (org.objectweb.asm.Handle).
struct Handle {
    int tag = 0;   // op::H_*
    std::string owner, name, desc;
    bool is_interface = false;
    bool operator==(const Handle& o) const { return tag == o.tag && owner == o.owner && name == o.name && desc == o.desc && is_interface == o.is_interface; }
};

struct ConstantDynamic;

// A loadable constant (an ldc's, a bootstrap argument, a field's ConstantValue): Integer, Float, Long, Double, String, Type, Handle, ConstantDynamic.
using Constant = std::variant<std::int32_t, float, std::int64_t, double, std::string, Type, Handle, std::shared_ptr<ConstantDynamic>>;

struct ConstantDynamic {
    std::string name, descriptor;
    Handle bootstrap_method;
    std::vector<Constant> bootstrap_method_arguments;
};

// ---- annotations

// An annotation's element value: a constant (boxed as Java's: Byte, Boolean, Character, Short, Integer, Long, Float, Double, String), an enum value
// [descriptor, name], a class (Type), a nested annotation, or an array of values.
struct AnnotationNode;
struct EnumValue {
    std::string desc, value;
};
struct AnnotationValue {
    enum Kind { BYTE, BOOLEAN, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE, STRING, ENUM, CLASS, ANNOTATION, ARRAY };
    Kind kind = INT;
    std::int64_t integral = 0;   // BYTE .. LONG (a boolean 0/1, a char its code)
    double floating = 0;         // FLOAT, DOUBLE
    std::string text;            // STRING
    EnumValue enum_value;        // ENUM
    std::optional<Type> type;    // CLASS
    std::shared_ptr<AnnotationNode> annotation;   // ANNOTATION
    std::vector<AnnotationValue> values;          // ARRAY
};

struct AnnotationNode {
    std::string desc;
    std::vector<std::pair<std::string, AnnotationValue>> values;   // name -> value, in order
};

// A type annotation: the annotation plus where it applies, kept as read: `type_ref` the target_type byte, `type_path` the target_info and type_path
// bytes that follow it (they name no constants). Those of code (on an instruction, a try-catch block or a local variable: their ranges are code
// offsets) are not kept.
struct TypeAnnotationNode : AnnotationNode {
    std::uint32_t type_ref = 0;
    Bytes type_path;
};

// ---- instructions

class LabelNode;

class InsnNode {   // org.objectweb.asm.tree.AbstractInsnNode
public:
    enum Kind { INSN, INT_INSN, VAR_INSN, TYPE_INSN, FIELD_INSN, METHOD_INSN, INVOKE_DYNAMIC_INSN, JUMP_INSN, LABEL, LDC_INSN, IINC_INSN,
                TABLESWITCH_INSN, LOOKUPSWITCH_INSN, MULTIANEWARRAY_INSN, FRAME, LINE };
    virtual ~InsnNode() = default;
    Kind get_type() const { return kind; }
    int get_opcode() const { return opcode; }   // -1 for a label, a line number or a frame
    InsnNode* get_next() const { return next; }
    InsnNode* get_previous() const { return previous; }

    Kind kind;
    int opcode;
    InsnNode* previous = nullptr;
    InsnNode* next = nullptr;

protected:
    InsnNode(Kind k, int op) : kind(k), opcode(op) {}
};

// Each kind of instruction (org.objectweb.asm.tree.*InsnNode); the fields are ASM's.
struct ZeroInsnNode : InsnNode {   // ASM's InsnNode: no operand
    explicit ZeroInsnNode(int op) : InsnNode(INSN, op) {}
};
struct IntInsnNode : InsnNode {     // bipush, sipush, newarray
    IntInsnNode(int op, int operand_) : InsnNode(INT_INSN, op), operand(operand_) {}
    int operand;
};
struct VarInsnNode : InsnNode {     // xload, xstore, ret
    VarInsnNode(int op, int var_) : InsnNode(VAR_INSN, op), var(var_) {}
    int var;
};
struct TypeInsnNode : InsnNode {    // new, anewarray, checkcast, instanceof
    TypeInsnNode(int op, std::string desc_) : InsnNode(TYPE_INSN, op), desc(std::move(desc_)) {}
    std::string desc;   // an internal name
};
struct FieldInsnNode : InsnNode {
    FieldInsnNode(int op, std::string owner_, std::string name_, std::string desc_)
        : InsnNode(FIELD_INSN, op), owner(std::move(owner_)), name(std::move(name_)), desc(std::move(desc_)) {}
    std::string owner, name, desc;
};
struct MethodInsnNode : InsnNode {
    MethodInsnNode(int op, std::string owner_, std::string name_, std::string desc_, bool itf_)
        : InsnNode(METHOD_INSN, op), owner(std::move(owner_)), name(std::move(name_)), desc(std::move(desc_)), itf(itf_) {}
    std::string owner, name, desc;
    bool itf;
};
struct InvokeDynamicInsnNode : InsnNode {
    InvokeDynamicInsnNode(std::string name_, std::string desc_, Handle bsm_, std::vector<Constant> bsm_args_)
        : InsnNode(INVOKE_DYNAMIC_INSN, op::INVOKEDYNAMIC), name(std::move(name_)), desc(std::move(desc_)), bsm(std::move(bsm_)), bsm_args(std::move(bsm_args_)) {}
    std::string name, desc;
    Handle bsm;
    std::vector<Constant> bsm_args;
};
struct JumpInsnNode : InsnNode {
    JumpInsnNode(int op, LabelNode* label_) : InsnNode(JUMP_INSN, op), label(label_) {}
    LabelNode* label;
};
class LabelNode : public InsnNode {
public:
    LabelNode() : InsnNode(LABEL, -1) {}
};
struct LdcInsnNode : InsnNode {
    explicit LdcInsnNode(Constant cst_) : InsnNode(LDC_INSN, op::LDC), cst(std::move(cst_)) {}
    Constant cst;
};
struct IincInsnNode : InsnNode {
    IincInsnNode(int var_, int incr_) : InsnNode(IINC_INSN, op::IINC), var(var_), incr(incr_) {}
    int var, incr;
};
struct TableSwitchInsnNode : InsnNode {
    TableSwitchInsnNode(int min_, int max_, LabelNode* dflt_, std::vector<LabelNode*> labels_)
        : InsnNode(TABLESWITCH_INSN, op::TABLESWITCH), min(min_), max(max_), dflt(dflt_), labels(std::move(labels_)) {}
    int min, max;
    LabelNode* dflt;
    std::vector<LabelNode*> labels;
};
struct LookupSwitchInsnNode : InsnNode {
    LookupSwitchInsnNode(LabelNode* dflt_, std::vector<int> keys_, std::vector<LabelNode*> labels_)
        : InsnNode(LOOKUPSWITCH_INSN, op::LOOKUPSWITCH), dflt(dflt_), keys(std::move(keys_)), labels(std::move(labels_)) {}
    LabelNode* dflt;
    std::vector<int> keys;
    std::vector<LabelNode*> labels;
};
struct MultiANewArrayInsnNode : InsnNode {
    MultiANewArrayInsnNode(std::string desc_, int dims_) : InsnNode(MULTIANEWARRAY_INSN, op::MULTIANEWARRAY), desc(std::move(desc_)), dims(dims_) {}
    std::string desc;
    int dims;
};
struct LineNumberNode : InsnNode {
    LineNumberNode(int line_, LabelNode* start_) : InsnNode(LINE, -1), line(line_), start(start_) {}
    int line;
    LabelNode* start;
};

// A method's instructions (org.objectweb.asm.tree.InsnList): a doubly linked list of nodes the method owns. Every node a pass makes for a method is
// made by its list (`make`), so the list owns it whether or not it is linked in.
class InsnList {
public:
    InsnList() = default;
    InsnList(const InsnList&) = delete;
    InsnList& operator=(const InsnList&) = delete;
    InsnList(InsnList&&) = default;
    InsnList& operator=(InsnList&&) = default;

    template <class T, class... A>
    T* make(A&&... args) {
        auto p = std::make_unique<T>(std::forward<A>(args)...);
        T* raw = p.get();
        owned_.push_back(std::move(p));
        return raw;
    }

    int size() const { return size_; }
    InsnNode* get_first() const { return first_; }
    InsnNode* get_last() const { return last_; }
    InsnNode* get(int index) const;   // the index-th node (a walk: lists are short)
    int index_of(const InsnNode* node) const;
    bool contains(const InsnNode* node) const;

    void add(InsnNode* node);                         // at the end
    void add(InsnList& list);                         // all of `list`, moved to the end (list is left empty)
    void insert(InsnNode* node);                      // at the start
    void insert(InsnList& list);
    void insert(InsnNode* previous, InsnNode* node);  // after `previous`
    void insert(InsnNode* previous, InsnList& list);
    void insert_before(InsnNode* next, InsnNode* node);
    void insert_before(InsnNode* next, InsnList& list);
    void set(InsnNode* old_node, InsnNode* new_node); // replaces
    void remove(InsnNode* node);
    void clear();                                     // unlinks all (the nodes stay owned)

    // for (InsnNode* i : list) -- the node after the current one is read before the body runs, so the body may remove the current one
    struct iterator {
        InsnNode* at;
        InsnNode* following;
        InsnNode* operator*() const { return at; }
        iterator& operator++() { at = following; following = at ? at->next : nullptr; return *this; }
        bool operator!=(const iterator& o) const { return at != o.at; }
    };
    iterator begin() const { return {first_, first_ ? first_->next : nullptr}; }
    iterator end() const { return {nullptr, nullptr}; }

private:
    std::vector<std::unique_ptr<InsnNode>> owned_;
    InsnNode* first_ = nullptr;
    InsnNode* last_ = nullptr;
    int size_ = 0;
};

struct TryCatchBlockNode {
    LabelNode* start;
    LabelNode* end;
    LabelNode* handler;
    std::optional<std::string> type;   // none: any (a finally)
};

struct LocalVariableNode {
    std::string name, desc;
    std::optional<std::string> signature;
    LabelNode* start;
    LabelNode* end;
    int index;
};

struct ParameterNode {
    std::optional<std::string> name;
    int access = 0;
};

// ---- members and the class

struct FieldNode {
    int access = 0;
    std::string name, desc;
    std::optional<std::string> signature;
    std::optional<Constant> value;   // ConstantValue
    std::vector<AnnotationNode> visible_annotations, invisible_annotations;
    std::vector<TypeAnnotationNode> visible_type_annotations, invisible_type_annotations;
    std::vector<std::pair<std::string, Bytes>> attrs;   // the attributes not modelled: name -> bytes
};

struct MethodNode {
    int access = 0;
    std::string name, desc;
    std::optional<std::string> signature;
    std::vector<std::string> exceptions;
    std::vector<ParameterNode> parameters;   // MethodParameters
    std::vector<AnnotationNode> visible_annotations, invisible_annotations;
    std::vector<TypeAnnotationNode> visible_type_annotations, invisible_type_annotations;
    std::optional<AnnotationValue> annotation_default;
    int visible_annotable_parameter_count = 0, invisible_annotable_parameter_count = 0;
    std::vector<std::vector<AnnotationNode>> visible_parameter_annotations, invisible_parameter_annotations;
    std::vector<std::pair<std::string, Bytes>> attrs;   // not modelled
    InsnList instructions;
    std::vector<TryCatchBlockNode> try_catch_blocks;
    int max_stack = 0, max_locals = 0;   // as read; recomputed on writing
    std::vector<LocalVariableNode> local_variables;
    bool has_code() const { return instructions.size() > 0; }
};

struct InnerClassNode {
    std::string name;
    std::optional<std::string> outer_name, inner_name;
    int access = 0;
};

struct RecordComponentNode {
    std::string name, descriptor;
    std::optional<std::string> signature;
    std::vector<AnnotationNode> visible_annotations, invisible_annotations;
    std::vector<TypeAnnotationNode> visible_type_annotations, invisible_type_annotations;
    std::vector<std::pair<std::string, Bytes>> attrs;
};

// A module-info's Module attribute, kept as read (its names are not remapped: Minecraft has none).
struct ModuleNode {
    Bytes module_attribute;
};

struct ClassNode {
    int version = 0;   // minor << 16 | major, as ASM packs it
    int access = 0;
    std::string name;
    std::optional<std::string> signature;
    std::optional<std::string> super_name;   // none for java/lang/Object
    std::vector<std::string> interfaces;
    std::optional<std::string> source_file, source_debug;
    std::optional<ModuleNode> module;
    std::optional<std::string> outer_class, outer_method, outer_method_desc;   // EnclosingMethod
    std::vector<AnnotationNode> visible_annotations, invisible_annotations;
    std::vector<TypeAnnotationNode> visible_type_annotations, invisible_type_annotations;
    std::vector<std::pair<std::string, Bytes>> attrs;   // not modelled
    std::vector<InnerClassNode> inner_classes;
    std::optional<std::string> nest_host_class;
    std::vector<std::string> nest_members, permitted_subclasses;
    std::vector<RecordComponentNode> record_components;
    // (lists of members: a pass may push, erase and reorder; a node's address may change when the list grows, so hold an index, not a pointer)
    std::vector<FieldNode> fields;
    std::vector<MethodNode> methods;
};

// The class in `data` (ASM's ClassReader into a ClassNode, frames skipped). FormatError for a class that is damaged or not a class.
ClassNode read(ByteView data, std::string_view name = "class");
// The class file (ASM's ClassWriter with COMPUTE_MAXS: the pool built in the order the parts are written, max stack and locals computed).
Bytes write(const ClassNode& c);

}  // namespace cb::java::bytecode
