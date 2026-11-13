/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from
   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */
#include "jrt.h"

typedef struct Base Base;
typedef struct Sub Sub;
typedef struct CalcMain CalcMain;
typedef struct Calc Calc;
extern const jclass Base_class;
extern const jclass Sub_class;
extern const jclass CalcMain_class;
extern const jclass Calc_class;

struct Base {
	jobject hdr_;
	int32_t size;
	int32_t scale;
};
struct Base_vt {
	jobject_vt o_;
};

struct Sub {
	Base super_;
	int32_t extra;
};
struct Sub_vt {
	jobject_vt o_;
};

struct CalcMain {
	jobject hdr_;
};
struct CalcMain_vt {
	jobject_vt o_;
};

struct Calc {
	jobject hdr_;
	jarray* cells;
	jobject* items;
	jstring* name;
};
struct Calc_vt {
	jobject_vt o_;
	int32_t (*fill)(jobject*, int32_t);
	jstring* (*label)(jobject*, int32_t);
	int32_t (*guarded)(jobject*, int32_t);
	int32_t (*cleanup)(jobject*, int32_t);
	int32_t (*search)(jobject*, int32_t);
	int32_t (*count)(jobject*, int32_t);
};
static int32_t Calc_total = 0;
static jarray* Calc_table = NULL;
static bool Calc_inited_;

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

static void Base_init_I(Base* self, int32_t var1);
static Base* Base_new_I(int32_t var1);
static void Base_init_II(Base* self, int32_t var1, int32_t var2);
static Base* Base_new_II(int32_t var1, int32_t var2);
static void Base_finit(Base* self);
static void Sub_init(Sub* self, int32_t var1);
static Sub* Sub_new(int32_t var1);
static int32_t Sub_build(int32_t);
static void Sub_finit(Sub* self);
static void CalcMain_init(CalcMain* self);
static CalcMain* CalcMain_new(void);
static void CalcMain_main(jarray*);
static void CalcMain_finit(CalcMain* self);
static void Calc_init(Calc* self);
static Calc* Calc_new(void);
static int32_t Calc_fill(jobject*, int32_t);
static int64_t Calc_mix(int64_t, int32_t);
static float Calc_wrap(float);
static jstring* Calc_label(jobject*, int32_t);
static int32_t Calc_guarded(jobject*, int32_t);
static int32_t Calc_cleanup(jobject*, int32_t);
static int32_t Calc_search(jobject*, int32_t);
static int32_t Calc_sum(jarray*);
static int32_t Calc_sums(int32_t);
static int32_t Calc_tabled(int32_t);
static int32_t Calc_made(int32_t);
static int32_t Calc_count(jobject*, int32_t);
static void Calc_finit(Calc* self);
static void Calc_clinit(void);
static inline int32_t Calc_fill_v(jobject* o, int32_t a0) { return ((const struct Calc_vt*)jrt_vt(o))->fill(o, a0); }
static inline jstring* Calc_label_v(jobject* o, int32_t a0) { return ((const struct Calc_vt*)jrt_vt(o))->label(o, a0); }
static inline int32_t Calc_guarded_v(jobject* o, int32_t a0) { return ((const struct Calc_vt*)jrt_vt(o))->guarded(o, a0); }
static inline int32_t Calc_cleanup_v(jobject* o, int32_t a0) { return ((const struct Calc_vt*)jrt_vt(o))->cleanup(o, a0); }
static inline int32_t Calc_search_v(jobject* o, int32_t a0) { return ((const struct Calc_vt*)jrt_vt(o))->search(o, a0); }
static inline int32_t Calc_count_v(jobject* o, int32_t a0) { return ((const struct Calc_vt*)jrt_vt(o))->count(o, a0); }
static void Base_init_I(Base* self, int32_t var1) {
	Base_finit(self);
	self->size = var1 * self->scale;
}

static Base* Base_new_I(int32_t var1) {
	Base* o = (Base*)jrt_alloc(&Base_class);
	Base_init_I(o, var1);
	return o;
}

static void Base_init_II(Base* self, int32_t var1, int32_t var2) {
	Base_init_I(self, var1);
	self->scale += var2;
}

