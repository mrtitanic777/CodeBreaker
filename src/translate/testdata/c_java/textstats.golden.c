/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from
   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */
#include "jrt.h"

typedef struct TextStats TextStats;
extern const jclass TextStats_class;

struct TextStats {
	jobject hdr_;
};
struct TextStats_vt {
	jobject_vt o_;
};
static jstring* TextStats_POEM = NULL;
static bool TextStats_inited_;

static jstring* lit_1;
static jstring* lit_2;
static jstring* lit_3;
static jstring* lit_4;
static jstring* lit_5;
static jstring* lit_6;
static jstring* lit_7;
static jstring* lit_8;

static void TextStats_init(TextStats* self);
static TextStats* TextStats_new(void);
static int32_t TextStats_vowels(jstring*);
static jstring* TextStats_histogram(jstring*);
static jstring* TextStats_reverseWords(jstring*);
static jstring* TextStats_sortedDigits(int32_t);
static int32_t TextStats_mix(int32_t, int32_t);
static int64_t TextStats_mixLong(int64_t, int32_t);
static int32_t TextStats_safeDiv(int32_t, int32_t);
static int32_t TextStats_saturate(double);
static int32_t TextStats_firstPair(int32_t);
static bool TextStats_isPalindrome(jstring*);
static jstring* TextStats_caesar(jstring*, int32_t);
static void TextStats_main(jarray*);
static void TextStats_finit(TextStats* self);
static void TextStats_clinit(void);
static void TextStats_init(TextStats* self) {
	TextStats_finit(self);
}

static TextStats* TextStats_new(void) {
	TextStats_clinit();
	TextStats* o = (TextStats*)jrt_alloc(&TextStats_class);
	TextStats_init(o);
	return o;
}

static int32_t TextStats_vowels(jstring* s) {
	TextStats_clinit();
	int32_t n = 0;
	{
		jarray* j2x_a1 = jrt_String_toCharArray_(jrt_String_toLowerCase_(s));
		{
			int32_t j2x_i2 = 0;
			for (; j2x_i2 < JLEN(j2x_a1); j2x_i2++) {
				char16_t c = JA(char16_t, j2x_a1, j2x_i2);
				switch (c) {
				case u'a':
				{
				}
				case u'e':
				{
				}
				case u'i':
				{
				}
				case u'o':
				{
				}
				case u'u':
				{
					n++;
					break;
				}
				default:
				{
					break;
				}
				}
			}
		}
	}
	return n;
}

static jstring* TextStats_histogram(jstring* s) {
	TextStats_clinit();
	jarray* counts = jrt_new_array(sizeof(int32_t), 26);
	{
		int32_t i = 0;
		for (; i < jrt_String_length_(s); i++) {
			char16_t c = jrt_Character_toLowerCase_C(jrt_String_charAt_I(s, i));
			if (c >= u'a' && c <= u'z') {
				JA(int32_t, counts, c - u'a')++;
			}
		}
	}
	jrt_StringBuilder* sb = jrt_StringBuilder_new_();
	{
		int32_t i = 0;
		for (; i < 26; i++) {
			if (JA(int32_t, counts, i) > 0) {
				jrt_StringBuilder_append_I(jrt_StringBuilder_append_C(sb, (char16_t)((u'a' + i))), JA(int32_t, counts, i));
			}
		}
	}
	return jrt_toString((jobject*)sb);
}

static jstring* TextStats_reverseWords(jstring* s) {
	TextStats_clinit();
	jarray* parts = jrt_String_split_Str(jrt_String_trim_(s), jrt_lit(&lit_1, u" ", 1));
	jrt_StringBuilder* sb = jrt_StringBuilder_new_();
	{
		int32_t i = JLEN(parts) - 1;
		for (; i >= 0; i--) {
			if (jrt_String_isEmpty_(JA(jstring*, parts, i))) {
				continue;
			}
			if (jrt_StringBuilder_length_(sb) > 0) {
				jrt_StringBuilder_append_C(sb, u' ');
			}
			jrt_StringBuilder_append_O(sb, (jobject*)jrt_StringBuilder_reverse_(jrt_StringBuilder_new_Str(JA(jstring*, parts, i))));
		}
	}
	return jrt_toString((jobject*)sb);
}

static jstring* TextStats_sortedDigits(int32_t v) {
	TextStats_clinit();
	jarray* cs = jrt_String_toCharArray_(jrt_Integer_toString_I(v));
	jrt_Arrays_sort_CA(cs);
	return jrt_String_new_CA(cs);
}

