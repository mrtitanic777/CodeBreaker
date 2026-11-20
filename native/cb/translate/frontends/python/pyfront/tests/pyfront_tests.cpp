// pyfront_tests -- the grammar cases.
//
// tests/cases/NAME.cases holds one or more cases separated by a line "#----"; NAME.expected
// holds, for each case in order, one line: CPython 3.14's
// ast.dump(ast.parse(case), include_attributes=True), or for a case CPython rejects
// "ERROR <lineno> <offset>".  The expected files were made once with CPython as the oracle
// (native/pyfront/README.md says how); this program checks pyfront against them.
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "pyfront/pyfront.h"
#include "pyfront/pyfront.hpp"

namespace fs = std::filesystem;

namespace {

std::string slurp(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

std::vector<std::string> split_cases(const std::string& s) {
    std::vector<std::string> out;
    std::string cur;
    std::istringstream in(s);
    std::string line;
    while (std::getline(in, line)) {
        if (line == "#----" || line == "#----\r") {
            out.push_back(cur);
            cur.clear();
        } else {
            cur += line;
            cur += '\n';
        }
    }
    out.push_back(cur);
    return out;
}

std::vector<std::string> lines(const std::string& s) {
    std::vector<std::string> out;
    std::istringstream in(s);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        out.push_back(line);
    }
    return out;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: pyfront_tests CASES_DIR\n");
        return 2;
    }
    int pass = 0, fail = 0;
    std::vector<fs::path> files;
    for (auto& e : fs::directory_iterator(argv[1]))
        if (e.path().extension() == ".cases") files.push_back(e.path());
    std::sort(files.begin(), files.end());
    for (const fs::path& py : files) {
        fs::path exp = py;
        exp.replace_extension(".expected");
        auto cases = split_cases(slurp(py));
        auto want = lines(slurp(exp));
        if (want.size() != cases.size()) {
            fprintf(stderr, "%s: %zu cases but %zu expected lines\n", py.string().c_str(), cases.size(), want.size());
            ++fail;
            continue;
        }
        for (size_t i = 0; i < cases.size(); ++i) {
            auto r = pyfront::parse(cases[i]);
            std::string got = r ? pyfront::dump(*r.module) : "ERROR " + std::to_string(r.error->lineno) + " " + std::to_string(r.error->offset);
            if (got == want[i]) {
                ++pass;
            } else {
                ++fail;
                fprintf(stderr, "FAIL %s case %zu\n--- source\n%s--- want\n%s\n--- got\n%s\n", py.filename().string().c_str(), i + 1,
                        cases[i].c_str(), want[i].c_str(), got.c_str());
                if (!r) fprintf(stderr, "    (%s)\n", r.error->msg.c_str());
            }
        }
    }
    // The C ABI on one case.
    {
        const char src[] = "x = 1\n";
        pyfront_tree* t = pyfront_parse(src, sizeof src - 1, "t.py");
        size_t n = 0;
        const char* d = pyfront_ok(t) ? pyfront_dump(t, 0, &n) : nullptr;
        std::string got = d ? std::string(d, n) : "";
        if (got == "Module(body=[Assign(targets=[Name(id='x', ctx=Store())], value=Constant(value=1))])") ++pass;
        else { ++fail; fprintf(stderr, "FAIL C ABI dump: %s\n", got.c_str()); }
        pyfront_free(t);
        t = pyfront_parse("x = (\n", 6, "t.py");
        if (!pyfront_ok(t) && pyfront_error_lineno(t) == 1 && pyfront_error_offset(t) == 5) ++pass;
        else { ++fail; fprintf(stderr, "FAIL C ABI error: %d %d\n", pyfront_error_lineno(t), pyfront_error_offset(t)); }
        pyfront_free(t);
    }
    // A BOM, CRLF and lone-CR line endings parse as LF does (columns are unchanged).
    {
        auto lf = pyfront::parse("if x:\n    y = 'a'\nz = f'{x}'\n");
        auto crlf = pyfront::parse("\xEF\xBB\xBFif x:\r\n    y = 'a'\r\nz = f'{x}'\r\n");
        auto cr = pyfront::parse("if x:\r    y = 'a'\rz = f'{x}'");
        std::string a = lf ? pyfront::dump(*lf.module) : "", b = crlf ? pyfront::dump(*crlf.module) : "", c = cr ? pyfront::dump(*cr.module) : "";
        if (!a.empty() && a == b && a == c) ++pass;
        else { ++fail; fprintf(stderr, "FAIL line endings:\n%s\n%s\n%s\n", a.c_str(), b.c_str(), c.c_str()); }
    }
    printf("pyfront grammar cases: %d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}
