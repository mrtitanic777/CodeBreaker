/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2026 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  C++ name demangling.
*
****************************************************************************/


#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string_view>

#include "watcom.h"
#include "demangle.h"
#include "run_state.hpp"

#include "clibext.h"

namespace {

constexpr std::string_view kPrefixImpLower = "__imp_";
constexpr std::string_view kPrefixImpUpper = "__IMP_";
constexpr std::string_view kPrefixData = ".PREFIX_DATA.";

constexpr size_t kAutoBufferSize = 80;
constexpr int kSuppressLimit = 2;
constexpr char kNul = '\0';

constexpr size_t kTruncatedHashLen = 4;

constexpr char kTruncatedPrefix1 = 'T';
constexpr char kManglePrefix1 = 'W';
constexpr char kManglePrefix2 = '?';
constexpr char kPrefixEmbedded = '?';
constexpr char kMemberPointer = 'M';
constexpr char kUnderscore = '_';
constexpr char kInternalPrefix = '.';
constexpr char kPeriod = '.';
constexpr char kScopePrefix = ':';
constexpr char kTemplatePrefix = ':';
constexpr char kTemplateInt = '0';
constexpr char kTemplateType = '1';
constexpr char kPositiveInt = 'Z';
constexpr char kNegativeInt = 'Y';
constexpr char kOpnamePrefix = '$';
constexpr char kSymbolSuffix = '$';
constexpr char kTypeNamePrefix = '$';
constexpr char kTypeNameSuffix = '$';
constexpr char kThisQualPrefix = '.';
constexpr char kArrayPrefix = '[';
constexpr char kArraySuffix = ']';
constexpr char kFunctionPrefix = '(';
constexpr char kFunctionSuffix = ')';
constexpr char kOpFunPrefix = 'O';
constexpr char kRelFunPrefix = 'R';
constexpr char kSpecTypePrefix = 'T';
constexpr char kWatFunPrefix = 'W';
constexpr char kAsgnFunPrefix = 'A';

constexpr char kGroupCPrefix = 'C';
constexpr char kCtorPrefix1 = 'C';
constexpr char kCtorPrefix2 = 'T';
constexpr char kOpConvertPrefix2 = 'V';

constexpr char kGroupDPrefix = 'D';
constexpr char kDtorPrefix1 = 'D';
constexpr char kDtorPrefix2 = 'T';
constexpr char kOpDeletePrefix2 = 'L';
constexpr char kArrayDeletePrefix2 = 'A';

constexpr char kGroupNPrefix = 'N';
constexpr char kOpNewPrefix2 = 'W';
constexpr char kArrayNewPrefix2 = 'A';

char upper(char c)
{
    return (c < 'a' || c > 'z') ? c : static_cast<char>(c - 0x20);
}

bool isDigitChar(char c)
{
    return c >= '0' && c <= '9';
}

bool isIdentifier(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == kUnderscore || c == kPeriod;
}

bool startsWith(const char *p, std::string_view prefix)
{
    return std::strncmp(p, prefix.data(), prefix.size()) == 0;
}

using ReallocFn = void *(*)(void *, size_t);

constexpr size_t kMaxReplicate = 10;

struct Replicate {
    const char *ptr;
    size_t len;
};

/*
 * what the demangler keeps between its nested calls: the realloc function and the names a mangled name can refer back to
 */
struct DemangleState {
    ReallocFn userRealloc = nullptr;
    std::array<Replicate, kMaxReplicate> replicate{};
    int nextReplicate = 0;
};

enum class SymPrefix : unsigned char { None, Imp, Data };

/* where a type's pieces go in the output: the cursor before it, the length after it, and whether it is a right-hand piece */
struct TypeState {
    size_t prefix;
    size_t suffix;
    bool right;
};

/*
 * mangled character translations
 */
enum class CharGroup : unsigned char { Unused, BasicType, Pointer, Modifier };

struct EncodingEntry {
    const char *text;       /* translated string */
    CharGroup group;        /* flavour of character */
};

constexpr char kLowerTableLimit = 'A';

constexpr std::array<EncodingEntry, 31> kTypeEncoding{{
    /* 'A' */   { "char ",      CharGroup::BasicType },
    /* 'B' */   { "float ",     CharGroup::BasicType },
    /* 'C' */   { "char ",      CharGroup::BasicType },
    /* 'D' */   { "double ",    CharGroup::BasicType },
    /* 'E' */   { "... ",       CharGroup::BasicType },
    /* 'F' */   { "__far ",     CharGroup::Modifier },
    /* 'G' */   { "__far16 ",   CharGroup::Modifier },
    /* 'H' */   { "__huge ",    CharGroup::Modifier },
    /* 'I' */   { "int ",       CharGroup::BasicType },
    /* 'J' */   { "__based(",   CharGroup::Modifier },
    /* 'K' */   { nullptr,      CharGroup::Unused },
    /* 'L' */   { "long ",      CharGroup::BasicType },
    /* 'M' */   { "::* ",       CharGroup::Pointer },
    /* 'N' */   { "__near ",    CharGroup::Modifier },
    /* 'O' */   { nullptr,      CharGroup::Unused },
    /* 'P' */   { "* ",         CharGroup::Pointer },
    /* 'Q' */   { "bool ",      CharGroup::BasicType },
    /* 'R' */   { "& ",         CharGroup::Pointer },
    /* 'S' */   { "short ",     CharGroup::BasicType },
    /* 'T' */   { "long double ", CharGroup::BasicType },
    /* 'U' */   { "unsigned ",  CharGroup::Modifier },
    /* 'V' */   { "void ",      CharGroup::BasicType },
    /* 'W' */   { "wchar_t ",   CharGroup::BasicType },
    /* 'X' */   { "const ",     CharGroup::Modifier },
    /* 'Y' */   { "volatile ",  CharGroup::Modifier },
    /* 'Z' */   { "__int64 ",   CharGroup::BasicType },
    /* '[' */   { nullptr,      CharGroup::Unused },
    /* '\' */   { nullptr,      CharGroup::Unused },
    /* ']' */   { nullptr,      CharGroup::Unused },
    /* '^' */   { nullptr,      CharGroup::Unused },
    /* '_' */   { "",           CharGroup::BasicType }  /* for ctor/dtor return type */
}};

constexpr std::array<const char *, 21> kOperatorFunction{{
    /* A */ ">>",
    /* B */ "<<",
    /* C */ "!",
    /* D */ "[]",
    /* E */ "->",
    /* F */ "*",
    /* G */ "++",
    /* H */ "--",
    /* I */ "-",
    /* J */ "+",
    /* K */ "&",
    /* L */ "->*",
    /* M */ "/",
    /* N */ "%",
    /* O */ ",",
    /* P */ "()",
    /* Q */ "~",
    /* R */ "^",
    /* S */ "|",
    /* T */ "&&",
    /* U */ "||"
}};

constexpr std::array<const char *, 6> kRelationalFunction{{
    /* A */ "==",
    /* B */ "!=",
    /* C */ "<",
    /* D */ "<=",
    /* E */ ">",
    /* F */ ">="
}};

constexpr std::array<const char *, 11> kAssignmentFunction{{
    /* A */ "=",
    /* B */ "*=",
    /* C */ "+=",
    /* D */ "-=",
    /* E */ "/=",
    /* F */ "%=",
    /* G */ ">>=",
    /* H */ "<<=",
    /* I */ "&=",
    /* J */ "|=",
    /* K */ "^="
}};

struct WatcomObject {
    const char *name;
    char first;
    char second;
};

constexpr std::array<WatcomObject, 19> kWatcomObject{{
    { "__internal",     'A', '*' },
    { "__onceonly",     'B', 'I' },
    { "__arrdtorblk",   'D', 'A' },
    { "__defarg",       'D', 'F' },
    { "__debuginfo",    'D', 'I' },
    { "__dtorobjblk",   'D', 'O' },
    { "__mbrptrthunk",  'M', 'P' },
    { "__staticinit",   'S', 'I' },
    { "__throwblk",     'T', 'H' },
    { "__typeid",       'T', 'I' },
    { "__typesig",      'T', 'S' },
    { "__rtti",         'V', 'A' },
    { "__vbtbl",        'V', 'B' },
    { "__vftbl",        'V', 'F' },
    { "__vmtbl",        'V', 'M' },
    { "__vfthunk",      'V', 'T' },
    { "__typstattab",   'S', 'T' },
    { "__stattabcmd",   'C', 'M' },
    { "<unique>",       'U', 'N' }
}};

bool typeChar(char c, CharGroup group)
{
    const int i = c - kLowerTableLimit;
    if (i >= 0 && i < static_cast<int>(kTypeEncoding.size()))
        return kTypeEncoding[static_cast<size_t>(i)].group == group;
    return false;
}

unsigned charToDigit(char c)
{
    if (c >= '0' && c <= '9')
        return static_cast<unsigned>(c - '0');
    if (c >= 'A' && c <= 'Z')
        return static_cast<unsigned>(c - 'A' + 10);
    if (c >= 'a' && c <= 'z')
        return static_cast<unsigned>(c - 'a' + 10);
    return static_cast<unsigned>(-1);
}

/*
 * One demangling: the mangled input, the output being built (in "test" mode, with no buffer, only its length is counted),
 * and the parser's cursors.
 *
 * Notes:
 *  demangler works in two modes, first "test" calculates required buffer size and second "write" demangle symbol to
 *      output buffer
 *  "test" mode doesn't get exact buffer size, but it guaranties size to output not overflow, the calculated size can
 *      be a few bytes higher then exact size
 *  demangler is in "test" mode if output buffer pointer is nullptr
 *  the "index" field acts as a cursor in the output buffer
 *  demangler uses insert mode in the output buffer, not overwrite, first making room in the output buffer (moving
 *      existing data) and then inserting data at the current cursor position.
 *
 *  "output" is the output buffer, "size" its length (not used in "test" mode), "index" the current output position,
 *  "count" the length of the demangled string (virtual)
 */
class Demangler {
public:
    Demangler(DemangleState &g, const char *in, size_t len, char *buff, size_t buffSize)
        : input(in), output(buff), g_(g)
    {
        if (len != 0) {
            /* length of mangled name is known */
            end = in + len;
        }
        if (buff != nullptr && buffSize > 0) {
            size = buffSize - 1;
            buff[0] = '\0';
        }
    }

