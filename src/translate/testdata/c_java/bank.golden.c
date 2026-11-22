/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from
   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */
#include "jrt.h"

typedef struct Bank Bank;
typedef struct Bank_InsufficientFunds Bank_InsufficientFunds;
typedef struct Bank_Account Bank_Account;
typedef struct Bank_Checking Bank_Checking;
typedef struct Bank_Savings Bank_Savings;
extern const jclass Bank_class;
extern const jclass Bank_InsufficientFunds_class;
extern const jclass Bank_Account_class;
extern const jclass Bank_Checking_class;
extern const jclass Bank_Savings_class;

struct Bank {
	jobject hdr_;
};
struct Bank_vt {
	jobject_vt o_;
};

struct Bank_InsufficientFunds {
	jrt_Throwable super_;
	int32_t shortBy;
};
struct Bank_InsufficientFunds_vt {
	jobject_vt o_;
	jstring* (*getMessage)(jobject*);
};

struct Bank_Account {
	jobject hdr_;
	jstring* id;
	int64_t balance;
	jobject* log;
};
struct Bank_Account_vt {
	jobject_vt o_;
	jstring* (*kind)(jobject*);
	int64_t (*fee)(jobject*, int64_t);
	void (*withdraw)(jobject*, int64_t);
	void (*deposit)(jobject*, int64_t);
};
static int32_t Bank_Account_opened = 0;

struct Bank_Checking {
	Bank_Account super_;
};
struct Bank_Checking_vt {
	jobject_vt o_;
	jstring* (*kind)(jobject*);
	int64_t (*fee)(jobject*, int64_t);
	void (*withdraw)(jobject*, int64_t);
	void (*deposit)(jobject*, int64_t);
};

struct Bank_Savings {
	Bank_Account super_;
	int32_t rate;
};
struct Bank_Savings_vt {
	jobject_vt o_;
	jstring* (*kind)(jobject*);
	int64_t (*fee)(jobject*, int64_t);
	void (*withdraw)(jobject*, int64_t);
	void (*deposit)(jobject*, int64_t);
	void (*accrue)(jobject*);
};

static jstring* lit_1;
static jstring* lit_2;
static jstring* lit_3;
static jstring* lit_4;
static jstring* lit_5;
static jstring* lit_6;
static jstring* lit_7;
static jstring* lit_8;
static jstring* lit_9;
static jstring* lit_10;
static jstring* lit_11;
static jstring* lit_12;
static jstring* lit_13;
static jstring* lit_14;
static jstring* lit_15;
static jstring* lit_16;
static jstring* lit_17;
static jstring* lit_18;
static jstring* lit_19;
static jstring* lit_20;
static jstring* lit_21;
static jstring* lit_22;
static jstring* lit_23;

