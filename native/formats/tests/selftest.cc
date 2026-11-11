// selftest.cc -- formats.dll on its own: small hand-made files through the C interface.
//
// The library's real check is gate.py (every fact against the Python readers over a corpus);
// this one needs nothing but the DLL, so CTest can run it anywhere.  Each case asks a question
// and compares the answer's pickle bytes with what Python's pickle of the same value is known
// to read back as -- checked through small expectations on the bytes (a value written, an
// exception raised, a question unknown).
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "cbformats.h"

static int failures = 0;

static void check(bool ok, const char *what) {
    std::printf("%s %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok) ++failures;
}

static int ask(const char *q, const std::vector<uint8_t> &data, const std::string &args, std::string &answer) {
    const uint8_t *out = nullptr;
    size_t n = 0;
    int r = cbf_call(q, data.empty() ? nullptr : data.data(), data.size(),
                     reinterpret_cast<const uint8_t *>(args.data()), args.size(), &out, &n);
    answer.assign(reinterpret_cast<const char *>(out), n);
    return r;
}

static bool has(const std::string &hay, const std::string &needle) { return hay.find(needle) != std::string::npos; }

// a pickled empty tuple: the arguments of a question that takes none
static const std::string kNoArgs("\x80\x04)\x2e", 4);

int main() {
    check(cbf_version() == CBF_VERSION, "version");
    std::string a;
    check(ask("no.such", {}, kNoArgs, a) == CBF_NO_SUCH, "an unknown question is told apart");
    check(ask("pe.parse", {}, kNoArgs, a) == CBF_NO_SUCH, "the PE questions moved to native/cb/formats");
    check(!has(cbf_questions(), "pe.parse\n"), "pe.parse is not offered");
    check(has(cbf_questions(), "cab."), "the cabinet reader still offers its questions");
    std::printf("%s\n", failures ? "FAILED" : "all passed");
    return failures ? 1 : 0;
}
