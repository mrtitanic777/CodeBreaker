/* jrt.h -- the Java library in C (C11), for whole programs written by codebreaker/translate/backends/c_java.py.
 *
 * The object model (docs/TRANSLATOR_DESIGN.md, phase 6: "classes as structs and function tables"):
 *   - an object starts with a jobject header pointing at its class (jclass); a class's struct starts with
 *     its superclass's struct, so a pointer to it is a pointer to each of its bases;
 *   - a class's virtual methods are a table (vt) its jclass points at: the slots of the base first, in the
 *     same order, then its own; an interface method is found through the class's interface tables (itabs);
 *   - instanceof and checked casts walk the class chain and the interface tables;
 *   - a Java exception is a longjmp to the innermost handler (jrt_try), carrying the Throwable;
 *   - memory: objects are allocated and never freed while the program runs (released at exit). That is
 *     the C runtime's stand-in for Java's collector: right for programs that end, and the place a collector
 *     (Boehm) or reference counting would go for long-running ones;
 *   - strings are UTF-16, as Java's are; text is written out as UTF-8.
 */
#ifndef JRT_H
#define JRT_H
#include <math.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

typedef struct jclass jclass;
typedef struct jobject { const jclass* cls; } jobject;
typedef struct jstring jstring;
typedef struct jobject_vt {
	jstring* (*toString)(jobject*);
	bool (*equals)(jobject*, jobject*);
	int32_t (*hashCode)(jobject*);
} jobject_vt;
typedef struct jitab { const jclass* iface; const void* vt; } jitab;
struct jclass {
	const char* name;
	const jclass* super;
	size_t size;
	const void* vt;
	const jitab* itabs;
	int n_itabs;
	int kind;                         /* 0 class, 1 interface, 2 array */
};
#define JRT_ITABS(a) a, (int)(sizeof(a) / sizeof(a[0]))

struct jstring { jobject hdr; int32_t len; char16_t* s; };
typedef struct jarray { jobject hdr; int32_t length; int32_t esize; void* data; } jarray;

extern const jclass jrt_Object_class, jrt_String_class, jrt_Array_class;
extern const jobject_vt jrt_Object_vt;

/* ---- objects */
void* jrt_alloc(const jclass* c);
jobject* jrt_itab(jobject* o, const jclass* iface);
const void* jrt_ivt(jobject* o, const jclass* iface);   /* the interface's table for o, or throws */
bool jrt_instanceof(jobject* o, const jclass* c);
jobject* jrt_cast(jobject* o, const jclass* c);
void* jrt_nn(void* o);                                   /* o, or NullPointerException */
static inline const void* jrt_vt(jobject* o) { return ((jobject*)jrt_nn(o))->cls->vt; }
jstring* jrt_toString(jobject* o);                       /* o.toString() (virtual) */
bool jrt_equals(jobject* a, jobject* b);                 /* a.equals(b) (virtual) */
int32_t jrt_hashCode(jobject* o);                        /* o.hashCode() (virtual) */
bool jrt_Objects_equals_OO(jobject* a, jobject* b);
int32_t jrt_Objects_hashCode_O(jobject* o);
jstring* jrt_Object_toString(jobject* o);                /* Object's own: Class@hash */
bool jrt_Object_equals(jobject* a, jobject* b);
int32_t jrt_Object_hashCode(jobject* o);

