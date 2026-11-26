// dotnet/decompiler/typesystem/type_arena.hpp -- where the type system objects made on the fly live: the types made from other types
// (arrays, pointers, references, generic instances, modified and nullability-annotated types, tuples, function pointers) and the members
// seen through a generic instance (specialized members). One object per distinct key, kept for as long as the compilation that made it.
// Upstream allocates such an object wherever it needs one and lets the garbage collector have it back; here an object made twice is the
// same object, so memory is bounded by the number of distinct ones.
//
//     const IType* a = make_array_type(compilation, int32, 1);      // (types.hpp: the factories)
//     a == make_array_type(compilation, int32, 1);                    // true: the same object
//
// The arena of an object made only of special types (dynamic*, an unknown's array ...) is the global one. Making an object may make
// others (a specialized method composes substitutions), so the arena makes it outside its lock and keeps the first one made.
#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace cb::dotnet::decompiler::typesystem {

// What makes a made object the object it is: its kind of construction, its parts and its numbers.
struct TypeKey {
    std::uint8_t construction = 0;   // which factory
    std::vector<const void*> parts;  // the component objects
    std::vector<std::int64_t> numbers;
    std::vector<std::string> names;  // a tuple's element names
    friend bool operator==(const TypeKey&, const TypeKey&) = default;
};

struct TypeKeyHash {
    std::size_t operator()(const TypeKey& k) const noexcept;
};

class TypeArena {
public:
    TypeArena() = default;
    ~TypeArena();
    TypeArena(const TypeArena&) = delete;
    TypeArena& operator=(const TypeArena&) = delete;

    // The object with this key, made by `make` (returning a std::unique_ptr<T>) the first time it is asked for.
    template <class T, class Make>
    const T* intern(TypeKey key, Make&& make) {
        {
            const std::lock_guard lock(mutex_);
            if (const auto it = objects_.find(key); it != objects_.end()) return static_cast<const T*>(it->second);
        }
        std::unique_ptr<T> made = make();
        const std::lock_guard lock(mutex_);
        if (const auto it = objects_.find(key); it != objects_.end()) return static_cast<const T*>(it->second);   // made meanwhile
        const T* raw = made.get();
        owned_.push_back(std::shared_ptr<void>(made.release(), [](void* p) { delete static_cast<T*>(p); }));
        objects_.emplace(std::move(key), raw);
        return raw;
    }

    // Keep an object that is not interned for the arena's life.
    template <class T>
    T* keep(std::unique_ptr<T> object) {
        const std::lock_guard lock(mutex_);
        T* raw = object.get();
        owned_.push_back(std::shared_ptr<void>(object.release(), [](void* p) { delete static_cast<T*>(p); }));
        return raw;
    }

    std::size_t size() const {
        const std::lock_guard lock(mutex_);
        return owned_.size();
    }

    // The arena of objects that belong to no compilation.
    static TypeArena& global();

private:
    mutable std::mutex mutex_;
    std::unordered_map<TypeKey, const void*, TypeKeyHash> objects_;
    std::vector<std::shared_ptr<void>> owned_;
};

}  // namespace cb::dotnet::decompiler::typesystem
