// xboxkrnl printf family: sprintf, _snprintf, _vsnprintf, swprintf, _snwprintf, vswprintf, _vscwprintf.
//
// Formatting follows the Microsoft CRT the XDK ships (Source/crt/output.c), which these kernel exports share:
// flags - + space # 0, width and precision (with *), size prefixes h l ll w I I32 I64, conversions c C d i o u x X
// e E f g G p s S Z. Notable Microsoft rules: %p is 8 uppercase hex digits; '#' adds no 0x prefix to zero; a
// precision on an integer cancels the '0' flag; the '0' flag also pads strings; exponents have at least 3 digits;
// a NULL string prints "(null)". %S/%C (and h/l/w) switch between narrow and wide arguments.
// Floating point follows the CRT's _fltout2/_fptostr: a 17-significant-digit decimal string (zeros after it), then
// rounding half-up on that string at the requested digit, so %.0f of 0.5 prints "1" and %.1f of 0.25 "0.3".
//
// Not implemented because their behaviour in the kernel is unknown: %n, %a, infinities and NaNs, and wide characters
// above 0xFF in narrow output (the CRT's C-locale wctomb fails there). They stop with a message.

#include "kernel/kernel.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace
{
    // Variadic arguments in guest memory. Each occupies an 8-byte slot; values narrower than 8 bytes are
    // right-justified (big-endian low word), exactly as the XDK's va_arg reads them.
    class GuestArgs
    {
    public:
        // Arguments passed directly: argument n is in r(3+n) for n < 8, else in the caller's slot at r1+0x10+8n.
        static GuestArgs FromRegisters(const PPCContext& ctx, uint32_t first)
        {
            GuestArgs args;
            args.ctx = &ctx;
            args.index = first;
            return args;
        }

        // A guest va_list: a pointer to the next slot.
        static GuestArgs FromVaList(uint32_t pointer)
        {
            GuestArgs args;
            args.pointer = pointer;
            return args;
        }

        // Host values standing for the slots (native code formatting the way a guest caller would).
        static GuestArgs FromValues(std::initializer_list<uint64_t> values)
        {
            GuestArgs args;
            args.values = values;
            return args;
        }

        uint64_t NextSlot()
        {
            if (!values.empty())
            {
                if (index >= values.size())
                    Fatal("printf: the format reads more than the {} arguments passed", values.size());
                return values[index++];
            }
            if (ctx)
            {
                const uint32_t n = index++;
                if (n < 8)
                {
                    const PPCRegister* registers[] = { &ctx->r3, &ctx->r4, &ctx->r5, &ctx->r6,
                                                       &ctx->r7, &ctx->r8, &ctx->r9, &ctx->r10 };
                    return registers[n]->u64;
                }
                return guest::g_memory.Read64(ctx->r1.u32 + 0x10 + n * 8);
            }
            pointer = (pointer + 7) & ~7u;
            const uint64_t value = guest::g_memory.Read64(pointer);
            pointer += 8;
            return value;
        }

        int32_t NextInt() { return int32_t(uint32_t(NextSlot())); }
        uint32_t NextPointer() { return uint32_t(NextSlot()); }

        double NextDouble()
        {
            const uint64_t bits = NextSlot();
            double value;
            std::memcpy(&value, &bits, sizeof(value));
            return value;
        }

    private:
        const PPCContext* ctx = nullptr;
        std::vector<uint64_t> values;
        uint32_t index = 0;
        uint32_t pointer = 0;
    };

    enum : uint32_t
    {
        kLeft = 1 << 0,       // '-'
        kSign = 1 << 1,       // '+'
        kSignSpace = 1 << 2,  // ' '
        kAlternate = 1 << 3,  // '#'
        kLeadZero = 1 << 4,   // '0'
        kShort = 1 << 5,      // h
        kLong = 1 << 6,       // l
        kLongLong = 1 << 7,   // ll
        kI64 = 1 << 8,        // I64
        kWideChar = 1 << 9,   // w
        kSigned = 1 << 10,
        kNegative = 1 << 11,
        kForceOctal = 1 << 12,
    };

    // A value as the CRT's _fltout2 describes it: |value| = 0.<digits> x 10^decpt, with 17 significant digits
    // (zero is "0" with decpt 0). Digits past the 17th read as '0'.
    struct Decimal
    {
        std::string digits;
        int decpt = 0;

        char Digit(size_t i) const { return i < digits.size() ? digits[i] : '0'; }
    };

    Decimal ToDecimal(double value)
    {
        Decimal d;
        if (value == 0)
        {
            d.digits = "0";
            return d;
        }
        char buffer[64];
        std::snprintf(buffer, sizeof(buffer), "%.16e", std::fabs(value));
        const char* p = buffer;
        for (; *p && *p != 'e'; p++)
            if (*p != '.')
                d.digits += *p;
        d.decpt = std::atoi(p + 1) + 1;
        return d;
    }

    // _fptostr: the first `count` digits, rounded half-up on the next digit; a carry out of the first digit adds a
    // leading '1' and increments decpt. No digits (and no rounding) for a negative count.
    std::string RoundDigits(Decimal& d, int count)
    {
        std::string out = "0";
        for (int i = 0; i < count; i++)
            out += d.Digit(size_t(i));
        if (count >= 0 && d.Digit(size_t(count)) >= '5')
        {
            size_t i = out.size() - 1;
            while (out[i] == '9')
                out[i--] = '0';
            out[i]++;
        }
        if (out[0] == '1')
            d.decpt++;
        else
            out.erase(0, 1);
        return out;
    }

    // _cftoe: d.ddd e+xxx (at least three exponent digits).
    std::string FormatE(double value, int precision, bool alternate, bool upper)
    {
        Decimal d = ToDecimal(value);
        const std::string digits = RoundDigits(d, precision + 1);
        const int exponent = value == 0 ? 0 : d.decpt - 1;
        std::string text(1, digits[0]);
        if (precision > 0 || alternate)
            text += '.';
        text.append(digits, 1, size_t(precision));
        text += upper ? 'E' : 'e';
        text += exponent < 0 ? '-' : '+';
        std::string magnitude = std::to_string(exponent < 0 ? -exponent : exponent);
        if (magnitude.size() < 3)
            magnitude.insert(0, 3 - magnitude.size(), '0');
        return text + magnitude;
    }

    // _cftof: `digits` are the rounded digits from 10^(decpt-1) down to 10^-precision.
    std::string FixedForm(const std::string& digits, int decpt, int precision, bool alternate)
    {
        std::string text;
        if (decpt > 0)
            text.assign(digits, 0, size_t(decpt));
        else
            text = "0";
        if (precision > 0 || alternate)
            text += '.';
        std::string fraction;
        if (decpt < 0)
            fraction.assign(size_t(-decpt), '0');
        if (digits.size() > size_t(decpt > 0 ? decpt : 0))
            fraction += digits.substr(size_t(decpt > 0 ? decpt : 0));
        fraction.resize(size_t(precision), '0');
        return text + fraction;
    }

    std::string FormatF(double value, int precision, bool alternate)
    {
        Decimal d = ToDecimal(value);
        const std::string digits = RoundDigits(d, precision + d.decpt);
        return FixedForm(digits, d.decpt, precision, alternate);
    }

    // _cropzeros: removes trailing zeros of the fraction (and a bare '.'), keeping any exponent.
    std::string CropZeros(std::string text)
    {
        const size_t dot = text.find('.');
        if (dot == std::string::npos)
            return text;
        size_t end = text.find_first_of("eE", dot);
        const std::string exponent = end == std::string::npos ? std::string() : text.substr(end);
        if (end == std::string::npos)
            end = text.size();
        size_t last = end;
        while (last > dot + 1 && text[last - 1] == '0')
            last--;
        if (last == dot + 1)
            last = dot;
        return text.substr(0, last) + exponent;
    }

    // _cftog: round to `precision` significant digits first; the exponent after rounding picks the form.
    std::string FormatG(double value, int precision, bool alternate, bool upper)
    {
        Decimal d = ToDecimal(value);
        Decimal rounded = d;
        RoundDigits(rounded, precision);
        const int magnitude = value == 0 ? 0 : rounded.decpt - 1;
        std::string text = (magnitude < -4 || magnitude >= precision)
            ? FormatE(value, precision - 1, alternate, upper)
            : FormatF(value, precision - rounded.decpt, alternate);
        return alternate ? text : CropZeros(text);
    }

    // Formats into code units: bytes for narrow output, UTF-16 code units for wide output.
    template<bool Wide>
    class Formatter
    {
    public:
        Formatter(uint32_t format, GuestArgs args) : format(format), args(args) {}

        std::u16string Run()
        {
            for (uint32_t p = format;; )
            {
                char16_t c = ReadUnit(p);
                p += kUnit;
                if (c == 0)
                    break;
                if (c != '%')
                {
                    out += c;
                    continue;
                }
                p = Conversion(p);
            }
            return out;
        }

    private:
        static constexpr uint32_t kUnit = Wide ? 2 : 1;

        static char16_t ReadUnit(uint32_t address)
        {
            return Wide ? char16_t(guest::g_memory.Read16(address)) : char16_t(guest::g_memory.Read8(address));
        }

        // Narrow output can only hold characters the C locale's wctomb accepts.
        char16_t ToOutput(char16_t c) const
        {
            if (!Wide && c > 0xFF)
                Fatal("printf: wide character U+{:04X} in narrow output (format at {:08X})", uint32_t(c), format);
            return c;
        }

        uint32_t Conversion(uint32_t p)
        {
            uint32_t flags = 0;
            int width = 0;
            int precision = -1;
            char16_t c = ReadUnit(p);

            for (;; p += kUnit, c = ReadUnit(p))
            {
                if (c == '-') flags |= kLeft;
                else if (c == '+') flags |= kSign;
                else if (c == ' ') flags |= kSignSpace;
                else if (c == '#') flags |= kAlternate;
                else if (c == '0') flags |= kLeadZero;
                else break;
            }

            if (c == '*')
            {
                width = args.NextInt();
                if (width < 0)
                {
                    flags |= kLeft;
                    width = -width;
                }
                p += kUnit;
                c = ReadUnit(p);
            }
            else
            {
                for (; c >= '0' && c <= '9'; p += kUnit, c = ReadUnit(p))
                    width = width * 10 + (c - '0');
            }

            if (c == '.')
            {
                p += kUnit;
                c = ReadUnit(p);
                precision = 0;
                if (c == '*')
                {
                    precision = args.NextInt();
                    if (precision < 0)
                        precision = -1;
                    p += kUnit;
                    c = ReadUnit(p);
                }
                else
                {
                    for (; c >= '0' && c <= '9'; p += kUnit, c = ReadUnit(p))
                        precision = precision * 10 + (c - '0');
                }
            }

            for (;; p += kUnit, c = ReadUnit(p))
            {
                if (c == 'h') flags |= kShort;
                else if (c == 'l')
                {
                    if (ReadUnit(p + kUnit) == 'l')
                    {
                        flags |= kLongLong;
                        p += kUnit;
                    }
                    else
                        flags |= kLong;
                }
                else if (c == 'w') flags |= kWideChar;
                else if (c == 'I')
                {
                    const char16_t a = ReadUnit(p + kUnit), b = ReadUnit(p + 2 * kUnit);
                    if (a == '6' && b == '4')
                    {
                        flags |= kI64;
                        p += 2 * kUnit;
                    }
                    else if (a == '3' && b == '2')
                    {
                        flags &= ~kI64;
                        p += 2 * kUnit;
                    }
                    else if (a == 'd' || a == 'i' || a == 'o' || a == 'u' || a == 'x' || a == 'X')
                    {
                        // Pointer-sized integer: 32 bits on this platform.
                    }
                    else
                        Fatal("printf: unsupported 'I' size prefix (format at {:08X})", format);
                }
                else break;
            }
            p += kUnit; // past the conversion character

            std::u16string text;
            std::u16string prefix;
            switch (c)
            {
            case 'C':
                if (!(flags & (kShort | kLong | kWideChar)))
                    flags |= Wide ? kShort : kWideChar;
                [[fallthrough]];
            case 'c':
            {
                const uint32_t value = uint32_t(args.NextInt());
                const bool wideArgument = Wide ? !(flags & kShort) : (flags & (kLong | kWideChar)) != 0;
                text += ToOutput(wideArgument ? char16_t(value & 0xFFFF) : char16_t(value & 0xFF));
                break;
            }
            case 'S':
                if (!(flags & (kShort | kLong | kWideChar)))
                    flags |= Wide ? kShort : kWideChar;
                [[fallthrough]];
            case 's':
            {
                const uint32_t string = args.NextPointer();
                const bool wideArgument = Wide ? !(flags & kShort) : (flags & (kLong | kWideChar)) != 0;
                if (string == 0)
                    AppendNull(text, precision);
                else
                    AppendString(text, string, wideArgument, precision < 0 ? UINT32_MAX : uint32_t(precision));
                break;
            }
            case 'Z':
            {
                // Pointer to an ANSI_STRING, or a UNICODE_STRING with l/w: Length (bytes), MaximumLength, Buffer.
                auto& mem = guest::g_memory;
                const uint32_t counted = args.NextPointer();
                const uint32_t buffer = counted ? mem.Read32(counted + 4) : 0;
                if (buffer == 0)
                    AppendNull(text, -1); // the CRT takes the whole "(null)" here, ignoring the precision
                else
                {
                    const bool wideArgument = (flags & (kLong | kWideChar)) != 0;
                    const uint32_t length = mem.Read16(counted) / (wideArgument ? 2 : 1);
                    AppendString(text, buffer, wideArgument, length, false);
                }
                break;
            }
            case 'd':
            case 'i':
                flags |= kSigned;
                text = Integer(flags, precision, 10, false, prefix);
                break;
            case 'u':
                text = Integer(flags, precision, 10, false, prefix);
                break;
            case 'o':
                if (flags & kAlternate)
                    flags |= kForceOctal;
                text = Integer(flags, precision, 8, false, prefix);
                break;
            case 'x':
            case 'X':
                text = Integer(flags, precision, 16, c == 'X', prefix);
                break;
            case 'p':
                flags &= ~(kShort | kLongLong | kI64);
                text = Integer(flags, 8, 16, true, prefix);
                break;
            case 'e':
            case 'E':
            case 'f':
            case 'g':
            case 'G':
            {
                flags |= kSigned;
                const double value = args.NextDouble();
                if (!std::isfinite(value))
                    Fatal("printf: non-finite floating-point value (format at {:08X})", format);
                if (std::signbit(value))
                    flags |= kNegative;
                const bool alternate = (flags & kAlternate) != 0;
                int digits = precision < 0 ? 6 : precision;
                if (digits > 512)
                    digits = 512; // MAXPRECISION
                std::string converted;
                if (c == 'e' || c == 'E')
                    converted = FormatE(value, digits, alternate, c == 'E');
                else if (c == 'f')
                    converted = FormatF(value, digits, alternate);
                else
                    converted = FormatG(value, digits == 0 ? 1 : digits, alternate, c == 'G');
                text.assign(converted.begin(), converted.end());
                break;
            }
            case 'n':
                Fatal("printf: %n conversion (format at {:08X})", format);
            case 'a':
            case 'A':
                Fatal("printf: %a conversion (format at {:08X})", format);
            default:
                Fatal("printf: unknown conversion character 0x{:X} (format at {:08X})", uint32_t(c), format);
            }

            if (flags & kSigned)
            {
                if (flags & kNegative)
                    prefix = u"-";
                else if (flags & kSign)
                    prefix = u"+";
                else if (flags & kSignSpace)
                    prefix = u" ";
            }
            const int padding = width - int(text.size()) - int(prefix.size());
            if (!(flags & (kLeft | kLeadZero)) && padding > 0)
                out.append(size_t(padding), u' ');
            out += prefix;
            if ((flags & kLeadZero) && !(flags & kLeft) && padding > 0)
                out.append(size_t(padding), u'0');
            out += text;
            if ((flags & kLeft) && padding > 0)
                out.append(size_t(padding), u' ');
            return p;
        }

        std::u16string Integer(uint32_t& flags, int precision, uint32_t radix, bool upper, std::u16string& prefix)
        {
            uint64_t number;
            if (flags & (kI64 | kLongLong))
            {
                const uint64_t value = args.NextSlot();
                number = value;
                if ((flags & kSigned) && int64_t(value) < 0)
                {
                    number = 0 - value;
                    flags |= kNegative;
                }
            }
            else
            {
                int64_t value;
                const uint32_t raw = uint32_t(args.NextSlot());
                if (flags & kShort)
                    value = (flags & kSigned) ? int64_t(int16_t(raw)) : int64_t(uint16_t(raw));
                else
                    value = (flags & kSigned) ? int64_t(int32_t(raw)) : int64_t(raw);
                if ((flags & kSigned) && value < 0)
                {
                    number = uint64_t(-value);
                    flags |= kNegative;
                }
                else
                    number = uint64_t(value);
                number &= 0xFFFFFFFF;
            }

            if (radix == 16 && (flags & kAlternate) && number != 0)
                prefix = upper ? u"0X" : u"0x";

            if (precision < 0)
                precision = 1;
            else
            {
                flags &= ~kLeadZero;
                if (precision > 512)
                    precision = 512;
            }

            std::u16string digits;
            while (precision-- > 0 || number != 0)
            {
                const uint32_t digit = uint32_t(number % radix);
                number /= radix;
                digits.insert(digits.begin(), char16_t(digit < 10 ? '0' + digit : (upper ? 'A' : 'a') + digit - 10));
            }
            if ((flags & kForceOctal) && (digits.empty() || digits[0] != '0'))
                digits.insert(digits.begin(), u'0');
            return digits;
        }

        void AppendNull(std::u16string& text, int precision)
        {
            const std::u16string null = u"(null)";
            text += precision < 0 ? null : null.substr(0, size_t(precision));
        }

        // Up to `limit` characters, stopping at a terminator when `terminated`.
        void AppendString(std::u16string& text, uint32_t address, bool wideArgument, uint32_t limit,
            bool terminated = true)
        {
            auto& mem = guest::g_memory;
            for (uint32_t i = 0; i < limit; i++)
            {
                const char16_t c = wideArgument ? char16_t(mem.Read16(address + i * 2)) : char16_t(mem.Read8(address + i));
                if (terminated && c == 0)
                    break;
                text += ToOutput(c);
            }
        }

        uint32_t format;
        GuestArgs args;
        std::u16string out;
    };

    std::u16string Format(bool wide, uint32_t format, GuestArgs args)
    {
        return wide ? Formatter<true>(format, args).Run() : Formatter<false>(format, args).Run();
    }

    // Stores `text` like the CRT's string output: at most `count` units, then a terminator if there is room.
    // Returns the length, or -1 if the text did not fit.
    int32_t Store(bool wide, uint32_t buffer, uint32_t count, const std::u16string& text)
    {
        auto& mem = guest::g_memory;
        const uint32_t unit = wide ? 2 : 1;
        const uint32_t length = uint32_t(text.size());
        if (buffer == 0)
            return int32_t(length); // count == 0: only measuring
        const uint32_t written = length < count ? length : count;
        for (uint32_t i = 0; i < written; i++)
        {
            if (wide)
                mem.Write16(buffer + i * unit, text[i]);
            else
                mem.Write8(buffer + i, uint8_t(text[i]));
        }
        if (length < count)
        {
            if (wide)
                mem.Write16(buffer + length * unit, 0);
            else
                mem.Write8(buffer + length, 0);
        }
        return length <= count ? int32_t(length) : -1;
    }

    // _snprintf/_vsnprintf/_snwprintf: a NULL format, or a NULL buffer with a nonzero count, is an invalid parameter.
    int32_t CountedPrint(bool wide, uint32_t buffer, uint32_t count, uint32_t format, GuestArgs args)
    {
        if (format == 0 || (buffer == 0 && count != 0))
            return -1;
        return Store(wide, buffer, count, Format(wide, format, args));
    }

    int32_t UnboundedPrint(bool wide, uint32_t buffer, uint32_t format, GuestArgs args)
    {
        if (format == 0 || buffer == 0)
            return -1;
        return Store(wide, buffer, UINT32_MAX, Format(wide, format, args));
    }
}

