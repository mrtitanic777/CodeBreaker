/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from
   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */
#include "jrt.h"

typedef struct Modern Modern;
typedef struct Modern_Point Modern_Point;
typedef struct Modern_Sq Modern_Sq;
typedef struct Modern_Circ Modern_Circ;
typedef struct Modern_Color Modern_Color;
extern const jclass Modern_class;
extern const jclass Modern_Point_class;
extern const jclass Modern_Shape_class;
extern const jclass Modern_Sq_class;
extern const jclass Modern_Circ_class;
extern const jclass Modern_Color_class;

struct Modern {
	jobject hdr_;
};
struct Modern_vt {
	jobject_vt o_;
};

struct Modern_Point {
	jobject hdr_;
	int32_t x;
	int32_t y;
};
struct Modern_Point_vt {
	jobject_vt o_;
	int32_t (*dist2)(jobject*);
	int32_t (*x)(jobject*);
	int32_t (*y)(jobject*);
};

struct Modern_Shape_vt {
	jobject_vt o_;
};

struct Modern_Sq {
	jobject hdr_;
	int32_t side;
};
struct Modern_Sq_vt {
	jobject_vt o_;
	int32_t (*side)(jobject*);
};

struct Modern_Circ {
	jobject hdr_;
	int32_t r;
};
struct Modern_Circ_vt {
	jobject_vt o_;
	int32_t (*r)(jobject*);
};

struct Modern_Color {
	jrt_Enum super_;
};
struct Modern_Color_vt {
	jobject_vt o_;
};
static Modern_Color* Modern_Color_RED;
static Modern_Color* Modern_Color_GREEN;
static Modern_Color* Modern_Color_BLUE;
static jarray* Modern_Color_values_;
static bool Modern_Color_inited_;

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

static void Modern_init(Modern* self);
static Modern* Modern_new(void);
static int32_t Modern_area(jobject*);
static jstring* Modern_name(Modern_Color*);
static int32_t Modern_sum(jobject*);
static int32_t Modern_squares(int32_t);
static jstring* Modern_words(jstring*);
static void Modern_main(jarray*);
static void Modern_finit(Modern* self);
static void Modern_Point_init(Modern_Point* self, int32_t x, int32_t y);
static Modern_Point* Modern_Point_new(int32_t x, int32_t y);
static int32_t Modern_Point_dist2(jobject*);
static int32_t Modern_Point_x(jobject*);
static int32_t Modern_Point_y(jobject*);
static jstring* Modern_Point_toString(jobject*);
static int32_t Modern_Point_hashCode(jobject*);
static bool Modern_Point_equals(jobject*, jobject*);
static void Modern_Point_finit(Modern_Point* self);
static void Modern_Sq_init(Modern_Sq* self, int32_t side);
static Modern_Sq* Modern_Sq_new(int32_t side);
static int32_t Modern_Sq_side(jobject*);
static jstring* Modern_Sq_toString(jobject*);
static int32_t Modern_Sq_hashCode(jobject*);
static bool Modern_Sq_equals(jobject*, jobject*);
static void Modern_Sq_finit(Modern_Sq* self);
static void Modern_Circ_init(Modern_Circ* self, int32_t r);
static Modern_Circ* Modern_Circ_new(int32_t r);
static int32_t Modern_Circ_r(jobject*);
static jstring* Modern_Circ_toString(jobject*);
static int32_t Modern_Circ_hashCode(jobject*);
static bool Modern_Circ_equals(jobject*, jobject*);
static void Modern_Circ_finit(Modern_Circ* self);
static void Modern_Color_init(Modern_Color* self, jstring* name_in, int32_t ordinal_in);
static Modern_Color* Modern_Color_new(jstring* name_in, int32_t ordinal_in);
static void Modern_Color_finit(Modern_Color* self);
static void Modern_Color_clinit(void);
static jarray* Modern_Color_values(void);
static inline int32_t Modern_Point_dist2_v(jobject* o) { return ((const struct Modern_Point_vt*)jrt_vt(o))->dist2(o); }
static inline int32_t Modern_Point_x_v(jobject* o) { return ((const struct Modern_Point_vt*)jrt_vt(o))->x(o); }
static inline int32_t Modern_Point_y_v(jobject* o) { return ((const struct Modern_Point_vt*)jrt_vt(o))->y(o); }
static inline int32_t Modern_Sq_side_v(jobject* o) { return ((const struct Modern_Sq_vt*)jrt_vt(o))->side(o); }
static inline int32_t Modern_Circ_r_v(jobject* o) { return ((const struct Modern_Circ_vt*)jrt_vt(o))->r(o); }
struct Lambda1 {
	jobject hdr_;
};
static int32_t Lambda1_fn(jobject* self_, int32_t x) {
	struct Lambda1* self = (struct Lambda1*)self_;
	(void)self;
	return x * x;
}
static const jrt_IntUnaryOperator_vt Lambda1_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda1_fn};
static const jitab Lambda1_itabs[] = {{&jrt_IntUnaryOperator_class, &Lambda1_vt}};
static const jclass Lambda1_class = {"Modern$$Lambda", &jrt_Object_class, sizeof(struct Lambda1), &jrt_Object_vt, JRT_ITABS(Lambda1_itabs), 0};
static jobject* Lambda1_new(void) {
	struct Lambda1* o = (struct Lambda1*)jrt_alloc(&Lambda1_class);
	return (jobject*)o;
}

