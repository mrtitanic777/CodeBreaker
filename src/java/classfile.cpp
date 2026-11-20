// java/classfile.cpp -- see classfile.hpp.
#include "java/classfile.hpp"

#include <array>
#include <format>

#include "core/error.hpp"
#include "core/io.hpp"

namespace cb::java::classfile {

namespace {

constexpr std::string_view kCfReader = "class";

[[noreturn]] void cf_bad(std::size_t offset, const std::string& what) { throw FormatError(std::string(kCfReader), offset, what); }

Attribute cf_attribute(io::ByteReader& r) {
    Attribute a;
    a.name = r.u16_be();
    const std::uint32_t size = r.u32_be();
    const ByteView data = r.bytes(size);
    a.data.assign(data.begin(), data.end());
    return a;
}

std::vector<Attribute> cf_attributes(io::ByteReader& r) {
    const std::uint16_t count = r.u16_be();
    std::vector<Attribute> out;
    out.reserve(count);
    for (std::uint16_t i = 0; i < count; ++i) out.push_back(cf_attribute(r));
    return out;
}

void cf_put_attributes(io::ByteWriter& w, const std::vector<Attribute>& list) {
    w.u16_be(static_cast<std::uint16_t>(list.size()));
    for (const Attribute& a : list) {
        w.u16_be(a.name);
        w.u32_be(static_cast<std::uint32_t>(a.data.size()));
        w.bytes(a.data);
    }
}

// ---- the opcodes: name and operand form
//  0 none   b signed byte   B unsigned byte (a local, newarray's type)   s signed short   C u8 pool index   c u16 pool index   j branch16   J branch32
//  i iinc   I invokeinterface   D invokedynamic   M multianewarray   T tableswitch   L lookupswitch   W wide   ? not an opcode
struct CfOp {
    const char* name;
    char form;
};

constexpr std::array<CfOp, 256> cf_ops() {
    std::array<CfOp, 256> t{};
    for (CfOp& o : t) o = {"", '?'};
    const CfOp list[] = {
        {"nop", 0}, {"aconst_null", 0}, {"iconst_m1", 0}, {"iconst_0", 0}, {"iconst_1", 0}, {"iconst_2", 0}, {"iconst_3", 0}, {"iconst_4", 0},
        {"iconst_5", 0}, {"lconst_0", 0}, {"lconst_1", 0}, {"fconst_0", 0}, {"fconst_1", 0}, {"fconst_2", 0}, {"dconst_0", 0}, {"dconst_1", 0},
        {"bipush", 'b'}, {"sipush", 's'}, {"ldc", 'C'}, {"ldc_w", 'c'}, {"ldc2_w", 'c'}, {"iload", 'B'}, {"lload", 'B'}, {"fload", 'B'},
        {"dload", 'B'}, {"aload", 'B'}, {"iload_0", 0}, {"iload_1", 0}, {"iload_2", 0}, {"iload_3", 0}, {"lload_0", 0}, {"lload_1", 0},
        {"lload_2", 0}, {"lload_3", 0}, {"fload_0", 0}, {"fload_1", 0}, {"fload_2", 0}, {"fload_3", 0}, {"dload_0", 0}, {"dload_1", 0},
        {"dload_2", 0}, {"dload_3", 0}, {"aload_0", 0}, {"aload_1", 0}, {"aload_2", 0}, {"aload_3", 0}, {"iaload", 0}, {"laload", 0},
        {"faload", 0}, {"daload", 0}, {"aaload", 0}, {"baload", 0}, {"caload", 0}, {"saload", 0}, {"istore", 'B'}, {"lstore", 'B'},
        {"fstore", 'B'}, {"dstore", 'B'}, {"astore", 'B'}, {"istore_0", 0}, {"istore_1", 0}, {"istore_2", 0}, {"istore_3", 0}, {"lstore_0", 0},
        {"lstore_1", 0}, {"lstore_2", 0}, {"lstore_3", 0}, {"fstore_0", 0}, {"fstore_1", 0}, {"fstore_2", 0}, {"fstore_3", 0}, {"dstore_0", 0},
        {"dstore_1", 0}, {"dstore_2", 0}, {"dstore_3", 0}, {"astore_0", 0}, {"astore_1", 0}, {"astore_2", 0}, {"astore_3", 0}, {"iastore", 0},
        {"lastore", 0}, {"fastore", 0}, {"dastore", 0}, {"aastore", 0}, {"bastore", 0}, {"castore", 0}, {"sastore", 0}, {"pop", 0},
        {"pop2", 0}, {"dup", 0}, {"dup_x1", 0}, {"dup_x2", 0}, {"dup2", 0}, {"dup2_x1", 0}, {"dup2_x2", 0}, {"swap", 0},
        {"iadd", 0}, {"ladd", 0}, {"fadd", 0}, {"dadd", 0}, {"isub", 0}, {"lsub", 0}, {"fsub", 0}, {"dsub", 0},
        {"imul", 0}, {"lmul", 0}, {"fmul", 0}, {"dmul", 0}, {"idiv", 0}, {"ldiv", 0}, {"fdiv", 0}, {"ddiv", 0},
        {"irem", 0}, {"lrem", 0}, {"frem", 0}, {"drem", 0}, {"ineg", 0}, {"lneg", 0}, {"fneg", 0}, {"dneg", 0},
        {"ishl", 0}, {"lshl", 0}, {"ishr", 0}, {"lshr", 0}, {"iushr", 0}, {"lushr", 0}, {"iand", 0}, {"land", 0},
        {"ior", 0}, {"lor", 0}, {"ixor", 0}, {"lxor", 0}, {"iinc", 'i'}, {"i2l", 0}, {"i2f", 0}, {"i2d", 0},
        {"l2i", 0}, {"l2f", 0}, {"l2d", 0}, {"f2i", 0}, {"f2l", 0}, {"f2d", 0}, {"d2i", 0}, {"d2l", 0},
        {"d2f", 0}, {"i2b", 0}, {"i2c", 0}, {"i2s", 0}, {"lcmp", 0}, {"fcmpl", 0}, {"fcmpg", 0}, {"dcmpl", 0},
        {"dcmpg", 0}, {"ifeq", 'j'}, {"ifne", 'j'}, {"iflt", 'j'}, {"ifge", 'j'}, {"ifgt", 'j'}, {"ifle", 'j'}, {"if_icmpeq", 'j'},
        {"if_icmpne", 'j'}, {"if_icmplt", 'j'}, {"if_icmpge", 'j'}, {"if_icmpgt", 'j'}, {"if_icmple", 'j'}, {"if_acmpeq", 'j'}, {"if_acmpne", 'j'}, {"goto", 'j'},
        {"jsr", 'j'}, {"ret", 'B'}, {"tableswitch", 'T'}, {"lookupswitch", 'L'}, {"ireturn", 0}, {"lreturn", 0}, {"freturn", 0}, {"dreturn", 0},
        {"areturn", 0}, {"return", 0}, {"getstatic", 'c'}, {"putstatic", 'c'}, {"getfield", 'c'}, {"putfield", 'c'}, {"invokevirtual", 'c'}, {"invokespecial", 'c'},
        {"invokestatic", 'c'}, {"invokeinterface", 'I'}, {"invokedynamic", 'D'}, {"new", 'c'}, {"newarray", 'B'}, {"anewarray", 'c'}, {"arraylength", 0}, {"athrow", 0},
        {"checkcast", 'c'}, {"instanceof", 'c'}, {"monitorenter", 0}, {"monitorexit", 0}, {"wide", 'W'}, {"multianewarray", 'M'}, {"ifnull", 'j'}, {"ifnonnull", 'j'},
        {"goto_w", 'J'}, {"jsr_w", 'J'},
    };
    for (std::size_t i = 0; i < std::size(list); ++i) t[i] = list[i];
    return t;
}

constexpr std::array<CfOp, 256> kCfOps = cf_ops();

std::size_t cf_pad(std::size_t offset) { return (4 - (offset + 1) % 4) % 4; }   // the bytes after a switch's opcode up to a multiple of 4

}  // namespace

std::string_view tag_name(Tag tag) {
    switch (tag) {
        case Tag::None: return "none";
        case Tag::Utf8: return "Utf8";
        case Tag::Integer: return "Integer";
        case Tag::Float: return "Float";
        case Tag::Long: return "Long";
        case Tag::Double: return "Double";
        case Tag::Class: return "Class";
        case Tag::String: return "String";
        case Tag::Fieldref: return "Fieldref";
        case Tag::Methodref: return "Methodref";
        case Tag::InterfaceMethodref: return "InterfaceMethodref";
        case Tag::NameAndType: return "NameAndType";
        case Tag::MethodHandle: return "MethodHandle";
        case Tag::MethodType: return "MethodType";
        case Tag::Dynamic: return "Dynamic";
        case Tag::InvokeDynamic: return "InvokeDynamic";
        case Tag::Module: return "Module";
        case Tag::Package: return "Package";
    }
    return "?";
}

const std::string& ClassFile::utf8(std::uint16_t index) const {
    if (index == 0 || index >= pool.size() || pool[index].tag != Tag::Utf8) cf_bad(0, std::format("constant {} is not a Utf8", index));
    return pool[index].bytes;
}

const std::string& ClassFile::class_name(std::uint16_t index) const {
    if (index == 0 || index >= pool.size() || pool[index].tag != Tag::Class) cf_bad(0, std::format("constant {} is not a Class", index));
    return utf8(pool[index].a);
}

std::optional<std::string> ClassFile::super_name() const {
    if (super_class == 0) return std::nullopt;
    return class_name(super_class);
}

const Attribute* ClassFile::find(const std::vector<Attribute>& list, std::string_view name) const {
    for (const Attribute& a : list)
        if (a.name < pool.size() && pool[a.name].tag == Tag::Utf8 && pool[a.name].bytes == name) return &a;
    return nullptr;
}

ClassFile parse(ByteView data, std::string_view name) {
    io::ByteReader r(data, name);
    ClassFile c;
    try {
        if (r.u32_be() != 0xCAFEBABE) cf_bad(0, "not a class file (no CAFEBABE)");
        c.minor = r.u16_be();
        c.major = r.u16_be();
        const std::uint16_t count = r.u16_be();
        if (count == 0) cf_bad(8, "a constant pool of size 0");
        c.pool.resize(count);
        for (std::uint16_t i = 1; i < count; ++i) {
            const std::size_t at = r.pos();
            Constant& k = c.pool[i];
            k.tag = static_cast<Tag>(r.u8());
            switch (k.tag) {
                case Tag::Utf8: {
                    const std::uint16_t size = r.u16_be();
                    const ByteView s = r.bytes(size);
                    k.bytes.assign(reinterpret_cast<const char*>(s.data()), s.size());
                    break;
                }
                case Tag::Integer:
                case Tag::Float: k.value = r.u32_be(); break;
                case Tag::Long:
                case Tag::Double:
                    k.value = r.u64_be();
                    if (i + 1 >= count) cf_bad(at, "a long or a double in the pool's last slot");
                    ++i;   // (the next slot is unusable)
                    break;
                case Tag::Class:
                case Tag::String:
                case Tag::MethodType:
                case Tag::Module:
                case Tag::Package: k.a = r.u16_be(); break;
                case Tag::Fieldref:
                case Tag::Methodref:
                case Tag::InterfaceMethodref:
                case Tag::NameAndType:
                case Tag::Dynamic:
                case Tag::InvokeDynamic:
                    k.a = r.u16_be();
                    k.b = r.u16_be();
                    break;
                case Tag::MethodHandle:
                    k.kind = r.u8();
                    k.a = r.u16_be();
                    break;
                default: cf_bad(at, std::format("constant {} has the unknown tag {}", i, static_cast<int>(k.tag)));
            }
        }
        c.access = r.u16_be();
        c.this_class = r.u16_be();
        c.super_class = r.u16_be();
        const std::uint16_t interfaces = r.u16_be();
        for (std::uint16_t i = 0; i < interfaces; ++i) c.interfaces.push_back(r.u16_be());
        for (std::vector<Member>* list : {&c.fields, &c.methods}) {
            const std::uint16_t n = r.u16_be();
            list->reserve(n);
            for (std::uint16_t i = 0; i < n; ++i) {
                Member m;
                m.access = r.u16_be();
                m.name = r.u16_be();
                m.descriptor = r.u16_be();
                m.attributes = cf_attributes(r);
                list->push_back(std::move(m));
            }
        }
        c.attributes = cf_attributes(r);
    } catch (const FormatError&) {
        throw;
    } catch (const Error& e) {   // (a read past the end)
        cf_bad(r.pos(), std::string("cut off: ") + e.what());
    }
    if (!r.eof()) cf_bad(r.pos(), std::format("{} bytes after the class", r.remaining()));
    (void)c.class_name(c.this_class);   // (this_class must name a class)
    return c;
}

Bytes write(const ClassFile& c) {
    io::ByteWriter w;
    w.u32_be(0xCAFEBABE);
    w.u16_be(c.minor);
    w.u16_be(c.major);
    w.u16_be(static_cast<std::uint16_t>(c.pool.size()));
    for (std::size_t i = 1; i < c.pool.size(); ++i) {
        const Constant& k = c.pool[i];
        if (k.tag == Tag::None) continue;   // (the slot after a long or a double)
        w.u8(static_cast<std::uint8_t>(k.tag));
        switch (k.tag) {
            case Tag::Utf8:
                w.u16_be(static_cast<std::uint16_t>(k.bytes.size()));
                w.bytes(as_bytes(k.bytes));
                break;
            case Tag::Integer:
            case Tag::Float: w.u32_be(static_cast<std::uint32_t>(k.value)); break;
            case Tag::Long:
            case Tag::Double: w.u64_be(k.value); break;
            case Tag::Class:
            case Tag::String:
            case Tag::MethodType:
            case Tag::Module:
            case Tag::Package: w.u16_be(k.a); break;
            case Tag::MethodHandle:
                w.u8(k.kind);
                w.u16_be(k.a);
                break;
            default:
                w.u16_be(k.a);
                w.u16_be(k.b);
                break;
        }
    }
    w.u16_be(c.access);
    w.u16_be(c.this_class);
    w.u16_be(c.super_class);
    w.u16_be(static_cast<std::uint16_t>(c.interfaces.size()));
    for (const std::uint16_t i : c.interfaces) w.u16_be(i);
    for (const std::vector<Member>* list : {&c.fields, &c.methods}) {
        w.u16_be(static_cast<std::uint16_t>(list->size()));
        for (const Member& m : *list) {
            w.u16_be(m.access);
            w.u16_be(m.name);
            w.u16_be(m.descriptor);
            cf_put_attributes(w, m.attributes);
        }
    }
    cf_put_attributes(w, c.attributes);
    return w.take();
}

std::string text(std::string_view m) {
    std::u16string units;
    for (std::size_t i = 0; i < m.size();) {
        const auto b = static_cast<unsigned char>(m[i]);
        if (b < 0x80) {
            units += static_cast<char16_t>(b);
            ++i;
        } else if ((b & 0xE0) == 0xC0 && i + 1 < m.size()) {
            units += static_cast<char16_t>(((b & 0x1F) << 6) | (static_cast<unsigned char>(m[i + 1]) & 0x3F));
            i += 2;
        } else if ((b & 0xF0) == 0xE0 && i + 2 < m.size()) {
            units += static_cast<char16_t>(((b & 0x0F) << 12) | ((static_cast<unsigned char>(m[i + 1]) & 0x3F) << 6) | (static_cast<unsigned char>(m[i + 2]) & 0x3F));
            i += 3;
        } else {   // (not Modified UTF-8: kept as the replacement character)
            units += u'�';
            ++i;
        }
    }
    std::string out;
    for (std::size_t i = 0; i < units.size(); ++i) {
        std::uint32_t cp = units[i];
        if (cp >= 0xD800 && cp <= 0xDBFF && i + 1 < units.size() && units[i + 1] >= 0xDC00 && units[i + 1] <= 0xDFFF) {
            cp = 0x10000 + ((cp - 0xD800) << 10) + (units[i + 1] - 0xDC00);
            ++i;
        }
        if (cp < 0x80) {
            out += static_cast<char>(cp);
        } else if (cp < 0x800) {
            out += static_cast<char>(0xC0 | (cp >> 6));
            out += static_cast<char>(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            out += static_cast<char>(0xE0 | (cp >> 12));
            out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (cp & 0x3F));
        } else {
            out += static_cast<char>(0xF0 | (cp >> 18));
            out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
            out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (cp & 0x3F));
        }
    }
    return out;
}

