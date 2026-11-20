// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the
// Java program's L2; it runs on jrt.hpp, the Java library in C++17.
#include "jrt.hpp"

using jrt::ref;
using jrt::Array;
using jrt::ArrayList;
using jrt::BiFunction;
using jrt::Collector;
using jrt::Enum;
using jrt::Function;
using jrt::HashMap;
using jrt::IntStream;
using jrt::IntUnaryOperator;
using jrt::Integer;
using jrt::List;
using jrt::Map;
using jrt::Object;
using jrt::Predicate;
using jrt::PrintStream;
using jrt::Stream;
using jrt::String;

struct Modern;
struct Modern_Point;
struct Modern_Shape;
struct Modern_Sq;
struct Modern_Circ;
struct Modern_Color;

struct Modern : virtual Object {
	const char16_t* jclass_() const override { return u"Modern"; }
	Modern();
	static int32_t area(ref<Modern_Shape> s);
	static ref<String> name(ref<Modern_Color> c);
	static int32_t sum(ref<List> xs);
	static int32_t squares(int32_t n);
	static ref<String> words(ref<String> s);
	static void main(ref<Array<ref<String>>> args);
};

struct Modern_Point : virtual Object {
	const char16_t* jclass_() const override { return u"Modern.Point"; }
	int32_t x_{};
	int32_t y_{};
	Modern_Point(int32_t x, int32_t y);
	virtual int32_t dist2();
	virtual int32_t x();
	virtual int32_t y();
	virtual ref<String> toString();
	virtual int32_t hashCode();
	virtual bool equals(ref<Object> o);
	Modern_Point() {}
};

struct Modern_Shape : virtual Object {
	const char16_t* jclass_() const override { return u"Modern.Shape"; }
};

struct Modern_Sq : virtual Object, virtual Modern_Shape {
	const char16_t* jclass_() const override { return u"Modern.Sq"; }
	int32_t side_{};
	Modern_Sq(int32_t side);
	virtual int32_t side();
	virtual ref<String> toString();
	virtual int32_t hashCode();
	virtual bool equals(ref<Object> o);
	Modern_Sq() {}
};

struct Modern_Circ : virtual Object, virtual Modern_Shape {
	const char16_t* jclass_() const override { return u"Modern.Circ"; }
	int32_t r_{};
	Modern_Circ(int32_t r);
	virtual int32_t r();
	virtual ref<String> toString();
	virtual int32_t hashCode();
	virtual bool equals(ref<Object> o);
	Modern_Circ() {}
};

struct Modern_Color : Enum {
	const char16_t* jclass_() const override { return u"Modern.Color"; }
	static inline ref<Modern_Color> RED;
	static inline ref<Modern_Color> GREEN;
	static inline ref<Modern_Color> BLUE;
	static inline ref<Array<ref<Modern_Color>>> values_;
	static ref<Array<ref<Modern_Color>>> values() { clinit_(); return values_->clone(); }
	static ref<Modern_Color> valueOf(ref<String> n);
	static inline bool inited_ = false;
	static void clinit_();
	void ctor_(ref<String> name_in, int32_t ordinal_in);
	void finit_();
};

Modern::Modern() {
}

int32_t Modern::area(ref<Modern_Shape> s) {
	ref<Modern_Sq> q{};
	if (jrt::isa<Modern_Sq>(s) && (q = jrt::cast<Modern_Sq>(s)) != nullptr) {
		int32_t j2xo1{};
		int32_t j2xo2{};
		return (j2xo1 = jrt::nn(q)->side(), j2xo2 = jrt::nn(q)->side(), j2xo1 * j2xo2);
	}
	ref<Modern_Circ> c{};
	if (jrt::isa<Modern_Circ>(s) && (c = jrt::cast<Modern_Circ>(s)) != nullptr) {
		int32_t j2xo3{};
		int32_t j2xo4{};
		return (j2xo3 = 3 * jrt::nn(c)->r(), j2xo4 = jrt::nn(c)->r(), j2xo3 * j2xo4);
	}
	return 0;
}

ref<String> Modern::name(ref<Modern_Color> c) {
	return [&]() -> ref<String> {
	switch (jrt::nn(c)->ordinal()) {
	case 0 /* RED */:
	{
		return jrt::lit(u"r");
	}
	case 1 /* GREEN */:
	{
		return jrt::lit(u"g");
	}
	default:
	{
		{
			ref<String> b = jrt::lit(u"b");
			return jrt::concat(b, b);
		}
	}
	}
	jrt::raise_new<jrt::IllegalStateException>(u"no switch arm matched");
}();
}

