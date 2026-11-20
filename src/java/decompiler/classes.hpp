// java/decompiler/classes.hpp -- the classes level of the decompiler: the tree of classes in a file (root, member, local, anonymous and lambda classes)
// and the wrapper that holds each class's decompiled methods; the passes that run on a whole class (field and constructor initializers, enums,
// assertions, `Foo.class` for old class files, lambdas, nested-class variables and member access); and the driver that decompiles each class of the
// loaded files to text.
//
//     Fernflower ff(saver, options, logger);
//     ff.add_space("game.jar", /*is_own=*/true);
//     ff.decompile_context();                          // every own class decompiled and handed to `saver`
//
// Ported from Fernflower (de.fernflower.main: ClassesProcessor and its ClassNode, ClassWriter's neighbours InitializerProcessor, EnumProcessor,
// AssertProcessor, ClassReference14Processor, Fernflower; main.rels: ClassWrapper, MethodWrapper, MethodProcessorRunnable, LambdaProcessor,
// NestedClassProcessor, NestedMemberAccess; struct.IDecompiledData), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this
// folder). ClassWriter is in class_writer.hpp. Classes, wrappers and nodes are objects of the run's arena (make<T>): pointers are the original's
// references. Differences from the original: a method's worker thread (MAX_PROCESSING_METHOD) cannot be killed in C++, so a method over its time limit is logged and waited for (its result is dropped);
// (see MethodProcessorRunnable); the original's StructClass.releaseResources and
// StructMethod.expandData (lazy loading) have nothing to do here.
#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "java/decompiler/collectors.hpp"
#include "java/decompiler/context.hpp"
#include "java/decompiler/decompiler.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/generics.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/decompiler/keyed_list.hpp"
#include "java/decompiler/structs.hpp"
#include "java/decompiler/text_buffer.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

// Other layers (their headers are not needed here).
class Statement;
class IfStatement;
class RootStatement;
class DirectGraph;   // modules/decompiler/flow (the flattened statements of a method)

class ClassWrapper;
class MethodWrapper;
class ClassesProcessor;
class ClassWriter;
class IdentifierConverter;   // renamer.hpp

// A method's signature fields: for each parameter of a constructor of a local or anonymous class the outer variable it stands for (none: a real
// parameter). The list itself is none when the class has no such analysis.
using SignatureFields = std::optional<std::vector<std::optional<VarVersionPair>>>;

// A class of the file being decompiled, in the tree of its nested classes (the original's ClassesProcessor.ClassNode).
struct ClassNode : ArenaObject {
    static constexpr int CLASS_ROOT = 0, CLASS_MEMBER = 1, CLASS_ANONYMOUS = 2, CLASS_LOCAL = 4, CLASS_LAMBDA = 8;
    static constexpr bool kept_across_classes = true;   // (a lambda's node, made while its class is written, joins the tree for good)

    // What a lambda class stands for: the invokedynamic's functional interface method and the method that has the lambda's body.
    struct LambdaInformation {
        std::string class_name;
        std::string method_name;
        std::string method_descriptor;
        std::string content_class_name;
        std::string content_method_name;
        std::string content_method_descriptor;
        int content_method_invocation_type = 0;   // values from CONSTANT_MethodHandle_REF_*
        std::string content_method_key;
        bool is_method_reference = false;
        bool is_content_method_static = false;
    };

    int type = 0;
    int access = 0;
    std::optional<std::string> simple_name;
    const StructClass* class_struct = nullptr;
    std::optional<std::string> enclosing_method;      // "name descriptor"
    InvocationExprent* super_invocation = nullptr;
    JavaHashMap<std::string, VarVersionPair, JavaStringHash> map_fields_to_vars;
    std::optional<VarType> anonymous_class_type;
    std::vector<ClassNode*> nested;
    JavaHashSet<std::string, JavaStringHash> enclosing_classes;
    ClassNode* parent = nullptr;
    std::optional<LambdaInformation> lambda_information;
    bool nameless_constructor_stub = false;

    // A lambda class: `class_struct` is the class holding the lambda's code.
    ClassNode(const std::string& content_class_name, const std::string& content_method_name, const std::string& content_method_descriptor,
              int content_method_invocation_type, const std::string& lambda_class_name, const std::string& lambda_method_name,
              const std::string& lambda_method_descriptor, const StructClass* class_struct);
    ClassNode(int type, const StructClass* class_struct);

    // The directly nested class of that name; null when none.
    ClassNode* get_class_node(const std::string& qualified_name) const;
    // The wrapper of this class (a lambda's is its enclosing class's).
    ClassWrapper* get_wrapper() const;

private:
    friend class ClassesProcessor;
    ClassWrapper* wrapper_ = nullptr;
};

