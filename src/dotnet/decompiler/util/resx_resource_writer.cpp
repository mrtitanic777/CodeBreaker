#include "dotnet/decompiler/util/resx_resource_writer.hpp"

#include <format>

#include "dotnet/decompiler/util/number_format.hpp"

namespace cb::dotnet::decompiler::util {

namespace {

const std::string kWinFormsAssemblyName = ", System.Windows.Forms, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089";
const std::string kMSCorLibAssemblyName = ", mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089";
const std::string kResXNullRefTypeName = "System.Resources.ResXNullRef" + kWinFormsAssemblyName;

// The schema as the writer emits it: each tab of the source text is two spaces, each ' is a ", lines end in "\r\n".
std::string make_schema() {
    static const char* const kLines[] = {
        "",
        "  <xsd:schema id=\"root\" xmlns=\"\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:msdata=\"urn:schemas-microsoft-com:xml-msdata\">",
        "    <xsd:element name=\"root\" msdata:IsDataSet=\"true\">",
        "      <xsd:complexType>",
        "        <xsd:choice maxOccurs=\"unbounded\">",
        "          <xsd:element name=\"data\">",
        "            <xsd:complexType>",
        "              <xsd:sequence>",
        "                <xsd:element name=\"value\" type=\"xsd:string\" minOccurs=\"0\" msdata:Ordinal=\"1\" />",
        "                <xsd:element name=\"comment\" type=\"xsd:string\" minOccurs=\"0\" msdata:Ordinal=\"2\" />",
        "              </xsd:sequence>",
        "              <xsd:attribute name=\"name\" type=\"xsd:string\" msdata:Ordinal=\"1\" />",
        "              <xsd:attribute name=\"type\" type=\"xsd:string\" msdata:Ordinal=\"3\" />",
        "              <xsd:attribute name=\"mimetype\" type=\"xsd:string\" msdata:Ordinal=\"4\" />",
        "            </xsd:complexType>",
        "          </xsd:element>",
        "          <xsd:element name=\"resheader\">",
        "            <xsd:complexType>",
        "              <xsd:sequence>",
        "                <xsd:element name=\"value\" type=\"xsd:string\" minOccurs=\"0\" msdata:Ordinal=\"1\" />",
        "              </xsd:sequence>",
        "              <xsd:attribute name=\"name\" type=\"xsd:string\" use=\"required\" />",
        "            </xsd:complexType>",
        "          </xsd:element>",
        "        </xsd:choice>",
        "      </xsd:complexType>",
        "    </xsd:element>",
        "  </xsd:schema>",
        "",
    };
    std::string schema;
    bool first = true;
    for (const char* line : kLines) {
        if (!first) schema += "\r\n";
        first = false;
        schema += line;
    }
    return schema;
}

// Convert.ToBase64String(bytes, Base64FormattingOptions.InsertLineBreaks): a "\r\n" after every 76 characters.
std::string to_base64_with_line_breaks(const Bytes& bytes) {
    static const char* const kAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    int line_length = 0;
    auto put = [&](char c) {
        if (line_length == 76) {
            out += "\r\n";
            line_length = 0;
        }
        out.push_back(c);
        ++line_length;
    };
    std::size_t i = 0;
    for (; i + 2 < bytes.size(); i += 3) {
        const std::uint32_t v = (static_cast<std::uint32_t>(bytes[i]) << 16) | (static_cast<std::uint32_t>(bytes[i + 1]) << 8) | bytes[i + 2];
        put(kAlphabet[(v >> 18) & 63]);
        put(kAlphabet[(v >> 12) & 63]);
        put(kAlphabet[(v >> 6) & 63]);
        put(kAlphabet[v & 63]);
    }
    if (i + 1 == bytes.size()) {
        const std::uint32_t v = static_cast<std::uint32_t>(bytes[i]) << 16;
        put(kAlphabet[(v >> 18) & 63]);
        put(kAlphabet[(v >> 12) & 63]);
        put('=');
        put('=');
    } else if (i + 2 == bytes.size()) {
        const std::uint32_t v = (static_cast<std::uint32_t>(bytes[i]) << 16) | (static_cast<std::uint32_t>(bytes[i + 1]) << 8);
        put(kAlphabet[(v >> 18) & 63]);
        put(kAlphabet[(v >> 12) & 63]);
        put(kAlphabet[(v >> 6) & 63]);
        put('=');
    }
    return out;
}

// A char16_t as the text char.ToString() gives (UTF-8; a lone surrogate is U+FFFD, as a UTF-8 writer encodes it).
std::string char_to_string(char16_t c) {
    std::uint32_t cp = c;
    if (cp >= 0xD800 && cp < 0xE000) cp = 0xFFFD;
    std::string out;
    if (cp < 0x80) {
        out.push_back(static_cast<char>(cp));
    } else if (cp < 0x800) {
        out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
    return out;
}

}  // namespace

ResXResourceWriter::ResXResourceWriter(const std::filesystem::path& file_name) : file_name_(file_name) {
    // the stream the caller made: the file exists (empty) from now on
    StreamWriter created(file_name);
    created.close();
}

ResXResourceWriter::ResXResourceWriter(TextWriter& text_writer) : text_writer_(&text_writer) {}

ResXResourceWriter::~ResXResourceWriter() {
    try {
        close();
    } catch (...) {
    }
}

void ResXResourceWriter::init_writer() {
    if (file_name_) {
        owned_text_writer_ = std::make_unique<StreamWriter>(*file_name_, /*utf8_bom=*/true);
        text_writer_ = owned_text_writer_.get();
    }
    writer_ = std::make_unique<XmlTextWriter>(*text_writer_);
    writer_->set_formatting(XmlFormatting::Indented);
    writer_->write_start_document();
    writer_->write_start_element("root");
    static const std::string schema = make_schema();
    writer_->write_raw(schema);
    write_header("resmimetype", "text/microsoft-resx");
    write_header("version", "1.3");
    write_header("reader", "System.Resources.ResXResourceReader" + kWinFormsAssemblyName);
    write_header("writer", "System.Resources.ResXResourceWriter" + kWinFormsAssemblyName);
}

void ResXResourceWriter::write_header(const std::string& name, const std::string& value) {
    writer_->write_start_element("resheader");
    writer_->write_attribute_string("name", name);
    writer_->write_start_element("value");
    writer_->write_string(value);
    writer_->write_end_element();
    writer_->write_end_element();
}

void ResXResourceWriter::write_nice_base64(const Bytes& value) { writer_->write_string(to_base64_with_line_breaks(value)); }

void ResXResourceWriter::write_bytes(const std::string& name, const std::optional<std::string>& type, const Bytes& value, const std::string& comment) {
    writer_->write_start_element("data");
    writer_->write_attribute_string("name", name);

    if (type) {
        writer_->write_attribute_string("type", *type);
        // byte[] should never get a mimetype, otherwise MS.NET won't be able to parse the data.
        if (*type != "System.Byte[]" + kMSCorLibAssemblyName) writer_->write_attribute_string("mimetype", kByteArraySerializedObjectMimeType);
        writer_->write_start_element("value");
        write_nice_base64(value);
    } else {
        writer_->write_attribute_string("mimetype", kBinSerializedObjectMimeType);
        writer_->write_start_element("value");
        write_nice_base64(value);
    }

    writer_->write_end_element();

    if (!comment.empty()) {
        writer_->write_start_element("comment");
        writer_->write_string(comment);
        writer_->write_end_element();
    }

    writer_->write_end_element();
}

void ResXResourceWriter::write_string(const std::string& name, const std::string& value, const std::optional<std::string>& type, const std::string& comment) {
    writer_->write_start_element("data");
    writer_->write_attribute_string("name", name);
    if (type) {
        writer_->write_attribute_string("type", *type);
    } else {
        writer_->write_attribute_string("xml:space", "preserve");
    }
    writer_->write_start_element("value");
    writer_->write_string(value);
    writer_->write_end_element();
    if (!comment.empty()) {
        writer_->write_start_element("comment");
        writer_->write_string(comment);
        writer_->write_end_element();
    }
    writer_->write_end_element();
    writer_->write_whitespace("\n  ");
}

void ResXResourceWriter::add_resource(const std::string& name, const ResourceValue& value, const std::string& comment) {
    if (written_) throw UsageError("The resource is already generated.");

    if (!writer_) init_writer();

    struct Visitor {
        ResXResourceWriter& self;
        const std::string& name;
        const std::string& comment;
        void operator()(std::monostate) const {
            // nulls written as ResXNullRef
            self.write_string(name, "", kResXNullRefTypeName, comment);
        }
        void operator()(const std::string& s) const { self.write_string(name, s, std::nullopt, comment); }
        void operator()(bool b) const { self.write_string(name, b ? "True" : "False", "System.Boolean" + kMSCorLibAssemblyName, comment); }
        void operator()(char16_t c) const { self.write_string(name, char_to_string(c), "System.Char" + kMSCorLibAssemblyName, comment); }
        void operator()(std::int8_t v) const { self.write_string(name, std::to_string(v), "System.SByte" + kMSCorLibAssemblyName, comment); }
        void operator()(std::uint8_t v) const { self.write_string(name, std::to_string(v), "System.Byte" + kMSCorLibAssemblyName, comment); }
        void operator()(std::int16_t v) const { self.write_string(name, std::to_string(v), "System.Int16" + kMSCorLibAssemblyName, comment); }
        void operator()(std::uint16_t v) const { self.write_string(name, std::to_string(v), "System.UInt16" + kMSCorLibAssemblyName, comment); }
        void operator()(std::int32_t v) const { self.write_string(name, std::to_string(v), "System.Int32" + kMSCorLibAssemblyName, comment); }
        void operator()(std::uint32_t v) const { self.write_string(name, std::to_string(v), "System.UInt32" + kMSCorLibAssemblyName, comment); }
        void operator()(std::int64_t v) const { self.write_string(name, std::to_string(v), "System.Int64" + kMSCorLibAssemblyName, comment); }
        void operator()(std::uint64_t v) const { self.write_string(name, std::to_string(v), "System.UInt64" + kMSCorLibAssemblyName, comment); }
        void operator()(float v) const { self.write_string(name, format_r(v), "System.Single" + kMSCorLibAssemblyName, comment); }
        void operator()(double v) const { self.write_string(name, format_r(v), "System.Double" + kMSCorLibAssemblyName, comment); }
        void operator()(const Decimal& v) const { self.write_string(name, v.to_string(), "System.Decimal" + kMSCorLibAssemblyName, comment); }
        void operator()(const DateTime& v) const { self.write_string(name, v.to_string(), "System.DateTime" + kMSCorLibAssemblyName, comment); }
        void operator()(const TimeSpan& v) const { self.write_string(name, v.to_string(), "System.TimeSpan" + kMSCorLibAssemblyName, comment); }
        void operator()(const Bytes& array) const { self.write_bytes(name, "System.Byte[]" + kMSCorLibAssemblyName, array, comment); }
        void operator()(const ResourceStream& memory_stream) const { self.write_bytes(name, std::nullopt, memory_stream.bytes, comment); }
        void operator()(const ResourceSerializedObject& rso) const { self.write_bytes(name, rso.type_name(), rso.get_bytes(), comment); }
    };
    std::visit(Visitor{*this, name, comment}, value);
}

void ResXResourceWriter::close() {
    if (writer_) {
        if (!written_) generate();
        writer_->close();
        writer_.reset();
        owned_text_writer_.reset();
        text_writer_ = nullptr;
        file_name_.reset();
    }
}

void ResXResourceWriter::generate() {
    if (!writer_) init_writer();
    if (written_) throw UsageError("The resource is already generated.");
    written_ = true;
    writer_->write_end_element();
    writer_->flush();
}

}  // namespace cb::dotnet::decompiler::util