static Base* Base_new_II(int32_t var1, int32_t var2) {
	Base* o = (Base*)jrt_alloc(&Base_class);
	Base_init_II(o, var1, var2);
	return o;
}

static void Base_finit(Base* self) {
	self->scale = 2;
}

static void Sub_init(Sub* self, int32_t var1) {
	Base_init_I((Base*)self, var1 + 1);
	Sub_finit(self);
	self->extra += ((Base*)jrt_nn(self))->size;
}

static Sub* Sub_new(int32_t var1) {
	Sub* o = (Sub*)jrt_alloc(&Sub_class);
	Sub_init(o, var1);
	return o;
}

static int32_t Sub_build(int32_t var0) {
	Sub* var1 = Sub_new(var0);
	return ((Sub*)jrt_nn(var1))->extra * 3 + ((Base*)jrt_nn(var1))->size;
}

static void Sub_finit(Sub* self) {
	self->extra = 7;
}

static void CalcMain_init(CalcMain* self) {
	CalcMain_finit(self);
}

static CalcMain* CalcMain_new(void) {
	CalcMain* o = (CalcMain*)jrt_alloc(&CalcMain_class);
	CalcMain_init(o);
	return o;
}

static void CalcMain_main(jarray* args) {
	Calc* c = Calc_new();
	jarray* inputs = jrt_array_of(sizeof(int32_t), 8, (int32_t[]){0, 1, -1, 7, 13, 100, JRT_INT_MIN, 2147483647});
	{
		int32_t i = 0;
		for (; i < JLEN(inputs); i++) {
			int32_t x = JA(int32_t, inputs, i);
			int32_t o1 = 0;
			int32_t o2 = 0;
			jstring* o3 = NULL;
			int32_t o4 = 0;
			int32_t o5 = 0;
			int32_t o6 = 0;
			int32_t o7 = 0;
			jrt_PrintStream_println_Str(jrt_System_out, (o1 = x, o2 = Calc_fill_v((jobject*)c, x), o3 = Calc_label_v((jobject*)c, x), o4 = Calc_guarded_v((jobject*)c, x), o5 = Calc_cleanup_v((jobject*)c, x), o6 = Calc_search_v((jobject*)c, x), o7 = Calc_count_v((jobject*)c, x), jrt_concat(14, jrt_lit(&lit_1, u"fill(", 5), jrt_str_I(o1), jrt_lit(&lit_2, u")=", 2), jrt_str_I(o2), jrt_lit(&lit_3, u" label=", 7), jrt_str_O((jobject*)o3), jrt_lit(&lit_4, u" guarded=", 9), jrt_str_I(o4), jrt_lit(&lit_5, u" cleanup=", 9), jrt_str_I(o5), jrt_lit(&lit_6, u" search=", 8), jrt_str_I(o6), jrt_lit(&lit_7, u" count=", 7), jrt_str_I(o7))));
			int64_t o8 = 0;
			float o9 = 0;
			int32_t o10 = 0;
			int32_t o11 = 0;
			int32_t o12 = 0;
			int32_t o13 = 0;
			jrt_PrintStream_println_Str(jrt_System_out, (o8 = Calc_mix((int64_t)x * 31LL, x), o9 = Calc_wrap((float)x / 3.0f), o10 = Calc_sums(x), o11 = Calc_tabled(x), o12 = Calc_made(x), o13 = Sub_build(x), jrt_concat(12, jrt_lit(&lit_8, u"  mix=", 6), jrt_str_J(o8), jrt_lit(&lit_9, u" wrap=", 6), jrt_str_F(o9), jrt_lit(&lit_10, u" sums=", 6), jrt_str_I(o10), jrt_lit(&lit_11, u" tabled=", 8), jrt_str_I(o11), jrt_lit(&lit_12, u" made=", 6), jrt_str_I(o12), jrt_lit(&lit_13, u" build=", 7), jrt_str_I(o13))));
		}
	}
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(4, jrt_lit(&lit_14, u"total=", 6), jrt_str_I((Calc_clinit(), Calc_total)), jrt_lit(&lit_15, u" name=", 6), jrt_str_O((jobject*)((Calc*)jrt_nn(c))->name)));
}

