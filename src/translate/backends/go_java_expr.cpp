// translate/backends/go_java_expr.cpp -- expressions of the Go back end for Java: literals and names, operators with Java's arithmetic, assignments and
// increments, casts, fields and arrays, calls, `new` and the library the runtime (jrt) has a word for (see go_java.hpp).

#include <cmath>
#include <format>
#include <unordered_map>

#include "translate/backends/go_java_internal.hpp"

namespace cb::translate::backends::go_java::detail {

namespace {

Code mk(std::string text, ir::TypeP type, int prec = kAtom) {
    Code c;
    c.text = std::move(text);
    c.type = std::move(type);
    c.prec = prec;
    return c;
}

bool is_comparison(const std::string& op) { return op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=" ; }

ir::TypeP eff(const ir::TypeP& t) {
    const std::string_view box = boxed_kind(t);
    return box.empty() ? t : prim_by_kind(box);
}

std::string go_prim(std::string_view kind) {
    if (kind == "i8") return "int8";
    if (kind == "i16") return "int16";
    if (kind == "i32") return "int32";
    if (kind == "i64") return "int64";
    if (kind == "u16") return "uint16";
    if (kind == "f32") return "float32";
    if (kind == "f64") return "float64";
    return "bool";
}

std::string simple_name(std::string_view qname) {
    const std::size_t dot = qname.rfind('.');
    return std::string(dot == std::string_view::npos ? qname : qname.substr(dot + 1));
}

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
    if (k == "typevar") return "Object";
    if (k == "class") return simple_name(class_name(t));
    return std::string(k);
}

std::string char_literal(std::uint32_t code) {
    switch (code) {
        case '\'': return "'\\''";
        case '\\': return "'\\\\'";
        case '\n': return "'\\n'";
        case '\t': return "'\\t'";
        case '\r': return "'\\r'";
    }
    if (code >= 0x20 && code < 0x7F) return std::string("'") + static_cast<char>(code) + "'";
    if (code >= 0xD800 && code <= 0xDFFF) return std::to_string(code);
    return std::format("'\\u{:04x}'", code);
}

// Wrap `v` in the width of a Java integer kind (Java's integer arithmetic wraps).
std::int64_t wrap_to(std::int64_t v, std::string_view kind) {
    if (kind == "i32") return static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::uint64_t>(v)));
    if (kind == "i16") return static_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint64_t>(v)));
    if (kind == "i8") return static_cast<std::int8_t>(static_cast<std::uint8_t>(static_cast<std::uint64_t>(v)));
    if (kind == "u16") return static_cast<std::uint16_t>(static_cast<std::uint64_t>(v));
    return v;
}

