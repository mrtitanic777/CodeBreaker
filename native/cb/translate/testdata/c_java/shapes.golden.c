/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from
   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */
#include "jrt.h"

typedef struct Shapes Shapes;
typedef struct Shapes_Circle Shapes_Circle;
typedef struct Shapes_Rect Shapes_Rect;
typedef struct Shapes_Square Shapes_Square;
typedef struct Shapes_Size Shapes_Size;
extern const jclass Shapes_class;
extern const jclass Shapes_Shape_class;
extern const jclass Shapes_Circle_class;
extern const jclass Shapes_Rect_class;
extern const jclass Shapes_Square_class;
extern const jclass Shapes_Size_class;

struct Shapes {
	jobject hdr_;
};
struct Shapes_vt {
	jobject_vt o_;
};

struct Shapes_Shape_vt {
	jobject_vt o_;
	double (*area)(jobject*);
	jstring* (*describe)(jobject*);
	jstring* (*name)(jobject*);
};

struct Shapes_Circle {
	jobject hdr_;
	double r;
};
struct Shapes_Circle_vt {
	jobject_vt o_;
	double (*area)(jobject*);
	jstring* (*name)(jobject*);
	double (*r)(jobject*);
};

struct Shapes_Rect {
	jobject hdr_;
	int32_t w;
	int32_t h;
};
struct Shapes_Rect_vt {
	jobject_vt o_;
	double (*area)(jobject*);
	jstring* (*name)(jobject*);
	int32_t (*w)(jobject*);
	int32_t (*h)(jobject*);
};

struct Shapes_Square {
	jobject hdr_;
	int32_t side;
};
struct Shapes_Square_vt {
	jobject_vt o_;
	double (*area)(jobject*);
	jstring* (*name)(jobject*);
	int32_t (*side)(jobject*);
};

struct Shapes_Size {
	jrt_Enum super_;
};
struct Shapes_Size_vt {
	jobject_vt o_;
};
static Shapes_Size* Shapes_Size_SMALL;
static Shapes_Size* Shapes_Size_MEDIUM;
static Shapes_Size* Shapes_Size_LARGE;
static jarray* Shapes_Size_values_;
static bool Shapes_Size_inited_;

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
static jstring* lit_20;
static jstring* lit_21;
static jstring* lit_22;
static jstring* lit_23;
static jstring* lit_24;
static jstring* lit_25;
static jstring* lit_26;
static jstring* lit_27;
static jstring* lit_28;
static jstring* lit_29;
static jstring* lit_30;
static jstring* lit_31;
static jstring* lit_32;
static jstring* lit_33;
static jstring* lit_34;
static jstring* lit_35;
static jstring* lit_36;
static jstring* lit_37;