/* ---- exceptions */
typedef struct jrt_try { jmp_buf jb; struct jrt_try* prev; jobject* ex; } jrt_try;
extern jrt_try* jrt_top;
static inline void jrt_push(jrt_try* t) { t->prev = jrt_top; t->ex = 0; jrt_top = t; }
static inline void jrt_pop(jrt_try* t) { jrt_top = t->prev; }
_Noreturn void jrt_throw(jobject* t);
_Noreturn void jrt_throw_new(const jclass* c, const char* msg);
_Noreturn void jrt_throw_npe(void);
jobject* jrt_new_throwable(const jclass* c, jstring* msg, jobject* cause);
typedef struct jrt_Throwable { jobject hdr; jstring* message; jobject* cause; } jrt_Throwable;
typedef struct jrt_Throwable_vt { jobject_vt base; jstring* (*getMessage)(jobject*); } jrt_Throwable_vt;
extern const jrt_Throwable_vt jrt_Throwable_vt_;
jstring* jrt_Throwable_toString(jobject* o);
jstring* jrt_Throwable_getMessage(jobject* o);
void jrt_Throwable_init(jobject* o, jstring* msg);
void jrt_Throwable_init_cause(jobject* o, jstring* msg, jobject* cause);
jstring* jrt_Throwable_getMessage_(jobject* o);          /* virtual */
#define JRT_EXC(N) extern const jclass jrt_##N##_class;
JRT_EXC(Throwable) JRT_EXC(Exception) JRT_EXC(Error) JRT_EXC(RuntimeException) JRT_EXC(IllegalArgumentException)
JRT_EXC(IllegalStateException) JRT_EXC(ArithmeticException) JRT_EXC(NullPointerException) JRT_EXC(ClassCastException)
JRT_EXC(UnsupportedOperationException) JRT_EXC(IndexOutOfBoundsException) JRT_EXC(ArrayIndexOutOfBoundsException)
JRT_EXC(StringIndexOutOfBoundsException) JRT_EXC(NegativeArraySizeException) JRT_EXC(NumberFormatException)
JRT_EXC(NoSuchElementException) JRT_EXC(IOException)

/* ---- strings */
jstring* jrt_str(const char16_t* s, int32_t n);
jstring* jrt_lit(jstring** slot, const char16_t* s, int32_t n);
jstring* jrt_str_I(int32_t v);
jstring* jrt_str_J(int64_t v);
jstring* jrt_str_D(double v);
jstring* jrt_str_F(float v);
jstring* jrt_str_Z(bool v);
jstring* jrt_str_C(char16_t v);
jstring* jrt_str_O(jobject* o);                          /* String.valueOf(Object): "null" or toString() */
jstring* jrt_concat(int n, ...);                         /* n jstring* */
int32_t jrt_String_length_(jstring* s);
char16_t jrt_String_charAt_I(jstring* s, int32_t i);
bool jrt_String_isEmpty_(jstring* s);
jstring* jrt_String_substring_I(jstring* s, int32_t b);
jstring* jrt_String_substring_II(jstring* s, int32_t b, int32_t e);
int32_t jrt_String_indexOf_I(jstring* s, int32_t c);
int32_t jrt_String_indexOf_Str(jstring* s, jstring* t);
bool jrt_String_contains_O(jstring* s, jobject* t);
bool jrt_String_startsWith_Str(jstring* s, jstring* t);
bool jrt_String_endsWith_Str(jstring* s, jstring* t);
bool jrt_String_equals_O(jstring* s, jobject* o);
bool jrt_String_equalsIgnoreCase_Str(jstring* s, jstring* t);
int32_t jrt_String_hashCode_(jstring* s);
int32_t jrt_String_compareTo_O(jstring* s, jobject* o);
jstring* jrt_String_toUpperCase_(jstring* s);
jstring* jrt_String_toLowerCase_(jstring* s);
jstring* jrt_String_trim_(jstring* s);
jstring* jrt_String_replace_CC(jstring* s, char16_t a, char16_t b);
jstring* jrt_String_repeat_I(jstring* s, int32_t n);
jarray* jrt_String_toCharArray_(jstring* s);
jarray* jrt_String_split_Str(jstring* s, jstring* sep);
jstring* jrt_String_toString_(jstring* s);
jstring* jrt_String_new_A(jarray* chars);
jstring* jrt_String_valueOf_A(jarray* chars);
jstring* jrt_String_format(jstring* fmt, int n, ...);   /* n jobject* (boxed) */
jstring* jrt_String_join(jstring* sep, jobject* parts);
#define jrt_String_valueOf_I jrt_str_I
#define jrt_String_valueOf_J jrt_str_J
#define jrt_String_valueOf_D jrt_str_D
#define jrt_String_valueOf_F jrt_str_F
#define jrt_String_valueOf_Z jrt_str_Z
#define jrt_String_valueOf_C jrt_str_C
#define jrt_String_valueOf_O jrt_str_O
#define jrt_String_indexOf_C jrt_String_indexOf_I
#define jrt_String_new_CA jrt_String_new_A
#define jrt_String_valueOf_CA jrt_String_new_A

