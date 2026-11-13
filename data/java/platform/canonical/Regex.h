#ifndef J2X_REGEX_H
#define J2X_REGEX_H
// java.util.regex, the part the games use: literals, `.`, classes `[a-z0-9_]` (and `[^...]`), the escapes
// \d \w \s \D \W \S and escaped metacharacters, groups `( )` (capturing) and `(?: )`, alternation `|`,
// the quantifiers ? * + {n} {n,} {n,m} (greedy), and the anchors ^ $. A backtracking matcher over the
// bytes of the string, as Java's over its chars for ASCII text (Beta's chunk-file names, JSON numbers).
// Not supported -- a pattern using them never matches: lazy/possessive quantifiers, lookaround,
// backreferences, named groups, flags. C++03 has no <regex>.
#include <string>
#include <vector>

class J2xRegex {
	// Nodes and sequences live in two pools and refer to each other by index (a node type holding vectors
	// of itself is not allowed before C++17).
	struct Node {
		enum Kind { CHR, ANY, SET, GROUP, BOL, EOL } kind;
		char c;                                   // CHR
		int set;                                  // SET: index into _sets
		std::vector<int> alts;                    // GROUP: its alternatives, indices into _seqs
		int cap;                                  // GROUP: capture index, or -1
		int lo, hi;                               // repetition: hi < 0 is unbounded
		Node() : kind(CHR), c(0), set(-1), cap(-1), lo(1), hi(1) {}
	};
	struct Cont {                                 // what is left to match after the current node
		int seq; size_t idx; const Cont* next;    // the rest of a sequence (seq >= 0) ...
		int rep; int count; int capStart;         // ... or a group that finished one repetition (rep >= 0)
	};
	std::vector<Node> _nodes;
	std::vector<std::vector<int> > _seqs;
	std::vector<std::vector<unsigned char> > _sets;
	int _top;

public:
	int ngroups;
	bool ok;

	explicit J2xRegex(const std::string& p) : _top(-1), ngroups(0), ok(true) {
		size_t i = 0;
		Node top;
		top.kind = Node::GROUP;
		top.cap = 0;
		top.alts = _alts(p, i);
		if (i != p.size()) ok = false;
		_nodes.push_back(top);
		_top = (int)_nodes.size() - 1;
	}

