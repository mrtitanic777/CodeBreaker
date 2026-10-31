/* jrt.c -- the Java library in C: the implementation of jrt.h (see there for the object model). */
#include "jrt.h"
#include <stdarg.h>

/* ------------------------------------------------------------------------------------------- objects */
jrt_try* jrt_top = 0;

void* jrt_alloc(const jclass* c) {
	jobject* o = (jobject*)calloc(1, c->size);
	if (!o) { fprintf(stderr, "out of memory\n"); exit(3); }
	o->cls = c;
	return o;
}

static const void* find_itab(const jclass* c, const jclass* iface) {
	for (; c; c = c->super)
		for (int i = 0; i < c->n_itabs; ++i)
			if (c->itabs[i].iface == iface) return c->itabs[i].vt;
	return 0;
}
jobject* jrt_itab(jobject* o, const jclass* iface) { return o && find_itab(o->cls, iface) ? o : 0; }
const void* jrt_ivt(jobject* o, const jclass* iface) {
	if (!o) jrt_throw_npe();
	const void* vt = find_itab(o->cls, iface);
	if (!vt) jrt_throw_new(&jrt_ClassCastException_class, iface->name);
	return vt;
}
bool jrt_instanceof(jobject* o, const jclass* c) {
	if (!o) return false;
	if (c == &jrt_Object_class) return true;
	if (c->kind == 1) return find_itab(o->cls, c) != 0;
	for (const jclass* k = o->cls; k; k = k->super)
		if (k == c) return true;
	return false;
}
jobject* jrt_cast(jobject* o, const jclass* c) {
	if (!o || jrt_instanceof(o, c)) return o;
	char buf[256];
	snprintf(buf, sizeof buf, "class %s cannot be cast to class %s", o->cls->name, c->name);
	jrt_throw_new(&jrt_ClassCastException_class, buf);
}
void* jrt_nn(void* o) { if (!o) jrt_throw_npe(); return o; }

jstring* jrt_toString(jobject* o) { return ((const jobject_vt*)jrt_vt(o))->toString(o); }
bool jrt_equals(jobject* a, jobject* b) { return ((const jobject_vt*)jrt_vt(a))->equals(a, b); }
int32_t jrt_hashCode(jobject* o) { return ((const jobject_vt*)jrt_vt(o))->hashCode(o); }
bool jrt_Objects_equals_OO(jobject* a, jobject* b) { return a == b || (a && jrt_equals(a, b)); }
int32_t jrt_Objects_hashCode_O(jobject* o) { return o ? jrt_hashCode(o) : 0; }

static jstring* str8(const char* s) {
	int32_t n = (int32_t)strlen(s);
	jstring* r = jrt_str(0, n);
	for (int32_t i = 0; i < n; ++i) r->s[i] = (unsigned char)s[i];
	return r;
}
int32_t jrt_Object_hashCode(jobject* o) { return (int32_t)(((uintptr_t)o) >> 4); }
bool jrt_Object_equals(jobject* a, jobject* b) { return a == b; }
jstring* jrt_Object_toString(jobject* o) {
	char buf[300];
	snprintf(buf, sizeof buf, "%s@%x", o->cls->name, (unsigned)jrt_hashCode(o));
	return str8(buf);
}
const jobject_vt jrt_Object_vt = {jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode};
const jclass jrt_Object_class = {"java.lang.Object", 0, sizeof(jobject), &jrt_Object_vt, 0, 0, 0};

/* ------------------------------------------------------------------------------------------- exceptions */
static jstring* throwable_toString(jobject* o) {
	jstring* m = jrt_Throwable_getMessage_(o);
	jstring* name = str8(o->cls->name);
	return m ? jrt_concat(3, name, str8(": "), m) : name;
}
jstring* jrt_Throwable_toString(jobject* o) { return throwable_toString(o); }
jstring* jrt_Throwable_getMessage(jobject* o) { return ((jrt_Throwable*)o)->message; }
jstring* jrt_Throwable_getMessage_(jobject* o) { return ((const jrt_Throwable_vt*)jrt_vt(o))->getMessage(o); }
void jrt_Throwable_init(jobject* o, jstring* msg) { ((jrt_Throwable*)o)->message = msg; }
void jrt_Throwable_init_cause(jobject* o, jstring* msg, jobject* cause) { ((jrt_Throwable*)o)->message = msg; ((jrt_Throwable*)o)->cause = cause; }
const jrt_Throwable_vt jrt_Throwable_vt_ = {{jrt_Throwable_toString, jrt_Object_equals, jrt_Object_hashCode}, jrt_Throwable_getMessage};
#define EXC(N, Q, S) const jclass jrt_##N##_class = {Q, &jrt_##S##_class, sizeof(jrt_Throwable), &jrt_Throwable_vt_, 0, 0, 0};
const jclass jrt_Throwable_class = {"java.lang.Throwable", &jrt_Object_class, sizeof(jrt_Throwable), &jrt_Throwable_vt_, 0, 0, 0};
EXC(Exception, "java.lang.Exception", Throwable)
EXC(Error, "java.lang.Error", Throwable)
EXC(RuntimeException, "java.lang.RuntimeException", Exception)
EXC(IllegalArgumentException, "java.lang.IllegalArgumentException", RuntimeException)
EXC(IllegalStateException, "java.lang.IllegalStateException", RuntimeException)
EXC(ArithmeticException, "java.lang.ArithmeticException", RuntimeException)
EXC(NullPointerException, "java.lang.NullPointerException", RuntimeException)
EXC(ClassCastException, "java.lang.ClassCastException", RuntimeException)
EXC(UnsupportedOperationException, "java.lang.UnsupportedOperationException", RuntimeException)
EXC(IndexOutOfBoundsException, "java.lang.IndexOutOfBoundsException", RuntimeException)
EXC(ArrayIndexOutOfBoundsException, "java.lang.ArrayIndexOutOfBoundsException", IndexOutOfBoundsException)
EXC(StringIndexOutOfBoundsException, "java.lang.StringIndexOutOfBoundsException", IndexOutOfBoundsException)
EXC(NegativeArraySizeException, "java.lang.NegativeArraySizeException", RuntimeException)
EXC(NumberFormatException, "java.lang.NumberFormatException", IllegalArgumentException)
EXC(NoSuchElementException, "java.util.NoSuchElementException", RuntimeException)
EXC(IOException, "java.io.IOException", Exception)

void jrt_throw(jobject* t) {
	if (!t) jrt_throw_npe();
	if (!jrt_top) {
		fflush(stdout);
		jstring* s = jrt_toString(t);
		fputs("Exception in thread \"main\" ", stderr);
		jrt_print(s, true, stderr);
		exit(1);
	}
	jrt_top->ex = t;
	longjmp(jrt_top->jb, 1);
}
void jrt_throw_new(const jclass* c, const char* msg) {
	jobject* e = (jobject*)jrt_alloc(c);
	if (msg) ((jrt_Throwable*)e)->message = str8(msg);
	jrt_throw(e);
}
void jrt_throw_npe(void) { jrt_throw_new(&jrt_NullPointerException_class, 0); }
jobject* jrt_new_throwable(const jclass* c, jstring* msg, jobject* cause) {
	jrt_Throwable* e = (jrt_Throwable*)jrt_alloc(c);
	e->message = msg;
	e->cause = cause;
	if (!msg && cause) e->message = jrt_toString(cause);
	return (jobject*)e;
}
void jrt_aioobe(int32_t i, int32_t n) {
	char buf[96];
	snprintf(buf, sizeof buf, "Index %d out of bounds for length %d", i, n);
	jrt_throw_new(&jrt_ArrayIndexOutOfBoundsException_class, buf);
}

/* ------------------------------------------------------------------------------------------- strings */
static jstring* string_toString(jobject* o) { return (jstring*)o; }
static bool string_equals(jobject* a, jobject* b) { return jrt_String_equals_O((jstring*)a, b); }
static int32_t string_hashCode(jobject* o) { return jrt_String_hashCode_((jstring*)o); }
static const jobject_vt string_vt = {string_toString, string_equals, string_hashCode};
static int32_t string_compareTo(jobject* a, jobject* b) { return jrt_String_compareTo_O((jstring*)a, b); }
static const jrt_Comparable_vt string_cmp_vt = {{string_toString, string_equals, string_hashCode}, string_compareTo};
static const jitab string_itabs[] = {{&jrt_Comparable_class, &string_cmp_vt}, {&jrt_CharSequence_class, &string_vt}};
const jclass jrt_String_class = {"java.lang.String", &jrt_Object_class, sizeof(jstring), &string_vt, JRT_ITABS(string_itabs), 0};

