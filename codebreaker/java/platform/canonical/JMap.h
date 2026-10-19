#ifndef JMAP_H
#define JMAP_H
#include <map>
#include <utility>
#include "JIterator.h"
#include "JList.h"
#include "Entry.h"
struct JObject;                                  // (Map)o unboxes through the ctor below (JavaRT.h)
template<class K, class V>
class JMap : public std::map<K, V> {
public:
	typedef typename std::map<K, V>::iterator _it;
	typedef typename std::map<K, V>::const_iterator _cit;
	// Java null, as for JList: `Map x = null` or a boxed null; only an empty map can be null
	bool _null;
	JMap() : _null(false) {}
	JMap(long long) : _null(true) {}                     // Java `Map x = null;`
	JMap(const JObject& o);                              // unbox a boxed Map; defined in JavaRT.h
	bool isNull() const { return _null && this->empty(); }
	V put(const K& k, const V& v) {                      // Java returns the PREVIOUS value (or null)
		_it it = this->find(k);
		if (it == this->end()) { this->insert(std::make_pair(k, v)); return J2xNull<V>::get(); }
		V old = it->second; it->second = v; return old;
	}
	V get(const K& k) const {                            // no insert (unlike operator[]); null if absent
		_cit it = this->find(k);
		return it == this->end() ? J2xNull<V>::get() : it->second;
	}
	bool containsKey(const K& k) const { return this->find(k) != this->end(); }
	V remove(const K& k) {                               // Java returns the removed value (or null)
		_it it = this->find(k);
		if (it == this->end()) return J2xNull<V>::get();
		V old = it->second; this->erase(it); return old;
	}
	bool isEmpty() const { return this->empty(); }
	// view methods -> by-value JList copies: begin/end (feed addAll / JSet(...) / JList(...)) + iterator().
	JList<V> values() const {
		JList<V> r; for (_cit i = this->begin(); i != this->end(); ++i) r.push_back(i->second); return r;
	}
	JList<K> keySet() const {
		JList<K> r; for (_cit i = this->begin(); i != this->end(); ++i) r.push_back(i->first); return r;
	}
	JList<Entry*> entrySet() const {                     // Entry.getKey()/getValue() return void* (cast at use)
		JList<Entry*> r;
		for (_cit i = this->begin(); i != this->end(); ++i)
			r.push_back(new Entry((void*)i->first, (void*)i->second));
		return r;
	}
	bool operator==(long long v) const { return v == 0 && isNull(); }   // Java `map == null`
	bool operator!=(long long v) const { return !(v == 0 && isNull()); }
	// java.util.AbstractMap.equals/hashCode (JSON node objects compare their backing map)
	bool equals(const JMap<K, V>& o) const { return (const std::map<K, V>&)*this == (const std::map<K, V>&)o; }
	template<class U> bool equals(const U&) const { return false; }   // non-map argument
	int hashCode() const { int h = 0; for (typename std::map<K, V>::const_iterator i = this->begin(); i != this->end(); ++i) h += (int)(size_t)&i->second; return h; }
};
template<class K, class V> struct J2xNull<JMap<K, V> > { static JMap<K, V> get() { return JMap<K, V>(0LL); } };
#endif // JMAP_H