static void Bank_init(Bank* self);
static Bank* Bank_new(void);
static int64_t Bank_simulate(int32_t);
static jstring* Bank_wordFreq(jstring*);
static jstring* Bank_ordered(int32_t);
static int32_t Bank_hashOrder(int32_t);
static void Bank_main(jarray*);
static void Bank_finit(Bank* self);
static void Bank_InsufficientFunds_init(Bank_InsufficientFunds* self, jstring* msg, int32_t shortBy);
static Bank_InsufficientFunds* Bank_InsufficientFunds_new(jstring* msg, int32_t shortBy);
static void Bank_InsufficientFunds_finit(Bank_InsufficientFunds* self);
static void Bank_Account_init(Bank_Account* self, jstring* id, int64_t initial);
static Bank_Account* Bank_Account_new(jstring* id, int64_t initial);
static void Bank_Account_withdraw(jobject*, int64_t);
static void Bank_Account_deposit(jobject*, int64_t);
static jstring* Bank_Account_toString(jobject*);
static void Bank_Account_finit(Bank_Account* self);
static void Bank_Checking_init(Bank_Checking* self, jstring* id, int64_t initial);
static Bank_Checking* Bank_Checking_new(jstring* id, int64_t initial);
static jstring* Bank_Checking_kind(jobject*);
static int64_t Bank_Checking_fee(jobject*, int64_t);
static void Bank_Checking_finit(Bank_Checking* self);
static void Bank_Savings_init(Bank_Savings* self, jstring* id, int64_t initial, int32_t rate);
static Bank_Savings* Bank_Savings_new(jstring* id, int64_t initial, int32_t rate);
static jstring* Bank_Savings_kind(jobject*);
static int64_t Bank_Savings_fee(jobject*, int64_t);
static void Bank_Savings_accrue(jobject*);
static void Bank_Savings_finit(Bank_Savings* self);
static inline jstring* Bank_InsufficientFunds_getMessage_v(jobject* o) { return ((const struct Bank_InsufficientFunds_vt*)jrt_vt(o))->getMessage(o); }
static inline jstring* Bank_Account_kind_v(jobject* o) { return ((const struct Bank_Account_vt*)jrt_vt(o))->kind(o); }
static inline int64_t Bank_Account_fee_v(jobject* o, int64_t a0) { return ((const struct Bank_Account_vt*)jrt_vt(o))->fee(o, a0); }
static inline void Bank_Account_withdraw_v(jobject* o, int64_t a0) { return ((const struct Bank_Account_vt*)jrt_vt(o))->withdraw(o, a0); }
static inline void Bank_Account_deposit_v(jobject* o, int64_t a0) { return ((const struct Bank_Account_vt*)jrt_vt(o))->deposit(o, a0); }
static inline void Bank_Savings_accrue_v(jobject* o) { return ((const struct Bank_Savings_vt*)jrt_vt(o))->accrue(o); }
struct Lambda1 {
	jobject hdr_;
};
static jobject* Lambda1_fn(jobject* self_, jobject* a0, jobject* a1) {
	struct Lambda1* self = (struct Lambda1*)self_;
	(void)self;
	return jrt_box_I(jrt_Integer_sum_II(jrt_unbox_I((jobject*)a0), jrt_unbox_I((jobject*)a1)));
}
static const jrt_BiFunction_vt Lambda1_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, Lambda1_fn};
static const jitab Lambda1_itabs[] = {{&jrt_BiFunction_class, &Lambda1_vt}};
static const jclass Lambda1_class = {"Bank$$Lambda", &jrt_Object_class, sizeof(struct Lambda1), &jrt_Object_vt, JRT_ITABS(Lambda1_itabs), 0};
static jobject* Lambda1_new(void) {
	struct Lambda1* o = (struct Lambda1*)jrt_alloc(&Lambda1_class);
	return (jobject*)o;
}

static void Bank_init(Bank* self) {
	Bank_finit(self);
}

static Bank* Bank_new(void) {
	Bank* o = (Bank*)jrt_alloc(&Bank_class);
	Bank_init(o);
	return o;
}

static int64_t Bank_simulate(int32_t seed) {
	volatile int64_t x = (int64_t)(seed);
	Bank_Checking* volatile c = Bank_Checking_new(jrt_concat(2, jrt_lit(&lit_1, u"c", 1), jrt_str_I(seed)), (int64_t)(500));
	Bank_Savings* volatile s = Bank_Savings_new(jrt_concat(2, jrt_lit(&lit_2, u"s", 1), jrt_str_I(seed)), (int64_t)(2000), 15);
	volatile int64_t failures = (int64_t)(0);
	{
		volatile int32_t i = 0;
		for (; i < 25; i++) {
			x = (x * 6364136223846793005LL + 1442695040888963407LL);
			volatile int64_t amt = (jrt_ushr_J(x, 33)) % 700;
			{
				jrt_try f1;
				jrt_push(&f1);
				if (setjmp(f1.jb) == 0) {
					{
						jrt_try t2;
						jrt_push(&t2);
						if (setjmp(t2.jb) == 0) {
							if ((x & 1) == 0) {
								Bank_Account_withdraw_v((jobject*)c, amt);
							} else {
								Bank_Account_withdraw_v((jobject*)s, amt);
							}
							jrt_pop(&t2);
						} else {
							jrt_pop(&t2);
							if (jrt_instanceof(t2.ex, &Bank_InsufficientFunds_class)) {
								Bank_InsufficientFunds* e = (Bank_InsufficientFunds*)t2.ex;
								failures += ((Bank_InsufficientFunds*)jrt_nn(e))->shortBy;
							} else {
								jrt_throw(t2.ex);
							}
						}
					}
					jrt_pop(&f1);
				} else {
					jrt_pop(&f1);
					Bank_Savings_accrue_v((jobject*)s);
					jrt_throw(f1.ex);
				}
				Bank_Savings_accrue_v((jobject*)s);
			}
			if (i % 5 == 0) {
				Bank_Account_deposit_v((jobject*)c, amt + 1);
			}
		}
	}
	return ((Bank_Account*)jrt_nn(c))->balance * 31 + ((Bank_Account*)jrt_nn(s))->balance + failures * 7;
}

