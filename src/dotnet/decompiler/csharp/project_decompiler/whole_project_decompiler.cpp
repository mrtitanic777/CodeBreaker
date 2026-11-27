#include "dotnet/decompiler/csharp/project_decompiler/whole_project_decompiler.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <stdexcept>
#include <thread>
#include <unordered_map>

#include "core/fs.hpp"
#include "core/parallel.hpp"
#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/error_tolerant_output_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/formatting_options_factory.hpp"
#include "dotnet/decompiler/csharp/project_decompiler/target_services.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax_tree.hpp"
#include "dotnet/decompiler/csharp/transforms/escape_invalid_identifiers.hpp"
#include "dotnet/decompiler/csharp/transforms/remove_cls_compliant_attribute.hpp"
#include "dotnet/decompiler/instrumentation/decompiler_event_source.hpp"
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/dotnet_path.hpp"
#include "dotnet/decompiler/util/resources_file.hpp"
#include "dotnet/decompiler/util/resx_resource_writer.hpp"
#include "dotnet/decompiler/util/win32_resources.hpp"
#include "dotnet/metadata/blob_reader.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

namespace srm = cb::dotnet::metadata;
namespace stdfs = std::filesystem;
namespace path = util::path;
using instrumentation::DecompilerEventSource;

namespace {

constexpr int kMaxSegmentLength = 255;

// ---- output ----------------------------------------------------------------------------------------------------------------------

// The text sink of the C# output visitor over a TextWriter of the project: where the file is written.
class FileTextWriter final : public output_visitor::TextWriter {
public:
    using output_visitor::TextWriter::write_line;
    explicit FileTextWriter(util::TextWriter& writer) : writer_(writer) {}
    void write(char ch) override { writer_.write_char(ch); }
    void write(std::string_view text) override { writer_.write(text); }
    void write_line() override { writer_.write_line(); }
    const std::string& new_line() const override {
        static const std::string kNewLine = "\r\n";
        return kNewLine;
    }

private:
    util::TextWriter& writer_;
};

// The default of DecompilerSettings.CSharpFormattingOptions.
output_visitor::CSharpFormattingOptions default_formatting_options() {
    output_visitor::CSharpFormattingOptions options = output_visitor::FormattingOptionsFactory::create_allman();
    options.indent_switch_body = false;
    options.array_initializer_wrapping = output_visitor::Wrapping::WrapIfTooLong;
    options.auto_property_formatting = output_visitor::PropertyFormatting::SingleLine;
    return options;
}

// Puts the error text where the file's contents would have gone. The writer itself may be what failed - a full disk, a stream already closed - so a
// second failure while reporting the first is dropped rather than allowed to take the export down.
void write_error_comment(util::TextWriter* writer, const std::exception& error) {
    if (writer == nullptr) return;
    try {
        // The failure may have interrupted the output visitor mid-line.
        writer->write_line();
        for (const std::string& line : CSharpDecompiler::get_error_comment_lines(error)) writer->write_line("// " + line);
    } catch (const util::OperationCanceledError&) {
        throw;
    } catch (...) {
    }
}

// The exception stored in an exception_ptr, as a DecompilerError about the file.
DecompilerError make_error(const metadata::MetadataFile& module, const std::string& message, const std::exception_ptr& stored) {
    try {
        std::rethrow_exception(stored);
    } catch (const std::exception& ex) {
        return DecompilerError(module, message, ex);
    } catch (...) {
        const std::runtime_error unknown("an exception that is not a std::exception");
        return DecompilerError(module, message, unknown);
    }
}

// ---- names -----------------------------------------------------------------------------------------------------------------------

bool is_reserved_file_system_name(const std::u16string& name) {
    std::string upper;
    for (char16_t c : name) {
        if (c >= 0x80) return false;
        upper.push_back(static_cast<char>(c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c));
    }
    static const char* const kNames[] = {"AUX",  "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "CON",
                                         "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9", "NUL",  "PRN"};
    for (const char* n : kNames)
        if (upper == n) return true;
    return false;
}

// Appends an underscore to the segment [segment_start..) of `b` if its base name (the part before `base_name_end`, or the whole segment when there was no
// dot) is a reserved Windows device name (CON, PRN, AUX, NUL, COM1-9, LPT1-9): "con" becomes "con_" and "con.txt" becomes "con_.txt". Windows
// device-name parsing ignores everything after the first dot, so the underscore must be inserted before it, not appended at the end.
void escape_reserved_file_system_name(std::u16string& b, int segment_start, int base_name_end) {
    if (base_name_end < 0) base_name_end = static_cast<int>(b.size());
    const int base_name_length = base_name_end - segment_start;
    // All reserved names are 3 or 4 characters long; checking the length first avoids making a substring for segments that cannot be reserved anyway.
    if ((base_name_length == 3 || base_name_length == 4) &&
        is_reserved_file_system_name(b.substr(static_cast<std::size_t>(segment_start), static_cast<std::size_t>(base_name_length))))
        b.insert(static_cast<std::size_t>(base_name_end), 1, u'_');
}

std::u16string trim(const std::u16string& s) {
    std::size_t start = 0, end = s.size();
    while (start < end && util::is_white_space(s[start])) ++start;
    while (end > start && util::is_white_space(s[end - 1])) --end;
    return s.substr(start, end - start);
}

// Cleans up a node name for use as a file system name. If `separate_at_dots` is active, dots are seen as segment separators. Each segment is limited to
// kMaxSegmentLength characters. If `treat_as_file_name` is active, we check for file a extension and try to preserve it, if it's valid.
std::string clean_up_name(const std::string& text_utf8, bool separate_at_dots, bool treat_as_file_name, bool treat_as_path) {
    std::u16string text = util::utf16(text_utf8);
    std::optional<std::u16string> extension;
    int current_segment_length = 0;
    // Extract extension from the end of the name, if valid
    if (treat_as_file_name) {
        // Check if input is a file name, i.e., has a valid extension. If yes, preserve extension and append it at the end. But only, if the extension
        // length does not exceed maxSegmentLength, if that's the case we just give up and treat the extension no different from the file name.
        const std::size_t last_dot = text.rfind(u'.');
        if (last_dot != std::u16string::npos && static_cast<int>(text.size() - last_dot) < kMaxSegmentLength) {
            const std::u16string original_text = text;
            extension = text.substr(last_dot);
            text.erase(last_dot);
            for (char16_t c : *extension) {
                if (!(util::is_letter_or_digit(c) || c == u'-' || c == u'_' || c == u'.')) {
                    // extension contains an invalid character, therefore cannot be a valid extension.
                    extension.reset();
                    text = original_text;
                    break;
                }
            }
        }
    }
    // Remove anything that could be confused with a rooted path.
    std::size_t pos = text.find(u':');
    if (pos != std::u16string::npos && pos > 0) text = text.substr(0, pos);
    text = trim(text);
    // Remove generics
    pos = text.find(u'`');
    if (pos != std::u16string::npos && pos > 0) text = trim(text.substr(0, pos));
    // Whitelist allowed characters, replace everything else:
    std::u16string b;
    b.reserve(text.size() + (extension ? extension->size() : 0));
#ifdef _WIN32
    const bool count_bytes = false;
#else
    const bool count_bytes = true;
#endif
    int segment_start = 0;
    int base_name_end = -1;
    for (char16_t c : text) {
        if (util::is_letter_or_digit(c) || c == u'-' || c == u'_') {
            if (count_bytes) {
                // Encoding.UTF8.GetByteCount of the one character (a lone surrogate is the three bytes of U+FFFD)
                current_segment_length += c < 0x80 ? 1 : c < 0x800 ? 2 : 3;
            } else {
                current_segment_length += 1;
            }
            // if the current segment exceeds maxSegmentLength characters, skip until the end of the segment.
            if (current_segment_length <= kMaxSegmentLength) b.push_back(c);
        } else if (c == u'.' && !b.empty() && b.back() != u'.') {
            ++current_segment_length;
            if (separate_at_dots) {
                // The dot ends the current segment.
                escape_reserved_file_system_name(b, segment_start, base_name_end);
                b.push_back(u'.');
                segment_start = static_cast<int>(b.size());
                base_name_end = -1;
                // Reset length at end of segment.
                current_segment_length = 0;
            } else if (current_segment_length <= kMaxSegmentLength) {
                // The first dot ends the segment's base name, the part Windows device-name parsing looks at.
                if (base_name_end < 0) base_name_end = static_cast<int>(b.size());
                b.push_back(u'.');   // allow dot, but never two in a row
            }
        } else if (treat_as_path && (c == u'/' || c == u'\\') && current_segment_length > 0) {
            // if we treat this as a file name, we've started a new segment
            escape_reserved_file_system_name(b, segment_start, base_name_end);
            b.push_back(static_cast<char16_t>(path::kDirectorySeparatorChar));
            segment_start = static_cast<int>(b.size());
            base_name_end = -1;
            current_segment_length = 0;
        } else {
            if (util::is_high_surrogate(c)) {
                // only add one replacement character for surrogate pairs
                continue;
            }

            ++current_segment_length;
            // if the current segment exceeds maxSegmentLength characters, skip until the end of the segment.
            if (current_segment_length <= kMaxSegmentLength) b.push_back(u'-');
        }
    }
    if (b.empty()) b.push_back(u'-');
    escape_reserved_file_system_name(b, segment_start, base_name_end);
    std::u16string name = b;
    if (extension) {
        // make sure that adding the extension to the filename does not exceed maxSegmentLength. trim the name, if necessary.
        if (static_cast<int>(name.size() + extension->size()) > kMaxSegmentLength) name.erase(name.size() - extension->size());
        name += *extension;
    }

    if (name == u".") return "_";
    return util::utf8(name);
}

// the application manifest without the white space the default one is written with, as Encoding.UTF8.GetString makes it (a byte order mark is skipped)
std::string clean_up_application_manifest(const Bytes& app_manifest) {
    const bool bom = app_manifest.size() >= 3 && app_manifest[0] == 0xEF && app_manifest[1] == 0xBB && app_manifest[2] == 0xBF;
    std::string result;
    result.reserve(app_manifest.size());
    for (std::size_t i = bom ? 3 : 0; i < app_manifest.size(); ++i) {
        const char c = static_cast<char>(app_manifest[i]);
        switch (c) {
        case '\t':
        case '\n':
        case '\r':
        case ' ': continue;
        default: break;
        }
        result.push_back(c);
    }
    return result;
}

bool is_default_application_manifest(const Bytes& app_manifest) {
    static const std::string kDefaultAppManifest =
        "<?xmlversion=\"1.0\"encoding=\"UTF-8\"standalone=\"yes\"?><assemblyxmlns=\"urn:schemas-microsoft-com"
        ":asm.v1\"manifestVersion=\"1.0\"><assemblyIdentityversion=\"1.0.0.0\"name=\"MyApplication.app\"/><tr"
        "ustInfoxmlns=\"urn:schemas-microsoft-com:asm.v2\"><security><requestedPrivilegesxmlns=\"urn:schemas-"
        "microsoft-com:asm.v3\"><requestedExecutionLevellevel=\"asInvoker\"uiAccess=\"false\"/></requestedPri"
        "vileges></security></trustInfo></assembly>";
    return clean_up_application_manifest(app_manifest) == kDefaultAppManifest;
}

constexpr int kRtIcon = 3;
constexpr int kRtGroupIcon = 14;
constexpr int kRtManifest = 24;

template <class T>
T read_le(const Bytes& data, std::size_t offset) {
    if (offset + sizeof(T) > data.size()) throw std::out_of_range("the icon group data is too short");
    T value{};
    std::memcpy(&value, data.data() + offset, sizeof(T));
    return value;
}

template <class T>
void write_le(Bytes& out, T value) {
    const std::size_t at = out.size();
    out.resize(at + sizeof(T));
    std::memcpy(out.data() + at, &value, sizeof(T));
}

// Makes the .ico file of the module's application icon from its icon group and icons; nullopt when it has none (or one is missing).
std::optional<Bytes> create_application_icon(const util::Win32ResourceDirectory& resources) {
    const util::Win32ResourceDirectory* group_directory = util::find(resources, util::Win32ResourceName(kRtGroupIcon));
    const util::Win32ResourceDirectory* group_first = group_directory != nullptr ? group_directory->first_directory() : nullptr;
    const util::Win32ResourceData* group_data = group_first != nullptr ? group_first->first_data() : nullptr;
    if (group_data == nullptr) return std::nullopt;
    const Bytes icon_group = group_data->data();

    const util::Win32ResourceDirectory* icon_dir = util::find(resources, util::Win32ResourceName(kRtIcon));
    if (icon_dir == nullptr) return std::nullopt;

    // GRPICONDIR: idReserved, idType, idCount (ushort each), then idCount GRPICONDIRENTRY of 14 bytes (packed): bWidth, bHeight, bColorCount, bReserved
    // (bytes), wPlanes, wBitCount (ushort), dwBytesInRes (uint), nID (short)
    Bytes out;
    write_le<std::uint16_t>(out, read_le<std::uint16_t>(icon_group, 0));   // idReserved
    write_le<std::uint16_t>(out, read_le<std::uint16_t>(icon_group, 2));   // idType
    const std::uint16_t icon_count = read_le<std::uint16_t>(icon_group, 4);
    write_le<std::uint16_t>(out, icon_count);

    std::uint32_t offset = (2 * 3) + (static_cast<std::uint32_t>(icon_count) * 0x10);
    for (int i = 0; i < icon_count; ++i) {
        const std::size_t entry = 6 + static_cast<std::size_t>(i) * 14;
        out.push_back(read_le<std::uint8_t>(icon_group, entry));       // bWidth
        out.push_back(read_le<std::uint8_t>(icon_group, entry + 1));   // bHeight
        out.push_back(read_le<std::uint8_t>(icon_group, entry + 2));   // bColorCount
        out.push_back(read_le<std::uint8_t>(icon_group, entry + 3));   // bReserved
        write_le<std::uint16_t>(out, read_le<std::uint16_t>(icon_group, entry + 4));   // wPlanes
        write_le<std::uint16_t>(out, read_le<std::uint16_t>(icon_group, entry + 6));   // wBitCount
        const std::uint32_t bytes_in_res = read_le<std::uint32_t>(icon_group, entry + 8);
        write_le<std::uint32_t>(out, bytes_in_res);
        write_le<std::uint32_t>(out, offset);
        offset += bytes_in_res;
    }

    for (int i = 0; i < icon_count; ++i) {
        const std::size_t entry = 6 + static_cast<std::size_t>(i) * 14;
        const std::int16_t id = read_le<std::int16_t>(icon_group, entry + 12);
        const util::Win32ResourceDirectory* icon_directory = icon_dir->find_directory(util::Win32ResourceName(static_cast<int>(id)));
        const util::Win32ResourceData* icon_data = icon_directory != nullptr ? icon_directory->first_data() : nullptr;
        if (icon_data == nullptr) return std::nullopt;
        const Bytes icon = icon_data->data();
        out.insert(out.end(), icon.begin(), icon.end());
    }

    return out;
}

std::optional<Bytes> create_application_manifest(const util::Win32ResourceDirectory& resources) {
    const util::Win32ResourceDirectory* by_type = util::find(resources, util::Win32ResourceName(kRtManifest));
    const util::Win32ResourceDirectory* first = by_type != nullptr ? by_type->first_directory() : nullptr;
    const util::Win32ResourceData* data = first != nullptr ? first->first_data() : nullptr;
    if (data == nullptr) return std::nullopt;
    return data->data();
}

// File.WriteAllBytes
void write_all_bytes(const std::string& file_name, ByteView data) {
    util::StreamWriter writer(path::from_text(file_name));
    writer.write(as_text(data));
    writer.close();
}

}  // namespace

