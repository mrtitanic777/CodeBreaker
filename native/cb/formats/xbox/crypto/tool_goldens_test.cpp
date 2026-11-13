// Tests of the small tools of the `crypto` command against what the Python printed and wrote for the same inputs: testdata/tools_goldens.txt, made by
// testdata/make_tools_goldens.py (`codebreaker.exe crypto <tool> ...`, the translated Python; the exp_signer cases by `python -m codebreaker` because the
// exe's ctypes key structures cannot sign). Each case builds the inputs the generator builds (pattern(n, seed) and the structured images below),
// runs the native command in a folder of its own and compares its output and the files it wrote (size and SHA-1) with the golden.
//
// The tools covered: bin2lang bootchain cpukey_ecd exp_signer flashplan format_key fuses hypervisor keyvault nand patch_checker smc xdk_extract
// xell_unpack xval.
//
// DIVERGES from the Python (each is normalised here, with what it is):
//   bin2lang, format_key  the Python's print() adds a blank line after the source it has printed; the native tool prints the source once
//   cpukey_ecd            the verdicts are compared as values: the native tool prints yes/no and one "error:" line each, not Python's dict and list repr
//   fuses                 the Python prints the CPU key a second time at the end
//   smc config            fields the data is too short for are "-" (the Python printed None)
//   nand info             the native tool lists the header's fields as rows; only path, size and type are compared
//   hypervisor info       keys_are_55 is yes/no, and the hash ranges are rows of hex (the Python printed a list of decimal pairs)
//   xval                  the typo "Intergrity" of one finding is fixed (here and in the Python source)
//   xell_unpack           the native tool lists the files it wrote; the Python printed nothing
//   xdk_extract           the Python crashed at the end of the file after extracting everything (exit 1); the native tool exits 0
//   files                 text the Python wrote with CRLF (it writes text the Windows way) is written with LF; compared with the CRLF made LF
#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <regex>
#include <string>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"
#include "cb/formats/xbox/crypto/cpukey_ecd.hpp"
#include "cb/formats/xbox/crypto/tools.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace tool_goldens_test {

using namespace cb;
namespace stdfs = std::filesystem;
namespace xcrypto = cb::formats::xbox::crypto;

using Files = std::map<std::string, Bytes>;

Bytes pattern(std::size_t n, int seed) {
    Bytes out(n);
    for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<std::uint8_t>((i * 131 + static_cast<std::size_t>(seed) * 17 + (i >> 8) * 7) & 0xFF);
    return out;
}

