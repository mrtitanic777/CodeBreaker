// dotnet/decompiler/output/text_output.hpp -- where the decompiler and disassembler write their text: an output with indentation, and
// references (to metadata, types, members, locals) and folds that a viewer may make clickable; a plain text output that keeps only the text.
//
//     PlainTextOutput out;
//     out.write(".class "); out.indent(); out.write_line(); ...
//     out.text();                              // what was written (lines end in "\r\n", as .NET's TextWriter on Windows writes them)
//
// Ported from ILSpy (ICSharpCode.Decompiler: ITextOutput, TextOutputExtensions, PlainTextOutput, TextOutputWithRollback), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Text is UTF-8. A local reference's identity (upstream's
// object) is a std::any.
#pragma once

#include <any>
#include <format>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/metadata/handles.hpp"

namespace cb::dotnet::decompiler {

namespace metadata {
class MetadataFile;
}
namespace typesystem {
class IType;
class IMember;
}
namespace disassembler {
struct OpCodeInfo;
}

class ITextOutput {
public:
    virtual ~ITextOutput() = default;
    virtual const std::string& indentation_string() const = 0;
    virtual void set_indentation_string(std::string value) = 0;
    virtual void indent() = 0;
    virtual void unindent() = 0;
    virtual void write(char ch) = 0;
    virtual void write(std::string_view text) = 0;
    virtual void write_line() = 0;
    virtual void write_reference(const disassembler::OpCodeInfo& op_code, bool omit_suffix = false) = 0;
    virtual void write_reference(const metadata::MetadataFile& module, dotnet::metadata::Handle handle, std::string_view text,
                                 std::string_view protocol = "decompile", bool is_definition = false) = 0;
    virtual void write_reference(const typesystem::IType& type, std::string_view text, bool is_definition = false) = 0;
    virtual void write_reference(const typesystem::IMember& member, std::string_view text, bool is_definition = false) = 0;
    virtual void write_local_reference(std::string_view text, std::any reference, bool is_definition = false, bool is_hover_only = false) = 0;
    virtual void mark_fold_start(std::string_view collapsed_text = "...", bool default_collapsed = false, bool is_definition = false) = 0;
    // Where an entity's declaration begins (the next fold marked is_definition reaches back to here).
    virtual void mark_definition_start() = 0;
    virtual void mark_fold_end() = 0;

    // TextOutputExtensions
    void write_line(std::string_view text) {
        write(text);
        write_line();
    }
    template <class... Args>
    void write_format(std::format_string<Args...> fmt, Args&&... args) {
        write(std::string_view(std::format(fmt, std::forward<Args>(args)...)));
    }
    template <class... Args>
    void write_line_format(std::format_string<Args...> fmt, Args&&... args) {
        write_line(std::string_view(std::format(fmt, std::forward<Args>(args)...)));
    }
};

class PlainTextOutput final : public ITextOutput {
public:
    using ITextOutput::write_line;
    explicit PlainTextOutput(std::string newline = "\r\n") : newline_(std::move(newline)) {}

    const std::string& text() const { return text_; }
    const std::string& indentation_string() const override { return indentation_; }
    void set_indentation_string(std::string value) override { indentation_ = std::move(value); }
    // The line and column the next character goes to (1-based; the column counts UTF-8 bytes, upstream UTF-16 units).
    int line() const { return line_; }
    int column() const { return column_ + (needs_indent_ ? indent_ : 0); }

    void indent() override { ++indent_; }
    void unindent() override { --indent_; }
    void write(char ch) override;
    void write(std::string_view text) override;
    void write_line() override;
    void write_reference(const disassembler::OpCodeInfo& op_code, bool omit_suffix = false) override;
    void write_reference(const metadata::MetadataFile&, dotnet::metadata::Handle, std::string_view text, std::string_view = "decompile",
                         bool = false) override {
        write(text);
    }
    void write_reference(const typesystem::IType&, std::string_view text, bool = false) override { write(text); }
    void write_reference(const typesystem::IMember&, std::string_view text, bool = false) override { write(text); }
    void write_local_reference(std::string_view text, std::any, bool = false, bool = false) override { write(text); }
    void mark_fold_start(std::string_view = "...", bool = false, bool = false) override {}
    void mark_definition_start() override {}
    void mark_fold_end() override {}

private:
    void write_indent();

    std::string text_;
    std::string newline_;
    std::string indentation_ = "\t";
    int indent_ = 0;
    bool needs_indent_ = false;
    int line_ = 1, column_ = 1;
};

// An output that records what is written and passes it on only on commit() (what is written before an error is dropped).
class TextOutputWithRollback final : public ITextOutput {
public:
    using ITextOutput::write_line;
    explicit TextOutputWithRollback(ITextOutput& target) : target_(target) {}
    void commit() {
        for (const auto& a : actions_) a(target_);
    }

    const std::string& indentation_string() const override { return target_.indentation_string(); }
    void set_indentation_string(std::string value) override { target_.set_indentation_string(std::move(value)); }
    void indent() override { actions_.push_back([](ITextOutput& t) { t.indent(); }); }
    void unindent() override { actions_.push_back([](ITextOutput& t) { t.unindent(); }); }
    void write(char ch) override { actions_.push_back([ch](ITextOutput& t) { t.write(ch); }); }
    void write(std::string_view text) override { actions_.push_back([s = std::string(text)](ITextOutput& t) { t.write(s); }); }
    void write_line() override { actions_.push_back([](ITextOutput& t) { t.write_line(); }); }
    void write_reference(const disassembler::OpCodeInfo& op_code, bool omit_suffix = false) override;
    void write_reference(const metadata::MetadataFile& module, dotnet::metadata::Handle handle, std::string_view text, std::string_view protocol = "decompile",
                         bool is_definition = false) override {
        actions_.push_back([&module, handle, s = std::string(text), p = std::string(protocol), is_definition](ITextOutput& t) {
            t.write_reference(module, handle, s, p, is_definition);
        });
    }
    void write_reference(const typesystem::IType& type, std::string_view text, bool is_definition = false) override {
        actions_.push_back([&type, s = std::string(text), is_definition](ITextOutput& t) { t.write_reference(type, s, is_definition); });
    }
    void write_reference(const typesystem::IMember& member, std::string_view text, bool is_definition = false) override {
        actions_.push_back([&member, s = std::string(text), is_definition](ITextOutput& t) { t.write_reference(member, s, is_definition); });
    }
    void write_local_reference(std::string_view text, std::any reference, bool is_definition = false, bool = false) override {
        // (as upstream: the hover-only flag is not passed on)
        actions_.push_back([s = std::string(text), reference, is_definition](ITextOutput& t) { t.write_local_reference(s, reference, is_definition); });
    }
    void mark_fold_start(std::string_view collapsed_text = "...", bool default_collapsed = false, bool is_definition = false) override {
        actions_.push_back([s = std::string(collapsed_text), default_collapsed, is_definition](ITextOutput& t) {
            t.mark_fold_start(s, default_collapsed, is_definition);
        });
    }
    void mark_definition_start() override { actions_.push_back([](ITextOutput& t) { t.mark_definition_start(); }); }
    void mark_fold_end() override { actions_.push_back([](ITextOutput& t) { t.mark_fold_end(); }); }

private:
    ITextOutput& target_;
    std::vector<std::function<void(ITextOutput&)>> actions_;
};

}  // namespace cb::dotnet::decompiler