// ---- construction --------------------------------------------------------------------------------------------------------------

WholeProjectDecompiler::WholeProjectDecompiler(metadata::IAssemblyResolver& assembly_resolver)
    : WholeProjectDecompiler(std::make_unique<DecompilerSettings>(), assembly_resolver) {}

WholeProjectDecompiler::WholeProjectDecompiler(std::unique_ptr<DecompilerSettings> settings, metadata::IAssemblyResolver& assembly_resolver)
    : WholeProjectDecompiler(*settings, assembly_resolver, nullptr, nullptr, nullptr) {
    // the settings made for this object belong to it
    owned_settings_ = std::move(settings);
}

WholeProjectDecompiler::WholeProjectDecompiler(const DecompilerSettings& settings, metadata::IAssemblyResolver& assembly_resolver,
                                               const IProjectFileWriter* project_writer, metadata::IAssemblyReferenceClassifier* assembly_reference_classifier,
                                               const debug_info::IDebugInfoProvider* debug_info_provider)
    : csharp_formatting_options(default_formatting_options()),
      settings_(&settings),
      assembly_resolver_(&assembly_resolver),
      assembly_reference_classifier_(assembly_reference_classifier),
      debug_info_provider_(debug_info_provider),
      project_guid_(util::guid::new_guid()),
      project_writer_(project_writer != nullptr ? project_writer : &project_file_writer_from_settings(settings)) {
    max_degree_of_parallelism = static_cast<int>(std::max(1u, std::thread::hardware_concurrency()));
    if (assembly_reference_classifier_ == nullptr) {
        owned_classifier_ = std::make_unique<metadata::AssemblyReferenceClassifier>();
        assembly_reference_classifier_ = owned_classifier_.get();
    }
}

