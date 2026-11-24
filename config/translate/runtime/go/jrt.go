// Package jrt is the runtime of the Go translation of a Java program (CodeBreaker, backends/go_java).
//
// What the translation leans on, and how Java's behaviour is kept:
//
//   - Java's int, long, short, byte, char, float and double are int32, int64, int16, int8, uint16, float32 and float64, whose arithmetic wraps
//     as Java's does. Division by zero is a Go panic that Try turns into an ArithmeticException.
//   - Object is `any`; the boxed numbers are the Go number types held in it; null is nil.
//   - A String is Java's UTF-16 text, immutable and nullable (an interface, so nil is null and a method call on it is a NullPointerException).
//   - An exception is a panic carrying a Throwable; Try catches it, and the Go run-time errors (nil, index, division, type assertion) are
//     turned into the Java exceptions they stand for.
//   - Cat is Java's string conversion (`"" + x`), so concatenation, append and println take any value.
//   - HashMap and HashSet keep Java's bucket order, so they print and iterate as Java's do.
package jrt

import (
	"bufio"
	"math"
	"os"
	"runtime"
	"strconv"
	"strings"
	"unicode"
	"unicode/utf16"
	"unicode/utf8"
)

// ---- Strings --------------------------------------------------------------------------------------------------------------------

// String is a Java String.
type String interface {
	Length() int32
	CharAt(i int32) uint16
	IsEmpty() bool
	IsBlank() bool
	Substring(from int32) String
	SubstringTo(from, to int32) String
	IndexOf(s String) int32
	IndexOfFrom(s String, from int32) int32
	IndexOfChar(c int32) int32
	IndexOfCharFrom(c, from int32) int32
	LastIndexOf(s String) int32
	LastIndexOfChar(c int32) int32
	LastIndexOfCharFrom(c, from int32) int32
	Equals(o any) bool
	EqualsIgnoreCase(o String) bool
	CompareTo(o String) int32
	CompareToIgnoreCase(o String) int32
	HashCode() int32
	StartsWith(p String) bool
	StartsWithFrom(p String, at int32) bool
	EndsWith(p String) bool
	Contains(p any) bool
	ToUpperCase() String
	ToLowerCase() String
	Trim() String
	Strip() String
	Concat(o String) String
	Repeat(n int32) String
	ToCharArray() []uint16
	ToString() String
	Split(re String) []String
	ReplaceChar(a, b uint16) String
	Replace(a, b any) String
	Chars() []uint16
	GoString() string
	isString()
}

type str struct{ u []uint16 }

var interned = map[string]String{}

// S is a string literal.
func S(s string) String {
	if v, ok := interned[s]; ok {
		return v
	}
	v := &str{utf16.Encode([]rune(s))}
	interned[s] = v
	return v
}

// FromUnits makes a String of UTF-16 units (no copy is made).
func FromUnits(u []uint16) String { return &str{u} }

