/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from
   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */
#include "jrt.h"

typedef struct Mix Mix;
typedef struct Mix_Color Mix_Color;
typedef struct Mix_Inner Mix_Inner;
typedef struct Mix_Box Mix_Box;
typedef struct Mix_anon1 Mix_anon1;
extern const jclass Mix_class;
extern const jclass Mix_Color_class;
extern const jclass Mix_Shape_class;
extern const jclass Mix_Inner_class;
extern const jclass Mix_Box_class;
extern const jclass Mix_anon1_class;

struct Mix {
	jobject hdr_;
	int32_t base;
};
struct Mix_vt {
	jobject_vt o_;
};
static int32_t Mix_LIMIT = 10;
static int32_t Mix_counter = 0;
static int64_t Mix_total = 5LL;
static jstring* Mix_label = NULL;
static bool Mix_inited_;

struct Mix_Color {
	jrt_Enum super_;
	int32_t w;
};
struct Mix_Color_vt {
	jobject_vt o_;
	int32_t (*weight)(jobject*);
};
static Mix_Color* Mix_Color_RED;
static Mix_Color* Mix_Color_GREEN;
static Mix_Color* Mix_Color_BLUE;
static jarray* Mix_Color_values_;
static bool Mix_Color_inited_;

struct Mix_Shape_vt {
	jobject_vt o_;
	double (*area)(jobject*);
};

struct Mix_Inner {
	jobject hdr_;
	Mix* outer_;
	int32_t v;
};
struct Mix_Inner_vt {
	jobject_vt o_;
	int32_t (*get)(jobject*);
};

struct Mix_Box {
	jobject hdr_;
	jobject* item;
};
struct Mix_Box_vt {
	jobject_vt o_;
	jobject* (*get)(jobject*);
};

struct Mix_anon1 {
	jobject hdr_;
};
struct Mix_anon1_vt {
	jobject_vt o_;
	double (*area)(jobject*);
};

static jstring* lit_1;
static jstring* lit_2;
static jstring* lit_3;
static jstring* lit_4;
static jstring* lit_5;
static jstring* lit_6;
static jstring* lit_7;
static jstring* lit_8;
static jstring* lit_9;
static jstring* lit_10;
static jstring* lit_11;
static jstring* lit_12;
static jstring* lit_13;
static jstring* lit_14;
static jstring* lit_15;
static jstring* lit_16;
static jstring* lit_17;
static jstring* lit_18;
static jstring* lit_19;

static void Mix_init(Mix* self);
static Mix* Mix_new(void);
static int32_t Mix_fib(int32_t);
static int32_t Mix_sw(int32_t);
static jstring* Mix_name(jstring*);
static jarray* Mix_grid(int32_t);
static int64_t Mix_mix(int64_t, int32_t, double, char16_t, int8_t);
static void Mix_thrower(int32_t);
static void Mix_main(jarray*);
static void Mix_finit(Mix* self);
static void Mix_clinit(void);
static void Mix_Color_init_V(Mix_Color* self, jstring* name_in, int32_t ordinal_in);
static Mix_Color* Mix_Color_new_V(jstring* name_in, int32_t ordinal_in);
static void Mix_Color_init_I(Mix_Color* self, jstring* name_in, int32_t ordinal_in, int32_t w);
static Mix_Color* Mix_Color_new_I(jstring* name_in, int32_t ordinal_in, int32_t w);
static int32_t Mix_Color_weight(jobject*);
static void Mix_Color_finit(Mix_Color* self);
static void Mix_Color_clinit(void);
static jarray* Mix_Color_values(void);
static void Mix_Inner_init(Mix_Inner* self, int32_t v);
static Mix_Inner* Mix_Inner_new(Mix* outer_in, int32_t v);
static int32_t Mix_Inner_get(jobject*);
static void Mix_Inner_finit(Mix_Inner* self);
static void Mix_Box_init(Mix_Box* self, jobject* item);
static Mix_Box* Mix_Box_new(jobject* item);
static jobject* Mix_Box_get(jobject*);
static void Mix_Box_finit(Mix_Box* self);
static void Mix_anon1_init(Mix_anon1* self);
static Mix_anon1* Mix_anon1_new(void);
static double Mix_anon1_area(jobject*);
static void Mix_anon1_finit(Mix_anon1* self);
static inline int32_t Mix_Color_weight_v(jobject* o) { return ((const struct Mix_Color_vt*)jrt_vt(o))->weight(o); }
static inline double Mix_Shape_area_i(jobject* o) { return ((const struct Mix_Shape_vt*)jrt_ivt(o, &Mix_Shape_class))->area(o); }
static inline int32_t Mix_Inner_get_v(jobject* o) { return ((const struct Mix_Inner_vt*)jrt_vt(o))->get(o); }
static inline jobject* Mix_Box_get_v(jobject* o) { return ((const struct Mix_Box_vt*)jrt_vt(o))->get(o); }
static inline double Mix_anon1_area_v(jobject* o) { return ((const struct Mix_anon1_vt*)jrt_vt(o))->area(o); }
struct Lambda1 {
	jobject hdr_;
};
static jobject* Lambda1_fn(jobject* self_, jobject* x) {
	struct Lambda1* self = (struct Lambda1*)self_;
	(void)self;
	return jrt_box_I(jrt_unbox_I((jobject*)x) * 2);
}
static const jrt_Function_vt Lambda1_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda1_fn};
static const jitab Lambda1_itabs[] = {{&jrt_Function_class, &Lambda1_vt}};
static const jclass Lambda1_class = {"Mix$$Lambda", &jrt_Object_class, sizeof(struct Lambda1), &jrt_Object_vt, JRT_ITABS(Lambda1_itabs), 0};
static jobject* Lambda1_new(void) {
	struct Lambda1* o = (struct Lambda1*)jrt_alloc(&Lambda1_class);
	return (jobject*)o;
}

