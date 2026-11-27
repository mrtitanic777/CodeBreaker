// dotnet/decompiler/csharp/output_visitor/text_writer.hpp -- the text sink the C# token writers write to (what .NET's TextWriter is for
// upstream): a TextWriter takes text and line breaks; a StringTextWriter collects them in a string, a TextOutputWriter passes them on to
// an ITextOutput (so the decompiler's output keeps its folds and references).
//
//     StringTextWriter writer;
//     auto tokens = TokenWriter::create(writer);
//     writer.text();      // what was written
//
// A line ends in new_line(), "\r\n" as .NET's Environment.NewLine on Windows (the text ILSpy's output is checked against); a TextWriter's
// text is UTF-8.
//
// Ported from ILSpy (System.IO.TextWriter as the token writers use it; ICSharpCode.Decompiler: TextOutputWriter), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "dotnet/decompiler/output/text_output.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

class TextWriter {
public:
    virtual ~TextWriter() = default;
    virtual void write(char ch) = 0;
    virtual void write(std::string_view text) = 0;
    // A line break: new_line().
    virtual void write_line() = 0;
    void write_line(std::string_view text) {
        write(text);
        write_line();
    }
    // The line break this writer writes (as .NET's TextWriter.NewLine); its length counts toward a token writer's length.
    virtual const std::string& new_line() const = 0;
};

class StringTextWriter final : public TextWriter {
public:
    using TextWriter::write_line;
    explicit StringTextWriter(std::string newline = "\r\n") : newline_(std::move(newline)) {}
    void write(char ch) override { text_.push_back(ch); }
    void write(std::string_view text) override { text_.append(text); }
    void write_line() override { text_.append(newline_); }
    const std::string& new_line() const override { return newline_; }
    const std::string& text() const { return text_; }
    std::string take_text() { return std::move(text_); }

private:
    std::string text_;
    std::string newline_;
};

// Writes into an ITextOutput; the line break is the output's own.
class TextOutputWriter final : public TextWriter {
public:
    using TextWriter::write_line;
    explicit TextOutputWriter(ITextOutput& output) : output_(output) {}
    void write(char ch) override { output_.write(ch); }
    void write(std::string_view text) override { output_.write(text); }
    void write_line() override { output_.write_line(); }
    const std::string& new_line() const override { return new_line_; }

private:
    ITextOutput& output_;
    std::string new_line_ = "\r\n";
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
