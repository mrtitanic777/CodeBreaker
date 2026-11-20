// java/decompiler/classes.cpp -- see classes.hpp. Ported from Fernflower (de.fernflower.main: ClassesProcessor, Fernflower; main.rels: ClassWrapper,
// MethodWrapper, MethodProcessorRunnable; struct.ContextUnit.save), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/classes.hpp"

#include <algorithm>
#include <chrono>
#include <deque>
#include <stdexcept>
#include <thread>
#include <unordered_set>
#include <vector>

#include "java/decompiler/annotations.hpp"
#include "java/decompiler/attributes.hpp"
#include "java/decompiler/cfg.hpp"
#include "java/decompiler/class_writer.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/dom_helper.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/finally_processor.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/instructions.hpp"
#include "java/decompiler/renamer.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/stack_vars.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

// ---- ClassNode

ClassNode::ClassNode(const std::string& content_class_name, const std::string& content_method_name, const std::string& content_method_descriptor,
                     int content_method_invocation_type, const std::string& lambda_class_name, const std::string& lambda_method_name,
                     const std::string& lambda_method_descriptor, const StructClass* class_struct_) {   // lambda class constructor
    type = CLASS_LAMBDA;
    class_struct = class_struct_;   // 'parent' class containing the static function

    LambdaInformation info;

    info.class_name = lambda_class_name;
    info.method_name = lambda_method_name;
    info.method_descriptor = lambda_method_descriptor;

    info.content_class_name = content_class_name;
    info.content_method_name = content_method_name;
    info.content_method_descriptor = content_method_descriptor;
    info.content_method_invocation_type = content_method_invocation_type;

    info.content_method_key = InterpreterUtil::make_unique_key(info.content_method_name, info.content_method_descriptor);

    anonymous_class_type = VarType::parse(lambda_class_name, true);

    bool is_method_reference = content_class_name != class_struct->qualified_name;
    if (!is_method_reference) {   // content method in the same class, check synthetic flag
        const StructMethod* mt = class_struct->get_method(content_method_name, content_method_descriptor);
        if (mt == nullptr) throw std::runtime_error("lambda content method not found: " + content_method_name);
        is_method_reference = !mt->is_synthetic();   // if not synthetic -> method reference
    }

    info.is_method_reference = is_method_reference;
    info.is_content_method_static = info.content_method_invocation_type == cc::CONSTANT_MethodHandle_REF_invokeStatic;   // FIXME (the original's): redundant?

    lambda_information = std::move(info);
}

ClassNode::ClassNode(int type_, const StructClass* class_struct_) : type(type_), class_struct(class_struct_) {
    const std::string& name = class_struct->qualified_name;
    simple_name = name.substr(name.rfind('/') == std::string::npos ? 0 : name.rfind('/') + 1);
}

ClassNode* ClassNode::get_class_node(const std::string& qualified_name) const {
    for (ClassNode* node : nested) {
        if (qualified_name == node->class_struct->qualified_name) return node;
    }
    return nullptr;
}

ClassWrapper* ClassNode::get_wrapper() const {
    const ClassNode* node = this;
    while (node->type == CLASS_LAMBDA) node = node->parent;
    return node->wrapper_;
}

// ---- ClassesProcessor

namespace {

// The original's Object[4] of an InnerClasses entry: {unused, simple name, type, access flags}.
struct InnerEntry {
    std::optional<std::string> simple_name;
    int type = 0;
    int access = 0;
    friend bool operator==(const InnerEntry& a, const InnerEntry& b) { return a.simple_name == b.simple_name && a.type == b.type && a.access == b.access; }
};

}  // namespace

