#include "recompile/xenonrecomp/XenonRecomp/pch.h"
#include "recompile/xenonrecomp/XenonRecomp/test_recompiler.h"
#include "recompile/xenonrecomp/XenonUtils/xenon_log.h"

// As a library (XENON_RECOMP_LIBRARY: CodeBreaker links it in), the program's main is xenon_recomp_main.
#ifdef XENON_RECOMP_LIBRARY
#define XENON_RECOMP_MAIN xenon_recomp_main
#else
#define XENON_RECOMP_MAIN main
#endif

static int recomp_run(int argc, char* argv[]);

int XENON_RECOMP_MAIN(int argc, char* argv[])
{
    try
    {
        return recomp_run(argc, argv);
    }
    catch (const std::exception& e)
    {
        xenon_log::println("error: {}", e.what());
        return 1;
    }
}

static int recomp_run(int argc, char* argv[])
{
#ifndef XENON_RECOMP_CONFIG_FILE_PATH
    if (argc < 3)
    {
        xenon_log::print("Usage: XenonRecomp [input TOML file path] [PPC context header file path]");
        return EXIT_SUCCESS;
    }
#endif

    const char* path = 
    #ifdef XENON_RECOMP_CONFIG_FILE_PATH
        XENON_RECOMP_CONFIG_FILE_PATH
    #else
        argv[1]
    #endif
        ;

    if (std::filesystem::is_regular_file(path))
    {
        Recompiler recompiler;
        if (!recompiler.LoadConfig(path))
            return -1;

        recompiler.Analyse();

        auto entry = recompiler.image.symbols.find(recompiler.image.entry_point);
        if (entry != recompiler.image.symbols.end())
        {
            entry->name = "_xstart" + recompiler.config.symbolSuffix;
        }

        const char* headerFilePath =
#ifdef XENON_RECOMP_HEADER_FILE_PATH
            XENON_RECOMP_HEADER_FILE_PATH
#else
            argv[2]
#endif
            ;

        recompiler.Recompile(headerFilePath);
    }
    else
    {
        TestRecompiler::RecompileTests(path, argv[2]);
    }

    return EXIT_SUCCESS;
}
