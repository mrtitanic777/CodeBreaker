// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the
// Java program's L2; it runs on jrt.hpp, the Java library in C++17.
#include "jrt.hpp"

using jrt::ref;
using jrt::Array;
using jrt::ArrayList;
using jrt::BinaryOperator;
using jrt::Collector;
using jrt::Comparator;
using jrt::Consumer;
using jrt::Function;
using jrt::IntBinaryOperator;
using jrt::IntFunction;
using jrt::IntPredicate;
using jrt::IntStream;
using jrt::IntUnaryOperator;
using jrt::Integer;
using jrt::List;
using jrt::Map;
using jrt::Object;
using jrt::Optional;
using jrt::OptionalDouble;
using jrt::Predicate;
using jrt::PrintStream;
using jrt::Runnable;
using jrt::Stream;
using jrt::String;
using jrt::StringBuilder;
using jrt::Supplier;
using jrt::ToIntFunction;

struct Streams;
struct Streams_Person;

struct Streams : virtual Object {
	const char16_t* jclass_() const override { return u"Streams"; }
	Streams();
	static int32_t sumOfSquares(int32_t n);
	static int64_t countEven(int32_t n);
	static ref<String> joined(int32_t n);
	static int32_t applyTwice(int32_t a, int32_t b);
	static ref<String> upperWords(ref<String> s);
	static int32_t firstOver(int32_t limit);
	static int32_t composed(int32_t x);
	static int32_t counter(int32_t n);
	static ref<List> people();
	static void main(ref<Array<ref<String>>> args);
};

struct Streams_Person : virtual Object {
	const char16_t* jclass_() const override { return u"Streams$Person"; }
	ref<String> name_{};
	int32_t age_{};
	Streams_Person(ref<String> name, int32_t age);
	virtual ref<String> name();
	virtual int32_t age();
	virtual ref<String> toString();
	virtual int32_t hashCode();
	virtual bool equals(ref<Object> o);
	Streams_Person() {}
};

Streams::Streams() {
}

int32_t Streams::sumOfSquares(int32_t n) {
	return jrt::nn(jrt::nn(jrt::IntStream::rangeClosed(1, jrt::Math::abs(n) % 100))->map(jrt::fn<IntUnaryOperator>([=](int32_t x) -> int32_t { return x * x; })))->sum();
}

int64_t Streams::countEven(int32_t n) {
	return jrt::nn(jrt::nn(jrt::IntStream::range(0, jrt::Math::abs(n) % 500))->filter(jrt::fn<IntPredicate>([=](int32_t x) -> bool { return x % 2 == 0; })))->count();
}

ref<String> Streams::joined(int32_t n) {
	ref<Stream> j2xo1{};
	ref<Collector> j2xo2{};
	return (j2xo1 = jrt::nn(jrt::IntStream::range(0, jrt::Math::abs(n) % 12))->mapToObj(jrt::fn<IntFunction>([=](int32_t a0) -> ref<Object> { return jrt::Integer_::toString(a0); })), j2xo2 = jrt::Collectors::joining(jrt::lit(u","), jrt::lit(u"["), jrt::lit(u"]")), jrt::cast<String>(jrt::nn(j2xo1)->collect(j2xo2)));
}

int32_t Streams::applyTwice(int32_t a, int32_t b) {
	ref<IntBinaryOperator> op = jrt::fn<IntBinaryOperator>([=](int32_t x, int32_t y) -> int32_t { return x * 31 + y; });
	return jrt::nn(op)->applyAsInt(jrt::nn(op)->applyAsInt(a, b), b);
}