func (s *str) isString()          {}
func (s *str) Chars() []uint16    { return s.u }
func (s *str) Length() int32      { return int32(len(s.u)) }
func (s *str) IsEmpty() bool      { return len(s.u) == 0 }
func (s *str) ToString() String   { return s }
func (s *str) GoString() string   { return string(utf16.Decode(s.u)) }
func (s *str) CharAt(i int32) uint16 {
	if i < 0 || int(i) >= len(s.u) {
		ThrowMsg("java.lang.StringIndexOutOfBoundsException", "Index "+strconv.Itoa(int(i))+" out of bounds for length "+strconv.Itoa(len(s.u)))
	}
	return s.u[i]
}
func (s *str) IsBlank() bool {
	for _, c := range s.u {
		if !isWS(c) {
			return false
		}
	}
	return true
}
func isWS(c uint16) bool {
	return c == ' ' || (c >= 9 && c <= 13) || (c >= 28 && c <= 31) || (c > 127 && unicode.IsSpace(rune(c)) && c != 0xA0 && c != 0x2007 && c != 0x202F)
}
func (s *str) Substring(from int32) String { return s.SubstringTo(from, int32(len(s.u))) }
func (s *str) SubstringTo(from, to int32) String {
	if from < 0 || to > int32(len(s.u)) || from > to {
		ThrowMsg("java.lang.StringIndexOutOfBoundsException", "begin "+strconv.Itoa(int(from))+", end "+strconv.Itoa(int(to))+", length "+strconv.Itoa(len(s.u)))
	}
	return &str{s.u[from:to:to]}
}
func indexUnits(h, n []uint16, from int) int {
	if from < 0 {
		from = 0
	}
	for i := from; i+len(n) <= len(h); i++ {
		j := 0
		for j < len(n) && h[i+j] == n[j] {
			j++
		}
		if j == len(n) {
			return i
		}
	}
	return -1
}
func (s *str) IndexOf(o String) int32 { return int32(indexUnits(s.u, o.Chars(), 0)) }
func (s *str) IndexOfFrom(o String, from int32) int32 {
	return int32(indexUnits(s.u, o.Chars(), int(from)))
}
func (s *str) IndexOfChar(c int32) int32 { return s.IndexOfCharFrom(c, 0) }
func (s *str) IndexOfCharFrom(c, from int32) int32 {
	n := charUnits(c)
	return int32(indexUnits(s.u, n, int(from)))
}
func charUnits(c int32) []uint16 {
	if c >= 0x10000 {
		return utf16.Encode([]rune{rune(c)})
	}
	return []uint16{uint16(c)}
}
func lastIndexUnits(h, n []uint16, from int) int {
	if from > len(h)-len(n) {
		from = len(h) - len(n)
	}
	for i := from; i >= 0; i-- {
		j := 0
		for j < len(n) && h[i+j] == n[j] {
			j++
		}
		if j == len(n) {
			return i
		}
	}
	return -1
}
func (s *str) LastIndexOf(o String) int32 { return int32(lastIndexUnits(s.u, o.Chars(), len(s.u))) }
func (s *str) LastIndexOfChar(c int32) int32 {
	return int32(lastIndexUnits(s.u, charUnits(c), len(s.u)))
}
func (s *str) LastIndexOfCharFrom(c, from int32) int32 {
	return int32(lastIndexUnits(s.u, charUnits(c), int(from)))
}
func (s *str) Equals(o any) bool {
	t, ok := o.(*str)
	if !ok || len(t.u) != len(s.u) {
		return false
	}
	for i, c := range s.u {
		if t.u[i] != c {
			return false
		}
	}
	return true
}
func foldUnit(c uint16) uint16 {
	return uint16(unicode.ToLower(unicode.ToUpper(rune(c))))
}
func (s *str) EqualsIgnoreCase(o String) bool {
	if o == nil || o.Length() != s.Length() {
		return false
	}
	for i, c := range s.u {
		if c != o.Chars()[i] && foldUnit(c) != foldUnit(o.Chars()[i]) {
			return false
		}
	}
	return true
}
func (s *str) CompareTo(o String) int32 {
	t := o.Chars()
	n := len(s.u)
	if len(t) < n {
		n = len(t)
	}
	for i := 0; i < n; i++ {
		if s.u[i] != t[i] {
			return int32(s.u[i]) - int32(t[i])
		}
	}
	return int32(len(s.u) - len(t))
}
func (s *str) CompareToIgnoreCase(o String) int32 {
	t := o.Chars()
	n := len(s.u)
	if len(t) < n {
		n = len(t)
	}
	for i := 0; i < n; i++ {
		if s.u[i] != t[i] {
			a, b := foldUnit(s.u[i]), foldUnit(t[i])
			if a != b {
				return int32(a) - int32(b)
			}
		}
	}
	return int32(len(s.u) - len(t))
}
func (s *str) HashCode() int32 {
	var h int32
	for _, c := range s.u {
		h = 31*h + int32(c)
	}
	return h
}
func (s *str) StartsWith(p String) bool { return s.StartsWithFrom(p, 0) }
func (s *str) StartsWithFrom(p String, at int32) bool {
	n := p.Chars()
	if at < 0 || int(at)+len(n) > len(s.u) {
		return false
	}
	for i, c := range n {
		if s.u[int(at)+i] != c {
			return false
		}
	}
	return true
}
func (s *str) EndsWith(p String) bool { return s.StartsWithFrom(p, int32(len(s.u)-len(p.Chars()))) }
func (s *str) Contains(p any) bool    { return indexUnits(s.u, ToStr(p).Chars(), 0) >= 0 }
func (s *str) ToUpperCase() String    { return S(strings.ToUpper(s.GoString())) }
func (s *str) ToLowerCase() String    { return S(strings.ToLower(s.GoString())) }
func (s *str) Trim() String {
	a, b := 0, len(s.u)
	for a < b && s.u[a] <= ' ' {
		a++
	}
	for b > a && s.u[b-1] <= ' ' {
		b--
	}
	return &str{s.u[a:b:b]}
}
func (s *str) Strip() String {
	a, b := 0, len(s.u)
	for a < b && isWS(s.u[a]) {
		a++
	}
	for b > a && isWS(s.u[b-1]) {
		b--
	}
	return &str{s.u[a:b:b]}
}
func (s *str) Concat(o String) String {
	u := make([]uint16, 0, len(s.u)+int(o.Length()))
	return &str{append(append(u, s.u...), o.Chars()...)}
}
func (s *str) Repeat(n int32) String {
	if n < 0 {
		ThrowMsg("java.lang.IllegalArgumentException", "count is negative: "+strconv.Itoa(int(n)))
	}
	u := make([]uint16, 0, len(s.u)*int(n))
	for i := int32(0); i < n; i++ {
		u = append(u, s.u...)
	}
	return &str{u}
}
func (s *str) ToCharArray() []uint16 { return append([]uint16{}, s.u...) }
func (s *str) ReplaceChar(a, b uint16) String {
	u := append([]uint16{}, s.u...)
	for i, c := range u {
		if c == a {
			u[i] = b
		}
	}
	return &str{u}
}
func (s *str) Replace(a, b any) String {
	x, y := ToStr(a).Chars(), ToStr(b).Chars()
	var out []uint16
	i := 0
	for i <= len(s.u) {
		j := indexUnits(s.u, x, i)
		if j < 0 || (len(x) == 0 && j >= len(s.u)+1) {
			break
		}
		out = append(out, s.u[i:j]...)
		out = append(out, y...)
		if len(x) == 0 {
			if j < len(s.u) {
				out = append(out, s.u[j])
			}
			i = j + 1
		} else {
			i = j + len(x)
		}
	}
	if i <= len(s.u) {
		out = append(out, s.u[i:]...)
	}
	return &str{out}
}

