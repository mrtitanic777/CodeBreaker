// ==== jrt_program.hpp ====
// written by CodeBreaker's C++17 back end from the Java program's L2; it runs on jrt.hpp, the Java library in C++17.
// Every file of the program starts from this one: the runtime, and a declaration of each class.
#pragma once
#include "jrt.hpp"

using jrt::ref;
using jrt::Array;
using jrt::IntUnaryOperator;
using jrt::Integer;
using jrt::Object;
using jrt::PrintStream;
using jrt::String;
using jrt::Supplier;

struct Names;
// ==== Names.hpp ====
#pragma once
#include "jrt_program.hpp"

struct Names : virtual Object {
	const char16_t* jclass_() const override { return u"Names"; }
	Names();
	static int32_t sizeof_(int32_t auto_);
	static void main(ref<Array<ref<String>>> args);
};

// ==== Names.cpp ====
#include "Names.hpp"

Names::Names() {
}

int32_t Names::sizeof_(int32_t auto_) {
	return auto_ * 2;
}

void Names::main(ref<Array<ref<String>>> args) {
	int32_t register_ = 7;
	int32_t self = 3;
	int32_t a_b = 5;
	ref<Supplier> sr = jrt::fn<Supplier>([=]() -> ref<Object> { return jrt::box(register_ + self + a_b); });
	ref<IntUnaryOperator> twice = jrt::fn<IntUnaryOperator>([=](int32_t x) -> int32_t { return x + register_; });
	ref<String> s = jrt::lit(u"\x00e9\xd83d\xde00x\t1");
	ref<Integer> j2xo1{};
	int32_t j2xo2{};
	int32_t j2xo3{};
	int32_t j2xo4{};
	int32_t j2xo5{};
	jrt::nn(jrt::System::out_())->println((j2xo1 = jrt::cast<Integer>(jrt::nn(sr)->get()), j2xo2 = jrt::nn(twice)->applyAsInt(1), j2xo3 = Names::sizeof_(4), j2xo4 = jrt::nn(s)->length(), j2xo5 = (int32_t)(jrt::nn(s)->charAt(1)), jrt::concat(j2xo1, jrt::lit(u" "), j2xo2, jrt::lit(u" "), j2xo3, jrt::lit(u" "), j2xo4, jrt::lit(u" "), j2xo5)));
}

// ==== main.cpp ====
// the program's entry: the class named by JRT_MAIN, else the first with a main
#include "jrt_program.hpp"
#include "Names.hpp"

int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "Names") == 0) return jrt::run_main(&Names::main, argc, argv);
	return jrt::run_main(&Names::main, argc, argv);
}