void put_be(Bytes& image, std::size_t at, std::uint64_t value, int bytes) {
    for (int i = 0; i < bytes; ++i) image[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(value >> (8 * (bytes - 1 - i)));
}

void put(Bytes& image, std::size_t at, ByteView data) { std::copy(data.begin(), data.end(), image.begin() + static_cast<std::ptrdiff_t>(at)); }

void stage(Bytes& image, std::size_t at, const char* magic, unsigned build, unsigned pairing, unsigned flags, std::uint32_t entry, std::uint32_t size,
           const Bytes* key = nullptr) {
    image[at] = static_cast<std::uint8_t>(magic[0]);
    image[at + 1] = static_cast<std::uint8_t>(magic[1]);
    put_be(image, at + 2, build, 2);
    put_be(image, at + 4, pairing, 2);
    put_be(image, at + 6, flags, 2);
    put_be(image, at + 8, entry, 4);
    put_be(image, at + 12, size, 4);
    if (key) put(image, at + 16, *key);
}

Bytes make_nand() {
    Bytes image(0xA1000, 0);
    const Bytes k3 = pattern(16, 3), k4 = pattern(16, 4), k5 = pattern(16, 5), k6 = pattern(16, 6), k7 = pattern(16, 7), k8 = pattern(16, 8), k9 = pattern(16, 9);
    stage(image, 0x8000, "CB", 0x1D69, 0x8000, 0x0801, 0x03000000, 0x2000, &k3);
    stage(image, 0xA000, "CB", 0x1D6A, 0, 0, 0x03000010, 0x1000);
    stage(image, 0xB000, "CD", 0x1D6B, 0, 0, 0x03000020, 0x1000, &k4);
    stage(image, 0x70000, "CE", 0x2A5F, 0, 1, 0x80000030, 0x3000, &k5);
    stage(image, 0x90010, "CF", 0x2A60, 0, 0, 0x03000040, 0x1000, &k6);
    stage(image, 0x91000, "CG", 0x2A60, 0, 0, 0x03000050, 0x1000, &k7);
    stage(image, 0xA0000, "HV", 0x1000, 0, 0, 0, 0x40000);
    stage(image, 0x80000, "CD", 0x1D6B, 0, 0, 0x03000060, 0x1000, &k8);
    stage(image, 0x82000, "CB", 0x1D6C, 0, 0, 0x03000070, 0, &k9);
    return image;
}

Bytes make_keyvault(std::uint32_t console_type, unsigned region) {
    Bytes kv = pattern(0x4000, 11);
    kv[0x18] = 1;
    put_be(kv, 0x1C, 0x00A5, 2);
    put_be(kv, 0x1E, 0x0102, 2);
    put(kv, 0x9C, as_bytes("123456789012"));
    put_be(kv, 0xB4, region, 2);
    put_be(kv, 0x9E4, console_type, 4);
    put_be(kv, 0x9F0, 0x10001, 4);
    return kv;
}

Bytes make_smc_plain() {
    Bytes smc = pattern(0x1800, 12);
    for (std::size_t i = smc.size() - 0x400; i < smc.size(); ++i) smc[i] = static_cast<std::uint8_t>(0xF0 | (smc[i] & 0x0F));
    const std::size_t at = smc.size() - 0x100;
    smc[at] = 2;
    smc[at + 1] = 3;
    smc[at + 2] = 0x11;
    smc[at + 3] = 4;
    return smc;
}

Bytes make_flash_header(std::uint32_t config_at, std::uint32_t smc_len, std::uint32_t smc_at, std::size_t total) {
    Bytes nand = pattern(total, 13);
    nand[0] = 0xFF;
    nand[1] = 0x4F;
    put_be(nand, 0x74, config_at, 4);
    put_be(nand, 0x78, smc_len, 4);
    put_be(nand, 0x7C, smc_at, 4);
    return nand;
}

Bytes make_hypervisor(int salt) {
    Bytes hv = pattern(0x40000, salt);
    std::fill_n(hv.begin() + 0x10100, 0x30, std::uint8_t{0x55});
    return hv;
}

Bytes make_patch_table(const std::vector<std::pair<std::uint32_t, Bytes>>& patches) {
    Bytes out;
    for (const auto& [address, words] : patches) {
        for (const std::uint64_t v : {std::uint64_t{address}, std::uint64_t{words.size() / 4}})
            for (int i = 3; i >= 0; --i) out.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
        append(out, words);
    }
    for (int i = 0; i < 4; ++i) out.push_back(0xFF);
    return out;
}

Bytes read_testdata(const std::string& name) { return fs::read_file(testing::data_path("formats/xbox/crypto/testdata/" + name)); }

Bytes make_xell(int seed) {
    const Bytes gz = read_testdata("xell_stage2_" + std::to_string(seed) + ".gz");
    Bytes image = pattern(16 * 1024, seed + 40);
    append(image, gz);
    image.resize(256 * 1024 - 16 * 1024 - 16 - 10 - 8 + 16 * 1024, 0);
    append(image, pattern(16, seed + 50));
    image.resize(image.size() + 18, 0);
    return image;
}

Bytes make_pe(const std::vector<std::uint32_t>& cabinets) {
    Bytes exe(0x40, 0);
    exe[0] = 'M';
    exe[1] = 'Z';
    exe[0x3C] = 0x40;
    const auto le = [&exe](std::uint64_t v, int bytes) {
        for (int i = 0; i < bytes; ++i) exe.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
    };
    for (const char c : std::string("PE\0\0", 4)) exe.push_back(static_cast<std::uint8_t>(c));
    le(0x14C, 2);   // machine
    le(2, 2);       // sections
    le(0, 4);
    le(0, 4);
    le(0, 4);
    le(224, 2);     // the optional header's size
    le(0x102, 2);
    exe.resize(exe.size() + 224, 0);
    Bytes raw;
    std::uint32_t raw_at = 0x200;
    for (int i = 0; i < 2; ++i) {
        const Bytes data = pattern(0x100 * static_cast<std::size_t>(i + 1), 20 + i);
        std::string name = ".sec" + std::to_string(i);
        name.resize(8, '\0');
        for (const char c : name) exe.push_back(static_cast<std::uint8_t>(c));
        le(data.size(), 4);
        le(0x1000 * static_cast<std::uint64_t>(i + 1), 4);
        le(data.size(), 4);
        le(raw_at, 4);
        le(0, 4);
        le(0, 4);
        le(0, 2);
        le(0, 2);
        le(0x60000020, 4);
        append(raw, data);
        raw_at += static_cast<std::uint32_t>(data.size());
    }
    exe.resize(0x200, 0);
    append(exe, raw);
    int n = 0;
    for (const std::uint32_t size : cabinets) {
        for (const char c : std::string("MSCF")) exe.push_back(static_cast<std::uint8_t>(c));
        le(0, 4);
        le(size, 4);
        append(exe, pattern(size - 12, 30 + n++));
    }
    return exe;
}

// ---- the golden file ----------------------------------------------------------------------------------------------------------------

std::string golden(const std::string& name) {
    const std::string log = fs::read_text(testing::data_path("formats/xbox/crypto/testdata/tools_goldens.txt"));
    std::string out;
    bool inside = false;
    for (const std::string_view line : text::lines(log)) {
        if (line.starts_with("== ")) inside = line.substr(3) == name;
        else if (inside) out += std::string(line) + "\n";
    }
    CB_CHECK(!out.empty());
    return out;
}

struct Case {
    std::string name;
    std::string tool;
    std::vector<std::string> args;   // "{w}" is the case's folder
    Files files;
    std::vector<std::string> outputs;
    std::vector<std::string> text_outputs;
};

// A scope in which the current folder is the case's, as the Python ran in it.
class InFolder {
public:
    explicit InFolder(const stdfs::path& folder) : before_(stdfs::current_path()) { stdfs::current_path(folder); }
    ~InFolder() { stdfs::current_path(before_); }
    InFolder(const InFolder&) = delete;
    InFolder& operator=(const InFolder&) = delete;

private:
    stdfs::path before_;
};

// Whether running the command with `args` throws an E (a failing handler's exception reaches the caller; the program's main turns it into an exit code).
template <class E>
bool fails_with(const cli::Command& command, std::vector<std::string> args) {
    try {
        (void)testing::run_command(command, std::move(args));
    } catch (const E&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

std::string forward(std::string s) { return text::replace_all(s, "\\", "/"); }

// What the native tool prints, then `exit=N`, then "file <path> <size> <sha1>" for each output.
std::string run_case(const Case& c) {
    const fs::ScopedTempDir dir{"cb-crypto-golden"};
    for (const auto& [rel, data] : c.files) {
        const stdfs::path path = dir.path() / fs::from_text(rel);
        fs::make_dirs(path.parent_path());
        fs::write_file(path, data);
    }
    const std::string w = forward(fs::to_text(dir.path()));
    std::vector<std::string> args = {c.tool};
    for (const std::string& a : c.args) args.push_back(text::replace_all(a, "{w}", w));
    std::string out;
    {
        const InFolder in(dir.path());
        out = testing::run_command(xcrypto::command(xcrypto::Environment{}), args);
    }
    out = text::replace_all(forward(out), w, "<w>");
    for (const std::string& rel : c.outputs) {
        const stdfs::path path = dir.path() / fs::from_text(rel);
        std::error_code ec;
        if (!stdfs::is_regular_file(path, ec)) {
            out += "file " + rel + " missing\n";
            continue;
        }
        Bytes data = fs::read_file(path);
        if (std::find(c.text_outputs.begin(), c.text_outputs.end(), rel) != c.text_outputs.end()) {
            Bytes lf;
            for (std::size_t i = 0; i < data.size(); ++i)
                if (!(data[i] == '\r' && i + 1 < data.size() && data[i + 1] == '\n')) lf.push_back(data[i]);
            data = lf;
        }
        out += std::format("file {} {} {}\n", rel, data.size(), cb::crypto::to_hex(cb::crypto::sha1(data)));
    }
    return out;
}

// `text_in` without its last line that starts with `prefix`.
std::string without_last_row(const std::string& text_in, std::string_view prefix) {
    std::vector<std::string> lines;
    for (const std::string_view l : text::lines(text_in)) lines.emplace_back(l);
    for (std::size_t i = lines.size(); i-- > 0;)
        if (lines[i].starts_with(prefix)) {
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(i));
            break;
        }
    std::string out;
    for (const std::string& l : lines) out += l + "\n";
    return out;
}

// The golden minus the line before `exit=` when it is the blank line print() added.
std::string without_print_newline(const std::string& g) { return text::replace_all(g, "\n\nexit=0\n", "\nexit=0\n"); }

std::string only_rows(const std::string& text_in, std::initializer_list<std::string_view> prefixes) {
    std::string out;
    for (const std::string_view l : text::lines(text_in))
        for (const std::string_view p : prefixes)
            if (l.starts_with(p)) out += std::string(l) + "\n";
    return out;
}

void check_case(const Case& c, const std::string& expected) {
    const std::string got = run_case(c);
    if (got != expected) testing::record_failure(__FILE__, __LINE__, "case " + c.name + ":\n--- native ---\n" + got + "--- python ---\n" + expected);
    testing::count_check();
}

// ---- tests --------------------------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.crypto.tool_goldens.bin2lang_writes_each_language_as_the_python_does", [] {
    const Bytes data37 = pattern(37, 1);
    const std::vector<std::pair<std::string, std::string>> languages = {{"python", "Data"}, {"c", "Buf"}, {"c++", "ExpPubKey"}, {"csharp", "Blob"},
                                                                         {"php", "payload"}, {"php-old", "payload"}, {"py", "x"}};
    for (const auto& [language, variable] : languages) {
        std::string slug = language;
        slug = text::replace_all(text::replace_all(slug, "+", "p"), "-", "_");
        check_case({"bin2lang_" + slug, "bin2lang", {"{w}/in.bin", "{w}/out.txt", "-l", language, "-b", "8", "-v", variable}, {{"in.bin", data37}}, {"out.txt"}, {"out.txt"}},
                   without_print_newline(golden("bin2lang_" + slug)));
    }
    check_case({"bin2lang_defaults", "bin2lang", {"{w}/in.bin", "{w}/out.txt"}, {{"in.bin", pattern(40, 2)}}, {"out.txt"}, {"out.txt"}},
               without_print_newline(golden("bin2lang_defaults")));
});