WholeProjectDecompiler::~WholeProjectDecompiler() = default;

LanguageVersion WholeProjectDecompiler::language_version() const { return language_version_ ? *language_version_ : settings_->get_minimum_required_version(); }

void WholeProjectDecompiler::set_language_version(LanguageVersion value) {
    validate_language_version(value);
    language_version_ = value;
}

void WholeProjectDecompiler::validate_language_version(LanguageVersion version) const {
    const LanguageVersion min_version = settings_->get_minimum_required_version();
    if (version < min_version) {
        throw UsageError("The chosen settings require at least " + language_version_text(min_version) + ". Please change the DecompilerSettings accordingly.");
    }
}

void WholeProjectDecompiler::record_error(DecompilerError error) {
    const std::lock_guard<std::mutex> lock(errors_mutex_);
    errors_.push_back(std::move(error));
}

void WholeProjectDecompiler::record_current_exception(const metadata::MetadataFile& module, const std::string& message) {
    try {
        throw;
    } catch (const util::OperationCanceledError&) {
        throw;
    } catch (const DecompilerError& error) {
        record_error(error);
    } catch (const std::exception& error) {
        record_error(DecompilerError(module, message, error));
    } catch (...) {
        const std::runtime_error unknown("an exception that is not a std::exception");
        record_error(DecompilerError(module, message, unknown));
    }
}

