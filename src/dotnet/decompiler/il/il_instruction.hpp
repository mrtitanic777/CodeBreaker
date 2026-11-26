// dotnet/decompiler/il/il_instruction.hpp -- the ILAst: ILSpy's tree of IL instructions. Every node is an ILInstruction with an opcode,
// a result stack type, child slots and lazily computed flags; nodes know their parent and their index in it, and a tree that is
// connected to a function keeps each variable's load, store and address lists current.
//
//     ILArena arena;                                  // owns every node and variable made while it is current
//     ILArenaScope scope(arena);
//     auto* store = make<StLoc>(v, make<LdcI4>(1));
//     store->value()->replace_with(make<LdcI4>(2));
//     for (ILInstruction* inst : function->descendants()) ...   // post-order, as upstream
//     if (auto* ldloc = as<LdLoc>(inst)) ...                     // upstream's `inst is LdLoc ldloc`
//
// Ownership: an ILAst is a graph that transforms take apart and put together freely (upstream relies on the garbage collector), so
// nodes and variables belong to an ILArena, the one current on the thread when they were made (one per decompiled method, kept
// until its C# is written); the tree holds raw pointers. Upstream's `out` parameters are pointers (`inst->match_ld_loc(&v)`, as C#'s
// `inst.MatchLdLoc(out v)`), so they never clash with the overloads that compare (`inst->match_ld_loc(v)`).
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: ILInstruction, InstructionFlags, SlotInfo, StackType, PrimitiveType,
// InstructionCollection<T>, IInstructionWith*Operand, ILiftableInstruction, the OpCode enum and the Match* methods of Instructions.cs
// and Instructions/PatternMatching.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
// Not ported: the DEBUG-only enumerator counting and dirty marks.
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "dotnet/decompiler/output/text_output.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"
#include "dotnet/decompiler/util/decimal.hpp"

namespace cb::dotnet::decompiler::typesystem {
class IType;
class IMember;
class IMethod;
class IField;
}  // namespace cb::dotnet::decompiler::typesystem

namespace cb::dotnet::decompiler::il {

using typesystem::IField;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::IType;
using util::Interval;

class ILInstruction;
class ILVariable;
class ILFunction;
class Block;
class BlockContainer;
class ILVisitor;
class ILTransformContext;
struct ILAstWritingOptions;
enum class BinaryNumericOperator : std::uint8_t;
enum class ConversionKind : std::uint8_t;
namespace patterns {
struct Match;
struct ListMatch;
}  // namespace patterns
namespace transforms {
class ExtractionContext;
}

// ---- the arena -------------------------------------------------------------------------------------------------------------------

// What an ILArena owns: instructions and variables.
class ArenaObject {
public:
    virtual ~ArenaObject() = default;
};

// Owns the nodes and variables made while it is the thread's current arena (ILArenaScope); frees them all when destroyed.
class ILArena {
public:
    ILArena() = default;
    ILArena(const ILArena&) = delete;
    ILArena& operator=(const ILArena&) = delete;

    template <class T, class... Args>
    T* make(Args&&... args) {
        auto owned = std::make_unique<T>(std::forward<Args>(args)...);
        T* result = owned.get();
        owned_.push_back(std::move(owned));
        return result;
    }
    template <class T>
    T* keep(std::unique_ptr<T> owned) {
        T* result = owned.get();
        owned_.push_back(std::move(owned));
        return result;
    }
    std::size_t size() const { return owned_.size(); }