ClassesProcessor::ClassesProcessor(const StructContext& context) {
    std::map<std::string, InnerEntry> map_inner_classes;
    std::map<std::string, JavaHashSet<std::string, JavaStringHash>> map_nested_class_references;
    std::map<std::string, JavaHashSet<std::string, JavaStringHash>> map_enclosing_class_references;
    std::map<std::string, std::string> map_new_simple_names;

    const bool b_decompile_inner = DecompilerContext::option(pref::DECOMPILE_INNER);

    // create class nodes
    for (const auto& class_entry : context.classes()) {
        const StructClass* cl = class_entry.second.get();
        if (cl->is_own() && !map_root_classes_.contains_key(cl->qualified_name)) {
            if (b_decompile_inner) {
                const InnerClassesAttribute* inner = cl->attribute<InnerClassesAttribute>(attr::INNER_CLASSES);
                if (inner != nullptr) {
                    for (std::size_t i = 0; i < inner->entries().size(); ++i) {
                        const InnerClassesAttribute::Entry& entry = inner->entries()[i];

                        InnerEntry arr;

                        const std::string& innername = entry.inner_class_name;

                        // nested class type
                        arr.type = entry.outer_class == 0 ? (entry.inner_name == 0 ? ClassNode::CLASS_ANONYMOUS : ClassNode::CLASS_LOCAL) : ClassNode::CLASS_MEMBER;

                        // original simple name
                        std::optional<std::string> simple_name = entry.simple_name;
                        const auto saved_name = map_new_simple_names.find(innername);

                        if (saved_name != map_new_simple_names.end()) {
                            simple_name = saved_name->second;
                        } else if (simple_name && DecompilerContext::option(pref::RENAME_ENTITIES)) {
                            const auto interceptor = std::dynamic_pointer_cast<const RenamerPoolInterceptor>(DecompilerContext::current().pool_interceptor);
                            IIdentifierRenamer* renamer = interceptor->get_helper();
                            if (renamer->to_be_renamed(IIdentifierRenamer::Type::ELEMENT_CLASS, simple_name, std::nullopt, std::nullopt)) {
                                simple_name = renamer->get_next_class_name(innername, simple_name);
                                map_new_simple_names[innername] = *simple_name;
                            }
                        }

                        arr.simple_name = simple_name;

                        // original access flags
                        arr.access = entry.access;

                        // enclosing class
                        std::string encl_class_name;
                        if (entry.outer_class != 0) {
                            encl_class_name = entry.outer_class_name.value_or("null");
                        } else {
                            encl_class_name = cl->qualified_name;
                        }

                        if (innername != encl_class_name) {   // self reference
                            const StructClass* enclosing_class = context.get_class(encl_class_name);
                            if (enclosing_class != nullptr && enclosing_class->is_own()) {   // own classes only
                                const auto arrold = map_inner_classes.find(innername);
                                if (arrold == map_inner_classes.end()) {
                                    map_inner_classes[innername] = arr;
                                } else if (!(arrold->second == arr)) {
                                    const std::string message = "Inconsistent inner class entries for " + innername + "!";
                                    DecompilerContext::get_logger().write_message(message, IFernflowerLogger::Severity::WARN);
                                }

                                // reference to the nested class
                                map_nested_class_references[encl_class_name].add(innername);

                                // reference to the enclosing class
                                map_enclosing_class_references[innername].add(encl_class_name);
                            }
                        }
                    }
                }
            }

            ClassNode* node = make<ClassNode>(ClassNode::CLASS_ROOT, cl);
            node->access = cl->access_flags();
            map_root_classes_.put(cl->qualified_name, node);
        }
    }

    if (b_decompile_inner) {
        // connect nested classes
        for (const auto& [root_name, root_node] : map_root_classes_.entries()) {
            (void)root_node;
            // root class?
            if (!map_inner_classes.contains(root_name)) {
                std::set<std::string> set_visited;
                std::deque<std::string> stack;

                stack.push_back(root_name);
                set_visited.insert(root_name);

                while (!stack.empty()) {
                    const std::string super_class = stack.front();
                    stack.pop_front();
                    ClassNode* supernode = get_class_node(super_class);

                    const auto set_nested_classes = map_nested_class_references.find(super_class);
                    if (set_nested_classes != map_nested_class_references.end()) {
                        const StructClass* scl = supernode->class_struct;
                        const InnerClassesAttribute* inner = scl->attribute<InnerClassesAttribute>(attr::INNER_CLASSES);
                        for (std::size_t i = 0; i < inner->entries().size(); ++i) {
                            const std::string& nested_class = inner->entries()[i].inner_class_name;
                            if (!set_nested_classes->second.contains(nested_class)) continue;

                            if (!set_visited.insert(nested_class).second) continue;

                            ClassNode* nestednode = get_class_node(nested_class);
                            if (nestednode == nullptr) {
                                DecompilerContext::get_logger().write_message("Nested class " + nested_class + " missing!", IFernflowerLogger::Severity::WARN);
                                continue;
                            }

                            const InnerEntry& arr = map_inner_classes.at(nested_class);

                            // if (arr type == CLASS_MEMBER) { FIXME (the original's): check for consistent naming }

                            nestednode->type = arr.type;
                            nestednode->simple_name = arr.simple_name;
                            nestednode->access = arr.access;

                            if (nestednode->type == ClassNode::CLASS_ANONYMOUS) {
                                const StructClass* cl = nestednode->class_struct;

                                // remove static if anonymous class
                                // a common compiler bug
                                nestednode->access &= ~cc::ACC_STATIC;

                                const std::vector<int>& interfaces = cl->interfaces();

                                if (!interfaces.empty()) {
                                    if (interfaces.size() > 1) {
                                        const std::string message = "Inconsistent anonymous class definition: " + cl->qualified_name;
                                        DecompilerContext::get_logger().write_message(message, IFernflowerLogger::Severity::WARN);
                                    }
                                    nestednode->anonymous_class_type = VarType::parse(cl->get_interface(0), true);
                                } else {
                                    nestednode->anonymous_class_type = VarType::parse(cl->super_class->get_string(), true);
                                }
                            } else if (nestednode->type == ClassNode::CLASS_LOCAL) {
                                // only abstract and final are permitted
                                // a common compiler bug
                                nestednode->access &= (cc::ACC_ABSTRACT | cc::ACC_FINAL);
                            }

                            supernode->nested.push_back(nestednode);
                            nestednode->parent = supernode;

                            const auto encl = map_enclosing_class_references.find(nested_class);
                            if (encl == map_enclosing_class_references.end()) throw std::runtime_error("missing enclosing class references");
                            for (const std::string& name : encl->second.items()) nestednode->enclosing_classes.add(name);

                            stack.push_back(nested_class);
                        }
                    }
                }
            }
        }
    }
}