jstring* jrt_str(const char16_t* s, int32_t n) {
	jstring* r = (jstring*)jrt_alloc(&jrt_String_class);
	r->len = n;
	r->s = (char16_t*)calloc((size_t)n + 1, sizeof(char16_t));
	if (s) memcpy(r->s, s, (size_t)n * sizeof(char16_t));
	return r;
}
typedef struct intern { jstring* s; struct intern* next; } intern;
static intern* interned[1024];
jstring* jrt_lit(jstring** slot, const char16_t* s, int32_t n) {
	if (*slot) return *slot;
	uint32_t h = 0;
	for (int32_t i = 0; i < n; ++i) h = h * 31 + s[i];
	for (intern* e = interned[h & 1023]; e; e = e->next)
		if (e->s->len == n && memcmp(e->s->s, s, (size_t)n * 2) == 0) return *slot = e->s;
	intern* e = (intern*)malloc(sizeof(intern));
	e->s = jrt_str(s, n);
	e->next = interned[h & 1023];
	interned[h & 1023] = e;
	return *slot = e->s;
}
jstring* jrt_str_I(int32_t v) { char b[16]; snprintf(b, sizeof b, "%d", v); return str8(b); }
jstring* jrt_str_J(int64_t v) { char b[32]; snprintf(b, sizeof b, "%lld", (long long)v); return str8(b); }
jstring* jrt_str_Z(bool v) { return str8(v ? "true" : "false"); }
jstring* jrt_str_C(char16_t v) { return jrt_str(&v, 1); }
jstring* jrt_str_O(jobject* o) {
	if (!o) return str8("null");
	jstring* s = jrt_toString(o);
	return s ? s : str8("null");
}
/* Java's Double.toString / Float.toString: the shortest digits that read back, laid out as Java does */
static void shortest(double d, int max, char* digits, int* exp) {
	char buf[64];
	for (int p = 1; p <= max; ++p) {
		snprintf(buf, sizeof buf, "%.*e", p - 1, d);
		double back = strtod(buf, 0);
		if (max == 9 ? (float)back == (float)d : back == d) break;
	}
	char* e = strchr(buf, 'e');
	*exp = atoi(e + 1);
	int k = 0;
	for (char* c = buf; c < e; ++c) if (*c >= '0' && *c <= '9') digits[k++] = *c;
	while (k > 1 && digits[k - 1] == '0') --k;
	digits[k] = 0;
}
static jstring* ftos(double d, int max) {
	if (d != d) return str8("NaN");
	if (isinf(d)) return str8(d > 0 ? "Infinity" : "-Infinity");
	if (d == 0) return str8(signbit(d) ? "-0.0" : "0.0");
	bool neg = d < 0;
	double a = neg ? -d : d;
	char dg[40], out[80];
	int exp;
	shortest(a, max, dg, &exp);
	int n = (int)strlen(dg);
	double lo = 1e-3, hi = 1e7;
	if (a >= lo && a < hi) {
		if (exp >= 0) {
			char ip[40], fp[40];
			int k;
			for (k = 0; k <= exp; ++k) ip[k] = k < n ? dg[k] : '0';
			ip[k] = 0;
			if (n > exp + 1) strcpy(fp, dg + exp + 1); else strcpy(fp, "0");
			snprintf(out, sizeof out, "%s%s.%s", neg ? "-" : "", ip, fp);
		} else {
			char zeros[40];
			int z = -exp - 1;
			memset(zeros, '0', z);
			zeros[z] = 0;
			snprintf(out, sizeof out, "%s0.%s%s", neg ? "-" : "", zeros, dg);
		}
	} else {
		snprintf(out, sizeof out, "%s%c.%sE%d", neg ? "-" : "", dg[0], n > 1 ? dg + 1 : "0", exp);
	}
	return str8(out);
}
jstring* jrt_str_D(double v) { return ftos(v, 17); }
jstring* jrt_str_F(float v) { return ftos((double)v, 9); }
jstring* jrt_concat(int n, ...) {
	va_list ap;
	int32_t total = 0;
	va_start(ap, n);
	for (int i = 0; i < n; ++i) total += va_arg(ap, jstring*)->len;
	va_end(ap);
	jstring* r = jrt_str(0, total);
	int32_t at = 0;
	va_start(ap, n);
	for (int i = 0; i < n; ++i) { jstring* s = va_arg(ap, jstring*); memcpy(r->s + at, s->s, (size_t)s->len * 2); at += s->len; }
	va_end(ap);
	return r;
}
int32_t jrt_String_length_(jstring* s) { return ((jstring*)jrt_nn(s))->len; }
bool jrt_String_isEmpty_(jstring* s) { return ((jstring*)jrt_nn(s))->len == 0; }
static _Noreturn void sioobe(int32_t b, int32_t e, int32_t n) {
	char buf[96];
	snprintf(buf, sizeof buf, "begin %d, end %d, length %d", b, e, n);
	jrt_throw_new(&jrt_StringIndexOutOfBoundsException_class, buf);
}
char16_t jrt_String_charAt_I(jstring* s, int32_t i) {
	jrt_nn(s);
	if (i < 0 || i >= s->len) {
		char buf[64];
		snprintf(buf, sizeof buf, "index %d, length %d", i, s->len);
		jrt_throw_new(&jrt_StringIndexOutOfBoundsException_class, buf);
	}
	return s->s[i];
}
jstring* jrt_String_substring_II(jstring* s, int32_t b, int32_t e) {
	jrt_nn(s);
	if (b < 0 || e > s->len || b > e) sioobe(b, e, s->len);
	return jrt_str(s->s + b, e - b);
}
jstring* jrt_String_substring_I(jstring* s, int32_t b) { return jrt_String_substring_II(s, b, ((jstring*)jrt_nn(s))->len); }
int32_t jrt_String_indexOf_I(jstring* s, int32_t c) {
	jrt_nn(s);
	for (int32_t i = 0; i < s->len; ++i) if (s->s[i] == c) return i;
	return -1;
}
static int32_t find(jstring* s, jstring* t, int32_t from) {
	jrt_nn(s); jrt_nn(t);
	for (int32_t i = from < 0 ? 0 : from; i + t->len <= s->len; ++i)
		if (memcmp(s->s + i, t->s, (size_t)t->len * 2) == 0) return i;
	return -1;
}
int32_t jrt_String_indexOf_Str(jstring* s, jstring* t) { return find(s, t, 0); }
bool jrt_String_contains_O(jstring* s, jobject* t) { return find(s, jrt_toString(t), 0) >= 0; }
bool jrt_String_startsWith_Str(jstring* s, jstring* t) { jrt_nn(s); jrt_nn(t); return t->len <= s->len && memcmp(s->s, t->s, (size_t)t->len * 2) == 0; }
bool jrt_String_endsWith_Str(jstring* s, jstring* t) { jrt_nn(s); jrt_nn(t); return t->len <= s->len && memcmp(s->s + s->len - t->len, t->s, (size_t)t->len * 2) == 0; }
bool jrt_String_equals_O(jstring* s, jobject* o) {
	jrt_nn(s);
	if (!o || o->cls != &jrt_String_class) return false;
	jstring* t = (jstring*)o;
	return t->len == s->len && memcmp(t->s, s->s, (size_t)s->len * 2) == 0;
}
int32_t jrt_String_hashCode_(jstring* s) {
	int32_t h = 0;
	jrt_nn(s);
	for (int32_t i = 0; i < s->len; ++i) h = (int32_t)(31u * (uint32_t)h + s->s[i]);
	return h;
}
int32_t jrt_String_compareTo_O(jstring* s, jobject* o) {
	jstring* t = (jstring*)jrt_cast((jobject*)jrt_nn(o), &jrt_String_class);
	int32_t n = s->len < t->len ? s->len : t->len;
	for (int32_t i = 0; i < n; ++i) if (s->s[i] != t->s[i]) return (int32_t)s->s[i] - (int32_t)t->s[i];
	return s->len - t->len;
}
static char16_t up(char16_t c) { return ((c >= 'a' && c <= 'z') || (c >= 0xE0 && c <= 0xFE && c != 0xF7)) ? c - 32 : c; }
static char16_t low(char16_t c) { return ((c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDE && c != 0xD7)) ? c + 32 : c; }
jstring* jrt_String_toUpperCase_(jstring* s) { jstring* r = jrt_str(((jstring*)jrt_nn(s))->s, s->len); for (int32_t i = 0; i < r->len; ++i) r->s[i] = up(r->s[i]); return r; }
jstring* jrt_String_toLowerCase_(jstring* s) { jstring* r = jrt_str(((jstring*)jrt_nn(s))->s, s->len); for (int32_t i = 0; i < r->len; ++i) r->s[i] = low(r->s[i]); return r; }
bool jrt_String_equalsIgnoreCase_Str(jstring* s, jstring* t) { return t && jrt_String_equals_O(jrt_String_toLowerCase_(s), (jobject*)jrt_String_toLowerCase_(t)); }
jstring* jrt_String_trim_(jstring* s) {
	int32_t b = 0, e = ((jstring*)jrt_nn(s))->len;
	while (b < e && s->s[b] <= ' ') ++b;
	while (e > b && s->s[e - 1] <= ' ') --e;
	return jrt_str(s->s + b, e - b);
}
jstring* jrt_String_replace_CC(jstring* s, char16_t a, char16_t b) { jstring* r = jrt_str(((jstring*)jrt_nn(s))->s, s->len); for (int32_t i = 0; i < r->len; ++i) if (r->s[i] == a) r->s[i] = b; return r; }
jstring* jrt_String_repeat_I(jstring* s, int32_t n) { jstring* r = jrt_str(0, ((jstring*)jrt_nn(s))->len * n); for (int32_t i = 0; i < n; ++i) memcpy(r->s + i * s->len, s->s, (size_t)s->len * 2); return r; }
jstring* jrt_String_toString_(jstring* s) { return (jstring*)jrt_nn(s); }
jarray* jrt_String_toCharArray_(jstring* s) { jarray* a = jrt_new_array(2, ((jstring*)jrt_nn(s))->len); memcpy(a->data, s->s, (size_t)s->len * 2); return a; }
jstring* jrt_String_new_A(jarray* a) { return jrt_str((const char16_t*)((jarray*)jrt_nn(a))->data, a->length); }
jstring* jrt_String_valueOf_A(jarray* a) { return jrt_String_new_A(a); }
jarray* jrt_String_split_Str(jstring* s, jstring* sep) {
	/* the patterns that are literal text: one character that is not a regular-expression metacharacter,
	   an escaped one, or text with none in it; any other pattern is UnsupportedOperationException */
	static const char16_t meta[] = u".$|()[{^?*+\\";
	jstring* t = (jstring*)jrt_nn(sep);
	if (t->len == 2 && t->s[0] == '\\') t = jrt_str(t->s + 1, 1);
	else for (int32_t i = 0; i < t->len; ++i)
		for (int k = 0; meta[k]; ++k) if (t->s[i] == meta[k]) jrt_throw_new(&jrt_UnsupportedOperationException_class, "String.split with a regular expression");
	jobject** parts = 0;
	int32_t np = 0, cap = 0, i = 0;
	jrt_nn(s);
	if (s->len == 0) { jarray* r = jrt_new_array(sizeof(jobject*), 1); ((jobject**)r->data)[0] = (jobject*)jrt_str(0, 0); return r; }
	for (;;) {
		int32_t p = find(s, t, i);
		if (cap == np) { cap = cap ? cap * 2 : 8; parts = (jobject**)realloc(parts, sizeof(jobject*) * cap); }
		if (p < 0) { parts[np++] = (jobject*)jrt_str(s->s + i, s->len - i); break; }
		parts[np++] = (jobject*)jrt_str(s->s + i, p - i);
		i = p + t->len;
	}
	while (np > 0 && ((jstring*)parts[np - 1])->len == 0) --np;
	return jrt_array_of(sizeof(jobject*), np, parts);
}