    // The thread's current arena (an error when there is none).
    static ILArena& current();
    static ILArena* current_or_null();

private:
    friend class ILArenaScope;
    std::vector<std::unique_ptr<ArenaObject>> owned_;
};

// Makes an arena the thread's current one for its lifetime.
class ILArenaScope {
public:
    explicit ILArenaScope(ILArena& arena);
    ~ILArenaScope();
    ILArenaScope(const ILArenaScope&) = delete;
    ILArenaScope& operator=(const ILArenaScope&) = delete;

private:
    ILArena* previous_;
};

// A node or variable made in the current arena (upstream's `new T(...)`).
template <class T, class... Args>
T* make(Args&&... args) {
    return ILArena::current().make<T>(std::forward<Args>(args)...);
}

// ---- small types -----------------------------------------------------------------------------------------------------------------

enum class OpCode : std::uint8_t {
    InvalidBranch, InvalidExpression, Nop, ILFunction, BlockContainer, Block, PinnedRegion, BinaryNumericInstruction,
    NumericCompoundAssign, UserDefinedCompoundAssign, DynamicCompoundAssign, BitNot, Arglist, Branch, Leave, IfInstruction,
    NullCoalescingInstruction, SwitchInstruction, SwitchSection, TryCatch, TryCatchHandler, TryFinally, TryFault, LockInstruction,
    UsingInstruction, DebugBreak, Comp, Call, CallVirt, CallIndirect, Ckfinite, Conv, LdLoc, LdLoca, StLoc, AddressOf,
    ThreeValuedBoolAnd, ThreeValuedBoolOr, NullableUnwrap, NullableRewrap, LdStr, LdStrUtf8, LdcI4, LdcI8, LdcF4, LdcF8, LdcDecimal,
    LdNull, LdFtn, LdVirtFtn, LdVirtDelegate, LdTypeToken, LdMemberToken, LocAlloc, LocAllocSpan, Cpblk, Initblk, LdFlda, LdsFlda,
    CastClass, IsInst, LdObj, LdObjIfRef, StObj, Box, Unbox, UnboxAny, NewObj, NewArr, DefaultValue, Throw, Rethrow, SizeOf, LdLen,
    LdElema, LdElemaInlineArray, GetPinnableReference, StringToInt, ExpressionTreeCast, UserDefinedLogicOperator,
    DynamicLogicOperatorInstruction, DynamicBinaryOperatorInstruction, DynamicUnaryOperatorInstruction, DynamicConvertInstruction,
    DynamicGetMemberInstruction, DynamicSetMemberInstruction, DynamicGetIndexInstruction, DynamicSetIndexInstruction,
    DynamicInvokeMemberInstruction, DynamicInvokeConstructorInstruction, DynamicInvokeInstruction, DynamicIsEventInstruction,
    MatchInstruction, MakeRefAny, RefAnyType, RefAnyValue, YieldReturn, Await, DeconstructInstruction, DeconstructResultInstruction,
    AnyNode,
};
inline constexpr int opcode_count = static_cast<int>(OpCode::AnyNode) + 1;

enum class InstructionFlags : std::uint32_t {
    None = 0,
    MayReadLocals = 0x10,
    MayWriteLocals = 0x20,
    SideEffect = 0x40,
    MayThrow = 0x100,
    MayBranch = 0x200,
    MayUnwrapNull = 0x400,
    EndPointUnreachable = 0x800,
    ControlFlow = 0x1000,
};
constexpr InstructionFlags operator|(InstructionFlags a, InstructionFlags b) {
    return static_cast<InstructionFlags>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
}
constexpr InstructionFlags operator&(InstructionFlags a, InstructionFlags b) {
    return static_cast<InstructionFlags>(static_cast<std::uint32_t>(a) & static_cast<std::uint32_t>(b));
}
constexpr InstructionFlags operator~(InstructionFlags a) { return static_cast<InstructionFlags>(~static_cast<std::uint32_t>(a)); }
constexpr InstructionFlags& operator|=(InstructionFlags& a, InstructionFlags b) { return a = a | b; }
constexpr InstructionFlags& operator&=(InstructionFlags& a, InstructionFlags b) { return a = a & b; }
constexpr bool any(InstructionFlags f) { return f != InstructionFlags::None; }

// The numeric order matters: where two branches meet with different stack types, the merged type is the larger.
enum class StackType : std::uint8_t { Unknown, I4, I, I8, F4, F8, O, Ref, Void };

// Values as srm::PrimitiveTypeCode's where they overlap.
enum class PrimitiveType : std::uint8_t {
    None = 0, I1 = 4, I2 = 6, I4 = 8, I8 = 10, R4 = 12, R8 = 13, U1 = 5, U2 = 7, U4 = 9, U8 = 11, I = 24, U = 25, Ref = 16, R = 254,
    Unknown = 255,
};

enum class ILPhase : std::uint8_t { InILReader, Normal, InAsyncAwait };

// A child slot's name and whether an expression may be inlined into it.
struct SlotInfo {
    std::string name;
    bool can_inline_into = false;
    bool is_collection = false;
    static const SlotInfo none;
    const std::string& to_string() const { return name; }
};

// ---- interfaces some instructions implement --------------------------------------------------------------------------------------

class IInstructionWithTypeOperand {
public:
    virtual ~IInstructionWithTypeOperand() = default;
    virtual const IType* type() const = 0;
};
class IInstructionWithFieldOperand {
public:
    virtual ~IInstructionWithFieldOperand() = default;
    virtual const IField* field() const = 0;
};
class IInstructionWithMethodOperand {
public:
    virtual ~IInstructionWithMethodOperand() = default;
    virtual const IMethod* method() const = 0;
};
class ILiftableInstruction {
public:
    virtual ~ILiftableInstruction() = default;
    virtual bool is_lifted() const = 0;
    virtual StackType underlying_result_type() const = 0;
};
class ISupportsVolatilePrefix {
public:
    virtual ~ISupportsVolatilePrefix() = default;
    virtual bool is_volatile() const = 0;
    virtual void set_is_volatile(bool value) = 0;
};
class ISupportsUnalignedPrefix {
public:
    virtual ~ISupportsUnalignedPrefix() = default;
    virtual std::uint8_t unaligned_prefix() const = 0;
    virtual void set_unaligned_prefix(std::uint8_t value) = 0;
};

// An instruction that reads, writes or takes the address of a variable (its variable's lists hold it while it is connected).
class IInstructionWithVariableOperand {
public:
    virtual ~IInstructionWithVariableOperand() = default;
    virtual ILVariable* variable() const = 0;
    virtual void set_variable(ILVariable* value) = 0;
    virtual int index_in_variable_instruction_mapping() const = 0;
    virtual void set_index_in_variable_instruction_mapping(int value) = 0;
    virtual ILInstruction* as_instruction() = 0;
};
class IStoreInstruction : public IInstructionWithVariableOperand {
public:
    int index_in_store_instruction_list = -1;
    int index_in_variable_instruction_mapping() const override { return index_in_store_instruction_list; }
    void set_index_in_variable_instruction_mapping(int value) override { index_in_store_instruction_list = value; }
};
class ILoadInstruction : public IInstructionWithVariableOperand {
public:
    int index_in_load_instruction_list = -1;
    int index_in_variable_instruction_mapping() const override { return index_in_load_instruction_list; }
    void set_index_in_variable_instruction_mapping(int value) override { index_in_load_instruction_list = value; }
};
class IAddressInstruction : public IInstructionWithVariableOperand {
public:
    int index_in_address_instruction_list = -1;
    int index_in_variable_instruction_mapping() const override { return index_in_address_instruction_list; }
    void set_index_in_variable_instruction_mapping(int value) override { index_in_address_instruction_list = value; }
};

// ---- ILInstruction ---------------------------------------------------------------------------------------------------------------

class ILInstruction : public ArenaObject {
public:
    const OpCode op_code;

