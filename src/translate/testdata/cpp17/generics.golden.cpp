// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the
// Java program's L2; it runs on jrt.hpp, the Java library in C++17.
// these classes can form a reference cycle, which the runtime's cycle collector frees: Generics_Box, Generics_Pair, Generics_Stack, Generics_Stack_anon1
#define JRT_CYCLES
#include "jrt.hpp"

using jrt::ref;
using jrt::Array;
using jrt::ArrayList;
using jrt::Function;
using jrt::Integer;
using jrt::Iterable;
using jrt::Iterator;
using jrt::List;
using jrt::NoSuchElementException;
using jrt::Object;
using jrt::Predicate;
using jrt::PrintStream;
using jrt::String;

struct Generics;
struct Generics_Box;
struct Generics_Pair;
struct Generics_Visitor;
struct Generics_Stack;
struct Generics_Stack_anon1;
struct Generics_Counter;
struct Generics_anon2;

struct Generics : virtual Object {
	const char16_t* jclass_() const override { return u"Generics"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	int32_t total{};
	Generics();
	static ref<Object> max(ref<List> xs);
	static int32_t countMatching(ref<List> xs, ref<Predicate> p);
	static int32_t boxed(int32_t v);
	static ref<String> pairText(int32_t a, ref<String> b);
	static int32_t maxOf(int32_t a, int32_t b);
	static ref<String> maxWord(ref<String> a, ref<String> b);
	static int32_t stackSum(int32_t n);
	static ref<String> visit(int32_t n);
	virtual int32_t hits(int32_t n);
	static int32_t innerHits(int32_t n);
	static void main(ref<Array<ref<String>>> args);
};

struct Generics_Box : virtual Object {
	const char16_t* jclass_() const override { return u"Generics$Box"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	ref<Object> value{};
	Generics_Box(ref<Object> value);
	virtual ref<Object> get();
	virtual void set(ref<Object> v);
	virtual ref<Generics_Box> map(ref<Function> f);
	Generics_Box() {}
};

struct Generics_Pair : virtual Object {
	const char16_t* jclass_() const override { return u"Generics$Pair"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	ref<Object> first_{};
	ref<Object> second_{};
	Generics_Pair(ref<Object> first, ref<Object> second);
	virtual ref<Generics_Pair> swap();
	virtual ref<Object> first();
	virtual ref<Object> second();
	virtual ref<String> toString();
	virtual int32_t hashCode();
	virtual bool equals(ref<Object> o);
	Generics_Pair() {}
};

struct Generics_Visitor : virtual Object {
	const char16_t* jclass_() const override { return u"Generics$Visitor"; }
	virtual ref<Object> visitNumber(int32_t n) = 0;
	virtual ref<Object> visitText(ref<String> s) = 0;
};

struct Generics_Stack : virtual Object, virtual Iterable {
	const char16_t* jclass_() const override { return u"Generics$Stack"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	ref<List> items{};
	void ctor_();
	virtual void push(ref<Object> t);
	virtual ref<Object> pop();
	virtual bool isEmpty();
	virtual ref<Iterator> iterator();
	void finit_();
};

struct Generics_Stack_anon1 : virtual Object, virtual Iterator {
	const char16_t* jclass_() const override { return u"Generics_Stack_anon1"; }
	ref<Generics_Stack> outer_;
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	int32_t i{};
	void ctor_();
	virtual bool hasNext();
	virtual ref<Object> next();
	void finit_();
};

struct Generics_Counter : virtual Object {
	const char16_t* jclass_() const override { return u"Generics$Counter"; }
	ref<Generics> outer_;
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	int32_t count{};
	void ctor_();
	virtual void hit();
	void finit_();
};

struct Generics_anon2 : virtual Object, virtual Generics_Visitor {
	const char16_t* jclass_() const override { return u"Generics_anon2"; }
	jrt::gc::Tracked gc_tracked_{this};
	void trace_(jrt::gc::Tracer& t) override;
	void clear_() override;
	void ctor_();
	virtual ref<Object> visitNumber(int32_t k);
	virtual ref<Object> visitText(ref<String> s);
	void finit_();
};

void Generics::trace_(jrt::gc::Tracer& t) {
	(void)t;
}
void Generics::clear_() {
}

Generics::Generics() {
}

ref<Object> Generics::max(ref<List> xs) {
	ref<Object> best = jrt::nn(xs)->get(0);
	for (ref<Object> x : jrt::in(xs)) {
		if (jrt::natural(x, best) > 0) {
			best = x;
		}
	}
	return best;
}

int32_t Generics::countMatching(ref<List> xs, ref<Predicate> p) {
	int32_t n = 0;
	for (ref<Object> x : jrt::in(xs)) {
		if (jrt::nn(p)->test(x)) {
			n++;
		}
	}
	return n;
}

int32_t Generics::boxed(int32_t v) {
	auto b = std::make_shared<Generics_Box>(jrt::box(v));
	jrt::nn(b)->set(jrt::box(jrt::unbox<int32_t>(jrt::cast<Integer>(jrt::nn(b)->get())) + 1));
	ref<Generics_Box> s = jrt::nn(b)->map(jrt::fn<Function>([=](ref<Object> x_in) -> ref<Object> {
	ref<Integer> x = jrt::cast<Integer>(x_in);
	return jrt::concat(jrt::lit(u"n"), x);
}));
	int32_t j2xo1{};
	ref<Integer> j2xo2{};
	return (j2xo1 = jrt::nn(jrt::cast<String>(jrt::nn(s)->get()))->length(), j2xo2 = jrt::cast<Integer>(jrt::nn(b)->get()), j2xo1 + jrt::unbox<int32_t>(j2xo2));
}

ref<String> Generics::pairText(int32_t a, ref<String> b) {
	auto p = std::make_shared<Generics_Pair>(jrt::box(a), b);
	ref<String> j2xo1{};
	ref<Integer> j2xo2{};
	return (j2xo1 = jrt::nn(jrt::nn(p)->swap())->toString(), j2xo2 = jrt::cast<Integer>(jrt::nn(p)->first()), jrt::concat(j2xo1, j2xo2));
}

int32_t Generics::maxOf(int32_t a, int32_t b) {
	ref<List> xs = jrt::make<ArrayList>();
	jrt::nn(xs)->add(jrt::box(a));
	jrt::nn(xs)->add(jrt::box(b));
	jrt::nn(xs)->add(jrt::box(a ^ b));
	return jrt::unbox<int32_t>(jrt::cast<Integer>(Generics::max(xs)));
}

ref<String> Generics::maxWord(ref<String> a, ref<String> b) {
	ref<List> xs = jrt::List::of(a, b, jrt::lit(u"m"));
	return jrt::cast<String>(Generics::max(xs));
}

int32_t Generics::stackSum(int32_t n) {
	auto st = jrt::make<Generics_Stack>();
	for (int32_t i = 0; i < n % 40; i++) {
		jrt::nn(st)->push(jrt::box(i * i));
	}
	int32_t sum = 0;
	for (int32_t v : jrt::each<int32_t>(st)) {
		sum = sum * 3 + v;
	}
	return sum;
}

ref<String> Generics::visit(int32_t n) {
	ref<Generics_Visitor> v = [&] { auto o_ = std::make_shared<Generics_anon2>();  o_->ctor_(); return o_; }();
	return (n % 2 == 0 ? jrt::cast<String>(jrt::nn(v)->visitNumber(n)) : jrt::cast<String>(jrt::nn(v)->visitText(jrt::concat(jrt::lit(u"t"), n))));
}

int32_t Generics::hits(int32_t n) {
	auto a = [&] { auto o_ = std::make_shared<Generics_Counter>(); o_->outer_ = jrt::self(this); o_->ctor_(); return o_; }();
	auto b = [&] { auto o_ = std::make_shared<Generics_Counter>(); o_->outer_ = jrt::self(this); o_->ctor_(); return o_; }();
	for (int32_t i = 0; i < n % 30; i++) {
		jrt::nn(a)->hit();
		if (i % 3 == 0) {
			jrt::nn(b)->hit();
		}
	}
	return jrt::nn(a)->count * 100 + jrt::nn(b)->count + this->total * 10000;
}

int32_t Generics::innerHits(int32_t n) {
	return jrt::nn(std::make_shared<Generics>())->hits(n);
}

void Generics::main(ref<Array<ref<String>>> args) {
	int32_t j2xo1{};
	ref<String> j2xo2{};
	int32_t j2xo3{};
	ref<String> j2xo4{};
	jrt::nn(jrt::System::out_())->println((j2xo1 = Generics::boxed(41), j2xo2 = Generics::pairText(7, jrt::lit(u"seven")), j2xo3 = Generics::maxOf(12, 5), j2xo4 = Generics::maxWord(jrt::lit(u"pear"), jrt::lit(u"apple")), jrt::concat(j2xo1, jrt::lit(u" "), j2xo2, jrt::lit(u" "), j2xo3, jrt::lit(u" "), j2xo4)));
	auto st = jrt::make<Generics_Stack>();
	jrt::nn(st)->push(jrt::lit(u"a"));
	jrt::nn(st)->push(jrt::lit(u"b"));
	jrt::nn(st)->push(jrt::lit(u"c"));
	for (ref<String> s : jrt::each<String>(st)) {
		jrt::nn(jrt::System::out_())->print(s);
	}
	ref<String> j2xo5{};
	ref<String> j2xo6{};
	bool j2xo7{};
	jrt::nn(jrt::System::out_())->println((j2xo5 = jrt::cast<String>(jrt::nn(st)->pop()), j2xo6 = jrt::cast<String>(jrt::nn(st)->pop()), j2xo7 = jrt::nn(st)->isEmpty(), jrt::concat(jrt::lit(u" "), j2xo5, j2xo6, jrt::lit(u" "), j2xo7)));
	try {
		jrt::cast<String>(jrt::nn(st)->pop());
		jrt::cast<String>(jrt::nn(st)->pop());
	} catch (jrt::JavaThrow& j2x_t8) {
		if (auto e = jrt::as<NoSuchElementException>(j2x_t8.t)) {
			jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"caught: "), jrt::nn(e)->getMessage()));
		} else {
			throw;
		}
	}
	int32_t j2xo9{};
	ref<String> j2xo10{};
	ref<String> j2xo11{};
	int32_t j2xo12{};
	jrt::nn(jrt::System::out_())->println((j2xo9 = Generics::stackSum(10), j2xo10 = Generics::visit(4), j2xo11 = Generics::visit(5), j2xo12 = Generics::innerHits(10), jrt::concat(j2xo9, jrt::lit(u" "), j2xo10, jrt::lit(u" "), j2xo11, jrt::lit(u" "), j2xo12)));
	ref<List> words = jrt::List::of(jrt::lit(u"alpha"), jrt::lit(u"beta"), jrt::lit(u"gamma"), jrt::lit(u"delta"), jrt::lit(u"epsilon"));
	int32_t j2xo13{};
	int32_t j2xo14{};
	jrt::nn(jrt::System::out_())->println((j2xo13 = Generics::countMatching(words, jrt::fn<Predicate>([=](ref<Object> w_in) -> bool {
	ref<String> w = jrt::cast<String>(w_in);
	return jrt::nn(w)->length() > 4;
})), j2xo14 = Generics::countMatching(words, jrt::fn<Predicate>([=](ref<Object> w_in) -> bool {
	ref<String> w = jrt::cast<String>(w_in);
	return jrt::nn(w)->contains(jrt::lit(u"l"));
})), jrt::concat(j2xo13, jrt::lit(u" "), j2xo14)));
}

void Generics_Box::trace_(jrt::gc::Tracer& t) {
	t(value);
}
void Generics_Box::clear_() {
	value = nullptr;
}

Generics_Box::Generics_Box(ref<Object> value) {
	this->value = value;
}

ref<Object> Generics_Box::get() {
	return this->value;
}

void Generics_Box::set(ref<Object> v) {
	this->value = v;
}

ref<Generics_Box> Generics_Box::map(ref<Function> f) {
	return std::make_shared<Generics_Box>(jrt::nn(f)->apply(this->value));
}

void Generics_Pair::trace_(jrt::gc::Tracer& t) {
	t(first_);
	t(second_);
}
void Generics_Pair::clear_() {
	first_ = nullptr;
	second_ = nullptr;
}

Generics_Pair::Generics_Pair(ref<Object> first, ref<Object> second) {
	this->first_ = first;
	this->second_ = second;
}

ref<Generics_Pair> Generics_Pair::swap() {
	return std::make_shared<Generics_Pair>(this->second_, this->first_);
}

ref<Object> Generics_Pair::first() {
	return this->first_;
}

ref<Object> Generics_Pair::second() {
	return this->second_;
}

ref<String> Generics_Pair::toString() {
	return jrt::concat(jrt::lit(u"Pair["), jrt::lit(u"first="), this->first_, jrt::lit(u", second="), this->second_, jrt::lit(u"]"));
}

int32_t Generics_Pair::hashCode() {
	int32_t h = 0;
	h = 31 * h + jrt::Objects::hashCode(this->first_);
	h = 31 * h + jrt::Objects::hashCode(this->second_);
	return h;
}

bool Generics_Pair::equals(ref<Object> o) {
	if (!(jrt::isa<Generics_Pair>(o))) {
		return false;
	}
	auto that = jrt::cast<Generics_Pair>(o);
	return jrt::Objects::equals(this->first_, jrt::nn(that)->first_) && jrt::Objects::equals(this->second_, jrt::nn(that)->second_);
}

void Generics_Stack::trace_(jrt::gc::Tracer& t) {
	t(items);
}
void Generics_Stack::clear_() {
	items = nullptr;
}

void Generics_Stack::ctor_() {
	finit_();
}

void Generics_Stack::push(ref<Object> t) {
	jrt::nn(this->items)->add(t);
}

ref<Object> Generics_Stack::pop() {
	if (jrt::nn(this->items)->isEmpty()) {
		jrt::raise(jrt::make<NoSuchElementException>(jrt::lit(u"empty stack")));
	}
	return jrt::nn(this->items)->remove(jrt::nn(this->items)->size() - 1);
}

bool Generics_Stack::isEmpty() {
	return jrt::nn(this->items)->isEmpty();
}

ref<Iterator> Generics_Stack::iterator() {
	return [&] { auto o_ = std::make_shared<Generics_Stack_anon1>(); o_->outer_ = jrt::self(this); o_->ctor_(); return o_; }();
}

void Generics_Stack::finit_() {
	this->items = jrt::make<ArrayList>();
}

void Generics_Stack_anon1::trace_(jrt::gc::Tracer& t) {
	t(outer_);
}
void Generics_Stack_anon1::clear_() {
	outer_ = nullptr;
}

void Generics_Stack_anon1::ctor_() {
	finit_();
}

bool Generics_Stack_anon1::hasNext() {
	return this->i >= 0;
}

ref<Object> Generics_Stack_anon1::next() {
	return jrt::nn(jrt::nn(this->outer_)->items)->get(this->i--);
}

void Generics_Stack_anon1::finit_() {
	this->i = jrt::nn(jrt::nn(this->outer_)->items)->size() - 1;
}

void Generics_Counter::trace_(jrt::gc::Tracer& t) {
	t(outer_);
}
void Generics_Counter::clear_() {
	outer_ = nullptr;
}

void Generics_Counter::ctor_() {
	finit_();
}

void Generics_Counter::hit() {
	this->count++;
	jrt::nn(this->outer_)->total++;
}

void Generics_Counter::finit_() {
}

void Generics_anon2::trace_(jrt::gc::Tracer& t) {
	(void)t;
}
void Generics_anon2::clear_() {
}

void Generics_anon2::ctor_() {
	finit_();
}

ref<Object> Generics_anon2::visitNumber(int32_t k) {
	return jrt::concat(jrt::lit(u"#"), k);
}

ref<Object> Generics_anon2::visitText(ref<String> s) {
	return jrt::concat(jrt::lit(u"'"), s, jrt::lit(u"'"));
}

void Generics_anon2::finit_() {
}


int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "Generics") == 0) return jrt::run_main(&Generics::main, argc, argv);
	return jrt::run_main(&Generics::main, argc, argv);
}