static int32_t TextStats_mix(int32_t a, int32_t b) {
	TextStats_clinit();
	int32_t h = a * -1640531535 + (b << 7) - (jrt_ushr_I(b, 3)) + (a >> 5);
	return h ^ (jrt_ushr_I(h, 16));
}

static int64_t TextStats_mixLong(int64_t a, int32_t s) {
	TextStats_clinit();
	return (a << (s & 63)) ^ (jrt_ushr_J(a, ((s + 3) & 63))) ^ (a * 1000003LL);
}

static int32_t TextStats_safeDiv(int32_t a, int32_t b) {
	TextStats_clinit();
	{
		jrt_try t1;
		jrt_push(&t1);
		if (setjmp(t1.jb) == 0) {
			{ int32_t r_ = jrt_div_I(a, b) + jrt_rem_I(a, b); jrt_pop(&t1); return r_; }
			jrt_pop(&t1);
		} else {
			jrt_pop(&t1);
			if (jrt_instanceof(t1.ex, &jrt_ArithmeticException_class)) {
				jrt_Throwable* e = (jrt_Throwable*)t1.ex;
				return JRT_INT_MIN;
			} else {
				jrt_throw(t1.ex);
			}
		}
	}
}

static int32_t TextStats_saturate(double d) {
	TextStats_clinit();
	return jrt_d2i((d * 1000000000.0));
}

static int32_t TextStats_firstPair(int32_t n) {
	TextStats_clinit();
	int32_t found = -1;
	{
		int32_t i = 1;
		for (; i < 50; i++) {
			{
				{
					int32_t j = i + 1;
					for (; j < 50; j++) {
						if (jrt_rem_I((i * j), (jrt_Math_abs_I(n % 97) + 3)) == 0 && i + j > 10) {
							found = i * 100 + j;
							goto outer_brk;
						}
						if (j > i + 20) {
							goto outer_cont;
						}
					}
				}
			}
			outer_cont:;
		}
	}
	outer_brk:;
	return found;
}

static bool TextStats_isPalindrome(jstring* s) {
	TextStats_clinit();
	int32_t i = 0;
	int32_t j = jrt_String_length_(s) - 1;
	while (i < j) {
		while (i < j && !jrt_Character_isLetterOrDigit_C(jrt_String_charAt_I(s, i))) {
			i++;
		}
		while (i < j && !jrt_Character_isLetterOrDigit_C(jrt_String_charAt_I(s, j))) {
			j--;
		}
		if (jrt_Character_toLowerCase_C(jrt_String_charAt_I(s, i)) != jrt_Character_toLowerCase_C(jrt_String_charAt_I(s, j))) {
			return false;
		}
		i++;
		j--;
	}
	return true;
}

static jstring* TextStats_caesar(jstring* s, int32_t k) {
	TextStats_clinit();
	jrt_StringBuilder* sb = jrt_StringBuilder_new_();
	int32_t shift = jrt_Math_floorMod_II(k, 26);
	{
		jarray* j2x_a3 = jrt_String_toCharArray_(s);
		{
			int32_t j2x_i4 = 0;
			for (; j2x_i4 < JLEN(j2x_a3); j2x_i4++) {
				char16_t c = JA(char16_t, j2x_a3, j2x_i4);
				if (jrt_Character_isUpperCase_C(c)) {
					jrt_StringBuilder_append_C(sb, (char16_t)((u'A' + (c - u'A' + shift) % 26)));
				} else if (jrt_Character_isLowerCase_C(c)) {
					jrt_StringBuilder_append_C(sb, (char16_t)((u'a' + (c - u'a' + shift) % 26)));
				} else {
					jrt_StringBuilder_append_C(sb, c);
				}
			}
		}
	}
	return jrt_toString((jobject*)sb);
}