    ILInstruction& operator=(const ILInstruction&) = delete;

    // --- the tree
    ILInstruction* parent() const { return parent_; }
    int child_index() const { return child_index_; }
    void set_child_index(int value) { child_index_ = value; }
    // The slot this node is in (nullptr at the root).
    const SlotInfo* slot_info() const;
    bool is_descendant_of(const ILInstruction* possible_ancestor) const;
    ILInstruction* get_common_parent(ILInstruction* other);
    // Whether this node comes before `other` in a post-order walk.
    bool is_before(const ILInstruction* other) const;
    // Puts `replacement` where this node is. (Upstream's remarks: the replacement must not contain this node; clone it first.)
    void replace_with(ILInstruction* replacement);

    class ChildrenCollection;
    ChildrenCollection children();
    class DescendantsRange;
    // Every node below this one and then this one, in post-order (lazily, as upstream: a node replaced during the walk is seen as
    // the walk reaches its slot).
    DescendantsRange descendants();
    std::vector<ILInstruction*> ancestors();

    virtual int get_child_count() const = 0;
    virtual ILInstruction* get_child(int index) const = 0;
    virtual void set_child(int index, ILInstruction* value) = 0;
    virtual const SlotInfo& get_child_slot(int index) const = 0;

    // --- types and flags
    virtual StackType result_type() const = 0;
    static StackType common_result_type(StackType a, StackType b);
    InstructionFlags flags() const;
    bool has_flag(InstructionFlags f) const { return any(flags() & f); }
    bool has_direct_flag(InstructionFlags f) const { return any(direct_flags() & f); }
    virtual InstructionFlags direct_flags() const = 0;

