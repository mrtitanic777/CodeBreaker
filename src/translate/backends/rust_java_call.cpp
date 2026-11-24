// translate/backends/rust_java_call.cpp -- calls, `new` and arrays of the Rust back end for Java: the program's own methods and constructors, and the
// library methods the runtime (jrt.rs) has a word for (see rust_java.hpp).

#include <format>
#include <map>
#include <unordered_map>

#include "translate/backends/rust_java_internal.hpp"

namespace cb::translate::backends::rust_java::detail {

namespace {

Code mk(std::string text, ir::TypeP type, int prec = kAtom) {
    Code c;
    c.text = std::move(text);
    c.type = std::move(type);
    c.prec = prec;
    return c;
}

// A Java type as the method tables spell it: int, String, char[].
std::string java_code(const ir::TypeP& t) {
    const std::string_view k = kind_of(t);
    if (k == "bool") return "boolean";
    if (k == "i8") return "byte";
    if (k == "i16") return "short";
    if (k == "u16") return "char";
    if (k == "i32") return "int";
    if (k == "i64") return "long";
    if (k == "f32") return "float";
    if (k == "f64") return "double";
    if (k == "array") return java_code(elem_of(t)) + "[]";
    if (k == "typevar") return "Object";   // (a type variable erases to Object)
    if (k == "class") {
        const std::string_view n = class_name(t);
        const std::size_t dot = n.rfind('.');
        return std::string(dot == std::string_view::npos ? n : n.substr(dot + 1));
    }
    return std::string(k);
}

std::string simple_name(std::string_view qname) {
    const std::size_t dot = qname.rfind('.');
    return std::string(dot == std::string_view::npos ? qname : qname.substr(dot + 1));
}

// The library methods the runtime has: `Owner.name(parameter types)` and the Rust that does it, $0 the receiver and $1... the arguments.
const std::unordered_map<std::string, std::string>& library_table() {
    static const std::unordered_map<std::string, std::string> table = [] {
        std::unordered_map<std::string, std::string> t;
        const auto add = [&](const char* key, const char* pattern) { t.emplace(key, pattern); };
        // String
        add("String.length()", "$0.length()");
        add("String.charAt(int)", "$0.charAt($1)");
        add("String.isEmpty()", "$0.isEmpty()");
        add("String.isBlank()", "$0.isBlank()");
        add("String.substring(int)", "$0.substring($1)");
        add("String.substring(int,int)", "$0.substring2($1, $2)");
        add("String.indexOf(String)", "$0.indexOf_str($1)");
        add("String.indexOf(int)", "$0.indexOf_char($1)");
        add("String.indexOf(String,int)", "$0.indexOf_str_from($1, $2)");
        add("String.indexOf(int,int)", "$0.indexOf_char_from($1, $2)");
        add("String.lastIndexOf(String)", "$0.lastIndexOf_str($1)");
        add("String.lastIndexOf(int)", "$0.lastIndexOf_char($1)");
        add("String.lastIndexOf(int,int)", "$0.lastIndexOf_char_from($1, $2)");
        add("String.equals(Object)", "$0.equals($1)");
        add("String.equalsIgnoreCase(String)", "$0.equalsIgnoreCase($1)");
        add("String.compareTo(String)", "$0.compareTo($1)");
        add("String.compareToIgnoreCase(String)", "$0.compareToIgnoreCase($1)");
        add("String.hashCode()", "$0.hashCode()");
        add("String.startsWith(String)", "$0.startsWith($1)");
        add("String.startsWith(String,int)", "$0.startsWith_from($1, $2)");
        add("String.endsWith(String)", "$0.endsWith($1)");
        add("String.contains(CharSequence)", "$0.contains($1)");
        add("String.toUpperCase()", "$0.toUpperCase()");
        add("String.toLowerCase()", "$0.toLowerCase()");
        add("String.trim()", "$0.trim()");
        add("String.strip()", "$0.strip()");
        add("String.concat(String)", "$0.concat($1)");
        add("String.repeat(int)", "$0.repeat($1)");
        add("String.toCharArray()", "$0.toCharArray()");
        add("String.toString()", "$0.toString()");
        add("String.split(String)", "$0.split($1)");
        add("String.replace(char,char)", "$0.replace_char($1, $2)");
        add("String.replace(CharSequence,CharSequence)", "$0.replace_str($1, $2)");
        for (const char* t_ : {"int", "long", "char", "double", "float", "boolean", "Object"}) t.emplace(std::string("String.valueOf(") + t_ + ")", "String_valueOf($1)");
        add("String.valueOf(char[])", "String_valueOf_chars($1)");
        add("String.copyValueOf(char[])", "String_copyValueOf($1)");
        // the number classes
        add("Integer.parseInt(String)", "Integer_parseInt($1)");
        add("Integer.parseInt(String,int)", "Integer_parseInt_radix($1, $2)");
        add("Integer.toString(int)", "String_valueOf($1)");
        add("Integer.toString(int,int)", "Integer_toString_radix($1, $2)");
        add("Integer.toHexString(int)", "Integer_toHexString($1)");
        add("Integer.toBinaryString(int)", "Integer_toBinaryString($1)");
        add("Integer.toOctalString(int)", "Integer_toOctalString($1)");
        add("Integer.compare(int,int)", "Integer_compare($1, $2)");
        add("Integer.signum(int)", "$1.signum()");
        add("Integer.bitCount(int)", "($1.count_ones() as i32)");
        add("Integer.max(int,int)", "$1.max($2)");
        add("Integer.min(int,int)", "$1.min($2)");
        add("Integer.sum(int,int)", "$1.wrapping_add($2)");
        add("Long.parseLong(String)", "Long_parseLong($1)");
        add("Long.toString(long)", "String_valueOf($1)");
        add("Long.toString(long,int)", "Long_toString_radix($1, $2)");
        add("Long.toHexString(long)", "Long_toHexString($1)");
        add("Long.toBinaryString(long)", "Long_toBinaryString($1)");
        add("Long.compare(long,long)", "Long_compare($1, $2)");
        add("Long.signum(long)", "($1.signum() as i32)");
        add("Long.bitCount(long)", "($1.count_ones() as i32)");
        add("Long.max(long,long)", "$1.max($2)");
        add("Long.min(long,long)", "$1.min($2)");
        add("Long.sum(long,long)", "$1.wrapping_add($2)");
        add("Double.parseDouble(String)", "Double_parseDouble($1)");
        add("Double.toString(double)", "String_valueOf($1)");
        add("Double.compare(double,double)", "Double_compare($1, $2)");
        add("Double.isNaN(double)", "$1.is_nan()");
        add("Double.isInfinite(double)", "$1.is_infinite()");
        add("Double.max(double,double)", "Math::max_f64($1, $2)");
        add("Double.min(double,double)", "Math::min_f64($1, $2)");
        add("Double.sum(double,double)", "($1 + $2)");
        add("Boolean.parseBoolean(String)", "Boolean_parseBoolean($1)");
        add("Boolean.toString(boolean)", "String_valueOf($1)");
        add("Character.isDigit(char)", "Character_isDigit($1)");
        add("Character.isLetter(char)", "Character_isLetter($1)");
        add("Character.isLetterOrDigit(char)", "Character_isLetterOrDigit($1)");
        add("Character.isUpperCase(char)", "Character_isUpperCase($1)");
        add("Character.isLowerCase(char)", "Character_isLowerCase($1)");
        add("Character.isWhitespace(char)", "Character_isWhitespace($1)");
        add("Character.toUpperCase(char)", "Character_toUpperCase($1)");
        add("Character.toLowerCase(char)", "Character_toLowerCase($1)");
        add("Character.getNumericValue(char)", "Character_getNumericValue($1)");
        add("Character.toString(char)", "Character_toString($1)");
        // Math
        add("Math.abs(int)", "$1.wrapping_abs()");
        add("Math.abs(long)", "$1.wrapping_abs()");
        add("Math.abs(double)", "$1.abs()");
        add("Math.abs(float)", "$1.abs()");
        add("Math.max(int,int)", "$1.max($2)");
        add("Math.max(long,long)", "$1.max($2)");
        add("Math.max(double,double)", "Math::max_f64($1, $2)");
        add("Math.min(int,int)", "$1.min($2)");
        add("Math.min(long,long)", "$1.min($2)");
        add("Math.min(double,double)", "Math::min_f64($1, $2)");
        add("Math.pow(double,double)", "Math::pow($1, $2)");
        add("Math.sqrt(double)", "$1.sqrt()");
        add("Math.cbrt(double)", "$1.cbrt()");
        add("Math.floor(double)", "$1.floor()");
        add("Math.ceil(double)", "$1.ceil()");
        add("Math.round(double)", "Math::round_f64($1)");
        add("Math.round(float)", "Math::round_f32($1)");
        add("Math.sin(double)", "$1.sin()");
        add("Math.cos(double)", "$1.cos()");
        add("Math.tan(double)", "$1.tan()");
        add("Math.asin(double)", "$1.asin()");
        add("Math.acos(double)", "$1.acos()");
        add("Math.atan(double)", "$1.atan()");
        add("Math.atan2(double,double)", "$1.atan2($2)");
        add("Math.exp(double)", "$1.exp()");
        add("Math.log(double)", "$1.ln()");
        add("Math.log10(double)", "$1.log10()");
        add("Math.hypot(double,double)", "$1.hypot($2)");
        add("Math.signum(double)", "Math::signum($1)");
        add("Math.toRadians(double)", "Math::toRadians($1)");
        add("Math.toDegrees(double)", "Math::toDegrees($1)");
        add("Math.floorDiv(int,int)", "Math::floorDiv_i32($1, $2)");
        add("Math.floorMod(int,int)", "Math::floorMod_i32($1, $2)");
        add("Math.floorDiv(long,long)", "Math::floorDiv_i64($1, $2)");
        add("Math.floorMod(long,long)", "Math::floorMod_i64($1, $2)");
        add("Math.addExact(int,int)", "Math::addExact_i32($1, $2)");
        add("Math.multiplyExact(int,int)", "Math::multiplyExact_i32($1, $2)");
        add("Math.addExact(long,long)", "Math::addExact_i64($1, $2)");
        add("Math.multiplyExact(long,long)", "Math::multiplyExact_i64($1, $2)");
        // StringBuilder
        for (const char* t_ : {"String", "char", "int", "long", "double", "float", "boolean", "Object", "CharSequence"})
            t.emplace(std::string("StringBuilder.append(") + t_ + ")", "$0.nn().append($1)");
        add("StringBuilder.append(char[])", "$0.nn().append_chars($1)");
        add("StringBuilder.toString()", "$0.nn().toString()");
        add("StringBuilder.length()", "$0.nn().length()");
        add("StringBuilder.isEmpty()", "$0.nn().isEmpty()");
        add("StringBuilder.charAt(int)", "$0.nn().charAt($1)");
        add("StringBuilder.setCharAt(int,char)", "$0.nn().setCharAt($1, $2)");
        add("StringBuilder.deleteCharAt(int)", "$0.nn().deleteCharAt($1)");
        add("StringBuilder.delete(int,int)", "$0.nn().delete($1, $2)");
        add("StringBuilder.reverse()", "$0.nn().reverse()");
        add("StringBuilder.setLength(int)", "$0.nn().setLength($1)");
        add("StringBuilder.indexOf(String)", "$0.nn().indexOf($1)");
        add("StringBuilder.substring(int)", "$0.nn().substring($1)");
        add("StringBuilder.substring(int,int)", "$0.nn().substring2($1, $2)");
        for (const char* t_ : {"String", "char", "int", "long", "double", "float", "boolean", "Object"})
            t.emplace(std::string("StringBuilder.insert(int,") + t_ + ")", "$0.nn().insert($1, $2)");
        // Object's three, on a reference of the program's
        add("Object.toString()", "$0.nn().toString()");
        add("Object.equals(Object)", "$0.nn().equals($1)");
        add("Object.hashCode()", "$0.nn().hashCode()");
        // Throwable
        add("Throwable.getMessage()", "$0.nn().getMessage()");
        add("Throwable.getLocalizedMessage()", "$0.nn().getLocalizedMessage()");
        add("Throwable.getCause()", "$0.nn().getCause()");
        add("Throwable.toString()", "$0.nn().toString()");
        add("Throwable.printStackTrace()", "$0.nn().printStackTrace()");
        // System
        add("System.exit(int)", "System::exit($1)");
        add("System.currentTimeMillis()", "System::currentTimeMillis()");
        add("System.nanoTime()", "System::nanoTime()");
        add("System.lineSeparator()", "System::lineSeparator()");
        add("System.arraycopy(Object,int,Object,int,int)", "arraycopy($1, $2, $3, $4, $5)");
        // Arrays
        for (const char* e : {"int", "long", "double", "float", "char", "boolean", "short", "byte"}) {
            const std::string a = std::string(e) + "[]";
            t.emplace("Arrays.toString(" + a + ")", "Arrays_toString($1)");
            t.emplace("Arrays.fill(" + a + "," + e + ")", "Arrays_fill($1, $2)");
            t.emplace("Arrays.copyOf(" + a + ",int)", "Arrays_copyOf($1, $2)");
            t.emplace("Arrays.copyOfRange(" + a + ",int,int)", "Arrays_copyOfRange($1, $2, $3)");
            t.emplace("Arrays.equals(" + a + "," + a + ")", "Arrays_equals($1, $2)");
            if (std::string_view(e) != "boolean") t.emplace("Arrays.sort(" + a + ")", "Arrays_sort($1)");
        }
        add("Arrays.toString(String[])", "Arrays_toString($1)");
        add("Integer.hashCode(int)", "$1.jhash()");
        add("Long.hashCode(long)", "$1.jhash()");
        add("Double.hashCode(double)", "$1.jhash()");
        add("Boolean.hashCode(boolean)", "$1.jhash()");
        add("Character.hashCode(char)", "$1.jhash()");
        add("Short.hashCode(short)", "$1.jhash()");
        add("Byte.hashCode(byte)", "$1.jhash()");
        add("Float.hashCode(float)", "$1.jhash()");
        // the boxed numbers and java.util: lists (ArrayList), sets (HashSet), maps (HashMap), their iterators and entries
        add("Integer.valueOf(int)", "Some($1)");
        add("Integer.valueOf(String)", "Some(Integer_parseInt($1))");
        add("Long.valueOf(long)", "Some($1)");
        add("Double.valueOf(double)", "Some($1)");
        add("Integer.intValue()", "$0.unbox()");
        add("Long.longValue()", "$0.unbox()");
        add("Double.doubleValue()", "$0.unbox()");
        add("List.add(Object)", "$0.nn().add($1)");
        add("List.add(int,Object)", "$0.nn().add_at($1, $2)");
        add("List.get(int)", "$0.nn().get($1)");
        add("List.set(int,Object)", "$0.nn().set($1, $2)");
        add("List.size()", "$0.nn().size()");
        add("List.isEmpty()", "$0.nn().isEmpty()");
        add("List.clear()", "$0.nn().clear()");
        add("List.contains(Object)", "$0.nn().contains($1)");
        add("List.indexOf(Object)", "$0.nn().indexOf($1)");
        add("List.lastIndexOf(Object)", "$0.nn().lastIndexOf($1)");
        add("List.remove(int)", "$0.nn().remove_at($1)");
        add("List.remove(Object)", "$0.nn().remove_obj($1)");
        add("List.iterator()", "$0.nn().iterator()");
        add("List.addAll(Collection)", "$0.nn().addAll($1)");
        add("List.toString()", "$0.nn().toString()");
        add("List.sort(Comparator)", "$0.nn().sort_natural($1)");
        add("Set.add(Object)", "$0.nn().add($1)");
        add("Set.contains(Object)", "$0.nn().contains($1)");
        add("Set.remove(Object)", "$0.nn().remove_obj($1)");
        add("Set.size()", "$0.nn().size()");
        add("Set.isEmpty()", "$0.nn().isEmpty()");
        add("Set.clear()", "$0.nn().clear()");
        add("Set.iterator()", "$0.nn().iterator()");
        add("Set.toString()", "$0.nn().toString()");
        add("Map.put(Object,Object)", "$0.nn().put($1, $2)");
        add("Map.get(Object)", "$0.nn().get($1)");
        add("Map.getOrDefault(Object,Object)", "$0.nn().getOrDefault($1, $2)");
        add("Map.containsKey(Object)", "$0.nn().containsKey($1)");
        add("Map.containsValue(Object)", "$0.nn().containsValue($1)");
        add("Map.remove(Object)", "$0.nn().remove($1)");
        add("Map.putIfAbsent(Object,Object)", "$0.nn().putIfAbsent($1, $2)");
        add("Map.size()", "$0.nn().size()");
        add("Map.isEmpty()", "$0.nn().isEmpty()");
        add("Map.clear()", "$0.nn().clear()");
        add("Map.keySet()", "$0.nn().keySet()");
        add("Map.values()", "$0.nn().values()");
        add("Map.entrySet()", "$0.nn().entrySet()");
        add("Map.toString()", "$0.nn().toString()");
        add("Entry.getKey()", "$0.nn().getKey()");
        add("Entry.getValue()", "$0.nn().getValue()");
        add("Iterator.hasNext()", "$0.nn().hasNext()");
        add("Iterator.next()", "$0.nn().next()");
        return t;
    }();
    return table;
}

}  // namespace

std::vector<std::string> Writer::call_args(std::span<ir::Node* const> args, const std::vector<ir::TypeP>& params, bool varargs) {
    std::vector<std::string> out;
    // a call with a variable number of arguments packs the extra ones into an array (unless it was given the array itself)
    const bool pack = varargs && !params.empty() &&
                      !(args.size() == params.size() && (kind_of(type_of(args.back())) == "array" || kind_of(type_of(args.back())) == "null"));
    const std::size_t fixed = pack ? params.size() - 1 : params.size();
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (pack && i >= fixed) break;
        const ir::TypeP want = i < fixed ? params[i] : (params.empty() ? ir::TypeP() : params.back());
        out.push_back(as_arg(args[i], want));
    }
    if (pack) {
        const ir::TypeP elem = elem_of(params.back());
        std::vector<std::string> items;
        for (std::size_t i = fixed; i < args.size(); ++i) items.push_back(as_arg(args[i], elem));
        out.push_back(items.empty() ? "array_of::<" + p_.rtype(elem) + ">(&[])" : "array_of(&[" + join(items) + "])");
    }
    return out;
}