/* String.format: %d %s %c %b %x %f %n %%, flags - 0 + , and width/precision. %f rounds half up on the
   shortest decimal digits, as java.util.Formatter does */
static void fixed_half_up(double d, int prec, char* out) {
	if (d != d) { strcpy(out, "NaN"); return; }
	if (isinf(d)) { strcpy(out, d > 0 ? "Infinity" : "-Infinity"); return; }
	bool neg = signbit(d);
	double a = fabs(d);
	char dg[40];
	int exp = 0;
	if (a == 0) { strcpy(dg, "0"); exp = 0; } else shortest(a, 17, dg, &exp);
	int point = exp + 1, n = (int)strlen(dg), keep = point + prec;
	char kept[400];
	int k = 0;
	for (int i = 0; i < keep && k < 390; ++i) kept[k++] = i < n ? dg[i] : '0';
	kept[k] = 0;
	if (keep >= 0 && keep < n && dg[keep] >= '5') {
		int i = k - 1;
		while (i >= 0 && kept[i] == '9') { kept[i] = '0'; --i; }
		if (i >= 0) kept[i]++;
		else { memmove(kept + 1, kept, k + 1); kept[0] = '1'; ++point; ++k; }
	}
	if (keep < 0) { k = 0; kept[0] = 0; }
	char ip[400], fp[400];
	if (point <= 0) {
		strcpy(ip, "0");
		int z = -point, j = 0;
		for (; j < z && j < prec; ++j) fp[j] = '0';
		for (int i = 0; j < prec; ++j, ++i) fp[j] = i < k ? kept[i] : '0';
		fp[prec] = 0;
	} else {
		int j;
		for (j = 0; j < point; ++j) ip[j] = j < k ? kept[j] : '0';
		ip[j] = 0;
		for (j = 0; j < prec; ++j) fp[j] = point + j < k ? kept[point + j] : '0';
		fp[prec] = 0;
	}
	char* ipz = ip;
	while (ipz[0] == '0' && ipz[1]) ++ipz;
	bool zero = true;
	for (char* c = ipz; *c; ++c) if (*c != '0') zero = false;
	for (char* c = fp; *c; ++c) if (*c != '0') zero = false;
	sprintf(out, "%s%s%s%s", neg && !zero ? "-" : "", ipz, prec > 0 ? "." : "", fp);
}
static void sb_add8(jrt_StringBuilder* b, const char* s) { while (*s) jrt_StringBuilder_append_C(b, (unsigned char)*s++); }
jstring* jrt_String_format(jstring* fmt, int n, ...) {
	va_list ap;
	jobject* args[32];
	va_start(ap, n);
	for (int i = 0; i < n && i < 32; ++i) args[i] = va_arg(ap, jobject*);
	va_end(ap);
	jrt_StringBuilder* out = jrt_StringBuilder_new_();
	jstring* f = (jstring*)jrt_nn(fmt);
	int ai = 0;
	for (int32_t i = 0; i < f->len; ++i) {
		if (f->s[i] != '%') { jrt_StringBuilder_append_C(out, f->s[i]); continue; }
		++i;
		char flags[8] = {0};
		int nf = 0, width = 0, prec = -1;
		while (i < f->len && nf < 7 && strchr("-0+, #", (char)f->s[i]) && f->s[i] < 128 && f->s[i]) flags[nf++] = (char)f->s[i++];
		while (i < f->len && f->s[i] >= '0' && f->s[i] <= '9') width = width * 10 + (f->s[i++] - '0');
		if (i < f->len && f->s[i] == '.') { ++i; prec = 0; while (i < f->len && f->s[i] >= '0' && f->s[i] <= '9') prec = prec * 10 + (f->s[i++] - '0'); }
		char conv = i < f->len ? (char)f->s[i] : 's';
		if (conv == 'n') { jrt_StringBuilder_append_C(out, '\n'); continue; }
		if (conv == '%') { jrt_StringBuilder_append_C(out, '%'); continue; }
		jobject* a = ai < n ? args[ai++] : 0;
		char body[512];
		body[0] = 0;
		char lc = conv | 32;
		if (lc == 'd') {
			long long v = a && a->cls == &jrt_Long_class ? ((jrt_Long*)a)->value : (a ? jrt_unbox_I(a) : 0);
			snprintf(body, sizeof body, strchr(flags, '+') ? "%+lld" : "%lld", v);
			if (strchr(flags, ',')) {
				char g[512]; int gi = 0, start = body[0] == '-' || body[0] == '+';
				int len = (int)strlen(body);
				for (int k = 0; k < len; ++k) { if (k > start && (len - k) % 3 == 0) g[gi++] = ','; g[gi++] = body[k]; }
				g[gi] = 0; strcpy(body, g);
			}
		} else if (lc == 'x') {
			if (a && a->cls == &jrt_Long_class) snprintf(body, sizeof body, "%llx", (unsigned long long)((jrt_Long*)a)->value);
			else snprintf(body, sizeof body, "%x", (unsigned)jrt_unbox_I(a));
		} else if (lc == 'f') {
			fixed_half_up(jrt_unbox_D(a), prec < 0 ? 6 : prec, body);
		} else if (lc == 'c') {
			jstring* s = jrt_str_C(jrt_unbox_C(a));
			for (int k = 0; k < s->len; ++k) body[k] = (char)s->s[k];
			body[s->len] = 0;
		} else if (lc == 'b') {
			strcpy(body, a ? (a->cls == &jrt_Boolean_class ? (((jrt_Boolean*)a)->value ? "true" : "false") : "true") : "false");
		} else {
			jstring* s = jrt_str_O(a);
			int32_t len = s->len;
			if (prec >= 0 && len > prec) len = prec;
			jrt_StringBuilder* tmp = jrt_StringBuilder_new_();
			for (int k = 0; k < len; ++k) jrt_StringBuilder_append_C(tmp, s->s[k]);
			int pad = width - len;
			if (pad > 0 && !strchr(flags, '-')) for (int k = 0; k < pad; ++k) jrt_StringBuilder_append_C(out, ' ');
			jrt_StringBuilder_append_Str(out, jrt_StringBuilder_toString_(tmp));
			if (pad > 0 && strchr(flags, '-')) for (int k = 0; k < pad; ++k) jrt_StringBuilder_append_C(out, ' ');
			continue;
		}
		if (conv >= 'A' && conv <= 'Z') for (char* c = body; *c; ++c) if (*c >= 'a' && *c <= 'z') *c -= 32;
		int len = (int)strlen(body);
		if (len < width) {
			char padded[1024];
			int pad = width - len;
			if (strchr(flags, '-')) { strcpy(padded, body); memset(padded + len, ' ', pad); padded[width] = 0; }
			else if (strchr(flags, '0')) { int s = (body[0] == '-' || body[0] == '+'); memcpy(padded, body, s); memset(padded + s, '0', pad); strcpy(padded + s + pad, body + s); }
			else { memset(padded, ' ', pad); strcpy(padded + pad, body); }
			strcpy(body, padded);
		}
		sb_add8(out, body);
	}
	return jrt_StringBuilder_toString_(out);
}

/* ------------------------------------------------------------------------------------------- boxes */
#define BOX_VT(N, T, HASH, EQ)                                                                            \
	static jstring* N##_toString(jobject* o) { return jrt_str_##T(((jrt_##N*)o)->value); }               \
	static bool N##_equals(jobject* a, jobject* b) { return b && b->cls == a->cls && EQ(((jrt_##N*)a)->value, ((jrt_##N*)b)->value); } \
	static int32_t N##_hashCode(jobject* o) { return HASH(((jrt_##N*)o)->value); }                     \
	static int32_t N##_compareTo(jobject* a, jobject* b) { return jrt_natural(a, b); }                  \
	static const jrt_Comparable_vt N##_vt = {{N##_toString, N##_equals, N##_hashCode}, N##_compareTo}; \
	static const jitab N##_itabs[] = {{&jrt_Comparable_class, &N##_vt}};                                \
	const jclass jrt_##N##_class = {"java.lang." #N, &jrt_Object_class, sizeof(jrt_##N), &N##_vt, JRT_ITABS(N##_itabs), 0};
