/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from
   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */
#include "jrt.h"

typedef struct Zoo Zoo;
typedef struct Zoo_Base Zoo_Base;
typedef struct Zoo_Dog Zoo_Dog;
typedef struct Zoo_Cat Zoo_Cat;
typedef struct Zoo_Pair Zoo_Pair;
typedef struct Zoo_Empty Zoo_Empty;
extern const jclass Zoo_class;
extern const jclass Zoo_Animal_class;
extern const jclass Zoo_Base_class;
extern const jclass Zoo_Dog_class;
extern const jclass Zoo_Cat_class;
extern const jclass Zoo_Pair_class;
extern const jclass Zoo_Empty_class;

struct Zoo {
	jobject hdr_;
};
struct Zoo_vt {
	jobject_vt o_;
};

struct Zoo_Animal_vt {
	jobject_vt o_;
	jstring* (*sound)(jobject*);
	jstring* (*greet)(jobject*);
	jstring* (*name)(jobject*);
};

struct Zoo_Base {
	jobject hdr_;
	jstring* name;
};
struct Zoo_Base_vt {
	jobject_vt o_;
	jstring* (*name)(jobject*);
	void (*describe)(jobject*);
};
static int32_t Zoo_Base_made = 0;

struct Zoo_Dog {
	Zoo_Base super_;
	int32_t barks;
};
struct Zoo_Dog_vt {
	jobject_vt o_;
	jstring* (*name)(jobject*);
	void (*describe)(jobject*);
	jstring* (*sound)(jobject*);
};

struct Zoo_Cat {
	Zoo_Base super_;
};
struct Zoo_Cat_vt {
	jobject_vt o_;
	jstring* (*name)(jobject*);
	void (*describe)(jobject*);
	jstring* (*sound)(jobject*);
};

struct Zoo_Pair {
	jobject hdr_;
	jstring* a;
	int32_t b;
};
struct Zoo_Pair_vt {
	jobject_vt o_;
	jstring* (*a)(jobject*);
	int32_t (*b)(jobject*);
};

