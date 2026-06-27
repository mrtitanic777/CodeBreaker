#ifndef CLASS_H
#define CLASS_H
// java.lang.Class -- backed by C++ RTTI (std::type_info). Identity, ==, getName and map-key
// ordering are REAL: `X.class` (Class(typeid(X))) and `obj.getClass()` (Class(typeid(*obj)))
// agree for the same runtime type. newInstance/getConstructor have no reflection equivalent and
// return 0 -- the class-keyed registries (EntityList/Packet/TileEntity) supply explicit factories.
#include <string>
#include <typeinfo>
class URL;
class InputStream;

class Class {
	const std::type_info* _ti;
public:
	Class() : _ti(0) {}
	Class(const std::type_info& ti) : _ti(&ti) {}
	std::string getName() const { return _ti ? std::string(_ti->name()) : std::string(); }
	std::string getSimpleName() const { return getName(); }
	bool equals(const Class& o) const { return *this == o; }
	bool operator==(const Class& o) const { return _ti == o._ti || (_ti && o._ti && *_ti == *o._ti); }
	bool operator!=(const Class& o) const { return !(*this == o); }
	bool operator<(const Class& o) const {                 // strict-weak order for std::map keys
		if (!_ti) return o._ti != 0;
		if (!o._ti) return false;
		return _ti->before(*o._ti) != 0;
	}
	int hashCode() const {
		std::string n = getName(); int h = 0;
		for (std::string::size_type i = 0; i < n.size(); ++i) h = h * 31 + (unsigned char)n[i];
		return h;
	}
	bool isInstance(...) const { return false; }
	bool isAssignableFrom(...) const { return false; }
	Class* getSuperclass() { return 0; }
	void* getConstructor(...) { return 0; }
	void* newInstance(...) { return 0; }
	void* getDeclaredFields(...) { return 0; }
	URL* getResource(...) { return 0; }
	InputStream* getResourceAsStream(...) { return 0; }
	operator Class*() { return this; }                     // usable where a Class* is expected
};
#endif // CLASS_H