	// Matcher.matches: the whole string; `g` gets (start, end) per group, group 0 first
	bool matchAll(const std::string& s, std::vector<int>& g) const { return _attempt(s, 0, true, g) == (int)s.size(); }
	// Matcher.lookingAt: a match from the start, not necessarily to the end -> its end, or -1
	int prefix(const std::string& s, std::vector<int>& g) const { return _attempt(s, 0, false, g); }
	// a pattern with no metacharacter matches only its own text (String.split(":") and the like)
	static bool literal(const std::string& p) { return p.find_first_of("\\[](){}.*+?^$|") == std::string::npos; }
	// Matcher.find: the first match starting at or after `from` -> its start (end in g[1]), or -1
	int find(const std::string& s, int from, std::vector<int>& g) const {
		for (int st = from; st <= (int)s.size(); ++st)
			if (_attempt(s, st, false, g) >= 0) return st;
		return -1;
	}

private:
	std::vector<int> _alts(const std::string& p, size_t& i) {
		std::vector<int> alts;
		_seqs.push_back(std::vector<int>());
		alts.push_back((int)_seqs.size() - 1);
		while (i < p.size() && p[i] != ')') {
			if (p[i] == '|') {
				++i;
				_seqs.push_back(std::vector<int>());
				alts.push_back((int)_seqs.size() - 1);
				continue;
			}
			Node n;
			if (!_atom(p, i, n)) { ok = false; return alts; }
			_quant(p, i, n);
			_nodes.push_back(n);
			_seqs[alts.back()].push_back((int)_nodes.size() - 1);
		}
		return alts;
	}
	int _newSet() { _sets.push_back(std::vector<unsigned char>(256, 0)); return (int)_sets.size() - 1; }
	void _range(int set, int a, int b) { for (int k = a; k <= b; ++k) _sets[set][(unsigned char)k] = 1; }
	bool _escapeSet(char e, int set) {
		switch (e) {
		case 'd': _range(set, '0', '9'); return true;
		case 'w': _range(set, 'a', 'z'); _range(set, 'A', 'Z'); _range(set, '0', '9'); _sets[set]['_'] = 1; return true;
		case 's': {
			const char* ws = " \t\n\r\f\v";
			for (const char* w = ws; *w; ++w) _sets[set][(unsigned char)*w] = 1;
			return true;
		}
		case 'D': case 'W': case 'S': {
			int t = _newSet();
			_escapeSet((char)(e + 32), t);
			for (int k = 0; k < 256; ++k) if (!_sets[t][k]) _sets[set][k] = 1;
			return true;
		}
		}
		return false;
	}
	static char _escapeChar(char e) {
		switch (e) { case 't': return '\t'; case 'n': return '\n'; case 'r': return '\r'; case 'f': return '\f'; }
		return e;
	}
	static bool _isEscapeSet(char e) {
		return e == 'd' || e == 'w' || e == 's' || e == 'D' || e == 'W' || e == 'S';
	}
	bool _atom(const std::string& p, size_t& i, Node& n) {
		char c = p[i++];
		if (c == '(') {
			n.kind = Node::GROUP;
			if (i + 1 < p.size() && p[i] == '?') {
				if (p[i + 1] != ':') return false;           // lookaround, named groups: not supported
				i += 2;
			} else {
				n.cap = ++ngroups;
			}
			n.alts = _alts(p, i);
			if (i >= p.size() || p[i] != ')') return false;
			++i;
			return true;
		}
		if (c == '[') {
			n.kind = Node::SET;
			n.set = _newSet();
			bool neg = i < p.size() && p[i] == '^';
			if (neg) ++i;
			bool first = true;
			while (i < p.size() && (p[i] != ']' || first)) {
				first = false;
				int a = (unsigned char)p[i++];
				if (a == '\\' && i < p.size()) {
					if (_isEscapeSet(p[i])) { _escapeSet(p[i], n.set); ++i; continue; }
					a = (unsigned char)_escapeChar(p[i++]);
				}
				if (i + 1 < p.size() && p[i] == '-' && p[i + 1] != ']') {
					int b = (unsigned char)p[i + 1];
					i += 2;
					if (b == '\\' && i < p.size()) b = (unsigned char)_escapeChar(p[i++]);
					_range(n.set, a, b);
				} else {
					_sets[n.set][a] = 1;
				}
			}
			if (i >= p.size()) return false;
			++i;                                            // the closing ']'
			if (neg) for (int k = 0; k < 256; ++k) _sets[n.set][k] = (unsigned char)!_sets[n.set][k];
			return true;
		}
		if (c == '\\') {
			if (i >= p.size()) return false;
			char e = p[i++];
			if (_isEscapeSet(e)) { n.kind = Node::SET; n.set = _newSet(); _escapeSet(e, n.set); return true; }
			if ((e >= '0' && e <= '9') || e == 'b' || e == 'B' || e == 'p' || e == 'P' || e == 'Q') return false;
			n.kind = Node::CHR; n.c = _escapeChar(e);
			return true;
		}
		if (c == '.') { n.kind = Node::ANY; return true; }
		if (c == '^') { n.kind = Node::BOL; return true; }
		if (c == '$') { n.kind = Node::EOL; return true; }
		if (c == '*' || c == '+' || c == '?' || c == '{') return false;
		n.kind = Node::CHR; n.c = c;
		return true;
	}
	void _quant(const std::string& p, size_t& i, Node& n) {
		if (i >= p.size()) return;
		char q = p[i];
		if (q == '?') { n.lo = 0; n.hi = 1; ++i; }
		else if (q == '*') { n.lo = 0; n.hi = -1; ++i; }
		else if (q == '+') { n.lo = 1; n.hi = -1; ++i; }
		else if (q == '{') {
			size_t j = i + 1; int a = 0, b; bool digits = false;
			while (j < p.size() && p[j] >= '0' && p[j] <= '9') { a = a * 10 + (p[j++] - '0'); digits = true; }
			if (!digits) { ok = false; return; }
			b = a;
			if (j < p.size() && p[j] == ',') {
				++j; b = -1;
				if (j < p.size() && p[j] >= '0' && p[j] <= '9') {
					b = 0;
					while (j < p.size() && p[j] >= '0' && p[j] <= '9') b = b * 10 + (p[j++] - '0');
				}
			}
			if (j >= p.size() || p[j] != '}') { ok = false; return; }
			n.lo = a; n.hi = b; i = j + 1;
		} else {
			return;
		}
		if (i < p.size() && (p[i] == '?' || p[i] == '+')) ok = false;   // lazy / possessive: not supported
	}

