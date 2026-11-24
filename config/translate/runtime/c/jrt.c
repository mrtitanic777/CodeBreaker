/* jrt.c -- the Java library in C: the implementation of jrt.h (see there for the object model). */
#include "jrt.h"
#include <stdarg.h>
#include <time.h>
#ifdef _WIN32
#include <sys/timeb.h>
#endif

/* ---------------------------------------------------------------------------------------- the collector
 * Mark and sweep, and it never moves anything. Every block this file allocates has a header and is listed. A collection starts when
 * what was allocated since the last one passes the limit (32 MB, or what was live then, whichever is more), so a short program
 * never has one. The roots are the registers, the stack and the program's writable static data; a word that points into a block keeps
 * it (interior pointers too), and a kept block's words are read the same way, except text, which holds no pointers. Reading
 * conservatively can only keep something too long, never free what is in use. Where the static data cannot be found (not Windows or
 * Linux), nothing is collected: the program runs as it would with no collector. JRT_GC_STRESS=N collects every N allocations.
 */
typedef struct gc_block { size_t size; uint32_t atomic, marked; } gc_block;   /* 16 bytes: what follows is aligned for anything */
enum { GC_MIN_LIMIT = 32 * 1024 * 1024 };
static gc_block** gc_all;
static size_t gc_count, gc_cap, gc_since, gc_live, gc_limit = GC_MIN_LIMIT;
static gc_block** gc_marks;
static size_t gc_mark_count, gc_mark_cap;
static const char* gc_stack_base;
static int gc_busy;
static long gc_stress = -1, gc_until;

static void gc_oom(void) { fprintf(stderr, "out of memory\n"); exit(3); }

#if defined(__GNUC__) || defined(__clang__)
#define GC_READS_ANYTHING __attribute__((no_sanitize("address"))) __attribute__((noinline))
#else
#define GC_READS_ANYTHING
#endif

static gc_block* gc_find(uintptr_t p) {
	size_t lo = 0, hi = gc_count;
	while (lo < hi) {
		size_t mid = lo + (hi - lo) / 2;
		if ((uintptr_t)gc_all[mid] <= p) lo = mid + 1; else hi = mid;
	}
	if (lo == 0) return 0;
	gc_block* b = gc_all[lo - 1];
	const uintptr_t start = (uintptr_t)(b + 1);
	return p >= start && p <= start + b->size ? b : 0;   /* (one past the end counts: a loop's end pointer) */
}

static void gc_keep(uintptr_t word) {
	gc_block* b = gc_find(word);
	if (!b || b->marked) return;
	b->marked = 1;
	if (b->atomic) return;
	if (gc_mark_count == gc_mark_cap) {
		gc_mark_cap = gc_mark_cap ? gc_mark_cap * 2 : 1024;
		gc_marks = (gc_block**)realloc(gc_marks, gc_mark_cap * sizeof(gc_block*));
		if (!gc_marks) gc_oom();
	}
	gc_marks[gc_mark_count++] = b;
}

GC_READS_ANYTHING static void gc_scan(const char* low, const char* high) {
	const size_t w = sizeof(uintptr_t);
	low += (w - (uintptr_t)low % w) % w;
	for (; low + w <= high; low += w) gc_keep(*(const volatile uintptr_t*)low);
}

#if defined(_WIN32)
extern char __ImageBase;
static int gc_scan_static(void) {   /* the writable sections of this program's image */
	const unsigned char* image = (const unsigned char*)&__ImageBase;
	const uint32_t pe = *(const uint32_t*)(image + 0x3C);
	const unsigned sections = *(const uint16_t*)(image + pe + 6), optional = *(const uint16_t*)(image + pe + 20);
	const unsigned char* s = image + pe + 24 + optional;
	for (unsigned i = 0; i < sections; ++i, s += 40) {
		if (!(*(const uint32_t*)(s + 36) & 0x80000000u)) continue;
		const char* at = (const char*)image + *(const uint32_t*)(s + 12);
		gc_scan(at, at + *(const uint32_t*)(s + 8));
	}
	return 1;
}
static int gc_can_collect(void) { return 1; }
#elif defined(__linux__)
extern char __data_start[], _end[];
static int gc_scan_static(void) { gc_scan(__data_start, _end); return 1; }
static int gc_can_collect(void) { return 1; }
#else
static int gc_scan_static(void) { return 0; }
static int gc_can_collect(void) { return 0; }
#endif

static int gc_order(const void* a, const void* b) {
	const uintptr_t x = (uintptr_t)*(gc_block* const*)a, y = (uintptr_t)*(gc_block* const*)b;
	return x < y ? -1 : x > y;
}

GC_READS_ANYTHING static void gc_collect(void) {
	if (gc_busy || !gc_stack_base || !gc_can_collect()) return;
	gc_busy = 1;
	qsort(gc_all, gc_count, sizeof(gc_block*), gc_order);
	jmp_buf registers;
#if defined(__GNUC__) || defined(__clang__)
	__builtin_unwind_init();
#endif
	(void)setjmp(registers);
	gc_scan((const char*)&registers, gc_stack_base);
	gc_scan_static();
	while (gc_mark_count) {
		gc_block* b = gc_marks[--gc_mark_count];
		gc_scan((const char*)(b + 1), (const char*)(b + 1) + b->size);
	}
	size_t kept = 0;
	gc_live = 0;
	for (size_t i = 0; i < gc_count; ++i) {
		gc_block* b = gc_all[i];
		if (b->marked) { b->marked = 0; gc_live += b->size; gc_all[kept++] = b; }
		else free(b);
	}
	gc_count = kept;
	gc_since = 0;
	gc_limit = gc_live > GC_MIN_LIMIT ? gc_live : GC_MIN_LIMIT;
	gc_busy = 0;
}

static void* gc_alloc(size_t size, int atomic, int zero) {
	if (gc_stress < 0) {
		const char* s = getenv("JRT_GC_STRESS");
		gc_stress = s ? atol(s) : 0;
		gc_until = gc_stress;
	}
	if (gc_since >= gc_limit || (gc_stress > 0 && --gc_until <= 0)) { gc_until = gc_stress; gc_collect(); }
	gc_block* b = (gc_block*)(zero ? calloc(1, sizeof(gc_block) + size) : malloc(sizeof(gc_block) + size));
	if (!b) gc_oom();
	b->size = size;
	b->atomic = (uint32_t)atomic;
	b->marked = 0;
	if (gc_count == gc_cap) {
		gc_cap = gc_cap ? gc_cap * 2 : 1024;
		gc_all = (gc_block**)realloc(gc_all, gc_cap * sizeof(gc_block*));
		if (!gc_all) gc_oom();
	}
	/* (kept in order of address only during a collection: gc_find is not used between them) */
	gc_all[gc_count++] = b;
	gc_since += size;
	return b + 1;
}

static void* gc_grow(void* old, size_t size, int atomic) {
	if (!old) return gc_alloc(size, atomic, 0);
	const gc_block* from = (const gc_block*)old - 1;
	void* to = gc_alloc(size, from->atomic, 0);    /* (`old` is on the caller's stack or in a live object: it survives this) */
	from = (const gc_block*)old - 1;
	memcpy(to, old, from->size < size ? from->size : size);
	return to;                                     /* (the old block goes at the next collection) */
}

size_t jrt_gc_collect(void) { gc_collect(); return gc_live; }

