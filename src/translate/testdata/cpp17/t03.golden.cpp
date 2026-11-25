// ==== jrt_program.hpp ====
// written by CodeBreaker's C++17 back end from the Java program's L2; it runs on jrt.hpp, the Java library in C++17.
// Every file of the program starts from this one: the runtime, and a declaration of each class.
// these classes can form a reference cycle, which the runtime's cycle collector frees: Calc
#define JRT_CYCLES
#pragma once
#include "jrt.hpp"

using jrt::ref;
using jrt::ArithmeticException;
using jrt::Array;
using jrt::ArrayList;
using jrt::Exception;
using jrt::List;
using jrt::Object;
using jrt::String;

struct Base;
struct Sub;
struct Calc;
// ==== Base.hpp ====
#pragma once
#include "jrt_program.hpp"

struct Base : virtual Object {
	const char16_t* jclass_() const override { return u"Base"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	int32_t size{};
	int32_t scale{};
	void ctor_(int32_t var1);
	void ctor_(int32_t var1, int32_t var2);
	void finit_();
};

// ==== Base.cpp ====
#include "Base.hpp"

void Base::trace_(jrt::gc::Tracer& t) {
	(void)t;
}
void Base::clear_() {
}

void Base::ctor_(int32_t var1) {
	finit_();
	this->size = var1 * this->scale;
}

void Base::ctor_(int32_t var1, int32_t var2) {
	this->ctor_(var1);
	this->scale += var2;
}

void Base::finit_() {
	this->scale = 2;
}

// ==== Sub.hpp ====
#pragma once
#include "jrt_program.hpp"
#include "Base.hpp"

struct Sub : Base {
	const char16_t* jclass_() const override { return u"Sub"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	int32_t extra{};
	void ctor_(int32_t var1);
	static int32_t build(int32_t var0);
	void finit_();
};

// ==== Sub.cpp ====
#include "Sub.hpp"
#include "Base.hpp"

void Sub::trace_(jrt::gc::Tracer& t) {
	Base::trace_(t);
}
void Sub::clear_() {
	Base::clear_();
}

void Sub::ctor_(int32_t var1) {
	Base::ctor_(var1 + 1);
	finit_();
	this->extra += this->size;
}

int32_t Sub::build(int32_t var0) {
	auto var1 = jrt::make<Sub>(var0);
	return jrt::nn(var1)->extra * 3 + jrt::nn(var1)->size;
}

void Sub::finit_() {
	this->extra = 7;
}

// ==== Calc.hpp ====
#pragma once
#include "jrt_program.hpp"

struct Calc : virtual Object {
	const char16_t* jclass_() const override { return u"Calc"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	static inline int32_t total{};
	static inline ref<Array<int32_t>> table{};
	ref<Array<int32_t>> cells{};
	ref<List> items{};
	ref<String> name{};
	static inline bool inited_ = false;
	static void clinit_();
	void ctor_();
	virtual int32_t fill(int32_t var1);
	static int64_t mix(int64_t var0, int32_t var2);
	static float wrap(float var0);
	virtual ref<String> label(int32_t var1);
	virtual int32_t guarded(int32_t var1);
	virtual int32_t cleanup(int32_t var1);
	virtual int32_t search(int32_t var1);
	static int32_t sum(ref<Array<int32_t>> var0);
	static int32_t sums(int32_t var0);
	static int32_t tabled(int32_t var0);
	static int32_t made(int32_t var0);
	virtual int32_t count(int32_t var1);
	void finit_();
};

// ==== Calc.cpp ====
#include "Calc.hpp"
#include "Base.hpp"

void Calc::trace_(jrt::gc::Tracer& t) {
	t(cells);
	t(items);
	t(name);
}
void Calc::clear_() {
	cells = nullptr;
	items = nullptr;
	name = nullptr;
}

void Calc::ctor_() {
	clinit_();
	finit_();
}

int32_t Calc::fill(int32_t var1) {
	int32_t var2 = 0;
	for (int32_t var3 = 0; var3 < jrt::nn(this->cells)->length; ++var3) {
		(*jrt::nn(this->cells))[var3] = var1 * var3 - (jrt::ushr(var3, 1));
		var2 += (*jrt::nn(this->cells))[var3];
	}
	Calc::total += var2;
	return (var2 > 100 ? var2 % 7 : -var2);
}

int64_t Calc::mix(int64_t var0, int32_t var2) {
	clinit_();
	int64_t var3 = var0 ^ ((int64_t)var2 << 3);
	while (var3 > 1000LL) {
		var3 = (int64_t)jrt::ushr(var3, (2 & 63));
	}
	return jrt::ushr(var3, 1);
}

float Calc::wrap(float var0) {
	clinit_();
	return (float)std::fmod(var0, 3.5f) + (float)(jrt::Math::floor((double)var0));
}

ref<String> Calc::label(int32_t var1) {
	ref<String> var2 = (var1 < 0 ? nullptr : jrt::concat(this->name, jrt::lit(u":"), var1));
	return (var2 == nullptr ? jrt::lit(u"none") : jrt::concat(var2, (char16_t)((65 + (var1 & 7)))));
}

int32_t Calc::guarded(int32_t var1) {
	ref<Calc> var2 = (var1 > 0 ? nullptr : jrt::self(this));
	try {
		return jrt::nn(jrt::nn(var2)->cells)->length + var1;
	} catch (jrt::JavaThrow& j2x_t1) {
		if (auto var3 = jrt::as<Exception>(j2x_t1.t)) {
			return -7;
		} else {
			throw;
		}
	}
}

int32_t Calc::cleanup(int32_t var1) {
	int32_t var2 = 0;
	try {
		try {
			var2 = jrt::div<int32_t>(100, (var1 & 3));
		} catch (jrt::JavaThrow& j2x_t1) {
			if (auto var3 = jrt::as<ArithmeticException>(j2x_t1.t)) {
				var2 = -1;
			} else {
				throw;
			}
		}
	} catch (...) {
		Calc::total += 1;
		throw;
	}
	{
		Calc::total += 1;
	}
	return var2 + Calc::total;
}

int32_t Calc::search(int32_t var1) {
	int32_t var2 = 0;
	for (int32_t var3 = 0; var3 < 5; ++var3) {
		{
			for (int32_t var4 = 0; var4 < 5; ++var4) {
				if (var3 * var4 == (var1 & 15)) {
					var2 = var3 * 10 + var4;
					goto label9_brk;
				}
				if (var4 > var3) {
					goto label9_cont;
				}
			}
		}
		label9_cont:;
	}
	label9_brk:;
	return var2;
}

int32_t Calc::sum(ref<Array<int32_t>> var0) {
	clinit_();
	int32_t var1 = 0;
	for (int32_t var2 = 0; var2 < jrt::nn(var0)->length; ++var2) {
		var1 += (*jrt::nn(var0))[var2];
	}
	return var1;
}

int32_t Calc::sums(int32_t var0) {
	clinit_();
	int32_t j2xo1{};
	int32_t j2xo2{};
	int32_t j2xo3{};
	int32_t j2xo4{};
	int32_t j2xo5{};
	int32_t j2xo6{};
	return (j2xo5 = (j2xo3 = (j2xo1 = Calc::sum(jrt::array_of<int32_t>({})), j2xo2 = Calc::sum(jrt::array_of<int32_t>({var0})), j2xo1 + j2xo2), j2xo4 = Calc::sum(jrt::array_of<int32_t>({var0, 2, 3})), j2xo3 + j2xo4), j2xo6 = Calc::sum(jrt::array_of<int32_t>({var0, var0})), j2xo5 + j2xo6);
}

int32_t Calc::tabled(int32_t var0) {
	clinit_();
	return (*jrt::nn(Calc::table))[var0 & 3] + Calc::total;
}

int32_t Calc::made(int32_t var0) {
	clinit_();
	auto var1 = jrt::make<Base>(var0, 3);
	return jrt::nn(var1)->size + jrt::nn(var1)->scale;
}

int32_t Calc::count(int32_t var1) {
	jrt::nn(this->items)->add(this->name);
	switch (var1 & 3) {
	case 0:
	{
		return jrt::nn(this->items)->size();
	}
	case 1:
	{
		jrt::nn(this->items)->clear();
	}
	default:
	{
		return -1;
	}
	}
}

void Calc::finit_() {
	this->cells = jrt::new_array<int32_t>(8);
	this->items = jrt::make<ArrayList>();
	this->name = jrt::lit(u"calc");
}

void Calc::clinit_() {
	if (inited_) return;
	inited_ = true;
	Calc::table = jrt::new_array<int32_t>(4);
	(*jrt::nn(Calc::table))[1] = 5;
	Calc::total = (*jrt::nn(Calc::table))[1] * 2;
}

// ==== main.cpp ====
// the program's entry: the class named by JRT_MAIN, else the first with a main
#include "jrt_program.hpp"

int main(int argc, char** argv) {
	return 0;
}
