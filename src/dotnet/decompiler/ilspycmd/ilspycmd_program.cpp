// dotnet/decompiler/ilspycmd/ilspycmd_program.cpp -- see ilspycmd_program.hpp.
#include "dotnet/decompiler/ilspycmd/ilspycmd_program.hpp"

#include <zlib.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "core/fs.hpp"
#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/project_decompiler/whole_project_decompiler.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/disassembler/reflection_disassembler.hpp"
#include "dotnet/decompiler/documentation/id_string_provider.hpp"
#include "dotnet/decompiler/ilspycmd/command_line.hpp"
#include "dotnet/decompiler/ilspycmd/dotnet_enum.hpp"
#include "dotnet/decompiler/ilspycmd/metadata_table_dumper.hpp"
#include "dotnet/decompiler/ilspycmd/program_exit_codes.hpp"
#include "dotnet/decompiler/ilspycmd/resource_extensions.hpp"
#include "dotnet/decompiler/ilspycmd/single_file_bundle.hpp"
#include "dotnet/decompiler/ilspycmd/solution_writer.hpp"
#include "dotnet/decompiler/ilspycmd/types_parser.hpp"
#include "dotnet/decompiler/ilspycmd/validation_attributes.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/reflection_helper.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/dotnet_path.hpp"
#include "dotnet/decompiler/util/number_format.hpp"
#include "dotnet/decompiler/util/platform.hpp"
#include "dotnet/decompiler/util/text_writer.hpp"
#include "dotnet/decompiler/util/xml_linq.hpp"
#include "dotnet/metadata/type_name.hpp"

