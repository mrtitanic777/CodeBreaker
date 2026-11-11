// cb/core/arena.hpp -- an arena for large trees that are built once and freed together (L2 nodes, IR).
//
// make<T>(args...) constructs a T inside the arena and returns a raw pointer that stays valid until the arena is
// destroyed or reset(); nodes hold such pointers (or the NodeId of `Arena::index_of`) instead of shared_ptrs.
// Destructors of non-trivially-destructible objects run, in reverse order of creation, when the arena goes.
// An arena is NOT thread-safe: one tree, one thread, one arena.
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <new>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace cb {

class Arena {
public:
    explicit Arena(std::size_t block_size = 1 << 16) : block_size_(block_size) {}
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;
    Arena(Arena&&) noexcept = default;
    Arena& operator=(Arena&&) noexcept = default;
    ~Arena() { reset(); }

    // Raw storage: `size` bytes aligned to `align` (a power of two), zero-initialised.
    void* allocate(std::size_t size, std::size_t align = alignof(std::max_align_t));

    template <class T, class... Args>
    T* make(Args&&... args) {
        void* p = allocate(sizeof(T), alignof(T));
        T* obj = new (p) T(std::forward<Args>(args)...);
        if constexpr (!std::is_trivially_destructible_v<T>) {
            finalizers_.push_back({obj, [](void* o) { static_cast<T*>(o)->~T(); }});
        }
        ++objects_;
        return obj;
    }

    // A copy of the text that lives as long as the arena (identifier and string tables).
    std::string_view intern(std::string_view s);

    // Destroy everything made so far; the memory is kept for reuse.
    void reset();

    std::size_t objects() const { return objects_; }
    std::size_t bytes_used() const { return used_; }

private:
    struct Finalizer {
        void* object;
        void (*destroy)(void*);
    };
    struct Block {
        std::unique_ptr<std::byte[]> data;
        std::size_t size = 0;
        std::size_t used = 0;
    };
    std::size_t block_size_;
    std::vector<Block> blocks_;
    std::size_t current_ = 0;   // index of the block being filled
    std::vector<Finalizer> finalizers_;
    std::size_t objects_ = 0;
    std::size_t used_ = 0;
};

}  // namespace cb
