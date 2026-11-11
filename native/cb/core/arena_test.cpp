#include "cb/core/arena.hpp"

#include <string>

#include "cb/core/error.hpp"

#include "cb/core/testing.hpp"

using namespace cb;

namespace {
struct Node {
    int value = 0;
    Node* next = nullptr;
};
struct Counted {
    explicit Counted(int* live) : live_(live) { ++*live_; }
    ~Counted() { --*live_; }
    int* live_;
};
}  // namespace

CB_TEST("core.arena.make_builds_a_linked_structure", [] {
    Arena arena(256);   // small blocks force several of them
    Node* head = nullptr;
    for (int i = 0; i < 1000; ++i) {
        Node* n = arena.make<Node>();
        n->value = i;
        n->next = head;
        head = n;
    }
    int sum = 0, count = 0;
    for (Node* n = head; n != nullptr; n = n->next) {
        sum += n->value;
        ++count;
    }
    CB_CHECK_EQ(count, 1000);
    CB_CHECK_EQ(sum, 499500);
    CB_CHECK_EQ(arena.objects(), 1000u);
});

CB_TEST("core.arena.alignment_and_zeroing", [] {
    Arena arena;
    (void)arena.allocate(1, 1);
    void* p = arena.allocate(64, 64);
    CB_CHECK_EQ(reinterpret_cast<std::uintptr_t>(p) % 64, 0u);
    const auto* bytes = static_cast<const unsigned char*>(p);
    for (int i = 0; i < 64; ++i) CB_CHECK_EQ(bytes[i], 0);
    void* big = arena.allocate(1 << 20, 16);   // larger than a block: gets its own
    CB_CHECK(big != nullptr);
    CB_CHECK_THROWS_AS(InternalError, arena.allocate(8, 3));
});

CB_TEST("core.arena.destructors_run_once_in_reverse", [] {
    int live = 0;
    {
        Arena arena;
        for (int i = 0; i < 10; ++i) arena.make<Counted>(&live);
        CB_CHECK_EQ(live, 10);
        arena.reset();
        CB_CHECK_EQ(live, 0);
        arena.make<Counted>(&live);
        CB_CHECK_EQ(live, 1);
    }
    CB_CHECK_EQ(live, 0);
});

CB_TEST("core.arena.intern_and_reuse_after_reset", [] {
    Arena arena;
    std::string_view a = arena.intern("hello");
    std::string_view b = arena.intern("");
    CB_CHECK_EQ(a, "hello");
    CB_CHECK_EQ(b, "");
    CB_CHECK_EQ(a.data()[5], '\0');   // NUL-terminated copy
    arena.reset();
    CB_CHECK_EQ(arena.bytes_used(), 0u);
    std::string_view c = arena.intern("again");
    CB_CHECK_EQ(c, "again");
});
