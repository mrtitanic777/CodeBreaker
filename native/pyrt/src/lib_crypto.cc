// lib_crypto.cc -- the parts of `cryptography` (and pycryptodome's DES) the Xbox crypto code uses, over
// Windows' bcrypt: hashes (Hash, SHA1 / MD5 / SHA256 ...), HMAC, Cipher with AES / TripleDES / DES (ECB,
// CBC) and ARC4, and RSA's numbers and keys (the arithmetic itself is the callers' own pow()).
#include <windows.h>
#include <bcrypt.h>

#include <cstring>

#include "rt.hpp"

namespace py {

TypeObj* T(const var& v);

namespace {

var nat(const char* n, native_fn f) { return new_native(n, std::move(f)); }
var klass(const char* name, std::vector<var> bases, const char* module, std::vector<std::pair<const char*, var>> members = {}) {
    var ns = new_dict();
    ns.setitem("__module__"_s, new_str(module));
    for (auto& [k, v] : members) ns.setitem(new_str(k), v);
    return make_class(name, name, bases, ns, {}, module);
}
var g_hashes, g_algos, g_modes;

// ------------------------------------------------------------------------------------------ hashes
LPCWSTR bcrypt_hash(const std::string& name, ULONG& size) {
    if (name == "sha1") { size = 20; return BCRYPT_SHA1_ALGORITHM; }
    if (name == "md5") { size = 16; return BCRYPT_MD5_ALGORITHM; }
    if (name == "sha256") { size = 32; return BCRYPT_SHA256_ALGORITHM; }
    if (name == "sha384") { size = 48; return BCRYPT_SHA384_ALGORITHM; }
    if (name == "sha512") { size = 64; return BCRYPT_SHA512_ALGORITHM; }
    raise(E_ValueError, "unsupported hash algorithm " + name);
}
std::string digest(const std::string& name, const std::string& data, const std::string* hmac_key) {
    ULONG size;
    LPCWSTR id = bcrypt_hash(name, size);
    BCRYPT_ALG_HANDLE alg;
    BCryptOpenAlgorithmProvider(&alg, id, nullptr, hmac_key ? BCRYPT_ALG_HANDLE_HMAC_FLAG : 0);
    BCRYPT_HASH_HANDLE h;
    BCryptCreateHash(alg, &h, nullptr, 0, hmac_key ? (PUCHAR)hmac_key->data() : nullptr, hmac_key ? (ULONG)hmac_key->size() : 0, 0);
    BCryptHashData(h, (PUCHAR)data.data(), (ULONG)data.size(), 0);
    std::string out(size, '\0');
    BCryptFinishHash(h, (PUCHAR)out.data(), size, 0);
    BCryptDestroyHash(h);
    BCryptCloseAlgorithmProvider(alg, 0);
    return out;
}

// ------------------------------------------------------------------------------------------ ciphers
// a block cipher context: whole blocks through bcrypt (the chaining value carried between calls), the
// rest kept for the next update; finalize wants nothing left over (no padding, as cryptography's)
struct CipherCtx : DataObj {
    std::string alg;      // "AES", "3DES", "DES", "ARC4"
    std::string key, iv;  // iv: CBC's running value
    bool cbc = false, encrypt = true, done = false;
    std::string pending;
    uint8_t S[256];
    int si = 0, sj = 0;   // ARC4's state
};
TypeObj* T_ctx = nullptr;
std::string block_run(CipherCtx* c, const std::string& data) {
    LPCWSTR id = c->alg == "AES" ? BCRYPT_AES_ALGORITHM : c->alg == "DES" ? BCRYPT_DES_ALGORITHM : BCRYPT_3DES_ALGORITHM;
    size_t bs = c->alg == "AES" ? 16 : 8;
    BCRYPT_ALG_HANDLE alg;
    if (BCryptOpenAlgorithmProvider(&alg, id, nullptr, 0) != 0) raise(E_RuntimeError, "bcrypt: no " + c->alg);
    const wchar_t* mode = c->cbc ? BCRYPT_CHAIN_MODE_CBC : BCRYPT_CHAIN_MODE_ECB;
    BCryptSetProperty(alg, BCRYPT_CHAINING_MODE, (PUCHAR)mode, (ULONG)((wcslen(mode) + 1) * 2), 0);
    std::string key = c->key;
    if (c->alg == "3DES" && key.size() == 16) key += key.substr(0, 8);   // (two keys: K1 K2 K1)
    if (c->alg == "3DES" && key.size() == 8) key = key + key + key;
    BCRYPT_KEY_HANDLE k;
    if (BCryptGenerateSymmetricKey(alg, &k, nullptr, 0, (PUCHAR)key.data(), (ULONG)key.size(), 0) != 0) {
        BCryptCloseAlgorithmProvider(alg, 0);
        raise(E_ValueError, "Invalid key size (" + std::to_string(c->key.size() * 8) + ") for " + c->alg + ".");
    }
    std::string out(data.size(), '\0');
    ULONG got = 0;
    std::string iv = c->iv;
    NTSTATUS st = c->encrypt
        ? BCryptEncrypt(k, (PUCHAR)data.data(), (ULONG)data.size(), nullptr, c->cbc ? (PUCHAR)iv.data() : nullptr, c->cbc ? (ULONG)iv.size() : 0, (PUCHAR)out.data(), (ULONG)out.size(), &got, 0)
        : BCryptDecrypt(k, (PUCHAR)data.data(), (ULONG)data.size(), nullptr, c->cbc ? (PUCHAR)iv.data() : nullptr, c->cbc ? (ULONG)iv.size() : 0, (PUCHAR)out.data(), (ULONG)out.size(), &got, 0);
    BCryptDestroyKey(k);
    BCryptCloseAlgorithmProvider(alg, 0);
    if (st != 0) raise(E_RuntimeError, "bcrypt: the cipher failed");
    if (c->cbc) c->iv = iv;   // (bcrypt leaves the last chaining block in iv)
    (void)bs;
    return out;
}
std::string ctx_update(CipherCtx* c, const std::string& in) {
    if (c->done) raise(E_ValueError, "Context was already finalized.");
    if (c->alg == "ARC4") {
        std::string out(in.size(), '\0');
        for (size_t n = 0; n < in.size(); n++) {
            c->si = (c->si + 1) & 255;
            c->sj = (c->sj + c->S[c->si]) & 255;
            std::swap(c->S[c->si], c->S[c->sj]);
            out[n] = (char)((uint8_t)in[n] ^ c->S[(c->S[c->si] + c->S[c->sj]) & 255]);
        }
        return out;
    }
    size_t bs = c->alg == "AES" ? 16 : 8;
    c->pending += in;
    size_t whole = c->pending.size() / bs * bs;
    if (!whole) return "";
    std::string out = block_run(c, c->pending.substr(0, whole));
    c->pending.erase(0, whole);
    return out;
}
var new_ctx(const var& cipher, bool encrypt) {
    var alg = cipher.getattr("algorithm"), mode = cipher.getattr("mode");
    auto* c = new CipherCtx();
    c->type = T_ctx;
    c->alg = str_utf8(alg.getattr("_bcrypt"));
    c->key = raw_of(alg.getattr("key"));
    c->encrypt = encrypt;
    if (c->alg == "ARC4") {
        for (int n = 0; n < 256; n++) c->S[n] = (uint8_t)n;
        for (int n = 0, j = 0; n < 256; n++) {
            j = (j + c->S[n] + (uint8_t)c->key[(size_t)n % c->key.size()]) & 255;
            std::swap(c->S[n], c->S[j]);
        }
    } else {
        if (mode.is_none()) raise(E_TypeError, "a block cipher needs a mode");
        c->cbc = str_utf8(mode.getattr("name")) == "CBC";
        if (c->cbc) c->iv = raw_of(mode.getattr("initialization_vector"));
    }
    return var(static_cast<Obj*>(c));
}

}  // namespace

void init_crypto_modules() {
    // the package tree (every level a module, as `from cryptography.hazmat.primitives.ciphers import ...` imports)
    for (const char* n : {"cryptography", "cryptography.hazmat", "cryptography.hazmat.primitives", "cryptography.hazmat.primitives.asymmetric",
                          "cryptography.hazmat.decrepit", "cryptography.hazmat.decrepit.ciphers", "cryptography.hazmat.backends", "Crypto"})
        register_native_module(n, [](const var& m) { mod_set(m, "__version__", "44.0.0 (pyrt over bcrypt)"_s); });
    register_native_module("cryptography.exceptions", [](const var& m) {
        mod_set(m, "InvalidSignature", klass("InvalidSignature", {t::Exception}, "cryptography.exceptions"));
        mod_set(m, "InvalidKey", klass("InvalidKey", {t::Exception}, "cryptography.exceptions"));
        mod_set(m, "AlreadyFinalized", klass("AlreadyFinalized", {t::Exception}, "cryptography.exceptions"));
    });
    register_native_module("cryptography.hazmat.primitives.hashes", [](const var& m) {
        var base = klass("HashAlgorithm", {}, "cryptography.hazmat.primitives.hashes");
        mod_set(m, "HashAlgorithm", base);
        struct H { const char* cls; const char* name; int size, block; };
        for (const H& h : {H{"SHA1", "sha1", 20, 64}, H{"MD5", "md5", 16, 64}, H{"SHA256", "sha256", 32, 64}, H{"SHA384", "sha384", 48, 128},
                           H{"SHA512", "sha512", 64, 128}})
            mod_set(m, h.cls, klass(h.cls, {base}, "cryptography.hazmat.primitives.hashes",
                                    {{"name", new_str(h.name)}, {"digest_size", var((int64_t)h.size)}, {"block_size", var((int64_t)h.block)}}));
        var hns = new_dict();
        hns.setitem("__init__"_s, nat("__init__", [](const CallArgs& a) {
            a.pos.at(0).setattr("algorithm", a.pos.at(1));
            a.pos[0].setattr("_data", new_bytes(""));
            return None;
        }));
        hns.setitem("update"_s, nat("update", [](const CallArgs& a) {
            a.pos.at(0).setattr("_data", new_bytes(raw_of(a.pos[0].getattr("_data")) + raw_of(a.pos.at(1))));
            return None;
        }));
        hns.setitem("copy"_s, nat("copy", [](const CallArgs& a) {
            var c = call1(tv(type_of(a.pos.at(0))), a.pos[0].getattr("algorithm"));
            c.setattr("_data", a.pos[0].getattr("_data"));
            return c;
        }));
        hns.setitem("finalize"_s, nat("finalize", [](const CallArgs& a) {
            var self = a.pos.at(0);
            return new_bytes(digest(str_utf8(self.getattr("algorithm").getattr("name")), raw_of(self.getattr("_data")), nullptr));
        }));
        mod_set(m, "Hash", make_class("Hash", "Hash", {}, hns, {}, "cryptography.hazmat.primitives.hashes"));
    });
    register_native_module("cryptography.hazmat.primitives.hmac", [](const var& m) {
        var ns = new_dict();
        ns.setitem("__init__"_s, nat("__init__", [](const CallArgs& a) {
            a.pos.at(0).setattr("_key", new_bytes(raw_of(a.pos.at(1))));
            a.pos[0].setattr("algorithm", a.pos.at(2));
            a.pos[0].setattr("_data", new_bytes(""));
            return None;
        }));
        ns.setitem("update"_s, nat("update", [](const CallArgs& a) {
            a.pos.at(0).setattr("_data", new_bytes(raw_of(a.pos[0].getattr("_data")) + raw_of(a.pos.at(1))));
            return None;
        }));
        ns.setitem("copy"_s, nat("copy", [](const CallArgs& a) {
            var s = a.pos.at(0);
            var c = call2(tv(type_of(s)), s.getattr("_key"), s.getattr("algorithm"));
            c.setattr("_data", s.getattr("_data"));
            return c;
        }));
        ns.setitem("finalize"_s, nat("finalize", [](const CallArgs& a) {
            var s = a.pos.at(0);
            std::string key = raw_of(s.getattr("_key"));
            return new_bytes(digest(str_utf8(s.getattr("algorithm").getattr("name")), raw_of(s.getattr("_data")), &key));
        }));
        ns.setitem("verify"_s, nat("verify", [](const CallArgs& a) -> var {
            var got = a.pos.at(0).call("finalize");
            if (!(got == a.pos.at(1))) raise(T(import("cryptography.exceptions").getattr("InvalidSignature")), "Signature did not match digest.");
            return None;
        }));
        mod_set(m, "HMAC", make_class("HMAC", "HMAC", {}, ns, {}, "cryptography.hazmat.primitives.hmac"));
    });
    // ciphers: algorithms carry their key (and bcrypt's name); modes their name and IV
    auto algorithms = [](const var& m) {
        var base = klass("CipherAlgorithm", {}, "cryptography.hazmat.primitives.ciphers.algorithms");
        struct A { const char* cls; const char* bc; int block; };
        for (const A& al : {A{"AES", "AES", 128}, A{"TripleDES", "3DES", 64}, A{"ARC4", "ARC4", 0}}) {
            var ns = new_dict();
            ns.setitem("name"_s, new_str(al.cls));
            ns.setitem("_bcrypt"_s, new_str(al.bc));
            ns.setitem("block_size"_s, var((int64_t)al.block));
            ns.setitem("__init__"_s, nat("__init__", [](const CallArgs& a) {
                a.pos.at(0).setattr("key", new_bytes(raw_of(a.pos.at(1))));
                a.pos[0].setattr("key_size", var((int64_t)raw_of(a.pos[1]).size() * 8));
                return None;
            }));
            mod_set(m, al.cls, make_class(al.cls, al.cls, {base}, ns, {}, "cryptography.hazmat.primitives.ciphers.algorithms"));
        }
        mod_set(m, "AES128", m.getattr("AES"));
        mod_set(m, "AES256", m.getattr("AES"));
    };
    register_native_module("cryptography.hazmat.primitives.ciphers.algorithms", algorithms);
    register_native_module("cryptography.hazmat.decrepit.ciphers.algorithms", [](const var& m) {
        var a = import("cryptography.hazmat.primitives.ciphers.algorithms");
        mod_set(m, "ARC4", a.getattr("ARC4"));
        mod_set(m, "TripleDES", a.getattr("TripleDES"));
    });
    register_native_module("cryptography.hazmat.primitives.ciphers.modes", [](const var& m) {
        var ecb = new_dict();
        ecb.setitem("name"_s, "ECB"_s);
        mod_set(m, "ECB", make_class("ECB", "ECB", {}, ecb, {}, "cryptography.hazmat.primitives.ciphers.modes"));
        var cbc = new_dict();
        cbc.setitem("name"_s, "CBC"_s);
        cbc.setitem("__init__"_s, nat("__init__", [](const CallArgs& a) {
            a.pos.at(0).setattr("initialization_vector", new_bytes(raw_of(a.pos.at(1))));
            return None;
        }));
        mod_set(m, "CBC", make_class("CBC", "CBC", {}, cbc, {}, "cryptography.hazmat.primitives.ciphers.modes"));
    });
    register_native_module("cryptography.hazmat.primitives.ciphers", [](const var& m) {
        T_ctx = new_type("_CipherContext", {}, Layout::NativeData, false, "cryptography.hazmat.primitives.ciphers");
        add_method(T_ctx, "update", [](const CallArgs& a) {
            return new_bytes(ctx_update(static_cast<CipherCtx*>(a.pos.at(0).ref()), raw_of(a.pos.at(1))));
        });
        add_method(T_ctx, "finalize", [](const CallArgs& a) -> var {
            auto* c = static_cast<CipherCtx*>(a.pos.at(0).ref());
            if (!c->pending.empty()) raise(E_ValueError, "The length of the provided data is not a multiple of the block length.");
            c->done = true;
            return new_bytes("");
        });
        var ns = new_dict();
        ns.setitem("__init__"_s, nat("__init__", [](const CallArgs& a) {
            a.pos.at(0).setattr("algorithm", a.pos.at(1));
            a.pos[0].setattr("mode", a.pos.size() > 2 ? a.pos[2] : var(None));
            return None;
        }));
        ns.setitem("encryptor"_s, nat("encryptor", [](const CallArgs& a) { return new_ctx(a.pos.at(0), true); }));
        ns.setitem("decryptor"_s, nat("decryptor", [](const CallArgs& a) { return new_ctx(a.pos.at(0), false); }));
        mod_set(m, "Cipher", make_class("Cipher", "Cipher", {}, ns, {}, "cryptography.hazmat.primitives.ciphers"));
        for (const char* n : {"CipherContext", "AEADEncryptionContext", "AEADDecryptionContext", "AEADCipherContext"}) mod_set(m, n, tv(T_ctx));
        mod_set(m, "algorithms", import("cryptography.hazmat.primitives.ciphers.algorithms"));
        mod_set(m, "modes", import("cryptography.hazmat.primitives.ciphers.modes"));
    });
    // RSA: numbers and keys (the callers do the arithmetic)
    register_native_module("cryptography.hazmat.primitives.asymmetric.rsa", [](const var& m) {
        const char* M = "cryptography.hazmat.primitives.asymmetric.rsa";
        var pubkey = klass("RSAPublicKey", {}, M), prvkey = klass("RSAPrivateKey", {}, M);
        mod_set(m, "RSAPublicKey", pubkey);
        mod_set(m, "RSAPrivateKey", prvkey);
        var pkns = new_dict();
        pkns.setitem("public_numbers"_s, nat("public_numbers", [](const CallArgs& a) { return a.pos.at(0).getattr("_numbers"); }));
        pkns.setitem("key_size"_s, property(nat("key_size", [](const CallArgs& a) { return a.pos.at(0).getattr("_numbers").getattr("n").call("bit_length"); })));
        var pubkey_impl = make_class("_RSAPublicKey", "_RSAPublicKey", {pubkey}, pkns, {}, M);
        var pn = new_dict();
        pn.setitem("__init__"_s, nat("__init__", [](const CallArgs& a) { a.pos.at(0).setattr("e", a.pos.at(1)); a.pos[0].setattr("n", a.pos.at(2)); return None; }));
        pn.setitem("public_key"_s, nat("public_key", [pubkey_impl](const CallArgs& a) {
            var k = call0(pubkey_impl);
            k.setattr("_numbers", a.pos.at(0));
            return k;
        }));
        pn.setitem("__eq__"_s, nat("__eq__", [](const CallArgs& a) {
            var x = a.pos.at(0), y = a.pos.at(1);
            return var(hasattr(y, "n"_s).truthy() && x.getattr("n") == y.getattr("n") && x.getattr("e") == y.getattr("e"));
        }));
        mod_set(m, "RSAPublicNumbers", make_class("RSAPublicNumbers", "RSAPublicNumbers", {}, pn, {}, M));
        var vkns = new_dict();
        vkns.setitem("private_numbers"_s, nat("private_numbers", [](const CallArgs& a) { return a.pos.at(0).getattr("_numbers"); }));
        vkns.setitem("public_key"_s, nat("public_key", [](const CallArgs& a) { return a.pos.at(0).getattr("_numbers").getattr("public_numbers").call("public_key"); }));
        vkns.setitem("key_size"_s, property(nat("key_size", [](const CallArgs& a) { return a.pos.at(0).getattr("_numbers").getattr("public_numbers").getattr("n").call("bit_length"); })));
        var prvkey_impl = make_class("_RSAPrivateKey", "_RSAPrivateKey", {prvkey}, vkns, {}, M);
        var vn = new_dict();
        vn.setitem("__init__"_s, nat("__init__", [](const CallArgs& a) {
            var s = a.pos.at(0);
            const char* names[] = {"p", "q", "d", "dmp1", "dmq1", "iqmp", "public_numbers"};
            for (int i = 0; i < 7; i++) s.setattr(names[i], arg(a, (size_t)i + 1, names[i]));
            return None;
        }));
        vn.setitem("private_key"_s, nat("private_key", [prvkey_impl](const CallArgs& a) {
            var k = call0(prvkey_impl);
            k.setattr("_numbers", a.pos.at(0));
            return k;
        }));
        mod_set(m, "RSAPrivateNumbers", make_class("RSAPrivateNumbers", "RSAPrivateNumbers", {}, vn, {}, M));
        mod_def(m, "rsa_crt_dmp1", [](const CallArgs& a) { return a.pos.at(0) % (a.pos.at(1) - var(1)); });
        mod_def(m, "rsa_crt_dmq1", [](const CallArgs& a) { return a.pos.at(0) % (a.pos.at(1) - var(1)); });
        mod_def(m, "rsa_crt_iqmp", [](const CallArgs& a) { return pow(a.pos.at(1), var(-1), a.pos.at(0)); });
    });
    // pycryptodome's DES: DES.new(key, DES.MODE_ECB).encrypt / .decrypt
    register_native_module("Crypto.Cipher", [](const var& m) { mod_set(m, "DES", import("Crypto.Cipher.DES")); });
    register_native_module("Crypto.Cipher.DES", [](const var& m) {
        mod_set(m, "MODE_ECB", var(1));
        mod_set(m, "MODE_CBC", var(2));
        mod_set(m, "block_size", var(8));
        mod_set(m, "key_size", var(8));
        var ns = new_dict();
        auto run = [](const CallArgs& a, bool enc) {
            var s = a.pos.at(0);
            CipherCtx c;
            c.alg = "DES";
            c.key = raw_of(s.getattr("_key"));
            c.cbc = s.getattr("_mode").as_int64() == 2;
            c.encrypt = enc;
            if (c.cbc) c.iv = raw_of(s.getattr("_iv"));
            std::string in = raw_of(a.pos.at(1));
            if (in.size() % 8) raise(E_ValueError, "Data must be aligned to block boundary in ECB mode");
            std::string out = block_run(&c, in);
            if (c.cbc) s.setattr("_iv", new_bytes(c.iv));
            return new_bytes(out);
        };
        ns.setitem("encrypt"_s, nat("encrypt", [run](const CallArgs& a) { return run(a, true); }));
        ns.setitem("decrypt"_s, nat("decrypt", [run](const CallArgs& a) { return run(a, false); }));
        var cls = make_class("DesCipher", "DesCipher", {}, ns, {}, "Crypto.Cipher.DES");
        mod_def(m, "new", [cls](const CallArgs& a) {
            var c = call0(cls);
            c.setattr("_key", new_bytes(raw_of(a.pos.at(0))));
            c.setattr("_mode", a.pos.size() > 1 ? a.pos[1] : var(1));
            c.setattr("_iv", kwarg(a, "iv", a.pos.size() > 2 ? a.pos[2] : new_bytes(std::string(8, '\0'))));
            return c;
        });
    });
}

}  // namespace py