static jstring* Bank_wordFreq(jstring* text) {
	jobject* counts = jrt_HashMap_new_();
	{
		jarray* j2x_a1 = jrt_String_split_Str(text, jrt_lit(&lit_3, u" ", 1));
		{
			int32_t j2x_i2 = 0;
			for (; j2x_i2 < JLEN(j2x_a1); j2x_i2++) {
				jstring* w = JA(jstring*, j2x_a1, j2x_i2);
				if (jrt_String_isEmpty_(w)) {
					continue;
				}
				jrt_Map_merge_OOO(counts, (jobject*)w, jrt_box_I(1), Lambda1_new());
			}
		}
	}
	jrt_StringBuilder* sb = jrt_StringBuilder_new_();
	{
		jobject* j2x_it3 = jrt_Set_iterator_(jrt_Map_entrySet_(counts));
		for (; jrt_Iterator_hasNext_(j2x_it3); ) {
			jobject* e = jrt_Iterator_next_(j2x_it3);
			jrt_StringBuilder* o1 = NULL;
			jobject* o2 = NULL;
			jrt_StringBuilder_append_C((o1 = jrt_StringBuilder_append_C(jrt_StringBuilder_append_Str(sb, (jstring*)jrt_cast((jobject*)jrt_MapEntry_getKey_(e), &jrt_String_class)), u'='), o2 = jrt_MapEntry_getValue_(e), jrt_StringBuilder_append_O(o1, o2)), u' ');
		}
	}
	return jrt_String_trim_(jrt_toString((jobject*)sb));
}

static jstring* Bank_ordered(int32_t n) {
	jobject* lm = jrt_LinkedHashMap_new_();
	jobject* tm = jrt_TreeMap_new_();
	{
		int32_t i = 0;
		for (; i < n % 10; i++) {
			int32_t k = (i * 7 + 3) % 11;
			(jstring*)jrt_cast((jobject*)jrt_Map_put_OO(lm, jrt_box_I(k), (jobject*)jrt_concat(2, jrt_lit(&lit_4, u"v", 1), jrt_str_I(i))), &jrt_String_class);
			(jstring*)jrt_cast((jobject*)jrt_Map_put_OO(tm, jrt_box_I(k), (jobject*)jrt_concat(2, jrt_lit(&lit_4, u"v", 1), jrt_str_I(i))), &jrt_String_class);
		}
	}
	jstring* o1 = NULL;
	jstring* o2 = NULL;
	return (o1 = jrt_str_O((jobject*)jrt_Map_keySet_(lm)), o2 = jrt_str_O((jobject*)jrt_Map_values_(tm)), jrt_concat(3, o1, jrt_lit(&lit_5, u"|", 1), o2));
}

static int32_t Bank_hashOrder(int32_t n) {
	jobject* m = jrt_HashMap_new_();
	{
		int32_t i = 0;
		for (; i < n % 40; i++) {
			jrt_Map_put_OO(m, (jobject*)jrt_concat(2, jrt_lit(&lit_6, u"key", 3), jrt_str_I((i * 13 % 17))), jrt_box_I(i));
		}
	}
	int32_t h = 0;
	{
		jobject* j2x_it4 = jrt_Set_iterator_(jrt_Map_keySet_(m));
		for (; jrt_Iterator_hasNext_(j2x_it4); ) {
			jstring* k = (jstring*)jrt_cast((jobject*)jrt_Iterator_next_(j2x_it4), &jrt_String_class);
			h = h * 31 + jrt_String_hashCode_(k);
		}
	}
	return h;
}

