// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the
// Java program's L2; it runs on jrt.hpp, the Java library in C++17.
// these classes can form a reference cycle, which the runtime's cycle collector frees: Mix_Box
#define JRT_CYCLES
#include "jrt.hpp"

using jrt::ref;
using jrt::Array;
using jrt::BiFunction;
using jrt::Enum;
using jrt::Exception;
using jrt::Function;
using jrt::IllegalStateException;
using jrt::Integer;
using jrt::Object;
using jrt::PrintStream;
using jrt::String;
using jrt::StringBuilder;
using jrt::Supplier;

struct Mix;
struct Mix_Color;
struct Mix_Shape;
struct Mix_Inner;
struct Mix_Box;
struct Mix_anon1;

struct Mix : virtual Object {
	const char16_t* jclass_() const override { return u"Mix"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	static inline int32_t LIMIT = 10;
	static inline int32_t counter{};
	static inline int64_t total = 5LL;
	static inline ref<String> label{};
	int32_t base{};
	static inline bool inited_ = false;
	static void clinit_();
	Mix();
	static int32_t fib(int32_t n);
	static int32_t sw(int32_t k);
	static ref<String> name(ref<String> s);
	static ref<Array<ref<Array<int32_t>>>> grid(int32_t n);
	static int64_t mix(int64_t a, int32_t b, double c, char16_t ch, int8_t by);
	static void thrower(int32_t x);
	static void main(ref<Array<ref<String>>> args);
};

struct Mix_Color : Enum {
	const char16_t* jclass_() const override { return u"Mix$Color"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	int32_t w{};
	static inline ref<Mix_Color> RED;
	static inline ref<Mix_Color> GREEN;
	static inline ref<Mix_Color> BLUE;
	static inline ref<Array<ref<Mix_Color>>> values_;
	static ref<Array<ref<Mix_Color>>> values() { clinit_(); return values_->clone(); }
	static ref<Mix_Color> valueOf(ref<String> n);
	static inline bool inited_ = false;
	static void clinit_();
	void ctor_(ref<String> name_in, int32_t ordinal_in);
	void ctor_(ref<String> name_in, int32_t ordinal_in, int32_t w);
	virtual int32_t weight();
	void finit_();
};

struct Mix_Shape : virtual Object {
	const char16_t* jclass_() const override { return u"Mix$Shape"; }
	virtual double area() = 0;
	template <class L> struct Of;
};
template <class L> struct Mix_Shape::Of : Mix_Shape {
	L l;
	explicit Of(L x) : l(std::move(x)) {}
	double area() override { return l(); }
};

struct Mix_Inner : virtual Object {
	const char16_t* jclass_() const override { return u"Mix$Inner"; }
	ref<Mix> outer_;
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	int32_t v{};
	void ctor_(int32_t v);
	virtual int32_t get();
	void finit_();
};

struct Mix_Box : virtual Object {
	const char16_t* jclass_() const override { return u"Mix$Box"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	ref<Object> item{};
	Mix_Box(ref<Object> item);
	virtual ref<Object> get();
	Mix_Box() {}
};

struct Mix_anon1 : virtual Object, virtual Mix_Shape {
	const char16_t* jclass_() const override { return u"Mix_anon1"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	void ctor_();
	virtual double area();
	void finit_();
};

void Mix::trace_(jrt::gc::Tracer& t) {
	(void)t;
}
void Mix::clear_() {
}

Mix::Mix() {
	clinit_();
	this->base = 4;
}

int32_t Mix::fib(int32_t n) {
	clinit_();
	int32_t j2xo1{};
	int32_t j2xo2{};
	return (n < 2 ? n : (j2xo1 = Mix::fib(n - 1), j2xo2 = Mix::fib(n - 2), j2xo1 + j2xo2));
}

int32_t Mix::sw(int32_t k) {
	clinit_();
	switch (k) {
	case 1:
	{
		return 10;
	}
	case 2:
	{
	}
	case 3:
	{
		k += 5;
		break;
	}
	default:
	{
		k = -1;
	}
	}
	return k;
}

ref<String> Mix::name(ref<String> s) {
	clinit_();
	{
		ref<String> j2x_s2 = (s);
		{
			if (jrt::nn(jrt::lit(u"a"))->equals(j2x_s2)) {
				return jrt::lit(u"alpha");
			} else if (jrt::nn(jrt::lit(u"b"))->equals(j2x_s2)) {
				return jrt::lit(u"beta");
			} else {
				return jrt::lit(u"other");
			}
		}
	}
}

ref<Array<ref<Array<int32_t>>>> Mix::grid(int32_t n) {
	clinit_();
	ref<Array<ref<Array<int32_t>>>> g = jrt::new_array2<int32_t>(n, n);
	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < n; j++) {
			(*jrt::nn((*jrt::nn(g))[i]))[j] = i * j;
		}
	}
	return g;
}

int64_t Mix::mix(int64_t a, int32_t b, double c, char16_t ch, int8_t by) {
	clinit_();
	int64_t r = a << ((b & 7) & 63);
	r ^= jrt::d2i(c);
	r += ch + by;
	r = (int64_t)jrt::ushr(r, (1 & 63));
	return r % 1000 + (b / 3);
}

void Mix::thrower(int32_t x) {
	clinit_();
	if (x > 2) {
		jrt::raise(jrt::make<IllegalStateException>(jrt::concat(jrt::lit(u"big "), x)));
	}
	if (x == 2) {
		jrt::raise(jrt::make<Exception>(jrt::lit(u"two")));
	}
}

void Mix::main(ref<Array<ref<String>>> args) {
	clinit_();
	auto m = std::make_shared<Mix>();
	auto in = [&] { auto o_ = std::make_shared<Mix_Inner>(); o_->outer_ = jrt::nonnull(m); o_->ctor_(3); return o_; }();
	int32_t j2xo1{};
	int32_t j2xo2{};
	int32_t j2xo3{};
	int32_t j2xo4{};
	int32_t j2xo5{};
	ref<String> j2xo6{};
	ref<String> j2xo7{};
	jrt::nn(jrt::System::out_())->println((j2xo1 = jrt::nn(in)->get(), j2xo2 = Mix::fib(10), j2xo3 = Mix::sw(1), j2xo4 = Mix::sw(3), j2xo5 = Mix::sw(9), j2xo6 = Mix::name(jrt::lit(u"b")), j2xo7 = Mix::name(jrt::lit(u"z")), jrt::concat(j2xo1, jrt::lit(u" "), j2xo2, jrt::lit(u" "), j2xo3, j2xo4, j2xo5, jrt::lit(u" "), j2xo6, j2xo7)));
	for (ref<Mix_Color> c : jrt::in(Mix_Color::values())) {
		ref<Mix_Color> j2xo8{};
		int32_t j2xo9{};
		int32_t j2xo10{};
		jrt::nn(jrt::System::out_())->println((j2xo8 = c, j2xo9 = jrt::nn(c)->weight(), j2xo10 = jrt::nn(c)->ordinal(), jrt::concat(j2xo8, jrt::lit(u":"), j2xo9, jrt::lit(u":"), j2xo10)));
	}
	ref<Mix_Shape> s = [&] { auto o_ = std::make_shared<Mix_anon1>();  o_->ctor_(); return o_; }();
	jrt::nn(jrt::System::out_())->println(jrt::nn(s)->area());
	ref<Array<ref<Array<int32_t>>>> g = Mix::grid(4);
	int32_t sum = 0;
	for (ref<Array<int32_t>> row : jrt::in(g)) {
		for (int32_t x : jrt::in(row)) {
			sum += x;
		}
	}
	jrt::nn(jrt::System::out_())->println(jrt::concat(sum, jrt::lit(u" "), Mix::mix(1234567LL, 5, 7.9, u'x', (int8_t)(3)), jrt::lit(u" "), Mix::label, Mix::counter, Mix::total));
	auto b = std::make_shared<Mix_Box>(jrt::lit(u"boxed"));
	int32_t j2xo11{};
	ref<String> j2xo12{};
	jrt::nn(jrt::System::out_())->println((j2xo11 = jrt::nn(jrt::cast<String>(jrt::nn(b)->get()))->length(), j2xo12 = jrt::cast<String>(jrt::nn(b)->get()), jrt::concat(j2xo11, j2xo12)));
	for (int32_t i = 0; i < 4; i++) {
		try {
			try {
				Mix::thrower(i);
				jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"ok "), i));
			} catch (jrt::JavaThrow& j2x_t13) {
				if (auto e = jrt::as<IllegalStateException>(j2x_t13.t)) {
					jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"ise "), jrt::nn(e)->getMessage()));
				} else if (auto e = jrt::as<Exception>(j2x_t13.t)) {
					jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"ex "), jrt::nn(e)->getMessage()));
				} else {
					throw;
				}
			}
		} catch (...) {
			Mix::counter++;
			throw;
		}
		{
			Mix::counter++;
		}
	}
	ref<Function> twice = jrt::fn<Function>([=](ref<Object> x_in) -> ref<Object> {
	ref<Integer> x = jrt::cast<Integer>(x_in);
	return jrt::box(jrt::unbox<int32_t>(x) * 2);
});
	ref<BiFunction> add = jrt::fn<BiFunction>([=](ref<Object> a0, ref<Object> a1) -> ref<Object> { return jrt::box(jrt::Integer_::sum(jrt::unbox<int32_t>(a0), jrt::unbox<int32_t>(a1))); });
	ref<Supplier> sup = jrt::fn<Supplier>([=]() -> ref<Object> { return jrt::concat(Mix::label, Mix::counter); });
	ref<Integer> j2xo14{};
	ref<Integer> j2xo15{};
	ref<String> j2xo16{};
	jrt::nn(jrt::System::out_())->println((j2xo14 = jrt::cast<Integer>(jrt::nn(twice)->apply(jrt::box(21))), j2xo15 = jrt::cast<Integer>(jrt::nn(add)->apply(jrt::box(3), jrt::box(4))), j2xo16 = jrt::cast<String>(jrt::nn(sup)->get()), jrt::concat(j2xo14, jrt::lit(u" "), j2xo15, jrt::lit(u" "), j2xo16)));
	auto sb = jrt::make<StringBuilder>();
	for (int32_t i = 0; i < 5; i++) {
		{
			int32_t j = 0;
			while (true) {
				if (j > i) {
					goto outer_cont;
				}
				if (i == 4) {
					goto outer_brk;
				}
				jrt::nn(jrt::nn(jrt::nn(jrt::nn(sb)->append(i))->append(u','))->append(j))->append(u' ');
				j++;
			}
		}
		outer_cont:;
	}
	outer_brk:;
	jrt::nn(jrt::System::out_())->println(jrt::nn(sb)->toString());
	ref<Object> o = b;
	if (jrt::isa<Mix_Box>(o)) {
		jrt::nn(jrt::System::out_())->println(jrt::lit(u"box"));
	}
	double d = 1.0 / 3;
	float f = 2.5f;
	jrt::nn(jrt::System::out_())->println(jrt::concat(d, jrt::lit(u" "), f, jrt::lit(u" "), jrt::d2l(d), jrt::lit(u" "), jrt::Math::max(3, 4), jrt::lit(u" "), jrt::Integer_::MAX_VALUE));
	int32_t k = 0;
	do {
		k += 3;
	} while (k < 10);
	char16_t c = (char16_t)((u'a' + k));
	jrt::nn(jrt::System::out_())->println(jrt::concat(k, jrt::lit(u" "), c, jrt::lit(u" "), (k > 5 && c != u'z' || k == 0)));
}

