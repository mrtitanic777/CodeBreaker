// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the
// Java program's L2; it runs on jrt.hpp, the Java library in C++17.
#include "jrt.hpp"

using jrt::ref;
using jrt::ArithmeticException;
using jrt::Array;
using jrt::Object;
using jrt::PrintStream;
using jrt::String;
using jrt::StringBuilder;

struct TextStats;

struct TextStats : virtual Object {
	const char16_t* jclass_() const override { return u"TextStats"; }
	static inline ref<String> POEM{};
	static inline bool inited_ = false;
	static void clinit_();
	TextStats();
	static int32_t vowels(ref<String> s);
	static ref<String> histogram(ref<String> s);
	static ref<String> reverseWords(ref<String> s);
	static ref<String> sortedDigits(int32_t v);
	static int32_t mix(int32_t a, int32_t b);
	static int64_t mixLong(int64_t a, int32_t s);
	static int32_t safeDiv(int32_t a, int32_t b);
	static int32_t saturate(double d);
	static int32_t firstPair(int32_t n);
	static bool isPalindrome(ref<String> s);
	static ref<String> caesar(ref<String> s, int32_t k);
	static void main(ref<Array<ref<String>>> args);
};

TextStats::TextStats() {
	clinit_();
}

int32_t TextStats::vowels(ref<String> s) {
	clinit_();
	int32_t n = 0;
	for (char16_t c : jrt::in(jrt::nn(jrt::nn(s)->toLowerCase())->toCharArray())) {
		switch (c) {
		case u'a':
		{
		}
		case u'e':
		{
		}
		case u'i':
		{
		}
		case u'o':
		{
		}
		case u'u':
		{
			n++;
			break;
		}
		default:
		{
			break;
		}
		}
	}
	return n;
}

ref<String> TextStats::histogram(ref<String> s) {
	clinit_();
	ref<Array<int32_t>> counts = jrt::new_array<int32_t>(26);
	for (int32_t i = 0; i < jrt::nn(s)->length(); i++) {
		char16_t c = jrt::Character::toLowerCase(jrt::nn(s)->charAt(i));
		if (c >= u'a' && c <= u'z') {
			(*jrt::nn(counts))[c - u'a']++;
		}
	}
	auto sb = jrt::make<StringBuilder>();
	for (int32_t i = 0; i < 26; i++) {
		if ((*jrt::nn(counts))[i] > 0) {
			jrt::nn(jrt::nn(sb)->append((char16_t)((u'a' + i))))->append((*jrt::nn(counts))[i]);
		}
	}
	return jrt::nn(sb)->toString();
}

ref<String> TextStats::reverseWords(ref<String> s) {
	clinit_();
	ref<Array<ref<String>>> parts = jrt::nn(jrt::nn(s)->trim())->split(jrt::lit(u" "));
	auto sb = jrt::make<StringBuilder>();
	for (int32_t i = jrt::nn(parts)->length - 1; i >= 0; i--) {
		if (jrt::nn((*jrt::nn(parts))[i])->isEmpty()) {
			continue;
		}
		if (jrt::nn(sb)->length() > 0) {
			jrt::nn(sb)->append(u' ');
		}
		jrt::nn(sb)->append(jrt::nn(jrt::make<StringBuilder>((*jrt::nn(parts))[i]))->reverse());
	}
	return jrt::nn(sb)->toString();
}

ref<String> TextStats::sortedDigits(int32_t v) {
	clinit_();
	ref<Array<char16_t>> cs = jrt::nn(jrt::Integer_::toString(v))->toCharArray();
	jrt::Arrays::sort(cs);
	return jrt::make<String>(cs);
}

int32_t TextStats::mix(int32_t a, int32_t b) {
	clinit_();
	int32_t h = a * -1640531535 + (b << 7) - (jrt::ushr(b, 3)) + (a >> 5);
	return h ^ (jrt::ushr(h, 16));
}

int64_t TextStats::mixLong(int64_t a, int32_t s) {
	clinit_();
	return (a << (s & 63)) ^ (jrt::ushr(a, ((s + 3) & 63))) ^ (a * 1000003LL);
}

int32_t TextStats::safeDiv(int32_t a, int32_t b) {
	clinit_();
	try {
		return jrt::div<int32_t>(a, b) + jrt::rem<int32_t>(a, b);
	} catch (jrt::JavaThrow& j2x_t1) {
		if (auto e = jrt::as<ArithmeticException>(j2x_t1.t)) {
			return jrt::Integer_::MIN_VALUE;
		} else {
			throw;
		}
	}
}

int32_t TextStats::saturate(double d) {
	clinit_();
	return jrt::d2i((d * 1000000000.0));
}

int32_t TextStats::firstPair(int32_t n) {
	clinit_();
	int32_t found = -1;
	for (int32_t i = 1; i < 50; i++) {
		{
			for (int32_t j = i + 1; j < 50; j++) {
				if (jrt::rem<int32_t>((i * j), (jrt::Math::abs(n % 97) + 3)) == 0 && i + j > 10) {
					found = i * 100 + j;
					goto outer_brk;
				}
				if (j > i + 20) {
					goto outer_cont;
				}
			}
		}
		outer_cont:;
	}
	outer_brk:;
	return found;
}