static void Shapes_init(Shapes* self);
static Shapes* Shapes_new(void);
static Shapes_Size* Shapes_classify(double);
static jstring* Shapes_sizeWord(int32_t);
static int32_t Shapes_sides(jobject*);
static int32_t Shapes_rectArea(int32_t, int32_t);
static bool Shapes_sameRect(int32_t, int32_t);
static jstring* Shapes_rectText(int32_t, int32_t);
static int32_t Shapes_rectHash(int32_t, int32_t);
static jstring* Shapes_dayKind(jstring*);
static int32_t Shapes_countKinds(int32_t);
static void Shapes_main(jarray*);
static void Shapes_finit(Shapes* self);
static jstring* Shapes_Shape_describe(jobject*);
static void Shapes_Circle_init(Shapes_Circle* self, double r);
static Shapes_Circle* Shapes_Circle_new(double r);
static double Shapes_Circle_area(jobject*);
static jstring* Shapes_Circle_name(jobject*);
static double Shapes_Circle_r(jobject*);
static jstring* Shapes_Circle_toString(jobject*);
static int32_t Shapes_Circle_hashCode(jobject*);
static bool Shapes_Circle_equals(jobject*, jobject*);
static void Shapes_Circle_finit(Shapes_Circle* self);
static void Shapes_Rect_init(Shapes_Rect* self, int32_t w, int32_t h);
static Shapes_Rect* Shapes_Rect_new(int32_t w, int32_t h);
static double Shapes_Rect_area(jobject*);
static jstring* Shapes_Rect_name(jobject*);
static int32_t Shapes_Rect_w(jobject*);
static int32_t Shapes_Rect_h(jobject*);
static jstring* Shapes_Rect_toString(jobject*);
static int32_t Shapes_Rect_hashCode(jobject*);
static bool Shapes_Rect_equals(jobject*, jobject*);
static void Shapes_Rect_finit(Shapes_Rect* self);
static void Shapes_Square_init(Shapes_Square* self, int32_t side);
static Shapes_Square* Shapes_Square_new(int32_t side);
static double Shapes_Square_area(jobject*);
static jstring* Shapes_Square_name(jobject*);
static int32_t Shapes_Square_side(jobject*);
static jstring* Shapes_Square_toString(jobject*);
static int32_t Shapes_Square_hashCode(jobject*);
static bool Shapes_Square_equals(jobject*, jobject*);
static void Shapes_Square_finit(Shapes_Square* self);
static void Shapes_Size_init(Shapes_Size* self, jstring* name_in, int32_t ordinal_in);
static Shapes_Size* Shapes_Size_new(jstring* name_in, int32_t ordinal_in);
static void Shapes_Size_finit(Shapes_Size* self);
static void Shapes_Size_clinit(void);
static jarray* Shapes_Size_values(void);
static inline double Shapes_Shape_area_i(jobject* o) { return ((const struct Shapes_Shape_vt*)jrt_ivt(o, &Shapes_Shape_class))->area(o); }
static inline jstring* Shapes_Shape_describe_i(jobject* o) { return ((const struct Shapes_Shape_vt*)jrt_ivt(o, &Shapes_Shape_class))->describe(o); }
static inline jstring* Shapes_Shape_name_i(jobject* o) { return ((const struct Shapes_Shape_vt*)jrt_ivt(o, &Shapes_Shape_class))->name(o); }
static inline double Shapes_Circle_area_v(jobject* o) { return ((const struct Shapes_Circle_vt*)jrt_vt(o))->area(o); }
static inline jstring* Shapes_Circle_name_v(jobject* o) { return ((const struct Shapes_Circle_vt*)jrt_vt(o))->name(o); }
static inline double Shapes_Circle_r_v(jobject* o) { return ((const struct Shapes_Circle_vt*)jrt_vt(o))->r(o); }
static inline double Shapes_Rect_area_v(jobject* o) { return ((const struct Shapes_Rect_vt*)jrt_vt(o))->area(o); }
static inline jstring* Shapes_Rect_name_v(jobject* o) { return ((const struct Shapes_Rect_vt*)jrt_vt(o))->name(o); }
static inline int32_t Shapes_Rect_w_v(jobject* o) { return ((const struct Shapes_Rect_vt*)jrt_vt(o))->w(o); }
static inline int32_t Shapes_Rect_h_v(jobject* o) { return ((const struct Shapes_Rect_vt*)jrt_vt(o))->h(o); }
static inline double Shapes_Square_area_v(jobject* o) { return ((const struct Shapes_Square_vt*)jrt_vt(o))->area(o); }
static inline jstring* Shapes_Square_name_v(jobject* o) { return ((const struct Shapes_Square_vt*)jrt_vt(o))->name(o); }
static inline int32_t Shapes_Square_side_v(jobject* o) { return ((const struct Shapes_Square_vt*)jrt_vt(o))->side(o); }
static void Shapes_init(Shapes* self) {
	Shapes_finit(self);
}

static Shapes* Shapes_new(void) {
	Shapes* o = (Shapes*)jrt_alloc(&Shapes_class);
	Shapes_init(o);
	return o;
}