#define SAME(a, b) ((a) == (b))
#define DSAME(a, b) (jrt_Double_doubleToLongBits_D(a) == jrt_Double_doubleToLongBits_D(b))
#define FSAME(a, b) (jrt_Float_compare_FF(a, b) == 0)
#define IDH(v) ((int32_t)(v))
BOX_VT(Integer, I, IDH, SAME)
BOX_VT(Long, J, jrt_Long_hashCode_J, SAME)
BOX_VT(Double, D, jrt_Double_hashCode_D, DSAME)
BOX_VT(Float, F, jrt_Float_hashCode_F, FSAME)
BOX_VT(Short, I, IDH, SAME)
BOX_VT(Byte, I, IDH, SAME)
BOX_VT(Character, C, IDH, SAME)
BOX_VT(Boolean, Z, jrt_Boolean_hashCode_Z, SAME)
#define MAKE(N, v) do { jrt_##N* b = (jrt_##N*)jrt_alloc(&jrt_##N##_class); b->value = (v); return (jobject*)b; } while (0)
jobject* jrt_box_I(int32_t v) {
	static jobject* cache[256];
	if (v >= -128 && v <= 127) {
		if (!cache[v + 128]) { jrt_Integer* b = (jrt_Integer*)jrt_alloc(&jrt_Integer_class); b->value = v; cache[v + 128] = (jobject*)b; }
		return cache[v + 128];
	}
	MAKE(Integer, v);
}
jobject* jrt_box_J(int64_t v) {
	static jobject* cache[256];
	if (v >= -128 && v <= 127) {
		if (!cache[v + 128]) { jrt_Long* b = (jrt_Long*)jrt_alloc(&jrt_Long_class); b->value = v; cache[v + 128] = (jobject*)b; }
		return cache[v + 128];
	}
	MAKE(Long, v);
}
jobject* jrt_box_D(double v) { MAKE(Double, v); }
jobject* jrt_box_F(float v) { MAKE(Float, v); }
jobject* jrt_box_S(int16_t v) { MAKE(Short, v); }
jobject* jrt_box_B(int8_t v) { MAKE(Byte, v); }
jobject* jrt_box_C(char16_t v) {
	static jobject* cache[128];
	if (v < 128) {
		if (!cache[v]) { jrt_Character* b = (jrt_Character*)jrt_alloc(&jrt_Character_class); b->value = v; cache[v] = (jobject*)b; }
		return cache[v];
	}
	MAKE(Character, v);
}
jobject* jrt_box_Z(bool v) {
	static jobject *t, *f;
	if (!t) { jrt_Boolean* a = (jrt_Boolean*)jrt_alloc(&jrt_Boolean_class); a->value = true; t = (jobject*)a; a = (jrt_Boolean*)jrt_alloc(&jrt_Boolean_class); f = (jobject*)a; }
	return v ? t : f;
}
int32_t jrt_unbox_I(jobject* o) {
	jrt_nn(o);
	if (o->cls == &jrt_Character_class) return ((jrt_Character*)o)->value;
	if (o->cls == &jrt_Short_class) return ((jrt_Short*)o)->value;
	if (o->cls == &jrt_Byte_class) return ((jrt_Byte*)o)->value;
	return ((jrt_Integer*)jrt_cast(o, &jrt_Integer_class))->value;
}
int64_t jrt_unbox_J(jobject* o) { jrt_nn(o); if (o->cls != &jrt_Long_class) return jrt_unbox_I(o); return ((jrt_Long*)o)->value; }
double jrt_unbox_D(jobject* o) {
	jrt_nn(o);
	if (o->cls == &jrt_Double_class) return ((jrt_Double*)o)->value;
	if (o->cls == &jrt_Float_class) return ((jrt_Float*)o)->value;
	if (o->cls == &jrt_Long_class) return (double)((jrt_Long*)o)->value;
	return jrt_unbox_I(o);
}
float jrt_unbox_F(jobject* o) { jrt_nn(o); if (o->cls == &jrt_Float_class) return ((jrt_Float*)o)->value; return (float)jrt_unbox_D(o); }
int16_t jrt_unbox_S(jobject* o) { return ((jrt_Short*)jrt_cast((jobject*)jrt_nn(o), &jrt_Short_class))->value; }
int8_t jrt_unbox_B(jobject* o) { return ((jrt_Byte*)jrt_cast((jobject*)jrt_nn(o), &jrt_Byte_class))->value; }
char16_t jrt_unbox_C(jobject* o) { return ((jrt_Character*)jrt_cast((jobject*)jrt_nn(o), &jrt_Character_class))->value; }
bool jrt_unbox_Z(jobject* o) { return ((jrt_Boolean*)jrt_cast((jobject*)jrt_nn(o), &jrt_Boolean_class))->value; }
int32_t jrt_natural(jobject* a, jobject* b) {
	if (!a || !b) jrt_throw_npe();
	if (a->cls == &jrt_String_class) return jrt_String_compareTo_O((jstring*)a, b);
	if (a->cls == &jrt_Integer_class || a->cls == &jrt_Short_class || a->cls == &jrt_Byte_class || a->cls == &jrt_Character_class)
		return jrt_Integer_compare_II(jrt_unbox_I(a), jrt_unbox_I(b));
	if (a->cls == &jrt_Long_class) return jrt_Long_compare_JJ(jrt_unbox_J(a), jrt_unbox_J(b));
	if (a->cls == &jrt_Double_class) return jrt_Double_compare_DD(jrt_unbox_D(a), jrt_unbox_D(b));
	if (a->cls == &jrt_Float_class) return jrt_Float_compare_FF(jrt_unbox_F(a), jrt_unbox_F(b));
	if (a->cls == &jrt_Boolean_class) return (int32_t)jrt_unbox_Z(a) - (int32_t)jrt_unbox_Z(b);
	if (jrt_instanceof(a, &jrt_Enum_class)) return ((jrt_Enum*)a)->ordinal - ((jrt_Enum*)jrt_cast(b, &jrt_Enum_class))->ordinal;
	const jrt_Comparable_vt* vt = (const jrt_Comparable_vt*)jrt_ivt(a, &jrt_Comparable_class);
	return vt->compareTo(a, b);
}
static jstring* radix(uint64_t v, int shift) {
	char b[70]; int k = 68; b[69] = 0;
	if (!v) b[k--] = '0';
	while (v) { b[k--] = "0123456789abcdef"[v & ((1u << shift) - 1)]; v >>= shift; }
	return str8(b + k + 1);
}
jstring* jrt_Integer_toString_I(int32_t v) { return jrt_str_I(v); }
jstring* jrt_Integer_toHexString_I(int32_t v) { return radix((uint32_t)v, 4); }
jstring* jrt_Integer_toBinaryString_I(int32_t v) { return radix((uint32_t)v, 1); }
int32_t jrt_Integer_parseInt_Str(jstring* s) {
	jrt_nn(s);
	int32_t i = 0;
	bool neg = false;
	long long v = 0;
	if (s->len > 0 && (s->s[0] == '-' || s->s[0] == '+')) { neg = s->s[0] == '-'; i = 1; }
	if (i >= s->len) goto bad;
	for (; i < s->len; ++i) {
		if (s->s[i] < '0' || s->s[i] > '9') goto bad;
		v = v * 10 + (s->s[i] - '0');
		if (v > 2147483648LL) goto bad;
	}
	if (neg) v = -v;
	if (v > JRT_INT_MAX || v < JRT_INT_MIN) goto bad;
	return (int32_t)v;
bad: {
		jstring* m = jrt_concat(3, str8("For input string: \""), s, str8("\""));
		jobject* e = (jobject*)jrt_alloc(&jrt_NumberFormatException_class);
		((jrt_Throwable*)e)->message = m;
		jrt_throw(e);
	}
}
int32_t jrt_Integer_compare_II(int32_t a, int32_t b) { return a < b ? -1 : (a == b ? 0 : 1); }
int32_t jrt_Integer_sum_II(int32_t a, int32_t b) { return (int32_t)((uint32_t)a + (uint32_t)b); }
int32_t jrt_Integer_max_II(int32_t a, int32_t b) { return a > b ? a : b; }
int32_t jrt_Integer_min_II(int32_t a, int32_t b) { return a < b ? a : b; }
jobject* jrt_Integer_valueOf_I(int32_t v) { return jrt_box_I(v); }
int32_t jrt_Integer_bitCount_I(int32_t v) { uint32_t u = (uint32_t)v; int n = 0; while (u) { n += u & 1; u >>= 1; } return n; }
jstring* jrt_Long_toString_J(int64_t v) { return jrt_str_J(v); }
jstring* jrt_Long_toHexString_J(int64_t v) { return radix((uint64_t)v, 4); }
int32_t jrt_Long_hashCode_J(int64_t v) { return (int32_t)(v ^ (int64_t)((uint64_t)v >> 32)); }
int32_t jrt_Long_compare_JJ(int64_t a, int64_t b) { return a < b ? -1 : (a == b ? 0 : 1); }
int64_t jrt_Double_doubleToRawLongBits_D(double v) { int64_t b; memcpy(&b, &v, 8); return b; }
int64_t jrt_Double_doubleToLongBits_D(double v) { if (v != v) return 0x7ff8000000000000LL; return jrt_Double_doubleToRawLongBits_D(v); }
int32_t jrt_Double_compare_DD(double a, double b) {
	if (a < b) return -1;
	if (a > b) return 1;
	int64_t x = jrt_Double_doubleToLongBits_D(a), y = jrt_Double_doubleToLongBits_D(b);
	return x == y ? 0 : (x < y ? -1 : 1);
}
int32_t jrt_Double_hashCode_D(double v) { return jrt_Long_hashCode_J(jrt_Double_doubleToLongBits_D(v)); }
bool jrt_Double_isNaN_D(double v) { return v != v; }
jstring* jrt_Double_toString_D(double v) { return jrt_str_D(v); }
double jrt_Double_parseDouble_Str(jstring* s) {
	char buf[128]; int k = 0;
	jstring* t = jrt_String_trim_(s);
	for (int32_t i = 0; i < t->len && k < 127; ++i) buf[k++] = (char)t->s[i];
	buf[k] = 0;
	char* end;
	double d = strtod(buf, &end);
	if (!k || *end) jrt_throw_new(&jrt_NumberFormatException_class, buf);
	return d;
}
int32_t jrt_Float_floatToRawIntBits_F(float v) { int32_t b; memcpy(&b, &v, 4); return b; }
int32_t jrt_Float_compare_FF(float a, float b) {
	if (a < b) return -1;
	if (a > b) return 1;
	int32_t x = a != a ? 0x7fc00000 : jrt_Float_floatToRawIntBits_F(a), y = b != b ? 0x7fc00000 : jrt_Float_floatToRawIntBits_F(b);
	return x == y ? 0 : (x < y ? -1 : 1);
}
int32_t jrt_Float_hashCode_F(float v) { return v != v ? 0x7fc00000 : jrt_Float_floatToRawIntBits_F(v); }
int32_t jrt_Boolean_hashCode_Z(bool v) { return v ? 1231 : 1237; }
bool jrt_Character_isDigit_C(char16_t c) { return c >= '0' && c <= '9'; }
bool jrt_Character_isLetter_C(char16_t c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xFF && c != 0xD7 && c != 0xF7); }
bool jrt_Character_isLetterOrDigit_C(char16_t c) { return jrt_Character_isLetter_C(c) || jrt_Character_isDigit_C(c); }
bool jrt_Character_isWhitespace_C(char16_t c) { return c == ' ' || (c >= 9 && c <= 13) || (c >= 0x1C && c <= 0x1F); }
bool jrt_Character_isUpperCase_C(char16_t c) { return (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDE && c != 0xD7); }
bool jrt_Character_isLowerCase_C(char16_t c) { return (c >= 'a' && c <= 'z') || (c >= 0xDF && c <= 0xFF && c != 0xF7); }
char16_t jrt_Character_toUpperCase_C(char16_t c) { return up(c); }
char16_t jrt_Character_toLowerCase_C(char16_t c) { return low(c); }
int32_t jrt_Character_getNumericValue_C(char16_t c) { if (c >= '0' && c <= '9') return c - '0'; if (c >= 'a' && c <= 'z') return c - 'a' + 10; if (c >= 'A' && c <= 'Z') return c - 'A' + 10; return -1; }

