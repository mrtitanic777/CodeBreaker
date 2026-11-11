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

static void put16(std::vector<uint8_t> &b, size_t at, uint16_t v) { b[at] = uint8_t(v); b[at + 1] = uint8_t(v >> 8); }
static void put32(std::vector<uint8_t> &b, size_t at, uint32_t v) { for (int k = 0; k < 4; ++k) b[at + k] = uint8_t(v >> (8 * k)); }

// a minimal PE32 DLL: one code section ".text" at RVA 0x1000, file offset 0x200, 0x10 bytes
static std::vector<uint8_t> tiny_pe() {
    std::vector<uint8_t> b(0x210, 0);
    b[0] = 'M'; b[1] = 'Z';
    put32(b, 0x3C, 0x40);
    std::memcpy(&b[0x40], "PE\0\0", 4);
    put16(b, 0x44, 0x014C);            // machine
    put16(b, 0x46, 1);                 // sections
    put32(b, 0x48, 0x12345678);        // timestamp
    put16(b, 0x54, 0xE0);              // optional header size
    put16(b, 0x56, 0x2102);            // characteristics: DLL
    const size_t opt = 0x58;
    put16(b, opt, 0x10B);
    b[opt + 2] = 6; b[opt + 3] = 1;
    put32(b, opt + 16, 0x1004);        // entry
    put32(b, opt + 28, 0x10000000);    // image base
    put16(b, opt + 68, 2);             // GUI
    put32(b, opt + 92, 16);            // data directories
    const size_t sec = opt + 0xE0;
    std::memcpy(&b[sec], ".text", 5);
    put32(b, sec + 8, 0x10);
    put32(b, sec + 12, 0x1000);
    put32(b, sec + 16, 0x10);
    put32(b, sec + 20, 0x200);
    put32(b, sec + 36, 0x60000020);
    return b;
}

int main() {
    check(cbf_version() == CBF_VERSION, "version");
    check(has(cbf_questions(), "pe.parse\n"), "pe.parse is offered");

    std::string a;
    check(ask("no.such", {}, kNoArgs, a) == CBF_NO_SUCH, "an unknown question is told apart");

    std::vector<uint8_t> pe = tiny_pe();
    check(ask("pe.parse", pe, kNoArgs, a) == CBF_OK, "pe.parse of a tiny DLL");
    check(has(a, "\x8c\x05.text"), "  its section is named .text");
    check(ask("pe.is_dll", pe, kNoArgs, a) == CBF_OK && has(a, "\x88"), "  it is a DLL");
    check(ask("pe.machine_name", pe, kNoArgs, a) == CBF_OK && has(a, "\x8c\x04i386"), "  machine i386");
    check(ask("pe.build_meta", pe, kNoArgs, a) == CBF_OK && has(a, "\x8c\x03GUI") && has(a, "\x8c\x03" "6.1"),
          "  build_meta: GUI, linker 6.1");
    check(ask("pe.code_sections", pe, kNoArgs, a) == CBF_OK && has(a, "J\x00\x10\x00\x10"), "  code at 0x10001000");
    check(ask("pe.exports", pe, kNoArgs, a) == CBF_OK && a == std::string("\x80\x04](e.", 6), "  no exports: []");

    std::vector<uint8_t> mz = {'M', 'Z', 0, 0};
    check(ask("pe.parse", mz, kNoArgs, a) == CBF_RAISED && has(a, "struct.error"), "a cut-off MZ header raises struct.error");
    std::vector<uint8_t> zm = {'Z', 'M', 0, 0};
    check(ask("pe.parse", zm, kNoArgs, a) == CBF_RAISED && has(a, "ValueError"), "not MZ raises ValueError");

    std::printf("%s\n", failures ? "FAILED" : "all passed");
    return failures ? 1 : 0;
}
