// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the
// Java program's L2; it runs on jrt.hpp, the Java library in C++17.
#include "jrt.hpp"

using jrt::ref;
using jrt::Array;
using jrt::ArrayList;
using jrt::Enum;
using jrt::IllegalArgumentException;
using jrt::List;
using jrt::Object;
using jrt::PrintStream;
using jrt::String;

struct Shapes;
struct Shapes_Shape;
struct Shapes_Circle;
struct Shapes_Rect;
struct Shapes_Square;
struct Shapes_Size;

struct Shapes : virtual Object {
	const char16_t* jclass_() const override { return u"Shapes"; }
	Shapes();
	static ref<Shapes_Size> classify(double area);
	static ref<String> sizeWord(int32_t k);
	static int32_t sides(ref<Object> o);
	static int32_t rectArea(int32_t w, int32_t h);
	static bool sameRect(int32_t a, int32_t b);
	static ref<String> rectText(int32_t a, int32_t b);
	static int32_t rectHash(int32_t a, int32_t b);
	static ref<String> dayKind(ref<String> day);
	static int32_t countKinds(int32_t n);
	static void main(ref<Array<ref<String>>> args);
};

struct Shapes_Shape : virtual Object {
	const char16_t* jclass_() const override { return u"Shapes.Shape"; }
	virtual double area() = 0;
	virtual ref<String> describe();
	virtual ref<String> name() = 0;
};

struct Shapes_Circle : virtual Object, virtual Shapes_Shape {
	const char16_t* jclass_() const override { return u"Shapes.Circle"; }
	double r_{};
	Shapes_Circle(double r);
	virtual double area();
	virtual ref<String> name();
	virtual double r();
	virtual ref<String> toString();
	virtual int32_t hashCode();
	virtual bool equals(ref<Object> o);
	Shapes_Circle() {}
};

struct Shapes_Rect : virtual Object, virtual Shapes_Shape {
	const char16_t* jclass_() const override { return u"Shapes.Rect"; }
	int32_t w_{};
	int32_t h_{};
	void ctor_(int32_t w, int32_t h);
	virtual double area();
	virtual ref<String> name();
	virtual int32_t w();
	virtual int32_t h();
	virtual ref<String> toString();
	virtual int32_t hashCode();
	virtual bool equals(ref<Object> o);
	void finit_();
};

struct Shapes_Square : virtual Object, virtual Shapes_Shape {
	const char16_t* jclass_() const override { return u"Shapes.Square"; }
	int32_t side_{};
	Shapes_Square(int32_t side);
	virtual double area();
	virtual ref<String> name();
	virtual int32_t side();
	virtual ref<String> toString();
	virtual int32_t hashCode();
	virtual bool equals(ref<Object> o);
	Shapes_Square() {}
};

struct Shapes_Size : Enum {
	const char16_t* jclass_() const override { return u"Shapes.Size"; }
	static inline ref<Shapes_Size> SMALL;
	static inline ref<Shapes_Size> MEDIUM;
	static inline ref<Shapes_Size> LARGE;
	static inline ref<Array<ref<Shapes_Size>>> values_;
	static ref<Array<ref<Shapes_Size>>> values() { clinit_(); return values_->clone(); }
	static ref<Shapes_Size> valueOf(ref<String> n);
	static inline bool inited_ = false;
	static void clinit_();
	void ctor_(ref<String> name_in, int32_t ordinal_in);
	void finit_();
};

Shapes::Shapes() {
}

ref<Shapes_Size> Shapes::classify(double area) {
	if (area < 10) {
		return (Shapes_Size::clinit_(), Shapes_Size::SMALL);
	}
	if (area < 100) {
		return (Shapes_Size::clinit_(), Shapes_Size::MEDIUM);
	}
	return (Shapes_Size::clinit_(), Shapes_Size::LARGE);
}