/* ---- boxes */
typedef struct jrt_Integer { jobject hdr; int32_t value; } jrt_Integer;
typedef struct jrt_Long { jobject hdr; int64_t value; } jrt_Long;
typedef struct jrt_Double { jobject hdr; double value; } jrt_Double;
typedef struct jrt_Float { jobject hdr; float value; } jrt_Float;
typedef struct jrt_Short { jobject hdr; int16_t value; } jrt_Short;
typedef struct jrt_Byte { jobject hdr; int8_t value; } jrt_Byte;
typedef struct jrt_Character { jobject hdr; char16_t value; } jrt_Character;
typedef struct jrt_Boolean { jobject hdr; bool value; } jrt_Boolean;
extern const jclass jrt_Integer_class, jrt_Long_class, jrt_Double_class, jrt_Float_class, jrt_Short_class,
	jrt_Byte_class, jrt_Character_class, jrt_Boolean_class;
jobject* jrt_box_I(int32_t v);
jobject* jrt_box_J(int64_t v);
jobject* jrt_box_D(double v);
jobject* jrt_box_F(float v);
jobject* jrt_box_S(int16_t v);
jobject* jrt_box_B(int8_t v);
jobject* jrt_box_C(char16_t v);
jobject* jrt_box_Z(bool v);
int32_t jrt_unbox_I(jobject* o);
int64_t jrt_unbox_J(jobject* o);
double jrt_unbox_D(jobject* o);
float jrt_unbox_F(jobject* o);
int16_t jrt_unbox_S(jobject* o);
int8_t jrt_unbox_B(jobject* o);
char16_t jrt_unbox_C(jobject* o);
bool jrt_unbox_Z(jobject* o);
int32_t jrt_natural(jobject* a, jobject* b);             /* Comparable.compareTo: Strings, boxes, enums, programs */
jstring* jrt_Integer_toString_I(int32_t v);
jstring* jrt_Integer_toHexString_I(int32_t v);
jstring* jrt_Integer_toBinaryString_I(int32_t v);
int32_t jrt_Integer_parseInt_Str(jstring* s);
int32_t jrt_Integer_compare_II(int32_t a, int32_t b);
int32_t jrt_Integer_sum_II(int32_t a, int32_t b);
int32_t jrt_Integer_max_II(int32_t a, int32_t b);
int32_t jrt_Integer_min_II(int32_t a, int32_t b);
jobject* jrt_Integer_valueOf_I(int32_t v);
#define jrt_Integer_hashCode_I(v) ((int32_t)(v))
int32_t jrt_Integer_bitCount_I(int32_t v);
jstring* jrt_Long_toString_J(int64_t v);
jstring* jrt_Long_toHexString_J(int64_t v);
int32_t jrt_Long_hashCode_J(int64_t v);
int32_t jrt_Long_compare_JJ(int64_t a, int64_t b);
int32_t jrt_Double_compare_DD(double a, double b);
int32_t jrt_Double_hashCode_D(double v);
int64_t jrt_Double_doubleToRawLongBits_D(double v);
int64_t jrt_Double_doubleToLongBits_D(double v);
bool jrt_Double_isNaN_D(double v);
jstring* jrt_Double_toString_D(double v);
double jrt_Double_parseDouble_Str(jstring* s);
int32_t jrt_Float_floatToRawIntBits_F(float v);
int32_t jrt_Float_compare_FF(float a, float b);
int32_t jrt_Float_hashCode_F(float v);
int32_t jrt_Boolean_hashCode_Z(bool v);
#define jrt_Character_hashCode_C(v) ((int32_t)(v))
#define jrt_Short_hashCode_S(v) ((int32_t)(v))
#define jrt_Byte_hashCode_B(v) ((int32_t)(v))
bool jrt_Character_isDigit_C(char16_t c);
bool jrt_Character_isLetter_C(char16_t c);
bool jrt_Character_isLetterOrDigit_C(char16_t c);
bool jrt_Character_isWhitespace_C(char16_t c);
bool jrt_Character_isUpperCase_C(char16_t c);
bool jrt_Character_isLowerCase_C(char16_t c);
char16_t jrt_Character_toUpperCase_C(char16_t c);
char16_t jrt_Character_toLowerCase_C(char16_t c);
int32_t jrt_Character_getNumericValue_C(char16_t c);
#define JRT_INT_MAX 2147483647
#define JRT_INT_MIN (-2147483647 - 1)
#define JRT_LONG_MAX 9223372036854775807LL
#define JRT_LONG_MIN (-9223372036854775807LL - 1)

