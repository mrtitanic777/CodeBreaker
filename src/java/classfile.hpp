// java/classfile.hpp -- a Java class file (JVMS chapter 4) read into a tree and written back. The first piece of CodeBreaker's own Java bytecode
// toolchain: what the Minecraft decompile pipeline reads, remaps and decompiles is class files, and this is where they are read.
//
//     const classfile::ClassFile c = classfile::parse(bytes);          // FormatError for a file that is not a class or is damaged
//     c.name();                                                        // "net/minecraft/client/Minecraft"
//     for (const classfile::Member& m : c.methods) c.utf8(m.name);     // "run", "<init>" ...
//     classfile::write(c) == bytes                                     // unchanged, the bytes come back exactly
//
// Every part is kept as it was: the constant pool in its order (a long or a double takes two slots, as in the file), the strings as their Modified
// UTF-8 bytes (text() turns one into UTF-8), each attribute as its bytes. A method's Code attribute is decoded on demand into instructions with their
// operands (decode_code), and encoded back (encode_code), so later passes can change bytecode and descriptors without losing anything they do not touch.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"

namespace cb::java::classfile {

enum class Tag : std::uint8_t {
    None = 0,   // index 0, and the slot after a long or a double
    Utf8 = 1, Integer = 3, Float = 4, Long = 5, Double = 6, Class = 7, String = 8, Fieldref = 9, Methodref = 10, InterfaceMethodref = 11,
    NameAndType = 12, MethodHandle = 15, MethodType = 16, Dynamic = 17, InvokeDynamic = 18, Module = 19, Package = 20,
};
std::string_view tag_name(Tag tag);

// One constant. `bytes`: a Utf8's Modified UTF-8. `value`: an Integer's or Float's 32 bits, a Long's or Double's 64. `a`, `b`: the indexes a constant
// names (Class: a = name; String: a = utf8; the refs: a = class, b = name-and-type; NameAndType: a = name, b = descriptor; MethodHandle: kind, a =
// reference; MethodType: a = descriptor; Dynamic/InvokeDynamic: a = bootstrap method, b = name-and-type; Module/Package: a = name).
struct Constant {
    Tag tag = Tag::None;
    std::string bytes;
    std::uint64_t value = 0;
    std::uint16_t a = 0, b = 0;
    std::uint8_t kind = 0;
};

struct Attribute {
    std::uint16_t name = 0;   // a Utf8 index
    Bytes data;
};

struct Member {   // a field or a method
    std::uint16_t access = 0, name = 0, descriptor = 0;
    std::vector<Attribute> attributes;
};

struct ClassFile {
    std::uint16_t minor = 0, major = 0;
    std::vector<Constant> pool;   // pool[0] is unused, as in the file
    std::uint16_t access = 0, this_class = 0, super_class = 0;
    std::vector<std::uint16_t> interfaces;
    std::vector<Member> fields, methods;
    std::vector<Attribute> attributes;

    // A Utf8 constant's bytes; FormatError when `index` is not one.
    const std::string& utf8(std::uint16_t index) const;
    // A Class constant's name ("java/lang/Object"); FormatError when `index` is not one.
    const std::string& class_name(std::uint16_t index) const;
    std::string name() const { return class_name(this_class); }
    std::optional<std::string> super_name() const;   // nullopt for java/lang/Object (index 0)
    // The attribute of that name, or null.
    const Attribute* find(const std::vector<Attribute>& list, std::string_view name) const;
};

// The class in `data` (the Java SE 25 format and every earlier one). FormatError: not a class file, a damaged pool or a cut-off file.
ClassFile parse(ByteView data, std::string_view name = "class");
// The class file's bytes.
Bytes write(const ClassFile& c);

// Modified UTF-8 (a NUL as two bytes, a character beyond the BMP as two three-byte surrogates) as UTF-8, and back.
std::string text(std::string_view modified_utf8);
std::string modified(std::string_view utf8);

// ---- bytecode

// An opcode's mnemonic ("invokevirtual"); "" for one the JVM does not define.
std::string_view opcode_name(std::uint8_t opcode);

struct Instruction {
    std::uint32_t offset = 0;          // from the start of the code
    std::uint8_t opcode = 0;
    bool wide = false;                 // after a `wide` prefix (its index and increment are 16 bits)
    std::int32_t index = 0;            // a pool index, a local variable, a branch target (absolute offset), the byte or short pushed, newarray's type
    std::int32_t extra = 0;            // iinc's increment, invokeinterface's count, multianewarray's dimensions
    std::int32_t default_target = 0;   // tableswitch, lookupswitch: absolute offsets
    std::int32_t low = 0;              // tableswitch
    std::vector<std::int32_t> keys;    // lookupswitch
    std::vector<std::int32_t> targets; // tableswitch, lookupswitch (absolute offsets)
};

struct ExceptionEntry {
    std::uint16_t start = 0, end = 0, handler = 0, catch_type = 0;
};

struct Code {
    std::uint16_t max_stack = 0, max_locals = 0;
    Bytes bytes;                                // the code as it was
    std::vector<Instruction> instructions;      // decoded from `bytes`
    std::vector<ExceptionEntry> exceptions;
    std::vector<Attribute> attributes;          // LineNumberTable, LocalVariableTable, StackMapTable ...
};

// A Code attribute's contents decoded. FormatError for an unknown opcode or code that runs off its end.
Code decode_code(ByteView attribute_data);
// The attribute's contents again. The instructions are encoded (not `bytes`): an instruction changed in place is written as changed, at the same offset
// (an instruction whose size changes moves what follows and is not supported: FormatError).
Bytes encode_code(const Code& code);

}  // namespace cb::java::classfile