ref<String> Shapes::sizeWord(int32_t k) {
	ref<Shapes_Size> s = (*jrt::nn(Shapes_Size::values()))[jrt::Math::floorMod(k, 3)];
	return [&]() -> ref<String> {
	switch (jrt::nn(s)->ordinal()) {
	case 0 /* SMALL */:
	{
		return jrt::lit(u"s");
	}
	case 1 /* MEDIUM */:
	{
		return jrt::lit(u"m");
	}
	case 2 /* LARGE */:
	{
		{
			ref<String> big = jrt::lit(u"L");
			return jrt::concat(big, big);
		}
	}
	}
	jrt::raise_new<jrt::IllegalStateException>(u"no switch arm matched");
}();
}

int32_t Shapes::sides(ref<Object> o) {
	ref<Shapes_Square> sq{};
	if (jrt::isa<Shapes_Square>(o) && (sq = jrt::cast<Shapes_Square>(o)) != nullptr) {
		return 4 + jrt::nn(sq)->side() * 0;
	}
	ref<Shapes_Rect> r{};
	int32_t j2xo1{};
	int32_t j2xo2{};
	if ((jrt::isa<Shapes_Rect>(o) && (r = jrt::cast<Shapes_Rect>(o)) != nullptr) && (j2xo1 = jrt::nn(r)->w(), j2xo2 = jrt::nn(r)->h(), j2xo1 != j2xo2)) {
		return 4;
	}
	if (jrt::isa<Shapes_Circle>(o)) {
		return 0;
	}
	return -1;
}

int32_t Shapes::rectArea(int32_t w, int32_t h) {
	auto r = jrt::make<Shapes_Rect>(jrt::Math::abs(w) % 1000, jrt::Math::abs(h) % 1000);
	return jrt::d2i(jrt::nn(r)->area());
}

bool Shapes::sameRect(int32_t a, int32_t b) {
	ref<Shapes_Rect> j2xo1{};
	ref<Shapes_Rect> j2xo2{};
	return (j2xo1 = jrt::make<Shapes_Rect>(a & 15, b & 15), j2xo2 = jrt::make<Shapes_Rect>(b & 15, a & 15), jrt::nn(j2xo1)->equals(j2xo2));
}

ref<String> Shapes::rectText(int32_t a, int32_t b) {
	return jrt::nn(jrt::make<Shapes_Rect>(a & 255, b & 255))->toString();
}

int32_t Shapes::rectHash(int32_t a, int32_t b) {
	return jrt::nn(jrt::make<Shapes_Rect>(a & 255, b & 255))->hashCode();
}

ref<String> Shapes::dayKind(ref<String> day) {
	return [&]() -> ref<String> {
	auto s_ = jrt::nn(day);
	if (s_->equals(jrt::lit(u"sat")) || s_->equals(jrt::lit(u"sun"))) {
		return jrt::lit(u"weekend");
	}
	if (s_->equals(jrt::lit(u"mon")) || s_->equals(jrt::lit(u"tue")) || s_->equals(jrt::lit(u"wed")) || s_->equals(jrt::lit(u"thu")) || s_->equals(jrt::lit(u"fri"))) {
		return jrt::lit(u"weekday");
	}
	{
		return jrt::lit(u"unknown");
	}
	jrt::raise_new<jrt::IllegalStateException>(u"no switch arm matched");
}();
}

int32_t Shapes::countKinds(int32_t n) {
	ref<List> shapes = jrt::make<ArrayList>();
	for (int32_t i = 0; i < n % 20; i++) {
		jrt::nn(shapes)->add([&]() -> ref<Shapes_Shape> {
	switch (i % 3) {
	case 0:
	{
		return std::make_shared<Shapes_Circle>((double)i);
	}
	case 1:
	{
		return jrt::make<Shapes_Rect>(i, i + 1);
	}
	default:
	{
		return std::make_shared<Shapes_Square>(i);
	}
	}
	jrt::raise_new<jrt::IllegalStateException>(u"no switch arm matched");
}());
	}
	int32_t total = 0;
	for (ref<Shapes_Shape> s : jrt::each<Shapes_Shape>(shapes)) {
		total += Shapes::sides(s);
	}
	return total;
}