    const char *input;
    const char *end = nullptr;
    const char *scopePtr = nullptr;
    char *output;
    size_t size = 0;            /* buffer length without null terminator */
    size_t count = 0;           /* virtual length (may exceed size) */
    size_t index = 0;
    size_t pendingLoc = 0;
    size_t scopeLen = 0;
    int suppressOutput = 1;
    unsigned scopeIndex = 0;
    bool ctdtPending = false;
    bool cvPending = false;
    bool scopeName = false;
    bool baseName = false;
    SymPrefix symPrefix = SymPrefix::None;

    bool mangledName();
    void fullMangledName();
    void doCopy();
    bool typeEncoding(TypeState &state);
    size_t terminateOutput();

private:
    DemangleState &g_;

    /* output */
    void zapSpace();
    void emitChar(char c);
    void emitStr(const char *p);
    void emitCopy(size_t idx, const char *ptr);
    void emitInteger(long value);
    void emitArraySize(size_t value);
    void emitEncoding(size_t idx) { emitStr(kTypeEncoding[idx].text); }
    void setIndex(size_t idx) { index = idx; }
    void resetIndex(size_t offsetFromEnd);
    void setSuppression()
    {
        if (suppressOutput)
            suppressOutput++;
    }
    void resetSuppression()
    {
        if (suppressOutput)
            --suppressOutput;
    }
    void forceSuppression()
    {
        if (suppressOutput <= kSuppressLimit)
            suppressOutput += kSuppressLimit + 1;
    }
    void unforceSuppression()
    {
        if (suppressOutput > kSuppressLimit)
            suppressOutput -= kSuppressLimit + 1;
    }