/* ------------------------------------------------------------------------------------------- numbers */
int32_t jrt_div_I(int32_t a, int32_t b) { if (!b) jrt_throw_new(&jrt_ArithmeticException_class, "/ by zero"); if (b == -1) return (int32_t)(0u - (uint32_t)a); return a / b; }
int32_t jrt_rem_I(int32_t a, int32_t b) { if (!b) jrt_throw_new(&jrt_ArithmeticException_class, "/ by zero"); if (b == -1) return 0; return a % b; }
int64_t jrt_div_J(int64_t a, int64_t b) { if (!b) jrt_throw_new(&jrt_ArithmeticException_class, "/ by zero"); if (b == -1) return (int64_t)(0ull - (uint64_t)a); return a / b; }
int64_t jrt_rem_J(int64_t a, int64_t b) { if (!b) jrt_throw_new(&jrt_ArithmeticException_class, "/ by zero"); if (b == -1) return 0; return a % b; }
int32_t jrt_d2i(double d) { if (d != d) return 0; if (d >= 2147483647.0) return JRT_INT_MAX; if (d <= -2147483648.0) return JRT_INT_MIN; return (int32_t)d; }
int64_t jrt_d2l(double d) { if (d != d) return 0; if (d >= 9223372036854775807.0) return JRT_LONG_MAX; if (d <= -9223372036854775808.0) return JRT_LONG_MIN; return (int64_t)d; }
int32_t jrt_Math_abs_I(int32_t v) { return v < 0 ? (int32_t)(0u - (uint32_t)v) : v; }
int64_t jrt_Math_abs_J(int64_t v) { return v < 0 ? (int64_t)(0ull - (uint64_t)v) : v; }
double jrt_Math_abs_D(double v) { return fabs(v); }
float jrt_Math_abs_F(float v) { return fabsf(v); }
int32_t jrt_Math_max_II(int32_t a, int32_t b) { return a > b ? a : b; }
int64_t jrt_Math_max_JJ(int64_t a, int64_t b) { return a > b ? a : b; }
double jrt_Math_max_DD(double a, double b) { if (a != a) return a; if (a == 0 && b == 0) return signbit(a) ? b : a; return a > b ? a : b; }
float jrt_Math_max_FF(float a, float b) { if (a != a) return a; if (a == 0 && b == 0) return signbit(a) ? b : a; return a > b ? a : b; }
int32_t jrt_Math_min_II(int32_t a, int32_t b) { return a < b ? a : b; }
int64_t jrt_Math_min_JJ(int64_t a, int64_t b) { return a < b ? a : b; }
double jrt_Math_min_DD(double a, double b) { if (a != a) return a; if (a == 0 && b == 0) return signbit(a) ? a : b; return a < b ? a : b; }
float jrt_Math_min_FF(float a, float b) { if (a != a) return a; if (a == 0 && b == 0) return signbit(a) ? a : b; return a < b ? a : b; }
int32_t jrt_Math_floorMod_II(int32_t a, int32_t b) { int32_t m = jrt_rem_I(a, b); if (m != 0 && ((m < 0) != (b < 0))) m += b; return m; }
int32_t jrt_Math_floorDiv_II(int32_t a, int32_t b) { int32_t q = jrt_div_I(a, b); if ((a % b != 0) && ((a < 0) != (b < 0))) --q; return q; }
int64_t jrt_Math_round_D(double v) { if (v != v) return 0; return jrt_d2l(floor(v + 0.5)); }
int32_t jrt_Math_round_F(float v) { if (v != v) return 0; return jrt_d2i(floor((double)v + 0.5)); }
double jrt_Math_signum_D(double v) { return v > 0 ? 1.0 : (v < 0 ? -1.0 : v); }

/* ------------------------------------------------------------------------------------------- arrays */
const jclass jrt_Array_class = {"[array", &jrt_Object_class, sizeof(jarray), &jrt_Object_vt, 0, 0, 2};
jarray* jrt_new_array(int32_t esize, int32_t n) {
	if (n < 0) { char b[32]; snprintf(b, sizeof b, "%d", n); jrt_throw_new(&jrt_NegativeArraySizeException_class, b); }
	jarray* a = (jarray*)jrt_alloc(&jrt_Array_class);
	a->length = n;
	a->esize = esize;
	a->data = calloc((size_t)(n ? n : 1), (size_t)esize);
	return a;
}
jarray* jrt_array_of(int32_t esize, int32_t n, const void* items) {
	jarray* a = jrt_new_array(esize, n);
	if (n) memcpy(a->data, items, (size_t)n * esize);
	return a;
}
jarray* jrt_new_array2(int32_t esize, int32_t n, int32_t m) {
	jarray* a = jrt_new_array(sizeof(jarray*), n);
	for (int32_t i = 0; i < n; ++i) ((jarray**)a->data)[i] = jrt_new_array(esize, m);
	return a;
}
jarray* jrt_array_clone(jarray* a) { jrt_nn(a); return jrt_array_of(a->esize, a->length, a->data); }
static int cmp_c(const void* a, const void* b) { return (int)*(const char16_t*)a - (int)*(const char16_t*)b; }
static int cmp_i(const void* a, const void* b) { int32_t x = *(const int32_t*)a, y = *(const int32_t*)b; return x < y ? -1 : x > y; }
static int cmp_d(const void* a, const void* b) { return jrt_Double_compare_DD(*(const double*)a, *(const double*)b); }
void jrt_Arrays_sort_CA(jarray* a) { qsort(((jarray*)jrt_nn(a))->data, a->length, 2, cmp_c); }
void jrt_Arrays_sort_IA(jarray* a) { qsort(((jarray*)jrt_nn(a))->data, a->length, 4, cmp_i); }
void jrt_Arrays_sort_DA(jarray* a) { qsort(((jarray*)jrt_nn(a))->data, a->length, 8, cmp_d); }
void jrt_Arrays_fill_IA(jarray* a, int32_t v) { for (int32_t i = 0; i < JLEN(a); ++i) ((int32_t*)a->data)[i] = v; }
jstring* jrt_Arrays_toString_IA(jarray* a) {
	if (!a) return str8("null");
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '[');
	for (int32_t i = 0; i < a->length; ++i) { if (i) sb_add8(b, ", "); jrt_StringBuilder_append_I(b, ((int32_t*)a->data)[i]); }
	jrt_StringBuilder_append_C(b, ']');
	return jrt_StringBuilder_toString_(b);
}
void jrt_System_arraycopy(jobject* src, int32_t sp, jobject* dst, int32_t dp, int32_t n) {
	jarray *s = (jarray*)jrt_nn(src), *d = (jarray*)jrt_nn(dst);
	if (n < 0 || sp < 0 || dp < 0 || sp + n > s->length || dp + n > d->length) jrt_aioobe(sp + n, s->length);
	memmove((char*)d->data + (size_t)dp * d->esize, (char*)s->data + (size_t)sp * s->esize, (size_t)n * s->esize);
}

