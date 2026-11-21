// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the
// Java program's L2; it runs on jrt.hpp, the Java library in C++17.
#include "jrt.hpp"

using jrt::ref;
using jrt::Array;
using jrt::ArrayList;
using jrt::Collection;
using jrt::Comparator;
using jrt::Integer;
using jrt::LinkedHashMap;
using jrt::List;
using jrt::Map;
using jrt::Object;
using jrt::PrintStream;
using jrt::RuntimeException;
using jrt::String;

struct Zoo;
struct Zoo_Animal;
struct Zoo_Base;
struct Zoo_Dog;
struct Zoo_Cat;
struct Zoo_Pair;
struct Zoo_Empty;

struct Zoo : virtual Object {
	const char16_t* jclass_() const override { return u"Zoo"; }
	Zoo();
	static int32_t pick(int32_t k);
	static int32_t sumTo(int32_t n);
	static void main(ref<Array<ref<String>>> args);
};

struct Zoo_Animal : virtual Object {
	const char16_t* jclass_() const override { return u"Zoo.Animal"; }
	virtual ref<String> sound() = 0;
	virtual ref<String> greet();
	virtual ref<String> name() = 0;
};

struct Zoo_Base : virtual Object, virtual Zoo_Animal {
	const char16_t* jclass_() const override { return u"Zoo.Base"; }
	ref<String> name_{};
	static inline int32_t made{};
	void ctor_(ref<String> n);
	virtual ref<String> name();
	virtual void describe();
	void finit_();
};

struct Zoo_Dog : Zoo_Base {
	const char16_t* jclass_() const override { return u"Zoo.Dog"; }
	int32_t barks{};
	void ctor_(ref<String> n);
	virtual ref<String> sound();
	virtual void describe();
	void finit_();
	using Zoo_Base::describe;
};

struct Zoo_Cat : Zoo_Base {
	const char16_t* jclass_() const override { return u"Zoo.Cat"; }
	void ctor_(ref<String> n);
	virtual ref<String> sound();
	void finit_();
};

struct Zoo_Pair : virtual Object {
	const char16_t* jclass_() const override { return u"Zoo.Pair"; }
	ref<String> a_{};
	int32_t b_{};
	Zoo_Pair(ref<String> a, int32_t b);
	virtual ref<String> a();
	virtual int32_t b();
	virtual ref<String> toString();
	virtual int32_t hashCode();
	virtual bool equals(ref<Object> o);
	Zoo_Pair() {}
};

struct Zoo_Empty : RuntimeException {
	const char16_t* jclass_() const override { return u"Zoo.Empty"; }
	void ctor_(ref<String> m);
	void finit_();
};

Zoo::Zoo() {
}

int32_t Zoo::pick(int32_t k) {
	if (k < 0) {
		jrt::raise(jrt::make<Zoo_Empty>(jrt::concat(jrt::lit(u"negative "), k)));
	}
	return k * 3;
}

int32_t Zoo::sumTo(int32_t n) {
	int32_t s = 0;
	for (int32_t i : jrt::in(jrt::array_of<int32_t>({1, 2, 3, n}))) {
		s += i;
	}
	return s;
}