int32_t kernel::FormatWide(uint32_t buffer, uint32_t count, uint32_t format, std::initializer_list<uint64_t> args)
{
    return CountedPrint(true, buffer, count, format, GuestArgs::FromValues(args));
}

// int sprintf(char* buffer, const char* format, ...)
GUEST_IMPORT(sprintf)
{
    ctx.r3.s64 = UnboundedPrint(false, ctx.r3.u32, ctx.r4.u32, GuestArgs::FromRegisters(ctx, 2));
}

// int _snprintf(char* buffer, size_t count, const char* format, ...)
GUEST_IMPORT(_snprintf)
{
    ctx.r3.s64 = CountedPrint(false, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, GuestArgs::FromRegisters(ctx, 3));
}

// int _vsnprintf(char* buffer, size_t count, const char* format, va_list args)
GUEST_IMPORT(_vsnprintf)
{
    ctx.r3.s64 = CountedPrint(false, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, GuestArgs::FromVaList(ctx.r6.u32));
}

// int swprintf(wchar_t* buffer, const wchar_t* format, ...): the old Microsoft form without a count.
GUEST_IMPORT(swprintf)
{
    ctx.r3.s64 = UnboundedPrint(true, ctx.r3.u32, ctx.r4.u32, GuestArgs::FromRegisters(ctx, 2));
}