void WholeProjectDecompiler::record_failures(const metadata::MetadataFile& file, const std::string& what, const std::function<void()>& step) {
    try {
        step();
    } catch (...) {
        record_current_exception(file, "Error writing " + what);
    }
}

std::string WholeProjectDecompiler::combine_target(const std::string& relative) const { return path::combine(target_directory_, relative); }

// ---- the project -----------------------------------------------------------------------------------------------------------------

void WholeProjectDecompiler::decompile_project(const std::shared_ptr<const metadata::MetadataFile>& file, const std::string& target_directory,
                                               const util::CancellationToken& cancellation_token) {
    const std::string project_file_name = path::combine(target_directory, clean_up_file_name(file->name(), std::string(".csproj")));
    std::unique_ptr<util::TextWriter> writer = create_file(project_file_name);
    decompile_project(file, target_directory, *writer, cancellation_token);
    writer->close();
}

solution::ProjectId WholeProjectDecompiler::decompile_project(const std::shared_ptr<const metadata::MetadataFile>& file_ptr, const std::string& target_directory,
                                                              util::TextWriter& project_file_writer, const util::CancellationToken& cancellation_token) {
    const metadata::MetadataFile& file = *file_ptr;
    if (target_directory.empty()) throw UsageError("Must set TargetDirectory");
    // The LanguageVersion setter already rejects a version below what the settings require, but the settings are mutable and shared, so re-validate
    // against the settings actually in effect now - otherwise the exported project would carry a LangVersion under which the emitted code cannot compile.
    if (language_version_) validate_language_version(*language_version_);
    DecompilerEventSource::log().project_decompilation_start(file.name());
    int code_file_count = 0, resource_file_count = 0;
    struct Stop {
        const std::string& name;
        int& code;
        int& resource;
        ~Stop() { DecompilerEventSource::log().project_decompilation_stop(name, code, resource); }
    } stop{file.name(), code_file_count, resource_file_count};

    target_directory_ = target_directory;
    directories_.clear();
    {
        const std::lock_guard<std::mutex> lock(errors_mutex_);
        errors_.clear();
    }
    std::vector<ProjectItemInfo> resources;
    record_failures(file, "resource files", [&] { write_resource_files_in_project(file, resources); });
    resource_file_count = static_cast<int>(resources.size());
    std::vector<PartialTypeInfo> partial_types;
    for (const ProjectItemInfo& r : resources)
        if (r.partial_types) partial_types.insert(partial_types.end(), r.partial_types->begin(), r.partial_types->end());
    std::vector<ProjectItemInfo> files = write_code_files_in_project(file_ptr, partial_types, cancellation_token);
    code_file_count = static_cast<int>(files.size());
    files.insert(files.end(), resources.begin(), resources.end());
    const auto* module = dynamic_cast<const metadata::PEFile*>(&file);
    if (module != nullptr) {
        record_failures(file, "miscellaneous files", [&] { write_miscellaneous_files_in_project(*module, files); });
    }
    if (strong_name_key_file_) {
        stdfs::copy_file(path::from_text(*strong_name_key_file_), path::from_text(path::combine(target_directory, path::get_file_name(*strong_name_key_file_))),
                         stdfs::copy_options::overwrite_existing);
    }

    project_writer_->write(project_file_writer, *this, files, file);

    const std::string platform_name = module != nullptr ? target_services::get_platform_name(*module) : "AnyCPU";
    return solution::ProjectId(platform_name, project_guid_, solution::ProjectTypeGuids::csharp_windows());
}