    /* input */
    char prevChar() const { return upper(*(input - 1)); }
    char currChar() const
    {
        if (end == nullptr || input < end)
            return upper(*input);
        return kNul;
    }
    void advanceChar()
    {
        if ((end == nullptr || input < end) && *input != kNul)
            input++;
    }
    bool base32Num(long &value);
    bool base10Num(size_t &value);
    bool getZzLen(size_t &value);

    /* the grammar */
    bool basicType(TypeState &state);
    bool pointer(TypeState &state);
    bool dimension(TypeState &state);
    bool array(TypeState &state);
    bool function(TypeState &state);
    bool tqFunction(TypeState &state);
    bool unmodifiedType(TypeState &state);
    bool basedEncoding(TypeState &state);
    bool modifierList(TypeState &state);
    bool templateArg(TypeState &state);
    bool templateName(TypeState &state);
    bool watcomObject(TypeState &state);
    bool specialTypeNames(TypeState &state);
    bool operatorTable(TypeState &state, const char *const *table, size_t n);
    bool dtor(TypeState &state);
    bool ctor();
    bool userConversion(TypeState &state);
    bool opNew(TypeState &state, const char *what);
    bool opName(TypeState &state);
    bool name(TypeState &state);
    bool symName(TypeState &state);
    bool scope(TypeState &state, size_t &symbolLength);
    bool scopedName(TypeState &state);
    bool recursiveMangledName(TypeState &state);
    size_t demangleRecursive(const char *in, char *buff, size_t buffSize);
};

void Demangler::zapSpace()
{
    /* if in test mode or no characters output then return (no action) */
    if (output == nullptr || index == 0)
        return;
    if (index > size)
        return;
    if (output[index - 1] != ' ')
        return;
    count--;
    index--;
    /* remove single space on index position, shrink string if possible */
    if (index != count) {
        size_t last = size;
        if (last > count)
            last = count;
        if (last > index)
            std::memmove(&output[index], &output[index + 1], last - index);
    }
}

constexpr size_t kStartAdjust = 1 << 8;

void Demangler::emitChar(char c)
{
    if (suppressOutput > kSuppressLimit)
        return;
    /* virtual length always increased */
    count++;
    /* if in test mode then return (no output) */
    if (output == nullptr)
        return;
    /* check if physical buffer needs to grow */
    if (count > size) {
        if (g_.userRealloc == nullptr)
            return;
        size_t adjustSize = kStartAdjust;
        for (;;) {
            if (adjustSize == 0)
                return;
            const size_t testSize = size + adjustSize;
            /* allocate + 1 byte for null terminator */
            char *testRealloc = static_cast<char *>(g_.userRealloc(output, testSize + 1));
            if (testRealloc != nullptr) {
                output = testRealloc;
                size = testSize;
                break;
            }
            adjustSize >>= 1;
        }
    }
    /* cannot write beyond physical buffer */
    if (index < size) {
        size_t last = size;
        if (last > count - 1)
            last = count - 1;
        if (last > index)
            std::memmove(&output[index + 1], &output[index], last - index);
        /* write character to output */
        output[index] = c;
        index++;
    }
}

void Demangler::emitStr(const char *p)
{
    if (p != nullptr) {
        while (*p != kNul) {
            emitChar(*p);
            ++p;
        }
    }
}

void Demangler::resetIndex(size_t offsetFromEnd)
{
    size_t i = count;
    if (i > offsetFromEnd) {
        i -= offsetFromEnd;
        if (i > size)
            i = size;
    }
    index = i;
}

/* an identifier (or any string): idx characters of ptr, or all of ptr when idx is 0 */
void Demangler::emitCopy(size_t idx, const char *ptr)
{
    if (baseName && ctdtPending)
        unforceSuppression();
    if (idx == 0) {
        emitStr(ptr);
    } else {
        if (output != nullptr && size > count) {
            if (idx > size - count)
                idx = size - count;
        }
        while (idx-- > 0)
            emitChar(*ptr++);
    }
    if (baseName && ctdtPending)
        forceSuppression();
}

void Demangler::emitInteger(long value)
{
    char buff[12];
    std::sprintf(buff, "%ld", value);
    emitStr(buff);
}

void Demangler::emitArraySize(size_t value)
{
    if (value != 0) {
        char buff[24];
        std::sprintf(buff, "%lu", static_cast<unsigned long>(value));
        emitStr(buff);
    }
}

bool Demangler::base32Num(long &value)
{
    unsigned dig;
    unsigned long v = 0;
    bool rc = false;

    while ((dig = charToDigit(currChar())) < 32) {
        v = v * 32 + dig;
        advanceChar();
        rc = true;
    }
    value = static_cast<long>(v);
    return rc;
}

bool Demangler::base10Num(size_t &value)
{
    unsigned dig;
    size_t v = 0;
    bool rc = false;

    while ((dig = charToDigit(currChar())) < 10) {
        v = v * 10 + dig;
        advanceChar();
        rc = true;
    }
    value = v;
    return rc;
}

bool Demangler::getZzLen(size_t &pvalue)
{
    unsigned uvalue;

    if ((uvalue = charToDigit(currChar())) < 36) {
        advanceChar();
        size_t value = uvalue;
        if ((uvalue = charToDigit(currChar())) < 36) {
            advanceChar();
            value = value * 36 + uvalue;
            if ((uvalue = charToDigit(currChar())) < 36) {
                advanceChar();
                pvalue = value * 36 + uvalue;
                return true;
            }
        }
    }
    return false;
}

bool Demangler::basicType(TypeState &state)
{
    const char c = currChar();
    if (typeChar(c, CharGroup::BasicType)) {
        /* put a signed before signed char */
        if (c == 'C' && prevChar() != 'U') {
            setIndex(state.prefix);
            emitStr("signed ");
        }
        advanceChar();
        setIndex(state.prefix);
        emitEncoding(static_cast<size_t>(c - kLowerTableLimit));
        return true;
    }
    return false;
}

bool Demangler::pointer(TypeState &state)
{
    const char c = currChar();
    if (typeChar(c, CharGroup::Pointer)) {
        advanceChar();
        setIndex(state.prefix);
        emitEncoding(static_cast<size_t>(c - kLowerTableLimit));
        if (c == kMemberPointer) {
            if (currChar() == kTypeNamePrefix) {
                advanceChar();
                if (!scopedName(state))
                    return false;
                if (currChar() != kTypeNamePrefix)
                    return false;
                advanceChar();
                zapSpace();
            }
        }
        return true;
    }
    return false;
}

bool Demangler::dimension(TypeState &state)
{
    size_t value;

    resetIndex(state.suffix);
    emitStr("[");
    if (base10Num(value))
        emitArraySize(value);
    if (currChar() == kArraySuffix) {
        advanceChar();
        emitStr("]");
        return true;
    }
    return false;
}

bool Demangler::array(TypeState &state)
{
    resetIndex(state.suffix);
    zapSpace();
    if (state.right) {
        setIndex(state.prefix);
        emitChar('(');
        resetIndex(state.suffix);
        emitChar(')');
        state.right = false;
    }
    while (currChar() == kArrayPrefix) {
        advanceChar();
        if (!dimension(state))
            return false;
    }
    return typeEncoding(state);
}

bool Demangler::function(TypeState &state)
{
    size_t convOffset = 0;

    resetIndex(state.suffix);
    if (cvPending) {
        emitChar(' ');
        convOffset = index;
    } else {
        zapSpace();
    }
    if (state.right) {
        setIndex(state.prefix);
        emitChar('(');
        resetIndex(state.suffix);
        emitChar(')');
        state.right = false;
    }
    TypeState newState = state;
    emitStr("( ");
    setSuppression();
    bool firstArg = false;
    while (currChar() != kFunctionSuffix) {
        if (firstArg) {
            resetIndex(state.suffix);
            zapSpace();
            emitStr(", ");
        }
        newState.prefix = index;
        if (!typeEncoding(newState))
            return false;
        firstArg = true;
    }
    advanceChar();
    resetIndex(state.suffix);
    if (!firstArg)
        zapSpace();
    resetSuppression();
    emitStr(")");
    if (cvPending) {
        unforceSuppression();
        setIndex(convOffset);
    } else {
        setIndex(state.prefix);
    }
    newState.prefix = index;
    if (typeEncoding(newState)) {
        cvPending = false;
        return true;
    }
    return false;
}

bool Demangler::tqFunction(TypeState &state)
{
    const char *tqPtr = input;
    size_t tqLen = 0;
    char c = currChar();
    while (typeChar(c, CharGroup::Modifier)) {
        /* note: doesn't handle based modifiers */
        tqLen++;
        advanceChar();
        c = currChar();
    }
    if (c == kFunctionPrefix) {
        advanceChar();
        if (function(state)) {
            resetIndex(state.suffix);
            if (tqLen > 0) {
                emitChar(' ');
                while (tqLen--) {
                    c = *tqPtr++;
                    emitEncoding(static_cast<size_t>(upper(c) - kLowerTableLimit));
                }
                zapSpace();
            }
            return true;
        }
    }
    return false;
}

bool Demangler::unmodifiedType(TypeState &state)
{
    const char c = currChar();
    if (c == kTypeNamePrefix) {
        advanceChar();
        if (scopedName(state)) {
            if (currChar() == kTypeNameSuffix) {
                advanceChar();
                return true;
            }
        }
    } else if (c == kFunctionPrefix) {
        advanceChar();
        return function(state);
    } else if (c == kThisQualPrefix) {
        advanceChar();
        return tqFunction(state);
    } else if (c == kArrayPrefix) {
        return array(state);
    } else if (typeChar(c, CharGroup::Pointer)) {
        if (pointer(state)) {
            state.right = true;
            return typeEncoding(state);
        }
    } else {
        return basicType(state);
    }
    return false;
}

bool Demangler::basedEncoding(TypeState &state)
{
    size_t len;

    switch (currChar()) {
    case 'S':
        advanceChar();
        emitStr("__self");
        emitStr(") ");
        return true;
    case 'V':
        advanceChar();
        emitStr("void");
        emitStr(") ");
        return true;
    case 'L':
        advanceChar();
        if (getZzLen(len)) {
            emitStr("\"");
            emitCopy(len, input);
            while (len--)
                advanceChar();
            emitStr("\"");
            emitStr(") ");
            return true;
        }
        break;
    case 'A':
    case 'F':
        advanceChar();
        if (currChar() == kTypeNamePrefix) {
            advanceChar();
            TypeState newState = state;
            newState.prefix = index;
            newState.suffix = count - index;
            if (scopedName(newState)) {
                resetIndex(newState.suffix);
                zapSpace();
                emitStr(") ");
                return true;
            }
        }
        break;
    }
    return false;
}

bool Demangler::modifierList(TypeState &state)
{
    char c = currChar();
    while (typeChar(c, CharGroup::Modifier)) {
        advanceChar();
        if (c == 'U' && currChar() == 'A') {
            /* don't emit unsigned before default char */
        } else {
            setIndex(state.prefix);
            emitEncoding(static_cast<size_t>(c - kLowerTableLimit));
        }
        if (c == 'J') {
            if (!basedEncoding(state))
                return false;
        }
        c = currChar();
    }
    return true;
}

bool Demangler::typeEncoding(TypeState &state)
{
    bool ret = false;

    if (baseName)
        forceSuppression();
    const char c = currChar();
    if (typeChar(c, CharGroup::Modifier)) {
        if (modifierList(state))
            ret = unmodifiedType(state);
    } else {
        ret = unmodifiedType(state);
    }
    return ret;
}

bool Demangler::templateArg(TypeState &state)
{
    char c = currChar();
    if (c == kTemplateInt) {
        long value;
        advanceChar();
        if (base32Num(value)) {
            c = currChar();
            if (c == kPositiveInt) {
                advanceChar();
                setIndex(state.prefix);
                emitInteger(value);
                return true;
            } else if (c == kNegativeInt) {
                advanceChar();
                setIndex(state.prefix);
                emitInteger(static_cast<long>(0UL - static_cast<unsigned long>(value)));
                return true;
            }
        }
    } else if (c == kTemplateType) {
        advanceChar();
        return typeEncoding(state);
    } else if (c == kPrefixEmbedded) {
        advanceChar();
        return recursiveMangledName(state);
    }
    return false;
}

bool Demangler::templateName(TypeState &state)
{
    TypeState newState = state;
    resetIndex(state.suffix);
    emitStr("<");
    bool firstArg = false;
    char c = currChar();
    while (c == kTemplateInt || c == kTemplateType || c == kPrefixEmbedded) {
        if (firstArg) {
            resetIndex(state.suffix);
            zapSpace();
            emitStr(",");
        }
        newState.prefix = index;
        if (!templateArg(newState))
            return false;
        firstArg = true;
        c = currChar();
    }
    resetIndex(state.suffix);
    if (!firstArg)
        zapSpace();
    emitStr(">");
    return true;
}

bool Demangler::watcomObject(TypeState &state)
{
    size_t len;
    const char c = currChar();

    if (c == 'A') {
        advanceChar();
        advanceChar();
        setIndex(state.prefix);
        emitStr(kWatcomObject[0].name);
    } else {
        const char first = c;
        advanceChar();
        const char second = currChar();
        advanceChar();
        size_t i = 1;
        for (; i < kWatcomObject.size(); i++) {
            if (first == kWatcomObject[i].first && second == kWatcomObject[i].second) {
                setIndex(state.prefix);
                emitStr(kWatcomObject[i].name);
                break;
            }
        }
        /* check to make sure we found something */
        if (i == kWatcomObject.size()) {
            setIndex(state.prefix);
            emitStr(kWatcomObject[0].name);
        }
    }
    if (getZzLen(len)) {
        while (len--)
            advanceChar();
        if (currChar() == kTypeNameSuffix) {
            advanceChar();
            return true;
        }
    }
    return false;
}

bool Demangler::specialTypeNames(TypeState &state)
{
    char c = currChar();
    if (c == 'E') {
        advanceChar();
        setIndex(state.prefix);
        emitStr("__anonymous_enum ");
        for (; (c = currChar()) != kNul;) {
            advanceChar();
            if (c == kTypeNameSuffix)
                return true;
        }
    }
    return false;
}

/* operator, relational or assignment function: the operator's name from its table */
bool Demangler::operatorTable(TypeState &state, const char *const *table, size_t n)
{
    const int i = currChar() - kLowerTableLimit;
    if (i >= 0 && i < static_cast<int>(n)) {
        advanceChar();
        setIndex(state.prefix);
        emitStr("operator ");
        emitStr(table[i]);
        return true;
    }
    return false;
}

bool Demangler::dtor(TypeState &state)
{
    setIndex(state.prefix);
    emitChar('~');
    pendingLoc = count - index;
    ctdtPending = true;
    return true;
}

/* operator new / delete (and their array forms) */
bool Demangler::opNew(TypeState &state, const char *what)
{
    setIndex(state.prefix);
    emitStr("operator ");
    emitStr(what);
    return true;
}

bool Demangler::ctor()
{
    pendingLoc = count - index;
    ctdtPending = true;
    return true;
}

bool Demangler::userConversion(TypeState &state)
{
    setIndex(state.prefix);
    emitStr("operator ");
    cvPending = true;
    return true;
}

bool Demangler::opName(TypeState &state)
{
    char c = currChar();
    switch (c) {
    case kGroupCPrefix:
        advanceChar();
        c = currChar();
        if (c == kCtorPrefix2) {
            advanceChar();
            return ctor();
        } else if (c == kOpConvertPrefix2) {
            advanceChar();
            return userConversion(state);
        }
        break;
    case kGroupDPrefix:
        advanceChar();
        c = currChar();
        if (c == kDtorPrefix2) {
            advanceChar();
            return dtor(state);
        } else if (c == kOpDeletePrefix2) {
            advanceChar();
            return opNew(state, "delete");
        } else if (c == kArrayDeletePrefix2) {
            advanceChar();
            return opNew(state, "delete []");
        }
        break;
    case kGroupNPrefix:
        advanceChar();
        c = currChar();
        if (c == kOpNewPrefix2) {
            advanceChar();
            return opNew(state, "new");
        } else if (c == kArrayNewPrefix2) {
            advanceChar();
            return opNew(state, "new []");
        }
        break;
    case kAsgnFunPrefix:
        advanceChar();
        return operatorTable(state, kAssignmentFunction.data(), kAssignmentFunction.size());
    case kRelFunPrefix:
        advanceChar();
        return operatorTable(state, kRelationalFunction.data(), kRelationalFunction.size());
    case kOpFunPrefix:
        advanceChar();
        return operatorTable(state, kOperatorFunction.data(), kOperatorFunction.size());
    case kSpecTypePrefix:
        advanceChar();
        return specialTypeNames(state);
    case kWatFunPrefix:
        advanceChar();
        return watcomObject(state);
    }
    return false;
}

bool Demangler::name(TypeState &state)
{
    char c = currChar();
    if (isDigitChar(c)) {
        const char *id;
        size_t len;
        const int i = c - '0';
        /* in case C++ compiler and demangler don't match; don't crash */
        if (i < 0 || i >= g_.nextReplicate || i >= static_cast<int>(kMaxReplicate)) {
            id = "@";
            len = 1;
        } else {
            id = g_.replicate[static_cast<size_t>(i)].ptr;
            len = g_.replicate[static_cast<size_t>(i)].len;
        }
        if (ctdtPending) {
            resetIndex(pendingLoc);
            emitCopy(len, id);
            ctdtPending = false;
        }
        if (scopeName && scopeIndex == 0) {
            scopeName = false;
            scopePtr = id;
            scopeLen = len;
        }
        setIndex(state.prefix);
        emitCopy(len, id);
        emitChar(' ');
        advanceChar();
        return true;
    }
    const char *ptr = input;
    size_t len = 0;
    while (isIdentifier(c)) {
        len++;
        advanceChar();
        c = currChar();
    }
    if (scopeName && scopeIndex == 0) {
        scopeName = false;
        scopePtr = ptr;
        scopeLen = len;
    }
    if (c == kSymbolSuffix) {
        advanceChar();
        if (ctdtPending) {
            resetIndex(pendingLoc);
            emitCopy(len, ptr);
            ctdtPending = false;
        }
        setIndex(state.prefix);
        emitCopy(len, ptr);
        emitChar(' ');
        if (g_.nextReplicate < static_cast<int>(kMaxReplicate)) {
            g_.replicate[static_cast<size_t>(g_.nextReplicate)] = Replicate{ ptr, len };
            g_.nextReplicate++;
        }
        return true;
    }
    return false;
}

bool Demangler::symName(TypeState &state)
{
    if (currChar() == kOpnamePrefix) {
        advanceChar();
        return opName(state);
    }
    return name(state);
}

bool Demangler::scope(TypeState &state, size_t &symbolLength)
{
    if (scopeName && scopeIndex > 0)
        scopeIndex--;
    const char c = currChar();
    if (c == kPrefixEmbedded) {
        advanceChar();
        return recursiveMangledName(state);
    } else if (c == kTemplatePrefix) {
        advanceChar();
        TypeState newState = state;
        newState.suffix = count - state.prefix - symbolLength;
        return templateName(newState);
    }
    const bool rc = symName(state);
    symbolLength = index - state.prefix - 1;
    return rc;
}

bool Demangler::scopedName(TypeState &state)
{
    if (symName(state)) {
        size_t symbolLength = index - state.prefix - 1;
        while (currChar() == kScopePrefix) {
            advanceChar();
            setIndex(state.prefix);
            if (currChar() != kTemplatePrefix) {
                if (baseName)
                    forceSuppression();
                emitStr("::");
            }
            if (!scope(state, symbolLength))
                return false;
            zapSpace();
        }
        return true;
    }
    return false;
}

bool Demangler::mangledName()
{
    TypeState newState{ 0, 0, false };
    if (scopedName(newState)) {
        resetIndex(0);
        zapSpace();
        return typeEncoding(newState);
    }
    return false;
}

void Demangler::fullMangledName()
{
    size_t advances = 1;

    if ((end == nullptr || static_cast<size_t>(end - input) >= kPrefixData.size()) && input[0] == '.' &&
        startsWith(input, kPrefixData)) {
        input += kPrefixData.size();
        symPrefix = SymPrefix::Data;
    } else if ((end == nullptr || static_cast<size_t>(end - input) >= kPrefixImpLower.size()) && input[0] == '_' &&
               (startsWith(input, kPrefixImpLower) || startsWith(input, kPrefixImpUpper))) {
        input += kPrefixImpLower.size();
        symPrefix = SymPrefix::Imp;
    }
    const char first = currChar();
    if (first == kTruncatedPrefix1 || first == kManglePrefix1) {
        if (first == kTruncatedPrefix1)
            advances += kTruncatedHashLen;
        advanceChar();
        if (currChar() == kManglePrefix2) {
            for (; advances != 0; --advances) {
                advanceChar();
                if (currChar() == kNul)
                    return;
            }
            mangledName();
        }
    }
}

void Demangler::doCopy()
{
    /* limit copy to available physical space if buffer exists */
    size_t maxLen = ~static_cast<size_t>(0);
    if (output != nullptr && size > count)
        maxLen = size - count;
    const char *ptr = input;
    size_t len = 0;
    char c = currChar();
    while (c != kNul && len < maxLen) {
        advanceChar();
        len++;
        c = currChar();
    }
    emitCopy(len, ptr);
}

size_t Demangler::terminateOutput()
{
    size_t outlen = count;
    if (output != nullptr) {
        if (size > 0) {
            if (outlen > size)
                outlen = size;
            /* remove all trailing spaces */
            while (outlen > 0 && output[outlen - 1] == ' ')
                outlen--;
        } else {
            outlen = 0;
        }
        output[outlen] = kNul;
    }
    /* size does not include '\0' */
    return outlen;
}

size_t Demangler::demangleRecursive(const char *in, char *buff, size_t buffSize)
{
    const ReallocFn saveReallocator = g_.userRealloc;
    g_.userRealloc = nullptr;
    Demangler data(g_, in, 0, buff, buffSize);
    data.mangledName();
    data.terminateOutput();
    g_.userRealloc = saveReallocator;
    return static_cast<size_t>(data.input - in);
}

bool Demangler::recursiveMangledName(TypeState &state)
{
    std::array<char, kAutoBufferSize> buff;

    input += demangleRecursive(input, buff.data(), buff.size());
    setIndex(state.prefix);
    emitCopy(0, buff.data());
    return true;
}

/* each public call starts with the replicated names forgotten and the caller's realloc function */
DemangleState &freshState(ReallocFn reallocator)
{
    DemangleState &g = cb_wlink::RunState::part<DemangleState>();
    g.nextReplicate = 0;
    g.userRealloc = reallocator;
    return g;
}

}  // namespace