static void TextStats_main(jarray* args) {
	TextStats_clinit();
	jarray* lines = jrt_String_split_Str(TextStats_POEM, jrt_lit(&lit_2, u"\n", 1));
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(4, jrt_str_I(JLEN(lines)), jrt_lit(&lit_3, u" lines, ", 8), jrt_str_I(TextStats_vowels(TextStats_POEM)), jrt_lit(&lit_4, u" vowels", 7)));
	{
		jarray* j2x_a5 = lines;
		{
			int32_t j2x_i6 = 0;
			for (; j2x_i6 < JLEN(j2x_a5); j2x_i6++) {
				jstring* line = JA(jstring*, j2x_a5, j2x_i6);
				jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(3, jrt_str_I(jrt_String_length_(line)), jrt_lit(&lit_5, u": ", 2), jrt_str_O((jobject*)TextStats_reverseWords(line))));
			}
		}
	}
	jrt_PrintStream_println_Str(jrt_System_out, TextStats_histogram(TextStats_POEM));
	jstring* o1 = NULL;
	int32_t o2 = 0;
	int64_t o3 = 0;
	jrt_PrintStream_println_Str(jrt_System_out, (o1 = TextStats_sortedDigits(907153), o2 = TextStats_mix(12345, 678), o3 = TextStats_mixLong(-987654321LL, 9), jrt_concat(5, jrt_str_O((jobject*)o1), jrt_lit(&lit_1, u" ", 1), jrt_str_I(o2), jrt_lit(&lit_1, u" ", 1), jrt_str_J(o3))));
	int32_t o4 = 0;
	int32_t o5 = 0;
	int32_t o6 = 0;
	jrt_PrintStream_println_Str(jrt_System_out, (o4 = TextStats_safeDiv(17, 5), o5 = TextStats_safeDiv(3, 0), o6 = TextStats_safeDiv(JRT_INT_MIN, -1), jrt_concat(5, jrt_str_I(o4), jrt_lit(&lit_1, u" ", 1), jrt_str_I(o5), jrt_lit(&lit_1, u" ", 1), jrt_str_I(o6))));
	int32_t o7 = 0;
	int32_t o8 = 0;
	int32_t o9 = 0;
	jrt_PrintStream_println_Str(jrt_System_out, (o7 = TextStats_saturate(3.5), o8 = TextStats_saturate(-1.0e+300), o9 = TextStats_saturate(NAN), jrt_concat(5, jrt_str_I(o7), jrt_lit(&lit_1, u" ", 1), jrt_str_I(o8), jrt_lit(&lit_1, u" ", 1), jrt_str_I(o9))));
	int32_t o10 = 0;
	bool o11 = 0;
	jstring* o12 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (o10 = TextStats_firstPair(12), o11 = TextStats_isPalindrome(jrt_lit(&lit_6, u"A man, a plan, a canal: Panama", 30)), o12 = TextStats_caesar(jrt_lit(&lit_7, u"Hello, World", 12), 29), jrt_concat(5, jrt_str_I(o10), jrt_lit(&lit_1, u" ", 1), jrt_str_Z(o11), jrt_lit(&lit_1, u" ", 1), jrt_str_O((jobject*)o12))));
	int32_t x = JRT_INT_MAX;
	x++;
	int64_t big = JRT_LONG_MAX;
	big += 2;
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(10, jrt_str_I(x), jrt_lit(&lit_1, u" ", 1), jrt_str_J(big), jrt_lit(&lit_1, u" ", 1), jrt_str_C((char16_t)(65)), jrt_str_I((int32_t)(u'z')), jrt_lit(&lit_1, u" ", 1), jrt_str_I((int32_t)(int8_t)(200)), jrt_lit(&lit_1, u" ", 1), jrt_str_I((int32_t)(int16_t)(70000))));
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(11, jrt_str_D(1.0 / 3), jrt_lit(&lit_1, u" ", 1), jrt_str_D(100.0), jrt_lit(&lit_1, u" ", 1), jrt_str_D(1.0e-05), jrt_lit(&lit_1, u" ", 1), jrt_str_D(12345678.9), jrt_lit(&lit_1, u" ", 1), jrt_str_F((float)(0.1)), jrt_lit(&lit_1, u" ", 1), jrt_str_D(-0.0)));
}

static void TextStats_finit(TextStats* self) {
}

static void TextStats_clinit(void) {
	if (TextStats_inited_) return;
	TextStats_inited_ = true;
	TextStats_POEM = jrt_lit(&lit_8, u"Shall I compare thee\nto a summer's day?\nThou art more lovely\nand more temperate.\n", 81);
}

static const struct TextStats_vt TextStats_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass TextStats_class = {"TextStats", &jrt_Object_class, sizeof(TextStats), &TextStats_vt_, 0, 0, 0};


int main(int argc, char** argv) {
	const char* which = getenv("JRT_MAIN");
	if (which && strcmp(which, "TextStats") == 0) return jrt_run_main(TextStats_main, argc, argv);
	return jrt_run_main(TextStats_main, argc, argv);
}
