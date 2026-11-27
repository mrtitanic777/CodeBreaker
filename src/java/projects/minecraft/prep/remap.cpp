// remap.cpp -- the mappings of a Minecraft version and the remapping of its classes with them; see java/projects/minecraft/prep/remap.hpp.
//
// Ported from RDI (org.mcphackers.rdi.injector.remapper.Remapper / FieldRenameMap / MethodRenameMap, injector.data.Mappings, nio.MappingsIO, whose
// tiny readers are Fabric's mapping-io: Tiny1FileReader / Tiny2FileReader / MemoryMappingTree) and RetroMCP (tools.mappings.MappingUtil.readNamespaces,
// tasks.TaskDecompile.getMappings), MIT License, (c) MCPHackers (mapping-io: Apache-2.0, (c) FabricMC).
// cb-unity: alone (file-local helpers of the port)
#include "java/projects/minecraft/prep/remap.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <functional>
#include <map>
#include <stdexcept>
#include <unordered_set>

#include "core/fs.hpp"

namespace cb::java::minecraft::prep {

namespace {

using bc::op::ACC_PRIVATE;
using bc::op::ACC_PROTECTED;
using bc::op::ACC_PUBLIC;
using bc::op::ACC_STATIC;

}  // namespace

// ---- FieldRenameMap / MethodRenameMap

std::optional<std::string> FieldRenameMap::get(const std::string& owner, const std::string& descriptor, const std::string& old_name) const {
    const auto it = renames_.find(FieldReference{owner, old_name, descriptor});
    if (it == renames_.end()) return std::nullopt;
    return it->second;
}

std::optional<std::string> FieldRenameMap::get_or_default(const std::string& owner, const std::string& descriptor, const std::string& old_name,
                                                          const std::optional<std::string>& default_value) const {
    const auto it = renames_.find(FieldReference{owner, old_name, descriptor});
    if (it == renames_.end()) return default_value;
    return it->second;
}

std::string FieldRenameMap::opt_get(const std::string& owner, const std::string& descriptor, const std::string& old_name) const {
    const auto it = renames_.find(FieldReference{owner, old_name, descriptor});
    if (it == renames_.end()) return old_name;
    return it->second;
}

void FieldRenameMap::put(const std::string& owner, const std::string& descriptor, const std::string& name, const std::string& new_name) {
    renames_[FieldReference{owner, name, descriptor}] = new_name;
}

void FieldRenameMap::put_all(const FieldRenameMap& other) {
    for (const auto& [k, v] : other.renames_) renames_[k] = v;
}

std::optional<std::string> MethodRenameMap::get(const std::string& owner, const std::string& descriptor, const std::string& old_name) const {
    const auto it = renames_.find(MethodReference{owner, old_name, descriptor});
    if (it == renames_.end()) return std::nullopt;
    return it->second;
}

std::optional<std::string> MethodRenameMap::get_or_default(const std::string& owner, const std::string& descriptor, const std::string& old_name,
                                                           const std::optional<std::string>& default_value) const {
    const auto it = renames_.find(MethodReference{owner, old_name, descriptor});
    if (it == renames_.end()) return default_value;
    return it->second;
}

std::string MethodRenameMap::opt_get(const std::string& owner, const std::string& descriptor, const std::string& old_name) const {
    const auto it = renames_.find(MethodReference{owner, old_name, descriptor});
    if (it == renames_.end()) return old_name;
    return it->second;
}

void MethodRenameMap::put(const std::string& owner, const std::string& descriptor, const std::string& name, const std::string& new_name) {
    renames_[MethodReference{owner, name, descriptor}] = new_name;
}

void MethodRenameMap::remove(const std::string& owner, const std::string& desc, const std::string& name) {
    renames_.erase(MethodReference{owner, name, desc});
}

void MethodRenameMap::put_all(const MethodRenameMap& other) {
    for (const auto& [k, v] : other.renames_) renames_[k] = v;
    for (const auto& [k, v] : other.lvt_) lvt_[k] = v;
}

const ParameterNames* MethodRenameMap::get_lv_mappings(const std::string& owner, const std::string& descriptor, const std::string& old_name) const {
    const auto it = lvt_.find(MethodReference{owner, old_name, descriptor});
    if (it == lvt_.end() || !it->second) return nullptr;
    return &*it->second;
}

void MethodRenameMap::set_lv_mappings(const std::string& owner, const std::string& descriptor, const std::string& old_name,
                                      const ParameterNames* params) {
    auto& slot = lvt_[MethodReference{owner, old_name, descriptor}];
    if (params != nullptr) slot = *params;
    else slot = std::nullopt;
}

// ---- the tiny files (mapping-io's Tiny1FileReader / Tiny2FileReader into a MemoryMappingTree)

namespace {

// (an IOException of mapping-io: the file is damaged)
struct TinyIoError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

using Names = std::vector<std::optional<std::string>>;   // by destination namespace

struct TinyArg {
    int lv_index = -1;
    std::optional<std::string> src_name;
    Names dst;
};

struct TinyMember {
    std::string src_name, src_desc;
    Names dst;
    std::vector<TinyArg> args;   // methods
};

struct TinyClass {
    std::string src_name;
    Names dst;
    std::vector<TinyMember> fields, methods;   // in the order first seen
    std::unordered_map<std::string, std::size_t> field_index, method_index;   // by name + NUL + descriptor
};

// MemoryMappingTree as the tiny readers fill it (one pass: the source namespace is the file's first column)
struct TinyTree {
    std::string src_namespace;
    std::vector<std::string> dst_namespaces;
    std::vector<TinyClass> classes;   // in the order first seen
    std::unordered_map<std::string, std::size_t> class_index;

    int get_namespace_id(const std::string& ns) const {
        if (ns == src_namespace) return -1;
        const auto it = std::find(dst_namespaces.begin(), dst_namespaces.end(), ns);
        return it == dst_namespaces.end() ? -2 : static_cast<int>(it - dst_namespaces.begin());
    }

    TinyClass& visit_class(const std::string& src_name) {
        const auto it = class_index.find(src_name);
        if (it != class_index.end()) return classes[it->second];
        TinyClass c;
        c.src_name = src_name;
        c.dst.assign(dst_namespaces.size(), std::nullopt);
        class_index[src_name] = classes.size();
        classes.push_back(std::move(c));
        return classes.back();
    }
    TinyMember& visit_member(TinyClass& c, bool is_method, const std::string& name, const std::string& desc) {
        auto& list = is_method ? c.methods : c.fields;
        auto& index = is_method ? c.method_index : c.field_index;
        const std::string key = name + '\0' + desc;
        const auto it = index.find(key);
        if (it != index.end()) return list[it->second];
        TinyMember m;
        m.src_name = name;
        m.src_desc = desc;
        m.dst.assign(dst_namespaces.size(), std::nullopt);
        index[key] = list.size();
        list.push_back(std::move(m));
        return list.back();
    }
    // MethodEntry.getArg(-1, lvIndex, srcName): an argument of that local variable index whose source name is compatible (or none)
    TinyArg& visit_method_arg(TinyMember& method, int lv_index, const std::optional<std::string>& src_name) {
        for (TinyArg& a : method.args) {
            if (lv_index < 0 || a.lv_index != lv_index) continue;
            if (!src_name || !a.src_name || *src_name == *a.src_name) {
                if (src_name) a.src_name = src_name;
                return a;
            }
        }
        TinyArg a;
        a.lv_index = lv_index;
        a.src_name = src_name;
        a.dst.assign(dst_namespaces.size(), std::nullopt);
        method.args.push_back(std::move(a));
        return method.args.back();
    }