// The library methods the runtime has: `Owner.name(parameter types)` and the Go that does it, $0 the receiver and $1... the arguments.
const std::unordered_map<std::string, std::string>& library_table() {
    static const std::unordered_map<std::string, std::string> table = [] {
        std::unordered_map<std::string, std::string> t;
        const auto add = [&](const char* key, const char* pattern) { t.emplace(key, pattern); };
        add("String.length()", "$0.Length()");
        add("String.charAt(int)", "$0.CharAt($1)");
        add("String.isEmpty()", "$0.IsEmpty()");
        add("String.isBlank()", "$0.IsBlank()");
        add("String.substring(int)", "$0.Substring($1)");
        add("String.substring(int,int)", "$0.SubstringTo($1, $2)");
        add("String.indexOf(String)", "$0.IndexOf($1)");
        add("String.indexOf(int)", "$0.IndexOfChar($1)");
        add("String.indexOf(String,int)", "$0.IndexOfFrom($1, $2)");
        add("String.indexOf(int,int)", "$0.IndexOfCharFrom($1, $2)");
        add("String.lastIndexOf(String)", "$0.LastIndexOf($1)");
        add("String.lastIndexOf(int)", "$0.LastIndexOfChar($1)");
        add("String.lastIndexOf(int,int)", "$0.LastIndexOfCharFrom($1, $2)");
        add("String.equals(Object)", "$0.Equals($1)");
        add("String.equalsIgnoreCase(String)", "$0.EqualsIgnoreCase($1)");
        add("String.compareTo(String)", "$0.CompareTo($1)");
        add("String.compareToIgnoreCase(String)", "$0.CompareToIgnoreCase($1)");
        add("String.hashCode()", "$0.HashCode()");
        add("String.startsWith(String)", "$0.StartsWith($1)");
        add("String.startsWith(String,int)", "$0.StartsWithFrom($1, $2)");
        add("String.endsWith(String)", "$0.EndsWith($1)");
        add("String.contains(CharSequence)", "$0.Contains($1)");
        add("String.toUpperCase()", "$0.ToUpperCase()");
        add("String.toLowerCase()", "$0.ToLowerCase()");
        add("String.trim()", "$0.Trim()");
        add("String.strip()", "$0.Strip()");
        add("String.concat(String)", "$0.Concat($1)");
        add("String.repeat(int)", "$0.Repeat($1)");
        add("String.toCharArray()", "$0.ToCharArray()");
        add("String.toString()", "$0.ToString()");
        add("String.split(String)", "$0.Split($1)");
        add("String.replace(char,char)", "$0.ReplaceChar($1, $2)");
        add("String.replace(CharSequence,CharSequence)", "$0.Replace($1, $2)");
        for (const char* t_ : {"int", "long", "char", "double", "float", "boolean", "Object"}) t.emplace(std::string("String.valueOf(") + t_ + ")", "jrt.ToStr($1)");
        add("String.valueOf(char[])", "jrt.StringValueOfChars($1)");
        add("String.copyValueOf(char[])", "jrt.StringValueOfChars($1)");
        add("Integer.parseInt(String)", "jrt.ParseInt($1, 10)");
        add("Integer.parseInt(String,int)", "jrt.ParseInt($1, $2)");
        add("Integer.valueOf(String)", "jrt.ParseInt($1, 10)");
        add("Integer.toString(int)", "jrt.ToStr($1)");
        add("Integer.toString(int,int)", "jrt.IntToString(int64($1), $2)");
        add("Integer.toHexString(int)", "jrt.IntToHex($1)");
        add("Integer.toBinaryString(int)", "jrt.IntToBinary($1)");
        add("Integer.toOctalString(int)", "jrt.IntToOctal($1)");
        add("Integer.compare(int,int)", "jrt.Cmp($1, $2)");
        add("Integer.signum(int)", "jrt.Signum(int64($1))");
        add("Integer.bitCount(int)", "jrt.BitCount(uint64(uint32($1)))");
        add("Integer.max(int,int)", "max($1, $2)");
        add("Integer.min(int,int)", "min($1, $2)");
        add("Integer.sum(int,int)", "($1 + $2)");
        add("Integer.valueOf(int)", "$1");
        add("Integer.intValue()", "$0.(int32)");
        add("Long.parseLong(String)", "jrt.ParseLong($1, 10)");
        add("Long.toString(long)", "jrt.ToStr($1)");
        add("Long.toHexString(long)", "jrt.LongToHex($1)");
        add("Long.toBinaryString(long)", "jrt.LongToBinary($1)");
        add("Long.compare(long,long)", "jrt.Cmp($1, $2)");
        add("Long.signum(long)", "jrt.Signum($1)");
        add("Long.bitCount(long)", "jrt.BitCount(uint64($1))");
        add("Long.max(long,long)", "max($1, $2)");
        add("Long.min(long,long)", "min($1, $2)");
        add("Long.sum(long,long)", "($1 + $2)");
        add("Long.valueOf(long)", "$1");
        add("Long.longValue()", "$0.(int64)");
        add("Double.parseDouble(String)", "jrt.ParseDouble($1)");
        add("Double.toString(double)", "jrt.ToStr($1)");
        add("Double.compare(double,double)", "jrt.DoubleCompare($1, $2)");
        add("Double.isNaN(double)", "math.IsNaN($1)");
        add("Double.isInfinite(double)", "math.IsInf($1, 0)");
        add("Double.max(double,double)", "max($1, $2)");
        add("Double.min(double,double)", "min($1, $2)");
        add("Double.sum(double,double)", "($1 + $2)");
        add("Double.valueOf(double)", "$1");
        add("Double.doubleValue()", "$0.(float64)");
        add("Boolean.parseBoolean(String)", "jrt.ParseBoolean($1)");
        add("Boolean.toString(boolean)", "jrt.ToStr($1)");
        add("Boolean.valueOf(boolean)", "$1");
        add("Boolean.booleanValue()", "$0.(bool)");
        add("Character.isDigit(char)", "jrt.CharIsDigit($1)");
        add("Character.isLetter(char)", "jrt.CharIsLetter($1)");
        add("Character.isLetterOrDigit(char)", "jrt.CharIsLetterOrDigit($1)");
        add("Character.isUpperCase(char)", "jrt.CharIsUpper($1)");
        add("Character.isLowerCase(char)", "jrt.CharIsLower($1)");
        add("Character.isWhitespace(char)", "jrt.CharIsWhitespace($1)");
        add("Character.toUpperCase(char)", "jrt.CharToUpper($1)");
        add("Character.toLowerCase(char)", "jrt.CharToLower($1)");
        add("Character.getNumericValue(char)", "jrt.CharNumeric($1)");
        add("Character.toString(char)", "jrt.ToStr($1)");
        add("Character.valueOf(char)", "$1");
        add("Character.charValue()", "$0.(uint16)");
        add("Math.abs(int)", "jrt.Abs($1)");
        add("Math.abs(long)", "jrt.Abs($1)");
        add("Math.abs(double)", "math.Abs($1)");
        add("Math.abs(float)", "float32(math.Abs(float64($1)))");
        for (const char* ty : {"int", "long", "double", "float"}) {
            t.emplace(std::string("Math.max(") + ty + "," + ty + ")", "max($1, $2)");
            t.emplace(std::string("Math.min(") + ty + "," + ty + ")", "min($1, $2)");
        }
        add("Math.pow(double,double)", "math.Pow($1, $2)");
        add("Math.sqrt(double)", "math.Sqrt($1)");
        add("Math.cbrt(double)", "math.Cbrt($1)");
        add("Math.floor(double)", "math.Floor($1)");
        add("Math.ceil(double)", "math.Ceil($1)");
        add("Math.rint(double)", "jrt.Rint($1)");
        add("Math.round(double)", "jrt.Round($1)");
        add("Math.round(float)", "jrt.RoundF($1)");
        add("Math.sin(double)", "math.Sin($1)");
        add("Math.cos(double)", "math.Cos($1)");
        add("Math.tan(double)", "math.Tan($1)");
        add("Math.asin(double)", "math.Asin($1)");
        add("Math.acos(double)", "math.Acos($1)");
        add("Math.atan(double)", "math.Atan($1)");
        add("Math.atan2(double,double)", "math.Atan2($1, $2)");
        add("Math.exp(double)", "math.Exp($1)");
        add("Math.log(double)", "math.Log($1)");
        add("Math.log10(double)", "math.Log10($1)");
        add("Math.hypot(double,double)", "math.Hypot($1, $2)");
        add("Math.signum(double)", "jrt.SignumD($1)");
        add("Math.toRadians(double)", "($1 * 0.017453292519943295)");
        add("Math.toDegrees(double)", "($1 * 57.29577951308232)");
        add("Math.floorDiv(int,int)", "jrt.FloorDiv($1, $2)");
        add("Math.floorMod(int,int)", "jrt.FloorMod($1, $2)");
        add("Math.floorDiv(long,long)", "jrt.FloorDiv($1, $2)");
        add("Math.floorMod(long,long)", "jrt.FloorMod($1, $2)");
        add("Math.addExact(int,int)", "jrt.AddExact32($1, $2)");
        add("Math.multiplyExact(int,int)", "jrt.MulExact32($1, $2)");
        add("Math.addExact(long,long)", "jrt.AddExact64($1, $2)");
        add("Math.multiplyExact(long,long)", "jrt.MulExact64($1, $2)");
        for (const char* t_ : {"String", "char", "int", "long", "double", "float", "boolean", "Object", "CharSequence"}) t.emplace(std::string("StringBuilder.append(") + t_ + ")", "$0.Append($1)");
        add("StringBuilder.append(char[])", "$0.AppendChars($1)");
        add("StringBuilder.toString()", "$0.ToString()");
        add("StringBuilder.length()", "$0.Length()");
        add("StringBuilder.isEmpty()", "$0.IsEmpty()");
        add("StringBuilder.charAt(int)", "$0.CharAt($1)");
        add("StringBuilder.setCharAt(int,char)", "$0.SetCharAt($1, $2)");
        add("StringBuilder.deleteCharAt(int)", "$0.DeleteCharAt($1)");
        add("StringBuilder.delete(int,int)", "$0.Delete($1, $2)");
        add("StringBuilder.reverse()", "$0.Reverse()");
        add("StringBuilder.setLength(int)", "$0.SetLength($1)");
        add("StringBuilder.indexOf(String)", "$0.IndexOf($1)");
        add("StringBuilder.substring(int)", "$0.Substring($1)");
        add("StringBuilder.substring(int,int)", "$0.SubstringTo($1, $2)");
        for (const char* t_ : {"String", "char", "int", "long", "double", "float", "boolean", "Object"}) t.emplace(std::string("StringBuilder.insert(int,") + t_ + ")", "$0.Insert($1, $2)");
        add("Object.toString()", "jrt.ToStringOf($0)");
        add("Object.equals(Object)", "jrt.Equals($0, $1)");
        add("Object.hashCode()", "jrt.HashCode($0)");
        add("Objects.equals(Object,Object)", "jrt.ObjEquals($1, $2)");
        add("Throwable.getMessage()", "$0.GetMessage()");
        add("Throwable.getLocalizedMessage()", "$0.GetLocalizedMessage()");
        add("Throwable.getCause()", "$0.GetCause()");
        add("Throwable.toString()", "jrt.ToStringOf($0)");
        add("Throwable.printStackTrace()", "$0.PrintStackTrace()");
        add("System.exit(int)", "jrt.Exit($1)");
        add("System.currentTimeMillis()", "jrt.CurrentTimeMillis()");
        add("System.nanoTime()", "jrt.NanoTime()");
        add("System.lineSeparator()", "jrt.LineSeparator()");
        add("System.arraycopy(Object,int,Object,int,int)", "jrt.ArrayCopy($1, $2, $3, $4, $5)");
        for (const char* e : {"int", "long", "double", "float", "char", "boolean", "short", "byte", "Object", "String"}) {
            const std::string a = std::string(e) + "[]";
            t.emplace("Arrays.toString(" + a + ")", "jrt.ArraysToString($1)");
            t.emplace("Arrays.fill(" + a + "," + e + ")", "jrt.ArraysFill($1, $2)");
            t.emplace("Arrays.copyOf(" + a + ",int)", "jrt.ArraysCopyOf($1, $2)");
            t.emplace("Arrays.copyOfRange(" + a + ",int,int)", "jrt.ArraysCopyOfRange($1, $2, $3)");
            if (std::string_view(e) != "Object" && std::string_view(e) != "String") t.emplace("Arrays.equals(" + a + "," + a + ")", "jrt.ArraysEquals($1, $2)");
            if (std::string_view(e) != "boolean" && std::string_view(e) != "Object" && std::string_view(e) != "String") t.emplace("Arrays.sort(" + a + ")", "jrt.ArraysSort($1)");
        }
        add("Arrays.sort(String[])", "jrt.ArraysSortStrings($1)");
        add("List.add(Object)", "$0.Add($1)");
        add("List.add(int,Object)", "$0.AddAt($1, $2)");
        add("List.get(int)", "$0.Get($1)");
        add("List.set(int,Object)", "$0.Set($1, $2)");
        add("List.size()", "$0.Size()");
        add("List.isEmpty()", "$0.IsEmpty()");
        add("List.clear()", "$0.Clear()");
        add("List.contains(Object)", "$0.Contains($1)");
        add("List.indexOf(Object)", "$0.IndexOf($1)");
        add("List.lastIndexOf(Object)", "$0.LastIndexOf($1)");
        add("List.remove(int)", "$0.RemoveAt($1)");
        add("List.remove(Object)", "$0.Remove($1)");
        add("List.iterator()", "$0.Iterator()");
        add("List.addAll(Collection)", "$0.AddAll($1)");
        add("List.containsAll(Collection)", "$0.ContainsAll($1)");
        add("List.toString()", "jrt.ToStringOf($0)");
        add("Set.add(Object)", "$0.Add($1)");
        add("Set.contains(Object)", "$0.Contains($1)");
        add("Set.remove(Object)", "$0.Remove($1)");
        add("Set.size()", "$0.Size()");
        add("Set.isEmpty()", "$0.IsEmpty()");
        add("Set.clear()", "$0.Clear()");
        add("Set.iterator()", "$0.Iterator()");
        add("Set.addAll(Collection)", "$0.AddAll($1)");
        add("Set.containsAll(Collection)", "$0.ContainsAll($1)");
        add("Set.toString()", "jrt.ToStringOf($0)");
        add("Map.put(Object,Object)", "$0.Put($1, $2)");
        add("Map.get(Object)", "$0.Get($1)");
        add("Map.getOrDefault(Object,Object)", "$0.GetOrDefault($1, $2)");
        add("Map.containsKey(Object)", "$0.ContainsKey($1)");
        add("Map.containsValue(Object)", "$0.ContainsValue($1)");
        add("Map.remove(Object)", "$0.Remove($1)");
        add("Map.putIfAbsent(Object,Object)", "$0.PutIfAbsent($1, $2)");
        add("Map.putAll(Map)", "$0.PutAll($1)");
        add("Map.size()", "$0.Size()");
        add("Map.isEmpty()", "$0.IsEmpty()");
        add("Map.clear()", "$0.Clear()");
        add("Map.keySet()", "$0.KeySet()");
        add("Map.values()", "$0.Values()");
        add("Map.entrySet()", "$0.EntrySet()");
        add("Map.toString()", "jrt.ToStringOf($0)");
        add("Entry.getKey()", "$0.GetKey()");
        add("Entry.getValue()", "$0.GetValue()");
        add("Entry.setValue(Object)", "$0.SetValue($1)");
        add("Iterator.hasNext()", "$0.HasNext()");
        add("Iterator.next()", "$0.Next()");
        add("Iterator.remove()", "$0.Remove()");
        return t;
    }();
    return table;
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------- constants

std::optional<std::int64_t> Writer::fold(const ir::Node* e) {
    e = strip_paren(e);
    if (e == nullptr) return std::nullopt;
    const std::string_view kind = kind_of(type_of(e));
    if (kind != "i32" && kind != "i64" && kind != "i16" && kind != "i8" && kind != "u16") return std::nullopt;
    if (const auto* lit = ir::get<ir::Lit>(e)) {
        if (const auto* i = std::get_if<std::int64_t>(&lit->value)) return *i;
        if (const auto* s = std::get_if<std::string>(&lit->value); s != nullptr && kind == "u16" && !s->empty()) {
            const unsigned char c0 = static_cast<unsigned char>((*s)[0]);
            if (c0 < 0x80) return c0;
        }
        return std::nullopt;
    }
    if (const auto* sel = ir::get<ir::Select>(e); sel != nullptr && sel->sym && !p_.model().info(sel->sym->owner.value_or(""))) {
        const std::string key = simple_name(sel->sym->owner.value_or("")) + "." + sel->sym->name;
        if (key == "Integer.MAX_VALUE") return 2147483647LL;
        if (key == "Integer.MIN_VALUE") return -2147483648LL;
        if (key == "Long.MAX_VALUE") return INT64_MAX;
        if (key == "Long.MIN_VALUE") return INT64_MIN;
        return std::nullopt;
    }
    if (const auto* u = ir::get<ir::Unary>(e)) {
        const auto v = fold(u->x);
        if (!v) return std::nullopt;
        if (u->op == "-") return wrap_to(static_cast<std::int64_t>(0 - static_cast<std::uint64_t>(*v)), kind);
        if (u->op == "+") return wrap_to(*v, kind);
        if (u->op == "~") return wrap_to(~*v, kind);
        return std::nullopt;
    }
    if (const auto* c = ir::get<ir::Cast>(e)) {
        const auto v = fold(c->x);
        return v ? std::optional<std::int64_t>(wrap_to(*v, kind)) : std::nullopt;
    }
    if (const auto* b = ir::get<ir::Binary>(e)) {
        if (kind != "i32" && kind != "i64") return std::nullopt;
        const auto l = fold(b->left), r = fold(b->right);
        if (!l || !r) return std::nullopt;
        const std::uint64_t x = static_cast<std::uint64_t>(*l), y = static_cast<std::uint64_t>(*r);
        const unsigned bits = kind == "i32" ? 31 : 63;
        const std::string& op = b->op;
        if (op == "+") return wrap_to(static_cast<std::int64_t>(x + y), kind);
        if (op == "-") return wrap_to(static_cast<std::int64_t>(x - y), kind);
        if (op == "*") return wrap_to(static_cast<std::int64_t>(x * y), kind);
        if (op == "/" || op == "%") {
            if (*r == 0 || (*r == -1 && *l == (kind == "i32" ? INT32_MIN : INT64_MIN))) return std::nullopt;
            return op == "/" ? *l / *r : *l % *r;
        }
        if (op == "&") return *l & *r;
        if (op == "|") return *l | *r;
        if (op == "^") return *l ^ *r;
        if (op == "<<") return wrap_to(static_cast<std::int64_t>(x << (y & bits)), kind);
        if (op == ">>") return *l >> (y & bits);
        if (op == ">>>") return kind == "i32" ? wrap_to(static_cast<std::int64_t>(static_cast<std::uint32_t>(x) >> (y & bits)), kind) : static_cast<std::int64_t>(x >> (y & bits));
        return std::nullopt;
    }
    return std::nullopt;
}

Code Writer::int_literal(std::int64_t v, const ir::TypeP& type) {
    Code c = mk(std::to_string(v), type, v < 0 ? kUnary : kAtom);
    c.bare = true;
    return c;
}

// ---------------------------------------------------------------------------------------------------------------- conversion

Code Writer::from_any(Code c, const ir::TypeP& want) {
    const std::string g = p_.gtype(want);
    if (g.empty() || g == "any") {
        c.type = want;
        return c;
    }
    if (g == "jrt.String") return mk("jrt.AsString(" + c.text + ")", want);
    if (is_prim_kind(kind_of(want))) return mk(wrap(c, kAtom) + ".(" + g + ")", want);
    return mk("jrt.As[" + g + "](" + c.text + ")", want);
}

Code Writer::numeric(const ir::Node* e) {
    Code c = expr(e);
    if (const std::string_view box = boxed_kind(c.type); !box.empty()) return conv(std::move(c), prim_by_kind(box));
    return c;
}

Code Writer::conv(Code value, const ir::TypeP& dst) {
    if (!dst || !value.type) return value;
    const std::string_view sk = kind_of(value.type), dk = kind_of(dst);
    if (dk == "void") return value;
    if (sk == "null") {
        if (dk == "class" || dk == "array" || dk == "typevar") return mk("nil", dst);
        throw Decline("null where a " + std::string(dk) + " is wanted");
    }
    if (is_numeric_kind(sk) && is_numeric_kind(dk)) {
        if (sk == dk) return value;
        if (value.bare) {   // an untyped constant takes the type its place wants
            if (is_float_kind(dk) && is_integral_kind(sk)) value.text += value.text.find_first_not_of("-0123456789") == std::string::npos ? ".0" : "";
            if (is_integral_kind(dk) && is_float_kind(sk)) return mk(go_prim(dk) + "(" + value.text + ")", dst);
            value.type = dst;
            return value;
        }
        if (is_float_kind(sk) && is_integral_kind(dk)) {
            const std::string wide = sk == "f32" ? "float64(" + value.text + ")" : value.text;
            if (dk == "i64") return mk("jrt.D2L(" + wide + ")", dst);
            if (dk == "i32") return mk("jrt.D2I(" + wide + ")", dst);
            return mk(go_prim(dk) + "(jrt.D2I(" + wide + "))", dst);
        }
        return mk(go_prim(dk) + "(" + value.text + ")", dst);
    }
    if (sk == "bool" && dk == "bool") return value;
    if (is_prim_kind(sk) && dk == "class" && p_.gtype(dst) == "any") {   // boxing: the number goes into an `any` as the type Java boxes it as
        const std::string_view box = boxed_kind(dst);
        Code prim = box.empty() ? std::move(value) : conv(std::move(value), prim_by_kind(box));
        return mk(typed_text(prim), dst);
    }
    if (const std::string_view box = boxed_kind(value.type); !box.empty() && is_prim_kind(dk)) {   // unboxing: null is a NullPointerException
        return conv(mk(wrap(value, kAtom) + ".(" + go_prim(box) + ")", prim_by_kind(box)), dst);
    }
    if ((sk == "class" || sk == "array" || sk == "typevar") && (dk == "class" || dk == "array" || dk == "typevar")) {
        value.type = dst;
        return value;
    }
    if (is_prim_kind(sk) && dk == "typevar") return mk(typed_text(value), dst);
    return value;
}

// ---------------------------------------------------------------------------------------------------------------- dispatch

Code Writer::expr(const ir::Node* e) {
    if (e == nullptr) throw Decline("an expression that is missing");
    return ir::visit(*e, [&](const auto& k) -> Code { return on(*e, k); });
}

template <class K> Code Writer::on(const ir::Node& n, const K&) {
    throw Decline("the expression " + std::string(ir::kind_name(n)));
}

Code Writer::on(const ir::Node&, const ir::Paren& k) {
    Code c = expr(k.x);
    if (c.prec < kAtom && c.text.find(' ') != std::string::npos) c.text = "(" + c.text + ")", c.prec = kAtom;
    return c;
}

Code Writer::on(const ir::Node& n, const ir::Lit& k) {
    const ir::LitValue& v = k.value;
    const ir::TypeP type = n.type;
    const std::string_view kind = kind_of(type);
    if (std::holds_alternative<std::monostate>(v)) return mk("nil", ir::null_type());
    if (kind == "bool") {
        const bool b = std::holds_alternative<bool>(v) ? std::get<bool>(v) : (std::holds_alternative<std::int64_t>(v) && std::get<std::int64_t>(v) != 0);
        return mk(b ? "true" : "false", type);
    }
    if (kind == "i8" || kind == "i16" || kind == "i32" || kind == "i64") {
        std::string digits;
        if (const auto* i = std::get_if<std::int64_t>(&v)) digits = std::to_string(*i);
        else if (const auto* u = std::get_if<std::uint64_t>(&v)) digits = std::to_string(*u);
        else throw Decline("an integer literal that is not a number");
        Code c = mk(digits, type, digits.starts_with("-") ? kUnary : kAtom);
        c.bare = true;
        return c;
    }
    if (kind == "f64" || kind == "f32") {
        double d = 0;
        if (const auto* f = std::get_if<double>(&v)) d = *f;
        else if (const auto* i = std::get_if<std::int64_t>(&v)) d = static_cast<double>(*i);
        else if (const auto* u = std::get_if<std::uint64_t>(&v)) d = static_cast<double>(*u);
        else if (const auto* s = std::get_if<std::string>(&v)) {
            if (*s == "NaN") d = std::nan("");
            else if (*s == "Infinity") d = HUGE_VAL;
            else if (*s == "-Infinity") d = -HUGE_VAL;
            else d = std::stod(*s);
        }
        const std::string go = kind == "f32" ? "float32" : "float64";
        if (std::isnan(d)) {
            p_.need_math();
            return mk(go + "(math.NaN())", type);
        }
        if (std::isinf(d)) {
            p_.need_math();
            return mk(go + "(math.Inf(" + (d > 0 ? "1" : "-1") + "))", type);
        }
        const std::string text = float_text(d, kind == "f32");
        Code c = mk(text, type, text.starts_with("-") ? kUnary : kAtom);
        c.bare = true;
        return c;
    }
    if (kind == "u16") {
        std::uint32_t code = 0;
        if (const auto* s = std::get_if<std::string>(&v)) {
            if (s->empty()) throw Decline("an empty char literal");
            const unsigned char c0 = static_cast<unsigned char>((*s)[0]);
            if (c0 < 0x80) code = c0;
            else {
                const std::size_t len = c0 >= 0xF0 ? 4 : c0 >= 0xE0 ? 3 : 2;
                code = len == 2 ? c0 & 0x1F : len == 3 ? c0 & 0x0F : c0 & 0x07;
                for (std::size_t i = 1; i < len && i < s->size(); ++i) code = (code << 6) | (static_cast<unsigned char>((*s)[i]) & 0x3F);
            }
        } else if (const auto* i = std::get_if<std::int64_t>(&v)) {
            code = static_cast<std::uint32_t>(*i);
        } else {
            throw Decline("a char literal that is not a character");
        }
        if (code > 0xFFFF) throw Decline("a char literal above U+FFFF");
        Code c = mk(char_literal(code), type);
        c.bare = true;
        return c;
    }
    if (const auto* s = std::get_if<std::string>(&v)) return mk("jrt.S(" + go_string(*s) + ")", ir::class_type("java.lang.String"));
    throw Decline("a literal of type " + std::string(kind));
}

Code Writer::on(const ir::Node& n, const ir::Name& k) {
    const ir::Sym& sym = need_sym(k.sym, "a name");
    ir::TypeP type = n.type ? n.type : sym.type;
    if (sym.kind != ir::SymKind::Local && sym.kind != ir::SymKind::Param) throw Decline("the name " + sym.name + " is not a local or parameter");
    return mk(ident(sym.name), type);
}

Code Writer::this_code() {
    if (static_) throw Decline("this in a static context");
    return mk(ci_->kind == "interface" ? "this" : "this.self", ir::class_type(ci_->qname));
}

Code Writer::on(const ir::Node&, const ir::This& k) {
    if (k.qualifier && class_name(k.qualifier) != ci_->qname && !p_.model().is_sub(ci_->qname, class_name(k.qualifier))) throw Decline("Outer.this");
    return this_code();
}

Code Writer::on(const ir::Node& n, const ir::Select& k) {
    const ir::Sym& sym = need_sym(k.sym, "a field");
    if (sym.kind == ir::SymKind::EnumConst) {
        const ClassInfo* owner = p_.model().info(sym.owner.value_or(""));
        if (owner == nullptr) throw Decline("the library enum constant " + sym.name);
        return mk(p_.static_name(*owner, field_ident(sym.name)), n.type ? n.type : sym.type);
    }
    if (sym.kind != ir::SymKind::Field) throw Decline("the member " + sym.name + " is not a field");
    const ir::TypeP type = n.type ? n.type : sym.type;
    const ClassInfo* owner = p_.model().info(sym.owner.value_or(""));
    if (owner == nullptr) return lib_static_field(k, type);
    const ClassInfo* dc = p_.declaring_class(*owner, sym.name);
    if (dc == nullptr) throw Decline("the field " + sym.name + " is not declared by " + owner->qname);
    const ir::FieldDecl* decl = nullptr;
    for (const ir::Node* m : dc->members())
        if (const auto* f = ir::get<ir::FieldDecl>(m); f != nullptr && f->sym && f->sym->name == sym.name) decl = f;
    const bool stat = sym.is_static || ir::is<ir::TypeName>(k.obj) || (decl != nullptr && has_mod(decl->mods, "static"));
    Code c;
    if (stat) {
        c = mk(p_.static_name(*dc, field_ident(sym.name)), decl != nullptr ? decl->type : type);
    } else {
        const ir::Node* obj = strip_paren(k.obj);
        std::string base;
        if (obj == nullptr || ir::is<ir::This>(obj)) {
            if (static_) throw Decline("this in a static context");
            base = "this";
        } else if (ir::is<ir::Super>(obj)) {
            base = "this";
        } else {
            base = wrap(expr(obj), kAtom) + "." + p_.accessor(*dc) + "()";
        }
        c = mk(base + "." + field_ident(sym.name), decl != nullptr ? decl->type : type);
    }
    // a field typed as a type variable holds an erased value
    if (decl != nullptr && p_.gtype(decl->type) != p_.gtype(type)) return from_any(c, type);
    c.type = type;
    return c;
}

Code Writer::on(const ir::Node&, const ir::Length& k) {
    const Code a = expr(k.arr);
    return mk("int32(len(" + a.text + "))", ir::i32_type());
}

Code Writer::on(const ir::Node& n, const ir::Index& k) {
    if (k.sym) throw Decline("an indexer");
    const std::vector<ir::Node*> idx = ir::statements(k.index);
    if (idx.size() != 1) throw Decline("an index with several arguments");
    const Code a = expr(k.arr);
    const Code i = conv(numeric(idx[0]), ir::i32_type());
    return mk(wrap(a, kAtom) + "[" + i.text + "]", n.type ? n.type : elem_of(a.type));
}

// ---------------------------------------------------------------------------------------------------------------- operators

Code Writer::on(const ir::Node& n, const ir::Unary& k) {
    const std::string& op = k.op;
    if (op == "++" || op == "--") return incdec_value(k);
    if (op == "!") {
        const Code x = conv(expr(k.x), ir::bool_type());
        return mk("!" + wrap(x, kUnary), ir::bool_type(), kUnary);
    }
    if (const auto v = fold(&n)) return int_literal(*v, n.type);
    const ir::TypeP pt = unary_promoted(eff(type_of(k.x)));
    const Code x = conv(numeric(k.x), pt);
    if (op == "-") {
        Code c = mk("-" + wrap(x, kUnary), pt, kUnary);
        c.bare = x.bare;
        return c;
    }
    if (op == "+") return x;
    if (op == "~") {
        Code c = mk("^" + wrap(x, kUnary), pt, kUnary);
        c.bare = x.bare;
        return c;
    }
    throw Decline("the unary operator " + op);
}

// The Java arithmetic of `l op r`.
Code Writer::arith(const std::string& op, Code l0, Code r0) {
    const std::string_view lk = kind_of(l0.type), rk = kind_of(r0.type);
    if (lk == "bool" && rk == "bool") {
        if (op == "&") return mk("jrt.BAnd(" + l0.text + ", " + r0.text + ")", ir::bool_type());
        if (op == "|") return mk("jrt.BOr(" + l0.text + ", " + r0.text + ")", ir::bool_type());
        if (op == "^") return mk(wrap(l0, kAdd) + " != " + wrap(r0, kAdd), ir::bool_type(), kCmp);
        throw Decline("the operator " + op + " on booleans");
    }
    if (!is_numeric_kind(lk) || !is_numeric_kind(rk)) throw Decline("the operator " + op + " on " + std::string(lk) + " and " + std::string(rk));
    if (op == "<<" || op == ">>" || op == ">>>") {
        if (!is_integral_kind(lk) || !is_integral_kind(rk)) throw Decline("a shift of a non-integer");
        const ir::TypeP pt = unary_promoted(l0.type);
        Code a = conv(l0, pt);
        a.text = typed_text(a);
        a.bare = false;
        const std::string mask = kind_of(pt) == "i64" ? "63" : "31";
        std::string count;
        if (r0.bare) {
            const std::int64_t v = std::stoll(r0.text.starts_with("'") ? "0" : r0.text) & std::stoll(mask);
            count = std::to_string(v);
        } else {
            count = "(" + r0.text + " & " + mask + ")";
        }
        if (op == "<<") return mk(wrap(a, kMul) + " << " + count, pt, kMul);
        if (op == ">>") return mk(wrap(a, kMul) + " >> " + count, pt, kMul);
        const std::string un = kind_of(pt) == "i64" ? "uint64" : "uint32";
        return mk(std::string(kind_of(pt) == "i64" ? "int64" : "int32") + "(" + un + "(" + a.text + ") >> " + count + ")", pt);
    }
    const ir::TypeP pt = promoted(l0.type, r0.type);
    Code a = conv(l0, pt), b = conv(r0, pt);
    const bool is_float = is_float_kind(kind_of(pt));
    if (a.bare && b.bare) {   // two untyped constants would be added exactly: type the first, so Java's rounding is kept
        a.text = typed_text(a);
        a.bare = false;
        a.prec = kAtom;
    }
    if (op == "+" || op == "-") return mk(wrap(a, kAdd) + " " + op + " " + wrap(b, kAdd + 1), pt, kAdd);
    if (op == "*") return mk(wrap(a, kMul) + " * " + wrap(b, kMul + 1), pt, kMul);
    if (op == "/" || op == "%") {
        const bool zero = b.bare && (b.text == "0" || b.text == "0.0");
        if (is_float) {
            if (op == "%") {
                const bool single = kind_of(pt) == "f32";
                return mk(single ? "float32(jrt.FMod(float64(" + typed_text(a) + "), float64(" + typed_text(b) + ")))" : "jrt.FMod(" + typed_text(a) + ", " + typed_text(b) + ")", pt);
            }
            if (zero) return mk("jrt.FDiv(" + typed_text(a) + ", " + typed_text(b) + ")", pt);
            return mk(wrap(a, kMul) + " / " + wrap(b, kMul + 1), pt, kMul);
        }
        if (zero) b = mk(kind_of(pt) == "i64" ? "jrt.Zero64" : "jrt.Zero32", pt);
        return mk(wrap(a, kMul) + " " + op + " " + wrap(b, kMul + 1), pt, kMul);
    }
    if (op == "&") return mk(wrap(a, kMul) + " & " + wrap(b, kMul + 1), pt, kMul);
    if (op == "|" || op == "^") return mk(wrap(a, kAdd) + " " + op + " " + wrap(b, kAdd + 1), pt, kAdd);
    throw Decline("the operator " + op);
}

Code Writer::on(const ir::Node& n, const ir::Binary& k) {
    const std::string& op = k.op;
    if (op == "&&" || op == "||") {
        const Code l = conv(expr(k.left), ir::bool_type()), r = conv(expr(k.right), ir::bool_type());
        const int prec = op == "&&" ? kAnd : kOr;
        return mk(wrap(l, prec) + " " + op + " " + wrap(r, prec + 1), ir::bool_type(), prec);
    }
    if (const auto v = fold(&n)) return int_literal(*v, n.type);
    if (op == "==" || op == "!=") {
        const ir::TypeP lt0 = type_of(k.left), rt0 = type_of(k.right);
        if (!boxed_kind(lt0).empty() && !boxed_kind(rt0).empty()) throw Decline("== on two boxed numbers compares references");
        const ir::TypeP lt = kind_of(rt0) == "null" ? lt0 : eff(lt0), rt = kind_of(lt0) == "null" ? rt0 : eff(rt0);
        const std::string_view lk = kind_of(lt), rk = kind_of(rt);
        if (lk == "null" || rk == "null") {
            const Code other = expr(kind_of(lt) == "null" ? k.right : k.left);
            if (is_prim_kind(kind_of(other.type))) throw Decline("comparing a primitive with null");
            return mk(wrap(other, kAdd) + " " + op + " nil", ir::bool_type(), kCmp);
        }
        if (is_numeric_kind(lk) && is_numeric_kind(rk)) {
            const ir::TypeP pt = promoted(lt, rt);
            Code l = conv(numeric(k.left), pt), r = conv(numeric(k.right), pt);
            if (l.bare && r.bare) l.text = typed_text(l), l.bare = false, l.prec = kAtom;
            return mk(wrap(l, kAdd) + " " + op + " " + wrap(r, kAdd), ir::bool_type(), kCmp);
        }
        if (lk == "bool" && rk == "bool") {
            const Code l = numeric(k.left), r = numeric(k.right);
            return mk(wrap(l, kAdd) + " " + op + " " + wrap(r, kAdd), ir::bool_type(), kCmp);
        }
        if ((lk == "class" || lk == "typevar") && (rk == "class" || rk == "typevar")) {
            const Code l = expr(k.left), r = expr(k.right);
            const std::string lg = p_.gtype(l.type), rg = p_.gtype(r.type);
            if (lg == rg) return mk(wrap(l, kAdd) + " " + op + " " + wrap(r, kAdd), ir::bool_type(), kCmp);
            return mk("any(" + l.text + ") " + op + " any(" + r.text + ")", ir::bool_type(), kCmp);
        }
        if (lk == "array" && rk == "array") {
            const Code l = expr(k.left), r = expr(k.right);
            const std::string test = "jrt.SameArr(" + l.text + ", " + r.text + ")";
            return op == "==" ? mk(test, ir::bool_type()) : mk("!" + test, ir::bool_type(), kUnary);
        }
        throw Decline("comparing " + std::string(lk) + " with " + std::string(rk));
    }
    if (is_comparison(op)) {
        const ir::TypeP pt = promoted(eff(type_of(k.left)), eff(type_of(k.right)));
        Code l = conv(numeric(k.left), pt), r = conv(numeric(k.right), pt);
        if (l.bare && r.bare) l.text = typed_text(l), l.bare = false, l.prec = kAtom;
        return mk(wrap(l, kAdd) + " " + op + " " + wrap(r, kAdd), ir::bool_type(), kCmp);
    }
    if (op == "+" && class_name(n.type) == "java.lang.String") {
        const Code l = expr(k.left), r = expr(k.right);
        return mk("jrt.Cat(" + typed_text(l) + ", " + typed_text(r) + ")", n.type);
    }
    return arith(op, numeric(k.left), numeric(k.right));
}

Code Writer::on(const ir::Node&, const ir::Concat& k) {
    std::vector<std::string> parts;
    for (const ir::Node* p : k.parts) {
        const ir::Node* q = strip_paren(p);
        if (const auto* lit = ir::get<ir::Lit>(q)) {   // a literal part is text already
            if (const auto* s = std::get_if<std::string>(&lit->value); s != nullptr && kind_of(type_of(q)) != "u16") {
                parts.push_back("jrt.S(" + go_string(*s) + ")");
                continue;
            }
            if (std::holds_alternative<std::monostate>(lit->value)) {
                parts.push_back("jrt.S(\"null\")");
                continue;
            }
        }
        const Code c = expr(p);
        parts.push_back(typed_text(c));
    }
    return mk("jrt.Cat(" + join(parts) + ")", ir::class_type("java.lang.String"));
}

Code Writer::on(const ir::Node& n, const ir::Cond& k) {
    const ir::TypeP type = n.type ? n.type : type_of(k.then);
    const Code c = conv(expr(k.cond), ir::bool_type());
    const Code a = conv(expr(k.then), type), b = conv(expr(k.orelse), type);
    const std::string g = p_.gtype(type);
    return mk("func() " + g + " { if " + c.text + " { return " + typed_text(a) + " }; return " + typed_text(b) + " }()", type);
}

Code Writer::on(const ir::Node& n, const ir::Cast& k) {
    const ir::TypeP dst = n.type;
    Code x = expr(k.x);
    const std::string_view sk = kind_of(x.type), dk = kind_of(dst);
    if (is_prim_kind(sk) && is_prim_kind(dk)) {
        if (x.bare && sk != dk) {   // a constant cast is a real cast: (byte) 200 wraps
            if (const auto v = fold(&n)) return int_literal(*v, dst);
            x.text = typed_text(x);
            x.bare = false;
            x.prec = kAtom;
        }
        return conv(std::move(x), dst);
    }
    if (dk == "class" || dk == "array" || dk == "typevar") {
        if (sk == "null") return conv(std::move(x), dst);
        if (is_prim_kind(sk)) return conv(std::move(x), dst);
        if (dk == "array") {
            x.type = dst;
            return x;
        }
        const std::string g = p_.gtype(dst);
        if (g == "any" || p_.gtype(x.type) == g) {
            x.type = dst;
            return x;
        }
        if (p_.model().is_sub(std::string(class_name(x.type)), std::string(class_name(dst)))) {   // an upcast
            x.type = dst;
            return x;
        }
        if (g == "jrt.String") return mk("jrt.AsString(" + x.text + ")", dst);
        if (g == "jrt.Throwable" || g.starts_with("jrt.") || g.starts_with("*")) {
            x.type = dst;
            return x;
        }
        const ClassInfo* target = p_.program_class(dst);
        std::string bin(class_name(dst));
        if (target != nullptr) {
            std::string name = target->qname;
            for (const ClassInfo* o = target->outer; o != nullptr && name.starts_with(o->qname + "."); o = o->outer) name = name.substr(0, o->qname.size()) + "$" + name.substr(o->qname.size() + 1);
            bin = name;
        }
        return mk("jrt.Cast[" + g + "](" + x.text + ", " + go_string(bin) + ")", dst);
    }
    if (is_prim_kind(dk) && !boxed_kind(x.type).empty()) return conv(std::move(x), dst);
    if (is_prim_kind(dk) && p_.gtype(x.type) == "any") return mk(wrap(x, kAtom) + ".(" + p_.gtype(dst) + ")", dst);
    throw Decline("a cast to " + std::string(dk));
}

Code Writer::on(const ir::Node&, const ir::InstanceOf& k) {
    if (k.binding != nullptr) throw Decline("an instanceof pattern the lowering left");
    const ir::TypeP test = k.test;
    if (kind_of(test) != "class") throw Decline("an instanceof of an array");
    const Code x = expr(k.x);
    const std::string g = p_.gtype(test);
    if (p_.program_class(test) != nullptr) return mk("jrt.Is[" + g + "](" + x.text + ")", ir::bool_type());
    return mk("jrt.InstanceOf(" + x.text + ", " + go_string(std::string(class_name(test))) + ")", ir::bool_type());
}

// ---------------------------------------------------------------------------------------------------------------- places and assignment

// Whether evaluating the expression twice is the same as once, including an index of names.
static bool pure_place(const ir::Node* t) {
    t = strip_paren(t);
    if (const auto* ix = ir::get<ir::Index>(t)) {
        const std::vector<ir::Node*> idx = ir::statements(ix->index);
        return pure_place(ix->arr) && idx.size() == 1 && (is_pure(idx[0]) || ir::is<ir::Lit>(strip_paren(idx[0])));
    }
    return is_pure(t);
}

void Writer::check_order(const ir::Node* target) {
    target = strip_paren(target);
    const ir::Sym* sym = nullptr;
    if (const auto* name = ir::get<ir::Name>(target)) sym = name->sym.get();
    else if (const auto* sel = ir::get<ir::Select>(target)) sym = sel->sym.get();
    if (sym == nullptr || cur_stmt_ == nullptr) return;
    int count = 0;
    for_each_node(cur_stmt_, [&](const ir::Node* n) {
        if (const auto* name = ir::get<ir::Name>(n); name != nullptr && name->sym && name->sym->name == sym->name && name->sym->kind == sym->kind) ++count;
        else if (const auto* sel = ir::get<ir::Select>(n); sel != nullptr && sel->sym && sel->sym->name == sym->name && sel->sym->owner == sym->owner) ++count;
    });
    if (count > 1) throw Decline("an assignment or increment used as a value where " + sym->name + " is also used (Go leaves the order open)");
}

Writer::Place Writer::place_of(const ir::Node* target) {
    target = strip_paren(target);
    Place p;
    p.type = type_of(target);
    if (const auto* name = ir::get<ir::Name>(target)) {
        p.read = p.lvalue = ident(need_sym(name->sym, "a name").name);
        return p;
    }
    if (const auto* sel = ir::get<ir::Select>(target)) {
        const ir::Sym& sym = need_sym(sel->sym, "a field");
        if (p_.model().info(sym.owner.value_or("")) == nullptr) throw Decline("assigning the library field " + sym.name);
        const Code read = on(*target, *sel);
        p.read = p.lvalue = read.text;
        if (read.text.starts_with("jrt.As")) throw Decline("assigning a field of a generic type through its erased value");
        return p;
    }
    if (const auto* idx = ir::get<ir::Index>(target)) {
        const Code c = on(*target, *idx);
        p.read = p.lvalue = c.text;
        return p;
    }
    throw Decline("an assignment to " + std::string(ir::kind_name(*target)));
}

// `target = value` or `target op= value` as one Go statement.
std::string Writer::assign_text(const ir::Node* target, const ir::Node* value, const std::optional<std::string>& op) {
    const Place pl = place_of(target);
    const ir::TypeP tt = pl.type;
    if (!op) {
        if (const auto* na = ir::get<ir::NewArray>(strip_paren(value)); na != nullptr && !value->type) const_cast<ir::Node*>(value)->type = tt;
        return pl.lvalue + " = " + value_of(value, tt);
    }
    if (class_name(tt) == "java.lang.String" && *op == "+") return pl.lvalue + " = jrt.Cat(" + pl.read + ", " + typed_text(expr(value)) + ")";
    const Code cur = mk(pl.read, eff(tt));
    const Code v = numeric(value);
    const std::string_view tk = kind_of(eff(tt));
    const bool shift = *op == "<<" || *op == ">>" || *op == ">>>";
    const bool direct = !shift && is_numeric_kind(tk) && kind_of(promoted(cur.type, v.type)) == tk && !(is_float_kind(tk) && *op == "%") && (*op == "+" || *op == "-" || *op == "*" || *op == "/" || *op == "%" || *op == "&" || *op == "|" || *op == "^") && !(v.bare && (v.text == "0" || v.text == "0.0") && (*op == "/" || *op == "%"));
    if (direct && boxed_kind(tt).empty()) {
        const Code rhs = conv(v, tt);
        return pl.lvalue + " " + *op + "= " + rhs.text;
    }
    const Code res = arith(*op, cur, v);
    return pl.lvalue + " = " + typed_text(conv(res, eff(tt))) ;
}

Code Writer::assign_value(const ir::Node* target, const ir::Node* value, const std::optional<std::string>& op) {
    if (!pure_place(target)) throw Decline("an assignment used as a value to a place with effects");
    check_order(target);
    const Place pl = place_of(target);
    const std::string stmt = assign_text(target, value, op);
    const std::string g = p_.gtype(pl.type);
    return mk("func() " + g + " { " + stmt + "; return " + pl.read + " }()", pl.type);
}

Code Writer::on(const ir::Node&, const ir::Assign& k) { return assign_value(k.target, k.value, k.op); }

std::string Writer::incdec_text(const ir::Unary& k) {
    const Place pl = place_of(k.x);
    if (!is_numeric_kind(kind_of(eff(pl.type))) || !boxed_kind(pl.type).empty()) throw Decline("++ or -- on " + std::string(kind_of(pl.type)));
    return pl.lvalue + (k.op == "++" ? "++" : "--");
}

Code Writer::incdec_value(const ir::Unary& k) {
    if (!pure_place(k.x)) throw Decline("an increment used as a value on a place with effects");
    check_order(k.x);
    const Place pl = place_of(k.x);
    const std::string stmt = incdec_text(k);
    const std::string g = p_.gtype(pl.type);
    if (k.postfix) return mk("func() " + g + " { v := " + pl.read + "; " + stmt + "; return v }()", pl.type);
    return mk("func() " + g + " { " + stmt + "; return " + pl.read + " }()", pl.type);
}

// ---------------------------------------------------------------------------------------------------------------- calls

Code Writer::lib_static_field(const ir::Select& sel, const ir::TypeP& type) {
    const ir::Sym& sym = *sel.sym;
    const std::string key = simple_name(sym.owner.value_or("")) + "." + sym.name;
    static const std::map<std::string, std::string> fields = {
        {"Integer.MAX_VALUE", "math.MaxInt32"},  {"Integer.MIN_VALUE", "math.MinInt32"},   {"Long.MAX_VALUE", "math.MaxInt64"},     {"Long.MIN_VALUE", "math.MinInt64"},
        {"Short.MAX_VALUE", "math.MaxInt16"},    {"Short.MIN_VALUE", "math.MinInt16"},     {"Byte.MAX_VALUE", "math.MaxInt8"},      {"Byte.MIN_VALUE", "math.MinInt8"},
        {"Character.MAX_VALUE", "65535"},        {"Character.MIN_VALUE", "0"},             {"Double.MAX_VALUE", "math.MaxFloat64"}, {"Double.MIN_VALUE", "math.SmallestNonzeroFloat64"},
        {"Float.MAX_VALUE", "math.MaxFloat32"},  {"Float.MIN_VALUE", "math.SmallestNonzeroFloat32"}, {"Math.PI", "math.Pi"},         {"Math.E", "math.E"},
        {"Double.POSITIVE_INFINITY", "math.Inf(1)"}, {"Double.NEGATIVE_INFINITY", "math.Inf(-1)"}, {"Double.NaN", "math.NaN()"},
        {"Float.POSITIVE_INFINITY", "float32(math.Inf(1))"}, {"Float.NEGATIVE_INFINITY", "float32(math.Inf(-1))"}, {"Float.NaN", "float32(math.NaN())"},
        {"System.out", "jrt.Out"}, {"System.err", "jrt.Err"}};
    const auto it = fields.find(key);
    if (it == fields.end()) throw Decline("the library field " + key);
    if (it->second.find("math.") != std::string::npos) p_.need_math();
    Code c = mk(it->second, type);
    if (key == "Math.PI" || key == "Math.E") c.text = "float64(" + c.text + ")";
    else if (it->second.starts_with("math.Max") || it->second.starts_with("math.Min") || it->second.starts_with("math.Smallest")) c.bare = true;
    else if (key == "Character.MAX_VALUE" || key == "Character.MIN_VALUE") c.bare = true;
    return c;
}

Code Writer::method_call(const ir::Node& n, const ir::Call& k) {
    const ir::Sym& s = *k.sym;
    const auto match = p_.model().method_decl(s);
    const ir::MethodDecl& m = method_of(match->method);
    const ClassInfo& dc = *match->cls;
    const jmodel::Override sig = p_.signature(dc, match->method);
    std::vector<std::string> args;
    {   // arguments, the trailing ones of a variable-arity call packed into an array
        std::vector<ir::TypeP> params = sig.params;
        const bool varargs = s.varargs || (m.sym && m.sym->varargs);
        std::size_t fixed = varargs && !params.empty() ? params.size() - 1 : params.size();
        for (std::size_t i = 0; i < k.args.size() && i < fixed; ++i) args.push_back(value_of(k.args[i], params[i]));
        if (varargs && !params.empty()) {
            const ir::TypeP arr = params.back();
            const bool passes_array = k.args.size() == params.size() && kind_of(type_of(k.args[fixed])) == "array";
            if (passes_array) {
                args.push_back(value_of(k.args[fixed], arr));
            } else {
                std::vector<std::string> items;
                for (std::size_t i = fixed; i < k.args.size(); ++i) items.push_back(value_of(k.args[i], elem_of(arr)));
                args.push_back(p_.gtype(arr) + "{" + join(items) + "}");
            }
        }
    }
    const ir::TypeP type = n.type ? n.type : s.ret;
    const auto finish = [&](Code c) {
        const std::string ret = p_.gtype(sig.result);
        if (!ret.empty() && kind_of(type) != "void" && p_.gtype(type) != ret) return from_any(std::move(c), type);
        c.type = type;
        return c;
    };
    if (is_static(m)) return finish(mk(p_.static_name(dc, p_.mname(m)) + "(" + join(args) + ")", type));
    const ir::Node* target = strip_paren(k.target);
    const bool implicit_this = target == nullptr || ir::is<ir::This>(target);
    const bool super_call = ir::is<ir::Super>(target);
    if (p_.is_object_method(m) && !super_call) {   // toString, equals and hashCode are found by the runtime
        const Code recv = implicit_this ? this_code() : expr(k.target);
        if (m.name == "toString") return mk("jrt.ToStringOf(" + recv.text + ")", type);
        if (m.name == "equals") return mk("jrt.Equals(" + recv.text + ", " + join(args) + ")", type);
        return mk("jrt.HashCode(" + recv.text + ")", type);
    }
    if (super_call) {
        if (dc.kind == "interface") return finish(mk(p_.static_name(dc, p_.mname(m)) + "_default(" + join(std::vector<std::string>{"this.self"}) + (args.empty() ? "" : ", " + join(args)) + ")", type));
        return finish(mk("this." + p_.obj(dc) + "." + p_.mname(m) + "(" + join(args) + ")", type));
    }
    if (implicit_this) {
        if (static_) throw Decline("this in a static context");
        if (ci_->kind == "interface") return finish(mk("this." + p_.mname(m) + "(" + join(args) + ")", type));
        if (is_private(m)) return finish(mk("this." + p_.mname(m) + "(" + join(args) + ")", type));
        return finish(mk("this.self." + p_.mname(m) + "(" + join(args) + ")", type));
    }
    const Code recv = expr(k.target);
    if (is_private(m)) return finish(mk(wrap(recv, kAtom) + "." + p_.accessor(dc) + "()." + p_.mname(m) + "(" + join(args) + ")", type));
    return finish(mk(wrap(recv, kAtom) + "." + p_.mname(m) + "(" + join(args) + ")", type));
}

Code Writer::on(const ir::Node& n, const ir::Call& k) {
    if (!k.sym || k.sym->kind != ir::SymKind::Method) throw Decline("a call that is not to a method");
    if (const ClassInfo* owner = p_.model().info(k.sym->owner.value_or("")); owner != nullptr && owner->kind == "enum" && !p_.model().method_decl(*k.sym)) {
        if (k.sym->name == "values" && k.args.empty()) return mk(p_.static_name(*owner, "values") + "()", n.type);
        if (k.sym->name == "valueOf" && k.args.size() == 1) return mk(p_.static_name(*owner, "valueOf") + "(" + value_of(k.args[0], ir::class_type("java.lang.String")) + ")", n.type);
    }
    if (const auto match = p_.model().method_decl(*k.sym)) return method_call(n, k);
    return lib_call(n, k);
}

Code Writer::lib_call(const ir::Node& n, const ir::Call& k) {
    const ir::Sym& s = *k.sym;
    std::string owner = s.owner.value_or("");
    const ir::TypeP type = n.type ? n.type : s.ret;
    const ir::Node* target = strip_paren(k.target);
    if (const ClassInfo* oc = p_.model().info(owner); owner == "java.lang.Enum" || (oc != nullptr && oc->kind == "enum")) {   // what every enum has
        const Code recv = (k.target == nullptr || ir::is<ir::This>(target)) ? this_code() : expr(k.target);
        if (s.name == "name" && k.args.empty()) return mk(wrap(recv, kAtom) + ".Name()", type);
        if (s.name == "ordinal" && k.args.empty()) return mk(wrap(recv, kAtom) + ".Ordinal()", type);
        if (s.name == "toString" && k.args.empty()) return mk("jrt.ToStringOf(" + recv.text + ")", type);
        if (s.name == "hashCode" && k.args.empty()) return mk("jrt.HashCode(" + recv.text + ")", type);
        if (s.name == "equals" && k.args.size() == 1) return mk("jrt.Equals(" + recv.text + ", " + expr(k.args[0]).text + ")", type);
        if (s.name == "compareTo" && k.args.size() == 1) return mk(wrap(recv, kAtom) + ".CompareTo(" + expr(k.args[0]).text + ")", type);
        throw Decline("the enum method " + s.name);
    }
    if (owner == "java.lang.Throwable" || p_.is_lib_throwable(owner)) owner = "java.lang.Throwable";
    if (owner == "java.lang.AbstractStringBuilder") owner = "java.lang.StringBuilder";
    std::string family;
    if (owner == "java.util.List" || owner == "java.util.ArrayList" || owner == "java.util.Collection" || owner == "java.util.AbstractList" || owner == "java.util.LinkedList") family = "List";
    else if (owner == "java.util.Set" || owner == "java.util.HashSet" || owner == "java.util.LinkedHashSet") family = "Set";
    else if (owner == "java.util.Map" || owner == "java.util.HashMap" || owner == "java.util.LinkedHashMap") family = "Map";
    else if (owner == "java.util.Iterator") family = "Iterator";
    else if (owner == "java.util.Map.Entry") family = "Entry";
    const std::string simple = family.empty() ? simple_name(owner) : family;
    if (ir::is<ir::Super>(target)) throw Decline("a call to " + owner + "." + s.name + " through super");

    if (owner == "java.io.PrintStream") {
        const std::string out = expr(k.target).text;
        if (s.name == "println" || s.name == "print") {
            if (k.args.empty()) return mk(out + ".Println()", type);
            if (k.args.size() != 1) throw Decline("PrintStream." + s.name + " with several arguments");
            const Code a = expr(k.args[0]);
            if (kind_of(a.type) == "array") {
                if (kind_of(elem_of(a.type)) != "u16") throw Decline("printing an array");
                return mk(out + (s.name == "println" ? ".PrintlnChars(" : ".PrintChars(") + a.text + ")", type);
            }
            return mk(out + "." + (s.name == "println" ? "Println" : "Print") + "(" + typed_text(a) + ")", type);
        }
        if (s.name == "flush") return mk(out + ".Flush()", type);
        if (s.name == "printf" || s.name == "format") {
            if (k.args.empty() || class_name(type_of(k.args[0])) != "java.lang.String") throw Decline("printf without a format string");
            std::vector<std::string> items = {expr(k.args[0]).text};
            for (std::size_t i = 1; i < k.args.size(); ++i) {
                const Code a = expr(k.args[i]);
                if (kind_of(a.type) == "array") throw Decline("printf with an array argument");
                items.push_back(typed_text(a));
            }
            return mk(out + ".Printf(" + join(items) + ")", type);
        }
        throw Decline("PrintStream." + s.name);
    }
    if (owner == "java.lang.String" && s.is_static && (s.name == "format" || s.name == "join")) {
        if (s.name == "format") {
            if (k.args.empty() || class_name(type_of(k.args[0])) != "java.lang.String") throw Decline("String.format with a Locale");
            std::vector<std::string> items;
            for (std::size_t i = 1; i < k.args.size(); ++i) {
                const Code a = expr(k.args[i]);
                if (kind_of(a.type) == "array") throw Decline("String.format with an array argument");
                items.push_back(typed_text(a));
            }
            return mk("jrt.Format(" + expr(k.args[0]).text + ", []any{" + join(items) + "})", type);
        }
        if (k.args.size() < 2) throw Decline("String.join without parts");
        if (k.args.size() == 2 && kind_of(type_of(k.args[1])) == "array") return mk("jrt.StringJoin(" + expr(k.args[0]).text + ", " + expr(k.args[1]).text + ")", type);
        if (k.args.size() == 2 && p_.gtype(type_of(k.args[1])) == "jrt.Collection") return mk("jrt.StringJoinOf(" + expr(k.args[0]).text + ", " + expr(k.args[1]).text + ")", type);
        std::vector<std::string> items;
        for (std::size_t i = 1; i < k.args.size(); ++i) items.push_back(expr(k.args[i]).text);
        return mk("jrt.StringJoin(" + expr(k.args[0]).text + ", []jrt.String{" + join(items) + "})", type);
    }
    if (owner == "java.lang.String" && s.name == "formatted") {
        std::vector<std::string> items;
        for (const ir::Node* a : k.args) items.push_back(typed_text(expr(a)));
        return mk("jrt.Format(" + expr(k.target).text + ", []any{" + join(items) + "})", type);
    }

    std::vector<std::string> codes;
    for (const ir::TypeP& p : s.params) codes.push_back(java_code(p));
    const std::string key = simple + "." + s.name + "(" + join(codes, ",") + ")";
    const auto it = library_table().find(key);
    if (it == library_table().end()) throw Decline("the library method " + key);
    const std::string& pattern = it->second;

    Code recv;
    const bool stat = s.is_static || ir::is<ir::TypeName>(target);
    if (!stat) recv = (target == nullptr || ir::is<ir::This>(target)) ? this_code() : expr(k.target);
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
            else if (family == "Map" && (s.name == "containsValue" || (i == 1 && (s.name == "put" || s.name == "putIfAbsent" || s.name == "getOrDefault")))) at = 1;
            if (at < held.size() && s.name != "remove" && s.name != "contains" && s.name != "indexOf" && s.name != "lastIndexOf" && s.name != "get" && s.name != "containsKey") want = held[at];
            else if (at >= held.size() && !is_prim_kind(kind_of(type_of(k.args[i])))) want = ir::class_type("java.lang.Object");
            else if (at >= held.size()) throw Decline("a collection call on a receiver whose element type is not known");
            else want = ir::class_type("java.lang.Object");
        }
        const Code a = expr(k.args[i]);
        if (p_.gtype(want) == "any" && kind_of(a.type) == "array") args.push_back(a);
        else args.push_back(conv(a, want));
    }
    std::string text;
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        if (pattern[i] != '$') {
            text += pattern[i];
            continue;
        }
        const std::size_t slot = static_cast<std::size_t>(pattern[++i] - '0');
        if (slot == 0) {
            text += wrap(recv, kAtom);
        } else {
            if (slot > args.size()) throw Decline("the call " + key + " has too few arguments");
            text += typed_text(args[slot - 1]);
        }
    }
    if (text.find("math.") != std::string::npos) p_.need_math();
    Code result = mk(text, type, pattern.starts_with("(") || pattern.find(' ') == std::string::npos ? kAtom : kAtom);
    // a collection's elements come out of it as `any`
    const bool returns_any = (!family.empty() && s.ret && (kind_of(s.ret) == "typevar" || class_name(s.ret) == "java.lang.Object") && s.name != "toString");
    if (returns_any && kind_of(type) != "void") return from_any(result, type);
    return result;
}