bool TextStats::isPalindrome(ref<String> s) {
	clinit_();
	int32_t i = 0;
	int32_t j = jrt::nn(s)->length() - 1;
	while (i < j) {
		while (i < j && !jrt::Character::isLetterOrDigit(jrt::nn(s)->charAt(i))) {
			i++;
		}
		while (i < j && !jrt::Character::isLetterOrDigit(jrt::nn(s)->charAt(j))) {
			j--;
		}
		if (jrt::Character::toLowerCase(jrt::nn(s)->charAt(i)) != jrt::Character::toLowerCase(jrt::nn(s)->charAt(j))) {
			return false;
		}
		i++;
		j--;
	}
	return true;
}

ref<String> TextStats::caesar(ref<String> s, int32_t k) {
	clinit_();
	auto sb = jrt::make<StringBuilder>();
	int32_t shift = jrt::Math::floorMod(k, 26);
	for (char16_t c : jrt::in(jrt::nn(s)->toCharArray())) {
		if (jrt::Character::isUpperCase(c)) {
			jrt::nn(sb)->append((char16_t)((u'A' + (c - u'A' + shift) % 26)));
		} else if (jrt::Character::isLowerCase(c)) {
			jrt::nn(sb)->append((char16_t)((u'a' + (c - u'a' + shift) % 26)));
		} else {
			jrt::nn(sb)->append(c);
		}
	}
	return jrt::nn(sb)->toString();
}

void TextStats::main(ref<Array<ref<String>>> args) {
	clinit_();
	ref<Array<ref<String>>> lines = jrt::nn(TextStats::POEM)->split(jrt::lit(u"\n"));
	jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::nn(lines)->length, jrt::lit(u" lines, "), TextStats::vowels(TextStats::POEM), jrt::lit(u" vowels")));
	for (ref<String> line : jrt::in(lines)) {
		jrt::nn(jrt::System::out_())->println(jrt::concat(jrt::nn(line)->length(), jrt::lit(u": "), TextStats::reverseWords(line)));
	}
	jrt::nn(jrt::System::out_())->println(TextStats::histogram(TextStats::POEM));
	ref<String> j2xo1{};
	int32_t j2xo2{};
	int64_t j2xo3{};
	jrt::nn(jrt::System::out_())->println((j2xo1 = TextStats::sortedDigits(907153), j2xo2 = TextStats::mix(12345, 678), j2xo3 = TextStats::mixLong(-987654321LL, 9), jrt::concat(j2xo1, jrt::lit(u" "), j2xo2, jrt::lit(u" "), j2xo3)));
	int32_t j2xo4{};
	int32_t j2xo5{};
	int32_t j2xo6{};
	jrt::nn(jrt::System::out_())->println((j2xo4 = TextStats::safeDiv(17, 5), j2xo5 = TextStats::safeDiv(3, 0), j2xo6 = TextStats::safeDiv(jrt::Integer_::MIN_VALUE, -1), jrt::concat(j2xo4, jrt::lit(u" "), j2xo5, jrt::lit(u" "), j2xo6)));
	int32_t j2xo7{};
	int32_t j2xo8{};
	int32_t j2xo9{};
	jrt::nn(jrt::System::out_())->println((j2xo7 = TextStats::saturate(3.5), j2xo8 = TextStats::saturate(-1.0e+300), j2xo9 = TextStats::saturate(jrt::Double_::NaN), jrt::concat(j2xo7, jrt::lit(u" "), j2xo8, jrt::lit(u" "), j2xo9)));
	int32_t j2xo10{};
	bool j2xo11{};
	ref<String> j2xo12{};
	jrt::nn(jrt::System::out_())->println((j2xo10 = TextStats::firstPair(12), j2xo11 = TextStats::isPalindrome(jrt::lit(u"A man, a plan, a canal: Panama")), j2xo12 = TextStats::caesar(jrt::lit(u"Hello, World"), 29), jrt::concat(j2xo10, jrt::lit(u" "), j2xo11, jrt::lit(u" "), j2xo12)));
	int32_t x = jrt::Integer_::MAX_VALUE;
	x++;
	int64_t big = jrt::Long_::MAX_VALUE;
	big += 2;
	jrt::nn(jrt::System::out_())->println(jrt::concat(x, jrt::lit(u" "), big, jrt::lit(u" "), (char16_t)(65), (int32_t)(u'z'), jrt::lit(u" "), (int32_t)(int8_t)(200), jrt::lit(u" "), (int32_t)(int16_t)(70000)));
	jrt::nn(jrt::System::out_())->println(jrt::concat(1.0 / 3, jrt::lit(u" "), 100.0, jrt::lit(u" "), 1.0e-05, jrt::lit(u" "), 12345678.9, jrt::lit(u" "), (float)(0.1), jrt::lit(u" "), -0.0));
}

void TextStats::clinit_() {
	if (inited_) return;
	inited_ = true;
	TextStats::POEM = jrt::lit(u"Shall I compare thee\nto a summer's day?\nThou art more lovely\nand more temperate.\n");
}


int main(int argc, char** argv) {
	const char* which = std::getenv("JRT_MAIN");
	if (which && std::strcmp(which, "TextStats") == 0) return jrt::run_main(&TextStats::main, argc, argv);
	return jrt::run_main(&TextStats::main, argc, argv);
}