// Split splits at a regular expression; only the literal forms and a few common classes are understood.
func (s *str) Split(re String) []String {
	pat := re.GoString()
	var parts []string
	text := s.GoString()
	if len(pat) == 1 && !strings.ContainsAny(pat, ".$|()[{^?*+\\") || (len(pat) == 2 && pat[0] == '\\' && !isAlnum(pat[1])) {
		sep := pat
		if len(pat) == 2 {
			sep = pat[1:]
		}
		parts = strings.Split(text, sep)
	} else if pat == "\\s+" || pat == "\\s" || pat == " +" || pat == ",\\s*" || pat == "\\s*,\\s*" {
		parts = splitWith(text, pat)
	} else if pat == "" {
		parts = nil
		for _, c := range s.u {
			parts = append(parts, string(utf16.Decode([]uint16{c})))
		}
	} else if !strings.ContainsAny(pat, ".$|()[{^?*+\\") {
		parts = strings.Split(text, pat)
	} else {
		ThrowMsg("java.lang.UnsupportedOperationException", "split on the pattern "+pat)
	}
	if len(parts) > 0 && parts[0] == "" && len(text) > 0 && pat == "" {
		parts = parts[1:]
	}
	for len(parts) > 1 && parts[len(parts)-1] == "" {
		parts = parts[:len(parts)-1]
	}
	if len(parts) == 1 && parts[0] == "" && len(text) > 0 {
		parts = nil
	}
	out := make([]String, len(parts))
	for i, p := range parts {
		out[i] = S(p)
	}
	return out
}
func isAlnum(c byte) bool {
	return c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'
}
func splitWith(text, pat string) []string {
	var parts []string
	start := 0
	i := 0
	for i < len(text) {
		j := i
		switch pat {
		case "\\s+", " +":
			for j < len(text) && (text[j] == ' ' || (pat == "\\s+" && (text[j] == '\t' || text[j] == '\n' || text[j] == '\r'))) {
				j++
			}
		case "\\s":
			if text[j] == ' ' || text[j] == '\t' || text[j] == '\n' || text[j] == '\r' {
				j++
			}
		case ",\\s*", "\\s*,\\s*":
			k := j
			if pat[0] == '\\' {
				for k < len(text) && text[k] == ' ' {
					k++
				}
			}
			if k < len(text) && text[k] == ',' {
				j = k + 1
				for j < len(text) && text[j] == ' ' {
					j++
				}
			}
		}
		if j > i {
			if !(i == 0 && start == 0 && false) {
				parts = append(parts, text[start:i])
			}
			start = j
			i = j
		} else {
			i++
		}
	}
	return append(parts, text[start:])
}

// ---- Objects: string conversion, equality and hash codes ---------------------------------------------------------------------------------

func nullStr() String { return S("null") }

// ToStr is String.valueOf(o).
func ToStr(o any) String {
	switch v := o.(type) {
	case nil:
		return nullStr()
	case String:
		return v
	case int32:
		return S(strconv.FormatInt(int64(v), 10))
	case int64:
		return S(strconv.FormatInt(v, 10))
	case int16:
		return S(strconv.FormatInt(int64(v), 10))
	case int8:
		return S(strconv.FormatInt(int64(v), 10))
	case uint16:
		return &str{[]uint16{v}}
	case bool:
		if v {
			return S("true")
		}
		return S("false")
	case float64:
		return S(DoubleToString(v))
	case float32:
		return S(FloatToString(v))
	case interface{ ToString() String }:
		return v.ToString()
	case interface{ ClassName() string }:
		return S(v.ClassName() + "@" + strconv.FormatInt(int64(identityHash(o)), 16))
	}
	return S(arrayTypeName(o) + "@" + strconv.FormatInt(int64(identityHash(o)), 16))
}

func arrayTypeName(o any) string {
	switch o.(type) {
	case []int32:
		return "[I"
	case []int64:
		return "[J"
	case []float64:
		return "[D"
	case []uint16:
		return "[C"
	case []bool:
		return "[Z"
	case []int8:
		return "[B"
	case []String:
		return "[Ljava.lang.String;"
	}
	return "[Ljava.lang.Object;"
}

var identityCounter int32 = 0x1b6d3586
var identities = map[any]int32{}

func identityHash(o any) int32 {
	defer func() { _ = recover() }()
	if h, ok := identities[o]; ok {
		return h
	}
	identityCounter = identityCounter*1103515245 + 12345
	h := identityCounter & 0x7fffffff
	identities[o] = h
	return h
}

// Cat is string concatenation.
func Cat(parts ...any) String {
	var u []uint16
	for _, p := range parts {
		u = append(u, ToStr(p).Chars()...)
	}
	return &str{u}
}

// Equals is a.equals(b) (a null receiver is a NullPointerException).
func Equals(a, b any) bool {
	switch v := a.(type) {
	case nil:
		panic(npe())
	case interface{ Equals(any) bool }:
		return v.Equals(b)
	case int32, int64, int16, int8, uint16, bool, float64, float32:
		return a == b
	}
	return sameRef(a, b)
}

// ObjEquals is Objects.equals(a, b).
func ObjEquals(a, b any) bool {
	if a == nil || b == nil {
		return a == nil && b == nil
	}
	return Equals(a, b)
}

func sameRef(a, b any) (same bool) {
	defer func() {
		if recover() != nil {
			same = false
		}
	}()
	return a == b
}

// HashCode is o.hashCode(); a null key hashes to 0 where a map asks (HashCodeOrZero).
func HashCode(o any) int32 {
	switch v := o.(type) {
	case nil:
		panic(npe())
	case int32:
		return v
	case int64:
		return int32(v ^ int64(uint64(v)>>32))
	case int16:
		return int32(v)
	case int8:
		return int32(v)
	case uint16:
		return int32(v)
	case bool:
		if v {
			return 1231
		}
		return 1237
	case float64:
		if v != v {
			return 0x7ff80000
		}
		b := math.Float64bits(v)
		return int32(b ^ b>>32)
	case float32:
		if v != v {
			return 0x7fc00000
		}
		return int32(math.Float32bits(v))
	case interface{ HashCode() int32 }:
		return v.HashCode()
	}
	return identityHash(o)
}