static void CalcMain_finit(CalcMain* self) {
}

static void Calc_init(Calc* self) {
	Calc_finit(self);
}

static Calc* Calc_new(void) {
	Calc_clinit();
	Calc* o = (Calc*)jrt_alloc(&Calc_class);
	Calc_init(o);
	return o;
}

static int32_t Calc_fill(jobject* self_, int32_t var1) {
	Calc* self = (Calc*)self_;
	int32_t var2 = 0;
	{
		int32_t var3 = 0;
		for (; var3 < JLEN(self->cells); ++var3) {
			JA(int32_t, self->cells, var3) = var1 * var3 - (jrt_ushr_I(var3, 1));
			var2 += JA(int32_t, self->cells, var3);
		}
	}
	Calc_total += var2;
	return (var2 > 100 ? var2 % 7 : -var2);
}

static int64_t Calc_mix(int64_t var0, int32_t var2) {
	Calc_clinit();
	int64_t var3 = var0 ^ ((int64_t)var2 << 3);
	while (var3 > 1000LL) {
		var3 = (int64_t)jrt_ushr_J(var3, (2 & 63));
	}
	return jrt_ushr_J(var3, 1);
}

static float Calc_wrap(float var0) {
	Calc_clinit();
	return (float)fmod(var0, 3.5f) + (float)(jrt_Math_floor_D((double)var0));
}

static jstring* Calc_label(jobject* self_, int32_t var1) {
	Calc* self = (Calc*)self_;
	jstring* var2 = (var1 < 0 ? NULL : jrt_concat(3, jrt_str_O((jobject*)self->name), jrt_lit(&lit_16, u":", 1), jrt_str_I(var1)));
	return ((jobject*)var2 == (jobject*)NULL ? jrt_lit(&lit_17, u"none", 4) : jrt_concat(2, jrt_str_O((jobject*)var2), jrt_str_C((char16_t)((65 + (var1 & 7))))));
}

static int32_t Calc_guarded(jobject* self_, int32_t var1) {
	Calc* self = (Calc*)self_;
	Calc* volatile var2 = (var1 > 0 ? NULL : self);
	{
		jrt_try t1;
		jrt_push(&t1);
		if (setjmp(t1.jb) == 0) {
			{ int32_t r_ = JLEN(((Calc*)jrt_nn(var2))->cells) + var1; jrt_pop(&t1); return r_; }
			jrt_pop(&t1);
		} else {
			jrt_pop(&t1);
			if (jrt_instanceof(t1.ex, &jrt_Exception_class)) {
				jrt_Throwable* var3 = (jrt_Throwable*)t1.ex;
				return -7;
			} else {
				jrt_throw(t1.ex);
			}
		}
	}
}

static int32_t Calc_cleanup(jobject* self_, int32_t var1) {
	Calc* self = (Calc*)self_;
	volatile int32_t var2 = 0;
	{
		jrt_try f1;
		jrt_push(&f1);
		if (setjmp(f1.jb) == 0) {
			{
				jrt_try t2;
				jrt_push(&t2);
				if (setjmp(t2.jb) == 0) {
					var2 = jrt_div_I(100, (var1 & 3));
					jrt_pop(&t2);
				} else {
					jrt_pop(&t2);
					if (jrt_instanceof(t2.ex, &jrt_ArithmeticException_class)) {
						jrt_Throwable* var3 = (jrt_Throwable*)t2.ex;
						var2 = -1;
					} else {
						jrt_throw(t2.ex);
					}
				}
			}
			jrt_pop(&f1);
		} else {
			jrt_pop(&f1);
			Calc_total += 1;
			jrt_throw(f1.ex);
		}
		Calc_total += 1;
	}
	return var2 + Calc_total;
}