static Shapes_Size* Shapes_classify(double area) {
	if (area < 10) {
		return (Shapes_Size_clinit(), Shapes_Size_SMALL);
	}
	if (area < 100) {
		return (Shapes_Size_clinit(), Shapes_Size_MEDIUM);
	}
	return (Shapes_Size_clinit(), Shapes_Size_LARGE);
}

static jstring* Shapes_sizeWord(int32_t k) {
	Shapes_Size* s = (Shapes_Size*)jrt_cast((jobject*)JA(jobject*, Shapes_Size_values(), jrt_Math_floorMod_II(k, 3)), &Shapes_Size_class);
	{
		jstring* j2x_sw1 = NULL;
		switch (jrt_Enum_ordinal_(s)) {
		case 0:
		{
			{
				j2x_sw1 = jrt_lit(&lit_1, u"s", 1);
				goto j2x_sw2_brk;
			}
		}
		case 1:
		{
			{
				j2x_sw1 = jrt_lit(&lit_2, u"m", 1);
				goto j2x_sw2_brk;
			}
		}
		case 2:
		{
			{
				jstring* big = jrt_lit(&lit_3, u"L", 1);
				{
					j2x_sw1 = jrt_concat(2, jrt_str_O((jobject*)big), jrt_str_O((jobject*)big));
					goto j2x_sw2_brk;
				}
			}
		}
		}
		j2x_sw2_brk:;
		return j2x_sw1;
	}
}

static int32_t Shapes_sides(jobject* o) {
	Shapes_Square* sq = NULL;
	if (jrt_instanceof((jobject*)o, &Shapes_Square_class) && (jobject*)(sq = (Shapes_Square*)jrt_cast((jobject*)o, &Shapes_Square_class)) != (jobject*)NULL) {
		return 4 + Shapes_Square_side_v((jobject*)sq) * 0;
	}
	Shapes_Rect* r = NULL;
	int32_t o1 = 0;
	int32_t o2 = 0;
	if ((jrt_instanceof((jobject*)o, &Shapes_Rect_class) && (jobject*)(r = (Shapes_Rect*)jrt_cast((jobject*)o, &Shapes_Rect_class)) != (jobject*)NULL) && (o1 = Shapes_Rect_w_v((jobject*)r), o2 = Shapes_Rect_h_v((jobject*)r), o1 != o2)) {
		return 4;
	}
	if (jrt_instanceof((jobject*)o, &Shapes_Circle_class)) {
		return 0;
	}
	return -1;
}

static int32_t Shapes_rectArea(int32_t w, int32_t h) {
	Shapes_Rect* r = Shapes_Rect_new(jrt_Math_abs_I(w) % 1000, jrt_Math_abs_I(h) % 1000);
	return jrt_d2i(Shapes_Rect_area_v((jobject*)r));
}

static bool Shapes_sameRect(int32_t a, int32_t b) {
	Shapes_Rect* o1 = NULL;
	Shapes_Rect* o2 = NULL;
	return (o1 = Shapes_Rect_new(a & 15, b & 15), o2 = Shapes_Rect_new(b & 15, a & 15), jrt_equals((jobject*)o1, (jobject*)o2));
}

static jstring* Shapes_rectText(int32_t a, int32_t b) {
	return jrt_toString((jobject*)Shapes_Rect_new(a & 255, b & 255));
}

static int32_t Shapes_rectHash(int32_t a, int32_t b) {
	return jrt_hashCode((jobject*)Shapes_Rect_new(a & 255, b & 255));
}

