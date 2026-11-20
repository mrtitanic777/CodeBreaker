// decompile/c_text_text.cpp -- the C text c_text writes of its own: the headers' constants, the declarations of the
// structures recovered, the types of the globals, the helpers of the software interrupts a 16-bit module takes.
#include <algorithm>
#include <cstdlib>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/c_text_internal.hpp"

namespace cb::decompile::c_text {

namespace {

#include "decompile/c_text_consts.inc"

constexpr std::string_view kCtCFlags[] = {"-bt=nt", "-zq", "-w0", "-s", "-zastd=c99", "-ecd"};
constexpr std::string_view kCtInterruptRegisters[] = {"ax", "bx", "cx", "dx", "si", "di", "ds", "es"};
constexpr std::string_view kCtLibm[] = {"sqrt", "sin", "cos", "tan", "atan2", "pow", "ldexp", "log", "floor", "fmod"};

bool ct_is_word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

bool ct_is_hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }

// The types a header declares: the word before the `;` of each `typedef ...;` that holds no parenthesis.
std::set<std::string> ct_typedef_names(std::string_view header) {
    std::set<std::string> names;
    std::size_t at = 0;
    while ((at = header.find("typedef ", at)) != std::string_view::npos) {
        const std::size_t from = at + 8;
        std::size_t end = from;
        while (end < header.size() && header[end] != ';' && header[end] != '(' && header[end] != ')') ++end;
        at = from;
        if (end >= header.size() || header[end] != ';') continue;
        std::size_t word = end;
        while (word > from && ct_is_word_char(header[word - 1])) --word;
        if (word < end) names.emplace(header.substr(word, end - word));
    }
    return names;
}

std::string ct_hex(std::int64_t v) { return std::format("{:x}", v); }

}  // namespace

std::span<const std::string_view> cflags() { return kCtCFlags; }
std::string_view header16() { return HEADER_TEXT; }
std::string_view seg_ptr() { return SEG_PTR_TEXT; }
std::span<const std::string_view> interrupt_registers() { return kCtInterruptRegisters; }

const OrderedMap<std::string, std::string>& intrinsics16() {
    static const OrderedMap<std::string, std::string> table = {
        {"in", "uint1 in(uint2 port);\n#pragma aux in = 0xEC parm [dx] value [al];"},
        {"out", "void out(uint2 port, uint1 value);\n#pragma aux out = 0xEE parm [dx] [al];"},
        {"LOCK", "#define LOCK()    /* the next instruction carries a LOCK prefix */"},
        {"UNLOCK", "#define UNLOCK()  /* (the end of that instruction) */"},
        // LSL: the limit of a selector's segment, 32 bits (lsl eax,eax; edx = its high half)
        {"SegmentLimit",
         "uint4 SegmentLimit(uint2 selector);\n#pragma aux SegmentLimit = 0x66 0x0f 0x03 0xc0 "
         "0x66 0x8b 0xd0 0x66 0xc1 0xea 0x10 parm [ax] value [dx ax] modify exact [ax dx];"},
        {"POPCOUNT", "static uint2 POPCOUNT(uint4 x) { uint2 n = 0; while (x) { n += (uint2)(x & 1); x >>= 1; } return n; }"},
    };
    return table;
}

Policy Policy::from_environment() {
    Policy p;
    p.selector_structs = fs::get_env("CB_SELECTOR_STRUCTS").value_or("") != "0";
    p.code_data = fs::get_env("CB_CODE_DATA").value_or("") != "0";
    p.prototypes = fs::get_env("CB_PROTOTYPES").value_or("1") != "0";
    return p;
}

// ---- the helpers of the interrupts -----------------------------------------------------------------------------------------

std::string interrupt_helper_name(std::uint32_t vector, std::span<const std::string> registers) {
    return std::format("int{:02x}_{}", vector, registers.empty() ? std::string("none") : text::join(registers, "_"));
}

