#ifndef JARRAY_H
#define JARRAY_H
// java.lang array: a length-carrying, BOUNDS-CHECKED, reference-counted handle to a heap block. Java arrays
// know their length and throw on out-of-range; here an out-of-range index returns a safe dummy slot instead
// of corrupting memory or wild-pointing (the 0xC.../IP-0 console crashes).
//
// Java's reference semantics: a copy of a JArray is another reference to the SAME elements (writes through
// one are seen through the other), and the elements live while any reference does. The block is one
// allocation, a small header (the reference count and the capacity) followed by the elements, and the
// handle points at the first element, so `a[i]` stays a bounds check plus a plain indexed load. The last
// reference to go frees the block (and destroys its elements, so an array of arrays releases its rows).
// J2X-BUGFIX array-never-freed: the handle used to be non-owning and nothing freed `new T[n]`, so every
// temporary array leaked (Indev's World::updateLight allocates an int[8192] per call: a 32-bit build ran
// out of address space generating a level).
//
// The count is atomic (the game's threads hand arrays to each other). Arrays only hold scalars, object
// pointers, strings or other arrays, never a counted reference back to themselves, so counts cannot cycle.
// A raw pointer taken from an array (data(), the T* conversion) is valid while some JArray still refers to
// the block, as Java's GC would keep it; keep a JArray, not the pointer, to hold an array.
// Every block has one value-initialized element past `length`, so a char array is also a NUL-terminated
// C string and `new T[0]` is a real (non-null) array. C++03 (the Xbox 360 compiler); moves on C++11.
#include <cstddef>      // std::size_t
#include <new>          // placement new, ::operator new

#if defined(_XBOX)
#include <xtl.h>
#define J2X_ARRAY_REF(c)    InterlockedIncrement(c)
#define J2X_ARRAY_UNREF(c)  InterlockedDecrementRelease(c)
#define J2X_ARRAY_ACQUIRE() __lwsync()
#elif defined(_MSC_VER)
#include <intrin.h>
#define J2X_ARRAY_REF(c)    _InterlockedIncrement(c)
#define J2X_ARRAY_UNREF(c)  _InterlockedDecrement(c)      // a full barrier
#define J2X_ARRAY_ACQUIRE() ((void)0)
#else
#define J2X_ARRAY_REF(c)    __atomic_add_fetch(c, 1, __ATOMIC_RELAXED)
#define J2X_ARRAY_UNREF(c)  __atomic_sub_fetch(c, 1, __ATOMIC_ACQ_REL)
#define J2X_ARRAY_ACQUIRE() ((void)0)
#endif
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define J2X_ARRAY_MOVES 1
#endif

struct JObject;                                  // (int[])o unboxes through the ctor below (JavaRT.h)

// The block's header. Its size is a multiple of 8, so the elements after it are aligned for any Java type.
struct JArrayBlock {
	volatile long refs;
	int cap;                                     // elements constructed: length + 1
};

template<class T>
class JArray {
	T* _buf;                                     // first element of a block, or 0 for Java `null`
	static T& _oob() { static T dummy; dummy = T(); return dummy; }   // safe slot for bad index
	static JArrayBlock* _block(T* buf) { return (JArrayBlock*)buf - 1; }
	static T* _alloc(int n) {                    // a block of n value-initialized elements, plus the spare one
		const int cap = n + 1;
		JArrayBlock* b = (JArrayBlock*)::operator new(sizeof(JArrayBlock) + sizeof(T) * (std::size_t)cap);
		b->refs = 1;
		b->cap = cap;
		T* buf = (T*)(b + 1);
		for (int i = 0; i < cap; ++i) new (buf + i) T();
		return buf;
	}
	static void _ref(T* buf) { if (buf) J2X_ARRAY_REF(&_block(buf)->refs); }
	static void _unref(T* buf) {
		if (!buf) return;
		JArrayBlock* b = _block(buf);
		if (J2X_ARRAY_UNREF(&b->refs) != 0) return;
		J2X_ARRAY_ACQUIRE();
		for (int i = b->cap; i-- > 0; ) buf[i].~T();
		::operator delete(b);
	}
public:
	int length;                                  // Java: arr.length
	JArray() : _buf(0), length(0) {}             // Java `null`
	// `new T[n]`. J2X-BUGFIX empty-array-not-null: n == 0 allocates too, so `arr != null` holds for it
	// (NibbleArray.isValid() of a 0-length array was false; found by the behavioural judge).
	explicit JArray(int n) : _buf(n >= 0 ? _alloc(n) : 0), length(n > 0 ? n : 0) {}
	// A new array holding a copy of n elements at buf (a null buf is Java `null`).
	JArray(const T* buf, int n) : _buf(0), length(0) {
		if (!buf) return;
		length = n > 0 ? n : 0;
		_buf = _alloc(length);
		for (int i = 0; i < length; ++i) _buf[i] = buf[i];
	}
	JArray(const JObject& o);                    // unbox a boxed array; defined in JavaRT.h
	// Java's array covariance: a DragonPart[] is an Entity[] (Beta 1.9's EntityDragon returns its parts so). An array of the base
	// pointers, each converted (a base reached through multiple inheritance sits at its own address); a null array stays null.
	// Written through, the converted array does not reach the original -- the game only reads these.
	template<class U> JArray(const JArray<U*>& o) : _buf(0), length(0) {
		U* const* src = o;
		if (!src) return;
		length = o.length;
		_buf = _alloc(length);
		for (int i = 0; i < length; ++i) _buf[i] = src[i];
	}
	JArray(const JArray& o) : _buf(o._buf), length(o.length) { _ref(_buf); }
	JArray& operator=(const JArray& o) {
		_ref(o._buf);                            // before the release: `a = a` and `a = a.row` stay alive
		T* old = _buf;
		_buf = o._buf;
		length = o.length;
		_unref(old);
		return *this;
	}
#ifdef J2X_ARRAY_MOVES
	JArray(JArray&& o) noexcept : _buf(o._buf), length(o.length) { o._buf = 0; o.length = 0; }
	JArray& operator=(JArray&& o) noexcept {
		if (this != &o) {
			T* old = _buf;
			_buf = o._buf;
			length = o.length;
			o._buf = 0;
			o.length = 0;
			_unref(old);
		}
		return *this;
	}
#endif
	~JArray() { _unref(_buf); }
	T& operator[](int i) { return (i >= 0 && i < length) ? _buf[i] : _oob(); }
	const T& operator[](int i) const { return (i >= 0 && i < length) ? _buf[i] : _oob(); }
	operator T*() { return _buf; }               // raw-pointer interop
	operator const T*() const { return _buf; }
	T* data() { return _buf; }
	const T* data() const { return _buf; }
};

// Multi-dimensional `new T[a][b]` / `new T[a][b][c]` -> nested JArray construction. C++ has no
// jagged-new in one expression; these build the rectangular nesting (each inner row its own JArray).
template<class T> JArray<JArray<T> > j2x_newarr2(int a, int b) {
	JArray<JArray<T> > r(a);
	for (int i = 0; i < a; ++i) r[i] = JArray<T>(b);
	return r;
}
template<class T> JArray<JArray<JArray<T> > > j2x_newarr3(int a, int b, int c) {
	JArray<JArray<JArray<T> > > r(a);
	for (int i = 0; i < a; ++i) {
		r[i] = JArray<JArray<T> >(b);
		for (int j = 0; j < b; ++j) r[i][j] = JArray<T>(c);
	}
	return r;
}
#endif // JARRAY_H