func hashOrZero(o any) int32 {
	if o == nil {
		return 0
	}
	return HashCode(o)
}

// Throw makes a runtime error of a library exception class.
func ThrowMsg(class, msg string) { panic(NewThrowable(class, S(msg))) }

// ---- Numbers --------------------------------------------------------------------------------------------------------------------------

func digitsOf(f float64, bits int) (digits string, exp int) {
	s := strconv.FormatFloat(f, 'e', -1, bits)
	e := strings.IndexByte(s, 'e')
	exp, _ = strconv.Atoi(s[e+1:])
	return strings.Replace(s[:e], ".", "", 1), exp
}

func javaFloatText(f float64, bits int) string {
	if f != f {
		return "NaN"
	}
	if math.IsInf(f, 1) {
		return "Infinity"
	}
	if math.IsInf(f, -1) {
		return "-Infinity"
	}
	if f == 0 {
		if math.Signbit(f) {
			return "-0.0"
		}
		return "0.0"
	}
	sign := ""
	if f < 0 {
		sign = "-"
		f = -f
	}
	d, e := digitsOf(f, bits)
	if f >= 1e-3 && f < 1e7 {
		il := e + 1
		switch {
		case il <= 0:
			return sign + "0." + strings.Repeat("0", -il) + d
		case len(d) <= il:
			return sign + d + strings.Repeat("0", il-len(d)) + ".0"
		}
		return sign + d[:il] + "." + d[il:]
	}
	if len(d) == 1 {
		d += "0"
	}
	return sign + d[:1] + "." + d[1:] + "E" + strconv.Itoa(e)
}

// DoubleToString is Double.toString.
func DoubleToString(f float64) string { return javaFloatText(f, 64) }

// FloatToString is Float.toString.
func FloatToString(f float32) string { return javaFloatText(float64(f), 32) }

// D2I is Java's (int) of a double: saturating, NaN to 0.
func D2I(f float64) int32 {
	if f != f {
		return 0
	}
	if f >= 2147483647 {
		return math.MaxInt32
	}
	if f <= -2147483648 {
		return math.MinInt32
	}
	return int32(f)
}

// D2L is Java's (long) of a double.
func D2L(f float64) int64 {
	if f != f {
		return 0
	}
	if f >= 9223372036854775807 {
		return math.MaxInt64
	}
	if f <= -9223372036854775808 {
		return math.MinInt64
	}
	return int64(f)
}

// DivInt and friends are used where the divisor is a literal zero (Go refuses to compile that).
func DivZero() int32 { return int32(0) / zero }

var zero int32

// Shl32 etc: Java masks a shift count.
func Ushr32(x int32, n int32) int32 { return int32(uint32(x) >> (uint32(n) & 31)) }
func Ushr64(x int64, n int32) int64 { return int64(uint64(x) >> (uint32(n) & 63)) }

// FMod is Java's % on doubles.
func FMod(a, b float64) float64 { return math.Mod(a, b) }

// Div0 divides floating point numbers where a literal zero divisor is written.
func FDiv(a, b float64) float64 { return a / b }

func numberFormat(s String, radix int32) {
	if s == nil {
		ThrowMsg("java.lang.NumberFormatException", "Cannot parse null string: null")
	}
	m := "For input string: \"" + s.GoString() + "\""
	if radix != 10 {
		m += " under radix " + strconv.Itoa(int(radix))
	}
	ThrowMsg("java.lang.NumberFormatException", m)
}

func ParseInt(s String, radix int32) int32 {
	if s == nil {
		numberFormat(s, radix)
	}
	t := s.GoString()
	if strings.HasPrefix(t, "+") && len(t) > 1 && t[1] != '-' && t[1] != '+' {
		t = t[1:]
	}
	v, err := strconv.ParseInt(t, int(radix), 32)
	if err != nil || strings.Contains(t, "_") {
		numberFormat(s, radix)
	}
	return int32(v)
}
func ParseLong(s String, radix int32) int64 {
	if s == nil {
		numberFormat(s, radix)
	}
	t := s.GoString()
	if strings.HasPrefix(t, "+") && len(t) > 1 && t[1] != '-' && t[1] != '+' {
		t = t[1:]
	}
	v, err := strconv.ParseInt(t, int(radix), 64)
	if err != nil || strings.Contains(t, "_") {
		numberFormat(s, radix)
	}
	return v
}
func ParseDouble(s String) float64 {
	if s == nil {
		panic(npe())
	}
	t := strings.TrimSpace(s.GoString())
	u := strings.TrimRight(t, "dDfF")
	if strings.HasSuffix(strings.ToLower(t), "d") || strings.HasSuffix(strings.ToLower(t), "f") {
		t = u
	}
	v, err := strconv.ParseFloat(t, 64)
	if err != nil && !strings.Contains(err.Error(), "range") || strings.ContainsAny(t, "_xXpP") && !strings.ContainsAny(t, "Ee") {
		if t == "NaN" {
			return math.NaN()
		}
		if t == "Infinity" || t == "+Infinity" {
			return math.Inf(1)
		}
		if t == "-Infinity" {
			return math.Inf(-1)
		}
		ThrowMsg("java.lang.NumberFormatException", "For input string: \""+s.GoString()+"\"")
	}
	return v
}
func ParseBoolean(s String) bool { return s != nil && strings.EqualFold(s.GoString(), "true") }

