#ifndef JLIST_H
#define JLIST_H
#include <vector>
#include <algorithm>

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
	JList() {}
	JList(long long) {}                                  // Java `List x = null;` -> empty list
	void add(const T& v) { this->push_back(v); }
	void add(int i, const T& v) { this->insert(this->begin() + i, v); }
	template<class C> void addAll(const C& o) { this->insert(this->end(), o.begin(), o.end()); }  // any container (vector/set/list)
	T get(int i) const { return (*this)[(size_t)i]; }
	T set(int i, const T& v) { T old = (*this)[(size_t)i]; (*this)[(size_t)i] = v; return old; }
	bool isEmpty() const { return this->empty(); }
	bool contains(const T& v) const { return std::find(this->begin(), this->end(), v) != this->end(); }
	int indexOf(const T& v) const {
		_cit it = std::find(this->begin(), this->end(), v);
		return it == this->end() ? -1 : (int)(it - this->begin());
	}
	T remove(int i) {                                    // List.remove(int index) -> removed element
		T v = (*this)[(size_t)i]; this->erase(this->begin() + i); return v;
	}
	template<class U> bool remove(const U& v) {          // List.remove(Object) -> was-present
		_it it = std::find(this->begin(), this->end(), v);
		if (it == this->end()) return false;
		this->erase(it); return true;
	}
	// Java `list != null` / `== null`: a by-value list is never null (it always exists).
	bool operator==(long long) const { return false; }
	bool operator!=(long long) const { return true; }
};
#endif // JLIST_H