static void Bank_main(jarray* args) {
	Bank_Checking* volatile c = Bank_Checking_new(jrt_lit(&lit_7, u"A1", 2), (int64_t)(100));
	Bank_Savings* volatile s = Bank_Savings_new(jrt_lit(&lit_8, u"B2", 2), (int64_t)(5000), 20);
	jstring* volatile o1 = NULL;
	jstring* volatile o2 = NULL;
	jstring* volatile o3 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (o1 = jrt_str_O((jobject*)c), o2 = jrt_str_O((jobject*)s), o3 = jrt_str_I(Bank_Account_opened), jrt_concat(5, o1, jrt_lit(&lit_3, u" ", 1), o2, jrt_lit(&lit_9, u" opened=", 8), o3)));
	{
		jrt_try f4;
		jrt_push(&f4);
		if (setjmp(f4.jb) == 0) {
			{
				jrt_try t5;
				jrt_push(&t5);
				if (setjmp(t5.jb) == 0) {
					Bank_Account_withdraw_v((jobject*)c, (int64_t)(50));
					Bank_Account_withdraw_v((jobject*)c, (int64_t)(80));
					jrt_pop(&t5);
				} else {
					jrt_pop(&t5);
					if (jrt_instanceof(t5.ex, &Bank_InsufficientFunds_class)) {
						Bank_InsufficientFunds* e = (Bank_InsufficientFunds*)t5.ex;
						jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(4, jrt_lit(&lit_10, u"failed: ", 8), jrt_str_O((jobject*)jrt_Throwable_getMessage_((jobject*)e)), jrt_lit(&lit_11, u" by ", 4), jrt_str_I(((Bank_InsufficientFunds*)jrt_nn(e))->shortBy)));
					} else {
						jrt_throw(t5.ex);
					}
				}
			}
			jrt_pop(&f4);
		} else {
			jrt_pop(&f4);
			jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_12, u"after: ", 7), jrt_str_O((jobject*)c)));
			jrt_throw(f4.ex);
		}
		jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_12, u"after: ", 7), jrt_str_O((jobject*)c)));
	}
	{
		jrt_try t6;
		jrt_push(&t6);
		if (setjmp(t6.jb) == 0) {
			Bank_Account_deposit_v((jobject*)s, (int64_t)(-5));
			jrt_pop(&t6);
		} else {
			jrt_pop(&t6);
			if (jrt_instanceof(t6.ex, &jrt_IllegalArgumentException_class)) {
				jrt_Throwable* e = (jrt_Throwable*)t6.ex;
				jrt_PrintStream_println_Str(jrt_System_out, jrt_concat(2, jrt_lit(&lit_13, u"rejected ", 9), jrt_str_O((jobject*)jrt_Throwable_getMessage_((jobject*)e))));
			} else {
				jrt_throw(t6.ex);
			}
		}
	}
	Bank_Savings_accrue_v((jobject*)s);
	jstring* volatile o7 = NULL;
	jstring* volatile o8 = NULL;
	jstring* volatile o9 = NULL;
	jrt_PrintStream_println_Str(jrt_System_out, (o7 = jrt_str_O((jobject*)s), o8 = jrt_str_O((jobject*)((Bank_Account*)jrt_nn(c))->log), o9 = jrt_str_O((jobject*)((Bank_Account*)jrt_nn(s))->log), jrt_concat(5, o7, jrt_lit(&lit_3, u" ", 1), o8, jrt_lit(&lit_3, u" ", 1), o9)));
	volatile int64_t o10 = 0;
	volatile int64_t o11 = 0;
	jrt_PrintStream_println_Str(jrt_System_out, (o10 = Bank_simulate(42), o11 = Bank_simulate(7), jrt_concat(3, jrt_str_J(o10), jrt_lit(&lit_3, u" ", 1), jrt_str_J(o11))));
	jrt_PrintStream_println_Str(jrt_System_out, Bank_wordFreq(jrt_lit(&lit_14, u"to be or not to be that is the question to be", 45)));
	jstring* volatile o12 = NULL;
	volatile int32_t o13 = 0;
	jrt_PrintStream_println_Str(jrt_System_out, (o12 = Bank_ordered(9), o13 = Bank_hashOrder(33), jrt_concat(3, jrt_str_O((jobject*)o12), jrt_lit(&lit_3, u" ", 1), jrt_str_I(o13))));
}