std::vector<std::string> interrupt_helpers(const Interrupts& used) {
    std::vector<std::string> out;
    if (used.empty()) return out;
    out.push_back("struct codebreaker_registers { uint2 ax, bx, cx, dx, si, di, ds, es; uint1 cf, zf; uint2 set; };");
    out.push_back("extern struct codebreaker_registers codebreaker_int;   /* as the last interrupt left them */");
    std::vector<std::string> regs(std::begin(kCtInterruptRegisters), std::end(kCtInterruptRegisters));
    regs.push_back("cf");
    regs.push_back("zf");
    for (const std::string& r : regs) out.push_back("#define int_" + text::upper(r) + " codebreaker_int." + r);
    for (const char* r : {"ax", "bx", "cx", "dx"}) {   // and their bytes: AL AH, BL BH ...
        const std::string name = r;
        const std::string up = text::upper(name.substr(0, 1));
        out.push_back("#define int_" + up + "L (*(uint1 *)&codebreaker_int." + name + ")");
        out.push_back("#define int_" + up + "H (*((uint1 *)&codebreaker_int." + name + " + 1))");
    }
    std::set<std::uint32_t> vectors;
    for (const Interrupt& i : used) vectors.insert(i.vector);
    for (const std::uint32_t v : vectors) out.push_back(std::format("void __far codebreaker_int{:02x}(void);              /* INT {:02X}h */", v, v));
    for (const Interrupt& i : used) {
        std::vector<std::string> params, sets;
        std::uint32_t mask = 0;
        for (const std::string& r : i.registers) {
            params.push_back("uint2 " + r);
            sets.push_back("codebreaker_int." + r + " = " + r + ";");
            const auto* where = std::find(std::begin(kCtInterruptRegisters), std::end(kCtInterruptRegisters), r);
            CB_ASSERT(where != std::end(kCtInterruptRegisters), "an interrupt sets a register the record has no place for: " + r);
            mask += 1u << (where - std::begin(kCtInterruptRegisters));
        }
        out.push_back("static uint4 " + interrupt_helper_name(i.vector, i.registers) + "(" + (params.empty() ? std::string("void") : text::join(params, ", ")) +
                      ") { " + text::join(sets, " ") + std::format(" codebreaker_int.set = 0x{:02x}; codebreaker_int{:02x}(); ", mask, i.vector) +
                      "return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }");
    }
    return out;
}