namespace cb::dotnet::decompiler::ilspycmd {
namespace {

namespace srm = cb::dotnet::metadata;
namespace stdfs = std::filesystem;

constexpr const char* kNewLine = "\r\n";

// The exit code of a .NET process that ended with an exception nothing handled (0xE0434352).
constexpr int kUnhandledExceptionExitCode = -532462766;

// ---- the command line's options, in the order upstream declares them (the help lists them in this order) -----------------------------

enum Opt : std::size_t {
    kOptVersion, kOptHelp, kOptOutputDir, kOptProject, kOptType, kOptMember, kOptIl, kOptIlSequencePoints, kOptGenPdb, kOptUsePdb, kOptList,
    kOptListResources, kOptResource, kOptDecompileBaml, kOptDumpTable, kOptJson, kOptLanguageVersion, kOptSettingsFile, kOptDecompilerSetting,
    kOptReferencePath, kOptIgnoreErrors, kOptNoDeadCode, kOptNoDeadStores, kOptDumpPackage, kOptNestedDirectories, kOptDisableUpdateCheck,
    kOptGenerateDiagrammer, kOptInclude, kOptExclude, kOptReportExcluded, kOptDocs, kOptStripNamespaces, kOptJsonOnly,
};

std::string version_text() {
    return std::string("ilspycmd: ") + kIlspycmdVersion + kNewLine + "ICSharpCode.Decompiler: " + kIlspycmdVersion;
}

std::vector<CommandOption> make_options() {
    const std::string generate_diagrammer = "--generate-diagrammer";
    const std::string exclude = generate_diagrammer + "-exclude";
    const std::string include = generate_diagrammer + "-include";
    // McMaster adds the values of an enum option and its default to the description, each on a line of its own.
    const std::string language_versions =
        "CSharp1, CSharp2, CSharp3, CSharp4, CSharp5, CSharp6, CSharp7, CSharp7_1, CSharp7_2, CSharp7_3, CSharp8_0, CSharp9_0, CSharp10_0, "
        "CSharp11_0, CSharp12_0, CSharp13_0, CSharp14_0, CSharp15_0, Preview, Latest";

    using T = OptionType;
    std::vector<CommandOption> options(kOptJsonOnly + 1);
    const auto set = [&](Opt index, std::string short_name, std::string long_name, std::string label, std::string description, OptionType type,
                         OptionRole role = OptionRole::Normal, bool show_in_help = true) {
        options[index] = CommandOption{std::move(short_name), std::move(long_name), std::move(label), std::move(description), type, role, show_in_help};
    };
    set(kOptVersion, "v", "version", "-v|--version", "Show version of ICSharpCode.Decompiler used.", T::NoValue, OptionRole::Version);
    set(kOptHelp, "h", "help", "-h|--help", "Show help information.", T::NoValue, OptionRole::Help);
    set(kOptOutputDir, "o", "outputdir", "-o|--outputdir <directory>", "The output directory, if omitted decompiler output is written to standard out.", T::SingleValue);
    set(kOptProject, "p", "project", "-p|--project", "Decompile assembly as compilable project. This requires the output directory option.", T::NoValue);
    set(kOptType, "t", "type", "-t|--type <type-name>", "The fully qualified name of the type to decompile.", T::SingleValue);
    set(kOptMember, "m", "member", "-m|--member <doc-id-or-token>",
        "The single member to decompile: an XML documentation id string (e.g. \"M:System.String.Concat(System.String,System.String)\", as also accepted by "
        "the UI's --navigateto option) or a metadata token (e.g. 0x06000005).",
        T::SingleValue);
    set(kOptIl, "il", "ilcode", "-il|--ilcode", "Show IL code.", T::NoValue);
    set(kOptIlSequencePoints, "", "il-sequence-points", "--il-sequence-points", "Show IL with sequence points. Implies -il.", T::NoValue);
    set(kOptGenPdb, "genpdb", "generate-pdb", "-genpdb|--generate-pdb", "Generate PDB.", T::NoValue);
    set(kOptUsePdb, "usepdb", "use-varnames-from-pdb", "-usepdb|--use-varnames-from-pdb", "Use variable names from PDB.", T::SingleOrNoValue);
    set(kOptList, "l", "list", "-l|--list <entity-type(s)>",
        "Lists all entities of the specified type(s). Valid types: c(lass), i(nterface), s(truct), d(elegate), e(num)", T::MultipleValue);
    set(kOptListResources, "", "list-resources", "--list-resources",
        "Lists all embedded resources in the assembly. Entries inside .resources containers are listed individually as '<container>/<entry>'.", T::NoValue);
    set(kOptResource, "", "resource", "--resource <name>",
        "Extract a single resource by name (as printed by --list-resources). Resources whose name ends with '.baml' are decompiled to XAML.", T::SingleValue);
    set(kOptDecompileBaml, "", "decompile-baml", "--decompile-baml",
        "When used with -p, decompile BAML resources to XAML files (Page items) instead of leaving them as raw byte streams.", T::NoValue);
    set(kOptDumpTable, "", "dump-table", "--dump-table <table>",
        "Dump a metadata table: prints RID, token, names, heap offsets and coded indexes of every row. <table> is the ECMA-335 table name (e.g. TypeDef, "
        "Property, MethodSemantics; case-insensitive) or table number (decimal or 0x-prefixed hex, e.g. 0x17).",
        T::SingleValue);
    set(kOptJson, "", "json", "--json", "Output as JSON. Currently only supported together with --dump-table.", T::NoValue);
    set(kOptLanguageVersion, "lv", "languageversion", "-lv|--languageversion <version>",
        "C# Language version: CSharp1, CSharp2, CSharp3, CSharp4, CSharp5, CSharp6, CSharp7, CSharp7_1, CSharp7_2, CSharp7_3, CSharp8_0, CSharp9_0, "
        "CSharp10_0, CSharp11_0, CSharp12_0, CSharp13_0, Preview or Latest\nAllowed values are: " + language_versions + ".\nDefault value is: Latest.",
        T::SingleValue);
    set(kOptSettingsFile, "", "ilspy-settingsfile", "--ilspy-settingsfile <path>", "Path to an ILSpy settings file.", T::SingleValue);
    set(kOptDecompilerSetting, "ds", "decompiler-setting", "-ds|--decompiler-setting <value>",
        "Set a decompiler setting. Use multiple times to set multiple settings.", T::MultipleValue);
    set(kOptReferencePath, "r", "referencepath", "-r|--referencepath <path>", "Path to a directory containing dependencies of the assembly that is being decompiled.",
        T::MultipleValue);
    set(kOptIgnoreErrors, "", "ignore-decompilation-errors", "--ignore-decompilation-errors",
        "Exit with success even when parts of the assembly could not be decompiled. The affected code carries the error text in the output and the failures "
        "are listed on stderr either way; only the exit status changes.",
        T::NoValue);
    set(kOptNoDeadCode, "", "no-dead-code", "--no-dead-code", "Remove dead code.", T::NoValue);
    set(kOptNoDeadStores, "", "no-dead-stores", "--no-dead-stores", "Remove dead stores.", T::NoValue);
    set(kOptDumpPackage, "d", "dump-package", "-d|--dump-package", "Dump package assemblies into a folder. This requires the output directory option.", T::NoValue);
    set(kOptNestedDirectories, "", "nested-directories", "--nested-directories", "Use nested directories for namespaces.", T::NoValue);
    set(kOptDisableUpdateCheck, "", "disable-updatecheck", "--disable-updatecheck",
        "If using ilspycmd in a tight loop or fully automated scenario, you might want to disable the automatic update check.", T::NoValue);
    set(kOptGenerateDiagrammer, "", "generate-diagrammer", generate_diagrammer,
        "Generates an interactive HTML diagrammer app from selected types in the target assembly - to the --outputdir or in a 'diagrammer' folder next to to "
        "the assembly by default.",
        T::NoValue);
    set(kOptInclude, "", "generate-diagrammer-include", include,
        "An optional regular expression matching Type.FullName used to whitelist types to include in the generated diagrammer.", T::SingleValue);
    set(kOptExclude, "", "generate-diagrammer-exclude", exclude,
        "An optional regular expression matching Type.FullName used to blacklist types to exclude from the generated diagrammer.", T::SingleValue);
    set(kOptReportExcluded, "", "generate-diagrammer-report-excluded", generate_diagrammer + "-report-excluded",
        "Outputs a report of types excluded from the generated diagrammer - whether by default because compiler-generated, explicitly by '" + exclude +
            "' or implicitly by '" + include + "'. You may find this useful to develop and debug your regular expressions.",
        T::NoValue);
    set(kOptDocs, "", "generate-diagrammer-docs", generate_diagrammer + "-docs",
        "The path or file:// URI of the XML file containing the target assembly's documentation comments. You only need to set this if a) you want your "
        "diagrams annotated with them and b) the file name differs from that of the assembly. To enable XML documentation output for your assembly, see "
        "https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/xmldoc/#create-xml-documentation-output",
        T::SingleValue);
    set(kOptStripNamespaces, "", "generate-diagrammer-strip-namespaces", generate_diagrammer + "-strip-namespaces",
        "Optional space-separated namespace names that are removed for brevity from XML documentation comments. Note that the order matters: e.g. replace "
        "'System.Collections' before 'System' to remove both of them completely.",
        T::MultipleValue);
    set(kOptJsonOnly, "", "generate-diagrammer-json-only", generate_diagrammer + "-json-only",
        "Whether to generate a model.json file instead of baking it into the HTML template. This is useful for the HTML/JS/CSS development loop.", T::NoValue,
        OptionRole::Normal, false);
    return options;
}

std::string extended_help_text_crlf() {
    // (the text starts with a line end and ends with one)
    static const char* text =
#include "dotnet/decompiler/ilspycmd/extended_help_text.inc"
        ;
    std::string result;
    for (const char* p = text; *p != '\0'; ++p) {
        if (*p == '\n') result += '\r';
        result += *p;
    }
    return result;
}

// ---- helpers ----------------------------------------------------------------------------------------------------------------------

// string.Trim()
std::string trim(const std::string& text) {
    const std::u16string s = util::utf16(text);
    std::size_t begin = 0, end = s.size();
    while (begin < end && util::is_white_space(s[begin])) ++begin;
    while (end > begin && util::is_white_space(s[end - 1])) --end;
    return util::utf8(s.substr(begin, end - begin));
}

bool is_null_or_white_space(const std::string& text) { return trim(text).empty(); }

stdfs::path to_path(const std::string& text) { return util::path::from_text(text); }

// Path.GetFullPath
std::string get_full_path(const std::string& path) {
    std::error_code ec;
    stdfs::path absolute = stdfs::absolute(to_path(path), ec);
    if (ec) return path;
    return util::path::native_text(absolute.lexically_normal());
}

std::string current_directory() {
    std::error_code ec;
    const stdfs::path current = stdfs::current_path(ec);
    return ec ? std::string() : util::path::native_text(current);
}

bool ends_with_ignore_case(const std::string& text, std::string_view suffix) {
    return text.size() >= suffix.size() && util::compare_ordinal_ignore_case(std::string_view(text).substr(text.size() - suffix.size()), suffix) == 0;
}

bool starts_with_ignore_case(const std::string& text, std::string_view prefix) {
    return text.size() >= prefix.size() && util::compare_ordinal_ignore_case(std::string_view(text).substr(0, prefix.size()), prefix) == 0;
}

std::string to_upper_invariant(const std::string& text) {
    std::u16string s = util::utf16(text);
    for (char16_t& c : s) c = c >= u'a' && c <= u'z' ? static_cast<char16_t>(c - u'a' + u'A') : c;
    return util::utf8(s);
}

// int.TryParse(text, NumberStyles.HexNumber, Invariant): white space around hex digits; the low 32 bits as a signed int.
bool try_parse_hex_int(std::string_view text, std::int32_t* value) {
    const auto is_space = [](char c) { return c == ' ' || (c >= '\t' && c <= '\r'); };
    while (!text.empty() && is_space(text.front())) text.remove_prefix(1);
    while (!text.empty() && is_space(text.back())) text.remove_suffix(1);
    if (text.empty()) return false;
    std::uint64_t v = 0;
    for (const char c : text) {
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
        else return false;
        v = v * 16 + static_cast<std::uint64_t>(digit);
        if (v > 0xFFFFFFFFull) return false;
    }
    *value = static_cast<std::int32_t>(static_cast<std::uint32_t>(v));
    return true;
}

// Raw deflate (System.IO.Compression.DeflateStream, decompressing).
Bytes inflate_raw(ByteView compressed) {
    z_stream stream{};
    if (inflateInit2(&stream, -MAX_WBITS) != Z_OK) throw InvalidDataError("The archive entry was compressed using an unsupported compression method.");
    stream.next_in = const_cast<Bytef*>(compressed.data());
    stream.avail_in = static_cast<uInt>(compressed.size());
    Bytes result;
    std::vector<Bytef> chunk(1 << 16);
    int status;
    do {
        stream.next_out = chunk.data();
        stream.avail_out = static_cast<uInt>(chunk.size());
        status = inflate(&stream, Z_NO_FLUSH);
        if (status != Z_OK && status != Z_STREAM_END) {
            inflateEnd(&stream);
            throw InvalidDataError("The archive entry was compressed using an unsupported compression method.");
        }
        result.insert(result.end(), chunk.data(), chunk.data() + (chunk.size() - stream.avail_out));
    } while (status != Z_STREAM_END && stream.avail_in > 0);
    inflateEnd(&stream);
    return result;
}

// The settings ILSpy keeps (the browsable ones of the decompiler's settings, by name: ILSpyX.Settings.DecompilerSettings.IsKnownOption).
struct KnownSetting {
    const char* name;
    bool DecompilerSettings::*member;
};

const std::vector<KnownSetting>& known_settings() {
    static const std::vector<KnownSetting> settings = {
#include "dotnet/decompiler/ilspycmd/known_settings.inc"
    };
    return settings;
}

// The text of an object's ToString() as ilspycmd writes a resource value that is no byte array.
std::string value_to_string(const util::ResourceValue& value) {
    struct Visitor {
        std::string operator()(const std::monostate&) const { return ""; }
        std::string operator()(const std::string& s) const { return s; }
        std::string operator()(bool b) const { return b ? "True" : "False"; }
        std::string operator()(char16_t c) const { return util::utf8(std::u16string(1, c)); }
        std::string operator()(std::uint8_t v) const { return std::to_string(v); }
        std::string operator()(std::int8_t v) const { return std::to_string(v); }
        std::string operator()(std::int16_t v) const { return std::to_string(v); }
        std::string operator()(std::uint16_t v) const { return std::to_string(v); }
        std::string operator()(std::int32_t v) const { return std::to_string(v); }
        std::string operator()(std::uint32_t v) const { return std::to_string(v); }
        std::string operator()(std::int64_t v) const { return std::to_string(v); }
        std::string operator()(std::uint64_t v) const { return std::to_string(v); }
        std::string operator()(float v) const { return util::format_r(v); }
        std::string operator()(double v) const { return util::format_r(v); }
        std::string operator()(const util::Decimal& v) const { return v.to_string(); }
        std::string operator()(const util::DateTime& v) const { return v.to_string(); }
        std::string operator()(const util::TimeSpan& v) const { return v.to_string(); }
        std::string operator()(const Bytes&) const { return ""; }
        std::string operator()(const util::ResourceStream&) const { return ""; }
        std::string operator()(const util::ResourceSerializedObject&) const { return ""; }
    };
    return std::visit(Visitor{}, value);
}

// Lets a UniversalAssemblyResolver answer as the classifier of the references of a project (it is one upstream).
class ResolverClassifier final : public metadata::IAssemblyReferenceClassifier {
public:
    explicit ResolverClassifier(metadata::UniversalAssemblyResolver& resolver) : resolver_(resolver) {}
    bool is_gac_assembly(const metadata::IAssemblyReference& reference) override {
        return metadata::UniversalAssemblyResolver::get_assembly_in_gac(reference).has_value();
    }
    bool is_shared_assembly(const metadata::IAssemblyReference& reference, std::string* runtime_pack) override {
        return resolver_.is_shared_assembly(reference, runtime_pack);
    }

private:
    metadata::UniversalAssemblyResolver& resolver_;
};

const char* type_kind_name(typesystem::TypeKind kind) {
    using K = typesystem::TypeKind;
    switch (kind) {
        case K::Other: return "Other";
        case K::Class: return "Class";
        case K::Interface: return "Interface";
        case K::Struct: return "Struct";
        case K::Delegate: return "Delegate";
        case K::Enum: return "Enum";
        case K::Void: return "Void";
        case K::Unknown: return "Unknown";
        case K::Null: return "Null";
        case K::None: return "None";
        case K::Dynamic: return "Dynamic";
        case K::UnboundTypeArgument: return "UnboundTypeArgument";
        case K::TypeParameter: return "TypeParameter";
        case K::Array: return "Array";
        case K::Pointer: return "Pointer";
        case K::ByReference: return "ByReference";
        case K::Intersection: return "Intersection";
        case K::ArgList: return "ArgList";
        case K::Tuple: return "Tuple";
        case K::ModOpt: return "ModOpt";
        case K::ModReq: return "ModReq";
        case K::NInt: return "NInt";
        case K::NUInt: return "NUInt";
        case K::FunctionPointer: return "FunctionPointer";
    }
    return "Other";
}

// ---- the program ----------------------------------------------------------------------------------------------------------------

class ILSpyCmdProgram {
public:
    ILSpyCmdProgram(std::ostream& out, std::ostream& err) : out_(out), err_(err), output_(&out) {}