struct Zoo_Empty {
	jrt_Throwable super_;
};
struct Zoo_Empty_vt {
	jobject_vt o_;
	jstring* (*getMessage)(jobject*);
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
static jstring* lit_20;

static void Zoo_init(Zoo* self);
static Zoo* Zoo_new(void);
static int32_t Zoo_pick(int32_t);
static int32_t Zoo_sumTo(int32_t);
static void Zoo_main(jarray*);
static void Zoo_finit(Zoo* self);
static jstring* Zoo_Animal_greet(jobject*);
static void Zoo_Base_init(Zoo_Base* self, jstring* n);
static Zoo_Base* Zoo_Base_new(jstring* n);
static jstring* Zoo_Base_name(jobject*);
static void Zoo_Base_describe(jobject*);
static void Zoo_Base_finit(Zoo_Base* self);
static void Zoo_Dog_init(Zoo_Dog* self, jstring* n);
static Zoo_Dog* Zoo_Dog_new(jstring* n);
static jstring* Zoo_Dog_sound(jobject*);
static void Zoo_Dog_describe(jobject*);
static void Zoo_Dog_finit(Zoo_Dog* self);
static void Zoo_Cat_init(Zoo_Cat* self, jstring* n);
static Zoo_Cat* Zoo_Cat_new(jstring* n);
static jstring* Zoo_Cat_sound(jobject*);
static void Zoo_Cat_finit(Zoo_Cat* self);
static void Zoo_Pair_init(Zoo_Pair* self, jstring* a, int32_t b);
static Zoo_Pair* Zoo_Pair_new(jstring* a, int32_t b);
static jstring* Zoo_Pair_a(jobject*);
static int32_t Zoo_Pair_b(jobject*);
static jstring* Zoo_Pair_toString(jobject*);
static int32_t Zoo_Pair_hashCode(jobject*);
static bool Zoo_Pair_equals(jobject*, jobject*);
static void Zoo_Pair_finit(Zoo_Pair* self);
static void Zoo_Empty_init(Zoo_Empty* self, jstring* m);
static Zoo_Empty* Zoo_Empty_new(jstring* m);
static void Zoo_Empty_finit(Zoo_Empty* self);
static inline jstring* Zoo_Animal_sound_i(jobject* o) { return ((const struct Zoo_Animal_vt*)jrt_ivt(o, &Zoo_Animal_class))->sound(o); }
static inline jstring* Zoo_Animal_greet_i(jobject* o) { return ((const struct Zoo_Animal_vt*)jrt_ivt(o, &Zoo_Animal_class))->greet(o); }
static inline jstring* Zoo_Animal_name_i(jobject* o) { return ((const struct Zoo_Animal_vt*)jrt_ivt(o, &Zoo_Animal_class))->name(o); }
static inline jstring* Zoo_Base_name_v(jobject* o) { return ((const struct Zoo_Base_vt*)jrt_vt(o))->name(o); }
static inline void Zoo_Base_describe_v(jobject* o) { return ((const struct Zoo_Base_vt*)jrt_vt(o))->describe(o); }
static inline jstring* Zoo_Dog_sound_v(jobject* o) { return ((const struct Zoo_Dog_vt*)jrt_vt(o))->sound(o); }
static inline jstring* Zoo_Cat_sound_v(jobject* o) { return ((const struct Zoo_Cat_vt*)jrt_vt(o))->sound(o); }
static inline jstring* Zoo_Pair_a_v(jobject* o) { return ((const struct Zoo_Pair_vt*)jrt_vt(o))->a(o); }
static inline int32_t Zoo_Pair_b_v(jobject* o) { return ((const struct Zoo_Pair_vt*)jrt_vt(o))->b(o); }
static inline jstring* Zoo_Empty_getMessage_v(jobject* o) { return ((const struct Zoo_Empty_vt*)jrt_vt(o))->getMessage(o); }
struct Lambda1 {
	jobject hdr_;
};
static int32_t Lambda1_fn(jobject* self_, jobject* x_in, jobject* y_in) {
	struct Lambda1* self = (struct Lambda1*)self_;
	(void)self;
	jstring* x = (jstring*)jrt_cast((jobject*)x_in, &jrt_String_class);
	jstring* y = (jstring*)jrt_cast((jobject*)y_in, &jrt_String_class);
	return jrt_String_length_(x) - jrt_String_length_(y);
}
static const jrt_Comparator_vt Lambda1_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda1_fn};
static const jitab Lambda1_itabs[] = {{&jrt_Comparator_class, &Lambda1_vt}};
static const jclass Lambda1_class = {"Zoo$$Lambda", &jrt_Object_class, sizeof(struct Lambda1), &jrt_Object_vt, JRT_ITABS(Lambda1_itabs), 0};
static jobject* Lambda1_new(void) {
	struct Lambda1* o = (struct Lambda1*)jrt_alloc(&Lambda1_class);
	return (jobject*)o;
}

static void Zoo_init(Zoo* self) {
	Zoo_finit(self);
}

static Zoo* Zoo_new(void) {
	Zoo* o = (Zoo*)jrt_alloc(&Zoo_class);
	Zoo_init(o);
	return o;
}

static int32_t Zoo_pick(int32_t k) {
	if (k < 0) {
		jrt_throw((jobject*)Zoo_Empty_new(jrt_concat(2, jrt_lit(&lit_1, u"negative ", 9), jrt_str_I(k))));
	}
	return k * 3;
}

static int32_t Zoo_sumTo(int32_t n) {
	int32_t s = 0;
	{
		jarray* j2x_a1 = jrt_array_of(sizeof(int32_t), 4, (int32_t[]){1, 2, 3, n});
		{
			int32_t j2x_i2 = 0;
			for (; j2x_i2 < JLEN(j2x_a1); j2x_i2++) {
				int32_t i = JA(int32_t, j2x_a1, j2x_i2);
				s += i;
			}
		}
	}
	return s;
}