    // --- IL ranges
    void add_il_range(Interval new_range) { il_range_ = combine_il_range(il_range_, new_range); }
    void add_il_range(const ILInstruction& source) { add_il_range(source.il_range_); }
    void set_il_range(const ILInstruction& source) { il_range_ = source.il_range_; }
    void set_il_range(Interval range) { il_range_ = range; }
    int start_il_offset() const { return il_range_.start; }
    int end_il_offset() const { return il_range_.end; }
    bool il_range_is_empty() const { return il_range_.is_empty(); }
    std::vector<Interval> il_ranges() const { return {il_range_}; }
    void write_il_range(ITextOutput& output, const ILAstWritingOptions& options) const;

    // --- text
    void write_to(ITextOutput& output, const ILAstWritingOptions& options) const;
    std::string to_string() const;

    // --- visitors (ILVisitor; the generic ILVisitor<T> and ILVisitor<C, T> dispatch in il_visitor.hpp)
    void accept_visitor(ILVisitor& visitor);

    // --- copies and patterns
    virtual ILInstruction* clone() const = 0;
    patterns::Match match(ILInstruction* node);
    virtual bool perform_match(ILInstruction* other, patterns::Match& match) = 0;
    virtual bool perform_match(patterns::ListMatch& list_match, patterns::Match& match);

    // --- inlining (ILInlining, ILExtraction; upstream's Extract(ILTransformContext) comes with the transforms, stage 6)
    virtual bool prepare_extract(int child_index, transforms::ExtractionContext& ctx);
    virtual bool can_inline_into_slot(int child_index, ILInstruction* expression_being_moved);
    virtual bool satisfies_slot_restriction_for_inlining(int child_index, ILInstruction* new_child);

    // --- debug checks (upstream's [Conditional("DEBUG")]; called by tests and debug builds)
    virtual void check_invariant(ILPhase phase);

    // --- used by InstructionCollection<T>
    void instruction_collection_added(ILInstruction* new_child);
    void instruction_collection_removed(ILInstruction* old_child);
    virtual void instruction_collection_update_complete();
    bool is_connected() const { return ref_count_ > 0; }
    void add_ref();
    void release_ref();