static void Bank_finit(Bank* self) {
}

static void Bank_InsufficientFunds_init(Bank_InsufficientFunds* self, jstring* msg, int32_t shortBy) {
	jrt_Throwable_init((jobject*)self, msg);
	Bank_InsufficientFunds_finit(self);
	self->shortBy = shortBy;
}

static Bank_InsufficientFunds* Bank_InsufficientFunds_new(jstring* msg, int32_t shortBy) {
	Bank_InsufficientFunds* o = (Bank_InsufficientFunds*)jrt_alloc(&Bank_InsufficientFunds_class);
	Bank_InsufficientFunds_init(o, msg, shortBy);
	return o;
}

static void Bank_InsufficientFunds_finit(Bank_InsufficientFunds* self) {
}

static void Bank_Account_init(Bank_Account* self, jstring* id, int64_t initial) {
	Bank_Account_finit(self);
	self->id = id;
	self->balance = initial;
	Bank_Account_opened++;
	jrt_List_add_O(self->log, (jobject*)jrt_concat(2, jrt_lit(&lit_15, u"open:", 5), jrt_str_O((jobject*)Bank_Account_kind_v((jobject*)self))));
}

static Bank_Account* Bank_Account_new(jstring* id, int64_t initial) {
	Bank_Account* o = (Bank_Account*)jrt_alloc(&Bank_Account_class);
	Bank_Account_init(o, id, initial);
	return o;
}

static void Bank_Account_withdraw(jobject* self_, int64_t amount) {
	Bank_Account* self = (Bank_Account*)self_;
	int64_t total = amount + Bank_Account_fee_v((jobject*)self, amount);
	if (total > self->balance) {
		jrt_throw((jobject*)Bank_InsufficientFunds_new(jrt_concat(3, jrt_lit(&lit_16, u"account ", 8), jrt_str_O((jobject*)self->id), jrt_lit(&lit_17, u" short", 6)), (int32_t)((total - self->balance))));
	}
	self->balance -= total;
	jrt_List_add_O(self->log, (jobject*)jrt_concat(2, jrt_lit(&lit_18, u"w:", 2), jrt_str_J(amount)));
}

static void Bank_Account_deposit(jobject* self_, int64_t amount) {
	Bank_Account* self = (Bank_Account*)self_;
	if (amount <= 0) {
		jrt_throw((jobject*)jrt_new_throwable(&jrt_IllegalArgumentException_class, jrt_concat(2, jrt_lit(&lit_19, u"bad amount ", 11), jrt_str_J(amount)), NULL));
	}
	self->balance += amount;
	jrt_List_add_O(self->log, (jobject*)jrt_concat(2, jrt_lit(&lit_20, u"d:", 2), jrt_str_J(amount)));
}

static jstring* Bank_Account_toString(jobject* self_) {
	Bank_Account* self = (Bank_Account*)self_;
	return jrt_String_format(jrt_lit(&lit_21, u"%s[%s] %d", 9), 3, (jobject*)Bank_Account_kind_v((jobject*)self), (jobject*)self->id, jrt_box_J(self->balance));
}

static void Bank_Account_finit(Bank_Account* self) {
	self->log = jrt_ArrayList_new_();
}

static void Bank_Checking_init(Bank_Checking* self, jstring* id, int64_t initial) {
	Bank_Account_init((Bank_Account*)self, id, initial);
	Bank_Checking_finit(self);
}

static Bank_Checking* Bank_Checking_new(jstring* id, int64_t initial) {
	Bank_Checking* o = (Bank_Checking*)jrt_alloc(&Bank_Checking_class);
	Bank_Checking_init(o, id, initial);
	return o;
}

