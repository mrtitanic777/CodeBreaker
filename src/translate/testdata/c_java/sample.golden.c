/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from
   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */
#include "jrt.h"

typedef struct AxisAlignedBB AxisAlignedBB;
typedef struct MathHelper MathHelper;
typedef struct FixtureMain FixtureMain;
typedef struct Vec3D Vec3D;
extern const jclass AxisAlignedBB_class;
extern const jclass MathHelper_class;
extern const jclass FixtureMain_class;
extern const jclass Vec3D_class;

struct AxisAlignedBB {
	jobject hdr_;
	double minX;
	double minY;
	double minZ;
	double maxX;
	double maxY;
	double maxZ;
};
struct AxisAlignedBB_vt {
	jobject_vt o_;
	bool (*intersectsWith)(jobject*, AxisAlignedBB*);
};

struct MathHelper {
	jobject hdr_;
};
struct MathHelper_vt {
	jobject_vt o_;
};
static jarray* MathHelper_SIN_TABLE = NULL;
static bool MathHelper_inited_;

struct FixtureMain {
	jobject hdr_;
};
struct FixtureMain_vt {
	jobject_vt o_;
};

struct Vec3D {
	jobject hdr_;
	double xCoord;
	double yCoord;
	double zCoord;
};
struct Vec3D_vt {
	jobject_vt o_;
	Vec3D* (*subtract)(jobject*, Vec3D*);
	Vec3D* (*getIntermediateWithXValue)(jobject*, Vec3D*, double);
};
static int32_t Vec3D_nextVector = 0;
static jarray* Vec3D_vectorList = NULL;
static bool Vec3D_inited_;

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

static void AxisAlignedBB_init(AxisAlignedBB* self, double var1, double var3, double var5, double var7, double var9, double var11);
static AxisAlignedBB* AxisAlignedBB_new(double var1, double var3, double var5, double var7, double var9, double var11);
static bool AxisAlignedBB_intersectsWith(jobject*, AxisAlignedBB*);
static void AxisAlignedBB_finit(AxisAlignedBB* self);
static void MathHelper_init(MathHelper* self);
static MathHelper* MathHelper_new(void);
static float MathHelper_sqrt_float(float);
static int32_t MathHelper_floor_double(double);
static void MathHelper_finit(MathHelper* self);
static void MathHelper_clinit(void);
static void FixtureMain_init(FixtureMain* self);
static FixtureMain* FixtureMain_new(void);
static void FixtureMain_main(jarray*);
static void FixtureMain_finit(FixtureMain* self);
static void Vec3D_init(Vec3D* self, double var1, double var3, double var5);
static Vec3D* Vec3D_new(double var1, double var3, double var5);
static Vec3D* Vec3D_subtract(jobject*, Vec3D*);
static Vec3D* Vec3D_getIntermediateWithXValue(jobject*, Vec3D*, double);
static void Vec3D_finit(Vec3D* self);
static void Vec3D_clinit(void);
static inline bool AxisAlignedBB_intersectsWith_v(jobject* o, AxisAlignedBB* a0) { return ((const struct AxisAlignedBB_vt*)jrt_vt(o))->intersectsWith(o, a0); }
static inline Vec3D* Vec3D_subtract_v(jobject* o, Vec3D* a0) { return ((const struct Vec3D_vt*)jrt_vt(o))->subtract(o, a0); }
static inline Vec3D* Vec3D_getIntermediateWithXValue_v(jobject* o, Vec3D* a0, double a1) { return ((const struct Vec3D_vt*)jrt_vt(o))->getIntermediateWithXValue(o, a0, a1); }
static void AxisAlignedBB_init(AxisAlignedBB* self, double var1, double var3, double var5, double var7, double var9, double var11) {
	AxisAlignedBB_finit(self);
	self->minX = var1;
	self->minY = var3;
	self->minZ = var5;
	self->maxX = var7;
	self->maxY = var9;
	self->maxZ = var11;
}

static AxisAlignedBB* AxisAlignedBB_new(double var1, double var3, double var5, double var7, double var9, double var11) {
	AxisAlignedBB* o = (AxisAlignedBB*)jrt_alloc(&AxisAlignedBB_class);
	AxisAlignedBB_init(o, var1, var3, var5, var7, var9, var11);
	return o;
}

static bool AxisAlignedBB_intersectsWith(jobject* self_, AxisAlignedBB* var1) {
	AxisAlignedBB* self = (AxisAlignedBB*)self_;
	if (((AxisAlignedBB*)jrt_nn(var1))->maxX <= self->minX || ((AxisAlignedBB*)jrt_nn(var1))->minX >= self->maxX) {
		return false;
	}
	return ((AxisAlignedBB*)jrt_nn(var1))->maxY > self->minY && ((AxisAlignedBB*)jrt_nn(var1))->minY < self->maxY;
}

