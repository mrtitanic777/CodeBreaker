// lib_db.cc -- sqlite3 over the SQLite amalgamation native/engine vendors (connect, execute with ?
// parameters, fetchone/fetchall/iteration, commit/rollback with Python's implicit transactions, the
// error classes), and pickle (dumps / loads of CPython's binary protocol for the values the cache keeps:
// None, bool, int, float, str, bytes, bytearray, tuple, list, dict, set, frozenset, and instances of
// classes by their module and name -- read and written so CPython's own pickles load here and back).
#include <cstring>

#include "rt.hpp"
#include "sqlite3.h"

namespace py {

std::wstring wide_(const std::string& s);

// ------------------------------------------------------------------------------------------ sqlite3
static TypeObj *T_conn, *T_cursor;
static var E_sqlError, E_sqlOperational, E_sqlIntegrity, E_sqlInterface, E_sqlDatabase, E_sqlProgramming;

struct ConnObj : DataObj {
    sqlite3* db = nullptr;
    bool in_tx = false;
    bool autocommit = false;       // isolation_level=None
    var row_factory;
    ~ConnObj() override {
        if (db) sqlite3_close_v2(db);
    }
};
struct CursorObj : DataObj {
    var conn;
    sqlite3_stmt* st = nullptr;
    bool done = true, have_row = false;
    int64_t rowcount = -1, lastrowid = 0;
    var description;
    ~CursorObj() override {
        if (st) sqlite3_finalize(st);
    }
};
static ConnObj* CO(const var& v) { return static_cast<ConnObj*>(v.ref()); }
static CursorObj* CU(const var& v) { return static_cast<CursorObj*>(v.ref()); }

[[noreturn]] static void sql_error(sqlite3* db, int rc) {
    int base = rc & 0xFF;
    var cls = base == SQLITE_CONSTRAINT ? E_sqlIntegrity : base == SQLITE_MISUSE || base == SQLITE_RANGE ? E_sqlProgramming : E_sqlOperational;
    std::string msg = db ? sqlite3_errmsg(db) : sqlite3_errstr(rc);
    raise(make_exc_args(T(cls), {new_str(msg)}));
}
static bool is_dml(const std::string& sql) {
    size_t i = 0;
    while (i < sql.size() && isspace((unsigned char)sql[i])) i++;
    std::string w;
    while (i < sql.size() && isalpha((unsigned char)sql[i])) w.push_back((char)toupper((unsigned char)sql[i++]));
    return w == "INSERT" || w == "UPDATE" || w == "DELETE" || w == "REPLACE";
}
static void bind_params(sqlite3* db, sqlite3_stmt* st, const var& params) {
    if (params.is_none() || params.is_unbound()) return;
    int n = sqlite3_bind_parameter_count(st);
    if (is_dict(params)) {
        for (int i = 1; i <= n; i++) {
            const char* name = sqlite3_bind_parameter_name(st, i);
            if (!name) continue;
            var v = params.getitem(new_str(name + 1));
            (void)v;
        }
    }
    std::vector<var> vals = is_dict(params) ? std::vector<var>{} : collect(params);
    if (!is_dict(params) && (int)vals.size() != n)
        raise(make_exc_args(T(E_sqlProgramming), {new_str("Incorrect number of bindings supplied. The current statement uses " + std::to_string(n) +
                                                          ", and there are " + std::to_string(vals.size()) + " supplied.")}));
    for (int i = 1; i <= n; i++) {
        var v;
        if (is_dict(params)) v = params.getitem(new_str(sqlite3_bind_parameter_name(st, i) + 1));
        else v = vals[(size_t)i - 1];
        int rc;
        if (v.is_none()) rc = sqlite3_bind_null(st, i);
        else if (is_int(v)) rc = sqlite3_bind_int64(st, i, v.as_int64());
        else if (v.is_float()) rc = sqlite3_bind_double(st, i, v.flt());
        else if (is_str(v)) {
            const std::string& s = str_utf8(v);
            rc = sqlite3_bind_text(st, i, s.data(), (int)s.size(), SQLITE_TRANSIENT);
        } else if (is_bytes(v)) {
            const std::string& s = raw_of(v);
            rc = sqlite3_bind_blob(st, i, s.data(), (int)s.size(), SQLITE_TRANSIENT);
        } else raise(make_exc_args(T(E_sqlProgramming), {new_str("Error binding parameter " + std::to_string(i) + ": type '" + tname(v) + "' is not supported")}));
        if (rc != SQLITE_OK) sql_error(db, rc);
    }
}
static var column(sqlite3_stmt* st, int i) {
    switch (sqlite3_column_type(st, i)) {
    case SQLITE_INTEGER: return var((int64_t)sqlite3_column_int64(st, i));
    case SQLITE_FLOAT: return var(sqlite3_column_double(st, i));
    case SQLITE_TEXT: return new_str(std::string((const char*)sqlite3_column_text(st, i), (size_t)sqlite3_column_bytes(st, i)));
    case SQLITE_BLOB: {
        const void* p = sqlite3_column_blob(st, i);
        return new_bytes(std::string((const char*)p, (size_t)sqlite3_column_bytes(st, i)));
    }
    default: return None;
    }
}
static bool step(CursorObj* c) {
    if (!c->st || c->done) return false;
    ConnObj* conn = CO(c->conn);
    int rc;
    {
        gil_release g;
        rc = sqlite3_step(c->st);
    }
    if (rc == SQLITE_ROW) { c->have_row = true; return true; }
    c->have_row = false;
    c->done = true;
    if (rc != SQLITE_DONE) sql_error(conn->db, rc);
    c->rowcount = sqlite3_changes(conn->db);
    c->lastrowid = sqlite3_last_insert_rowid(conn->db);
    return false;
}
static var current_row(CursorObj* c) {
    int n = sqlite3_column_count(c->st);
    std::vector<var> v;
    for (int i = 0; i < n; i++) v.push_back(column(c->st, i));
    var row = new_tuple(v);
    ConnObj* conn = CO(c->conn);
    if (!conn->row_factory.is_unbound() && !conn->row_factory.is_none()) row = call2(conn->row_factory, var(static_cast<Obj*>(c)), row);
    return row;
}
static var execute(const var& cur, const std::string& sql, const var& params) {
    CursorObj* c = CU(cur);
    ConnObj* conn = CO(c->conn);
    if (!conn->db) raise(make_exc_args(T(E_sqlProgramming), {"Cannot operate on a closed database."_s}));
    if (c->st) { sqlite3_finalize(c->st); c->st = nullptr; }
    if (!conn->autocommit && is_dml(sql) && sqlite3_get_autocommit(conn->db)) {
        int rc = sqlite3_exec(conn->db, "BEGIN", nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK) sql_error(conn->db, rc);
    }
    const char* tail = nullptr;
    int rc = sqlite3_prepare_v2(conn->db, sql.c_str(), (int)sql.size(), &c->st, &tail);
    if (rc != SQLITE_OK) sql_error(conn->db, rc);
    if (tail && *tail) {
        std::string rest = tail;
        if (rest.find_first_not_of(" \t\r\n;") != std::string::npos) {
            sqlite3_finalize(c->st);
            c->st = nullptr;
            raise(make_exc_args(T(E_sqlProgramming), {"You can only execute one statement at a time."_s}));
        }
    }
    bind_params(conn->db, c->st, params);
    c->done = false;
    c->rowcount = -1;
    int ncol = sqlite3_column_count(c->st);
    if (ncol) {
        std::vector<var> d;
        for (int i = 0; i < ncol; i++) d.push_back(new_tuple({new_str(sqlite3_column_name(c->st, i)), None, None, None, None, None, None}));
        c->description = new_tuple(d);
    } else c->description = None;
    step(c);
    return cur;
}
static var new_cursor(const var& conn) {
    auto* c = new CursorObj();
    c->type = T_cursor;
    c->conn = conn;
    return var(static_cast<Obj*>(c));
}

static void init_sqlite3(const var& m) {
    auto err = [&](const char* n, const var& base) {
        var c = make_class(n, n, {base}, new_dict(), {}, "sqlite3");
        mod_set(m, n, c);
        return c;
    };
    var warn = err("Warning", t::Exception);
    E_sqlError = err("Error", t::Exception);
    E_sqlInterface = err("InterfaceError", E_sqlError);
    E_sqlDatabase = err("DatabaseError", E_sqlError);
    E_sqlOperational = err("OperationalError", E_sqlDatabase);
    E_sqlIntegrity = err("IntegrityError", E_sqlDatabase);
    E_sqlProgramming = err("ProgrammingError", E_sqlDatabase);
    err("DataError", E_sqlDatabase);
    err("InternalError", E_sqlDatabase);
    err("NotSupportedError", E_sqlDatabase);
    (void)warn;
    T_conn = new_type("Connection", {}, Layout::NativeData, true, "sqlite3");
    T_cursor = new_type("Cursor", {}, Layout::NativeData, true, "sqlite3");
    mod_set(m, "Connection", tv(T_conn));
    mod_set(m, "Cursor", tv(T_cursor));
    mod_set(m, "Binary", t::bytes);
    mod_set(m, "sqlite_version", new_str(sqlite3_libversion()));
    mod_set(m, "version", "2.6.0"_s);
    mod_set(m, "PARSE_DECLTYPES", 1);
    mod_set(m, "PARSE_COLNAMES", 2);
    mod_def(m, "connect", [](const CallArgs& a) {
        std::string path = fs_path(arg(a, 0, "database"));
        double timeout = arg(a, 1, "timeout", 5.0).as_double();
        var iso = kwarg(a, "isolation_level", ""_s);
        auto* c = new ConnObj();
        c->type = T_conn;
        var cv(static_cast<Obj*>(c));
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI | SQLITE_OPEN_FULLMUTEX;
        int rc = sqlite3_open_v2(path.c_str(), &c->db, flags, nullptr);
        if (rc != SQLITE_OK) {
            std::string msg = c->db ? sqlite3_errmsg(c->db) : "unable to open database file";
            if (c->db) { sqlite3_close_v2(c->db); c->db = nullptr; }
            raise(make_exc_args(T(E_sqlOperational), {new_str(msg)}));
        }
        sqlite3_busy_timeout(c->db, (int)(timeout * 1000));
        c->autocommit = iso.is_none();
        return cv;
    });
    TypeObj* t = T_conn;
    add_method(t, "cursor", [](const CallArgs& a) { return new_cursor(a.pos[0]); });
    add_method(t, "execute", [](const CallArgs& a) {
        return execute(new_cursor(a.pos[0]), str_utf8(a.pos.at(1)), a.pos.size() > 2 ? a.pos[2] : var(None));
    });
    add_method(t, "executemany", [](const CallArgs& a) {
        var cur = new_cursor(a.pos[0]);
        for (var p : each(a.pos.at(2))) execute(cur, str_utf8(a.pos.at(1)), p);
        return cur;
    });
    add_method(t, "executescript", [](const CallArgs& a) {
        ConnObj* c = CO(a.pos[0]);
        char* err = nullptr;
        int rc = sqlite3_exec(c->db, str_utf8(a.pos.at(1)).c_str(), nullptr, nullptr, &err);
        if (err) sqlite3_free(err);
        if (rc != SQLITE_OK) sql_error(c->db, rc);
        return new_cursor(a.pos[0]);
    });
    add_method(t, "commit", [](const CallArgs& a) {
        ConnObj* c = CO(a.pos[0]);
        if (c->db && !sqlite3_get_autocommit(c->db)) {
            int rc = sqlite3_exec(c->db, "COMMIT", nullptr, nullptr, nullptr);
            if (rc != SQLITE_OK) sql_error(c->db, rc);
        }
        return None;
    });
    add_method(t, "rollback", [](const CallArgs& a) {
        ConnObj* c = CO(a.pos[0]);
        if (c->db && !sqlite3_get_autocommit(c->db)) sqlite3_exec(c->db, "ROLLBACK", nullptr, nullptr, nullptr);
        return None;
    });
    add_method(t, "close", [](const CallArgs& a) {
        ConnObj* c = CO(a.pos[0]);
        if (c->db) { sqlite3_close_v2(c->db); c->db = nullptr; }
        return None;
    });
    add_method(t, "__enter__", [](const CallArgs& a) { return a.pos[0]; });
    add_method(t, "__exit__", [](const CallArgs& a) {
        ConnObj* c = CO(a.pos[0]);
        if (c->db && !sqlite3_get_autocommit(c->db))
            sqlite3_exec(c->db, a.pos.at(1).is_none() ? "COMMIT" : "ROLLBACK", nullptr, nullptr, nullptr);
        return var(false);
    });
    add_getter(t, "total_changes", [](const var& s) { return var((int64_t)sqlite3_total_changes(CO(s)->db)); });
    add_getter(t, "in_transaction", [](const var& s) { return var(CO(s)->db && !sqlite3_get_autocommit(CO(s)->db)); });
    t = T_cursor;
    add_method(t, "execute", [](const CallArgs& a) { return execute(a.pos[0], str_utf8(a.pos.at(1)), a.pos.size() > 2 ? a.pos[2] : var(None)); });
    add_method(t, "executemany", [](const CallArgs& a) {
        for (var p : each(a.pos.at(2))) execute(a.pos[0], str_utf8(a.pos.at(1)), p);
        return a.pos[0];
    });
    add_method(t, "fetchone", [](const CallArgs& a) {
        CursorObj* c = CU(a.pos[0]);
        if (!c->have_row) return var(None);
        var r = current_row(c);
        step(c);
        return r;
    });
    add_method(t, "fetchall", [](const CallArgs& a) {
        CursorObj* c = CU(a.pos[0]);
        std::vector<var> out;
        while (c->have_row) {
            out.push_back(current_row(c));
            step(c);
        }
        return new_list(out);
    });
    add_method(t, "fetchmany", [](const CallArgs& a) {
        CursorObj* c = CU(a.pos[0]);
        int64_t n = a.pos.size() > 1 ? a.pos[1].as_int64() : 1;
        std::vector<var> out;
        while (c->have_row && (int64_t)out.size() < n) {
            out.push_back(current_row(c));
            step(c);
        }
        return new_list(out);
    });
    add_method(t, "__iter__", [](const CallArgs& a) {
        var self = a.pos[0];
        return new_iter([self](var& out) {
            CursorObj* c = CU(self);
            if (!c->have_row) return false;
            out = current_row(c);
            step(c);
            return true;
        });
    });
    add_method(t, "close", [](const CallArgs& a) {
        CursorObj* c = CU(a.pos[0]);
        if (c->st) { sqlite3_finalize(c->st); c->st = nullptr; }
        return None;
    });
    add_getter(t, "rowcount", [](const var& s) { return var(CU(s)->rowcount); });
    add_getter(t, "lastrowid", [](const var& s) { return var(CU(s)->lastrowid); });
    add_getter(t, "description", [](const var& s) { return CU(s)->description.is_unbound() ? var(None) : CU(s)->description; });
    add_getter(t, "connection", [](const var& s) { return CU(s)->conn; });
}

// ------------------------------------------------------------------------------------------ pickle
static var E_PicklingError, E_UnpicklingError;

struct Pickler {
    std::string out;
    std::unordered_map<const Obj*, uint32_t> memo;
    bool fast = false;   // (Pickler.fast: no memo, an object passed twice written twice)
    void memoize(const var& v) {
        if (!v.is_ref() || fast) return;
        uint32_t i = (uint32_t)memo.size();
        memo[v.ref()] = i;
        out.push_back('\x94');
    }
    bool get(const var& v) {
        if (!v.is_ref() || fast) return false;
        auto it = memo.find(v.ref());
        if (it == memo.end()) return false;
        if (it->second < 256) { out.push_back('h'); out.push_back((char)it->second); }
        else { out.push_back('j'); uint32_t x = it->second; out.append((const char*)&x, 4); }
        return true;
    }
    void u32(uint32_t n) { out.append((const char*)&n, 4); }
    void u64(uint64_t n) { out.append((const char*)&n, 8); }
    void dump(const var& v) {
        if (v.is_none()) { out.push_back('N'); return; }
        if (v.is_bool()) { out.push_back(v.small() ? '\x88' : '\x89'); return; }
        if (v.is_small_int() || (is_big(v) && v.ref()->type == T_int)) {
            Big b = to_big(v);
            if (b.fits_i64()) {
                int64_t x = b.i64();
                if (x >= 0 && x < 256) { out.push_back('K'); out.push_back((char)x); return; }
                if (x >= 0 && x < 65536) { out.push_back('M'); out.push_back((char)(x & 0xFF)); out.push_back((char)(x >> 8)); return; }
                if (x >= INT32_MIN && x <= INT32_MAX) { out.push_back('J'); int32_t y = (int32_t)x; out.append((const char*)&y, 4); return; }
            }
            // LONG1: little-endian two's complement, the fewest bytes
            std::string bytes;
            Big x = b;
            int nbytes = b.bit_length() / 8 + 1;
            Big m = b.sign() < 0 ? b + shl(Big(1), nbytes * 8) : b;
            for (int i = 0; i < nbytes; i++) { bytes.push_back((char)floormod(m, Big(256)).i64()); m = shr(m, 8); }
            while (bytes.size() > 1) {
                unsigned char last = (unsigned char)bytes.back(), prev = (unsigned char)bytes[bytes.size() - 2];
                if ((last == 0 && !(prev & 0x80)) || (last == 0xFF && (prev & 0x80))) bytes.pop_back();
                else break;
            }
            (void)x;
            out.push_back('\x8a');
            out.push_back((char)bytes.size());
            out += bytes;
            return;
        }
        if (v.is_float()) {
            double d = v.flt();
            uint64_t bits;
            memcpy(&bits, &d, 8);
            out.push_back('G');
            for (int i = 7; i >= 0; i--) out.push_back((char)(bits >> (8 * i)));
            return;
        }
        if (get(v)) return;
        TypeObj* t = type_of(v);
        if (t == T_str) {
            const std::string& s = str_utf8(v);
            if (s.size() < 256) { out.push_back('\x8c'); out.push_back((char)s.size()); }
            else { out.push_back('X'); u32((uint32_t)s.size()); }
            out += s;
            memoize(v);
            return;
        }
        if (t == T_bytes) {
            const std::string& s = raw_of(v);
            if (s.size() < 256) { out.push_back('C'); out.push_back((char)s.size()); }
            else { out.push_back('B'); u32((uint32_t)s.size()); }
            out += s;
            memoize(v);
            return;
        }
        if (t == T_bytearray) {
            const std::string& s = raw_of(v);
            out.push_back('\x96');
            u64(s.size());
            out += s;
            memoize(v);
            return;
        }
        if (t == T_tuple) {
            auto& items = tuple_items(v);
            if (items.empty()) { out.push_back(')'); return; }
            if (items.size() <= 3) {
                for (auto& x : items) dump(x);
                out.push_back((char)(0x85 + items.size() - 1));
            } else {
                out.push_back('(');
                for (auto& x : items) dump(x);
                out.push_back('t');
            }
            memoize(v);
            return;
        }
        if (t == T_list) {
            out.push_back(']');
            memoize(v);
            auto items = list_items(v);
            for (size_t i = 0; i < items.size(); i += 1000) {
                size_t e = std::min(items.size(), i + 1000);
                if (e - i == 1) { dump(items[i]); out.push_back('a'); continue; }
                out.push_back('(');
                for (size_t k = i; k < e; k++) dump(items[k]);
                out.push_back('e');
            }
            return;
        }
        if (t == T_dict) {
            out.push_back('}');
            memoize(v);
            std::vector<std::pair<var, var>> items;
            for (var k : each(v)) items.push_back({k, v.getitem(k)});
            for (size_t i = 0; i < items.size(); i += 1000) {
                size_t e = std::min(items.size(), i + 1000);
                if (e - i == 1) { dump(items[i].first); dump(items[i].second); out.push_back('s'); continue; }
                out.push_back('(');
                for (size_t k = i; k < e; k++) { dump(items[k].first); dump(items[k].second); }
                out.push_back('u');
            }
            return;
        }
        if (t == T_set) {
            out.push_back('\x8f');
            memoize(v);
            auto items = collect(v);
            if (!items.empty()) {
                out.push_back('(');
                for (auto& x : items) dump(x);
                out.push_back('\x90');
            }
            return;
        }
        if (t == T_frozenset) {
            out.push_back('(');
            for (var x : each(v)) dump(x);
            out.push_back('\x91');
            memoize(v);
            return;
        }
        if (t->layout == Layout::Type) {
            auto* ty = static_cast<TypeObj*>(v.ref());
            global(ty->module, ty->qualname);
            memoize(v);
            return;
        }
        if (!t->builtin && (t->layout == Layout::Object || t->layout == Layout::Exception)) {
            // copyreg.__newobj__: cls.__new__(cls), then the instance dict (BUILD)
            global(t->module, t->qualname);
            out.push_back(')');
            out.push_back('\x81');
            memoize(v);
            var st;
            var gs;
            if (lookup_special(v, "__getstate__", gs)) st = call0(gs);
            else st = v.ref()->attrs ? var(v.ref()->attrs) : new_dict();
            dump(st);
            out.push_back('b');
            return;
        }
        raise(make_exc_args(T(E_PicklingError), {new_str("cannot pickle '" + t->name + "' object")}));
    }
    void global(const std::string& mod, const std::string& name) {
        dump(new_str(mod));
        dump(new_str(name));
        out.push_back('\x93');
    }
};

struct Unpickler {
    const std::string& s;
    size_t i = 0;
    std::vector<var> stack;
    std::vector<size_t> marks;
    std::vector<var> memo;
    [[noreturn]] void fail(const std::string& m) { raise(make_exc_args(T(E_UnpicklingError), {new_str(m)})); }
    void need(size_t n) { if (i + n > s.size()) raise(make_exc_args(E_EOFError, {"Ran out of input"_s})); }
    uint32_t u32() { need(4); uint32_t x; memcpy(&x, s.data() + i, 4); i += 4; return x; }
    uint64_t u64() { need(8); uint64_t x; memcpy(&x, s.data() + i, 8); i += 8; return x; }
    std::string take(size_t n) { need(n); std::string r = s.substr(i, n); i += n; return r; }
    std::vector<var> pop_mark() {
        if (marks.empty()) fail("could not find MARK");
        size_t m = marks.back();
        marks.pop_back();
        std::vector<var> items(stack.begin() + (long long)m, stack.end());
        stack.resize(m);
        return items;
    }
    var pop() { if (stack.empty()) fail("unpickling stack underflow"); var v = stack.back(); stack.pop_back(); return v; }
    var load() {
        for (;;) {
            need(1);
            unsigned char op = (unsigned char)s[i++];
            switch (op) {
            case 0x80: i++; break;                  // PROTO
            case 0x95: i += 8; break;               // FRAME
            case '.': return pop();
            case '(': marks.push_back(stack.size()); break;
            case '0': pop(); break;
            case '1': pop_mark(); break;
            case '2': stack.push_back(stack.back()); break;
            case 'N': stack.push_back(None); break;
            case 0x88: stack.push_back(true); break;
            case 0x89: stack.push_back(false); break;
            case 'K': need(1); stack.push_back(var((int64_t)(unsigned char)s[i++])); break;
            case 'M': need(2); stack.push_back(var((int64_t)((unsigned char)s[i] | ((unsigned char)s[i + 1] << 8)))); i += 2; break;
            case 'J': { int32_t x = (int32_t)u32(); stack.push_back(var((int64_t)x)); break; }
            case 0x8a: case 0x8b: {
                size_t n = op == 0x8a ? (need(1), (unsigned char)s[i++]) : u32();
                std::string b = take(n);
                Big v(0);
                for (size_t k = n; k-- > 0;) v = shl(v, 8) + Big((int64_t)(unsigned char)b[k]);
                if (n && ((unsigned char)b[n - 1] & 0x80)) v = v - shl(Big(1), (int)(n * 8));
                stack.push_back(new_int(v));
                break;
            }
            case 'I': case 'L': {   // text ints (protocol 0)
                size_t e = s.find('\n', i);
                std::string t = s.substr(i, e - i);
                i = e + 1;
                if (!t.empty() && t.back() == 'L') t.pop_back();
                if (t == "01") stack.push_back(true);
                else if (t == "00") stack.push_back(false);
                else stack.push_back(int_from(t, 10));
                break;
            }
            case 'G': {
                need(8);
                uint64_t bits = 0;
                for (int k = 0; k < 8; k++) bits = (bits << 8) | (unsigned char)s[i + k];
                i += 8;
                double d;
                memcpy(&d, &bits, 8);
                stack.push_back(d);
                break;
            }
            case 0x8c: { need(1); size_t n = (unsigned char)s[i++]; stack.push_back(new_str(take(n))); break; }
            case 'X': { size_t n = u32(); stack.push_back(new_str(take(n))); break; }
            case 0x8d: { size_t n = (size_t)u64(); stack.push_back(new_str(take(n))); break; }
            case 'C': { need(1); size_t n = (unsigned char)s[i++]; stack.push_back(new_bytes(take(n))); break; }
            case 'B': { size_t n = u32(); stack.push_back(new_bytes(take(n))); break; }
            case 0x8e: { size_t n = (size_t)u64(); stack.push_back(new_bytes(take(n))); break; }
            case 0x96: { size_t n = (size_t)u64(); stack.push_back(new_bytes(take(n), true)); break; }
            case ')': stack.push_back(new_tuple()); break;
            case 0x85: { var a = pop(); stack.push_back(new_tuple({a})); break; }
            case 0x86: { var b = pop(), a = pop(); stack.push_back(new_tuple({a, b})); break; }
            case 0x87: { var c = pop(), b = pop(), a = pop(); stack.push_back(new_tuple({a, b, c})); break; }
            case 't': stack.push_back(new_tuple(pop_mark())); break;
            case ']': stack.push_back(new_list()); break;
            case 'l': stack.push_back(new_list(pop_mark())); break;
            case '}': stack.push_back(new_dict()); break;
            case 'd': {
                auto items = pop_mark();
                var d = new_dict();
                for (size_t k = 0; k + 1 < items.size(); k += 2) d.setitem(items[k], items[k + 1]);
                stack.push_back(d);
                break;
            }
            case 0x8f: stack.push_back(set({})); break;
            case 0x91: stack.push_back(frozenset_from(new_list(pop_mark()))); break;
            case 'a': { var x = pop(); list_items(stack.back()).push_back(x); break; }
            case 'e': {
                auto items = pop_mark();
                var l = stack.back();
                if (is_list(l)) for (auto& x : items) list_items(l).push_back(x);
                else for (auto& x : items) l.call("append", x);
                break;
            }
            case 's': { var v = pop(), k = pop(); stack.back().setitem(k, v); break; }
            case 'u': {
                auto items = pop_mark();
                for (size_t k = 0; k + 1 < items.size(); k += 2) stack.back().setitem(items[k], items[k + 1]);
                break;
            }
            case 0x90: { auto items = pop_mark(); for (auto& x : items) stack.back().call("add", x); break; }
            case 0x94: memo.push_back(stack.back()); break;
            case 'q': { need(1); size_t k = (unsigned char)s[i++]; if (memo.size() <= k) memo.resize(k + 1); memo[k] = stack.back(); break; }
            case 'r': { size_t k = u32(); if (memo.size() <= k) memo.resize(k + 1); memo[k] = stack.back(); break; }
            case 'h': { need(1); size_t k = (unsigned char)s[i++]; if (k >= memo.size()) fail("Memo value not found"); stack.push_back(memo[k]); break; }
            case 'j': { size_t k = u32(); if (k >= memo.size()) fail("Memo value not found"); stack.push_back(memo[k]); break; }
            case 0x93: { var name = pop(), mod = pop(); stack.push_back(find_class(str_utf8(mod), str_utf8(name))); break; }
            case 'c': {
                size_t e = s.find('\n', i);
                std::string mod = s.substr(i, e - i);
                i = e + 1;
                e = s.find('\n', i);
                std::string name = s.substr(i, e - i);
                i = e + 1;
                stack.push_back(find_class(mod, name));
                break;
            }
            case 'R': { var args = pop(), f = pop(); CallArgs a; a.pos = collect(args); stack.push_back(call_obj(f, a)); break; }
            case 0x81: {   // NEWOBJ
                var args = pop(), cls = pop();
                CallArgs a;
                a.pos.push_back(cls);
                for (auto& x : collect(args)) a.pos.push_back(x);
                const var* nw = T(cls)->lookup("__new__");
                var f = nw->is_ref() && nw->ref()->type == T_staticmethod ? static_cast<WrapObj*>(nw->ref())->f : *nw;
                stack.push_back(call_obj(f, a));
                break;
            }
            case 'b': {   // BUILD
                var st = pop();
                var obj = stack.back();
                var ss;
                if (lookup_special(obj, "__setstate__", ss)) { call1(ss, st); break; }
                var slotstate;
                if (is_tuple(st) && tuple_items(st).size() == 2) { slotstate = tuple_items(st)[1]; st = tuple_items(st)[0]; }
                if (is_dict(st)) for (var k : each(st)) obj.setattr(str_utf8(k).c_str(), st.getitem(k));
                if (!slotstate.is_unbound() && is_dict(slotstate)) for (var k : each(slotstate)) obj.setattr(str_utf8(k).c_str(), slotstate.getitem(k));
                break;
            }
            default: {
                char b[64];
                snprintf(b, sizeof b, "invalid load key, '\\x%02x'.", op);
                fail(b);
            }
            }
        }
    }
    var find_class(const std::string& mod, const std::string& name) {
        if (mod == "builtins" || mod == "__builtin__") return builtin(name.c_str());
        if (mod == "copyreg" && name == "_reconstructor")
            return native("_reconstructor", [](const CallArgs& a) {
                var cls = a.pos.at(0);
                auto* o = new Obj();
                o->type = T(cls);
                return var(o);
            });
        if (mod == "_codecs" && name == "encode")
            return native("encode", [](const CallArgs& a) { return a.pos.at(0).call("encode", a.pos.size() > 1 ? a.pos[1] : "utf-8"_s); });
        var m = import(mod.c_str());
        var v = m;
        size_t st = 0;
        for (size_t k = 0; k <= name.size(); k++)
            if (k == name.size() || name[k] == '.') { v = v.getattr(new_str(name.substr(st, k - st))); st = k + 1; }
        return v;
    }
};

static void init_pickle(const var& m) {
    var pe = make_class("PickleError", "PickleError", {t::Exception}, new_dict(), {}, "pickle");
    E_PicklingError = make_class("PicklingError", "PicklingError", {pe}, new_dict(), {}, "pickle");
    E_UnpicklingError = make_class("UnpicklingError", "UnpicklingError", {pe}, new_dict(), {}, "pickle");
    mod_set(m, "PickleError", pe);
    mod_set(m, "PicklingError", E_PicklingError);
    mod_set(m, "UnpicklingError", E_UnpicklingError);
    mod_set(m, "HIGHEST_PROTOCOL", 5);
    mod_set(m, "DEFAULT_PROTOCOL", 5);
    // Pickler(file, protocol): .fast, .dump(obj) -- one pickle per dump, written to the file
    var pns = new_dict();
    pns.setitem("__init__"_s, new_native("__init__", [](const CallArgs& a) {
        var self = a.pos.at(0);
        self.setattr("_file", a.pos.at(1));
        var proto = arg(a, 2, "protocol", None);
        self.setattr("_protocol", proto.is_none() ? var(5) : proto);
        self.setattr("fast", var(false));
        return None;
    }));
    pns.setitem("dump"_s, new_native("dump", [](const CallArgs& a) {
        var self = a.pos.at(0);
        Pickler p;
        p.fast = self.getattr("fast").truthy();
        int proto = (int)self.getattr("_protocol").as_int64();
        p.out = std::string("\x80", 1) + (char)(proto < 2 ? 2 : proto > 5 ? 5 : proto);
        p.dump(a.pos.at(1));
        p.out.push_back('.');
        self.getattr("_file").call("write", new_bytes(p.out));
        return None;
    }));
    mod_set(m, "Pickler", make_class("Pickler", "Pickler", {}, pns, {}, "pickle"));
    mod_def(m, "dumps", [](const CallArgs& a) {
        Pickler p;
        var proto = arg(a, 1, "protocol", None);
        int pv = proto.is_none() ? 5 : (int)proto.as_int64();
        p.out = std::string("\x80", 1) + (char)(pv < 2 ? 2 : pv > 5 ? 5 : pv);
        p.dump(a.pos.at(0));
        p.out.push_back('.');
        return new_bytes(p.out);
    });
    mod_def(m, "loads", [](const CallArgs& a) {
        Unpickler u{raw_of(a.pos.at(0))};
        return u.load();
    });
    mod_def(m, "dump", [](const CallArgs& a) {
        Pickler p;
        p.out = "\x80\x05";
        p.dump(a.pos.at(0));
        p.out.push_back('.');
        a.pos.at(1).call("write", new_bytes(p.out));
        return None;
    });
    mod_def(m, "load", [](const CallArgs& a) {
        var data = a.pos.at(0).call("read");
        Unpickler u{raw_of(data)};
        return u.load();
    });
}

void init_db_modules() {
    register_native_module("sqlite3", init_sqlite3);
    register_native_module("pickle", init_pickle);
}

}  // namespace py