void ClassesProcessor::write_class(const StructClass* cl, TextBuffer& buffer) {
    ClassNode* root = get_class_node(cl->qualified_name);
    if (root->type != ClassNode::CLASS_ROOT) return;

    DecompilerContext& ctx = DecompilerContext::current();
    DecompilerContext::get_logger().start_reading_class(cl->qualified_name);
    try {
        import_collector_ = std::make_unique<ImportCollector>(root);
        ctx.import_collector = import_collector_.get();
        ctx.counters = std::make_shared<CounterContainer>();
        source_mapper_ = std::make_unique<BytecodeSourceMapper>();
        ctx.bytecode_source_mapper = source_mapper_.get();
        ImportCollector& import_collector = *import_collector_;

        LambdaProcessor().process_class(root);

        // add simple class names to implicit import
        add_classname_to_import(root, import_collector);

        // build wrappers for all nested classes (that's where actual processing takes place)
        init_wrappers(root);

        NestedClassProcessor().process_class(root, root);

        NestedMemberAccess().propagate_member_access(root);

        TextBuffer class_buffer;
        ClassWriter().class_to_java(root, class_buffer, 0, nullptr);

        int total_offset_lines = 0;

        const std::size_t index = cl->qualified_name.rfind('/');
        if (index != std::string::npos) {
            total_offset_lines += 2;
            std::string package_name = cl->qualified_name.substr(0, index);
            std::replace(package_name.begin(), package_name.end(), '/', '.');

            buffer.append("package ");
            buffer.append(package_name);
            buffer.append(";");
            buffer.append_line_separator();
            buffer.append_line_separator();
        }

        const int import_lines_written = import_collector.write_imports(buffer);
        if (import_lines_written > 0) {
            buffer.append_line_separator();
            total_offset_lines += import_lines_written + 1;
        }

        total_offset_lines = buffer.count_lines();
        buffer.append(class_buffer);

        if (DecompilerContext::option(pref::BYTECODE_SOURCE_MAPPING)) {
            BytecodeSourceMapper* mapper = ctx.bytecode_source_mapper;
            mapper->add_total_offset(total_offset_lines);
            if (DecompilerContext::option(pref::DUMP_ORIGINAL_LINES)) {
                buffer.dump_original_line_numbers(mapper->get_original_lines_mapping());
            }
            if (DecompilerContext::option(pref::UNIT_TEST_MODE)) {
                buffer.append_line_separator();
                mapper->dump_mapping(buffer, true);
            }
        }
    } catch (...) {
        destroy_wrappers(root);
        DecompilerContext::get_logger().end_reading_class();
        throw;
    }
    destroy_wrappers(root);
    DecompilerContext::get_logger().end_reading_class();
}