size_t __demangle_l(            /* DEMANGLE A C++ NAME */
    char const *input,          /* - mangled C++ name */
    size_t len,                 /* - length of mangled name */
    char *buff,                 /* - output buffer for demangled C++ name */
    size_t buff_size )          /* - size of output buffer */
{
    DemangleState &g = freshState(nullptr);
    if (len == 0)
        len = std::strlen(input);
    Demangler data(g, input, len, buff, buff_size);
    if (__is_mangled(input, len))
        data.fullMangledName();
    else
        data.doCopy();
    /* size does not include '\0' */
    return data.terminateOutput();
}

size_t __is_mangled( char const *name, size_t len )
{
    if (len > 2) {
        len -= 2;
        size_t offset = 2;
        if (len > kPrefixData.size() && name[0] == '.' && startsWith(name, kPrefixData)) {
            len -= kPrefixData.size();
            name += kPrefixData.size();
            offset += kPrefixData.size();
        } else if (len > kPrefixImpLower.size() && name[0] == '_' &&
                   (startsWith(name, kPrefixImpLower) || startsWith(name, kPrefixImpUpper))) {
            len -= kPrefixImpLower.size();
            name += kPrefixImpLower.size();
            offset += kPrefixImpLower.size();
        }
        if (name[0] == kManglePrefix1 && name[1] == kManglePrefix2)
            return offset;
        else if (len > kTruncatedHashLen && name[0] == kTruncatedPrefix1 && name[1] == kManglePrefix2)
            return offset + kTruncatedHashLen;
    }
    return 0;
}