/* ------------------------------------------------------------------------------------------- StringBuilder */
static jstring* sb_toString(jobject* o) { return jrt_StringBuilder_toString_((jrt_StringBuilder*)o); }
static const jobject_vt sb_vt = {sb_toString, jrt_Object_equals, jrt_Object_hashCode};
static const jitab sb_itabs[] = {{&jrt_CharSequence_class, &sb_vt}};
const jclass jrt_StringBuilder_class = {"java.lang.StringBuilder", &jrt_Object_class, sizeof(jrt_StringBuilder), &sb_vt, JRT_ITABS(sb_itabs), 0};
jrt_StringBuilder* jrt_StringBuilder_new_(void) { return (jrt_StringBuilder*)jrt_alloc(&jrt_StringBuilder_class); }
jrt_StringBuilder* jrt_StringBuilder_new_I(int32_t cap) { (void)cap; return jrt_StringBuilder_new_(); }
jrt_StringBuilder* jrt_StringBuilder_new_Str(jstring* s) { return jrt_StringBuilder_append_Str(jrt_StringBuilder_new_(), (jstring*)jrt_nn(s)); }
static void sb_grow(jrt_StringBuilder* b, int32_t extra) {
	if (b->len + extra <= b->cap) return;
	int32_t c = b->cap ? b->cap * 2 : 16;
	while (c < b->len + extra) c *= 2;
	b->s = (char16_t*)realloc(b->s, (size_t)c * 2);
	b->cap = c;
}
jrt_StringBuilder* jrt_StringBuilder_append_Str(jrt_StringBuilder* b, jstring* s) {
	jrt_nn(b);
	if (!s) s = str8("null");
	sb_grow(b, s->len);
	memcpy(b->s + b->len, s->s, (size_t)s->len * 2);
	b->len += s->len;
	return b;
}
jrt_StringBuilder* jrt_StringBuilder_append_C(jrt_StringBuilder* b, char16_t c) { jrt_nn(b); sb_grow(b, 1); b->s[b->len++] = c; return b; }
jrt_StringBuilder* jrt_StringBuilder_append_I(jrt_StringBuilder* b, int32_t v) { return jrt_StringBuilder_append_Str(b, jrt_str_I(v)); }
jrt_StringBuilder* jrt_StringBuilder_append_J(jrt_StringBuilder* b, int64_t v) { return jrt_StringBuilder_append_Str(b, jrt_str_J(v)); }
jrt_StringBuilder* jrt_StringBuilder_append_D(jrt_StringBuilder* b, double v) { return jrt_StringBuilder_append_Str(b, jrt_str_D(v)); }
jrt_StringBuilder* jrt_StringBuilder_append_F(jrt_StringBuilder* b, float v) { return jrt_StringBuilder_append_Str(b, jrt_str_F(v)); }
jrt_StringBuilder* jrt_StringBuilder_append_Z(jrt_StringBuilder* b, bool v) { return jrt_StringBuilder_append_Str(b, jrt_str_Z(v)); }
jrt_StringBuilder* jrt_StringBuilder_append_O(jrt_StringBuilder* b, jobject* o) { return jrt_StringBuilder_append_Str(b, jrt_str_O(o)); }
jrt_StringBuilder* jrt_StringBuilder_reverse_(jrt_StringBuilder* b) {
	jrt_nn(b);
	for (int32_t i = 0, j = b->len - 1; i < j; ++i, --j) { char16_t t = b->s[i]; b->s[i] = b->s[j]; b->s[j] = t; }
	return b;
}
jrt_StringBuilder* jrt_StringBuilder_insert_IStr(jrt_StringBuilder* b, int32_t at, jstring* s) {
	jrt_nn(b);
	if (!s) s = str8("null");
	sb_grow(b, s->len);
	memmove(b->s + at + s->len, b->s + at, (size_t)(b->len - at) * 2);
	memcpy(b->s + at, s->s, (size_t)s->len * 2);
	b->len += s->len;
	return b;
}
jrt_StringBuilder* jrt_StringBuilder_deleteCharAt_I(jrt_StringBuilder* b, int32_t i) {
	jrt_nn(b);
	if (i < 0 || i >= b->len) jrt_throw_new(&jrt_StringIndexOutOfBoundsException_class, 0);
	memmove(b->s + i, b->s + i + 1, (size_t)(b->len - i - 1) * 2);
	--b->len;
	return b;
}
int32_t jrt_StringBuilder_length_(jrt_StringBuilder* b) { return ((jrt_StringBuilder*)jrt_nn(b))->len; }
char16_t jrt_StringBuilder_charAt_I(jrt_StringBuilder* b, int32_t i) { jrt_nn(b); if (i < 0 || i >= b->len) jrt_throw_new(&jrt_StringIndexOutOfBoundsException_class, 0); return b->s[i]; }
void jrt_StringBuilder_setCharAt_IC(jrt_StringBuilder* b, int32_t i, char16_t c) { jrt_nn(b); if (i < 0 || i >= b->len) jrt_throw_new(&jrt_StringIndexOutOfBoundsException_class, 0); b->s[i] = c; }
void jrt_StringBuilder_setLength_I(jrt_StringBuilder* b, int32_t n) { jrt_nn(b); sb_grow(b, n > b->len ? n - b->len : 0); for (int32_t i = b->len; i < n; ++i) b->s[i] = 0; b->len = n; }
jstring* jrt_StringBuilder_toString_(jrt_StringBuilder* b) { jrt_nn(b); return jrt_str(b->s, b->len); }

/* ------------------------------------------------------------------------------------------- output */
void jrt_print(jstring* s, bool nl, FILE* f) {
	if (s) {
		for (int32_t i = 0; i < s->len; ++i) {
			uint32_t c = s->s[i];
			if (c >= 0xD800 && c < 0xDC00 && i + 1 < s->len) { c = 0x10000 + ((c - 0xD800) << 10) + (s->s[i + 1] - 0xDC00); ++i; }
			if (c < 0x80) fputc((int)c, f);
			else if (c < 0x800) { fputc(0xC0 | (c >> 6), f); fputc(0x80 | (c & 63), f); }
			else if (c < 0x10000) { fputc(0xE0 | (c >> 12), f); fputc(0x80 | ((c >> 6) & 63), f); fputc(0x80 | (c & 63), f); }
			else { fputc(0xF0 | (c >> 18), f); fputc(0x80 | ((c >> 12) & 63), f); fputc(0x80 | ((c >> 6) & 63), f); fputc(0x80 | (c & 63), f); }
		}
	}
	if (nl) fputc('\n', f);
}

/* ------------------------------------------------------------------------------------------- interfaces */
#define IFACE(N, Q) const jclass jrt_##N##_class = {Q, 0, 0, 0, 0, 0, 1};
IFACE(Iterable, "java.lang.Iterable") IFACE(Iterator, "java.util.Iterator") IFACE(Comparable, "java.lang.Comparable")
IFACE(Comparator, "java.util.Comparator") IFACE(Runnable, "java.lang.Runnable") IFACE(Function, "java.util.function.Function")
IFACE(BiFunction, "java.util.function.BiFunction") IFACE(UnaryOperator, "java.util.function.UnaryOperator")
IFACE(BinaryOperator, "java.util.function.BinaryOperator") IFACE(Supplier, "java.util.function.Supplier")
IFACE(Consumer, "java.util.function.Consumer") IFACE(BiConsumer, "java.util.function.BiConsumer")
IFACE(Predicate, "java.util.function.Predicate") IFACE(IntBinaryOperator, "java.util.function.IntBinaryOperator")
IFACE(IntUnaryOperator, "java.util.function.IntUnaryOperator") IFACE(IntPredicate, "java.util.function.IntPredicate")
IFACE(IntFunction, "java.util.function.IntFunction") IFACE(ToIntFunction, "java.util.function.ToIntFunction")
IFACE(IntConsumer, "java.util.function.IntConsumer") IFACE(Collection, "java.util.Collection") IFACE(List, "java.util.List")
IFACE(Set, "java.util.Set") IFACE(Map, "java.util.Map") IFACE(CharSequence, "java.lang.CharSequence")

/* ------------------------------------------------------------------------------------------- collections */
/* an iterator over a snapshot of the elements; remove() goes back to the collection */
typedef struct jrt_Iter { jobject hdr; jobject* src; jobject** items; int32_t n, i; jobject* last; bool has; } jrt_Iter;
static bool iter_hasNext(jobject* o) { jrt_Iter* it = (jrt_Iter*)o; return it->i < it->n; }
static jobject* iter_next(jobject* o) {
	jrt_Iter* it = (jrt_Iter*)o;
	if (it->i >= it->n) jrt_throw_new(&jrt_NoSuchElementException_class, 0);
	it->has = true;
	return it->last = it->items[it->i++];
}
static const jrt_Iterator_vt iter_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, iter_hasNext, iter_next};
static const jitab iter_itabs[] = {{&jrt_Iterator_class, &iter_vt}};
static const jclass iter_class = {"java.util.Iterator", &jrt_Object_class, sizeof(jrt_Iter), &iter_vt, JRT_ITABS(iter_itabs), 0};

static jstring* coll_toString(jobject* o);
static bool list_equals(jobject* a, jobject* b);
static int32_t list_hashCode(jobject* o);
static jobject* coll_iterator(jobject* o) { return jrt_Collection_iterator_(o); }
static const jrt_Iterable_vt list_vt = {{coll_toString, list_equals, list_hashCode}, coll_iterator};
static const jitab list_itabs[] = {{&jrt_Iterable_class, &list_vt}, {&jrt_Collection_class, &list_vt}, {&jrt_List_class, &list_vt}};
const jclass jrt_ArrayList_class = {"java.util.ArrayList", &jrt_Object_class, sizeof(jrt_ArrayList), &list_vt, JRT_ITABS(list_itabs), 0};

