#ifndef JLIST_H
#define JLIST_H
#include <vector>
#include <algorithm>
#include <set>
#include "JIterator.h"
struct JObject;                                  // (List)o unboxes through the ctor below (JavaRT.h)

// java.util.List / ArrayList / LinkedList, held BY VALUE. Subclasses std::vector<T> so every
// existing STL use keeps working (iterators, operator[], size(), push_back, range loops, and
// passing where a std::vector& is expected); ADDS the Java List API the converter does not
// rewrite to STL: remove (by index AND by value), contains, indexOf, set, 2-arg add. This is
// the JavaString pattern for collections -- members beat fragile text rewrites, and are the
// only correct option when the receiver is a chained expression or the arg has side effects
// (e.g. list.remove(i--)).
template<class T>
class JList : public std::vector<T> {
public:
	typedef typename std::vector<T>::iterator _it;
	typedef typename std::vector<T>::const_iterator _cit;
	// Java null: `List x = null`, or a boxed null (Map.get of a missing key). Only an empty list can be
	// null, so anything pushed into it -- through add() or the inherited std::vector mutators -- makes it
	// non-null. `list == null` reads it (SoundPool's missing sound name went on to get() an empty list).
	bool _null;
	JList() : _null(false) {}
	JList(long long) : _null(true) {}                    // Java `List x = null;`
	JList(const JObject& o);                             // unbox a boxed List; defined in JavaRT.h
	bool isNull() const { return _null && this->empty(); }
	// element-cast copy: `return map.values();` builds a JList<void*> from a JList<JObject> (the raw
	// Collection return type erases the element), and `new ArrayList(otherTypedList)` copies across
	// element types. Same-type copy uses the compiler's copy ctor (an exact match beats this template).
	template<class U> JList(const JList<U>& o) : _null(o.isNull()) {
		this->reserve(o.size());
		for (typename JList<U>::const_iterator i = o.begin(); i != o.end(); ++i) this->push_back((T)*i);
	}
	// `new ArrayList(someSet)` -- copy a Set (std::set-derived JSet) into a List, element-cast per slot.
	template<class U> JList(const std::set<U>& s) : _null(false) {
		for (typename std::set<U>::const_iterator i = s.begin(); i != s.end(); ++i) this->push_back((T)*i);
	}
	Iterator* iterator() const { return j2xIter(*this); }   // .iterator() on a chained/foreign receiver
	void add(const T& v) { this->push_back(v); }
	void add(int i, const T& v) { this->insert(this->begin() + i, v); }
	template<class C> void addAll(const C& o) { this->insert(this->end(), o.begin(), o.end()); }  // any container (vector/set/list)
	template<class C> bool removeAll(const C& o) {       // remove every element present in o
		bool changed = false;
		for (typename C::const_iterator k = o.begin(); k != o.end(); ++k) {
			_it it = std::find(this->begin(), this->end(), *k);
			if (it != this->end()) { this->erase(it); changed = true; }
		}
		return changed;
	}
	// BOUNDS-SAFE get/set (Java List throws IndexOutOfBounds; here we return a safe default / no-op
	// instead of an out-of-range std::vector[] -> memory corruption). Mirrors JArray's safe-slot rule.
	// FIXED BUG jlist-oob-set: an unchecked set() on an EMPTY list (RegionFile ctor when length()==0)
	// wrote past the vector and corrupted the heap -> world-gen hang. Range-check every index.
	T get(int i) const { return (i >= 0 && (size_t)i < this->size()) ? (*this)[(size_t)i] : T(); }
	T set(int i, const T& v) {
		if (i < 0 || (size_t)i >= this->size()) return T();
		T old = (*this)[(size_t)i]; (*this)[(size_t)i] = v; return old;
	}
	// Java's List.size() is an int: `size() - 1` stays a signed index on a 64-bit host (where size_t would
	// pick remove-by-value, FIXED BUG jlist-size-t-remove: Alpha's World.updatingLighting crashed on PC)
	int size() const { return (int)std::vector<T>::size(); }
	bool isEmpty() const { return this->empty(); }
	bool contains(const T& v) const { return std::find(this->begin(), this->end(), v) != this->end(); }
	int indexOf(const T& v) const {
		_cit it = std::find(this->begin(), this->end(), v);
		return it == this->end() ? -1 : (int)(it - this->begin());
	}
	T remove(int i) {                                    // List.remove(int index) -> removed element
		T v = (*this)[(size_t)i]; this->erase(this->begin() + i); return v;
	}
	// List.remove(int index) with a non-`int` integer index (unsigned/long from a Java int expr). These
	// NON-template overloads beat the by-value template below for integer args, so an index remove never
	// boxes the index into T (JObject(unsigned) is ambiguous) or mistakes it for a remove-by-value.
	T remove(unsigned i) { return this->remove((int)i); }
	T remove(long i) { return this->remove((int)i); }
	T remove(long long i) { return this->remove((int)i); }
	T remove(unsigned long i) { return this->remove((int)i); }
	T remove(unsigned long long i) { return this->remove((int)i); }   // size_t on a 64-bit host
	template<class U> bool remove(const U& v) {          // List.remove(Object) -> was-present
		// box U to the element type first: on a JList<JObject>, `std::find(.., EntityPlayer*)` would
		// compare `JObject == EntityPlayer*`, which is ambiguous (JObject has both operator==(JObject)
		// and an implicit EntityPlayer*<->JObject conversion). T(v) forces an element==element compare.
		T needle = T(v);
		_it it = std::find(this->begin(), this->end(), needle);
		if (it == this->end()) return false;
		this->erase(it); return true;
	}
	int hashCode() const { int h = 1; for (_cit i = this->begin(); i != this->end(); ++i) h = 31 * h; return h; }
	template<class C> bool equals(const C& o) const { return this->size() == o.size() && std::equal(this->begin(), this->end(), o.begin()); }
	// Java `list == null` / `!= null`
	bool operator==(long long v) const { return v == 0 && isNull(); }
	bool operator!=(long long v) const { return !(v == 0 && isNull()); }
};
// What Java's null is as a value of type V -- what Map.get gives for a missing key: V() for most (a null
// pointer, a null JavaString, a null JObject), but a default JList/JMap/JSet is an EMPTY one, not null.
template<class V> struct J2xNull { static V get() { return V(); } };
template<class T> struct J2xNull<JList<T> > { static JList<T> get() { return JList<T>(0LL); } };

