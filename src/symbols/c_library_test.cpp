// Tests of cb/symbols/c_library: a small table in the file's layout, and the project's own table (config/symbols/c_library.json).
#include "symbols/c_library.hpp"

#include "core/error.hpp"
#include "core/testing.hpp"

namespace cl = cb::symbols::c_library;

CB_TEST("symbols.c_library.reads_the_table", [] {
    const cl::CLibrary library = cl::CLibrary::parse(
        R"({"functions": {"strcpy": ["string.h", "p", "p,p"], "printf": ["stdio.h", "i", "p,..."], "exit": ["process.h", "v", "i"],
            "atof": ["math.h", "d", "p"], "ftell": ["stdio.h", "l", "p"]}})");
    CB_CHECK_EQ(library.size(), std::size_t{5});
    const cl::Prototype* copy = library.find("strcpy");
    CB_CHECK(copy != nullptr);
    CB_CHECK_EQ(copy->header, "string.h");
    CB_CHECK_EQ(copy->result_bytes(4), std::uint32_t{4});   // large model: a far pointer
    CB_CHECK_EQ(copy->result_bytes(2), std::uint32_t{2});   // small model: a near one
    CB_CHECK((copy->argument_sizes(4) == std::vector<std::string>{"4p", "4p"}));
    CB_CHECK((copy->argument_sizes(2) == std::vector<std::string>{"2", "2"}));
    CB_CHECK(library.find("printf")->argument_sizes(4).empty());   // as many as the caller passes
    CB_CHECK(library.find("printf")->variadic);
    CB_CHECK_EQ(library.find("exit")->result_bytes(4), std::uint32_t{0});
    CB_CHECK_EQ(library.find("atof")->result_bytes(4), std::uint32_t{0});   // a double is not in AX
    CB_CHECK_EQ(library.find("ftell")->result_bytes(4), std::uint32_t{4});
    CB_CHECK(library.find("nothing") == nullptr);
});

CB_TEST("symbols.c_library.refuses_a_table_that_is_not_one", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, cl::CLibrary::parse("not json"));
    CB_CHECK_THROWS_AS(cb::ParseError, cl::CLibrary::parse(R"({"functions": {"f": ["h"]}})"));
});

CB_TEST("symbols.c_library.the_projects_table", [] {
    const cl::CLibrary library = cl::CLibrary::load(cb::testing::data_path("../config/symbols/c_library.json"));
    CB_CHECK(library.size() > 150);
    CB_CHECK_EQ(library.find("strlen")->header, "string.h");
    CB_CHECK_EQ(library.find("fopen")->header, "stdio.h");
    // the C type of each pointer argument, as Microsoft C 4.0's header declares it
    CB_CHECK((library.find("fwrite")->pointers == std::vector<std::string>{"char *", "", "", "FILE *"}));
    CB_CHECK(library.find("abs")->pointers.empty());
});

CB_TEST("symbols.c_library.reads_each_pointer_arguments_type", [] {
    const cl::CLibrary library = cl::CLibrary::parse(
        R"({"functions": {"fputs": ["stdio.h", "i", "p,p", "char *,FILE *"], "fprintf": ["stdio.h", "i", "p,p,...", "FILE *,char *"],
            "abs": ["stdlib.h", "i", "i"]}})");
    CB_CHECK((library.find("fputs")->pointers == std::vector<std::string>{"char *", "FILE *"}));
    CB_CHECK((library.find("fprintf")->pointers == std::vector<std::string>{"FILE *", "char *"}));
    CB_CHECK(library.find("abs")->pointers.empty());
    CB_CHECK_THROWS_AS(cb::ParseError, cl::CLibrary::parse(R"({"functions": {"f": ["h", "i", "p", 4]}})"));
});