void Shapes::main(ref<Array<ref<String>>> args) {
	ref<Shapes_Circle> j2xo1{};
	ref<Shapes_Rect> j2xo2{};
	ref<Shapes_Square> j2xo3{};
	ref<Shapes_Rect> j2xo4{};
	ref<List> shapes = (j2xo1 = std::make_shared<Shapes_Circle>(1.5), j2xo2 = jrt::make<Shapes_Rect>(3, 4), j2xo3 = std::make_shared<Shapes_Square>(5), j2xo4 = jrt::make<Shapes_Rect>(2, 2), jrt::List::of(j2xo1, j2xo2, j2xo3, j2xo4));
	double sum = (double)(0);
	for (ref<Shapes_Shape> s : jrt::each<Shapes_Shape>(shapes)) {
		ref<String> j2xo5{};
		ref<Shapes_Size> j2xo6{};
		int32_t j2xo7{};
		jrt::nn(jrt::System::out_())->println((j2xo5 = jrt::nn(s)->describe(), j2xo6 = Shapes::classify(jrt::nn(s)->area()), j2xo7 = Shapes::sides(s), jrt::concat(j2xo5, jrt::lit(u" -> "), j2xo6, jrt::lit(u" sides="), j2xo7)));
		sum += jrt::nn(s)->area();
	}
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"total "), sum));
	ref<Shapes_Rect> j2xo8{};
	ref<Shapes_Circle> j2xo9{};
	ref<Shapes_Rect> j2xo10{};
	ref<Shapes_Rect> j2xo11{};
	bool j2xo12{};
	jrt::nn(jrt::System::out_())->println((j2xo8 = jrt::make<Shapes_Rect>(1, 2), j2xo9 = std::make_shared<Shapes_Circle>(0.5), j2xo12 = (j2xo10 = jrt::make<Shapes_Rect>(1, 2), j2xo11 = jrt::make<Shapes_Rect>(1, 2), jrt::nn(j2xo10)->equals(j2xo11)), jrt::concat(j2xo8, jrt::lit(u" "), j2xo9, jrt::lit(u" "), j2xo12)));
	for (int32_t k = 0; k < 4; k++) {
		jrt::nn(jrt::System::out_())->print(jrt::concat(Shapes::sizeWord(k), jrt::lit(u",")));
	}
	jrt::nn(jrt::System::out_())->println();
	ref<String> j2xo13{};
	ref<String> j2xo14{};
	ref<String> j2xo15{};
	jrt::nn(jrt::System::out_())->println((j2xo13 = Shapes::dayKind(jrt::lit(u"sun")), j2xo14 = Shapes::dayKind(jrt::lit(u"wed")), j2xo15 = Shapes::dayKind(jrt::lit(u"xyz")), jrt::concat(j2xo13, jrt::lit(u" "), j2xo14, jrt::lit(u" "), j2xo15)));
	try {
		jrt::make<Shapes_Rect>(-1, 2);
	} catch (jrt::JavaThrow& j2x_t16) {
		if (auto e = jrt::as<IllegalArgumentException>(j2x_t16.t)) {
			jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"caught "), jrt::nn(e)->getMessage()));
		} else {
			throw;
		}
	}
	jrt::nn(jrt::System::out_())->println(Shapes::countKinds(10));
}

ref<String> Shapes_Shape::describe() {
	ref<String> j2xo1{};
	ref<String> j2xo2{};
	return (j2xo1 = this->name(), j2xo2 = jrt::String::format(jrt::lit(u"%.2f"), jrt::box(this->area())), jrt::concat(j2xo1, jrt::lit(u" with area "), j2xo2));
}

Shapes_Circle::Shapes_Circle(double r) {
	this->r_ = r;
}

double Shapes_Circle::area() {
	return jrt::Math::PI * this->r_ * this->r_;
}

ref<String> Shapes_Circle::name() {
	return jrt::lit(u"circle");
}

double Shapes_Circle::r() {
	return this->r_;
}

ref<String> Shapes_Circle::toString() {
	return jrt::concat(jrt::lit(u"Circle["), jrt::lit(u"r="), this->r_, jrt::lit(u"]"));
}