static jstring* Shapes_dayKind(jstring* day) {
	{
		jstring* j2x_sw3 = NULL;
		{
			jstring* j2x_s5 = (day);
			{
				if (jrt_String_equals_O(jrt_lit(&lit_4, u"sat", 3), (jobject*)j2x_s5) || jrt_String_equals_O(jrt_lit(&lit_5, u"sun", 3), (jobject*)j2x_s5)) {
					{
						j2x_sw3 = jrt_lit(&lit_6, u"weekend", 7);
						goto j2x_sw4_brk;
					}
				} else if (jrt_String_equals_O(jrt_lit(&lit_7, u"mon", 3), (jobject*)j2x_s5) || jrt_String_equals_O(jrt_lit(&lit_8, u"tue", 3), (jobject*)j2x_s5) || jrt_String_equals_O(jrt_lit(&lit_9, u"wed", 3), (jobject*)j2x_s5) || jrt_String_equals_O(jrt_lit(&lit_10, u"thu", 3), (jobject*)j2x_s5) || jrt_String_equals_O(jrt_lit(&lit_11, u"fri", 3), (jobject*)j2x_s5)) {
					{
						j2x_sw3 = jrt_lit(&lit_12, u"weekday", 7);
						goto j2x_sw4_brk;
					}
				} else {
					{
						j2x_sw3 = jrt_lit(&lit_13, u"unknown", 7);
						goto j2x_sw4_brk;
					}
				}
			}
			j2x_sw4_brk:;
		}
		return j2x_sw3;
	}
}

static int32_t Shapes_countKinds(int32_t n) {
	jobject* shapes = jrt_ArrayList_new_();
	{
		int32_t i = 0;
		for (; i < n % 20; i++) {
			{
				jobject* j2x_sw6 = NULL;
				switch (i % 3) {
				case 0:
				{
					{
						j2x_sw6 = (jobject*)Shapes_Circle_new((double)(i));
						goto j2x_sw7_brk;
					}
				}
				case 1:
				{
					{
						j2x_sw6 = (jobject*)Shapes_Rect_new(i, i + 1);
						goto j2x_sw7_brk;
					}
				}
				default:
				{
					{
						j2x_sw6 = (jobject*)Shapes_Square_new(i);
						goto j2x_sw7_brk;
					}
				}
				}
				j2x_sw7_brk:;
				jrt_List_add_O(shapes, j2x_sw6);
			}
		}
	}
	int32_t total = 0;
	{
		jobject* j2x_it8 = jrt_List_iterator_(shapes);
		for (; jrt_Iterator_hasNext_(j2x_it8); ) {
			jobject* s = jrt_Iterator_next_(j2x_it8);
			total += Shapes_sides(s);
		}
	}
	return total;
}