#if !defined( INSIDE_WLIB ) && !defined( INSIDE_WDIS )

namespace {

mangled_type checkInternal(char const *p, size_t len)
{
    mangled_type type;

    if (p[0] == kInternalPrefix) {
        len--;
        type = __MANGLED_INTERNAL;
    } else if (p[0] == kOpnamePrefix && upper(p[1]) == kWatFunPrefix) {
        len -= 2;
        type = __MANGLED_INTERNAL;
    } else if (p[0] == kOpnamePrefix && upper(p[1]) == kCtorPrefix1 && upper(p[2]) == kCtorPrefix2) {
        len -= 3;
        type = __MANGLED_CTOR;
    } else if (p[0] == kOpnamePrefix && upper(p[1]) == kDtorPrefix1 && upper(p[2]) == kDtorPrefix2) {
        len -= 3;
        type = __MANGLED_DTOR;
    } else {
        type = __MANGLED;
    }
    if (len == 0)
        type = __NOT_MANGLED;
    return type;
}

}  // namespace

mangled_type __is_mangled_internal( char const *name, size_t len )
{
    if (len > 2) {
        len -= 2;
        size_t offset = 2;
        if (name[0] == kManglePrefix1 && name[1] == kManglePrefix2) {
            return checkInternal(name + offset, len);
        } else if (len > kTruncatedHashLen && name[0] == kTruncatedPrefix1 && name[1] == kManglePrefix2) {
            len -= kTruncatedHashLen;
            offset += kTruncatedHashLen;
            return checkInternal(name + offset, len);
        }
    }
    return __NOT_MANGLED;
}