ref<String> Streams::upperWords(ref<String> s) {
	ref<Stream> j2xo1{};
	ref<Collector> j2xo2{};
	return (j2xo1 = jrt::nn(jrt::nn(jrt::Arrays::stream(jrt::nn(s)->split(jrt::lit(u" "))))->filter(jrt::fn<Predicate>([=](ref<Object> w_in) -> bool {
	ref<String> w = jrt::cast<String>(w_in);
	return !jrt::nn(w)->isEmpty();
})))->map(jrt::fn<Function>([=](ref<Object> a0) -> ref<Object> { return jrt::nn(jrt::cast<String>(a0))->toUpperCase(); })), j2xo2 = jrt::Collectors::joining(jrt::lit(u"-")), jrt::cast<String>(jrt::nn(j2xo1)->collect(j2xo2)));
}

int32_t Streams::firstOver(int32_t limit) {
	ref<Optional> f = jrt::nn(jrt::nn(jrt::nn(jrt::IntStream::iterate(1, jrt::fn<IntUnaryOperator>([=](int32_t x) -> int32_t { return x * 3; })))->boxed())->filter(jrt::fn<Predicate>([=](ref<Object> x_in) -> bool {
	ref<Integer> x = jrt::cast<Integer>(x_in);
	return jrt::unbox<int32_t>(x) > limit % 100000;
})))->findFirst();
	return jrt::unbox<int32_t>(jrt::cast<Integer>(jrt::nn(f)->orElse(jrt::box(-1))));
}

int32_t Streams::composed(int32_t x) {
	ref<Function> add = jrt::fn<Function>([=](ref<Object> v_in) -> ref<Object> {
	ref<Integer> v = jrt::cast<Integer>(v_in);
	return jrt::box(jrt::unbox<int32_t>(v) + 7);
});
	ref<Function> twice = jrt::fn<Function>([=](ref<Object> v_in) -> ref<Object> {
	ref<Integer> v = jrt::cast<Integer>(v_in);
	return jrt::box(jrt::unbox<int32_t>(v) * 2);
});
	ref<Predicate> big = jrt::fn<Predicate>([=](ref<Object> v_in) -> bool {
	ref<Integer> v = jrt::cast<Integer>(v_in);
	return jrt::unbox<int32_t>(v) > 50;
});
	int32_t r = jrt::unbox<int32_t>(jrt::cast<Integer>(jrt::nn(twice)->apply(jrt::cast<Integer>(jrt::nn(add)->apply(jrt::box(x % 1000))))));
	return (jrt::nn(big)->test(jrt::box(r)) ? r : -r);
}

int32_t Streams::counter(int32_t n) {
	ref<Array<int32_t>> box = jrt::array_of<int32_t>({0});
	ref<Runnable> inc = jrt::fn<Runnable>([=]() -> void { (*jrt::nn(box))[0] += 2; });
	for (int32_t i = 0; i < n % 50; i++) {
		jrt::nn(inc)->run();
	}
	ref<Supplier> get = jrt::fn<Supplier>([=]() -> ref<Object> { return jrt::box((*jrt::nn(box))[0]); });
	return jrt::unbox<int32_t>(jrt::cast<Integer>(jrt::nn(get)->get()));
}

ref<List> Streams::people() {
	ref<List> ps = jrt::make<ArrayList>();
	jrt::nn(ps)->add(std::make_shared<Streams_Person>(jrt::lit(u"Ada"), 36));
	jrt::nn(ps)->add(std::make_shared<Streams_Person>(jrt::lit(u"Linus"), 21));
	jrt::nn(ps)->add(std::make_shared<Streams_Person>(jrt::lit(u"Grace"), 85));
	jrt::nn(ps)->add(std::make_shared<Streams_Person>(jrt::lit(u"Alan"), 41));
	jrt::nn(ps)->add(std::make_shared<Streams_Person>(jrt::lit(u"Barbara"), 36));
	return ps;
}

