package jrt

// String.format and printf: the conversions %d %s %S %c %b %n %x %X %o %e %E %f %%, with the flags - 0 + space , ( and a width and precision.

import (
	"math"
	"strconv"
	"strings"
	"unicode/utf16"
)

func badFormat(class, msg string) { ThrowMsg(class, msg) }

// Format is String.format(f, args).
func Format(f String, args []any) String {
	if f == nil {
		panic(npe())
	}
	in := f.GoString()
	var out strings.Builder
	next := 0
	for i := 0; i < len(in); {
		c := in[i]
		if c != '%' {
			out.WriteByte(c)
			i++
			continue
		}
		i++
		start := i
		var left, zero, plus, space, comma, paren, alt bool
		for ; i < len(in); i++ {
			switch in[i] {
			case '-':
				left = true
			case '0':
				zero = true
			case '+':
				plus = true
			case ' ':
				space = true
			case ',':
				comma = true
			case '(':
				paren = true
			case '#':
				alt = true
			default:
				goto flagsDone
			}
		}
	flagsDone:
		width, prec := -1, -1
		w0 := i
		for i < len(in) && in[i] >= '0' && in[i] <= '9' {
			i++
		}
		if i > w0 {
			width, _ = strconv.Atoi(in[w0:i])
		}
		if i < len(in) && in[i] == '.' {
			i++
			p0 := i
			for i < len(in) && in[i] >= '0' && in[i] <= '9' {
				i++
			}
			prec, _ = strconv.Atoi(in[p0:i])
		}
		if i >= len(in) {
			badFormat("java.util.UnknownFormatConversionException", "Conversion = '%'")
		}
		conv := in[i]
		i++
		_ = start
		if conv == 'n' {
			out.WriteByte('\n')
			continue
		}
		if conv == '%' {
			out.WriteString(pad("%", width, left, false))
			continue
		}
		if next >= len(args) {
			badFormat("java.util.MissingFormatArgumentException", "Format specifier '%"+in[start:i]+"'")
		}
		arg := args[next]
		next++
		var body string
		switch conv {
		case 'd':
			var v int64
			switch x := arg.(type) {
			case nil:
				body = "null"
			case int32:
				v = int64(x)
			case int64:
				v = x
			case int16:
				v = int64(x)
			case int8:
				v = int64(x)
			default:
				badFormat("java.util.IllegalFormatConversionException", "d != "+ClassNameOf(arg))
			}
			if arg != nil {
				body = signed(strconv.FormatInt(absInt(v), 10), v < 0, plus, space, paren, comma, zero, width, left)
			}
		case 'x', 'X', 'o':
			base := 16
			if conv == 'o' {
				base = 8
			}
			var u uint64
			switch x := arg.(type) {
			case int32:
				u = uint64(uint32(x))
			case int64:
				u = uint64(x)
			case int16:
				u = uint64(uint16(x))
			case int8:
				u = uint64(uint8(x))
			default:
				badFormat("java.util.IllegalFormatConversionException", string(conv)+" != "+ClassNameOf(arg))
			}
			body = strconv.FormatUint(u, base)
			if alt {
				body = map[int]string{16: "0x", 8: "0"}[base] + body
			}
			if conv == 'X' {
				body = strings.ToUpper(body)
			}
			if zero && width > len(body) {
				body = strings.Repeat("0", width-len(body)) + body
			}
		case 's', 'S':
			body = ToStr(arg).GoString()
			if prec >= 0 && prec < len(utf16.Encode([]rune(body))) {
				body = string(utf16.Decode(utf16.Encode([]rune(body))[:prec]))
			}
			if conv == 'S' {
				body = strings.ToUpper(body)
			}
		case 'c':
			switch x := arg.(type) {
			case nil:
				body = "null"
			case uint16:
				body = string(utf16.Decode([]uint16{x}))
			case int32:
				body = string(rune(x))
			default:
				badFormat("java.util.IllegalFormatConversionException", "c != "+ClassNameOf(arg))
			}
		case 'b', 'B':
			if arg == nil {
				body = "false"
			} else if b, ok := arg.(bool); ok {
				body = strconv.FormatBool(b)
			} else {
				body = "true"
			}
			if conv == 'B' {
				body = strings.ToUpper(body)
			}
		case 'f', 'e', 'E', 'g':
			var v float64
			switch x := arg.(type) {
			case nil:
				body = "null"
			case float64:
				v = x
			case float32:
				v = float64(x)
			default:
				badFormat("java.util.IllegalFormatConversionException", string(conv)+" != "+ClassNameOf(arg))
			}
			if arg != nil {
				if prec < 0 {
					prec = 6
				}
				neg := v < 0 || (v == 0 && math.Signbit(v))
				var digits string
				switch {
				case v != v:
					digits, neg = "NaN", false
				case math.IsInf(v, 0):
					digits = "Infinity"
				case conv == 'f':
					digits = fixedText(math.Abs(v), prec, comma)
				case conv == 'g':
					digits = fixedText(math.Abs(v), prec, comma)
				default:
					digits = sciText(math.Abs(v), prec)
					if conv == 'E' {
						digits = strings.ToUpper(digits)
					}
				}
				body = signed(digits, neg, plus, space, paren, false, zero && v == v && !math.IsInf(v, 0), width, left)
			}
		default:
			badFormat("java.util.UnknownFormatConversionException", "Conversion = '"+string(conv)+"'")
		}
		out.WriteString(pad(body, width, left, false))
	}
	return S(out.String())
}