static void Shapes_main(jarray* args) {
	Shapes_Circle* volatile o1 = NULL;
	Shapes_Rect* volatile o2 = NULL;
	Shapes_Square* volatile o3 = NULL;
	Shapes_Rect* volatile o4 = NULL;
	jobject* volatile shapes = (o1 = Shapes_Circle_new(1.5), o2 = Shapes_Rect_new(3, 4), o3 = Shapes_Square_new(5), o4 = Shapes_Rect_new(2, 2), jrt_List_of(4, (jobject*)o1, (jobject*)o2, (jobject*)o3, (jobject*)o4));
	volatile double sum = (double)(0);
	{
		jobject* volatile j2x_it9 = jrt_List_iterator_(shapes);
		for (; jrt_Iterator_hasNext_(j2x_it9); ) {
			jobject* volatile s = jrt_Iterator_next_(j2x_it9);
			jstring* volatile o5 = NULL;
			Shapes_Size* volatile o6 = NULL;
			volatile int32_t o7 = 0;
			jrt_PrintStream_println_Str(jrt_System_out, (o5 = Shapes_Shape_describe_i((jobject*)s), o6 = Shapes_classify(Shapes_Shape_area_i((jobject*)s)), o7 = Shapes_sides(s), jrt_concat(5, jrt_str_O((jobject*)o5), jrt_lit(&lit_14, u" -> ", 4), jrt_str_O((jobject*)o6), jrt_lit(&lit_15, u" sides=", 7), jrt_str_I(o7))));
			sum += Shapes_Shape_area_i((jobject*)s);
		}
	}
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_16, u"total ", 6), jrt_str_D(sum)));
	Shapes_Rect* volatile o8 = NULL;
	Shapes_Circle* volatile o9 = NULL;
	Shapes_Rect* volatile o10 = NULL;
	Shapes_Rect* volatile o11 = NULL;
	volatile bool o12 = 0;
	jrt_PrintStream_println_Str(jrt_System_out, (o8 = Shapes_Rect_new(1, 2), o9 = Shapes_Circle_new(0.5), o12 = (o10 = Shapes_Rect_new(1, 2), o11 = Shapes_Rect_new(1, 2), jrt_equals((jobject*)o10, (jobject*)o11)), jrt_concat(5, jrt_str_O((jobject*)o8), jrt_lit(&lit_17, u" ", 1), jrt_str_O((jobject*)o9), jrt_lit(&lit_17, u" ", 1), jrt_str_Z(o12))));
	{
		volatile int32_t k = 0;
		for (; k < 4; k++) {
			jrt_PrintStream_print_Str(jrt_System_out, jrt_concat(2, jrt_str_O((jobject*)Shapes_sizeWord(k)), jrt_lit(&lit_18, u",", 1)));
		}
	}
	jrt_PrintStream_println_(jrt_System_out);
	jstring* volatile o13 = NULL;
	jstring* volatile o14 = NULL;
	jstring* volatile o15 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (o13 = Shapes_dayKind(jrt_lit(&lit_5, u"sun", 3)), o14 = Shapes_dayKind(jrt_lit(&lit_9, u"wed", 3)), o15 = Shapes_dayKind(jrt_lit(&lit_19, u"xyz", 3)), jrt_concat(5, jrt_str_O((jobject*)o13), jrt_lit(&lit_17, u" ", 1), jrt_str_O((jobject*)o14), jrt_lit(&lit_17, u" ", 1), jrt_str_O((jobject*)o15))));
	{
		jrt_try t16;
		jrt_push(&t16);
		if (setjmp(t16.jb) == 0) {
			Shapes_Rect_new(-1, 2);
			jrt_pop(&t16);
		} else {
			jrt_pop(&t16);
			if (jrt_instanceof(t16.ex, &jrt_IllegalArgumentException_class)) {
				jrt_Throwable* e = (jrt_Throwable*)t16.ex;
				jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_20, u"caught ", 7), jrt_str_O((jobject*)jrt_Throwable_getMessage_((jobject*)e))));
			} else {
				jrt_throw(t16.ex);
			}
		}
	}
	jrt_PrintStream_println_I(jrt_System_out, Shapes_countKinds(10));
}

static void Shapes_finit(Shapes* self) {
}

static jstring* Shapes_Shape_describe(jobject* self_) {
	jobject* self = self_;
	jstring* o1 = NULL;
	jstring* o2 = NULL;
	return (o1 = Shapes_Shape_name_i((jobject*)self), o2 = jrt_String_format(jrt_lit(&lit_21, u"%.2f", 4), 1, jrt_box_D(Shapes_Shape_area_i((jobject*)self))), jrt_concat(3, jrt_str_O((jobject*)o1), jrt_lit(&lit_22, u" with area ", 11), jrt_str_O((jobject*)o2)));
}

static void Shapes_Circle_init(Shapes_Circle* self, double r) {
	Shapes_Circle_finit(self);
	self->r = r;
}

static Shapes_Circle* Shapes_Circle_new(double r) {
	Shapes_Circle* o = (Shapes_Circle*)jrt_alloc(&Shapes_Circle_class);
	Shapes_Circle_init(o, r);
	return o;
}

static double Shapes_Circle_area(jobject* self_) {
	Shapes_Circle* self = (Shapes_Circle*)self_;
	return JRT_PI * self->r * self->r;
}

static jstring* Shapes_Circle_name(jobject* self_) {
	Shapes_Circle* self = (Shapes_Circle*)self_;
	return jrt_lit(&lit_23, u"circle", 6);
}