// ---- code files ------------------------------------------------------------------------------------------------------------------

bool WholeProjectDecompiler::include_type_when_decompiling_project(const metadata::MetadataFile& module, srm::TypeDefinitionHandle type) const {
    const srm::MetadataReader& metadata = module.metadata();
    const srm::TypeDefinition type_def = metadata.get_type_definition(type);
    const std::string name(metadata.get_string(type_def.name()));
    const std::string ns(metadata.get_string(type_def.namespace_()));
    if (name == "<Module>" || CSharpDecompiler::member_is_hidden(&module, srm::EntityHandle(type), *settings_)) return false;
    if (ns == "XamlGeneratedNamespace" && name == "GeneratedInternalTypeHelper") return false;
    if (!type_def.is_nested() && transforms::RemoveEmbeddedAttributes::attribute_names().contains(ns + "." + name)) return false;
    return true;
}

std::unique_ptr<util::TextWriter> WholeProjectDecompiler::create_file(const std::string& file_path) {
    return std::make_unique<util::StreamWriter>(path::from_text(file_path));
}

void WholeProjectDecompiler::create_directory(const std::string& directory_path) {
    const stdfs::path directory = path::from_text(directory_path);
    try {
        stdfs::create_directories(directory);
    } catch (const stdfs::filesystem_error&) {
        // a file is in the way
        stdfs::remove(directory);
        stdfs::create_directories(directory);
    }
}

std::unique_ptr<CSharpDecompiler> WholeProjectDecompiler::create_decompiler(const typesystem::DecompilerTypeSystem& ts) {
    auto decompiler = std::make_unique<CSharpDecompiler>(ts, *settings_);
    decompiler->debug_info_provider = debug_info_provider_;
    decompiler->ast_transforms().push_back(std::make_unique<transforms::EscapeInvalidIdentifiers>());
    decompiler->ast_transforms().push_back(std::make_unique<transforms::RemoveCLSCompliantAttribute>());
    return decompiler;
}

std::vector<ProjectItemInfo> WholeProjectDecompiler::write_assembly_info(const typesystem::DecompilerTypeSystem& ts, const util::CancellationToken& cancellation_token) {
    std::unique_ptr<CSharpDecompiler> decompiler = create_decompiler(ts);
    decompiler->cancellation_token = cancellation_token;
    decompiler->ast_transforms().push_back(std::make_unique<transforms::RemoveCompilerGeneratedAssemblyAttributes>());
    syntax::SyntaxTree* syntax_tree = decompiler->decompile_module_and_assembly_attributes();

    const std::string prop = "Properties";
    if (directories_.insert(prop).second) create_directory(combine_target(prop));
    const std::string assembly_info = path::combine(prop, "AssemblyInfo.cs");
    {
        std::unique_ptr<util::TextWriter> w = create_file(combine_target(assembly_info));
        FileTextWriter text(*w);
        output_visitor::CSharpOutputVisitor visitor(text, csharp_formatting_options);
        syntax_tree->accept_visitor(visitor);
        w->close();
    }
    return {ProjectItemInfo("Compile", assembly_info)};
}

namespace {

// A file of the project: its name, and the types it holds (what GroupBy(..., StringComparer.OrdinalIgnoreCase) makes).
struct FileGroup {
    std::string key;
    std::vector<srm::TypeDefinitionHandle> types;
};

// files.GroupBy(name of the type's file, ignoring case): groups in the order their first member comes, each with the first key it met.
template <class KeyOf>
std::vector<FileGroup> group_by_file(const std::vector<srm::TypeDefinitionHandle>& types, KeyOf key_of) {
    std::vector<FileGroup> groups;
    std::unordered_map<std::string, std::size_t, util::OrdinalIgnoreCaseHash, util::OrdinalIgnoreCaseEqual> index;
    for (const srm::TypeDefinitionHandle type : types) {
        std::string key = key_of(type);
        const auto it = index.find(key);
        if (it == index.end()) {
            index.emplace(key, groups.size());
            groups.push_back(FileGroup{std::move(key), {type}});
        } else {
            groups[it->second].types.push_back(type);
        }
    }
    return groups;
}

}  // namespace