// The classes of the files being decompiled as a tree, and the entry of writing one file's classes.
class ClassesProcessor {
public:
    static constexpr int AVERAGE_CLASS_SIZE = 16 * 1024;

    // Makes a node for each own class and joins the nested ones to their enclosing classes (through each InnerClasses attribute; DECOMPILE_INNER).
    explicit ClassesProcessor(const StructContext& context);

    // Writes the source of `cl` (a root class: package, imports and the class with its nested ones) into `buffer`; nothing for a class that is nested in
    // another. Throws what a class that cannot be decompiled throws (Fernflower::get_class_content logs it).
    void write_class(const StructClass* cl, TextBuffer& buffer);

    // The class nodes by qualified name ("a/b/C$D"; a lambda's by its generated name): root classes, nested ones and lambdas.
    JavaHashMap<std::string, ClassNode*, JavaStringHash>& get_map_root_classes() { return map_root_classes_; }
    const JavaHashMap<std::string, ClassNode*, JavaStringHash>& get_map_root_classes() const { return map_root_classes_; }
    // getMapRootClasses().get(name): null when there is none.
    ClassNode* get_class_node(const std::string& qualified_name) const {
        ClassNode* const* p = map_root_classes_.get(qualified_name);
        return p != nullptr ? *p : nullptr;
    }

private:
    static void init_wrappers(ClassNode* node);
    static void add_classname_to_import(ClassNode* node, ImportCollector& imp);
    static void destroy_wrappers(ClassNode* node);

    JavaHashMap<std::string, ClassNode*, JavaStringHash> map_root_classes_;
    std::unique_ptr<ImportCollector> import_collector_;         // the last class written's (the context points at them until the next class)
    std::unique_ptr<BytecodeSourceMapper> source_mapper_;
};

// How a generic type's class names are written in the current run (the import collector's short names; whether a class is one being decompiled).
generics::NameContext current_name_context();

// A class with its methods decompiled (each to a statement tree), the initializers lifted out of its constructors, and the members hidden.
class ClassWrapper : public ArenaObject {
public:
    explicit ClassWrapper(const StructClass* class_struct) : class_struct_(class_struct) {}

    // Decompiles every method of the class (logging and marking one that fails: MethodWrapper::decompiled_with_errors).
    void init();

    MethodWrapper* get_method_wrapper(const std::string& name, const std::string& descriptor) const;
    const StructClass* get_class_struct() const { return class_struct_; }
    KeyedList<MethodWrapper*, std::string>& get_methods() { return methods_; }
    std::set<std::string>& get_hidden_members() { return hidden_members_; }                                  // "name descriptor" of fields and methods
    KeyedList<Exprent*, std::string>& get_static_field_initializers() { return static_field_initializers_; }    // by "name descriptor"
    KeyedList<Exprent*, std::string>& get_dynamic_field_initializers() { return dynamic_field_initializers_; }

private:
    const StructClass* class_struct_;
    std::set<std::string> hidden_members_;
    KeyedList<Exprent*, std::string> static_field_initializers_;
    KeyedList<Exprent*, std::string> dynamic_field_initializers_;
    KeyedList<MethodWrapper*, std::string> methods_;
};

// A method with what its decompilation produced.
class MethodWrapper : public ArenaObject {
public:
    RootStatement* root = nullptr;                    // null: no code, or it failed
    VarProcessor* varproc = nullptr;
    const StructMethod* method_struct = nullptr;
    std::shared_ptr<CounterContainer> counter;
    DirectGraph* graph = nullptr;
    SignatureFields signature_fields;
    bool decompiled_with_errors = false;
    std::set<std::string> set_outer_var_names;

    MethodWrapper(RootStatement* root, VarProcessor* varproc, const StructMethod* method_struct, std::shared_ptr<CounterContainer> counter)
        : root(root), varproc(varproc), method_struct(method_struct), counter(std::move(counter)) {}

    // The flattened statements of the method, built on first use (null when there is no root).
    DirectGraph* get_or_build_graph();
};

// One method's trip through the passes (the original runs it on a worker thread when MAX_PROCESSING_METHOD is set, and waits for it up to that limit).
class MethodProcessorRunnable {
public:
    MethodProcessorRunnable(const StructMethod* method, VarProcessor* var_proc, DecompilerContext* parent_context)
        : method_(method), var_proc_(var_proc), parent_context_(parent_context) {}

    // The thread's body: makes the parent's context current, runs code_to_java, keeps its result or its exception, and wakes the waiter.
    void run();
    // The method's passes in the original's order (graph, dominator tree, statements, expressions, variables, helpers): the method's statement tree.
    static RootStatement* code_to_java(const StructMethod* mt, VarProcessor* var_proc);
    // The result of run(); rethrows what it threw.
    RootStatement* get_result();
    bool is_finished() const { return finished_; }