static void Zoo_main(jarray* args) {
	jobject* volatile zoo = jrt_ArrayList_new_();
	jrt_List_add_O(zoo, (jobject*)Zoo_Dog_new(jrt_lit(&lit_2, u"rex", 3)));
	jrt_List_add_O(zoo, (jobject*)Zoo_Cat_new(jrt_lit(&lit_3, u"tom", 3)));
	{
		jobject* volatile j2x_it3 = jrt_List_iterator_(zoo);
		for (; jrt_Iterator_hasNext_(j2x_it3); ) {
			jobject* volatile a = jrt_Iterator_next_(j2x_it3);
			jrt_PrintStream_println_Str(jrt_System_out, Zoo_Animal_greet_i((jobject*)a));
		}
	}
	jstring* volatile jrt_o1 = NULL;
	jstring* volatile jrt_o2 = NULL;
	jstring* volatile jrt_o3 = NULL;
	jstring* volatile jrt_o4 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (jrt_o1 = jrt_str_I(Zoo_Base_made), jrt_o2 = jrt_str_I(((Zoo_Dog*)jrt_nn(((Zoo_Dog*)jrt_cast((jobject*)jrt_List_get_I(zoo, 0), &Zoo_Dog_class))))->barks), jrt_o3 = jrt_str_O((jobject*)Zoo_Pair_new(jrt_lit(&lit_5, u"x", 1), 2)), jrt_o4 = jrt_str_I(Zoo_sumTo(4)), jrt_concat(7, jrt_o1, jrt_lit(&lit_4, u" ", 1), jrt_o2, jrt_lit(&lit_4, u" ", 1), jrt_o3, jrt_lit(&lit_4, u" ", 1), jrt_o4)));
	{
		jrt_try f5;
		jrt_push(&f5);
		if (setjmp(f5.jb) == 0) {
			{
				jrt_try t6;
				jrt_push(&t6);
				if (setjmp(t6.jb) == 0) {
					Zoo_pick(-2);
					jrt_pop(&t6);
				} else {
					jrt_pop(&t6);
					if (jrt_instanceof(t6.ex, &Zoo_Empty_class)) {
						Zoo_Empty* e = (Zoo_Empty*)t6.ex;
						jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_6, u"caught ", 7), jrt_str_O((jobject*)jrt_Throwable_getMessage_((jobject*)e))));
					} else {
						jrt_throw(t6.ex);
					}
				}
			}
			jrt_pop(&f5);
		} else {
			jrt_pop(&f5);
			jrt_PrintStream_println_Str(jrt_System_out, jrt_lit(&lit_7, u"finally", 7));
			jrt_throw(f5.ex);
		}
		jrt_PrintStream_println_Str(jrt_System_out, jrt_lit(&lit_7, u"finally", 7));
	}
	jobject* volatile m = jrt_LinkedHashMap_new_();
	jrt_Map_put_OO(m, (jobject*)jrt_lit(&lit_8, u"one", 3), jrt_box_I(1));
	jrt_Map_put_OO(m, (jobject*)jrt_lit(&lit_9, u"two", 3), jrt_box_I(2));
	jstring* volatile jrt_o7 = NULL;
	jstring* volatile jrt_o8 = NULL;
	jstring* volatile jrt_o9 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (jrt_o7 = jrt_str_O((jobject*)m), jrt_o8 = jrt_str_I(Zoo_pick(5)), jrt_o9 = jrt_str_O((jobject*)jrt_String_format(jrt_lit(&lit_10, u"%5.2f|%d", 8), 2, jrt_box_D(3.14159), jrt_box_I(42))), jrt_concat(5, jrt_o7, jrt_lit(&lit_4, u" ", 1), jrt_o8, jrt_lit(&lit_4, u" ", 1), jrt_o9)));
	jobject* volatile byLen = Lambda1_new();
	jobject* volatile ws = jrt_ArrayList_new_O(jrt_List_of(3, (jobject*)jrt_lit(&lit_11, u"ccc", 3), (jobject*)jrt_lit(&lit_12, u"a", 1), (jobject*)jrt_lit(&lit_13, u"bb", 2)));
	jrt_List_sort_O(ws, byLen);
	jrt_PrintStream_println_O(jrt_System_out, ws);
}

static void Zoo_finit(Zoo* self) {
}

static jstring* Zoo_Animal_greet(jobject* self_) {
	jobject* self = self_;
	jstring* jrt_o1 = NULL;
	jstring* jrt_o2 = NULL;
	return (jrt_o1 = Zoo_Animal_name_i((jobject*)self), jrt_o2 = Zoo_Animal_sound_i((jobject*)self), jrt_concat(3, jrt_str_O((jobject*)jrt_o1), jrt_lit(&lit_14, u" says ", 6), jrt_str_O((jobject*)jrt_o2)));
}

static void Zoo_Base_init(Zoo_Base* self, jstring* n) {
	Zoo_Base_finit(self);
	self->name = n;
	Zoo_Base_made++;
	Zoo_Base_describe_v((jobject*)self);
}