void Mix::clinit_() {
	if (inited_) return;
	inited_ = true;
	Mix::label = jrt::lit(u"mix");
	Mix::counter = 3;
}

void Mix_Color::trace_(jrt::gc::Tracer& t) {
	Enum::trace_(t);
}
void Mix_Color::clear_() {
	Enum::clear_();
}

void Mix_Color::ctor_(ref<String> name_in, int32_t ordinal_in) {
	clinit_();
	this->ctor_(name_in, ordinal_in, 1);
}

void Mix_Color::ctor_(ref<String> name_in, int32_t ordinal_in, int32_t w) {
	clinit_();
	jrt::Enum::ctor_(name_in, ordinal_in);
	finit_();
	this->w = w;
}

int32_t Mix_Color::weight() {
	return this->w;
}

void Mix_Color::finit_() {
}

void Mix_Color::clinit_() {
	if (inited_) return;
	inited_ = true;
	RED = jrt::make<Mix_Color>(jrt::lit(u"RED"), 0);
	GREEN = jrt::make<Mix_Color>(jrt::lit(u"GREEN"), 1, 5);
	BLUE = jrt::make<Mix_Color>(jrt::lit(u"BLUE"), 2);
	values_ = jrt::array_of<ref<Mix_Color>>({RED, GREEN, BLUE});
}