// ---------------------------------------------------------------------------------------------------------------- the program's methods

Code Writer::method_call(const ir::Node& n, const ir::Call& k) {
    const ir::Sym& s = *k.sym;
    const auto match = p_.model().method_decl(s);
    const ir::MethodDecl& m = method_of(match->method);
    const ClassInfo& dc = *match->cls;
    std::vector<ir::TypeP> params;
    for (const ir::Node* pn : m.params) params.push_back(param_of(pn).type);
    const std::vector<std::string> args = call_args(k.args, params, s.varargs);
    const ir::TypeP type = n.type ? n.type : s.ret;
    const ir::Node* target = strip_paren(k.target);
    if (is_static(m)) return mk(p_.holder(dc) + "::" + p_.mname(m) + "(" + join(args) + ")", type);
    const bool super_call = ir::is<ir::Super>(target);
    if (is_private(m) || super_call) {   // not looked up through the trait: the body of that very method
        const std::string recv = (target == nullptr || ir::is<ir::This>(target) || super_call) ? receiver(nullptr).text : wrap(receiver(target), kAtom);
        std::vector<std::string> all = {recv};
        all.insert(all.end(), args.begin(), args.end());
        return mk(p_.holder(dc) + "::" + p_.mname(m) + "(" + join(all) + ")", type);
    }
    const Code recv = receiver(k.target);
    return mk(wrap(recv, kAtom) + "." + p_.mname(m) + "(" + join(args) + ")", type);
}