void Streams::main(ref<Array<ref<String>>> args) {
	int32_t j2xo1{};
	int64_t j2xo2{};
	ref<String> j2xo3{};
	jrt::nn(jrt::System::out_())->println((j2xo1 = Streams::sumOfSquares(10), j2xo2 = Streams::countEven(11), j2xo3 = Streams::joined(5), jrt::concat(j2xo1, jrt::lit(u" "), j2xo2, jrt::lit(u" "), j2xo3)));
	ref<List> ps = Streams::people();
	ref<Stream> j2xo4{};
	ref<Stream> j2xo5{};
	ref<Collector> j2xo6{};
	ref<List> names = (j2xo5 = jrt::nn(jrt::nn((j2xo4 = jrt::nn(ps)->stream(), jrt::nn(j2xo4)->filter(jrt::fn<Predicate>([=](ref<Object> p_in) -> bool {
	ref<Streams_Person> p = jrt::cast<Streams_Person>(p_in);
	return jrt::nn(p)->age() > 30;
}))))->map(jrt::fn<Function>([=](ref<Object> a0) -> ref<Object> { return jrt::nn(jrt::cast<Streams_Person>(a0))->name(); })))->sorted(), j2xo6 = jrt::Collectors::toList(), jrt::cast<List>(jrt::nn(j2xo5)->collect(j2xo6)));
	jrt::nn(jrt::System::out_())->println(names);
	jrt::nn(ps)->sort(jrt::nn(jrt::Comparator::comparingInt(jrt::fn<ToIntFunction>([=](ref<Object> a0) -> int32_t { return jrt::nn(jrt::cast<Streams_Person>(a0))->age(); })))->thenComparing(jrt::fn<Function>([=](ref<Object> a0) -> ref<Object> { return jrt::nn(jrt::cast<Streams_Person>(a0))->name(); })));
	for (ref<Streams_Person> p : jrt::each<Streams_Person>(ps)) {
		ref<String> j2xo7{};
		int32_t j2xo8{};
		jrt::nn(jrt::System::out_())->print((j2xo7 = jrt::nn(p)->name(), j2xo8 = jrt::nn(p)->age(), jrt::concat(j2xo7, jrt::lit(u":"), j2xo8, jrt::lit(u" "))));
	}
	jrt::nn(jrt::System::out_())->println();
	ref<Stream> j2xo9{};
	ref<Collector> j2xo10{};
	ref<Map> byAge = (j2xo9 = jrt::nn(ps)->stream(), j2xo10 = jrt::Collectors::groupingBy(jrt::fn<Function>([=](ref<Object> a0) -> ref<Object> { return jrt::box(jrt::nn(jrt::cast<Streams_Person>(a0))->age()); })), jrt::cast<Map>(jrt::nn(j2xo9)->collect(j2xo10)));
	int32_t j2xo11{};
	bool j2xo12{};
	int32_t j2xo13{};
	jrt::nn(jrt::System::out_())->println((j2xo11 = jrt::nn(jrt::cast<List>(jrt::nn(byAge)->get(jrt::box(36))))->size(), j2xo12 = jrt::nn(byAge)->containsKey(jrt::box(21)), j2xo13 = jrt::nn(byAge)->size(), jrt::concat(j2xo11, jrt::lit(u" "), j2xo12, jrt::lit(u" "), j2xo13)));
	ref<Stream> j2xo14{};
	ref<Comparator> j2xo15{};
	ref<Optional> oldest = (j2xo14 = jrt::nn(ps)->stream(), j2xo15 = jrt::Comparator::comparingInt(jrt::fn<ToIntFunction>([=](ref<Object> a0) -> int32_t { return jrt::nn(jrt::cast<Streams_Person>(a0))->age(); })), jrt::nn(j2xo14)->max(j2xo15));
	jrt::nn(jrt::System::out_())->println(jrt::cast<String>(jrt::nn(jrt::nn(oldest)->map(jrt::fn<Function>([=](ref<Object> a0) -> ref<Object> { return jrt::nn(jrt::cast<Streams_Person>(a0))->name(); })))->orElse(jrt::lit(u"nobody"))));
	int32_t total = jrt::nn(jrt::nn(jrt::nn(ps)->stream())->mapToInt(jrt::fn<ToIntFunction>([=](ref<Object> a0) -> int32_t { return jrt::nn(jrt::cast<Streams_Person>(a0))->age(); })))->sum();
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"total age "), total, jrt::lit(u" avg "), jrt::nn(jrt::nn(jrt::nn(jrt::nn(ps)->stream())->mapToInt(jrt::fn<ToIntFunction>([=](ref<Object> a0) -> int32_t { return jrt::nn(jrt::cast<Streams_Person>(a0))->age(); })))->average())->getAsDouble()));
	jrt::nn(jrt::System::out_())->println(Streams::upperWords(jrt::lit(u"the quick  brown fox")));
	int32_t j2xo16{};
	int32_t j2xo17{};
	int32_t j2xo18{};
	jrt::nn(jrt::System::out_())->println((j2xo16 = Streams::firstOver(1000), j2xo17 = Streams::composed(30), j2xo18 = Streams::counter(7), jrt::concat(j2xo16, jrt::lit(u" "), j2xo17, jrt::lit(u" "), j2xo18)));
	ref<List> nums = jrt::Arrays::asList(jrt::box(5), jrt::box(3), jrt::box(8), jrt::box(1), jrt::box(9), jrt::box(2));
	ref<Integer> j2xo19{};
	bool j2xo20{};
	bool j2xo21{};
	jrt::nn(jrt::System::out_())->println((j2xo19 = jrt::cast<Integer>(jrt::nn(jrt::nn(nums)->stream())->reduce(jrt::box(0), jrt::fn<BinaryOperator>([=](ref<Object> a0, ref<Object> a1) -> ref<Object> { return jrt::box(jrt::Integer_::sum(jrt::unbox<int32_t>(a0), jrt::unbox<int32_t>(a1))); }))), j2xo20 = jrt::nn(jrt::nn(nums)->stream())->anyMatch(jrt::fn<Predicate>([=](ref<Object> v_in) -> bool {
	ref<Integer> v = jrt::cast<Integer>(v_in);
	return jrt::unbox<int32_t>(v) > 8;
})), j2xo21 = jrt::nn(jrt::nn(nums)->stream())->allMatch(jrt::fn<Predicate>([=](ref<Object> v_in) -> bool {
	ref<Integer> v = jrt::cast<Integer>(v_in);
	return jrt::unbox<int32_t>(v) > 0;
})), jrt::concat(j2xo19, jrt::lit(u" "), j2xo20, jrt::lit(u" "), j2xo21)));
	auto sb = jrt::make<StringBuilder>();
	jrt::nn(nums)->forEach(jrt::fn<Consumer>([=](ref<Object> v_in) -> void {
	ref<Integer> v = jrt::cast<Integer>(v_in);
	jrt::nn(jrt::nn(sb)->append(v))->append(u';');
}));
	jrt::nn(jrt::System::out_())->println(sb);
}

Streams_Person::Streams_Person(ref<String> name, int32_t age) {
	this->name_ = name;
	this->age_ = age;
}

ref<String> Streams_Person::name() {
	return this->name_;
}

int32_t Streams_Person::age() {
	return this->age_;
}

ref<String> Streams_Person::toString() {
	return jrt::concat(jrt::lit(u"Person["), jrt::lit(u"name="), this->name_, jrt::lit(u", age="), this->age_, jrt::lit(u"]"));
}

int32_t Streams_Person::hashCode() {
	int32_t h = 0;
	h = 31 * h + jrt::Objects::hashCode(this->name_);
	h = 31 * h + jrt::Integer_::hashCode(this->age_);
	return h;
}

bool Streams_Person::equals(ref<Object> o) {
	if (!(jrt::isa<Streams_Person>(o))) {
		return false;
	}
	auto that = jrt::cast<Streams_Person>(o);
	return jrt::Objects::equals(this->name_, jrt::nn(that)->name_) && this->age_ == jrt::nn(that)->age_;
}


int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "Streams") == 0) return jrt::run_main(&Streams::main, argc, argv);
	return jrt::run_main(&Streams::main, argc, argv);
}
