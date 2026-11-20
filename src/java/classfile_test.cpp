// Tests of java/classfile.hpp: real class files come back byte for byte, their code decodes into instructions and encodes back exactly, the names
// read, Modified UTF-8, and what is not a class file is refused with where and why.
#include "java/classfile.hpp"

#include <filesystem>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "java/console/zip_archive.hpp"

namespace cb::java::classfile {
namespace {

// Every class of a jar: parsed, written back, and each method's code decoded and encoded back; how many classes and instructions that was.
struct CfRound {
    std::size_t classes = 0, methods = 0, instructions = 0, mismatched = 0, code_mismatched = 0;
};

CfRound cf_round_trip(const std::filesystem::path& jar) {
    CfRound r;
    const console::ZipArchive zip = console::ZipArchive::open(jar);
    for (const console::ZipArchive::Entry& e : zip.entries()) {
        if (e.is_directory() || !e.name.ends_with(".class")) continue;
        const Bytes bytes = zip.read(e);
        const ClassFile c = parse(bytes, e.name);
        ++r.classes;
        if (write(c) != bytes) ++r.mismatched;
        for (const Member& m : c.methods) {
            const Attribute* code = c.find(m.attributes, "Code");
            if (code == nullptr) continue;
            ++r.methods;
            const Code decoded = decode_code(code->data);
            r.instructions += decoded.instructions.size();
            if (encode_code(decoded) != code->data) ++r.code_mismatched;
        }
    }
    return r;
}

CB_TEST("java.classfile.real_classes_come_back_byte_for_byte", [] {
    const CfRound r = cf_round_trip(testing::data_path("java/decompiler/testdata/jar/gradle-wrapper.jar"));
    CB_CHECK(r.classes == 31 && r.methods > 50 && r.instructions > 500);
    CB_CHECK_EQ(r.mismatched, 0u);
    CB_CHECK_EQ(r.code_mismatched, 0u);
    // the decompiler engine's own jar when it has been built here: hundreds of classes of every kind (switches, wide, invokedynamic)
    const std::filesystem::path big = testing::data_path("../work/fernflower-src/retromcp.jar");
    std::error_code ec;
    if (std::filesystem::is_regular_file(big, ec)) {
        const CfRound b = cf_round_trip(big);
        CB_CHECK(b.classes > 800);
        CB_CHECK_EQ(b.mismatched, 0u);
        CB_CHECK_EQ(b.code_mismatched, 0u);
    }
});

CB_TEST("java.classfile.the_names_a_class_holds", [] {
    const console::ZipArchive zip = console::ZipArchive::open(testing::data_path("java/decompiler/testdata/jar/gradle-wrapper.jar"));
    bool found = false;
    for (const console::ZipArchive::Entry& e : zip.entries()) {
        if (e.name != "org/gradle/wrapper/GradleWrapperMain.class") continue;
        const ClassFile c = parse(zip.read(e), e.name);
        CB_CHECK_EQ(c.name(), "org/gradle/wrapper/GradleWrapperMain");
        CB_CHECK_EQ(c.super_name().value_or(""), "java/lang/Object");
        bool has_main = false;
        for (const Member& m : c.methods)
            if (c.utf8(m.name) == "main" && c.utf8(m.descriptor) == "([Ljava/lang/String;)V") has_main = true;
        CB_CHECK(has_main);
        CB_CHECK(c.major >= 45);
        found = true;
    }
    CB_CHECK(found);
});

CB_TEST("java.classfile.code_decodes_into_instructions", [] {
    // max_stack 2, max_locals 1, 29 bytes of code: iconst_1; bipush -3; iadd; wide iinc 300 by -2; tableswitch (at 10, one byte of padding) 1..1 all to 28;
    // ireturn at 28. No handlers, no attributes.
    const Bytes attr = {0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1D,
                        0x04, 0x10, 0xFD, 0x60, 0xC4, 0x84, 0x01, 0x2C, 0xFF, 0xFE, 0xAA, 0x00,
                        0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x12,
                        0xAC,
                        0x00, 0x00, 0x00, 0x00};
    const Code c = decode_code(attr);
    CB_CHECK(c.max_stack == 2 && c.max_locals == 1 && c.bytes.size() == 29);
    CB_CHECK_EQ(c.instructions.size(), 6u);
    CB_CHECK_EQ(opcode_name(c.instructions[0].opcode), "iconst_1");
    CB_CHECK(c.instructions[1].opcode == 0x10 && c.instructions[1].index == -3);
    CB_CHECK(c.instructions[3].wide && c.instructions[3].opcode == 0x84 && c.instructions[3].index == 300 && c.instructions[3].extra == -2);
    const Instruction& sw = c.instructions[4];
    CB_CHECK(sw.opcode == 0xAA && sw.offset == 10 && sw.default_target == 28 && sw.low == 1 && sw.targets == std::vector<std::int32_t>({28}));
    CB_CHECK(c.instructions[5].offset == 28 && opcode_name(c.instructions[5].opcode) == "ireturn");
    CB_CHECK(encode_code(c) == attr);
    // an instruction changed in place is written as changed
    Code changed = c;
    changed.instructions[1].index = 7;
    CB_CHECK_EQ(encode_code(changed)[8 + 2], 7);
    // an unknown opcode is refused
    Bytes bad = attr;
    bad[8] = 0xCB;
    CB_CHECK_THROWS_AS(FormatError, decode_code(bad));
});

CB_TEST("java.classfile.modified_utf8", [] {
    CB_CHECK_EQ(text("plain"), "plain");
    CB_CHECK_EQ(text(std::string("a\xC0\x80" "b", 4)), std::string("a\0b", 3));                  // NUL as two bytes
    CB_CHECK_EQ(text("\xED\xA0\xBD\xED\xB8\x80"), "\xF0\x9F\x98\x80");                              // a surrogate pair -> one character
    CB_CHECK_EQ(modified("\xF0\x9F\x98\x80"), "\xED\xA0\xBD\xED\xB8\x80");
    CB_CHECK_EQ(modified(std::string("a\0b", 3)), std::string("a\xC0\x80" "b", 4));
    CB_CHECK_EQ(text(modified("caf\xC3\xA9")), "caf\xC3\xA9");
});

CB_TEST("java.classfile.what_is_not_a_class_is_refused", [] {
    CB_CHECK_THROWS_AS(FormatError, parse(Bytes{'P', 'K', 3, 4, 0, 0, 0, 0}));
    CB_CHECK_THROWS_AS(FormatError, parse(Bytes{0xCA, 0xFE, 0xBA, 0xBE, 0, 0, 0, 50, 0, 3, 1}));   // cut off in the pool
    CB_CHECK_THROWS_AS(FormatError, parse(Bytes{0xCA, 0xFE, 0xBA, 0xBE, 0, 0, 0, 50, 0, 2, 99, 0, 0}));   // an unknown tag
    try {
        (void)parse(Bytes{0xCA, 0xFE, 0xBA, 0xBE, 0, 0, 0, 50, 0, 2, 99});
    } catch (const FormatError& e) {
        CB_CHECK(std::string(e.what()).find("unknown tag") != std::string::npos);
    }
});

}  // namespace
}  // namespace cb::java::classfile