struct Lambda2 {
	jobject hdr_;
};
static jobject* Lambda2_fn(jobject* self_, jobject* a0, jobject* a1) {
	struct Lambda2* self = (struct Lambda2*)self_;
	(void)self;
	return jrt_box_I(jrt_Integer_sum_II(jrt_unbox_I((jobject*)a0), jrt_unbox_I((jobject*)a1)));
}
static const jrt_BiFunction_vt Lambda2_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda2_fn};
static const jitab Lambda2_itabs[] = {{&jrt_BiFunction_class, &Lambda2_vt}};
static const jclass Lambda2_class = {"Mix$$Lambda", &jrt_Object_class, sizeof(struct Lambda2), &jrt_Object_vt, JRT_ITABS(Lambda2_itabs), 0};
static jobject* Lambda2_new(void) {
	struct Lambda2* o = (struct Lambda2*)jrt_alloc(&Lambda2_class);
	return (jobject*)o;
}

struct Lambda3 {
	jobject hdr_;
};
static jobject* Lambda3_fn(jobject* self_) {
	struct Lambda3* self = (struct Lambda3*)self_;
	(void)self;
	return (jobject*)jrt_concat(2, jrt_str_O((jobject*)Mix_label), jrt_str_I(Mix_counter));
}
static const jrt_Supplier_vt Lambda3_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda3_fn};
static const jitab Lambda3_itabs[] = {{&jrt_Supplier_class, &Lambda3_vt}};
static const jclass Lambda3_class = {"Mix$$Lambda", &jrt_Object_class, sizeof(struct Lambda3), &jrt_Object_vt, JRT_ITABS(Lambda3_itabs), 0};
static jobject* Lambda3_new(void) {
	struct Lambda3* o = (struct Lambda3*)jrt_alloc(&Lambda3_class);
	return (jobject*)o;
}

static void Mix_init(Mix* self) {
	Mix_finit(self);
}

static Mix* Mix_new(void) {
	Mix_clinit();
	Mix* o = (Mix*)jrt_alloc(&Mix_class);
	Mix_init(o);
	return o;
}

static int32_t Mix_fib(int32_t n) {
	Mix_clinit();
	int32_t jrt_o1 = 0;
	int32_t jrt_o2 = 0;
	return (n < 2 ? n : (jrt_o1 = Mix_fib(n - 1), jrt_o2 = Mix_fib(n - 2), jrt_o1 + jrt_o2));
}

static int32_t Mix_sw(int32_t k) {
	Mix_clinit();
	switch (k) {
	case 1:
	{
		return 10;
	}
	case 2:
	{
	}
	case 3:
	{
		k += 5;
		break;
	}
	default:
	{
		k = -1;
	}
	}
	return k;
}