static int32_t Calc_search(jobject* self_, int32_t var1) {
	Calc* self = (Calc*)self_;
	int32_t var2 = 0;
	{
		int32_t var3 = 0;
		for (; var3 < 5; ++var3) {
			{
				{
					int32_t var4 = 0;
					for (; var4 < 5; ++var4) {
						if (var3 * var4 == (var1 & 15)) {
							var2 = var3 * 10 + var4;
							goto label9_brk;
						}
						if (var4 > var3) {
							goto label9_cont;
						}
					}
				}
			}
			label9_cont:;
		}
	}
	label9_brk:;
	return var2;
}

static int32_t Calc_sum(jarray* var0) {
	Calc_clinit();
	int32_t var1 = 0;
	{
		int32_t var2 = 0;
		for (; var2 < JLEN(var0); ++var2) {
			var1 += JA(int32_t, var0, var2);
		}
	}
	return var1;
}

static int32_t Calc_sums(int32_t var0) {
	Calc_clinit();
	int32_t o1 = 0;
	int32_t o2 = 0;
	int32_t o3 = 0;
	int32_t o4 = 0;
	int32_t o5 = 0;
	int32_t o6 = 0;
	return (o5 = (o3 = (o1 = Calc_sum(jrt_new_array(sizeof(int32_t), 0)), o2 = Calc_sum(jrt_array_of(sizeof(int32_t), 1, (int32_t[]){var0})), o1 + o2), o4 = Calc_sum(jrt_array_of(sizeof(int32_t), 3, (int32_t[]){var0, 2, 3})), o3 + o4), o6 = Calc_sum(jrt_array_of(sizeof(int32_t), 2, (int32_t[]){var0, var0})), o5 + o6);
}

static int32_t Calc_tabled(int32_t var0) {
	Calc_clinit();
	return JA(int32_t, Calc_table, var0 & 3) + Calc_total;
}

static int32_t Calc_made(int32_t var0) {
	Calc_clinit();
	Base* var1 = Base_new_II(var0, 3);
	return ((Base*)jrt_nn(var1))->size + ((Base*)jrt_nn(var1))->scale;
}

static int32_t Calc_count(jobject* self_, int32_t var1) {
	Calc* self = (Calc*)self_;
	jrt_List_add_O(self->items, (jobject*)self->name);
	switch (var1 & 3) {
	case 0:
	{
		return jrt_List_size_(self->items);
	}
	case 1:
	{
		jrt_List_clear_(self->items);
	}
	default:
	{
		return -1;
	}
	}
}

static void Calc_finit(Calc* self) {
	self->cells = jrt_new_array(sizeof(int32_t), 8);
	self->items = jrt_ArrayList_new_();
	self->name = jrt_lit(&lit_18, u"calc", 4);
}

static void Calc_clinit(void) {
	if (Calc_inited_) return;
	Calc_inited_ = true;
	Calc_table = jrt_new_array(sizeof(int32_t), 4);
	JA(int32_t, Calc_table, 1) = 5;
	Calc_total = JA(int32_t, Calc_table, 1) * 2;
}

static const struct Base_vt Base_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Base_class = {"Base", &jrt_Object_class, sizeof(Base), &Base_vt_, 0, 0, 0};

static const struct Sub_vt Sub_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Sub_class = {"Sub", &Base_class, sizeof(Sub), &Sub_vt_, 0, 0, 0};

static const struct CalcMain_vt CalcMain_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass CalcMain_class = {"CalcMain", &jrt_Object_class, sizeof(CalcMain), &CalcMain_vt_, 0, 0, 0};

static const struct Calc_vt Calc_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (int32_t (*)(jobject*, int32_t))Calc_fill, (jstring* (*)(jobject*, int32_t))Calc_label, (int32_t (*)(jobject*, int32_t))Calc_guarded, (int32_t (*)(jobject*, int32_t))Calc_cleanup, (int32_t (*)(jobject*, int32_t))Calc_search, (int32_t (*)(jobject*, int32_t))Calc_count};
const jclass Calc_class = {"Calc", &jrt_Object_class, sizeof(Calc), &Calc_vt_, 0, 0, 0};


int main(int argc, char** argv) {
	const char* which = getenv("JRT_MAIN");
	if (which && strcmp(which, "CalcMain") == 0) return jrt_run_main(CalcMain_main, argc, argv);
	return jrt_run_main(CalcMain_main, argc, argv);
}