void ClassesProcessor::init_wrappers(ClassNode* node) {
    const StackDepth depth;
    if (node->type == ClassNode::CLASS_LAMBDA) return;

    ClassWrapper* wrapper = make<ClassWrapper>(node->class_struct);
    wrapper->init();

    node->wrapper_ = wrapper;

    for (ClassNode* nd : node->nested) init_wrappers(nd);
}

void ClassesProcessor::add_classname_to_import(ClassNode* node, ImportCollector& imp) {
    const StackDepth depth;
    if (node->simple_name && !node->simple_name->empty()) {
        imp.get_short_name(node->type == ClassNode::CLASS_ROOT ? node->class_struct->qualified_name : *node->simple_name, false);
    }

    for (ClassNode* nd : node->nested) add_classname_to_import(nd, imp);
}

void ClassesProcessor::destroy_wrappers(ClassNode* node) {
    const StackDepth depth;
    node->wrapper_ = nullptr;
    // (the original's node.classStruct.releaseResources(): classes are not loaded lazily here)

    for (ClassNode* nd : node->nested) destroy_wrappers(nd);
}

generics::NameContext current_name_context() {
    generics::NameContext names;
    names.short_name = [](const std::string& java_class_name) { return DecompilerContext::current().import_collector->get_short_name(java_class_name); };
    names.is_own_class = [](const std::string& internal_name) {
        const StructClass* cl = DecompilerContext::current().struct_context->get_class(internal_name);
        return cl != nullptr && cl->is_own();
    };
    return names;
}

// ---- ClassWrapper