    int run(std::span<const std::string> args);

private:
    // A decompiler with what it needs kept alive.
    struct DecompilerHandle {
        std::shared_ptr<metadata::PEFile> module;
        std::unique_ptr<metadata::UniversalAssemblyResolver> resolver;
        std::unique_ptr<DecompilerSettings> settings;
        std::unique_ptr<csharp::CSharpDecompiler> decompiler;
    };

    void err_line(const std::string& text) { err_ << text << kNewLine; }
    void output_line(const std::string& text) { *output_ << text << kNewLine; }

    // The command line: what McMaster does before OnExecuteAsync (the options' values, then the checks).
    bool read_options(const ParsedCommandLine& parsed);
    std::optional<std::string> validate(const ParsedCommandLine& parsed) const;

    int on_execute();
    int perform_per_file_action(const std::string& file_name, const std::optional<std::string>& output_directory);
    std::optional<std::string> resolve_output_directory(const std::optional<std::string>& output_directory) const;
    // File.CreateText(path) as the program's `output`.
    void create_output_file(const std::string& path);

    std::unique_ptr<DecompilerSettings> get_settings(const metadata::PEFile& module);
    std::unique_ptr<metadata::UniversalAssemblyResolver> make_resolver(const std::string& assembly_file_name, const metadata::PEFile& module);
    DecompilerHandle get_decompiler(const std::string& assembly_file_name);
    void note_debug_info_not_loaded();

    int list_content(const std::string& assembly_file_name, const std::set<typesystem::TypeKind>& kinds);
    int list_resources(const std::string& assembly_file_name);
    int extract_resource(const std::string& assembly_file_name, const std::string& resource_name, const std::optional<std::string>& output_directory);
    int show_il(const std::string& assembly_file_name);
    void report_decompilation_errors(const std::string& assembly_file_name, const std::vector<DecompilerError>& errors);
    int exit_code_for_decompilation_errors() const;
    solution::ProjectId decompile_as_project(const std::string& assembly_file_name, const std::string& project_file_name);
    int decompile(const std::string& assembly_file_name, const std::optional<std::string>& type_name);
    int decompile_member(const std::string& assembly_file_name, const std::string& id_or_token);
    static bool try_resolve_member(const typesystem::DecompilerTypeSystem& type_system, const std::string& id_or_token, srm::EntityHandle* handle, std::string* error);
    static bool try_resolve_type(const typesystem::DecompilerTypeSystem& type_system, const std::string& type_name, const typesystem::ITypeDefinition** type_definition,
                                 std::string* error);
    int generate_pdb_for_assembly(const std::string& assembly_file_name);
    int dump_package_assemblies(const std::string& package_file_name, const std::optional<std::string>& output_directory);

    std::ostream& out_;
    std::ostream& err_;
    std::ostream* output_;                       // upstream's `output` (Console.Out, or a file)
    std::unique_ptr<std::ofstream> output_file_;
    int decompilation_error_count_ = 0;
    bool debug_info_note_written_ = false;