CB_TEST("formats.xbox.crypto.tool_goldens.bin2lang_refuses_an_unknown_language_and_no_data", [] {
    const fs::ScopedTempDir dir{"cb-crypto-golden"};
    const std::string w = forward(fs::to_text(dir.path()));
    fs::write_file(dir.path() / "in.bin", pattern(4, 1));
    fs::write_file(dir.path() / "empty.bin", Bytes{});
    const cli::Command crypto = xcrypto::command(xcrypto::Environment{});
    CB_CHECK((fails_with<UsageError>(crypto, {"bin2lang", w + "/in.bin", w + "/o.txt", "-l", "cobol"})));
    CB_CHECK((fails_with<FormatError>(crypto, {"bin2lang", w + "/empty.bin", w + "/o.txt"})));
    CB_CHECK((fails_with<NotFoundError>(crypto, {"bin2lang", w + "/missing.bin", w + "/o.txt"})));
});

CB_TEST("formats.xbox.crypto.tool_goldens.bootchain_finds_the_stages_the_python_finds", [] {
    check_case({"bootchain_full", "bootchain", {"{w}/nand.bin"}, {{"nand.bin", make_nand()}}, {}, {}}, golden("bootchain_full"));
    check_case({"bootchain_empty", "bootchain", {"{w}/nand.bin"}, {{"nand.bin", Bytes(0x1000, 0)}}, {}, {}}, golden("bootchain_empty"));
    check_case({"bootchain_short", "bootchain", {"{w}/nand.bin"}, {{"nand.bin", Bytes(0x20, 0)}}, {}, {}}, golden("bootchain_short"));
});

