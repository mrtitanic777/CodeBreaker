// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the
// Java program's L2; it runs on jrt.hpp, the Java library in C++17.
#include "jrt.hpp"

using jrt::ref;
using jrt::Array;
using jrt::Object;
using jrt::PrintStream;
using jrt::String;

struct AxisAlignedBB;
struct MathHelper;
struct FixtureMain;
struct Vec3D;

struct AxisAlignedBB : virtual Object {
	const char16_t* jclass_() const override { return u"sample.AxisAlignedBB"; }
	double minX{};
	double minY{};
	double minZ{};
	double maxX{};
	double maxY{};
	double maxZ{};
	AxisAlignedBB(double var1, double var3, double var5, double var7, double var9, double var11);
	virtual bool intersectsWith(ref<AxisAlignedBB> var1);
	AxisAlignedBB() {}
};

struct MathHelper : virtual Object {
	const char16_t* jclass_() const override { return u"sample.MathHelper"; }
	static inline ref<Array<float>> SIN_TABLE{};
	static inline bool inited_ = false;
	static void clinit_();
	MathHelper();
	static float sqrt_float(float var0);
	static int32_t floor_double(double var0);
};

struct FixtureMain : virtual Object {
	const char16_t* jclass_() const override { return u"sample.FixtureMain"; }
	FixtureMain();
	static void main(ref<Array<ref<String>>> args);
};

struct Vec3D : virtual Object {
	const char16_t* jclass_() const override { return u"sample.Vec3D"; }
	static inline int32_t nextVector = 0;
	static inline ref<Array<ref<Vec3D>>> vectorList{};
	double xCoord{};
	double yCoord{};
	double zCoord{};
	static inline bool inited_ = false;
	static void clinit_();
	Vec3D(double var1, double var3, double var5);
	virtual ref<Vec3D> subtract(ref<Vec3D> var1);
	virtual ref<Vec3D> getIntermediateWithXValue(ref<Vec3D> var1, double var2);
	Vec3D() {}
};

AxisAlignedBB::AxisAlignedBB(double var1, double var3, double var5, double var7, double var9, double var11) {
	this->minX = var1;
	this->minY = var3;
	this->minZ = var5;
	this->maxX = var7;
	this->maxY = var9;
	this->maxZ = var11;
}

bool AxisAlignedBB::intersectsWith(ref<AxisAlignedBB> var1) {
	if (var1->maxX <= this->minX || var1->minX >= this->maxX) {
		return false;
	}
	return var1->maxY > this->minY && var1->minY < this->maxY;
}

MathHelper::MathHelper() {
	clinit_();
}

float MathHelper::sqrt_float(float var0) {
	clinit_();
	return (float)(jrt::Math::sqrt((double)var0));
}

int32_t MathHelper::floor_double(double var0) {
	clinit_();
	int32_t var2 = jrt::d2i(var0);
	return (var0 < (double)var2 ? var2 - 1 : var2);
}

void MathHelper::clinit_() {
	if (inited_) return;
	inited_ = true;
	MathHelper::SIN_TABLE = jrt::new_array<float>(65536);
	for (int32_t var0 = 0; var0 < 65536; ++var0) {
		(*jrt::nn(MathHelper::SIN_TABLE))[var0] = (float)(jrt::Math::sin((double)var0 * jrt::Math::PI * 2.0 / 65536.0));
	}
}

FixtureMain::FixtureMain() {
}

void FixtureMain::main(ref<Array<ref<String>>> args) {
	ref<Array<double>> ds = jrt::array_of<double>({0.0, 0.5, -0.5, 1.999, -1.001, 12345.678, -98765.4321});
	for (double d : jrt::in(ds)) {
		double j2xo1{};
		int32_t j2xo2{};
		float j2xo3{};
		jrt::nn(jrt::System::out_())->println((j2xo1 = d, j2xo2 = MathHelper::floor_double(d), j2xo3 = MathHelper::sqrt_float((float)(jrt::Math::abs(d))), jrt::concat(jrt::lit(u"floor("), j2xo1, jrt::lit(u")="), j2xo2, jrt::lit(u" sqrt="), j2xo3)));
	}
	auto a = std::make_shared<Vec3D>(1.0, 2.0, 3.0);
	auto b = std::make_shared<Vec3D>(4.5, -2.0, 0.25);
	ref<Vec3D> d = jrt::nn(a)->subtract(b);
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"subtract "), d->xCoord, jrt::lit(u" "), d->yCoord, jrt::lit(u" "), d->zCoord));
	ref<Vec3D> m = jrt::nn(a)->getIntermediateWithXValue(b, 2.0);
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"intermediate "), ((m == nullptr ? jrt::lit(u"null") : jrt::concat(m->xCoord, jrt::lit(u","), m->yCoord, jrt::lit(u","), m->zCoord)))));
	ref<Vec3D> n = jrt::nn(a)->getIntermediateWithXValue(std::make_shared<Vec3D>(1.0000001, 0.0, 0.0), 2.0);
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"near "), ((n == nullptr ? jrt::lit(u"null") : jrt::lit(u"made")))));
	auto box = std::make_shared<AxisAlignedBB>((double)(0), (double)(0), (double)(0), (double)(1), (double)(1), (double)(1));
	ref<Array<ref<AxisAlignedBB>>> others = jrt::array_of<ref<AxisAlignedBB>>({std::make_shared<AxisAlignedBB>(0.5, 0.5, 0.5, (double)(2), (double)(2), (double)(2)), std::make_shared<AxisAlignedBB>((double)(1), (double)(0), (double)(0), (double)(2), (double)(1), (double)(1)), std::make_shared<AxisAlignedBB>((double)(-1), (double)(-1), (double)(-1), 0.01, 0.01, 0.01), std::make_shared<AxisAlignedBB>(0.2, (double)(3), 0.2, 0.8, (double)(4), 0.8)});
	for (ref<AxisAlignedBB> o : jrt::in(others)) {
		jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"intersects "), jrt::nn(box)->intersectsWith(o)));
	}
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"pool "), (Vec3D::clinit_(), Vec3D::nextVector), jrt::lit(u" "), jrt::nn((Vec3D::clinit_(), Vec3D::vectorList))->length));
}

Vec3D::Vec3D(double var1, double var3, double var5) {
	clinit_();
	this->xCoord = var1;
	this->yCoord = var3;
	this->zCoord = var5;
}

ref<Vec3D> Vec3D::subtract(ref<Vec3D> var1) {
	return std::make_shared<Vec3D>(var1->xCoord - this->xCoord, var1->yCoord - this->yCoord, var1->zCoord - this->zCoord);
}

ref<Vec3D> Vec3D::getIntermediateWithXValue(ref<Vec3D> var1, double var2) {
	double var4 = var1->xCoord - this->xCoord;
	if (var4 * var4 < 1.0e-07) {
		return nullptr;
	}
	return std::make_shared<Vec3D>(var2, this->yCoord, this->zCoord);
}

void Vec3D::clinit_() {
	if (inited_) return;
	inited_ = true;
	Vec3D::vectorList = jrt::new_array<ref<Vec3D>>(256);
}


int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "FixtureMain") == 0) return jrt::run_main(&FixtureMain::main, argc, argv);
	return jrt::run_main(&FixtureMain::main, argc, argv);
}