func IntToString(v int64, radix int32) String { return S(strconv.FormatInt(v, int(radix))) }
func IntToHex(v int32) String                 { return S(strconv.FormatUint(uint64(uint32(v)), 16)) }
func IntToBinary(v int32) String              { return S(strconv.FormatUint(uint64(uint32(v)), 2)) }
func IntToOctal(v int32) String               { return S(strconv.FormatUint(uint64(uint32(v)), 8)) }
func LongToHex(v int64) String                { return S(strconv.FormatUint(uint64(v), 16)) }
func LongToBinary(v int64) String             { return S(strconv.FormatUint(uint64(v), 2)) }
func Cmp[T int32 | int64 | uint16 | int16 | int8 | float64 | float32](a, b T) int32 {
	if a < b {
		return -1
	}
	if a > b {
		return 1
	}
	return 0
}
func DoubleCompare(a, b float64) int32 {
	if a < b {
		return -1
	}
	if a > b {
		return 1
	}
	x, y := int64(math.Float64bits(a)), int64(math.Float64bits(b))
	if a != a {
		x = 0x7ff8000000000000
	}
	if b != b {
		y = 0x7ff8000000000000
	}
	return Cmp(x, y)
}
func BoolCompare(a, b bool) int32 {
	if a == b {
		return 0
	}
	if a {
		return 1
	}
	return -1
}
func Signum(v int64) int32 { return Cmp(v, 0) }
func BitCount(v uint64) int32 {
	n := int32(0)
	for ; v != 0; v &= v - 1 {
		n++
	}
	return n
}

// ---- Characters ---------------------------------------------------------------------------------------------------------------------------

func CharIsDigit(c uint16) bool      { return unicode.IsDigit(rune(c)) }
func CharIsLetter(c uint16) bool     { return unicode.IsLetter(rune(c)) }
func CharIsLetterOrDigit(c uint16) bool { return unicode.IsLetter(rune(c)) || unicode.IsDigit(rune(c)) }
func CharIsUpper(c uint16) bool      { return unicode.IsUpper(rune(c)) }
func CharIsLower(c uint16) bool      { return unicode.IsLower(rune(c)) }
func CharIsWhitespace(c uint16) bool { return isWS(c) }
func CharToUpper(c uint16) uint16    { return uint16(unicode.ToUpper(rune(c))) }
func CharToLower(c uint16) uint16    { return uint16(unicode.ToLower(rune(c))) }
func CharNumeric(c uint16) int32 {
	switch {
	case c >= '0' && c <= '9':
		return int32(c - '0')
	case c >= 'a' && c <= 'z':
		return int32(c-'a') + 10
	case c >= 'A' && c <= 'Z':
		return int32(c-'A') + 10
	}
	return -1
}

// ---- Math -------------------------------------------------------------------------------------------------------------------------------------

func MaxD(a, b float64) float64 {
	if a != a || b != b {
		return math.NaN()
	}
	if a == 0 && b == 0 {
		if math.Signbit(a) {
			return b
		}
		return a
	}
	if a > b {
		return a
	}
	return b
}
func MinD(a, b float64) float64 {
	if a != a || b != b {
		return math.NaN()
	}
	if a == 0 && b == 0 {
		if math.Signbit(a) {
			return a
		}
		return b
	}
	if a < b {
		return a
	}
	return b
}
func Max[T int32 | int64](a, b T) T {
	if a > b {
		return a
	}
	return b
}
func Min[T int32 | int64](a, b T) T {
	if a < b {
		return a
	}
	return b
}
func Abs[T int32 | int64](a T) T {
	if a < 0 {
		return -a
	}
	return a
}
func Round(f float64) int64 {
	if f != f {
		return 0
	}
	return D2L(math.Floor(f + 0.5))
}
func RoundF(f float32) int32 { return D2I(math.Floor(float64(f) + 0.5)) }
func Rint(f float64) float64 { return math.RoundToEven(f) }
func SignumD(f float64) float64 {
	if f > 0 {
		return 1
	}
	if f < 0 {
		return -1
	}
	return f
}
func FloorDiv[T int32 | int64](a, b T) T {
	q := a / b
	if (a%b != 0) && ((a < 0) != (b < 0)) {
		q--
	}
	return q
}
func FloorMod[T int32 | int64](a, b T) T {
	m := a % b
	if m != 0 && ((m < 0) != (b < 0)) {
		m += b
	}
	return m
}
func AddExact32(a, b int32) int32 {
	r := int64(a) + int64(b)
	if r != int64(int32(r)) {
		ThrowMsg("java.lang.ArithmeticException", "integer overflow")
	}
	return int32(r)
}
func MulExact32(a, b int32) int32 {
	r := int64(a) * int64(b)
	if r != int64(int32(r)) {
		ThrowMsg("java.lang.ArithmeticException", "integer overflow")
	}
	return int32(r)
}
func AddExact64(a, b int64) int64 {
	r := a + b
	if (a^r)&(b^r) < 0 {
		ThrowMsg("java.lang.ArithmeticException", "long overflow")
	}
	return r
}
func MulExact64(a, b int64) int64 {
	if a != 0 && (a*b/a != b || (a == -1 && b == math.MinInt64)) {
		ThrowMsg("java.lang.ArithmeticException", "long overflow")
	}
	return a * b
}

// ---- Output ---------------------------------------------------------------------------------------------------------------------------------

// PrintStream is System.out or System.err.
type PrintStream struct{ w *bufio.Writer }

var Out = &PrintStream{bufio.NewWriterSize(os.Stdout, 1<<16)}
var Err = &PrintStream{bufio.NewWriterSize(os.Stderr, 1<<12)}