// codebreaker_int<n> loads the registers the helper set (codebreaker_int.set, a bit each in the registers' order) from
// codebreaker_int -- the others keep what they hold, DS and ES the caller's -- takes INT n, and stores every register and CF and ZF
// back.
InterruptRoutines interrupt_routines(const std::set<std::uint32_t>& vectors) {
    std::map<std::string, int> off, bit;
    for (int i = 0; i < 8; ++i) {
        off[std::string(kCtInterruptRegisters[i])] = 2 * i;
        bit[std::string(kCtInterruptRegisters[i])] = 1 << i;
    }
    std::vector<std::string> out{"; the software interrupts the module takes, each as the original takes it: the registers",
                                 "; from codebreaker_int, INT n, the registers and the carry and zero flags back into it",
                                 ".286", "DGROUP group _DATA", "_DATA segment word public 'DATA'", "        extrn _codebreaker_int:word",
                                 "_DATA ends", "_TEXT segment word public 'XCODE'", "        assume cs:_TEXT, ds:DGROUP"};
    for (const std::uint32_t v : vectors) {
        const std::string name = std::format("_codebreaker_int{:02x}", v);
        for (const std::string& l : {"        public " + name, name + " proc far", std::string("        push bp"), std::string("        push si"),
                                     std::string("        push di"), std::string("        push ds"), std::string("        push es"),
                                     std::string("        mov  bp, ds                                   ; DGROUP, to come back to"),
                                     std::string("        push ds                                       ; DS as the caller has it")})
            out.push_back(l);
        constexpr const char* kOrder[] = {"es", "ax", "bx", "cx", "dx", "si", "di", "ds"};
        for (int k = 0; k < 8; ++k) {
            const std::string r = kOrder[k];
            const std::string skip = std::format("L_{:02x}_{}", v, k);
            out.push_back("        test word ptr _codebreaker_int+18, " + std::to_string(bit[r]));
            out.push_back("        jz   short " + skip);
            if (r == "ds") {   // (DS last: it is what the others are read through)
                out.push_back("        pop  ax");
                out.push_back("        push word ptr _codebreaker_int+" + std::to_string(off["ds"]));
            } else {
                out.push_back("        mov  " + r + ", word ptr _codebreaker_int+" + std::to_string(off[r]));
            }
            out.push_back(skip + ":");
        }
        const std::string lax = std::format("L_{:02x}_ax", v);
        for (const std::string& l : {"        test word ptr _codebreaker_int+18, " + std::to_string(bit["ax"]), "        jz   short " + lax,
                                     "        mov  ax, word ptr _codebreaker_int+" + std::to_string(off["ax"]), lax + ":", std::string("        pop  ds"),
                                     std::format("        int  {:03X}h", v), std::string("        pushf"), std::string("        push ds"),
                                     std::string("        mov  ds, bp")})
            out.push_back(l);
        for (const char* r : {"ax", "bx", "cx", "dx", "si", "di", "es"})
            out.push_back("        mov  word ptr _codebreaker_int+" + std::to_string(off[r]) + ", " + r);
        for (const std::string& l : {"        pop  word ptr _codebreaker_int+" + std::to_string(off["ds"]), std::string("        pop  ax"),
                                     std::string("        mov  byte ptr _codebreaker_int+16, al"), std::string("        and  byte ptr _codebreaker_int+16, 1"),
                                     std::string("        shr  ax, 6"), std::string("        and  al, 1"),
                                     std::string("        mov  byte ptr _codebreaker_int+17, al"), std::string("        pop  es"),
                                     std::string("        pop  ds"), std::string("        pop  di"), std::string("        pop  si"),
                                     std::string("        pop  bp"), std::string("        retf"), name + " endp"})
            out.push_back(l);
    }
    out.push_back("_TEXT ends");
    out.push_back("end");
    out.push_back("");
    return {text::join(out, "\n"), "#include \"m.h\"\nstruct codebreaker_registers codebreaker_int;\n"};
}

// ---- strings and selectors ---------------------------------------------------------------------------------------------------

std::optional<std::string> string_literal(ByteView chunk) {
    if (chunk.size() < 2 || chunk.back() != 0) return std::nullopt;
    std::size_t printable = 0;
    for (const std::uint8_t c : chunk)
        if (c >= 32 && c < 127) ++printable;
    if (printable * 2 < chunk.size()) return std::nullopt;
    for (const std::uint8_t c : chunk)
        if (c != 0 && !(c >= 32 && c < 127) && c != 9 && c != 10 && c != 13) return std::nullopt;
    std::string out = "\"";
    for (std::size_t i = 0; i + 1 < chunk.size(); ++i) {
        switch (chunk[i]) {
            case 0: out += "\\000"; break;
            case 9: out += "\\t"; break;
            case 10: out += "\\n"; break;
            case 13: out += "\\r"; break;
            case 34: out += "\\\""; break;
            case 92: out += "\\\\"; break;
            case 63: out += "\\?"; break;
            default: out.push_back(static_cast<char>(chunk[i]));
        }
    }
    return out + "\"";
}

SharedSelectorStructs shared_selector_structs(const translate::native_l2::Library& library,
                                              const std::map<std::string, std::map<std::string, std::map<std::int64_t, std::string>>>& shared) {
    SharedSelectorStructs out;
    for (const auto& [name, per] : shared) {   // (std::map: sorted by name, as the module's header is)
        std::map<std::int64_t, std::string> every;
        bool agree = true;
        for (const auto& [function, fields] : per) {
            for (const auto& [offset, type] : fields) {
                const auto [slot, added] = every.try_emplace(offset, type);
                if (!added && slot->second != type) {
                    agree = false;
                    break;
                }
            }
            if (!agree) break;
        }
        auto layout = [&](const std::map<std::int64_t, std::string>& fields) {
            OrderedMap<std::int64_t, std::string> in;
            for (const auto& [offset, type] : fields) in.insert_or_assign(offset, type);
            return library.seg_layout(in);
        };
        std::optional<std::vector<std::string>> body;
        if (agree) body = layout(every);
        if (body) {
            out.declarations.push_back("typedef _Packed struct " + name + " { " + text::join(*body, " ") + " } " + name + ";");
            continue;
        }
        for (const auto& [function, fields] : per) {
            const std::string own = "seg_" + text::sanitize_identifier(function) + "_" + name.substr(std::min<std::size_t>(4, name.size()));
            const std::optional<std::vector<std::string>> own_body = layout(fields);
            if (!own_body) continue;   // (fields that overlap have no declaration to give)
            out.declarations.push_back("typedef _Packed struct " + own + " { " + text::join(*own_body, " ") + " } " + own + ";");
            out.renames[function][name] = own;
        }
    }
    return out;
}