struct Lambda2 {
	jobject hdr_;
};
static jobject* Lambda2_fn(jobject* self_, jobject* v) {
	struct Lambda2* self = (struct Lambda2*)self_;
	(void)self;
	return jrt_box_I(jrt_unbox_I((jobject*)v) * 2);
}
static const jrt_Function_vt Lambda2_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda2_fn};
static const jitab Lambda2_itabs[] = {{&jrt_Function_class, &Lambda2_vt}};
static const jclass Lambda2_class = {"Modern$$Lambda", &jrt_Object_class, sizeof(struct Lambda2), &jrt_Object_vt, JRT_ITABS(Lambda2_itabs), 0};
static jobject* Lambda2_new(void) {
	struct Lambda2* o = (struct Lambda2*)jrt_alloc(&Lambda2_class);
	return (jobject*)o;
}

struct Lambda3 {
	jobject hdr_;
};
static jobject* Lambda3_fn(jobject* self_, jobject* a0) {
	struct Lambda3* self = (struct Lambda3*)self_;
	(void)self;
	return (jobject*)jrt_String_valueOf_O(a0);
}
static const jrt_Function_vt Lambda3_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda3_fn};
static const jitab Lambda3_itabs[] = {{&jrt_Function_class, &Lambda3_vt}};
static const jclass Lambda3_class = {"Modern$$Lambda", &jrt_Object_class, sizeof(struct Lambda3), &jrt_Object_vt, JRT_ITABS(Lambda3_itabs), 0};
static jobject* Lambda3_new(void) {
	struct Lambda3* o = (struct Lambda3*)jrt_alloc(&Lambda3_class);
	return (jobject*)o;
}

struct Lambda4 {
	jobject hdr_;
};
static bool Lambda4_fn(jobject* self_, jobject* v) {
	struct Lambda4* self = (struct Lambda4*)self_;
	(void)self;
	return jrt_unbox_I((jobject*)v) % 2 == 0;
}
static const jrt_Predicate_vt Lambda4_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda4_fn};
static const jitab Lambda4_itabs[] = {{&jrt_Predicate_class, &Lambda4_vt}};
static const jclass Lambda4_class = {"Modern$$Lambda", &jrt_Object_class, sizeof(struct Lambda4), &jrt_Object_vt, JRT_ITABS(Lambda4_itabs), 0};
static jobject* Lambda4_new(void) {
	struct Lambda4* o = (struct Lambda4*)jrt_alloc(&Lambda4_class);
	return (jobject*)o;
}

struct Lambda5 {
	jobject hdr_;
};
static jobject* Lambda5_fn(jobject* self_, jobject* a0, jobject* a1) {
	struct Lambda5* self = (struct Lambda5*)self_;
	(void)self;
	return jrt_box_I(jrt_Integer_sum_II(jrt_unbox_I((jobject*)a0), jrt_unbox_I((jobject*)a1)));
}
static const jrt_BiFunction_vt Lambda5_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda5_fn};
static const jitab Lambda5_itabs[] = {{&jrt_BiFunction_class, &Lambda5_vt}};
static const jclass Lambda5_class = {"Modern$$Lambda", &jrt_Object_class, sizeof(struct Lambda5), &jrt_Object_vt, JRT_ITABS(Lambda5_itabs), 0};
static jobject* Lambda5_new(void) {
	struct Lambda5* o = (struct Lambda5*)jrt_alloc(&Lambda5_class);
	return (jobject*)o;
}

