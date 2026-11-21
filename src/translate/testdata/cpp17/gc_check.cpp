// The cycle collector on its own: rings are freed, what is held stays, a method running on an unreferenced object is safe.
#define JRT_CYCLES
#include "jrt.hpp"
using namespace jrt;

static long alive = 0;

struct Node : virtual Object {
	gc::Tracked gc_tracked_{this};
	ref<Object> next;
	ref<ArrayList> list;
	ref<HashMap> map;
	ref<Array<ref<Object>>> arr;
	int value = 7;
	Node() { ++alive; }
	~Node() override { value = -1; --alive; }
	void trace_(gc::Tracer& t) override { t(next); t(list); t(map); t(arr); }
	void clear_() override { next = nullptr; list = nullptr; map = nullptr; arr = nullptr; }
	int run_after_letting_go(ref<Object>& holder);
};

static void churn(int n) {
	for (int i = 0; i < n; ++i) {
		auto a = std::make_shared<Node>();
		auto b = std::make_shared<Node>();
		a->next = b;
		b->next = a;
	}
}

int Node::run_after_letting_go(ref<Object>& holder) {
	holder = nullptr;   // only the ring holds this object now, and this running method
	churn(30000);
	return value;
}

static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { ++failures; std::printf("FAILED: %s\n", what); } }

static void through_containers(int n) {
	for (int i = 0; i < n; ++i) {
		auto a = std::make_shared<Node>();
		auto b = std::make_shared<Node>();
		auto c = std::make_shared<Node>();
		a->list = std::make_shared<ArrayList>();
		a->list->add(b);
		b->map = std::make_shared<HashMap>();
		b->map->put(str(u"k"), c);
		c->arr = new_array<ref<Object>>(1);
		(*c->arr)[0] = a;
	}
}

static void jmain(ref<Array<ref<String>>>) {
	// a held ring survives every collection
	auto kept = std::make_shared<Node>();
	auto other = std::make_shared<Node>();
	kept->next = other;
	other->next = kept;
	std::weak_ptr<Node> kept_other = other;
	other = nullptr;
	churn(200000);
	gc::collect();
	check(alive < 40, "rings are freed");
	check(!kept_other.expired() && kept_other.lock()->value == 7, "a ring held by a local stays");

	through_containers(20000);
	gc::collect();
	check(alive < 40, "rings through a list, a map and an array are freed");

	// held only by a static: stays
	static ref<Object> root;
	auto r = std::make_shared<Node>();
	r->next = std::make_shared<Node>();
	std::dynamic_pointer_cast<Node>(r->next)->next = r;
	root = r;
	std::weak_ptr<Node> rooted = r;
	r = nullptr;
	churn(20000);
	gc::collect();
	check(!rooted.expired(), "a ring held by a static stays");

	// the object a method is running on, referenced by nothing but its own ring
	Node* running = dynamic_cast<Node*>(root.get());
	check(running->run_after_letting_go(root) == 7, "the object a method runs on is not freed under it");
	running = nullptr;
	churn(10000);
	gc::collect();
	check(rooted.expired() || alive < 40, "that ring goes once the method has returned");
	check(gc::heap().freed > 400000, "the collector did the freeing");
	kept = nullptr;
	std::printf("alive %ld, freed %zu, failures %d\n", alive, gc::heap().freed, failures);
	std::fflush(stdout);
	if (failures) std::exit(1);
}

int main(int argc, char** argv) { return run_main(jmain, argc, argv); }