static Zoo_Base* Zoo_Base_new(jstring* n) {
	Zoo_Base* o = (Zoo_Base*)jrt_alloc(&Zoo_Base_class);
	Zoo_Base_init(o, n);
	return o;
}

static jstring* Zoo_Base_name(jobject* self_) {
	Zoo_Base* self = (Zoo_Base*)self_;
	return self->name;
}

static void Zoo_Base_describe(jobject* self_) {
	Zoo_Base* self = (Zoo_Base*)self_;
}

static void Zoo_Base_finit(Zoo_Base* self) {
}

static void Zoo_Dog_init(Zoo_Dog* self, jstring* n) {
	Zoo_Base_init((Zoo_Base*)self, n);
	Zoo_Dog_finit(self);
}

static Zoo_Dog* Zoo_Dog_new(jstring* n) {
	Zoo_Dog* o = (Zoo_Dog*)jrt_alloc(&Zoo_Dog_class);
	Zoo_Dog_init(o, n);
	return o;
}

static jstring* Zoo_Dog_sound(jobject* self_) {
	Zoo_Dog* self = (Zoo_Dog*)self_;
	return jrt_lit(&lit_15, u"woof", 4);
}

static void Zoo_Dog_describe(jobject* self_) {
	Zoo_Dog* self = (Zoo_Dog*)self_;
	self->barks = 1;
}

static void Zoo_Dog_finit(Zoo_Dog* self) {
}

static void Zoo_Cat_init(Zoo_Cat* self, jstring* n) {
	Zoo_Base_init((Zoo_Base*)self, n);
	Zoo_Cat_finit(self);
}

static Zoo_Cat* Zoo_Cat_new(jstring* n) {
	Zoo_Cat* o = (Zoo_Cat*)jrt_alloc(&Zoo_Cat_class);
	Zoo_Cat_init(o, n);
	return o;
}

static jstring* Zoo_Cat_sound(jobject* self_) {
	Zoo_Cat* self = (Zoo_Cat*)self_;
	return jrt_lit(&lit_16, u"meow", 4);
}

static void Zoo_Cat_finit(Zoo_Cat* self) {
}

static void Zoo_Pair_init(Zoo_Pair* self, jstring* a, int32_t b) {
	Zoo_Pair_finit(self);
	self->a = a;
	self->b = b;
}

static Zoo_Pair* Zoo_Pair_new(jstring* a, int32_t b) {
	Zoo_Pair* o = (Zoo_Pair*)jrt_alloc(&Zoo_Pair_class);
	Zoo_Pair_init(o, a, b);
	return o;
}

static jstring* Zoo_Pair_a(jobject* self_) {
	Zoo_Pair* self = (Zoo_Pair*)self_;
	return self->a;
}

static int32_t Zoo_Pair_b(jobject* self_) {
	Zoo_Pair* self = (Zoo_Pair*)self_;
	return self->b;
}

static jstring* Zoo_Pair_toString(jobject* self_) {
	Zoo_Pair* self = (Zoo_Pair*)self_;
	return jrt_concat(6, jrt_lit(&lit_17, u"Pair[", 5), jrt_lit(&lit_18, u"a=", 2), jrt_str_O((jobject*)self->a), jrt_lit(&lit_19, u", b=", 4), jrt_str_I(self->b), jrt_lit(&lit_20, u"]", 1));
}

static int32_t Zoo_Pair_hashCode(jobject* self_) {
	Zoo_Pair* self = (Zoo_Pair*)self_;
	int32_t h = 0;
	h = 31 * h + jrt_Objects_hashCode_O((jobject*)self->a);
	h = 31 * h + jrt_Integer_hashCode_I(self->b);
	return h;
}

static bool Zoo_Pair_equals(jobject* self_, jobject* o) {
	Zoo_Pair* self = (Zoo_Pair*)self_;
	if (!(jrt_instanceof((jobject*)o, &Zoo_Pair_class))) {
		return false;
	}
	Zoo_Pair* that = (Zoo_Pair*)jrt_cast((jobject*)o, &Zoo_Pair_class);
	return jrt_Objects_equals_OO((jobject*)self->a, (jobject*)((Zoo_Pair*)jrt_nn(that))->a) && self->b == ((Zoo_Pair*)jrt_nn(that))->b;
}