ref<Mix_Color> Mix_Color::valueOf(ref<String> n) {
	clinit_();
	for (auto& v : *values_) if (v->name()->equals(n)) return v;
	jrt::raise_msg(std::make_shared<jrt::IllegalArgumentException>(), u"No enum constant Mix.Color." + n->s);
}

void Mix_Inner::trace_(jrt::gc::Tracer& t) {
	t(outer_);
}
void Mix_Inner::clear_() {
	outer_ = nullptr;
}

void Mix_Inner::ctor_(int32_t v) {
	finit_();
	this->v = v + jrt::nn(this->outer_)->base;
}

int32_t Mix_Inner::get() {
	return this->v * 2;
}

void Mix_Inner::finit_() {
}

void Mix_Box::trace_(jrt::gc::Tracer& t) {
	t(item);
}
void Mix_Box::clear_() {
	item = nullptr;
}

Mix_Box::Mix_Box(ref<Object> item) {
	this->item = item;
}

ref<Object> Mix_Box::get() {
	return this->item;
}

void Mix_anon1::trace_(jrt::gc::Tracer& t) {
	(void)t;
}
void Mix_anon1::clear_() {
}

void Mix_anon1::ctor_() {
	finit_();
}

double Mix_anon1::area() {
	return 2.5 * (Mix::clinit_(), Mix::LIMIT);
}

void Mix_anon1::finit_() {
}


int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "Mix") == 0) return jrt::run_main(&Mix::main, argc, argv);
	return jrt::run_main(&Mix::main, argc, argv);
}