func (p *PrintStream) write(s String) {
	for _, c := range utf16.Decode(s.Chars()) {
		p.w.WriteRune(c)
	}
}
func (p *PrintStream) Print(o any)   { p.write(ToStr(o)) }
func (p *PrintStream) Println(o ...any) {
	if len(o) > 0 {
		p.write(ToStr(o[0]))
	}
	p.w.WriteByte('\n')
	if p == Err {
		p.w.Flush()
	}
}
func (p *PrintStream) PrintChars(c []uint16)   { p.write(&str{c}) }
func (p *PrintStream) PrintlnChars(c []uint16) { p.write(&str{c}); p.w.WriteByte('\n') }
func (p *PrintStream) Printf(f String, args ...any) *PrintStream {
	p.write(Format(f, args))
	return p
}
func (p *PrintStream) Flush() { p.w.Flush() }

// Exit is System.exit.
func Exit(code int32) {
	Out.Flush()
	Err.Flush()
	os.Exit(int(code))
}

var startNanos = nowNanos()

func CurrentTimeMillis() int64 { return nowNanos() / 1e6 }
func NanoTime() int64           { return nowNanos() - startNanos + 1 }
func LineSeparator() String     { return S("\n") }

// ---- StringBuilder ------------------------------------------------------------------------------------------------------------------

type StringBuilder struct{ u []uint16 }

func NewStringBuilder() *StringBuilder          { return &StringBuilder{} }
func NewStringBuilderOf(s String) *StringBuilder { return &StringBuilder{append([]uint16{}, s.Chars()...)} }
func (b *StringBuilder) Append(o any) *StringBuilder {
	b.u = append(b.u, ToStr(o).Chars()...)
	return b
}
func (b *StringBuilder) AppendChars(c []uint16) *StringBuilder { b.u = append(b.u, c...); return b }
func (b *StringBuilder) AppendRange(s String, from, to int32) *StringBuilder {
	b.u = append(b.u, s.Chars()[from:to]...)
	return b
}
func (b *StringBuilder) ToString() String { return &str{append([]uint16{}, b.u...)} }
func (b *StringBuilder) Length() int32    { return int32(len(b.u)) }
func (b *StringBuilder) IsEmpty() bool    { return len(b.u) == 0 }
func (b *StringBuilder) CharAt(i int32) uint16 {
	if i < 0 || int(i) >= len(b.u) {
		ThrowMsg("java.lang.StringIndexOutOfBoundsException", "index "+strconv.Itoa(int(i))+",length "+strconv.Itoa(len(b.u)))
	}
	return b.u[i]
}
func (b *StringBuilder) SetCharAt(i int32, c uint16) {
	if i < 0 || int(i) >= len(b.u) {
		ThrowMsg("java.lang.StringIndexOutOfBoundsException", "index "+strconv.Itoa(int(i))+",length "+strconv.Itoa(len(b.u)))
	}
	b.u[i] = c
}
func (b *StringBuilder) DeleteCharAt(i int32) *StringBuilder {
	if i < 0 || int(i) >= len(b.u) {
		ThrowMsg("java.lang.StringIndexOutOfBoundsException", "index "+strconv.Itoa(int(i))+",length "+strconv.Itoa(len(b.u)))
	}
	b.u = append(b.u[:i], b.u[i+1:]...)
	return b
}
func (b *StringBuilder) Delete(from, to int32) *StringBuilder {
	if to > int32(len(b.u)) {
		to = int32(len(b.u))
	}
	if from < 0 || from > to {
		ThrowMsg("java.lang.StringIndexOutOfBoundsException", "start "+strconv.Itoa(int(from))+", end "+strconv.Itoa(int(to))+", length "+strconv.Itoa(len(b.u)))
	}
	b.u = append(b.u[:from], b.u[to:]...)
	return b
}
func (b *StringBuilder) Reverse() *StringBuilder {
	r := utf16.Decode(b.u)
	for i, j := 0, len(r)-1; i < j; i, j = i+1, j-1 {
		r[i], r[j] = r[j], r[i]
	}
	b.u = utf16.Encode(r)
	return b
}
func (b *StringBuilder) SetLength(n int32) {
	for int32(len(b.u)) < n {
		b.u = append(b.u, 0)
	}
	b.u = b.u[:n]
}
func (b *StringBuilder) IndexOf(s String) int32 { return int32(indexUnits(b.u, s.Chars(), 0)) }
func (b *StringBuilder) Insert(at int32, o any) *StringBuilder {
	if at < 0 || int(at) > len(b.u) {
		ThrowMsg("java.lang.StringIndexOutOfBoundsException", "offset "+strconv.Itoa(int(at))+", length "+strconv.Itoa(len(b.u)))
	}
	ins := ToStr(o).Chars()
	b.u = append(b.u[:at], append(append([]uint16{}, ins...), b.u[at:]...)...)
	return b
}
func (b *StringBuilder) Substring(from int32) String { return b.ToString().Substring(from) }
func (b *StringBuilder) SubstringTo(from, to int32) String {
	return b.ToString().SubstringTo(from, to)
}

// ---- Throwables -----------------------------------------------------------------------------------------------------------------------

// Throwable is any Java exception or error.
type Throwable interface {
	GetMessage() String
	GetLocalizedMessage() String
	GetCause() Throwable
	ToString() String
	PrintStackTrace()
	ClassName() string
	LibBase() string
	throwable() *ThrowableBase
}

// ThrowableBase is what every exception holds; a program's exception class embeds it.
type ThrowableBase struct {
	Msg   String
	Cause Throwable
	Self  Throwable
	class string
}

func (t *ThrowableBase) throwable() *ThrowableBase { return t }