jobject* jrt_ArrayList_new_(void) { return (jobject*)jrt_alloc(&jrt_ArrayList_class); }
jobject* jrt_ArrayList_new_I(int32_t n) { (void)n; return jrt_ArrayList_new_(); }
static void al_add(jrt_ArrayList* l, int32_t at, jobject* x) {
	if (l->size == l->cap) { l->cap = l->cap ? l->cap * 2 : 10; l->items = (jobject**)realloc(l->items, sizeof(jobject*) * l->cap); }
	memmove(l->items + at + 1, l->items + at, sizeof(jobject*) * (l->size - at));
	l->items[at] = x;
	++l->size;
}
static jrt_ArrayList* as_list(jobject* o) { return (jrt_ArrayList*)jrt_cast((jobject*)jrt_nn(o), &jrt_ArrayList_class); }
static void immutable(jrt_ArrayList* l) { if (l->immutable) jrt_throw_new(&jrt_UnsupportedOperationException_class, 0); }
static void ioobe(int32_t i, int32_t n) {
	char b[96];
	snprintf(b, sizeof b, "Index %d out of bounds for length %d", i, n);
	jrt_throw_new(&jrt_IndexOutOfBoundsException_class, b);
}
jobject* jrt_ArrayList_new_O(jobject* c) {
	jobject* l = jrt_ArrayList_new_();
	jobject* it = jrt_Collection_iterator_(c);
	while (jrt_Iterator_hasNext_(it)) jrt_Collection_add_O(l, jrt_Iterator_next_(it));
	return l;
}
jobject* jrt_List_of(int n, ...) {
	jrt_ArrayList* l = (jrt_ArrayList*)jrt_ArrayList_new_();
	va_list ap;
	va_start(ap, n);
	for (int i = 0; i < n; ++i) { jobject* x = va_arg(ap, jobject*); if (!x) jrt_throw_npe(); al_add(l, l->size, x); }
	va_end(ap);
	l->immutable = true;
	return (jobject*)l;
}
static jrt_HashMap* as_map(jobject* o) { return (jrt_HashMap*)jrt_cast((jobject*)jrt_nn(o), &jrt_HashMap_class); }
static void map_entries(jrt_HashMap* m, jobject*** out, int32_t* n);
bool jrt_Collection_add_O(jobject* c, jobject* x) {
	jrt_nn(c);
	if (c->cls == &jrt_ArrayList_class) { jrt_ArrayList* l = (jrt_ArrayList*)c; immutable(l); al_add(l, l->size, x); return true; }
	if (c->cls == &jrt_MapSet_class) {
		jrt_MapSet* s = (jrt_MapSet*)c;
		if (s->kind != 0) jrt_throw_new(&jrt_UnsupportedOperationException_class, 0);
		if (jrt_Map_containsKey_O((jobject*)s->m, x)) return false;
		jrt_Map_put_OO((jobject*)s->m, x, jrt_box_Z(true));
		return true;
	}
	jrt_throw_new(&jrt_UnsupportedOperationException_class, "add");
}
int32_t jrt_Collection_size_(jobject* c) {
	jrt_nn(c);
	if (c->cls == &jrt_ArrayList_class) return ((jrt_ArrayList*)c)->size;
	if (c->cls == &jrt_MapSet_class) return ((jrt_MapSet*)c)->m->count;
	jrt_throw_new(&jrt_UnsupportedOperationException_class, "size");
}
bool jrt_Collection_isEmpty_(jobject* c) { return jrt_Collection_size_(c) == 0; }
jobject* jrt_Collection_iterator_(jobject* c) {
	jrt_nn(c);
	jrt_Iter* it = (jrt_Iter*)jrt_alloc(&iter_class);
	it->src = c;
	if (c->cls == &jrt_ArrayList_class) {
		jrt_ArrayList* l = (jrt_ArrayList*)c;
		it->n = l->size;
		it->items = (jobject**)malloc(sizeof(jobject*) * (l->size ? l->size : 1));
		memcpy(it->items, l->items, sizeof(jobject*) * l->size);
		return (jobject*)it;
	}
	if (c->cls == &jrt_MapSet_class) {
		jrt_MapSet* s = (jrt_MapSet*)c;
		jobject** es;
		int32_t n;
		map_entries(s->m, &es, &n);
		for (int32_t i = 0; i < n; ++i) if (s->kind != 2) es[i] = s->kind == 0 ? ((jrt_node*)es[i])->key : ((jrt_node*)es[i])->value;
		it->items = es;
		it->n = n;
		return (jobject*)it;
	}
	return jrt_Iterable_iterator_(c);
}
bool jrt_Collection_contains_O(jobject* c, jobject* x) {
	jrt_nn(c);
	if (c->cls == &jrt_MapSet_class && ((jrt_MapSet*)c)->kind == 0) return jrt_Map_containsKey_O((jobject*)((jrt_MapSet*)c)->m, x);
	jobject* it = jrt_Collection_iterator_(c);
	while (jrt_Iterator_hasNext_(it)) if (jrt_Objects_equals_OO(x, jrt_Iterator_next_(it))) return true;
	return false;
}
bool jrt_Collection_remove_O(jobject* c, jobject* x) {
	jrt_nn(c);
	if (c->cls == &jrt_ArrayList_class) { int32_t i = jrt_List_indexOf_O(c, x); if (i < 0) return false; jrt_List_remove_I(c, i); return true; }
	if (c->cls == &jrt_MapSet_class) { jrt_MapSet* s = (jrt_MapSet*)c; bool had = jrt_Map_containsKey_O((jobject*)s->m, x); jrt_Map_remove_O((jobject*)s->m, x); return had; }
	jrt_throw_new(&jrt_UnsupportedOperationException_class, "remove");
}
void jrt_Collection_clear_(jobject* c) {
	jrt_nn(c);
	if (c->cls == &jrt_ArrayList_class) { immutable((jrt_ArrayList*)c); ((jrt_ArrayList*)c)->size = 0; return; }
	if (c->cls == &jrt_MapSet_class) { jrt_HashMap* m = ((jrt_MapSet*)c)->m; memset(m->table, 0, sizeof(jrt_node*) * m->cap); m->count = 0; m->head = m->tail = 0; return; }
}
jobject* jrt_List_get_I(jobject* o, int32_t i) { jrt_ArrayList* l = as_list(o); if (i < 0 || i >= l->size) ioobe(i, l->size); return l->items[i]; }
jobject* jrt_List_set_IO(jobject* o, int32_t i, jobject* x) { jrt_ArrayList* l = as_list(o); immutable(l); if (i < 0 || i >= l->size) ioobe(i, l->size); jobject* old = l->items[i]; l->items[i] = x; return old; }
void jrt_List_add_IO(jobject* o, int32_t i, jobject* x) { jrt_ArrayList* l = as_list(o); immutable(l); if (i < 0 || i > l->size) ioobe(i, l->size); al_add(l, i, x); }
jobject* jrt_List_remove_I(jobject* o, int32_t i) {
	jrt_ArrayList* l = as_list(o);
	immutable(l);
	if (i < 0 || i >= l->size) ioobe(i, l->size);
	jobject* old = l->items[i];
	memmove(l->items + i, l->items + i + 1, sizeof(jobject*) * (l->size - i - 1));
	--l->size;
	return old;
}
int32_t jrt_List_indexOf_O(jobject* o, jobject* x) { jrt_ArrayList* l = as_list(o); for (int32_t i = 0; i < l->size; ++i) if (jrt_Objects_equals_OO(x, l->items[i])) return i; return -1; }
static jobject* sort_cmp;
static void merge_sort(jobject** a, jobject** tmp, int32_t n) {      /* stable, as Java's sort is */
	if (n < 2) return;
	int32_t h = n / 2;
	merge_sort(a, tmp, h);
	merge_sort(a + h, tmp, n - h);
	int32_t i = 0, j = h, k = 0;
	while (i < h && j < n) {
		int32_t c = sort_cmp ? jrt_Comparator_compare_OO(sort_cmp, a[j], a[i]) : jrt_natural(a[j], a[i]);
		tmp[k++] = c < 0 ? a[j++] : a[i++];
	}
	while (i < h) tmp[k++] = a[i++];
	while (j < n) tmp[k++] = a[j++];
	memcpy(a, tmp, sizeof(jobject*) * n);
}
void jrt_List_sort_O(jobject* o, jobject* cmp) {
	jrt_ArrayList* l = as_list(o);
	immutable(l);
	jobject* saved = sort_cmp;
	sort_cmp = cmp;
	jobject** tmp = (jobject**)malloc(sizeof(jobject*) * (l->size ? l->size : 1));
	merge_sort(l->items, tmp, l->size);
	free(tmp);
	sort_cmp = saved;
}
void jrt_Collections_sort_O(jobject* l) { jrt_List_sort_O(l, 0); }
void jrt_Collections_sort_OO(jobject* l, jobject* cmp) { jrt_List_sort_O(l, cmp); }
void jrt_Collections_reverse_O(jobject* o) { jrt_ArrayList* l = as_list(o); for (int32_t i = 0, j = l->size - 1; i < j; ++i, --j) { jobject* t = l->items[i]; l->items[i] = l->items[j]; l->items[j] = t; } }
static jstring* coll_toString(jobject* o) {
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '[');
	jobject* it = jrt_Collection_iterator_(o);
	bool first = true;
	while (jrt_Iterator_hasNext_(it)) {
		if (!first) sb_add8(b, ", ");
		first = false;
		jobject* x = jrt_Iterator_next_(it);
		jrt_StringBuilder_append_Str(b, x == o ? str8("(this Collection)") : jrt_str_O(x));
	}
	jrt_StringBuilder_append_C(b, ']');
	return jrt_StringBuilder_toString_(b);
}
static bool list_equals(jobject* a, jobject* b) {
	if (!b || b->cls != &jrt_ArrayList_class) return false;
	jrt_ArrayList *x = (jrt_ArrayList*)a, *y = (jrt_ArrayList*)b;
	if (x->size != y->size) return false;
	for (int32_t i = 0; i < x->size; ++i) if (!jrt_Objects_equals_OO(x->items[i], y->items[i])) return false;
	return true;
}
static int32_t list_hashCode(jobject* o) {
	jrt_ArrayList* l = (jrt_ArrayList*)o;
	int32_t h = 1;
	for (int32_t i = 0; i < l->size; ++i) h = (int32_t)(31u * (uint32_t)h + (uint32_t)jrt_Objects_hashCode_O(l->items[i]));
	return h;
}

/* HashMap: Java's table -- the hash spreading, the bin order, the resizing -- so iteration order is Java's.
   kind 1 (LinkedHashMap) iterates in insertion order, kind 2 (TreeMap) in key order. */