static jstring* Mix_name(jstring* s) {
	Mix_clinit();
	{
		jstring* j2x_s2 = (s);
		{
			if (jrt_String_equals_O(jrt_lit(&lit_1, u"a", 1), (jobject*)j2x_s2)) {
				return jrt_lit(&lit_2, u"alpha", 5);
			} else if (jrt_String_equals_O(jrt_lit(&lit_3, u"b", 1), (jobject*)j2x_s2)) {
				return jrt_lit(&lit_4, u"beta", 4);
			} else {
				return jrt_lit(&lit_5, u"other", 5);
			}
		}
	}
}

static jarray* Mix_grid(int32_t n) {
	Mix_clinit();
	jarray* g = jrt_new_array2(sizeof(int32_t), n, n);
	{
		int32_t i = 0;
		for (; i < n; i++) {
			{
				int32_t j = 0;
				for (; j < n; j++) {
					JA(int32_t, JA(jarray*, g, i), j) = i * j;
				}
			}
		}
	}
	return g;
}

static int64_t Mix_mix(int64_t a, int32_t b, double c, char16_t ch, int8_t by) {
	Mix_clinit();
	int64_t r = a << ((b & 7) & 63);
	r ^= jrt_d2i(c);
	r += ch + by;
	r = (int64_t)jrt_ushr_J(r, (1 & 63));
	return r % 1000 + (b / 3);
}

static void Mix_thrower(int32_t x) {
	Mix_clinit();
	if (x > 2) {
		jrt_throw((jobject*)jrt_new_throwable(&jrt_IllegalStateException_class, jrt_concat(2, jrt_lit(&lit_6, u"big ", 4), jrt_str_I(x)), NULL));
	}
	if (x == 2) {
		jrt_throw((jobject*)jrt_new_throwable(&jrt_Exception_class, jrt_lit(&lit_7, u"two", 3), NULL));
	}
}