int32_t Modern::sum(ref<List> xs) {
	int32_t s = 0;
	for (int32_t v : jrt::each<int32_t>(xs)) {
		s += v;
	}
	return s;
}

int32_t Modern::squares(int32_t n) {
	return jrt::nn(jrt::nn(jrt::IntStream::rangeClosed(1, jrt::Math::abs(n) % 50))->map(jrt::fn<IntUnaryOperator>([=](int32_t x) -> int32_t { return x * x; })))->sum();
}

ref<String> Modern::words(ref<String> s) {
	return [&]() -> ref<String> {
	auto s_ = jrt::nn(s);
	if (s_->equals(jrt::lit(u"a")) || s_->equals(jrt::lit(u"b"))) {
		return jrt::lit(u"ab");
	}
	{
		return jrt::lit(u"other");
	}
	jrt::raise_new<jrt::IllegalStateException>(u"no switch arm matched");
}();
}

void Modern::main(ref<Array<ref<String>>> args) {
	ref<List> xs = jrt::make<ArrayList>();
	for (int32_t i = 0; i < 6; i++) {
		jrt::nn(xs)->add(jrt::box(i * i));
	}
	ref<Function> twice = jrt::fn<Function>([=](ref<Object> v_in) -> ref<Object> {
	ref<Integer> v = jrt::cast<Integer>(v_in);
	return jrt::box(jrt::unbox<int32_t>(v) * 2);
});
	int32_t j2xo1{};
	ref<Integer> j2xo2{};
	int32_t j2xo3{};
	jrt::nn(jrt::System::out_())->println((j2xo1 = Modern::sum(xs), j2xo2 = jrt::cast<Integer>(jrt::nn(twice)->apply(jrt::box(21))), j2xo3 = Modern::squares(10), jrt::concat(j2xo1, jrt::lit(u" "), j2xo2, jrt::lit(u" "), j2xo3)));
	ref<Modern_Point> j2xo4{};
	int32_t j2xo5{};
	ref<Modern_Point> j2xo6{};
	ref<Modern_Point> j2xo7{};
	bool j2xo8{};
	jrt::nn(jrt::System::out_())->println((j2xo4 = std::make_shared<Modern_Point>(3, 4), j2xo5 = jrt::nn(std::make_shared<Modern_Point>(3, 4))->dist2(), j2xo8 = (j2xo6 = std::make_shared<Modern_Point>(1, 2), j2xo7 = std::make_shared<Modern_Point>(1, 2), jrt::nn(j2xo6)->equals(j2xo7)), jrt::concat(j2xo4, jrt::lit(u" "), j2xo5, jrt::lit(u" "), j2xo8)));
	int32_t j2xo9{};
	int32_t j2xo10{};
	ref<String> j2xo11{};
	ref<String> j2xo12{};
	jrt::nn(jrt::System::out_())->println((j2xo9 = Modern::area(std::make_shared<Modern_Sq>(4)), j2xo10 = Modern::area(std::make_shared<Modern_Circ>(2)), j2xo11 = Modern::name((Modern_Color::clinit_(), Modern_Color::BLUE)), j2xo12 = Modern::words(jrt::lit(u"b")), jrt::concat(j2xo9, jrt::lit(u" "), j2xo10, jrt::lit(u" "), j2xo11, jrt::lit(u" "), j2xo12)));
	ref<Stream> j2xo13{};
	ref<Collector> j2xo14{};
	jrt::nn(jrt::System::out_())->println((j2xo13 = jrt::nn(jrt::nn(jrt::nn(xs)->stream())->filter(jrt::fn<Predicate>([=](ref<Object> v_in) -> bool {
	ref<Integer> v = jrt::cast<Integer>(v_in);
	return jrt::unbox<int32_t>(v) % 2 == 0;
})))->map(jrt::fn<Function>([=](ref<Object> a0) -> ref<Object> { return jrt::String::valueOf(a0); })), j2xo14 = jrt::Collectors::joining(jrt::lit(u",")), jrt::cast<String>(jrt::nn(j2xo13)->collect(j2xo14))));
	ref<Map> m = jrt::make<HashMap>();
	for (ref<String> w : jrt::in(jrt::nn(jrt::lit(u"x y x z y x"))->split(jrt::lit(u" ")))) {
		jrt::cast<Integer>(jrt::nn(m)->merge(w, jrt::box(1), jrt::fn<BiFunction>([=](ref<Object> a0, ref<Object> a1) -> ref<Object> { return jrt::box(jrt::Integer_::sum(jrt::unbox<int32_t>(a0), jrt::unbox<int32_t>(a1))); })));
	}
	jrt::nn(jrt::System::out_())->println(m);
}