static void AxisAlignedBB_finit(AxisAlignedBB* self) {
}

static void MathHelper_init(MathHelper* self) {
	MathHelper_finit(self);
}

static MathHelper* MathHelper_new(void) {
	MathHelper_clinit();
	MathHelper* o = (MathHelper*)jrt_alloc(&MathHelper_class);
	MathHelper_init(o);
	return o;
}

static float MathHelper_sqrt_float(float var0) {
	MathHelper_clinit();
	return (float)(jrt_Math_sqrt_D((double)var0));
}

static int32_t MathHelper_floor_double(double var0) {
	MathHelper_clinit();
	int32_t var2 = jrt_d2i(var0);
	return (var0 < (double)var2 ? var2 - 1 : var2);
}

static void MathHelper_finit(MathHelper* self) {
}

static void MathHelper_clinit(void) {
	if (MathHelper_inited_) return;
	MathHelper_inited_ = true;
	MathHelper_SIN_TABLE = jrt_new_array(sizeof(float), 65536);
	{
		int32_t var0 = 0;
		for (; var0 < 65536; ++var0) {
			JA(float, MathHelper_SIN_TABLE, var0) = (float)(jrt_Math_sin_D((double)var0 * JRT_PI * 2.0 / 65536.0));
		}
	}
}

static void FixtureMain_init(FixtureMain* self) {
	FixtureMain_finit(self);
}

static FixtureMain* FixtureMain_new(void) {
	FixtureMain* o = (FixtureMain*)jrt_alloc(&FixtureMain_class);
	FixtureMain_init(o);
	return o;
}

static void FixtureMain_main(jarray* args) {
	jarray* ds = jrt_array_of(sizeof(double), 7, (double[]){0.0, 0.5, -0.5, 1.999, -1.001, 12345.678, -98765.4321});
	{
		jarray* j2x_a1 = ds;
		{
			int32_t j2x_i2 = 0;
			for (; j2x_i2 < JLEN(j2x_a1); j2x_i2++) {
				double d = JA(double, j2x_a1, j2x_i2);
				double jrt_o1 = 0;
				int32_t jrt_o2 = 0;
				float jrt_o3 = 0;
				jrt_PrintStream_println_Str(jrt_System_out, (jrt_o1 = d, jrt_o2 = MathHelper_floor_double(d), jrt_o3 = MathHelper_sqrt_float((float)(jrt_Math_abs_D(d))), jrt_concat(6, jrt_lit(&lit_1, u"floor(", 6), jrt_str_D(jrt_o1), jrt_lit(&lit_2, u")=", 2), jrt_str_I(jrt_o2), jrt_lit(&lit_3, u" sqrt=", 6), jrt_str_F(jrt_o3))));
			}
		}
	}
	Vec3D* a = Vec3D_new(1.0, 2.0, 3.0);
	Vec3D* b = Vec3D_new(4.5, -2.0, 0.25);
	Vec3D* d = Vec3D_subtract_v((jobject*)a, b);
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(6, jrt_lit(&lit_4, u"subtract ", 9), jrt_str_D(((Vec3D*)jrt_nn(d))->xCoord), jrt_lit(&lit_5, u" ", 1), jrt_str_D(((Vec3D*)jrt_nn(d))->yCoord), jrt_lit(&lit_5, u" ", 1), jrt_str_D(((Vec3D*)jrt_nn(d))->zCoord)));
	Vec3D* m = Vec3D_getIntermediateWithXValue_v((jobject*)a, b, 2.0);
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_6, u"intermediate ", 13), jrt_str_O((jobject*)(((jobject*)m == (jobject*)NULL ? jrt_lit(&lit_7, u"null", 4) : jrt_concat(5, jrt_str_D(((Vec3D*)jrt_nn(m))->xCoord), jrt_lit(&lit_8, u",", 1), jrt_str_D(((Vec3D*)jrt_nn(m))->yCoord), jrt_lit(&lit_8, u",", 1), jrt_str_D(((Vec3D*)jrt_nn(m))->zCoord)))))));
	Vec3D* n = Vec3D_getIntermediateWithXValue_v((jobject*)a, Vec3D_new(1.0000001, 0.0, 0.0), 2.0);
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_9, u"near ", 5), jrt_str_O((jobject*)(((jobject*)n == (jobject*)NULL ? jrt_lit(&lit_7, u"null", 4) : jrt_lit(&lit_10, u"made", 4))))));
	AxisAlignedBB* box = AxisAlignedBB_new((double)(0), (double)(0), (double)(0), (double)(1), (double)(1), (double)(1));
	jarray* others = jrt_array_of(sizeof(jobject*), 4, (jobject*[]){(jobject*)AxisAlignedBB_new(0.5, 0.5, 0.5, (double)(2), (double)(2), (double)(2)), (jobject*)AxisAlignedBB_new((double)(1), (double)(0), (double)(0), (double)(2), (double)(1), (double)(1)), (jobject*)AxisAlignedBB_new((double)(-1), (double)(-1), (double)(-1), 0.01, 0.01, 0.01), (jobject*)AxisAlignedBB_new(0.2, (double)(3), 0.2, 0.8, (double)(4), 0.8)});
	{
		jarray* j2x_a3 = others;
		{
			int32_t j2x_i4 = 0;
			for (; j2x_i4 < JLEN(j2x_a3); j2x_i4++) {
				AxisAlignedBB* o = (AxisAlignedBB*)jrt_cast((jobject*)JA(jobject*, j2x_a3, j2x_i4), &AxisAlignedBB_class);
				jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_11, u"intersects ", 11), jrt_str_Z(AxisAlignedBB_intersectsWith_v((jobject*)box, o))));
			}
		}
	}
	jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(4, jrt_lit(&lit_12, u"pool ", 5), jrt_str_I((Vec3D_clinit(), Vec3D_nextVector)), jrt_lit(&lit_5, u" ", 1), jrt_str_I(JLEN((Vec3D_clinit(), Vec3D_vectorList)))));
}

