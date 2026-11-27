// dotnet/decompiler/util/resx_resource_writer.hpp -- a .resx file written: the XML a WinForms or WPF project holds its resources in, with each
// resource as a `data` element (a string as text, a number as text with its type, bytes and streams as base64).
//
//     ResXResourceWriter writer("Form1.resx");                 // the file is made now; the XML is written from the first resource on
//     for (auto& [name, value] : ResourcesFile(bytes).entries()) writer.add_resource(name, value);
//     writer.close();                                           // (or the destructor)
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: ResXResourceWriter, from ResXResourceWriter.cs, which is Mono's), Copyright
// AlphaSierraPapa for the ILSpy team, MIT; Mono's part, Copyright (c) 2004-2005 Novell, Inc. under the same terms (see NOTICE in
// dotnet/decompiler). Upstream's three constructors (a stream, a TextWriter, a file name) are two here: a file name, and a TextWriter. The file
// of the file name constructor is the stream a caller made (FileMode.Create): it exists, empty, from the constructor on, and is written as UTF-8
// with a byte order mark when the first resource comes (StreamWriter(stream, Encoding.UTF8) writes one). The schema text has "\r\n" line
// breaks, as in ILSpy's released DLL; the XML is by util/xml_text_writer.hpp, indented, ending where the writer is closed.
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "dotnet/decompiler/util/resources_file.hpp"
#include "dotnet/decompiler/util/text_writer.hpp"
#include "dotnet/decompiler/util/xml_text_writer.hpp"

namespace cb::dotnet::decompiler::util {

class ResXResourceWriter {
public:
    static constexpr const char* kBinSerializedObjectMimeType = "application/x-microsoft.net.object.binary.base64";
    static constexpr const char* kByteArraySerializedObjectMimeType = "application/x-microsoft.net.object.bytearray.base64";
    static constexpr const char* kDefaultSerializedObjectMimeType = kBinSerializedObjectMimeType;
    static constexpr const char* kResMimeType = "text/microsoft-resx";
    static constexpr const char* kSoapSerializedObjectMimeType = "application/x-microsoft.net.object.soap.base64";
    static constexpr const char* kVersion = "2.0";

    // Writes to the file (created, or truncated, here). IoError when it cannot be.
    explicit ResXResourceWriter(const std::filesystem::path& file_name);
    // Writes to the writer, which must stay alive as long as this one; close() closes it.
    explicit ResXResourceWriter(TextWriter& text_writer);
    ResXResourceWriter(const ResXResourceWriter&) = delete;
    ResXResourceWriter& operator=(const ResXResourceWriter&) = delete;
    // Dispose: closes (ends the XML) and does not throw.
    ~ResXResourceWriter();

    // UsageError (InvalidOperationException upstream) after the XML was ended.
    void add_resource(const std::string& name, const ResourceValue& value, const std::string& comment = {});
    void add_resource(const std::string& name, const Bytes& value) { add_resource(name, ResourceValue(value)); }
    void add_resource(const std::string& name, const std::string& value) { add_resource(name, ResourceValue(value)); }

    // Ends the XML (when anything was written) and closes the file.
    void close();
    // Ends the root element; UsageError when that was done.
    void generate();

    const std::string& base_path() const { return base_path_; }
    void set_base_path(std::string value) { base_path_ = std::move(value); }

private:
    void init_writer();
    void write_header(const std::string& name, const std::string& value);
    void write_nice_base64(const Bytes& value);
    void write_bytes(const std::string& name, const std::optional<std::string>& type, const Bytes& value, const std::string& comment);
    void write_string(const std::string& name, const std::string& value, const std::optional<std::string>& type, const std::string& comment);

    std::optional<std::filesystem::path> file_name_;
    TextWriter* text_writer_ = nullptr;
    std::unique_ptr<TextWriter> owned_text_writer_;
    std::unique_ptr<XmlTextWriter> writer_;
    bool written_ = false;
    std::string base_path_;
};

}  // namespace cb::dotnet::decompiler::util