void ClassWrapper::init() {
    DecompilerContext& ctx = DecompilerContext::current();
    ctx.current_class = class_struct_;
    DecompilerContext::get_logger().start_class(class_struct_->qualified_name);

    // collect field names
    std::set<std::string> set_field_names;
    for (const auto& fd : class_struct_->fields()) set_field_names.insert(fd->name());

    const int max_sec = std::stoi(DecompilerContext::property(pref::MAX_PROCESSING_METHOD));
    const bool test_mode = DecompilerContext::option(pref::UNIT_TEST_MODE);

    for (const auto& mt_ptr : class_struct_->methods()) {
        const StructMethod* mt = mt_ptr.get();
        DecompilerContext::get_logger().start_method(mt->name() + " " + mt->descriptor());

        auto vc = std::make_unique<VarNamesCollector>();
        ctx.var_names_collector = vc.get();

        auto counter = std::make_shared<CounterContainer>();
        ctx.counters = counter;

        ctx.current_method = mt;
        ctx.current_method_descriptor = MethodDescriptor::parse(mt->descriptor());

        VarProcessor* var_proc = make<VarProcessor>();
        ctx.current_var_processor = var_proc;

        RootStatement* root = nullptr;
        bool is_error = false;

        try {
            if (mt->contains_code()) {
                if (max_sec == 0 || test_mode) {
                    root = MethodProcessorRunnable::code_to_java(mt, var_proc);
                } else {
                    MethodProcessorRunnable mt_proc(mt, var_proc, &ctx);

                    std::thread mt_thread([&mt_proc] { mt_proc.run(); });
                    const auto stop_at = std::chrono::steady_clock::now() + std::chrono::seconds(max_sec);

                    while (!mt_proc.is_finished()) {
                        {
                            std::unique_lock<std::mutex> lk(mt_proc.lock);
                            mt_proc.cv.wait_for(lk, std::chrono::milliseconds(200));
                        }

                        if (std::chrono::steady_clock::now() >= stop_at) {
                            const std::string message = "Processing time limit exceeded for method " + mt->name() + ", execution interrupted.";
                            DecompilerContext::get_logger().write_message(message, IFernflowerLogger::Severity::ERROR);
                            is_error = true;
                            break;
                        }
                    }

                    // (a thread cannot be killed: it is waited for, and its result dropped when the limit was exceeded)
                    mt_thread.join();

                    if (!is_error) root = mt_proc.get_result();
                }
            } else {
                const bool this_var = !mt->has_modifier(cc::ACC_STATIC);

                const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());

                int param_count = 0;
                if (this_var) {
                    var_proc->get_this_vars().put(VarVersionPair(0, 0), class_struct_->qualified_name);
                    param_count = 1;
                }

                param_count += static_cast<int>(md.params.size());

                int var_index = 0;
                for (int i = 0; i < param_count; ++i) {
                    var_proc->set_var_name(VarVersionPair(var_index, 0), vc->get_free_name(var_index));

                    if (this_var) {
                        if (i == 0) {
                            ++var_index;
                        } else {
                            var_index += md.params[static_cast<std::size_t>(i - 1)].stack_size;
                        }
                    } else {
                        var_index += md.params[static_cast<std::size_t>(i)].stack_size;
                    }
                }
            }
        } catch (const std::exception& ex) {
            DecompilerContext::get_logger().write_message("Method " + mt->name() + " " + mt->descriptor() + " couldn't be decompiled.", ex);
            is_error = true;
        }

        MethodWrapper* method_wrapper = make<MethodWrapper>(root, var_proc, mt, counter);

        method_wrapper->decompiled_with_errors = is_error;

        methods_.add_with_key(method_wrapper, InterpreterUtil::make_unique_key(mt->name(), mt->descriptor()));

        // rename vars so that no one has the same name as a field
        VarNamesCollector field_names(set_field_names);
        var_proc->refresh_var_names(field_names);

        // if debug information present and should be used
        if (DecompilerContext::option(pref::USE_DEBUG_VAR_NAMES)) {
            const LocalVariableTableAttribute* attr_lvt = mt->attribute<LocalVariableTableAttribute>(attr::LOCAL_VARIABLE_TABLE);
            if (attr_lvt != nullptr) var_proc->set_debug_var_names(attr_lvt->var_names());
        }

        DecompilerContext::get_logger().end_method();
        ctx.var_names_collector = nullptr;   // (the collector is local to this turn)
    }

    DecompilerContext::get_logger().end_class();
}

MethodWrapper* ClassWrapper::get_method_wrapper(const std::string& name, const std::string& descriptor) const {
    return methods_.get_with_key(InterpreterUtil::make_unique_key(name, descriptor));
}

// ---- MethodWrapper

DirectGraph* MethodWrapper::get_or_build_graph() {
    if (graph == nullptr && root != nullptr) {
        FlattenStatementsHelper flatthelper;
        graph = flatthelper.build_direct_graph(root);
    }
    return graph;
}

// ---- MethodProcessorRunnable

void MethodProcessorRunnable::run() {
    DecompilerContext::set_current(parent_context_);

    error_ = nullptr;
    root_ = nullptr;

    try {
        root_ = code_to_java(method_, var_proc_);
    } catch (...) {
        error_ = std::current_exception();
    }

    DecompilerContext::set_current(nullptr);

    {
        std::lock_guard<std::mutex> lk(lock);
        finished_ = true;
    }
    cv.notify_all();
}

namespace {

// A method's control-flow graph, kept for as long as the run lives: the statements made from it point into its blocks.
struct GraphHolder : ArenaObject {
    std::shared_ptr<InstructionSequence> seq;
    std::unique_ptr<ControlFlowGraph> graph;
};

}  // namespace