Code Writer::lib_new(const ir::Node& n, const ir::New& k) {
    const ir::Sym& s = need_sym(k.sym, "a constructor");
    const std::string owner = s.owner.value_or("");
    const ir::TypeP type = n.type;
    if (owner == "java.lang.StringBuilder") {
        if (k.args.empty() || (s.params.size() == 1 && kind_of(s.params[0]) == "i32")) {
            if (!k.args.empty() && !is_pure(k.args[0])) throw Decline("a StringBuilder capacity that is computed");
            return mk("jrt.NewStringBuilder()", type);
        }
        if (s.params.size() == 1 && class_name(s.params[0]) == "java.lang.String") return mk("jrt.NewStringBuilderOf(" + expr(k.args[0]).text + ")", type);
        throw Decline("a StringBuilder of a CharSequence");
    }
    const bool is_list = owner == "java.util.ArrayList" || owner == "java.util.LinkedList";
    const bool is_map = owner == "java.util.HashMap" || owner == "java.util.LinkedHashMap";
    const bool is_set = owner == "java.util.HashSet" || owner == "java.util.LinkedHashSet";
    if (is_list || is_map || is_set) {
        const bool linked = owner.starts_with("java.util.Linked") && !is_list;
        const std::string base = is_list ? "ArrayList" : is_map ? (linked ? "LinkedHashMap" : "HashMap") : (linked ? "LinkedHashSet" : "HashSet");
        if (k.args.empty()) return mk("jrt.New" + base + "()", type);
        if (s.params.size() == 1 && kind_of(s.params[0]) == "i32") {
            if (linked) return mk("jrt.New" + base + "()", type);
            return mk("jrt.New" + base + "Cap(" + expr(k.args[0]).text + ")", type);
        }
        if (s.params.size() == 1) return mk("jrt.New" + base + "Of(" + expr(k.args[0]).text + ")", type);
        throw Decline("a " + owner + " built from these arguments");
    }
    if (owner == "java.lang.String" && s.params.size() == 1 && kind_of(s.params[0]) == "array") return mk("jrt.StringValueOfChars(" + expr(k.args[0]).text + ")", type);
    if (owner == "java.lang.String" && s.params.empty()) return mk("jrt.FromUnits(nil)", type);
    if (p_.is_lib_throwable(owner)) {
        const std::string q = go_string(owner);
        const std::size_t count = k.args.size();
        if (count == 0) return mk("jrt.NewThrowable(" + q + ", nil)", type);
        if (count == 1 && class_name(s.params[0]) == "java.lang.String") return mk("jrt.NewThrowable(" + q + ", " + expr(k.args[0]).text + ")", type);
        if (count == 1) return mk("jrt.NewThrowableCause(" + q + ", " + expr(k.args[0]).text + ")", type);
        if (count == 2) return mk("jrt.NewThrowableMC(" + q + ", " + expr(k.args[0]).text + ", " + expr(k.args[1]).text + ")", type);
    }
    throw Decline("new of the library class " + owner);
}

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
    const std::vector<std::string> args = call_args(k.args, params);
    return mk("New" + cls->cname + p_.ctor_suffix(*cls, match->method) + "(" + join(args) + ")", ir::class_type(cls->qname));
}