    // the options (the properties of upstream's class)
    std::vector<std::string> input_assembly_names_;
    std::optional<std::string> output_directory_;
    bool create_compilable_project_flag_ = false;
    std::optional<std::string> type_name_;
    std::optional<std::string> member_id_string_;
    bool show_il_code_flag_ = false;
    bool show_il_sequence_points_flag_ = false;
    bool create_debug_info_flag_ = false;
    bool input_pdb_file_is_set_ = false;
    std::optional<std::string> input_pdb_file_value_;
    std::vector<std::string> entity_types_;
    bool list_resources_flag_ = false;
    std::optional<std::string> resource_name_;
    bool decompile_baml_flag_ = false;
    std::optional<std::string> dump_table_name_;
    bool json_output_flag_ = false;
    LanguageVersion language_version_ = LanguageVersion::Latest;
    std::optional<std::string> ilspy_settings_file_;
    std::vector<std::string> decompiler_setting_overrides_;
    std::vector<std::string> reference_paths_;
    bool ignore_decompilation_errors_flag_ = false;
    bool remove_dead_code_ = false;
    bool remove_dead_stores_ = false;
    bool dump_package_flag_ = false;
    bool nested_directories_ = false;
    bool generate_diagrammer_ = false;
};

bool ILSpyCmdProgram::read_options(const ParsedCommandLine& parsed) {
    const auto& o = parsed.options;
    const auto single = [&](Opt i) -> std::optional<std::string> {
        if (!o[i].is_set || o[i].values.empty()) return std::nullopt;
        return o[i].values[0];
    };
    input_assembly_names_ = parsed.arguments;
    output_directory_ = single(kOptOutputDir);
    create_compilable_project_flag_ = o[kOptProject].is_set;
    type_name_ = single(kOptType);
    member_id_string_ = single(kOptMember);
    show_il_code_flag_ = o[kOptIl].is_set;
    show_il_sequence_points_flag_ = o[kOptIlSequencePoints].is_set;
    create_debug_info_flag_ = o[kOptGenPdb].is_set;
    input_pdb_file_is_set_ = o[kOptUsePdb].is_set;
    input_pdb_file_value_ = single(kOptUsePdb);
    entity_types_ = o[kOptList].values;
    list_resources_flag_ = o[kOptListResources].is_set;
    resource_name_ = single(kOptResource);
    decompile_baml_flag_ = o[kOptDecompileBaml].is_set;
    dump_table_name_ = single(kOptDumpTable);
    json_output_flag_ = o[kOptJson].is_set;
    ilspy_settings_file_ = single(kOptSettingsFile);
    decompiler_setting_overrides_ = o[kOptDecompilerSetting].values;
    reference_paths_ = o[kOptReferencePath].values;
    ignore_decompilation_errors_flag_ = o[kOptIgnoreErrors].is_set;
    remove_dead_code_ = o[kOptNoDeadCode].is_set;
    remove_dead_stores_ = o[kOptNoDeadStores].is_set;
    dump_package_flag_ = o[kOptDumpPackage].is_set;
    nested_directories_ = o[kOptNestedDirectories].is_set;
    generate_diagrammer_ = o[kOptGenerateDiagrammer].is_set;

    if (const std::optional<std::string> version = single(kOptLanguageVersion)) {
        std::uint64_t value = 0;
        if (!enum_try_parse(enums::language_version(), *version, true, 32, true, &value)) {
            const std::string message =
                "Invalid value specified for languageversion. Allowed values are: CSharp1, CSharp2, CSharp3, CSharp4, CSharp5, CSharp6, CSharp7, CSharp7_1, "
                "CSharp7_2, CSharp7_3, CSharp8_0, CSharp9_0, CSharp10_0, CSharp11_0, CSharp12_0, CSharp13_0, CSharp14_0, CSharp15_0, Preview, Latest.";
            throw CommandParsingError(message, false, "System.FormatException: " + message);
        }
        language_version_ = static_cast<LanguageVersion>(static_cast<std::int32_t>(static_cast<std::uint32_t>(value)));
    }
    return true;
}

// The first thing wrong: the program as a whole, then the arguments, then the options in the order they are declared.
std::optional<std::string> ILSpyCmdProgram::validate(const ParsedCommandLine&) const {
    if (auto message = project_option_requires_output_directory(create_compilable_project_flag_, output_directory_)) return message;

    if (input_assembly_names_.empty()) return "The Assembly file name(s) field is required.";
    const std::string working_directory = current_directory();
    if (auto message = files_exist(input_assembly_names_, working_directory)) return message;

    if (input_pdb_file_is_set_)
        if (auto message = file_exists_or_null(input_pdb_file_value_, working_directory)) return message;
    if (ilspy_settings_file_)
        if (auto message = file_exists(*ilspy_settings_file_)) return message;
    for (const std::string& path : reference_paths_)
        if (auto message = directory_exists(path)) return message;
    return std::nullopt;
}

int ILSpyCmdProgram::run(std::span<const std::string> args) {
    static const std::vector<CommandOption> options = make_options();
    ParsedCommandLine parsed;
    try {
        parsed = parse_command_line(args, options);
    } catch (const CommandParsingError& error) {
        out_ << "Specify --help for a list of available options and commands." << kNewLine;
        err_ << error.unhandled_exception_text();
        return kUnhandledExceptionExitCode;
    }

    if (parsed.stopped_by == OptionRole::Version) {
        out_ << version_text() << kNewLine;
        return 0;
    }
    if (parsed.stopped_by == OptionRole::Help) {
        HelpInfo info;
        info.version_text = version_text();
        info.description = "dotnet tool for decompiling .NET assemblies and generating portable PDBs";
        info.usage = "Usage: ilspycmd [options] <Assembly file name(s)>";
        info.argument_name = "Assembly file name(s)";
        info.argument_description = "The list of assemblies that is being decompiled. This argument is mandatory.";
        info.extended_help_text = extended_help_text_crlf();
        out_ << generate_help_text(info, options);
        return 0;
    }

    try {
        read_options(parsed);
    } catch (const CommandParsingError& error) {
        err_ << error.unhandled_exception_text();
        return kUnhandledExceptionExitCode;
    }

    if (const std::optional<std::string> message = validate(parsed)) {
        err_ << *message << kNewLine;
        out_ << "Specify --help for a list of available options and commands." << kNewLine;
        return 1;
    }

    int exit_code;
    try {
        exit_code = on_execute();
    } catch (const std::exception& ex) {
        err_ << "Unhandled exception. " << ex.what() << kNewLine;
        exit_code = kUnhandledExceptionExitCode;
    }
    out_.flush();
    err_.flush();
    return exit_code;
}

std::optional<std::string> ILSpyCmdProgram::resolve_output_directory(const std::optional<std::string>& output_directory) const {
    // path is not set
    if (!output_directory || is_null_or_white_space(*output_directory)) return std::nullopt;
    // resolve relative path, backreferences ('.' and '..') and other platform-specific path elements
    return get_full_path(*output_directory);
}

void ILSpyCmdProgram::create_output_file(const std::string& path) {
    output_file_.reset();
    output_file_ = std::make_unique<std::ofstream>(to_path(path), std::ios::binary | std::ios::trunc);
    if (!*output_file_) throw IoError(to_path(path), 0, "cannot create the file");
    output_ = output_file_.get();
}

int ILSpyCmdProgram::on_execute() {
    // (the check for a newer version of the tool is not made)
    output_ = &out_;
    const std::optional<std::string> output_directory = resolve_output_directory(output_directory_);

    if (output_directory) stdfs::create_directories(to_path(*output_directory));

    if (json_output_flag_ && !dump_table_name_) {
        err_line("The --json option is currently only supported together with --dump-table.");
        return ProgramExitCodes::ex_usage;
    }

    try {
        int exit_code;
        if (create_compilable_project_flag_) {
            // (a blank --outputdir passes the check of the command line and is no directory: Path.Combine(null, ...) throws)
            if (!output_directory) throw UsageError("Value cannot be null. (Parameter 'path1')");
            if (input_assembly_names_.size() == 1) {
                const std::string project_file_name =
                    util::path::combine(*output_directory, util::path::get_file_name_without_extension(input_assembly_names_[0]) + ".csproj");
                decompile_as_project(input_assembly_names_[0], project_file_name);
                exit_code = exit_code_for_decompilation_errors();
            } else {
                std::vector<SolutionProject> projects;
                for (const std::string& file : input_assembly_names_) {
                    const std::string project_file_name =
                        util::path::combine(*output_directory, util::path::get_file_name_without_extension(file), util::path::get_file_name_without_extension(file) + ".csproj");
                    stdfs::create_directories(to_path(*util::path::get_directory_name(project_file_name)));
                    const solution::ProjectId project_id = decompile_as_project(file, project_file_name);
                    projects.push_back(SolutionProject{project_file_name, project_id.platform_name(), project_id.guid(), project_id.type_guid()});
                }
                write_solution_file(util::path::combine(*output_directory, util::path::get_file_name_without_extension(*output_directory) + ".sln"), projects);
                exit_code = exit_code_for_decompilation_errors();
            }
        } else if (generate_diagrammer_) {
            err_line("The HTML diagrammer (--generate-diagrammer) is not supported by this build: it is not ported.");
            exit_code = ProgramExitCodes::ex_software;
        } else {
            exit_code = 0;
            bool failed = false;
            for (const std::string& file : input_assembly_names_) {
                const int result = perform_per_file_action(file, output_directory);
                if (result != 0) {
                    exit_code = result;
                    failed = true;
                    break;
                }
            }
            if (!failed) exit_code = exit_code_for_decompilation_errors();
        }
        output_->flush();
        output_file_.reset();
        return exit_code;
    } catch (const std::exception& ex) {
        output_file_.reset();
        err_line(std::string(ex.what()));
        return ProgramExitCodes::ex_software;
    }
}

int ILSpyCmdProgram::perform_per_file_action(const std::string& file_name, const std::optional<std::string>& output_directory) {
    if (!entity_types_.empty()) {
        std::vector<std::string> values;
        for (const std::string& v : entity_types_) {
            std::size_t start = 0;
            while (true) {
                const std::size_t split = v.find_first_of(",;", start);
                values.push_back(v.substr(start, split == std::string::npos ? std::string::npos : split - start));
                if (split == std::string::npos) break;
                start = split + 1;
            }
        }
        const std::set<typesystem::TypeKind> kinds = TypesParser::parse_selection(values);
        if (output_directory) {
            const std::string output_name = util::path::get_file_name_without_extension(file_name);
            create_output_file(util::path::combine(*output_directory, output_name) + ".list.txt");
        }
        return list_content(file_name, kinds);
    }
    if (show_il_code_flag_ || show_il_sequence_points_flag_) {
        if (output_directory) {
            const std::string output_name = util::path::get_file_name_without_extension(file_name);
            create_output_file(util::path::combine(*output_directory, output_name) + ".il");
        }
        return show_il(file_name);
    }
    if (create_debug_info_flag_) return generate_pdb_for_assembly(file_name);
    if (dump_package_flag_) return dump_package_assemblies(file_name, output_directory);
    if (list_resources_flag_) {
        if (output_directory) {
            const std::string output_name = util::path::get_file_name_without_extension(file_name);
            create_output_file(util::path::combine(*output_directory, output_name) + ".resources.txt");
        }
        return list_resources(file_name);
    }
    if (resource_name_) return extract_resource(file_name, *resource_name_, output_directory);
    if (dump_table_name_) {
        srm::TableIndex table;
        if (!MetadataTableDumper::try_parse_table_name(*dump_table_name_, &table)) {
            err_line("Unknown metadata table '" + *dump_table_name_ + "'.");
            err_line("Supported tables: " + MetadataTableDumper::supported_table_names());
            return ProgramExitCodes::ex_usage;
        }
        if (output_directory) {
            // per-file writer, disposed here
            const std::string output_name = util::path::get_file_name_without_extension(file_name);
            const std::string path = util::path::combine(*output_directory, output_name) + "." + MetadataTableDumper::table_to_string(table) + (json_output_flag_ ? ".json" : ".txt");
            std::ofstream table_output(to_path(path), std::ios::binary | std::ios::trunc);
            if (!table_output) throw IoError(to_path(path), 0, "cannot create the file");
            return MetadataTableDumper::dump_table(file_name, table_output, table, json_output_flag_);
        }
        return MetadataTableDumper::dump_table(file_name, *output_, table, json_output_flag_);
    }

    if (member_id_string_ && type_name_) {
        err_line("The --type and --member options are mutually exclusive.");
        return ProgramExitCodes::ex_usage;
    }

    if (output_directory) {
        const std::string output_name = util::path::get_file_name_without_extension(file_name);
        create_output_file(util::path::combine(*output_directory, (!type_name_ || type_name_->empty()) ? output_name : *type_name_) + ".decompiled.cs");
    }

    if (member_id_string_) return decompile_member(file_name, *member_id_string_);
    return decompile(file_name, type_name_);
}

// ---- settings and decompilers ---------------------------------------------------------------------------------------------------

// XmlConvert.ToBoolean of the attribute's value in lower case: "true", "1", "false", "0" (white space around is ignored).
bool xml_attribute_to_bool(const std::string& value) {
    std::string s = to_upper_invariant(value);
    for (char& c : s) c = c >= 'A' && c <= 'Z' ? static_cast<char>(c - 'A' + 'a') : c;
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t' || s.front() == '\r' || s.front() == '\n')) s.erase(s.begin());
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r' || s.back() == '\n')) s.pop_back();
    if (s == "1" || s == "true") return true;
    if (s == "0" || s == "false") return false;
    throw UsageError("The string '" + s + "' is not a valid Boolean value.");
}

// ILSpyX: the settings of the file's <DecompilerSettings> element (a file that is not XML, or cannot be read, holds none).
std::unique_ptr<DecompilerSettings> load_ilspy_settings(const std::string& file_name) {
    auto settings = std::make_unique<DecompilerSettings>();
    std::string text;
    try {
        text = fs::read_text(to_path(file_name));
    } catch (const IoError&) {
        return settings;
    }
    util::XDocument document;
    try {
        document = util::XDocument::parse(text);
    } catch (const util::XmlError&) {
        return settings;
    }
    const util::XElement* root = document.root();
    const util::XElement* section = root != nullptr ? root->element("DecompilerSettings") : nullptr;
    if (section == nullptr) return settings;
    for (const KnownSetting& setting : known_settings())
        if (const util::XAttribute* attribute = section->attribute(setting.name)) (*settings).*setting.member = xml_attribute_to_bool(attribute->value());
    return settings;
}

std::unique_ptr<DecompilerSettings> ILSpyCmdProgram::get_settings(const metadata::PEFile& module) {
    std::unique_ptr<DecompilerSettings> decompiler_settings;

    if (ilspy_settings_file_) {
        try {
            decompiler_settings = load_ilspy_settings(*ilspy_settings_file_);
        } catch (const std::exception& ex) {
            err_line("Error loading ILSpy settings file '" + *ilspy_settings_file_ + "': " + ex.what());
        }
    }

    if (!decompiler_settings) {
        decompiler_settings = std::make_unique<DecompilerSettings>(language_version_);
        decompiler_settings->throw_on_assembly_resolve_errors = false;
        decompiler_settings->remove_dead_code = remove_dead_code_;
        decompiler_settings->remove_dead_stores = remove_dead_stores_;
        decompiler_settings->use_sdk_style_project_format = csharp::project_decompiler::WholeProjectDecompiler::can_use_sdk_style_project_format(module);
        decompiler_settings->use_nested_directories_for_namespaces = nested_directories_;
    }

    for (const std::string& entry : decompiler_setting_overrides_) {
        const std::size_t equals = entry.find('=');
        if (equals == std::string::npos || equals == 0) {
            err_line("Decompiler setting '" + entry + "' is invalid; use '<Name>=<Value>'");
            continue;
        }

        const std::string name = trim(entry.substr(0, equals));
        const std::string value = trim(entry.substr(equals + 1));

        const auto& settings = known_settings();
        const auto known = std::find_if(settings.begin(), settings.end(), [&](const KnownSetting& s) { return name == s.name; });
        if (known == settings.end()) {
            err_line("Decompiler setting '" + name + "' is unknown.");
            continue;
        }

        // Convert.ChangeType(value, typeof(bool)): Boolean.Parse, which ignores case and white space around and accepts only True and False.
        const std::string lower = [&] {
            std::string s = value;
            for (char& c : s) c = c >= 'A' && c <= 'Z' ? static_cast<char>(c - 'A' + 'a') : c;
            return s;
        }();
        if (lower != "true" && lower != "false") {
            err_line("Decompiler setting '" + name + "': Value '" + value + "' could not be converted to 'System.Boolean'.");
            continue;
        }
        (*decompiler_settings).*(known->member) = lower == "true";
    }

    return decompiler_settings;
}

std::unique_ptr<metadata::UniversalAssemblyResolver> ILSpyCmdProgram::make_resolver(const std::string& assembly_file_name, const metadata::PEFile& module) {
    auto resolver = std::make_unique<metadata::UniversalAssemblyResolver>(to_path(assembly_file_name), false, metadata::detect_target_framework_id(module));
    for (const std::string& path : reference_paths_) resolver->add_search_directory(to_path(path));
    return resolver;
}

// Debug information (a PDB) is not read: the decompiler gets none, as when no PDB is asked for.
void ILSpyCmdProgram::note_debug_info_not_loaded() {
    if (!input_pdb_file_is_set_ || debug_info_note_written_) return;
    debug_info_note_written_ = true;
    err_line("Reading a PDB file is not supported by this build: --use-varnames-from-pdb is ignored.");
}

ILSpyCmdProgram::DecompilerHandle ILSpyCmdProgram::get_decompiler(const std::string& assembly_file_name) {
    DecompilerHandle handle;
    handle.module = metadata::PEFile::open(to_path(assembly_file_name));
    handle.resolver = make_resolver(assembly_file_name, *handle.module);
    handle.settings = get_settings(*handle.module);
    note_debug_info_not_loaded();
    handle.decompiler = std::make_unique<csharp::CSharpDecompiler>(to_path(assembly_file_name), *handle.resolver, *handle.settings);
    return handle;
}

// ---- the actions ------------------------------------------------------------------------------------------------------------------

int ILSpyCmdProgram::list_content(const std::string& assembly_file_name, const std::set<typesystem::TypeKind>& kinds) {
    DecompilerHandle handle = get_decompiler(assembly_file_name);

    for (const typesystem::ITypeDefinition* type : handle.decompiler->type_system().main_module().type_definitions()) {
        if (!kinds.contains(type->kind())) continue;
        output_line(std::string(type_kind_name(type->kind())) + " " + type->full_type_name().reflection_name());
    }
    return 0;
}

int ILSpyCmdProgram::list_resources(const std::string& assembly_file_name) {
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(to_path(assembly_file_name));
    for (const std::string& path : ResourceExtensions::enumerate_resource_paths(*module)) output_line(path);
    return 0;
}

int ILSpyCmdProgram::extract_resource(const std::string& assembly_file_name, const std::string& resource_name, const std::optional<std::string>& output_directory) {
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(to_path(assembly_file_name));
    util::ResourceValue value;
    if (!ResourceExtensions::try_get_resource(*module, resource_name, &value)) {
        err_line("Resource '" + resource_name + "' not found.");
        err_line("Available resources:");
        for (const std::string& p : ResourceExtensions::enumerate_resource_paths(*module)) err_line("  " + p);
        return ProgramExitCodes::ex_dataerr;
    }

    const bool is_baml = ends_with_ignore_case(resource_name, ".baml");
    if (is_baml && std::holds_alternative<Bytes>(value)) {
        err_line("The BAML resource '" + resource_name + "' cannot be decompiled to XAML: the BAML decompiler is not ported to this build.");
        return ProgramExitCodes::ex_software;
    }

    if (const Bytes* binary = std::get_if<Bytes>(&value)) {
        if (output_directory) {
            const std::string file_name = csharp::project_decompiler::WholeProjectDecompiler::sanitize_file_name(util::path::get_file_name(resource_name));
            const std::string full_path = util::path::combine(*output_directory, file_name);
            fs::write_file(to_path(full_path), ByteView(*binary));
        } else {
            output_->write(reinterpret_cast<const char*>(binary->data()), static_cast<std::streamsize>(binary->size()));
            output_->flush();
        }
    } else {
        const std::string text = value_to_string(value);
        if (output_directory) {
            const std::string file_name = csharp::project_decompiler::WholeProjectDecompiler::sanitize_file_name(util::path::get_file_name(resource_name));
            const std::string full_path = util::path::combine(*output_directory, file_name);
            fs::write_text(to_path(full_path), text);
        } else {
            *output_ << text;
        }
    }
    return 0;
}

int ILSpyCmdProgram::show_il(const std::string& assembly_file_name) {
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(to_path(assembly_file_name));
    note_debug_info_not_loaded();
    *output_ << disassembler::show_il(*module);
    return 0;
}

void ILSpyCmdProgram::report_decompilation_errors(const std::string& assembly_file_name, const std::vector<DecompilerError>& errors) {
    if (errors.empty()) return;
    decompilation_error_count_ += static_cast<int>(errors.size());
    err_line("While decompiling " + assembly_file_name + ":");
    for (const std::string& line : csharp::CSharpDecompiler::get_error_summary_lines(static_cast<int>(errors.size()))) err_line(line);
    for (const DecompilerError& error : errors) err_line("  " + csharp::CSharpDecompiler::get_error_headline(error));
}

int ILSpyCmdProgram::exit_code_for_decompilation_errors() const {
    return decompilation_error_count_ == 0 || ignore_decompilation_errors_flag_ ? 0 : ProgramExitCodes::ex_software;
}

solution::ProjectId ILSpyCmdProgram::decompile_as_project(const std::string& assembly_file_name, const std::string& project_file_name) {
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(to_path(assembly_file_name));
    const std::unique_ptr<metadata::UniversalAssemblyResolver> resolver = make_resolver(assembly_file_name, *module);
    const std::unique_ptr<DecompilerSettings> settings = get_settings(*module);
    note_debug_info_not_loaded();
    if (decompile_baml_flag_)
        err_line("--decompile-baml is not supported by this build (the BAML decompiler is not ported): BAML resources stay raw byte streams.");
    ResolverClassifier classifier(*resolver);
    csharp::project_decompiler::WholeProjectDecompiler decompiler(*settings, *resolver, nullptr, &classifier, nullptr);
    util::StreamWriter project_file_writer(to_path(project_file_name));
    const std::string directory = util::path::get_directory_name(project_file_name).value_or("");
    const solution::ProjectId project_id = decompiler.decompile_project(module, directory, project_file_writer);
    project_file_writer.close();
    report_decompilation_errors(assembly_file_name, decompiler.errors());
    return project_id;
}

int ILSpyCmdProgram::decompile(const std::string& assembly_file_name, const std::optional<std::string>& type_name) {
    DecompilerHandle handle = get_decompiler(assembly_file_name);
    csharp::CSharpDecompiler& decompiler = *handle.decompiler;

    if (!type_name) {
        *output_ << decompiler.decompile_whole_module_as_string();
        report_decompilation_errors(assembly_file_name, decompiler.errors());
        return 0;
    }

    const typesystem::ITypeDefinition* type_definition = nullptr;
    std::string error;
    if (!try_resolve_type(decompiler.type_system(), *type_name, &type_definition, &error)) {
        err_line(error);
        return ProgramExitCodes::ex_dataerr;
    }

    *output_ << decompiler.decompile_type_as_string(type_definition->full_type_name());
    report_decompilation_errors(assembly_file_name, decompiler.errors());
    return 0;
}

int ILSpyCmdProgram::decompile_member(const std::string& assembly_file_name, const std::string& id_or_token) {
    DecompilerHandle handle = get_decompiler(assembly_file_name);
    csharp::CSharpDecompiler& decompiler = *handle.decompiler;

    srm::EntityHandle entity_handle;
    std::string error;
    if (!try_resolve_member(decompiler.type_system(), id_or_token, &entity_handle, &error)) {
        err_line(error);
        return ProgramExitCodes::ex_dataerr;
    }

    *output_ << decompiler.decompile_as_string(std::vector<srm::EntityHandle>{entity_handle});
    report_decompilation_errors(assembly_file_name, decompiler.errors());
    return 0;
}

// Resolves a member reference supplied on the command line: either an XML documentation id string ("M:...", "P:...", "F:...", "E:..." or "T:...") or a
// metadata token in hexadecimal "0x06000005" form.
bool ILSpyCmdProgram::try_resolve_member(const typesystem::DecompilerTypeSystem& type_system, const std::string& id_or_token, srm::EntityHandle* handle,
                                         std::string* error) {
    *handle = srm::EntityHandle();
    error->clear();
    const std::string trimmed = trim(id_or_token);

    if (starts_with_ignore_case(trimmed, "0x")) {
        std::int32_t token_value;
        if (!try_parse_hex_int(std::string_view(trimmed).substr(2), &token_value)) {
            *error = "'" + trimmed + "' is not a valid metadata token; expected a hexadecimal value like 0x06000005.";
            return false;
        }
        const srm::MetadataReader& metadata = type_system.main_module().metadata_file()->metadata();
        const srm::EntityHandle candidate = srm::entity_handle(static_cast<std::uint32_t>(token_value));
        const std::int32_t row_number = token_value & 0x00ffffff;
        std::int32_t row_count = 0;
        switch (candidate.kind()) {
            case srm::HandleKind::TypeDefinition: row_count = static_cast<std::int32_t>(metadata.type_definitions().size()); break;
            case srm::HandleKind::FieldDefinition: row_count = static_cast<std::int32_t>(metadata.field_definitions().size()); break;
            case srm::HandleKind::MethodDefinition: row_count = static_cast<std::int32_t>(metadata.method_definitions().size()); break;
            case srm::HandleKind::PropertyDefinition: row_count = static_cast<std::int32_t>(metadata.property_definitions().size()); break;
            case srm::HandleKind::EventDefinition: row_count = static_cast<std::int32_t>(metadata.event_definitions().size()); break;
            default: row_count = 0; break;
        }
        if (row_number < 1 || row_number > row_count) {
            *error = "Metadata token " + trimmed + " does not reference a type or member of this module.";
            return false;
        }
        *handle = candidate;
        return true;
    }

    const typesystem::IEntity* entity;
    try {
        entity = documentation::id_string_provider::find_entity(trimmed, typesystem::SimpleTypeResolveContext(type_system.main_module()));
    } catch (const typesystem::ReflectionNameParseError& ex) {
        *error = "'" + trimmed + "' is not a valid documentation id string: " + ex.what();
        return false;
    }
    if (entity == nullptr || entity->metadata_token().is_nil()) {
        *error = "Member '" + trimmed +
                 "' was not found in this module. Expected an XML documentation id string (e.g. \"M:System.String.Concat(System.String,System.String)\") or a "
                 "metadata token (e.g. 0x06000005).";
        return false;
    }
    if (entity->parent_module() != &type_system.main_module()) {
        *error = "Member '" + trimmed + "' is defined in '" + (entity->parent_module() != nullptr ? entity->parent_module()->assembly_name() : std::string()) +
                 "', not in this module.";
        return false;
    }
    *handle = entity->metadata_token();
    return true;
}

// ---- finding a type by the name given on the command line -------------------------------------------------------------------------

// Whether `suffix_segments` equals the trailing run of '.'-separated segments of `full_name`.
bool is_trailing_segment_path(const std::string& full_name, const std::vector<std::string>& suffix_segments) {
    const auto split = [](const std::string& text) {
        std::vector<std::string> parts;
        std::size_t start = 0;
        while (true) {
            const std::size_t dot = text.find('.', start);
            parts.push_back(text.substr(start, dot == std::string::npos ? std::string::npos : dot - start));
            if (dot == std::string::npos) break;
            start = dot + 1;
        }
        return parts;
    };
    const std::vector<std::string> segments = split(full_name);
    if (suffix_segments.size() > segments.size()) return false;
    const std::size_t offset = segments.size() - suffix_segments.size();
    for (std::size_t i = 0; i < suffix_segments.size(); ++i)
        if (segments[offset + i] != suffix_segments[i]) return false;
    return true;
}

// Parses a type name in any reflection-grammar spelling and reduces it to the shape of INamedElement.FullName.
bool try_normalize_type_name(const std::string& type_name, std::string* normalized) {
    normalized->clear();
    std::shared_ptr<const srm::TypeName> parsed_holder = srm::TypeName::try_parse(type_name);
    if (!parsed_holder) return false;
    const srm::TypeName* parsed = parsed_holder.get();

    // Peel decorations down to the underlying type definition: arrays, pointers and byrefs expose an element type, and a constructed generic
    // exposes its open definition.
    while (parsed->is_array() || parsed->is_pointer() || parsed->is_by_ref()) parsed = &parsed->get_element_type();
    if (parsed->is_constructed_generic_type()) parsed = &parsed->get_generic_type_definition();

    // parsed->full_name() is now free of assembly and generic-argument noise; only the arity suffixes and '+' separators still differ.
    const std::string& reflection_name = parsed->full_name();
    std::string result;
    std::size_t i = 0;
    while (i < reflection_name.size()) {
        const char c = reflection_name[i];
        if (c == '`') {
            ++i;
            while (i < reflection_name.size() && reflection_name[i] >= '0' && reflection_name[i] <= '9') ++i;
        } else {
            result += c == '+' ? '.' : c;
            ++i;
        }
    }
    *normalized = result;
    return true;
}

// The single type definition matching the predicate. Sets `error` when it matches more than one, so the caller can report the ambiguity.
template <class Predicate>
bool try_single_match(const std::vector<const typesystem::ITypeDefinition*>& all_types, Predicate predicate, const std::string& type_name,
                      const typesystem::ITypeDefinition** type_definition, std::string* error) {
    *type_definition = nullptr;
    error->clear();
    std::vector<const typesystem::ITypeDefinition*> matches;
    for (const typesystem::ITypeDefinition* t : all_types)
        if (predicate(*t)) matches.push_back(t);
    if (matches.size() == 1) {
        *type_definition = matches[0];
        return true;
    }
    if (matches.size() > 1) {
        *error = "The type name '" + type_name + "' is ambiguous between:" + kNewLine;
        for (std::size_t i = 0; i < matches.size(); ++i) {
            if (i != 0) *error += kNewLine;
            *error += "    " + matches[i]->full_type_name().reflection_name();
        }
        *error += std::string(kNewLine) + "Specify the full reflection name (including the `n generic-arity suffix).";
    }
    return false;
}

// Whether every character of `value` appears in `text` in order (case-insensitively), allowing gaps.
bool is_subsequence(const std::string& value, const std::string& text) {
    const std::u16string v = util::utf16(to_upper_invariant(value));
    const std::u16string t = util::utf16(to_upper_invariant(text));
    std::size_t i = 0;
    for (const char16_t c : t)
        if (i < v.size() && c == v[i]) ++i;
    return i == v.size();
}

std::vector<std::string> select_suggestions(const std::vector<const typesystem::ITypeDefinition*>& all_types,
                                            const std::function<bool(const typesystem::ITypeDefinition&)>& predicate) {
    std::vector<std::string> names;
    for (const typesystem::ITypeDefinition* t : all_types)
        if (predicate(*t)) names.push_back(t->full_type_name().reflection_name());
    std::vector<std::string> distinct;
    for (const std::string& n : names)
        if (std::find(distinct.begin(), distinct.end(), n) == distinct.end()) distinct.push_back(n);
    std::stable_sort(distinct.begin(), distinct.end(), util::OrdinalIgnoreCaseLess());
    if (distinct.size() > 10) distinct.resize(10);
    return distinct;
}

std::string format_not_found(const std::string& type_name, const std::vector<const typesystem::ITypeDefinition*>& all_types) {
    std::string message = "Could not find a type named '" + type_name + "'.";

    // Suggest types whose name contains the requested simple name (case-insensitive).
    const std::size_t last_dot = type_name.rfind('.');
    std::string simple_name = last_dot != std::string::npos ? type_name.substr(last_dot + 1) : type_name;
    const std::size_t backtick = simple_name.find('`');
    if (backtick != std::string::npos) simple_name = simple_name.substr(0, backtick);

    // Prefer types whose simple name contains the requested name; if a typo means nothing contains it, fall back to a subsequence match.
    const std::string upper_simple_name = to_upper_invariant(simple_name);
    std::vector<std::string> suggestions = select_suggestions(
        all_types, [&](const typesystem::ITypeDefinition& t) { return to_upper_invariant(t.name()).find(upper_simple_name) != std::string::npos; });
    if (suggestions.empty()) suggestions = select_suggestions(all_types, [&](const typesystem::ITypeDefinition& t) { return is_subsequence(simple_name, t.name()); });

    if (!suggestions.empty()) {
        message += std::string(kNewLine) + "Did you mean one of the following?" + kNewLine;
        for (std::size_t i = 0; i < suggestions.size(); ++i) {
            if (i != 0) message += kNewLine;
            message += "    " + suggestions[i];
        }
    }
    return message;
}

// Resolves a type name supplied on the command line to a single type definition: a ladder of progressively looser rules, each requiring a
// unique hit (the exact reflection name; the FullName shape of the input; that without case; the simple name; a trailing path of segments).
bool ILSpyCmdProgram::try_resolve_type(const typesystem::DecompilerTypeSystem& type_system, const std::string& type_name,
                                       const typesystem::ITypeDefinition** type_definition, std::string* error) {
    *type_definition = nullptr;
    error->clear();

    // Exact match on the reflection name. This is the canonical form printed by --list-* (e.g. "Ns.List`1") and the only form that resolves generic types
    // directly.
    const typesystem::IType* exact_type = typesystem::find_type(type_system, typesystem::FullTypeName(type_name));
    const typesystem::ITypeDefinition* exact = exact_type != nullptr ? exact_type->get_definition() : nullptr;
    if (exact != nullptr) {
        *type_definition = exact;
        return true;
    }

    const std::vector<const typesystem::ITypeDefinition*> all_types = type_system.main_module().type_definitions();

    // Reduce whatever spelling the user gave to the shape of ITypeDefinition.FullName; if the input is not a well-formed type name we match against it
    // as-is.
    std::string parsed;
    const std::string normalized = try_normalize_type_name(type_name, &parsed) ? parsed : type_name;

    // Arity- and separator-insensitive: "Ns.CachedPsiValue" finds "Ns.CachedPsiValue`1", and any spelling of a nested generic finds it.
    if (try_single_match(all_types, [&](const typesystem::ITypeDefinition& t) { return t.full_name() == normalized; }, type_name, type_definition, error)) return true;
    if (!error->empty()) return false;

    // Case-insensitive variant of the same.
    if (try_single_match(all_types, [&](const typesystem::ITypeDefinition& t) { return util::compare_ordinal_ignore_case(t.full_name(), normalized) == 0; },
                         type_name, type_definition, error))
        return true;
    if (!error->empty()) return false;

    // Simple name only, with the namespace omitted (e.g. "CachedPsiValue").
    if (try_single_match(all_types, [&](const typesystem::ITypeDefinition& t) { return util::compare_ordinal_ignore_case(t.name(), normalized) == 0; },
                         type_name, type_definition, error))
        return true;
    if (!error->empty()) return false;

    // Trailing segment path: "Dictionary.KeyCollection" finds "System.Collections.Generic.Dictionary`2+KeyCollection". Matching whole '.'-separated
    // segments keeps "ReadOnlyDictionary.KeyCollection" from being treated as a match.
    std::vector<std::string> suffix_segments;
    {
        std::size_t start = 0;
        while (true) {
            const std::size_t dot = normalized.find('.', start);
            suffix_segments.push_back(normalized.substr(start, dot == std::string::npos ? std::string::npos : dot - start));
            if (dot == std::string::npos) break;
            start = dot + 1;
        }
    }
    if (try_single_match(all_types, [&](const typesystem::ITypeDefinition& t) { return is_trailing_segment_path(t.full_name(), suffix_segments); }, type_name,
                         type_definition, error))
        return true;
    if (!error->empty()) return false;

    *error = format_not_found(type_name, all_types);
    return false;
}

int ILSpyCmdProgram::generate_pdb_for_assembly(const std::string& assembly_file_name) {
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(to_path(assembly_file_name));

    // PortablePdbWriter.HasCodeViewDebugDirectoryEntry: a debug directory entry of type CodeView (2).
    const std::vector<formats::pe::DebugEntry> entries = module->reader().pe_image().debug_entries();
    const bool has_code_view = std::any_of(entries.begin(), entries.end(), [](const formats::pe::DebugEntry& e) { return e.type == 2; });
    if (!has_code_view) {
        err_line("Cannot create PDB file for " + assembly_file_name + ", because it does not contain a PE Debug Directory Entry of type 'CodeView'.");
        return ProgramExitCodes::ex_dataerr;
    }

    err_line("Generating a portable PDB (-genpdb) is not supported by this build: the PDB writer is not ported.");
    return ProgramExitCodes::ex_software;
}

int ILSpyCmdProgram::dump_package_assemblies(const std::string& package_file_name, const std::optional<std::string>& output_directory) {
    const Bytes package = fs::read_file(to_path(package_file_name));
    std::int64_t bundle_header_offset = 0;
    if (!SingleFileBundle::is_bundle(ByteView(package), &bundle_header_offset)) {
        err_line("Cannot dump assembiles for " + package_file_name + ", because it is not a single file bundle.");
        return ProgramExitCodes::ex_dataerr;
    }

    const SingleFileBundle::Header manifest = SingleFileBundle::read_manifest(ByteView(package), bundle_header_offset);
    for (const SingleFileBundle::Entry& entry : manifest.entries) {
        std::string slashed = entry.relative_path;
        std::replace(slashed.begin(), slashed.end(), '\\', '/');
        if (slashed.find("../") != std::string::npos || util::path::is_path_rooted(entry.relative_path)) {
            err_line("Skipping single-file entry '" + entry.relative_path + "' because it might refer to a location outside of the bundle output directory.");
            continue;
        }

        Bytes contents;
        const auto size = static_cast<std::uint64_t>(entry.size);
        const auto stored = static_cast<std::uint64_t>(entry.compressed_size == 0 ? entry.size : entry.compressed_size);
        if (entry.offset < 0 || entry.size < 0 || entry.compressed_size < 0 || static_cast<std::uint64_t>(entry.offset) + stored > package.size())
            throw InvalidDataError("The entry '" + entry.relative_path + "' lies outside of the bundle.");
        const ByteView stored_bytes(package.data() + entry.offset, static_cast<std::size_t>(stored));
        if (entry.compressed_size == 0) {
            contents.assign(stored_bytes.begin(), stored_bytes.end());
        } else {
            contents = inflate_raw(stored_bytes);
            if (contents.size() != size) {
                err_line("Corrupted single-file entry '" + entry.relative_path + "'. Declared decompressed size '" + std::to_string(entry.size) +
                         "' is not the same as actual decompressed size '" + std::to_string(contents.size()) + "'.");
                return ProgramExitCodes::ex_dataerr;
            }
        }

        if (!output_directory) throw UsageError("Value cannot be null. (Parameter 'path1')");
        const std::string target = util::path::combine(*output_directory, entry.relative_path);
        if (const std::optional<std::string> directory = util::path::get_directory_name(target)) stdfs::create_directories(to_path(*directory));
        fs::write_file(to_path(target), ByteView(contents));
    }

    return 0;
}

}  // namespace

int run_ilspycmd(std::span<const std::string> args, std::ostream& out, std::ostream& err) {
    ILSpyCmdProgram program(out, err);
    return program.run(args);
}

}  // namespace cb::dotnet::decompiler::ilspycmd
