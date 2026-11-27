// Tests of dotnet/decompiler's ilspycmd (ICSharpCode.ILSpyCmd): the command line, the help text, the listings, the IL, a type and a member
// decompiled, the metadata table dump, and the messages of every kind of mistake, against what the installed ilspycmd 11.0 writes for the
// test assembly (testdata/inputs/Sample.dll). Each golden (testdata/golden/ilspycmd/<case>.txt) holds the exit code, the standard output and the
// standard error, as: "exit=N", "--stdout--", the output, "--stderr--", the error output; the path of the assembly is {INPUT} and the current
// directory {CWD}. The stack trace of an unhandled .NET exception is not part of a golden (the port does not write one).
// work/ilspy-port/il4/IC/make_goldens.ps1 remakes the goldens with the installed tool (CB_REGOLD=1 rewrites them from this port's output instead).
#include <filesystem>
#include <fstream>
#include <sstream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "dotnet/decompiler/ilspycmd/command_line.hpp"
#include "dotnet/decompiler/ilspycmd/dotnet_enum.hpp"
#include "dotnet/decompiler/ilspycmd/ilspycmd_program.hpp"
#include "dotnet/decompiler/ilspycmd/metadata_table_dumper.hpp"
#include "dotnet/decompiler/ilspycmd/types_parser.hpp"
#include "dotnet/decompiler/util/dotnet_path.hpp"