RootStatement* MethodProcessorRunnable::code_to_java(const StructMethod* mt, VarProcessor* var_proc) {
    const StructClass* cl = &mt->class_struct();

    const bool is_initializer = mt->name() == cc::CLINIT_NAME;   // for now static initializer only

    // (mt.expandData(): the code is parsed here)
    GraphHolder* holder = make<GraphHolder>();
    holder->seq = parse_bytecode(*mt);
    holder->graph = std::make_unique<ControlFlowGraph>(*holder->seq);
    ControlFlowGraph& graph = *holder->graph;

    dead_code::remove_dead_blocks(graph);

    graph.inline_jsr(*mt);

    // TODO (the original's): move to the start, before jsr inlining
    dead_code::connect_dummy_exit_block(graph);

    dead_code::remove_gotos(graph);

    exception_deobfuscator::remove_circular_ranges(graph);

    exception_deobfuscator::restore_pop_ranges(graph);

    if (DecompilerContext::option(pref::REMOVE_EMPTY_RANGES)) exception_deobfuscator::remove_empty_ranges(graph);

    if (DecompilerContext::option(pref::NO_EXCEPTIONS_RETURN)) {
        // special case: single return instruction outside of a protected range
        dead_code::incorporate_value_returns(graph);
    }

    //		ExceptionDeobfuscator.restorePopRanges(graph);

    exception_deobfuscator::insert_empty_exception_handler_blocks(graph);

    dead_code::merge_basic_blocks(graph);

    DecompilerContext::counter_container().set_counter(CounterContainer::VAR_COUNTER, mt->local_variables());

    if (exception_deobfuscator::has_obfuscated_exceptions(graph)) {
        DecompilerContext::get_logger().write_message("Heavily obfuscated exception ranges found!", IFernflowerLogger::Severity::WARN);
    }

    RootStatement* root = DomHelper::parse_graph(graph);

    FinallyProcessor f_proc(var_proc);
    while (f_proc.iterate_graph(mt, root, graph)) root = DomHelper::parse_graph(graph);

    // remove synchronized exception handler
    // not until now because of comparison between synchronized statements in the finally cycle
    DomHelper::remove_synchronized_handler(root);

    //    LabelHelper.lowContinueLabels(root, new HashSet<StatEdge>());

    SequenceHelper::condense_sequences(root);

    ClearStructHelper::clear_statements(root);

    ExprProcessor proc;
    proc.process_statement(root, cl);

    SequenceHelper::condense_sequences(root);

    while (true) {
        StackVarsProcessor stack_proc;
        stack_proc.simplify_stack_vars(root, mt, cl);

        var_proc->set_var_versions(root);

        if (!PPandMMHelper().find_pp_and_mm(root)) break;
    }

    while (true) {
        LabelHelper::clean_up_edges(root);

        while (true) {
            MergeHelper::enhance_loops(root);

            if (LoopExtractHelper::extract_loops(root)) continue;

            if (!IfHelper::merge_all_ifs(root)) break;
        }

        if (DecompilerContext::option(pref::IDEA_NOT_NULL_ANNOTATION)) {
            if (IdeaNotNullHelper::remove_hardcoded_checks(root, mt)) {
                SequenceHelper::condense_sequences(root);

                StackVarsProcessor stack_proc;
                stack_proc.simplify_stack_vars(root, mt, cl);

                var_proc->set_var_versions(root);
            }
        }

        LabelHelper::identify_labels(root);

        if (InlineSingleBlockHelper::inline_single_blocks(root)) continue;

        // initializer may have at most one return point, so no transformation of method exits permitted
        if (!(is_initializer || !ExitHelper::condense_exits(root))) continue;

        //      if(!EliminateLoopsHelper.eliminateLoops(root)) {
        break;
        //      }
    }

    ExitHelper::remove_redundant_returns(root);

    SecondaryFunctionsHelper::identify_secondary_functions(root);

    var_proc->set_var_definitions(root);

    // must be the last invocation, because it makes the statement structure inconsistent
    // FIXME (the original's): new edge type needed
    LabelHelper::replace_continue_with_break(root);

    // (mt.releaseResources(): nothing to free)

    return root;
}

