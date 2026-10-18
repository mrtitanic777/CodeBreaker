#ifndef JARRAY_H
#define JARRAY_H
// java.lang array: a length-carrying, BOUNDS-CHECKED view over a heap buffer. Java arrays
// know their length and throw on out-of-range; here an out-of-range index returns a safe
// dummy slot instead of corrupting memory or wild-pointing (the 0xC.../IP-0 console crashes).
// Non-owning on copy (shallow) so a by-value copy never double-frees -- game arrays are
// long-lived. Implicit conversion to T* for GL / nio-buffer / System.arraycopy interop.

struct JObject;                                  // (int[])o unboxes through the ctor below (JavaRT.h)

template<class T>
class JArray {
	T* _buf;
	static T& _oob() { static T dummy; dummy = T(); return dummy; }   // safe slot for bad index
public:
	int length;                                  // Java: arr.length
	JArray() : _buf(0), length(0) {}
	explicit JArray(int n) : _buf(n > 0 ? new T[n]() : 0), length(n > 0 ? n : 0) {}
	JArray(T* buf, int n) : _buf(buf), length(n) {}
	JArray(const JObject& o);                    // unbox a boxed array; defined in JavaRT.h
	T& operator[](int i) { return (i >= 0 && i < length) ? _buf[i] : _oob(); }
	const T& operator[](int i) const { return (i >= 0 && i < length) ? _buf[i] : _oob(); }
	operator T*() { return _buf; }               // raw-pointer interop
	operator const T*() const { return _buf; }
	T* data() { return _buf; }
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