CB_TEST("formats.xbox.crypto.tool_goldens.bootchain_extracts_a_stage", [] {
    const fs::ScopedTempDir dir{"cb-crypto-golden"};
    const std::string w = forward(fs::to_text(dir.path()));
    const Bytes nand = make_nand();
    fs::write_file(dir.path() / "nand.bin", nand);
    const cli::Command crypto = xcrypto::command(xcrypto::Environment{});
    CB_CHECK(testing::run_command(crypto, {"bootchain", w + "/nand.bin", "--extract", "3", "--output", w + "/ce.bin"}).ends_with("exit=0\n"));
    const Bytes ce = fs::read_file(dir.path() / "ce.bin");     // stage 3 of the report: CE at 0x70000, 0x3000 bytes
    CB_CHECK_EQ(ce.size(), std::size_t{0x3000});
    CB_CHECK(std::equal(ce.begin(), ce.end(), nand.begin() + 0x70000));
    CB_CHECK((fails_with<UsageError>(crypto, {"bootchain", w + "/nand.bin", "--extract", "9", "--output", w + "/x.bin"})));
    CB_CHECK((fails_with<UsageError>(crypto, {"bootchain", w + "/nand.bin", "--extract", "1"})));
});

CB_TEST("formats.xbox.crypto.tool_goldens.cpukey_ecd_agrees_with_the_python_on_its_verdicts", [] {
    namespace ecd = xcrypto::cpukey_ecd;
    struct Row {
        std::string name;
        std::string key;
    };
    const std::vector<Row> rows = {{"cpukey_ecd_zero_key", std::string(32, '0')}, {"cpukey_ecd_not_hex", std::string(32, 'z')}, {"cpukey_ecd_short", "abcd"},
                                   {"cpukey_ecd_spaced", "00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00"}};
    // the keys the generator made are in the goldens' text (valid_N: Random(seed).randbytes(16) until the weight is right; bad_*: one bit changed); they are
    // not remade here: the arguments are read back from the case names' own keys by running the golden's verdicts against the native tool on keys that
    // differ in the same ways
    const auto parse = [](const std::string& g) {
        ecd::Validation v;
        for (const std::string_view line : text::lines(g)) {
            const auto yes = [&line] { return line.ends_with("True"); };
            if (line.starts_with("  valid ")) v.valid = yes();
            else if (line.starts_with("  length_ok")) v.length_ok = yes();
            else if (line.starts_with("  non_zero")) v.non_zero = yes();
            else if (line.starts_with("  hamming_ok")) v.hamming_ok = yes();
            else if (line.starts_with("  hamming_weight")) v.hamming_weight = *text::parse_int<int>(text::trim(line.substr(16)));
            else if (line.starts_with("  ecd_ok")) v.ecd_ok = yes();
            else if (line.starts_with("  errors")) {
                static const std::regex quoted("'([^']*)'");
                const std::string list(line.substr(8));
                for (std::sregex_iterator it(list.begin(), list.end(), quoted), end; it != end; ++it) v.errors.push_back((*it)[1]);
            }
        }
        return v;
    };
    const auto same = [](const ecd::Validation& a, const ecd::Validation& b) {
        return a.valid == b.valid && a.length_ok == b.length_ok && a.non_zero == b.non_zero && a.hamming_ok == b.hamming_ok &&
               a.hamming_weight == b.hamming_weight && a.ecd_ok == b.ecd_ok && a.errors == b.errors;
    };
    for (const Row& row : rows) CB_CHECK(same(ecd::validate(row.key), parse(golden(row.name))));
    // the valid keys: the golden's key is not in the file, so take it from the Python's own generator by the property that matters: a key made by
    // the native generator passes the verdict the Python gives a key made the same way, and each way of breaking one gives the Python's reasons
    xcrypto::xecrypt::SeededRandom random(7);
    for (int i = 0; i < 20; ++i) {
        const ecd::Key key = ecd::generate(random);
        CB_CHECK(ecd::verify_ecd(key));
        CB_CHECK_EQ(ecd::hamming_weight(key), ecd::kValidHammingWeight);
        CB_CHECK(xcrypto::xecrypt::cpu_key_valid(key));        // the console's own check agrees
        const std::string hex = text::upper(cb::crypto::to_hex(key));
        CB_CHECK(same(ecd::validate(hex), parse(golden("cpukey_ecd_valid_1"))));
        ecd::Key flipped = key;
        flipped[14] ^= 1;
        CB_CHECK(same(ecd::validate(text::lower(cb::crypto::to_hex(flipped))), parse(golden("cpukey_ecd_bad_ecd"))));
        ecd::Key changed = key;
        changed[0] ^= 1;
        const ecd::Validation v = ecd::validate(cb::crypto::to_hex(changed));
        CB_CHECK(!v.valid && !v.ecd_ok);
    }
});