static void Modern_init(Modern* self) {
	Modern_finit(self);
}

static Modern* Modern_new(void) {
	Modern* o = (Modern*)jrt_alloc(&Modern_class);
	Modern_init(o);
	return o;
}

static int32_t Modern_area(jobject* s) {
	Modern_Sq* q = NULL;
	if (jrt_instanceof((jobject*)s, &Modern_Sq_class) && (jobject*)(q = (Modern_Sq*)jrt_cast((jobject*)s, &Modern_Sq_class)) != (jobject*)NULL) {
		int32_t jrt_o1 = 0;
		int32_t jrt_o2 = 0;
		return (jrt_o1 = Modern_Sq_side_v((jobject*)q), jrt_o2 = Modern_Sq_side_v((jobject*)q), jrt_o1 * jrt_o2);
	}
	Modern_Circ* c = NULL;
	if (jrt_instanceof((jobject*)s, &Modern_Circ_class) && (jobject*)(c = (Modern_Circ*)jrt_cast((jobject*)s, &Modern_Circ_class)) != (jobject*)NULL) {
		int32_t jrt_o3 = 0;
		int32_t jrt_o4 = 0;
		return (jrt_o3 = 3 * Modern_Circ_r_v((jobject*)c), jrt_o4 = Modern_Circ_r_v((jobject*)c), jrt_o3 * jrt_o4);
	}
	return 0;
}

static jstring* Modern_name(Modern_Color* c) {
	{
		jstring* j2x_sw1 = NULL;
		switch (jrt_Enum_ordinal_(c)) {
		case 0:
		{
			{
				j2x_sw1 = jrt_lit(&lit_1, u"r", 1);
				goto j2x_sw2_brk;
			}
		}
		case 1:
		{
			{
				j2x_sw1 = jrt_lit(&lit_2, u"g", 1);
				goto j2x_sw2_brk;
			}
		}
		default:
		{
			{
				jstring* b = jrt_lit(&lit_3, u"b", 1);
				{
					j2x_sw1 = jrt_concat(2, jrt_str_O((jobject*)b), jrt_str_O((jobject*)b));
					goto j2x_sw2_brk;
				}
			}
		}
		}
		j2x_sw2_brk:;
		return j2x_sw1;
	}
}

static int32_t Modern_sum(jobject* xs) {
	int32_t s = 0;
	{
		jobject* j2x_it3 = jrt_List_iterator_(xs);
		for (; jrt_Iterator_hasNext_(j2x_it3); ) {
			int32_t v = jrt_unbox_I((jobject*)jrt_Iterator_next_(j2x_it3));
			s += v;
		}
	}
	return s;
}

static int32_t Modern_squares(int32_t n) {
	return jrt_IntStream_sum_(jrt_IntStream_map_O(jrt_IntStream_rangeClosed_II(1, jrt_Math_abs_I(n) % 50), Lambda1_new()));
}

static jstring* Modern_words(jstring* s) {
	{
		jstring* j2x_sw4 = NULL;
		{
			jstring* j2x_s6 = (s);
			{
				if (jrt_String_equals_O(jrt_lit(&lit_4, u"a", 1), (jobject*)j2x_s6) || jrt_String_equals_O(jrt_lit(&lit_3, u"b", 1), (jobject*)j2x_s6)) {
					{
						j2x_sw4 = jrt_lit(&lit_5, u"ab", 2);
						goto j2x_sw5_brk;
					}
				} else {
					{
						j2x_sw4 = jrt_lit(&lit_6, u"other", 5);
						goto j2x_sw5_brk;
					}
				}
			}
			j2x_sw5_brk:;
		}
		return j2x_sw4;
	}
}