// Init is the constructor of the library class `class` (the nearest library class a program's exception extends).
func (t *ThrowableBase) Init(self Throwable, class string, msg String, cause Throwable) {
	t.Self, t.class, t.Msg, t.Cause = self, class, msg, cause
}
func (t *ThrowableBase) InitCause(self Throwable, class string, cause Throwable) {
	t.Self, t.class, t.Cause = self, class, cause
	if cause != nil {
		t.Msg = cause.ToString()
	}
}
func (t *ThrowableBase) GetMessage() String          { return t.Msg }
func (t *ThrowableBase) GetLocalizedMessage() String { return t.Self.GetMessage() }
func (t *ThrowableBase) GetCause() Throwable         { return t.Cause }
func (t *ThrowableBase) LibBase() string             { return t.class }
func (t *ThrowableBase) ClassName() string           { return t.class }
func (t *ThrowableBase) ToString() String {
	m := t.Self.GetLocalizedMessage()
	if m == nil {
		return S(t.Self.ClassName())
	}
	return Cat(S(t.Self.ClassName()), S(": "), m)
}
func (t *ThrowableBase) PrintStackTrace() {
	Err.Println(t.Self.ToString())
	Err.Println(S("\tat " + "main"))
}

// NewThrowable makes a library exception of the class.
func NewThrowable(class string, msg String) Throwable {
	t := &ThrowableBase{}
	t.Init(t, class, msg, nil)
	return t
}

// NewThrowableCause is new E(cause).
func NewThrowableCause(class string, cause Throwable) Throwable {
	t := &ThrowableBase{}
	t.InitCause(t, class, cause)
	return t
}

func npe() Throwable { return NewThrowable("java.lang.NullPointerException", nil) }

var libSuper = map[string]string{
	"java.lang.Exception": "java.lang.Throwable", "java.lang.Error": "java.lang.Throwable", "java.lang.RuntimeException": "java.lang.Exception",
	"java.lang.IllegalArgumentException": "java.lang.RuntimeException", "java.lang.IllegalStateException": "java.lang.RuntimeException",
	"java.lang.NumberFormatException": "java.lang.IllegalArgumentException", "java.lang.ArithmeticException": "java.lang.RuntimeException",
	"java.lang.NullPointerException": "java.lang.RuntimeException", "java.lang.ClassCastException": "java.lang.RuntimeException",
	"java.lang.IndexOutOfBoundsException": "java.lang.RuntimeException", "java.lang.ArrayIndexOutOfBoundsException": "java.lang.IndexOutOfBoundsException",
	"java.lang.StringIndexOutOfBoundsException": "java.lang.IndexOutOfBoundsException", "java.lang.NegativeArraySizeException": "java.lang.RuntimeException",
	"java.lang.ArrayStoreException": "java.lang.RuntimeException", "java.lang.UnsupportedOperationException": "java.lang.RuntimeException",
	"java.lang.SecurityException": "java.lang.RuntimeException", "java.util.NoSuchElementException": "java.lang.RuntimeException",
	"java.util.InputMismatchException": "java.util.NoSuchElementException", "java.util.ConcurrentModificationException": "java.lang.RuntimeException",
	"java.util.EmptyStackException": "java.lang.RuntimeException", "java.lang.CloneNotSupportedException": "java.lang.Exception",
	"java.lang.InterruptedException": "java.lang.Exception", "java.lang.ReflectiveOperationException": "java.lang.Exception",
	"java.lang.ClassNotFoundException": "java.lang.ReflectiveOperationException", "java.io.IOException": "java.lang.Exception",
	"java.io.FileNotFoundException": "java.io.IOException", "java.io.UncheckedIOException": "java.lang.RuntimeException",
	"java.lang.VirtualMachineError": "java.lang.Error", "java.lang.StackOverflowError": "java.lang.VirtualMachineError",
	"java.lang.OutOfMemoryError": "java.lang.VirtualMachineError", "java.lang.AssertionError": "java.lang.Error",
	"java.util.concurrent.TimeoutException": "java.lang.Exception", "java.lang.LinkageError": "java.lang.Error",
	"java.lang.ExceptionInInitializerError": "java.lang.LinkageError", "java.util.MissingResourceException": "java.lang.RuntimeException",
	"java.time.DateTimeException": "java.lang.RuntimeException", "java.lang.EnumConstantNotPresentException": "java.lang.RuntimeException",
}

// InstanceOf is `ex instanceof C` for a library class C.
func InstanceOf(o any, class string) bool {
	if o == nil {
		return false
	}
	if t, ok := o.(Throwable); ok {
		for c := t.LibBase(); c != ""; c = libSuper[c] {
			if c == class {
				return true
			}
		}
		return class == "java.lang.Object"
	}
	switch class {
	case "java.lang.Object":
		return true
	case "java.lang.String", "java.lang.CharSequence":
		_, ok := o.(String)
		return ok
	case "java.lang.Integer":
		_, ok := o.(int32)
		return ok
	case "java.lang.Long":
		_, ok := o.(int64)
		return ok
	case "java.lang.Double":
		_, ok := o.(float64)
		return ok
	case "java.lang.Float":
		_, ok := o.(float32)
		return ok
	case "java.lang.Short":
		_, ok := o.(int16)
		return ok
	case "java.lang.Byte":
		_, ok := o.(int8)
		return ok
	case "java.lang.Character":
		_, ok := o.(uint16)
		return ok
	case "java.lang.Boolean":
		_, ok := o.(bool)
		return ok
	case "java.lang.Number":
		switch o.(type) {
		case int32, int64, float64, float32, int16, int8:
			return true
		}
	case "java.lang.StringBuilder":
		_, ok := o.(*StringBuilder)
		return ok
	case "java.util.List", "java.util.ArrayList":
		_, ok := o.(List)
		return ok
	case "java.util.Collection":
		_, ok := o.(Collection)
		return ok
	case "java.util.Set", "java.util.HashSet":
		_, ok := o.(Set)
		return ok
	case "java.util.Map", "java.util.HashMap":
		_, ok := o.(Map)
		return ok
	}
	return false
}