Modern_Point::Modern_Point(int32_t x, int32_t y) {
	this->x_ = x;
	this->y_ = y;
}

int32_t Modern_Point::dist2() {
	return this->x_ * this->x_ + this->y_ * this->y_;
}

int32_t Modern_Point::x() {
	return this->x_;
}

int32_t Modern_Point::y() {
	return this->y_;
}

ref<String> Modern_Point::toString() {
	return jrt::concat(jrt::lit(u"Point["), jrt::lit(u"x="), this->x_, jrt::lit(u", y="), this->y_, jrt::lit(u"]"));
}

int32_t Modern_Point::hashCode() {
	int32_t h = 0;
	h = 31 * h + jrt::Integer_::hashCode(this->x_);
	h = 31 * h + jrt::Integer_::hashCode(this->y_);
	return h;
}

bool Modern_Point::equals(ref<Object> o) {
	if (!(jrt::isa<Modern_Point>(o))) {
		return false;
	}
	auto that = jrt::cast<Modern_Point>(o);
	return this->x_ == that->x_ && this->y_ == that->y_;
}

Modern_Sq::Modern_Sq(int32_t side) {
	this->side_ = side;
}

int32_t Modern_Sq::side() {
	return this->side_;
}

ref<String> Modern_Sq::toString() {
	return jrt::concat(jrt::lit(u"Sq["), jrt::lit(u"side="), this->side_, jrt::lit(u"]"));
}

int32_t Modern_Sq::hashCode() {
	int32_t h = 0;
	h = 31 * h + jrt::Integer_::hashCode(this->side_);
	return h;
}

bool Modern_Sq::equals(ref<Object> o) {
	if (!(jrt::isa<Modern_Sq>(o))) {
		return false;
	}
	auto that = jrt::cast<Modern_Sq>(o);
	return this->side_ == that->side_;
}

Modern_Circ::Modern_Circ(int32_t r) {
	this->r_ = r;
}

int32_t Modern_Circ::r() {
	return this->r_;
}

ref<String> Modern_Circ::toString() {
	return jrt::concat(jrt::lit(u"Circ["), jrt::lit(u"r="), this->r_, jrt::lit(u"]"));
}

int32_t Modern_Circ::hashCode() {
	int32_t h = 0;
	h = 31 * h + jrt::Integer_::hashCode(this->r_);
	return h;
}

bool Modern_Circ::equals(ref<Object> o) {
	if (!(jrt::isa<Modern_Circ>(o))) {
		return false;
	}
	auto that = jrt::cast<Modern_Circ>(o);
	return this->r_ == that->r_;
}

void Modern_Color::ctor_(ref<String> name_in, int32_t ordinal_in) {
	clinit_();
	jrt::Enum::ctor_(name_in, ordinal_in);
	finit_();
}

void Modern_Color::finit_() {
}

void Modern_Color::clinit_() {
	if (inited_) return;
	inited_ = true;
	RED = jrt::make<Modern_Color>(jrt::lit(u"RED"), 0);
	GREEN = jrt::make<Modern_Color>(jrt::lit(u"GREEN"), 1);
	BLUE = jrt::make<Modern_Color>(jrt::lit(u"BLUE"), 2);
	values_ = jrt::array_of<ref<Modern_Color>>({RED, GREEN, BLUE});
}

ref<Modern_Color> Modern_Color::valueOf(ref<String> n) {
	clinit_();
	for (auto& v : *values_) if (v->name()->equals(n)) return v;
	jrt::raise_msg(std::make_shared<jrt::IllegalArgumentException>(), u"No enum constant Modern.Color." + n->s);
}


int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "Modern") == 0) return jrt::run_main(&Modern::main, argc, argv);
	return jrt::run_main(&Modern::main, argc, argv);
}