std::string modified(std::string_view u) {
    std::string out;
    const auto unit = [&out](std::uint32_t c) {
        if (c != 0 && c < 0x80) {
            out += static_cast<char>(c);
        } else if (c < 0x800) {   // (NUL too: two bytes)
            out += static_cast<char>(0xC0 | (c >> 6));
            out += static_cast<char>(0x80 | (c & 0x3F));
        } else {
            out += static_cast<char>(0xE0 | (c >> 12));
            out += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (c & 0x3F));
        }
    };
    for (std::size_t i = 0; i < u.size();) {
        const auto b = static_cast<unsigned char>(u[i]);
        std::uint32_t cp = b;
        std::size_t n = 1;
        if ((b & 0xE0) == 0xC0) n = 2, cp = b & 0x1F;
        else if ((b & 0xF0) == 0xE0) n = 3, cp = b & 0x0F;
        else if ((b & 0xF8) == 0xF0) n = 4, cp = b & 0x07;
        for (std::size_t k = 1; k < n && i + k < u.size(); ++k) cp = (cp << 6) | (static_cast<unsigned char>(u[i + k]) & 0x3F);
        i += n;
        if (cp >= 0x10000) {
            cp -= 0x10000;
            unit(0xD800 + (cp >> 10));
            unit(0xDC00 + (cp & 0x3FF));
        } else {
            unit(cp);
        }
    }
    return out;
}