    // --- the Match* methods (Instructions.cs's generated ones and PatternMatching.cs's)
    bool match_invalid_branch() const;
    bool match_invalid_expression() const;
    bool match_nop() const;
    bool match_pinned_region(ILVariable** variable, ILInstruction** init, ILInstruction** body) const;
    bool match_arglist() const;
    bool match_try_catch_handler(ILInstruction** filter, ILInstruction** body, ILVariable** variable) const;
    bool match_try_catch_handler(ILVariable** variable) const;
    bool match_lock_instruction(ILInstruction** on_expression, ILInstruction** body) const;
    bool match_using_instruction(ILVariable** variable, ILInstruction** resource_expression, ILInstruction** body) const;
    bool match_debug_break() const;
    bool match_ckfinite(ILInstruction** argument) const;
    bool match_ld_loc(ILVariable** variable) const;
    bool match_ld_loc(const ILVariable* variable) const;
    bool match_ld_loca(ILVariable** variable) const;
    bool match_ld_loca(const ILVariable* variable) const;
    // ldloc (a reference-typed variable) or ldloca
    bool match_ld_loc_ref(ILVariable** variable) const;
    bool match_ld_loc_ref(const ILVariable* variable) const;
    bool match_ld_this() const;
    bool match_st_loc(ILVariable** variable, ILInstruction** value) const;
    bool match_st_loc(ILVariable** variable) const;
    bool match_st_loc(const ILVariable* variable, ILInstruction** value) const;
    bool match_address_of(ILInstruction** value, const IType** type) const;
    bool match_three_valued_bool_and(ILInstruction** left, ILInstruction** right) const;
    bool match_three_valued_bool_or(ILInstruction** left, ILInstruction** right) const;
    bool match_nullable_rewrap(ILInstruction** argument) const;
    bool match_ld_str(std::string* value) const;
    bool match_ld_str_utf8(std::string* value) const;
    bool match_ldc_i4(std::int32_t* value) const;
    bool match_ldc_i4(std::int32_t value) const;
    bool match_ldc_i8(std::int64_t* value) const;
    bool match_ldc_f4(float* value) const;
    bool match_ldc_f4(float value) const;
    bool match_ldc_f8(double* value) const;
    bool match_ldc_f8(double value) const;
    bool match_ldc_decimal(util::Decimal* value) const;
    // ldc.i4, ldc.i8 or conv of one to native int
    bool match_ldc_i(std::int64_t* value) const;
    bool match_ldc_i(std::int64_t value) const;
    bool match_ld_null() const;
    bool match_ld_ftn(const IMethod** method) const;
    bool match_ld_virt_ftn(ILInstruction** argument, const IMethod** method) const;
    bool match_ld_virt_delegate(ILInstruction** argument, const IType** type, const IMethod** method) const;
    bool match_ld_type_token(const IType** type) const;
    bool match_ld_member_token(const IMember** member) const;
    bool match_loc_alloc(ILInstruction** argument) const;
    bool match_loc_alloc_span(ILInstruction** argument, const IType** type) const;
    bool match_cpblk(ILInstruction** dest_address, ILInstruction** source_address, ILInstruction** size) const;
    bool match_initblk(ILInstruction** address, ILInstruction** value, ILInstruction** size) const;
    bool match_ld_flda(ILInstruction** target, const IField** field) const;
    bool match_lds_flda(const IField** field) const;
    bool match_cast_class(ILInstruction** argument, const IType** type) const;
    bool match_is_inst(ILInstruction** argument, const IType** type) const;
    bool match_ld_obj(ILInstruction** target, const IType** type) const;
    bool match_ld_obj(ILInstruction** target, const IType* type) const;
    bool match_ld_obj_if_ref(ILInstruction** target, const IType** type) const;
    bool match_st_obj(ILInstruction** target, ILInstruction** value, const IType** type) const;
    bool match_box(ILInstruction** argument, const IType** type) const;
    bool match_unbox(ILInstruction** argument, const IType** type) const;
    bool match_unbox_any(ILInstruction** argument, const IType** type) const;
    bool match_new_arr(const IType** type) const;
    bool match_default_value(const IType** type) const;
    bool match_throw(ILInstruction** argument) const;
    bool match_rethrow() const;
    bool match_size_of(const IType** type) const;
    bool match_ld_elema(const IType** type, ILInstruction** array) const;
    bool match_ld_elema_inline_array(const IType** type, ILInstruction** array) const;
    bool match_get_pinnable_reference(ILInstruction** argument, const IMethod** method) const;
    bool match_user_defined_logic_operator(const IMethod** method, ILInstruction** left, ILInstruction** right) const;
    bool match_match_instruction(ILVariable** variable, const IMethod** method, ILInstruction** tested_operand) const;
    bool match_make_ref_any(ILInstruction** argument, const IType** type) const;
    bool match_ref_any_type(ILInstruction** argument) const;
    bool match_ref_any_value(ILInstruction** argument, const IType** type) const;
    bool match_yield_return(ILInstruction** value) const;
    bool match_await(ILInstruction** value) const;
    bool match_ld_len(StackType type, ILInstruction** array) const;
    // leave of the function's outermost container
    bool match_return(ILInstruction** value) const;
    bool match_branch(Block** target_block) const;
    bool match_branch(const Block* target_block) const;
    bool match_leave(BlockContainer** target_container, ILInstruction** value) const;
    bool match_leave(const BlockContainer* target_container, ILInstruction** value) const;
    bool match_leave(BlockContainer** target_container) const;
    bool match_leave(const BlockContainer* target_container) const;
    bool match_if_instruction(ILInstruction** condition, ILInstruction** true_inst, ILInstruction** false_inst) const;
    bool match_if_instruction_positive_condition(ILInstruction** condition, ILInstruction** true_inst, ILInstruction** false_inst) const;
    // if without else (the false instruction is a nop)
    bool match_if_instruction(ILInstruction** condition, ILInstruction** true_inst) const;
    bool match_logic_and(ILInstruction** lhs, ILInstruction** rhs) const;
    bool match_logic_or(ILInstruction** lhs, ILInstruction** rhs) const;
    bool match_logic_not(ILInstruction** arg) const;
    bool match_comp_equals(ILInstruction** left, ILInstruction** right) const;
    bool match_comp_equals_null(ILInstruction** arg) const;
    bool match_comp_not_equals_null(ILInstruction** arg) const;
    bool match_comp_not_equals(ILInstruction** left, ILInstruction** right) const;
    bool match_ld_fld(ILInstruction** target, const IField** field) const;
    bool match_lds_fld(const IField** field) const;
    bool match_lds_fld(const IField* field) const;
    bool match_sts_fld(const IField** field, ILInstruction** value) const;
    bool match_st_fld(ILInstruction** target, const IField** field, ILInstruction** value) const;
    bool match_binary_numeric_instruction(BinaryNumericOperator op) const;
    bool match_binary_numeric_instruction(BinaryNumericOperator op, ILInstruction** left, ILInstruction** right) const;
    bool match_binary_numeric_instruction(BinaryNumericOperator* op, ILInstruction** left, ILInstruction** right) const;
    bool match_default_or_null_or_zero() const;
    // The argument of the conv of `kind` this is, or this.
    virtual ILInstruction* unwrap_conv(ConversionKind kind);

protected:
    explicit ILInstruction(OpCode op_code) : op_code(op_code) {}
    // Upstream's ShallowClone (MemberwiseClone): the fields copied, the copy disconnected, without a parent and with flags to compute.
    ILInstruction(const ILInstruction& other);

