// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the
// Java program's L2; it runs on jrt.hpp, the Java library in C++17.
// reference counting cannot free a cycle: these classes can form one, where a collector
// (Boehm) would be needed to reclaim it: Bank_Account
#include "jrt.hpp"

using jrt::ref;
using jrt::Array;
using jrt::ArrayList;
using jrt::BiFunction;
using jrt::Collection;
using jrt::Exception;
using jrt::HashMap;
using jrt::IllegalArgumentException;
using jrt::Integer;
using jrt::LinkedHashMap;
using jrt::List;
using jrt::Map;
using jrt::Object;
using jrt::PrintStream;
using jrt::Set;
using jrt::String;
using jrt::StringBuilder;
using jrt::TreeMap;

struct Bank;
struct Bank_InsufficientFunds;
struct Bank_Account;
struct Bank_Checking;
struct Bank_Savings;

struct Bank : virtual Object {
	const char16_t* jclass_() const override { return u"Bank"; }
	Bank();
	static int64_t simulate(int32_t seed);
	static ref<String> wordFreq(ref<String> text);
	static ref<String> ordered(int32_t n);
	static int32_t hashOrder(int32_t n);
	static void main(ref<Array<ref<String>>> args);
};

struct Bank_InsufficientFunds : Exception {
	const char16_t* jclass_() const override { return u"Bank.InsufficientFunds"; }
	int32_t shortBy{};
	void ctor_(ref<String> msg, int32_t shortBy);
	void finit_();
};

struct Bank_Account : virtual Object {
	const char16_t* jclass_() const override { return u"Bank.Account"; }
	static inline int32_t opened = 0;
	ref<String> id{};
	int64_t balance{};
	ref<List> log{};
	void ctor_(ref<String> id, int64_t initial);
	virtual ref<String> kind() = 0;
	virtual int64_t fee(int64_t amount) = 0;
	virtual void withdraw(int64_t amount);
	virtual void deposit(int64_t amount);
	virtual ref<String> toString();
	void finit_();
};

struct Bank_Checking : Bank_Account {
	const char16_t* jclass_() const override { return u"Bank.Checking"; }
	void ctor_(ref<String> id, int64_t initial);
	virtual ref<String> kind();
	virtual int64_t fee(int64_t amount);
	void finit_();
	using Bank_Account::fee;
	using Bank_Account::kind;
};

struct Bank_Savings : Bank_Account {
	const char16_t* jclass_() const override { return u"Bank.Savings"; }
	int32_t rate{};
	void ctor_(ref<String> id, int64_t initial, int32_t rate);
	virtual ref<String> kind();
	virtual int64_t fee(int64_t amount);
	virtual void accrue();
	void finit_();
	using Bank_Account::fee;
	using Bank_Account::kind;
};

Bank::Bank() {
}

int64_t Bank::simulate(int32_t seed) {
	int64_t x = (int64_t)seed;
	auto c = jrt::make<Bank_Checking>(jrt::concat(jrt::lit(u"c"), seed), (int64_t)(500));
	auto s = jrt::make<Bank_Savings>(jrt::concat(jrt::lit(u"s"), seed), (int64_t)(2000), 15);
	int64_t failures = (int64_t)(0);
	for (int32_t i = 0; i < 25; i++) {
		x = (x * 6364136223846793005LL + 1442695040888963407LL);
		int64_t amt = (jrt::ushr(x, 33)) % 700;
		try {
			try {
				if ((x & 1) == 0) {
					jrt::nn(c)->withdraw(amt);
				} else {
					jrt::nn(s)->withdraw(amt);
				}
			} catch (jrt::JavaThrow& j2x_t1) {
				if (auto e = jrt::as<Bank_InsufficientFunds>(j2x_t1.t)) {
					failures += e->shortBy;
				} else {
					throw;
				}
			}
		} catch (...) {
			jrt::nn(s)->accrue();
			throw;
		}
		{
			jrt::nn(s)->accrue();
		}
		if (i % 5 == 0) {
			jrt::nn(c)->deposit(amt + 1);
		}
	}
	return c->balance * 31 + s->balance + failures * 7;
}

ref<String> Bank::wordFreq(ref<String> text) {
	ref<Map> counts = jrt::make<HashMap>();
	for (ref<String> w : jrt::in(jrt::nn(text)->split(jrt::lit(u" ")))) {
		if (jrt::nn(w)->isEmpty()) {
			continue;
		}
		jrt::cast<Integer>(jrt::nn(counts)->merge(w, jrt::box(1), jrt::fn<BiFunction>([=](ref<Object> a0, ref<Object> a1) -> ref<Object> { return jrt::box(jrt::Integer_::sum(jrt::unbox<int32_t>(a0), jrt::unbox<int32_t>(a1))); })));
	}
	auto sb = jrt::make<StringBuilder>();
	for (auto [e_key, e_value] : jrt::entries<String, Integer>(counts)) {
		ref<StringBuilder> j2xo1{};
		jrt::nn((j2xo1 = jrt::nn(jrt::nn(sb)->append(e_key))->append(u'='), jrt::nn(j2xo1)->append(e_value)))->append(u' ');
	}
	return jrt::nn(jrt::nn(sb)->toString())->trim();
}

ref<String> Bank::ordered(int32_t n) {
	ref<Map> lm = jrt::make<LinkedHashMap>();
	ref<Map> tm = jrt::make<TreeMap>();
	for (int32_t i = 0; i < n % 10; i++) {
		int32_t k = (i * 7 + 3) % 11;
		jrt::cast<String>(jrt::nn(lm)->put(jrt::box(k), jrt::concat(jrt::lit(u"v"), i)));
		jrt::cast<String>(jrt::nn(tm)->put(jrt::box(k), jrt::concat(jrt::lit(u"v"), i)));
	}
	ref<Set> j2xo1{};
	ref<Collection> j2xo2{};
	return (j2xo1 = jrt::nn(lm)->keySet(), j2xo2 = jrt::nn(tm)->values(), jrt::concat(j2xo1, jrt::lit(u"|"), j2xo2));
}