static void Mix_main(jarray* args) {
	Mix_clinit();
	Mix* volatile m = Mix_new();
	Mix_Inner* volatile in = Mix_Inner_new((Mix*)jrt_nn(m), 3);
	volatile int32_t jrt_o1 = 0;
	volatile int32_t jrt_o2 = 0;
	volatile int32_t jrt_o3 = 0;
	volatile int32_t jrt_o4 = 0;
	volatile int32_t jrt_o5 = 0;
	jstring* volatile jrt_o6 = NULL;
	jstring* volatile jrt_o7 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (jrt_o1 = Mix_Inner_get_v((jobject*)in), jrt_o2 = Mix_fib(10), jrt_o3 = Mix_sw(1), jrt_o4 = Mix_sw(3), jrt_o5 = Mix_sw(9), jrt_o6 = Mix_name(jrt_lit(&lit_3, u"b", 1)), jrt_o7 = Mix_name(jrt_lit(&lit_8, u"z", 1)), jrt_concat(10, jrt_str_I(jrt_o1), jrt_lit(&lit_9, u" ", 1), jrt_str_I(jrt_o2), jrt_lit(&lit_9, u" ", 1), jrt_str_I(jrt_o3), jrt_str_I(jrt_o4), jrt_str_I(jrt_o5), jrt_lit(&lit_9, u" ", 1), jrt_str_O((jobject*)jrt_o6), jrt_str_O((jobject*)jrt_o7))));
	{
		jarray* volatile j2x_a3 = Mix_Color_values();
		{
			volatile int32_t j2x_i4 = 0;
			for (; j2x_i4 < JLEN(j2x_a3); j2x_i4++) {
				Mix_Color* volatile c = (Mix_Color*)jrt_cast((jobject*)JA(jobject*, j2x_a3, j2x_i4), &Mix_Color_class);
				jstring* volatile jrt_o8 = NULL;
				jstring* volatile jrt_o9 = NULL;
				jstring* volatile jrt_o10 = NULL;
				jrt_PrintStream_println_Str(jrt_System_out, (jrt_o8 = jrt_str_O((jobject*)c), jrt_o9 = jrt_str_I(Mix_Color_weight_v((jobject*)c)), jrt_o10 = jrt_str_I(jrt_Enum_ordinal_((jobject*)c)), jrt_concat(5, jrt_o8, jrt_lit(&lit_10, u":", 1), jrt_o9, jrt_lit(&lit_10, u":", 1), jrt_o10)));
			}
		}
	}
	jobject* volatile s = (jobject*)Mix_anon1_new();
	jrt_PrintStream_println_D(jrt_System_out, Mix_Shape_area_i((jobject*)s));
	jarray* volatile g = Mix_grid(4);
	volatile int32_t sum = 0;
	{
		jarray* volatile j2x_a7 = g;
		{
			volatile int32_t j2x_i8 = 0;
			for (; j2x_i8 < JLEN(j2x_a7); j2x_i8++) {
				jarray* volatile row = JA(jarray*, j2x_a7, j2x_i8);
				jarray* volatile j2x_a5 = row;
				{
					volatile int32_t j2x_i6 = 0;
					for (; j2x_i6 < JLEN(j2x_a5); j2x_i6++) {
						volatile int32_t x = JA(int32_t, j2x_a5, j2x_i6);
						sum += x;
					}
				}
			}
		}
	}
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(7, jrt_str_I(sum), jrt_lit(&lit_9, u" ", 1), jrt_str_J(Mix_mix(1234567LL, 5, 7.9, u'x', (int8_t)(3))), jrt_lit(&lit_9, u" ", 1), jrt_str_O((jobject*)Mix_label), jrt_str_I(Mix_counter), jrt_str_J(Mix_total)));
	Mix_Box* volatile b = Mix_Box_new((jobject*)jrt_lit(&lit_11, u"boxed", 5));
	volatile int32_t jrt_o11 = 0;
	jstring* volatile jrt_o12 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (jrt_o11 = jrt_String_length_((jstring*)jrt_cast((jobject*)Mix_Box_get_v((jobject*)b), &jrt_String_class)), jrt_o12 = (jstring*)jrt_cast((jobject*)Mix_Box_get_v((jobject*)b), &jrt_String_class), jrt_concat(2, jrt_str_I(jrt_o11), jrt_str_O((jobject*)jrt_o12))));
	{
		volatile int32_t i = 0;
		for (; i < 4; i++) {
			{
				jrt_try f13;
				jrt_push(&f13);
				if (setjmp(f13.jb) == 0) {
					{
						jrt_try t14;
						jrt_push(&t14);
						if (setjmp(t14.jb) == 0) {
							Mix_thrower(i);
							jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_12, u"ok ", 3), jrt_str_I(i)));
							jrt_pop(&t14);
						} else {
							jrt_pop(&t14);
							if (jrt_instanceof(t14.ex, &jrt_IllegalStateException_class)) {
								jrt_Throwable* e = (jrt_Throwable*)t14.ex;
								jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_13, u"ise ", 4), jrt_str_O((jobject*)jrt_Throwable_getMessage_((jobject*)e))));
							} else if (jrt_instanceof(t14.ex, &jrt_Exception_class)) {
								jrt_Throwable* e = (jrt_Throwable*)t14.ex;
								jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_14, u"ex ", 3), jrt_str_O((jobject*)jrt_Throwable_getMessage_((jobject*)e))));
							} else {
								jrt_throw(t14.ex);
							}
						}
					}
					jrt_pop(&f13);
				} else {
					jrt_pop(&f13);
					Mix_counter++;
					jrt_throw(f13.ex);
				}
				Mix_counter++;
			}
		}
	}
	jobject* volatile twice = Lambda1_new();
	jobject* volatile add = Lambda2_new();
	jobject* volatile sup = Lambda3_new();
	jstring* volatile jrt_o15 = NULL;
	jstring* volatile jrt_o16 = NULL;
	jstring* volatile jrt_o17 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (jrt_o15 = jrt_str_O((jobject*)jrt_Function_apply_O(twice, jrt_box_I(21))), jrt_o16 = jrt_str_O((jobject*)jrt_BiFunction_apply_OO(add, jrt_box_I(3), jrt_box_I(4))), jrt_o17 = jrt_str_O((jobject*)(jstring*)jrt_cast((jobject*)jrt_Supplier_get_(sup), &jrt_String_class)), jrt_concat(5, jrt_o15, jrt_lit(&lit_9, u" ", 1), jrt_o16, jrt_lit(&lit_9, u" ", 1), jrt_o17)));
	jrt_StringBuilder* volatile sb = jrt_StringBuilder_new_();
	{
		volatile int32_t i = 0;
		for (; i < 5; i++) {
			{
				volatile int32_t j = 0;
				while (true) {
					if (j > i) {
						goto outer_cont;
					}
					if (i == 4) {
						goto outer_brk;
					}
					jrt_StringBuilder_append_C(jrt_StringBuilder_append_I(jrt_StringBuilder_append_C(jrt_StringBuilder_append_I(sb, i), u','), j), u' ');
					j++;
				}
			}
			outer_cont:;
		}
	}
	outer_brk:;
	jrt_PrintStream_println_Str(jrt_System_out, jrt_toString((jobject*)sb));
	jobject* volatile o = (jobject*)b;
	if (jrt_instanceof((jobject*)o, &Mix_Box_class)) {
		jrt_PrintStream_println_Str(jrt_System_out, jrt_lit(&lit_15, u"box", 3));
	}
	volatile double d = 1.0 / 3;
	volatile float f = 2.5f;
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(9, jrt_str_D(d), jrt_lit(&lit_9, u" ", 1), jrt_str_F(f), jrt_lit(&lit_9, u" ", 1), jrt_str_J(jrt_d2l(d)), jrt_lit(&lit_9, u" ", 1), jrt_str_I(jrt_Math_max_II(3, 4)), jrt_lit(&lit_9, u" ", 1), jrt_str_I(JRT_INT_MAX)));
	volatile int32_t k = 0;
	do {
		k += 3;
	} while (k < 10);
	volatile char16_t c = (char16_t)((u'a' + k));
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(5, jrt_str_I(k), jrt_lit(&lit_9, u" ", 1), jrt_str_C(c), jrt_lit(&lit_9, u" ", 1), jrt_str_Z((k > 5 && c != u'z' || k == 0))));
}