CB_TEST("formats.xbox.crypto.tool_goldens.fuses_print_the_lines_as_the_python_does", [] {
    for (const auto& [name, data] : std::vector<std::pair<std::string, Bytes>>{{"fuses_pattern", pattern(0x60, 2)}, {"fuses_clean", Bytes(0x60, 0)}, {"fuses_longer", pattern(0x80, 3)}})
        check_case({name, "fuses", {"{w}/fuses.bin"}, {{"fuses.bin", data}}, {}, {}}, without_last_row(golden(name), "  cpu_key"));   // (the Python printed the CPU key again, last)
});

CB_TEST("formats.xbox.crypto.tool_goldens.fuses_refuse_too_little", [] {
    const fs::ScopedTempDir dir{"cb-crypto-golden"};
    fs::write_file(dir.path() / "f.bin", Bytes(0x5F, 1));
    CB_CHECK((fails_with<FormatError>(xcrypto::command(xcrypto::Environment{}), {"fuses", forward(fs::to_text(dir.path())) + "/f.bin"})));
});

CB_TEST("formats.xbox.crypto.tool_goldens.keyvault_reads_the_fields_the_python_reads", [] {
    const std::vector<std::tuple<std::string, std::uint32_t, unsigned>> rows = {{"retail", 2, 0x00FF}, {"devkit", 1, 0xFF00}, {"recovery", 0x40000001, 0},
                                                                                {"unknown", 0x77, 1}, {"test_retail", 0x40000002, 0xFFFF}};
    for (const auto& [name, type, region] : rows)
        check_case({"keyvault_" + name, "keyvault", {"{w}/kv.bin"}, {{"kv.bin", make_keyvault(type, region)}}, {}, {}}, golden("keyvault_" + name));
});

CB_TEST("formats.xbox.crypto.tool_goldens.flashplan_gives_the_pythons_advice", [] {
    const std::vector<std::pair<std::string, std::vector<std::string>>> rows = {
        {"flashplan_plan", {"plan", "Corona 4G", "RGH 3"}},
        {"flashplan_plan_jasper", {"plan", "Jasper 256MB", "RGH 1.2", "--class", "FreeBoot"}},
        {"flashplan_plan_retail", {"plan", "Falcon", "None", "--class", "Retail"}},
        {"flashplan_plan_devkit", {"plan", "Trinity", "DevRGL", "--class", "Devkit"}},
        {"flashplan_plan_nandmu", {"plan", "Corona", "S-RGH", "--nandmu"}},
        {"flashplan_plan_unknown", {"plan", "Mystery Box", "Argon Data", "--class", "JTAG"}},
        {"flashplan_image_type", {"image-type", "FreeBoot", "RGH 1"}},
        {"flashplan_image_type_devgl", {"image-type", "Devkit", "DevGL"}},
        {"flashplan_secure_boot", {"secure-boot", "Corona 4G"}},
        {"flashplan_secure_boot_trinity", {"secure-boot", "Trinity"}},
        {"flashplan_secure_boot_xenon", {"secure-boot", "Xenon"}},
        {"flashplan_cpld", {"cpld", "06E5E093"}},
        {"flashplan_cpld_other", {"cpld", "12345678"}},
        {"flashplan_motherboards", {"motherboards"}},
        {"flashplan_hacks", {"hacks"}}};
    for (const auto& [name, args] : rows) check_case({name, "flashplan", args, {}, {}, {}}, golden(name));
});