std::map<std::string, std::int64_t> call_counts(const std::map<std::string, std::set<std::int64_t>>& calls, const std::set<std::string>& names) {
    std::map<std::string, std::int64_t> out;
    for (const auto& [callee, counts] : calls)
        if (names.contains(callee) && counts.size() == 1) out[callee] = *counts.begin();
    return out;
}

// ---- internal ---------------------------------------------------------------------------------------------------------------

namespace internal {

std::string_view types_text() { return TYPES_TEXT; }
std::string_view helpers_text() { return HELPERS_TEXT; }
std::string_view powerpc_text() { return POWERPC_TEXT; }
std::span<const std::string_view> libm_names() { return kCtLibm; }

std::string by_size(std::int64_t size, std::string_view fallback) {
    switch (size) {
        case 1: return "uint1";
        case 2: return "uint2";
        case 4: return "uint4";
        case 8: return "uint8";
        default: return std::string(fallback);
    }
}

bool is_by_size_type(std::string_view type) { return type == "uint1" || type == "uint2" || type == "uint4" || type == "uint8"; }

const std::string& x86_helpers() {
    static const std::string helpers = [] {
        const std::string have(helpers_text());
        auto kind = [](int n, bool is_signed) {
            const int w = n == 1 ? 1 : n == 2 ? 2 : n <= 4 ? 4 : 8;
            return std::string(is_signed ? "int" : "uint") + std::to_string(w);
        };
        auto mask = [](int n) {
            std::string s = "0x";
            for (int i = 0; i < n; ++i) s += "FF";
            return s + "ULL";
        };
        std::vector<std::string> out;
        for (int a = 1; a < 8; ++a) {
            for (int b = 1; b < 9 - a; ++b) {
                if (!internal::has(have, std::format("CONCAT{}{}(", a, b)))
                    out.push_back(std::format("#define CONCAT{}{}(a,b) ((", a, b) + kind(a + b, false) + ")((((uint8)(a) & " + mask(a) +
                                  std::format(") << {}) | ((uint8)(b) & ", 8 * b) + mask(b) + ")))");
            }
        }
        for (int a = 2; a < 9; ++a) {
            for (int b = 1; b < a; ++b) {
                if (!internal::has(have, std::format("SUB{}{}(", a, b)))
                    out.push_back(std::format("#define SUB{}{}(x,n) ((", a, b) + kind(b, false) + ")((((uint8)(x) & " + mask(a) + ") >> (8 * (n))) & " +
                                  mask(b) + "))");
                if (!internal::has(have, std::format("ZEXT{}{}(", b, a)))
                    out.push_back(std::format("#define ZEXT{}{}(x) ((", b, a) + kind(a, false) + ")((uint8)(x) & " + mask(b) + "))");
                if (!internal::has(have, std::format("SEXT{}{}(", b, a)) && (b == 1 || b == 2 || b == 4))
                    out.push_back(std::format("#define SEXT{}{}(x) ((", b, a) + kind(a, true) + ")(" + kind(b, true) + ")(x))");
            }
        }
        for (const char* x : {
                 "double sqrt(double); double sin(double); double cos(double); double tan(double);",
                 "double atan2(double, double); double pow(double, double); double ldexp(double, int);",
                 "double log(double); double floor(double); double fmod(double, double);",
                 // (to the nearest whole number, halves to the even one -- written out, so that a
                 // module that never rounds does not have the mathematics library linked into it)
                 "#define ROUND(x) (((double)(x) - floor((double)(x)) > 0.5 || ((double)(x) - floor((double)(x)) == 0.5 && "
                 "fmod(floor((double)(x)), 2.0) != 0)) ? floor((double)(x)) + 1 : floor((double)(x)))",
                 "#define SQRT(x) sqrt((double)(x))", "#define ABS(x) ((x) < 0 ? -(x) : (x))", "#define NAN(x) ((x) != (x))",
                 "#define fsin(x) sin((double)(x))", "#define fcos(x) cos((double)(x))", "#define fptan(x) tan((double)(x))",
                 "#define fpatan(a,b) atan2((double)(a), (double)(b))", "#define f2xm1(x) (pow(2.0, (double)(x)) - 1.0)",
                 "#define fscale(a,b) ldexp((double)(a), (int)(b))", "#define log2(x) (log((double)(x)) / 0.6931471805599453)"})
            out.push_back(x);
        return text::join(out, "\n") + "\n";
    }();
    return helpers;
}

std::map<std::string, std::int64_t> struct_sizes(const std::vector<StructNote>& structs) {
    std::map<std::string, std::int64_t> out;
    for (const StructNote& s : structs) out.try_emplace(s.name, s.bytes);
    return out;
}

std::string lines_of(const std::vector<std::string>& v) {
    std::string out;
    for (const std::string& t : v) out += t + "\n";
    return out;
}

std::vector<std::string> struct_decls(const std::vector<StructNote>& structs, std::string_view header, bool far16) {
    struct Layout {
        std::int64_t size = 0;
        std::vector<core16::StructField> fields;
    };
    OrderedMap<std::string, Layout> found;   // (declared in the order the decompiler found them)
    for (const StructNote& s : structs) {
        if (found.contains(s.name)) continue;
        Layout layout{s.bytes, s.fields};
        std::sort(layout.fields.begin(), layout.fields.end(), [](const core16::StructField& a, const core16::StructField& b) {
            return std::tie(a.offset, a.bytes, a.type) < std::tie(b.offset, b.bytes, b.type);
        });
        found.insert_or_assign(s.name, std::move(layout));
    }
    std::vector<std::string> out;
    if (found.empty()) return out;
    std::set<std::string> known = ct_typedef_names(header);
    for (const char* w : {"char", "short", "int", "long", "float", "double", "void", "code"}) known.insert(w);
    for (const auto& [name, layout] : found) known.insert(name);
    for (const auto& [name, layout] : found) out.push_back("typedef struct " + name + " " + name + ";");
    for (const auto& [name, layout] : found) {
        std::vector<std::string> body;
        std::int64_t at = 0;
        for (const core16::StructField& f : layout.fields) {
            if (f.offset < at || f.offset + f.bytes > layout.size) continue;   // (overlapping or outside: left as bytes)
            if (f.offset > at) body.push_back("uint1 _pad_" + ct_hex(at) + "[" + std::to_string(f.offset - at) + "];");
            const std::size_t stars_at = f.type.find_last_not_of("*^") + 1;   // the type's word, then its stars
            std::string base = f.type.substr(0, stars_at);
            const std::string stars = f.type.substr(stars_at);
            const bool whole_width = f.bytes == 1 || f.bytes == 2 || f.bytes == 4 || f.bytes == 8;
            if (!known.contains(base)) base = (stars.empty() && whole_width) ? "xunknown" + std::to_string(f.bytes) : "xunknown1";
            if (stars.empty() && found.contains(base))   // (a structure held whole: its bytes)
                base = whole_width ? "xunknown" + std::to_string(f.bytes) : "xunknown1";
            std::string pointer;
            for (const char c : stars) pointer += (c == '^') ? " __far *" : " *";
            body.push_back(base + pointer + " f_" + ct_hex(f.offset) + ";");
            at = f.offset + f.bytes;
        }
        if (layout.size > at) body.push_back("uint1 _pad_" + ct_hex(at) + "[" + std::to_string(layout.size - at) + "];");
        out.push_back(std::string(far16 ? "_Packed " : "") + "struct " + name + " { " + text::join(body, " ") + " };");
    }
    return out;
}

std::optional<std::pair<std::string, std::string>> parse_ram_name(std::string_view name) {
    std::size_t letters = 0;
    while (letters < name.size() && name[letters] >= 'a' && name[letters] <= 'z') ++letters;
    const std::string_view rest = name.substr(letters);
    if (letters == 0 || rest.size() != 11 || rest.substr(0, 3) != "Ram") return std::nullopt;
    for (const char c : rest.substr(3))
        if (!ct_is_hex_digit(c)) return std::nullopt;
    return std::make_pair(std::string(name.substr(0, letters)), std::string(rest.substr(3)));
}

bool is_underscored_ram_name(std::string_view name) { return name.size() > 1 && name[0] == '_' && parse_ram_name(name.substr(1)).has_value(); }

std::string ram_type(std::string_view prefix, std::int64_t size, bool called) {
    const std::size_t stars = prefix.find_first_not_of('p') == std::string_view::npos ? prefix.size() : prefix.find_first_not_of('p');
    std::string_view base = prefix.substr(stars);
    if (base.empty()) base = "x";
    const std::string star_text(stars, '*');
    if (stars != 0) {
        if (base[0] == 'c') return called ? "code " + star_text : "char " + star_text;
        const char* type = base[0] == 'i' ? "int4" : base[0] == 'u' ? "uint4" : base[0] == 'b' ? "bool" : base[0] == 'f' ? "float4" : "xunknown4";
        return std::string(type) + " " + star_text;
    }
    if (called) return "code *";
    if (base[0] == 'i') return "int" + std::to_string(size);
    if (base[0] == 'c') return "char";
    return by_size(size);
}

std::string global_type16(std::string_view prefix, std::int64_t size, std::optional<std::int64_t> elem) {
    if (prefix[0] == 'p' && prefix.size() > 1) {   // a pointer, by the decompiler's own naming (piRam: int *)
        const std::string_view body = prefix.substr(prefix.find_first_not_of('p') == std::string_view::npos ? prefix.size() : prefix.find_first_not_of('p'));
        const char first = body.empty() ? '\0' : body[0];
        std::string to = first == 'i' ? "int2" : first == 'u' ? "uint2" : first == 'c' ? "char" : first == 'b' ? "bool" : "xunknown2";
        // (px..: a pointer to an unknown of 1, 2 or 4 bytes -- the core says which; `p + 2` counts in those: USER's
        // DisableOEMLayer read [[12Eh]+4] for the original's [[12Eh]+2])
        if (elem && *elem != 0 && prefix.size() - body.size() == 1) {
            const std::string width = std::to_string(*elem);
            if (first == 'x') to = "xunknown" + width;
            else if (first == 'i') to = "int" + width;
            else if (first == 'u') to = "uint" + width;
            else if (first == 'c') to = "char";
            else if (first == 'b') to = "bool";
            else to = "xunknown" + width;
        }
        return to + " " + (size == 4 ? "__far " : "") + std::string(prefix.size() - body.size(), '*');
    }
    const bool is_signed = prefix.starts_with('i') || prefix.starts_with('c');
    if (size == 1) return is_signed ? "int1" : "uint1";
    if (size == 2) return is_signed ? "int2" : "uint2";
    if (size == 4) return is_signed ? "int4" : "uint4";
    return "uint2";
}

std::optional<std::pair<std::string, std::int64_t>> split_import_symbol(std::string_view symbol) {
    const std::size_t at = symbol.rfind('_');
    if (at == std::string_view::npos || at == 0 || at + 1 == symbol.size()) return std::nullopt;
    const auto number = text::parse_int<std::int64_t>(symbol.substr(at + 1), 10);
    if (!number || !std::all_of(symbol.begin() + at + 1, symbol.end(), [](char c) { return c >= '0' && c <= '9'; })) return std::nullopt;
    return std::make_pair(std::string(symbol.substr(0, at)), *number);
}

}  // namespace internal

}  // namespace cb::decompile::c_text
