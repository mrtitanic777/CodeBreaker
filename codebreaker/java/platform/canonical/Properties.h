#ifndef PROPERTIES_H
#define PROPERTIES_H
// ============================ Canonical java.util.Properties ==================================
// REAL key/value table -- NOT a no-op stub. StringTranslate does translateTable->load(lang stream) then
// translateKey(k) -> getProperty(k, k). A no-op load()+empty getProperty() leaves the i18n table empty,
// so EVERY translated string resolves to "" -- blank menu buttons and blank UI text game-wide (the
// classic "menu labels are missing" symptom). load() parses a .lang file (key=value / key:value lines).
// =============================================================================================
#include <map>
#include <string>
#include "JavaString.h"
#include "InputStream.h"

class Properties {
	std::map<std::string, std::string> _map;

	void _parseLine(const std::string& raw) {
		std::string::size_type s = 0;
		while (s < raw.size() && (raw[s] == ' ' || raw[s] == '\t' || raw[s] == '\r')) ++s;
		if (s >= raw.size() || raw[s] == '#' || raw[s] == '!') return;    // blank / comment
		std::string line = raw.substr(s);
		std::string::size_type eq = line.find('=');
		std::string::size_type co = line.find(':');
		if (eq == std::string::npos || (co != std::string::npos && co < eq)) eq = co;  // '=' or ':' (first)
		if (eq == std::string::npos) return;
		std::string key = line.substr(0, eq);
		std::string val = line.substr(eq + 1);
		while (!key.empty() && (key[key.size() - 1] == ' ' || key[key.size() - 1] == '\t')) key.erase(key.size() - 1);
		while (!val.empty() && (val[val.size() - 1] == '\r' || val[val.size() - 1] == '\n')) val.erase(val.size() - 1);
		if (!key.empty()) _map[key] = val;
	}

public:
	Properties(...) {}
	virtual ~Properties() {}

	// load(InputStream): pull the whole stream, split on newlines, parse each key=value line.
	virtual void load(InputStream* in) {
		if (!in) return;
		std::string line;
		int c;
		while ((c = in->read()) != -1) {
			if (c == '\n') { _parseLine(line); line.clear(); }
			else           { line += (char)c; }
		}
		if (!line.empty()) _parseLine(line);
	}
	virtual void load(...) {}                     // non-stream overloads (Reader) -> no-op fallback
	virtual void store(...) {}

	// Params are JavaString (NOT std::string): a JCat concat arg has BOTH operator JavaString() and
	// operator std::string(), so binding it to const std::string& is AMBIGUOUS (C2440); to JavaString it is
	// unambiguous (the std::string path would need a 2nd user conversion). JavaString is-a std::string, so
	// the map ops below still work.
	virtual void setProperty(const JavaString& k, const JavaString& v) { _map[k] = v; }
	virtual void put(const JavaString& k, const JavaString& v)         { _map[k] = v; }
	virtual void setProperty(...) {}             // fallback for any other call shape
	virtual void put(...) {}

	virtual JavaString getProperty(const JavaString& key) {
		std::map<std::string, std::string>::iterator it = _map.find(key);
		return it != _map.end() ? JavaString(it->second) : JavaString();
	}
	// java.util.Properties.getProperty(key, default): the stored value, else the default -- this is what
	// StringTranslate relies on to fall back to the raw key when a translation is missing.
	virtual JavaString getProperty(const JavaString& key, const JavaString& def) {
		std::map<std::string, std::string>::iterator it = _map.find(key);
		return it != _map.end() ? JavaString(it->second) : def;
	}
};
#endif // PROPERTIES_H