static jstring* map_toString(jobject* o);
static bool map_equals(jobject* a, jobject* b) { return a == b; }
static const jobject_vt map_vt = {map_toString, map_equals, jrt_Object_hashCode};
static const jitab map_itabs[] = {{&jrt_Map_class, &map_vt}};
const jclass jrt_HashMap_class = {"java.util.HashMap", &jrt_Object_class, sizeof(jrt_HashMap), &map_vt, JRT_ITABS(map_itabs), 0};
static jstring* node_toString(jobject* o) { jrt_node* e = (jrt_node*)o; return jrt_concat(3, jrt_str_O(e->key), str8("="), jrt_str_O(e->value)); }
static const jobject_vt node_vt = {node_toString, jrt_Object_equals, jrt_Object_hashCode};
const jclass jrt_node_class = {"java.util.HashMap.Node", &jrt_Object_class, sizeof(jrt_node), &node_vt, 0, 0, 0};
static const jrt_Iterable_vt set_vt = {{coll_toString, jrt_Object_equals, jrt_Object_hashCode}, coll_iterator};
static const jitab set_itabs[] = {{&jrt_Iterable_class, &set_vt}, {&jrt_Collection_class, &set_vt}, {&jrt_Set_class, &set_vt}};
const jclass jrt_MapSet_class = {"java.util.HashSet", &jrt_Object_class, sizeof(jrt_MapSet), &set_vt, JRT_ITABS(set_itabs), 0};

static jrt_HashMap* new_map(int kind) { jrt_HashMap* m = (jrt_HashMap*)jrt_alloc(&jrt_HashMap_class); m->kind = kind; return m; }
jobject* jrt_HashMap_new_(void) { return (jobject*)new_map(0); }
jobject* jrt_LinkedHashMap_new_(void) { return (jobject*)new_map(1); }
jobject* jrt_TreeMap_new_(void) { return (jobject*)new_map(2); }
jobject* jrt_TreeMap_new_O(jobject* cmp) { jrt_HashMap* m = new_map(2); m->cmp = cmp; return (jobject*)m; }
static int32_t spread(jobject* k) { if (!k) return 0; int32_t h = jrt_hashCode(k); return h ^ (int32_t)((uint32_t)h >> 16); }
static int32_t key_cmp(jrt_HashMap* m, jobject* a, jobject* b) { return m->cmp ? jrt_Comparator_compare_OO(m->cmp, a, b) : jrt_natural(a, b); }
static jrt_node* map_find(jrt_HashMap* m, jobject* k) {
	if (m->kind == 2) {
		if (!k && !m->cmp) jrt_throw_npe();
		for (jrt_node* e = m->head; e; e = e->after) { int32_t c = key_cmp(m, e->key, k); if (c == 0) return e; if (c > 0) break; }
		return 0;
	}
	if (!m->cap) return 0;
	int32_t h = spread(k);
	for (jrt_node* e = m->table[h & (m->cap - 1)]; e; e = e->next)
		if (e->hash == h && (e->key == k || (k && jrt_equals(k, e->key)))) return e;
	return 0;
}
static void resize(jrt_HashMap* m) {
	int32_t old = m->cap, n = old ? old * 2 : 16;
	jrt_node** t = (jrt_node**)calloc((size_t)n, sizeof(jrt_node*));
	for (int32_t i = 0; i < old; ++i) {                 /* each bin split in two, its order kept, as Java does */
		jrt_node *lo = 0, *loT = 0, *hi = 0, *hiT = 0;
		for (jrt_node* e = m->table[i]; e;) {
			jrt_node* nx = e->next;
			e->next = 0;
			if ((e->hash & old) == 0) { if (loT) loT->next = e; else lo = e; loT = e; }
			else { if (hiT) hiT->next = e; else hi = e; hiT = e; }
			e = nx;
		}
		t[i] = lo;
		t[i + old] = hi;
	}
	free(m->table);
	m->table = t;
	m->cap = n;
}
static jrt_node* map_insert(jrt_HashMap* m, jobject* k, jobject* v, bool head) {
	jrt_node* n = (jrt_node*)jrt_alloc(&jrt_node_class);
	n->key = k;
	n->value = v;
	if (m->kind == 2) {                                  /* sorted list */
		jrt_node** at = &m->head;
		jrt_node* prev = 0;
		while (*at && key_cmp(m, (*at)->key, k) < 0) { prev = *at; at = &(*at)->after; }
		n->after = *at;
		n->before = prev;
		if (*at) (*at)->before = n; else m->tail = n;
		*at = n;
		++m->count;
		return n;
	}
	n->hash = spread(k);
	if (head) { if (!m->cap || m->count > m->cap * 3 / 4) resize(m); }   /* merge: resize first, then the head */
	else if (!m->cap) resize(m);
	int32_t i = n->hash & (m->cap - 1);
	if (head || !m->table[i]) { n->next = m->table[i]; m->table[i] = n; }
	else { jrt_node* t = m->table[i]; while (t->next) t = t->next; t->next = n; }
	n->before = m->tail;
	if (m->tail) m->tail->after = n; else m->head = n;
	m->tail = n;
	++m->count;
	if (!head && m->count > m->cap * 3 / 4) resize(m);
	return n;
}
jobject* jrt_Map_get_O(jobject* o, jobject* k) { jrt_node* e = map_find(as_map(o), k); return e ? e->value : 0; }
bool jrt_Map_containsKey_O(jobject* o, jobject* k) { return map_find(as_map(o), k) != 0; }
jobject* jrt_Map_put_OO(jobject* o, jobject* k, jobject* v) {
	jrt_HashMap* m = as_map(o);
	jrt_node* e = map_find(m, k);
	if (e) { jobject* old = e->value; e->value = v; return old; }
	map_insert(m, k, v, false);
	return 0;
}
jobject* jrt_Map_remove_O(jobject* o, jobject* k) {
	jrt_HashMap* m = as_map(o);
	jrt_node* e = map_find(m, k);
	if (!e) return 0;
	if (m->kind != 2) {
		jrt_node** p = &m->table[e->hash & (m->cap - 1)];
		while (*p != e) p = &(*p)->next;
		*p = e->next;
	}
	if (e->before) e->before->after = e->after; else m->head = e->after;
	if (e->after) e->after->before = e->before; else m->tail = e->before;
	--m->count;
	return e->value;
}
int32_t jrt_Map_size_(jobject* o) { return as_map(o)->count; }
bool jrt_Map_isEmpty_(jobject* o) { return as_map(o)->count == 0; }
jobject* jrt_Map_getOrDefault_OO(jobject* o, jobject* k, jobject* d) { jrt_node* e = map_find(as_map(o), k); return e ? e->value : d; }
jobject* jrt_Map_merge_OOO(jobject* o, jobject* k, jobject* v, jobject* f) {
	jrt_HashMap* m = as_map(o);
	jrt_node* e = map_find(m, k);
	if (e && e->value) {
		jobject* n = jrt_BiFunction_apply_OO(f, e->value, v);
		if (n) e->value = n; else jrt_Map_remove_O(o, k);
		return n;
	}
	if (e) { e->value = v; return v; }
	map_insert(m, k, v, true);
	return v;
}
static void map_entries(jrt_HashMap* m, jobject*** out, int32_t* n) {
	jobject** es = (jobject**)malloc(sizeof(jobject*) * (m->count ? m->count : 1));
	int32_t k = 0;
	if (m->kind == 0) {
		for (int32_t i = 0; i < m->cap; ++i) for (jrt_node* e = m->table[i]; e; e = e->next) es[k++] = (jobject*)e;
	} else {
		for (jrt_node* e = m->head; e; e = e->after) es[k++] = (jobject*)e;
	}
	*out = es;
	*n = k;
}
static jobject* view(jobject* o, int kind) { jrt_MapSet* s = (jrt_MapSet*)jrt_alloc(&jrt_MapSet_class); s->m = as_map(o); s->kind = kind; return (jobject*)s; }
jobject* jrt_Map_keySet_(jobject* m) { return view(m, 0); }
jobject* jrt_Map_values_(jobject* m) { return view(m, 1); }
jobject* jrt_Map_entrySet_(jobject* m) { return view(m, 2); }
jobject* jrt_MapEntry_getKey_(jobject* e) { return ((jrt_node*)jrt_cast((jobject*)jrt_nn(e), &jrt_node_class))->key; }
jobject* jrt_MapEntry_getValue_(jobject* e) { return ((jrt_node*)jrt_cast((jobject*)jrt_nn(e), &jrt_node_class))->value; }
static jstring* map_toString(jobject* o) {
	jrt_HashMap* m = (jrt_HashMap*)o;
	jobject** es;
	int32_t n;
	map_entries(m, &es, &n);
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '{');
	for (int32_t i = 0; i < n; ++i) { if (i) sb_add8(b, ", "); jrt_StringBuilder_append_Str(b, node_toString(es[i])); }
	jrt_StringBuilder_append_C(b, '}');
	return jrt_StringBuilder_toString_(b);
}
jobject* jrt_HashSet_new_(void) { return view(jrt_HashMap_new_(), 0); }
jobject* jrt_LinkedHashSet_new_(void) { return view(jrt_LinkedHashMap_new_(), 0); }
jobject* jrt_TreeSet_new_(void) { return view(jrt_TreeMap_new_(), 0); }
jstring* jrt_String_join(jstring* sep, jobject* parts) {
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jobject* it = jrt_Collection_iterator_(parts);
	bool first = true;
	while (jrt_Iterator_hasNext_(it)) { if (!first) jrt_StringBuilder_append_Str(b, sep); first = false; jrt_StringBuilder_append_Str(b, jrt_str_O(jrt_Iterator_next_(it))); }
	return jrt_StringBuilder_toString_(b);
}

/* ------------------------------------------------------------------------------------------- enums */
jstring* jrt_Enum_toString(jobject* o) { return ((jrt_Enum*)o)->name; }
static const jobject_vt enum_vt = {jrt_Enum_toString, jrt_Object_equals, jrt_Object_hashCode};
const jclass jrt_Enum_class = {"java.lang.Enum", &jrt_Object_class, sizeof(jrt_Enum), &enum_vt, 0, 0, 0};

int jrt_run_main(void (*m)(jarray*), int argc, char** argv) {
	jarray* args = jrt_new_array(sizeof(jobject*), argc > 1 ? argc - 1 : 0);
	for (int i = 1; i < argc; ++i) ((jobject**)args->data)[i - 1] = (jobject*)str8(argv[i]);
	m(args);
	fflush(stdout);
	return 0;
}