Code Writer::on(const ir::Node& n, const ir::Call& k) {
    if (!k.sym || k.sym->kind != ir::SymKind::Method) throw Decline("a call that is not to a method");
    if (const ClassInfo* owner = p_.model().info(k.sym->owner.value_or("")); owner != nullptr && owner->kind == "enum" && !p_.model().method_decl(*k.sym)) {
        if (k.sym->name == "values" && k.args.empty()) return mk(p_.holder(*owner) + "::values()", n.type);
        if (k.sym->name == "valueOf" && k.args.size() == 1) return mk(p_.holder(*owner) + "::valueOf(" + expr(k.args[0]).text + ")", n.type);
    }
    if (const auto match = p_.model().method_decl(*k.sym)) return method_call(n, k);
    return lib_call(n, k);
}

// ---------------------------------------------------------------------------------------------------------------- the library

Code Writer::lib_static_field(const ir::Select& sel, const ir::TypeP& type) {
    const ir::Sym& sym = *sel.sym;
    const std::string key = simple_name(sym.owner.value_or("")) + "." + sym.name;
    static const std::map<std::string, std::string> fields = {
        {"Integer.MAX_VALUE", "i32::MAX"},          {"Integer.MIN_VALUE", "i32::MIN"},     {"Long.MAX_VALUE", "i64::MAX"},       {"Long.MIN_VALUE", "i64::MIN"},
        {"Short.MAX_VALUE", "i16::MAX"},            {"Short.MIN_VALUE", "i16::MIN"},       {"Byte.MAX_VALUE", "i8::MAX"},        {"Byte.MIN_VALUE", "i8::MIN"},
        {"Character.MAX_VALUE", "u16::MAX"},        {"Character.MIN_VALUE", "u16::MIN"},   {"Double.MAX_VALUE", "f64::MAX"},     {"Double.MIN_VALUE", "f64::from_bits(1)"},
        {"Double.POSITIVE_INFINITY", "f64::INFINITY"}, {"Double.NEGATIVE_INFINITY", "f64::NEG_INFINITY"}, {"Double.NaN", "f64::NAN"},
        {"Float.MAX_VALUE", "f32::MAX"},            {"Float.MIN_VALUE", "f32::from_bits(1)"}, {"Float.POSITIVE_INFINITY", "f32::INFINITY"},
        {"Float.NEGATIVE_INFINITY", "f32::NEG_INFINITY"}, {"Float.NaN", "f32::NAN"},        {"Math.PI", "Math::PI"},              {"Math.E", "Math::E"},
        {"System.out", "System::out()"},            {"System.err", "System::err()"}};
    const auto it = fields.find(key);
    if (it == fields.end()) throw Decline("the library field " + key);
    return mk(it->second, type);
}

