// decompile/function_in_test.cpp -- one function cut out of a module's C, by address, by segment:offset and by name.
#include <string>

#include "core/testing.hpp"
#include "decompile/decompiler.hpp"

namespace {

const char* const kFunctionInModule =
    "#include <windows.h>\n"
    "\n"
    "/* the first */\n"
    "int sub_10001000(int a)\n"
    "{\n"
    "    return a;\n"
    "}\n"
    "\n"
    "// a 16-bit one\n"
    "void sub_1_0116(void)\r\n"
    "{\r\n"
    "}\r\n"
    "\n"
    "int LZCopy(int from, int to) {\n"
    "    return func_0x7f021036(from, to);\n"
    "}\n";

}  // namespace

CB_TEST("decompile.decompiler.function_in", [] {
    using cb::decompile::decompiler::function_in;
    CB_CHECK_EQ(function_in(kFunctionInModule, "10001000").value_or("?"), std::string("/* the first */\nint sub_10001000(int a)\n{\n    return a;\n}"));
    CB_CHECK_EQ(function_in(kFunctionInModule, "1:116").value_or("?"), std::string("// a 16-bit one\nvoid sub_1_0116(void)\n{\n}"));
    CB_CHECK_EQ(function_in(kFunctionInModule, "LZCopy").value_or("?"), std::string("int LZCopy(int from, int to) {\n    return func_0x7f021036(from, to);\n}"));
    CB_CHECK_EQ(function_in(kFunctionInModule, "lzcopy").has_value(), true);   // (a name compares without case)
    CB_CHECK_EQ(function_in(kFunctionInModule, "7f021036").has_value(), false);   // (a call is not a header)
    CB_CHECK_EQ(function_in(kFunctionInModule, "2:0116").has_value(), false);
    CB_CHECK_EQ(function_in("", "10001000").has_value(), false);
});
