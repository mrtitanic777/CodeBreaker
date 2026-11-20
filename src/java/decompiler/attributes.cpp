// java/decompiler/attributes.cpp -- see attributes.hpp. Ported from Fernflower (de.fernflower.struct.attr), Copyright 2000-2015 JetBrains s.r.o.,
// Apache License 2.0.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/attributes.hpp"
#include "java/decompiler/annotations.hpp"

#include "core/error.hpp"
#include "core/io.hpp"

namespace cb::java::decompiler {

namespace {

std::string at_text(const ConstantPool& pool, int index) {
    const PrimitiveConstant* p = pool.primitive(static_cast<std::size_t>(index));
    return p != nullptr && std::holds_alternative<std::string>(p->value) ? p->get_string() : std::string();
}

io::ByteReader at_reader(const Attribute& a) { return io::ByteReader(a.info(), "class.attribute"); }

}  // namespace

std::unique_ptr<Attribute> Attribute::create(std::string_view name, ByteView info, const ConstantPool& pool) {
    std::unique_ptr<Attribute> a;
    if (name == attr::INNER_CLASSES) a = std::make_unique<InnerClassesAttribute>();
    else if (name == attr::CONSTANT_VALUE) a = std::make_unique<ConstantValueAttribute>();
    else if (name == attr::SIGNATURE) a = std::make_unique<GenericSignatureAttribute>();
    else if (name == attr::ANNOTATION_DEFAULT) a = std::make_unique<AnnotationDefaultAttribute>();
    else if (name == attr::EXCEPTIONS) a = std::make_unique<ExceptionsAttribute>();
    else if (name == attr::ENCLOSING_METHOD) a = std::make_unique<EnclosingMethodAttribute>();
    else if (name == attr::RUNTIME_VISIBLE_ANNOTATIONS || name == attr::RUNTIME_INVISIBLE_ANNOTATIONS) a = std::make_unique<AnnotationAttribute>();
    else if (name == attr::RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS || name == attr::RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS)
        a = std::make_unique<AnnotationParameterAttribute>();
    else if (name == attr::RUNTIME_VISIBLE_TYPE_ANNOTATIONS || name == attr::RUNTIME_INVISIBLE_TYPE_ANNOTATIONS) a = std::make_unique<AnnotationTypeAttribute>();
    else if (name == attr::LOCAL_VARIABLE_TABLE) a = std::make_unique<LocalVariableTableAttribute>();
    else if (name == attr::BOOTSTRAP_METHODS) a = std::make_unique<BootstrapMethodsAttribute>();
    else if (name == attr::SYNTHETIC || name == attr::DEPRECATED) a = std::make_unique<Attribute>();
    else if (name == attr::LINE_NUMBER_TABLE) a = std::make_unique<LineNumberTableAttribute>();
    else if (name == attr::SOURCE_FILE) a = std::make_unique<SourceFileAttribute>();
    else return nullptr;
    a->name_ = std::string(name);
    a->info_.assign(info.begin(), info.end());
    a->init_content(pool);
    return a;
}

void ConstantValueAttribute::init_content(const ConstantPool&) { index_ = at_reader(*this).u16_be(); }

void ExceptionsAttribute::init_content(const ConstantPool&) {
    io::ByteReader r = at_reader(*this);
    const std::uint16_t n = r.u16_be();
    for (std::uint16_t i = 0; i < n; ++i) throws_.push_back(r.u16_be());
}

std::string ExceptionsAttribute::exception_class_name(std::size_t index, const ConstantPool& pool) const { return at_text(pool, throws_.at(index)); }

void InnerClassesAttribute::init_content(const ConstantPool& pool) {
    io::ByteReader r = at_reader(*this);
    const std::uint16_t n = r.u16_be();
    for (std::uint16_t i = 0; i < n; ++i) {
        Entry e;
        e.inner_class = r.u16_be();
        e.outer_class = r.u16_be();
        e.inner_name = r.u16_be();
        e.access = r.u16_be();
        e.inner_class_name = at_text(pool, e.inner_class);
        if (e.outer_class != 0) e.outer_class_name = at_text(pool, e.outer_class);
        if (e.inner_name != 0) e.simple_name = at_text(pool, e.inner_name);
        entries_.push_back(std::move(e));
    }
}

void LineNumberTableAttribute::init_content(const ConstantPool&) {
    io::ByteReader r = at_reader(*this);
    const std::size_t n = std::size_t{r.u16_be()} * 2;
    line_info_.resize(n);
    for (std::size_t i = 0; i < n; i += 2) {
        line_info_[i] = r.u16_be();
        line_info_[i + 1] = r.u16_be();
    }
}

int LineNumberTableAttribute::find_line_number(int pc) const {
    if (line_info_.size() >= 2)
        for (std::ptrdiff_t i = static_cast<std::ptrdiff_t>(line_info_.size()) - 2; i >= 0; i -= 2)
            if (pc >= line_info_[static_cast<std::size_t>(i)]) return line_info_[static_cast<std::size_t>(i) + 1];
    return -1;
}

void LocalVariableTableAttribute::init_content(const ConstantPool& pool) {
    io::ByteReader r = at_reader(*this);
    const std::uint16_t n = r.u16_be();
    for (std::uint16_t i = 0; i < n; ++i) {
        (void)r.bytes(4);   // start pc, length
        const int name_index = r.u16_be();
        (void)r.bytes(2);   // descriptor
        const int var_index = r.u16_be();
        names_[var_index] = at_text(pool, name_index);
    }
}

void LocalVariableTableAttribute::add_local_variable_table(const LocalVariableTableAttribute& other) {
    for (const auto& [index, name] : other.names_) names_[index] = name;
}

void SourceFileAttribute::init_content(const ConstantPool& pool) { file_name_ = at_text(pool, at_reader(*this).u16_be()); }

void GenericSignatureAttribute::init_content(const ConstantPool& pool) { signature_ = at_text(pool, at_reader(*this).u16_be()); }

void EnclosingMethodAttribute::init_content(const ConstantPool& pool) {
    io::ByteReader r = at_reader(*this);
    const int class_index = r.u16_be();
    const int method_index = r.u16_be();
    class_name_ = at_text(pool, class_index);
    if (method_index != 0)
        if (const LinkConstant* lk = pool.link(static_cast<std::size_t>(method_index))) {
            method_name_ = lk->elementname;
            method_descriptor_ = lk->descriptor;
        }
}

void BootstrapMethodsAttribute::init_content(const ConstantPool& pool) {
    io::ByteReader r = at_reader(*this);
    const std::uint16_t n = r.u16_be();
    for (std::uint16_t i = 0; i < n; ++i) {
        const int method_ref = r.u16_be();
        const std::uint16_t args = r.u16_be();
        std::vector<const PooledConstant*> list;
        for (std::uint16_t j = 0; j < args; ++j) list.push_back(pool.constant(r.u16_be()));
        method_refs_.push_back(pool.link(static_cast<std::size_t>(method_ref)));
        method_arguments_.push_back(std::move(list));
    }
}

}  // namespace cb::java::decompiler