static void Modern_main(jarray* args) {
	jobject* xs = jrt_ArrayList_new_();
	{
		int32_t i = 0;
		for (; i < 6; i++) {
			jrt_List_add_O(xs, jrt_box_I(i * i));
		}
	}
	jobject* twice = Lambda2_new();
	jstring* jrt_o1 = NULL;
	jstring* jrt_o2 = NULL;
	jstring* jrt_o3 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (jrt_o1 = jrt_str_I(Modern_sum(xs)), jrt_o2 = jrt_str_O((jobject*)jrt_Function_apply_O(twice, jrt_box_I(21))), jrt_o3 = jrt_str_I(Modern_squares(10)), jrt_concat(5, jrt_o1, jrt_lit(&lit_7, u" ", 1), jrt_o2, jrt_lit(&lit_7, u" ", 1), jrt_o3)));
	jstring* jrt_o4 = NULL;
	jstring* jrt_o5 = NULL;
	Modern_Point* jrt_o6 = NULL;
	Modern_Point* jrt_o7 = NULL;
	jstring* jrt_o8 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (jrt_o4 = jrt_str_O((jobject*)Modern_Point_new(3, 4)), jrt_o5 = jrt_str_I(Modern_Point_dist2_v((jobject*)Modern_Point_new(3, 4))), jrt_o8 = jrt_str_Z((jrt_o6 = Modern_Point_new(1, 2), jrt_o7 = Modern_Point_new(1, 2), jrt_equals((jobject*)jrt_o6, (jobject*)jrt_o7))), jrt_concat(5, jrt_o4, jrt_lit(&lit_7, u" ", 1), jrt_o5, jrt_lit(&lit_7, u" ", 1), jrt_o8)));
	int32_t jrt_o9 = 0;
	int32_t jrt_o10 = 0;
	jstring* jrt_o11 = NULL;
	jstring* jrt_o12 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (jrt_o9 = Modern_area((jobject*)Modern_Sq_new(4)), jrt_o10 = Modern_area((jobject*)Modern_Circ_new(2)), jrt_o11 = Modern_name((Modern_Color_clinit(), Modern_Color_BLUE)), jrt_o12 = Modern_words(jrt_lit(&lit_3, u"b", 1)), jrt_concat(7, jrt_str_I(jrt_o9), jrt_lit(&lit_7, u" ", 1), jrt_str_I(jrt_o10), jrt_lit(&lit_7, u" ", 1), jrt_str_O((jobject*)jrt_o11), jrt_lit(&lit_7, u" ", 1), jrt_str_O((jobject*)jrt_o12))));
	jobject* jrt_o13 = NULL;
	jobject* jrt_o14 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (jrt_o13 = jrt_Stream_map_O(jrt_Stream_filter_O(jrt_Collection_stream_(xs), Lambda4_new()), Lambda3_new()), jrt_o14 = jrt_Collectors_joining_O((jobject*)jrt_lit(&lit_8, u",", 1)), (jstring*)jrt_cast((jobject*)jrt_Stream_collect_O(jrt_o13, jrt_o14), &jrt_String_class)));
	jobject* m = jrt_HashMap_new_();
	{
		jarray* j2x_a7 = jrt_String_split_Str(jrt_lit(&lit_9, u"x y x z y x", 11), jrt_lit(&lit_7, u" ", 1));
		{
			int32_t j2x_i8 = 0;
			for (; j2x_i8 < JLEN(j2x_a7); j2x_i8++) {
				jstring* w = JA(jstring*, j2x_a7, j2x_i8);
				jrt_Map_merge_OOO(m, (jobject*)w, jrt_box_I(1), Lambda5_new());
			}
		}
	}
	jrt_PrintStream_println_O(jrt_System_out, m);
}

static void Modern_finit(Modern* self) {
}

static void Modern_Point_init(Modern_Point* self, int32_t x, int32_t y) {
	Modern_Point_finit(self);
	self->x = x;
	self->y = y;
}

static Modern_Point* Modern_Point_new(int32_t x, int32_t y) {
	Modern_Point* o = (Modern_Point*)jrt_alloc(&Modern_Point_class);
	Modern_Point_init(o, x, y);
	return o;
}

static int32_t Modern_Point_dist2(jobject* self_) {
	Modern_Point* self = (Modern_Point*)self_;
	return self->x * self->x + self->y * self->y;
}

static int32_t Modern_Point_x(jobject* self_) {
	Modern_Point* self = (Modern_Point*)self_;
	return self->x;
}

static int32_t Modern_Point_y(jobject* self_) {
	Modern_Point* self = (Modern_Point*)self_;
	return self->y;
}

static jstring* Modern_Point_toString(jobject* self_) {
	Modern_Point* self = (Modern_Point*)self_;
	return jrt_concat(6, jrt_lit(&lit_10, u"Point[", 6), jrt_lit(&lit_11, u"x=", 2), jrt_str_I(self->x), jrt_lit(&lit_12, u", y=", 4), jrt_str_I(self->y), jrt_lit(&lit_13, u"]", 1));
}

