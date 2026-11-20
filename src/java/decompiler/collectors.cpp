// java/decompiler/collectors.cpp -- see collectors.hpp. Ported from Fernflower (de.fernflower.main.collectors, util.TextUtil), Copyright 2000-2015
// JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/collectors.hpp"

#include <algorithm>
#include <cstdio>

#include "java/decompiler/attributes.hpp"
#include "java/decompiler/classes.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/structs.hpp"

namespace cb::java::decompiler {

// ---- BytecodeMappingTracer

void BytecodeMappingTracer::shift_source_lines(int shift) {
    for (auto& entry : mapping_) entry.second += shift;
}

void BytecodeMappingTracer::add_mapping(int bytecode_offset) {
    if (!mapping_.contains(bytecode_offset)) mapping_[bytecode_offset] = current_source_line_;
}

void BytecodeMappingTracer::add_mapping(const std::optional<std::set<int>>& bytecode_offsets) {
    if (bytecode_offsets) {
        for (const int bytecode_offset : *bytecode_offsets) add_mapping(bytecode_offset);
    }
}

void BytecodeMappingTracer::add_tracer(const BytecodeMappingTracer* tracer) {
    if (tracer != nullptr) {
        for (const auto& [offset, line] : tracer->mapping_) {
            if (!mapping_.contains(offset)) mapping_[offset] = line;
        }
    }
}

std::map<int, int> BytecodeMappingTracer::get_original_lines_mapping() const {
    if (line_number_table_ == nullptr) return {};

    std::map<int, int> res;
    const std::vector<int>& data = line_number_table_->raw_data();
    for (std::size_t i = 0; i + 1 < data.size(); i += 2) {
        const int original_offset = data[i];
        const int original_line = data[i + 1];
        const auto it = mapping_.find(original_offset);
        if (it != mapping_.end()) res[original_line] = it->second;
    }
    for (const auto& [offset, line] : mapping_) {
        const int original_line = line_number_table_->find_line_number(offset);
        if (original_line > -1) res[original_line] = line;
    }
    return res;
}

// ---- BytecodeSourceMapper

void BytecodeSourceMapper::add_mapping(const std::string& class_name, const std::string& method_name, int bytecode_offset, int source_line) {
    auto& class_mapping = mapping_.get_or_put(class_name);   // (order of classes and of methods is kept)
    auto& method_mapping = class_mapping.get_or_put(method_name);
    // don't overwrite
    if (!method_mapping.contains(bytecode_offset)) method_mapping[bytecode_offset] = source_line;
}

void BytecodeSourceMapper::add_tracer(const std::string& class_name, const std::string& method_name, const BytecodeMappingTracer& tracer) {
    for (const auto& [offset, line] : tracer.get_mapping()) add_mapping(class_name, method_name, offset, line);
    for (const auto& [original, decompiled] : tracer.get_original_lines_mapping()) lines_mapping_[original] = decompiled;
}

void BytecodeSourceMapper::dump_mapping(TextBuffer& buffer, bool offsets_to_hex) const {
    if (mapping_.empty() && lines_mapping_.empty()) return;

    const std::string line_separator = DecompilerContext::new_line_separator();

    for (const auto& [class_name, class_mapping] : mapping_) {
        buffer.append("class '" + class_name + "' {" + line_separator);

        bool is_first_method = true;
        for (const auto& [method_name, method_mapping] : class_mapping) {
            if (!is_first_method) buffer.append_line_separator();

            buffer.append_indent(1).append("method '" + method_name + "' {" + line_separator);

            for (const auto& [offset, line] : method_mapping) {   // (sorted by offset)
                std::string str_offset;
                if (offsets_to_hex) {
                    char hex[16];
                    std::snprintf(hex, sizeof hex, "%x", static_cast<unsigned>(offset));
                    str_offset = hex;
                } else {
                    str_offset = std::to_string(line);
                }
                buffer.append_indent(2).append(str_offset).append_indent(2).append(std::to_string(line + offset_total_) + line_separator);
            }
            buffer.append_indent(1).append("}").append_line_separator();

            is_first_method = false;
        }

        buffer.append("}").append_line_separator().append_line_separator();
    }

    // lines mapping
    buffer.append("Lines mapping:").append_line_separator();
    for (const auto& [original, decompiled] : lines_mapping_) {
        buffer.append(original).append(" <-> ").append(decompiled + offset_total_ + 1).append_line_separator();
    }
}

std::vector<int> BytecodeSourceMapper::get_original_lines_mapping() const {
    std::vector<int> res;
    res.reserve(lines_mapping_.size() * 2);
    for (const auto& [original, decompiled] : lines_mapping_) {
        res.push_back(original);
        res.push_back(decompiled + offset_total_ + 1);   // make it 1 based
    }
    return res;
}

// ---- ImportCollector

ImportCollector::ImportCollector(const ClassNode* root) {
    const std::string& clname = root->class_struct->qualified_name;
    const std::size_t index = clname.rfind('/');
    if (index != std::string::npos) {
        current_package_slash_ = clname.substr(0, index);
        current_package_point_ = current_package_slash_;
        std::replace(current_package_point_.begin(), current_package_point_.end(), '/', '.');
        current_package_slash_ += "/";
    }
}

std::string ImportCollector::get_short_name(std::string fullname) { return get_short_name(std::move(fullname), true); }

std::string ImportCollector::get_short_name(std::string fullname, bool imported) {
    ClassesProcessor* clproc = DecompilerContext::current().class_processor;
    std::string slashed = fullname;
    std::replace(slashed.begin(), slashed.end(), '.', '/');
    const ClassNode* node = clproc->get_class_node(slashed);

    std::optional<std::string> retname;

    if (node != nullptr && node->class_struct->is_own()) {
        retname = node->simple_name.value_or("null");

        while (node->parent != nullptr && node->type == ClassNode::CLASS_MEMBER) {
            retname = node->parent->simple_name.value_or("null") + "." + *retname;
            node = node->parent;
        }

        if (node->type == ClassNode::CLASS_ROOT) {
            fullname = node->class_struct->qualified_name;
            std::replace(fullname.begin(), fullname.end(), '/', '.');
        } else {
            return *retname;
        }
    } else {
        std::replace(fullname.begin(), fullname.end(), '$', '.');
    }

    std::string nshort = fullname;
    std::string npackage;

    const std::size_t lastpoint = fullname.rfind('.');
    if (lastpoint != std::string::npos) {
        nshort = fullname.substr(lastpoint + 1);
        npackage = fullname.substr(0, lastpoint);
    }

    const StructContext* context = DecompilerContext::current().struct_context;

    // check for another class which could 'shadow' this one. Two cases:
    // 1) class with the same short name in the current package
    // 2) class with the same short name in the default package
    const bool exists_default_class = (context->get_class(current_package_slash_ + nshort) != nullptr && npackage != current_package_point_)   // current package
                                      || (context->get_class(nshort) != nullptr && !current_package_point_.empty());                            // default package

    const auto existing = map_simple_names_.find(nshort);
    if (exists_default_class || (existing != map_simple_names_.end() && npackage != existing->second)) {
        return fullname;
    }
    if (existing == map_simple_names_.end()) {
        map_simple_names_[nshort] = npackage;

        if (!imported) set_not_imported_names_.insert(nshort);
    }

    return !retname ? nshort : *retname;
}

int ImportCollector::write_imports(TextBuffer& buffer) {
    int importlines_written = 0;

    const std::vector<std::string> imports = pack_imports();

    for (const std::string& s : imports) {
        buffer.append("import ");
        buffer.append(s);
        buffer.append(";");
        buffer.append_line_separator();

        ++importlines_written;
    }

    return importlines_written;
}

std::vector<std::string> ImportCollector::pack_imports() const {
    // (package, name) sorted: the package first, then the name
    std::vector<std::pair<std::string, std::string>> lst;
    for (const auto& [name, package] : map_simple_names_) lst.emplace_back(package, name);
    std::sort(lst.begin(), lst.end());

    std::vector<std::string> res;
    for (const auto& [package, name] : lst) {
        // exclude a current class or one of the nested ones, java.lang and empty packages
        if (!set_not_imported_names_.contains(name) && package != JAVA_LANG_PACKAGE && package != current_package_point_ && !package.empty()) {
            res.push_back(package + "." + name);
        }
    }
    return res;
}

// ---- VarNamesCollector

std::string VarNamesCollector::get_free_name(int index) { return get_free_name("var" + std::to_string(index)); }

std::string VarNamesCollector::get_free_name(std::string proposition) {
    while (used_names_.contains(proposition)) proposition += "x";
    used_names_.insert(proposition);
    return proposition;
}

// ---- TextUtil

void TextUtil::write_qualified_super(TextBuffer& buf, const std::string& qualifier) {
    const ClassNode* class_node = DecompilerContext::current().current_class_node;
    if (qualifier != class_node->class_struct->qualified_name) {
        buf.append(DecompilerContext::current().import_collector->get_short_name(ExprProcessor::build_java_class_name(qualifier))).append('.');
    }
    buf.append("super");
}

}  // namespace cb::java::decompiler