    void validate_child(ILInstruction* inst) const;
    // Puts `new_value` in the child field `child_pointer` (slot `index`), keeping parents and reference counts.
    template <class T>
    void set_child_instruction(T*& child_pointer, T* new_value, int index);

    void invalidate_flags();
    virtual InstructionFlags compute_flags() const = 0;
    virtual void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const = 0;
    virtual void connected();
    virtual void disconnected();
    static Interval combine_il_range(Interval old_range, Interval new_range);
    static constexpr InstructionFlags invalid_flags = static_cast<InstructionFlags>(0xFFFFFFFFu);

private:
    void set_parent(ILInstruction* p) { parent_ = p; }
    int count_ancestors() const;

    ILInstruction* parent_ = nullptr;
    int child_index_ = -1;
    mutable InstructionFlags flags_ = invalid_flags;
    Interval il_range_;
    std::uint8_t ref_count_ = 0;
};

// Children by index (live: reads the node each time).
class ILInstruction::ChildrenCollection {
public:
    explicit ChildrenCollection(ILInstruction* inst) : inst_(inst) {}
    int count() const { return inst_->get_child_count(); }
    ILInstruction* operator[](int index) const { return inst_->get_child(index); }
    void set(int index, ILInstruction* value) const { inst_->set_child(index, value); }

    class iterator {
    public:
        using value_type = ILInstruction*;
        using difference_type = std::ptrdiff_t;
        iterator(ILInstruction* inst, int pos) : inst_(inst), pos_(pos) {}
        ILInstruction* operator*() const { return inst_->get_child(pos_); }
        iterator& operator++() {
            ++pos_;
            return *this;
        }
        bool operator==(const iterator& other) const { return pos_ == other.pos_; }

    private:
        ILInstruction* inst_;
        int pos_;
    };
    // The end is fixed when the walk starts (upstream's ChildrenEnumerator).
    iterator begin() const { return {inst_, 0}; }
    iterator end() const { return {inst_, count()}; }

private:
    ILInstruction* inst_;
};
inline ILInstruction::ChildrenCollection ILInstruction::children() { return ChildrenCollection(this); }

// The post-order walk of descendants().
class ILInstruction::DescendantsRange {
public:
    explicit DescendantsRange(ILInstruction* root) : root_(root) {}
    class iterator {
    public:
        using value_type = ILInstruction*;
        using difference_type = std::ptrdiff_t;
        iterator() = default;
        explicit iterator(ILInstruction* root);
        ILInstruction* operator*() const { return current_; }
        iterator& operator++() {
            advance();
            return *this;
        }
        bool operator==(const iterator& other) const { return current_ == other.current_ && done_ == other.done_; }