static int32_t Modern_Point_hashCode(jobject* self_) {
	Modern_Point* self = (Modern_Point*)self_;
	int32_t h = 0;
	h = 31 * h + jrt_Integer_hashCode_I(self->x);
	h = 31 * h + jrt_Integer_hashCode_I(self->y);
	return h;
}

static bool Modern_Point_equals(jobject* self_, jobject* o) {
	Modern_Point* self = (Modern_Point*)self_;
	if (!(jrt_instanceof((jobject*)o, &Modern_Point_class))) {
		return false;
	}
	Modern_Point* that = (Modern_Point*)jrt_cast((jobject*)o, &Modern_Point_class);
	return self->x == ((Modern_Point*)jrt_nn(that))->x && self->y == ((Modern_Point*)jrt_nn(that))->y;
}

static void Modern_Point_finit(Modern_Point* self) {
}

static void Modern_Sq_init(Modern_Sq* self, int32_t side) {
	Modern_Sq_finit(self);
	self->side = side;
}

static Modern_Sq* Modern_Sq_new(int32_t side) {
	Modern_Sq* o = (Modern_Sq*)jrt_alloc(&Modern_Sq_class);
	Modern_Sq_init(o, side);
	return o;
}

static int32_t Modern_Sq_side(jobject* self_) {
	Modern_Sq* self = (Modern_Sq*)self_;
	return self->side;
}

static jstring* Modern_Sq_toString(jobject* self_) {
	Modern_Sq* self = (Modern_Sq*)self_;
	return jrt_concat(4, jrt_lit(&lit_14, u"Sq[", 3), jrt_lit(&lit_15, u"side=", 5), jrt_str_I(self->side), jrt_lit(&lit_13, u"]", 1));
}

static int32_t Modern_Sq_hashCode(jobject* self_) {
	Modern_Sq* self = (Modern_Sq*)self_;
	int32_t h = 0;
	h = 31 * h + jrt_Integer_hashCode_I(self->side);
	return h;
}

static bool Modern_Sq_equals(jobject* self_, jobject* o) {
	Modern_Sq* self = (Modern_Sq*)self_;
	if (!(jrt_instanceof((jobject*)o, &Modern_Sq_class))) {
		return false;
	}
	Modern_Sq* that = (Modern_Sq*)jrt_cast((jobject*)o, &Modern_Sq_class);
	return self->side == ((Modern_Sq*)jrt_nn(that))->side;
}

static void Modern_Sq_finit(Modern_Sq* self) {
}

static void Modern_Circ_init(Modern_Circ* self, int32_t r) {
	Modern_Circ_finit(self);
	self->r = r;
}

static Modern_Circ* Modern_Circ_new(int32_t r) {
	Modern_Circ* o = (Modern_Circ*)jrt_alloc(&Modern_Circ_class);
	Modern_Circ_init(o, r);
	return o;
}

static int32_t Modern_Circ_r(jobject* self_) {
	Modern_Circ* self = (Modern_Circ*)self_;
	return self->r;
}

static jstring* Modern_Circ_toString(jobject* self_) {
	Modern_Circ* self = (Modern_Circ*)self_;
	return jrt_concat(4, jrt_lit(&lit_16, u"Circ[", 5), jrt_lit(&lit_17, u"r=", 2), jrt_str_I(self->r), jrt_lit(&lit_13, u"]", 1));
}

static int32_t Modern_Circ_hashCode(jobject* self_) {
	Modern_Circ* self = (Modern_Circ*)self_;
	int32_t h = 0;
	h = 31 * h + jrt_Integer_hashCode_I(self->r);
	return h;
}

static bool Modern_Circ_equals(jobject* self_, jobject* o) {
	Modern_Circ* self = (Modern_Circ*)self_;
	if (!(jrt_instanceof((jobject*)o, &Modern_Circ_class))) {
		return false;
	}
	Modern_Circ* that = (Modern_Circ*)jrt_cast((jobject*)o, &Modern_Circ_class);
	return self->r == ((Modern_Circ*)jrt_nn(that))->r;
}

static void Modern_Circ_finit(Modern_Circ* self) {
}

