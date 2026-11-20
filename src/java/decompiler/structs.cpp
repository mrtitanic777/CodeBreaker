// java/decompiler/structs.cpp -- see structs.hpp. Ported from Fernflower (de.fernflower.struct), Copyright 2000-2015 JetBrains s.r.o., Apache License
// 2.0.
#include "java/decompiler/structs.hpp"

#include <algorithm>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "java/console/zip_archive.hpp"
#include "java/decompiler/code_constants.hpp"

namespace cb::java::decompiler {

namespace {

std::string st_text(const ConstantPool& pool, int index) {
    const PrimitiveConstant* p = dynamic_cast<const PrimitiveConstant*>(pool.constant(static_cast<std::size_t>(index)));
    return p != nullptr && std::holds_alternative<std::string>(p->value) ? p->get_string() : std::string();
}

}  // namespace

// ---- StructMember

bool StructMember::is_synthetic() const { return has_modifier(cc::ACC_SYNTHETIC) || attributes_.contains_key(std::string(attr::SYNTHETIC)); }

Attributes StructMember::read_attributes(const std::vector<classfile::Attribute>& list, const classfile::ClassFile& cf, const ConstantPool& pool) {
    Attributes out;
    for (const classfile::Attribute& a : list) {
        const std::string name = st_text(pool, a.name);
        std::shared_ptr<Attribute> made = read_attribute(a, name, cf, pool);
        if (!made) continue;
        if (name == attr::LOCAL_VARIABLE_TABLE && out.contains_key(name)) {   // every variable table merged into the first
            auto* table = dynamic_cast<LocalVariableTableAttribute*>(out.get_with_key(name).get());
            table->add_local_variable_table(dynamic_cast<const LocalVariableTableAttribute&>(*made));
        } else {
            out.add_with_key(made, made->name());
        }
    }
    return out;
}

std::shared_ptr<Attribute> StructMember::read_attribute(const classfile::Attribute& a, const std::string& name, const classfile::ClassFile&, const ConstantPool& pool) {
    return std::shared_ptr<Attribute>(Attribute::create(name, a.data, pool));
}

// ---- StructField

StructField::StructField(const classfile::Member& m, const classfile::ClassFile& cf, const StructClass& owner) {
    access_flags_ = m.access;
    std::tie(name_, descriptor_) = owner.pool().class_element(ConstantPool::FIELD, owner.qualified_name, m.name, m.descriptor);
    attributes_ = read_attributes(m.attributes, cf, owner.pool());
}

// ---- StructMethod

StructMethod::StructMethod(const classfile::Member& m, const classfile::ClassFile& cf, StructClass& owner) : owner_(&owner) {
    access_flags_ = m.access;
    std::tie(name_, descriptor_) = owner.pool().class_element(ConstantPool::METHOD, owner.qualified_name, m.name, m.descriptor);
    attributes_ = read_attributes(m.attributes, cf, owner.pool());
    if (code_attributes_) {   // the Code attribute's own attributes (the line numbers, the variable tables) join the method's
        attributes_.add_all_with_key(*code_attributes_);
        code_attributes_.reset();
    }
}

std::shared_ptr<Attribute> StructMethod::read_attribute(const classfile::Attribute& a, const std::string& name, const classfile::ClassFile& cf, const ConstantPool& pool) {
    if (name == attr::CODE) {
        if (owner_->is_own()) {
            classfile::Code code = classfile::decode_code(a.data);
            contains_code_ = true;
            local_variables_ = code.max_locals;
            code_attributes_ = read_attributes(code.attributes, cf, pool);
            code_ = std::move(code);
        } else {   // a library class: its code is skipped, its attributes read (as the original does)
            io::ByteReader r(a.data, "class.Code");
            (void)r.bytes(4);
            (void)r.bytes(r.u32_be());
            (void)r.bytes(std::size_t{8} * r.u16_be());
            std::vector<classfile::Attribute> list;
            for (std::uint16_t n = r.u16_be(); n > 0; --n) {
                classfile::Attribute inner;
                inner.name = r.u16_be();
                const ByteView data = r.bytes(r.u32_be());
                inner.data.assign(data.begin(), data.end());
                list.push_back(std::move(inner));
            }
            code_attributes_ = read_attributes(list, cf, pool);
        }
        return nullptr;
    }
    return StructMember::read_attribute(a, name, cf, pool);
}

// ---- StructClass

StructClass::StructClass(ByteView bytes, bool own, std::shared_ptr<const PoolInterceptor> interceptor) : own_(own) {
    const classfile::ClassFile cf = classfile::parse(bytes);
    minor_ = cf.minor;
    major_ = cf.major;
    pool_ = std::make_unique<ConstantPool>(cf, std::move(interceptor));
    access_flags_ = cf.access;
    const PrimitiveConstant* this_class = pool_->primitive(cf.this_class);
    qualified_name = this_class != nullptr ? this_class->get_string() : std::string();
    super_class = pool_->primitive(cf.super_class);
    for (const std::uint16_t i : cf.interfaces) {
        interfaces_.push_back(i);
        const PrimitiveConstant* p = pool_->primitive(i);
        interface_names_.push_back(p != nullptr ? p->get_string() : std::string());
    }
    for (const classfile::Member& m : cf.fields) {
        auto field = std::make_shared<StructField>(m, cf, *this);
        fields_.add_with_key(field, unique_key(field->name(), field->descriptor()));
    }
    for (const classfile::Member& m : cf.methods) {
        auto method = std::make_shared<StructMethod>(m, cf, *this);
        methods_.add_with_key(method, unique_key(method->name(), method->descriptor()));
    }
    attributes_ = read_attributes(cf.attributes, cf, *pool_);
}

StructField* StructClass::get_field(const std::string& name, const std::string& descriptor) const { return fields_.get_with_key(unique_key(name, descriptor)).get(); }

StructMethod* StructClass::get_method(const std::string& key) const { return methods_.get_with_key(key).get(); }

StructMethod* StructClass::get_method(const std::string& name, const std::string& descriptor) const { return get_method(unique_key(name, descriptor)); }

int StructClass::bytecode_version() const {
    switch (major_) {
        case 52: return cc::BYTECODE_JAVA_8;
        case 51: return cc::BYTECODE_JAVA_7;
        case 50: return cc::BYTECODE_JAVA_6;
        case 49: return cc::BYTECODE_JAVA_5;
        default: return cc::BYTECODE_JAVA_LE_4;
    }
}

// ---- StructContext

StructContext::StructContext(Logger log, std::shared_ptr<const PoolInterceptor> interceptor) : log_(std::move(log)), interceptor_(std::move(interceptor)) {
    ContextUnit unit;   // the default unit: the classes given one by one
    units_.emplace("", std::move(unit));
}

StructClass* StructContext::get_class(const std::string& name) const {
    const auto it = classes_.find(name);
    return it == classes_.end() ? nullptr : it->second.get();
}

void StructContext::add_space(const std::filesystem::path& file, bool own) { add_space("", file, own, 0); }

void StructContext::add_class(ContextUnit& unit, ByteView bytes, bool own, const std::string& entry, const std::filesystem::path& file) {
    try {
        auto cl = std::make_unique<StructClass>(bytes, own, interceptor_);
        StructClass* raw = cl.get();
        classes_[raw->qualified_name] = std::move(cl);
        unit.classes.push_back(raw);
        unit.class_entries.push_back(entry);
    } catch (const std::exception& ex) {
        if (log_) log_("Corrupted class file: " + fs::to_text(file) + " (" + ex.what() + ")");
    }
}

void StructContext::add_space(const std::string& path_in, const std::filesystem::path& file, bool own, int level) {
    std::error_code ec;
    std::string path = path_in;
    if (std::filesystem::is_directory(file, ec)) {
        if (level == 1) path += fs::to_text(file.filename());
        else if (level > 1) path += "/" + fs::to_text(file.filename());
        std::vector<std::filesystem::path> files;
        for (const auto& e : std::filesystem::directory_iterator(file, ec)) files.push_back(e.path());
        std::sort(files.begin(), files.end());
        for (auto it = files.rbegin(); it != files.rend(); ++it) add_space(path, *it, own, level + 1);   // (in reverse, as the original)
        return;
    }
    const std::string filename = fs::to_text(file.filename());
    const bool jar = filename.ends_with(".jar"), zip = filename.ends_with(".zip");
    if (jar || zip) {
        try {
            add_archive(path, file, jar ? ContextUnit::TYPE_JAR : ContextUnit::TYPE_ZIP, own);
        } catch (const std::exception& ex) {
            if (log_) log_("Corrupted archive file: " + fs::to_text(file) + " (" + ex.what() + ")");
        }
        return;
    }
    ContextUnit& unit = units_[path];
    if (unit.filename.empty() && unit.classes.empty() && unit.other_entries.empty()) {
        unit.type = ContextUnit::TYPE_FOLDER;
        unit.filename = path;
        unit.own = own;
    }
    if (filename.ends_with(".class")) {
        const Bytes bytes = fs::read_file(file);
        add_class(unit, bytes, own, filename, file);
    } else {
        unit.other_entries.emplace_back(fs::to_text(std::filesystem::absolute(file)), filename);
    }
}

void StructContext::add_archive(const std::string& external_path, const std::filesystem::path& file, int type, bool own) {
    const std::string key = external_path + "/" + fs::to_text(file);
    auto [it, fresh] = units_.try_emplace(key);
    ContextUnit& unit = it->second;
    if (fresh) {
        unit.type = type;
        unit.archive_path = external_path;
        unit.filename = fs::to_text(file.filename());
        unit.own = own;
    }
    const console::ZipArchive zip = console::ZipArchive::open(file);
    std::set<std::string> dirs;
    for (const console::ZipArchive::Entry& e : zip.entries()) {
        const std::string& name = e.name;   // ('/' already)
        if (e.is_directory()) {
            dirs.insert(name);
        } else if (name.ends_with(".class")) {
            if (const std::size_t slash = name.rfind('/'); slash != std::string::npos) dirs.insert(name.substr(0, slash));
            Bytes bytes;
            try {
                bytes = zip.read(e);
            } catch (const std::exception& ex) {
                if (log_) log_("Corrupted class file: " + fs::to_text(file) + " (" + ex.what() + ")");
                continue;
            }
            add_class(unit, bytes, own, name, file);
        } else {
            if (name == "META-INF/MANIFEST.MF") {
                const Bytes m = zip.read(e);
                unit.manifest = std::string(reinterpret_cast<const char*>(m.data()), m.size());
            }
            unit.other_entries.emplace_back(fs::to_text(std::filesystem::absolute(file)), name);
        }
    }
    for (const std::string& d : dirs) unit.dir_entries.push_back(d);   // (the original's set order is a hash order; the entries' order does not change the output)
}

}  // namespace cb::java::decompiler