    private:
        struct Frame {
            ILInstruction* inst;
            int pos;
            int end;
        };
        void advance();
        std::vector<Frame> stack_;
        ILInstruction* root_ = nullptr;
        ILInstruction* current_ = nullptr;
        bool done_ = true;
    };
    iterator begin() const { return iterator(root_); }
    iterator end() const { return iterator(); }

private:
    ILInstruction* root_;
};
inline ILInstruction::DescendantsRange ILInstruction::descendants() { return DescendantsRange(this); }

template <class T>
void ILInstruction::set_child_instruction(T*& child_pointer, T* new_value, int index) {
    T* old_value = child_pointer;
    if (old_value == new_value && new_value != nullptr && new_value->parent_ == this && new_value->child_index_ == index) return;
    child_pointer = new_value;
    if (new_value != nullptr) {
        new_value->parent_ = this;
        new_value->child_index_ = index;
    }
    invalidate_flags();
    if (ref_count_ > 0) {
        // The new value may be a subtree of the old one: AddRef first, so the part that stays connected is not disconnected and
        // connected again.
        if (new_value != nullptr) new_value->add_ref();
        if (old_value != nullptr) old_value->release_ref();
    }
}

// ---- casts -----------------------------------------------------------------------------------------------------------------------

namespace detail {
template <class T>
concept HasOpCode = requires { T::opcode; };
}
// `inst` as a T, or nullptr (upstream's `inst as T` / `inst is T t`): by opcode for the sealed instruction classes (they declare
// `static constexpr OpCode opcode`), by dynamic_cast for the abstract ones and the interfaces.
template <class T>
T* as(ILInstruction* inst) {
    if (inst == nullptr) return nullptr;
    if constexpr (detail::HasOpCode<T>)
        return inst->op_code == T::opcode ? static_cast<T*>(inst) : nullptr;
    else
        return dynamic_cast<T*>(inst);
}
template <class T>
const T* as(const ILInstruction* inst) {
    return as<T>(const_cast<ILInstruction*>(inst));
}
template <class T>
bool is(const ILInstruction* inst) {
    return as<T>(inst) != nullptr;
}

// ---- InstructionCollection<T> -----------------------------------------------------------------------------------------------------

// The children of a collection slot (a block's instructions, a container's blocks ...), keeping parents, child indices and reference
// counts as the list changes.
template <class T>
class InstructionCollection {
public:
    InstructionCollection(ILInstruction* parent_instruction, int first_child_index)
        : parent_(parent_instruction), first_child_index_(first_child_index) {}
    InstructionCollection(const InstructionCollection&) = delete;
    InstructionCollection& operator=(const InstructionCollection&) = delete;

    int count() const { return static_cast<int>(list_.size()); }
    bool empty() const { return list_.empty(); }
    T* operator[](int index) const { return list_.at(static_cast<std::size_t>(index)); }
    void set(int index, T* value) {
        T* old_value = list_.at(static_cast<std::size_t>(index));
        if (!(old_value == value && value->parent() == parent_ && value->child_index() == index)) {
            list_[static_cast<std::size_t>(index)] = value;
            value->set_child_index(index + first_child_index_);
            parent_->instruction_collection_added(value);
            parent_->instruction_collection_removed(old_value);
            parent_->instruction_collection_update_complete();
        }
    }
    auto begin() const { return list_.begin(); }
    auto end() const { return list_.end(); }
    const std::vector<T*>& items() const { return list_; }

    int index_of(const ILInstruction* item) const {
        if (item == nullptr) return -1;
        // the item's primary position first, then a search (the ILAst may hold the item in several places for a while)
        int index = item->child_index() - first_child_index_;
        if (index >= 0 && index < count() && list_[static_cast<std::size_t>(index)] == item) return index;
        for (std::size_t i = 0; i < list_.size(); ++i)
            if (list_[i] == item) return static_cast<int>(i);
        return -1;
    }
    bool contains(const ILInstruction* item) const { return index_of(item) >= 0; }