bool __unmangled_name(          /* FIND UNMANGLED BASE NAME */
    char const *name,           /* - mangled C++ name */
    size_t len,                 /* - length of mangled name */
    char const **basep,         /* - location of C++ base name */
    size_t *base_sizep )        /* - size of C++ base name */
{                               /* return true if name mangled */
    if (len == 0)
        len = std::strlen(name);
    const size_t mangledPrefixLen = __is_mangled(name, len);
    if (mangledPrefixLen == 0) {
        *basep = name;
        *base_sizep = len;
        return false;
    }
    const char *end = name + len;
    name += mangledPrefixLen;   /* skip magic cookie */
    *basep = name;
    size_t baseSize = 0;
    if (*name == kOpnamePrefix) {
        baseSize += 3;
        name++;
        if (upper(*name) == kWatFunPrefix)
            baseSize++;
    } else {
        for (; name != end; ++name) {
            if (*name == kSymbolSuffix)
                break;
            baseSize++;
        }
    }
    *base_sizep = baseSize;
    return true;
}

#if !defined( INSIDE_WLINK )

#if !defined( INSIDE_DIP )

size_t __demangle_t(            /* DEMANGLE A C++ TYPE */
    char const *input,          /* - mangled C++ type */
    size_t len,                 /* - length of mangled type */
    char *buff,                 /* - output buffer for demangled C++ type */
    size_t buff_size )          /* - size of output buffer */
{
    DemangleState &g = freshState(nullptr);
    Demangler data(g, input, len, buff, buff_size);
    data.suppressOutput = 0;
    TypeState newState{ 0, 0, false };
    data.typeEncoding(newState);
    /* size does not include '\0' */
    return data.terminateOutput();
}