static void FixtureMain_finit(FixtureMain* self) {
}

static void Vec3D_init(Vec3D* self, double var1, double var3, double var5) {
	Vec3D_finit(self);
	self->xCoord = var1;
	self->yCoord = var3;
	self->zCoord = var5;
}

static Vec3D* Vec3D_new(double var1, double var3, double var5) {
	Vec3D_clinit();
	Vec3D* o = (Vec3D*)jrt_alloc(&Vec3D_class);
	Vec3D_init(o, var1, var3, var5);
	return o;
}

static Vec3D* Vec3D_subtract(jobject* self_, Vec3D* var1) {
	Vec3D* self = (Vec3D*)self_;
	return Vec3D_new(((Vec3D*)jrt_nn(var1))->xCoord - self->xCoord, ((Vec3D*)jrt_nn(var1))->yCoord - self->yCoord, ((Vec3D*)jrt_nn(var1))->zCoord - self->zCoord);
}

static Vec3D* Vec3D_getIntermediateWithXValue(jobject* self_, Vec3D* var1, double var2) {
	Vec3D* self = (Vec3D*)self_;
	double var4 = ((Vec3D*)jrt_nn(var1))->xCoord - self->xCoord;
	if (var4 * var4 < 1.0e-07) {
		return NULL;
	}
	return Vec3D_new(var2, self->yCoord, self->zCoord);
}

static void Vec3D_finit(Vec3D* self) {
}

static void Vec3D_clinit(void) {
	if (Vec3D_inited_) return;
	Vec3D_inited_ = true;
	Vec3D_vectorList = jrt_new_array(sizeof(jobject*), 256);
}

static const struct AxisAlignedBB_vt AxisAlignedBB_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (bool (*)(jobject*, AxisAlignedBB*))AxisAlignedBB_intersectsWith};
const jclass AxisAlignedBB_class = {"sample.AxisAlignedBB", &jrt_Object_class, sizeof(AxisAlignedBB), &AxisAlignedBB_vt_, 0, 0, 0};

static const struct MathHelper_vt MathHelper_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass MathHelper_class = {"sample.MathHelper", &jrt_Object_class, sizeof(MathHelper), &MathHelper_vt_, 0, 0, 0};

static const struct FixtureMain_vt FixtureMain_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass FixtureMain_class = {"sample.FixtureMain", &jrt_Object_class, sizeof(FixtureMain), &FixtureMain_vt_, 0, 0, 0};

static const struct Vec3D_vt Vec3D_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, (Vec3D* (*)(jobject*, Vec3D*))Vec3D_subtract, (Vec3D* (*)(jobject*, Vec3D*, double))Vec3D_getIntermediateWithXValue};
const jclass Vec3D_class = {"sample.Vec3D", &jrt_Object_class, sizeof(Vec3D), &Vec3D_vt_, 0, 0, 0};


int main(int argc, char** argv) {
	const char* which = getenv("JRT_MAIN");
	if (which && strcmp(which, "FixtureMain") == 0) return jrt_run_main(FixtureMain_main, argc, argv);
	return jrt_run_main(FixtureMain_main, argc, argv);
}