std::vector<ProjectItemInfo> WholeProjectDecompiler::write_code_files_in_project(const std::shared_ptr<const metadata::MetadataFile>& module_ptr,
                                                                                 const std::vector<PartialTypeInfo>& partial_types,
                                                                                 const util::CancellationToken& cancellation_token) {
    const metadata::MetadataFile& module = *module_ptr;
    const srm::MetadataReader& metadata = module.metadata();
    auto get_file_name_for_handle = [&](srm::TypeDefinitionHandle h) {
        const srm::TypeDefinition type = metadata.get_type_definition(h);
        const std::string file = clean_up_file_name(std::string(metadata.get_string(type.name())), std::string(".cs"));
        const std::string ns(metadata.get_string(type.namespace_()));
        if (ns.empty()) return file;
        const std::string dir = settings_->use_nested_directories_for_namespaces ? clean_up_path(ns) : clean_up_directory_name(ns);
        if (directories_.insert(dir).second) {
            create_directory(combine_target(dir));
        }
        return path::combine(dir, file);
    };

    std::vector<srm::TypeDefinitionHandle> included;
    for (const srm::TypeDefinitionHandle td : metadata::get_top_level_type_definitions(metadata))
        if (include_type_when_decompiling_project(module, td)) included.push_back(td);
    std::vector<FileGroup> files = group_by_file(included, get_file_name_for_handle);
    DecompilationProgress progress;
    progress.total_units = static_cast<int>(files.size());
    progress.title = "Exporting project...";
    const typesystem::DecompilerTypeSystem ts(module_ptr, *assembly_resolver_, CSharpDecompiler::get_type_system_options(*settings_));
    std::vector<std::string> missing_files;
    std::mutex missing_files_mutex;
    // the types that need a file of their own because a file refers to them
    std::vector<srm::TypeDefinitionHandle> work_list;
    std::unordered_set<std::uint32_t> work_list_set;
    std::mutex work_list_mutex;
    std::unordered_set<std::uint32_t> processed_types;
    std::mutex progress_mutex;
    std::atomic<int> units_completed{0};

    auto process_files = [&](const std::vector<FileGroup>& group_files) {
        for (const FileGroup& f : group_files)
            for (const srm::TypeDefinitionHandle t : f.types) processed_types.insert(t.row());
        const int degree = std::max(1, std::min(max_degree_of_parallelism, static_cast<int>(group_files.size())));
        cb::parallel::ThreadPool pool(static_cast<std::size_t>(degree));
        cb::parallel::parallel_for(pool, group_files.size(), [&](std::size_t index) {
            cancellation_token.throw_if_cancellation_requested();
            const FileGroup& file = group_files[index];
            DecompilerEventSource::log().project_file_start(file.key, static_cast<int>(file.types.size()));
            // Everything that can fail for this one file - creating it included, which is where a path too long for the file system surfaces - belongs
            // inside the try.
            std::unique_ptr<util::TextWriter> w;
            std::unique_ptr<CSharpDecompiler> decompiler;
            auto finish = [&] {
                if (decompiler) {
                    for (const DecompilerError& error : decompiler->errors()) record_error(error);
                }
                try {
                    if (w) w->close();   // Dispose flushes: on a full disk this is where the write actually fails.
                } catch (const util::OperationCanceledError&) {
                    throw;
                } catch (const std::exception& ex) {
                    record_error(DecompilerError(module, "Error writing '" + file.key + "'", ex));
                }
                DecompilerEventSource::log().project_file_stop(file.key);
            };
            try {
                w = create_file(combine_target(file.key));
                decompiler = create_decompiler(ts);

                for (const PartialTypeInfo& partial_type : partial_types) decompiler->add_partial_type_definition(partial_type);

                decompiler->cancellation_token = cancellation_token;
                syntax::SyntaxTree* syntax_tree = decompiler->decompile_types(file.types);

                for (syntax::AstNode* node : syntax_tree->descendants()) {
                    const auto* type_rr = dynamic_cast<const semantics::TypeResolveResult*>(get_resolve_result(node));
                    const typesystem::ITypeDefinition* td = type_rr != nullptr && type_rr->type() != nullptr ? type_rr->type()->get_definition() : nullptr;
                    if (td == nullptr || td->parent_module() != static_cast<const typesystem::IModule*>(&ts.main_metadata_module())) continue;
                    while (td->declaring_type_definition() != nullptr) td = td->declaring_type_definition();
                    const srm::EntityHandle token = td->metadata_token();
                    if (!token.is_nil()) {
                        const srm::TypeDefinitionHandle type_handle(token);
                        if (!processed_types.contains(type_handle.row())) {
                            const std::lock_guard<std::mutex> lock(work_list_mutex);
                            if (work_list_set.insert(type_handle.row()).second) work_list.push_back(type_handle);
                        }
                    }
                }

                // A member the output visitor cannot write is replaced by the error text rather than truncating the file where it failed.
                FileTextWriter text(*w);
                output_visitor::ErrorTolerantOutputVisitor output_visitor(text, csharp_formatting_options);
                syntax_tree->accept_visitor(output_visitor);
                for (const std::exception_ptr& output_error : output_visitor.errors())
                    record_error(make_error(module, "Error writing '" + file.key + "'", output_error));
            } catch (const util::OperationCanceledError&) {
                finish();
                throw;
            } catch (const std::exception& inner_exception) {
                // Whatever the decompiler could not cope with here, the remaining files are unaffected and the user still gets a complete project; the
                // error takes the place of the file's contents.
                if (const auto* decompiler_error = dynamic_cast<const DecompilerError*>(&inner_exception)) {
                    record_error(*decompiler_error);
                } else {
                    record_error(DecompilerError(module, "Error decompiling for '" + file.key + "'", inner_exception));
                }
                if (!w) {
                    // Nothing was written, so nothing can carry the error text - and the project must not claim a file that is not there.
                    const std::lock_guard<std::mutex> lock(missing_files_mutex);
                    missing_files.push_back(file.key);
                }
                write_error_comment(w.get(), inner_exception);
            }
            finish();
            DecompilationProgress snapshot;
            {
                const std::lock_guard<std::mutex> lock(progress_mutex);
                progress.status = file.key;
                progress.units_completed = ++units_completed;
                snapshot = progress;
            }
            if (progress_indicator) progress_indicator(snapshot);
        });
    };

    process_files(files);
    while (!work_list.empty()) {
        std::vector<srm::TypeDefinitionHandle> pending;
        pending.swap(work_list);
        work_list_set.clear();
        std::vector<FileGroup> additional_files = group_by_file(pending, get_file_name_for_handle);
        process_files(additional_files);
        files.insert(files.end(), additional_files.begin(), additional_files.end());
        const std::lock_guard<std::mutex> lock(progress_mutex);
        progress.total_units = static_cast<int>(files.size());
    }

    // The assembly-level attributes are a single file like any other: failing to decompile them costs that file, not the export.
    std::vector<ProjectItemInfo> assembly_info;
    try {
        assembly_info = write_assembly_info(ts, cancellation_token);
    } catch (...) {
        record_current_exception(module, "Error decompiling the module and assembly attributes");
    }

    // files.Select(f => f.Key).Except(missingFiles, Platform.FileNameComparer).Select(f => new ProjectItemInfo("Compile", f)).Concat(assemblyInfo)
    std::vector<ProjectItemInfo> result;
    std::unordered_set<std::string, util::FileNameHash, util::FileNameEqual> seen;
    for (const std::string& missing : missing_files) seen.insert(missing);
    for (const FileGroup& f : files) {
        if (!seen.insert(f.key).second) continue;
        result.emplace_back("Compile", f.key);
    }
    result.insert(result.end(), assembly_info.begin(), assembly_info.end());
    return result;
}