CB_TEST("formats.xbox.crypto.tool_goldens.smc_decrypts_encrypts_extracts_and_reads_like_the_python", [] {
    const Bytes plain = make_smc_plain();
    const Bytes encrypted = xcrypto::xecrypt::smc_encrypt(plain);
    Bytes nand = make_flash_header(0x1000, 0x1800, 0x2000, 0x4000);
    put(nand, 0x2000, encrypted);
    check_case({"smc_decrypt", "smc", {"decrypt", "{w}/smc.bin", "{w}/dec.bin"}, {{"smc.bin", encrypted}}, {"dec.bin"}, {}}, golden("smc_decrypt"));
    check_case({"smc_encrypt", "smc", {"encrypt", "{w}/smc.bin", "{w}/enc.bin"}, {{"smc.bin", plain}}, {"enc.bin"}, {}}, golden("smc_encrypt"));
    check_case({"smc_info", "smc", {"info", "{w}/smc.bin"}, {{"smc.bin", encrypted}}, {}, {}}, golden("smc_info"));
    check_case({"smc_info_no_version", "smc", {"info", "{w}/smc.bin"}, {{"smc.bin", pattern(0x500, 14)}}, {}, {}}, golden("smc_info_no_version"));
    check_case({"smc_extract", "smc", {"extract", "{w}/nand.bin", "{w}/smc.bin"}, {{"nand.bin", nand}}, {"smc.bin"}, {}}, golden("smc_extract"));
    check_case({"smc_config_nand", "smc", {"config", "{w}/nand.bin"}, {{"nand.bin", nand}}, {}, {}}, golden("smc_config_nand"));
    check_case({"smc_config_raw", "smc", {"config", "{w}/cfg.bin"}, {{"cfg.bin", pattern(0x2A0, 15)}}, {}, {}}, golden("smc_config_raw"));
    check_case({"smc_config_short", "smc", {"config", "{w}/cfg.bin"}, {{"cfg.bin", pattern(10, 16)}}, {}, {}},
               text::replace_all(golden("smc_config_short"), "None", "-"));
});

CB_TEST("formats.xbox.crypto.tool_goldens.smc_refuses_a_nand_it_cannot_read", [] {
    const fs::ScopedTempDir dir{"cb-crypto-golden"};
    const std::string w = forward(fs::to_text(dir.path()));
    fs::write_file(dir.path() / "short.bin", Bytes(0x20, 0));
    fs::write_file(dir.path() / "huge.bin", make_flash_header(0x1000, 0x5000, 0x2000, 0x8000));      // 0x5000 is over the SMC's 0x4000
    fs::write_file(dir.path() / "past.bin", make_flash_header(0x1000, 0x1000, 0x7800, 0x8000));      // runs off the end
    const cli::Command crypto = xcrypto::command(xcrypto::Environment{});
    for (const char* name : {"short.bin", "huge.bin", "past.bin"}) CB_CHECK((fails_with<FormatError>(crypto, {"smc", "extract", w + "/" + name, w + "/o.bin"})));
});

CB_TEST("formats.xbox.crypto.tool_goldens.nand_info_and_the_hypervisor_cut", [] {
    const auto head = [](const std::string& g) { return only_rows(g, {"  path", "  size", "  type"}) + "exit=0\n"; };
    const auto info = [&](const std::string& name, Bytes data) {
        Case c{name, "nand", {"info", "{w}/dump.bin"}, {{"dump.bin", std::move(data)}}, {}, {}};
        const std::string got = run_case(c);
        CB_CHECK_EQ(head(got), head(golden(name)));
    };
    info("nand_info_small", pattern(0x200, 17));
    info("nand_info_tiny", pattern(0x40, 18));
    check_case({"nand_extract_hv", "nand", {"extract-hv", "{w}/dump.bin", "{w}/hv.bin"}, {{"dump.bin", pattern(0x40100, 19)}}, {"hv.bin"}, {}}, golden("nand_extract_hv"));
    check_case({"nand_hv_alias", "nand", {"hv", "{w}/dump.bin", "{w}/hv.bin"}, {{"dump.bin", pattern(0x40000, 20)}}, {"hv.bin"}, {}}, golden("nand_hv_alias"));
});

CB_TEST("formats.xbox.crypto.tool_goldens.hypervisor_info_and_clean", [] {
    const Bytes hv = make_hypervisor(21);
    const std::string got = run_case({"hypervisor_info", "hypervisor", {"info", "{w}/hv.bin"}, {{"hv.bin", hv}}, {}, {}});
    std::string expected = golden("hypervisor_info");
    // keys_are_55 and the hash ranges are written differently; the rows the Python printed are the same values
    CB_CHECK_EQ(only_rows(got, {"  size", "  build", "  qfe", "  white_key", "  aes_key", "  hash_key", "  security_hash_100f0"}),
                only_rows(expected, {"  size", "  build", "  qfe", "  white_key", "  aes_key", "  hash_key", "  security_hash_100f0"}));
    CB_CHECK(got.find("keys_are_55          yes") != std::string::npos && expected.find("keys_are_55          True") != std::string::npos);
    // the pairs: "(92801934, 294918042)" in decimal against "0x05... 0x11..." in hex
    static const std::regex pair_pattern("\\((\\d+), (\\d+)\\)");
    std::vector<std::string> python_pairs;
    for (std::sregex_iterator it(expected.begin(), expected.end(), pair_pattern), end; it != end; ++it)
        python_pairs.push_back(std::format("0x{:08X} 0x{:08X}", std::stoul((*it)[1]), std::stoul((*it)[2])));
    CB_CHECK_EQ(python_pairs.size(), std::size_t{6});
    for (std::size_t i = 0; i < python_pairs.size(); ++i) CB_CHECK(got.find(std::format("  {:<20} {}", std::format("hash_range_{}", i), python_pairs[i])) != std::string::npos);
    check_case({"hypervisor_clean", "hypervisor", {"clean", "{w}/dumped.bin", "{w}/base.bin", "{w}/clean.bin"}, {{"dumped.bin", hv}, {"base.bin", make_hypervisor(22)}}, {"clean.bin"}, {}},
               golden("hypervisor_clean"));
    check_case({"hypervisor_clean_short_base", "hypervisor", {"clean", "{w}/dumped.bin", "{w}/base.bin", "{w}/clean.bin"}, {{"dumped.bin", hv}, {"base.bin", pattern(0x12000, 23)}},
                {"clean.bin"}, {}},
               golden("hypervisor_clean_short_base"));
});

