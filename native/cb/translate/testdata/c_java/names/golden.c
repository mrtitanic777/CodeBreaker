/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from
   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */
#include "jrt.h"

typedef struct Names Names;
extern const jclass Names_class;

struct Names {
	jobject hdr_;
};
struct Names_vt {
	jobject_vt o_;
};

static jstring* lit_1;
static jstring* lit_2;

static void Names_init(Names* self);
static Names* Names_new(void);
static int32_t Names_sizeof_(int32_t);
static void Names_main(jarray*);
static void Names_finit(Names* self);
struct Lambda1 {
	jobject hdr_;
	int32_t cap_register_;
	int32_t cap_self_;
	int32_t cap_a_b;
};
static jobject* Lambda1_fn(jobject* self_) {
	struct Lambda1* self = (struct Lambda1*)self_;
	(void)self;
	return jrt_box_I(self->cap_register_ + self->cap_self_ + self->cap_a_b);
}
static const jrt_Supplier_vt Lambda1_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda1_fn};
static const jitab Lambda1_itabs[] = {{&jrt_Supplier_class, &Lambda1_vt}};
static const jclass Lambda1_class = {"Names$$Lambda", &jrt_Object_class, sizeof(struct Lambda1), &jrt_Object_vt, JRT_ITABS(Lambda1_itabs), 0};
static jobject* Lambda1_new(int32_t c_register_, int32_t c_self_, int32_t c_a_b) {
	struct Lambda1* o = (struct Lambda1*)jrt_alloc(&Lambda1_class);
	o->cap_register_ = c_register_;
	o->cap_self_ = c_self_;
	o->cap_a_b = c_a_b;
	return (jobject*)o;
}

struct Lambda2 {
	jobject hdr_;
	int32_t cap_register_;
};
static int32_t Lambda2_fn(jobject* self_, int32_t x) {
	struct Lambda2* self = (struct Lambda2*)self_;
	(void)self;
	return x + self->cap_register_;
}
static const jrt_IntUnaryOperator_vt Lambda2_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda2_fn};
static const jitab Lambda2_itabs[] = {{&jrt_IntUnaryOperator_class, &Lambda2_vt}};
static const jclass Lambda2_class = {"Names$$Lambda", &jrt_Object_class, sizeof(struct Lambda2), &jrt_Object_vt, JRT_ITABS(Lambda2_itabs), 0};
static jobject* Lambda2_new(int32_t c_register_) {
	struct Lambda2* o = (struct Lambda2*)jrt_alloc(&Lambda2_class);
	o->cap_register_ = c_register_;
	return (jobject*)o;
}

static void Names_init(Names* self) {
	Names_finit(self);
}

static Names* Names_new(void) {
	Names* o = (Names*)jrt_alloc(&Names_class);
	Names_init(o);
	return o;
}

static int32_t Names_sizeof_(int32_t auto_) {
	return auto_ * 2;
}

static void Names_main(jarray* args) {
	int32_t register_ = 7;
	int32_t self_ = 3;
	int32_t a_b = 5;
	jobject* sr = Lambda1_new(register_, self_, a_b);
	jobject* twice = Lambda2_new(register_);
	jstring* s = jrt_lit(&lit_1, u"\x00e9\xd83d\xde00x\x0009" u"1", 6);
	jobject* o1 = NULL;
	int32_t o2 = 0;
	int32_t o3 = 0;
	int32_t o4 = 0;
	int32_t o5 = 0;
	jrt_PrintStream_println_Str(jrt_System_out, (o1 = jrt_Supplier_get_(sr), o2 = jrt_IntUnaryOperator_applyAsInt_I(twice, 1), o3 = Names_sizeof_(4), o4 = jrt_String_length_(s), o5 = (int32_t)(jrt_String_charAt_I(s, 1)), jrt_concat(9, jrt_str_O((jobject*)o1), jrt_lit(&lit_2, u" ", 1), jrt_str_I(o2), jrt_lit(&lit_2, u" ", 1), jrt_str_I(o3), jrt_lit(&lit_2, u" ", 1), jrt_str_I(o4), jrt_lit(&lit_2, u" ", 1), jrt_str_I(o5))));
}

static void Names_finit(Names* self) {
}

static const struct Names_vt Names_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Names_class = {"Names", &jrt_Object_class, sizeof(Names), &Names_vt_, 0, 0, 0};


int main(int argc, char** argv) {
	const char* which = getenv("JRT_MAIN");
	if (which && strcmp(which, "Names") == 0) return jrt_run_main(Names_main, argc, argv);
	return jrt_run_main(Names_main, argc, argv);
}