void Zoo::main(ref<Array<ref<String>>> args) {
	ref<List> zoo = jrt::make<ArrayList>();
	jrt::nn(zoo)->add(jrt::make<Zoo_Dog>(jrt::lit(u"rex")));
	jrt::nn(zoo)->add(jrt::make<Zoo_Cat>(jrt::lit(u"tom")));
	for (ref<Zoo_Animal> a : jrt::each<Zoo_Animal>(zoo)) {
		jrt::nn(jrt::System::out_())->println(jrt::nn(a)->greet());
	}
	int32_t j2xo1{};
	int32_t j2xo2{};
	ref<Zoo_Pair> j2xo3{};
	int32_t j2xo4{};
	jrt::nn(jrt::System::out_())->println((j2xo1 = Zoo_Base::made, j2xo2 = jrt::nn((jrt::cast<Zoo_Dog>(jrt::cast<Zoo_Animal>(jrt::nn(zoo)->get(0)))))->barks, j2xo3 = std::make_shared<Zoo_Pair>(jrt::lit(u"x"), 2), j2xo4 = Zoo::sumTo(4), jrt::concat(j2xo1, jrt::lit(u" "), j2xo2, jrt::lit(u" "), j2xo3, jrt::lit(u" "), j2xo4)));
	try {
		try {
			Zoo::pick(-2);
		} catch (jrt::JavaThrow& j2x_t5) {
			if (auto e = jrt::as<Zoo_Empty>(j2x_t5.t)) {
				jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"caught "), jrt::nn(e)->getMessage()));
			} else {
				throw;
			}
		}
	} catch (...) {
		jrt::nn(jrt::System::out_())->println(jrt::lit(u"finally"));
		throw;
	}
	{
		jrt::nn(jrt::System::out_())->println(jrt::lit(u"finally"));
	}
	ref<Map> m = jrt::make<LinkedHashMap>();
	jrt::cast<Integer>(jrt::nn(m)->put(jrt::lit(u"one"), jrt::box(1)));
	jrt::cast<Integer>(jrt::nn(m)->put(jrt::lit(u"two"), jrt::box(2)));
	jrt::nn(jrt::System::out_())->println(jrt::concat(m, jrt::lit(u" "), Zoo::pick(5), jrt::lit(u" "), jrt::String::format(jrt::lit(u"%5.2f|%d"), jrt::box(3.14159), jrt::box(42))));
	ref<Comparator> byLen = jrt::fn<Comparator>([=](ref<Object> x_in, ref<Object> y_in) -> int32_t {
	ref<String> x = jrt::cast<String>(x_in);
	ref<String> y = jrt::cast<String>(y_in);
	return jrt::nn(x)->length() - jrt::nn(y)->length();
});
	ref<List> ws = jrt::make<ArrayList>(jrt::List::of(jrt::lit(u"ccc"), jrt::lit(u"a"), jrt::lit(u"bb")));
	jrt::nn(ws)->sort(byLen);
	jrt::nn(jrt::System::out_())->println(ws);
}

ref<String> Zoo_Animal::greet() {
	ref<String> j2xo1{};
	ref<String> j2xo2{};
	return (j2xo1 = this->name(), j2xo2 = this->sound(), jrt::concat(j2xo1, jrt::lit(u" says "), j2xo2));
}

void Zoo_Base::ctor_(ref<String> n) {
	finit_();
	this->name_ = n;
	Zoo_Base::made++;
	this->describe();
}

ref<String> Zoo_Base::name() {
	return this->name_;
}

void Zoo_Base::describe() {
}

void Zoo_Base::finit_() {
}

void Zoo_Dog::ctor_(ref<String> n) {
	Zoo_Base::ctor_(n);
	finit_();
}

ref<String> Zoo_Dog::sound() {
	return jrt::lit(u"woof");
}

void Zoo_Dog::describe() {
	this->barks = 1;
}

void Zoo_Dog::finit_() {
}

void Zoo_Cat::ctor_(ref<String> n) {
	Zoo_Base::ctor_(n);
	finit_();
}

ref<String> Zoo_Cat::sound() {
	return jrt::lit(u"meow");
}

void Zoo_Cat::finit_() {
}

Zoo_Pair::Zoo_Pair(ref<String> a, int32_t b) {
	this->a_ = a;
	this->b_ = b;
}

ref<String> Zoo_Pair::a() {
	return this->a_;
}

int32_t Zoo_Pair::b() {
	return this->b_;
}

ref<String> Zoo_Pair::toString() {
	return jrt::concat(jrt::lit(u"Pair["), jrt::lit(u"a="), this->a_, jrt::lit(u", b="), this->b_, jrt::lit(u"]"));
}

int32_t Zoo_Pair::hashCode() {
	int32_t h = 0;
	h = 31 * h + jrt::Objects::hashCode(this->a_);
	h = 31 * h + jrt::Integer_::hashCode(this->b_);
	return h;
}

bool Zoo_Pair::equals(ref<Object> o) {
	if (!(jrt::isa<Zoo_Pair>(o))) {
		return false;
	}
	auto that = jrt::cast<Zoo_Pair>(o);
	return jrt::Objects::equals(this->a_, jrt::nn(that)->a_) && this->b_ == jrt::nn(that)->b_;
}

void Zoo_Empty::ctor_(ref<String> m) {
	RuntimeException::ctor_(m);
	finit_();
}

void Zoo_Empty::finit_() {
}


int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "Zoo") == 0) return jrt::run_main(&Zoo::main, argc, argv);
	return jrt::run_main(&Zoo::main, argc, argv);
}