RootStatement* MethodProcessorRunnable::get_result() {
    if (error_) std::rethrow_exception(error_);
    return root_;
}

// ---- Fernflower

Fernflower::Fernflower(IResultSaver& saver, const std::map<std::string, std::string>& options, IFernflowerLogger& logger) : saver_(saver), logger_(logger) {
    DecompilerContext dc;   // (its properties are the defaults)
    for (const auto& [key, value] : options) dc.properties[key] = value;
    dc.counters = std::make_shared<CounterContainer>();
    dc.fernflower_logger = &logger;

    const auto level = dc.properties.find(std::string(pref::LOG_LEVEL));
    if (level != dc.properties.end()) {
        if (const auto severity = IFernflowerLogger::severity_of(level->second)) logger.set_severity(*severity);
    }

    scope_ = std::make_unique<DecompilerScope>(std::move(dc));
    make_struct_context(nullptr);
}

void Fernflower::make_struct_context(std::shared_ptr<const PoolInterceptor> interceptor) {
    struct_context_ = std::make_unique<StructContext>([this](const std::string& message) { logger_.write_message(message, IFernflowerLogger::Severity::ERROR); },
                                                       std::move(interceptor));
    for (const auto& [path, is_own] : spaces_) struct_context_->add_space(path, is_own);
}

void Fernflower::add_space(const std::filesystem::path& file, bool is_own) {
    spaces_.emplace_back(file, is_own);
    struct_context_->add_space(file, is_own);
}

void Fernflower::decompile_context() {
    DecompilerContext& ctx = scope_->context();

    if (DecompilerContext::option(pref::RENAME_ENTITIES)) {
        renamer_ = std::make_shared<IdentifierConverter>();
        std::shared_ptr<RenamerPoolInterceptor> interceptor = renamer_->rename(*struct_context_);
        ctx.pool_interceptor = interceptor;
        make_struct_context(interceptor);   // (context.reloadContext())
    }

    classes_processor_ = std::make_unique<ClassesProcessor>(*struct_context_);

    ctx.class_processor = classes_processor_.get();
    ctx.struct_context = struct_context_.get();

    save_context();
}

void Fernflower::clear_context() {
    scope_.reset();   // (the original's setCurrentContext(null))
}

void Fernflower::save_context() {
    for (const auto& [name, unit] : struct_context_->units()) {
        (void)name;
        if (unit.own) save_unit(unit);
    }
}

void Fernflower::save_unit(const ContextUnit& unit) {
    DecompilerContext::get_logger().start_save(static_cast<int>(unit.classes.size()));

    const auto save_class = [&](std::size_t i) -> std::optional<std::string> {
        DecompilerContext::get_logger().update_save(static_cast<int>(i));
        return get_class_entry_name(unit.classes[i], unit.class_entries[i]);
    };

    switch (unit.type) {
        case ContextUnit::TYPE_FOLDER: {
            // create folder
            saver_.save_folder(unit.filename);

            // non-class files
            for (const auto& pair : unit.other_entries) saver_.copy_file(pair.first, unit.filename, pair.second);

            // classes
            for (std::size_t i = 0; i < unit.classes.size(); ++i) {
                const StructClass* cl = unit.classes[i];
                const std::optional<std::string> entry_name = save_class(i);
                if (entry_name) {
                    const std::optional<std::string> content = get_class_content(cl);
                    if (content) {
                        std::vector<int> mapping;
                        if (DecompilerContext::option(pref::BYTECODE_SOURCE_MAPPING)) {
                            mapping = DecompilerContext::current().bytecode_source_mapper->get_original_lines_mapping();
                        }
                        saver_.save_class_file(unit.filename, cl->qualified_name, *entry_name, *content, mapping);
                    }
                }
            }
            break;
        }

        case ContextUnit::TYPE_JAR:
        case ContextUnit::TYPE_ZIP: {
            // create archive file
            saver_.save_folder(unit.archive_path);
            saver_.create_archive(unit.archive_path, unit.filename, unit.manifest);

            // directory entries
            for (const std::string& dir_entry : unit.dir_entries) saver_.save_dir_entry(unit.archive_path, unit.filename, dir_entry);

            // non-class entries
            for (const auto& pair : unit.other_entries) {
                std::string upper = pair.second;
                std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
                if (unit.type != ContextUnit::TYPE_JAR || upper != "META-INF/MANIFEST.MF") {
                    saver_.copy_entry(pair.first, unit.archive_path, unit.filename, pair.second);
                }
            }

            // classes
            for (std::size_t i = 0; i < unit.classes.size(); ++i) {
                const StructClass* cl = unit.classes[i];
                const std::optional<std::string> entry_name = save_class(i);
                if (entry_name) {
                    const std::optional<std::string> content = get_class_content(cl);
                    saver_.save_class_entry(unit.archive_path, unit.filename, cl->qualified_name, *entry_name, content);
                }
            }

            saver_.close_archive(unit.archive_path, unit.filename);
            break;
        }
        default:
            break;
    }
}