    std::mutex lock;
    std::condition_variable cv;   // notified when run() ends

private:
    const StructMethod* method_;
    VarProcessor* var_proc_;
    DecompilerContext* parent_context_;
    RootStatement* root_ = nullptr;
    std::exception_ptr error_;
    std::atomic<bool> finished_{false};
};

// Replaces lambdas (invokedynamic through LambdaMetafactory) by nodes of lambda classes in the class tree.
class LambdaProcessor {
public:
    static constexpr std::string_view JAVAC_LAMBDA_CLASS = "java/lang/invoke/LambdaMetafactory";
    static constexpr std::string_view JAVAC_LAMBDA_METHOD = "metafactory";
    static constexpr std::string_view JAVAC_LAMBDA_ALT_METHOD = "altMetafactory";

    void process_class(ClassNode* node);
    // Always false (the original's result is unused).
    bool has_lambda(ClassNode* node);
};

// The nested classes: their enclosing methods, the variables a local or anonymous class takes from its method, the definitions of local classes.
class NestedClassProcessor {
public:
    void process_class(ClassNode* root, ClassNode* node);

private:
    // A constructor parameter of a local class: the field that holds it and the variable it is passed as.
    class VarFieldPair : public ArenaObject {
    public:
        std::string keyfield;
        VarVersionPair varpaar;
        VarFieldPair(std::string field, VarVersionPair varpaar) : keyfield(std::move(field)), varpaar(varpaar) {}
        bool equals(const VarFieldPair* o) const { return o == this || (o != nullptr && keyfield == o->keyfield && varpaar == o->varpaar); }
        std::int32_t java_hash() const { return java_string_hash(keyfield) + varpaar.java_hash(); }
    };
    using FieldMasks = JavaHashMap<std::string, std::vector<VarFieldPair*>, JavaStringHash>;   // constructor descriptor -> mask (null: no field)

    static void set_lambda_vars(ClassNode* parent, ClassNode* child);
    static void check_not_found_classes(ClassNode* root, ClassNode* node);
    static bool insert_nested_class(ClassNode* root, ClassNode* child);
    static void compute_local_vars_and_definitions(ClassNode* node);
    static void insert_local_vars(ClassNode* parent, ClassNode* child);
    static FieldMasks get_mask_local_vars(ClassWrapper* wrapper);
    // The field of the class that holds the constructor parameter `index` (none: it is not an outer variable).
    static std::optional<std::string> get_enclosing_var_field(const StructClass* cl, MethodWrapper* meth, DirectGraph* graph, int index);
    static void merge_list_signatures(std::vector<VarFieldPair*>& first, std::vector<VarFieldPair*>& second, bool both);
    static void set_local_class_definition(MethodWrapper* meth, ClassNode* node);
    static Statement* find_first_block(Statement* stat, LinkedSet<Statement*>& set_stats);
    static Statement* get_def_statement(Statement* stat, const VarType& classtype, LinkedSet<Statement*>& set_stats);
    static bool search_for_class(Exprent* exprent, const VarType& classtype);
};

// The synthetic access methods of nested classes (access$000 ...): found, and their calls replaced by the member they reach.
class NestedMemberAccess {
public:
    void propagate_member_access(ClassNode* root);

private:
    static constexpr int METHOD_ACCESS_NORMAL = 1, METHOD_ACCESS_FIELD_GET = 2, METHOD_ACCESS_FIELD_SET = 3, METHOD_ACCESS_METHOD = 4;

    void compute_method_types(ClassNode* node);
    void compute_method_type(ClassNode* node, MethodWrapper* method);
    void eliminate_static_access(ClassNode* node);
    bool replace_invocations(ClassNode* caller, MethodWrapper* meth, Exprent* exprent);
    static bool same_tree(ClassNode* caller, ClassNode* callee);
    Exprent* replace_access_exprent(ClassNode* caller, MethodWrapper* methdest, InvocationExprent* invexpr);

    bool no_synth_flag_ = false;
    LinkedMap<MethodWrapper*, int> map_method_type_;
};

// Field initializers and the constructor/static-initializer bodies they were written in.
class InitializerProcessor {
public:
    static void extract_initializers(ClassWrapper* wrapper);

private:
    static void lift_constructor(ClassWrapper* wrapper);
    static void hide_empty_super(ClassWrapper* wrapper);
    static void extract_static_initializers(ClassWrapper* wrapper, MethodWrapper* meth);
    static void extract_dynamic_initializers(ClassWrapper* wrapper);
    static bool is_exprent_independent(Exprent* exprent, MethodWrapper* meth);
    static Statement* find_first_data(Statement* stat);
    static bool is_invocation_init_constructor(InvocationExprent* inv, MethodWrapper* meth, ClassWrapper* wrapper, bool with_this);
};