/* ---- numbers */
int32_t jrt_div_I(int32_t a, int32_t b);
int32_t jrt_rem_I(int32_t a, int32_t b);
int64_t jrt_div_J(int64_t a, int64_t b);
int64_t jrt_rem_J(int64_t a, int64_t b);
int32_t jrt_d2i(double d);
int64_t jrt_d2l(double d);
static inline int32_t jrt_ushr_I(int32_t a, int32_t n) { return (int32_t)((uint32_t)a >> (n & 31)); }
static inline int64_t jrt_ushr_J(int64_t a, int32_t n) { return (int64_t)((uint64_t)a >> (n & 63)); }
int32_t jrt_Math_abs_I(int32_t v);
int64_t jrt_Math_abs_J(int64_t v);
double jrt_Math_abs_D(double v);
float jrt_Math_abs_F(float v);
int32_t jrt_Math_max_II(int32_t a, int32_t b);
int64_t jrt_Math_max_JJ(int64_t a, int64_t b);
double jrt_Math_max_DD(double a, double b);
float jrt_Math_max_FF(float a, float b);
int32_t jrt_Math_min_II(int32_t a, int32_t b);
int64_t jrt_Math_min_JJ(int64_t a, int64_t b);
double jrt_Math_min_DD(double a, double b);
float jrt_Math_min_FF(float a, float b);
int32_t jrt_Math_floorMod_II(int32_t a, int32_t b);
int32_t jrt_Math_floorDiv_II(int32_t a, int32_t b);
int64_t jrt_Math_round_D(double v);
int32_t jrt_Math_round_F(float v);
double jrt_Math_signum_D(double v);
#define jrt_Math_sqrt_D sqrt
#define jrt_Math_pow_DD pow
#define jrt_Math_sin_D sin
#define jrt_Math_cos_D cos
#define jrt_Math_tan_D tan
#define jrt_Math_atan_D atan
#define jrt_Math_atan2_DD atan2
#define jrt_Math_floor_D floor
#define jrt_Math_ceil_D ceil
#define jrt_Math_exp_D exp
#define jrt_Math_log_D log
#define jrt_Math_log10_D log10
#define jrt_Math_hypot_DD hypot
#define jrt_Math_cbrt_D cbrt
#define JRT_PI 3.141592653589793
#define JRT_E 2.718281828459045

/* ---- arrays */
jarray* jrt_new_array(int32_t esize, int32_t n);
jarray* jrt_array_of(int32_t esize, int32_t n, const void* items);
jarray* jrt_new_array2(int32_t esize, int32_t n, int32_t m);
jarray* jrt_array_clone(jarray* a);
static inline jarray* jrt_ck(jarray* a, int32_t i) {
	extern _Noreturn void jrt_aioobe(int32_t i, int32_t n);
	if (!a) jrt_throw_npe();
	if ((uint32_t)i >= (uint32_t)a->length) jrt_aioobe(i, a->length);
	return a;
}
#define JA(T, a, i) (((T*)jrt_ck((jarray*)(a), (i))->data)[(i)])
#define JLEN(a) (((jarray*)jrt_nn(a))->length)
void jrt_Arrays_sort_CA(jarray* a);
void jrt_Arrays_sort_IA(jarray* a);
void jrt_Arrays_sort_DA(jarray* a);
void jrt_Arrays_fill_IA(jarray* a, int32_t v);
jstring* jrt_Arrays_toString_IA(jarray* a);
void jrt_System_arraycopy(jobject* src, int32_t sp, jobject* dst, int32_t dp, int32_t n);

