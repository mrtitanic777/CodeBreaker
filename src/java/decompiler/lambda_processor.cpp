// java/decompiler/lambda_processor.cpp -- see classes.hpp. Ported from Fernflower (de.fernflower.main.rels.LambdaProcessor), Copyright 2000-2015 JetBrains
// s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <map>
#include <set>
#include <stdexcept>

#include "java/decompiler/attributes.hpp"
#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/instructions.hpp"

namespace cb::java::decompiler {

void LambdaProcessor::process_class(ClassNode* node) {
    for (ClassNode* child : node->nested) process_class(child);

    has_lambda(node);
}

bool LambdaProcessor::has_lambda(ClassNode* node) {
    ClassesProcessor* cl_processor = DecompilerContext::current().class_processor;

    const StructClass* cl = node->class_struct;

    if (cl->bytecode_version() < cc::BYTECODE_JAVA_8) {   // lambda beginning with Java 8
        return false;
    }

    const BootstrapMethodsAttribute* bootstrap = cl->attribute<BootstrapMethodsAttribute>(attr::BOOTSTRAP_METHODS);
    if (bootstrap == nullptr || bootstrap->methods_number() == 0) {
        return false;   // no bootstrap constants in pool
    }

    std::set<int> lambda_methods;

    // find lambda bootstrap constants
    for (std::size_t i = 0; i < bootstrap->methods_number(); ++i) {
        const LinkConstant* method_ref = bootstrap->method_reference(i);   // method handle

        // FIXME (the original's): extend for Eclipse etc. at some point
        if (JAVAC_LAMBDA_CLASS == method_ref->classname && (JAVAC_LAMBDA_METHOD == method_ref->elementname || JAVAC_LAMBDA_ALT_METHOD == method_ref->elementname)) {
            lambda_methods.insert(static_cast<int>(i));
        }
    }

    if (lambda_methods.empty()) {
        return false;   // no lambda bootstrap constant found
    }

    std::map<std::string, std::string> map_methods_lambda;

    // iterate over code and find invocations of bootstrap methods. Replace them with anonymous classes.
    for (const auto& mt_ptr : cl->methods()) {
        const StructMethod* mt = mt_ptr.get();

        if (!mt->contains_code()) continue;   // (no instruction sequence)
        const std::shared_ptr<InstructionSequence> seq = parse_bytecode(*mt);
        if (seq != nullptr && seq->length() > 0) {
            const int len = static_cast<int>(seq->length());
            for (int i = 0; i < len; ++i) {
                const Instruction& instr = *seq->instr(static_cast<std::size_t>(i));
                if (instr.opcode == cc::opc_invokedynamic) {
                    const LinkConstant* invoke_dynamic = cl->pool().link(static_cast<std::size_t>(instr.operand(0)));

                    if (lambda_methods.contains(invoke_dynamic->index1)) {   // lambda invocation found
                        const std::vector<const PooledConstant*>& bootstrap_arguments = bootstrap->method_arguments(static_cast<std::size_t>(invoke_dynamic->index1));
                        const MethodDescriptor md = MethodDescriptor::parse(invoke_dynamic->descriptor);

                        const std::string lambda_class_name = md.ret.value.value();
                        const std::string lambda_method_name = invoke_dynamic->elementname;
                        const auto* method_type = dynamic_cast<const PrimitiveConstant*>(bootstrap_arguments.at(2));
                        if (method_type == nullptr) throw std::runtime_error("bad lambda bootstrap argument");
                        const std::string lambda_method_descriptor = method_type->get_string();   // method type

                        const auto* content_method_handle = dynamic_cast<const LinkConstant*>(bootstrap_arguments.at(1));
                        if (content_method_handle == nullptr) throw std::runtime_error("bad lambda bootstrap argument");

                        ClassNode* node_lambda = make<ClassNode>(content_method_handle->classname, content_method_handle->elementname, content_method_handle->descriptor,
                                                                 content_method_handle->index1, lambda_class_name, lambda_method_name, lambda_method_descriptor, cl);
                        node_lambda->simple_name = cl->qualified_name + "##Lambda_" + std::to_string(invoke_dynamic->index1) + "_" + std::to_string(invoke_dynamic->index2);
                        node_lambda->enclosing_method = InterpreterUtil::make_unique_key(mt->name(), mt->descriptor());

                        node->nested.push_back(node_lambda);
                        node_lambda->parent = node;

                        cl_processor->get_map_root_classes().put(*node_lambda->simple_name, node_lambda);
                        map_methods_lambda[node_lambda->lambda_information->content_method_key] = *node_lambda->simple_name;
                    }
                }
            }
        }
        // (mt.releaseResources())
    }

    // build class hierarchy on lambda
    for (std::size_t i = 0; i < node->nested.size(); ++i) {
        ClassNode* nd = node->nested[i];
        if (nd->type == ClassNode::CLASS_LAMBDA) {
            const auto parent_class_name = map_methods_lambda.find(nd->enclosing_method.value_or(std::string()));
            if (parent_class_name != map_methods_lambda.end()) {
                ClassNode* parent_class = cl_processor->get_class_node(parent_class_name->second);
                parent_class->nested.push_back(nd);
                nd->parent = parent_class;
            }
        }
    }

    // FIXME (the original's): mixed hierarchy?

    return false;
}

}  // namespace cb::java::decompiler