CB_TEST("formats.xbox.crypto.tool_goldens.xval_names_the_violations_the_python_names", [] {
    // the encrypted values are the generator's (DES of the two words under the serial's key); the arguments are in the goldens' case names only by
    // their meaning, so they are made again here with the native DES and the HMAC key the tool documents
    const auto encrypted = [](const std::string& serial, std::uint32_t high, std::uint32_t low) {
        Bytes key = to_bytes(serial);
        key.push_back(0);
        const Bytes mac = xcrypto::xecrypt::hmac_sha(key, as_bytes("XBOX360SSB"));
        Bytes plain(8);
        put_be(plain, 0, high, 4);
        put_be(plain, 4, low, 4);
        xcrypto::xecrypt::BlockCipher des(xcrypto::xecrypt::CipherAlgorithm::Des, ByteView(mac).first(8), xcrypto::xecrypt::CipherMode::Ecb);
        return text::upper(cb::crypto::to_hex(des.encrypt(plain)));
    };
    const std::string serial = "123456789012";
    const std::vector<std::tuple<std::string, std::uint32_t, std::uint32_t>> rows = {
        {"clean", 0, 0}, {"invalid", 0xFFFFFFFF, 0xFFFFFFFF}, {"some_flags", 0, 0x189}, {"all_known_flags", 0, 0x1FFFF}, {"unknown_bits", 0, 0x28000}, {"high_word_only", 0x10, 0}, {"both_words", 0x10, 0x20}};
    for (const auto& [name, high, low] : rows)
        check_case({"xval_" + name, "xval", {serial, encrypted(serial, high, low)}, {}, {}, {}}, text::replace_all(golden("xval_" + name), "Intergrity", "Integrity"));
    check_case({"xval_other_serial", "xval", {"YYYYYYYYYYYY", encrypted("YYYYYYYYYYYY", 0, 0x1100)}, {}, {}, {}}, text::replace_all(golden("xval_other_serial"), "Intergrity", "Integrity"));
    std::string dashed = encrypted(serial, 0, 0x81);
    dashed = dashed.substr(0, 4) + "-" + dashed.substr(4, 4) + "-" + dashed.substr(8, 4) + "-" + dashed.substr(12, 4);
    check_case({"xval_dashes", "xval", {serial, dashed}, {}, {}, {}}, golden("xval_dashes"));
});

CB_TEST("formats.xbox.crypto.tool_goldens.xval_refuses_what_is_not_a_serial_or_a_value", [] {
    const cli::Command crypto = xcrypto::command(xcrypto::Environment{});
    CB_CHECK((fails_with<UsageError>(crypto, {"xval", "short", "0000000000000000"})));
    CB_CHECK((fails_with<UsageError>(crypto, {"xval", "123456789012", "00112233"})));
    CB_CHECK((fails_with<UsageError>(crypto, {"xval", "123456789012", "not hex at all!!"})));
});

CB_TEST("formats.xbox.crypto.tool_goldens.patch_checker_reports_the_conflicts_the_python_reports", [] {
    const Bytes table_a = make_patch_table({{0x1000, pattern(16, 24)}, {0x1008, pattern(16, 25)}, {0x2000, pattern(8, 26)}});
    const Bytes table_b = make_patch_table({{0x100, pattern(8, 27)}, {0x200, pattern(8, 28)}});
    check_case({"patch_checker_default", "patch_checker", {},
                {{"Output/NonZero/RGL_flash.bin", table_a}, {"Output/NonZero/RGL_hdd.bin", table_b}, {"Output/Zero/VRGL_flash.bin", table_b}, {"Output/Zero/VRGL_hdd.bin", table_a}}, {}, {}},
               golden("patch_checker_default"));
    // tables named on the command line are checked instead of the default four (the Python reads only the four)
    const std::string got = run_case({"patch_checker_files", "patch_checker", {"{w}/a.bin", "{w}/b.bin"}, {{"a.bin", table_a}, {"b.bin", table_b}}, {}, {}});
    CB_CHECK_EQ(got, "Patch Conflict!\nPatch A: 0x00001000 - 0x00001010\nPatch B: 0x00001008 - 0x00001018\nPatch Conflict!\nPatch A: 0x00001008 - 0x00001018\nPatch B: 0x00001000 - 0x00001010\nexit=0\n");
});