/* ---- StringBuilder */
typedef struct jrt_StringBuilder { jobject hdr; int32_t len, cap; char16_t* s; } jrt_StringBuilder;
extern const jclass jrt_StringBuilder_class;
jrt_StringBuilder* jrt_StringBuilder_new_(void);
jrt_StringBuilder* jrt_StringBuilder_new_I(int32_t cap);
jrt_StringBuilder* jrt_StringBuilder_new_Str(jstring* s);
jrt_StringBuilder* jrt_StringBuilder_append_Str(jrt_StringBuilder* b, jstring* s);
jrt_StringBuilder* jrt_StringBuilder_append_C(jrt_StringBuilder* b, char16_t c);
jrt_StringBuilder* jrt_StringBuilder_append_I(jrt_StringBuilder* b, int32_t v);
jrt_StringBuilder* jrt_StringBuilder_append_J(jrt_StringBuilder* b, int64_t v);
jrt_StringBuilder* jrt_StringBuilder_append_D(jrt_StringBuilder* b, double v);
jrt_StringBuilder* jrt_StringBuilder_append_F(jrt_StringBuilder* b, float v);
jrt_StringBuilder* jrt_StringBuilder_append_Z(jrt_StringBuilder* b, bool v);
jrt_StringBuilder* jrt_StringBuilder_append_O(jrt_StringBuilder* b, jobject* o);
jrt_StringBuilder* jrt_StringBuilder_reverse_(jrt_StringBuilder* b);
jrt_StringBuilder* jrt_StringBuilder_insert_IStr(jrt_StringBuilder* b, int32_t at, jstring* s);
jrt_StringBuilder* jrt_StringBuilder_deleteCharAt_I(jrt_StringBuilder* b, int32_t i);
int32_t jrt_StringBuilder_length_(jrt_StringBuilder* b);
char16_t jrt_StringBuilder_charAt_I(jrt_StringBuilder* b, int32_t i);
void jrt_StringBuilder_setCharAt_IC(jrt_StringBuilder* b, int32_t i, char16_t c);
void jrt_StringBuilder_setLength_I(jrt_StringBuilder* b, int32_t n);
jstring* jrt_StringBuilder_toString_(jrt_StringBuilder* b);
#define jrt_StringBuilder_append_S(b, v) jrt_StringBuilder_append_I(b, (int32_t)(v))
#define jrt_StringBuilder_append_B(b, v) jrt_StringBuilder_append_I(b, (int32_t)(v))
#define jrt_StringBuilder_append_CA(b, v) jrt_StringBuilder_append_Str(b, jrt_String_valueOf_A(v))

/* ---- output */
void jrt_print(jstring* s, bool nl, FILE* f);
#define jrt_PrintStream_println_Str(o, s) jrt_print(jrt_str_O((jobject*)(s)), true, (FILE*)(o))
#define jrt_PrintStream_println_O(o, s) jrt_print(jrt_str_O((jobject*)(s)), true, (FILE*)(o))
#define jrt_PrintStream_println_I(o, v) jrt_print(jrt_str_I(v), true, (FILE*)(o))
#define jrt_PrintStream_println_J(o, v) jrt_print(jrt_str_J(v), true, (FILE*)(o))
#define jrt_PrintStream_println_D(o, v) jrt_print(jrt_str_D(v), true, (FILE*)(o))
#define jrt_PrintStream_println_F(o, v) jrt_print(jrt_str_F(v), true, (FILE*)(o))
#define jrt_PrintStream_println_Z(o, v) jrt_print(jrt_str_Z(v), true, (FILE*)(o))
#define jrt_PrintStream_println_C(o, v) jrt_print(jrt_str_C(v), true, (FILE*)(o))
#define jrt_PrintStream_println_CA(o, v) jrt_print(jrt_String_valueOf_A(v), true, (FILE*)(o))
#define jrt_PrintStream_println_(o) jrt_print(0, true, (FILE*)(o))
#define jrt_PrintStream_print_Str(o, s) jrt_print(jrt_str_O((jobject*)(s)), false, (FILE*)(o))
#define jrt_PrintStream_print_O(o, s) jrt_print(jrt_str_O((jobject*)(s)), false, (FILE*)(o))
#define jrt_PrintStream_print_I(o, v) jrt_print(jrt_str_I(v), false, (FILE*)(o))
#define jrt_PrintStream_print_J(o, v) jrt_print(jrt_str_J(v), false, (FILE*)(o))
#define jrt_PrintStream_print_D(o, v) jrt_print(jrt_str_D(v), false, (FILE*)(o))
#define jrt_PrintStream_print_F(o, v) jrt_print(jrt_str_F(v), false, (FILE*)(o))
#define jrt_PrintStream_print_Z(o, v) jrt_print(jrt_str_Z(v), false, (FILE*)(o))
#define jrt_PrintStream_print_C(o, v) jrt_print(jrt_str_C(v), false, (FILE*)(o))
#define jrt_System_out ((void*)stdout)
#define jrt_System_err ((void*)stderr)