Code Writer::lib_call(const ir::Node& n, const ir::Call& k) {
    const ir::Sym& s = *k.sym;
    std::string owner = s.owner.value_or("");
    if (owner == "java.lang.Enum" || (p_.model().info(owner) != nullptr && p_.model().info(owner)->kind == "enum")) {   // what every enum has
        const Code recv = (k.target == nullptr || ir::is<ir::This>(strip_paren(k.target))) ? this_code() : expr(k.target);
        const ClassInfo* cls = p_.program_class(recv.type);
        if (cls == nullptr || cls->kind != "enum") throw Decline("an enum method on something that is not a program enum");
        const std::string fields = wrap(recv, kAtom) + ".nn()." + p_.fields_fn(*cls) + "().";
        if (s.name == "name" && k.args.empty()) return mk(fields + "enum_name.get()", n.type);
        if (s.name == "ordinal" && k.args.empty()) return mk(fields + "enum_ordinal.get()", n.type);
        if (s.name == "toString" && k.args.empty()) return mk(wrap(recv, kAtom) + ".nn().toString()", n.type);
        if (s.name == "hashCode" && k.args.empty()) return mk(wrap(recv, kAtom) + ".nn().hashCode()", n.type);
        if (s.name == "equals" && k.args.size() == 1) return mk(wrap(recv, kAtom) + ".nn().equals(" + conv(expr(k.args[0]), ir::class_type("java.lang.Object")).text + ")", n.type);
        if (s.name == "compareTo" && k.args.size() == 1) {
            const Code other = expr(k.args[0]);
            return mk(fields + "enum_ordinal.get().wrapping_sub(" + wrap(other, kAtom) + ".nn()." + p_.fields_fn(*cls) + "().enum_ordinal.get())", n.type);
        }
        throw Decline("the enum method " + s.name);
    }
    if (owner == "java.lang.Throwable" || p_.is_lib_throwable(owner)) owner = "java.lang.Throwable";
    if (owner == "java.lang.AbstractStringBuilder") owner = "java.lang.StringBuilder";   // (where the methods of a StringBuilder are declared)
    if (const std::string_view box = boxed_kind(ir::class_type(owner)); !box.empty() && !s.is_static && k.target != nullptr) {   // a method of a box
        const ir::TypeP type = n.type ? n.type : s.ret;
        const Code recv = expr(k.target);
        const std::string value = receiver_text(recv) + ".unbox()";
        if (s.name == "equals" && k.args.size() == 1) return mk("box_equals(" + recv.text + ", " + conv(expr(k.args[0]), ir::class_type(owner)).text + ")", type);
        if (s.name == "toString" && k.args.empty()) return mk("String_valueOf(" + value + ")", type);
        if (s.name == "hashCode" && k.args.empty()) return mk(value + ".jhash()", type);
        if (s.name == "compareTo" && k.args.size() == 1) {
            const std::string other = conv(expr(k.args[0]), ir::class_type(owner)).text;
            const std::string fn = box == "i32" ? "Integer_compare" : box == "i64" ? "Long_compare" : box == "f64" ? "Double_compare" : "";
            if (fn.empty()) throw Decline("compareTo on a " + owner);
            return mk(fn + "(" + value + ", " + receiver_text(mk(other, ir::class_type(owner))) + ".unbox())", type);
        }
        if (s.name.size() > 5 && s.name.ends_with("Value") && k.args.empty()) {   // intValue(), doubleValue() ...
            static const std::map<std::string, std::string> kinds = {{"int", "i32"}, {"long", "i64"}, {"double", "f64"}, {"float", "f32"}, {"short", "i16"}, {"byte", "i8"}, {"boolean", "bool"}, {"char", "u16"}};
            const auto it = kinds.find(s.name.substr(0, s.name.size() - 5));
            if (it != kinds.end()) return conv(mk(value, prim_by_kind(box)), prim_by_kind(it->second));
        }
    }
    if (owner == "java.util.Objects") {
        const ir::TypeP type = n.type ? n.type : s.ret;
        // anything goes in as it is: the runtime's equality and hash codes take what the program has
        std::vector<Code> given;
        for (const ir::Node* a : k.args) given.push_back(expr(a));
        const auto hashed = [](const Code& c) { return receiver_text(c) + ".jhash()"; };
        if (s.name == "equals" && given.size() == 2) return mk("Objects_equals(" + given[0].text + ", " + given[1].text + ")", type);
        if (s.name == "hashCode" && given.size() == 1) return mk(hashed(given[0]), type);
        if (s.name == "toString" && given.size() == 1) return mk("String_valueOf(" + given[0].text + ")", type);
        if (s.name == "requireNonNull" && given.size() == 1) return mk("Objects_requireNonNull(" + given[0].text + ")", type);
        if (s.name == "hash") {
            std::vector<std::string> items;
            if (given.size() == 1 && kind_of(given[0].type) == "array") throw Decline("Objects.hash of an array");
            for (const Code& c : given) items.push_back(hashed(c));
            return mk("Objects_hash(&[" + join(items) + "])", type);
        }
        throw Decline("Objects." + s.name);
    }
    const ir::TypeP result_type = n.type ? n.type : s.ret;
    if ((owner == "java.util.List" && s.name == "of") || (owner == "java.util.Arrays" && s.name == "asList")) {
        const ir::TypeP& type = result_type;
        const auto* ct = std::get_if<ir::ClassType>(&type->body);
        if (ct == nullptr || ct->args.size() != 1) throw Decline(owner + "." + s.name + " whose element type is not known");
        if (k.args.size() == 1 && kind_of(type_of(k.args[0])) == "array") throw Decline(owner + "." + s.name + " of an array");
        std::vector<std::string> items;
        for (const ir::Node* a : k.args) items.push_back(conv(expr(a), ct->args[0]).text);
        return mk("ArrayList::<" + p_.rtype(ct->args[0]) + ">::of(vec![" + join(items) + "])", type);
    }
    if (owner == "java.util.Collections" && (s.name == "sort" || s.name == "reverse") && k.args.size() == 1)
        return mk("Collections_" + s.name + "(" + expr(k.args[0]).text + ")", result_type);
    std::string family;   // the java.util collection the owner is, by the name the table uses
    if (owner == "java.util.List" || owner == "java.util.ArrayList" || owner == "java.util.Collection" || owner == "java.util.AbstractList") family = "List";
    else if (owner == "java.util.Set" || owner == "java.util.HashSet" || owner == "java.util.LinkedHashSet") family = "Set";
    else if (owner == "java.util.Map" || owner == "java.util.HashMap" || owner == "java.util.LinkedHashMap") family = "Map";
    else if (owner == "java.util.Iterator") family = "Iterator";
    else if (owner == "java.util.Map.Entry") family = "Entry";
    const std::string simple = family.empty() ? simple_name(owner) : family;
    const ir::Node* target = strip_paren(k.target);
    const ir::TypeP type = n.type ? n.type : s.ret;
    if (ir::is<ir::Super>(target)) throw Decline("a call to " + owner + "." + s.name + " through super");

    // printing
    if (owner == "java.io.PrintStream") {
        const std::string out = expr(k.target).text;
        if (s.name == "println" || s.name == "print") {
            if (k.args.empty()) return mk(out + ".println0()", type);
            if (k.args.size() != 1) throw Decline("PrintStream." + s.name + " with several arguments");
            const Code a = expr(k.args[0]);
            if (kind_of(a.type) == "array") {
                if (kind_of(elem_of(a.type)) != "u16") throw Decline("printing an array");
                return mk(out + "." + s.name + "_chars(" + a.text + ")", type);
            }
            return mk(out + "." + s.name + "(" + a.text + ")", type);
        }
        if (s.name == "flush") return mk(out + ".flush()", type);
        if (s.name == "printf" || s.name == "format") {
            if (k.args.empty() || class_name(type_of(k.args[0])) != "java.lang.String") throw Decline("printf without a format string");
            std::vector<std::string> items;
            for (std::size_t i = 1; i < k.args.size(); ++i) {
                const Code a = expr(k.args[i]);
                if (kind_of(a.type) == "array") throw Decline("printf with an array argument");
                items.push_back("fa(" + a.text + ")");
            }
            return mk(out + ".printf(" + expr(k.args[0]).text + ", &[" + join(items) + "])", type);
        }
        throw Decline("PrintStream." + s.name);
    }
    if (owner == "java.lang.String" && s.is_static && (s.name == "format" || s.name == "join")) {
        std::vector<std::string> items;
        if (s.name == "format") {
            if (k.args.empty() || class_name(type_of(k.args[0])) != "java.lang.String") throw Decline("String.format with a Locale");
            for (std::size_t i = 1; i < k.args.size(); ++i) {
                const Code a = expr(k.args[i]);
                if (kind_of(a.type) == "array") throw Decline("String.format with an array argument");
                items.push_back("fa(" + a.text + ")");
            }
            return mk("jformat(" + expr(k.args[0]).text + ", &[" + join(items) + "])", type);
        }
        if (k.args.size() < 1) throw Decline("String.join without a separator");
        if (k.args.size() == 2 && kind_of(type_of(k.args[1])) == "array" && class_name(elem_of(type_of(k.args[1]))) == "java.lang.String")
            return mk("String_join(" + expr(k.args[0]).text + ", &" + wrap(expr(k.args[1]), kAtom) + ".units())", type);
        for (std::size_t i = 1; i < k.args.size(); ++i) {
            if (class_name(type_of(k.args[i])) != "java.lang.String") throw Decline("String.join of something but Strings");
            items.push_back(expr(k.args[i]).text);
        }
        return mk("String_join(" + expr(k.args[0]).text + ", &[" + join(items) + "])", type);
    }
    if (owner == "java.lang.String" && s.name == "formatted") {
        std::vector<std::string> items;
        for (const ir::Node* a : k.args) items.push_back("fa(" + expr(a).text + ")");
        return mk("jformat(" + expr(k.target).text + ", &[" + join(items) + "])", type);
    }

    std::vector<std::string> codes;
    for (const ir::TypeP& p : s.params) codes.push_back(java_code(p));
    const std::string key = simple + "." + s.name + "(" + join(codes, ",") + ")";
    const auto it = library_table().find(key);
    if (it == library_table().end()) throw Decline("the library method " + key);
    const std::string& pattern = it->second;

    // the receiver and the arguments
    Code recv;
    const bool stat = s.is_static || ir::is<ir::TypeName>(target);
    if (!stat) {
        if (target == nullptr || ir::is<ir::This>(target)) recv = this_code();
        else recv = expr(k.target);
    }
    const bool raw_objects = owner == "java.lang.String" || owner == "java.lang.StringBuilder";
    std::vector<Code> args;
    std::vector<ir::TypeP> held;   // what the collection holds: E, or K and V
    if (!family.empty() && !stat) {
        if (const auto* ct = std::get_if<ir::ClassType>(&recv.type->body)) held = ct->args;
    }
    for (std::size_t i = 0; i < k.args.size() && i < s.params.size(); ++i) {
        ir::TypeP want = s.params[i];
        const std::string wn(class_name(want));
        if (!family.empty() && (wn == "java.lang.Object" || kind_of(want) == "typevar")) {   // an erased element, key or value: what the receiver says it is
            std::size_t at = 0;
            if (const auto* tv = std::get_if<ir::TypeVarType>(&want->body)) at = tv->name == "V" ? 1 : 0;
            else if (family == "Map" && s.name == "containsValue") at = 1;
            if (at >= held.size()) throw Decline("a collection call on a receiver whose element type is not known");
            want = held[at];
        } else if (!family.empty() && wn.starts_with("java.util.")) {
            args.push_back(expr(k.args[i]));
            continue;
        }
        if (raw_objects && (wn == "java.lang.Object" || wn == "java.lang.CharSequence")) {
            const Code a = expr(k.args[i]);
            if (kind_of(a.type) == "array") throw Decline("an array given as an Object to " + key);
            args.push_back(a);
        } else if (owner == "java.lang.System" && s.name == "arraycopy" && (i == 0 || i == 2)) {
            args.push_back(expr(k.args[i]));
        } else if (owner == "java.lang.String" && s.name == "valueOf" && wn == "java.lang.Object") {
            args.push_back(expr(k.args[i]));
        } else {
            if (!ir::TypeP(want)) throw Decline("a parameter the front end did not type");
            args.push_back(conv(expr(k.args[i]), want));
        }
    }
    // substitute
    std::string text;
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        if (pattern[i] != '$') {
            text += pattern[i];
            continue;
        }
        const std::size_t slot = static_cast<std::size_t>(pattern[++i] - '0');
        const bool receiver_position = i + 1 < pattern.size() && pattern[i + 1] == '.';
        const Code* c = nullptr;
        if (slot == 0) {
            c = &recv;
        } else {
            if (slot > args.size()) throw Decline("the call " + key + " has too few arguments");
            c = &args[slot - 1];
        }
        if (receiver_position) {
            text += receiver_text(*c);
        } else {
            text += c->text;
        }
    }
    return mk(text, type);
}

