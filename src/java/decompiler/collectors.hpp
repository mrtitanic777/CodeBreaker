// java/decompiler/collectors.hpp -- what a class's decompilation gathers as it goes: the import list (each class name shortened, and the imports a file
// needs worked out), the variable names a method has used, which bytecode offset each written line came from (per method, and per class with the
// original's line numbers), and the small helpers the passes share (InterpreterUtil's comparisons, TextUtil's `Outer.super`).
//
//     ImportCollector imports(root_node);
//     imports.get_short_name("java.util.List");          // "List", and "java.util.List" is now an import
//     imports.write_imports(buffer);                      // "import java.util.List;" lines, sorted by package then name
//
// Ported from Fernflower (de.fernflower.main.collectors: ImportCollector, VarNamesCollector, BytecodeMappingTracer, BytecodeSourceMapper;
// util.InterpreterUtil's comparisons, util.TextUtil), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// CounterContainer is in context.hpp, InterpreterUtil's text helpers are in text_buffer.hpp (indent_string, char_to_unicode_literal,
// is_printable_unicode) and its file helpers belong to the driver (decompiler.hpp). The original's HashMap<Integer, Integer> mappings are ordered
// maps here: nothing reads them in their hash order (they are sorted when written, and the line numbers go into a map).
#pragma once

#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "java/decompiler/context.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/decompiler/keyed_list.hpp"
#include "java/decompiler/text_buffer.hpp"

namespace cb::java::decompiler {

class LineNumberTableAttribute;

// Which bytecode offset each source line a method was written as came from (the original's BytecodeMappingTracer): the passes tell it the line they
// are writing (increment_current_source_line) and the offsets of what they write (add_mapping).
class BytecodeMappingTracer {
public:
    BytecodeMappingTracer() = default;
    explicit BytecodeMappingTracer(int initial_source_line) : current_source_line_(initial_source_line) {}

    void increment_current_source_line() { ++current_source_line_; }
    void increment_current_source_line(int number_lines) { current_source_line_ += number_lines; }
    void shift_source_lines(int shift);
    // The first line an offset was written on is the one kept.
    void add_mapping(int bytecode_offset);
    void add_mapping(const std::optional<std::set<int>>& bytecode_offsets);   // none: nothing
    void add_tracer(const BytecodeMappingTracer* tracer);                     // null: nothing

    const std::map<int, int>& get_mapping() const { return mapping_; }       // bytecode offset -> source line
    int get_current_source_line() const { return current_source_line_; }
    void set_current_source_line(int line) { current_source_line_ = line; }
    void set_line_number_table(const LineNumberTableAttribute* table) { line_number_table_ = table; }
    // The original source line -> the line written, through the method's LineNumberTable (empty without one).
    std::map<int, int> get_original_lines_mapping() const;

private:
    int current_source_line_ = 0;
    const LineNumberTableAttribute* line_number_table_ = nullptr;
    std::map<int, int> mapping_;
};

// The mappings of every method of a class: class -> method ("name descriptor") -> bytecode offset -> source line, in the order they were added.
class BytecodeSourceMapper {
public:
    // An offset already mapped keeps its line.
    void add_mapping(const std::string& class_name, const std::string& method_name, int bytecode_offset, int source_line);
    void add_tracer(const std::string& class_name, const std::string& method_name, const BytecodeMappingTracer& tracer);
    // The `class '...' { method '...' { offset line } }` listing and the "Lines mapping:" table (the original's unit-test dump).
    void dump_mapping(TextBuffer& buffer, bool offsets_to_hex) const;
    int get_total_offset() const { return offset_total_; }
    void set_total_offset(int offset_total) { offset_total_ = offset_total; }
    void add_total_offset(int offset_total) { offset_total_ += offset_total; }
    // Original line to decompiled line, 1-based: original, decompiled, original, decompiled ...
    std::vector<int> get_original_lines_mapping() const;

private:
    int offset_total_ = 0;
    LinkedMap<std::string, LinkedMap<std::string, std::map<int, int>>> mapping_;
    std::map<int, int> lines_mapping_;   // original line -> decompiled line
};

// The imports of one class file and the short names it uses: a class name is written short when no other class of that short name has been
// written (or is in the current package, or in the default package); else in full.
class ImportCollector {
public:
    static constexpr std::string_view JAVA_LANG_PACKAGE = "java.lang";

    explicit ImportCollector(const ClassNode* root);   // the file's root class: its package is the current one
    // `fullname` is dotted or slashed; an own nested class comes back as Outer.Inner.
    std::string get_short_name(std::string fullname);
    // `imported` false: the name is only reserved (the class being written and its nested ones, which need no import).
    std::string get_short_name(std::string fullname, bool imported);
    // "import a.b.C;" lines (the packages sorted, then the names); returns how many.
    int write_imports(TextBuffer& buffer);

private:
    std::vector<std::string> pack_imports() const;

    std::map<std::string, std::string> map_simple_names_;   // short name -> package
    std::set<std::string> set_not_imported_names_;
    std::string current_package_slash_, current_package_point_;
};

// The names the local variables of a method took, and the next free one.
class VarNamesCollector {
public:
    VarNamesCollector() = default;
    explicit VarNamesCollector(const std::set<std::string>& names) : used_names_(names) {}
    void add_name(const std::string& value) { used_names_.insert(value); }
    // "var<index>" or the first of it with "x" appended until free; the name is taken.
    std::string get_free_name(int index);
    std::string get_free_name(std::string proposition);

private:
    std::set<std::string> used_names_;
};

// The original's util.InterpreterUtil comparisons (null-aware equality) and its unique member key.
class InterpreterUtil {
public:
    static constexpr bool IS_WINDOWS =
#ifdef _WIN32
        true;
#else
        false;
#endif
    // "name descriptor" (keyed_list.hpp's unique_key).
    static std::string make_unique_key(const std::string& name, const std::string& descriptor) { return unique_key(name, descriptor); }
    // The same members, any order (the original's equalSets: sizes equal and no member of one missing from the other).
    template <class C1, class C2>
    static bool equal_sets(const C1& c1, const C2& c2) {
        if (c1.size() != c2.size()) return false;
        for (const auto& x : c1) {
            bool found = false;
            for (const auto& y : c2) found = found || x == y;
            if (!found) return false;
        }
        return true;
    }
    // The same elements in the same order (compared with ==; lists of expressions use equal_exprent_lists).
    template <class T>
    static bool equal_lists(const std::vector<T>& first, const std::vector<T>& second) {
        return first == second;
    }
    template <class T>
    static bool equal_objects(const std::optional<T>& first, const std::optional<T>& second) {
        return first == second;
    }
};

// `Outer.super` as an explicit qualified super reference.
class TextUtil {
public:
    // Writes "<short name of qualifier>.super", or "super" when `qualifier` is the class being written (DecompilerContext::current_class_node).
    static void write_qualified_super(TextBuffer& buf, const std::string& qualifier);
};

}  // namespace cb::java::decompiler