static jstring* Bank_Checking_kind(jobject* self_) {
	Bank_Checking* self = (Bank_Checking*)self_;
	return jrt_lit(&lit_22, u"checking", 8);
}

static int64_t Bank_Checking_fee(jobject* self_, int64_t amount) {
	Bank_Checking* self = (Bank_Checking*)self_;
	return (int64_t)((amount > 1000 ? 2 : 1));
}

static void Bank_Checking_finit(Bank_Checking* self) {
}

static void Bank_Savings_init(Bank_Savings* self, jstring* id, int64_t initial, int32_t rate) {
	Bank_Account_init((Bank_Account*)self, id, initial);
	Bank_Savings_finit(self);
	self->rate = rate;
}

static Bank_Savings* Bank_Savings_new(jstring* id, int64_t initial, int32_t rate) {
	Bank_Savings* o = (Bank_Savings*)jrt_alloc(&Bank_Savings_class);
	Bank_Savings_init(o, id, initial, rate);
	return o;
}

static jstring* Bank_Savings_kind(jobject* self_) {
	Bank_Savings* self = (Bank_Savings*)self_;
	return jrt_lit(&lit_23, u"savings", 7);
}

static int64_t Bank_Savings_fee(jobject* self_, int64_t amount) {
	Bank_Savings* self = (Bank_Savings*)self_;
	return amount / 100;
}

static void Bank_Savings_accrue(jobject* self_) {
	Bank_Savings* self = (Bank_Savings*)self_;
	((Bank_Account*)jrt_nn(self))->balance += ((Bank_Account*)jrt_nn(self))->balance * self->rate / 1000;
}

static void Bank_Savings_finit(Bank_Savings* self) {
}

static const struct Bank_vt Bank_vt_ = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}};
const jclass Bank_class = {"Bank", &jrt_Object_class, sizeof(Bank), &Bank_vt_, 0, 0, 0};

static const struct Bank_InsufficientFunds_vt Bank_InsufficientFunds_vt_ = {{jrt_Throwable_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))jrt_Throwable_getMessage};
const jclass Bank_InsufficientFunds_class = {"Bank.InsufficientFunds", &jrt_Exception_class, sizeof(Bank_InsufficientFunds), &Bank_InsufficientFunds_vt_, 0, 0, 0};

static const struct Bank_Account_vt Bank_Account_vt_ = {{Bank_Account_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))0, (int64_t (*)(jobject*, int64_t))0, (void (*)(jobject*, int64_t))Bank_Account_withdraw, (void (*)(jobject*, int64_t))Bank_Account_deposit};
const jclass Bank_Account_class = {"Bank.Account", &jrt_Object_class, sizeof(Bank_Account), &Bank_Account_vt_, 0, 0, 0};

static const struct Bank_Checking_vt Bank_Checking_vt_ = {{Bank_Account_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))Bank_Checking_kind, (int64_t (*)(jobject*, int64_t))Bank_Checking_fee, (void (*)(jobject*, int64_t))Bank_Account_withdraw, (void (*)(jobject*, int64_t))Bank_Account_deposit};
const jclass Bank_Checking_class = {"Bank.Checking", &Bank_Account_class, sizeof(Bank_Checking), &Bank_Checking_vt_, 0, 0, 0};

static const struct Bank_Savings_vt Bank_Savings_vt_ = {{Bank_Account_toString, jrt_Object_equals, jrt_Object_hashCode}, (jstring* (*)(jobject*))Bank_Savings_kind, (int64_t (*)(jobject*, int64_t))Bank_Savings_fee, (void (*)(jobject*, int64_t))Bank_Account_withdraw, (void (*)(jobject*, int64_t))Bank_Account_deposit, (void (*)(jobject*))Bank_Savings_accrue};
const jclass Bank_Savings_class = {"Bank.Savings", &Bank_Account_class, sizeof(Bank_Savings), &Bank_Savings_vt_, 0, 0, 0};


int main(int argc, char** argv) {
	const char* which = getenv("JRT_MAIN");
	if (which && strcmp(which, "Bank") == 0) return jrt_run_main(Bank_main, argc, argv);
	return jrt_run_main(Bank_main, argc, argv);
}