size_t __demangle_r(            /* DEMANGLE A C++ NAME */
    char const *input,          /* - mangled C++ name */
    size_t len,                 /* - length of mangled name */
    char **buffp,               /* - output buffer for demangled C++ name */
    size_t buff_size,           /* - size of output buffer */
    void * (*realloc)( void *, size_t ) ) /* - size adjuster for output */
{
    DemangleState &g = freshState(realloc);
    if (len == 0)
        len = std::strlen(input);
    char *buff = nullptr;
    if (buffp != nullptr)
        buff = *buffp;
    Demangler data(g, input, len, buff, buff_size);
    data.suppressOutput = 0;
    if (__is_mangled(input, len))
        data.fullMangledName();
    else
        data.doCopy();
    const size_t outlen = data.terminateOutput();
    if (buffp != nullptr)
        *buffp = data.output;
    /* size does not include '\0' */
    return outlen;
}

#endif /* !INSIDE_DIP */

bool __scope_name(              /* EXTRACT A C++ SCOPE */
    char const *input,          /* - mangled C++ name */
    size_t len,                 /* - length of mangled name */
    unsigned index,             /* - scope wanted */
    char const **scopep,        /* - location of C++ scope name */
    size_t *scope_sizep )       /* - size of C++ scope name */
{                               /* returns true on success */
    DemangleState &g = freshState(nullptr);
    *scopep = nullptr;
    *scope_sizep = 0;
    if (len == 0)
        len = std::strlen(input);
    if (__is_mangled(input, len) == 0)
        return false;
    Demangler data(g, input, len, nullptr, 0);
    data.scopeName = true;
    data.scopeIndex = index;
    data.fullMangledName();
    if (data.scopeLen != 0) {
        *scopep = data.scopePtr;
        *scope_sizep = data.scopeLen;
        return true;
    }
    return false;
}