/* ---- interfaces a program implements: their tables (each a jobject_vt and the methods) */
#define JRT_IFACE(N) extern const jclass jrt_##N##_class;
JRT_IFACE(Iterable) JRT_IFACE(Iterator) JRT_IFACE(Comparable) JRT_IFACE(Comparator) JRT_IFACE(Runnable)
JRT_IFACE(Function) JRT_IFACE(BiFunction) JRT_IFACE(UnaryOperator) JRT_IFACE(BinaryOperator) JRT_IFACE(Supplier)
JRT_IFACE(Consumer) JRT_IFACE(BiConsumer) JRT_IFACE(Predicate) JRT_IFACE(IntBinaryOperator) JRT_IFACE(IntUnaryOperator)
JRT_IFACE(IntPredicate) JRT_IFACE(IntFunction) JRT_IFACE(ToIntFunction) JRT_IFACE(IntConsumer)
JRT_IFACE(Collection) JRT_IFACE(List) JRT_IFACE(Set) JRT_IFACE(Map) JRT_IFACE(CharSequence)
typedef struct jrt_Iterable_vt { jobject_vt base; jobject* (*iterator)(jobject*); } jrt_Iterable_vt;
typedef struct jrt_Iterator_vt { jobject_vt base; bool (*hasNext)(jobject*); jobject* (*next)(jobject*); } jrt_Iterator_vt;
typedef struct jrt_Comparable_vt { jobject_vt base; int32_t (*compareTo)(jobject*, jobject*); } jrt_Comparable_vt;
typedef struct jrt_Comparator_vt { jobject_vt base; int32_t (*compare)(jobject*, jobject*, jobject*); } jrt_Comparator_vt;
typedef struct jrt_Runnable_vt { jobject_vt base; void (*run)(jobject*); } jrt_Runnable_vt;
typedef struct jrt_Function_vt { jobject_vt base; jobject* (*apply)(jobject*, jobject*); } jrt_Function_vt;
typedef struct jrt_BiFunction_vt { jobject_vt base; jobject* (*apply)(jobject*, jobject*, jobject*); } jrt_BiFunction_vt;
typedef jrt_Function_vt jrt_UnaryOperator_vt;
typedef jrt_BiFunction_vt jrt_BinaryOperator_vt;
typedef struct jrt_Supplier_vt { jobject_vt base; jobject* (*get)(jobject*); } jrt_Supplier_vt;
typedef struct jrt_Consumer_vt { jobject_vt base; void (*accept)(jobject*, jobject*); } jrt_Consumer_vt;
typedef struct jrt_BiConsumer_vt { jobject_vt base; void (*accept)(jobject*, jobject*, jobject*); } jrt_BiConsumer_vt;
typedef struct jrt_Predicate_vt { jobject_vt base; bool (*test)(jobject*, jobject*); } jrt_Predicate_vt;
typedef struct jrt_IntBinaryOperator_vt { jobject_vt base; int32_t (*applyAsInt)(jobject*, int32_t, int32_t); } jrt_IntBinaryOperator_vt;
typedef struct jrt_IntUnaryOperator_vt { jobject_vt base; int32_t (*applyAsInt)(jobject*, int32_t); } jrt_IntUnaryOperator_vt;
typedef struct jrt_IntPredicate_vt { jobject_vt base; bool (*test)(jobject*, int32_t); } jrt_IntPredicate_vt;
typedef struct jrt_IntFunction_vt { jobject_vt base; jobject* (*apply)(jobject*, int32_t); } jrt_IntFunction_vt;
typedef struct jrt_ToIntFunction_vt { jobject_vt base; int32_t (*applyAsInt)(jobject*, jobject*); } jrt_ToIntFunction_vt;
typedef struct jrt_IntConsumer_vt { jobject_vt base; void (*accept)(jobject*, int32_t); } jrt_IntConsumer_vt;
#define JRT_ICALL(I, o, m, ...) (((const jrt_##I##_vt*)jrt_ivt((jobject*)(o), &jrt_##I##_class))->m((jobject*)(o), ##__VA_ARGS__))
#define jrt_Iterable_iterator_(o) JRT_ICALL(Iterable, o, iterator)
#define jrt_Iterator_hasNext_(o) JRT_ICALL(Iterator, o, hasNext)
#define jrt_Iterator_next_(o) JRT_ICALL(Iterator, o, next)
#define jrt_Comparator_compare_OO(o, a, b) JRT_ICALL(Comparator, o, compare, a, b)
#define jrt_Runnable_run_(o) JRT_ICALL(Runnable, o, run)
#define jrt_Function_apply_O(o, a) JRT_ICALL(Function, o, apply, a)
#define jrt_UnaryOperator_apply_O(o, a) JRT_ICALL(Function, o, apply, a)
#define jrt_BiFunction_apply_OO(o, a, b) JRT_ICALL(BiFunction, o, apply, a, b)
#define jrt_BinaryOperator_apply_OO(o, a, b) JRT_ICALL(BiFunction, o, apply, a, b)
#define jrt_Supplier_get_(o) JRT_ICALL(Supplier, o, get)
#define jrt_Consumer_accept_O(o, a) JRT_ICALL(Consumer, o, accept, a)
#define jrt_BiConsumer_accept_OO(o, a, b) JRT_ICALL(BiConsumer, o, accept, a, b)
#define jrt_Predicate_test_O(o, a) JRT_ICALL(Predicate, o, test, a)
#define jrt_IntBinaryOperator_applyAsInt_II(o, a, b) JRT_ICALL(IntBinaryOperator, o, applyAsInt, a, b)
#define jrt_IntUnaryOperator_applyAsInt_I(o, a) JRT_ICALL(IntUnaryOperator, o, applyAsInt, a)
#define jrt_IntPredicate_test_I(o, a) JRT_ICALL(IntPredicate, o, test, a)
#define jrt_IntFunction_apply_I(o, a) JRT_ICALL(IntFunction, o, apply, a)
#define jrt_ToIntFunction_applyAsInt_O(o, a) JRT_ICALL(ToIntFunction, o, applyAsInt, a)
#define jrt_IntConsumer_accept_I(o, a) JRT_ICALL(IntConsumer, o, accept, a)
#define jrt_Comparable_compareTo_O(o, a) jrt_natural((jobject*)(o), (jobject*)(a))