static double Shapes_Circle_r(jobject* self_) {
	Shapes_Circle* self = (Shapes_Circle*)self_;
	return self->r;
}

static jstring* Shapes_Circle_toString(jobject* self_) {
	Shapes_Circle* self = (Shapes_Circle*)self_;
	return jrt_concat(4, jrt_lit(&lit_24, u"Circle[", 7), jrt_lit(&lit_25, u"r=", 2), jrt_str_D(self->r), jrt_lit(&lit_26, u"]", 1));
}

static int32_t Shapes_Circle_hashCode(jobject* self_) {
	Shapes_Circle* self = (Shapes_Circle*)self_;
	int32_t h = 0;
	h = 31 * h + jrt_Double_hashCode_D(self->r);
	return h;
}

static bool Shapes_Circle_equals(jobject* self_, jobject* o) {
	Shapes_Circle* self = (Shapes_Circle*)self_;
	if (!(jrt_instanceof((jobject*)o, &Shapes_Circle_class))) {
		return false;
	}
	Shapes_Circle* that = (Shapes_Circle*)jrt_cast((jobject*)o, &Shapes_Circle_class);
	return jrt_Double_compare_DD(self->r, ((Shapes_Circle*)jrt_nn(that))->r) == 0;
}

static void Shapes_Circle_finit(Shapes_Circle* self) {
}

static void Shapes_Rect_init(Shapes_Rect* self, int32_t w, int32_t h) {
	Shapes_Rect_finit(self);
	if (w < 0 || h < 0) {
		jrt_throw((jobject*)jrt_new_throwable(&jrt_IllegalArgumentException_class, jrt_lit(&lit_27, u"negative side", 13), NULL));
	}
	self->w = w;
	self->h = h;
}

static Shapes_Rect* Shapes_Rect_new(int32_t w, int32_t h) {
	Shapes_Rect* o = (Shapes_Rect*)jrt_alloc(&Shapes_Rect_class);
	Shapes_Rect_init(o, w, h);
	return o;
}

static double Shapes_Rect_area(jobject* self_) {
	Shapes_Rect* self = (Shapes_Rect*)self_;
	return (double)(self->w * self->h);
}

static jstring* Shapes_Rect_name(jobject* self_) {
	Shapes_Rect* self = (Shapes_Rect*)self_;
	return jrt_lit(&lit_28, u"rect", 4);
}

static int32_t Shapes_Rect_w(jobject* self_) {
	Shapes_Rect* self = (Shapes_Rect*)self_;
	return self->w;
}

static int32_t Shapes_Rect_h(jobject* self_) {
	Shapes_Rect* self = (Shapes_Rect*)self_;
	return self->h;
}

static jstring* Shapes_Rect_toString(jobject* self_) {
	Shapes_Rect* self = (Shapes_Rect*)self_;
	return jrt_concat(6, jrt_lit(&lit_29, u"Rect[", 5), jrt_lit(&lit_30, u"w=", 2), jrt_str_I(self->w), jrt_lit(&lit_31, u", h=", 4), jrt_str_I(self->h), jrt_lit(&lit_26, u"]", 1));
}

static int32_t Shapes_Rect_hashCode(jobject* self_) {
	Shapes_Rect* self = (Shapes_Rect*)self_;
	int32_t h = 0;
	h = 31 * h + jrt_Integer_hashCode_I(self->w);
	h = 31 * h + jrt_Integer_hashCode_I(self->h);
	return h;
}

static bool Shapes_Rect_equals(jobject* self_, jobject* o) {
	Shapes_Rect* self = (Shapes_Rect*)self_;
	if (!(jrt_instanceof((jobject*)o, &Shapes_Rect_class))) {
		return false;
	}
	Shapes_Rect* that = (Shapes_Rect*)jrt_cast((jobject*)o, &Shapes_Rect_class);
	return self->w == ((Shapes_Rect*)jrt_nn(that))->w && self->h == ((Shapes_Rect*)jrt_nn(that))->h;
}