size_t __demangled_basename(    /* CREATE DEMANGLED BASE NAME */
    char const *input,          /* - mangled C++ name */
    size_t len,                 /* - length of mangled name */
    char *buff,                 /* - output buffer for demangled C++ base name */
    size_t buff_size )          /* - size of output buffer */
{                               /* return len of output */
    DemangleState &g = freshState(nullptr);
    if (len == 0)
        len = std::strlen(input);
    Demangler data(g, input, len, buff, buff_size);
    data.suppressOutput = 0;
    data.baseName = true;
    if (__is_mangled(input, len))
        data.fullMangledName();
    else
        data.doCopy();
    /* size does not include '\0' */
    return data.terminateOutput();
}

namespace {

/* the operator's mangled name when op is in the table (3 characters written to buff) */
bool mangleFromTable(char const *op, size_t len, char *buff, const char *const *table, size_t n, char prefix)
{
    for (size_t i = 0; i < n; i++) {
        if (std::strncmp(op, table[i], len) == 0 && table[i][len] == kNul) {
            buff[0] = kOpnamePrefix;
            buff[1] = prefix;
            buff[2] = static_cast<char>(i + kLowerTableLimit);
            return true;
        }
    }
    return false;
}

}  // namespace

size_t __mangle_operator(       /* MANGLE OPERATOR NAME */
    char const *op,             /* - operator token */
    size_t len,                 /* - length of operator token */
    char *buff )                /* - output buffer for mangled operator name */
{                               /* return len of mangled operator name; ZERO if not found */
    if (len == 0)
        len = std::strlen(op);
    if (mangleFromTable(op, len, buff, kOperatorFunction.data(), kOperatorFunction.size(), kOpFunPrefix) ||
        mangleFromTable(op, len, buff, kRelationalFunction.data(), kRelationalFunction.size(), kRelFunPrefix) ||
        mangleFromTable(op, len, buff, kAssignmentFunction.data(), kAssignmentFunction.size(), kAsgnFunPrefix))
        return 3;
    return 0;
}

#endif /* !INSIDE_WLINK */

#endif /* !INSIDE_WLIB && !INSIDE_WDIS */