static void Modern_Color_init(Modern_Color* self, jstring* name_in, int32_t ordinal_in) {
	((jrt_Enum*)self)->name = name_in;
	((jrt_Enum*)self)->ordinal = ordinal_in;
	Modern_Color_finit(self);
}

static Modern_Color* Modern_Color_new(jstring* name_in, int32_t ordinal_in) {
	Modern_Color_clinit();
	Modern_Color* o = (Modern_Color*)jrt_alloc(&Modern_Color_class);
	Modern_Color_init(o, name_in, ordinal_in);
	return o;
}

static void Modern_Color_finit(Modern_Color* self) {
}

static void Modern_Color_clinit(void) {
	if (Modern_Color_inited_) return;
	Modern_Color_inited_ = true;
	Modern_Color_RED = Modern_Color_new(jrt_lit(&lit_18, u"RED", 3), 0);
	Modern_Color_GREEN = Modern_Color_new(jrt_lit(&lit_19, u"GREEN", 5), 1);
	Modern_Color_BLUE = Modern_Color_new(jrt_lit(&lit_20, u"BLUE", 4), 2);
	Modern_Color_values_ = jrt_new_array(sizeof(jobject*), 3);
	JA(jobject*, Modern_Color_values_, 0) = (jobject*)Modern_Color_RED;
	JA(jobject*, Modern_Color_values_, 1) = (jobject*)Modern_Color_GREEN;
	JA(jobject*, Modern_Color_values_, 2) = (jobject*)Modern_Color_BLUE;
}

static jarray* Modern_Color_values(void) {
	Modern_Color_clinit();
	return jrt_array_clone(Modern_Color_values_);
}

static const struct Modern_vt Modern_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Modern_class = {"Modern", &jrt_Object_class, sizeof(Modern), &Modern_vt_, 0, 0, 0};

static const struct Modern_Point_vt Modern_Point_vt_ = {{Modern_Point_toString, Modern_Point_equals, Modern_Point_hashCode}, (int32_t (*)(jobject*))Modern_Point_dist2, (int32_t (*)(jobject*))Modern_Point_x, (int32_t (*)(jobject*))Modern_Point_y};
const jclass Modern_Point_class = {"Modern$Point", &jrt_Object_class, sizeof(Modern_Point), &Modern_Point_vt_, 0, 0, 0};

const jclass Modern_Shape_class = {"Modern$Shape", 0, 0, 0, 0, 0, 1};

static const struct Modern_Sq_vt Modern_Sq_vt_ = {{Modern_Sq_toString, Modern_Sq_equals, Modern_Sq_hashCode}, (int32_t (*)(jobject*))Modern_Sq_side};
static const struct Modern_Shape_vt Modern_Sq_as_Shape = {{Modern_Sq_toString, Modern_Sq_equals, Modern_Sq_hashCode}};
static const jitab Modern_Sq_itabs[] = {{&Modern_Shape_class, &Modern_Sq_as_Shape}};
const jclass Modern_Sq_class = {"Modern$Sq", &jrt_Object_class, sizeof(Modern_Sq), &Modern_Sq_vt_, JRT_ITABS(Modern_Sq_itabs), 0};

static const struct Modern_Circ_vt Modern_Circ_vt_ = {{Modern_Circ_toString, Modern_Circ_equals, Modern_Circ_hashCode}, (int32_t (*)(jobject*))Modern_Circ_r};
static const struct Modern_Shape_vt Modern_Circ_as_Shape = {{Modern_Circ_toString, Modern_Circ_equals, Modern_Circ_hashCode}};
static const jitab Modern_Circ_itabs[] = {{&Modern_Shape_class, &Modern_Circ_as_Shape}};
const jclass Modern_Circ_class = {"Modern$Circ", &jrt_Object_class, sizeof(Modern_Circ), &Modern_Circ_vt_, JRT_ITABS(Modern_Circ_itabs), 0};

static const struct Modern_Color_vt Modern_Color_vt_ = {{jrt_Enum_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Modern_Color_class = {"Modern$Color", &jrt_Enum_class, sizeof(Modern_Color), &Modern_Color_vt_, 0, 0, 0};


int main(int argc, char** argv) {
	const char* which = getenv("JRT_MAIN");
	if (which && strcmp(which, "Modern") == 0) return jrt_run_main(Modern_main, argc, argv);
	return jrt_run_main(Modern_main, argc, argv);
}
