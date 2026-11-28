// Tests of dotnet/decompiler's C# stage (stages 7-10 of docs/DOTNET_DECOMPILER_PLAN.md): the whole-module C# text against ILSpy's own
// (`ilspycmd <assembly>` with no options; work/ilspy-port/scripts/cscompare.sh).
#include <format>
#include <fstream>
#include <cstdint>
#include <cstdio>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/l2_body.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"

namespace cb::dotnet::decompiler::csharp {
namespace {

#ifdef _WIN32
// CB_DN_CS_CRASH=1: a crash prints its code and the module-relative addresses of the call stack (look them up in a linker /MAP file).
LONG WINAPI print_crash(EXCEPTION_POINTERS* info) {
    std::fprintf(stderr, "crash %08lx at rva %llx\n", info->ExceptionRecord->ExceptionCode,
                 static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(info->ExceptionRecord->ExceptionAddress) -
                                                 reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr))));
    void* frames[64];
    const USHORT count = CaptureStackBackTrace(0, 64, frames, nullptr);
    for (USHORT i = 0; i < count; i++)
        std::fprintf(stderr, "  rva %llx\n", static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(frames[i]) -
                                                                              reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr))));
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif

// ilspycmd writes its standard output through Console.Out, whose encoding is the console's code page: UTF-8 by default here (a UTF-8
// console). CB_DN_CS_CODEPAGE=437 writes what a console of that code page gets: a character it has no code for becomes its best fit.
std::string encode_like_console(const std::string& utf8) {
#ifdef _WIN32
    int code_page = 65001;
    if (const auto env = fs::get_env("CB_DN_CS_CODEPAGE")) code_page = std::stoi(*env);
    if (code_page == 65001 || utf8.empty()) return utf8;
    const int wide_count = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    std::wstring wide(static_cast<std::size_t>(wide_count), L' ');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), wide.data(), wide_count);
    const int count = WideCharToMultiByte(static_cast<UINT>(code_page), 0, wide.data(), wide_count, nullptr, 0, nullptr, nullptr);
    std::string bytes(static_cast<std::size_t>(count), ' ');
    WideCharToMultiByte(static_cast<UINT>(code_page), 0, wide.data(), wide_count, bytes.data(), count, nullptr, nullptr);
    return bytes;
#else
    return utf8;
#endif
}

void install_crash_filter() {
#ifdef _WIN32
    SetUnhandledExceptionFilter(&print_crash);
#endif
}

// ad hoc: CB_DN_CS_IN=<assembly> CB_DN_CS_OUT=<text> writes what `ilspycmd <assembly>` writes to its standard output: the whole module
// decompiled with the settings of the command line tool when no option is given (language version Latest, unresolved references
// tolerated), the references found by a UniversalAssemblyResolver of the assembly's target framework. Errors that did not stop the
// decompilation go to standard error as ilspycmd's ReportDecompilationErrors writes them.
// CB_DN_CS_L2=<file> also writes what the builders read from L2 (stage 14): the bodies, the ones refused, the instructions read from
// their node and those read from the ILAst.
CB_TEST("dotnet.decompiler.csharp.wip_dump", [] {
    const auto in = fs::get_env("CB_DN_CS_IN");
    const auto out = fs::get_env("CB_DN_CS_OUT");
    if (!in || !out) return;
    if (fs::get_env("CB_DN_CS_CRASH")) install_crash_filter();
    const std::filesystem::path assembly_file_name(*in);
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(assembly_file_name);
    metadata::UniversalAssemblyResolver resolver(assembly_file_name, false, metadata::detect_target_framework_id(*module));
    DecompilerSettings settings(LanguageVersion::Latest);
    settings.throw_on_assembly_resolve_errors = false;
    settings.remove_dead_code = false;
    settings.remove_dead_stores = false;
    settings.use_nested_directories_for_namespaces = false;
    CSharpDecompiler decompiler(assembly_file_name, resolver, settings);
    if (fs::get_env("CB_DN_CS_TRACE")) {
        // find the type a crash is in: every top-level type alone, named on standard error before it is decompiled
        for (const typesystem::ITypeDefinition* type : decompiler.type_system().main_metadata_module().top_level_type_definitions()) {
            std::cerr << "type " << type->full_type_name().reflection_name() << std::endl;
            for (const typesystem::IMethod* method : type->methods()) {
                if (method->metadata_token().is_nil()) continue;
                std::cerr << "  method " << method->name() << std::endl;
                decompiler.decompile_as_string({method->metadata_token()});
            }
        }
    }
    const std::string text = decompiler.decompile_whole_module_as_string();
    std::ofstream(std::filesystem::path(*out), std::ios::binary) << encode_like_console(text);
    if (const auto stats = fs::get_env("CB_DN_CS_L2")) {
        // stage 14: what the builders read from L2
        const L2Lane* lane = decompiler.l2_lane();
        std::ofstream(std::filesystem::path(*stats), std::ios::binary)
            << std::format("bodies {} refused {} from_l2 {} from_ilast {}\n", lane ? lane->bodies : 0, lane ? lane->refused : 0,
                           lane ? lane->from_l2 : 0, lane ? lane->from_ilast : 0);
        if (lane != nullptr) {
            std::ofstream by_op(std::filesystem::path(*stats), std::ios::binary | std::ios::app);
            const auto ms = [](std::chrono::steady_clock::duration d) { return std::chrono::duration<double, std::milli>(d).count(); };
            by_op << std::format("time transforms_ms {:.1f} lowering_ms {:.1f} builders_ms {:.1f}\n", ms(lane->transforms_time), ms(lane->lowering_time),
                                 ms(lane->builders_time));
            for (std::size_t op = 0; op < lane->from_ilast_by_op.size(); ++op)
                if (lane->from_ilast_by_op[op] != 0)
                    by_op << std::format("ilast {} {} without_node {}\n", il::opcode_name(static_cast<il::OpCode>(op)), lane->from_ilast_by_op[op],
                                         lane->without_node_by_op[op]);
        }
    }
    if (!decompiler.errors().empty()) {
        std::cerr << "While decompiling " << *in << ":\n";
        for (const DecompilerError& error : decompiler.errors()) std::cerr << "  " << CSharpDecompiler::get_error_headline(error) << "\n";
    }
});

}  // namespace
}  // namespace cb::dotnet::decompiler::csharp