// ---- resources -------------------------------------------------------------------------------------------------------------------

void WholeProjectDecompiler::write_resource_files_in_project(const metadata::MetadataFile& module, std::vector<ProjectItemInfo>& items) {
    for (const std::unique_ptr<metadata::Resource>& r : metadata::get_resources(module)) {
        if (r->resource_type() != metadata::ResourceType::Embedded) continue;
        std::vector<ProjectItemInfo> resource_items;
        try {
            write_resource_file_in_project(*r, resource_items);
        } catch (...) {
            // One resource nobody can decode - a mangled .resources blob, a BAML stream the decompiler chokes on - costs that resource, not the ones
            // behind it.
            record_current_exception(module, "Error writing resource '" + r->name() + "'");
            continue;
        }
        for (ProjectItemInfo& item : resource_items) items.push_back(std::move(item));
    }
}

namespace {

bool ends_with_ignore_case(const std::string& text, const std::string& suffix) {
    return text.size() >= suffix.size() && util::compare_ordinal_ignore_case(std::string_view(text).substr(text.size() - suffix.size()), suffix) == 0;
}

}  // namespace

void WholeProjectDecompiler::write_resource_file_in_project(const metadata::Resource& r, std::vector<ProjectItemInfo>& items) {
    const std::optional<ByteView> stream = r.try_open_stream();
    if (!stream) return;

    if (ends_with_ignore_case(r.name(), ".resources")) {
        bool decoded_into_individual_files;
        std::vector<ProjectItemInfo> individual_resources;
        std::int64_t stream_position = 0;   // where the stream upstream reads from is left when the file has been read
        try {
            const util::ResourcesFile resources_file(*stream, &stream_position);
            if (resources_file.all_entries_are_streams()) {
                for (int i = 0; i < resources_file.resource_count(); ++i) {
                    const std::string name = resources_file.get_resource_name(i);
                    const util::ResourceValue value = resources_file.get_resource_value(i);
                    const std::string file_name = sanitize_file_name(name);
                    const std::optional<std::string> dir_name = path::get_directory_name(file_name);
                    if (dir_name && !dir_name->empty() && directories_.insert(*dir_name).second) {
                        create_directory(combine_target(*dir_name));
                    }
                    const auto& entry_stream = std::get<util::ResourceStream>(value);
                    for (ProjectItemInfo& item : write_resource_to_file(file_name, name, ByteView(entry_stream.bytes))) individual_resources.push_back(std::move(item));
                }
                decoded_into_individual_files = true;
            } else {
                decoded_into_individual_files = false;
            }
        } catch (const srm::BadImageFormatError&) {
            decoded_into_individual_files = false;
        } catch (const util::EndOfStreamError&) {
            decoded_into_individual_files = false;
        }
        if (decoded_into_individual_files) {
            for (ProjectItemInfo& entry : individual_resources) items.push_back(std::move(entry));
        } else {
            const std::string file_name = get_file_name_for_resource(r.name());
            for (ProjectItemInfo& entry : write_resource_to_file(file_name, r.name(), *stream)) items.push_back(std::move(entry));
        }
    } else {
        const std::string file_name = get_file_name_for_resource(r.name());
        write_all_bytes(combine_target(file_name), *stream);
        items.push_back(ProjectItemInfo("EmbeddedResource", file_name).with("LogicalName", r.name()));
    }
}

std::vector<ProjectItemInfo> WholeProjectDecompiler::write_resource_to_file(const std::string& file_name, const std::string& resource_name, ByteView entry_stream) {
    // Where the stream is read from when the resource has been looked at: a resource that cannot be decoded is copied from there, as upstream's stream
    // is left there.
    std::int64_t stream_position = 0;
    if (ends_with_ignore_case(file_name, ".resources")) {
        const std::string resx = path::change_extension(file_name, std::string_view(".resx"));
        try {
            {
                util::ResXResourceWriter writer(path::from_text(combine_target(resx)));
                const util::ResourcesFile entries(entry_stream, &stream_position);
                for (int i = 0; i < entries.resource_count(); ++i) {
                    const std::string name = entries.get_resource_name(i);
                    writer.add_resource(name, entries.get_resource_value(i));
                }
            }
            return {ProjectItemInfo("EmbeddedResource", resx).with("LogicalName", resource_name)};
        } catch (const srm::BadImageFormatError&) {
            // if the .resources can't be decoded, just save them as-is
        } catch (const util::EndOfStreamError&) {
            // if the .resources can't be decoded, just save them as-is
        }
    }
    const std::size_t from = static_cast<std::size_t>(std::min<std::int64_t>(stream_position, static_cast<std::int64_t>(entry_stream.size())));
    write_all_bytes(combine_target(file_name), entry_stream.subspan(from));
    return {ProjectItemInfo("EmbeddedResource", file_name).with("LogicalName", resource_name)};
}

