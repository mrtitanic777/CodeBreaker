// ==== jrt_program.hpp ====
// written by CodeBreaker's C++17 back end from the Java program's L2; it runs on jrt.hpp, the Java library in C++17.
// Every file of the program starts from this one: the runtime, and a declaration of each class.
#pragma once
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
// ==== sample/AxisAlignedBB.hpp ====
#pragma once
#include "jrt_program.hpp"

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

// ==== sample/AxisAlignedBB.cpp ====
#include "sample/AxisAlignedBB.hpp"

AxisAlignedBB::AxisAlignedBB(double var1, double var3, double var5, double var7, double var9, double var11) {
	this->minX = var1;
	this->minY = var3;
	this->minZ = var5;
	this->maxX = var7;
	this->maxY = var9;
	this->maxZ = var11;
}

bool AxisAlignedBB::intersectsWith(ref<AxisAlignedBB> var1) {
	if (jrt::nn(var1)->maxX <= this->minX || jrt::nn(var1)->minX >= this->maxX) {
		return false;
	}
	return jrt::nn(var1)->maxY > this->minY && jrt::nn(var1)->minY < this->maxY;
}

// ==== sample/MathHelper.hpp ====
#pragma once
#include "jrt_program.hpp"

struct MathHelper : virtual Object {
	const char16_t* jclass_() const override { return u"sample.MathHelper"; }
	static inline ref<Array<float>> SIN_TABLE{};
	static inline bool inited_ = false;
	static void clinit_();
	MathHelper();
	static float sqrt_float(float var0);
	static int32_t floor_double(double var0);
};

// ==== sample/MathHelper.cpp ====
#include "sample/MathHelper.hpp"

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

// ==== sample/FixtureMain.hpp ====
#pragma once
#include "jrt_program.hpp"

struct FixtureMain : virtual Object {
	const char16_t* jclass_() const override { return u"sample.FixtureMain"; }
	FixtureMain();
	static void main(ref<Array<ref<String>>> args);
};

// ==== sample/FixtureMain.cpp ====
#include "sample/FixtureMain.hpp"
#include "sample/AxisAlignedBB.hpp"
#include "sample/MathHelper.hpp"
#include "sample/Vec3D.hpp"

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
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"subtract "), jrt::nn(d)->xCoord, jrt::lit(u" "), jrt::nn(d)->yCoord, jrt::lit(u" "), jrt::nn(d)->zCoord));
	ref<Vec3D> m = jrt::nn(a)->getIntermediateWithXValue(b, 2.0);
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"intermediate "), ((m == nullptr ? jrt::lit(u"null") : jrt::concat(jrt::nn(m)->xCoord, jrt::lit(u","), jrt::nn(m)->yCoord, jrt::lit(u","), jrt::nn(m)->zCoord)))));
	ref<Vec3D> n = jrt::nn(a)->getIntermediateWithXValue(std::make_shared<Vec3D>(1.0000001, 0.0, 0.0), 2.0);
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"near "), ((n == nullptr ? jrt::lit(u"null") : jrt::lit(u"made")))));
	auto box = std::make_shared<AxisAlignedBB>((double)(0), (double)(0), (double)(0), (double)(1), (double)(1), (double)(1));
	ref<Array<ref<AxisAlignedBB>>> others = jrt::array_of<ref<AxisAlignedBB>>({std::make_shared<AxisAlignedBB>(0.5, 0.5, 0.5, (double)(2), (double)(2), (double)(2)), std::make_shared<AxisAlignedBB>((double)(1), (double)(0), (double)(0), (double)(2), (double)(1), (double)(1)), std::make_shared<AxisAlignedBB>((double)(-1), (double)(-1), (double)(-1), 0.01, 0.01, 0.01), std::make_shared<AxisAlignedBB>(0.2, (double)(3), 0.2, 0.8, (double)(4), 0.8)});
	for (ref<AxisAlignedBB> o : jrt::in(others)) {
		jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"intersects "), jrt::nn(box)->intersectsWith(o)));
	}
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"pool "), (Vec3D::clinit_(), Vec3D::nextVector), jrt::lit(u" "), jrt::nn((Vec3D::clinit_(), Vec3D::vectorList))->length));
}

// ==== sample/Vec3D.hpp ====
#pragma once
#include "jrt_program.hpp"

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

// ==== sample/Vec3D.cpp ====
#include "sample/Vec3D.hpp"

Vec3D::Vec3D(double var1, double var3, double var5) {
	clinit_();
	this->xCoord = var1;
	this->yCoord = var3;
	this->zCoord = var5;
}

ref<Vec3D> Vec3D::subtract(ref<Vec3D> var1) {
	return std::make_shared<Vec3D>(jrt::nn(var1)->xCoord - this->xCoord, jrt::nn(var1)->yCoord - this->yCoord, jrt::nn(var1)->zCoord - this->zCoord);
}

ref<Vec3D> Vec3D::getIntermediateWithXValue(ref<Vec3D> var1, double var2) {
	double var4 = jrt::nn(var1)->xCoord - this->xCoord;
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

// ==== main.cpp ====
// the program's entry: the class named by JRT_MAIN, else the first with a main
#include "jrt_program.hpp"
#include "sample/FixtureMain.hpp"

int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "FixtureMain") == 0) return jrt::run_main(&FixtureMain::main, argc, argv);
	return jrt::run_main(&FixtureMain::main, argc, argv);
}