/* everything below allocates through the collector */
#define malloc(n) gc_alloc((n), 0, 0)
#define calloc(n, m) gc_alloc((size_t)(n) * (size_t)(m), 0, 1)
#define realloc(p, n) gc_grow((p), (n), 0)
#define free(p) ((void)(p))

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
EXC(OutOfMemoryError, "java.lang.OutOfMemoryError", Error)
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
void jrt_Throwable_addSuppressed_O(jobject* t, jobject* s) {
	jrt_nn(t);
	if (t == s) jrt_throw_new(&jrt_IllegalArgumentException_class, "Self-suppression not permitted");
	if (!s) jrt_throw_new(&jrt_NullPointerException_class, "Cannot suppress a null exception.");
	jrt_Throwable* e = (jrt_Throwable*)t;
	if (!e->suppressed) e->suppressed = jrt_ArrayList_new_();
	jrt_Collection_add_O(e->suppressed, s);
}
jarray* jrt_Throwable_getSuppressed_(jobject* t) {
	jrt_Throwable* e = (jrt_Throwable*)jrt_nn(t);
	if (!e->suppressed) return jrt_new_array(sizeof(jobject*), 0);
	jrt_ArrayList* l = (jrt_ArrayList*)e->suppressed;
	return jrt_array_of(sizeof(jobject*), l->size, l->items);
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
	r->s = (char16_t*)gc_alloc(((size_t)n + 1) * sizeof(char16_t), 1, 1);
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
/* the text as UTF-8, Java's default: a surrogate pair is one character, a lone surrogate a ? */
jarray* jrt_String_getBytes_(jstring* s) {
	const jstring* t = (const jstring*)jrt_nn(s);
	int32_t n = 0;
	for (int32_t pass = 0; pass < 2; ++pass) {
		jarray* a = pass ? jrt_new_array(1, n) : NULL;
		int8_t* out = a ? (int8_t*)a->data : NULL;
		int32_t k = 0;
		for (int32_t i = 0; i < t->len; ++i) {
			uint32_t c = t->s[i];
			if (c >= 0xD800 && c <= 0xDBFF && i + 1 < t->len && t->s[i + 1] >= 0xDC00 && t->s[i + 1] <= 0xDFFF) {
				c = 0x10000 + ((c - 0xD800) << 10) + (t->s[i + 1] - 0xDC00);
				++i;
			} else if (c >= 0xD800 && c <= 0xDFFF) {
				c = '?';
			}
			uint8_t b[4];
			int32_t m = 0;
			if (c < 0x80) { b[m++] = (uint8_t)c; }
			else if (c < 0x800) { b[m++] = (uint8_t)(0xC0 | (c >> 6)); b[m++] = (uint8_t)(0x80 | (c & 0x3F)); }
			else if (c < 0x10000) { b[m++] = (uint8_t)(0xE0 | (c >> 12)); b[m++] = (uint8_t)(0x80 | ((c >> 6) & 0x3F)); b[m++] = (uint8_t)(0x80 | (c & 0x3F)); }
			else { b[m++] = (uint8_t)(0xF0 | (c >> 18)); b[m++] = (uint8_t)(0x80 | ((c >> 12) & 0x3F)); b[m++] = (uint8_t)(0x80 | ((c >> 6) & 0x3F)); b[m++] = (uint8_t)(0x80 | (c & 0x3F)); }
			for (int32_t j = 0; j < m; ++j) { if (out) out[k] = (int8_t)b[j]; ++k; }
		}
		if (pass) return a;
		n = k;
	}
	return NULL;
}
jarray* jrt_String_toCharArray_(jstring* s) { jarray* a = jrt_new_array(2, ((jstring*)jrt_nn(s))->len); memcpy(a->data, s->s, (size_t)s->len * 2); return a; }
jstring* jrt_String_new_A(jarray* a) { return jrt_str((const char16_t*)((jarray*)jrt_nn(a))->data, a->length); }
jstring* jrt_String_valueOf_A(jarray* a) { return jrt_String_new_A(a); }
jstring* jrt_String_new_(void) { return jrt_str(0, 0); }
jstring* jrt_String_new_Str(jstring* s) { jrt_nn(s); return jrt_str(s->s, s->len); }
jstring* jrt_String_new_CAII(jarray* a, int32_t at, int32_t n) {
	jrt_nn(a);
	if (at < 0 || n < 0 || at > a->length - n) {
		char buf[80];
		snprintf(buf, sizeof buf, "offset %d, count %d, length %d", at, n, a->length);
		jrt_throw_new(&jrt_StringIndexOutOfBoundsException_class, buf);
	}
	return jrt_str((const char16_t*)a->data + at, n);
}
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

/* String.format: %d %s %c %b %h %x %o %f %e %n %%, the flags - 0 + , space # (, width, precision, and `%2$s` and `%<s` for which argument.
   %f and %e round half up on the shortest decimal digits, as java.util.Formatter does */
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
	sprintf(out, "%s%s%s%s", neg ? "-" : "", ipz, prec > 0 ? "." : "", fp);
}
static void sb_add8(jrt_StringBuilder* b, const char* s) { while (*s) jrt_StringBuilder_append_C(b, (unsigned char)*s++); }
static void exp_half_up(double d, int prec, char* out) {
	if (d != d) { strcpy(out, "NaN"); return; }
	if (isinf(d)) { strcpy(out, d > 0 ? "Infinity" : "-Infinity"); return; }
	if (prec > 340) prec = 340;
	const char* sign = signbit(d) ? "-" : "";
	double a = fabs(d);
	char digits[360] = "0";
	int exp = 0, keep = prec + 1;
	if (a != 0) shortest(a, 17, digits, &exp);
	int n = (int)strlen(digits);
	if (n > keep) {
		bool round_up = digits[keep] >= '5';
		digits[keep] = 0;
		if (round_up) {
			int i = keep - 1;
			while (i >= 0 && digits[i] == '9') digits[i--] = '0';
			if (i >= 0) digits[i]++;
			else { memmove(digits + 1, digits, (size_t)keep); digits[0] = '1'; digits[keep] = 0; ++exp; }
		}
		n = keep;
	}
	for (; n < keep; ++n) digits[n] = '0';
	digits[keep] = 0;
	const int shown = exp < 0 ? -exp : exp;
	sprintf(out, "%s%c%s%s" "e%c%s%d", sign, digits[0], prec > 0 ? "." : "", digits + 1, exp < 0 ? '-' : '+', shown < 10 ? "0" : "", shown);
}
/* the thousands of a number's whole part */
static void group_thousands(char* body) {
	char g[1024];
	int gi = 0, start = body[0] == '-' ? 1 : 0;
	const char* dot = strchr(body, '.');
	int whole = dot ? (int)(dot - body) : (int)strlen(body);
	for (int k = 0; k < start; ++k) g[gi++] = body[k];
	for (int k = start; k < whole; ++k) { if (k > start && (whole - k) % 3 == 0) g[gi++] = ','; g[gi++] = body[k]; }
	strcpy(g + gi, body + whole);
	strcpy(body, g);
}
static _Noreturn void format_error(const char* what, char conv) {
	char m[64];
	snprintf(m, sizeof m, "%s: %c", what, conv);
	jrt_throw_new(&jrt_IllegalArgumentException_class, m);
}
static bool is_integral_box(const jobject* a) { return a->cls == &jrt_Integer_class || a->cls == &jrt_Long_class || a->cls == &jrt_Short_class || a->cls == &jrt_Byte_class; }
static jstring* format_with(jstring* fmt, int n, jobject** args);
jstring* jrt_String_format(jstring* fmt, int n, ...) {
	va_list ap;
	jobject* args[64];
	if (n > 64) jrt_throw_new(&jrt_UnsupportedOperationException_class, "String.format with more than 64 arguments");
	va_start(ap, n);
	for (int i = 0; i < n; ++i) args[i] = va_arg(ap, jobject*);
	va_end(ap);
	return format_with(fmt, n, args);
}
/* String.format(fmt, Object[]): the array's elements are the arguments */
jstring* jrt_String_format_A(jstring* fmt, jarray* a) {
	jrt_nn(a);
	return format_with(fmt, a->length, (jobject**)a->data);
}
static jstring* format_with(jstring* fmt, int n, jobject** args) {
	jrt_StringBuilder* out = jrt_StringBuilder_new_();
	jstring* f = (jstring*)jrt_nn(fmt);
	int next = 0, last = -1;
	for (int32_t i = 0; i < f->len; ++i) {
		if (f->s[i] != '%') { jrt_StringBuilder_append_C(out, f->s[i]); continue; }
		++i;
		int chosen = -1;                                     /* `%2$s` names its argument, `%<s` repeats the last */
		int32_t d = i;
		while (d < f->len && f->s[d] >= '0' && f->s[d] <= '9') ++d;
		if (d > i && d < f->len && f->s[d] == '$') {
			chosen = 0;
			for (int32_t k = i; k < d; ++k) chosen = chosen * 10 + (f->s[k] - '0');
			--chosen;
			i = d + 1;
		} else if (i < f->len && f->s[i] == '<') {
			chosen = last;
			++i;
		}
		char flags[10] = {0};
		int nf = 0, width = 0, prec = -1;
		while (i < f->len && nf < 9 && f->s[i] < 128 && f->s[i] && strchr("-0+, #(", (char)f->s[i])) flags[nf++] = (char)f->s[i++];
		while (i < f->len && f->s[i] >= '0' && f->s[i] <= '9') width = width * 10 + (f->s[i++] - '0');
		if (i < f->len && f->s[i] == '.') { ++i; prec = 0; while (i < f->len && f->s[i] >= '0' && f->s[i] <= '9') prec = prec * 10 + (f->s[i++] - '0'); }
		if (i >= f->len) format_error("Unknown format conversion", '%');
		const char conv = f->s[i] < 128 ? (char)f->s[i] : '?';
		if (conv == 'n') { jrt_StringBuilder_append_C(out, '\n'); continue; }
		if (conv == '%') { jrt_StringBuilder_append_C(out, '%'); continue; }
		const int at = chosen >= 0 ? chosen : next++;
		if (at < 0 || at >= n) format_error("Missing format argument", conv);
		last = at;
		jobject* a = args[at];
		const bool upper = conv >= 'A' && conv <= 'Z';
		const char lc = upper ? (char)(conv + 32) : conv;
		const bool plus = strchr(flags, '+') != 0, space = strchr(flags, ' ') != 0, paren = strchr(flags, '(') != 0;
		const bool left = strchr(flags, '-') != 0, alt = strchr(flags, '#') != 0;
		bool zero = strchr(flags, '0') != 0;
		char body[1024];
		jstring* text = 0;
		if (!strchr("dxofescbh", lc)) format_error("Unknown format conversion", conv);
		if (lc == 's' || lc == 'b' || lc == 'h' || !a) {     /* (any conversion of a null writes null) */
			zero = false;
			if (lc == 'b') text = str8(!a ? "false" : a->cls == &jrt_Boolean_class ? (((jrt_Boolean*)a)->value ? "true" : "false") : "true");
			else if (lc == 'h' && a) text = jrt_Integer_toHexString_I(jrt_hashCode(a));
			else text = jrt_str_O(a);
			if (prec >= 0 && text->len > prec) text = jrt_str(text->s, prec);
		} else if (lc == 'c') {
			zero = false;
			if (a->cls == &jrt_Character_class) text = jrt_str_C(((jrt_Character*)a)->value);
			else if (a->cls == &jrt_Integer_class || a->cls == &jrt_Short_class || a->cls == &jrt_Byte_class) text = jrt_str_C((char16_t)jrt_unbox_I(a));
			else format_error("Illegal format conversion", conv);
		} else if (lc == 'd') {
			if (!is_integral_box(a)) format_error("Illegal format conversion", conv);
			snprintf(body, sizeof body, "%lld", (long long)jrt_unbox_J(a));
			if (strchr(flags, ',')) group_thousands(body);
		} else if (lc == 'x' || lc == 'o') {                 /* a negative number as the bits of its own type */
			if (!is_integral_box(a)) format_error("Illegal format conversion", conv);
			uint64_t bits = (uint64_t)jrt_unbox_J(a);
			if (a->cls == &jrt_Integer_class) bits &= 0xFFFFFFFFu;
			else if (a->cls == &jrt_Short_class) bits &= 0xFFFFu;
			else if (a->cls == &jrt_Byte_class) bits &= 0xFFu;
			snprintf(body, sizeof body, "%s%llx", alt && lc == 'x' ? "0x" : "", (unsigned long long)bits);
			if (lc == 'o') snprintf(body, sizeof body, "%s%llo", alt ? "0" : "", (unsigned long long)bits);
		} else {                                             /* f, e */
			if (a->cls != &jrt_Double_class && a->cls != &jrt_Float_class) format_error("Illegal format conversion", conv);
			const double v = jrt_unbox_D(a);
			if (lc == 'f') fixed_half_up(v, prec < 0 ? 6 : prec, body);
			else exp_half_up(v, prec < 0 ? 6 : prec, body);
			if (!isfinite(v)) zero = false;                  /* (NaN and the infinities are padded with spaces) */
			else if (lc == 'f' && strchr(flags, ',')) group_thousands(body);
		}
		if (!text) {                                         /* a number: its sign as the flags say */
			if (lc == 'd' || lc == 'f' || lc == 'e') {
				const bool nan = strcmp(body, "NaN") == 0;
				if (body[0] == '-' && paren) { size_t k = strlen(body); memmove(body, body + 1, k); memmove(body + 1, body, k); body[0] = '('; body[k] = ')'; body[k + 1] = 0; }
				else if (body[0] != '-' && !nan && (plus || space)) { memmove(body + 1, body, strlen(body) + 1); body[0] = plus ? '+' : ' '; }
			}
			text = str8(body);
		}
		if (upper) text = jrt_String_toUpperCase_(text);
		const int pad = width - text->len;
		if (pad <= 0) { jrt_StringBuilder_append_Str(out, text); continue; }
		if (left) {
			jrt_StringBuilder_append_Str(out, text);
			for (int k = 0; k < pad; ++k) jrt_StringBuilder_append_C(out, ' ');
		} else if (zero) {                                   /* zeros go after the sign or the 0x */
			int32_t skip = text->len && strchr("-+ (", (char)text->s[0]) ? 1 : 0;
			if (lc == 'x' && alt) skip = 2;
			for (int32_t k = 0; k < skip; ++k) jrt_StringBuilder_append_C(out, text->s[k]);
			for (int k = 0; k < pad; ++k) jrt_StringBuilder_append_C(out, '0');
			for (int32_t k = skip; k < text->len; ++k) jrt_StringBuilder_append_C(out, text->s[k]);
		} else {
			for (int k = 0; k < pad; ++k) jrt_StringBuilder_append_C(out, ' ');
			jrt_StringBuilder_append_Str(out, text);
		}
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
double jrt_Double_longBitsToDouble_J(int64_t bits) { double d; memcpy(&d, &bits, sizeof d); return d; }
int32_t jrt_Float_floatToIntBits_F(float v) {
	int32_t bits;
	if (v != v) return 0x7fc00000;   // every NaN is the one NaN, as Java's has it
	memcpy(&bits, &v, sizeof bits);
	return bits;
}
float jrt_Float_intBitsToFloat_I(int32_t bits) { float f; memcpy(&f, &bits, sizeof f); return f; }
// (whole numbers step through unsigned, so the largest wraps to the smallest as Java's does)
#define JRT_BOX_STEP(L, T, NEXT) \
	jobject* jrt_box_step_##L(jobject* volatile* slot, int by, bool postfix) { \
		jobject* before = *slot; \
		T v = jrt_unbox_##L(before); \
		jobject* after = jrt_box_##L((T)(NEXT)); \
		*slot = after; \
		return postfix ? before : after; \
	}
JRT_BOX_STEP(I, int32_t, (uint32_t)v + (uint32_t)by)
JRT_BOX_STEP(J, int64_t, (uint64_t)v + (uint64_t)(int64_t)by)
JRT_BOX_STEP(D, double, v + by)
JRT_BOX_STEP(F, float, v + (float)by)
JRT_BOX_STEP(S, int16_t, (uint32_t)v + (uint32_t)by)
JRT_BOX_STEP(B, int8_t, (uint32_t)v + (uint32_t)by)
JRT_BOX_STEP(C, char16_t, (uint32_t)v + (uint32_t)by)
bool jrt_unbox_Z(jobject* o) { return ((jrt_Boolean*)jrt_cast((jobject*)jrt_nn(o), &jrt_Boolean_class))->value; }
int32_t jrt_natural(jobject* a, jobject* b) {
	if (!a || !b) jrt_throw_npe();
	if (a->cls == &jrt_String_class) return jrt_String_compareTo_O((jstring*)a, b);
	if (a->cls == &jrt_Integer_class) return jrt_Integer_compare_II(jrt_unbox_I(a), jrt_unbox_I(b));
	if (a->cls == &jrt_Short_class) return (int32_t)jrt_unbox_S(a) - (int32_t)jrt_unbox_S(b);   // (these three give the difference, as Java's do)
	if (a->cls == &jrt_Byte_class) return (int32_t)jrt_unbox_B(a) - (int32_t)jrt_unbox_B(b);
	if (a->cls == &jrt_Character_class) return (int32_t)jrt_unbox_C(a) - (int32_t)jrt_unbox_C(b);
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
jstring* jrt_Integer_toString_II(int32_t v, int32_t radix) {
	char b[40];
	int k = 38;
	int64_t left = v < 0 ? -(int64_t)v : v;
	if (radix < 2 || radix > 36) radix = 10;
	b[39] = 0;
	do {
		b[k--] = "0123456789abcdefghijklmnopqrstuvwxyz"[left % radix];
		left /= radix;
	} while (left > 0);
	if (v < 0) b[k--] = '-';
	return str8(b + k + 1);
}
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
int32_t jrt_Integer_parseInt_StrI(jstring* s, int32_t radix) {
	jrt_nn(s);
	int32_t i = 0;
	bool neg = false;
	long long v = 0;
	if (radix < 2 || radix > 36) goto bad;
	if (s->len > 0 && (s->s[0] == '-' || s->s[0] == '+')) { neg = s->s[0] == '-'; i = 1; }
	if (i >= s->len) goto bad;
	for (; i < s->len; ++i) {
		int32_t c = s->s[i], d = -1;
		if (c >= '0' && c <= '9') d = c - '0';
		else if (c >= 'a' && c <= 'z') d = c - 'a' + 10;
		else if (c >= 'A' && c <= 'Z') d = c - 'A' + 10;
		if (d < 0 || d >= radix) goto bad;
		v = v * radix + d;
		if (v > 2147483648LL) goto bad;
	}
	if (neg) v = -v;
	if (v > JRT_INT_MAX || v < JRT_INT_MIN) goto bad;
	return (int32_t)v;
bad: {
		jstring* m = jrt_concat(3, str8("For input string: \""), s, str8("\""));
		if (radix != 10) m = jrt_concat(3, m, str8(" under radix "), jrt_str_I(radix));
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
jobject* jrt_Long_valueOf_J(int64_t v) { return jrt_box_J(v); }
jobject* jrt_Double_valueOf_D(double v) { return jrt_box_D(v); }
jobject* jrt_Float_valueOf_F(float v) { return jrt_box_F(v); }
jobject* jrt_Byte_valueOf_B(int8_t v) { return jrt_box_B(v); }
jobject* jrt_Short_valueOf_S(int16_t v) { return jrt_box_S(v); }
jobject* jrt_Boolean_valueOf_Z(bool v) { return jrt_box_Z(v); }
jobject* jrt_Byte_new_B(int8_t v) { return jrt_box_B(v); }
jobject* jrt_Short_new_S(int16_t v) { return jrt_box_S(v); }
jobject* jrt_Boolean_new_Z(bool v) { return jrt_box_Z(v); }
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
	a->data = gc_alloc((size_t)(n ? n : 1) * (size_t)esize, (size_t)esize < sizeof(void*), 1);   /* (elements smaller than a pointer hold none: the collector need not read them) */
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
static jarray* array_level(int32_t esize, int n, const int32_t* dims) {
	if (n == 1) return jrt_new_array(esize, dims[0]);
	jarray* a = jrt_new_array(sizeof(jarray*), dims[0]);
	for (int32_t i = 0; i < dims[0]; ++i) ((jarray**)a->data)[i] = array_level(esize, n - 1, dims + 1);
	return a;
}
jarray* jrt_new_array_n(int32_t esize, int n, const int32_t* dims) {
	for (int i = 0; i < n; ++i) if (dims[i] < 0) { char b[32]; snprintf(b, sizeof b, "%d", dims[i]); jrt_throw_new(&jrt_NegativeArraySizeException_class, b); }
	return array_level(esize, n, dims);
}
jarray* jrt_array_clone(jarray* a) { jrt_nn(a); return jrt_array_of(a->esize, a->length, a->data); }
static int cmp_c(const void* a, const void* b) { return (int)*(const char16_t*)a - (int)*(const char16_t*)b; }
static int cmp_i(const void* a, const void* b) { int32_t x = *(const int32_t*)a, y = *(const int32_t*)b; return x < y ? -1 : x > y; }
static int cmp_d(const void* a, const void* b) { return jrt_Double_compare_DD(*(const double*)a, *(const double*)b); }
void jrt_Arrays_sort_CA(jarray* a) { qsort(((jarray*)jrt_nn(a))->data, a->length, 2, cmp_c); }
void jrt_Arrays_sort_IA(jarray* a) { qsort(((jarray*)jrt_nn(a))->data, a->length, 4, cmp_i); }
void jrt_Arrays_sort_DA(jarray* a) { qsort(((jarray*)jrt_nn(a))->data, a->length, 8, cmp_d); }
// Arrays.fill(a, from, to, v): the range is checked first, as Java's is
static void fill_range(jarray* a, int32_t from, int32_t to) {
	char text[80];
	if (from > to) {
		snprintf(text, sizeof text, "fromIndex(%d) > toIndex(%d)", from, to);
		jrt_throw_new(&jrt_IllegalArgumentException_class, text);
	}
	if (from < 0 || to > JLEN(a)) {
		snprintf(text, sizeof text, "Array index out of range: %d", from < 0 ? from : to);
		jrt_throw_new(&jrt_ArrayIndexOutOfBoundsException_class, text);
	}
}
void jrt_Arrays_fill_IAIII(jarray* a, int32_t from, int32_t to, int32_t v) { fill_range(a, from, to); for (int32_t i = from; i < to; ++i) ((int32_t*)a->data)[i] = v; }
void jrt_Arrays_fill_JAIIJ(jarray* a, int32_t from, int32_t to, int64_t v) { fill_range(a, from, to); for (int32_t i = from; i < to; ++i) ((int64_t*)a->data)[i] = v; }
void jrt_Arrays_fill_DAIID(jarray* a, int32_t from, int32_t to, double v) { fill_range(a, from, to); for (int32_t i = from; i < to; ++i) ((double*)a->data)[i] = v; }
void jrt_Arrays_fill_FAIIF(jarray* a, int32_t from, int32_t to, float v) { fill_range(a, from, to); for (int32_t i = from; i < to; ++i) ((float*)a->data)[i] = v; }
void jrt_Arrays_fill_BAIIB(jarray* a, int32_t from, int32_t to, int8_t v) { fill_range(a, from, to); for (int32_t i = from; i < to; ++i) ((int8_t*)a->data)[i] = v; }
void jrt_Arrays_fill_SAIIS(jarray* a, int32_t from, int32_t to, int16_t v) { fill_range(a, from, to); for (int32_t i = from; i < to; ++i) ((int16_t*)a->data)[i] = v; }
void jrt_Arrays_fill_CAIIC(jarray* a, int32_t from, int32_t to, char16_t v) { fill_range(a, from, to); for (int32_t i = from; i < to; ++i) ((char16_t*)a->data)[i] = v; }
void jrt_Arrays_fill_ZAIIZ(jarray* a, int32_t from, int32_t to, bool v) { fill_range(a, from, to); for (int32_t i = from; i < to; ++i) ((bool*)a->data)[i] = v; }
void jrt_Arrays_fill_OAIIO(jarray* a, int32_t from, int32_t to, jobject* v) { fill_range(a, from, to); for (int32_t i = from; i < to; ++i) ((jobject**)a->data)[i] = v; }
void jrt_Arrays_fill_IA(jarray* a, int32_t v) { for (int32_t i = 0; i < JLEN(a); ++i) ((int32_t*)a->data)[i] = v; }
jstring* jrt_Arrays_toString_IA(jarray* a) {
	if (!a) return str8("null");
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '[');
	for (int32_t i = 0; i < a->length; ++i) { if (i) sb_add8(b, ", "); jrt_StringBuilder_append_I(b, ((int32_t*)a->data)[i]); }
	jrt_StringBuilder_append_C(b, ']');
	return jrt_StringBuilder_toString_(b);
}
jstring* jrt_Arrays_toString_CA(jarray* a) {
	if (!a) return str8("null");
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '[');
	for (int32_t i = 0; i < a->length; ++i) { if (i) sb_add8(b, ", "); jrt_StringBuilder_append_C(b, ((char16_t*)a->data)[i]); }
	jrt_StringBuilder_append_C(b, ']');
	return jrt_StringBuilder_toString_(b);
}
jstring* jrt_Arrays_toString_OA(jarray* a) {
	if (!a) return str8("null");
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '[');
	for (int32_t i = 0; i < a->length; ++i) { if (i) sb_add8(b, ", "); jrt_StringBuilder_append_Str(b, jrt_str_O(((jobject**)a->data)[i])); }
	jrt_StringBuilder_append_C(b, ']');
	return jrt_StringBuilder_toString_(b);
}
jstring* jrt_Arrays_toString_JA(jarray* a) {
	if (!a) return str8("null");
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '[');
	for (int32_t i = 0; i < a->length; ++i) { if (i) sb_add8(b, ", "); jrt_StringBuilder_append_Str(b, jrt_str_J(((int64_t*)a->data)[i])); }
	jrt_StringBuilder_append_C(b, ']');
	return jrt_StringBuilder_toString_(b);
}
jstring* jrt_Arrays_toString_DA(jarray* a) {
	if (!a) return str8("null");
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '[');
	for (int32_t i = 0; i < a->length; ++i) { if (i) sb_add8(b, ", "); jrt_StringBuilder_append_Str(b, jrt_str_D(((double*)a->data)[i])); }
	jrt_StringBuilder_append_C(b, ']');
	return jrt_StringBuilder_toString_(b);
}
jstring* jrt_Arrays_toString_ZA(jarray* a) {
	if (!a) return str8("null");
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '[');
	for (int32_t i = 0; i < a->length; ++i) { if (i) sb_add8(b, ", "); jrt_StringBuilder_append_Str(b, jrt_str_Z(((bool*)a->data)[i])); }
	jrt_StringBuilder_append_C(b, ']');
	return jrt_StringBuilder_toString_(b);
}
jstring* jrt_Arrays_toString_BA(jarray* a) {
	if (!a) return str8("null");
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jrt_StringBuilder_append_C(b, '[');
	for (int32_t i = 0; i < a->length; ++i) { if (i) sb_add8(b, ", "); jrt_StringBuilder_append_I(b, ((int8_t*)a->data)[i]); }
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
	b->s = (char16_t*)gc_grow(b->s, (size_t)c * 2, 1);
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

void* jrt_print_to(void* stream, jstring* s) { jrt_print(s, false, (FILE*)stream); return stream; }

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

/* a class of the program may extend ArrayList or HashMap: its object begins with the runtime's, so what holds one is checked by class chain */
static bool is_al(jobject* c) { return c->cls == &jrt_ArrayList_class || jrt_instanceof(c, &jrt_ArrayList_class); }
static bool is_hm(jobject* c) { return c->cls == &jrt_HashMap_class || jrt_instanceof(c, &jrt_HashMap_class); }
#define LOAD(m) ((m)->load > 0 ? (m)->load : 0.75f)   /* (a map made by a subclass starts all zero) */
jstring* jrt_list_toString(jobject* o) { return coll_toString(o); }
bool jrt_list_equals(jobject* a, jobject* b) { return list_equals(a, b); }
int32_t jrt_list_hashCode(jobject* o) { return list_hashCode(o); }
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
static void resizable(jrt_ArrayList* l) { if (l->immutable || l->fixed) jrt_throw_new(&jrt_UnsupportedOperationException_class, 0); }
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
/* Arrays.asList: a list of the array's own elements, so a set reaches the array; its size cannot change */
jobject* jrt_Arrays_asList_OA(jarray* a) {
	jrt_nn(a);
	jrt_ArrayList* l = (jrt_ArrayList*)jrt_ArrayList_new_();
	l->items = (jobject**)a->data;
	l->size = l->cap = a->length;
	l->fixed = true;
	return (jobject*)l;
}
void jrt_Iterable_forEach_O(jobject* c, jobject* f) {
	jrt_nn(f);
	jobject* it = jrt_Collection_iterator_(c);
	while (jrt_Iterator_hasNext_(it)) jrt_Consumer_accept_O(f, jrt_Iterator_next_(it));
}
static jrt_HashMap* as_map(jobject* o) { return (jrt_HashMap*)jrt_cast((jobject*)jrt_nn(o), &jrt_HashMap_class); }
static void map_entries(jrt_HashMap* m, jobject*** out, int32_t* n);
bool jrt_Collection_add_O(jobject* c, jobject* x) {
	jrt_nn(c);
	if (is_al(c)) { jrt_ArrayList* l = (jrt_ArrayList*)c; resizable(l); al_add(l, l->size, x); return true; }
	if (c->cls == &jrt_MapSet_class) {
		jrt_MapSet* s = (jrt_MapSet*)c;
		if (s->kind != 0 || s->m->immutable) jrt_throw_new(&jrt_UnsupportedOperationException_class, 0);
		if (jrt_Map_containsKey_O((jobject*)s->m, x)) return false;
		jrt_Map_put_OO((jobject*)s->m, x, jrt_box_Z(true));
		return true;
	}
	jrt_throw_new(&jrt_UnsupportedOperationException_class, "add");
}
int32_t jrt_Collection_size_(jobject* c) {
	jrt_nn(c);
	if (is_al(c)) return ((jrt_ArrayList*)c)->size;
	if (c->cls == &jrt_MapSet_class) return ((jrt_MapSet*)c)->m->count;
	jrt_throw_new(&jrt_UnsupportedOperationException_class, "size");
}
bool jrt_Collection_isEmpty_(jobject* c) { return jrt_Collection_size_(c) == 0; }
jobject* jrt_Collection_iterator_(jobject* c) {
	jrt_nn(c);
	jrt_Iter* it = (jrt_Iter*)jrt_alloc(&iter_class);
	it->src = c;
	if (is_al(c)) {
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
	if (is_al(c)) { int32_t i = jrt_List_indexOf_O(c, x); if (i < 0) return false; jrt_List_remove_I(c, i); return true; }
	if (c->cls == &jrt_MapSet_class) {
		jrt_MapSet* s = (jrt_MapSet*)c;
		if (s->m->immutable) jrt_throw_new(&jrt_UnsupportedOperationException_class, 0);
		bool had = jrt_Map_containsKey_O((jobject*)s->m, x);
		jrt_Map_remove_O((jobject*)s->m, x);
		return had;
	}
	jrt_throw_new(&jrt_UnsupportedOperationException_class, "remove");
}
void jrt_Collection_clear_(jobject* c) {
	jrt_nn(c);
	if (is_al(c)) {
		jrt_ArrayList* l = (jrt_ArrayList*)c;
		if (l->fixed && l->size == 0) return;   /* (Java's clear of an empty fixed list removes nothing, so it does not throw) */
		resizable(l);
		l->size = 0;
		return;
	}
	if (c->cls == &jrt_MapSet_class) { jrt_Map_clear_((jobject*)((jrt_MapSet*)c)->m); return; }
}
jobject* jrt_List_get_I(jobject* o, int32_t i) { jrt_ArrayList* l = as_list(o); if (i < 0 || i >= l->size) ioobe(i, l->size); return l->items[i]; }
jobject* jrt_List_set_IO(jobject* o, int32_t i, jobject* x) { jrt_ArrayList* l = as_list(o); immutable(l); if (i < 0 || i >= l->size) ioobe(i, l->size); jobject* old = l->items[i]; l->items[i] = x; return old; }
void jrt_List_add_IO(jobject* o, int32_t i, jobject* x) { jrt_ArrayList* l = as_list(o); resizable(l); if (i < 0 || i > l->size) ioobe(i, l->size); al_add(l, i, x); }
jobject* jrt_List_remove_I(jobject* o, int32_t i) {
	jrt_ArrayList* l = as_list(o);
	resizable(l);
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
	if (!b || !is_al(b)) return false;
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
static void map_walk(jrt_HashMap* m, jobject*** out, int32_t* n);
static jrt_node* map_find(jrt_HashMap* m, jobject* k);
static bool map_equals(jobject* a, jobject* b) {
	if (a == b) return true;
	if (!b || !is_hm(b)) return false;
	jrt_HashMap *x = (jrt_HashMap*)a, *y = (jrt_HashMap*)b;
	if (x->count != y->count) return false;
	jobject** es;
	int32_t n;
	map_walk(x, &es, &n);
	for (int32_t i = 0; i < n; ++i) {
		jrt_node* e = (jrt_node*)es[i];
		jrt_node* other = map_find(y, e->key);
		if (!other || !jrt_Objects_equals_OO(e->value, other->value)) return false;
	}
	return true;
}
static int32_t node_hashCode(jobject* o) { jrt_node* e = (jrt_node*)o; return jrt_Objects_hashCode_O(e->key) ^ jrt_Objects_hashCode_O(e->value); }
static bool node_equals(jobject* a, jobject* b) {
	if (a == b) return true;
	if (!b || b->cls != &jrt_node_class) return false;
	jrt_node *x = (jrt_node*)a, *y = (jrt_node*)b;
	return jrt_Objects_equals_OO(x->key, y->key) && jrt_Objects_equals_OO(x->value, y->value);
}
static int32_t map_hashCode(jobject* o) {   /* (a sum: the same in any order) */
	jobject** es;
	int32_t n;
	uint32_t h = 0;
	map_walk((jrt_HashMap*)o, &es, &n);
	for (int32_t i = 0; i < n; ++i) h += (uint32_t)node_hashCode(es[i]);
	return (int32_t)h;
}
static const jobject_vt map_vt = {map_toString, map_equals, map_hashCode};
static const jitab map_itabs[] = {{&jrt_Map_class, &map_vt}};
jstring* jrt_map_toString(jobject* o) { return map_toString(o); }
bool jrt_map_equals(jobject* a, jobject* b) { return map_equals(a, b); }
int32_t jrt_map_hashCode(jobject* o) { return map_hashCode(o); }
const jclass jrt_HashMap_class = {"java.util.HashMap", &jrt_Object_class, sizeof(jrt_HashMap), &map_vt, JRT_ITABS(map_itabs), 0};
static jstring* node_toString(jobject* o) { jrt_node* e = (jrt_node*)o; return jrt_concat(3, jrt_str_O(e->key), str8("="), jrt_str_O(e->value)); }
static const jobject_vt node_vt = {node_toString, node_equals, node_hashCode};
const jclass jrt_node_class = {"java.util.HashMap.Node", &jrt_Object_class, sizeof(jrt_node), &node_vt, 0, 0, 0};
/* a set's elements, however it is made: the keys of its map, or the entries */
static jobject* set_item(const jrt_MapSet* s, jobject* entry) { return s->kind == 0 ? ((jrt_node*)entry)->key : entry; }
static bool set_has(jrt_MapSet* s, jobject* x) {
	if (s->kind == 0) return map_find(s->m, x) != 0;
	if (!x || x->cls != &jrt_node_class) return false;
	jrt_node* e = map_find(s->m, ((jrt_node*)x)->key);
	return e && jrt_Objects_equals_OO(e->value, ((jrt_node*)x)->value);
}
static bool set_equals(jobject* a, jobject* b) {
	if (a == b) return true;
	if (!b || b->cls != &jrt_MapSet_class) return false;
	jrt_MapSet *x = (jrt_MapSet*)a, *y = (jrt_MapSet*)b;
	if (x->kind == 1 || y->kind == 1 || x->m->count != y->m->count) return false;   /* (a map's values are not a set) */
	jobject** es;
	int32_t n;
	map_walk(x->m, &es, &n);
	for (int32_t i = 0; i < n; ++i) if (!set_has(y, set_item(x, es[i]))) return false;
	return true;
}
static int32_t set_hashCode(jobject* o) {
	jrt_MapSet* s = (jrt_MapSet*)o;
	if (s->kind == 1) return jrt_Object_hashCode(o);
	jobject** es;
	int32_t n;
	uint32_t h = 0;
	map_walk(s->m, &es, &n);
	for (int32_t i = 0; i < n; ++i) h += (uint32_t)jrt_Objects_hashCode_O(set_item(s, es[i]));
	return (int32_t)h;
}
static const jrt_Iterable_vt set_vt = {{coll_toString, set_equals, set_hashCode}, coll_iterator};
static const jitab set_itabs[] = {{&jrt_Iterable_class, &set_vt}, {&jrt_Collection_class, &set_vt}, {&jrt_Set_class, &set_vt}};
const jclass jrt_MapSet_class = {"java.util.HashSet", &jrt_Object_class, sizeof(jrt_MapSet), &set_vt, JRT_ITABS(set_itabs), 0};

static jrt_HashMap* new_map(int kind) { jrt_HashMap* m = (jrt_HashMap*)jrt_alloc(&jrt_HashMap_class); m->kind = kind; m->load = 0.75f; return m; }
static int32_t table_size_for(int32_t n) { int32_t c = 1; while (c < n && c < (1 << 30)) c <<= 1; return c; }
static jobject* sized_map(int kind, int32_t capacity, float load) {
	if (capacity < 0) {
		char b[64];
		snprintf(b, sizeof b, "Illegal initial capacity: %d", capacity);
		jrt_throw_new(&jrt_IllegalArgumentException_class, b);
	}
	if (!(load > 0)) {
		jstring* text = jrt_concat(2, str8("Illegal load factor: "), jrt_str_F(load));
		jrt_throw((jobject*)jrt_new_throwable(&jrt_IllegalArgumentException_class, text, 0));
	}
	jrt_HashMap* m = new_map(kind);
	m->load = load;
	m->threshold = table_size_for(capacity);
	return (jobject*)m;
}
jobject* jrt_HashMap_new_(void) { return (jobject*)new_map(0); }
jobject* jrt_HashMap_new_I(int32_t capacity) { return sized_map(0, capacity, 0.75f); }
jobject* jrt_HashMap_new_IF(int32_t capacity, float load) { return sized_map(0, capacity, load); }
jobject* jrt_HashMap_new_O(jobject* from) { jobject* m = jrt_HashMap_new_(); jrt_Map_putAll_O(m, from); return m; }
jobject* jrt_LinkedHashMap_new_(void) { return (jobject*)new_map(1); }
jobject* jrt_LinkedHashMap_new_I(int32_t capacity) { return sized_map(1, capacity, 0.75f); }
jobject* jrt_LinkedHashMap_new_IF(int32_t capacity, float load) { return sized_map(1, capacity, load); }
jobject* jrt_LinkedHashMap_new_O(jobject* from) { jobject* m = jrt_LinkedHashMap_new_(); jrt_Map_putAll_O(m, from); return m; }
jobject* jrt_TreeMap_new_(void) { return (jobject*)new_map(2); }
jobject* jrt_TreeMap_new_O(jobject* x) {                     /* the two constructors of one argument: a map to copy, or a Comparator */
	jrt_HashMap* m = new_map(2);
	if (x && is_hm(x)) jrt_Map_putAll_O((jobject*)m, x);
	else m->cmp = x;
	return (jobject*)m;
}
static int32_t spread(jobject* k) { if (!k) return 0; int32_t h = jrt_hashCode(k); return h ^ (int32_t)((uint32_t)h >> 16); }
static int32_t key_cmp(jrt_HashMap* m, jobject* a, jobject* b) { return m->cmp ? jrt_Comparator_compare_OO(m->cmp, a, b) : jrt_natural(a, b); }
static void mutable_map(const jrt_HashMap* m) { if (m->immutable) jrt_throw_new(&jrt_UnsupportedOperationException_class, 0); }
/* A TreeMap: its entries in key order, linked for walking and held in `table` for finding by halves. */
static int32_t tree_at(jrt_HashMap* m, jobject* k) {
	int32_t low = 0, high = m->count;
	while (low < high) {
		const int32_t middle = low + (high - low) / 2;
		if (key_cmp(m, m->table[middle]->key, k) < 0) low = middle + 1;
		else high = middle;
	}
	return low;
}
static jrt_node* map_find(jrt_HashMap* m, jobject* k) {
	if (m->immutable && !k) jrt_throw_npe();                  /* (Set.of and Map.of do not hold a null, and say so when asked) */
	if (m->kind == 2) {
		if (!k && !m->cmp) jrt_throw_npe();
		const int32_t at = tree_at(m, k);
		if (at < m->count && key_cmp(m, m->table[at]->key, k) == 0) return m->table[at];
		return 0;
	}
	if (!m->cap) return 0;
	int32_t h = spread(k);
	for (jrt_node* e = m->table[h & (m->cap - 1)]; e; e = e->next)
		if (e->hash == h && (e->key == k || (k && jrt_equals(k, e->key)))) return e;
	return 0;
}
static void resize(jrt_HashMap* m) {                         /* Java's: the first table is the capacity asked for, or 16 */
	int32_t old = m->cap, n, next = 0;
	if (old) { n = old * 2; if (old >= 16) next = m->threshold << 1; }
	else if (m->threshold > 0) n = m->threshold;
	else { n = 16; next = 12; }
	m->threshold = next ? next : (int32_t)((float)n * LOAD(m));
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
static void grown_bin(jrt_HashMap* m) {                      /* past eight in a bin: a small table grows, a large one makes a tree */
	if (m->cap < 64) resize(m);
	else if (m->kind == 0) m->tree_bin = true;
}
/* put appends to its bin and grows after; merge and the compute family grow first and go at the bin's head */
static jrt_node* map_insert(jrt_HashMap* m, jobject* k, jobject* v, bool head) {
	jrt_node* n = (jrt_node*)jrt_alloc(&jrt_node_class);
	n->key = k;
	n->value = v;
	if (m->kind == 2) {                                  /* in key order */
		const int32_t at = tree_at(m, k);
		if (m->count == m->cap) {
			const int32_t room = m->cap ? m->cap * 2 : 16;
			jrt_node** more = (jrt_node**)calloc((size_t)room, sizeof(jrt_node*));
			if (m->count) memcpy(more, m->table, (size_t)m->count * sizeof(jrt_node*));
			m->table = more;
			m->cap = room;
		}
		jrt_node* next = at < m->count ? m->table[at] : 0;
		jrt_node* prev = at > 0 ? m->table[at - 1] : 0;
		memmove(m->table + at + 1, m->table + at, (size_t)(m->count - at) * sizeof(jrt_node*));
		m->table[at] = n;
		n->after = next;
		n->before = prev;
		if (next) next->before = n; else m->tail = n;
		if (prev) prev->after = n; else m->head = n;
		++m->count;
		return n;
	}
	n->hash = spread(k);
	if (!m->cap || (head && m->count > m->threshold)) resize(m);
	int32_t i = n->hash & (m->cap - 1), in_bin = 0;
	jrt_node* last = 0;
	for (jrt_node* e = m->table[i]; e; e = e->next) { last = e; ++in_bin; }
	if (head || !last) { n->next = m->table[i]; m->table[i] = n; }
	else last->next = n;
	if (in_bin >= (head ? 7 : 8)) grown_bin(m);
	n->before = m->tail;
	if (m->tail) m->tail->after = n; else m->head = n;
	m->tail = n;
	++m->count;
	if (!head && m->count > m->threshold) resize(m);
	return n;
}
jobject* jrt_Map_get_O(jobject* o, jobject* k) { jrt_node* e = map_find(as_map(o), k); return e ? e->value : 0; }
bool jrt_Map_containsKey_O(jobject* o, jobject* k) { return map_find(as_map(o), k) != 0; }
jobject* jrt_Map_put_OO(jobject* o, jobject* k, jobject* v) {
	jrt_HashMap* m = as_map(o);
	mutable_map(m);
	jrt_node* e = map_find(m, k);
	if (e) { jobject* old = e->value; e->value = v; return old; }
	map_insert(m, k, v, false);
	return 0;
}
jobject* jrt_Map_remove_O(jobject* o, jobject* k) {
	jrt_HashMap* m = as_map(o);
	mutable_map(m);
	jrt_node* e = map_find(m, k);
	if (!e) return 0;
	if (m->kind != 2) {
		jrt_node** p = &m->table[e->hash & (m->cap - 1)];
		while (*p != e) p = &(*p)->next;
		*p = e->next;
	} else {
		const int32_t at = tree_at(m, e->key);
		memmove(m->table + at, m->table + at + 1, (size_t)(m->count - at - 1) * sizeof(jrt_node*));
		m->table[m->count - 1] = 0;
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
	mutable_map(m);
	if (!v || !f) jrt_throw_npe();
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
jobject* jrt_Map_putIfAbsent_OO(jobject* o, jobject* k, jobject* v) {
	jrt_HashMap* m = as_map(o);
	mutable_map(m);
	jrt_node* e = map_find(m, k);
	if (e) { jobject* old = e->value; if (!old) e->value = v; return old; }
	map_insert(m, k, v, false);
	return 0;
}
jobject* jrt_Map_computeIfAbsent_OO(jobject* o, jobject* k, jobject* f) {
	jrt_HashMap* m = as_map(o);
	mutable_map(m);
	jrt_nn(f);
	jrt_node* e = map_find(m, k);
	if (e && e->value) return e->value;
	jobject* v = jrt_Function_apply_O(f, k);
	if (!v) return 0;
	if (e) e->value = v; else map_insert(m, k, v, true);
	return v;
}
jobject* jrt_Map_computeIfPresent_OO(jobject* o, jobject* k, jobject* f) {
	jrt_HashMap* m = as_map(o);
	mutable_map(m);
	jrt_nn(f);
	jrt_node* e = map_find(m, k);
	if (!e || !e->value) return 0;
	jobject* v = jrt_BiFunction_apply_OO(f, k, e->value);
	if (v) e->value = v; else jrt_Map_remove_O(o, k);
	return v;
}
jobject* jrt_Map_compute_OO(jobject* o, jobject* k, jobject* f) {
	jrt_HashMap* m = as_map(o);
	mutable_map(m);
	jrt_nn(f);
	jrt_node* e = map_find(m, k);
	jobject* v = jrt_BiFunction_apply_OO(f, k, e ? e->value : 0);
	if (e) { if (v) e->value = v; else jrt_Map_remove_O(o, k); }
	else if (v) map_insert(m, k, v, true);
	return v;
}
void jrt_Map_clear_(jobject* o) {
	jrt_HashMap* m = as_map(o);
	mutable_map(m);
	if (m->table) memset(m->table, 0, sizeof(jrt_node*) * m->cap);
	m->count = 0;
	m->head = m->tail = 0;
	m->tree_bin = false;
}
static void map_walk(jrt_HashMap* m, jobject*** out, int32_t* n) {   /* its entries, in the order Java walks them */
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
static void map_entries(jrt_HashMap* m, jobject*** out, int32_t* n) {   /* the same for what the program can see: an order Java has */
	if (m->tree_bin) jrt_throw_new(&jrt_UnsupportedOperationException_class, "this HashMap has a bin Java keeps as a tree: its order cannot be reproduced");
	if (m->shuffled && m->count > 1) jrt_throw_new(&jrt_UnsupportedOperationException_class, "Set.of and Map.of change their order on every run of Java");
	map_walk(m, out, n);
}
void jrt_Map_putAll_O(jobject* o, jobject* from) {
	jrt_HashMap* m = as_map(o);
	jrt_HashMap* src = as_map(from);
	mutable_map(m);
	if (m->kind != 2 && src->count > 0) {                /* Java sizes the table for what is coming */
		if (!m->cap) {
			const int32_t wanted = (int32_t)ceil(src->count / (double)LOAD(m));
			if (wanted > m->threshold) m->threshold = table_size_for(wanted);
		} else {
			while (src->count > m->threshold && m->cap < (1 << 30)) resize(m);
		}
	}
	jobject** es;
	int32_t n;
	map_entries(src, &es, &n);
	for (int32_t i = 0; i < n; ++i) jrt_Map_put_OO(o, ((jrt_node*)es[i])->key, ((jrt_node*)es[i])->value);
}
void jrt_Map_forEach_O(jobject* o, jobject* f) {
	jrt_nn(f);
	jobject** es;
	int32_t n;
	map_entries(as_map(o), &es, &n);
	for (int32_t i = 0; i < n; ++i) jrt_BiConsumer_accept_OO(f, ((jrt_node*)es[i])->key, ((jrt_node*)es[i])->value);
}
bool jrt_Map_containsValue_O(jobject* o, jobject* v) {
	jrt_HashMap* m = as_map(o);
	if (m->immutable && !v) jrt_throw_npe();
	jobject** es;
	int32_t n;
	map_walk(m, &es, &n);
	for (int32_t i = 0; i < n; ++i) if (jrt_Objects_equals_OO(v, ((jrt_node*)es[i])->value)) return true;
	return false;
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
jobject* jrt_HashSet_new_I(int32_t capacity) { return view(jrt_HashMap_new_I(capacity), 0); }
jobject* jrt_HashSet_new_IF(int32_t capacity, float load) { return view(jrt_HashMap_new_IF(capacity, load), 0); }
static jobject* add_all(jobject* set, jobject* from) {
	jobject* it = jrt_Collection_iterator_(from);
	while (jrt_Iterator_hasNext_(it)) jrt_Collection_add_O(set, jrt_Iterator_next_(it));
	return set;
}
bool jrt_Collection_addAll_O(jobject* c, jobject* from) {
	jobject* copy = jrt_ArrayList_new_O(from);               /* (a collection may be added to itself) */
	bool changed = false;
	jobject* it = jrt_Collection_iterator_(copy);
	while (jrt_Iterator_hasNext_(it)) changed = jrt_Collection_add_O(c, jrt_Iterator_next_(it)) || changed;
	return changed;
}
bool jrt_Collection_removeAll_O(jobject* c, jobject* other) {
	jrt_nn(other);
	jobject* kept = jrt_ArrayList_new_();                    /* what stays, in its order */
	bool changed = false;
	jobject* it = jrt_Collection_iterator_(c);
	while (jrt_Iterator_hasNext_(it)) {
		jobject* x = jrt_Iterator_next_(it);
		if (jrt_Collection_contains_O(other, x)) changed = true;
		else jrt_Collection_add_O(kept, x);
	}
	if (!changed) return false;
	jrt_Collection_clear_(c);
	add_all(c, kept);
	return true;
}
jobject* jrt_TreeSet_first_(jobject* s) {
	const jrt_MapSet* set = (const jrt_MapSet*)jrt_nn(s);
	if (!set->m->head) jrt_throw_new(&jrt_NoSuchElementException_class, NULL);
	return set_item(set, (jobject*)set->m->head);
}
int64_t jrt_System_currentTimeMillis_(void) {
#ifdef _WIN32
	struct _timeb now;
	_ftime(&now);
	return (int64_t)now.time * 1000 + now.millitm;
#else
	struct timespec now;
	timespec_get(&now, TIME_UTC);
	return (int64_t)now.tv_sec * 1000 + now.tv_nsec / 1000000;
#endif
}
bool jrt_Double_isInfinite_D(double v) { return v == INFINITY || v == -INFINITY; }
int64_t jrt_Long_parseLong_Str(jstring* s) {
	jrt_nn(s);
	int32_t i = s->len > 0 && (s->s[0] == '-' || s->s[0] == '+') ? 1 : 0;
	const bool negative = s->len > 0 && s->s[0] == '-';
	uint64_t v = 0;
	const uint64_t limit = negative ? (uint64_t)1 << 63 : ((uint64_t)1 << 63) - 1;
	bool good = i < s->len;
	for (; good && i < s->len; ++i) {
		const unsigned digit = (unsigned)s->s[i] - '0';
		good = digit <= 9 && v <= (limit - digit) / 10;
		if (good) v = v * 10 + digit;
	}
	if (!good) jrt_throw_new(&jrt_NumberFormatException_class, "For input string");
	return negative ? (int64_t)(0 - v) : (int64_t)v;
}
float jrt_Float_parseFloat_Str(jstring* s) { return (float)jrt_Double_parseDouble_Str(s); }
jobject* jrt_Character_valueOf_C(char16_t c) { return jrt_box_C(c); }
int64_t jrt_System_nanoTime_(void) { return (int64_t)((double)clock() * (1000000000.0 / CLOCKS_PER_SEC)); }
void jrt_System_exit_I(int32_t status) {
	fflush(NULL);
	exit(status);
}
jobject* jrt_Object_new_(void) { return (jobject*)jrt_alloc(&jrt_Object_class); }
jstring* jrt_String_replace_OO(jstring* s, jobject* target, jobject* with) {
	jrt_nn(s);
	jstring* from = jrt_str_O((jobject*)jrt_nn(target));
	jstring* to = jrt_str_O((jobject*)jrt_nn(with));
	jrt_StringBuilder* out = jrt_StringBuilder_new_();
	int32_t at = 0;
	if (from->len == 0) {                                    /* an empty target: the new text around every character */
		for (; at < s->len; ++at) {
			jrt_StringBuilder_append_Str(out, to);
			jrt_StringBuilder_append_Str(out, jrt_String_substring_II(s, at, at + 1));
		}
		jrt_StringBuilder_append_Str(out, to);
		return jrt_StringBuilder_toString_(out);
	}
	for (int32_t i = 0; i + from->len <= s->len;) {
		if (memcmp(s->s + i, from->s, (size_t)from->len * sizeof(char16_t)) != 0) {
			++i;
			continue;
		}
		jrt_StringBuilder_append_Str(out, jrt_String_substring_II(s, at, i));
		jrt_StringBuilder_append_Str(out, to);
		i += from->len;
		at = i;
	}
	jrt_StringBuilder_append_Str(out, jrt_String_substring_I(s, at));
	return jrt_StringBuilder_toString_(out);
}
int32_t jrt_String_indexOf_StrI(jstring* s, jstring* t, int32_t from) {
	jrt_nn(s);
	jrt_nn(t);
	if (from < 0) from = 0;
	for (int32_t i = from; i + t->len <= s->len; ++i) {
		if (memcmp(s->s + i, t->s, (size_t)t->len * sizeof(char16_t)) == 0) return i;
	}
	return from <= s->len && t->len == 0 ? from : (t->len == 0 ? s->len : -1);
}
int32_t jrt_String_lastIndexOf_Str(jstring* s, jstring* t) {
	jrt_nn(s);
	jrt_nn(t);
	for (int32_t i = s->len - t->len; i >= 0; --i) {
		if (memcmp(s->s + i, t->s, (size_t)t->len * sizeof(char16_t)) == 0) return i;
	}
	return -1;
}
void jrt_Arrays_fill_BAB(jarray* a, int8_t v) {
	for (int32_t i = 0; i < JLEN(a); ++i) ((int8_t*)a->data)[i] = v;
}
/* Arrays.sort(a, comparator): a merge sort, so equal elements keep their order as Java's do */
void jrt_Arrays_sort_OAO(jarray* a, jobject* comparator) {
	const int32_t n = JLEN(a);
	jarray* spare = jrt_new_array(sizeof(jobject*), n);          /* (a collected array: what it holds stays reachable) */
	jobject** from = (jobject**)a->data;
	jobject** into = (jobject**)spare->data;
	for (int32_t width = 1; width < n; width *= 2) {
		for (int32_t low = 0; low < n; low += 2 * width) {
			const int32_t mid = low + width < n ? low + width : n;
			const int32_t high = low + 2 * width < n ? low + 2 * width : n;
			int32_t l = low, r = mid, k = low;
			while (l < mid && r < high) {
				const bool right = comparator ? jrt_Comparator_compare_OO(comparator, from[l], from[r]) > 0 : jrt_natural(from[l], from[r]) > 0;
				into[k++] = right ? from[r++] : from[l++];
			}
			while (l < mid) into[k++] = from[l++];
			while (r < high) into[k++] = from[r++];
		}
		jobject** swapped = from;
		from = into;
		into = swapped;
	}
	if (from != (jobject**)a->data) memcpy(a->data, from, (size_t)n * sizeof(jobject*));
}
jobject* jrt_HashSet_new_O(jobject* from) {                  /* Java's size for a copy */
	const int32_t wanted = (int32_t)((float)jrt_Collection_size_(from) / 0.75f) + 1;
	return add_all(jrt_HashSet_new_I(wanted > 16 ? wanted : 16), from);
}
jobject* jrt_LinkedHashSet_new_(void) { return view(jrt_LinkedHashMap_new_(), 0); }
jobject* jrt_LinkedHashSet_new_I(int32_t capacity) { return view(jrt_LinkedHashMap_new_I(capacity), 0); }
jobject* jrt_LinkedHashSet_new_IF(int32_t capacity, float load) { return view(jrt_LinkedHashMap_new_IF(capacity, load), 0); }
jobject* jrt_LinkedHashSet_new_O(jobject* from) { return add_all(jrt_LinkedHashSet_new_(), from); }
jobject* jrt_TreeSet_new_(void) { return view(jrt_TreeMap_new_(), 0); }
jobject* jrt_TreeSet_new_O(jobject* x) {                     /* a collection to copy, or a Comparator */
	if (x && (is_al(x) || x->cls == &jrt_MapSet_class)) return add_all(jrt_TreeSet_new_(), x);
	return view(jrt_TreeMap_new_O(x), 0);
}
jobject* jrt_Set_of(int n, ...) {
	jrt_HashMap* m = new_map(1);
	va_list ap;
	va_start(ap, n);
	for (int i = 0; i < n; ++i) {
		jobject* x = va_arg(ap, jobject*);
		if (!x) jrt_throw_npe();
		if (map_find(m, x)) jrt_throw((jobject*)jrt_new_throwable(&jrt_IllegalArgumentException_class, jrt_concat(2, str8("duplicate element: "), jrt_str_O(x)), 0));
		map_insert(m, x, jrt_box_Z(true), false);
	}
	va_end(ap);
	m->immutable = m->shuffled = true;
	return view((jobject*)m, 0);
}
jobject* jrt_Map_of(int n, ...) {
	jrt_HashMap* m = new_map(1);
	va_list ap;
	va_start(ap, n);
	for (int i = 0; i + 1 < n; i += 2) {
		jobject* k = va_arg(ap, jobject*);
		jobject* v = va_arg(ap, jobject*);
		if (!k || !v) jrt_throw_npe();
		if (map_find(m, k)) jrt_throw((jobject*)jrt_new_throwable(&jrt_IllegalArgumentException_class, jrt_concat(2, str8("duplicate key: "), jrt_str_O(k)), 0));
		map_insert(m, k, v, false);
	}
	va_end(ap);
	m->immutable = m->shuffled = true;
	return (jobject*)m;
}
int32_t jrt_string_case(jstring* s, int n, jstring* const* labels) {
	jrt_nn(s);
	for (int i = 0; i < n; ++i) if (jrt_String_equals_O(s, (jobject*)labels[i])) return i;
	return -1;
}

/* ------------------------------------------------------------------------------------------- java.util.Random */
typedef struct jrt_Random { jobject hdr; uint64_t seed; double spare; bool has_spare; } jrt_Random;
static const jclass jrt_Random_class = {"java.util.Random", &jrt_Object_class, sizeof(jrt_Random), &jrt_Object_vt, 0, 0, 0};
enum { RANDOM_ADD = 0xB };
#define RANDOM_MULTIPLIER 0x5DEECE66DULL
#define RANDOM_MASK ((1ULL << 48) - 1)
static jrt_Random* as_random(jobject* o) { return (jrt_Random*)jrt_cast((jobject*)jrt_nn(o), &jrt_Random_class); }
static int32_t random_next(jrt_Random* r, int bits) {
	r->seed = (r->seed * RANDOM_MULTIPLIER + RANDOM_ADD) & RANDOM_MASK;
	return (int32_t)(uint32_t)(r->seed >> (48 - bits));
}
void jrt_Random_setSeed_J(jobject* o, int64_t seed) {
	jrt_Random* r = as_random(o);
	r->seed = ((uint64_t)seed ^ RANDOM_MULTIPLIER) & RANDOM_MASK;
	r->has_spare = false;
}
jobject* jrt_Random_new_J(int64_t seed) {
	jobject* o = (jobject*)jrt_alloc(&jrt_Random_class);
	jrt_Random_setSeed_J(o, seed);
	return o;
}
jobject* jrt_Random_new_(void) {                             /* (Java's unseeded generator differs on every run too) */
	static uint64_t uniquifier = 8682522807148205381ULL;
	uniquifier *= 1181783497276652981ULL;
	return jrt_Random_new_J((int64_t)(uniquifier ^ (uint64_t)time(0) ^ (uint64_t)clock()));
}
int32_t jrt_Random_nextInt_(jobject* o) { return random_next(as_random(o), 32); }
int32_t jrt_Random_nextInt_I(jobject* o, int32_t bound) {
	jrt_Random* g = as_random(o);
	if (bound <= 0) jrt_throw_new(&jrt_IllegalArgumentException_class, "bound must be positive");
	int32_t r = random_next(g, 31);
	const uint32_t m = (uint32_t)bound - 1;
	if (((uint32_t)bound & m) == 0) return (int32_t)(((int64_t)bound * (int64_t)r) >> 31);
	for (int32_t u = r; (int32_t)((uint32_t)u - (uint32_t)(r = u % bound) + m) < 0; u = random_next(g, 31)) {}
	return r;
}
int64_t jrt_Random_nextLong_(jobject* o) {
	jrt_Random* g = as_random(o);
	const int64_t high = random_next(g, 32);
	const int64_t low = random_next(g, 32);
	return (int64_t)(((uint64_t)high << 32) + (uint64_t)low);
}
bool jrt_Random_nextBoolean_(jobject* o) { return random_next(as_random(o), 1) != 0; }
float jrt_Random_nextFloat_(jobject* o) { return (float)random_next(as_random(o), 24) / (float)(1 << 24); }
double jrt_Random_nextDouble_(jobject* o) {
	jrt_Random* g = as_random(o);
	const int64_t high = random_next(g, 26);
	const int64_t low = random_next(g, 27);
	return (double)((high << 27) + low) * 0x1.0p-53;
}
double jrt_Random_nextGaussian_(jobject* o) {
	jrt_Random* g = as_random(o);
	if (g->has_spare) { g->has_spare = false; return g->spare; }
	double v1, v2, s;
	do {
		v1 = 2 * jrt_Random_nextDouble_(o) - 1;
		v2 = 2 * jrt_Random_nextDouble_(o) - 1;
		s = v1 * v1 + v2 * v2;
	} while (s >= 1 || s == 0);
	const double scale = sqrt(-2 * log(s) / s);
	g->spare = v2 * scale;
	g->has_spare = true;
	return v1 * scale;
}

jstring* jrt_String_join(jstring* sep, jobject* parts) {
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	jobject* it = jrt_Collection_iterator_(parts);
	bool first = true;
	while (jrt_Iterator_hasNext_(it)) { if (!first) jrt_StringBuilder_append_Str(b, sep); first = false; jrt_StringBuilder_append_Str(b, jrt_str_O(jrt_Iterator_next_(it))); }
	return jrt_StringBuilder_toString_(b);
}

/* ------------------------------------------------------------------------------------------- streams
 * Lazy, as Java's are: each stage pulls one element from the stage before it, so a filter or a map runs only for what the end of the
 * pipeline asks for, in Java's order, and an endless source with a limit ends. A stage is used once. Stream and IntStream are one
 * structure: `ints` says which half of an item is the value.
 */
typedef struct jitem { jobject* o; int32_t v; } jitem;
typedef struct jrt_Stream {
	jobject hdr;
	bool (*pull)(struct jrt_Stream*, jitem*);
	struct jrt_Stream* up;
	jobject* fn;
	jobject* held;
	int64_t i, n;
	bool ints, used;
} jrt_Stream;
static const jclass jrt_Stream_class = {"java.util.stream.ReferencePipeline", &jrt_Object_class, sizeof(jrt_Stream), &jrt_Object_vt, 0, 0, 0};

static jrt_Stream* stream_used(jobject* o) {
	jrt_Stream* s = (jrt_Stream*)jrt_nn(o);
	if (s->used) jrt_throw_new(&jrt_IllegalStateException_class, "stream has already been operated upon or closed");
	s->used = true;
	return s;
}
static jobject* stage(bool (*pull)(jrt_Stream*, jitem*), jobject* up, jobject* fn, bool ints, int64_t n) {
	jrt_Stream* s = (jrt_Stream*)jrt_alloc(&jrt_Stream_class);
	s->pull = pull;
	s->up = up ? stream_used(up) : 0;
	s->fn = fn;
	s->ints = ints;
	s->n = n;
	return (jobject*)s;
}
#define NEXT(s, x) ((s)->up->pull((s)->up, (x)))
/* a lambda is made for the interface its parameter names: UnaryOperator is a Function, BinaryOperator a BiFunction */
static jobject* call1(jobject* f, jobject* x) {
	return jrt_instanceof(f, &jrt_UnaryOperator_class) ? jrt_UnaryOperator_apply_O(f, x) : jrt_Function_apply_O(f, x);
}
static jobject* call2(jobject* f, jobject* a, jobject* b) {
	return jrt_instanceof(f, &jrt_BinaryOperator_class) ? jrt_BinaryOperator_apply_OO(f, a, b) : jrt_BiFunction_apply_OO(f, a, b);
}

static bool p_iter(jrt_Stream* s, jitem* x) {
	if (!jrt_Iterator_hasNext_(s->held)) return false;
	x->o = jrt_Iterator_next_(s->held);
	return true;
}
static bool p_range(jrt_Stream* s, jitem* x) { if (s->i >= s->n) return false; x->v = (int32_t)s->i++; return true; }
static bool p_array(jrt_Stream* s, jitem* x) {
	jarray* a = (jarray*)s->held;
	if (s->i >= a->length) return false;
	if (s->ints) x->v = ((int32_t*)a->data)[s->i++]; else x->o = ((jobject**)a->data)[s->i++];
	return true;
}
static bool p_filter(jrt_Stream* s, jitem* x) { while (NEXT(s, x)) if (jrt_Predicate_test_O(s->fn, x->o)) return true; return false; }
static bool p_ifilter(jrt_Stream* s, jitem* x) { while (NEXT(s, x)) if (jrt_IntPredicate_test_I(s->fn, x->v)) return true; return false; }
static bool p_map(jrt_Stream* s, jitem* x) { if (!NEXT(s, x)) return false; x->o = call1(s->fn, x->o); return true; }
static bool p_imap(jrt_Stream* s, jitem* x) { if (!NEXT(s, x)) return false; x->v = jrt_IntUnaryOperator_applyAsInt_I(s->fn, x->v); return true; }
static bool p_to_int(jrt_Stream* s, jitem* x) { if (!NEXT(s, x)) return false; x->v = jrt_ToIntFunction_applyAsInt_O(s->fn, x->o); return true; }
static bool p_to_obj(jrt_Stream* s, jitem* x) { if (!NEXT(s, x)) return false; x->o = jrt_IntFunction_apply_I(s->fn, x->v); return true; }
static bool p_boxed(jrt_Stream* s, jitem* x) { if (!NEXT(s, x)) return false; x->o = jrt_box_I(x->v); return true; }
static bool p_peek(jrt_Stream* s, jitem* x) { if (!NEXT(s, x)) return false; jrt_Consumer_accept_O(s->fn, x->o); return true; }
static bool p_ipeek(jrt_Stream* s, jitem* x) { if (!NEXT(s, x)) return false; jrt_IntConsumer_accept_I(s->fn, x->v); return true; }
static bool p_limit(jrt_Stream* s, jitem* x) { if (s->i >= s->n) return false; ++s->i; return NEXT(s, x); }
static bool p_skip(jrt_Stream* s, jitem* x) {
	for (; s->i < s->n; ++s->i) if (!NEXT(s, x)) return false;
	return NEXT(s, x);
}
static bool p_flat(jrt_Stream* s, jitem* x) {
	for (;;) {
		jrt_Stream* inner = (jrt_Stream*)s->held;
		if (inner && inner->pull(inner, x)) return true;
		s->held = 0;
		if (!NEXT(s, x)) return false;
		jobject* made = call1(s->fn, x->o);
		if (made) s->held = (jobject*)stream_used((jobject*)jrt_cast(made, &jrt_Stream_class));
	}
}
static bool p_sorted(jrt_Stream* s, jitem* x) {
	if (!s->held) {
		jobject* l = jrt_ArrayList_new_();
		jitem y = {0, 0};
		while (NEXT(s, &y)) jrt_Collection_add_O(l, s->ints ? jrt_box_I(y.v) : y.o);
		jrt_List_sort_O(l, s->fn);
		s->held = l;
	}
	const jrt_ArrayList* l = (const jrt_ArrayList*)s->held;
	if (s->i >= l->size) return false;
	jobject* v = l->items[s->i++];
	if (s->ints) x->v = jrt_unbox_I(v); else x->o = v;
	return true;
}
static bool p_distinct(jrt_Stream* s, jitem* x) {
	if (!s->held) s->held = jrt_LinkedHashSet_new_();
	while (NEXT(s, x)) if (jrt_Collection_add_O(s->held, s->ints ? jrt_box_I(x->v) : x->o)) return true;
	return false;
}

jobject* jrt_Collection_stream_(jobject* c) {
	jrt_Stream* s = (jrt_Stream*)stage(p_iter, 0, 0, false, 0);
	s->held = jrt_Collection_iterator_((jobject*)jrt_nn(c));
	return (jobject*)s;
}
static jobject* array_stream(jarray* a, bool ints) {
	jrt_Stream* s = (jrt_Stream*)stage(p_array, 0, 0, ints, 0);
	s->held = (jobject*)jrt_nn(a);
	return (jobject*)s;
}
jobject* jrt_Arrays_stream_OA(jarray* a) { return array_stream(a, false); }
jobject* jrt_Arrays_stream_IA(jarray* a) { return array_stream(a, true); }
jobject* jrt_IntStream_range_II(int32_t a, int32_t b) { jrt_Stream* s = (jrt_Stream*)stage(p_range, 0, 0, true, b); s->i = a; return (jobject*)s; }
jobject* jrt_IntStream_rangeClosed_II(int32_t a, int32_t b) { jrt_Stream* s = (jrt_Stream*)stage(p_range, 0, 0, true, (int64_t)b + 1); s->i = a; return (jobject*)s; }

static int64_t count_of(jobject* o, bool ints) { (void)ints; jrt_Stream* s = stream_used(o); jitem x = {0, 0}; int64_t n = 0; while (s->pull(s, &x)) ++n; return n; }
static int64_t not_negative(int64_t n) {
	if (n < 0) { char b[48]; snprintf(b, sizeof b, "%lld", (long long)n); jrt_throw_new(&jrt_IllegalArgumentException_class, b); }
	return n;
}

jobject* jrt_Stream_filter_O(jobject* s, jobject* f) { return stage(p_filter, s, f, false, 0); }
jobject* jrt_Stream_map_O(jobject* s, jobject* f) { return stage(p_map, s, f, false, 0); }
jobject* jrt_Stream_mapToInt_O(jobject* s, jobject* f) { return stage(p_to_int, s, f, true, 0); }
jobject* jrt_Stream_flatMap_O(jobject* s, jobject* f) { return stage(p_flat, s, f, false, 0); }
jobject* jrt_Stream_peek_O(jobject* s, jobject* f) { return stage(p_peek, s, f, false, 0); }
jobject* jrt_Stream_limit_J(jobject* s, int64_t n) { return stage(p_limit, s, 0, false, not_negative(n)); }
jobject* jrt_Stream_skip_J(jobject* s, int64_t n) { return stage(p_skip, s, 0, false, not_negative(n)); }
jobject* jrt_Stream_sorted_(jobject* s) { return stage(p_sorted, s, 0, false, 0); }
jobject* jrt_Stream_sorted_O(jobject* s, jobject* c) { return stage(p_sorted, s, c, false, 0); }
jobject* jrt_Stream_distinct_(jobject* s) { return stage(p_distinct, s, 0, false, 0); }
int64_t jrt_Stream_count_(jobject* s) { return count_of(s, false); }
void jrt_Stream_forEach_O(jobject* o, jobject* f) { jrt_Stream* s = stream_used(o); jitem x = {0, 0}; while (s->pull(s, &x)) jrt_Consumer_accept_O(f, x.o); }
jobject* jrt_Stream_toList_(jobject* o) {
	jrt_Stream* s = stream_used(o);
	jobject* l = jrt_ArrayList_new_();
	jitem x = {0, 0};
	while (s->pull(s, &x)) jrt_Collection_add_O(l, x.o);
	((jrt_ArrayList*)l)->immutable = true;
	return l;
}
jobject* jrt_Stream_reduce_OO(jobject* o, jobject* id, jobject* f) {
	jrt_Stream* s = stream_used(o);
	jobject* acc = id;
	jitem x = {0, 0};
	while (s->pull(s, &x)) acc = call2(f, acc, x.o);
	return acc;
}
bool jrt_Stream_anyMatch_O(jobject* o, jobject* p) { jrt_Stream* s = stream_used(o); jitem x = {0, 0}; while (s->pull(s, &x)) if (jrt_Predicate_test_O(p, x.o)) return true; return false; }
bool jrt_Stream_allMatch_O(jobject* o, jobject* p) { jrt_Stream* s = stream_used(o); jitem x = {0, 0}; while (s->pull(s, &x)) if (!jrt_Predicate_test_O(p, x.o)) return false; return true; }
bool jrt_Stream_noneMatch_O(jobject* o, jobject* p) { return !jrt_Stream_anyMatch_O(o, p); }

jobject* jrt_IntStream_filter_O(jobject* s, jobject* f) { return stage(p_ifilter, s, f, true, 0); }
jobject* jrt_IntStream_map_O(jobject* s, jobject* f) { return stage(p_imap, s, f, true, 0); }
jobject* jrt_IntStream_mapToObj_O(jobject* s, jobject* f) { return stage(p_to_obj, s, f, false, 0); }
jobject* jrt_IntStream_boxed_(jobject* s) { return stage(p_boxed, s, 0, false, 0); }
jobject* jrt_IntStream_peek_O(jobject* s, jobject* f) { return stage(p_ipeek, s, f, true, 0); }
jobject* jrt_IntStream_limit_J(jobject* s, int64_t n) { return stage(p_limit, s, 0, true, not_negative(n)); }
jobject* jrt_IntStream_skip_J(jobject* s, int64_t n) { return stage(p_skip, s, 0, true, not_negative(n)); }
jobject* jrt_IntStream_sorted_(jobject* s) { return stage(p_sorted, s, 0, true, 0); }
jobject* jrt_IntStream_distinct_(jobject* s) { return stage(p_distinct, s, 0, true, 0); }
int32_t jrt_IntStream_sum_(jobject* o) { jrt_Stream* s = stream_used(o); jitem x = {0, 0}; uint32_t t = 0; while (s->pull(s, &x)) t += (uint32_t)x.v; return (int32_t)t; }
int64_t jrt_IntStream_count_(jobject* s) { return count_of(s, true); }
void jrt_IntStream_forEach_O(jobject* o, jobject* f) { jrt_Stream* s = stream_used(o); jitem x = {0, 0}; while (s->pull(s, &x)) jrt_IntConsumer_accept_I(f, x.v); }
int32_t jrt_IntStream_reduce_IO(jobject* o, int32_t id, jobject* f) {
	jrt_Stream* s = stream_used(o);
	int32_t acc = id;
	jitem x = {0, 0};
	while (s->pull(s, &x)) acc = jrt_IntBinaryOperator_applyAsInt_II(f, acc, x.v);
	return acc;
}
bool jrt_IntStream_anyMatch_O(jobject* o, jobject* p) { jrt_Stream* s = stream_used(o); jitem x = {0, 0}; while (s->pull(s, &x)) if (jrt_IntPredicate_test_I(p, x.v)) return true; return false; }
bool jrt_IntStream_allMatch_O(jobject* o, jobject* p) { jrt_Stream* s = stream_used(o); jitem x = {0, 0}; while (s->pull(s, &x)) if (!jrt_IntPredicate_test_I(p, x.v)) return false; return true; }
bool jrt_IntStream_noneMatch_O(jobject* o, jobject* p) { return !jrt_IntStream_anyMatch_O(o, p); }
jarray* jrt_IntStream_toArray_(jobject* o) {
	jrt_Stream* s = stream_used(o);
	jobject* l = jrt_ArrayList_new_();
	jitem x = {0, 0};
	while (s->pull(s, &x)) jrt_Collection_add_O(l, jrt_box_I(x.v));
	const int32_t n = jrt_Collection_size_(l);
	jarray* a = jrt_new_array(sizeof(int32_t), n);
	for (int32_t i = 0; i < n; ++i) ((int32_t*)a->data)[i] = jrt_unbox_I(jrt_List_get_I(l, i));
	return a;
}

/* Collectors: what to do with the elements, run by collect */
enum { C_LIST, C_SET, C_JOIN, C_COUNT, C_GROUP, C_SUM };
typedef struct jrt_Collector { jobject hdr; int kind; jobject* a; jobject* b; jobject* c; } jrt_Collector;
static const jclass jrt_Collector_class = {"java.util.stream.Collectors.CollectorImpl", &jrt_Object_class, sizeof(jrt_Collector), &jrt_Object_vt, 0, 0, 0};
static jobject* collector(int kind, jobject* a, jobject* b, jobject* c) {
	jrt_Collector* r = (jrt_Collector*)jrt_alloc(&jrt_Collector_class);
	r->kind = kind;
	r->a = a;
	r->b = b;
	r->c = c;
	return (jobject*)r;
}
jobject* jrt_Collectors_toList_(void) { return collector(C_LIST, 0, 0, 0); }
jobject* jrt_Collectors_toSet_(void) { return collector(C_SET, 0, 0, 0); }
jobject* jrt_Collectors_joining_(void) { return collector(C_JOIN, 0, 0, 0); }
jobject* jrt_Collectors_joining_O(jobject* sep) { return collector(C_JOIN, (jobject*)jrt_nn(sep), 0, 0); }
jobject* jrt_Collectors_joining_OOO(jobject* sep, jobject* pre, jobject* post) { return collector(C_JOIN, (jobject*)jrt_nn(sep), (jobject*)jrt_nn(pre), (jobject*)jrt_nn(post)); }
jobject* jrt_Collectors_counting_(void) { return collector(C_COUNT, 0, 0, 0); }
jobject* jrt_Collectors_summingInt_O(jobject* f) { return collector(C_SUM, (jobject*)jrt_nn(f), 0, 0); }
jobject* jrt_Collectors_groupingBy_O(jobject* key) { return collector(C_GROUP, (jobject*)jrt_nn(key), 0, 0); }
jobject* jrt_Collectors_groupingBy_OO(jobject* key, jobject* down) { return collector(C_GROUP, (jobject*)jrt_nn(key), (jobject*)jrt_nn(down), 0); }

jobject* jrt_Stream_collect_O(jobject* o, jobject* how) {
	const jrt_Collector* c = (const jrt_Collector*)jrt_cast((jobject*)jrt_nn(how), &jrt_Collector_class);
	jrt_Stream* s = stream_used(o);
	jitem x = {0, 0};
	switch (c->kind) {
	case C_LIST: case C_SET: {
		jobject* r = c->kind == C_LIST ? jrt_ArrayList_new_() : jrt_HashSet_new_();
		while (s->pull(s, &x)) jrt_Collection_add_O(r, x.o);
		return r;
	}
	case C_JOIN: {
		jrt_StringBuilder* b = jrt_StringBuilder_new_();
		if (c->b) jrt_StringBuilder_append_Str(b, jrt_str_O(c->b));
		for (bool first = true; s->pull(s, &x); first = false) {
			if (!first && c->a) jrt_StringBuilder_append_Str(b, jrt_str_O(c->a));
			jrt_StringBuilder_append_Str(b, jrt_str_O(x.o));
		}
		if (c->c) jrt_StringBuilder_append_Str(b, jrt_str_O(c->c));
		return (jobject*)jrt_StringBuilder_toString_(b);
	}
	case C_COUNT: {
		int64_t n = 0;
		while (s->pull(s, &x)) ++n;
		return jrt_box_J(n);
	}
	case C_SUM: {
		uint32_t t = 0;
		while (s->pull(s, &x)) t += (uint32_t)jrt_ToIntFunction_applyAsInt_O(c->a, x.o);
		return jrt_box_I((int32_t)t);
	}
	default: {
		jobject* m = jrt_HashMap_new_();
		while (s->pull(s, &x)) {
			jobject* k = call1(c->a, x.o);
			if (!k) jrt_throw_new(&jrt_NullPointerException_class, "element cannot be mapped to a null key");
			jobject* l = jrt_Map_get_O(m, k);
			if (!l) { l = jrt_ArrayList_new_(); jrt_Map_put_OO(m, k, l); }
			jrt_Collection_add_O(l, x.o);
		}
		if (c->b) {
			jobject* it = jrt_Collection_iterator_(jrt_Map_entrySet_(m));
			while (jrt_Iterator_hasNext_(it)) {
				jobject* e = jrt_Iterator_next_(it);
				jrt_Map_put_OO(m, jrt_MapEntry_getKey_(e), jrt_Stream_collect_O(jrt_Collection_stream_(jrt_MapEntry_getValue_(e)), c->b));
			}
		}
		return m;
	}
	}
}

/* ------------------------------------------------------------------------------------------- Optional
 * One structure for the three classes; the class says which half holds the value. */
typedef struct jopt { jobject hdr; jobject* o; double d; int32_t i; bool has; } jopt;
static jstring* opt_toString(jobject* s);
static jstring* opti_toString(jobject* s);
static jstring* optd_toString(jobject* s);
static bool opt_equals(jobject* a, jobject* b);
static int32_t opt_hashCode(jobject* s);
static const jobject_vt opt_vt = {opt_toString, opt_equals, opt_hashCode};
static const jobject_vt opti_vt = {opti_toString, opt_equals, opt_hashCode};
static const jobject_vt optd_vt = {optd_toString, opt_equals, opt_hashCode};
static const jclass opt_class = {"java.util.Optional", &jrt_Object_class, sizeof(jopt), &opt_vt, 0, 0, 0};
static const jclass opti_class = {"java.util.OptionalInt", &jrt_Object_class, sizeof(jopt), &opti_vt, 0, 0, 0};
static const jclass optd_class = {"java.util.OptionalDouble", &jrt_Object_class, sizeof(jopt), &optd_vt, 0, 0, 0};
static jstring* opt_text(const char* name, bool has, jstring* value) {
	jrt_StringBuilder* b = jrt_StringBuilder_new_();
	sb_add8(b, name);
	if (!has) { sb_add8(b, ".empty"); return jrt_StringBuilder_toString_(b); }
	jrt_StringBuilder_append_C(b, '[');
	jrt_StringBuilder_append_Str(b, value);
	jrt_StringBuilder_append_C(b, ']');
	return jrt_StringBuilder_toString_(b);
}
static jstring* opt_toString(jobject* s) { const jopt* p = (const jopt*)s; return opt_text("Optional", p->has, p->has ? jrt_str_O(p->o) : 0); }
static jstring* opti_toString(jobject* s) { const jopt* p = (const jopt*)s; return opt_text("OptionalInt", p->has, p->has ? jrt_str_I(p->i) : 0); }
static jstring* optd_toString(jobject* s) { const jopt* p = (const jopt*)s; return opt_text("OptionalDouble", p->has, p->has ? jrt_str_D(p->d) : 0); }
static bool opt_equals(jobject* a, jobject* b) {
	if (a == b) return true;
	if (!b || a->cls != b->cls) return false;
	const jopt *x = (const jopt*)a, *y = (const jopt*)b;
	if (x->has != y->has) return false;
	if (!x->has) return true;
	if (a->cls == &opti_class) return x->i == y->i;
	if (a->cls == &optd_class) return jrt_Double_compare_DD(x->d, y->d) == 0;
	return jrt_Objects_equals_OO(x->o, y->o);
}
static int32_t opt_hashCode(jobject* s) {
	const jopt* p = (const jopt*)s;
	if (!p->has) return 0;
	if (s->cls == &opti_class) return p->i;
	if (s->cls == &optd_class) return jrt_Double_hashCode_D(p->d);
	return jrt_Objects_hashCode_O(p->o);
}
static jopt* opt_new(const jclass* c, bool has) { jopt* p = (jopt*)jrt_alloc(c); p->has = has; return p; }
static const jopt* opt_of(jobject* o, const jclass* c) { return (const jopt*)jrt_cast((jobject*)jrt_nn(o), c); }
static _Noreturn void opt_none(void) { jrt_throw_new(&jrt_NoSuchElementException_class, "No value present"); }
static jobject* opt_int(bool has, int32_t v) { jopt* p = opt_new(&opti_class, has); p->i = v; return (jobject*)p; }

jobject* jrt_Optional_empty_(void) { return (jobject*)opt_new(&opt_class, false); }
jobject* jrt_Optional_of_O(jobject* v) { jrt_nn(v); jopt* p = opt_new(&opt_class, true); p->o = v; return (jobject*)p; }
jobject* jrt_Optional_ofNullable_O(jobject* v) { return v ? jrt_Optional_of_O(v) : jrt_Optional_empty_(); }
bool jrt_Optional_isPresent_(jobject* o) { return opt_of(o, &opt_class)->has; }
bool jrt_Optional_isEmpty_(jobject* o) { return !opt_of(o, &opt_class)->has; }
jobject* jrt_Optional_get_(jobject* o) { const jopt* p = opt_of(o, &opt_class); if (!p->has) opt_none(); return p->o; }
jobject* jrt_Optional_orElse_O(jobject* o, jobject* other) { const jopt* p = opt_of(o, &opt_class); return p->has ? p->o : other; }
jobject* jrt_Optional_orElseGet_O(jobject* o, jobject* f) { const jopt* p = opt_of(o, &opt_class); return p->has ? p->o : jrt_Supplier_get_(f); }
jobject* jrt_Optional_map_O(jobject* o, jobject* f) {
	const jopt* p = opt_of(o, &opt_class);
	jrt_nn(f);
	return p->has ? jrt_Optional_ofNullable_O(call1(f, p->o)) : o;
}
jobject* jrt_Optional_filter_O(jobject* o, jobject* f) {
	const jopt* p = opt_of(o, &opt_class);
	jrt_nn(f);
	return !p->has || jrt_Predicate_test_O(f, p->o) ? o : jrt_Optional_empty_();
}
void jrt_Optional_ifPresent_O(jobject* o, jobject* f) { const jopt* p = opt_of(o, &opt_class); if (p->has) jrt_Consumer_accept_O(f, p->o); }
bool jrt_OptionalInt_isPresent_(jobject* o) { return opt_of(o, &opti_class)->has; }
int32_t jrt_OptionalInt_getAsInt_(jobject* o) { const jopt* p = opt_of(o, &opti_class); if (!p->has) opt_none(); return p->i; }
int32_t jrt_OptionalInt_orElse_I(jobject* o, int32_t other) { const jopt* p = opt_of(o, &opti_class); return p->has ? p->i : other; }
bool jrt_OptionalDouble_isPresent_(jobject* o) { return opt_of(o, &optd_class)->has; }
double jrt_OptionalDouble_getAsDouble_(jobject* o) { const jopt* p = opt_of(o, &optd_class); if (!p->has) opt_none(); return p->d; }
double jrt_OptionalDouble_orElse_D(jobject* o, double other) { const jopt* p = opt_of(o, &optd_class); return p->has ? p->d : other; }

/* the terminal stages that answer with an Optional */
jobject* jrt_Stream_findFirst_(jobject* o) {
	jrt_Stream* s = stream_used(o);
	jitem x = {0, 0};
	return s->pull(s, &x) ? jrt_Optional_of_O(x.o) : jrt_Optional_empty_();
}
jobject* jrt_Stream_reduce_O(jobject* o, jobject* f) {
	jrt_Stream* s = stream_used(o);
	jitem x = {0, 0};
	if (!s->pull(s, &x)) return jrt_Optional_empty_();
	jobject* acc = x.o;
	while (s->pull(s, &x)) acc = call2(f, acc, x.o);
	return jrt_Optional_of_O(acc);
}
/* as Java's BinaryOperator.minBy and maxBy: on a tie the earlier element stays */
static jobject* best_of(jobject* o, jobject* c, int sign) {
	jrt_nn(c);
	jrt_Stream* s = stream_used(o);
	jitem x = {0, 0};
	if (!s->pull(s, &x)) return jrt_Optional_empty_();
	jobject* acc = x.o;
	while (s->pull(s, &x)) if (sign * jrt_Comparator_compare_OO(c, acc, x.o) < 0) acc = x.o;
	return jrt_Optional_of_O(acc);
}
jobject* jrt_Stream_min_O(jobject* o, jobject* c) { return best_of(o, c, -1); }
jobject* jrt_Stream_max_O(jobject* o, jobject* c) { return best_of(o, c, 1); }
static bool p_iiterate(jrt_Stream* s, jitem* x) {
	if (s->n) s->i = jrt_IntUnaryOperator_applyAsInt_I(s->fn, (int32_t)s->i); else s->n = 1;
	x->v = (int32_t)s->i;
	return true;
}
jobject* jrt_IntStream_iterate_IO(int32_t seed, jobject* f) {
	jrt_nn(f);
	jrt_Stream* s = (jrt_Stream*)stage(p_iiterate, 0, f, true, 0);
	s->i = seed;
	return (jobject*)s;
}
jobject* jrt_IntStream_findFirst_(jobject* o) {
	jrt_Stream* s = stream_used(o);
	jitem x = {0, 0};
	return s->pull(s, &x) ? opt_int(true, x.v) : opt_int(false, 0);
}
static jobject* int_best(jobject* o, bool want_max) {
	jrt_Stream* s = stream_used(o);
	jitem x = {0, 0};
	if (!s->pull(s, &x)) return opt_int(false, 0);
	int32_t acc = x.v;
	while (s->pull(s, &x)) if (want_max ? x.v > acc : x.v < acc) acc = x.v;
	return opt_int(true, acc);
}
jobject* jrt_IntStream_min_(jobject* o) { return int_best(o, false); }
jobject* jrt_IntStream_max_(jobject* o) { return int_best(o, true); }
jobject* jrt_IntStream_average_(jobject* o) {
	jrt_Stream* s = stream_used(o);
	jitem x = {0, 0};
	int64_t n = 0, total = 0;
	while (s->pull(s, &x)) { ++n; total += x.v; }
	jopt* p = opt_new(&optd_class, n > 0);
	if (n > 0) p->d = (double)total / (double)n;
	return (jobject*)p;
}

/* ------------------------------------------------------------------------------------------- Comparator */
enum { CMP_NATURAL, CMP_REVERSE, CMP_KEY, CMP_KEY_BY, CMP_KEY_INT, CMP_REVERSED, CMP_THEN };
typedef struct jcmp { jobject hdr; int kind; jobject* a; jobject* b; } jcmp;
static int32_t cmp_compare(jobject* self, jobject* x, jobject* y) {
	const jcmp* c = (const jcmp*)self;
	switch (c->kind) {
	case CMP_NATURAL: return jrt_natural((jobject*)jrt_nn(x), (jobject*)jrt_nn(y));
	case CMP_REVERSE: return jrt_natural((jobject*)jrt_nn(y), (jobject*)jrt_nn(x));
	case CMP_KEY: { jobject* kx = call1(c->a, x); return jrt_natural(kx, call1(c->a, y)); }
	case CMP_KEY_BY: { jobject* kx = call1(c->a, x); return jrt_Comparator_compare_OO(c->b, kx, call1(c->a, y)); }
	case CMP_KEY_INT: { int32_t kx = jrt_ToIntFunction_applyAsInt_O(c->a, x); return jrt_Integer_compare_II(kx, jrt_ToIntFunction_applyAsInt_O(c->a, y)); }
	case CMP_REVERSED: return jrt_Comparator_compare_OO(c->a, y, x);
	default: { int32_t r = jrt_Comparator_compare_OO(c->a, x, y); return r != 0 ? r : jrt_Comparator_compare_OO(c->b, x, y); }
	}
}
static const jrt_Comparator_vt cmp_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, cmp_compare};
static const jitab cmp_itabs[] = {{&jrt_Comparator_class, &cmp_vt}};
static const jclass cmp_class = {"java.util.Comparator", &jrt_Object_class, sizeof(jcmp), &cmp_vt, JRT_ITABS(cmp_itabs), 0};
static jobject* cmp_new(int kind, jobject* a, jobject* b) {
	jcmp* c = (jcmp*)jrt_alloc(&cmp_class);
	c->kind = kind;
	c->a = a;
	c->b = b;
	return (jobject*)c;
}
jobject* jrt_Comparator_naturalOrder_(void) { return cmp_new(CMP_NATURAL, 0, 0); }
jobject* jrt_Comparator_reverseOrder_(void) { return cmp_new(CMP_REVERSE, 0, 0); }
jobject* jrt_Comparator_comparing_O(jobject* key) { return cmp_new(CMP_KEY, (jobject*)jrt_nn(key), 0); }
jobject* jrt_Comparator_comparing_OO(jobject* key, jobject* by) { return cmp_new(CMP_KEY_BY, (jobject*)jrt_nn(key), (jobject*)jrt_nn(by)); }
jobject* jrt_Comparator_comparingInt_O(jobject* key) { return cmp_new(CMP_KEY_INT, (jobject*)jrt_nn(key), 0); }
jobject* jrt_Comparator_reversed_(jobject* c) { return cmp_new(CMP_REVERSED, (jobject*)jrt_nn(c), 0); }
static jobject* cmp_then(jobject* c, jobject* next) { return cmp_new(CMP_THEN, (jobject*)jrt_nn(c), next); }
jobject* jrt_Comparator_thenComparing_O(jobject* c, jobject* next) {
	jrt_nn(next);
	return cmp_then(c, jrt_instanceof(next, &jrt_Comparator_class) ? next : jrt_Comparator_comparing_O(next));
}
jobject* jrt_Comparator_thenComparing_OO(jobject* c, jobject* key, jobject* by) { return cmp_then(c, jrt_Comparator_comparing_OO(key, by)); }
jobject* jrt_Comparator_thenComparingInt_O(jobject* c, jobject* key) { return cmp_then(c, jrt_Comparator_comparingInt_O(key)); }

/* ------------------------------------------------------------------------------------------- enums */
jstring* jrt_Enum_toString(jobject* o) { return ((jrt_Enum*)o)->name; }
jobject* jrt_Enum_valueOf(jarray* constants, const char* type, jstring* name) {
	if (!name) jrt_throw_new(&jrt_NullPointerException_class, "Name is null");
	for (int32_t i = 0; i < constants->length; ++i) {
		jobject* e = ((jobject**)constants->data)[i];
		if (jrt_String_equals_O(((jrt_Enum*)e)->name, (jobject*)name)) return e;
	}
	jstring* text = jrt_concat(4, str8("No enum constant "), str8(type), str8("."), name);
	jrt_throw(jrt_new_throwable(&jrt_IllegalArgumentException_class, text, 0));
}
static const jobject_vt enum_vt = {jrt_Enum_toString, jrt_Object_equals, jrt_Object_hashCode};
const jclass jrt_Enum_class = {"java.lang.Enum", &jrt_Object_class, sizeof(jrt_Enum), &enum_vt, 0, 0, 0};

int jrt_run_main(void (*m)(jarray*), int argc, char** argv) {
	volatile char stack_top = 0;
	gc_stack_base = (const char*)&stack_top + 1;
	jarray* args = jrt_new_array(sizeof(jobject*), argc > 1 ? argc - 1 : 0);
	for (int i = 1; i < argc; ++i) ((jobject**)args->data)[i - 1] = (jobject*)str8(argv[i]);
	m(args);
	fflush(stdout);
	return 0;
}