static void Shapes_Rect_finit(Shapes_Rect* self) {
}

static void Shapes_Square_init(Shapes_Square* self, int32_t side) {
	Shapes_Square_finit(self);
	self->side = side;
}

static Shapes_Square* Shapes_Square_new(int32_t side) {
	Shapes_Square* o = (Shapes_Square*)jrt_alloc(&Shapes_Square_class);
	Shapes_Square_init(o, side);
	return o;
}

static double Shapes_Square_area(jobject* self_) {
	Shapes_Square* self = (Shapes_Square*)self_;
	return (double)(self->side * self->side);
}

static jstring* Shapes_Square_name(jobject* self_) {
	Shapes_Square* self = (Shapes_Square*)self_;
	return jrt_lit(&lit_32, u"square", 6);
}

static int32_t Shapes_Square_side(jobject* self_) {
	Shapes_Square* self = (Shapes_Square*)self_;
	return self->side;
}

static jstring* Shapes_Square_toString(jobject* self_) {
	Shapes_Square* self = (Shapes_Square*)self_;
	return jrt_concat(4, jrt_lit(&lit_33, u"Square[", 7), jrt_lit(&lit_34, u"side=", 5), jrt_str_I(self->side), jrt_lit(&lit_26, u"]", 1));
}

static int32_t Shapes_Square_hashCode(jobject* self_) {
	Shapes_Square* self = (Shapes_Square*)self_;
	int32_t h = 0;
	h = 31 * h + jrt_Integer_hashCode_I(self->side);
	return h;
}

static bool Shapes_Square_equals(jobject* self_, jobject* o) {
	Shapes_Square* self = (Shapes_Square*)self_;
	if (!(jrt_instanceof((jobject*)o, &Shapes_Square_class))) {
		return false;
	}
	Shapes_Square* that = (Shapes_Square*)jrt_cast((jobject*)o, &Shapes_Square_class);
	return self->side == ((Shapes_Square*)jrt_nn(that))->side;
}

static void Shapes_Square_finit(Shapes_Square* self) {
}

static void Shapes_Size_init(Shapes_Size* self, jstring* name_in, int32_t ordinal_in) {
	((jrt_Enum*)self)->name = name_in;
	((jrt_Enum*)self)->ordinal = ordinal_in;
	Shapes_Size_finit(self);
}

static Shapes_Size* Shapes_Size_new(jstring* name_in, int32_t ordinal_in) {
	Shapes_Size_clinit();
	Shapes_Size* o = (Shapes_Size*)jrt_alloc(&Shapes_Size_class);
	Shapes_Size_init(o, name_in, ordinal_in);
	return o;
}

static void Shapes_Size_finit(Shapes_Size* self) {
}

static void Shapes_Size_clinit(void) {
	if (Shapes_Size_inited_) return;
	Shapes_Size_inited_ = true;
	Shapes_Size_SMALL = Shapes_Size_new(jrt_lit(&lit_35, u"SMALL", 5), 0);
	Shapes_Size_MEDIUM = Shapes_Size_new(jrt_lit(&lit_36, u"MEDIUM", 6), 1);
	Shapes_Size_LARGE = Shapes_Size_new(jrt_lit(&lit_37, u"LARGE", 5), 2);
	Shapes_Size_values_ = jrt_new_array(sizeof(jobject*), 3);
	JA(jobject*, Shapes_Size_values_, 0) = (jobject*)Shapes_Size_SMALL;
	JA(jobject*, Shapes_Size_values_, 1) = (jobject*)Shapes_Size_MEDIUM;
	JA(jobject*, Shapes_Size_values_, 2) = (jobject*)Shapes_Size_LARGE;
}

static jarray* Shapes_Size_values(void) {
	Shapes_Size_clinit();
	return jrt_array_clone(Shapes_Size_values_);
}

static const struct Shapes_vt Shapes_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Shapes_class = {"Shapes", &jrt_Object_class, sizeof(Shapes), &Shapes_vt_, 0, 0, 0};