    void add(T* value) {
        value->set_child_index(count() + first_child_index_);
        list_.push_back(value);
        parent_->instruction_collection_added(value);
        parent_->instruction_collection_update_complete();
    }
    template <class Range>
    void add_range(const Range& values) {
        for (auto* value : values) {
            value->set_child_index(count() + first_child_index_);
            list_.push_back(value);
            parent_->instruction_collection_added(value);
        }
        parent_->instruction_collection_update_complete();
    }
    template <class Range>
    void replace_list(const Range& new_list) {
        int index = 0;
        for (auto* value : new_list) {
            value->set_child_index(index + first_child_index_);
            if (index < count()) {
                T* old_value = list_[static_cast<std::size_t>(index)];
                list_[static_cast<std::size_t>(index)] = value;
                parent_->instruction_collection_added(value);
                parent_->instruction_collection_removed(old_value);
            } else {
                list_.push_back(value);
                parent_->instruction_collection_added(value);
            }
            ++index;
        }
        for (int i = index; i < count(); ++i) parent_->instruction_collection_removed(list_[static_cast<std::size_t>(i)]);
        list_.resize(static_cast<std::size_t>(index));
        parent_->instruction_collection_update_complete();
    }
    void insert(int index, T* item) {
        list_.insert(list_.begin() + index, item);
        item->set_child_index(index);
        parent_->instruction_collection_added(item);
        for (int i = index + 1; i < count(); ++i) {
            T* other = list_[static_cast<std::size_t>(i)];
            // only where that is the item's primary position (it may have several parents for a while)
            if (other->parent() == parent_ && other->child_index() == i + first_child_index_ - 1) other->set_child_index(i + first_child_index_);
        }
        parent_->instruction_collection_update_complete();
    }
    void remove_at(int index) {
        parent_->instruction_collection_removed(list_.at(static_cast<std::size_t>(index)));
        list_.erase(list_.begin() + index);
        for (int i = index; i < count(); ++i) {
            T* other = list_[static_cast<std::size_t>(i)];
            if (other->parent() == parent_ && other->child_index() == i + first_child_index_ + 1) other->set_child_index(i + first_child_index_);
        }
        parent_->instruction_collection_update_complete();
    }
    // Removes the item at `index`, moving the last item there.
    void swap_remove_at(int index) {
        parent_->instruction_collection_removed(list_.at(static_cast<std::size_t>(index)));
        int remove_index = count() - 1;
        T* moved = list_[static_cast<std::size_t>(index)] = list_[static_cast<std::size_t>(remove_index)];
        list_.pop_back();
        if (moved->parent() == parent_ && moved->child_index() == remove_index + first_child_index_) moved->set_child_index(index + first_child_index_);
        parent_->instruction_collection_update_complete();
    }
    void clear() {
        for (T* entry : list_) parent_->instruction_collection_removed(entry);
        list_.clear();
        parent_->instruction_collection_update_complete();
    }
    bool remove(const ILInstruction* item) {
        int index = index_of(item);
        if (index < 0) return false;
        remove_at(index);
        return true;
    }
    void remove_range(int index, int count_) {
        for (int i = 0; i < count_; ++i) parent_->instruction_collection_removed(list_.at(static_cast<std::size_t>(index + i)));
        list_.erase(list_.begin() + index, list_.begin() + index + count_);
        for (int i = index; i < count(); ++i) {
            T* other = list_[static_cast<std::size_t>(i)];
            if (other->parent() == parent_ && other->child_index() == i + first_child_index_ + count_) other->set_child_index(i + first_child_index_);
        }
        parent_->instruction_collection_update_complete();
    }
    template <class Predicate>
    int remove_all(Predicate predicate) {
        std::size_t j = 0;
        for (std::size_t i = 0; i < list_.size(); ++i) {
            T* item = list_[i];
            if (predicate(item)) {
                parent_->instruction_collection_removed(item);
            } else {
                if (item->parent() == parent_ && item->child_index() == static_cast<int>(i) + first_child_index_)
                    item->set_child_index(static_cast<int>(j) + first_child_index_);
                list_[j++] = item;
            }
        }
        int removed = static_cast<int>(list_.size() - j);
        if (removed > 0) {
            list_.resize(j);
            parent_->instruction_collection_update_complete();
        }
        return removed;
    }
    void move_element_to_index(int old_index, int new_index) {
        T* item = list_.at(static_cast<std::size_t>(old_index));
        insert(new_index, item);
        remove_at(old_index < new_index ? old_index : old_index + 1);
    }
    void move_element_to_index(T* item, int new_index) {
        int old_index = index_of(item);
        if (old_index >= 0) {
            insert(new_index, item);
            remove_at(old_index < new_index ? old_index : old_index + 1);
        }
    }
    void move_element_to_end(int index) { move_element_to_index(index, count()); }
    void move_element_to_end(T* item) { move_element_to_index(item, count()); }

    T* first() const { return list_.at(0); }
    T* first_or_default() const { return list_.empty() ? nullptr : list_.front(); }
    T* last() const { return list_.at(list_.size() - 1); }
    T* last_or_default() const { return list_.empty() ? nullptr : list_.back(); }
    T* second_to_last_or_default() const { return list_.size() > 1 ? list_[list_.size() - 2] : nullptr; }
    T* element_at_or_default(int index) const { return index >= 0 && index < count() ? list_[static_cast<std::size_t>(index)] : nullptr; }

private:
    ILInstruction* parent_;
    int first_child_index_;
    std::vector<T*> list_;
};

}  // namespace cb::dotnet::decompiler::il