int32_t Shapes_Circle::hashCode() {
	int32_t h = 0;
	h = 31 * h + jrt::Double_::hashCode(this->r_);
	return h;
}

bool Shapes_Circle::equals(ref<Object> o) {
	if (!(jrt::isa<Shapes_Circle>(o))) {
		return false;
	}
	auto that = jrt::cast<Shapes_Circle>(o);
	return jrt::Double_::compare(this->r_, jrt::nn(that)->r_) == 0;
}

void Shapes_Rect::ctor_(int32_t w, int32_t h) {
	finit_();
	if (w < 0 || h < 0) {
		jrt::raise(jrt::make<IllegalArgumentException>(jrt::lit(u"negative side")));
	}
	this->w_ = w;
	this->h_ = h;
}

double Shapes_Rect::area() {
	return (double)(this->w_ * this->h_);
}

ref<String> Shapes_Rect::name() {
	return jrt::lit(u"rect");
}

int32_t Shapes_Rect::w() {
	return this->w_;
}

int32_t Shapes_Rect::h() {
	return this->h_;
}

ref<String> Shapes_Rect::toString() {
	return jrt::concat(jrt::lit(u"Rect["), jrt::lit(u"w="), this->w_, jrt::lit(u", h="), this->h_, jrt::lit(u"]"));
}

int32_t Shapes_Rect::hashCode() {
	int32_t h = 0;
	h = 31 * h + jrt::Integer_::hashCode(this->w_);
	h = 31 * h + jrt::Integer_::hashCode(this->h_);
	return h;
}

bool Shapes_Rect::equals(ref<Object> o) {
	if (!(jrt::isa<Shapes_Rect>(o))) {
		return false;
	}
	auto that = jrt::cast<Shapes_Rect>(o);
	return this->w_ == jrt::nn(that)->w_ && this->h_ == jrt::nn(that)->h_;
}

void Shapes_Rect::finit_() {
}

Shapes_Square::Shapes_Square(int32_t side) {
	this->side_ = side;
}

double Shapes_Square::area() {
	return (double)(this->side_ * this->side_);
}

ref<String> Shapes_Square::name() {
	return jrt::lit(u"square");
}

int32_t Shapes_Square::side() {
	return this->side_;
}

ref<String> Shapes_Square::toString() {
	return jrt::concat(jrt::lit(u"Square["), jrt::lit(u"side="), this->side_, jrt::lit(u"]"));
}

int32_t Shapes_Square::hashCode() {
	int32_t h = 0;
	h = 31 * h + jrt::Integer_::hashCode(this->side_);
	return h;
}

bool Shapes_Square::equals(ref<Object> o) {
	if (!(jrt::isa<Shapes_Square>(o))) {
		return false;
	}
	auto that = jrt::cast<Shapes_Square>(o);
	return this->side_ == jrt::nn(that)->side_;
}

void Shapes_Size::ctor_(ref<String> name_in, int32_t ordinal_in) {
	clinit_();
	jrt::Enum::ctor_(name_in, ordinal_in);
	finit_();
}

void Shapes_Size::finit_() {
}

void Shapes_Size::clinit_() {
	if (inited_) return;
	inited_ = true;
	SMALL = jrt::make<Shapes_Size>(jrt::lit(u"SMALL"), 0);
	MEDIUM = jrt::make<Shapes_Size>(jrt::lit(u"MEDIUM"), 1);
	LARGE = jrt::make<Shapes_Size>(jrt::lit(u"LARGE"), 2);
	values_ = jrt::array_of<ref<Shapes_Size>>({SMALL, MEDIUM, LARGE});
}

ref<Shapes_Size> Shapes_Size::valueOf(ref<String> n) {
	clinit_();
	for (auto& v : *values_) if (v->name()->equals(n)) return v;
	jrt::raise_msg(std::make_shared<jrt::IllegalArgumentException>(), u"No enum constant Shapes.Size." + n->s);
}


int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "Shapes") == 0) return jrt::run_main(&Shapes::main, argc, argv);
	return jrt::run_main(&Shapes::main, argc, argv);
}