const jclass Shapes_Shape_class = {"Shapes.Shape", 0, 0, 0, 0, 0, 1};

static const struct Shapes_Circle_vt Shapes_Circle_vt_ = {{Shapes_Circle_toString, Shapes_Circle_equals, Shapes_Circle_hashCode}, (double (*)(jobject*))Shapes_Circle_area, (jstring* (*)(jobject*))Shapes_Circle_name, (double (*)(jobject*))Shapes_Circle_r};
static const struct Shapes_Shape_vt Shapes_Circle_as_Shape = {{Shapes_Circle_toString, Shapes_Circle_equals, Shapes_Circle_hashCode}, (double (*)(jobject*))Shapes_Circle_area, (jstring* (*)(jobject*))Shapes_Shape_describe, (jstring* (*)(jobject*))Shapes_Circle_name};
static const jitab Shapes_Circle_itabs[] = {{&Shapes_Shape_class, &Shapes_Circle_as_Shape}};
const jclass Shapes_Circle_class = {"Shapes.Circle", &jrt_Object_class, sizeof(Shapes_Circle), &Shapes_Circle_vt_, JRT_ITABS(Shapes_Circle_itabs), 0};

static const struct Shapes_Rect_vt Shapes_Rect_vt_ = {{Shapes_Rect_toString, Shapes_Rect_equals, Shapes_Rect_hashCode}, (double (*)(jobject*))Shapes_Rect_area, (jstring* (*)(jobject*))Shapes_Rect_name, (int32_t (*)(jobject*))Shapes_Rect_w, (int32_t (*)(jobject*))Shapes_Rect_h};
static const struct Shapes_Shape_vt Shapes_Rect_as_Shape = {{Shapes_Rect_toString, Shapes_Rect_equals, Shapes_Rect_hashCode}, (double (*)(jobject*))Shapes_Rect_area, (jstring* (*)(jobject*))Shapes_Shape_describe, (jstring* (*)(jobject*))Shapes_Rect_name};
static const jitab Shapes_Rect_itabs[] = {{&Shapes_Shape_class, &Shapes_Rect_as_Shape}};
const jclass Shapes_Rect_class = {"Shapes.Rect", &jrt_Object_class, sizeof(Shapes_Rect), &Shapes_Rect_vt_, JRT_ITABS(Shapes_Rect_itabs), 0};

static const struct Shapes_Square_vt Shapes_Square_vt_ = {{Shapes_Square_toString, Shapes_Square_equals, Shapes_Square_hashCode}, (double (*)(jobject*))Shapes_Square_area, (jstring* (*)(jobject*))Shapes_Square_name, (int32_t (*)(jobject*))Shapes_Square_side};
static const struct Shapes_Shape_vt Shapes_Square_as_Shape = {{Shapes_Square_toString, Shapes_Square_equals, Shapes_Square_hashCode}, (double (*)(jobject*))Shapes_Square_area, (jstring* (*)(jobject*))Shapes_Shape_describe, (jstring* (*)(jobject*))Shapes_Square_name};
static const jitab Shapes_Square_itabs[] = {{&Shapes_Shape_class, &Shapes_Square_as_Shape}};
const jclass Shapes_Square_class = {"Shapes.Square", &jrt_Object_class, sizeof(Shapes_Square), &Shapes_Square_vt_, JRT_ITABS(Shapes_Square_itabs), 0};

static const struct Shapes_Size_vt Shapes_Size_vt_ = {{jrt_Enum_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Shapes_Size_class = {"Shapes.Size", &jrt_Enum_class, sizeof(Shapes_Size), &Shapes_Size_vt_, 0, 0, 0};


int main(int argc, char** argv) {
	const char* which = getenv("JRT_MAIN");
	if (which && strcmp(which, "Shapes") == 0) return jrt_run_main(Shapes_main, argc, argv);
	return jrt_run_main(Shapes_main, argc, argv);
}