std::string_view opcode_name(std::uint8_t opcode) { return kCfOps[opcode].name; }

Code decode_code(ByteView attribute_data) {
    io::ByteReader r(attribute_data, "class.Code");
    Code code;
    try {
        code.max_stack = r.u16_be();
        code.max_locals = r.u16_be();
        const std::uint32_t size = r.u32_be();
        const ByteView bytes = r.bytes(size);
        code.bytes.assign(bytes.begin(), bytes.end());
        io::ByteReader c(bytes, "class.Code.code");
        while (!c.eof()) {
            Instruction in;
            in.offset = static_cast<std::uint32_t>(c.pos());
            in.opcode = c.u8();
            char form = kCfOps[in.opcode].form;
            if (form == 'W') {   // wide: the instruction it widens
                in.wide = true;
                in.opcode = c.u8();
                in.index = c.u16_be();
                if (in.opcode == 132) in.extra = c.i16_be();   // iinc
                else if (kCfOps[in.opcode].form != 'B' || in.opcode == 188) cf_bad(in.offset, std::format("wide {}", opcode_name(in.opcode)));
                code.instructions.push_back(std::move(in));
                continue;
            }
            switch (form) {
                case 0: break;
                case 'b': in.index = static_cast<std::int8_t>(c.u8()); break;
                case 'B':
                case 'C': in.index = c.u8(); break;
                case 's': in.index = c.i16_be(); break;
                case 'c': in.index = c.u16_be(); break;
                case 'j': in.index = static_cast<std::int32_t>(in.offset) + c.i16_be(); break;
                case 'J': in.index = static_cast<std::int32_t>(in.offset) + c.i32_be(); break;
                case 'i':
                    in.index = c.u8();
                    in.extra = static_cast<std::int8_t>(c.u8());
                    break;
                case 'I':
                    in.index = c.u16_be();
                    in.extra = c.u8();
                    (void)c.u8();
                    break;
                case 'D':
                    in.index = c.u16_be();
                    (void)c.u16_be();
                    break;
                case 'M':
                    in.index = c.u16_be();
                    in.extra = c.u8();
                    break;
                case 'T': {
                    (void)c.bytes(cf_pad(in.offset));
                    in.default_target = static_cast<std::int32_t>(in.offset) + c.i32_be();
                    in.low = c.i32_be();
                    const std::int32_t high = c.i32_be();
                    if (high < in.low || static_cast<std::int64_t>(high) - in.low > 65535) cf_bad(in.offset, "a tableswitch with a bad range");
                    for (std::int64_t k = in.low; k <= high; ++k) in.targets.push_back(static_cast<std::int32_t>(in.offset) + c.i32_be());
                    break;
                }
                case 'L': {
                    (void)c.bytes(cf_pad(in.offset));
                    in.default_target = static_cast<std::int32_t>(in.offset) + c.i32_be();
                    const std::int32_t pairs = c.i32_be();
                    if (pairs < 0 || pairs > 65535) cf_bad(in.offset, "a lookupswitch with a bad count");
                    for (std::int32_t k = 0; k < pairs; ++k) {
                        in.keys.push_back(c.i32_be());
                        in.targets.push_back(static_cast<std::int32_t>(in.offset) + c.i32_be());
                    }
                    break;
                }
                default: cf_bad(in.offset, std::format("the unknown opcode {}", in.opcode));
            }
            code.instructions.push_back(std::move(in));
        }
        const std::uint16_t handlers = r.u16_be();
        for (std::uint16_t i = 0; i < handlers; ++i) {
            ExceptionEntry e;
            e.start = r.u16_be();
            e.end = r.u16_be();
            e.handler = r.u16_be();
            e.catch_type = r.u16_be();
            code.exceptions.push_back(e);
        }
        code.attributes = cf_attributes(r);
    } catch (const FormatError&) {
        throw;
    } catch (const Error& e) {
        cf_bad(r.pos(), std::string("Code cut off: ") + e.what());
    }
    return code;
}