	// -- matching: backtracking with explicit continuations -------------------------------------------
	int _attempt(const std::string& s, int at, bool whole, std::vector<int>& g) const {
		if (!ok) return -1;
		g.assign(2 * (ngroups + 1), -1);
		Cont end = { -1, 0, 0, -1, 0, -1 };
		return _node(s, at, _top, 0, &end, whole, g);
	}
	// node `ni`, already matched `count` times, at `pos`; then the continuation `k`. Greedy: one more first.
	int _node(const std::string& s, int pos, int ni, int count, const Cont* k, bool whole, std::vector<int>& g) const {
		const Node& n = _nodes[ni];
		if (n.hi < 0 || count < n.hi) {
			int r = _one(s, pos, ni, count, k, whole, g);
			if (r >= 0) return r;
		}
		return count >= n.lo ? _cont(s, pos, k, whole, g) : -1;
	}
	int _one(const std::string& s, int pos, int ni, int count, const Cont* k, bool whole, std::vector<int>& g) const {
		const Node& n = _nodes[ni];
		bool more = pos < (int)s.size();
		switch (n.kind) {
		case Node::CHR:
			return more && s[pos] == n.c ? _node(s, pos + 1, ni, count + 1, k, whole, g) : -1;
		case Node::ANY:
			return more && s[pos] != '\n' ? _node(s, pos + 1, ni, count + 1, k, whole, g) : -1;
		case Node::SET:
			return more && _sets[n.set][(unsigned char)s[pos]] ? _node(s, pos + 1, ni, count + 1, k, whole, g) : -1;
		case Node::BOL:
			return pos == 0 ? _cont(s, pos, k, whole, g) : -1;
		case Node::EOL:
			return pos == (int)s.size() ? _cont(s, pos, k, whole, g) : -1;
		case Node::GROUP: {
			Cont again = { -1, 0, k, ni, count + 1, pos };
			for (size_t a = 0; a < n.alts.size(); ++a) {
				Cont inner = { n.alts[a], 0, &again, -1, 0, -1 };
				std::vector<int> save(g);
				int r = _cont(s, pos, &inner, whole, g);
				if (r >= 0) return r;
				g = save;
			}
			return -1;
		}
		}
		return -1;
	}
	int _cont(const std::string& s, int pos, const Cont* k, bool whole, std::vector<int>& g) const {
		if (k->seq >= 0) {
			const std::vector<int>& seq = _seqs[k->seq];
			if (k->idx < seq.size()) {
				Cont rest = *k;
				rest.idx = k->idx + 1;
				return _node(s, pos, seq[k->idx], 0, &rest, whole, g);
			}
			return _cont(s, pos, k->next, whole, g);        // the alternative is done: back to its group
		}
		if (k->rep < 0)                                      // the end of the pattern
			return (!whole || pos == (int)s.size()) ? pos : -1;
		// a group finished a repetition: record its capture, then repeat it or go on
		const Node& n = _nodes[k->rep];
		if (k->count > n.lo && pos == k->capStart) return -1;   // an empty repetition past the minimum
		int c0 = 0, c1 = 0;
		if (n.cap >= 0) { c0 = g[2 * n.cap]; c1 = g[2 * n.cap + 1]; g[2 * n.cap] = k->capStart; g[2 * n.cap + 1] = pos; }
		int r = _node(s, pos, k->rep, k->count, k->next, whole, g);
		if (r < 0 && n.cap >= 0) { g[2 * n.cap] = c0; g[2 * n.cap + 1] = c1; }
		return r;
	}
};
#endif // J2X_REGEX_H
