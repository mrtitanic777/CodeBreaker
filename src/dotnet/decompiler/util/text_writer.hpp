// dotnet/decompiler/util/text_writer.hpp -- where text goes (System.IO.TextWriter as ILSpy uses it): a writer that takes UTF-8 text, knows its
// encoding's name (what an XML declaration names) and its line ending, and is either a string (StringWriter) or a file (StreamWriter).
//
//     StringWriter w;  w.write("a");  w.write_line("b");  w.str() == "ab\r\n";
//     StreamWriter f(path);  f.write_line("text");   // UTF-8 without a byte order mark, as StreamWriter(path) writes; closed (flushed) by close() or the destructor
//
// Ported from ILSpy's use of System.IO.TextWriter, StringWriter and StreamWriter (WholeProjectDecompiler.CreateFile, the project file
// writers, ResXResourceWriter), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Text is UTF-8; the line
// ending is "\r\n", as .NET's TextWriter writes it on Windows, which every writer of this port follows.
#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

namespace cb::dotnet::decompiler::util {

class TextWriter {
public:
    virtual ~TextWriter() = default;
    // The text, as UTF-8.
    virtual void write(std::string_view text) = 0;
    void write_char(char c) { write(std::string_view(&c, 1)); }
    void write_line() { write("\r\n"); }
    void write_line(std::string_view text) {
        write(text);
        write_line();
    }
    virtual void flush() {}
    // Flushes and closes; writing after it is a UsageError. Closing twice is fine (Dispose).
    virtual void close() { flush(); }
    // Encoding.WebName of the writer's encoding ("utf-8", "utf-16"): what an XML declaration says.
    virtual std::string encoding_web_name() const = 0;
};

// A writer into a string. Its encoding is UTF-16 (a StringWriter's), so an XML declaration written to it says "utf-16".
class StringWriter final : public TextWriter {
public:
    void write(std::string_view text) override { text_.append(text); }
    std::string encoding_web_name() const override { return "utf-16"; }
    const std::string& str() const { return text_; }

private:
    std::string text_;
};

// A writer into a file it creates (or truncates): UTF-8 without a byte order mark, or with one (utf8_bom: Encoding.UTF8, which writes the
// three bytes first). IoError (core/error.hpp) when the file cannot be created or written.
class StreamWriter final : public TextWriter {
public:
    explicit StreamWriter(const std::filesystem::path& path, bool utf8_bom = false);
    StreamWriter(const StreamWriter&) = delete;
    StreamWriter& operator=(const StreamWriter&) = delete;
    ~StreamWriter() override;
    void write(std::string_view text) override;
    void flush() override;
    // Flushes; an error here (a full disk) is thrown, as Dispose throws it upstream.
    void close() override;
    std::string encoding_web_name() const override { return "utf-8"; }

private:
    std::filesystem::path path_;
    std::FILE* file_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::util