static void Mix_finit(Mix* self) {
	self->base = 4;
}

static void Mix_clinit(void) {
	if (Mix_inited_) return;
	Mix_inited_ = true;
	Mix_label = jrt_lit(&lit_16, u"mix", 3);
	Mix_counter = 3;
}

static void Mix_Color_init_V(Mix_Color* self, jstring* name_in, int32_t ordinal_in) {
	((jrt_Enum*)self)->name = name_in;
	((jrt_Enum*)self)->ordinal = ordinal_in;
	Mix_Color_init_I(self, name_in, ordinal_in, 1);
}

static Mix_Color* Mix_Color_new_V(jstring* name_in, int32_t ordinal_in) {
	Mix_Color_clinit();
	Mix_Color* o = (Mix_Color*)jrt_alloc(&Mix_Color_class);
	Mix_Color_init_V(o, name_in, ordinal_in);
	return o;
}

static void Mix_Color_init_I(Mix_Color* self, jstring* name_in, int32_t ordinal_in, int32_t w) {
	((jrt_Enum*)self)->name = name_in;
	((jrt_Enum*)self)->ordinal = ordinal_in;
	Mix_Color_finit(self);
	self->w = w;
}

static Mix_Color* Mix_Color_new_I(jstring* name_in, int32_t ordinal_in, int32_t w) {
	Mix_Color_clinit();
	Mix_Color* o = (Mix_Color*)jrt_alloc(&Mix_Color_class);
	Mix_Color_init_I(o, name_in, ordinal_in, w);
	return o;
}

static int32_t Mix_Color_weight(jobject* self_) {
	Mix_Color* self = (Mix_Color*)self_;
	return self->w;
}

static void Mix_Color_finit(Mix_Color* self) {
}

static void Mix_Color_clinit(void) {
	if (Mix_Color_inited_) return;
	Mix_Color_inited_ = true;
	Mix_Color_RED = Mix_Color_new_V(jrt_lit(&lit_17, u"RED", 3), 0);
	Mix_Color_GREEN = Mix_Color_new_I(jrt_lit(&lit_18, u"GREEN", 5), 1, 5);
	Mix_Color_BLUE = Mix_Color_new_V(jrt_lit(&lit_19, u"BLUE", 4), 2);
	Mix_Color_values_ = jrt_new_array(sizeof(jobject*), 3);
	JA(jobject*, Mix_Color_values_, 0) = (jobject*)Mix_Color_RED;
	JA(jobject*, Mix_Color_values_, 1) = (jobject*)Mix_Color_GREEN;
	JA(jobject*, Mix_Color_values_, 2) = (jobject*)Mix_Color_BLUE;
}