int32_t Bank::hashOrder(int32_t n) {
	ref<Map> m = jrt::make<HashMap>();
	for (int32_t i = 0; i < n % 40; i++) {
		jrt::cast<Integer>(jrt::nn(m)->put(jrt::concat(jrt::lit(u"key"), (i * 13 % 17)), jrt::box(i)));
	}
	int32_t h = 0;
	for (ref<String> k : jrt::each<String>(jrt::nn(m)->keySet())) {
		h = h * 31 + jrt::nn(k)->hashCode();
	}
	return h;
}

void Bank::main(ref<Array<ref<String>>> args) {
	auto c = jrt::make<Bank_Checking>(jrt::lit(u"A1"), (int64_t)(100));
	auto s = jrt::make<Bank_Savings>(jrt::lit(u"B2"), (int64_t)(5000), 20);
	jrt::nn(jrt::System::out_())->println(jrt::concat(c, jrt::lit(u" "), s, jrt::lit(u" opened="), Bank_Account::opened));
	try {
		try {
			jrt::nn(c)->withdraw((int64_t)(50));
			jrt::nn(c)->withdraw((int64_t)(80));
		} catch (jrt::JavaThrow& j2x_t1) {
			if (auto e = jrt::as<Bank_InsufficientFunds>(j2x_t1.t)) {
				jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"failed: "), jrt::nn(e)->getMessage(), jrt::lit(u" by "), e->shortBy));
			} else {
				throw;
			}
		}
	} catch (...) {
		jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"after: "), c));
		throw;
	}
	{
		jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"after: "), c));
	}
	try {
		jrt::nn(s)->deposit((int64_t)(-5));
	} catch (jrt::JavaThrow& j2x_t2) {
		if (auto e = jrt::as<IllegalArgumentException>(j2x_t2.t)) {
			jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::lit(u"rejected "), jrt::nn(e)->getMessage()));
		} else {
			throw;
		}
	}
	jrt::nn(s)->accrue();
	jrt::nn(jrt::System::out_())->println(jrt::concat(s, jrt::lit(u" "), c->log, jrt::lit(u" "), s->log));
	int64_t j2xo3{};
	int64_t j2xo4{};
	jrt::nn(jrt::System::out_())->println((j2xo3 = Bank::simulate(42), j2xo4 = Bank::simulate(7), jrt::concat(j2xo3, jrt::lit(u" "), j2xo4)));
	jrt::nn(jrt::System::out_())->println(Bank::wordFreq(jrt::lit(u"to be or not to be that is the question to be")));
	ref<String> j2xo5{};
	int32_t j2xo6{};
	jrt::nn(jrt::System::out_())->println((j2xo5 = Bank::ordered(9), j2xo6 = Bank::hashOrder(33), jrt::concat(j2xo5, jrt::lit(u" "), j2xo6)));
}

void Bank_InsufficientFunds::ctor_(ref<String> msg, int32_t shortBy) {
	Exception::ctor_(msg);
	finit_();
	this->shortBy = shortBy;
}

void Bank_InsufficientFunds::finit_() {
}

void Bank_Account::ctor_(ref<String> id, int64_t initial) {
	finit_();
	this->id = id;
	this->balance = initial;
	Bank_Account::opened++;
	jrt::nn(this->log)->add(jrt::concat(jrt::lit(u"open:"), this->kind()));
}

void Bank_Account::withdraw(int64_t amount) {
	int64_t total = amount + this->fee(amount);
	if (total > this->balance) {
		jrt::raise(jrt::make<Bank_InsufficientFunds>(jrt::concat(jrt::lit(u"account "), this->id, jrt::lit(u" short")), (int32_t)((total - this->balance))));
	}
	this->balance -= total;
	jrt::nn(this->log)->add(jrt::concat(jrt::lit(u"w:"), amount));
}

void Bank_Account::deposit(int64_t amount) {
	if (amount <= 0) {
		jrt::raise(jrt::make<IllegalArgumentException>(jrt::concat(jrt::lit(u"bad amount "), amount)));
	}
	this->balance += amount;
	jrt::nn(this->log)->add(jrt::concat(jrt::lit(u"d:"), amount));
}

ref<String> Bank_Account::toString() {
	return jrt::String::format(jrt::lit(u"%s[%s] %d"), this->kind(), this->id, jrt::box(this->balance));
}

void Bank_Account::finit_() {
	this->log = jrt::make<ArrayList>();
}

void Bank_Checking::ctor_(ref<String> id, int64_t initial) {
	Bank_Account::ctor_(id, initial);
	finit_();
}

ref<String> Bank_Checking::kind() {
	return jrt::lit(u"checking");
}

int64_t Bank_Checking::fee(int64_t amount) {
	return (int64_t)((amount > 1000 ? 2 : 1));
}

void Bank_Checking::finit_() {
}

void Bank_Savings::ctor_(ref<String> id, int64_t initial, int32_t rate) {
	Bank_Account::ctor_(id, initial);
	finit_();
	this->rate = rate;
}

ref<String> Bank_Savings::kind() {
	return jrt::lit(u"savings");
}

int64_t Bank_Savings::fee(int64_t amount) {
	return amount / 100;
}

void Bank_Savings::accrue() {
	this->balance += this->balance * this->rate / 1000;
}

void Bank_Savings::finit_() {
}


int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "Bank") == 0) return jrt::run_main(&Bank::main, argc, argv);
	return jrt::run_main(&Bank::main, argc, argv);
}