std::optional<std::string> Fernflower::get_class_entry_name(const StructClass* cl, const std::string& entry_name) {
    const ClassNode* node = classes_processor_->get_class_node(cl->qualified_name);
    if (node->type != ClassNode::CLASS_ROOT) return std::nullopt;

    if (DecompilerContext::option(pref::RENAME_ENTITIES)) {
        const std::string simple_classname = cl->qualified_name.substr(cl->qualified_name.rfind('/') == std::string::npos ? 0 : cl->qualified_name.rfind('/') + 1);
        const std::size_t slash = entry_name.rfind('/');
        return entry_name.substr(0, slash == std::string::npos ? 0 : slash + 1) + simple_classname + ".java";
    }
    const std::size_t dot = entry_name.rfind(".class");
    if (dot == std::string::npos) throw std::out_of_range("entry name has no .class: " + entry_name);
    return entry_name.substr(0, dot) + ".java";
}

namespace {

// What the original's collector does once a class is written: the objects made for it (its statements, expressions, SSA sets ...) are unreachable,
// and freed with its arena region (Arena::ClassRegion). The class nodes made on the way stay (a lambda's node joins the class tree for good); what
// they held of the freed objects (the constructor's super call of an anonymous class) is cleared. Without this the run keeps every object of every
// class to its end (11 GB on a 600-class jar).
class ClassArenaScope {
public:
    explicit ClassArenaScope(ClassNode* root) : root_(root), region_(*DecompilerContext::current().arena) {}
    ~ClassArenaScope() {
        if (root_ != nullptr) clear_freed_references(root_);
    }
    ClassArenaScope(const ClassArenaScope&) = delete;
    ClassArenaScope& operator=(const ClassArenaScope&) = delete;

private:
    // (each node once: a lambda node can be its own parent)
    static void clear_freed_references(ClassNode* root) {
        std::unordered_set<ClassNode*> seen{root};
        std::vector<ClassNode*> todo{root};
        while (!todo.empty()) {
            ClassNode* node = todo.back();
            todo.pop_back();
            node->super_invocation = nullptr;
            for (ClassNode* nd : node->nested) {
                if (seen.insert(nd).second) todo.push_back(nd);
            }
        }
    }

    ClassNode* root_;
    Arena::ClassRegion region_;
};

}  // namespace

std::optional<std::string> Fernflower::get_class_content(const StructClass* cl) {
    ClassNode* root = classes_processor_->get_class_node(cl->qualified_name);
    try {
        const ClassArenaScope arena_scope(root);
        TextBuffer buffer;
        buffer.append(DecompilerContext::property(pref::BANNER));
        classes_processor_->write_class(cl, buffer);
        return buffer.to_string();
    } catch (const std::exception& ex) {
        DecompilerContext::get_logger().write_message("Class " + cl->qualified_name + " couldn't be fully decompiled.", ex);
        return std::nullopt;
    }
}

}  // namespace cb::java::decompiler
