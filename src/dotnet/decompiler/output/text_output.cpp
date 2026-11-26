// dotnet/decompiler/output/text_output.cpp -- PlainTextOutput and the rollback output's opcode references.
#include "dotnet/decompiler/output/text_output.hpp"

#include "dotnet/decompiler/disassembler/opcode_info.hpp"

namespace cb::dotnet::decompiler {

void PlainTextOutput::write_indent() {
    if (!needs_indent_) return;
    needs_indent_ = false;
    for (int i = 0; i < indent_; ++i) text_ += indentation_;
    column_ += indent_;
}

void PlainTextOutput::write(char ch) {
    write_indent();
    text_ += ch;
    ++column_;
}

void PlainTextOutput::write(std::string_view text) {
    write_indent();
    text_ += text;
    column_ += static_cast<int>(text.size());
}

void PlainTextOutput::write_line() {
    text_ += newline_;
    needs_indent_ = true;
    ++line_;
    column_ = 1;
}

void PlainTextOutput::write_reference(const disassembler::OpCodeInfo& op_code, bool omit_suffix) {
    if (omit_suffix) {
        const std::size_t last_dot = op_code.name.rfind('.');
        if (last_dot != std::string::npos && last_dot > 0) write(std::string_view(op_code.name).substr(0, last_dot + 1));
    } else {
        write(op_code.name);
    }
}

void TextOutputWithRollback::write_reference(const disassembler::OpCodeInfo& op_code, bool) {
    // (as upstream: the omitted suffix is not passed on)
    actions_.push_back([op_code](ITextOutput& t) { t.write_reference(op_code); });
}

}  // namespace cb::dotnet::decompiler
