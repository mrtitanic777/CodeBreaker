#ifndef JSET_H
#define JSET_H
#include <set>
#include "JIterator.h"
template<class T> class JSet : public std::set<T> {
public:
	typedef typename std::set<T>::const_iterator _cit;
	JSet() {}
	JSet(long long) {}                                   // `Set x = null;` -> empty set
	template<class C> JSet(const C& o) { this->insert(o.begin(), o.end()); }   // new TreeSet(collection)
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
	bool operator==(long long) const { return false; }
	bool operator!=(long long) const { return true; }
};
#endif // JSET_H