// int _snwprintf(wchar_t* buffer, size_t count, const wchar_t* format, ...)
GUEST_IMPORT(_snwprintf)
{
    ctx.r3.s64 = CountedPrint(true, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, GuestArgs::FromRegisters(ctx, 3));
}

// int vswprintf(wchar_t* buffer, const wchar_t* format, va_list args): the old Microsoft form without a count.
GUEST_IMPORT(vswprintf)
{
    ctx.r3.s64 = UnboundedPrint(true, ctx.r3.u32, ctx.r4.u32, GuestArgs::FromVaList(ctx.r5.u32));
}

// int _vscwprintf(const wchar_t* format, va_list args): the length the output would have.
GUEST_IMPORT(_vscwprintf)
{
    ctx.r3.s64 = ctx.r3.u32 ? int32_t(Format(true, ctx.r3.u32, GuestArgs::FromVaList(ctx.r4.u32)).size()) : -1;
}

// int vsprintf(char* buffer, const char* format, va_list args)
GUEST_IMPORT(vsprintf)
{
    ctx.r3.s64 = UnboundedPrint(false, ctx.r3.u32, ctx.r4.u32, GuestArgs::FromVaList(ctx.r5.u32));
}

// int _vsnwprintf(wchar_t* buffer, size_t count, const wchar_t* format, va_list args)
GUEST_IMPORT(_vsnwprintf)
{
    ctx.r3.s64 = CountedPrint(true, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, GuestArgs::FromVaList(ctx.r6.u32));
}