// An enum's synthetic constructor parameters, values() and $VALUES, hidden.
class EnumProcessor {
public:
    static void clear_enum(ClassWrapper* wrapper);

private:
    static Statement* find_first_data(Statement* stat);
    static bool is_invocation_super_constructor(InvocationExprent* inv, MethodWrapper* meth, ClassWrapper* wrapper);
};

// `if (!$assertionsDisabled && cond) throw new AssertionError(...)` back to `assert cond : ...`.
class AssertProcessor {
public:
    static void build_assertions(ClassNode* node);

private:
    // The original's Object[]{exprent, Boolean}.
    struct AssertionExprent {
        Exprent* exprent;
        bool found;
    };
    static const VarType& class_assertion_error();   // CLASS_ASSERTION_ERROR: java/lang/AssertionError
    static const StructField* find_assertion_field(ClassNode* node);
    static bool replace_assertions(Statement* statement, const std::string& classname, const std::string& key);
    static bool replace_assertion(Statement* parent, IfStatement* stat, const std::string& classname, const std::string& key);
    static InvocationExprent* is_assertion_error(Statement* stat);
    static AssertionExprent get_assertion_exprent(Exprent* exprent, const std::string& classname, const std::string& key);
    static bool is_assertion_field(Exprent* exprent, const std::string& classname, const std::string& key);
};

// `Foo.class` as compiled before Java 5 (a synthetic class$ method with a try/catch around Class.forName), back to the class literal.
class ClassReference14Processor {
public:
    ExitExprent* bodyexprent = nullptr;      // the patterns the class$ method is compared with
    ExitExprent* handlerexprent = nullptr;

    ClassReference14Processor();
    void process_class_references(ClassNode* node);

private:
    static void process_class_rec(ClassNode* node, LinkedMap<ClassWrapper*, MethodWrapper*>& map_class_meths, LinkedSet<ClassWrapper*>& set_found);
    void map_class_methods(ClassNode* node, LinkedMap<ClassWrapper*, MethodWrapper*>& map);
    static bool replace_invocations(Exprent* exprent, ClassWrapper* wrapper, MethodWrapper* meth);
    // The class name a `class$("...")` call stands for (none: it is not such a call).
    static std::optional<std::string> is_class14_invocation(Exprent* exprent, ClassWrapper* wrapper, MethodWrapper* meth);
};

// What decides a class's entry name and its content, for the loader that saves (the original's struct.IDecompiledData).
class IDecompiledData {
public:
    virtual ~IDecompiledData() = default;
    // The output entry of a class's source (nullopt: none, for a nested class); the class's entry name in the unit is `entry_name`.
    virtual std::optional<std::string> get_class_entry_name(const StructClass* cl, const std::string& entry_name) = 0;
    // The class's Java source; nullopt for one that could not be decompiled (logged).
    virtual std::optional<std::string> get_class_content(const StructClass* cl) = 0;
};

// The decompiler (the original's main.Fernflower): the loaded files, the options and the logger of a run, and the steps of it.
class Fernflower : public IDecompiledData {
public:
    // Starts the run's context (DecompilerContext: the options over their defaults, the counters, the logger); it is current on this thread until the
    // Fernflower ends or clear_context() is called.
    Fernflower(IResultSaver& saver, const std::map<std::string, std::string>& options, IFernflowerLogger& logger);
    Fernflower(const Fernflower&) = delete;
    Fernflower& operator=(const Fernflower&) = delete;

    // The spaces are remembered: renaming makes a new StructContext (its pools take the renamer's names) and adds them again.
    void add_space(const std::filesystem::path& file, bool is_own);
    // Renames (RENAME_ENTITIES), builds the class tree and saves every own unit through the saver.
    void decompile_context();
    void clear_context();
    StructContext& get_struct_context() { return *struct_context_; }

    std::optional<std::string> get_class_entry_name(const StructClass* cl, const std::string& entry_name) override;
    std::optional<std::string> get_class_content(const StructClass* cl) override;

private:
    // ContextUnit.save: the unit's folder or archive with its other entries and each class (through the saver).
    void save_unit(const ContextUnit& unit);
    void save_context();
    void make_struct_context(std::shared_ptr<const PoolInterceptor> interceptor);

    IResultSaver& saver_;
    IFernflowerLogger& logger_;
    std::unique_ptr<DecompilerScope> scope_;
    std::unique_ptr<StructContext> struct_context_;
    std::unique_ptr<ClassesProcessor> classes_processor_;
    std::shared_ptr<IdentifierConverter> renamer_;   // keeps the renamer's helper alive (RENAME_ENTITIES)
    std::vector<std::pair<std::filesystem::path, bool>> spaces_;
};

}  // namespace cb::java::decompiler