// Java String-concatenation of a List calls AbstractCollection.toString() -> "[a, b, c]". Provide a
// stream op so `JCat() + list` compiles (used only in exception-message paths on the port).
#include <ostream>
template<class T> inline std::ostream& operator<<(std::ostream& o, const JList<T>& v) {
	o << '[';
	for (typename std::vector<T>::size_type i = 0; i < v.size(); ++i) { if (i) o << ", "; o << v[i]; }
	return o << ']';
}

// J2X no-GC caller-frees cleanup: a builder method (e.g. Level.getCubes) returns a fresh container of
// heap objects the CALLER now owns. Java GC reclaimed them; on the 360 the caller must delete them. The
// converter drops one of these RAII guards right after such a call -- its destructor frees every element
// at scope exit, so it is correct across early returns, breaks, and (were they enabled) exceptions. Only
// emitted when the elements are used transiently (iterated, never stored), so the delete cannot dangle.
template<class C> struct J2X_ScopedFreeElems {
	C& _c;
	explicit J2X_ScopedFreeElems(C& c) : _c(c) {}
	~J2X_ScopedFreeElems() { for (int _i = 0; _i < (int)_c.size(); ++_i) delete _c[_i]; }
private:
	J2X_ScopedFreeElems(const J2X_ScopedFreeElems&);
	J2X_ScopedFreeElems& operator=(const J2X_ScopedFreeElems&);
};

// J2X no-GC caller-frees cleanup for a single heap SCALAR: a builder returns `new X(...)` used inline as
// a call argument (Level.getCubes(bb.expand(...))). The converter hoists it into a temp and hands the
// temp here so its destructor frees it at scope exit -- emitted only when the receiving method provably
// just reads the argument (never stores/returns it), so the delete cannot dangle.
template<class T> struct J2X_ScopedDelete {
	T* _p;
	explicit J2X_ScopedDelete(T* p) : _p(p) {}
	~J2X_ScopedDelete() { delete _p; }
private:
	J2X_ScopedDelete(const J2X_ScopedDelete&);
	J2X_ScopedDelete& operator=(const J2X_ScopedDelete&);
};
#endif // JLIST_H