/* ---- collections (ArrayList, the immutable List.of, HashMap/LinkedHashMap/TreeMap, HashSet/LinkedHashSet/TreeSet) */
typedef struct jrt_ArrayList { jobject hdr; int32_t size, cap; jobject** items; bool immutable; } jrt_ArrayList;
typedef struct jrt_node { jobject hdr; jobject* key; jobject* value; int32_t hash; struct jrt_node* next; struct jrt_node* after; struct jrt_node* before; } jrt_node;
typedef struct jrt_HashMap { jobject hdr; jrt_node** table; int32_t cap, count; int kind; jrt_node* head; jrt_node* tail; jobject* cmp; } jrt_HashMap;
typedef struct jrt_MapSet { jobject hdr; jrt_HashMap* m; int kind; } jrt_MapSet;   /* 0 keys 1 values 2 entries; a HashSet is kind 0 */
extern const jclass jrt_ArrayList_class, jrt_HashMap_class, jrt_MapSet_class, jrt_node_class;
jobject* jrt_ArrayList_new_(void);
jobject* jrt_ArrayList_new_I(int32_t n);
jobject* jrt_ArrayList_new_O(jobject* c);
jobject* jrt_List_of(int n, ...);
bool jrt_Collection_add_O(jobject* c, jobject* x);
int32_t jrt_Collection_size_(jobject* c);
bool jrt_Collection_isEmpty_(jobject* c);
bool jrt_Collection_contains_O(jobject* c, jobject* x);
bool jrt_Collection_remove_O(jobject* c, jobject* x);
void jrt_Collection_clear_(jobject* c);
jobject* jrt_Collection_iterator_(jobject* c);
jobject* jrt_List_get_I(jobject* l, int32_t i);
jobject* jrt_List_set_IO(jobject* l, int32_t i, jobject* x);
void jrt_List_add_IO(jobject* l, int32_t i, jobject* x);
jobject* jrt_List_remove_I(jobject* l, int32_t i);
int32_t jrt_List_indexOf_O(jobject* l, jobject* x);
void jrt_List_sort_O(jobject* l, jobject* cmp);
void jrt_Collections_sort_O(jobject* l);
void jrt_Collections_sort_OO(jobject* l, jobject* cmp);
void jrt_Collections_reverse_O(jobject* l);
jobject* jrt_HashMap_new_(void);
jobject* jrt_LinkedHashMap_new_(void);
jobject* jrt_TreeMap_new_(void);
jobject* jrt_TreeMap_new_O(jobject* cmp);
jobject* jrt_Map_get_O(jobject* m, jobject* k);
jobject* jrt_Map_put_OO(jobject* m, jobject* k, jobject* v);
jobject* jrt_Map_remove_O(jobject* m, jobject* k);
bool jrt_Map_containsKey_O(jobject* m, jobject* k);
int32_t jrt_Map_size_(jobject* m);
bool jrt_Map_isEmpty_(jobject* m);
jobject* jrt_Map_getOrDefault_OO(jobject* m, jobject* k, jobject* d);
jobject* jrt_Map_merge_OOO(jobject* m, jobject* k, jobject* v, jobject* f);
jobject* jrt_Map_keySet_(jobject* m);
jobject* jrt_Map_values_(jobject* m);
jobject* jrt_Map_entrySet_(jobject* m);
jobject* jrt_MapEntry_getKey_(jobject* e);
jobject* jrt_MapEntry_getValue_(jobject* e);
jobject* jrt_HashSet_new_(void);
jobject* jrt_LinkedHashSet_new_(void);
jobject* jrt_TreeSet_new_(void);
#define jrt_ArrayList_add_O jrt_Collection_add_O
#define jrt_List_add_O jrt_Collection_add_O
#define jrt_Set_add_O jrt_Collection_add_O
#define jrt_HashSet_add_O jrt_Collection_add_O
#define jrt_ArrayList_size_ jrt_Collection_size_
#define jrt_List_size_ jrt_Collection_size_
#define jrt_Set_size_ jrt_Collection_size_
#define jrt_ArrayList_isEmpty_ jrt_Collection_isEmpty_
#define jrt_List_isEmpty_ jrt_Collection_isEmpty_
#define jrt_Set_isEmpty_ jrt_Collection_isEmpty_
#define jrt_List_contains_O jrt_Collection_contains_O
#define jrt_Set_contains_O jrt_Collection_contains_O
#define jrt_List_remove_O jrt_Collection_remove_O
#define jrt_Set_remove_O jrt_Collection_remove_O
#define jrt_ArrayList_get_I jrt_List_get_I
#define jrt_ArrayList_remove_I jrt_List_remove_I
#define jrt_ArrayList_set_IO jrt_List_set_IO
#define jrt_List_iterator_ jrt_Collection_iterator_
#define jrt_Set_iterator_ jrt_Collection_iterator_
#define jrt_HashMap_get_O jrt_Map_get_O
#define jrt_HashMap_put_OO jrt_Map_put_OO
#define jrt_Map_Entry_getKey_ jrt_MapEntry_getKey_
#define jrt_Map_Entry_getValue_ jrt_MapEntry_getValue_

/* ---- enums */
typedef struct jrt_Enum { jobject hdr; jstring* name; int32_t ordinal; } jrt_Enum;
extern const jclass jrt_Enum_class;
jstring* jrt_Enum_toString(jobject* o);
#define jrt_Enum_name_(o) (((jrt_Enum*)jrt_nn(o))->name)
#define jrt_Enum_ordinal_(o) (((jrt_Enum*)jrt_nn(o))->ordinal)

int jrt_run_main(void (*m)(jarray*), int argc, char** argv);

#endif
