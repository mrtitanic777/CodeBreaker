#ifndef JSET_H
#define JSET_H
#include <set>
#include "JIterator.h"
#include "JList.h"                              // J2xNull
template<class T> class JSet : public std::set<T> {
public:
	typedef typename std::set<T>::const_iterator _cit;
	// Java null, as for JList: `Set x = null`; only an empty set can be null
	bool _null;
	JSet() : _null(false) {}
	JSet(long long) : _null(true) {}                     // `Set x = null;`
	template<class C> JSet(const C& o) : _null(false) { this->insert(o.begin(), o.end()); }   // new TreeSet(collection)
	bool isNull() const { return _null && this->empty(); }
	// templated add/contains/remove cast the arg to the element type T: a RAW `Set<Integer>` becomes
	// JSet<void*>, so `add(intValue)` must C-cast int->void* (stored consistently; membership still works).
	template<class U> bool add(const U& v) { return this->insert((T)v).second; }
	template<class C> bool addAll(const C& o) { std::size_t n = this->size(); this->insert(o.begin(), o.end()); return this->size() != n; }
	template<class C> bool removeAll(const C& o) { bool ch = false; for (typename C::const_iterator k = o.begin(); k != o.end(); ++k) { if (this->erase(*k)) ch = true; } return ch; }
	template<class U> bool remove(const U& v) { return this->erase((T)v) != 0; }
	template<class U> bool contains(const U& v) const { return this->find((T)v) != this->end(); }
	bool isEmpty() const { return this->empty(); }
	T first() const { return *this->begin(); }
	Iterator* iterator() const { return j2xIter(*this); }   // .iterator() on a chained/temporary receiver
	bool operator==(long long v) const { return v == 0 && isNull(); }   // Java `set == null`
	bool operator!=(long long v) const { return !(v == 0 && isNull()); }
};
template<class T> struct J2xNull<JSet<T> > { static JSet<T> get() { return JSet<T>(0LL); } };
#endif // JSET_H