Code Writer::lib_new(const ir::Node& n, const ir::New& k) {
    const ir::Sym& s = need_sym(k.sym, "a constructor");
    const std::string owner = s.owner.value_or("");
    const ir::TypeP type = n.type;
    if (owner == "java.lang.StringBuilder") {
        if (k.args.empty() || (s.params.size() == 1 && kind_of(s.params[0]) == "i32")) {
            if (!k.args.empty() && !is_pure(k.args[0])) throw Decline("a StringBuilder capacity that is computed");
            return mk("StringBuilder::new()", type);
        }
        if (s.params.size() == 1) {
            const Code a = expr(k.args[0]);
            if (class_name(a.type) != "java.lang.String") throw Decline("a StringBuilder of a CharSequence");
            return mk("StringBuilder::from_str(" + a.text + ")", type);
        }
    }
    if (owner == "java.util.ArrayList" || owner == "java.util.HashMap" || owner == "java.util.LinkedHashMap" || owner == "java.util.HashSet" ||
        owner == "java.util.LinkedHashSet") {
        const auto* ct = std::get_if<ir::ClassType>(&type->body);
        if (ct == nullptr || ct->args.empty()) throw Decline("new " + owner + " with no type arguments");
        if (!k.args.empty() && !(owner == "java.util.ArrayList" && s.params.size() == 1 && kind_of(s.params[0]) == "i32" && is_pure(k.args[0])))
            throw Decline("a " + owner + " built from arguments");
        std::vector<std::string> targs;
        for (const ir::TypeP& a : ct->args) targs.push_back(p_.rtype(a));
        const bool linked = owner.starts_with("java.util.Linked");
        const std::string name = owner == "java.util.ArrayList" ? "ArrayList" : owner.ends_with("Map") ? "HashMap" : "HashSet";
        return mk(name + "::<" + join(targs) + ">::" + (linked ? "new_linked" : "new") + "()", type);
    }
    if (owner == "java.lang.String" && s.params.size() == 1 && kind_of(s.params[0]) == "array" && kind_of(elem_of(s.params[0])) == "u16")
        return mk("String_valueOf_chars(" + expr(k.args[0]).text + ")", type);
    if (p_.is_lib_throwable(owner)) {
        const std::string q = rs_string(owner);
        const std::size_t count = k.args.size();
        if (count == 0) return mk("new_exception(" + q + ", Str::NULL, None)", ir::class_type(owner));
        if (count == 1 && class_name(s.params[0]) == "java.lang.String") return mk("new_exception(" + q + ", " + expr(k.args[0]).text + ", None)", ir::class_type(owner));
        if (count == 1) return mk("new_exception_cause(" + q + ", " + conv(expr(k.args[0]), ir::class_type("java.lang.Throwable")).text + ")", ir::class_type(owner));
        if (count == 2)
            return mk("new_exception(" + q + ", " + expr(k.args[0]).text + ", " + conv(expr(k.args[1]), ir::class_type("java.lang.Throwable")).text + ")", ir::class_type(owner));
    }
    throw Decline("new of the library class " + owner);
}

