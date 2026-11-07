// test_cpp17.cc -- the C++17 back end's unit tests (CTest): the rule set chosen by Unit.lang, and the
// generated text of a Python module and of a Java program against what the gate accepted (the
// fixtures in tests/data/cpp17: each case's L2 and its C++). The full gate -- every file of both
// former Python back ends, byte for byte -- is docs/NATIVE.md's; tests/translate/test_python_frontend.py
// and test_cpp17_backend.py judge the text by compiling and running it.
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "cpp17.hpp"
#include "json.hpp"

static int failures = 0, checks = 0;

#define CHECK(c)                                                                                           \
    do {                                                                                                   \
        checks++;                                                                                          \
        if (!(c)) {                                                                                        \
            failures++;                                                                                    \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c);                                       \
        }                                                                                                  \
    } while (0)

static std::string readfile(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// (a checkout may have turned the fixtures' line ends into CRLF: the text written has none)
static std::string data(const char* name) {
    std::string s = readfile(std::string(L2_TESTDATA) + "/cpp17/" + name), out;
    for (char c : s)
        if (c != '\r') out += c;
    return out;
}

static bool declines(const py::V& lang) {
    try {
        cpp17::semantics(lang);
    } catch (cpp17::Decline&) {
        return true;
    }
    return false;
}

static void first_difference(const std::string& got, const std::string& want) {
    size_t n = 0;
    while (n < got.size() && n < want.size() && got[n] == want[n]) n++;
    std::printf("   differ at byte %zu: got ...%s... want ...%s...\n", n, got.substr(n, 60).c_str(), want.substr(n, 60).c_str());
}

static void test_rules() {
    CHECK(cpp17::semantics(py::V("python")) == cpp17::Lang::Python);
    CHECK(cpp17::semantics(py::V("java")) == cpp17::Lang::Java);
    CHECK(cpp17::semantics(py::V()) == cpp17::Lang::Java);          // L2 from the Java front end says none
    CHECK(declines(py::V("csharp")));
    CHECK(declines(py::V("c")));
    CHECK(declines(py::V("cpp")));
}

static void test_python_module() {
    py::V unit = py::json_parse(data("semantics.unit.json"));
    py::V out = cpp17::write_unit(unit, API_MAPS);
    std::string h = out[0].str(), s = out[1].str();
    std::string wh = data("semantics.hpp"), ws = data("semantics.cpp");
    if (std::getenv("CPP17_WRITE_FIXTURES")) {   // (after a deliberate change to what the back end writes)
        std::ofstream(std::string(L2_TESTDATA) + "/cpp17/semantics.hpp", std::ios::binary) << h;
        std::ofstream(std::string(L2_TESTDATA) + "/cpp17/semantics.cpp", std::ios::binary) << s;
        wh = h, ws = s;
    }
    CHECK(h == wh);
    if (h != wh) first_difference(h, wh);
    CHECK(s == ws);
    if (s != ws) first_difference(s, ws);
    CHECK(out[2].size() > 0);                                         // every function's lines
    bool declined = false;
    try {
        py::V java = py::json_parse(data("modern.units.json"));
        cpp17::write_unit(java[0], API_MAPS);
    } catch (cpp17::Decline&) {
        declined = true;
    }
    CHECK(declined);                                                  // a Java unit is written whole
}

static void test_java_program() {
    py::V units = py::json_parse(data("modern.units.json"));
    std::string got = cpp17::program(units, py::V());
    std::string want = data("modern.cpp");
    CHECK(got == want);
    if (got != want) first_difference(got, want);
}

int main() {
    test_rules();
    test_python_module();
    test_java_program();
    std::printf("%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