std::string WholeProjectDecompiler::get_file_name_for_resource(const std::string& full_name_in) {
    // Clean up the name first and ensure the length does not exceed the maximum length supported by the OS.
    const std::string full_name = sanitize_file_name(full_name_in);
    // The purpose of the below algorithm is to "maximize" the directory name and "minimize" the file name. That is, a full name of the form
    // "Namespace1.Namespace2{...}.NamespaceN.ResourceName" is split such that the directory part Namespace1\Namespace2\... reuses as many existing
    // directories as possible, and only the remaining name parts are used as prefix for the filename. This is not affected by the
    // UseNestedDirectoriesForNamespaces setting.
    std::vector<std::string> split_name;
    {
        std::size_t start = 0;
        for (std::size_t i = 0; i <= full_name.size(); ++i) {
            if (i == full_name.size() || full_name[i] == '\\' || full_name[i] == '/') {
                split_name.push_back(full_name.substr(start, i - start));
                start = i + 1;
            }
        }
    }
    auto join = [&](const char* separator, std::size_t from, std::size_t count) {
        std::string result;
        for (std::size_t i = 0; i < count; ++i) {
            if (i > 0) result += separator;
            result += split_name[from + i];
        }
        return result;
    };
    std::string file_name = join(".", 0, split_name.size());
    const std::string separator(1, path::kDirectorySeparatorChar);
    for (std::size_t i = split_name.size() - 1; i > 0; --i) {
        const std::string ns = join(separator.c_str(), 0, i);
        if (directories_.contains(ns)) {
            const std::string name = join(".", i, split_name.size() - i);
            file_name = path::combine(ns, name);
            break;
        }
    }
    return file_name;
}

// ---- miscellaneous files ---------------------------------------------------------------------------------------------------------

std::optional<ProjectItemInfo> WholeProjectDecompiler::try_write(const metadata::MetadataFile& module, const std::string& what,
                                                                 const std::function<std::optional<ProjectItemInfo>()>& write) {
    try {
        return write();
    } catch (...) {
        record_current_exception(module, "Error writing '" + what + "'");
        return std::nullopt;
    }
}

void WholeProjectDecompiler::write_miscellaneous_files_in_project(const metadata::PEFile& module, std::vector<ProjectItemInfo>& items) {
    const std::unique_ptr<util::Win32ResourceDirectory> resources = util::read_win32_resources(module.reader());
    if (!resources) return;

    // Each file is written on its own, so the one that fails is the only one lost.
    if (auto item = try_write(module, "app.ico", [&]() -> std::optional<ProjectItemInfo> {
            const std::optional<Bytes> app_icon = create_application_icon(*resources);
            if (!app_icon) return std::nullopt;
            write_all_bytes(combine_target("app.ico"), *app_icon);
            return ProjectItemInfo("ApplicationIcon", "app.ico");
        }))
        items.push_back(std::move(*item));

    if (auto item = try_write(module, "app.manifest", [&]() -> std::optional<ProjectItemInfo> {
            const std::optional<Bytes> app_manifest = create_application_manifest(*resources);
            if (!app_manifest || is_default_application_manifest(*app_manifest)) return std::nullopt;
            write_all_bytes(combine_target("app.manifest"), *app_manifest);
            return ProjectItemInfo("ApplicationManifest", "app.manifest");
        }))
        items.push_back(std::move(*item));

    if (auto item = try_write(module, "app.config", [&]() -> std::optional<ProjectItemInfo> {
            const std::string app_config = path::native_text(module.file_name()) + ".config";
            std::error_code ec;
            if (!stdfs::is_regular_file(path::from_text(app_config), ec)) return std::nullopt;
            stdfs::copy_file(path::from_text(app_config), path::from_text(combine_target("app.config")), stdfs::copy_options::overwrite_existing);
            return ProjectItemInfo("ApplicationConfig", path::get_file_name(app_config));
        }))
        items.push_back(std::move(*item));
}

// ---- names -----------------------------------------------------------------------------------------------------------------------

std::string WholeProjectDecompiler::clean_up_file_name(const std::string& text, const std::optional<std::string>& extension) {
    std::string name;
    if (!extension || extension->empty() || extension->starts_with('.')) name = text + extension.value_or(std::string());
    else name = text + "." + *extension;

    return clean_up_name(name, /*separate_at_dots=*/false, /*treat_as_file_name=*/extension.has_value() && !extension->empty(), /*treat_as_path=*/false);
}

std::string WholeProjectDecompiler::sanitize_file_name(const std::string& file_name) {
    return clean_up_name(file_name, /*separate_at_dots=*/false, /*treat_as_file_name=*/true, /*treat_as_path=*/true);
}

std::string WholeProjectDecompiler::clean_up_directory_name(const std::string& text) {
    return clean_up_name(text, /*separate_at_dots=*/false, /*treat_as_file_name=*/false, /*treat_as_path=*/false);
}

std::string WholeProjectDecompiler::clean_up_path(const std::string& text) {
    std::string result = clean_up_name(text, /*separate_at_dots=*/true, /*treat_as_file_name=*/false, /*treat_as_path=*/true);
    for (char& c : result)
        if (c == '.') c = path::kDirectorySeparatorChar;
    return result;
}

bool WholeProjectDecompiler::can_use_sdk_style_project_format(const metadata::MetadataFile& module) {
    return target_services::detect_target_framework(module).moniker().has_value();
}

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