// ---------------------------------------------------------------------------------------------------------------- new

Code Writer::on(const ir::Node& n, const ir::New& k) {
    if (k.body != nullptr) throw Decline("an anonymous class");
    if (k.outer != nullptr) throw Decline("an inner class instance with an outer object");
    const ir::Sym& s = need_sym(k.sym, "a constructor");
    const ClassInfo* cls = p_.model().info(s.owner.value_or(""));
    if (cls == nullptr) return lib_new(n, k);
    const auto match = p_.model().method_decl(s);
    if (!match) throw Decline("a constructor of " + cls->qname + " the program does not declare");
    if (!p_.is_concrete(*cls)) throw Decline("new of the abstract class " + cls->qname);
    std::vector<ir::TypeP> params;
    for (const ir::Node* pn : method_of(match->method).params) params.push_back(param_of(pn).type);
    const std::vector<std::string> args = call_args(k.args, params, s.varargs);
    return mk(p_.obj_name(*cls) + "::new" + p_.ctor_suffix(*cls, match->method) + "(" + join(args) + ")", ir::class_type(cls->qname));
}

Code Writer::on(const ir::Node& n, const ir::NewArray& k) {
    ir::TypeP type = n.type;
    if (!type) {
        if (const auto hint = array_hint_.find(&n); hint != array_hint_.end()) type = hint->second;
    }
    if (kind_of(type) != "array") throw Decline("an array creation with no array type");
    const ir::TypeP elem = elem_of(type);
    const std::string et = p_.rtype(elem);
    if (!k.init.empty() || k.dims.empty()) {   // `new T[] { ... }` (also an empty one)
        std::vector<std::string> items;
        for (const ir::Node* x : k.init) {
            if (ir::is<ir::NewArray>(x) && !x->type) array_hint_[x] = elem;
            items.push_back(conv(expr(x), elem).text);
        }
        if (items.empty()) return mk("array_of::<" + et + ">(&[])", type);
        return mk("array_of(&[" + join(items) + "])", type);
    }
    std::vector<std::string> dims;
    for (const ir::Node* d : k.dims) dims.push_back(conv(expr(d), ir::i32_type()).text);
    if (dims.size() == 1) return mk("new_array::<" + et + ">(" + dims[0] + ")", type);
    ir::TypeP leaf = type;   // what the innermost arrays hold (with `new T[a][b][]`, null arrays)
    for (std::size_t i = 0; i < dims.size(); ++i) leaf = elem_of(leaf);
    if (dims.size() == 2) return mk("new_array2::<" + p_.rtype(leaf) + ">(" + join(dims) + ")", type);
    if (dims.size() == 3) return mk("new_array3::<" + p_.rtype(leaf) + ">(" + join(dims) + ")", type);
    throw Decline("an array of " + std::to_string(dims.size()) + " dimensions");
}

}  // namespace cb::translate::backends::rust_java::detail