CB_TEST("formats.xbox.crypto.tool_goldens.xell_unpack_splits_the_images_like_the_python", [] {
    Case c{"xell_unpack", "xell_unpack", {}, {{"XeLL/xell-1f.bin", make_xell(1)}, {"XeLL/xell-2f.bin", make_xell(2)}, {"XeLL/xell-gggggg.bin", make_xell(3)}}, {}, {}};
    for (const char* name : {"xell-1f", "xell-2f", "xell-gggggg"})
        for (const char* suffix : {"-stage-1.bin", "-stage-2-compressed.elf", "-stage-2-decompressed.elf"}) c.outputs.push_back(std::string("XeLL/") + name + suffix);
    const std::string expected = golden("xell_unpack");
    const std::string got = run_case(c);
    // (the native tool lists the files it wrote; the Python printed nothing)
    CB_CHECK_EQ(only_rows(got, {"file ", "exit="}), only_rows(expected, {"file ", "exit="}));
    CB_CHECK(got.find("XeLL/xell-1f-stage-1.bin") != std::string::npos);
});

CB_TEST("formats.xbox.crypto.tool_goldens.xell_unpack_refuses_a_damaged_second_stage", [] {
    const fs::ScopedTempDir dir{"cb-crypto-golden"};
    Bytes image = make_xell(1);
    image[0x4000 + 40] ^= 0x55;      // inside the gzip stream
    fs::make_dirs(dir.path() / "XeLL");
    fs::write_file(dir.path() / "XeLL" / "damaged.bin", image);
    const std::string w = forward(fs::to_text(dir.path()));
    CB_CHECK((fails_with<FormatError>(xcrypto::command(xcrypto::Environment{}), {"xell_unpack", "--dir", w + "/XeLL", "damaged"})));
    fs::write_file(dir.path() / "XeLL" / "tiny.bin", Bytes(100, 1));
    CB_CHECK((fails_with<FormatError>(xcrypto::command(xcrypto::Environment{}), {"xell_unpack", "--dir", w + "/XeLL", "tiny"})));
});

CB_TEST("formats.xbox.crypto.tool_goldens.xdk_extract_writes_the_cabinets_after_the_program", [] {
    const std::string cabs[] = {"xdk_0.cab", "xdk_1.cab", "xdk_2.cab"};
    Case c{"xdk_extract", "xdk_extract", {"{w}/setup.exe", "{w}/cabs", "-p", "xdk"}, {{"setup.exe", make_pe({300, 5000, 64})}}, {}, {}};
    for (const std::string& cab : cabs) c.outputs.push_back("cabs/" + cab);
    // (the Python extracted all three and then crashed reading past the end of the file: exit 1)
    check_case(c, text::replace_all(golden("xdk_extract"), "exit=1", "exit=0"));
    check_case({"xdk_extract_no_cabs", "xdk_extract", {"{w}/setup.exe", "{w}/cabs"}, {{"setup.exe", make_pe({})}}, {}, {}}, text::replace_all(golden("xdk_extract_no_cabs"), "exit=1", "exit=0"));
});

CB_TEST("formats.xbox.crypto.tool_goldens.xdk_extract_refuses_what_is_not_an_i386_pe", [] {
    const fs::ScopedTempDir dir{"cb-crypto-golden"};
    const std::string w = forward(fs::to_text(dir.path()));
    fs::write_file(dir.path() / "text.exe", as_bytes("this is not a program, it is just text of some length............"));
    Bytes arm = make_pe({});
    arm[0x44] = 0x00;
    arm[0x45] = 0x01;        // machine 0x100: not i386
    fs::write_file(dir.path() / "arm.exe", arm);
    const cli::Command crypto = xcrypto::command(xcrypto::Environment{});
    CB_CHECK((fails_with<FormatError>(crypto, {"xdk_extract", w + "/text.exe", w + "/o"})));
    CB_CHECK((fails_with<FormatError>(crypto, {"xdk_extract", w + "/arm.exe", w + "/o"})));
});

CB_TEST("formats.xbox.crypto.tool_goldens.exp_signer_signs_like_the_python", [] {
    const Bytes prv = read_testdata("test_rsa_prv.bin");
    check_case({"exp_signer_plain", "exp_signer", {"{w}/payload.bin", "-o", "{w}/out.bin", "-k", "{w}/prv.bin"}, {{"payload.bin", pattern(100, 31)}, {"prv.bin", prv}}, {"out.bin"}, {}},
               golden("exp_signer_plain"));
    check_case({"exp_signer_xbonline", "exp_signer", {"{w}/payload.bin", "-o", "{w}/out.bin", "-k", "{w}/prv.bin", "-i", "0x48564050"}, {{"payload.bin", pattern(0x1000, 32)}, {"prv.bin", prv}},
                {"out.bin"}, {}},
               golden("exp_signer_xbonline"));
    check_case({"exp_signer_in_place", "exp_signer", {"{w}/payload.bin", "-k", "{w}/prv.bin"}, {{"payload.bin", pattern(48, 33)}, {"prv.bin", prv}}, {"payload.bin"}, {}},
               golden("exp_signer_in_place"));
});

CB_TEST("formats.xbox.crypto.tool_goldens.format_key_writes_the_header_like_the_python", [] {
    check_case({"format_key", "format_key", {}, {{"Keys/custom_pub.bin", read_testdata("test_rsa_pub.bin")}}, {"Output/Zero/ExpPubKey.h"}, {"Output/Zero/ExpPubKey.h"}},
               without_print_newline(golden("format_key")));
});

}  // namespace tool_goldens_test