    // MappingTreeView.mapClassName(name, -1, namespace)
    std::string map_class_name(const std::string& name, int ns) const {
        if (ns == -1) return name;
        const auto it = class_index.find(name);
        if (it == class_index.end()) return name;
        const auto& ret = classes[it->second].dst.at(static_cast<std::size_t>(ns));
        return ret ? *ret : name;
    }
    // MappingTreeView.mapDesc(desc, -1, namespace)
    std::string map_desc(const std::string& desc, int ns) const {
        std::string ret;
        bool changed = false;
        std::size_t copy_offset = 0, offset = 0;
        const std::size_t end = desc.size();
        while (true) {
            char c;
            do {
                if (offset >= end) {
                    if (!changed) return desc;
                    ret.append(desc, copy_offset, end - copy_offset);
                    return ret;
                }
                c = desc[offset++];
            } while (c != 'L');
            std::size_t id_end = offset;
            while (id_end < end && desc[id_end] != ';') ++id_end;
            if (id_end >= end) throw std::invalid_argument("invalid descriptor: " + desc);
            const std::string cls = desc.substr(offset, id_end - offset);
            const std::string mapped = map_class_name(cls, ns);
            if (mapped != cls) {
                changed = true;
                ret.append(desc, copy_offset, offset - copy_offset);
                ret += mapped;
                copy_offset = id_end;
            }
            offset = id_end + 1;
        }
    }

    // ElementMappingView.getName(int) / MemberMappingView.getDesc(int); an unknown namespace (-2) names nothing
    std::optional<std::string> class_name(const TinyClass& c, int ns) const {
        if (ns == -2) return std::nullopt;
        if (ns == -1) return c.src_name;
        return c.dst.at(static_cast<std::size_t>(ns));
    }
    std::optional<std::string> member_name(const TinyMember& m, int ns) const {
        if (ns == -2) return std::nullopt;
        if (ns == -1) return m.src_name;
        return m.dst.at(static_cast<std::size_t>(ns));
    }
    std::optional<std::string> member_desc(const TinyMember& m, int ns) const {
        if (ns == -2) return std::nullopt;
        if (ns >= 0) return map_desc(m.src_desc, ns);
        return m.src_desc;
    }
    std::optional<std::string> arg_name(const TinyArg& a, int ns) const {
        if (ns == -2) return std::nullopt;
        if (ns == -1) return a.src_name;
        return a.dst.at(static_cast<std::size_t>(ns));
    }
};

// Tiny2Util.unescape
std::string tiny_unescape(const std::string& str) {
    std::size_t pos = str.find('\\');
    if (pos == std::string::npos) return str;
    std::string ret;
    std::size_t start = 0;
    do {
        ret.append(str, start, pos - start);
        ++pos;
        if (pos >= str.size()) throw std::runtime_error("incomplete escape sequence at the end");
        switch (str[pos]) {
            case '\\': ret += '\\'; break;
            case 'n': ret += '\n'; break;
            case 'r': ret += '\r'; break;
            case '0': ret += '\0'; break;
            case 't': ret += '\t'; break;
            default: throw std::runtime_error(std::string("invalid escape character: \\") + str[pos]);
        }
        start = pos + 1;
    } while ((pos = str.find('\\', start)) != std::string::npos);
    ret.append(str, start, std::string::npos);
    return ret;
}

// The lines of the file as mapping-io's ColumnFileReader hands them out: a line is taken by the indentation (tabs) it starts with, its columns are
// separated by tabs, a column past the end is none, a trailing tab ends with an empty column.
class TinyLines {
public:
    explicit TinyLines(const std::string& text) {
        std::size_t start = 0;
        while (start < text.size()) {
            std::size_t nl = text.find('\n', start);
            if (nl == std::string::npos) nl = text.size();
            std::string line = text.substr(start, nl - start);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            lines_.push_back(std::move(line));
            start = nl + 1;
        }
    }

    // nextLine(indent): the next line when it starts with `indent` tabs (at level 0 the empty lines before it are skipped); its columns are then read
    bool next_line(int indent) {
        if (indent == 0) {
            while (pos_ < lines_.size() && lines_[pos_].empty()) ++pos_;
        }
        if (pos_ >= lines_.size()) return false;
        const std::string& line = lines_[pos_];
        const std::size_t n = static_cast<std::size_t>(indent);
        if (line.size() < n) return false;
        for (std::size_t i = 0; i < n; ++i) {
            if (line[i] != '\t') return false;
        }
        set_columns(line, n);
        ++pos_;
        return true;
    }
    // (the first line: no indentation)
    void first_line() {
        if (lines_.empty()) throw TinyIoError("invalid/unsupported tiny file: no header");
        set_columns(lines_[0], 0);
        pos_ = 1;
    }

    bool next_col(const char* expected) {
        if (col_ < cols_.size() && cols_[col_] == expected) {
            ++col_;
            return true;
        }
        return false;
    }
    std::optional<std::string> next_col(bool unescape = false) {
        if (col_ >= cols_.size()) return std::nullopt;
        const std::string& c = cols_[col_++];
        return unescape ? tiny_unescape(c) : c;
    }
    int next_int_col() {
        const auto str = next_col(false);
        if (!str) return -1;
        std::size_t used = 0;
        int v = 0;
        try {
            v = std::stoi(*str, &used);
        } catch (const std::exception&) {
            used = 0;
        }
        if (used != str->size() || str->empty()) throw TinyIoError("invalid number in line " + std::to_string(pos_) + ": " + *str);
        return v;
    }
    bool is_at_eol() const { return col_ >= cols_.size(); }
    std::size_t line_number() const { return pos_; }

private:
    void set_columns(const std::string& line, std::size_t from) {
        cols_.clear();
        col_ = 0;
        std::size_t start = from;
        while (true) {
            const std::size_t tab = line.find('\t', start);
            if (tab == std::string::npos) {
                cols_.push_back(line.substr(start));
                break;
            }
            cols_.push_back(line.substr(start, tab - start));
            start = tab + 1;
        }
    }