namespace cb::dotnet::decompiler::ilspycmd {
namespace {

std::filesystem::path testdata(std::string_view relative) { return testing::data_path(std::string("dotnet/testdata/") + std::string(relative)); }

std::string input_path() { return fs::to_text(testdata("inputs/Sample.dll")); }

std::string replace_all(std::string text, const std::string& from, const std::string& to) {
    if (from.empty()) return text;
    for (std::size_t pos = 0; (pos = text.find(from, pos)) != std::string::npos; pos += to.size()) text.replace(pos, from.size(), to);
    return text;
}

// The first line where two texts differ, for the failure message.
std::string first_difference(const std::string& actual, const std::string& expected) {
    std::istringstream a(actual), e(expected);
    std::string al, el;
    for (int line = 1;; ++line) {
        const bool ha = static_cast<bool>(std::getline(a, al)), he = static_cast<bool>(std::getline(e, el));
        if (!ha && !he) return "(the same lines; a difference in line endings)";
        if (al != el || ha != he) return std::format("line {}:\n  ours:     {}\n  ilspycmd: {}", line, ha ? al : "<end>", he ? el : "<end>");
    }
}

struct RunResult {
    int exit_code = 0;
    std::string out, err;
};

// Runs ilspycmd with the arguments ("{INPUT}" is the test assembly) after --disable-updatecheck, as the goldens were made.
RunResult run(std::vector<std::string> args) {
    for (std::string& a : args)
        if (a == "{INPUT}") a = input_path();
    args.insert(args.begin(), "--disable-updatecheck");
    std::ostringstream out, err;
    RunResult result;
    result.exit_code = run_ilspycmd(args, out, err);
    result.out = out.str();
    result.err = err.str();
    return result;
}

std::string current_directory_text() { return util::path::native_text(std::filesystem::current_path()); }

// The golden's text of a run.
std::string render(const RunResult& r) {
    std::string text = "exit=" + std::to_string(r.exit_code) + "\r\n--stdout--\r\n" + r.out + "--stderr--\r\n" + r.err;
    text = replace_all(text, input_path(), "{INPUT}");
    return replace_all(text, current_directory_text(), "{CWD}");
}

struct Case {
    const char* name;
    std::vector<std::string> args;
};

void check_case(const Case& c) {
    const std::string actual = render(run(c.args));
    const std::string expected = testing::regold_golden(testdata(std::string("golden/ilspycmd/") + c.name + ".txt"), actual);
    CB_CHECK(actual == expected);
    if (actual != expected) testing::record_failure(__FILE__, __LINE__, std::string(c.name) + ": " + first_difference(actual, expected));
}

void check_cases(const std::vector<Case>& cases) {
    for (const Case& c : cases) check_case(c);
}

// The standard output stored in a golden.
std::string golden_stdout(const char* name) {
    const std::string text = fs::read_text(testdata(std::string("golden/ilspycmd/") + name + ".txt"));
    const std::size_t begin = text.find("--stdout--\r\n") + std::string("--stdout--\r\n").size();
    return text.substr(begin, text.find("--stderr--\r\n") - begin);
}

// ---- help, version, listings -------------------------------------------------------------------------------------------

CB_TEST("dotnet.decompiler.ilspycmd.help_and_version", [] {
    check_cases({{"help", {"--help"}},
                 {"version", {"-v"}},
                 {"help_wins_over_later_error", {"-h", "--bogus"}},
                 {"error_before_help", {"--bogus", "-h"}}});
});

CB_TEST("dotnet.decompiler.ilspycmd.list", [] {
    check_cases({{"list_c", {"-l", "c", "{INPUT}"}},
                 {"list_all", {"-l", "c,i,s,d,e", "{INPUT}"}},
                 {"list_words", {"-l", "classes", "-l", "interface", "{INPUT}"}},
                 {"list_none", {"-l", "x", "{INPUT}"}},
                 {"list_resources", {"--list-resources", "{INPUT}"}},
                 {"language_version_number", {"-lv", "7", "-l", "c", "{INPUT}"}}});
});

CB_TEST("dotnet.decompiler.ilspycmd.il", [] { check_cases({{"il", {"-il", "{INPUT}"}}}); });

// ---- the metadata tables ---------------------------------------------------------------------------------------------------

CB_TEST("dotnet.decompiler.ilspycmd.dump_table", [] {
    const char* tables[] = {"Module",       "TypeRef",     "TypeDef",         "FieldPtr",        "Field",         "MethodPtr",   "MethodDef",
                            "ParamPtr",     "Param",       "InterfaceImpl",   "MemberRef",       "Constant",      "CustomAttribute", "FieldMarshal",
                            "DeclSecurity", "ClassLayout", "FieldLayout",     "StandAloneSig",   "EventMap",      "EventPtr",    "Event",
                            "PropertyMap",  "PropertyPtr", "Property",        "MethodSemantics", "MethodImpl",    "ModuleRef",   "TypeSpec",
                            "ImplMap",      "FieldRva",    "Assembly",        "AssemblyRef",     "File",          "ExportedType", "ManifestResource",
                            "NestedClass",  "GenericParam", "MethodSpec",     "GenericParamConstraint"};
    std::vector<std::string> names;
    for (const char* t : tables) names.push_back(std::string("dump_") + t);
    for (std::size_t i = 0; i < names.size(); ++i) check_case(Case{names[i].c_str(), {"--dump-table", tables[i], "{INPUT}"}});
});

CB_TEST("dotnet.decompiler.ilspycmd.dump_table_json", [] { check_cases({{"dump_table_json", {"--dump-table", "TypeDef", "--json", "{INPUT}"}}}); });

CB_TEST("dotnet.decompiler.ilspycmd.dump_table_names", [] {
    cb::dotnet::metadata::TableIndex table;
    CB_CHECK(MetadataTableDumper::try_parse_table_name("typedef", &table) && table == cb::dotnet::metadata::TableIndex::TypeDef);
    CB_CHECK(MetadataTableDumper::try_parse_table_name("TYPEDEF", &table) && table == cb::dotnet::metadata::TableIndex::TypeDef);
    CB_CHECK(MetadataTableDumper::try_parse_table_name("0x17", &table) && table == cb::dotnet::metadata::TableIndex::Property);
    CB_CHECK(MetadataTableDumper::try_parse_table_name("23", &table) && table == cb::dotnet::metadata::TableIndex::Property);
    CB_CHECK(MetadataTableDumper::try_parse_table_name("258", &table) && table == cb::dotnet::metadata::TableIndex::TypeDef);   // (TableIndex)258 is a byte
    CB_CHECK(!MetadataTableDumper::try_parse_table_name("0x24", &table));   // AssemblyRefProcessor: not supported
    CB_CHECK(!MetadataTableDumper::try_parse_table_name("EncLog", &table));
    CB_CHECK(!MetadataTableDumper::try_parse_table_name("", &table));
    CB_CHECK(!MetadataTableDumper::try_parse_table_name("-1", &table));
    CB_CHECK(MetadataTableDumper::table_to_string(cb::dotnet::metadata::TableIndex::MethodSemantics) == "MethodSemantics");
});

// ---- a type, a member, the whole module ---------------------------------------------------------------------------------

CB_TEST("dotnet.decompiler.ilspycmd.type", [] {
    check_cases({{"type_circle", {"-t", "Sample.Circle", "{INPUT}"}},
                 {"type_simple_name", {"-t", "circle", "{INPUT}"}},
                 {"type_nested", {"-t", "Sample.Box`1+Nested`1", "{INPUT}"}},
                 {"type_missing", {"-t", "Nope", "{INPUT}"}},
                 {"type_typo", {"-t", "Cicle", "{INPUT}"}}});
});

CB_TEST("dotnet.decompiler.ilspycmd.member", [] {
    check_cases({{"member_token", {"-m", "0x06000001", "{INPUT}"}},
                 {"member_docid", {"-m", "M:Sample.Circle.get_Radius", "{INPUT}"}},
                 {"member_bad_token", {"-m", "0xZZ", "{INPUT}"}},
                 {"member_bad_row", {"-m", "0x06ffffff", "{INPUT}"}},
                 {"member_bad_docid", {"-m", "M:Foo", "{INPUT}"}},
                 {"member_other_module", {"-m", "M:System.Object.ToString", "{INPUT}"}},
                 {"type_and_member", {"-t", "X", "-m", "0x06000001", "{INPUT}"}}});
});

CB_TEST("dotnet.decompiler.ilspycmd.decompile_module", [] {
    check_cases({{"decompile_all", {"{INPUT}"}}, {"decompile_csharp7", {"-lv", "CSharp7", "{INPUT}"}}});
});

// ---- mistakes ---------------------------------------------------------------------------------------------------------------

CB_TEST("dotnet.decompiler.ilspycmd.command_line_mistakes", [] {
    check_cases({{"no_arguments", {}},
                 {"bogus_option", {"--bogus", "{INPUT}"}},
                 {"cluster_option", {"-lc", "{INPUT}"}},
                 {"missing_value", {"-o"}},
                 {"language_version_bad", {"-lv", "Foo", "{INPUT}"}},
                 {"project_without_output", {"-p", "{INPUT}"}},
                 {"file_missing", {"nofile.dll"}},
                 {"file_missing_second", {"{INPUT}", "nofile.dll"}},
                 {"unexpected_value", {"-p=x", "{INPUT}"}},
                 {"duplicate_option", {"-o", "a", "-o", "b", "-l", "c", "{INPUT}"}},
                 {"pdb_missing", {"-usepdb=nofile.pdb", "{INPUT}"}},
                 {"settings_file_missing", {"--ilspy-settingsfile", "nofile.xml", "{INPUT}"}},
                 {"reference_path_missing", {"-r", "nodir", "{INPUT}"}},
                 {"json_without_table", {"--json", "{INPUT}"}}});
});

CB_TEST("dotnet.decompiler.ilspycmd.action_mistakes", [] {
    check_cases({{"dump_unknown_table", {"--dump-table", "Nope", "{INPUT}"}},
                 {"resource_missing", {"--resource", "x", "{INPUT}"}},
                 {"dump_package_not_a_bundle", {"-d", "{INPUT}"}},
                 {"generate_pdb_no_codeview", {"-genpdb", "{INPUT}"}}});
});

CB_TEST("dotnet.decompiler.ilspycmd.decompiler_settings", [] {
    check_cases({{"setting_invalid", {"-ds", "foo", "-l", "c", "{INPUT}"}},
                 {"setting_unknown", {"-ds", "Nope=1", "-l", "c", "{INPUT}"}},
                 {"setting_bad_value", {"-ds", "NativeIntegers=maybe", "-l", "c", "{INPUT}"}},
                 {"setting_not_browsable", {"-ds", "LoadInMemory=true", "-l", "c", "{INPUT}"}}});
    // a good value, and white space around the name and the value, are accepted
    const RunResult ok = run({"-ds", " NativeIntegers = false ", "-ds", "Dynamic=TRUE", "-l", "c", "{INPUT}"});
    CB_CHECK_EQ(ok.exit_code, 0);
    CB_CHECK_EQ(ok.err, std::string());
});

CB_TEST("dotnet.decompiler.ilspycmd.settings_file", [] {
    fs::ScopedTempDir dir("ilspycmd");
    const std::filesystem::path good = dir.path() / "settings.xml";
    fs::write_text(good, "<ILSpy><DecompilerSettings NativeIntegers=\"false\" Dynamic=\"0\"/></ILSpy>");
    const RunResult ok = run({"--ilspy-settingsfile", fs::to_text(good), "-l", "c", "{INPUT}"});
    CB_CHECK_EQ(ok.exit_code, 0);
    CB_CHECK_EQ(ok.err, std::string());
    CB_CHECK(ok.out == golden_stdout("list_c"));

    // a file that is not XML holds no settings; one with a value that is no boolean is reported and the defaults are used
    const std::filesystem::path broken = dir.path() / "broken.xml";
    fs::write_text(broken, "this is not xml");
    CB_CHECK(run({"--ilspy-settingsfile", fs::to_text(broken), "-l", "c", "{INPUT}"}).err.empty());
    const std::filesystem::path bad = dir.path() / "bad.xml";
    fs::write_text(bad, "<ILSpy><DecompilerSettings Dynamic=\"maybe\"/></ILSpy>");
    const RunResult reported = run({"--ilspy-settingsfile", fs::to_text(bad), "-l", "c", "{INPUT}"});
    CB_CHECK_EQ(reported.exit_code, 0);
    CB_CHECK(reported.err.starts_with("Error loading ILSpy settings file '"));
    CB_CHECK(reported.err.find("The string 'maybe' is not a valid Boolean value.") != std::string::npos);
});

// ---- output directory ---------------------------------------------------------------------------------------------------

CB_TEST("dotnet.decompiler.ilspycmd.output_directory", [] {
    fs::ScopedTempDir dir("ilspycmd");
    const std::string out_dir = fs::to_text(dir.path() / "out");   // made by the tool
    const RunResult list = run({"-l", "c", "-o", out_dir, "{INPUT}"});
    CB_CHECK_EQ(list.exit_code, 0);
    CB_CHECK(list.out.empty());
    CB_CHECK(fs::read_text(dir.path() / "out" / "Sample.list.txt") == golden_stdout("list_c"));

    CB_CHECK_EQ(run({"-il", "-o", out_dir, "{INPUT}"}).exit_code, 0);
    CB_CHECK(fs::read_text(dir.path() / "out" / "Sample.il") == golden_stdout("il"));

    CB_CHECK_EQ(run({"--dump-table", "TypeDef", "-o", out_dir, "{INPUT}"}).exit_code, 0);
    CB_CHECK(fs::read_text(dir.path() / "out" / "Sample.TypeDef.txt") == golden_stdout("dump_TypeDef"));

    CB_CHECK_EQ(run({"-t", "Sample.Circle", "-o", out_dir, "{INPUT}"}).exit_code, 0);
    CB_CHECK(fs::read_text(dir.path() / "out" / "Sample.Circle.decompiled.cs") == golden_stdout("type_circle"));

    // the project: one .csproj for one assembly
    const RunResult project = run({"-p", "-o", out_dir, "{INPUT}"});
    CB_CHECK_EQ(project.exit_code, 0);
    CB_CHECK(std::filesystem::exists(dir.path() / "out" / "Sample.csproj"));
});

// ---- the pieces ------------------------------------------------------------------------------------------------------------

CB_TEST("dotnet.decompiler.ilspycmd.types_parser", [] {
    using typesystem::TypeKind;
    using Kinds = std::set<TypeKind>;
    CB_CHECK(TypesParser::parse_selection({"c"}) == Kinds{TypeKind::Class});
    CB_CHECK(TypesParser::parse_selection({"cid"}) == (Kinds{TypeKind::Class, TypeKind::Interface, TypeKind::Delegate}));
    CB_CHECK(TypesParser::parse_selection({"c", "e"}).empty());   // two values: words only, and "c" is not a word
    CB_CHECK(TypesParser::parse_selection({"classes"}) == Kinds{TypeKind::Class});
    CB_CHECK(TypesParser::parse_selection({"Struct", "INTERFACE"}) == (Kinds{TypeKind::Struct, TypeKind::Interface}));
    CB_CHECK(TypesParser::parse_selection({"x"}).empty());
    CB_CHECK(TypesParser::parse_selection({""}).empty());
    CB_CHECK(TypesParser::parse_selection({"delegates", "q"}) == Kinds{TypeKind::Delegate});
    CB_CHECK(TypesParser::parse_selection({"ds"}) == (Kinds{TypeKind::Delegate, TypeKind::Struct}));
});

CB_TEST("dotnet.decompiler.ilspycmd.dotnet_enum", [] {
    CB_CHECK_EQ(enum_to_string(enums::type_attributes(), 0x100101), std::string("Public, Sealed, BeforeFieldInit"));
    CB_CHECK_EQ(enum_to_string(enums::type_attributes(), 0), std::string("NotPublic"));
    CB_CHECK_EQ(enum_to_string(enums::type_attributes(), 7), std::string("NestedFamORAssem"));
    CB_CHECK_EQ(enum_to_string(enums::type_attributes(), 24), std::string("ExtendedLayout"));
    CB_CHECK_EQ(enum_to_string(enums::assembly_flags(), 0), std::string("0"));
    CB_CHECK_EQ(enum_to_string(enums::method_attributes(), 0x1886), std::string("Public, HideBySig, SpecialName, RTSpecialName"));
    CB_CHECK_EQ(enum_to_string(enums::method_attributes(), 0), std::string("PrivateScope"));
    CB_CHECK_EQ(enum_to_string(enums::method_attributes(), 0x10000 | 8), std::string("65544"));   // a bit with no name: the number
    CB_CHECK_EQ(enum_to_string(enums::method_impl_attributes(), 3), std::string("CodeTypeMask"));   // not [Flags]: the first of equal values
    CB_CHECK_EQ(enum_to_string(enums::method_impl_attributes(), 24), std::string("24"));
    CB_CHECK_EQ(enum_to_string(enums::generic_parameter_attributes(), 7), std::string("VarianceMask, ReferenceTypeConstraint"));
    CB_CHECK_EQ(enum_to_string(enums::constant_type_code(), 14), std::string("String"));
    CB_CHECK_EQ(enum_to_string(enums::declarative_security_action(), 6), std::string("LinkDemand"));
    std::uint64_t value = 0;
    CB_CHECK(enum_try_parse(enums::language_version(), " csharp7_3 ", true, 32, true, &value) && value == 703);
    CB_CHECK(enum_try_parse(enums::language_version(), "Latest,CSharp7", true, 32, true, &value) && value == 0x7FFFFFFF);
    CB_CHECK(enum_try_parse(enums::language_version(), "99", true, 32, true, &value) && value == 99);
    CB_CHECK(enum_try_parse(enums::language_version(), "-1", true, 32, true, &value) && static_cast<std::int32_t>(value) == -1);
    CB_CHECK(!enum_try_parse(enums::language_version(), "Foo", true, 32, true, &value));
    CB_CHECK(!enum_try_parse(enums::language_version(), "Latest,", true, 32, true, &value));
    CB_CHECK(!enum_try_parse(enums::language_version(), "", true, 32, true, &value));
    CB_CHECK(!enum_try_parse(enums::language_version(), "csharp7", false, 32, true, &value));
    CB_CHECK(!enum_try_parse(enums::table_index(), "256", true, 8, false, &value));
});

CB_TEST("dotnet.decompiler.ilspycmd.command_line_parser", [] {
    const std::vector<CommandOption> options = {
        {"o", "outputdir", "-o|--outputdir <directory>", "", OptionType::SingleValue},
        {"p", "project", "-p|--project", "", OptionType::NoValue},
        {"l", "list", "-l|--list <x>", "", OptionType::MultipleValue},
        {"usepdb", "use-pdb", "-usepdb|--use-pdb", "", OptionType::SingleOrNoValue},
        {"h", "help", "-h|--help", "", OptionType::NoValue, OptionRole::Help}};
    const auto parse = [&](std::vector<std::string> args) { return parse_command_line(args, options); };

    ParsedCommandLine r = parse({"a.dll", "-o", "-p", "b.dll", "--list=x", "-l:y", "-usepdb", "-"});
    CB_CHECK(r.options[0].is_set && r.options[0].values == std::vector<std::string>{"-p"});   // the next argument is the value, whatever it is
    CB_CHECK(!r.options[1].is_set);
    CB_CHECK((r.options[2].values == std::vector<std::string>{"x", "y"}));
    CB_CHECK(r.options[3].is_set && r.options[3].values.empty());   // SingleOrNoValue: no value from the next argument
    CB_CHECK((r.arguments == std::vector<std::string>{"a.dll", "b.dll", "-"}));

    r = parse({"-usepdb=x.pdb", "--outputdir=C:\\x=y"});
    CB_CHECK(r.options[3].values == std::vector<std::string>{"x.pdb"});
    CB_CHECK(r.options[0].values == std::vector<std::string>{"C:\\x=y"});   // only the first separator splits

    r = parse({"-h", "--bogus"});
    CB_CHECK(r.stopped_by == OptionRole::Help);

    const auto message_of = [&](std::vector<std::string> args) {
        try {
            parse(std::move(args));
        } catch (const CommandParsingError& e) {
            return std::string(e.what());
        }
        return std::string("(no error)");
    };
    CB_CHECK_EQ(message_of({"--bogus"}), std::string("Unrecognized option '--bogus'"));
    CB_CHECK_EQ(message_of({"-pl"}), std::string("Unrecognized option '-pl'"));
    CB_CHECK_EQ(message_of({"--p"}), std::string("Unrecognized option '--p'"));
    CB_CHECK_EQ(message_of({"-project"}), std::string("Unrecognized option '-project'"));
    CB_CHECK_EQ(message_of({"--"}), std::string("Unrecognized option '--'"));
    CB_CHECK_EQ(message_of({"-o"}), std::string("Missing value for option 'o'"));
    CB_CHECK_EQ(message_of({"--outputdir"}), std::string("Missing value for option 'outputdir'"));
    CB_CHECK_EQ(message_of({"--project=x"}), std::string("Unexpected value 'x' for option 'project'"));
    CB_CHECK_EQ(message_of({"-o", "a", "--outputdir", "b"}), std::string("Unexpected value 'b' for option 'outputdir'"));
    CB_CHECK_EQ(message_of({"-O", "a"}), std::string("Unrecognized option '-O'"));
});

}  // namespace
}  // namespace cb::dotnet::decompiler::ilspycmd