Bytes encode_code(const Code& code) {
    io::ByteWriter c;
    for (const Instruction& in : code.instructions) {
        if (c.size() != in.offset) cf_bad(in.offset, std::format("{} is at {} now: an instruction before it changed its size", opcode_name(in.opcode), c.size()));
        if (in.wide) {
            c.u8(196);
            c.u8(in.opcode);
            c.u16_be(static_cast<std::uint16_t>(in.index));
            if (in.opcode == 132) c.u16_be(static_cast<std::uint16_t>(static_cast<std::int16_t>(in.extra)));
            continue;
        }
        c.u8(in.opcode);
        const std::int32_t at = static_cast<std::int32_t>(in.offset);
        switch (kCfOps[in.opcode].form) {
            case 0: break;
            case 'b':
            case 'B':
            case 'C': c.u8(static_cast<std::uint8_t>(in.index)); break;
            case 's':
            case 'c': c.u16_be(static_cast<std::uint16_t>(in.index)); break;
            case 'j': c.u16_be(static_cast<std::uint16_t>(static_cast<std::int16_t>(in.index - at))); break;
            case 'J': c.u32_be(static_cast<std::uint32_t>(in.index - at)); break;
            case 'i':
                c.u8(static_cast<std::uint8_t>(in.index));
                c.u8(static_cast<std::uint8_t>(static_cast<std::int8_t>(in.extra)));
                break;
            case 'I':
                c.u16_be(static_cast<std::uint16_t>(in.index));
                c.u8(static_cast<std::uint8_t>(in.extra));
                c.u8(0);
                break;
            case 'D':
                c.u16_be(static_cast<std::uint16_t>(in.index));
                c.u16_be(0);
                break;
            case 'M':
                c.u16_be(static_cast<std::uint16_t>(in.index));
                c.u8(static_cast<std::uint8_t>(in.extra));
                break;
            case 'T':
                for (std::size_t k = cf_pad(in.offset); k > 0; --k) c.u8(0);
                c.u32_be(static_cast<std::uint32_t>(in.default_target - at));
                c.u32_be(static_cast<std::uint32_t>(in.low));
                c.u32_be(static_cast<std::uint32_t>(in.low + static_cast<std::int32_t>(in.targets.size()) - 1));
                for (const std::int32_t t : in.targets) c.u32_be(static_cast<std::uint32_t>(t - at));
                break;
            case 'L':
                for (std::size_t k = cf_pad(in.offset); k > 0; --k) c.u8(0);
                c.u32_be(static_cast<std::uint32_t>(in.default_target - at));
                c.u32_be(static_cast<std::uint32_t>(in.targets.size()));
                for (std::size_t k = 0; k < in.targets.size(); ++k) {
                    c.u32_be(static_cast<std::uint32_t>(in.keys[k]));
                    c.u32_be(static_cast<std::uint32_t>(in.targets[k] - at));
                }
                break;
            default: cf_bad(in.offset, std::format("the unknown opcode {}", in.opcode));
        }
    }
    const Bytes body = c.take();
    io::ByteWriter w;
    w.u16_be(code.max_stack);
    w.u16_be(code.max_locals);
    w.u32_be(static_cast<std::uint32_t>(body.size()));
    w.bytes(body);
    w.u16_be(static_cast<std::uint16_t>(code.exceptions.size()));
    for (const ExceptionEntry& e : code.exceptions) {
        w.u16_be(e.start);
        w.u16_be(e.end);
        w.u16_be(e.handler);
        w.u16_be(e.catch_type);
    }
    cf_put_attributes(w, code.attributes);
    return w.take();
}

}  // namespace cb::java::classfile