static void Zoo_Pair_finit(Zoo_Pair* self) {
}

static void Zoo_Empty_init(Zoo_Empty* self, jstring* m) {
	jrt_Throwable_init((jobject*)self, m);
	Zoo_Empty_finit(self);
}

static Zoo_Empty* Zoo_Empty_new(jstring* m) {
	Zoo_Empty* o = (Zoo_Empty*)jrt_alloc(&Zoo_Empty_class);
	Zoo_Empty_init(o, m);
	return o;
}

static void Zoo_Empty_finit(Zoo_Empty* self) {
}

static const struct Zoo_vt Zoo_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Zoo_class = {"Zoo", &jrt_Object_class, sizeof(Zoo), &Zoo_vt_, 0, 0, 0};

const jclass Zoo_Animal_class = {"Zoo.Animal", 0, 0, 0, 0, 0, 1};

static const struct Zoo_Base_vt Zoo_Base_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))Zoo_Base_name, (void (*)(jobject*))Zoo_Base_describe};
static const struct Zoo_Animal_vt Zoo_Base_as_Animal = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))0, (jstring* (*)(jobject*))Zoo_Animal_greet, (jstring* (*)(jobject*))Zoo_Base_name};
static const jitab Zoo_Base_itabs[] = {{&Zoo_Animal_class, &Zoo_Base_as_Animal}};
const jclass Zoo_Base_class = {"Zoo.Base", &jrt_Object_class, sizeof(Zoo_Base), &Zoo_Base_vt_, JRT_ITABS(Zoo_Base_itabs), 0};

static const struct Zoo_Dog_vt Zoo_Dog_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))Zoo_Base_name, (void (*)(jobject*))Zoo_Dog_describe, (jstring* (*)(jobject*))Zoo_Dog_sound};
static const struct Zoo_Animal_vt Zoo_Dog_as_Animal = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))Zoo_Dog_sound, (jstring* (*)(jobject*))Zoo_Animal_greet, (jstring* (*)(jobject*))Zoo_Base_name};
static const jitab Zoo_Dog_itabs[] = {{&Zoo_Animal_class, &Zoo_Dog_as_Animal}};
const jclass Zoo_Dog_class = {"Zoo.Dog", &Zoo_Base_class, sizeof(Zoo_Dog), &Zoo_Dog_vt_, JRT_ITABS(Zoo_Dog_itabs), 0};

static const struct Zoo_Cat_vt Zoo_Cat_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))Zoo_Base_name, (void (*)(jobject*))Zoo_Base_describe, (jstring* (*)(jobject*))Zoo_Cat_sound};
static const struct Zoo_Animal_vt Zoo_Cat_as_Animal = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))Zoo_Cat_sound, (jstring* (*)(jobject*))Zoo_Animal_greet, (jstring* (*)(jobject*))Zoo_Base_name};
static const jitab Zoo_Cat_itabs[] = {{&Zoo_Animal_class, &Zoo_Cat_as_Animal}};
const jclass Zoo_Cat_class = {"Zoo.Cat", &Zoo_Base_class, sizeof(Zoo_Cat), &Zoo_Cat_vt_, JRT_ITABS(Zoo_Cat_itabs), 0};

static const struct Zoo_Pair_vt Zoo_Pair_vt_ = {{Zoo_Pair_toString, Zoo_Pair_equals, Zoo_Pair_hashCode}, (jstring* (*)(jobject*))Zoo_Pair_a, (int32_t (*)(jobject*))Zoo_Pair_b};
const jclass Zoo_Pair_class = {"Zoo.Pair", &jrt_Object_class, sizeof(Zoo_Pair), &Zoo_Pair_vt_, 0, 0, 0};

static const struct Zoo_Empty_vt Zoo_Empty_vt_ = {{jrt_Throwable_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))jrt_Throwable_getMessage};
const jclass Zoo_Empty_class = {"Zoo.Empty", &jrt_RuntimeException_class, sizeof(Zoo_Empty), &Zoo_Empty_vt_, 0, 0, 0};


int main(int argc, char** argv) {
	const char* which = getenv("JRT_MAIN");
	if (which && strcmp(which, "Zoo") == 0) return jrt_run_main(Zoo_main, argc, argv);
	return jrt_run_main(Zoo_main, argc, argv);
}