static jarray* Mix_Color_values(void) {
	Mix_Color_clinit();
	return jrt_array_clone(Mix_Color_values_);
}

static void Mix_Inner_init(Mix_Inner* self, int32_t v) {
	Mix_Inner_finit(self);
	self->v = v + self->outer_->base;
}

static Mix_Inner* Mix_Inner_new(Mix* outer_in, int32_t v) {
	Mix_Inner* o = (Mix_Inner*)jrt_alloc(&Mix_Inner_class);
	o->outer_ = outer_in;
	Mix_Inner_init(o, v);
	return o;
}

static int32_t Mix_Inner_get(jobject* self_) {
	Mix_Inner* self = (Mix_Inner*)self_;
	return self->v * 2;
}

static void Mix_Inner_finit(Mix_Inner* self) {
}

static void Mix_Box_init(Mix_Box* self, jobject* item) {
	Mix_Box_finit(self);
	self->item = item;
}

static Mix_Box* Mix_Box_new(jobject* item) {
	Mix_Box* o = (Mix_Box*)jrt_alloc(&Mix_Box_class);
	Mix_Box_init(o, item);
	return o;
}

static jobject* Mix_Box_get(jobject* self_) {
	Mix_Box* self = (Mix_Box*)self_;
	return self->item;
}

static void Mix_Box_finit(Mix_Box* self) {
}

static void Mix_anon1_init(Mix_anon1* self) {
	Mix_anon1_finit(self);
}

static Mix_anon1* Mix_anon1_new(void) {
	Mix_anon1* o = (Mix_anon1*)jrt_alloc(&Mix_anon1_class);
	Mix_anon1_init(o);
	return o;
}

static double Mix_anon1_area(jobject* self_) {
	Mix_anon1* self = (Mix_anon1*)self_;
	return 2.5 * (Mix_clinit(), Mix_LIMIT);
}

static void Mix_anon1_finit(Mix_anon1* self) {
}

static const struct Mix_vt Mix_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Mix_class = {"Mix", &jrt_Object_class, sizeof(Mix), &Mix_vt_, 0, 0, 0};

static const struct Mix_Color_vt Mix_Color_vt_ = {{jrt_Enum_toString, jrt_Object_equals, jrt_Object_hashCode}, (int32_t (*)(jobject*))Mix_Color_weight};
const jclass Mix_Color_class = {"Mix$Color", &jrt_Enum_class, sizeof(Mix_Color), &Mix_Color_vt_, 0, 0, 0};

const jclass Mix_Shape_class = {"Mix$Shape", 0, 0, 0, 0, 0, 1};

static const struct Mix_Inner_vt Mix_Inner_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (int32_t (*)(jobject*))Mix_Inner_get};
const jclass Mix_Inner_class = {"Mix$Inner", &jrt_Object_class, sizeof(Mix_Inner), &Mix_Inner_vt_, 0, 0, 0};

static const struct Mix_Box_vt Mix_Box_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (jobject* (*)(jobject*))Mix_Box_get};
const jclass Mix_Box_class = {"Mix$Box", &jrt_Object_class, sizeof(Mix_Box), &Mix_Box_vt_, 0, 0, 0};

static const struct Mix_anon1_vt Mix_anon1_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (double (*)(jobject*))Mix_anon1_area};
static const struct Mix_Shape_vt Mix_anon1_as_Shape = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (double (*)(jobject*))Mix_anon1_area};
static const jitab Mix_anon1_itabs[] = {{&Mix_Shape_class, &Mix_anon1_as_Shape}};
const jclass Mix_anon1_class = {"Mix_anon1", &jrt_Object_class, sizeof(Mix_anon1), &Mix_anon1_vt_, JRT_ITABS(Mix_anon1_itabs), 0};


int main(int argc, char** argv) {
	const char* which = getenv("JRT_MAIN");
	if (which && strcmp(which, "Mix") == 0) return jrt_run_main(Mix_main, argc, argv);
	return jrt_run_main(Mix_main, argc, argv);
}