func absInt(v int64) int64 {
	if v < 0 {
		return -v // (math.MinInt64 stays negative; its digits are written from the unsigned value below)
	}
	return v
}

func pad(s string, width int, left, zero bool) string {
	n := len(utf16.Encode([]rune(s)))
	if width <= n {
		return s
	}
	if left {
		return s + strings.Repeat(" ", width-n)
	}
	return strings.Repeat(" ", width-n) + s
}

func groupDigits(d string) string {
	if len(d) <= 3 {
		return d
	}
	var out []byte
	for i := range d {
		if i > 0 && (len(d)-i)%3 == 0 {
			out = append(out, ',')
		}
		out = append(out, d[i])
	}
	return string(out)
}

// signed puts the sign flags around the digits of a number; zero padding goes between the sign and the digits.
func signed(digits string, neg, plus, space, paren, comma, zero bool, width int, left bool) string {
	if strings.HasPrefix(digits, "-") { // MinInt64
		digits = digits[1:]
	}
	if comma {
		if i := strings.IndexByte(digits, '.'); i >= 0 {
			digits = groupDigits(digits[:i]) + digits[i:]
		} else {
			digits = groupDigits(digits)
		}
	}
	prefix, suffix := "", ""
	switch {
	case neg && paren:
		prefix, suffix = "(", ")"
	case neg:
		prefix = "-"
	case plus:
		prefix = "+"
	case space:
		prefix = " "
	}
	if zero && !left && width > len(prefix)+len(digits)+len(suffix) {
		digits = strings.Repeat("0", width-len(prefix)-len(digits)-len(suffix)) + digits
	}
	return prefix + digits + suffix
}

// roundHalfUp keeps `keep` leading digits of d, rounding the dropped part half up as Java's Formatter does on the shortest digits.
func roundHalfUp(d string, keep int) (string, bool) {
	if keep >= len(d) {
		return d + strings.Repeat("0", keep-len(d)), false
	}
	up := keep >= 0 && d[keep] >= '5'
	if keep < 0 {
		return "", false
	}
	b := []byte(d[:keep])
	if up {
		i := keep - 1
		for ; i >= 0; i-- {
			if b[i] == '9' {
				b[i] = '0'
			} else {
				b[i]++
				break
			}
		}
		if i < 0 {
			return "1" + string(b), true
		}
	}
	return string(b), false
}

// fixedText is %.{prec}f of a non-negative number.
func fixedText(v float64, prec int, comma bool) string {
	if v == 0 {
		if prec == 0 {
			return "0"
		}
		return "0." + strings.Repeat("0", prec)
	}
	d, e := digitsOf(v, 64)
	il := e + 1 // digits before the point
	keep := il + prec
	var kept string
	carry := false
	if keep < 0 {
		kept = ""
	} else {
		kept, carry = roundHalfUp(d, keep)
	}
	if carry {
		il++
	}
	var ip, fp string
	switch {
	case il <= 0:
		ip = "0"
		fp = strings.Repeat("0", -il) + kept
		if len(fp) > prec {
			fp = fp[:prec]
		}
		for len(fp) < prec {
			fp += "0"
		}
	default:
		for len(kept) < il+prec {
			kept += "0"
		}
		ip, fp = kept[:il], kept[il:]
	}
	if prec == 0 {
		return ip
	}
	return ip + "." + fp
}

// sciText is %.{prec}e of a non-negative number.
func sciText(v float64, prec int) string {
	if v == 0 {
		return "0." + strings.Repeat("0", prec) + "e+00"
	}
	d, e := digitsOf(v, 64)
	kept, carry := roundHalfUp(d, prec+1)
	if carry {
		kept = kept[:prec+1]
		e++
	}
	m := kept[:1]
	if prec > 0 {
		m += "." + kept[1:]
	}
	sign := "+"
	if e < 0 {
		sign, e = "-", -e
	}
	es := strconv.Itoa(e)
	if len(es) < 2 {
		es = "0" + es
	}
	return m + "e" + sign + es
}