Code Writer::on(const ir::Node& n, const ir::NewArray& k) {
    const ir::TypeP type = n.type;
    if (kind_of(type) != "array") throw Decline("an array creation with no array type");
    const ir::TypeP elem = elem_of(type);
    const std::string et = p_.gtype(elem);
    if (!k.init.empty() || k.dims.empty()) {   // `new T[] { ... }` (also an empty one)
        std::vector<std::string> items;
        for (const ir::Node* x : k.init) {
            if (ir::is<ir::NewArray>(x) && !x->type) const_cast<ir::Node*>(x)->type = elem;
            items.push_back(value_of(x, elem));
        }
        return mk("[]" + et + "{" + join(items) + "}", type);
    }
    std::vector<std::string> dims;
    for (const ir::Node* d : k.dims) dims.push_back(typed_text(conv(numeric(d), ir::i32_type())));
    if (dims.size() == 1) return mk("make([]" + et + ", " + dims[0] + ")", type);
    ir::TypeP leaf = type;   // what the innermost arrays hold
    for (std::size_t i = 0; i < dims.size(); ++i) leaf = elem_of(leaf);
    if (dims.size() == 2) return mk("jrt.NewArr2[" + p_.gtype(leaf) + "](" + join(dims) + ")", type);
    if (dims.size() == 3) return mk("jrt.NewArr3[" + p_.gtype(leaf) + "](" + join(dims) + ")", type);
    throw Decline("an array of " + std::to_string(dims.size()) + " dimensions");
}

}  // namespace cb::translate::backends::go_java::detail