    std::vector<std::string> lines_;
    std::size_t pos_ = 0;
    std::vector<std::string> cols_;
    std::size_t col_ = 0;
};

void read_dst_names(TinyLines& r, Names& dst, bool escape) {
    for (std::size_t i = 0; i < dst.size(); ++i) {
        const auto name = r.next_col(escape);
        if (!name) throw TinyIoError("missing name columns in line " + std::to_string(r.line_number()));
        if (!name->empty()) dst[i] = *name;
    }
}

void read_comment(TinyLines& r) {
    const auto comment = r.next_col(true);
    if (!comment) throw TinyIoError("missing comment in line " + std::to_string(r.line_number()));
}

// (Tiny2FileReader.readElement: the names, then the comments below)
void read_element(TinyLines& r, Names& dst, int kind_level, bool escape) {
    read_dst_names(r, dst, escape);
    while (r.next_line(kind_level + 1)) {
        if (r.next_col("c")) read_comment(r);
    }
}

void read_method(TinyLines& r, TinyTree& tree, TinyMember& method, bool escape) {
    read_dst_names(r, method.dst, escape);
    while (r.next_line(2)) {
        if (r.next_col("p")) {
            const int lv_index = r.next_int_col();
            if (lv_index < 0) throw TinyIoError("missing/invalid parameter-lv-index in line " + std::to_string(r.line_number()));
            auto src_name = r.next_col(escape);
            if (!src_name) throw TinyIoError("missing parameter-name-a column in line " + std::to_string(r.line_number()));
            if (src_name->empty()) src_name = std::nullopt;
            TinyArg& arg = tree.visit_method_arg(method, lv_index, src_name);
            read_element(r, arg.dst, 2, escape);
        } else if (r.next_col("v")) {
            const int lv_index = r.next_int_col();
            if (lv_index < 0) throw TinyIoError("missing/invalid variable-lv-index in line " + std::to_string(r.line_number()));
            const int start_op_idx = r.next_int_col();
            if (start_op_idx < 0) throw TinyIoError("missing/invalid variable-lv-start-offset in line " + std::to_string(r.line_number()));
            r.next_int_col();   // the lvt row index
            const auto src_name = r.next_col(escape);
            if (!src_name) throw TinyIoError("missing variable-name-a column in line " + std::to_string(r.line_number()));
            Names unused(tree.dst_namespaces.size());   // (the variables are not kept: RDI reads none)
            read_element(r, unused, 2, escape);
        } else if (r.next_col("c")) {
            read_comment(r);
        }
    }
}

void read_class(TinyLines& r, TinyTree& tree, TinyClass& cls, bool escape) {
    read_dst_names(r, cls.dst, escape);
    while (r.next_line(1)) {
        if (r.next_col("f")) {
            const auto src_desc = r.next_col(escape);
            if (!src_desc || src_desc->empty()) throw TinyIoError("missing field-desc-a in line " + std::to_string(r.line_number()));
            const auto src_name = r.next_col(escape);
            if (!src_name || src_name->empty()) throw TinyIoError("missing field-name-a in line " + std::to_string(r.line_number()));
            TinyMember& field = tree.visit_member(cls, false, *src_name, *src_desc);
            read_element(r, field.dst, 1, escape);
        } else if (r.next_col("m")) {
            const auto src_desc = r.next_col(escape);
            if (!src_desc || src_desc->empty()) throw TinyIoError("missing method-desc-a in line " + std::to_string(r.line_number()));
            const auto src_name = r.next_col(escape);
            if (!src_name || src_name->empty()) throw TinyIoError("missing method-name-a in line " + std::to_string(r.line_number()));
            TinyMember& method = tree.visit_member(cls, true, *src_name, *src_desc);
            read_method(r, tree, method, escape);
        } else if (r.next_col("c")) {
            read_comment(r);
        }
    }
}

void read_tiny2(TinyLines& r, TinyTree& tree) {
    r.first_line();
    if (!(r.next_col("tiny") && r.next_int_col() == 2 && r.next_int_col() >= 0)) throw TinyIoError("invalid/unsupported tiny file: no tiny 2 header");
    const auto src = r.next_col();
    if (!src || src->empty()) throw TinyIoError("no source namespace in Tiny v2 header");
    tree.src_namespace = *src;
    while (!r.is_at_eol()) {
        const auto ns = r.next_col();
        if (!ns || ns->empty()) throw TinyIoError("empty destination namespace in Tiny v2 header");
        tree.dst_namespaces.push_back(*ns);
    }
    for (const auto& ns : tree.dst_namespaces) {
        if (ns == tree.src_namespace) {
            throw std::invalid_argument("namespace \"" + ns + "\" is present on both source and destination side simultaneously");
        }
    }
    bool escape = false;
    while (r.next_line(1)) {
        const auto key = r.next_col();
        if (!key) throw TinyIoError("missing property key in line " + std::to_string(r.line_number()));
        r.next_col(true);   // the value (unescaped: a bad escape is an error)
        if (*key == "escaped-names") escape = true;
    }
    while (r.next_line(0)) {
        if (r.next_col("c")) {
            const auto name = r.next_col(escape);
            if (!name || name->empty()) throw TinyIoError("missing class-name-a in line " + std::to_string(r.line_number()));
            read_class(r, tree, tree.visit_class(*name), escape);
        }
    }
}

void read_tiny1(TinyLines& r, TinyTree& tree) {
    r.first_line();
    if (!r.next_col("v1")) throw TinyIoError("invalid/unsupported tiny file: no tiny 1 header");
    const auto src = r.next_col();
    if (!src || src->empty()) throw TinyIoError("no source namespace in Tiny v1 header");
    tree.src_namespace = *src;
    while (!r.is_at_eol()) {
        const auto ns = r.next_col();
        if (!ns || ns->empty()) throw TinyIoError("empty destination namespace in Tiny v1 header");
        tree.dst_namespaces.push_back(*ns);
    }
    for (const auto& ns : tree.dst_namespaces) {
        if (ns == tree.src_namespace) {
            throw std::invalid_argument("namespace \"" + ns + "\" is present on both source and destination side simultaneously");
        }
    }
    std::optional<std::string> last_class;
    TinyClass* cls = nullptr;
    while (r.next_line(0)) {
        if (r.next_col("CLASS")) {
            const auto name = r.next_col();
            if (!name || name->empty()) throw TinyIoError("missing class-name-a in line " + std::to_string(r.line_number()));
            last_class = *name;
            cls = &tree.visit_class(*name);
            read_dst_names(r, cls->dst, false);
        } else {
            const bool is_method = r.next_col("METHOD");
            if (!is_method && !r.next_col("FIELD")) continue;   // (a comment line or the intermediary counters: nothing RDI reads)
            const auto name = r.next_col();
            if (!name || name->empty()) throw TinyIoError("missing class-name-a in line " + std::to_string(r.line_number()));
            if (!last_class || *name != *last_class) {
                last_class = *name;
                cls = &tree.visit_class(*name);
            }
            const auto desc = r.next_col();
            if (!desc || desc->empty()) throw TinyIoError("missing member-desc-a in line " + std::to_string(r.line_number()));
            const auto member_name = r.next_col();
            if (!member_name || member_name->empty()) throw TinyIoError("missing member-name-a in line " + std::to_string(r.line_number()));
            TinyMember& m = tree.visit_member(*cls, is_method, *member_name, *desc);
            read_dst_names(r, m.dst, false);
        }
    }
}

// MappingsIO.getParameterMappings
ParameterNames parameter_mappings(const TinyTree& tree, const std::vector<TinyArg>& args, int target) {
    int max_index = -1;
    for (const TinyArg& a : args) max_index = std::max(a.lv_index, max_index);
    ParameterNames params(static_cast<std::size_t>(max_index + 1));
    for (const TinyArg& a : args) params[static_cast<std::size_t>(a.lv_index)] = tree.arg_name(a, target);
    return params;
}

Mappings read_mappings(const std::filesystem::path& path, const std::function<std::pair<int, int>(const TinyTree&)>& namespaces) {
    Mappings mappings;
    const auto provider = MappingsIO::get_provider(path);
    if (!provider) return mappings;
    TinyTree tree;
    try {
        const std::string text = fs::read_text(path);
        TinyLines lines(text);
        if (*provider == Mappings::Provider::TINY1) read_tiny1(lines, tree);
        else read_tiny2(lines, tree);
    } catch (const TinyIoError& e) {
        std::fprintf(stderr, "%s: %s\n", path.string().c_str(), e.what());
        return mappings;
    }
    const auto [src, target] = namespaces(tree);
    for (const TinyClass& cls : tree.classes) {
        std::string class_name;
        if (const auto n = tree.class_name(cls, src)) {
            class_name = *n;
            if (const auto t = tree.class_name(cls, target)) mappings.classes[*n] = *t;
        } else {
            class_name = cls.src_name;
        }
        for (const TinyMember& f : cls.fields) {
            const auto from = tree.member_name(f, src);
            const auto to = tree.member_name(f, target);
            if (!from || !to) continue;
            mappings.fields.put(class_name, tree.member_desc(f, src).value_or(""), *from, *to);
        }
        for (const TinyMember& m : cls.methods) {
            const auto from = tree.member_name(m, src);
            const auto to = tree.member_name(m, target);
            if (!from || !to) continue;
            const std::string desc = tree.member_desc(m, src).value_or("");
            mappings.methods.put(class_name, desc, *from, *to);
            const ParameterNames params = parameter_mappings(tree, m.args, target);
            mappings.methods.set_lv_mappings(class_name, desc, *from, &params);
        }
    }
    return mappings;
}

std::string read_first_line(const std::filesystem::path& path, bool& readable) {
    readable = true;
    std::string text;
    try {
        text = fs::read_text(path);
    } catch (const std::exception& e) {
        std::fprintf(stderr, "%s\n", e.what());
        readable = false;
        return {};
    }
    const std::size_t end = text.find_first_of("\r\n");
    return end == std::string::npos ? text : text.substr(0, end);
}

}  // namespace

Mappings MappingsIO::read(const std::filesystem::path& path, const std::string& src_namespace, const std::string& target_namespace) {
    return read_mappings(path, [&](const TinyTree& t) { return std::pair{t.get_namespace_id(src_namespace), t.get_namespace_id(target_namespace)}; });
}

Mappings MappingsIO::read(const std::filesystem::path& path, int src_namespace, int target_namespace) {
    return read_mappings(path, [&](const TinyTree&) { return std::pair{src_namespace, target_namespace}; });
}

std::optional<Mappings::Provider> MappingsIO::get_provider(const std::filesystem::path& mappings) {
    bool readable = false;
    const std::string header = read_first_line(mappings, readable);
    if (!readable) return std::nullopt;
    if (header.starts_with("tiny\t2\t0\t")) return Mappings::Provider::TINY2;
    if (header.starts_with("v1\t")) return Mappings::Provider::TINY1;
    return std::nullopt;
}

std::vector<std::string> read_namespaces(const std::filesystem::path& mappings) {
    bool readable = false;
    std::string header = read_first_line(mappings, readable);
    std::vector<std::string> namespaces;
    bool invalid = !readable;
    std::string rest;
    if (!invalid) {
        if (header.starts_with("tiny\t2\t0\t")) rest = header.substr(9);
        else if (header.starts_with("v1\t")) rest = header.substr(3);
        else invalid = true;
    }
    if (invalid) throw std::runtime_error("No valid tiny header in " + mappings.string());
    // String.trim(), then split("\t") (the trailing empty strings are dropped; nothing left is one empty string)
    const auto is_blank = [](char c) { return static_cast<unsigned char>(c) <= ' '; };
    std::size_t b = 0, e = rest.size();
    while (b < e && is_blank(rest[b])) ++b;
    while (e > b && is_blank(rest[e - 1])) --e;
    rest = rest.substr(b, e - b);
    std::size_t start = 0;
    while (true) {
        const std::size_t tab = rest.find('\t', start);
        if (tab == std::string::npos) {
            namespaces.push_back(rest.substr(start));
            break;
        }
        namespaces.push_back(rest.substr(start, tab - start));
        start = tab + 1;
    }
    while (namespaces.size() > 1 && namespaces.back().empty()) namespaces.pop_back();
    return namespaces;
}

std::optional<Mappings> get_mappings(const std::filesystem::path& mappings_path, const ClassStorage& storage, const std::string& side_name) {
    if (!std::filesystem::exists(mappings_path)) return std::nullopt;
    const std::vector<std::string> namespaces = read_namespaces(mappings_path);
    const bool joined = std::find(namespaces.begin(), namespaces.end(), "official") != namespaces.end();
    Mappings mappings = MappingsIO::read(mappings_path, joined ? "official" : side_name, "named");
    for (const std::string& name : storage.get_all_classes()) {
        if (name.find('/') == std::string::npos && !mappings.classes.contains(name)) mappings.classes[name] = "net/minecraft/src/" + name;
    }
    return mappings;
}

// ---- Remapper

namespace {

using NameSet = std::unordered_set<std::string>;
using Children = std::unordered_map<std::string, NameSet>;

// (Set.addAll: whether any was added)
bool add_all(NameSet& to, const NameSet& from) {
    if (&to == &from) return false;
    bool modified = false;
    for (const std::string& s : from) modified |= to.insert(s).second;
    return modified;
}

}  // namespace

std::optional<std::string> Remapper::mapped_class(const std::string& name) const {
    const auto it = mappings_->classes.find(name);
    if (it == mappings_->classes.end()) return std::nullopt;
    return it->second;
}

void Remapper::create_method_hierarchy() {
    hierarchised_method_renames_.clear();
    Children children;
    for (bc::ClassNode* node : *storage_) {
        std::vector<std::string> parents;
        if (node->super_name) parents.push_back(*node->super_name);
        parents.insert(parents.end(), node->interfaces.begin(), node->interfaces.end());
        for (const std::string& parent : parents) children[parent].insert(node->name);
    }
    bool modified;
    do {
        modified = false;
        for (bc::ClassNode* node : *storage_) {
            const auto own = children.find(node->name);
            if (own == children.end()) continue;
            if (node->super_name) modified |= add_all(children[*node->super_name], own->second);
            for (const std::string& s : node->interfaces) modified |= add_all(children[s], own->second);
        }
    } while (modified);
    for (bc::ClassNode* node : *storage_) {
        for (bc::MethodNode& method : node->methods) {
            const auto new_name = mappings_->methods.get(node->name, method.desc, method.name);
            const ParameterNames* params = mappings_->methods.get_lv_mappings(node->name, method.desc, method.name);
            if (!new_name) continue;
            hierarchised_method_renames_.put(node->name, method.desc, method.name, *new_name);
            hierarchised_method_renames_.set_lv_mappings(node->name, method.desc, method.name, params);
            const auto own = children.find(node->name);
            if (own == children.end()) continue;
            const NameSet& child_nodes = own->second;
            bool skip = false;
            if ((method.access & ACC_STATIC) != 0) {
                for (const std::string& child : child_nodes) {
                    bc::ClassNode* child_node = storage_->get_class(child);
                    if (child_node == nullptr) continue;
                    for (const bc::MethodNode& method2 : child_node->methods) {
                        if (method2.name == method.name && method2.desc == method.desc) {
                            skip = true;
                            break;
                        }
                    }
                    if (skip) break;
                }
            }
            if (skip) continue;
            if ((method.access & ACC_PROTECTED) != 0 || (method.access & ACC_PUBLIC) != 0) {
                for (const std::string& child : child_nodes) {
                    hierarchised_method_renames_.put(child, method.desc, method.name, *new_name);
                    hierarchised_method_renames_.set_lv_mappings(child, method.desc, method.name, params);
                }
            } else if ((method.access & ACC_PRIVATE) == 0) {
                // Package-protected
                for (const std::string& child : child_nodes) {
                    if (ClassStorage::in_one_package(node->name, child)) {
                        hierarchised_method_renames_.put(child, method.desc, method.name, *new_name);
                        hierarchised_method_renames_.set_lv_mappings(child, method.desc, method.name, params);
                    }
                }
            }
        }
    }
    hierarchised_method_renames_.put_all(mappings_->methods);
}

void Remapper::create_field_hierarchy() {
    hierarchised_field_renames_.clear();
    Children children;
    for (bc::ClassNode* node : *storage_) {
        if (node->super_name) children[*node->super_name].insert(node->name);
    }
    bool modified;
    do {
        modified = false;
        for (bc::ClassNode* node : *storage_) {
            const auto own = children.find(node->name);
            if (own == children.end()) continue;
            if (node->super_name) modified |= add_all(children[*node->super_name], own->second);
        }
    } while (modified);
    for (bc::ClassNode* node : *storage_) {
        for (bc::FieldNode& field : node->fields) {
            const auto new_name = mappings_->fields.get(node->name, field.desc, field.name);
            if (!new_name) continue;
            hierarchised_field_renames_.put(node->name, field.desc, field.name, *new_name);
            const auto own = children.find(node->name);
            if (own == children.end()) continue;
            const NameSet& child_nodes = own->second;
            bool skip = false;
            for (const std::string& child : child_nodes) {
                bc::ClassNode* child_node = storage_->get_class(child);
                if (child_node == nullptr) continue;
                for (const bc::FieldNode& field2 : child_node->fields) {
                    if (field2.name == field.name && field2.desc == field.desc) {
                        skip = true;
                        break;
                    }
                }
                if (skip) break;
            }
            if (skip) continue;
            if ((field.access & ACC_PROTECTED) != 0 || (field.access & ACC_PUBLIC) != 0) {
                for (const std::string& child : child_nodes) hierarchised_field_renames_.put(child, field.desc, field.name, *new_name);
            } else if ((field.access & ACC_PRIVATE) == 0) {
                // Package-protected
                const std::size_t slash = node->name.rfind('/');
                const std::string package_name = slash != std::string::npos ? node->name.substr(0, slash) : "";
                for (const std::string& child : child_nodes) {
                    const std::size_t slash2 = child.rfind('/');
                    const std::string package_name2 = slash2 != std::string::npos ? child.substr(0, slash2) : "";
                    if (package_name == package_name2) hierarchised_field_renames_.put(child, field.desc, field.name, *new_name);
                }
            }
        }
    }
    hierarchised_field_renames_.put_all(mappings_->fields);
}

void Remapper::process() {
    create_method_hierarchy();
    create_field_hierarchy();

    for (bc::ClassNode* node : *storage_) {
        for (bc::FieldNode& field : node->fields) remap_field(node->name, field);
        for (bc::InnerClassNode& inner : node->inner_classes) {
            // (the inner names are not remapped)
            if (inner.outer_name) {
                if (const auto new_outer = mapped_class(*inner.outer_name)) inner.outer_name = *new_outer;
            }
            if (const auto new_name = mapped_class(inner.name)) inner.name = *new_name;
        }
        for (std::string& itf : node->interfaces) {
            if (const auto n = mapped_class(itf)) itf = *n;
        }
        remap_annotations(node->invisible_type_annotations);
        remap_annotations(node->invisible_annotations);
        remap_annotations(node->visible_type_annotations);
        remap_annotations(node->visible_annotations);
        for (bc::MethodNode& method : node->methods) remap_method(*node, method);
        // (a module's names are kept as read: Minecraft has no module-info)
        if (node->nest_host_class) node->nest_host_class = remap_internal_name(*node->nest_host_class);
        for (std::string& member : node->nest_members) member = remap_internal_name(member);
        if (node->outer_class) {
            if (node->outer_method && node->outer_method_desc) {
                node->outer_method = hierarchised_method_renames_.opt_get(*node->outer_class, *node->outer_method_desc, *node->outer_method);
            }
            node->outer_class = remap_internal_name(*node->outer_class);
        }
        if (node->outer_method_desc) {
            std::string out;
            if (remap_signature(*node->outer_method_desc, out)) node->outer_method_desc = out;
        }
        for (std::string& member : node->permitted_subclasses) member = remap_internal_name(member);
        for (bc::RecordComponentNode& record : node->record_components) {
            std::string out;
            if (remap_signature(record.descriptor, out)) record.descriptor = out;
            remap_annotations(record.invisible_annotations);
            remap_annotations(record.invisible_type_annotations);
            remap_annotations(record.visible_annotations);
            remap_annotations(record.visible_type_annotations);
            if (record.signature) {
                out.clear();
                if (remap_signature(*record.signature, out)) record.signature = out;
            }
        }
        if (node->signature) {
            std::string out;
            // Class signatures are formatted differently than method or field signatures, but we can just ignore this
            // caveat here as the method will consider the invalid tokens are primitive objects.
            if (remap_signature(*node->signature, out)) node->signature = out;
        }
        if (node->super_name) node->super_name = remap_internal_name(*node->super_name);
        // remap the node's name if required
        if (const auto new_name = mapped_class(node->name)) node->name = *new_name;
    }
    storage_->update_cache();
}

void Remapper::remap_annotation(bc::AnnotationNode& annotation) {
    const std::string internal_name = annotation.desc.substr(1, annotation.desc.size() - 2);
    if (const auto n = mapped_class(internal_name)) annotation.desc = "L" + *n + ";";
    for (auto& [name, value] : annotation.values) remap_annotation_value(value);
}

template <class A>
void Remapper::remap_annotations(std::vector<A>& annotations) {
    for (bc::AnnotationNode& annotation : annotations) remap_annotation(annotation);
}

void Remapper::remap_annotation_value(bc::AnnotationValue& value) {
    using Kind = bc::AnnotationValue::Kind;
    switch (value.kind) {
        case Kind::CLASS: {
            if (!value.type) break;
            std::string out;
            if (remap_signature(value.type->get_descriptor(), out)) value.type = bc::Type::get_type(out);
            break;
        }
        case Kind::ENUM: {
            const std::string& desc = value.enum_value.desc;
            const std::string internal_name = desc.substr(1, desc.size() - 2);
            value.enum_value.value = hierarchised_field_renames_.opt_get(internal_name, desc, value.enum_value.value);
            if (const auto n = mapped_class(internal_name)) value.enum_value.desc = "L" + *n + ";";
            break;
        }
        case Kind::ANNOTATION:
            if (value.annotation) remap_annotation(*value.annotation);
            break;
        case Kind::ARRAY:
            for (bc::AnnotationValue& v : value.values) remap_annotation_value(v);
            break;
        default:
            break;   // Irrelevant
    }
}

void Remapper::remap_bsm_arg(std::vector<bc::Constant>& bsm_args, std::size_t index) {
    bc::Constant& bsm_arg = bsm_args[index];
    if (const auto* type = std::get_if<bc::Type>(&bsm_arg)) {
        if (type->get_sort() == bc::Type::METHOD) {
            std::string out;
            if (remap_signature(type->get_descriptor(), out)) bsm_arg = bc::Type::get_method_type(out);
        } else if (type->get_sort() == bc::Type::OBJECT) {
            const std::string old_val = type->get_internal_name();
            const std::string remapped = remap_internal_name(old_val);
            if (old_val != remapped) bsm_arg = bc::Type::get_object_type(remapped);
        } else {
            throw std::invalid_argument("Unexpected bsm arg Type sort. Sort = " + std::to_string(type->get_sort()) + "; type = " + type->get_descriptor());
        }
    } else if (const auto* handle = std::get_if<bc::Handle>(&bsm_arg)) {
        const std::string old_name = handle->name;
        std::string h_owner = handle->owner;
        const std::string new_name = hierarchised_method_renames_.opt_get(h_owner, handle->desc, old_name);
        const auto new_owner = mapped_class(h_owner);
        bool modified = old_name != new_name;
        if (new_owner) {
            h_owner = *new_owner;
            modified = true;
        }
        std::string desc = handle->desc;
        std::string out;
        if (remap_signature(desc, out)) {
            desc = out;
            modified = true;
        }
        if (modified) bsm_arg = bc::Handle{handle->tag, h_owner, new_name, desc, handle->is_interface};
    } else if (std::holds_alternative<std::string>(bsm_arg)) {
        // Do nothing.
    } else {
        throw std::invalid_argument("Unexpected bsm arg class at index " + std::to_string(index));
    }
}

void Remapper::remap_field(const std::string& owner, bc::FieldNode& field) {
    field.name = hierarchised_field_renames_.opt_get(owner, field.desc, field.name);
    const int type_type = field.desc[0];
    if (type_type == '[' || type_type == 'L') {
        // Remap descriptor
        field.desc = remap_single_desc(field.desc);
        // Remap signature
        if (field.signature) {
            std::string out;
            if (remap_signature(*field.signature, out)) field.signature = out;
        }
    }
}

std::string Remapper::remap_internal_name(const std::string& internal_name) {
    if (!internal_name.empty() && internal_name[0] == '[') return remap_single_desc(internal_name);
    if (const auto remapped = mapped_class(internal_name)) return *remapped;
    return internal_name;
}

std::optional<bc::Type> Remapper::get_param_type(const std::vector<bc::Type>& parameters, bool is_static, int lv_index) {
    int param_index = is_static ? 0 : 1;
    for (const bc::Type& p : parameters) {
        if (param_index == lv_index) return p;
        param_index += p.get_size();
    }
    return std::nullopt;
}

void Remapper::remap_method(bc::ClassNode& owner, bc::MethodNode& method) {
    const ParameterNames* lv_mappings_ptr = hierarchised_method_renames_.get_lv_mappings(owner.name, method.desc, method.name);
    const std::optional<ParameterNames> lv_mappings = lv_mappings_ptr != nullptr ? std::optional<ParameterNames>(*lv_mappings_ptr) : std::nullopt;
    method.name = hierarchised_method_renames_.opt_get(owner.name, method.desc, method.name);
    for (std::string& exception : method.exceptions) {
        if (const auto n = mapped_class(exception)) exception = *n;
    }
    remap_annotations(method.invisible_type_annotations);
    // (the local variable annotations are not modelled)
    remap_annotations(method.invisible_annotations);
    remap_annotations(method.visible_annotations);
    remap_annotations(method.visible_type_annotations);
    for (auto& annotations : method.invisible_parameter_annotations) remap_annotations(annotations);
    for (bc::LocalVariableNode& lvn : method.local_variables) {
        const int type_type = lvn.desc[0];
        bool is_object_array = type_type == '[';
        int array_dimension = 0;
        if (is_object_array) {
            if (lvn.desc.back() == ';') {
                // calculate depth
                int array_type;
                do {
                    array_type = lvn.desc.at(static_cast<std::size_t>(++array_dimension));
                } while (array_type == '[');
            } else {
                is_object_array = false;
            }
        }
        if (is_object_array || type_type == 'L') {
            // Remap descriptor
            const bc::Type type = bc::Type::get_type(lvn.desc);
            const std::string internal_name = type.get_internal_name();
            if (const auto new_internal_name = mapped_class(internal_name)) {
                if (is_object_array) lvn.desc = std::string(static_cast<std::size_t>(array_dimension), '[') + *new_internal_name + ";";
                else lvn.desc = "L" + *new_internal_name + ";";
            }
            if (lvn.signature) {
                std::string out;
                if (remap_signature(*lvn.signature, out)) lvn.signature = out;
            }
        }
    }
    for (bc::TryCatchBlockNode& catch_block : method.try_catch_blocks) {
        if (catch_block.type) {
            if (const auto n = mapped_class(*catch_block.type)) catch_block.type = *n;
        }
        // (a try-catch block's type annotations are not modelled)
    }
    {
        std::string out;
        if (remap_signature(method.desc, out)) {
            // The field signature and method desc system are similar enough that this works;
            method.desc = out;
        }
    }
    if (method.signature) {
        std::string out;
        if (remap_signature(*method.signature, out)) method.signature = out;
    }
    const bool is_static = (method.access & ACC_STATIC) != 0;
    const std::vector<bc::Type> param_types = bc::Type::get_argument_types(method.desc);
    method.parameters.clear();
    const auto lower_first = [](std::string s) {
        for (char& c : s) {
            if (static_cast<unsigned char>(c) < 0x80) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        return s;
    };
    if (lv_mappings) {
        for (std::size_t i = 0; i < lv_mappings->size(); ++i) {
            std::optional<std::string> name = (*lv_mappings)[i];
            const auto type = get_param_type(param_types, is_static, static_cast<int>(i));
            if (type) {
                if (!name) {
                    const std::string desc = (type->get_sort() == bc::Type::OBJECT || type->get_sort() == bc::Type::ARRAY) ? type->get_internal_name() : type->get_descriptor();
                    const std::size_t last_slash_plus_one = desc.rfind('/') == std::string::npos ? 0 : desc.rfind('/') + 1;
                    name = lower_first(desc.substr(last_slash_plus_one, 1)) + desc.substr(last_slash_plus_one + 1) + std::to_string(i);
                }
                method.parameters.push_back(bc::ParameterNode{name, 0});
            }
        }
    } else {
        const int size = bc::Type::get_arguments_and_return_sizes(method.desc) >> 2;
        for (int i = 0; i < size; ++i) {
            const auto type = get_param_type(param_types, is_static, i);
            if (type) {
                const std::string desc = (type->get_sort() == bc::Type::OBJECT || type->get_sort() == bc::Type::ARRAY) ? type->get_internal_name() : type->get_descriptor();
                const std::size_t last_slash_plus_one = desc.rfind('/') == std::string::npos ? 0 : desc.rfind('/') + 1;
                const std::string name = lower_first(desc.substr(last_slash_plus_one, 1)) + (desc.size() > 1 ? desc.substr(last_slash_plus_one + 1) : "") + std::to_string(i);
                method.parameters.push_back(bc::ParameterNode{name, 0});
            }
        }
    }
    // (RDI skips a Number default; remapping one changes nothing)
    if (method.annotation_default) remap_annotation_value(*method.annotation_default);
    for (bc::InsnNode* insn : method.instructions) {
        switch (insn->get_type()) {
            case bc::InsnNode::FIELD_INSN: {
                auto* instruction = static_cast<bc::FieldInsnNode*>(insn);
                if (const auto field_name = hierarchised_field_renames_.get(instruction->owner, instruction->desc, instruction->name)) instruction->name = *field_name;
                instruction->desc = remap_single_desc(instruction->desc);
                instruction->owner = remap_internal_name(instruction->owner);
                break;
            }
            case bc::InsnNode::INVOKE_DYNAMIC_INSN: {
                auto* specialised = static_cast<bc::InvokeDynamicInsnNode*>(insn);
                for (std::size_t i = 0; i < specialised->bsm_args.size(); ++i) remap_bsm_arg(specialised->bsm_args, i);
                std::string out;
                if (remap_signature(specialised->desc, out)) specialised->desc = out;
                break;
            }
            case bc::InsnNode::LDC_INSN: {
                auto* specialised = static_cast<bc::LdcInsnNode*>(insn);
                if (const auto* type = std::get_if<bc::Type>(&specialised->cst)) {
                    const std::string desc_string = type->get_descriptor();
                    const std::string new_desc_string = remap_single_desc(desc_string);
                    if (desc_string != new_desc_string) specialised->cst = bc::Type::get_type(new_desc_string);
                }
                break;
            }
            case bc::InsnNode::METHOD_INSN: {
                auto* instruction = static_cast<bc::MethodInsnNode*>(insn);
                const bool is_array = !instruction->owner.empty() && instruction->owner[0] == '[';
                if (!is_array) {   // Javac sometimes invokes methods on array objects
                    instruction->name = hierarchised_method_renames_.opt_get(instruction->owner, instruction->desc, instruction->name);
                    if (const auto new_owner = mapped_class(instruction->owner)) instruction->owner = *new_owner;
                } else {
                    instruction->owner = remap_single_desc(instruction->owner);
                }
                std::string out;
                if (remap_signature(instruction->desc, out)) instruction->desc = out;
                break;
            }
            case bc::InsnNode::MULTIANEWARRAY_INSN: {
                auto* instruction = static_cast<bc::MultiANewArrayInsnNode*>(insn);
                instruction->desc = remap_single_desc(instruction->desc);
                break;
            }
            case bc::InsnNode::TYPE_INSN: {
                auto* instruction = static_cast<bc::TypeInsnNode*>(insn);
                instruction->desc = remap_internal_name(instruction->desc);
                break;
            }
            default:
                break;   // (a frame is not modelled)
        }
    }
}

bool Remapper::remap_signature(const std::string& signature, std::string& out) {
    out.clear();
    return remap_signature(out, signature, 0, static_cast<int>(signature.size()));
}

bool Remapper::remap_signature(std::string& signature_out, const std::string& signature, int start, int end) {
    if (start == end) return false;
    const int type = static_cast<unsigned char>(signature.at(static_cast<std::size_t>(start++)));
    switch (type) {
        case 'T':
            // generics type parameter
            // fall-through intended as they are similar enough in format compared to objects
        case 'L': {
            // object
            // find the end of the internal name of the object
            int end_object = start;
            while (true) {
                if (end_object + 1 == end) return false;
                // this will skip a character, but this is not interesting as class names have to be at least 1 character long
                const char codepoint = signature.at(static_cast<std::size_t>(++end_object));
                if (codepoint == ';') {
                    std::string name = signature.substr(static_cast<std::size_t>(start), static_cast<std::size_t>(end_object - start));
                    bool modified = false;
                    if (const auto new_name = mapped_class(name)) {
                        name = *new_name;
                        modified = true;
                    }
                    signature_out += static_cast<char>(type);
                    signature_out += name;
                    signature_out += ';';
                    modified |= remap_signature(signature_out, signature, ++end_object, end);
                    return modified;
                } else if (codepoint == '<') {
                    // generics
                    int opening_brackets = 1;
                    int end_generics = end_object;
                    while (true) {
                        const char cp = signature.at(static_cast<std::size_t>(++end_generics));
                        if (cp == '>') {
                            if (--opening_brackets == 0) break;
                        } else if (cp == '<') {
                            opening_brackets++;
                        }
                    }
                    std::string name = signature.substr(static_cast<std::size_t>(start), static_cast<std::size_t>(end_object - start));
                    bool modified = false;
                    if (const auto new_name = mapped_class(name)) {
                        name = *new_name;
                        modified = true;
                    }
                    signature_out += 'L';
                    signature_out += name;
                    signature_out += '<';
                    {
                        const int inner_end = end_generics++;
                        modified |= remap_signature(signature_out, signature, end_object + 1, inner_end);
                    }
                    signature_out += '>';
                    // apparently that can be rarely be a '.'
                    signature_out += signature.at(static_cast<std::size_t>(end_generics));
                    modified |= remap_signature(signature_out, signature, ++end_generics, end);
                    return modified;
                }
            }
        }
        default:
            // '+', '*', '(', ')', '[' and the primitives: copied; the rest could still be modified
            signature_out += static_cast<char>(type);
            return remap_signature(signature_out, signature, start, end);
    }
}

std::string Remapper::remap_single_desc(const std::string& input) {
    const std::size_t index_of_l = input.find('L');
    if (index_of_l == std::string::npos) return input;
    const std::string internal_name = input.substr(index_of_l + 1, input.size() - 1 - (index_of_l + 1));
    const auto new_internal_name = mapped_class(internal_name);
    if (!new_internal_name) return input;
    return std::string(index_of_l, '[') + "L" + *new_internal_name + ";";
}

}  // namespace cb::java::minecraft::prep

// ---- CodeBreaker's own: what a version's mappings leave that its decompiled source cannot say --------------------------------------------------

namespace cb::java::minecraft::prep {

namespace {

// The classes of the storage above `name` (its superclass and interfaces, theirs, ...), nearest first; classes outside the storage are left out.
std::vector<const bc::ClassNode*> ancestors_of(const ClassStorage& storage, const bc::ClassNode& node) {
    std::vector<const bc::ClassNode*> out;
    std::unordered_set<std::string> seen;
    std::vector<std::string> next;
    if (node.super_name) next.push_back(*node.super_name);
    next.insert(next.end(), node.interfaces.begin(), node.interfaces.end());
    for (std::size_t i = 0; i < next.size(); ++i) {
        if (!seen.insert(next[i]).second) continue;
        const bc::ClassNode* up = storage.get_class(next[i]);
        if (up == nullptr) continue;
        out.push_back(up);
        if (up->super_name) next.push_back(*up->super_name);
        next.insert(next.end(), up->interfaces.begin(), up->interfaces.end());
    }
    return out;
}

bool renameable(const bc::MethodNode& m) { return m.name != "<init>" && m.name != "<clinit>" && (m.access & ACC_PRIVATE) == 0; }

// "(LWorld;III)Z" -> "(LWorld;III)" and "Z".
std::pair<std::string, std::string> split_desc(const std::string& desc) {
    const std::size_t close = desc.find(')');
    if (close == std::string::npos) return {desc, ""};
    return {desc.substr(0, close + 1), desc.substr(close + 1)};
}

// A word for a return type, for a name: "Z" -> "Z", "Ljava/lang/String;" -> "String", "[I" -> "IArray".
std::string return_word(std::string_view ret) {
    std::size_t dims = 0;
    while (dims < ret.size() && ret[dims] == '[') ++dims;
    std::string_view base = ret.substr(dims);
    std::string word;
    if (base.size() > 2 && base.front() == 'L') {
        base = base.substr(1, base.size() - 2);
        if (const std::size_t slash = base.rfind('/'); slash != std::string_view::npos) base = base.substr(slash + 1);
        for (const char c : base) word += std::isalnum(static_cast<unsigned char>(c)) ? c : '_';
    } else {
        word = std::string(base);
    }
    for (std::size_t i = 0; i < dims; ++i) word += "Array";
    return word;
}

}  // namespace

Mappings completed_for_source(const Mappings& given, const ClassStorage& storage) {
    Mappings out = given;
    // 1. a method the mappings name in a class, and not in the class or interface above that declares it, is named there too (and so in every
    //    class below that, by the remapper's own carrying down): IWorldAccess.a, which RenderGlobal implements as markBlockNeedsUpdate
    for (const bc::ClassNode* node : storage) {
        for (const bc::MethodNode& m : node->methods) {
            if (!renameable(m) || (m.access & ACC_STATIC) != 0) continue;
            const std::optional<std::string> named = given.methods.get(node->name, m.desc, m.name);
            if (!named) continue;
            for (const bc::ClassNode* up : ancestors_of(storage, *node)) {
                const bool declares = std::any_of(up->methods.begin(), up->methods.end(), [&](const bc::MethodNode& u) { return u.name == m.name && u.desc == m.desc && renameable(u); });
                if (declares && !out.methods.get(up->name, m.desc, m.name)) out.methods.put(up->name, m.desc, m.name, *named);
            }
        }
    }
    // 2. two methods of a class (its own, or its own and one above it) with one name and the same parameters, told apart only by what they return:
    //    the bytecode allows it, Java and C++ source do not. The one met later is named <name>_<return type> -- the same name in every class that
    //    declares it, so an override keeps overriding
    std::unordered_map<std::string, std::map<std::string, std::string>> done;   // class -> {final name + parameters: return type}
    std::function<const std::map<std::string, std::string>&(const bc::ClassNode&)> names_of = [&](const bc::ClassNode& node) -> const std::map<std::string, std::string>& {
        if (const auto it = done.find(node.name); it != done.end()) return it->second;
        std::map<std::string, std::string> seen;
        done[node.name] = {};   // (a cycle in a damaged hierarchy ends here)
        std::vector<std::string> parents;
        if (node.super_name) parents.push_back(*node.super_name);
        parents.insert(parents.end(), node.interfaces.begin(), node.interfaces.end());
        for (const std::string& p : parents)
            if (const bc::ClassNode* up = storage.get_class(p)) {
                const auto& inherited = names_of(*up);
                seen.insert(inherited.begin(), inherited.end());
            }
        for (const bc::MethodNode& m : node.methods) {
            if (!renameable(m)) continue;
            const auto [params, ret] = split_desc(m.desc);
            std::string name = out.methods.get(node.name, m.desc, m.name).value_or(m.name);
            const auto clash = seen.find(name + params);
            if (clash != seen.end() && clash->second != ret) {
                name += "_" + return_word(ret);
                out.methods.put(node.name, m.desc, m.name, name);
            }
            seen.emplace(name + params, ret);
        }
        return done[node.name] = std::move(seen);
    };
    for (const bc::ClassNode* node : storage) names_of(*node);
    return out;
}

}  // namespace cb::java::minecraft::prep