// Is reports whether o is a T (a program class, as a Go interface).
func Is[T any](o any) bool {
	_, ok := o.(T)
	return ok
}

// ClassNameOf is getClass().getName() of anything.
func ClassNameOf(o any) string {
	switch v := o.(type) {
	case String:
		return "java.lang.String"
	case int32:
		return "java.lang.Integer"
	case int64:
		return "java.lang.Long"
	case float64:
		return "java.lang.Double"
	case float32:
		return "java.lang.Float"
	case int16:
		return "java.lang.Short"
	case int8:
		return "java.lang.Byte"
	case uint16:
		return "java.lang.Character"
	case bool:
		return "java.lang.Boolean"
	case *StringBuilder:
		return "java.lang.StringBuilder"
	case *ArrayList:
		return "java.util.ArrayList"
	case *HashMap:
		if v.linked {
			return "java.util.LinkedHashMap"
		}
		return "java.util.HashMap"
	case *HashSet:
		return "java.util.HashSet"
	case interface{ ClassName() string }:
		return v.ClassName()
	}
	return "java.lang.Object"
}

// Cast is the checked cast `(T) o`.
func Cast[T any](o any, name string) T {
	var z T
	if o == nil {
		return z
	}
	if t, ok := o.(T); ok {
		return t
	}
	cn := ClassNameOf(o)
	ThrowMsg("java.lang.ClassCastException", "class "+cn+" cannot be cast to class "+name+" ("+cn+" and "+name+" are in unnamed module of loader 'app')")
	return z
}

// As is the cast javac adds where an erased value is used as its real type.
func As[T any](o any) T {
	var z T
	if o == nil {
		return z
	}
	return o.(T)
}

// AsString is (String) o.
func AsString(o any) String {
	if o == nil {
		return nil
	}
	if s, ok := o.(String); ok {
		return s
	}
	ThrowMsg("java.lang.ClassCastException", "class "+ClassNameOf(o)+" cannot be cast to class java.lang.String (java.lang.String is in module java.base of loader 'bootstrap'; "+ClassNameOf(o)+" is in unnamed module of loader 'app')")
	return nil
}

// ToThrowable turns what a panic carries into the Java exception it stands for.
func ToThrowable(r any) Throwable {
	switch v := r.(type) {
	case Throwable:
		return v
	case runtime.Error:
		m := v.Error()
		switch {
		case strings.Contains(m, "divide by zero"):
			return NewThrowable("java.lang.ArithmeticException", S("/ by zero"))
		case strings.Contains(m, "index out of range"):
			return NewThrowable("java.lang.ArrayIndexOutOfBoundsException", S(indexMessage(m)))
		case strings.Contains(m, "nil pointer") || strings.Contains(m, "interface is nil") || strings.Contains(m, "nil map"):
			return npe()
		case strings.Contains(m, "interface conversion"):
			return NewThrowable("java.lang.ClassCastException", S(m))
		case strings.Contains(m, "makeslice"):
			return NewThrowable("java.lang.NegativeArraySizeException", S("-1"))
		case strings.Contains(m, "stack overflow"):
			return NewThrowable("java.lang.StackOverflowError", nil)
		}
		return NewThrowable("java.lang.Error", S(m))
	}
	return NewThrowable("java.lang.Error", S("panic"))
}

// "index out of range [5] with length 2" is Java's "Index 5 out of bounds for length 2".
func indexMessage(m string) string {
	i := strings.Index(m, "[")
	j := strings.Index(m, "]")
	k := strings.LastIndex(m, "length ")
	if i < 0 || j < i {
		return m
	}
	if k < 0 {
		return "Index " + m[i+1:j] + " out of bounds for length 0"
	}
	return "Index " + m[i+1:j] + " out of bounds for length " + m[k+7:]
}

// Try runs body; a Java exception it throws goes to handler (which says whether a catch clause took it), and finally runs last whatever happened.
func Try(body func(), handler func(ex Throwable) bool, finally func()) {
	if finally != nil {
		defer finally()
	}
	if handler == nil {
		body()
		return
	}
	func() {
		defer func() {
			if r := recover(); r != nil {
				ex := ToThrowable(r)
				if !handler(ex) {
					panic(ex)
				}
			}
		}()
		body()
	}()
}

// Run runs a program's main: an exception nothing caught ends it with exit code 1, as the JVM does.
func Run(main func()) {
	defer func() {
		if r := recover(); r != nil {
			Out.Flush()
			ex := ToThrowable(r)
			Err.w.WriteString("Exception in thread \"main\" " + ex.ToString().GoString() + "\n\tat main\n")
			Err.Flush()
			os.Exit(1)
		}
		Out.Flush()
		Err.Flush()
	}()
	main()
}

// SameArr is == on two arrays.
func SameArr[T any](a, b []T) bool {
	if a == nil || b == nil {
		return a == nil && b == nil
	}
	return len(a) == len(b) && (len(a) == 0 || &a[0] == &b[0])
}

// Utf8Len keeps the utf8 import used by programs with wide text.
func Utf8Len(s string) int { return utf8.RuneCountInString(s) }
